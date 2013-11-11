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
//	Test harness for CBioServerSideTestUtilities. 
//
//  BIO messages are created by synchronous calls....

#pragma warning( disable : 4100 )

//  Includes...
#include <e32base.h>
#include <e32std.h>
#include <e32test.h>
#include <e32hal.h>
#include <e32uid.h>
#include <msvapi.h>
#include <msventry.h>
#include "MSVSERV.H"
#include <msvuids.h>
#include <msvruids.h>
#include <msvids.h>
#include <f32fsys.h>
#include <s32file.h>
#include <barsc.h>
#include <msvreg.h>

#include <txtrich.h>				//	rich text stuff
#include <txtfmlyr.h>				//	para format gubbins..

#include <biouids.h>	// Defines for use in Bio messaging
#include <bitsids.h>		// Uids for use in Test Harnesses

#include "biotestutils.h"


// Uncomment following define to print the details to screen
// Difficult to view as there's so much information

//#define _TBIOTEST_PRINTING_  


#define _TBIOTEST_LOGGING_  // Commment this line to disable logging

//  Constants ids.

//  Classes defined/used
class CTestScheduler;
class CObserverTester;

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
    EStop
    } TTesterState;


//**********************************************************************************
//
//  CTestScheduler - Concrete active scheduler class for our test harness
//
//**********************************************************************************
class CTestScheduler : public CActiveScheduler
    {
    public:
        //  inline
        void Error(TInt anError) const;
    };


void CTestScheduler::Error(TInt anError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\nLeave signalled, reason=%d\n"),anError);
    }

//******************************************************************************
//
//		CMessGeneratorTester - class declarations
//
//******************************************************************************

class CServerSideMsgGeneratorTester : public CActive
    {
    //  public API
    public:
        static CServerSideMsgGeneratorTester * NewL();
        static CServerSideMsgGeneratorTester * NewLC();
        ~CServerSideMsgGeneratorTester ();

		//	Starts everything off
		void StartL();
		//  Sets the active scheduler going
        void QueueOperationAsync(TInt aErr);
    

    protected:
        CServerSideMsgGeneratorTester ();
        void ConstructL();
        
		void InitialiseTesterL();

    private:    // Active Object functions
        void DoCancel();
        void RunL();

    private:
        //  Resources
		CBioTestUtils*				iBIOGenerator;
        //  Entry, Entry Selection, message details..
        CMsvEntrySelection*			iSelection;

		TBIOMessageType				iCurrentMessageType;
		
		//  States   
		TTesterState				iState;
        //  Service Ids
        TMsvId                      iBIOServiceId;
    };



//
//																				//
//  Implementation of the CMessGeneratorTester test harness...					//
//																				//
//

CServerSideMsgGeneratorTester* CServerSideMsgGeneratorTester::NewL()
    {
    CServerSideMsgGeneratorTester* self = CServerSideMsgGeneratorTester::NewLC();
    CleanupStack::Pop();
    return self;
    }

CServerSideMsgGeneratorTester* CServerSideMsgGeneratorTester::NewLC()
    {
    CServerSideMsgGeneratorTester* self = new (ELeave) CServerSideMsgGeneratorTester();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CServerSideMsgGeneratorTester::CServerSideMsgGeneratorTester()
: CActive(EPriorityNormal)
    {
    }

CServerSideMsgGeneratorTester::~CServerSideMsgGeneratorTester()
    {
	// NB don't delete the Logger or Printer objects because this object doesn't own them
	Cancel();
	//
    delete iSelection;
	//
	delete iBIOGenerator;
    }

void CServerSideMsgGeneratorTester::ConstructL()
    {
	// Create the SMS Test Message Utility object
	iBIOGenerator = CBioTestUtils::NewL(test);
	iBIOGenerator->GoServerSideL();

//	Add ourselves to the active scheduler
//   User::LeaveIfError(iTimer.CreateLocal());
    CActiveScheduler::Add(this);
	}

//
//																					//
//						StartL - sets the tester going								//
//																					//
//

void CServerSideMsgGeneratorTester::StartL()
	{
    // Set the Scheduler going so we go to our RunL().
	iState=EStart;
    QueueOperationAsync(KErrNone);
	}


//
//																				//
//						Tester's Active Object Stuff							//
//																				//
//

void CServerSideMsgGeneratorTester::RunL()
    {
    TInt err = iStatus.Int();

    if(err != KErrNone)
        {
        CActiveScheduler::Stop();
        }
    switch(iState)
        {
		case EStart:   
			iState = EStop;
			InitialiseTesterL();
            break;
		case EStop:
			CActiveScheduler::Stop();
            break;
        }
    }

void CServerSideMsgGeneratorTester::DoCancel()
    {
    }

void CServerSideMsgGeneratorTester::QueueOperationAsync(TInt aErr)
    {
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }

void CServerSideMsgGeneratorTester::InitialiseTesterL()
	{
	// Get the NB if we don't need this two step process, use SetSmsServiceL().
	//iBIOGenerator->SetSmsServiceIdL();
	iBIOGenerator->SetBIOServiceIdL();
	// Empty the Inbox
	iBIOGenerator->EmptyInboxMessagesL();
	iSelection = iBIOGenerator->GenerateMessagesL();
	QueueOperationAsync(KErrNone);
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
    CServerSideMsgGeneratorTester* theTester = 
						CServerSideMsgGeneratorTester::NewLC(); //  release context on create 
	theTester->StartL();
	CActiveScheduler::Start();
    CleanupStack::PopAndDestroy(); //theTester
    CleanupStack::PopAndDestroy(); //theScheduler
	rFs.Close();
    }


