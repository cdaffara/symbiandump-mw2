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
// Contributors: Ashok Kumar
//
// Description:
//
 
#include "t_msv_converterdbadapter.h"
#include "cmsvversion1version2converter.h"


#define DEFAULT_RETURN KErrNone
#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);


_LIT(KDbFileName, "D:\\messaging.db");
_LIT(KVersion1MessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\messaging.db");
_LIT(KDriveD,"D:\\");

CTestConverterDBAdapter::CTestConverterDBAdapter()
	{
	}

CTestConverterDBAdapter::~CTestConverterDBAdapter()
	{
	}

void CTestConverterDBAdapter::SetupL()
	{
	_LIT(KFunction, "*_*SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	SetUpTest();
		
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);
	}



void CTestConverterDBAdapter::TearDownL()
	{
	_LIT(KFunction, "*_*TearDownL");
	INFO_PRINTF1(KFunction);
		
	delete iDbAdapter;
		
	__UHEAP_MARKEND;
	}

void CTestConverterDBAdapter::SetUpTest()
	{
	_LIT(KFunction, "SetUpTest");
	INFO_PRINTF1(KFunction);
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KVersion1MessageStore,KDriveD));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	}
	
void CTestConverterDBAdapter::TestCreateIndexEntryL()
	{
	_LIT(KFunction, "TestCreateIndexEntryL");
	INFO_PRINTF1(KFunction);
	
	TMsvEntry sampleTMsvEntry;
		
	sampleTMsvEntry.SetId(20987);
	sampleTMsvEntry.SetParent(4096);
	
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	
	sampleTMsvEntry.SetUnread(ETrue);
	sampleTMsvEntry.SetAttachment(ETrue);
	sampleTMsvEntry.SetNew(ETrue);
	sampleTMsvEntry.SetPriority(EMsvLowPriority);
	sampleTMsvEntry.iSize = 64;
	sampleTMsvEntry.iDate = 1234;
	sampleTMsvEntry.iType = KUidMsvServiceEntry;
	sampleTMsvEntry.iMtm = KUidMsvServiceEntry;

	iDbAdapter->InitializeLConverterStatementL();

	TRAPD(error ,iDbAdapter->CreateIndexEntryL(sampleTMsvEntry, 3098));
	MY_ASSERT_EQUALS(error , KErrNone);

	TRAP(error ,iDbAdapter->CreateIndexEntryL(sampleTMsvEntry, 3098));
	MY_ASSERT_TRUE(error != KErrNone);

	}
	
void CTestConverterDBAdapter::TestChangeVersionL()
	{
	_LIT(KFunction, "TestChangeVersionL");
	INFO_PRINTF1(KFunction);
	
	TRAPD(error ,iDbAdapter->ChangeVersionL(-1));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	TRAP(error ,iDbAdapter->ValidateDatabaseL(EDriveD));
	MY_ASSERT_EQUALS(error , KErrNotSupported);
	
	}
	
void CTestConverterDBAdapter::TestGetAllMessageIdsL()
	{
	_LIT(KFunction, "TestGetAllMessageIdsL");
	INFO_PRINTF1(KFunction);
	
	// for imap
	TInt32 imapMtmId			= {0x1000102A};	// 268439594
	RArray<TMsvId> serviceIdArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(imapMtmId, serviceIdArray));
	
	RArray<TMsvId> idArray;
	TMsvId attachmentId = 0;
	for(TInt i = 0; i< serviceIdArray.Count();i++)
		{
		TRAP(error ,iDbAdapter->GetAllMessageIdsL(serviceIdArray[i] ,idArray,imapMtmId));
		MY_ASSERT_EQUALS(error , KErrNone);
				
		idArray.Reset();
		
		TRAP(error ,iDbAdapter->GetAllMimeIdsL( attachmentId, idArray));	
		MY_ASSERT_EQUALS(error , KErrNone);
		}
	serviceIdArray.Close();
	}


void CTestConverterDBAdapter::TestGetAllServiceIDsForMtmL()
	{
	_LIT(KFunction, "TestGetAllServiceIDsForMtmL");
	INFO_PRINTF1(KFunction);
	// for imap
	TInt32 imapMtmId			= {0x1000102A};	// 268439594
	RArray<TMsvId> serviceIdArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(imapMtmId, serviceIdArray));
	MY_ASSERT_EQUALS(error , KErrNone);
	serviceIdArray.Close();
	}


void CTestConverterDBAdapter::TestGetAllIndexEntryTableIdsL()
	{
	_LIT(KFunction, "TestGetAllIndexEntryTableIdsL");
	INFO_PRINTF1(KFunction);

	RArray<TMsvId> idArray;
	TRAPD(error ,iDbAdapter->GetAllIndexEntryTableIdsL(idArray));
	MY_ASSERT_EQUALS(error , KErrNone);
	idArray.Close();
	
	}
	
void CTestConverterDBAdapter::TestGetMessageCountL()
	{
	_LIT(KFunction, "TestGetMessageCountL");
	INFO_PRINTF1(KFunction);
	
		// for pop
	TInt32 imapMtmId			= {0x1000102A};	// 268439594
	RArray<TMsvId> serviceIdArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(imapMtmId, serviceIdArray));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	TRAP(error ,iDbAdapter->GetMessageCountL(serviceIdArray[0]));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	serviceIdArray.Close();
	}

void CTestConverterDBAdapter::TestCreateConversionStatusTableL()
	{
	_LIT(KFunction, "TestCreateConversionStatusTableL");
	INFO_PRINTF1(KFunction);

	TRAPD(error ,iDbAdapter->RemoveConversionStatusTableL());
	MY_ASSERT_EQUALS(error , KErrNone);

	TRAP(error ,iDbAdapter->CreateConversionStatusTableL());
	MY_ASSERT_EQUALS(error , KErrNone);
	}
	
void CTestConverterDBAdapter::TestAddConversionStatusEntryL()
	{
	_LIT(KFunction, "TestAddConversionStatusEntryL");
	INFO_PRINTF1(KFunction);
	// create some entries
	TConversionResumeStatus status;
	status.lastId = 0;
	status.mtmId.iUid = 0;
	status.serviceId = 0;
	status.sourceVersion = 1;
	status.targetVersion = 2;
	status.remainingCount = 100;
	
	TRAPD(error ,iDbAdapter->AddConversionStatusEntryL(status));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	TRAP(error ,iDbAdapter->AddConversionStatusEntryL(status));
	MY_ASSERT_TRUE(error != KErrNone);
	}

void CTestConverterDBAdapter::TestRemoveConversionStatusTableL()
	{
	_LIT(KFunction, "TestRemoveConversionStatusTableL");
	INFO_PRINTF1(KFunction);
	
	TRAPD(error ,iDbAdapter->RemoveConversionStatusTableL());
	MY_ASSERT_EQUALS(error , KErrNone);
	}
	
void CTestConverterDBAdapter::TestGetConversionStatusL()
	{
	_LIT(KFunction, "TestGetConversionStatusL");
	INFO_PRINTF1(KFunction);
	TConversionResumeStatus status;
		
	TRAPD(error ,iDbAdapter->GetConversionStatusL(status));
	MY_ASSERT_EQUALS(error , KErrNone);
	}


void CTestConverterDBAdapter::TestUpdateConversionStatusEntryL()
	{
	_LIT(KFunction, "TestUpdateConversionStatusEntryL");
	INFO_PRINTF1(KFunction);

	TConversionResumeStatus status;
	status.lastId = 0;
	status.mtmId.iUid = 5;
	status.serviceId = 1;
	status.sourceVersion = 1;
	status.targetVersion = 2;
	status.remainingCount = 1000;
	
	TRAPD(error ,iDbAdapter->AddConversionStatusEntryL(status));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	// change remaining count and last id
	status.lastId = 104856;
	status.remainingCount = 100;
	
	TRAP(error ,iDbAdapter->UpdateConversionStatusEntryL(status));
	MY_ASSERT_EQUALS(error , KErrNone);
	}
	
void CTestConverterDBAdapter::TestRemoveConversionStatusEntriesL()
	{
	_LIT(KFunction, "TestRemoveConversionStatusEntriesL");
	INFO_PRINTF1(KFunction);
	
	TRAPD(error ,iDbAdapter->RemoveConversionStatusEntriesL());
	MY_ASSERT_EQUALS(error , KErrNone);
	}




CTestSuite* CTestConverterDBAdapter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestChangeVersionL);	
	ADD_ASYNC_TEST_STEP(TestCreateIndexEntryL);
	ADD_ASYNC_TEST_STEP(TestGetAllServiceIDsForMtmL);
	ADD_ASYNC_TEST_STEP(TestGetAllMessageIdsL);	
	ADD_ASYNC_TEST_STEP(TestGetAllIndexEntryTableIdsL);	
	ADD_ASYNC_TEST_STEP(TestGetMessageCountL);	
	ADD_ASYNC_TEST_STEP(TestRemoveConversionStatusTableL);
	ADD_ASYNC_TEST_STEP(TestCreateConversionStatusTableL);	
	ADD_ASYNC_TEST_STEP(TestAddConversionStatusEntryL);	
	ADD_ASYNC_TEST_STEP(TestGetConversionStatusL);	
	ADD_ASYNC_TEST_STEP(TestUpdateConversionStatusEntryL);	
	ADD_ASYNC_TEST_STEP(TestRemoveConversionStatusEntriesL);	
	END_SUITE;
	}

