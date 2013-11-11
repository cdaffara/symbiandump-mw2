// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include <e32std.h>
#include <e32test.h>
#include <msvstd.h>
#include <msvids.h>
#include <msvuids.h>
#include <pushentry.h>
#include <push/pushlog.h>
#include <push/cwappushmsgutils.h>
#include "testlog.h"
#include "t_serviceutils.h"
#include "t_pushentry.h"


_LIT(KPushEntryTestTitle,"T_PushEntry");
_LIT(KFilename,"picture.jpg");
_LIT(KFilename2,"image.gif");

// Uncomment the following define to switch on Heap Failure Testing
// #define __HEAP_FAILURE_TESTING



// Global variables
RTest gTest(_L("t_PushMessage Test Harness"));






/** Active scheduler defined for this testcode to catch error */
void CTestScheduler::Error(TInt aError) const
    {
    CActiveScheduler::Stop();
    gTest.Printf(_L("\nLeave signalled, reason=%d\n"),aError);
    }





CMessageDetails* CMessageDetails::NewL()
	{
	CMessageDetails* self = new (ELeave) CMessageDetails();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CMessageDetails::ConstructL()
	{
	iPushMsgType =0;   //Uid of message type goes in iBioType
	iStatusFlag = 0;
	iActionFlag =0;
	iSiId = HBufC::NewL(100);
	iCreated = Time::NullTTime(); //.DateTime();
	iExpires = Time::NullTTime();
	iURL =  HBufC::NewL(100);
	iText = HBufC::NewL(100);
	iFrom = HBufC8::NewL(100);
	iHeader = HBufC8::NewL(100);
	iContentType=HBufC::NewL(100);
	iMessageBody =HBufC8::NewL(300);
	iMsgDetails =HBufC::NewL(300);
	iMsgDescription =HBufC::NewL(300);
	iFilename = HBufC::NewL(100);
	}

CMessageDetails::CMessageDetails()
	{
	}

CMessageDetails::~CMessageDetails()
	{
	delete iSiId;
	delete iURL;
	delete iText;
	delete iFrom;	
	delete iHeader;
	delete iContentType;
	delete iMessageBody;
	delete iMsgDetails;
	delete iMsgDescription;
	delete iFilename;
	}

void CMessageDetails::ResetL()
	{
	iPushMsgType =0;   //Uid of message type goes in iBioType
	iStatusFlag = 0;
	iActionFlag =0;
	delete iSiId;
	iSiId = NULL;
	iSiId = HBufC::NewL(100);

	iCreated= Time::NullTTime();
	iExpires = Time::NullTTime();

	delete iURL; 
	iURL =NULL;
	iURL = HBufC::NewL(100);

	delete iText;
	iText = NULL;
	iText = HBufC::NewL(100);

	delete iFrom;
	iFrom = NULL;
	iFrom = HBufC8::NewL(100);

	delete iHeader;
	iHeader = NULL;
	iHeader = HBufC8::NewL(100);

	delete iContentType;
	iContentType =NULL;
	iContentType = HBufC::NewL(100);
	
	delete iMessageBody;
	iMessageBody=NULL;
	iMessageBody = HBufC8::NewL(300);
	}



// Test Harness - Active Object with a lot of states. Tests Get/Set, Save/Retrieve & Update,
// for Push Entries. Also tests the Message Utilities.  Use both together during the operations.
CPushEntryTester* CPushEntryTester::NewL(MWapPushLog& aLog)
	{
	CPushEntryTester* self = new (ELeave) CPushEntryTester(aLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CPushEntryTester::ConstructL()
	{
	__LOG_ALWAYS("Test Results");
	__LOG_ALWAYS(" ");

	iPushUtilities = CWapPushMsgUtils::NewL();
	iSISLPushUtilities = CSISLPushMsgUtils::NewL();
	iDetails = CMessageDetails::NewL();
	iMsvEntry = iPushUtilities->Session().GetEntryL(KMsvRootIndexEntryId);
    CActiveScheduler::Add(this);	
	}


CPushEntryTester::CPushEntryTester(MWapPushLog& aLog)
:CActive(CActive::EPriorityIdle), iLog(aLog)
	{
	}


CPushEntryTester::~CPushEntryTester()
	{
	Cancel();
	delete iMsvEntry;
	delete iDetails;
	delete iPushEntry;
	delete iPushUtilities;
	delete iSISLPushUtilities;
	}

void CPushEntryTester::LogFormatInt(const TDesC& aFormat, TInt aInt)
	{
	iFormatBuffer.Format(aFormat, aInt);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CPushEntryTester::LogFormatInt(const TDesC& aFormat, TInt aInt1, TInt aInt2)
	{
	iFormatBuffer.Format(aFormat, aInt1, aInt2);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CPushEntryTester::LogFormatStr(const TDesC& aFormat, const TDesC& aDes)
	{
	iFormatBuffer.Format(aFormat, &aDes);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CPushEntryTester::LogStr(const TDesC& aDes)
	{
	iLog.WPLPrintf(aDes);
	}



void CPushEntryTester::RunL()
	{
	switch(iState)
		{
		case ETestInitialising:  
			__LOG_ALWAYS("=========    TEST - SI GET/SET WITH PARAMETERS              =========");
			CreateMsgDetailsL(EFalse);
			SiGetSetTestL();
			iState =ETestSIGetSet;// Test the setters getters with Non Null fields
			QueueOperationAsync(KErrNone);
			break;
		case ETestSIGetSet:
			__LOG_ALWAYS("=========    TEST - SI SAVE/RETRIEVE WITH PARAMETERS        =========");
			SiSaveRetrieveTestL();
			iState = ETestSISaveRetrieveWithStringAppID;//Test Save Retrieve with Non Null fields
			QueueOperationAsync(KErrNone);
			break;

		case ETestSISaveRetrieveWithStringAppID:
			__LOG_ALWAYS("=========    TEST - SI SAVE/RETRIEVE WITH PARAMETERS AND STRING APPID  =========");
			SiSaveRetrieveTestL();
			iState = ETestSISaveRetrieveWithIntAppID;
			QueueOperationAsync(KErrNone);
			break;

		case ETestSISaveRetrieveWithIntAppID:
			__LOG_ALWAYS("=========    TEST - SI SAVE/RETRIEVE WITH PARAMETERS AND INT APPID  =========");
			iState = ETestSISaveRetrieve;
			SiSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSISaveRetrieve:
			__LOG_ALWAYS("=========    TEST - SI GET/SET WITH NULL FIELDS             =========");
			CreateMsgDetailsL(ETrue);
			SiGetSetTestL();
			iState = ETestSIGetSetNull;// Test the setters getters with Null fields
			QueueOperationAsync(KErrNone);
			break;

		case ETestSIGetSetNull:
			iState =  ETestSISaveRetrieveNull; //Test Save/Retrieve with Empty fields
			__LOG_ALWAYS("=========    TEST - SI SAVE/RETRIEVE WITH NULL FIELDS       =========");
			SiSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSISaveRetrieveNull:
			__LOG_ALWAYS("=========    TEST - UPDATING A EXISTING SI MESSAGE          =========");
			iState = ETestSIChangeEntry;
			ChangeSIEntryTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSIChangeEntry:
			__LOG_ALWAYS("=========    TEST - SL GET/SET WITH PARAMETERS              =========");
			iState = ETestSLGetSet;//Test SL Set/Get functions with Empty fields
			CreateMsgDetailsL(EFalse);
			SlGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLGetSet:
			iState = ETestSLSaveRetrieve;
			__LOG_ALWAYS("=========    TEST - SL SAVE/RETRIEVE WITH PARAMETERS        =========");
			SlSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLSaveRetrieve:
			__LOG_ALWAYS("=========    TEST - SL GET/SET WITH EMPTY FIELDS            =========");
			iState = ETestSLGetSetNull;//Test SL Set/Get functions with Empty fields
			CreateMsgDetailsL(ETrue);
			SlGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLGetSetNull:
			__LOG_ALWAYS("=========    TEST - SL SAVE/RETRIEVE WITH EMPTY FIELDS      =========");
			iState = ETestSLSaveRetrieveNull;
			SlSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLSaveRetrieveNull:
			__LOG_ALWAYS("=========    TEST - UPDATING A EXISTING SL MESSAGE          =========");
			iState = ETestSLChangeEntry;
			ChangeSLEntryTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLChangeEntry:
			__LOG_ALWAYS("=========    TEST - MULTI GET/SET WITH PARAMETERS           =========");
			iState = ETestMultiGetSet;
			CreateMsgDetailsL(EFalse);
			MultiGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestMultiGetSet:
			__LOG_ALWAYS("=========    TEST - MULTI SAVE/RETRIEVE WITH PARAMETERS      =========");
			iState = ETestMultiSaveRetrieve;
			MultiSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestMultiSaveRetrieve: 
			__LOG_ALWAYS("=========    TEST - MULTI SET/GET WITH EMPTY FIELDS         =========");
			iState = ETestMultiGetSetNull;
			CreateMsgDetailsL(ETrue);
			MultiGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;
		
		case ETestMultiGetSetNull:
			__LOG_ALWAYS("=========   TEST - MULTI SAVE/RETRIEVE WITH EMPTY FIELDS    =========");
			iState = ETestMultiSaveRetrieveNull;
			MultiSaveRetrieveTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestMultiSaveRetrieveNull:
			__LOG_ALWAYS("=========    TEST - UNKNOWN GET/SET WITH PARAMETERS          =========");
			iState = ETestUnknownGetSet;
			CreateMsgDetailsL(EFalse);		
			UnknownGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownGetSet:
			__LOG_ALWAYS("=========    TEST - UNKNOWN SAVE/RETRIEVE WITH PARAMETERS    ==========");
			iState = ETestUnknownSaveRetrieve;
			UnknownSaveRetrieveTestL(EFalse);
			QueueOperationAsync(KErrNone);
			break;	

		case ETestUnknownSaveRetrieve:
			__LOG_ALWAYS("=========    TEST - UNKNOWN SAVE/RETRIEVE WITH PARAMETERS AND FILENAME     =========");
			iState = ETestUnknownSaveRetrieveWithFilename;
			UnknownSaveRetrieveTestL(ETrue);
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownSaveRetrieveWithFilename:
			__LOG_ALWAYS("=========    TEST - UNKNOWN GET/SET WITH EMPTY FIELDS        =========");
			iState = ETestUnknownGetSetNull;
			CreateMsgDetailsL(ETrue);
			UnknownGetSetTestL();
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownGetSetNull:
			__LOG_ALWAYS("=========    TEST - UNKNOWN SAVE/RETRIEVE WITH EMPTY FIELDS   =========");
			iState = ETestUnknownSaveRetrieveNull;
			UnknownSaveRetrieveTestL(EFalse);
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownSaveRetrieveNull:  
			__LOG_ALWAYS("=========    TEST - UNKNOWN SAVE/RETRIEVE WITH EMPTY FIELDS AND FILENAME     =========");
			iState = ETestUnknownSaveRetrieveNullWithFilename;
			UnknownSaveRetrieveTestL(ETrue);
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownSaveRetrieveNullWithFilename: 	// Test updating an Unknown Msg
			__LOG_ALWAYS("=========     TEST - UPDATING A EXISTING UNKNOWN MESSAGE     =========");
			iState = ETestUnknownChangeEntry;
			ChangeUnknownEntryTestL(EFalse);
			QueueOperationAsync(KErrNone);
			break;

		case ETestUnknownChangeEntry:	// Test updating an Unknown Msg with filename
			__LOG_ALWAYS("=========    TEST - UPDATING A EXISTING UNKNOWN MESSAGE  WITH FILENAME     =========");
			iState = ETestUnknownChangeEntryWithFilename;  
			ChangeUnknownEntryTestL(ETrue);
			QueueOperationAsync(KErrNone);
			break;


		case ETestUnknownChangeEntryWithFilename:
			__LOG_ALWAYS("=========    TEST - SI FIND BY AN ID                          =========");
			iState = ETestSIFindSiId;  
			CreateMoreSiMessagesL();
			FindSiIdTestL(EFalse); // Not looking for a null ID string
			QueueOperationAsync(KErrNone);
			break;

		case ETestSIFindSiId:
			iState =  ETestSIFindSiIdNull; //Test Find Function  with Empty fields
			__LOG_ALWAYS("=========    TEST - SI FIND A NULL ID                         =========");
			FindSiIdTestL(ETrue);  // Looking for a null ID string
			QueueOperationAsync(KErrNone);
			break;

		case ETestSIFindSiIdNull:
			iState =  ETestSIFindSiUrl; //Test Find with Non Null URL field
			__LOG_ALWAYS("=========    TEST - SI FIND A URL                             =========");
			FindUrlTestL(EFalse, KUidWapPushMsgSI);
			QueueOperationAsync(KErrNone);
			break;

		case ETestSIFindSiUrl:
			iState = ETestSIFindSiUrlNull;  //Test Find with a Null URL field
			__LOG_ALWAYS("=========    TEST - SI FIND A NULL URL                        =========");
			FindUrlTestL(ETrue, KUidWapPushMsgSI);
			QueueOperationAsync(KErrNone); 
			break;

		case ETestSIFindSiUrlNull:
			__LOG_ALWAYS("=========    TEST - SL FIND A URL                             =========");
			iState = ETestSLFindUrl;
			CreateMoreSLMessagesL();
			FindUrlTestL(EFalse, KUidWapPushMsgSL);
			QueueOperationAsync(KErrNone);
			break;
		
		case ETestSLFindUrl :
			__LOG_ALWAYS("=========    TEST - SL FIND NULL URL                          =========");
			iState = ETestSLFindUrlNull;
			FindUrlTestL(ETrue , KUidWapPushMsgSL);
			QueueOperationAsync(KErrNone);
			break;

		case ETestSLFindUrlNull:
			__LOG_ALWAYS("=========    TEST - GET HIGHEST SI ACTION                     =========");
			iState = ETestSIGetHiAction;
			GetActionTestL(KUidWapPushMsgSI);
			QueueOperationAsync(KErrNone); 
			break;


		case ETestSIGetHiAction:
			__LOG_ALWAYS("=========    TEST - GET HIGHEST SL ACTION                     =========");
			iState = ETestSLGetHiAction ;
			GetActionTestL(KUidWapPushMsgSL);
			QueueOperationAsync(KErrNone); 
			break;


		case ETestSLGetHiAction:
			iState = ETestGetExpiry;
			__LOG_ALWAYS("=========    TEST - GET EARLIEST SI EXPIRY                    =========");
			//DumpPushMessageDetailsL(KUidWapPushMsgSI);
			//DumpPushMessageDetailsL(KUidWapPushMsgSL);
			//DumpPushMessageDetailsL(KUidWapPushMsgMultiPart);
			//DumpPushMessageDetailsL(KUidWapPushMsgUnknown);
			GetExpiryTestL();
 			QueueOperationAsync(KErrNone); 
			break;

		case ETestGetExpiry:
			iState = ETestGetNextExpiry;
			__LOG_ALWAYS("=========    TEST - GET NEXT     SI EXPIRY                    =========");
			DeleteEarliestExpiryL();
			GetExpiryTestL();
			QueueOperationAsync(KErrNone); 
			break;

		case ETestGetNextExpiry:
			iState = ETestComplete;
			if (!iFailures)
				{
				__LOG_ALWAYS(" ");
				__LOG_ALWAYS("Tests Completed");
				}
			else 
				{
				LogFormatInt(_L("=========      TESTS FAILED !! TOTAL OF %d UNSUCESSFUL        ========="), iFailures);
				}
			QueueOperationAsync(KErrNone);
			break;

		case ETestComplete: //finished
			User::After(10000);
			Cancel();
			CActiveScheduler::Stop();
			break;
		default:
			break;
		}
	User::After(500);
	}


void CPushEntryTester::QueueOperationAsync(TInt aErr)
    {
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }

void CPushEntryTester::DoCancel()
	{
	}

void CPushEntryTester::StartL()
	{
	iFailures = 0;
	CreateHeaders();
	iState = ETestInitialising;
	QueueOperationAsync(KErrNone);
	}

// Halt the Scheduler & inform the user
TInt CPushEntryTester::RunError(TInt aError)
	{
	if (aError != KErrNone)
		{
		LogFormatInt(_L("Error %d  occurred"), aError);
		__LOG_DEBUG("Tests Failed!");
		iFailures++;
		CActiveScheduler::Stop();
		}
	return KErrNone;
	}




// Tests use of get/set methods on a CSIPushMsgEntry
// Create new entry,set values on TMsvEntry,set Push Entry, display & set new values
void CPushEntryTester::SiGetSetTestL()
	{
	__LOG_ALWAYS("***** TESTING SI SETTER FUNCTIONS *****");

	CSIPushMsgEntry* siEntry = NULL;
	
#if defined (__HEAP_FAILURE_TESTING)
	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;
	
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,siEntry = DoSiCreateTestL());		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSiCreateTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	siEntry = DoSiCreateTestL(); 
#endif
	CleanupStack::PushL(siEntry);
	OutputMessageDetailsL(*siEntry);  // Tests our get functions - don't allocate memory


#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
	
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoSiSetTestL(*siEntry));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSiSetTestL()"), failCount);
			finished = ETrue;
			}
		
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	DoSiSetTestL(*siEntry); 
#endif

	OutputMessageDetailsL(*siEntry);
	CleanupStack::PopAndDestroy();
	__LOG_ALWAYS("*****   SI SET/GET -  TEST SUCCESSFUL  ******");
	}

CSIPushMsgEntry* CPushEntryTester::DoSiCreateTestL()
	{	
	TPtrC ptr;
	TMsvEntry entry;

	entry.iServiceId = 0x0001234;
	entry.iMtm.iUid = 0x0012345;  // dummy value to check default gets set automatically!
	entry.iBioType  = 0x0054321;  // ditto 
	entry.iType = KUidMsvMessageEntry;
	entry.SetMtmData1(0x01FF); 
	ptr.Set(_L("123456@somewhere.com"));

	CSIPushMsgEntry* siEntry = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siEntry);
	siEntry->SetEntry(entry);

	siEntry->SetExpires( TTime(_L("20020000:120000.0000")) );
	siEntry->SetHeaderL(iDummyHeader2);	
	siEntry->SetTextL(KUrl2);


	siEntry->SetMsgDescriptionL(ptr);
	siEntry->SetMsgDetailsL(_L("http;//www.garbage.com"));
	
	CleanupStack::Pop();
	return siEntry;	
	}

void CPushEntryTester::DoSiSetTestL(CSIPushMsgEntry& aSiEntry)
	{
	aSiEntry.SetAction(iDetails->iActionFlag);
	aSiEntry.SetCreated( iDetails->iCreated );
	aSiEntry.SetExpires( iDetails->iExpires);
	aSiEntry.SetFromL(*iDetails->iFrom);
	aSiEntry.SetHeaderL(*iDetails->iHeader);
	aSiEntry.SetIdL(*iDetails->iSiId);
	aSiEntry.SetStatus(iDetails->iStatusFlag); 
	aSiEntry.SetTextL(*iDetails->iText);
	aSiEntry.SetUrlL(*iDetails->iURL);

	aSiEntry.SetMsgDetailsL(*iDetails->iMsgDetails);
	aSiEntry.SetMsgDescriptionL(*iDetails->iMsgDescription);
	}



// Tests the Save and Retrieve functions - saves a message and then loads it 
// - should remain unchanged
void CPushEntryTester::SiSaveRetrieveTestL()
	{
	__LOG_ALWAYS("*****   TEST STORING NEW SI PUSH ENTRY  DATA   *****");  // Log details
	TMsvId msgEntryId =0;
	CSIPushMsgEntry* siSave =NULL;

#if defined (__HEAP_FAILURE_TESTING)
	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;
	
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, siSave = DoSiSaveTestL());		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSiSaveTestL()"), failCount);
			finished = ETrue;
			}
		
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	siSave = DoSiSaveTestL(); 
#endif

	CleanupStack::PushL(siSave);
	OutputMessageDetailsL(*siSave);
	msgEntryId = siSave->Entry().Id();
	
	__LOG_ALWAYS("*****  TEST RESTORING DATA INTO NEW PUSH ENTRY  *****");
	CSIPushMsgEntry* siLoad = NULL;

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;  //reset loop variables
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,siLoad = DoSiRetrieveTestL(msgEntryId));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSiRetrieveTestL(msgEntryId)"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	siLoad = DoSiRetrieveTestL(msgEntryId); 
#endif
	CleanupStack::PushL(siLoad);

	OutputMessageDetailsL(*siLoad);
	CleanupStack::PopAndDestroy(2);  //siLoad, siSave
	__LOG_ALWAYS("***** SI  SAVE/RETRIEVE -  TEST SUCCESSFUL  ******");
	if (iDetails->iNullEntries)
		iSINullMsg = msgEntryId; // Hold the id of the null entry for a later test
	else
		iSIToChange = msgEntryId;  //Hold the id of the non null entry for changing later
	}

CSIPushMsgEntry* CPushEntryTester::DoSiSaveTestL()
	{
	CSIPushMsgEntry* siSave=NULL;
	if (iState==ETestSISaveRetrieveWithStringAppID)
		{	
		siSave = CSIPushMsgEntry::NewL();
		}
	else if (iState==ETestSISaveRetrieveWithIntAppID)
		{
		TInt tmp=5432;
		siSave = CSIPushMsgEntry::NewL(tmp);
		}
	else
		{
		siSave = CSIPushMsgEntry::NewL();
		}

	CleanupStack::PushL(siSave);
	siSave->SetIdL(*iDetails->iSiId);	
	siSave->SetHeaderL(*iDetails->iHeader);
	siSave->SetAction(iDetails->iActionFlag);
	siSave->SetStatus(iDetails->iStatusFlag);
//	siSave->SetCreated(iDetails->iCreated);
	
	siSave->SetExpires(iDetails->iExpires);
	siSave->SetUrlL(*iDetails->iURL);
	siSave->SetTextL(*iDetails->iText);
	siSave->SetFromL(*iDetails->iFrom); 
	
	TTime now;	
	now.UniversalTime();
	siSave->SetCreated(now);

	if (iPushMsgFolderId ==KMsvNullIndexEntryId)
		iSISLPushUtilities->GetPushMsgFolderIdL(iPushMsgFolderId);
    siSave->SaveL(iPushUtilities->Session(),iPushMsgFolderId);
	CleanupStack::Pop();
	return siSave;
	}

CSIPushMsgEntry* CPushEntryTester::DoSiRetrieveTestL(TMsvId aMsgId)
	{
	CSIPushMsgEntry* siLoad = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siLoad);  
	siLoad->RetrieveL(iPushUtilities->Session(), aMsgId);
	CleanupStack::Pop();
	return siLoad; // for displaying data
	}


// Tests use of get/set methods on a CSLPushMsgEntry
// Create new entry,set values on TMsvEntry,set Push Entry, display & set new values
void CPushEntryTester::SlGetSetTestL()
	{
	CSLPushMsgEntry* slEntry = NULL;
	__LOG_ALWAYS("***** TESTING SL SETTER FUNCTIONS  *****");

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,slEntry = DoSlCreateTestL());		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSlCreateTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	slEntry = DoSlCreateTestL(); 
#endif
	CleanupStack::PushL(slEntry);
	OutputMessageDetailsL(*slEntry);

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoSlSetTestL(*slEntry));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSlSetTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	DoSlSetTestL(*slEntry); 
#endif

	OutputMessageDetailsL(*slEntry);
	CleanupStack::PopAndDestroy();  //slEntry
	__LOG_ALWAYS("***** SL PUSH ENTRY SET/GET -  TEST SUCCESSFUL  ******");
	}

CSLPushMsgEntry* CPushEntryTester::DoSlCreateTestL()
	{
	TMsvEntry entry;
	entry.iServiceId = 0x0999999; // Dummy value 
	entry.iMtm.iUid = 0x11111111; //Silly value
	entry.iBioType  = 0x44444444;  //another silly value 
	entry.iType = KUidMsvMessageEntry;
	entry.SetMtmData1(0x1FF);

	CSLPushMsgEntry* slEntry = CSLPushMsgEntry::NewL();
	CleanupStack::PushL(slEntry);
	slEntry->SetEntry(entry);
	slEntry->SetHeaderL(iDummyHeader2);
	slEntry->SetUrlL(KUrl3);

	slEntry->SetMsgDescriptionL(_L("http://www.waffle.com"));
	slEntry->SetTimeSent(KDateExpiry4); 
	slEntry->SetMsgDetailsL(_L("Henry Higgins"));

	CleanupStack::Pop();
	return slEntry;
	}

void CPushEntryTester::DoSlSetTestL(CSLPushMsgEntry& aSlEntry)
	{
	aSlEntry.SetAction(iDetails->iActionFlag);
	aSlEntry.SetFromL(*iDetails->iFrom);
	aSlEntry.SetStatus(iDetails->iStatusFlag);  
	aSlEntry.SetUrlL(*iDetails->iURL);	
	aSlEntry.SetTimeSent(iDetails->iCreated);
	aSlEntry.SetHeaderL(*iDetails->iHeader);
	}


// Tests the Save and Retrieve functions - saves a message and then loads it - should remain unchanged
void CPushEntryTester::SlSaveRetrieveTestL()
	{
	TMsvId msgEntryId;
	CSLPushMsgEntry* slSave = NULL;

	__LOG_ALWAYS("*****   TEST STORING NEW SL PUSH ENTRY  DATA   *****");  // Log details

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,slSave = DoSlSaveTestL());		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSlSaveTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	slSave = DoSlSaveTestL(); 
#endif

	CleanupStack::PushL(slSave);
	msgEntryId = slSave->Entry().Id();
	OutputMessageDetailsL(*slSave);

	CSLPushMsgEntry* slLoad = NULL;
#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,slLoad = DoSlRetrieveTestL(msgEntryId));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoSlRetrieveTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else 
	slLoad = DoSlRetrieveTestL(msgEntryId); 
#endif

	CleanupStack::PushL(slLoad);
	__LOG_ALWAYS("*****  TEST RESTORING DATA INTO NEW PUSH ENTRY  *****");
	OutputMessageDetailsL(*slLoad);
	CleanupStack::PopAndDestroy(2);  //slLoad, slSave

	if (iDetails->iNullEntries)
		iSLNullMsg = msgEntryId;
	else
		iSLToChange = msgEntryId;  //Hold the id of the non null entry for changing later
	__LOG_ALWAYS("***** SL  SAVE/RETRIEVE -  TEST SUCCESSFUL  ******");
	}

CSLPushMsgEntry* CPushEntryTester::DoSlSaveTestL()
	{
	CSLPushMsgEntry* slSave = CSLPushMsgEntry::NewL();
	CleanupStack::PushL(slSave);
	slSave->SetHeaderL(*iDetails->iHeader);
	slSave->SetUrlL(*iDetails->iURL);
	slSave->SetFromL(*iDetails->iFrom); 
	slSave->SetTimeSent(iDetails->iCreated);
	slSave->SetAction(iDetails->iActionFlag);
	slSave->SetStatus(iDetails->iStatusFlag);
	if (iPushMsgFolderId==KMsvNullIndexEntryId)
		iSISLPushUtilities->GetPushMsgFolderIdL(iPushMsgFolderId);
	slSave->SaveL(iPushUtilities->Session(), iPushMsgFolderId);
	CleanupStack::Pop();
	return slSave;
	}

CSLPushMsgEntry* CPushEntryTester::DoSlRetrieveTestL(TMsvId aId)
	{
	CSLPushMsgEntry* slLoad = CSLPushMsgEntry::NewL();
	CleanupStack::PushL(slLoad);  //slEntry
	slLoad->RetrieveL(iPushUtilities->Session(), aId);
	CleanupStack::Pop();
	return slLoad;
	}


// Tests use of get/set methods on a CMultiPushMsgEntry
// Create new entry,set values on TMsvEntry,set Push Entry, display & set new values
void CPushEntryTester::MultiGetSetTestL()
	{
	CMultiPartPushMsgEntry* multiEntry = NULL;
	__LOG_ALWAYS("***** TESTING MULTIPART SETTER FUNCTIONS *****");	
#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,multiEntry= DoMultiCreateTestL());		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoMultiCreateTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	multiEntry = DoMultiCreateTestL();;
#endif
	CleanupStack::PushL(multiEntry);
	OutputMessageDetailsL(*multiEntry);	

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoMultiSetTestL(*multiEntry) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoMultiSetTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	DoMultiSetTestL(*multiEntry); 
#endif
	OutputMessageDetailsL(*multiEntry);
	CleanupStack::PopAndDestroy(); //multiEntry
	__LOG_ALWAYS("*****   MULTIPART SET/GET -  TEST SUCCESSFUL  ******");
	}


CMultiPartPushMsgEntry* CPushEntryTester::DoMultiCreateTestL()
	{
	TMsvEntry entry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm.iUid = 0x11111111; //Silly value
	entry.iBioType  = 0x44444444;  //another silly value 
	entry.iType = KUidMsvMessageEntry;
	entry.SetMtmData1(0x1FF);

	CMultiPartPushMsgEntry* multiEntry = CMultiPartPushMsgEntry::NewL();
	CleanupStack::PushL(multiEntry);
	multiEntry->SetEntry(entry);
	multiEntry->SetFromL(_L8("Eliza Dolittle"));
	multiEntry->SetContentTypeL(KMultiContent);
	multiEntry->SetStatus(6);
	multiEntry->SetTimeSent(KDateExpiry3);
	multiEntry->SetMsgDescriptionL(*iDetails->iText);

	CleanupStack::Pop();
	return multiEntry;
	}

void CPushEntryTester::DoMultiSetTestL(CMultiPartPushMsgEntry& aMultiEntry)
	{
	aMultiEntry.SetFromL(*iDetails->iFrom);
	aMultiEntry.SetHeaderL(iDummyHeader1);
	aMultiEntry.SetStatus(iDetails->iStatusFlag); 
	aMultiEntry.SetContentTypeL(*iDetails->iContentType);
	aMultiEntry.SetTimeSent(iDetails->iCreated);
	}

void CPushEntryTester::MultiSaveRetrieveTestL()
	{
	TMsvId msgEntryId;
	CMultiPartPushMsgEntry*  multiSave = NULL;

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,multiSave = DoMultiSaveTestL() );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoMultiSaveTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	multiSave = DoMultiSaveTestL(); 
#endif
	CleanupStack::PushL(multiSave);
	__LOG_ALWAYS("*****   TEST STORING NEW MULTI PUSH ENTRY  DATA   *****");  // Log details
	msgEntryId = multiSave->Entry().Id();
	CreateMultiChildrenL(msgEntryId);
	OutputMessageDetailsL(*multiSave);

	__LOG_ALWAYS("*****  TESTING RESTORING DATA INTO NEW PUSH ENTRY  *****");
	CMultiPartPushMsgEntry* multiLoad = NULL;

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, multiLoad = DoMultiRetrieveTestL(msgEntryId) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoMultiRetrieveTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	multiLoad = DoMultiRetrieveTestL(msgEntryId); 
#endif

	CleanupStack::PushL(multiLoad);
	OutputMessageDetailsL(*multiLoad);
	
	// Get the child entries - don't bother with heap testing - tested with Unknown Entries
	iMsvEntry->SetEntryL(msgEntryId);
	CMsvEntrySelection* children = iMsvEntry->ChildrenL();
	CleanupStack::PushL(children);
	CUnknownPushMsgEntry* unknownMsg = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(unknownMsg);

	for (TInt loop=0; loop<children->Count(); loop++)
		{
		__LOG_DEBUG("*****   Details of MultiPart Child entry    *****");
		unknownMsg->RetrieveL(iPushUtilities->Session(), children->At(loop));
		OutputMessageDetailsL(*unknownMsg);		
		}

	CleanupStack::PopAndDestroy(4); //unknownMsg, children, multiLoad, multiSave
	__LOG_ALWAYS("***** MULTIPART  SAVE/RETRIEVE -  TEST SUCCESSFUL  ******");
	}


CMultiPartPushMsgEntry* CPushEntryTester::DoMultiSaveTestL()
	{
	CMultiPartPushMsgEntry*  multiSave = CMultiPartPushMsgEntry::NewL();
	CleanupStack::PushL(multiSave);
	multiSave ->SetHeaderL(*iDetails->iHeader);
	multiSave ->SetStatus(iDetails->iStatusFlag);
	if (iDetails->iNullEntries)
		multiSave ->SetContentTypeL(*iDetails->iContentType);
	else
		multiSave->SetContentTypeL(_L("multipart/related"));	
	multiSave ->SetFromL(*iDetails->iFrom); 
	multiSave ->SetTimeSent(iDetails->iCreated);
	multiSave->SaveL(iPushUtilities->Session(),KMsvGlobalInBoxIndexEntryId);
	CleanupStack::Pop();
	return multiSave;
	}

CMultiPartPushMsgEntry*  CPushEntryTester::DoMultiRetrieveTestL(TMsvId aId)
	{
	CMultiPartPushMsgEntry* multiLoad = CMultiPartPushMsgEntry::NewL();
	CleanupStack::PushL(multiLoad);
	multiLoad->RetrieveL(iPushUtilities->Session(), aId);
	CleanupStack::Pop();
	return multiLoad;
	}

// Tests use of get/set methods on a CSLUnknownMsgEntry
// Create new entry,set values on TMsvEntry,set Push Entry, display & set new values
void CPushEntryTester::UnknownGetSetTestL()
	{
	__LOG_ALWAYS("***** TESTING UNKNOWN SETTER FUNCTIONS *****");	
	CUnknownPushMsgEntry* unknownEntry= NULL;

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,unknownEntry = DoUnknownCreateTestL() );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoUnknownCreateTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	unknownEntry = DoUnknownCreateTestL(); 
#endif

	CleanupStack::PushL(unknownEntry);
	OutputMessageDetailsL(*unknownEntry);

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoUnknownSetTestL(*unknownEntry) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoUnknownSetTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	DoUnknownSetTestL(*unknownEntry); 
#endif
	OutputMessageDetailsL(*unknownEntry);

	__LOG_ALWAYS("***** UNKNOWN  SET/GET  - TEST SUCCESSFUL  ******");
	CleanupStack::PopAndDestroy();
	}

CUnknownPushMsgEntry* CPushEntryTester::DoUnknownCreateTestL()
	{
	TMsvEntry entry;

	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm.iUid = 0x11111111; //Silly value
	entry.iType = KUidMsvMessageEntry;
	entry.SetMtmData1(0x1FF);  //511

	CUnknownPushMsgEntry* unknownEntry = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(unknownEntry);	unknownEntry->SetEntry(entry);
	unknownEntry->SetContentTypeL(_L("multipart/unrelated"));
	unknownEntry->SetStatus(3);
	unknownEntry->SetTimeSent(KDateExpiry4);
	unknownEntry->SetFromL(_L8("Henry Higgins"));
	unknownEntry->SetHeaderL(iDummyHeader2);
	unknownEntry->SetMessageDataL(_L8("A long long piece of text for the message body..........."));
	unknownEntry->SetTimeSent(iDetails->iCreated);
	unknownEntry->SetMsgDescriptionL(*iDetails->iText);
	unknownEntry->SetContentFileNameL(KFilename);	
	
	CleanupStack::Pop();
	return unknownEntry;
	}


void CPushEntryTester::DoUnknownSetTestL(CUnknownPushMsgEntry& aUnknownEntry)
	{
	aUnknownEntry.SetStatus(iDetails->iStatusFlag);  
	aUnknownEntry.SetContentTypeL(*iDetails->iContentType);
	aUnknownEntry.SetStatus(iDetails->iStatusFlag);
	aUnknownEntry.SetFromL(*iDetails->iFrom);
	aUnknownEntry.SetHeaderL(*iDetails->iHeader);
	aUnknownEntry.SetTimeSent(iDetails->iCreated);
	aUnknownEntry.SetMessageDataL(*iDetails->iMessageBody);
	aUnknownEntry.SetContentFileNameL(*iDetails->iFilename);
	}

void CPushEntryTester::UnknownSaveRetrieveTestL(TBool aFilename)
	{
	TMsvId msgEntryId;
	CUnknownPushMsgEntry* unknownSave= NULL;
	CUnknownPushMsgEntry* unknownLoad = NULL;

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, unknownSave = DoUnknownSaveTestL(aFilename) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoUnknownSaveTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	unknownSave = DoUnknownSaveTestL(aFilename); 
#endif
	CleanupStack::PushL(unknownSave);
	msgEntryId = unknownSave->Entry().Id();

	__LOG_DEBUG("*****    STORING NEW UNKNOWN PUSH ENTRY  DATA   *****");
	OutputMessageDetailsL(*unknownSave);

#if defined (__HEAP_FAILURE_TESTING)
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,unknownLoad = DoUnknownRetrieveTestL(msgEntryId) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoUnknownRetriveTestL()()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#else 
	unknownLoad = DoUnknownRetrieveTestL(msgEntryId); 
#endif
	CleanupStack::PushL(unknownLoad);

	__LOG_DEBUG("*****  TESTING RESTORING DATA INTO NEW PUSH ENTRY  *****");

	if (!iDetails->iNullEntries)
		iUnknownToChange = msgEntryId;
	OutputMessageDetailsL(*unknownLoad);
	CleanupStack::PopAndDestroy(2);// unknownLoad,unknownSave
	__LOG_ALWAYS("***** UNKNOWN  SAVE/RETRIEVE - TEST SUCCESSFUL  ******");
	}


CUnknownPushMsgEntry* CPushEntryTester::DoUnknownSaveTestL(TBool aFilename)
	{
	CUnknownPushMsgEntry* unknownSave = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(unknownSave);
	unknownSave->SetHeaderL(*iDetails->iHeader);
	unknownSave->SetContentTypeL(*iDetails->iContentType);
	if (aFilename)
		unknownSave->SetContentFileNameL(*iDetails->iFilename);
	unknownSave->SetMessageDataL(*iDetails->iMessageBody);
	unknownSave->SetStatus(iDetails->iStatusFlag);
	unknownSave->SetFromL(*iDetails->iFrom); 
	unknownSave->SetTimeSent(iDetails->iCreated);
	
	unknownSave->SaveL(iPushUtilities->Session(), KMsvGlobalInBoxIndexEntryId);
	CleanupStack::Pop();
	return unknownSave;
	}

CUnknownPushMsgEntry* CPushEntryTester::DoUnknownRetrieveTestL(TMsvId aId)
	{
	CUnknownPushMsgEntry* unknownLoad = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(unknownLoad);
	unknownLoad->RetrieveL(iPushUtilities->Session(), aId);
	CleanupStack::Pop();
	return unknownLoad;
	}

void CPushEntryTester::ChangeSIEntryTestL()
	{
	// Retrieve has already been tested previously 
	CSIPushMsgEntry* siMsg = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siMsg);
	siMsg->RetrieveL(iPushUtilities->Session(), iSIToChange);
	__LOG_DEBUG("*****    ORIGINAL SI ENTRY     *****");
	OutputMessageDetailsL(*siMsg);	

#if defined (__HEAP_FAILURE_TESTING)
//  Uncomment the following lines & check CommitL in UpdateL if you want to test 
//  if it leaves the message in an partially changed state.
//	for (TInt loop = 0; loop<100; loop++)
//		{
		TInt error = KErrNone;
		TBool finished = EFalse;
		TInt failCount = 0;

		while(!finished)
			{
			__UHEAP_FAILNEXT(failCount++);
			TRAP(error,DoChangeSiEntryL(*siMsg) );		
			if (error == KErrNone)
				{
				__UHEAP_RESET;
				LogFormatInt(_L("Made %d attempts at calling DoChangeSiEntryL()"), failCount);
				finished = ETrue;
				}
			else  // Handle error
				{
				gTest(error == KErrNoMemory);// Check if error is out of memory
				__UHEAP_RESET;
				}
			}
//		}

#ifndef _DEBUG
	failCount =1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
//	DoSiChangeEntryL(*siMsg); 
#endif

	__LOG_DEBUG("*****    UPDATED SI ENTRY     *****");
	OutputMessageDetailsL(*siMsg);
	__LOG_ALWAYS("*****    CHANGING SI ENTRY - TEST SUCCESSFUL     *****");
	CleanupStack::PopAndDestroy();


}


void CPushEntryTester::DoChangeSiEntryL(CSIPushMsgEntry& aSiMsg)
	{
	aSiMsg.SetCreated(KDateExpiry4);
	aSiMsg.SetExpires(KDateExpiry4);
	aSiMsg.SetIdL(KSiID6);
	aSiMsg.SetUrlL(KUrl2);
	aSiMsg.SetHeaderL(iDummyHeader2);
	aSiMsg.SetFromL(KFrom);
	aSiMsg.SetAction(4); // Undefined value to test it
	aSiMsg.SetStatus(4);
	aSiMsg.SetTextL(KUrl5);
	aSiMsg.UpdateL(iPushUtilities->Session());
	}


void CPushEntryTester::ChangeSLEntryTestL()
	{
	CSLPushMsgEntry* slMsg = CSLPushMsgEntry::NewL();
	CleanupStack::PushL(slMsg);

	slMsg->RetrieveL(iPushUtilities->Session(), iSLToChange);
	__LOG_ALWAYS("*****    RETRIEVING ORIGINAL SL ENTRY     *****");
	OutputMessageDetailsL(*slMsg);

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoChangeSlEntryL(*slMsg) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoChangeSlEntryL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
	failCount =1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
//	DoSlChangeEntryL(*slMsg); 
#endif
	__LOG_ALWAYS("*****    UPDATED SL ENTRY     *****");
	OutputMessageDetailsL(*slMsg);
	__LOG_ALWAYS("*****    CHANGING SL ENTRY - TEST SUCCESSFUL     *****");
	CleanupStack::PopAndDestroy();
	}


void CPushEntryTester::DoChangeSlEntryL(CSLPushMsgEntry& aSlMsg)
	{
	aSlMsg.SetHeaderL(iDummyHeader2);
	aSlMsg.SetAction(3); // Undefined value to test it
	aSlMsg.SetStatus(4);
	aSlMsg.SetUrlL(KUrl6);
	aSlMsg.SetTimeSent(KDateExpiry3);
	aSlMsg.UpdateL(iPushUtilities->Session());
	}


void CPushEntryTester::ChangeUnknownEntryTestL(TBool aFilename)
	{
	CUnknownPushMsgEntry* unkMsg = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(unkMsg);

	unkMsg->RetrieveL(iPushUtilities->Session(), iUnknownToChange);
	__LOG_ALWAYS("*****    RETRIEVING ORIGINAL SL ENTRY     *****");
	OutputMessageDetailsL(*unkMsg);

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,DoChangeUnknownEntryL(*unkMsg, aFilename) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoChangeUnknownEntryL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
	failCount =1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
	DoChangeUnknownEntryL(*unkMsg, aFilename); 
#endif
	__LOG_ALWAYS("*****    UPDATED UNKNOWN ENTRY     *****");
	OutputMessageDetailsL(*unkMsg);
	__LOG_ALWAYS("*****    CHANGING UNKNOWN ENTRY - TEST SUCCESSFUL     *****");
	CleanupStack::PopAndDestroy();
	}


void CPushEntryTester::DoChangeUnknownEntryL(CUnknownPushMsgEntry& aUnkMsg, TBool aFilename)
	{
	aUnkMsg.SetHeaderL(iDummyHeader2);
	aUnkMsg.SetStatus(4);
	aUnkMsg.SetTimeSent(KDateExpiry3);
	if (aFilename)
		aUnkMsg.SetContentFileNameL(KFilename2);
	aUnkMsg.SetMessageDataL(KDummyBody2);
	aUnkMsg.UpdateL(iPushUtilities->Session());
	}



// Harded coded target Id string - we store the TMsvId during  CreateMoreSiMessages.
void CPushEntryTester::FindSiIdTestL(TBool aSearchForNullStr)
	{
	__LOG_ALWAYS("*****    TESTING FINDING SI ID     *****");

	TPtrC ptr;
	TBuf<255> buffer;
	if (aSearchForNullStr)
		ptr.Set(KNullDesC);
	else
		ptr.Set(KSiID3);

	CMsvEntrySelection* msgSel = NULL;
	

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,
			msgSel = iSISLPushUtilities->FindSiIdLC(ptr);
		CleanupStack::Pop();
			);		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling msgSel = WapPushMsgUtils::FindSiIdLC(ptr)"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
	failCount =1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
	msgSel = iSISLPushUtilities->FindSiIdLC(ptr); 
	CleanupStack::Pop(); // make the cleanup calls match Heap Testing
#endif
	CleanupStack::PushL(msgSel);

	buffer.Format(_L("Search for  '%S' yielded %d  msg "), &ptr, msgSel->Count());
	LogStr(buffer);

	for (TInt count =0; count <msgSel->Count(); count++)
		{
		TBuf<255> buffer;
		buffer.Format(_L("Search for  '%S' yielded msg %d with Id 0x%08X"), &ptr, count, msgSel->At(count));
//		__LOG_DEBUG(buffer);
		}

	// Normally Should only be one entry in array and that should be equal to iSINullMsg
	// With Heap Failure testing switched on, get lots of partially complete entries.
	if (    (aSearchForNullStr && (msgSel->Find(iSINullMsg)!=KErrNotFound) ) 
		 || (!aSearchForNullStr && (msgSel->Find(iTargetSiIDMsg)!=KErrNotFound) )  )

		{
		__LOG_ALWAYS("*****    FIND SI ID  -  TEST SUCCESSFUL     *****");
		}
	else 
		{
		iFailures++;
		__LOG_ALWAYS("*****    FIND SI ID  -  TEST FAILED      *****");
		}
	CleanupStack::PopAndDestroy(); //msgSel
	}


void CPushEntryTester::FindUrlTestL(TBool aSearchForNullStr, TUid aUid)
	{
	TPtrC ptr;
	TBuf<255> buffer;
	TMsvId targetId =KMsvNullIndexEntryId;
	TPtrC displayPtr;

	if (aUid ==KUidWapPushMsgSI)
		{
		displayPtr.Set(_L("SI "));		
		targetId = aSearchForNullStr?iSINullMsg:iTargetSIUrlMsg;
		ptr.Set(aSearchForNullStr ? TPtrC(KNullDesC) : TPtrC(KUrl3) );
		}
	else if (aUid ==KUidWapPushMsgSL)
		{
		displayPtr.Set(_L("SL "));		
		targetId = aSearchForNullStr ? iSLNullMsg:iTargetSLUrlMsg;
		ptr.Set(aSearchForNullStr ? TPtrC(KNullDesC):TPtrC(KUrl3) );
		}

	LogFormatStr(_L("*****    TESTING FINDING A %S MSG URL     *****"), displayPtr);

	TMsvId msgId = KMsvNullIndexEntryId; 	
#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,msgId = iSISLPushUtilities->FindUrlL(ptr, aUid) );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling msgSel = WapPushMsgUtils::FindSiIdLC()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
		failCount =1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
	msgId = iSISLPushUtilities->FindUrlL(ptr, aUid); 
#endif

	buffer.Format(_L("Searched messages of type 0x%08X with Url '%S'"),KUidWapPushMsgSI.iUid, &ptr);
	LogStr(buffer);
	LogFormatInt(_L("Find operation yielded Id 0x%08X"),  msgId);
	if ( msgId == targetId) 
		{
		LogFormatStr(_L("*****    FIND %S URL  -  TEST SUCCESSFUL     *****"), displayPtr);	
		}
	else
		{
		iFailures++;
		LogFormatStr(_L("*****    FIND %S URL  -  TEST FAILED         *****"), displayPtr);	
		}
	}


void CPushEntryTester::GetActionTestL(TUid aPushType)
	{
	TPtrC ptr;
	TMsvId targetId;
	TMsvId resultId = KMsvNullIndexEntryId;

	if (aPushType ==KUidWapPushMsgSI)
		{
		ptr.Set(_L("SI"));
		targetId = iHighestSIActionID;
		}
	else
		{
		ptr.Set(_L("SL"));
		targetId = iHighestSLActionID;
		}
	LogFormatStr(_L("*****    TESTING FINDING HIGHEST %S ACTION     *****"), ptr);


#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, resultId = iSISLPushUtilities->GetHighestActionL(aPushType));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling msgSel = WapPushMsgUtils::GetHighestActionL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}
#ifndef _DEBUG
		failCount = 1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else // if no Heap Failure testing
	resultId = iSISLPushUtilities->GetHighestActionL(aPushType); 
#endif


	LogFormatInt(_L("Get Action operation for Msg Type 0x%08X, yielded Id 0x%08X"), aPushType.iUid, resultId);
	
	if (targetId ==resultId)
		{
		LogFormatStr(_L("*****    FIND HIGHEST %S ACTION - TEST SUCCESSFUL    *****"), ptr);
		}
	else
		{
		iFailures++;
		LogFormatStr(_L("*****    FIND HIGHEST %S ACTION - TEST FAILED        *****"), ptr);
		}
	}

void CPushEntryTester::GetExpiryTestL()
	{
	__LOG_ALWAYS("*****    TESTING FINDING EARLIEST EXPIRY     *****");

#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, iExpired = iSISLPushUtilities->GetNextExpiryL() );		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling msgSel = WapPushMsgUtils::GetNextExpiryL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
	failCount = 1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
	iExpired = iSISLPushUtilities->GetNextExpiryL(); 
#endif
	
	CSIPushMsgEntry* siMsg = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siMsg);
	if (iExpired != KMsvNullIndexEntryId) //Otherwise Retrieve panics!!
		{
		siMsg->RetrieveL(iPushUtilities->Session(), iExpired); 
		OutputMessageDetailsL(*siMsg);
		TBuf<100> temp;
		TTime expiryDate = siMsg->Expires().Int64();
		expiryDate.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
		TPtrC ptr;
		ptr.Set(temp);
		LogFormatStr(_L("Next Expirys date  :=  %S"), ptr);
		__LOG_ALWAYS("*****    FIND NEXT EXPIRY - TEST SUCCESSFUL        *****");

		}
	else
		{
		__LOG_ALWAYS("*****    FIND NEXT EXPIRY - TEST FAILED         *****");
		}
	CleanupStack::PopAndDestroy();  //siMsg
	}


void CPushEntryTester::DeleteEarliestExpiryL()
	{
#if defined (__HEAP_FAILURE_TESTING)
	TInt error = KErrNone;
	TBool finished = EFalse;
	TInt failCount = 0;
		while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, iSISLPushUtilities->DeleteEntryL(iExpired));		
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling msgSel = WapPushMsgUtils::DeleteEntryL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			gTest(error == KErrNoMemory);// Check if error is out of memory
			__UHEAP_RESET;
			}
		}

#ifndef _DEBUG
	failCount = 1;  // Bodge to stop compiler complaining on Non Debug builds
#endif

#else 
	iSISLPushUtilities->DeleteEntryL(iExpired);
#endif
	CMsvEntry* msvEntry = iPushUtilities->Session().GetEntryL(iExpired);
	CleanupStack::PushL(msvEntry);
	TMsvEntry entry = msvEntry->Entry();
	if (CWapPushMsgUtils::IsDeleted(entry))
		{
		LogFormatInt(_L("Msg 0x%08X has been flagged for deletion"), iExpired);
		}
	else
		{
		LogFormatInt(_L("Msg 0x%08X has not been flagged for deletion"), iExpired);
		}

	CleanupStack::PopAndDestroy();//msvEntry
	}



// Use if you want to see the contents of ALL the messages for a particular type
void CPushEntryTester::DumpPushMessageDetailsL(TUid aPushType)
	{
	TPtrC ptr;
	TBuf<255> buffer;
	if (aPushType== KUidWapPushMsgSI)
		ptr.Set(_L("SI"));
	else if (aPushType ==KUidWapPushMsgSL)
		ptr.Set(_L("SL"));
	else if (aPushType == KUidWapPushMsgMultiPart)
		ptr.Set(_L("MULTI"));
	else if (aPushType == KUidWapPushMsgUnknown)
		ptr.Set(_L("UNKNOWN"));
	else 
		ptr.Set(_L("Unrecognised type"));

	LogFormatStr(_L("*****    Displaying details of all %S messages         *****"),ptr);

	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(sort);

	if (iPushMsgFolderId ==KMsvNullIndexEntryId) // Shouldn't be
		iSISLPushUtilities->GetPushMsgFolderIdL(iPushMsgFolderId);

	iMsvEntry->SetEntryL(iPushMsgFolderId);
	CMsvEntrySelection* children;
	children = iMsvEntry->ChildrenWithMtmL(KUidMtmWapPush);
	CleanupStack::PushL(children);

	CMsvEntrySelection* pushSel = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(pushSel);

	for (TInt count = 0; count <children->Count(); count++)
		{
		iMsvEntry->SetEntryL(children->At(count));
		if (iMsvEntry->Entry().iBioType == aPushType.iUid)
			pushSel->AppendL(children->At(count));		
		}

	buffer.Format(_L("*****    Have %d %S messages in folder   ******"), pushSel->Count(), &ptr); 
	LogStr(buffer);
	for (TInt out=0; out< pushSel->Count(); out++)
		{
		if (pushSel->At(out) != KMsvNullIndexEntryId)
			{
			TBool streamExists=EFalse;
			iMsvEntry->SetEntryL(pushSel->At(out));

			LogFormatInt(_L("Details for message %d"), out+1);
			if (iMsvEntry->HasStoreL())
				{
				CMsvStore* store = iMsvEntry->ReadStoreL();
				CleanupStack::PushL(store);
				if (store->IsPresentL(aPushType) )
					streamExists =ETrue;
				CleanupStack::PopAndDestroy();
				}
			if (streamExists)
				{
				if (aPushType == KUidWapPushMsgSI)
					{
					CSIPushMsgEntry* siMsg = CSIPushMsgEntry::NewL();
					CleanupStack::PushL(siMsg);
					siMsg->RetrieveL(iPushUtilities->Session(), pushSel->At(out));
					OutputMessageDetailsL(*siMsg);
					CleanupStack::PopAndDestroy();
					}
				else if (aPushType == KUidWapPushMsgSL)
					{
					CSLPushMsgEntry* slMsg = CSLPushMsgEntry::NewL();
					CleanupStack::PushL(slMsg);
					slMsg->RetrieveL(iPushUtilities->Session(), pushSel->At(out));
					OutputMessageDetailsL(*slMsg);
					CleanupStack::PopAndDestroy();
					}
				else if (aPushType == KUidWapPushMsgMultiPart)
					{
					CMultiPartPushMsgEntry* multiMsg = CMultiPartPushMsgEntry::NewL();
					CleanupStack::PushL(multiMsg);
					multiMsg->RetrieveL(iPushUtilities->Session(), pushSel->At(out));
					OutputMessageDetailsL(*multiMsg);
					CleanupStack::PopAndDestroy();
					}
				else if (aPushType == KUidWapPushMsgUnknown)
					{
					CUnknownPushMsgEntry* unknownMsg = CUnknownPushMsgEntry::NewL();
					CleanupStack::PushL(unknownMsg);
					unknownMsg->RetrieveL(iPushUtilities->Session(), pushSel->At(out));
					OutputMessageDetailsL(*unknownMsg);
					CleanupStack::PopAndDestroy();
					}

				}
			else
				{
				__LOG_DEBUG("No stream found!.  Not a proper a Push Message.");
				OutputTMsvEntryDetailsL(iMsvEntry->Entry());
				buffer.Format(_L("%S Msg 0x%08X Stream with %S Uid not found"),ptr, iMsvEntry->Entry().Id(), ptr);
				}
			}
		}
	CleanupStack::PopAndDestroy(2); //siSel, children
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	}



void CPushEntryTester::CreateHeaders()
	{
	TUint8 value=0;
	TInt i;
	for (i=0; i<34; i++)
		{
		value = KMessageHeader2[i];
		iDummyHeader1.Append(value);
		}

	for (i=0; i<9; i++)
		{
		value = KMessageHeader3[i];
		iDummyHeader2.Append(value);
		}
	}




void CPushEntryTester::CreateMsgDetailsL(TBool aNullFields)
	{
	iDetails->ResetL();
	if (aNullFields) // ==ETrue
		{
		iDetails->iNullEntries = ETrue;
		return;
		}
	iDetails->iNullEntries =EFalse;
	*iDetails->iSiId = KSiID1;
	iDetails->iStatusFlag = 4;  // Not a valid value, but tests that it's set okay
	iDetails->iActionFlag = 1; //  CSIPushMsgEntry::ESIPushMsgSignalNone & CSLPushMsgEntry::ESLPushMsgExecuteLow;
	//iCreated leave as is  00.00.00 GMT 00-00-0000
	iDetails->iCreated=KDateExpiry1;
	iDetails->iExpires=KDateExpiry1; // 12.00.00 GMT 26-07-2000
	*iDetails->iContentType =_L("image/jpeg");
	*iDetails->iURL=KUrl1;
	*iDetails->iText=_L("Some Text");
	*iDetails->iFrom =_L8("Fred Smith");
	*iDetails->iHeader =iDummyHeader1;
	*iDetails->iMessageBody=KDummyBody;
	*iDetails->iFilename = KFilename;	
	}

// Create another 4 SI Push Entries in the message folder.
void CPushEntryTester::CreateMoreSiMessagesL()
	{
	TMsvId msgEntryId =0;
	TBuf<255> buffer;

	CreateMsgDetailsL(EFalse);
	if (iPushMsgFolderId ==KMsvNullIndexEntryId)
		iSISLPushUtilities->GetPushMsgFolderIdL(iPushMsgFolderId);
	__LOG_DEBUG("Created additional SI Entries");
	for (TInt count =0; count<4; count++)
		{
		CSIPushMsgEntry* siMsg = CSIPushMsgEntry::NewL();
		CleanupStack::PushL(siMsg);

		siMsg->SetHeaderL(*iDetails->iHeader);
		siMsg->SetStatus(iDetails->iStatusFlag);
		siMsg->SetCreated(iDetails->iCreated);
		siMsg->SetTextL(*iDetails->iText);
		siMsg->SetFromL(*iDetails->iFrom); 
		switch (count)
			{
			case 0:
				siMsg->SetIdL(KSiID2);	
				siMsg->SetUrlL(KUrl2);
				siMsg->SetExpires(KDateExpiry2);
				break;
			case 1:
				siMsg->SetIdL(KSiID3);
				siMsg->SetUrlL(KUrl3);
				siMsg->SetExpires(KDateExpiry3);
				break;
			case 2:
				siMsg->SetIdL(KSiID4);	
				siMsg->SetUrlL(KUrl4);
				siMsg->SetExpires(KDateExpiry4);
				break;
			case 3:
				siMsg->SetIdL(KSiID5);	
				siMsg->SetUrlL(KUrl5);
				siMsg->SetExpires(KDateExpiry5);
				break;
			}
		siMsg->SetAction(count+2);
		msgEntryId = siMsg->SaveL(iPushUtilities->Session(),iPushMsgFolderId);
		TTime expiryDate = siMsg->Expires().Int64();
		expiryDate.FormatL(buffer, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
		LogFormatStr(_L("Expires date  :=  %S"), buffer);


		buffer.Format(_L("Msg 0x%08X with  ID=%S,  URL=%S,  Action=%d"),
						msgEntryId, &(siMsg->Id()), &(siMsg->Url()),count+2);
		LogStr(buffer);  

		if (count == 1)
			{
			iTargetSiIDMsg = msgEntryId;
			iTargetSIUrlMsg = msgEntryId;
			}
		CleanupStack::PopAndDestroy();
		}
	iHighestSIActionID = msgEntryId;  //Hold on to this for later check
	}


void  CPushEntryTester::CreateMoreSLMessagesL()
	{
	TMsvId msgEntryId =0;
	TBuf<255> buffer;

	CreateMsgDetailsL(EFalse);
	if (iPushMsgFolderId ==KMsvNullIndexEntryId)
		iSISLPushUtilities->GetPushMsgFolderIdL(iPushMsgFolderId);
	
	for (TInt count =0; count<4; count++)
		{
		CSLPushMsgEntry* slMsg = CSLPushMsgEntry::NewL();
		CleanupStack::PushL(slMsg);

		slMsg->SetHeaderL(*iDetails->iHeader);
		slMsg->SetStatus(iDetails->iStatusFlag);
		slMsg->SetFromL(*iDetails->iFrom); 
		switch (count)
			{
			case 0:
				slMsg->SetUrlL(KUrl2);	
				break;
			case 1:
				slMsg->SetUrlL(KUrl3);	
				break;
			case 2:
				slMsg->SetUrlL(KUrl4);	
				break;
			case 3:
				slMsg->SetUrlL(KUrl5);	
				break;
			default:
				slMsg->SetUrlL(*iDetails->iURL);

			}
		slMsg->SetAction(count+2);
		msgEntryId = slMsg->SaveL(iPushUtilities->Session(),iPushMsgFolderId);
		buffer.Format(_L("Created SL message 0x%08X with URL = %S, Action = %d"), msgEntryId, &(slMsg->Url()), count+2);
		LogStr(buffer);
		if (count == 1)
			iTargetSLUrlMsg = msgEntryId;
		CleanupStack::PopAndDestroy();//slMsg
		}
	iHighestSLActionID = msgEntryId;  //Hold on to this for later check
	}



void CPushEntryTester::CreateMultiChildrenL(TMsvId aParentEntry)
	{
	// Create and Save SI child entry
	CUnknownPushMsgEntry* child = CUnknownPushMsgEntry::NewL();
	CleanupStack::PushL(child);
	child->SetHeaderL(*iDetails->iHeader);
	child->SetMessageDataL(*iDetails->iMessageBody);
	child->SetContentTypeL(_L("text/vnd.wap.si"));
	child->SetFromL(*iDetails->iFrom); 
	child->SaveL(iPushUtilities->Session(),aParentEntry);
	
	// Create and Save SL child entry
	child->SetHeaderL(TPtrC8(KMessageHeader3));
	child->SetFromL(*iDetails->iFrom); 
	child->SetContentTypeL(_L("text/vnd.wap.sl"));
	child->SetTimeSent(iDetails->iCreated);
	child->SetMessageDataL(_L8("The body of a binary message - needs to be processed"));
	child->SaveL(iPushUtilities->Session(), aParentEntry);

	// Create and Save WML child entry
	child->SetHeaderL(TPtrC8(KMessageHeader3));
	child->SetFromL(*iDetails->iFrom); 
	child->SetContentTypeL(_L("text/vnd.wap.wml"));
	child->SetTimeSent(iDetails->iCreated);
	child->SetMessageDataL(_L8("<wml>\n <card id= \"card1 \" title= \"Crystal Demo \" >\n <wml\\>"));
	child->SaveL(iPushUtilities->Session(), aParentEntry);

	// Create and Save HTML child entry
	child->SetHeaderL(TPtrC8(KMessageHeader3));
	child->SetFromL(*iDetails->iFrom); 
	child->SetContentTypeL(_L("text/html"));
	child->SetTimeSent(iDetails->iCreated);
	child->SetMessageDataL(_L8("<html> Some HTML for a web page <html>"));
	child->SaveL(iPushUtilities->Session(), aParentEntry);

	// Create and Save GIF child entry
	child->SetHeaderL(TPtrC8(KMessageHeader3));
	child->SetFromL(*iDetails->iFrom); 
	child->SetContentTypeL(_L("image/gif"));
	child->SetTimeSent(iDetails->iCreated);
	child->SetMessageDataL(_L8("A binary GIF file qasdghgf92tkug;,b voif0r.,,ASFSrrf"));
	child->SaveL(iPushUtilities->Session(), aParentEntry);

	// Create and Save XML child entry
	child->SetHeaderL(TPtrC8(KMessageHeader3));
	child->SetFromL(*iDetails->iFrom); 
	child->SetContentTypeL(_L("application/xml"));
	child->SetTimeSent(iDetails->iCreated);
	child->SetMessageDataL(_L8("<XML> A little bit of XML <XML\\>"));
	child->SaveL(iPushUtilities->Session(), aParentEntry);

	CleanupStack::PopAndDestroy();
	}



// Prints & Logs the details of a TMsvEntry
void CPushEntryTester::OutputTMsvEntryDetailsL(const TMsvEntry& aEntry)
	{
	TPtrC truePtr;
	TPtrC falsePtr;
	
	truePtr.Set(_L("ETrue"));
	falsePtr.Set(_L("EFalse"));

	LogFormatInt(_L("          iID                      :=  0x%08X"), aEntry.Id());
    LogFormatInt(_L("          Parent ID                :=  0x%08X"), aEntry.Parent());
	LogFormatInt(_L("          iService                 :=  0x%08X"), aEntry.iServiceId);
	LogFormatInt(_L("          iType                    :=  0x%08X"), aEntry.iType.iUid);
	LogFormatInt(_L("          iMtm                     :=  0x%08X"), aEntry.iMtm.iUid);
	LogFormatInt(_L("          iBioType                 :=  0x%08X"), aEntry.iBioType);
	LogFormatInt(_L("          iMtmData1                :=  %d"), aEntry.MtmData1());
	LogFormatInt(_L("          iMtmData2                :=  %d"), aEntry.MtmData2() );
	LogFormatInt(_L("          iMtmData3                :=  %d"), aEntry.MtmData3() );
	}

void CPushEntryTester::OutputPushMsgDetailsL(const CPushMsgEntryBase& aEntry)
	{
	TBuf<100> temp;
	TPtrC ptr;

	TTime date = aEntry.ReceivedDate();
	date.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	ptr.Set(temp);
	LogFormatStr(_L("          Received Date            :=  %S"),ptr);

	ptr.Set(aEntry.MsgDetails());
	LogFormatStr(_L("          iDetails(Url)            :=  %S"),ptr);
	ptr.Set(aEntry.MsgDescription());
	LogFormatStr(_L("          iDescription             :=  %S"),ptr);
	
	TInt intID;
	TBool isAnInt;
	TPtrC8 ptr8;
	if (aEntry.AppID(ptr8, intID, isAnInt)!=KErrNotFound)
		{
		if (isAnInt)
			{
			LogFormatInt(_L("          AppID                    :=  %d"), intID);
			}
		else
			{
			temp.Copy(ptr8);
			LogFormatStr(_L("          AppID                    :=  %S"),temp);
			}
		}
	}

// Outputs the details of a SI Message - lists TMsvEntry first, 
// then uses Push Entry functions - should be the same values where data's mapped to TMsvEntry
void CPushEntryTester::OutputMessageDetailsL(CSIPushMsgEntry& aSIEntry)
	{
	TMsvEntry entry = aSIEntry.Entry();
	TBuf<200> temp;
	TBuf8<200> viewer;
	TPtrC bufPtr;
	
	__LOG_DEBUG( "   TMsvEntry values");
	OutputTMsvEntryDetailsL(entry);
	OutputPushMsgDetailsL(aSIEntry);

	__LOG_DEBUG( "   CSIPushMsgEntry values");
	LogFormatInt(_L("          Message Type             :=  0x%08X"), aSIEntry.Entry().iMtm.iUid);
	LogFormatInt(_L("          Push Message Type        :=  0x%08X"), aSIEntry.PushMsgType());
	LogFormatInt(_L("          Status value             :=  %d"), aSIEntry.Status());
	LogFormatInt(_L("          Action value             :=  %d"), aSIEntry.Action());

	viewer.Copy(aSIEntry.Id());
	bufPtr.Set(aSIEntry.Id());
	LogFormatStr(_L("          SI  Id                   :=  %S"), bufPtr);

	TTime entryTime = aSIEntry.Created();
	temp.Zero();
	entryTime.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	bufPtr.Set(temp);
	LogFormatStr(_L("          Created                  :=  %S"), bufPtr);
	bufPtr.Set(aSIEntry.Url());
	LogFormatStr(_L("          Url                      :=  %S"), bufPtr);
	bufPtr.Set( aSIEntry.Text());
	LogFormatStr(_L("          Text Info                :=  %S"), bufPtr);
	
	entryTime = aSIEntry.Expires();
	entryTime.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	bufPtr.Set(temp);
	LogFormatStr(_L("          Expires                  :=  %S"), bufPtr);

	temp.Copy(aSIEntry.Header());
	bufPtr.Set(temp);
	LogFormatStr(_L("          Header                   :=  %S"), bufPtr);

	temp.Copy(aSIEntry.From());
	bufPtr.Set(temp);
	LogFormatStr(_L("          From                     :=  %S"), bufPtr);
	bufPtr.Set(aSIEntry.ContentType());
	LogFormatStr(_L("          Content Type             :=  %S"), bufPtr);
	}


// Outputs the details of a SL Message - lists TMsvEntry first, 
// then uses Push Entry functions - should be the same values where data's mapped to TMsvEntry
void CPushEntryTester::OutputMessageDetailsL(CSLPushMsgEntry& aSLEntry)
	{
	TMsvEntry entry = aSLEntry.Entry();
	TBuf<200> temp;
	TPtrC bufPtr;

	__LOG_DEBUG( "   TMsvEntry values");
	OutputTMsvEntryDetailsL(entry);
	OutputPushMsgDetailsL(aSLEntry);
	
	__LOG_DEBUG( "   CSLPushMsgEntry values");
	LogFormatInt(_L("          Message Type             :=  0x%08X"), aSLEntry.Entry().iMtm.iUid);
	LogFormatInt(_L("          Push Message Type        :=  0x%08X"), aSLEntry.PushMsgType());
	TTime entryTime = aSLEntry.TimeSent();
	temp.Zero();
	entryTime.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	bufPtr.Set(temp);
	LogFormatStr(_L("          Message Date             :=  %S"), bufPtr);
	LogFormatInt(_L("          Status value             :=  %d"), aSLEntry.Status());
	LogFormatInt(_L("          Action value             :=  %d"), aSLEntry.Action());

	bufPtr.Set(aSLEntry.Url());
	LogFormatStr(_L("          Url                      :=  %S"), bufPtr);

	temp.Copy(aSLEntry.Header());
	bufPtr.Set(temp);
	LogFormatStr(_L("          Header                   :=  %S"), bufPtr);

	temp.Copy(aSLEntry.From());
	bufPtr.Set(temp);
	LogFormatStr(_L("          From                     :=  %S"), bufPtr);
	bufPtr.Set(aSLEntry.ContentType());
	LogFormatStr(_L("          Content Type             :=  %S"), bufPtr);
	}

// Outputs the details of an Unknown Push Message - lists TMsvEntry first, 
// then uses Push Entry functions - should be the same values where data's mapped to TMsvEntry
void CPushEntryTester::OutputMessageDetailsL(CUnknownPushMsgEntry& aUnknownEntry)
	{
	TMsvEntry entry = aUnknownEntry.Entry();
	TBuf<200> temp;
	TPtrC bufPtr;
	
	__LOG_DEBUG("TMsvEntry values");
	OutputTMsvEntryDetailsL(entry);
	OutputPushMsgDetailsL(aUnknownEntry);

	__LOG_DEBUG("CUnknownPushMsgEntry values");
	LogFormatInt(_L("          Message Type             :=  0x%08X"), aUnknownEntry.Entry().iMtm.iUid);
	LogFormatInt(_L("          Push Message Type        :=  0x%08X"), aUnknownEntry.PushMsgType());
	LogFormatInt(_L("          Status value             :=  %d"), aUnknownEntry.Status());

	TTime entryTime = aUnknownEntry.TimeSent();
	temp.Zero();
	entryTime.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	bufPtr.Set(temp);
	LogFormatStr(_L("          Message Date             :=  %S"), bufPtr);

	bufPtr.Set(aUnknownEntry.ContentType());
	LogFormatStr(_L("          Content-Type             :=  %S"), bufPtr );
	bufPtr.Set(aUnknownEntry.ContentFileNameL());
	LogFormatStr(_L("          Content-Filename         :=  %S"), bufPtr );

	temp.Copy(aUnknownEntry.Header());
	bufPtr.Set(temp);
	LogFormatStr(_L("          Header                   :=  %S"), bufPtr);
	temp.Copy(aUnknownEntry.From());
	bufPtr.Set(temp);
	LogFormatStr(_L("          From                     :=  %S"), bufPtr);


	HBufC* buffer = HBufC::NewL(aUnknownEntry.MessageData().Length());
	buffer->Des().Copy(aUnknownEntry.MessageData());
	bufPtr.Set(buffer->Des());
	LogFormatStr(_L("          Message Data             :=  %S"), bufPtr);
	delete buffer;
	}

// Outputs the details of a MultiPart Message - lists TMsvEntry first, 
// then uses Push Entry functions - should be the same values where data's mapped to TMsvEntry
void CPushEntryTester::OutputMessageDetailsL(CMultiPartPushMsgEntry& aMultiEntry)
	{
	TMsvEntry entry = aMultiEntry.Entry();
	TBuf<200> temp;
	TPtrC bufPtr;
	
	__LOG_DEBUG( "   TMsvEntry values");
	OutputTMsvEntryDetailsL(entry);
	OutputPushMsgDetailsL(aMultiEntry);
	
	__LOG_DEBUG( "   CMultiPartPushMsgEntry values");
	LogFormatInt(_L("          Message Type             :=  0x%08X"), aMultiEntry.Entry().iMtm.iUid);
	LogFormatInt(_L("          Push Message Type        :=  0x%08X"), aMultiEntry.PushMsgType());
	LogFormatInt(_L("          Status value             :=  %d"), aMultiEntry.Status());

	TTime entryTime = aMultiEntry.TimeSent();
	temp.Zero();
	entryTime.FormatL(temp, _L("%-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3" )    );
	bufPtr.Set(temp);
	LogFormatStr(_L("          Message Date             :=  %S"), bufPtr);


	bufPtr.Set(aMultiEntry.ContentType());
	LogFormatStr(_L("          Content-Type             :=  %S"), bufPtr);	
	temp.Copy(aMultiEntry.Header());
	bufPtr.Set(temp);
	LogFormatStr(_L("          Header                   :=  %S"), bufPtr);
	temp.Copy(aMultiEntry.From());
	bufPtr.Set(temp);
	LogFormatStr(_L("          From                     :=  %S"), bufPtr);
	}


/** Set up push folder in the message server */
LOCAL_C TMsvId SetupPushServiceL()
	{
	gTest.Printf(_L("Getting Service Id"));
	CWapPushTestMsgUtils* myTestUtils = CWapPushTestMsgUtils::NewL();
	CleanupStack::PushL(myTestUtils);
	// Uncomment this if you want to remove existing service entries
	//myTestUtils->ClearExistingServiceEntriesL(KUidMtmWapPush);

	CMsvEntrySelection* idArray = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(idArray);

	TMsvId pushService =KMsvNullIndexEntryId;
	myTestUtils->PushServiceIdL(pushService,idArray);

	if ((pushService == KMsvNullIndexEntryId) && (idArray->Count() ==0) ) // Found no service
		{
		gTest.Printf(_L("No Service found - creating new one"));
		pushService = myTestUtils->CreateServiceL();
		gTest.Printf(_L("Created New Service Entry Id 0x%08X"), pushService);
		}
	else
		{
		gTest.Printf(_L("Found %d service entries\n"), idArray->Count());
		for (TInt count=0; count < idArray->Count(); count++)
			{
			gTest.Printf(_L("Entry %d  - Service Id 0x%08X"), count, idArray->At(count));
			}
		}
	
	idArray->Reset();

	// Remove the message folder  - test if the Msg Utils create if it's not there.
	// myTestUtils->RemoveAllPushFolderEntriesL();

	TMsvId pushFolder=KMsvNullIndexEntryId;
	myTestUtils->PushFolderIdL(pushFolder, idArray);

	if ((pushFolder == KMsvNullIndexEntryId) && (idArray->Count() ==0) ) // Found no service
		{
		gTest.Printf(_L("No Push Msg Folder found - creating new one"));
		pushFolder = myTestUtils->CreatePushMsgFolderL();
		gTest.Printf(_L("Created New Push Folder Entry Id 0x%08X"), pushFolder);

		}
	else
		{
		gTest.Printf(_L("Found %d Push Folder entries\n"), idArray->Count());
		for (TInt count=0; count < idArray->Count(); count++)
			{
			gTest.Printf(_L("Entry %d  - Folder Id 0x%08X"), count, idArray->At(count));
			}
		}

	//Clean out all previous push entries from under the Push Service Entry
	//myTestUtils->RemoveServiceEntryChildrenL(pushService);


	// Clean the push msg folder of existing entries
	myTestUtils->RemoveEntriesFromLocalServiceFolderL(pushFolder, KUidMtmWapPush);
	myTestUtils->RemoveEntriesFromLocalServiceFolderL(KMsvGlobalInBoxIndexEntryIdValue, KUidMtmWapPush);

	CleanupStack::PopAndDestroy(2);  //myTestUtils, idArray
	return pushFolder;
	}


/** setup test environment, create CPushEntryTester object and run tests */
LOCAL_C void doMainL()
	{
	gTest.Start(KPushEntryTestTitle);
	gTest.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-PUSHMSGENTRY-T_PUSHENTRY-0001 "));	

	CTestScheduler* activeScheduler = new CTestScheduler;
	gTest(activeScheduler!=NULL);
	CActiveScheduler::Install(activeScheduler);
	CleanupStack::PushL(activeScheduler);

	CWapPushLog* log = CWapPushLog::NewL(*gTest.Console());
	log->SetLogFileName(_L("TPushEntry.txt"));
	CleanupStack::PushL(log);
	CPushEntryTester* myTester = CPushEntryTester::NewL(*log);
	CleanupStack::PushL(myTester);
	TMsvId pushFolder = SetupPushServiceL();
	myTester->SetPushFolderId(pushFolder);
	myTester->StartL();
	
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(3); //activeScheduler, log, myTester
	gTest.End();
	gTest.Close();
	}


/** entry point for test executable */
GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;

	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);
	delete theCleanup;	

	__UHEAP_MARKEND;
	return(KErrNone);
	}
