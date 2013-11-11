/*
* Copyright (c) 1999-2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#ifndef _TESTWAPP_H_
#define _TESTWAPP_H_

#include <e32std.h>
#include <e32test.h>
#include <c32comm.h>
#include <in_sock.h>

extern RTest Test;
extern RSocketServ SocketServer;
extern RHeap* TestHeap;

#define BIO_MSG_ENTRY_PARSED     1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised

_LIT(KBIOWAPPrefix, "WAP*");	// prefix for WAP message
const  TPtrC KWappTestTxtFileDir = _L("c:\\test\\bio\\wapp\\");  //dir for source files
#define MSV_INDEX_FILE      _L("c:\\system\\Mail\\index")  // Index for mail messages.
#define KSmsFileName        _L("c:\\Mailtest\\Wapptest\\Wapp.txt")
const  TPtrC KWappLogFile = _L("C:\\Logs\\Bio\\Wap.log"); // file for test logging
const  TPtrC KFileName   = _L("Wap.dat");
const  TPtrC KAttachmentName	= _L("Wappbmp.cng"); 



#define KTestWappPriority   (EPriorityStandard)    //Change this....

// Support for printing message details to the test console:
#define _TEST_PRINTING_	// Uncomment this to enable printing.
// Support for logging details of messages and tests:
#define _TEST_LOGGING_	// Uncomment this to enable logging.

// Panic errors:
const TInt KWappNullPtr		      = (111);
const TInt KBioMsgNotParsed		  = (222);
const TInt KBioMsgEntryHasNoStore = (333);
const TInt KBioMsgNotProcessed    = (444);

//const TUid KUidMsvMessageDriveStream = {0x1000163E};
const TUid  KUIDMsvBIODataStream     = {0x10005268};

const TPtrC KWappToken = _L("WAP Access Point");		// A script header.
const TPtrC KWappParserName = _L("WAP Access Point Parser");	// A human readable name.
const TVersion KVer1(0,0,1);							// A version number.


// Forward references:
class CExampleScheduler;
LOCAL_C void DisplayErrorReason(TInt& aReason);

//////////////////////////
// Global declarations: //
//////////////////////////
LOCAL_D CExampleScheduler* exampleScheduler;
LOCAL_D RTest test(_L("WAPP Test Harness"));
LOCAL_D RFs        gFs;

//  Logging support	
LOCAL_D CMsvBIOTestPrinter*  myPrinter;

//  Logging support	
LOCAL_D CMsvBIOTestLogger* myLogger;

void CommInitL(TBool aEnhanced);


////////////////////////
// Class definitions: //
////////////////////////

///////////////////////////////
// CExampleScheduler: INTERFACE //
///////////////////////////////
class CExampleScheduler : public CActiveScheduler
	{
public:
	void		Error(TInt anError) const;
	};


///////////////////////////
// CTestTimer: INTERFACE //
///////////////////////////
class CTestTimer : public CTimer
    {
public:
    static CTestTimer* NewL();
private:
    CTestTimer();
    void RunL();
    };

//////////////////////////////////////
// CDummySessionObserver: INTERFACE //
//////////////////////////////////////
// Does nothing at all but is required for the CMsvSession constructor.
class CDummySessionObserver : public MMsvSessionObserver
    {
        void HandleSessionEventL(TMsvSessionEvent aEvent,
            TAny* arg1, TAny* arg2, TAny* arg3);
    };


//////////////////////////
// CTestWapp: INTERFACE //
//////////////////////////

class CTestWapp : public CActive
    {
    public:
        ~CTestWapp();
        CTestWapp();
        CTestWapp(CMsvServerEntry* aEntry, CMsvBIOTestLogger* aLogger,
												CMsvBIOTestPrinter* aPrinter);
        static CTestWapp* NewL(CMsvServerEntry* aEntry, CMsvBIOTestLogger* aLogger=NULL,
												CMsvBIOTestPrinter* aPrinter=NULL);
        void ConstructL();
        void StartL(TInt aCmd);
        void RunL();
        void DoCancel();
		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);

        enum TSessionState
            {
			EReadingFiles,
			ECreateNewMessage,
			ECreateParser,
            EParse,
            EProcess,
			EDisplay,			// Display entry data.
			EFinish
            };

	private:
		CBaseScriptParser* CreateParserL();
		void QueueOperationAsync(TInt aErr);
		void CreateFilenameArrayL();
		void GenerateNextMessageL();
		void UnloadParserL();
		void InitialiseParserL();
		void ExtractBodyPartsL();	
		void MakeMessage();
		void ParseMessageTextL();
		void ProcessMessageL();
        void DisplayEntryDataL();
		void DisplayStoreL();
		void InternalizeL(RMsvReadStream& aReadStream);
		void ReLoadParsedDataL(CMsvStore& aStore);
		void DisplayParsedFieldsL(CMsvStore& aStore);
		void DisplayBookmarksL();
		void DisplayLogDescriptorL(const TDesC& aBuffer);
		void AppendDescriptorL(TDes& aWriteString, const TDesC& aReadString);


		TInt GetWAPEntryTotalL(); //returns the current number of entries in the WAP table

    private:
        TInt						iState;		// Holds the current state of the test harness.
        CMsvServerEntry*			iEntry;		// N.B. don't own this!
		TMsvId						iEntryId;	// ID of sms entry.
      
		CDir*						iDir;	// Holds the names of the files to process.	
		TInt						iNumFiles;
		TInt						iMessagesCreated;
		TInt						iFilesProcessed;
		CRegisteredParserDll*		iRegisteredParserDll;
        CBaseScriptParser*			iParser;
		CMsvBIOTestLogger*			iLogger;  // N.B. don't own this
		CMsvBIOTestPrinter*			iPrinter; // or this!

        HBufC*						iMessageBody;
		CBIOServerSideTestUtils*	iMessageMaker;

		CBIODatabase*				iBioDatabase;
		TInt						iNumWapEntries; //Number of entries in the WAP table
		HBufC*						iCurrentSettingsName;  // Name of the current settings obtained from characteristic in message
		TUint32						iCurrentIspID;
		TUint32						iCurrentIapID;
		TInt32						iWapModemID;
		CArrayPtrSeg<CParsedField>*	iParsedFieldArray;
    };


#endif

