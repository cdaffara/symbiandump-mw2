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

#include "t_msv_performancecu.h"
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

RTest test2(KTest);
	
CTestPerformanceCU::CTestPerformanceCU()
:iServerSide(ETrue)
	{
	}
	

CTestPerformanceCU::~CTestPerformanceCU()
	{
	}
	
	
void CTestPerformanceCU::SetupL()
	{
	_LIT(KFunction, ": Inside SetupL");
	INFO_PRINTF1(KFunction);
	
	iTestActive = new (ELeave) CTestActive();
	
		iTestUtils = CEmailTestUtils::NewL(test2);
		iTestUtils->CleanMessageFolderL();
		iTestUtils->FileSession().SetSessionPath(_L("C:\\"));
		iTestUtils->GoClientSideL();
		RDebug::Printf("..CEmailTestUtils::NewL(test2) OK..");

	}
	

void CTestPerformanceCU::TearDownL()
	{
	_LIT(KFunction, ": Inside TearDownL");
	INFO_PRINTF1(KFunction);
	
		delete iTestUtils;
		iTestUtils = 0;
		delete iTestActive;
	}
	
void CTestPerformanceCU::TestCopyAndUpdateEntry()
	{
	_LIT(KFunction, "TestCopyAndUpdateEntry");
	INFO_PRINTF1(KFunction);
	
	
	TSecureId owner = 0x999;
	TUint32 start, stop;
	TMsvEntry entry;
	TReal64 diff =0;
	TMsvId temp = 0;
	
	iServerSide = EFalse;
	
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
	
	}


		
void CTestPerformanceCU::TestDeleteMultipleEntries()
	{
	_LIT(KFunction, "TestDeleteMultipleEntries");
	INFO_PRINTF1(KFunction);
	
	TUint32 start, stop;
	TMsvEntry entry;
	TReal64 diff =0;
	iServerSide = EFalse;
	
	INFO_PRINTF1(_L("<b>CLIENT SIDE</b>"));
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);
	
	CMsvEntry* parent = iTestUtils->iMsvSession->GetEntryL(KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* children = NULL;
	CMsvOperation* del = NULL;
	
	//10 entries.
	for(TInt index = 0; index < 10; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20000+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidSMTPService;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		entry.IncPcSyncCount();
		CMsvOperation* add = parent->CreateL(entry, iTestActive->iStatus);
		CleanupStack::PushL(add);
		iTestActive->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(1, add);
		}
	children = parent->ChildrenL();
	parent->SetEntryNoCheckL(KMsvGlobalInBoxIndexEntryId); //bring to cache
	del = parent->DeleteL(*children, iTestActive->iStatus);
	CleanupStack::PushL(del);
	start = User::FastCounter();
	iTestActive->StartL();
	CActiveScheduler::Start();
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	CleanupStack::PopAndDestroy(1, del);
	delete children;
	INFO_PRINTF4(_L("---Time taken to <b>DELETE</b> %d entries under Inbox which has %d entries: <b>%f sec</b>"), 10, 10, diff);


	//100 entries.
	for(TInt index = 0; index < 100; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)20500+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidSMTPService;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		entry.IncPcSyncCount();
		CMsvOperation* add = parent->CreateL(entry, iTestActive->iStatus);
		CleanupStack::PushL(add);
		iTestActive->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(1, add);
		}
	children = parent->ChildrenL();
	parent->SetEntryNoCheckL(KMsvGlobalInBoxIndexEntryId); //bring to cache
	del = parent->DeleteL(*children, iTestActive->iStatus);
	CleanupStack::PushL(del);
	start = User::FastCounter();
	iTestActive->StartL();
	CActiveScheduler::Start();
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	CleanupStack::PopAndDestroy(1, del);
	delete children;
	INFO_PRINTF4(_L("---Time taken to <b>DELETE</b> %d entries under Inbox which has %d entries: <b>%f sec</b>"), 100, 100, diff);
	
	
	//500 entries.
	for(TInt index = 0; index < 500; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)21000+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidSMTPService;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		entry.IncPcSyncCount();
		CMsvOperation* add = parent->CreateL(entry, iTestActive->iStatus);
		CleanupStack::PushL(add);
		iTestActive->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(1, add);
		}
	children = parent->ChildrenL();
	parent->SetEntryNoCheckL(KMsvGlobalInBoxIndexEntryId); //bring to cache
	del = parent->DeleteL(*children, iTestActive->iStatus);
	CleanupStack::PushL(del);
	start = User::FastCounter();
	iTestActive->StartL();
	CActiveScheduler::Start();
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	CleanupStack::PopAndDestroy(1, del);
	delete children;
	INFO_PRINTF4(_L("---Time taken to <b>DELETE</b> %d entries under Inbox which has %d entries: <b>%f sec</b>"), 500, 500, diff);
	
	
	//1000 entries.
	for(TInt index = 0; index < 1000; ++index) //children of the folder
		{
		TMsvEntry entry;
		entry.SetId((TMsvId)22000+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidSMTPService;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		entry.IncPcSyncCount();
		CMsvOperation* add = parent->CreateL(entry, iTestActive->iStatus);
		CleanupStack::PushL(add);
		iTestActive->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(1, add);
		}
	children = parent->ChildrenL();
	parent->SetEntryNoCheckL(KMsvGlobalInBoxIndexEntryId); //bring to cache
	del = parent->DeleteL(*children, iTestActive->iStatus);
	CleanupStack::PushL(del);
	start = User::FastCounter();
	iTestActive->StartL();
	CActiveScheduler::Start();
	stop = User::FastCounter();
	diff = (TReal64)(stop-start)/frequency;
	CleanupStack::PopAndDestroy(1, del);
	delete children;
	INFO_PRINTF4(_L("---Time taken to <b>DELETE</b> %d entries under Inbox which has %d entries: <b>%f sec</b>"), 1000, 1000, diff);
	
	}
	
CTestSuite* CTestPerformanceCU::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	
//	ADD_ASYNC_TEST_STEP(TestCopyAndUpdateEntry);
	
	ADD_ASYNC_TEST_STEP(TestDeleteMultipleEntries);
	
	END_SUITE;
	}