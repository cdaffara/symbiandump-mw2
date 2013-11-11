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

#include "t_msv_performancem.h"
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

RTest test3(KTest);
	
CTestPerformanceM::CTestPerformanceM()
:iServerSide(ETrue)
	{
	}
	

CTestPerformanceM::~CTestPerformanceM()
	{
	}
	
	
void CTestPerformanceM::SetupL()
	{
	_LIT(KFunction, ": Inside SetupL");
	INFO_PRINTF1(KFunction);
	
	RFs fileHandle;
	TInt err = fileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);

		iTestUtils = CEmailTestUtils::NewL(test3);
		iTestUtils->CleanMessageFolderL();
		iTestUtils->FileSession().SetSessionPath(_L("C:\\"));
		iTestUtils->GoClientSideL();
		RDebug::Printf("..CEmailTestUtils::NewL(test3) OK..");
	
	fileHandle.Close();
	}
	

void CTestPerformanceM::TearDownL()
	{
	_LIT(KFunction, ": Inside TearDownL");
	INFO_PRINTF1(KFunction);
	
	/*if(NULL != iTestUtils)
		{*/
		delete iTestUtils;
		iTestUtils = 0;
		//}
	}

void CTestPerformanceM::TestMoveMultipleEntry()
	{
	_LIT(KFunction, "TestMoveMultipleEntry");
	INFO_PRINTF1(KFunction);
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	CMsvEntry* parent = NULL;
	TMsvLocalOperationProgress progress = TMsvLocalOperationProgress();
	CMsvEntrySelection* entries = new(ELeave) CMsvEntrySelection;
	TReal64 diff = 0;
	
	iServerSide = EFalse;
	
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
	}
	
	
CTestSuite* CTestPerformanceM::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	
	ADD_ASYNC_TEST_STEP(TestMoveMultipleEntry);
	
	END_SUITE;
	}
