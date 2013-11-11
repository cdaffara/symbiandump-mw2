//
// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file t_msv_performance.cpp
 @internalTechnology
*/


#include <txtrich.h>
#include <txtfmlyr.h>
#include "e32test.h"
#include "t_msv_store_step.h"
#include "CMsvCachedStore.h"
#include <test\testexecutestepbase.h>
#include "CFakeStoreManager.h"

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

//Database
_LIT(KTestDbFileName, "c:\\messaging.db"); 


#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);



_LIT(KDbFileName, "\\messaging.db");
_LIT(KMsvServerPattern, "!MsvServer*");




RTest test(_L("MSV Store Testrig"));

const TMsvId KEntryId1 = 1;
const TMsvId KEntryId2 = 2;
const TMsvId KEntryId3 = 3;
const TMsvId KEntryId4 = 4;

const TUid KUidMsgTypePOP3			= {0x10001029};	// 268439593
//

class CNullMsvStoreObserver : public CBase, public MMsvStoreObserver
	{
public:
	void HandleStoreEvent(TMsvStoreEvent, TMsvId) {};
	};

//

class CSingleMsvStoreObserver : public CBase, public MMsvStoreObserver
	{
public:
	CSingleMsvStoreObserver(TMsvId aId);
	void HandleStoreEvent(TMsvStoreEvent aEvent, TMsvId aId);
	void Reset(TMsvId aId);
public:
	TMsvId iId;
	TInt iCount;
	TBool iLocked;
	};

CSingleMsvStoreObserver::CSingleMsvStoreObserver(TMsvId aId)
	{
	iId = aId;
	}

void CSingleMsvStoreObserver::Reset(TMsvId aId)
	{
	iId = aId;
	iCount=0;
	iLocked=EFalse;
	}

void CSingleMsvStoreObserver::HandleStoreEvent(TMsvStoreEvent aEvent, TMsvId aId)
	{
	test(aId==iId);
	switch (aEvent)
		{
		case EMsvReadStoreClosed:
			iCount--;
			test(iCount>=0);
			break;
		case EMsvEditStoreClosed:
			test(iLocked);
			iLocked=EFalse;
			iCount--;
			test(iCount>=0);
			break;
		}
	}


//
// needed to allow public access to open functions
// also acts as actual implementation to test use of StoreL()

class CTestMsvStore : public CMsvStore
	{
public:

	static CMsvStore* OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId);
	static CMsvStore* OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId);

private:

	CTestMsvStore(MMsvStoreObserver& aObserver, RFs& aFs, TMsvId aId, MMsvStoreManager& aStoreManager) : CMsvStore(aObserver, aFs, aId, aStoreManager) {};

	};

CMsvStore* CTestMsvStore::OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId)
	{
	return  CMsvStore::OpenForReadL(aObserver, aFs, aStoreManager,aId,aMtmId);
	}

CMsvStore* CTestMsvStore::OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId)
	{
	return  CMsvStore::OpenForWriteL(aObserver, aFs, aStoreManager,aId,aMtmId);
	}




//

	
CTestStore::CTestStore()
	{
	}
	

CTestStore::~CTestStore()
	{
	}
	
	
void CTestStore::SetupL()
	{
	_LIT(KFunction, ": Inside SetupL");
	INFO_PRINTF1(KFunction);
	
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);

	
	}
	

void CTestStore::TearDownL()
	{
	_LIT(KFunction, ": Inside TearDownL");
	INFO_PRINTF1(KFunction);
	}


void CTestStore::TestOpenForReadL()
	{
	
	_LIT(KFunction, ":Test For OpenForReadL");
	INFO_PRINTF1(KFunction);
	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);



	CleanupStack::PopAndDestroy(storeManager);
	}

void CTestStore::TestOpenForWriteL()
	{
	_LIT(KFunction, ":Test For OpenForWriteL");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	
	TEntryStoreType aEntry = EHeaderEntry;
	
	TRAP(error, store1 = CTestMsvStore::OpenForWriteL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);


	CleanupStack::PopAndDestroy(storeManager);


	}

void CTestStore::TestStoreHeaderL()
	{
	_LIT(KFunction, ":Test For Store  Header");
	INFO_PRINTF1(KFunction);
	
	
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	CFieldPair* field = new CFieldPair();
	field->iFieldNumValue = 5555;
	TInt err;
	
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	field = new CFieldPair();
	TTime date;
	date.UniversalTime();
	field->iFieldNumValue = date.Int64();
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	TRAP(err, metadata->iFieldPairList.AppendL(field));

	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	
	TEntryStoreType aEntry = EHeaderEntry;
	TRAP(error, store1 = CTestMsvStore::OpenForWriteL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);


	TMsvWriteStore storeWriter(*store1);
	storeWriter.AssignL(metadata);
	TRAP(error, storeWriter.CommitL());
	ASSERT_TRUE(error == KErrNone);

	CleanupStack::PopAndDestroy(storeManager);

	}

void CTestStore::TestReStoreheaderL()
	{
	_LIT(KFunction, ":Test For ReStore  Header");
	INFO_PRINTF1(KFunction);

	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);



	CHeaderFields* RcvHeaderRow = NULL;
	TMsvReadStore storeReader((*store1), KUidMsgTypePOP3);
	TRAP(error,storeReader.LoadL(RcvHeaderRow));
	ASSERT_TRUE(error != KErrNone);
	
	CHeaderFields* RcvHeaderRow1 = NULL;
	TMsvReadStore storeReader1((*store1), KUidMsgTypePOP3);
	TRAP(error,storeReader1.ReadL(RcvHeaderRow1));
	ASSERT_TRUE(error != KErrNone);
	CleanupStack::PopAndDestroy(storeManager);

	}

void CTestStore::TestCommitL()
	{
	_LIT(KFunction, ":Test For CommitL  Header");
	INFO_PRINTF1(KFunction);

	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->CommitL());
	ASSERT_TRUE(error != KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	}


void CTestStore::TestStoreBodyTextL()
	{
	
	_LIT(KFunction, ":Test For StoreBodyTextL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	
	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);
	richText->InsertL(0, _L("Test Message"));

	
	TRAP(error,store1->StoreBodyTextL(*richText));
	ASSERT_TRUE(error != KErrNone);
	CleanupStack::PopAndDestroy(4,storeManager);
	
	
	}

void CTestStore::TestReStoreBodyTextL()
	{
	
	_LIT(KFunction, ":Test For ReStoreBodyTextL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error != KErrNone);
	
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	
	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);
	richText->InsertL(0, _L("Test Message"));

	
	
	TRAP(error,store1->RestoreBodyTextL((*richText)));
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(4,storeManager);
	
	}
	
	
void CTestStore::TestHasBodyTextL()
	{
	_LIT(KFunction, ":Test For HasBodyTextL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->HasBodyTextL());
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);

	}

void CTestStore::TestIsPresentL()
	{
	_LIT(KFunction, ":Test For IsPresentL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->IsPresentL(KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);

	}

void CTestStore::TestDeleteBodyTextL()
	{
	_LIT(KFunction, ":Test For DeleteBodyTextL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->DeleteBodyTextL());
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	
	}
	
	
void CTestStore::TestDeleteL()
	{
	_LIT(KFunction, ":Test For DeleteL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->DeleteL());
	ASSERT_TRUE(error != KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	
	}
	
	
void CTestStore::TestRemoveL()
	{
	_LIT(KFunction, ":Test For Remove ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->Remove(KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	}	
	
	
void CTestStore::TestSizeL()	
	{
	_LIT(KFunction, ":Test For Size ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->SizeL());
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	
	
	}
	
	
void CTestStore::TestRevertL()
	{
	_LIT(KFunction, ":Test For RevertL ");
	INFO_PRINTF1(KFunction);

	
	RFs as;
	as.Connect();	
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(as);
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;


	TInt error = 0;
	CMsvStore* store1;
	TEntryStoreType aEntry = EHeaderEntry;

	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, as, *storeManager,KEntryId1,KUidMsgTypePOP3 ));
	ASSERT_TRUE(error == KErrNone);
	
	TRAP(error,store1->SizeL());
	ASSERT_TRUE(error == KErrNone);
	CleanupStack::PopAndDestroy(storeManager);
	
	}

	
CTestSuite* CTestStore::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestOpenForReadL);
	ADD_ASYNC_TEST_STEP(TestOpenForWriteL);
	ADD_ASYNC_TEST_STEP(TestStoreHeaderL);
	ADD_ASYNC_TEST_STEP(TestReStoreheaderL);
	ADD_ASYNC_TEST_STEP(TestCommitL);
	ADD_ASYNC_TEST_STEP(TestHasBodyTextL);
	ADD_ASYNC_TEST_STEP(TestIsPresentL);
	ADD_ASYNC_TEST_STEP(TestDeleteBodyTextL);
	ADD_ASYNC_TEST_STEP(TestDeleteL);
	ADD_ASYNC_TEST_STEP(TestRemoveL);
	ADD_ASYNC_TEST_STEP(TestSizeL);
	ADD_ASYNC_TEST_STEP(TestRevertL);
	END_SUITE;
	}
