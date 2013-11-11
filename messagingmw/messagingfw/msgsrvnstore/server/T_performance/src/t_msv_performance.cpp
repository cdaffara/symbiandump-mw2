// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MACRO Declaration
// 
//

/**
 @file t_msv_performance.cpp
 @internalTechnology
*/

#include "t_msv_performance.h"
#include <hal.h>


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

_LIT(KTest, "Performance measurements");
_LIT(KDefaultIndexFile2, "C:\\Private\\1000484b\\Mail2\\Index");
_LIT(KTestDb, "C:\\messaging.db");

RTest test1(KTest);
	
CTestPerformance1::CTestPerformance1()
:iServerSide(ETrue)
	{
	}
	

CTestPerformance1::~CTestPerformance1()
	{
	}
	
	
void CTestPerformance1::SetupL()
	{
	_LIT(KFunction, ": Inside SetupL");
	INFO_PRINTF1(KFunction);
	
	RFs fileHandle;
	TInt err = fileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);

	if(iServerSide)
		{
		TRAP(err, iServer = CMsvServer::NewL());
		MY_ASSERT_EQUALS(err, KErrNone);
		iIndexAdapter = iServer->Context().IndexAdapter();
		RDebug::Printf("..Created server with IndexAdapter..");
		}
	else
		{
		iTestUtils = CEmailTestUtils::NewL(test1);
		iTestUtils->CleanMessageFolderL();
		iTestUtils->FileSession().SetSessionPath(_L("C:\\"));
		iTestUtils->GoClientSideL();
		RDebug::Printf("..CEmailTestUtils::NewL(test1) OK..");
		}
	fileHandle.Close();
	}
	

void CTestPerformance1::TearDownL()
	{
	_LIT(KFunction, ": Inside TearDownL");
	INFO_PRINTF1(KFunction);
	
	delete iServer;
	iServer = 0;
	iIndexAdapter = 0;
	
	/*if(NULL != iTestUtils)
		{*/
		delete iTestUtils;
		iTestUtils = 0;
		//}
	}


	
void CTestPerformance1::TestGetSingleEntry()
	{
	_LIT(KFunction, "TestGetSingleEntry");
	INFO_PRINTF1(KFunction);
	
	TMsvId id = KFirstFreeEntryId;
	TSecureId owner = 0x999;
	TUint32 start, stop;
	
	INFO_PRINTF1(_L("<b>SERVER SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	//[1]. Create single entry under Inbox and get the entry.
		
	TMsvEntry entry;
	entry.SetId(id+1);
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	TMsvId temp = entry.iId;
	
	
	CArrayPtrSeg<const TMsvEntry>* dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	delete dump;
	
	//iIndexAdapter->FlushCache();
	TMsvEntry* oEntry;
	start = User::FastCounter();
	iIndexAdapter->GetEntry(temp, oEntry, ETrue); //ETrue = add to cache
	stop = User::FastCounter();
	TReal64 diff = (TReal64)(stop-start)/frequency;
	MY_ASSERT_TRUE(oEntry->iId == temp);
	INFO_PRINTF2(_L("---Time taken for GetEntry: <b>%f sec</b>"), diff);
	}
	
	
	
void CTestPerformance1::TestAddMoveDeleteEntry()
	{
	_LIT(KFunction, "TestAddMoveDeleteEntry");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvId id = KFirstFreeEntryId;
	TMsvEntry entry;
	TMsvId temp;
	CMsvEntrySelection* entries = NULL;
	CArrayPtrSeg<const TMsvEntry>* dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	
	TReal64 move = 0;
	
	INFO_PRINTF1(_L("<b>SERVER SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);

	
	//[1]. Create entries under Inbox.
	//iIndexAdapter->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	entry.SetId(id+50); //entry #1
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm = KUidMsvFolderEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	stop = User::FastCounter();
	temp = entry.iId;
	TReal64 add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under an empty Inbox: <b>%f sec</b>"), 1, add1);

	//[3]. AddEntry.
	//3.0 AddEntry when a folder has 10 entries.
	for(TInt index = 0; index < 10; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)26000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	/*
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	delete dump;
	*/
	
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	TReal64 del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 10 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(27500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, add1);

	//3.1 Add 90 more entries to make it 100 entries. Now AddEntry again.
	for(TInt index = 0; index < 90; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)28000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 100 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 10 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(28500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, add1);
	
	//3.2 Add 400 more entries to make it 500 entries.
	for(TInt index = 0; index < 400; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)29000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 500 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 500 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(29500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 500 entries: <b>%f sec</b>"), 1, add1);
	
	//3.3 Add 500 more entries to make it 1000 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 1000 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	 
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 1000 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(20500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 1000 entries: <b>%f sec</b>"), 1, add1);
	
	//3.3 Add 500 more entries to make it 1500 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 1500 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 1500 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(21500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 1500 entries: <b>%f sec</b>"), 1, add1);
	
	//3.4 Add 500 more entries to make it 2000 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 2000 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 2000 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(22500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 2000 entries: <b>%f sec</b>"), 1, add1);
	
	//3.5 Add 500 more entries to make it 2500 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 2500 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 2500 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(23500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 2500 entries: <b>%f sec</b>"), 1, add1);
	
	//3.6 Add 500 more entries to make it 3000 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)24000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	entries = new(ELeave)CMsvEntrySelection;
	entries->AppendL(temp);
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries); //DELETE when in cache
	stop = User::FastCounter();
	delete entries;
	del1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entry under Outbox which has 3000 entries: <b>%f sec</b>"), 1, del1);

	entry.SetId(temp); //add that DELETEd entry again
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	
	iIndexAdapter->LockEntryAndStore(temp);
	start = User::FastCounter(); //move an entry from Outbox to Inbox.
	//iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId, ETrue); //synchronous move.
	iIndexAdapter->MoveEntry(temp, KMsvGlobalInBoxIndexEntryId); //synchronous move.
	stop = User::FastCounter();
	move = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>MOVE</b> %d entry from Outbox which has 3000 entries: <b>%f sec</b>"), 1, move);
	iIndexAdapter->MoveEntry(temp, KMsvGlobalOutBoxIndexEntryId); //move the entry back to Outbox.
	iIndexAdapter->ReleaseEntryAndStore(temp);
	
	entry.SetId(24500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 3000 entries: <b>%f sec</b>"), 1, add1);
	
	}
	
void CTestPerformance1::TestCopyAndUpdateEntry()
	{
	_LIT(KFunction, "TestCopyAndUpdateEntry");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	TReal64 diff =0;
	TMsvId temp = 0;
	
	iServerSide = EFalse;
	//TearDownL(); //tear down tests to be able to go to client side
	delete iServer;
	iServer = 0;
	iIndexAdapter = 0;
	SetupL();
	
	INFO_PRINTF1(_L("<b>CLIENT SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	CMsvEntry* parent = iTestUtils->iMsvSession->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	
	//[1]. 
	//1.0 Folder has 10 entries.
	for(TInt index = 0; index < 10; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	parent->SetEntryNoCheckL(KMsvGlobalOutBoxIndexEntryId); //bring to cache
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, diff);
	
	// 100 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 90; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 100 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 100 entries: <b>%f sec</b>"), 1, diff);

	// 500 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 400; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 500 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 500 entries: <b>%f sec</b>"), 1, diff);

	// 1000 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 1000 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 1000 entries: <b>%f sec</b>"), 1, diff);

	// 1500 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)24000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 1500 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 1500 entries: <b>%f sec</b>"), 1, diff);

	// 2000 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)25000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 2000 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 2000 entries: <b>%f sec</b>"), 1, diff);


	// 2500 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)26000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 2500 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 2500 entries: <b>%f sec</b>"), 1, diff);

	// 3000 entries.
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)27000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		if(index == 0) temp = entry.iId;
		}
	start = User::FastCounter();
	parent->CopyL(temp, KMsvGlobalInBoxIndexEntryId);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>COPY</b> %d entry under Outbox which has 3000 entries: <b>%f sec</b>"), 1, diff);

	parent->SetEntryL(temp);
	entry = parent->Entry();
	entry.iDescription.Set(_L("updated description"));
	entry.iDetails.Set(_L("updated details"));
	start = User::FastCounter();
	parent->ChangeL(entry);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>CHANGE</b> %d entry under Outbox which has 3000 entries: <b>%f sec</b>"), 1, diff);
	
	iServerSide = ETrue; //restore to server side
	//delete iTestUtils;
	}
	
void CTestPerformance1::TestMoveMultipleEntry()
	{
	_LIT(KFunction, "TestMoveMultipleEntry");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	CMsvEntry* parent = NULL;
	TMsvLocalOperationProgress progress = TMsvLocalOperationProgress();
	CMsvEntrySelection* entries = new(ELeave) CMsvEntrySelection;
	TReal64 diff = 0;
	
	iServerSide = EFalse;
	//TearDownL(); //tear down tests to be able to go to client side
	delete iServer;
	iServer = 0;
	iIndexAdapter = 0;
	SetupL();
	
	INFO_PRINTF1(_L("<b>CLIENT SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	parent = iTestUtils->iMsvSession->GetEntryL(KMsvDraftEntryId);
	
	//[1]. Move entries from Drafts to Outbox, and back again, and so on.
	for(TInt index = 0; index < 10; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvDraftEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	parent->SetEntryNoCheckL(KMsvDraftEntryId); //bring to cache
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvGlobalOutBoxIndexEntryId, progress); //iTestActive->iStatus);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Drafts which has 10 entries: <b>%f sec</b>"), 10, diff);
	MY_ASSERT_EQUALS(entries->Count(), 10);
	
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 90; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	parent->SetEntryNoCheckL(KMsvGlobalOutBoxIndexEntryId); //bring to cache
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvDraftEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Outbox which has 100 entries: <b>%f sec</b>"), 100, diff);
	MY_ASSERT_EQUALS(entries->Count(), 100);
	
	parent->SetEntryL(KMsvDraftEntryId);
	for(TInt index = 0; index < 400; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvDraftEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvGlobalOutBoxIndexEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Drafts which has 500 entries: <b>%f sec</b>"), 500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 500);
	
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvDraftEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Outbox which has 1000 entries: <b>%f sec</b>"), 1000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 1000);
	
	parent->SetEntryL(KMsvDraftEntryId);
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23000+index);
		entry.SetParent(KMsvDraftEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvGlobalOutBoxIndexEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Drafts which has 1500 entries: <b>%f sec</b>"), 1500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 1500);
	
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)24000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvDraftEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Outbox which has 2000 entries: <b>%f sec</b>"), 2000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 2000);
	
	parent->SetEntryL(KMsvDraftEntryId);
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)25000+index);
		entry.SetParent(KMsvDraftEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvGlobalOutBoxIndexEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Drafts which has 2500 entries: <b>%f sec</b>"), 2500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 2500);
	
	parent->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)26000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		parent->CreateL(entry);
		entries->AppendL(entry.iId);
		}
	start = User::FastCounter();
	parent->MoveL(*entries, KMsvDraftEntryId, progress);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to MOVE %d entries from Outbox which has 3000 entries: <b>%f sec</b>"), 3000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 3000);
	delete entries;
	
	iServerSide = ETrue;
	//delete iTestUtils;
	}
	
void CTestPerformance1::TestDeleteMultipleEntry()
	{
	_LIT(KFunction, "TestDeleteMultipleEntry");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	TReal64 diff =0;
	TMsvLocalOperationProgress progress = TMsvLocalOperationProgress();
	CMsvEntrySelection* entries = new(ELeave) CMsvEntrySelection;
	CArrayPtrSeg<const TMsvEntry>* dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	
	INFO_PRINTF1(_L("<b>SERVER SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	CleanupStack::PushL(entries);
	
	//[1]. Deletion of multiple entries from Outbox.
	//10 entries
	for(TInt index = 0; index < 10; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 10 entries: <b>%f sec</b>"), 10, diff);
	MY_ASSERT_EQUALS(entries->Count(), 10);
	CleanupStack::PopAndDestroy(); //entries

	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//100 entries
	for(TInt index = 0; index < 100; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 100 entries: <b>%f sec</b>"), 100, diff);
	MY_ASSERT_EQUALS(entries->Count(), 100);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//500 entries
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 500 entries: <b>%f sec</b>"), 500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 500);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//1000 entries
	for(TInt index = 0; index < 1000; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 1000 entries: <b>%f sec</b>"), 1000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 1000);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//1500 entries
	for(TInt index = 0; index < 1500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 1500 entries: <b>%f sec</b>"), 1500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 1500);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//2000 entries
	for(TInt index = 0; index < 2000; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)24000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 2000 entries: <b>%f sec</b>"), 2000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 2000);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//2500 entries
	for(TInt index = 0; index < 2500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)25000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 2500 entries: <b>%f sec</b>"), 2500, diff);
	MY_ASSERT_EQUALS(entries->Count(), 2500);
	CleanupStack::PopAndDestroy(); //entries
	
	entries = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(entries);
	
	
	//3000 entries
	for(TInt index = 0; index < 3000; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)26000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		entries->AppendL(entry.iId);
		iIndexAdapter->LockEntryAndStore(entry.iId);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	
	start = User::FastCounter();
	iIndexAdapter->DeleteSelection(*entries);
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---Time taken to <b>DELETE</b> %d entries from Outbox which has 3000 entries: <b>%f sec</b>"), 3000, diff);
	MY_ASSERT_EQUALS(entries->Count(), 3000);
	CleanupStack::PopAndDestroy(); //entries
	
	}
	
void CTestPerformance1::TestGetAllEntries()
	{
	_LIT(KFunction, "TestGetAllEntries");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	TReal64 diff =0;
	CArrayPtrSeg<const TMsvEntry>* children = NULL;
	CArrayPtrSeg<const TMsvEntry>* dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
		
	INFO_PRINTF1(_L("<b>SERVER SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	
	//[1]. Get under a folder i.e GetChildren.
	//10 entries.
	for(TInt index = 0; index < 10; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 10);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 10 entries: <b>%f sec</b>"), 10, diff);
	delete children;
	
	
	//100 entries.
	for(TInt index = 0; index < 90; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20500+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 100);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 100 entries: <b>%f sec</b>"), 100, diff);
	delete children;


	//500 entries.
	for(TInt index = 0; index < 400; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 500);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 500 entries: <b>%f sec</b>"), 500, diff);
	delete children;

	
	//1000 entries
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21500+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 1000);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 1000 entries: <b>%f sec</b>"), 1000, diff);
	delete children;
	
	
	//1500 entries.
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 1500);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 1500 entries: <b>%f sec</b>"), 1500, diff);
	delete children;


	//2000 entries.
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22500+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 2000);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 2000 entries: <b>%f sec</b>"), 2000, diff);
	delete children;


	//2500 entries.
	for(TInt index = 0; index < 500; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 2500);
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 2500 entries: <b>%f sec</b>"), 2500, diff);
	delete children;

	//3000 entries.
	TInt added = 0;
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	TRAPD(ret, iIndexAdapter->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *dump, TMsvSelectionOrdering())); //bring to cache
	for(TInt index = 0; index < 3000; ++index)
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)23500+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		added += iIndexAdapter->AddEntry(entry, owner, EFalse);
		}
	children = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	CleanupStack::PushL(children);
	start = User::FastCounter();
	iIndexAdapter->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *children, TMsvSelectionOrdering());
	stop = User::FastCounter();
	MY_ASSERT_TRUE(children->Count() == 3000);
	CleanupStack::PopAndDestroy(); //children
	diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF3(_L("---GET %d entries from Outbox which has 3000 entries: <b>%f sec</b>"), 3000, diff);
	delete dump;
	
	}
	

void CTestPerformance1::TestFunc()
	{
	_LIT(KFunction, "TestFunc");
	INFO_PRINTF1(KFunction);
	
	//iIndexAdapter->GetDbAdapter()->WipeDB();
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvId id = KFirstFreeEntryId;
	TMsvEntry entry;
	TMsvId temp;
	CMsvEntrySelection* entries = NULL;
	CArrayPtrSeg<const TMsvEntry>* dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	
	TReal64 move = 0;
	
	INFO_PRINTF1(_L("<b>SERVER SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);

	//iIndexAdapter->FlushCache();
	
	//[1]. Create entries under Inbox.
	entry.SetId(id+50); //entry #1
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm = KUidMsvFolderEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	stop = User::FastCounter();
	temp = entry.iId;
	TReal64 add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under an empty Inbox: <b>%f sec</b>"), 1, add1);

	
	//[3]. AddEntry.
	//3.0 AddEntry when a folder has 10 entries.
	for(TInt index = 0; index < 10; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)26000+index);
		entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvMessageEntry;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		iIndexAdapter->AddEntry(entry, owner, EFalse);
		if(index == 0) temp = entry.iId;
		}
	/*
	delete dump;
	dump = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iIndexAdapter->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *dump, TMsvSelectionOrdering()); //bring to cache
	delete dump;
	*/
	//iIndexAdapter->FlushCache();
	
	entry.SetId(27500);
	entry.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	start = User::FastCounter();
	iIndexAdapter->AddEntry(entry, owner, EFalse);
	temp = entry.iId;
	stop = User::FastCounter();
	add1 = (TReal64)(stop - start)/frequency;
	INFO_PRINTF3(_L("---Time taken to ADD %d entry under Outbox which has 10 entries: <b>%f sec</b>"), 1, add1);

	}
	
	
CTestSuite* CTestPerformance1::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	
	ADD_ASYNC_TEST_STEP(TestGetSingleEntry);
	ADD_ASYNC_TEST_STEP(TestAddMoveDeleteEntry);
	ADD_ASYNC_TEST_STEP(TestDeleteMultipleEntry);
	
	ADD_ASYNC_TEST_STEP(TestCopyAndUpdateEntry);
	
	ADD_ASYNC_TEST_STEP(TestMoveMultipleEntry);
	
	ADD_ASYNC_TEST_STEP(TestGetAllEntries);
	
	ADD_ASYNC_TEST_STEP(TestFunc);
	
	END_SUITE;
	}
