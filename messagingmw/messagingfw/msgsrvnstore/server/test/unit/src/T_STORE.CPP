// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <txtrich.h>
#include <txtfmlyr.h>
#include "Testsetup.h"
#include <msvstore.h>
#include "CMsvCachedStore.h"
#include "CFakeStoreManager.h"

// local variables//

RTest test(_L("MSV Store Testrig"));
CParaFormatLayer* theParaFormatLayer;
CCharFormatLayer* theCharFormatLayer;
CRichText* theBodyText;
TBuf<50> theMessageContents;

// Global Uids
const TUid KUidMsvEntryFile={0x10003C68};

// Local Uids
const TUid KWRONGUid		= {0x10220107};
const TUid KTestUid1		= {0x10220108};
const TUid KTestUid2		= {0x10220109};

	const TMsvId KEntryId1 = 1;
	const TMsvId KEntryId2 = 2;
	const TMsvId KEntryId3 = 3;
	const TMsvId KEntryId4 = 4;

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

	static CMsvStore* OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);
	static CMsvStore* OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);

private:

	CTestMsvStore(MMsvStoreObserver& aObserver, RFs& aFs, TMsvId aId, MMsvStoreManager& aStoreManager) : CMsvStore(aObserver, aFs, aId, aStoreManager) {};

	};

CMsvStore* CTestMsvStore::OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	return CMsvStore::OpenForReadL(aObserver, aFs, aStoreManager, aId);
	}

CMsvStore* CTestMsvStore::OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	return CMsvStore::OpenForWriteL(aObserver, aFs, aStoreManager, aId);
	}


//
	
LOCAL_C void CheckStoreContentsL()
	{

	TPtrC view = _L("");
	TCharFormat format;
	theBodyText->GetChars(view, format, 0);

	TInt ret=theMessageContents==view;
	if (ret!=KErrNone)
		User::Leave(ret);
	}

LOCAL_C void TestMsvDictStoreL()
	{


	CMsvCachedStore* mdStore1;
	CMsvCachedStore* mdStore2;
	TInt error;

	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);
	storeManager->DeleteFileStoreL(KEntryId2);
	storeManager->DeleteFileStoreL(KEntryId3);
	storeManager->DeleteFileStoreL(KEntryId4);

	// open a non-existant store
	mdStore1 = CMsvCachedStore::OpenL(KEntryId1, *storeManager, EFalse);
	RMsvWriteStream stream;
	stream.AssignLC(*mdStore1,KWRONGUid);
	_LIT(KSomeData,"Some Data");
	stream.WriteL((TUint16 *) KSomeData.iBuf,10);
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	mdStore1->CommitL();
	delete mdStore1;

	// open an existant store
	mdStore1 = CMsvCachedStore::OpenL(KEntryId1, *storeManager, ETrue);

	delete mdStore1;

	// open a store already open
	mdStore1 = CMsvCachedStore::OpenL(KEntryId1, *storeManager, ETrue);
	mdStore2 = CMsvCachedStore::OpenL(KEntryId1, *storeManager, ETrue);
	delete mdStore1;
	delete mdStore2;

	// try to open wrong file type
	const TUidType type1(KPermanentFileStoreLayoutUid,KWRONGUid,KUidMsvEntryFile);
	RFile file;
	REPORT_IF_ERROR(file.Create(theUtils->FileSession(), storeManager->StoreFilename(KEntryId3), EFileShareAny|EFileWrite));
	CFileStore* store = CPermanentFileStore::NewL(file);
	store->SetTypeL(type1);
	store->CommitL();
	delete store;
	file.Close();
	TRAP(error, mdStore1 = CMsvCachedStore::OpenL(KEntryId3, *storeManager, ETrue));
	test(error==KErrCorrupt);

	// open a non-existant store with file failure

	// open a non-existant store with heap failure

	// open an existant store with file failure

	// open an existant store with heap failure

	CleanupStack::PopAndDestroy(storeManager);
	}

LOCAL_C void TestMsvStoreCreate1L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;
	CMsvStore* store2;
	TInt error;

	// open non-existant store for read
	TRAP(error, store1 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1));
	test(error==KErrNotFound);

	// open non-existant store for write
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store1->CommitL();
	delete store1;

	store2 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId2);
	store2->CommitL();
	delete store2;


	// open stores with multiple reads
	store1 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store2 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId2);
	delete store1;
	delete store2;

	// open stores for multiple writes
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store2 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId2);
	delete store2;
	delete store1;
	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}


LOCAL_C void TestMsvStoreReadAndEditL()
	{
    __UHEAP_MARK;

	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;

	// read from a stream into the same stream but close the reader before commiting the write stream
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	RMsvWriteStream out;
	out.AssignLC(*store1,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store1->Commit();
	delete store1;

	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	RMsvReadStream in;
	in.OpenLC(*store1,KNullUid);
	test(in.ReadInt32L()==1234);
	CleanupStack::PopAndDestroy(&in);

	in.OpenLC(*store1,KNullUid);
	out.AssignLC(*store1,KNullUid);
	out.WriteInt32L(in.ReadInt32L()+1);
	in.Close();
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	CleanupStack::Pop(&in);
	store1->CommitL();
	delete store1;

	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	in.OpenLC(*store1,KNullUid);
	test(in.ReadInt32L()==1235);
	CleanupStack::PopAndDestroy(&in);
	delete store1;

	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);

    __UHEAP_MARKEND;

	}


LOCAL_C void TestMsvStoreCreate2L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;
	CMsvStore* store2;

	// open stores for read then write
	store1 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store2 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId2);
	delete store1;
	delete store2;

	// open stores for write then read
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store2 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId2);
	delete store1;
	delete store2;

	delete nullObserver;

	storeManager->DeleteFileStoreL(KEntryId1);
	storeManager->DeleteFileStoreL(KEntryId2);
	
	CleanupStack::PopAndDestroy(storeManager);
	}


LOCAL_C void TestMsvStoreDeleteL()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;

	// Deleting with lock
	// open non-existant store for writing then delete
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store1->DeleteL();

	// try to delete again
	store1->DeleteL(); // no error occurs

	// try to restore body text
	TInt error;
	TRAP(error, store1->RestoreBodyTextL(*theBodyText));
	test(error==KErrNotFound);

	// try to create a read stream

	// try to create a write stream

	delete store1; 
	
	// Deleting without lock
	// open existant store for reading then try to delete
	store1 = CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store1->CommitL();
	delete store1;
	store1 = CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	TRAP(error, store1->DeleteL());
	test(error==KErrAccessDenied);
	

	delete store1;

	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}




LOCAL_C void TestMsvBodyTextL()
	{
	TInt error;
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	// create new write store
	CMsvStore* store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	
	// check the body text does not exist
	test(!store1->HasBodyTextL());

	// store some body text
	theMessageContents = _L("Test message contents");
	theBodyText->InsertL(0, theMessageContents);
	store1->StoreBodyTextL(*theBodyText);

	// check the body text does exist
	test(store1->HasBodyTextL());

	// restore the text and check its the same that went in
	store1->RestoreBodyTextL(*theBodyText);
	CheckStoreContentsL();

	// store new body text
	theBodyText->Reset();
	theMessageContents = _L("New test message contents");
	theBodyText->InsertL(0, theMessageContents);
	store1->StoreBodyTextL(*theBodyText);

	// restore the new body text
	store1->RestoreBodyTextL(*theBodyText);
	CheckStoreContentsL();

	// check the body text does exist
	test(store1->HasBodyTextL());

	// get rid of write store and test we can still read contents
	REPORT_IF_ERROR(store1->Commit());
	delete store1;

	// reopen the store and check the contents
	store1=CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	test(store1->HasBodyTextL());
	store1->RestoreBodyTextL(*theBodyText);
	CheckStoreContentsL();
	delete store1;



	// delete the body text stream
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store1->DeleteBodyTextL();
	test(!store1->HasBodyTextL());

	// try to read it again
	TRAP(error, store1->RestoreBodyTextL(*theBodyText));
	test(error==KErrNotFound);

	// get rid of write store and test we can still read contents
	REPORT_IF_ERROR(store1->Commit());
	delete store1;

	// reopen the store and check the contents
	store1=CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	test(!store1->HasBodyTextL());
	TRAP(error, store1->RestoreBodyTextL(*theBodyText));
	test(error==KErrNotFound);
		
	
	delete store1;
	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}



LOCAL_C void TestMsvStreams1L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;
	TInt error;
	TInt value;
	
	// create a blank store
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	store1->CommitL();
	delete store1;

	//
	// READ only store
	store1=CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	
	// inline tests
	test(store1->IsNullL());
	test(store1->IsPresentL(KTestUid1)==EFalse);

	// read non-existant stream
	TRAP(error, readStream.OpenL(*store1, KWRONGUid));
	test(error==KErrNotFound);

	// writing to a read only store
	TRAP(error, writeStream.AssignL(*store1,KTestUid1));
	test(error==KErrAccessDenied);

	// removing a stream
	TRAP(error, store1->RemoveL(KTestUid1));
	test(error==KErrAccessDenied);
	store1->Remove(KTestUid1); // failure not reported

	// try committing changes
	TRAP(error, store1->CommitL());
	test(error==KErrAccessDenied);
	error = store1->Commit();
	test(error==KErrAccessDenied);

	// try reverting changes
	TRAP(error, store1->RevertL());
	test(error==KErrAccessDenied);
	store1->Revert(); // failure not reported

	delete store1;

	
	//
	// WRITE/READ store
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);

	test(store1->IsNullL());
	test(store1->IsPresentL(KTestUid1)==EFalse);

	// writing new stream
	writeStream.AssignL(*store1, KTestUid1);
	writeStream << TInt32(42);
	writeStream.CommitL();
	writeStream.Close();

	test(store1->IsNullL()==EFalse);
	test(store1->IsPresentL(KTestUid1));

	// read non-existant stream on non-empty store, search key bigger than existant one
	TRAP(error, readStream.OpenL(*store1, KTestUid2));
	test(error==KErrNotFound);
	
	// read non-existant stream on non-empty store, search key smaller than existant one
	TRAP(error, readStream.OpenL(*store1, KWRONGUid));
	test(error==KErrNotFound);
	
	// read the stream back in
	readStream.OpenL(*store1, KTestUid1);
	value = readStream.ReadInt32L();
	test(value==42);
	readStream.Close();
	// commit and close the store
	store1->CommitL();
	delete store1;

	//
	// reopen WRITE/READ store
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	
	test(store1->IsNullL()==EFalse);
	test(store1->IsPresentL(KTestUid1));
	
	// read the stream back in
	readStream.OpenL(*store1, KTestUid1);
	value = readStream.ReadInt32L();
	test(value==42);;
	readStream.Close();
	// rewrite the stream
	writeStream.AssignL(*store1, KTestUid1);
	writeStream << TInt32(234);
	writeStream.CommitL();
	writeStream.Close();
	// reread the stream
	readStream.OpenL(*store1, KTestUid1);
	value = readStream.ReadInt32L();
	test(value==234);
	readStream.Close();
	store1->CommitL();
	delete store1;


	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}


LOCAL_C void TestMsvStreams2L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;
	RMsvReadStream readStream;
//	RMsvWriteStream writeStream;
	TInt value;
	
	//
	// READ only store
	store1=CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);

	test(store1->IsNullL()==EFalse);
	test(store1->IsPresentL(KTestUid1));

	// check the store has been correctly committed
	readStream.OpenL(*store1, KTestUid1);
	value = readStream.ReadInt32L();
	test(value==234);
	readStream.Close();

	delete store1;

	//
	// WRITE/READ store
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);

	test(store1->IsPresentL(KTestUid1));
	store1->RemoveL(KTestUid1);
	test(store1->IsPresentL(KTestUid1)==EFalse);
	store1->CommitL();
	delete store1;

	//
	// reopen WRITE/READ store
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	test(store1->IsNullL());
	test(store1->IsPresentL(KTestUid1)==EFalse);
	delete store1;

	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}


LOCAL_C void TestSizeL()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	
	storeManager->DeleteFileStoreL(KEntryId1);

	CNullMsvStoreObserver* nullObserver = new(ELeave) CNullMsvStoreObserver;

	CMsvStore* store1;
	RMsvWriteStream writeStream;
	TInt size;

	// create a blank store - check size can be read
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	size = store1->SizeL();
	store1->CommitL();
	size = store1->SizeL();
	delete store1; store1=NULL;

	// reopen the store (read  only) and check the size is correct
	store1=CTestMsvStore::OpenForReadL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	test(store1->SizeL()==size);
	delete store1; store1=NULL;

	// reopen the store (edit) and check the size is correct
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	test(store1->SizeL()==size);
	// write a stream
	writeStream.AssignL(*store1, KTestUid1);
	writeStream << TInt32(42);
	writeStream.CommitL();
	writeStream.Close();
	// test store has changed
	store1->CommitL();	
	test(store1->SizeL()>size);
	size = store1->SizeL();
	delete store1; store1=NULL;

	TInt error;
	store1=CTestMsvStore::OpenForWriteL(*nullObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	TRAP(error, store1->DeleteL());
	TRAP(error, store1->SizeL());
	test(error==KErrNotFound);
	delete store1; store1=NULL;
	
	delete nullObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}	
	
	
	
	
LOCAL_C void TestObservers1L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	TMsvId id=1;

	CSingleMsvStoreObserver* singleObserver = new(ELeave) CSingleMsvStoreObserver(id);

	CMsvStore* store1;
	CMsvStore* store2;
	CMsvStore* store3;

	// closing read only stores
	store1=CTestMsvStore::OpenForReadL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	store2=CTestMsvStore::OpenForReadL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	store3=CTestMsvStore::OpenForReadL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;

	//
	test(singleObserver->iCount==3);
	delete store2;	store2=NULL;
	test(singleObserver->iCount==2);
	delete store1;	store1=NULL;
	test(singleObserver->iCount==1);
	delete store3;	store3=NULL;
	test(singleObserver->iCount==0);
	singleObserver->Reset(id);


	// closing write stores
	store1=CTestMsvStore::OpenForWriteL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	singleObserver->iLocked=ETrue;
	//
	test(singleObserver->iCount==1);
	test(singleObserver->iLocked);
	delete store1;	store1=NULL;
	test(singleObserver->iCount==0);
	test(singleObserver->iLocked==EFalse);
	singleObserver->Reset(id);

	delete singleObserver;

	CleanupStack::PopAndDestroy(storeManager);
	}



LOCAL_C void TestObservers2L()
	{
	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	TMsvId id=1;

	CSingleMsvStoreObserver* singleObserver = new(ELeave) CSingleMsvStoreObserver(id);

	CMsvStore* store1;
	CMsvStore* store2;
	CMsvStore* store3;


	// closing read and write stores
	store1=CTestMsvStore::OpenForReadL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	store2=CTestMsvStore::OpenForReadL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	store3=CTestMsvStore::OpenForWriteL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	singleObserver->iLocked=ETrue;

	//
	test(singleObserver->iCount==3);
	test(singleObserver->iLocked);
	delete store1;	store1=NULL;
	test(singleObserver->iCount==2);
	test(singleObserver->iLocked);
	delete store3;	store3=NULL;
	test(singleObserver->iCount==1);
	test(singleObserver->iLocked==EFalse);
	delete store2;	store2=NULL;
	test(singleObserver->iCount==0);
	test(singleObserver->iLocked==EFalse);
	singleObserver->Reset(id);

	// deleting store
	store1=CTestMsvStore::OpenForWriteL(*singleObserver, theUtils->FileSession(), *storeManager, KEntryId1);
	singleObserver->iCount++;
	singleObserver->iLocked=ETrue;
	//
	test(singleObserver->iCount==1);
	test(singleObserver->iLocked);
	store1->DeleteL();
	test(singleObserver->iCount==1);
	test(singleObserver->iLocked);
	delete store1;	store1=NULL;
	test(singleObserver->iCount==0);
	test(singleObserver->iLocked==EFalse);
	singleObserver->Reset(id);

	delete singleObserver;
	
	CleanupStack::PopAndDestroy(storeManager);
	}



LOCAL_C void doTestsL()
	{
	theParaFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(theParaFormatLayer);

	theCharFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(theCharFormatLayer);

	theBodyText = CRichText::NewL(theParaFormatLayer, theCharFormatLayer);
	CleanupStack::PushL(theBodyText);

	// file stores
	CMsgsTestUtils::Start(_L("File stores"));
	TestMsvDictStoreL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Message stores"));
	TestMsvStoreCreate1L();	
	TestMsvStoreCreate2L();	
	TestSizeL();
	TestMsvStoreDeleteL();	
	TestMsvBodyTextL();
	TestMsvStreams1L();	
	TestMsvStreams2L();	
	TestMsvStoreReadAndEditL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Store observers"));
	TestObservers1L();
	TestObservers2L();
	CMsgsTestUtils::Complete();


	CFakeStoreManager* storeManager = CFakeStoreManager::NewL(theUtils->FileSession());
	CleanupStack::PushL(storeManager);
	storeManager->DeleteFileStoreL(KEntryId1);
	storeManager->DeleteFileStoreL(KEntryId2);
	storeManager->DeleteFileStoreL(KEntryId3);
	storeManager->DeleteFileStoreL(KEntryId4);
	CleanupStack::PopAndDestroy(storeManager);

	CleanupStack::PopAndDestroy(3); // theBodyText, theCharFormatLayer, theParaFormatLayer
	}
