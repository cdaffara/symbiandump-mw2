

// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ctestheadersearchsort.cpp
// 
//

#include "ctestattachmentsearch.h"
#include <msvapi.h>
#include <msvsearchsortquery.h>
#include "msvsearchsortoponheaderbody.h"
#include <msvids.h>
#include <msvuids.h>
#include <mtclreg.h>
#include <mtclbase.h>
#include <mtmdef.h>
#include <txtrich.h>
#include "emailtestutils.h"
#include "msvtestutils.h"
#include <miuthdr.h>
#include <e32base.h>
#include <e32std.h>
#include <msvipc.h>
#include "MCLIENT.H"
#include "MSERVER.H"






#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");


//_LIT(KShortDescription, "1667 testing");



const TUid KUidFindTestSMTPMtm = {0x10001028};


_LIT8(KAttachmentContent, "content!");
#define KMiutRemoteSize 					123456
#define KMaxImHeaderStringLengthLong		KMaxImHeaderStringLength+1


LOCAL_D TBuf<KMaxImHeaderStringLengthLong> longValue;
LOCAL_D TBuf8<KMaxImHeaderStringLengthLong> longValue2;

RTest testattachment(_L("Find Test"));
LOCAL_D CEmailTestUtils* testUtils;

//LOCAL_D TMsvId folderId;
//LOCAL_D TMsvId entry1Id;
//LOCAL_D TMsvId entry2Id;
//LOCAL_D TMsvId entry3Id;
//LOCAL_D TMsvId entry4Id;

//Database
_LIT(KTestDbFileName, "c:\\messaging.db");






LOCAL_C void InitL()
	{
	testUtils = CEmailTestUtils::NewLC(testattachment);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	//testUtils->GoServerSideL();
	
	}
	
LOCAL_C void Closedown()
	{
	testattachment.Close();
	CleanupStack::PopAndDestroy(1);  //testUtils
	
	}



CTestAttachmentSearch::CTestAttachmentSearch()
	{
	}

CTestAttachmentSearch ::~CTestAttachmentSearch()
	{
	
	}


void CTestAttachmentSearch::SetupL()
	{
	_LIT(KSetupL, "SetupL");
	INFO_PRINTF1(KSetupL);
	
	iActiveWaiter = new (ELeave) CActiveWaiter();
	
	TFileName DBfilename(KTestDbFileName);
	
	//Delete if the Db is still existing
	RSqlDatabase::Delete(DBfilename);
	
	//Create , Intialize and open the database
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	iDBadapter = CMsvDBAdapter::NewL();
#else
	iDBadapter = CMsvDBAdapter::NewL(DBfilename);
#endif
	
	//CMsgsTestUtils::InstallMtmL(KFindMtmDataFile, KFindMtmName, KUidFindTestMtm, KFindMtmFilename);
	}

void CTestAttachmentSearch:: CreateNewPlaintextMessageL()
	{
	/*
	// Create new Plaintext message
	*/

	TMsvEmailTypeList msvEmailTypeList = 0;
	TMsvPartList partList = (KMsvMessagePartBody | KMsvMessagePartAttachments);
	CImEmailOperation* emailOperation = CImEmailOperation::CreateNewL(iActiveWaiter->iStatus, *testUtils->iMsvSession, KMsvGlobalOutBoxIndexEntryIdValue, partList, msvEmailTypeList, KUidFindTestSMTPMtm);
	CleanupStack::PushL(emailOperation);

	
	iActiveWaiter->WaitActive();
	
	//check progress
	TMsvId temp;
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf = emailOperation->ProgressL();
	paramPack.Set(progBuf);
	newMessageId = paramPack();
	if (newMessageId != NULL)
	{
		_LIT(KStr, "\t Created New plaintext message");
		INFO_PRINTF1(KStr);
	}
	iSelectionArray.Append(newMessageId)	;

	CleanupStack::PopAndDestroy(1); // emailOperation, testMsvOperationTimer
	}

void CTestAttachmentSearch::TestAddAttachmentAsLinkL()
	{
	CMsvEntry* cEntry = CreateTestEntryLC();		
		
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	HBufC* paramFilePath=HBufC::NewLC(20);
	
	(paramFilePath->Des()).Format(_L("c:\\msgtest\\1.txt"));
	
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvLinkedFile);
	CleanupStack::PushL(attachment);
	
		
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	attachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	manager.AddLinkedAttachmentL(*paramFilePath, attachment, iActiveWaiter->iStatus);
	 // ownership passed to manager

	iActiveWaiter->WaitActive();
	CleanupStack::Pop(attachment);
	TMsvAttachmentId attachmentId = attachment->Id();
	
	_LIT(KFormatId, "TMsvId %D");
	INFO_PRINTF1(_L("Added Attachments wit id"));
	INFO_PRINTF2(KFormatId,attachmentId);
	
	store->CommitL();
	CleanupStack::PopAndDestroy(3);
	
	}

void CreateAttachmentFileL(RFs& aFs, const TDesC& aFileName)
	{
	// Delete the file if it exists.
	TInt err = aFs.Delete(aFileName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	// Create a new attachment file
	RFile file;
	User::LeaveIfError(file.Create(aFs, aFileName, EFileWrite));
	User::LeaveIfError(file.Write(KAttachmentContent));
	User::LeaveIfError(file.Write(KAttachmentContent));
	file.Close();	
	}
	
void CTestAttachmentSearch::TestAddFileAttachmentL()
	{
	_LIT(KTxtFileName, "NewFile.txt");
	_LIT8(KTxtFileMimeType, "*/*");
		
	HBufC* dataFilePath=HBufC::NewLC(20);
	(dataFilePath->Des()).Format(_L("c:\\msgtest\\"));

	CMsvEntry* entry = CreateTestEntryLC();
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);

	attachment->SetMimeTypeL(KTxtFileMimeType());
	attachment->SetAttachmentNameL(KTxtFileName());
	
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	RFile newFile;
	manager.CreateAttachmentL(KTxtFileName, newFile, attachment, iActiveWaiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	iActiveWaiter->WaitActive();
	
	CleanupClosePushL(newFile);
		
	TMsvAttachmentId attachmentId = attachment->Id();
	
	TInt fileSize = 1;//PopulateFileL(newFile, *dataFilePath);
	attachment->SetSize(fileSize);
	
	CleanupStack::PopAndDestroy(&newFile);
		
	store->CommitL();
	CleanupStack::PopAndDestroy(store);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(dataFilePath);
	
	
	}


void CTestAttachmentSearch::TearDownL()
	{
	_LIT(KTearDownL, "TearDownL");
	INFO_PRINTF1(KTearDownL);
	delete iActiveWaiter;
	delete iDBadapter;
	}


//

CMsvEntry* CTestAttachmentSearch::CreateTestEntryLC()
	{
	// Create an Entry 
	CMsvEntry* cEntry = CMsvEntry::NewL(*(testUtils->iMsvSession), KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering()); 
	CleanupStack::PushL(cEntry);
	
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidFindTestSMTPMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	cEntry->SetEntryL(entry.Id());
	iSelectionArray.Append(entry.Id());
	return cEntry;
	}
	
void CTestAttachmentSearch:: TestAddMessageAsAttachmentL(TMsvId aMessageId)
	{

	// add a message as an attachment to the message
	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*(testUtils->iMsvEntry));

	// Make sure you are set on the Message Id
	testUtils->iMsvEntry->SetEntryL(aMessageId);
	
	MMsvAttachmentManager& attachmentManager = imEmailMessage->AttachmentManager();
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvMessageEntry);
	CleanupStack::PushL(attachment);
	attachmentManager.AddEntryAsAttachmentL(0x00100002,attachment,iActiveWaiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to attachment manager
	
	iActiveWaiter->WaitActive(); 
	
	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf = imEmailMessage->ProgressL();	
	paramPack.Set(progBuf);
	attachmentMessageId = paramPack();	

	CleanupStack::PopAndDestroy(1); // imEmailMessage, 
	} 

void CTestAttachmentSearch::TestLinkAttachmentTypeSearchL()
	{
	InitL();

	testUtils->GoClientSideL();
	

//	CreateNewPlaintextMessageL();
	TestAddAttachmentAsLinkL();


		
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchString, "0");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchString1, "ya*");//Search String
	TBufC<10> searchString1(KSearchString1);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	searchQuery->iIsSearchQuery=ETrue;
	//searchQuery->AddSearchQueryOptions(EMsvTo, searchString, EMsvEqual, EAnd);
    searchQuery->AddSearchOptionL (EMsvAttachment, 1, EMsvEqual);

	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery  has 1 level
	EMsvTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);	
	INFO_PRINTF1(_L("Searching for Linked Attachments"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	ASSERT_EQUALS(array.Count(), 1);
	
	
		
	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(2);
	Closedown();
	
	}

void CTestAttachmentSearch::TestAttachmentFileTypeSearchL()
	{
	InitL();

	testUtils->GoClientSideL();
	

	TestAddFileAttachmentL();

		
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchString, "as*");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchString1, "ya*");//Search String
	TBufC<10> searchString1(KSearchString1);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->iIsSearchQuery=ETrue;
	//searchQuery->AddSearchQueryOptions(EMsvTo, searchString, EMsvEqual, EAnd);
    searchQuery->AddSearchOptionL (EMsvAttachment, 0, EMsvEqual);

	
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery  has 1 level
	EMsvTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);	
	INFO_PRINTF1(_L("Searching for File Attachments"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	ASSERT_EQUALS(array.Count(), 1);
	
	
		
	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(2);
	Closedown();	
	}

void CTestAttachmentSearch::TestAttachmentMessageTypeSearchL()
	{
	InitL();

	testUtils->GoClientSideL();
	
	CreateNewPlaintextMessageL();
	TestAddMessageAsAttachmentL(newMessageId);
		
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	searchQuery->iIsSearchQuery=ETrue;
	//searchQuery->AddSearchQueryOptions(EMsvTo, searchString, EMsvEqual, EAnd);
    searchQuery->AddSearchOptionL (EMsvAttachment, 2, EMsvEqual);

	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery  has 1 level
	EMsvTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);	
	INFO_PRINTF1(_L("Searching for message entry attachment"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	ASSERT_EQUALS(array.Count(), 1);
	
	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(2);
	Closedown();	
	}

	
CTestSuite* CTestAttachmentSearch::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestLinkAttachmentTypeSearchL);
	ADD_ASYNC_TEST_STEP(TestAttachmentFileTypeSearchL);
//	ADD_ASYNC_TEST_STEP(TestAttachmentMessageTypeSearchL);
	END_SUITE;
	}
