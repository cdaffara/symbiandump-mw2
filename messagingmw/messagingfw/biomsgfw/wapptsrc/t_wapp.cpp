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
// Test harness for WAP Access Point Settings Parser.
// The test harness simply creates new messages from binary data files
// and calls the WAP Access Point Settings Parser to parse them.
// 
//

#include <e32uid.h>
#include <e32test.h>
#include <e32cons.h>
#include <s32file.h>
#include <e32base.h>

// Comms stuff
#include <msvids.h>
#include <msvuids.h>
#include <msvruids.h>
#include <msvstd.h>
#include <msventry.h>
#include <mtsr.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <imcvtext.h>

// General parser stuff:
#include <bsp.h>
#include <regpsdll.h>	// Parser registration.

#include <biouids.h>	// Defines for use in Bio messaging
#include <bitsids.h>		// Uids for use in Test Harnesses

#include "biotestutils.h"
#include <biodb.h>		//  CBIODatabase.
#include "tmsvbioinfo.h"

// WAP stuff:

#include <etelpckt.h>
#include <etelmm.h>
#include <wapp.h>
#include <wappdef.h>
#include "wapperr.h"
#include "T_Wapp.h"

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

_LIT(KEBookmarkExtension,	".eBM");
_LIT(KEBookmarkItemBegin,	"BEGIN:eBOOKMARK");
_LIT(KEBookmarkItemEnd,		"END:eBOOKMARK");
const TInt KNumberOfBookmarkFields = 3;

//#define __HEAPFAILURE__    // set the compiler flag to include test code for heap failure

//
// CExampleScheduler: Implementation //
//
void CTestScheduler::Error(TInt aError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\nLeave signalled, reason=%d\n"),aError);
    }
// end CExampleScheduler::Error


//
//		CTestWapp: IMPLEMENTATION		//
//

//
//		Factory methods					//
//
CTestWapp::CTestWapp(RFs& aFs)
:CActive(KTestWappPriority),
iFs(aFs),
iWAPModemID(KWappModemInitValue)  // Initialise to KErrNotFound
    {
    }
// end CTestWapp::CTestWapp


CTestWapp::~CTestWapp()
    {
    Cancel();

	delete iCurrentSettingsName;
//	delete iBioDatabase;
	delete iMessageBody;
	delete iMsvSelection;
    if (iWappParser)
		{
		delete iWappParser;
		}

	delete iBioTestUtils;
   }
// end CTestWapp::~CTestWapp


CTestWapp* CTestWapp::NewL(RFs& aFs)
    {
    CTestWapp* self = new (ELeave) CTestWapp(aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();  // self
	return self;
	}
// end CTestWapp::NewL


void CTestWapp::ConstructL()
    {
	iBioTestUtils = CBioTestUtils::NewL(test, ETuGoClientSide);

	
	//Initialise our member variables
	iParsedFieldsNum = 0;   //No parsed fields so far 
	iTestSuccesses=0;
	iTestFailures = 0;
	iMessagesCreated=0;
	iTestCounter = 0;		// Current Test Number start at 0
	// Chose from one of following:- 
	//iTestMode = ETWappConcise;       
	//iTestMode =  ETWappDetailed; 
	iTestMode = ETWappVerbose;
    CActiveScheduler::Add(this);
	iBioTestUtils->WriteComment(_L("Created WAPP Test Harness"));
	test.Console()->ClearScreen(); // get rid of menu
    }
// end CTestWapp::ConstructL


//
// Active object stuff //
//
void CTestWapp::RunL()
    {
	DoRunL();
	}
	
TInt CTestWapp::RunError(TInt aError)
	{
	iBioTestUtils->TestHarnessFailed(aError);
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestWapp::DoRunL()
	{	
	TInt result=iStatus.Int();


    if (result != KErrNone )
        {
        test.Console()->ClearScreen();
        test.Console()->SetPos(0,0);
        DisplayErrorReasonL(result);		// Stops active scheduler if the error is unknown.
		iState = EWappDisplay;
        }
		
	TPtrC tempPtr;
	HBufC* tempBuffer;

    switch (iState)
        {
        case EWappClearCommDB:
			// Create the Comms DB
			DeleteCommDbTableL();  // Trash all the CommDb entries
			iState = EWappCreateNewMessages;
			QueueOperationAsync(KErrNone);
			break;
		case EWappCreateNewMessages:
			iState = EWappCreateParser;
			tempPtr.Set(KWappTestTxtFileDir);
			tempBuffer= tempPtr.AllocL();
			CleanupStack::PushL(tempBuffer);
			OutputFormattedTextL(_L("Creating messages from files in directory %S"),tempBuffer);
			TRAPD(error, iMsvSelection = iBioTestUtils->GenerateMessagesL(KWappTestTxtFileDir, ETrue) );
			if (error !=KErrNone)
				{
				OutputFormattedTextL(_L("Error %d occurred when creating messages"),error);
				User::Leave(error);
				}
			iMessagesCreated= iMsvSelection->Count();
			if (iTestMode != ETWappConcise)
				OutputFormattedTextL(_L("Created %d WAPP messages "),iMessagesCreated );
			if (iMessagesCreated ==0)     // No WAPP messages 
				User::Leave(KErrNotFound);// Ensure we don't try to parse with no messages
			QueueOperationAsync(KErrNone);
			CleanupStack::Pop(); //tempBuffer;
			delete tempBuffer;
			tempBuffer = NULL;
			break;
		case EWappCreateParser:
			// Create parser and add ourselves to Active Scheduler
			iState = EWappProcessUnparsed;
			iBioTestUtils->TestStart(iTestCounter+1); // Origin at Zero
			CreateParserL();  // Sets iEntry to new ID, & creates parser 
			if (iTestMode ==ETWappVerbose)
				OutputFormattedTextL(_L("Running tests on message %d with Entry Id %d"), iTestCounter+1, iEntryId);
			QueueOperationAsync(KErrNone);
			break;		
		case EWappProcessUnparsed:
			//Try processing the unparsed message - should fail - catch the error
			iState =EWappParse;
			if (iTestMode !=ETWappConcise)
				OutputFormattedTextL(_L("Process unparsed message"));
			ProcessUnparsedMessageL();  // Note that this should fail
            SetActive();
			break;
		case EWappParse:
			//Extract the body of the unparsed message and pass it to WAPP for parsing
			iState =EWappParseAgain;
			ExtractBodyPartsL();// extracts body text from current message.
			if (iTestMode != ETWappConcise)
				OutputFormattedTextL(_L("Parse"));
			ParseMessageL();
            SetActive();            
            break;		
		case EWappParseAgain:
			// Parse the parsed message again. Shouldn't generate an error. 
			// Should skip the full parsing op
			iState = EWappProcess;
			if (iTestMode != ETWappConcise)
				OutputFormattedTextL(_L("Re-parse"));
			ParseMessageL();
			SetActive();
			break;
		case EWappProcess:
			//Process the parsed fields in the stream store attachment to the message
			iState = EWappProcessAgain;
			if (iTestMode !=ETWappConcise)
				OutputFormattedTextL(_L("Process"));
			ProcessMessageL();
            SetActive();
			break;
		case EWappProcessAgain:
			//Process the parsed fields again - shouldn't create any errors
			iState = EWappDisplay;
			if (iTestMode != ETWappConcise)
				OutputFormattedTextL(_L("Re-Process"));
			ProcessMessageL();
			SetActive();
			break;
		case EWappDisplay:
			//Display the details of the parsed fields and CommDb as required.
			UnloadParser();
			if (iTestMode != ETWappConcise)
				RecordEntryDataL();
			iBioTestUtils->TestFinish(iTestCounter+1); // origin at zero.
			iTestCounter++;						// NB last time here, also shifts our zero for Test Summary!
			if (iTestCounter <iMessagesCreated) // Still files to process into messages
				iState = EWappCreateParser;		// so loop through the operations again.
			else
				iState = EWappComplete;	 
			if (result ==KErrNone)
				iTestSuccesses++;
			else
				iTestFailures++;
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;
		case EWappComplete:
			//Complete the tests
			test.Console()->ClearScreen();
			test.Console()->SetPos(0,0);
			//DisplayTestSummaryL(); Bit confusing at the moment
			if (result ==KErrNone)
				iBioTestUtils->TestHarnessCompleted();
			else
				iBioTestUtils->TestHarnessFailed(result);
			if (iWappParser)
				iWappParser->Cancel();
			CActiveScheduler::Stop();
			break;	
        default:
            break;
        }
    }
// end CTestWapp::RunL


void CTestWapp::DoCancel()
    {
    }
// end CTestWapp::DoCancel


void CTestWapp::QueueOperationAsync(TInt aErr)
    {
	// Set our status flag, prior to placing ourselves on the Active Scheduler.
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }
// End of CTestWapp::QueueOperationAsync


void CTestWapp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}
// end CTestWapp::RequestComplete



//
// Class functions		//	Not a good name, but ne'er mind.
//

//
//		StartL	-  The function controlling state engine 
//
void CTestWapp::StartL()
    {
//	TRAPD(err, iBioTestUtils->RunAuto() ? RunAutoTest() : ShowMenuL()); 
//	if (err)
//		iBioTestUtils->TestHarnessFailed(err);
//	else
		{
		Cancel();			// Halt any AO stuff
		iState=EWappClearCommDB; // set initial state
		InitialiseTesterL();  // Initialise variables and get AO going.
		}
	}
// end CTestWapp::StartL

void CTestWapp::RunAutoTest()
	{
	test.Next(_L("CTestWapp::Start()\n"));
	iTestMode = ETWappConcise;;
	}

void CTestWapp::ShowMenuL()
	{
	iBioTestUtils->ResetMenu();

	iBioTestUtils->AppendToMenuL(_L("Verbose Test  -   Logs parsed fields and CommDb tables"));
	iBioTestUtils->AppendToMenuL(_L("Detailed Test  -   Logs parsed fields only"));
	iBioTestUtils->AppendToMenuL(_L("Succinct Test -   Logs pass and fails"));

	TInt result = iBioTestUtils->DisplayMenu(_L("Schedule Sending Test"));

	switch (result)
		{
		case 1:
			iTestMode = ETWappVerbose;
			break;
		case 2:
			iTestMode = ETWappDetailed;
			break;
		case 3:
		default:
			iTestMode = ETWappConcise;
			break;
		}
	}


void CTestWapp::InitialiseTesterL()
	{
	// Get the NB if we don't need this two step process, use SetSmsServiceL().
	//iBioTestUtils->SetSmsServiceIdL();
	TRAPD(error, iBioTestUtils->SetBIOServiceIdL());
	if (error == KErrNotFound)
		{
		iBioTestUtils->CreateServicesL();
		}
	else
		{
		User::LeaveIfError(error);
		}
	// Empty the Inbox
	iBioTestUtils->EmptyInboxMessagesL();
	if (iTestMode == ETWappVerbose)
		iBioTestUtils->WriteComment(_L("Removed all messages from Global Inbox\n"));
	QueueOperationAsync(KErrNone);
	}


void CTestWapp::DeleteCommDbTableL()
	{
	// This is a very destructive function - it nukes all the CommDB tables in which 
	// WAPP makes entries.  Ensures that we are starting with an empty database.
	// WAPPs ability to overwrite existing entries is tested by having files with the same NAME
	// Characteristic. 

	TInt ret=KErrNone;
	TInt WAPEntries;
	TInt WAPIpEntries;
	TInt WAPSmsEntries;
	TInt ISPOutEntries;
	TInt IAPOutEntries;
	TInt GPRSEntries;

	CMDBSession* iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	TInt err = 0;
	CMDBRecordSet<CCDWAPAccessPointRecord> wapRecordSet(KCDTIdWAPAccessPointRecord);	//Open WAP AccessPoint table
	TRAP(err,wapRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		WAPEntries = wapRecordSet.iRecords.Count();
		if(WAPEntries > 0) 
			{
			TRAPD(err, wapRecordSet.DeleteL(*iDBSession));	
			}			
		WAPEntries = wapRecordSet.iRecords.Count();	
		}
	
	CMDBRecordSet<CCDWAPIPBearerRecord> wapIpBearerRecordSet(KCDTIdWAPIPBearerRecord);	//Open WAP IP Bearer table
	TRAP(err,wapIpBearerRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		WAPIpEntries = wapIpBearerRecordSet.iRecords.Count();
		if(WAPIpEntries > 0) 
			{
			TRAPD(err, wapIpBearerRecordSet.DeleteL(*iDBSession));	
			}
		WAPIpEntries = wapIpBearerRecordSet.iRecords.Count();
		}
		
	CMDBRecordSet<CCDWAPSMSBearerRecord> wapSmsBearerRecordSet(KCDTIdWAPSMSBearerRecord);	//Open WAP SMS Bearer table
	TRAP(err,wapSmsBearerRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		WAPSmsEntries = wapSmsBearerRecordSet.iRecords.Count();
		if(WAPSmsEntries > 0) 
			{
			TRAPD(err, wapSmsBearerRecordSet.DeleteL(*iDBSession));	
			}			
		WAPSmsEntries = wapSmsBearerRecordSet.iRecords.Count();
		}
		
	CMDBRecordSet<CCDIAPRecord> iapRecordSet(KCDTIdIAPRecord);	//Open Dial Out IAP table
	TRAP(err,iapRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		IAPOutEntries = iapRecordSet.iRecords.Count();
		if(IAPOutEntries > 0) 
			{
			TRAPD(err, iapRecordSet.DeleteL(*iDBSession));	
			}
		IAPOutEntries = iapRecordSet.iRecords.Count();
		}
	
	CMDBRecordSet<CCDDialOutISPRecord> dialOutIspRecordSet(KCDTIdDialOutISPRecord);	//Open Dial Out ISP table
	TRAP(err,dialOutIspRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		ISPOutEntries = dialOutIspRecordSet.iRecords.Count();
		if(ISPOutEntries > 0) 
			{
			TRAPD(err, dialOutIspRecordSet.DeleteL(*iDBSession));	
			}
		ISPOutEntries = dialOutIspRecordSet.iRecords.Count();
		}
		
	//CMDBRecordSet<CCDWCDMAPacketServiceRecord> gprsRecordSet;	//Open Gprs table
	CMDBRecordSet<CCDOutgoingGprsRecord> gprsRecordSet(KCDTIdOutgoingGprsRecord);
	TRAP(err,gprsRecordSet.LoadL(*iDBSession));
	if(err == KErrNone)
		{
		GPRSEntries = gprsRecordSet.iRecords.Count();
		if(GPRSEntries > 0) 
			{
			TRAPD(err, gprsRecordSet.DeleteL(*iDBSession));	
			}
		GPRSEntries = gprsRecordSet.iRecords.Count();
		}
		
	if (iTestMode == ETWappVerbose)
		{
		OutputFormattedTextL(_L("COMMDAT entries deleted:"));
		OutputTextListL(_L("WAP Table "), WAPEntries);
		OutputTextListL(_L("WAP IP Bearer"), WAPIpEntries);
		OutputTextListL(_L("WAP SMS Bearer "), WAPSmsEntries);
		OutputTextListL(_L("Dial Out IAP "), IAPOutEntries);
		OutputTextListL(_L("Dial Out ISP"), ISPOutEntries);
		OutputTextListL(_L("Outgoing GPRS"), GPRSEntries);
		}
	
	delete iDBSession;	
	iDBSession = NULL;
	}


void CTestWapp::CreateParserL()
	{
	// Point our iEntry to the relevant message entry and create the parser using this Id
	test.Printf(_L("\nCreating a WAPP parser...\n"));
	iEntryId = (*iMsvSelection)[iTestCounter];
	if (iWappParser)
		{
		delete iWappParser;
		iWappParser = NULL;
		}

#if defined (__HEAPFAILURE__)

	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		// 
		TRAP(error,iWappParser = iBioTestUtils->CreateParserL(iEntryId) );
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			finished = ETrue;
			__UHEAP_RESET;
			}
		// Handle error
		else
			{
			// Check if error is out of memory or a specific parser error
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	iWappParser = iBioTestUtils->CreateParserL(iEntryId);
#endif 

	__ASSERT_ALWAYS(iWappParser!=NULL, User::Panic(_L("WAPPTSRC"), KWappNullPtr));
	if (iWappParser)
		{
		if (iTestMode==ETWappVerbose)
			iBioTestUtils->WriteComment(_L("Created a WAP Access Point parser."));
		}
	else
		{
		if (iTestMode==ETWappVerbose)
			iBioTestUtils->WriteComment(_L("Failed to create a WAP Access Point Parser - error."));
		User::Leave(KWappErrNullValue);
		}
    }
// end CTestWapp::CreateParserL()


//
//	ExtractBodyPartsL()		Create a rich text object containing the contents of the
//							store. Copies the contents of this into class member descriptor.
//
void CTestWapp::ExtractBodyPartsL()
	{
	if (iMessageBody)
		delete iMessageBody;
	iMessageBody = NULL;
	iMessageBody = HBufC::NewL(2048);

	iMessageBody->Des().Copy(iBioTestUtils->MessageBodyL( (*iMsvSelection) [iTestCounter]) );
	}
// End of CTestWapp::ExtractBodyPartsL


//
//		ProcessUnparsedMessageL()  - Calls ProcessL for an unparsed message - should leave
//									 with error code -608
//
void CTestWapp::ProcessUnparsedMessageL()
	{
	// NB	Expect parser to fail with KWappErrMsgUnparsed (-609) from the 
	// ProcessL call because the message is unparsed!!

	TInt error = KErrNone;

#if defined (__HEAPFAILURE__)

	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		// 
		TRAP(error,iWappParser->ProcessL(iStatus));	
		if ( error ==KErrNone || error ==  KWappErrMsgUnparsed)
			{
			__UHEAP_RESET;
			finished = ETrue;
			}
		// Handle error
		else
			{
			// Check if error is out of memory or a specific fax rendering error
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
		TRAP(error,iWappParser->ProcessL(iStatus));	
#endif


	if (iTestMode ==ETWappVerbose)
		{
		if (error == KWappErrMsgUnparsed)
			OutputFormattedTextL(_L("ProcessL on unparsed message gave KWappErrMsgUnparsed (%d)  - OK! "),error);
		else 
			OutputFormattedTextL(_L("ProcessL call returned errror %d "),error);
		}
	if (error!=KErrNone && error != KWappErrMsgUnparsed)
		RequestComplete(iStatus,error);
	else if (error == KWappErrMsgUnparsed)
		RequestComplete(iStatus,KErrNone);
	}
// End of CTestWapp::ProcessUnparsedMessageL

//
//		ParseMessageTextL()	-	Calls ParseL passing contents of message to function.
//
void CTestWapp::ParseMessageL()
	{
	TInt error = KErrNone;

#if defined (__HEAPFAILURE__)

	TBool finished = EFalse;
	TInt failCount = 0;
	
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		// 
		TRAP(error,iWappParser->ParseL(iStatus,iMessageBody->Des()));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			finished = ETrue;
			}
		// Handle error
		else
			{
			// Check if error is out of memory or a specific fax rendering error
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}

#else 
	TRAP(error,iWappParser->ParseL(iStatus,iMessageBody->Des())); //RunL handles error in status
#endif

	if (iTestMode == ETWappDetailed || iTestMode== ETWappVerbose)
		OutputFormattedTextL(_L("ParseL returned error code %d"), error);
	if (error!=KErrNone)
		RequestComplete(iStatus,error);
	}
// End of CTestWapp::ParseMessageTextL


void CTestWapp::ProcessMessageL()
	{
	TInt error = KErrNone;

#if defined (__HEAPFAILURE__)
	TBool finished = EFalse;
	TInt failCount = 0;

	
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		// 
		TRAP(error,iWappParser->ProcessL(iStatus)); 
		if (error == KErrNone || error == KWappErrUnexpectedValue ||
			error == KWappErrUnrecognised||error == KWappErrBadMessage ||
			error == KWappErrMandatoryTagMissing)
			{
			__UHEAP_RESET;
			finished = ETrue;
			}
		// Handle error
		else
			{
			// Check if error is out of memory or a specific fax rendering error
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	TRAP(error, iWappParser->ProcessL(iStatus)); // RunL handles error later
#endif

	if (iTestMode == ETWappDetailed || iTestMode== ETWappVerbose)
		OutputFormattedTextL(_L("ProcessL call returned error %d "),error);
	if (error!=KErrNone)
		RequestComplete(iStatus, error);
	}
// End of CTestWapp::ProcessMessageL


//
//		UnloadParser()	-	Unload the parser and the registered dll.
//
void CTestWapp::UnloadParser()
	{
	delete iWappParser;
	iWappParser = NULL;
	test.Printf(_L("Unloaded the parser.\n"));
	}
// End of CTestWapp::UnloadParser


void CTestWapp::RecordEntryDataL()
    {
	iBioTestUtils->LogCurrentMessageTypeL(iEntryId);
	LogParsedDataL();		// Display parsed data fields
	
	if (iTestMode == ETWappVerbose)
		{
		DisplayCommDatTablesL();
		DisplayBookmarksL();
		DisplayMMSURLL();
		}
  }
// End of CTestWapp::DisplayEntryDataL


void CTestWapp::LogParsedDataL()
// Restores parsed data from message store and logs it.
	{
	// Run this first to create Parsed Field array for SetEntryNameL - leaves if no stream store!
	TRAPD(error, iBioTestUtils->LogExtractedFieldsL(iEntryId)); 
	
	if (error !=KErrNone)
		{
		if (error ==KErrNotFound)
			OutputFormattedTextL(_L("Entry has no Parsed Data - ParseL failed."));
		else
			OutputFormattedTextL(_L("Error %d occurred accessing Entry StreamStore"), error);
		iParsedFieldsNum =0;  // Flag that ParseL failed, so we don't try to go any further
		return;
		}
		
	SetEntryNameL();	// Find 1st Name characteristic ->Settings Name 
						// NB LogExtractedFields must succeed for this to work!
	}
// End of CTestWapp::DisplayStoreL



//
//	SetEntryNameL() - Finds first name characteristic, default if not specified in 
//					  message.  Copy the value to the settings name object
//
void CTestWapp::SetEntryNameL()
	{
	// Should never get here unless ParseL succeeded and there's parsed fields.
	TInt count =0;

	CArrayPtrSeg<CParsedField>& tempArrayHandle = iBioTestUtils->ParsedFieldArray();

	iParsedFieldsNum = tempArrayHandle.Count();

	if (iCurrentSettingsName != NULL)
		{
		delete iCurrentSettingsName;
		iCurrentSettingsName=NULL;
		}

	// Search for the first Characteristic NAME Tag
	while (count<tempArrayHandle.Count()  && iCurrentSettingsName ==NULL)
		{
		if (tempArrayHandle.At(count)->FieldName().CompareF(KWappCharName)==0) 
			{
			if (count +1 < tempArrayHandle.Count())
				{
				// Next field name should also be NAME & value should be none NULL
				if (  (tempArrayHandle.At(count+1)->FieldName().CompareF(KWappName)==0)
					&& (iCurrentSettingsName == NULL)
					&& (count <=tempArrayHandle.Count()) )
					{
					iCurrentSettingsName = HBufC::NewL(tempArrayHandle.At(count+1)->FieldValue().Length());
					iCurrentSettingsName->Des().Copy(tempArrayHandle.At(count+1)->FieldValue());
					}
				}
			}
		count++;
		}
	}

void CTestWapp::DisplayCommDatTablesL()
	{	
	TBuf<60> entryName;
	TInt retVal = 0;
	CMDBSession* iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	
	test.Printf(_L("\n"));  	
	OutputFormattedTextL( _L("----------	COMM Dat Entries for Message % d		----------"), iTestCounter+1);

	if (!(iParsedFieldsNum >0) || iCurrentSettingsName == NULL)
		{
		test.Printf(_L("\n"));
		OutputFormattedTextL(_L("COMM DB Entries not found") );
		test.Printf(_L("\n"));	
		return;  // failed to restore data - therefore failed to get Settings name
		}

	entryName.Copy(*iCurrentSettingsName);  // Name of the current entry
	
	CMDBRecordSetBase* genericRecord = GetGenericPointerL(KCDTIdWAPAccessPointRecord);
	CleanupStack::PushL(genericRecord);		
	
	CCDWAPAccessPointRecord *wapAPRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
	wapAPRecord->iRecordName.SetMaxLengthL(entryName.Length());
	wapAPRecord->iRecordName = entryName;
	genericRecord->iRecords.Append(wapAPRecord);
	
	TBool searchResult = genericRecord->FindL(*iDBSession);
		
	test.Printf(_L("\n"));	
	OutputFormattedTextL(_L("**********		WAP AP Table Entry		**********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for WAP AP returned error %d "), KErrNotFound);			
		return;
		}
	else
		{
		retVal = DisplayCommDatWapApTableL(*genericRecord);	
		}
			
	if (retVal != KErrNone) 
		{
		return;				// possibly multiple WAP entries of the same name!!			
		}
	CleanupStack::PopAndDestroy( genericRecord); 
	
	genericRecord = GetGenericPointerL(KCDTIdWAPSMSBearerRecord);
	CleanupStack::PushL(genericRecord);	
	
	CCDWAPSMSBearerRecord *smsBearerRecord = static_cast<CCDWAPSMSBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPSMSBearerRecord));
	
	smsBearerRecord->iWAPAccessPointId = iWapAccessPointID;
	genericRecord->iRecords.Append(smsBearerRecord);	
	searchResult = genericRecord->FindL(*iDBSession);	
		
	test.Printf(_L("\n"));	
	OutputFormattedTextL(_L("**********		WAP Sms Table Entry		**********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for WAP SMS Entry returned error %d "), KErrNotFound);			
		//return;
		}
	else
		{		
		retVal = DisplayCommDatWapSmsTableL(*genericRecord);	
		}
			
	if (retVal != KErrNone) 
		{
		//return;				// possibly multiple entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy(genericRecord); 	
		
	// Append the "GsmCsd" suffix to the Entry Name
	entryName.Append(KGsmCsdSuffix);
	
	// display the other tables, Dial Out ISP first 	
	genericRecord = GetGenericPointerL(KCDTIdDialOutISPRecord);
	CleanupStack::PushL(genericRecord);	
	
	CCDDialOutISPRecord *dialoutISPPRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
	dialoutISPPRecord->iRecordName.SetMaxLengthL(entryName.Length());
	dialoutISPPRecord->iRecordName = entryName;
	genericRecord->iRecords.Append(dialoutISPPRecord);
	
	searchResult = genericRecord->FindL(*iDBSession);
	TInt pp= ((CCDDialOutISPRecord*)(genericRecord->iRecords[0]))->iRecordTag.RecordId();
	
	test.Printf(_L("\n"));	
	OutputFormattedTextL(_L("**********		Dial Out ISP Entries		**********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for DialOut ISP returned error %d "), KErrNotFound);			
		return;
		}
	else
		{		
		retVal = DisplayCommDatDialOutISPTableL(*genericRecord, entryName);	
		}
			
	if (retVal != KErrNone) 
		{
		return;				// possibly multiple WAP entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord); 	
	
	genericRecord = GetGenericPointerL(KCDTIdIAPRecord);
	CleanupStack::PushL(genericRecord);	
	
	CCDIAPRecord *iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	iapRecord->iRecordName.SetMaxLengthL(entryName.Length());
	iapRecord->iRecordName = entryName;
	genericRecord->iRecords.Append(iapRecord);
	
	searchResult = genericRecord->FindL(*iDBSession);
	
	test.Printf(_L("\n"));	 
	OutputFormattedTextL( _L("**********          IAP Entries          **********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for IAP returned error %d "), KErrNotFound);			
		return;
		}
	else
		{		
		retVal = DisplayCommDatIAPTableL(*genericRecord, entryName, ETestWappGsmCsdBearer);	
		}
			
	if (retVal != KErrNone) 
		{
		return;				// possibly multiple WAP entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord);	
	
	genericRecord=GetGenericPointerL(KCDTIdWAPIPBearerRecord);
	CleanupStack::PushL(genericRecord);
	
	CCDWAPIPBearerRecord *ipBearerRecord = static_cast<CCDWAPIPBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
	ipBearerRecord->iWAPAccessPointId = iWapAccessPointID;
	genericRecord->iRecords.Append(ipBearerRecord);	
	
	searchResult = genericRecord->FindL(*iDBSession);
	
	test.Printf(_L("\n"));	 
	OutputFormattedTextL( _L("**********          WAP IP Table Entry          **********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for WAP IP returned error %d "), KErrNotFound);			
		//return;
		}
	else
		{		
		retVal = DisplayCommDatWapIpTableL(*genericRecord, entryName, iGsmCsdIapId);	
		}
			
	if (retVal != KErrNone) 
		{
		//return;				// possibly multiple WAP entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord); 	
	
	// Append the "Gprs" suffix to the Entry Name
	entryName.Copy(*iCurrentSettingsName);
	entryName.Append(KGprsSuffix);
	
	// Display Grps tables Outgoing WCDMA first, IAP, then WAP IP Bearer entry
	genericRecord = GetGenericPointerL(KCDTIdOutgoingGprsRecord);
	CleanupStack::PushL(genericRecord);	
	
	CCDOutgoingGprsRecord *gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
	gprsRecord->iRecordName.SetMaxLengthL(entryName.Length());
	gprsRecord->iRecordName = entryName;
	
	genericRecord->iRecords.Append(gprsRecord);
	searchResult = genericRecord->FindL(*iDBSession);
		
	test.Printf(_L("\n"));	 
	OutputFormattedTextL( _L("**********          GPRS Entry          **********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for GPRS returned error %d "), KErrNotFound);			
		//return;
		}
	else
		{		
		retVal = DisplayCommDatGprsTableL(*genericRecord, entryName);	
		}
			
	if (retVal != KErrNone) 
		{
		//return;				// possibly multiple WAP entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord);
	
	
	genericRecord=GetGenericPointerL(KCDTIdIAPRecord);
	CleanupStack::PushL(genericRecord);	
	
	iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));	
	iapRecord->iRecordName.SetMaxLengthL(entryName.Length());
	iapRecord->iRecordName = entryName;	
	genericRecord->iRecords.Append(iapRecord);
	
	searchResult = genericRecord->FindL(*iDBSession);
	
	test.Printf(_L("\n"));	 
	OutputFormattedTextL( _L("**********          IAP Entry          **********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for IAP returned error %d "), KErrNotFound);			
		//return;
		}
	else
		{		
		// This fn also sets iGprsIapId value - only way to determine the correct entry in WAP IP table
		retVal = DisplayCommDatIAPTableL(*genericRecord, entryName, ETestWappGprsBearer);	
		}
			
	if (retVal != KErrNone) 
		{
		//return;				// possibly multiple WAP entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord); 
	
	genericRecord=GetGenericPointerL(KCDTIdWAPIPBearerRecord);
	CleanupStack::PushL(genericRecord);		
	ipBearerRecord = static_cast<CCDWAPIPBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
	
	ipBearerRecord->iWAPAccessPointId = iWapAccessPointID;	
	genericRecord->iRecords.Append(ipBearerRecord);
	searchResult = genericRecord->FindL(*iDBSession);
	
	test.Printf(_L("\n"));	 
	OutputFormattedTextL( _L("**********          WAP IP Table Entry          **********"));
	
	if(!searchResult)
		{
		OutputFormattedTextL(_L("CommDat Table for WAP IP returned error %d "), KErrNotFound);			
	//	return;
		}
	else
		{		
		retVal = DisplayCommDatWapIpTableL(*genericRecord, entryName, iGprsIapId);			
		}
			
	if (retVal != KErrNone) 
		{
	//	return;				// possibly multiple entries of the same name!!			
		}				
	CleanupStack::PopAndDestroy( genericRecord); 
	
	OutputFormattedTextL( _L("------------------------------------------------------------"));
	
	delete iDBSession;	
	iDBSession = NULL;
	}

TInt CTestWapp::DisplayCommDatGprsTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName)
	{	
	TInt recCount = aGenericRecBase.iRecords.Count();
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry %S not found in IAP Table !"), &aEntryName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("ERROR - entry %S not a unique entry in IAP table !\n"), &aEntryName);
		return KErrNotFound;
		}
	// One unique Iap Table record ! Display a title then details 
	CCDOutgoingGprsRecord* record= (CCDOutgoingGprsRecord*) aGenericRecBase.iRecords[0];
	
	OutputTextListL(_L("GPRS_APN"), record->iGPRSAPN);

	if(record->iGPRSPDPType == RPacketContext::EPdpTypeIPv4)
		{
		OutputTextListL(_L("GPRS_PDP_TYPE "), _L("EPdpTypeIPv4"));	
		}
	else if(record->iGPRSPDPType == RPacketContext::EPdpTypeIPv6)
		{
		OutputTextListL(_L("GPRS_PDP_TYPE "), _L("EPdpTypeIPv6"));	
		}
	else if(record->iGPRSPDPType == RPacketContext::EPdpTypeX25)
		{	
		OutputTextListL(_L("GPRS_PDP_TYPE "), _L("EPdpTypeX25"));	
		}
	
	OutputTextListL(_L("GPRS_PDP_ADDRESS"), record->iGPRSPDPAddress);

	OutputTextListL(_L("GPRS_IF_PROMPT_FOR_AUTH"), record->iGPRSIfPromptForAuth);

	OutputTextListL(_L("GPRS_IF_AUTH_NAME"), record->iGPRSIfAuthName);

	OutputTextListL(_L("GPRS_IF_AUTH_PASS"), record->iGPRSIfAuthPass);	

	OutputTextListL(_L("GPRS_IP_GATEWAY"), record->iGPRSIPGateway);

	OutputTextListL(_L("GPRS_IP_DNS_ADDR_FROM_SERVER"), record->iGPRSIPDNSAddrFromServer);
	
	OutputTextListL(_L("GPRS_DISABLE_PLAIN_TEXT_AUTH"), record->iGPRSDisablePlainTextAuth);
	
	return KErrNone;	
	}
TInt CTestWapp::DisplayCommDatWapIpTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName, TUint32 aIapId)
	{
	TInt recCount = aGenericRecBase.iRecords.Count();
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry \"%S\" not found in WAP IP Table !"),&aEntryName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("Error - entry \"%S\" not a unique entry in WAP IP table !"), &aEntryName);
		return KErrNotFound;
		}
	// One unique Wap Table record ! Record title and contents	
	CCDWAPIPBearerRecord* record= (CCDWAPIPBearerRecord*) aGenericRecBase.iRecords[0];

	if (record->iWAPIAP != aIapId)
		{
		OutputFormattedTextL(_L("Entry \"%S\" not found in WAP IP Table !"),&aEntryName);
		return KErrNotFound;
		}

	OutputTextListL(_L("WAP IP Table ID (COMMDB_ID)"), record->iRecordTag.RecordId());

	OutputTextListL(_L("WAP_ACCESS_POINT_ID"), record->iWAPAccessPointId);

	OutputTextListL(_L("WAP_GATEWAY_ADDRESS"), record->iWAPGatewayAddress);
 	
	OutputTextListL(_L("WAP_IAP"), iCurrentIapID);

	// The days of this field are also numbered - will be removed from CommDb before long
	
	if(record->iWAPWSPOption == EWapWspOptionConnectionless)
		{
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Non-continuous connection type "));	
		}
	else if(record->iWAPWSPOption == EWapWspOptionConnectionOriented)
		{
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Continuous connection type "));	
		}
	else
		{		
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Unknown connection type "));		
		}
		
	if (record->iWAPSecurity)
		OutputTextListL(_L("WAP_SECURITY"), _L("WSP Security enabled "));
	else 
		OutputTextListL(_L("WAP_SECURITY"), _L("WSP security disabled "));

	OutputTextListL(_L("WAP_PROXY_PORT"), record->iWAPProxyPort);

	OutputTextListL(_L("WAP_PROXY_LOGIN_NAME"), record->iWAPProxyLoginName);

	OutputTextListL(_L("WAP_PROXY_LOGIN_PASS"), record->iWAPProxyLoginPass);

	User::After(1000);	
	return KErrNone;  // return good status value so that calling function continues executing			
	}

TInt CTestWapp::DisplayCommDatIAPTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName, TTestWappBearerType aBearerType)
	{	
	TInt recCount = aGenericRecBase.iRecords.Count();
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry %S not found in IAP Table !"), &aEntryName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("ERROR - entry %S not a unique entry in IAP table !\n"), &aEntryName);
		return KErrNotFound;
		}
	// One unique Iap Table record ! Display a title then details 
	CCDIAPRecord* record= (CCDIAPRecord*) aGenericRecBase.iRecords[0];
	
	OutputTextListL(_L("Dial-Out IAP name(COMMDB_NAME)"), record->iRecordName);

	OutputTextListL(_L("Dial Out IAP ID (iRecordId)"), record->iRecordTag.RecordId());

	switch (aBearerType)
		{
		case ETestWappGsmCsdBearer:
			iGsmCsdIapId = record->iRecordTag.RecordId();
			break;
		case ETestWappGprsBearer:
			iGprsIapId = record->iRecordTag.RecordId();
			break;
		case ETestWappUnknownBearer:
		case ETestWappGsmSmsBearer:
		case ETestWappGsmUssdBearer:
		case ETestWappIS136CsdBearer:
		default:
			{
			//Do nothing
			}
			break;
		}

	OutputTextListL(_L("IAP_SERVICE"), record->iService);

	// Added reading ISP_TYPE
	OutputTextListL(_L("IAP_SERVICE_TYPE"), record->iServiceType);

	User::After(1000);
	return KErrNone;	
	}
	
TInt CTestWapp::DisplayCommDatDialOutISPTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName)	
	{
	TInt recCount = aGenericRecBase.iRecords.Count();
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry \"%S\" not found in ISP Table !"), &aEntryName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("Error - entry %S not a unique entry in ISP table ! "), &aEntryName);
		return KErrNotFound;
		}
	CCDDialOutISPRecord* record= (CCDDialOutISPRecord*) aGenericRecBase.iRecords[0];

	OutputTextListL(_L("WAP Table Name(COMMDB_NAME)"), record->iRecordName);
	
	OutputTextListL(_L("WAP Table ID (iRecordId)"), record->iRecordTag.RecordId());
	
	OutputTextListL(_L("ISP_DESCRIPTION"), record->iDescription);
	
	if(record->iType == EIspTypeInternetOnly)
		{
		OutputTextListL(_L("ISP_TYPE"), _L("Internet Only ISP"));	
		}
	else if(record->iType == EIspTypeWAPOnly)
		{
		OutputTextListL(_L("ISP_TYPE"), _L("WAP Only ISP"));	
		}
	else if(record->iType == EIspTypeInternetAndWAP)
		{
		OutputTextListL(_L("ISP_TYPE"), _L("Internet and WAP ISP"));	
		}
	else 
		{
		OutputTextListL(_L("ISP_TYPE"), _L("Type of ISP unknown"));	
		}		
	
 	OutputTextListL(_L("ISP_DEFAULT_TEL_NUM"), record->iDefaultTelNum);
		
	if (record->iDialResolution)
		OutputTextListL(_L("ISP_DIAL_RESOLUTION"), _L("True - use Dial Resolution"));
	else 
		OutputTextListL(_L("ISP_DIAL_RESOLUTION"), _L("False - don't use Dial Resolution"));

	if (record->iUseLoginScript)
		OutputTextListL(_L("ISP_USE_LOGIN_SCRIPT"), _L("True - use login script"));
	else 
		OutputTextListL(_L("ISP_USE_LOGIN_SCRIPT"), _L("False - don't use login script"));

	if (record->iUseLoginScript)
		{		
		OutputTextListL(_L("ISP_LOGIN_SCRIPT"), record->iLoginScript);	
		}
		
	if (record->iPromptForLogin)
		OutputTextListL(_L("ISP_PROMPT_FOR_LOGIN"), _L("True - prompt for username and password"));
	else 
		OutputTextListL(_L("ISP_PROMPT_FOR_LOGIN"), _L("False - Do not prompt for username and password"));
 	
	OutputTextListL(_L("ISP_LOGIN_NAME"), record->iLoginName);
 	
	OutputTextListL(_L("ISP_LOGIN_PASS"), record->iLoginPass);
 	
	if (record->iDisplayPct)
		OutputTextListL(_L("ISP_DISPLAY_PCT"), _L("True - display PCT"));
	else
		OutputTextListL(_L("ISP_DISPLAY_PCT"), _L("False - don't display PCT"));
	
	OutputTextListL(_L("ISP_IF_PARAMS"), record->iIfParams);

	OutputTextListL(_L("ISP_IF_NETWORKS"), record->iIfNetworks);

	if (record->iIfPromptForAuth)
		OutputTextListL(_L("ISP_IF_PROMPT_FOR_AUTH"), _L("True - prompt for authentication"));
	else
		OutputTextListL(_L("ISP_IF_PROMPT_FOR_AUTH"), _L("False - don't prompt for authentication"));

	OutputTextListL(_L("ISP_IF_AUTH_NAME"), record->iIfAuthName);
	
	OutputTextListL(_L("ISP_IF_AUTH_PASS"), record->iIfAuthPass);
	
	OutputTextListL(_L("ISP_IF_AUTH_RETRIES"), record->iIfAuthRetries);

	if (record->iIfCallbackEnabled)
		OutputTextListL(_L("ISP_IF_CALLBACK_ENABLED"), _L("True - Callback enabled"));
	else
		OutputTextListL(_L("ISP_IF_CALLBACK_ENABLED"), _L("False - Callback disabled"));

	if (record->iIfCallbackEnabled)
		{
		// Following are only NOT NULL if callback is enabled		
		OutputTextListL(_L("ISP_IF_CALLBACK_TYPE"), record->iIfCallbackType);
		TPtr16 tempPtr(0,0);
		tempPtr.Copy(record->iIfCallbackInfo);
		OutputTextListL(_L("ISP_IF_CALLBACK_INFO"), tempPtr);

		OutputTextListL(_L("ISP_IF_CALLBACK_TIMEOUT"), record->iCallbackTimeout);
		}
	else 
		{
		// Callback isn't enabled so don't try to read these fields as they are NULL
		OutputTextListL(_L("ISP_IF_CALLBACK_TYPE"), _L("Not defined"));
 		OutputTextListL(_L("ISP_IF_CALLBACK_INFO"), _L("Not defined"));
		OutputTextListL(_L("ISP_IF_CALLBACK_TIMEOUT"), _L("Not defined"));
		}

	if (record->iIpAddrFromServer)
		OutputTextListL(_L("ISP_IP_ADDR_FROM_SERVER"), _L("True - get IP address from the server"));
	else
		OutputTextListL(_L("ISP_IP_ADDR_FROM_SERVER"), _L("False - know IP address"));
	
	OutputTextListL(_L("ISP_IP_ADDR"), record->iIpAddr);

	OutputTextListL(_L("ISP_IP_NETMASK"), record->iIpNetMask);

	OutputTextListL(_L("ISP_IP_GATEWAY"), record->iIpGateway);
	
	if (record->iIpDnsAddrFromServer)
		OutputTextListL(_L("ISP_IP_DNS_ADDR_FROM_SERVER"), _L("True - Get DNS address from server"));
	else 
		OutputTextListL(_L("ISP_IP_DNS_ADDR_FROM_SERVER"), _L("False - Don't get DNS address from server"));

	OutputTextListL(_L("ISP_IP_NAME_SERVER1"), record->iIpNameServer1);

	OutputTextListL(_L("ISP_IP_NAME_SERVER2"), record->iIpNameServer2);

	if (record->iEnableIpHeaderComp)
		OutputTextListL(_L("ISP_ENABLE_IP_HEADER_COMP"), _L("True - Enable compression of IP header "));
	else 
		OutputTextListL(_L("ISP_ENABLE_IP_HEADER_COMP"), _L("False - Disable compression of IP header"));
	
	if (record->iDisablePlainTextAuth)
		OutputTextListL(_L("ISP_DISABLE_PLAIN_TEXT_AUTH"), _L("True - Disable plain text authentication"));
	else 
		OutputTextListL(_L("ISP_DISABLE_PLAIN_TEXT_AUTH"), _L("False - Enable plain text authentication"));

	if (record->iEnableSwComp)
		OutputTextListL(_L("ISP_ENABLE_SW_COMP"), _L("True - Enable software compression"));
	else 
		OutputTextListL(_L("ISP_ENABLE_SW_COMP"), _L("False - Disable software compression"));
	
	if(record->iBearerSpeed == RMobileCall::ESpeedAutobauding)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - Auto"));		
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed9600)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 9600 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed14400)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 14400 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed19200)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 19200 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed28800)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 28800 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed38400)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 38400 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed48000)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 48000 baud"));	
		}
	else if(record->iBearerSpeed == RMobileCall::ESpeed56000)
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed - 56000 baud"));	
		}
	else 
		{
		OutputTextListL(_L("ISP_BEARER_SPEED"), _L("Speed  - unknown!"));
		}
		
	if( record->iBearerProtocol == RMobileCall::EProtocolUnspecified)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - unspecified"));
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolV22bis)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - V22bis"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolV32)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - V32"));			
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolV34)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - V34"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolV110)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - V110"));			
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolV120)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - V120"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolX31FlagStuffing)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - x31 Flag Stuffing"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolPIAFS)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - PIAFS"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolBitTransparent)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - Bit Transparent"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolPstnMultimediaVoiceFallback)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - Pstn Multimedia Voice Fallback"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolPstnMultimedia)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - Pstn Multimedia"));	
		}
	else if(record->iBearerProtocol == RMobileCall::EProtocolIsdnMultimedia)
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - Isdn Multimedia"));	
		}
	else
		{
		OutputTextListL(_L("ISP_BEARER_PROTOCOL"), _L("Protocol - unspecified"));	
		}
			
	User::After(1000);
	// Finished without error - return appropriate value
	return KErrNone;	
	}
TInt CTestWapp::DisplayCommDatWapSmsTableL(CMDBRecordSetBase& aGenericRecBase)
	{
	TInt recCount = aGenericRecBase.iRecords.Count();
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry \"%S\" not found in WAP Sms Table !"),iCurrentSettingsName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("Error - entry \"%S\" not a unique entry in WAP Sms table !"), iCurrentSettingsName);
		return KErrNotFound;
		}
	// One unique Wap Table record ! Record title and contents
	CCDWAPSMSBearerRecord* record= (CCDWAPSMSBearerRecord*) aGenericRecBase.iRecords[0];
	
	OutputTextListL(_L("WAP SMS Table ID (COMMDB_ID)"), record->iRecordTag.RecordId());

	OutputTextListL(_L("WAP_ACCESS_POINT_ID"), record->iWAPAccessPointId);

	OutputTextListL(_L("WAP_GATEWAY_ADDRESS"), record->iWAPGatewayAddress);

	OutputTextListL(_L("WAP_SERVICE_CENTRE_ADDRESS"), record->iWAPServiceCentreAddress);

	if(record->iWAPWSPOption == EWapWspOptionConnectionless)
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Non-continuous connection type "));
	else if(record->iWAPWSPOption == EWapWspOptionConnectionOriented)
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Continuous connection type "));
	else
		OutputTextListL(_L("WAP_WSP_OPTION"), _L("Unknown connection type "));
	
	if (record->iWAPSecurity)
		OutputTextListL(_L("WAP_SECURITY"), _L("WSP Security enabled "));
	else 
		OutputTextListL(_L("WAP_SECURITY"), _L("WSP security disabled "));

	User::After(1000);	
	return KErrNone;  // return good status value so that calling function continues executing		
	}

TInt CTestWapp::DisplayCommDatWapApTableL(CMDBRecordSetBase& aGenericRecBase)
	{	
	TInt recCount = aGenericRecBase.iRecords.Count();	
	
	if ( recCount == 0 )
		{
		OutputFormattedTextL(_L("Entry \"%S\" not found in WAP AP Table !"), iCurrentSettingsName);
		return KErrNotFound;
		}
	else if (recCount > 1)
		{
		OutputFormattedTextL(_L("Error - entry %S not a unique entry in WAP AP table !"), iCurrentSettingsName);
		return KErrNotFound;	
		}	
	//Only one record exists, read the field values
	CCDWAPAccessPointRecord* record= (CCDWAPAccessPointRecord*) aGenericRecBase.iRecords[0];
		
	// One unique Wap Table record ! Record title and contents

	OutputTextListL(_L("WAP Table Name(COMMDB_NAME)"), record->iRecordName); 
	
	OutputTextListL(_L("WAP Table ID (iRecordId)"), record->iRecordTag.RecordId()); 

	OutputTextListL(_L("WAP_START_PAGE"), record->iWAPStartPage); 
	
	OutputTextListL(_L("WAP_CURRENT_BEARER"), record->iWAPCurrentBearer); 
	
	User::After(1000);
	
	return KErrNone;  // return good status value so that calling function continues executing			
	}
	
CMDBRecordSetBase* CTestWapp::GetGenericPointerL(TMDBElementId aRecordElementId)
	{
	CMDBRecordSetBase*	tablePtr;
	switch(aRecordElementId)
		{
		case KCDTIdDialOutISPRecord:
			{
			CMDBRecordSet<CCDDialOutISPRecord>* dialoutIsp = new(ELeave) CMDBRecordSet<CCDDialOutISPRecord>(KCDTIdDialOutISPRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(dialoutIsp);			 
			}
			break;		
		case KCDTIdIAPRecord:
			{
			CMDBRecordSet<CCDIAPRecord>* iapRecord = new(ELeave) CMDBRecordSet<CCDIAPRecord>(KCDTIdIAPRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(iapRecord);	
			}
			break;
		case KCDTIdOutgoingGprsRecord:
			{
			CMDBRecordSet<CCDWCDMAPacketServiceRecord>* wcdmaRecord = new(ELeave) CMDBRecordSet<CCDWCDMAPacketServiceRecord>(KCDTIdWCDMAPacketServiceRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(wcdmaRecord);	
			}
			break;
		case KCDTIdWAPAccessPointRecord:
			{
			CMDBRecordSet<CCDWAPAccessPointRecord>* wapAccessPtRecord = new(ELeave) CMDBRecordSet<CCDWAPAccessPointRecord>(KCDTIdWAPAccessPointRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(wapAccessPtRecord);	
			}
			break;
		case KCDTIdWAPIPBearerRecord:
			{
			CMDBRecordSet<CCDWAPIPBearerRecord>* wapIpRecord = new(ELeave) CMDBRecordSet<CCDWAPIPBearerRecord>(KCDTIdWAPIPBearerRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(wapIpRecord);	
			}
			break;
		case KCDTIdWAPSMSBearerRecord:
			{
			CMDBRecordSet<CCDWAPSMSBearerRecord>* smsBearerRecord = new(ELeave) CMDBRecordSet<CCDWAPSMSBearerRecord>(KCDTIdWAPSMSBearerRecord);
			tablePtr = static_cast<CMDBRecordSetBase*>(smsBearerRecord);	
			}
			break;				
		}
	return tablePtr;
	}


void CTestWapp::DisplayBookmarksL()
	{
	// Print out and log the attachment file containing the WAP Bookmarks
	
	HBufC*	attachmentBuf =NULL;


	TFileName fileName;
	
	fileName.Num(iEntryId,EHex);

	fileName.Append(KEBookmarkExtension);
 
 	CMsvStore* store = iBioTestUtils->iMsvEntry->EditStoreL();
 	CleanupStack::PushL(store);
 	MMsvAttachmentManager& manager = store->AttachmentManagerL();

 	// find the attachment
 	TBool found = EFalse;
 	RFile attachmentFile;
 	for( TInt ii=0; ii<manager.AttachmentCount(); ++ii )
 		{
 		CMsvAttachment* attachment = manager.GetAttachmentInfoL(ii);
 		if( attachment->AttachmentName().CompareF(fileName) == 0 )
 			{
 			// found a match, open the file and load
 			delete attachment;
 			attachmentFile = manager.GetAttachmentFileL(ii);
 			found = ETrue; 
 			break;
 			}
 		delete attachment;
 		}
 	CleanupStack::PopAndDestroy(store);
 	
 	if(found)
 		{
 		// read the file data into a buffer
 		CleanupClosePushL(attachmentFile);
 		TInt fileSize;
 		User::LeaveIfError(attachmentFile.Size(fileSize));
 		
 		HBufC8* fileReadBuffer = HBufC8::NewLC(fileSize);
 		TPtr8 bufferPtr = fileReadBuffer->Des();
 		User::LeaveIfError(attachmentFile.Read(bufferPtr));
 		
		// convert to unicode 		
 		attachmentBuf = HBufC::NewL(fileReadBuffer->Length());
 		attachmentBuf->Des().Copy(*fileReadBuffer);
 		CleanupStack::PopAndDestroy(2, &attachmentFile); // fileReadBuffer, attachmentFile
 		CleanupStack::PushL(attachmentBuf);
 		}
 	else
 		{
 		OutputFormattedTextL(_L("**********		Bookmark Attachment File	**********"));
		OutputFormattedTextL(_L("      No bookmark file found."));
		return;
 		}
 
						
	OutputFormattedTextL(_L("**********		Bookmark Attachment File	**********"));
		
	TInt numBookmarks =1;
	TLex bookmarkLex = attachmentBuf->Des();
	TPtrC bookmarkPtr;


	while (!bookmarkLex.Eos())
		{
		bookmarkLex.Mark();	// Mark the start of this Bookmark
		while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek())==EFalse ) // Find the end of the Name
			bookmarkLex.Inc();
		
		bookmarkPtr.Set(bookmarkLex.MarkedToken());

		while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek()))
				bookmarkLex.Inc();  // Increment past the linefeed
		
		if (bookmarkPtr.CompareF(KEBookmarkItemBegin)!=0)  // check prefix for a bookmark
		{
			OutputFormattedTextL(_L("Bookmark %d  %S Begin Tag Missing"), numBookmarks, &bookmarkPtr);
			break;
		}

		for (TInt count=0; count < KNumberOfBookmarkFields; count++)
		{
			bookmarkLex.Mark();
			while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek())==EFalse ) // Find the end of the field
				bookmarkLex.Inc();
			bookmarkPtr.Set(bookmarkLex.MarkedToken());

			while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek()))
				bookmarkLex.Inc();	// increment past the linefeed

			OutputFormattedTextL(_L("Bookmark %d  %S"), numBookmarks, &bookmarkPtr);
		}

		bookmarkLex.Mark();	// Mark the start of the end tag

		while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek())==EFalse) // Find the end of the end tag
			bookmarkLex.Inc();

		bookmarkPtr.Set(bookmarkLex.MarkedToken());

		while (!bookmarkLex.Eos() && IsLineTermination(bookmarkLex.Peek()))
			bookmarkLex.Inc();	// increment past the linefeed

		if (bookmarkPtr.CompareF(KEBookmarkItemEnd) !=0)  // check end tag for a bookmark
		{
			OutputFormattedTextL(_L("Bookmark %d  %S End Tag Missing"), numBookmarks, &bookmarkPtr);
			break;
		}

		numBookmarks++;
		}

	CleanupStack::PopAndDestroy(attachmentBuf);
	  }


void CTestWapp::DisplayTestSummaryL()
	{
	OutputFormattedTextL(_L("----------       Test Summary      ----------"));
	OutputFormattedTextL(_L("Total number of tests             =   %d"), iTestCounter);
	OutputFormattedTextL(_L("Sucessful ParseL/ProcessL Ops     =   %d"), iTestSuccesses);
	OutputFormattedTextL(_L("Unsuccessful ParseL/ProcessL ops  =   %d"), iTestFailures);
	}

TBool CTestWapp::IsLineTermination(TChar aChar)
	{
	// Checks if the argument is a linebreak of any kind.
	if (  ( aChar != CEditableText::EParagraphDelimiter)
		&&( aChar != CEditableText::ELineBreak)
		&&( aChar != CEditableText::EPageBreak) //KBioLineFeed) 
		&&( aChar != (TChar) '\n')
		&&( aChar != (TChar) '\r'))
		
		return EFalse;    // Character Not a line break
	else 
		return ETrue;
	}

//
//	OutputTextListL() - Appends the second descriptor to the first descriptor with a separator
//							  between them.If Tabbing is on a tab is inserted at the front of the text
//
void CTestWapp::OutputTextListL(const TDesC& aFieldName, const TDesC& aValueName)
	{
	HBufC* outputBuf = HBufC::NewLC(200);
	TInt	  paddingFactor;
	TInt	  loop;
	_LIT(KSpace, " ");
	_LIT(KSeparatorString, " : ");

	outputBuf->Des().Copy(aFieldName);
	paddingFactor = 30 -outputBuf->Des().Length();
	for (loop =0; loop < paddingFactor; loop++)
		{
		outputBuf->Des().Append(KSpace);
		}
	outputBuf->Des().Append(KSeparatorString);
	if ( (outputBuf->Des().Length()+aValueName.Length()) <=200)
		outputBuf->Des().Append(aValueName); //create formatted buffer
	else
		{
		for (loop =0; loop < outputBuf->Des().MaxLength()-outputBuf->Des().Length(); loop++)
			{
			outputBuf->Des().Append( aValueName[loop]);
			}
		}
	PrintDescriptor(outputBuf->Des()); //print and log the formatted text
	iBioTestUtils->WriteComment(outputBuf->Des());
	CleanupStack::PopAndDestroy();  // outputBuf
	}

//
//	OutputTextListL()  - Converts the int to a string and then adds it to the first descriptor
//						 with a separator between them. If Tabbing is on a tab is inserted at the front of the text
//
void CTestWapp::OutputTextListL(const TDesC& aFieldName, const TUint aValue)
	{
	HBufC* formatBuf = HBufC::NewLC(64);
	HBufC* outputBuf = HBufC::NewLC(100);
	TInt	paddingFactor;
	_LIT(KSpace, " ");
	_LIT(KFormatString, " : %d");

	formatBuf->Des().Copy(aFieldName);
	paddingFactor = 30 -formatBuf->Des().Length();
	for (TInt i =0; i < paddingFactor; i++)
		formatBuf->Des().Append(KSpace);
	formatBuf->Des().Append(KFormatString);
	outputBuf->Des().Format(formatBuf->Des(), aValue);
	PrintDescriptor(outputBuf->Des());
	iBioTestUtils->WriteComment(outputBuf->Des());
	CleanupStack::PopAndDestroy(2);// outputBuf,formatBuf
	}


//
//  OutputFormattedTextL()   -  Creates a formatted string using the two paramenter passed in
//									  prints out to console and then to log, with a preceding tab if Tabbing is on.
void CTestWapp::OutputFormattedTextL(TRefByValue<const TDesC> aFmt,...)
										   //const TDesC& aFormatString, TInt aValue)
	{
	HBufC*  tempBuffer=HBufC::NewLC(1024); // aFormatString.Length()+20);

	VA_LIST list;
	VA_START(list,aFmt);
	// create the formatted list
	tempBuffer->Des().AppendFormatList(aFmt,list);
	
	test.Printf(tempBuffer->Des());
	test.Printf(_L("\n"));
	iBioTestUtils->WriteComment(tempBuffer->Des());
	CleanupStack::PopAndDestroy();  // tempDes
	}

//		DisplayLogDescriptorL()	-	Helper function that prints the descriptor to screen
//									and logs it to file
//	
void CTestWapp::PrintDescriptor(const TDesC& aBuffer)
	{
	test.Printf(aBuffer);
	test.Printf(_L("\n"));
	}


void CTestWapp::DisplayErrorReasonL(TInt& aReason)
    {
	
    switch (aReason)
        {
        case KErrNone:
			OutputFormattedTextL(_L("General Error (0)  - Test sequence completed successfully."));
            break;
		case KErrNotFound:
			OutputFormattedTextL(_L("General Error (-1)  - Not found."));
            break;
		case KErrCancel:
			OutputFormattedTextL(_L("General Error (-3) - Session cancelled."));
            break;
        case KErrNoMemory:
			OutputFormattedTextL(_L("General Error (-4) - No memory."));
            break;   
        case KErrNotSupported:
			OutputFormattedTextL(_L("General Error (-5) - Function Not Supported"));
            break;
        case KErrAccessDenied:
			OutputFormattedTextL(_L("General Error (-21) - Access Denied."));
            break;
		case KErrEof:
			OutputFormattedTextL(_L("General Error (-25) - Unexpected end of file."));
            break;
        case KErrDisconnected:
			OutputFormattedTextL(_L("General Error (-36) - Disconnected."));
            break;

		// General Base Class String Parser error code 
        case KBspInvalidMessage:
			OutputFormattedTextL(_L("Base Parser Error (-500) - Invalid message"));
            break;

		// Error codes specific to the WAPP parser
        case KWappBaseError:
			OutputFormattedTextL(_L("General WAP Access Point Parser Error (-600)."));
            break;
        case KWappErrXMLVersion:
			OutputFormattedTextL(_L("WAPP Error (-601)  - Unknown XML version."));
            break;
        case KWappErrOutsideBoundaries:
			OutputFormattedTextL(_L("WAPP Error (-602) - Index exceeds boundaries."));
            break;
        case KWappErrStringTable:
			OutputFormattedTextL(_L("WAPP Error (-603) - Bad String table reference."));
            break;
        case KWappErrEOS:
			OutputFormattedTextL(_L("WAPP Error (-604) - Unexpected end of message."));
            break;
        case KWappErrUnexpectedValue:
			OutputFormattedTextL(_L("WAPP Error (-605) - Unexpected value for current byte."));
            break;
        case KWappErrNoAttributes:
			OutputFormattedTextL(_L("WAPP Error (-606) - Not found expected attributes."));
            break;
        case KWappErrMandatoryTagMissing:
			OutputFormattedTextL(_L("WAPP Error (-607) - Mandatory token missing."));
            break;
        case KWappErrStoreNotFound:
			OutputFormattedTextL(_L("WAPP Error (-608) - No store attached to this entry."));
            break;
        case KWappErrMsgUnparsed:
			OutputFormattedTextL(_L("WAPP Error (-609) - Message not yet parsed."));
            break;
		case KWappErrUnrecognised:
			OutputFormattedTextL(_L("WAPP Error (-610) - Unrecognised value."));
            break;
		case KWappErrNullValue:
			OutputFormattedTextL(_L("WAPP Error (-611) - Unexpected Null value."));
            break;
		case KWappErrContent:
			OutputFormattedTextL(_L("WAPP Error (-612) - Message Content error occurred."));
            break;
		case KWappErrNoDBRecords:
			OutputFormattedTextL(_L("WAPP Error (-613) - No entries in the Comms DB for this message."));
            break;
		case KWappErrNotSupported:
			OutputFormattedTextL(_L("WAPP Error (-614) - Not supported by WAPP."));
            break;
		case KWappErrBadMessage:
			OutputFormattedTextL(_L("WAPP Error (-615) - Badly formed message - invalid XML document"));
			break;
		case KWappErrNoTermination:
			OutputFormattedTextL(_L("WAPP Error (-616) - Invalid termination of data structure"));
			break;
		default:
			OutputFormattedTextL(_L("Unknown error code = %d."), aReason);
			CActiveScheduler::Stop();		// Return from call to CActiveScheduler::Start().
            break;
        }
    }
// end DisplayErrorReasonL

//		DisplayMMSURLL()	-	Helper function that retrieves the MMS server address
//                              URL and logs it to file.
//		MMS is no longer supported. This function now does nothing.
void CTestWapp::DisplayMMSURLL()
	{
		OutputFormattedTextL(_L("MMS is no longer supported"));
	}


//
//				GLOBAL FUNCTIONS: IMPLEMENTATION				//
//


LOCAL_C void doParserTestL(RFs& aFs)
	{
	// Test the parser (Adds parser test to active scheduler):
    CTestWapp* testParser = CTestWapp::NewL(aFs);
    CleanupStack::PushL(testParser);
    test(testParser != NULL);
    testParser->StartL();		// Create message as first stage of testing.
    CActiveScheduler::Start();							// Run through parser test actions. See RunL.

	// Clean up:
    CleanupStack::PopAndDestroy(); // testParser
	test.Printf(_L("Cleaning up - testParser\n"));
	}


LOCAL_C void doMainL()
    {
    // Create a local variable for the Active Scheduler:
	CTestScheduler* myTestScheduler;
    myTestScheduler = new (ELeave) CTestScheduler;
    CleanupStack::PushL( myTestScheduler );
    CActiveScheduler::Install( myTestScheduler );

	// Create a file server session:
	RFs gFs;
    User::LeaveIfError(gFs.Connect());
    gFs.SetSessionPath(_L("C:\\"));

	doParserTestL(gFs); 

	CleanupStack::PopAndDestroy();	// myTestScheduler
	gFs.Close();
    }
// end doMainL()


GLDEF_C TInt E32Main()
    {
    test.Title();	// RTest test(_L("WAPP Test Harness")) has been declared globally.
    test.Start(_L("Wapp Test Harness"));

    __UHEAP_MARK;
    CTrapCleanup* cleanup=CTrapCleanup::New();
    test(cleanup!=NULL);
    TRAPD(error,doMainL());

	// Was the test sequence successful?
	if (error !=KErrNone)   
		test.Printf(_L("\nTest sequence failed , error code %d\n"),error);
	else
		test.Printf(_L("\nTests completed, error status %d\n"), error);
	delete cleanup;
    __UHEAP_MARKEND;

	test.End();
	test.Close();
    return KErrNone;
    }
// end E32Main()
