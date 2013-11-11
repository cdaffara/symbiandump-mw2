// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//	Test harness for CBioClientSideTestUtilities. 
//
//  BIO messages are created by synchronous calls....

#pragma warning( disable : 4100 )

//  Includes...
#include <e32test.h>
#include <e32hal.h>
#include <e32uid.h>
#include <msvapi.h>
#include <msvuids.h>
#include <msvids.h>
#include <f32fsys.h>
#include <s32file.h>
#include <barsc.h>
#include <msvreg.h>
#include <txtrich.h>				//	rich text stuff
#include <txtfmlyr.h>				//	para format gubbins..

#include <biouids.h>   // Important defines for use in the BIO messaging project
#include <bitsids.h>	//Message uids defines for the Test harnesses
#include "biotestutils.h"

// Uncomment following define to print the details to screen
// Difficult to view as there's so much information
//  Constants ids.


//  Classes defined/used
class CTestScheduler;
class CObserverTester;
class CDummySessionObserver;

//  Global functions..
//  functions()
GLDEF_C void doMainL();
GLDEF_C TInt E32Main();

//  Resources..
GLDEF_C RTest test(_L("BIO Message Generator Test Rig"));
LOCAL_D CTrapCleanup*       theCleanup;
LOCAL_D CTestScheduler*		theScheduler;
LOCAL_D RFs                 rFs;

// States for the tester
typedef enum
    {
    EStart,
	EReadingFiles,
	ECreatingMessages,
	EGeneratingResults,
    EStop
    } TTesterState;

//
//  CTestScheduler - test active scheduler..
//
class CTestScheduler : public CActiveScheduler
    { 
    public:
        //  inline
        void Error(TInt aError) const;
    };

void CTestScheduler::Error(TInt anError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\nLeave signalled, reason=%d\n"),anError);
    }

//

//
//																				//
//			CClientSideMessGeneratorTester - class definition					//
//																				//
//

class CClientSideMessGeneratorTester : public CActive
    {
    //  public API
    public:
        static CClientSideMessGeneratorTester * NewL(TTimeIntervalMicroSeconds32 aTime);
        static CClientSideMessGeneratorTester * NewLC(TTimeIntervalMicroSeconds32 aTime);
        ~CClientSideMessGeneratorTester ();

		//	Starts everything off
		void StartL();
		//  Sets the active scheduler going
        void QueueOperationAsync(TInt aErr);
        
		//  Asks Message server to enumerate the results..
        TInt GenerateResults(); 
        void DoGenerateResultsL();   

    protected:
        CClientSideMessGeneratorTester (TTimeIntervalMicroSeconds32 aTime);
        void ConstructL();
        void CreateFilenameArrayL();
		void GenerateNextMessageL();
		TInt DoGenerateMessages();
		void InitialiseTesterL();
		void SetMessageType(const TDesC& aFileName);

#ifdef _TBIOTEST_LOGGING_
		//  Additional Logging functionality
		void LogNewMessageDetails(TMsvId amessageId);
		void ChangeELineBreaksToLF(const TDesC& aDesIn,  TBuf<1024>& aBuffer);
		TBool IsEOL( TChar ch); // TUint8 ch );	    
#endif  //_TBIOTEST_LOGGING_

    private:    // Active Object functions
        void DoCancel();
        void RunL();

    private:
        //  Resources
		CBioTestUtils*				iBIOGenerator;
        RTimer						iTimer;

		CDir*						iDir;		
		TInt						iNumFiles;
		TInt						iFilesProcessed;
		TInt						iMessagesCreated;
		TBIOMessageType				iCurrentMessageType;
		
		//  States   
		TTesterState				iState;
        //  Service Ids
        TMsvId                      iBIOServiceId;
        TMsvId                      iNewEntryId;
        //  General.
        TTimeIntervalMicroSeconds32 iNotificationPeriod;
        TBool                       iReleaseContextOnCreate;

        TMsvLocalOperationProgress              iProgress;
        TPckgBuf<TMsvLocalOperationProgress>    iLocProgressBuf;
    };

//
//																				//
//			Implementation The tester class..									//
//																				//
//

CClientSideMessGeneratorTester * CClientSideMessGeneratorTester ::NewL(TTimeIntervalMicroSeconds32 aTime)
    {
    CClientSideMessGeneratorTester * self = 
						CClientSideMessGeneratorTester ::NewLC(aTime);
    CleanupStack::Pop();
    return self;
    }

CClientSideMessGeneratorTester * CClientSideMessGeneratorTester ::NewLC(TTimeIntervalMicroSeconds32 aTime)
    {
    CClientSideMessGeneratorTester * self = 
					new (ELeave) CClientSideMessGeneratorTester (aTime);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CClientSideMessGeneratorTester ::CClientSideMessGeneratorTester (TTimeIntervalMicroSeconds32 aTime)
: CActive(EPriorityNormal),
iNotificationPeriod(aTime)
    {
    }

CClientSideMessGeneratorTester ::~CClientSideMessGeneratorTester ()
    {
	// NB don't delete the Logger or Printer objects because this object doesn't own them
	Cancel();
	
    delete iDir;
	//
	delete iBIOGenerator;
    }

void CClientSideMessGeneratorTester ::ConstructL()
	{
	// Create the SMS Test Message Utility object
	iBIOGenerator = CBioTestUtils::NewL(test);
	iBIOGenerator->GoClientSideL();

	iNumFiles = 0;
	iFilesProcessed = 0;
	iMessagesCreated = 0;
	
	//	Add ourselves to the active scheduler
    User::LeaveIfError(iTimer.CreateLocal());
    CActiveScheduler::Add(this);
    }

//
//																				//
//						Start the tester off									//
//																				//
//
void CClientSideMessGeneratorTester ::StartL()
	{
    // Set the Scheduler going so we go to our RunL().
	iState=EStart;
    QueueOperationAsync(KErrNone);
	}

//
//																				//
//							Active Object Stuff									//
//																				//
//

void CClientSideMessGeneratorTester ::RunL()
    {
    TInt err = iStatus.Int();

    if(err != KErrNone)
        {
		iBIOGenerator->TestHarnessFailed(err);
        CActiveScheduler::Stop();
        }
    switch(iState)
        {
		case EStart:   
			iState = EReadingFiles;
			iBIOGenerator->TestStart(1,_L("Initialise tester"));
			InitialiseTesterL();
			iBIOGenerator->TestFinish(1);
            break;
		case EReadingFiles:
			iState = ECreatingMessages;
			test.Printf(_L("Reading Text Filenames \n"));
			iBIOGenerator->TestStart(2,_L("Create filename array"));
			CreateFilenameArrayL();
			iBIOGenerator->TestFinish(2);
			QueueOperationAsync(KErrNone);	
			break;
		case ECreatingMessages:   // don't set the next state here
			if (iFilesProcessed ==0)
				iBIOGenerator->TestStart(3,_L("Generating messages"));
			iBIOGenerator->SetSessionPath(KBIOTxtFilePath);
		    GenerateNextMessageL();  // state gets set here when we processed all the messages
			QueueOperationAsync(KErrNone);
			break;
        // 1st time in start the notifications.
		case EGeneratingResults:
			iState = EStop;
			iBIOGenerator->TestFinish(3);
			iBIOGenerator->TestStart(4,_L("Do some results"));
            QueueOperationAsync(KErrNone);
			break;
		case EStop:
			iBIOGenerator->FindChildrenL(KMsvGlobalInBoxIndexEntryId);
			iBIOGenerator->TestHarnessCompleted();
			CActiveScheduler::Stop();
            break;
        }
    }

void CClientSideMessGeneratorTester ::DoCancel()
    {
    }

void CClientSideMessGeneratorTester ::QueueOperationAsync(TInt aErr)
    {
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }


void CClientSideMessGeneratorTester ::InitialiseTesterL()
	{
	// Get the NB if we don't need this two step process, use SetSmsServiceL().
	//iBIOGenerator->SetSmsServiceIdL();
	iBIOGenerator->SetBIOServiceIdL();
	// Empty the Inbox
	iBIOGenerator->EmptyInboxMessagesL();
	QueueOperationAsync(KErrNone);
	}


void CClientSideMessGeneratorTester ::CreateFilenameArrayL()
    {
	// Creates an array of descriptors for the body parts of our messages
	//Check that the source directory exists, if not warn the user and create one
	TInt err;
	if (iDir)
		delete iDir;
	err = rFs.GetDir(KBIOTxtFilePath, KEntryAttMatchMask, ESortByName, iDir);

	if (err == KErrPathNotFound)
		{
		test.Printf(_L("Error - source file directory not found\n\n"));
		TInt makeDirErr = rFs.MkDirAll(KBIOTxtFilePath);
		if (makeDirErr==KErrNone)
			{
			test.Printf(_L("Creating directory for files\nPlease add files to %S\n"), KBIOTxtFilePath);
			User::After(1000000);
 			User::Leave(KErrNotFound);
			}
		else 
			{
			test.Printf(_L("Error %d occurred when making directory %S"),makeDirErr, &KBIOTxtFilePath);	
			User::Leave(makeDirErr);
			}
		}
	else if (err!=KErrNone)
		{
		test.Printf(_L("Error %d occurred"),err );
		User::After(100000);
		User::Leave(err);
		}

	// Set the session path for the RF
	
	rFs.SetSessionPath(KBIOTxtFilePath);
	test.Printf(_L("Set Session path to %S \n"), &KBIOTxtFilePath);

	//  Get a directory listing for the textfiles...
	if (iDir) 
		delete iDir;
	User::LeaveIfError(rFs.GetDir(_L("*.txt"), KEntryAttNormal, ESortByName, iDir));
    TInt count = iDir->Count();
    if(count == 0)
        {
		test.Printf(_L("No files in the source directory %S !!"), &KBIOTxtFilePath);
        User::Leave(KErrNotFound);   // No files to process
        }
	iNumFiles = count;
	iCurrentMessageType = ENoMessage;  // Set to error condition
	// Display some info for the user
	test.Printf(_L("Have %d text files in directory \n"), count);		
	test.Printf(_L("\n")); //move down one line 
    }

//
//																					//
//						Creating one message stuff.									//
//																					//
//

void CClientSideMessGeneratorTester ::GenerateNextMessageL()
	{
	TBufC<KMaxFileName> currentFile;
	TEntry tempEntry;
	HBufC* tempDesc;

	// Check if there are any more files of the current type
	if (iFilesProcessed < iNumFiles)
		{
		tempEntry = (*iDir)[iFilesProcessed];
		currentFile=( tempEntry.iName );
		// Not processed all the messages - so keep the current state
		SetMessageType(currentFile);
		iFilesProcessed++;   // Here because need to update the counter promptly
		if (iCurrentMessageType!=ENoMessage) // skip any dodgy filenames
			{
			tempDesc = currentFile.AllocL();
			CleanupStack::PushL(tempDesc);
			test.Console()->SetPos(0, 8);
			test.Printf(_L("Reading file %S"), tempDesc);
			iBIOGenerator->CreateBIOEntryFromFileL(currentFile, iCurrentMessageType);
			iMessagesCreated++;
			test.Console()->SetPos(0, 10);
			test.Printf(_L("Created entry No. %d\n"), (iMessagesCreated));
			test.Printf(_L("So far have discarded %d files out of a total of %d\n"), (iFilesProcessed-iMessagesCreated), iNumFiles);
			CleanupStack::PopAndDestroy(); // tempDesc;
			}
		}
	else if (iFilesProcessed == iNumFiles) // Completed everything
		{
		iState =EGeneratingResults;  // Done creating messages
		}
	else // shouldn't be anything else as iMessagesProcessed
		{
		User::Leave(KErrUnknown);  // Have an error but don't know how
		}
	}

void CClientSideMessGeneratorTester ::SetMessageType(const TDesC& aFileName)
	{

	// Check each file prefix in turn with the filename passed as a parameter. 
	// If cannot find the correct type set to ENoMessage to indicate an error. 
	// GenerateNextMessage will then skip the file!

	if (aFileName.MatchF(KBIOIapPrefix)==0)  // File starts with "iap"
		{
		iCurrentMessageType = EBioIapSettingsMessage; 
		return;
		}
	if(aFileName.MatchF(KBIOEnpPrefix)==0) // File name starts "enp"
		{
		iCurrentMessageType = EBioEnpMessage;
		return;
		}
	if (aFileName.MatchF(KBIORingTonePrefix)==0)//Filename begins "rtone"
		{
		iCurrentMessageType = EBioRingTonesMessage;
		return;
		}
	if (aFileName.MatchF(KBIOOpLogoPrefix)==0)	// Filename begins "oplogo"
		{
		iCurrentMessageType = EBioOpLogoMessage;
		return;
		}
	if (aFileName.MatchF(KBIOcBusinessCardPrefix)==0)// Filename begins "cbc"
		{
		iCurrentMessageType = EBioCompBusCardMessage;
		return;
		}
	if (aFileName.MatchF(KBIOvCardPrefix)==0)
		{
		iCurrentMessageType = EBiovCardMessage;
		return;
		}
	if (aFileName.MatchF(KBIOvCalenderPrefix)==0)
		{
		iCurrentMessageType = EBiovCalenderMessage; 
		return;
		}
	// if we've reached this point it's an unknown filename 
	iCurrentMessageType = ENoMessage;
	}

TInt CClientSideMessGeneratorTester ::GenerateResults()
    {
    TInt err = KErrNone;
    TRAP(err, DoGenerateResultsL());
    return err;
    }


void CClientSideMessGeneratorTester ::DoGenerateResultsL()
    {
	}

//*****************************************************************************
//
//  Implementation; global stuff
//
//*****************************************************************************

GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	theCleanup = CTrapCleanup::New();
	TRAPD(err,doMainL());		
	test(err==KErrNone);
	delete theCleanup;	
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
    }

GLDEF_C void doMainL()
    {
	// Create an active scheduler for the program session
    theScheduler = new (ELeave) CTestScheduler();
    CleanupStack::PushL(theScheduler);
    CActiveScheduler::Install(theScheduler);
    test.Title();
    test.Start(_L("Starting test harness"));
    rFs.Connect();

    //  Can't actually lock a message client side
    CClientSideMessGeneratorTester * theTester = 
						CClientSideMessGeneratorTester ::NewLC(100000); //  release context on create 
	theTester->StartL();
	CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(); //theTester

    CleanupStack::PopAndDestroy(); //theScheduler
	rFs.Close();
    }

