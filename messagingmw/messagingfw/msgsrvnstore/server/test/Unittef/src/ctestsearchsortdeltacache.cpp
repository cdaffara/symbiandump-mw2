
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
// ctestsearchsortCacheManager.cpp
// 
//


#include "ctestsearchsortdeltacache.h"
#include "msvsearchsortcachemanager.h"

TFileName dbFileName;
TMsvEntry sampleTMsvEntry;
TInt bulkCount = 50;


CTestSearchSortDeltaCache::CTestSearchSortDeltaCache()
	{
	}

CTestSearchSortDeltaCache ::~CTestSearchSortDeltaCache()
	{
	
	}
	
void CTestSearchSortDeltaCache::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	}

void CTestSearchSortDeltaCache::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
	delete iServer;
	}

void CTestSearchSortDeltaCache::TestHandelL()
	{
	iServer=0;
	iIndexAdapter=0;

	TRAPD(err, iServer = CMsvServer::NewL());
	
	iSearchSortDeltaCache = CMsvSearchSortDeltaCache::Instance();
	ASSERT_EQUALS(err, KErrNone);
	
	iIndexAdapter = iServer->Context().IndexAdapter();

	InitCacheMangerL();
	AddEntry();
	TestDeleteEntry();
	TestChangeEntry();
	}
	
void CTestSearchSortDeltaCache::AddEntry()
	{
	_LIT(KFunction, "AddEntry");
	INFO_PRINTF1(KFunction);
	
	TSecureId aOwnerId = 0x999;
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.SetParent(4096);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	sampleTMsvEntry.SetId(10);
	iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue);
	ASSERT_TRUE(iSearchSortDeltaCache->iNewEntry.Count() == 1 );
	}


void CTestSearchSortDeltaCache::TestDeleteEntry()
	{
	_LIT(KFunction, "TestDeleteEntry");
	INFO_PRINTF1(KFunction);
	iIndexAdapter->LockEntry(10);
	iIndexAdapter->DeleteEntry(10);
	ASSERT_TRUE(iSearchSortDeltaCache->iDeleteEntry.Count() == 0 );
	}

void CTestSearchSortDeltaCache::TestChangeEntry()
	{
	_LIT(KFunction, "TestChangeEntry");
	INFO_PRINTF1(KFunction);
	TSecureId aOwnerId = 0x999;
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.SetParent(4096);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	sampleTMsvEntry.SetId(10);
	iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue);

	TInt32 mtm1 = 35;
	TInt32 mtm2 = 45;
	
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	iIndexAdapter->ChangeEntry(sampleTMsvEntry, 0x999, ETrue);
	ASSERT_TRUE(iSearchSortDeltaCache->iUpdateEntry.Count() == 0 );
	}

void CTestSearchSortDeltaCache::InitCacheMangerL()
	{
	
	//CACHE ENTRY 1:-
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	iSearchQuery->iParentId = 100;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	_LIT(KSearchString, "SEARCH SORT TEST");
	TBufC16<40> iSearchString(KSearchString);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString, EMsvEqual);
	iSearchQuery->iQueryCount = 1;
	
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	
	
	//ADDENTRY :-
	CMSvSearchSortCacheManager::Instance()->AddEntryL(*iCacheEntry);
	CleanupStack::PopAndDestroy(1); // Query
	
	}

CTestSuite* CTestSearchSortDeltaCache::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestHandelL);
	END_SUITE;
	}

