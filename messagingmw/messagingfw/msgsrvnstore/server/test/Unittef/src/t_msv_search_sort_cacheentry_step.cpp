// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_msv_search_sort_cacheentry_step.h"
#include "msvsearchsortcacheentry.h"
#include "msvsearchsortcachemanager.h"


/***********************************/
// Extra includes from CMsvServer.cpp


#include <e32std.h>

#include <barsc.h>
#include <barsread.h>
#pragma warning( disable : 4245 ) 
#include <bautils.h>
#pragma warning( default : 4245 ) 
#include <bafindf.h>
#include <u32std.h>

#include "MSVSTD.H"
#include "MSVIPC.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVRUIDS.H"

#include <msgs.rsg>

#include "MTSR.H"
#include "MSVENTRY.H"
#include "MSERVER.H"
#include "MSVRBLD.H"
#include "MSVSERV.H"
#include "MSVDELET.H"
#include "MSVMOVE.H"
#include "MSVUTILS.H"

#include "MSVPANIC.H"
#include "MSVAPI.H"
#include "indexcontext.h"
#include "CMsvCachedStore.h"
#include "CMsvBackupHandler.h"
#include "cmsvmailinitwaiter.h"
#include "msvcacheindextableentry.h"
#include "msventryfreepool.h"



CTestSearchSortCacheEntry::CTestSearchSortCacheEntry()
	{
	}

CTestSearchSortCacheEntry ::~CTestSearchSortCacheEntry()
	{
	
	}
	
void CTestSearchSortCacheEntry::SetupL()
	{
	_LIT(KSetupL, "SetupL");
	INFO_PRINTF1(KSetupL);

	TInt err;
	TRAP(err, iServer = CMsvServer::NewL());
	ASSERT_EQUALS(err, KErrNone);
	
	iSearchSortCacheManager= CMSvSearchSortCacheManager::CreateManagerL(*iServer);
	
	CMSvSearchSortCacheManager* searchSortCacheManager = CMSvSearchSortCacheManager::Instance();
	ASSERT_EQUALS(searchSortCacheManager, iSearchSortCacheManager);
	}

void CTestSearchSortCacheEntry::TearDownL()
	{
	_LIT(KTearDownL, "TearDownL");
	INFO_PRINTF1(KTearDownL);
	delete iServer;
	}

void CTestSearchSortCacheEntry::TestCreateEntryL()
	{
	_LIT(KTestEntryL, "TestCreateEntryL");
	INFO_PRINTF1(KTestEntryL);

	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	
	iSearchQuery->iParentId = 100;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	
	_LIT(KSearchString, "TEST");
	TBufC16<4> iSearchString(KSearchString);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString, EMsvEqual);
	
	_LIT(KEEMsgTo, "TESTEMsgTo");
	TBufC16<30> iSearchString1(KEEMsgTo);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString1, EMsvEqual);

	_LIT(KEMsgFromString, "TESTEMsgFrom");
	TBufC16<40> iSearchString2(KEMsgFromString);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString2, EMsvEqual);

	
	TInt iSearchString3 = 33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString3, EMsvEqual);


	
	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString4(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString4, EMsvEqual);
	

	iSearchQuery->iQueryCount = 5;
	TInt  iterationLimit =0;
	
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, ETrue,iterationLimit);
	
	_LIT(KTestTypeofQuery, "TestCacheEntryTypeofQuery");
	INFO_PRINTF1(KTestTypeofQuery);
	ASSERT_FALSE(iCacheEntry->IsSortQuery());

	_LIT(KTestParentId, "TestCacheEntryQueryParentId");
	INFO_PRINTF1(KTestParentId);
	ASSERT_TRUE(iCacheEntry->iParentId == 100);
	
	_LIT(KTestCaseSensetive, "TestCacheEntryQueryCaseSensetive");
	INFO_PRINTF1(KTestCaseSensetive);
	ASSERT_TRUE(iCacheEntry->IsCaseSensetive());

	_LIT(KTestWholeWord, "TestCacheEntryQueryWholeWord");
	INFO_PRINTF1(KTestWholeWord);
	ASSERT_FALSE(iCacheEntry->IsWholeWord());

	_LIT(KTestWildCharacter, "TestCacheEntryQueryWildCharacter");
	INFO_PRINTF1(KTestWildCharacter);
	ASSERT_FALSE(iCacheEntry->IsWildCharacter());

	_LIT(KTestExplicitSortReq, "TestCacheEntryQueryExplicitSortReq");
	INFO_PRINTF1(KTestExplicitSortReq);
	ASSERT_TRUE(iCacheEntry->IsExplicitSortReq());
	ASSERT_TRUE(iCacheEntry->iMsgExplicitSortPart == EMsvSize);


	_LIT(KTestSortType, "TestCacheEntryQuerySortType");
	INFO_PRINTF1(KTestSortType);
	ASSERT_FALSE(iCacheEntry->iResultType == EMsvSortAscending);

	_LIT(KTestResultType, "TestCacheEntryResultType");
	INFO_PRINTF1(KTestResultType);
	ASSERT_TRUE(iCacheEntry->iResultType ==EMsvResultAsTMsvId);

	//Query  content..
	_LIT(KTestQueryContent, "TestCacheEntryQueryContent");
	INFO_PRINTF1(KTestQueryContent);
	
	_LIT(KEMsgSizestring1, "33");
	TBufC16<30> iSearchString31(KEMsgSizestring1);

	
	for(TInt i= 0; i < iCacheEntry->iMsgQuery.Count(); i++)
		{
		TMsvMessagePart iMsgPart = iCacheEntry->iMsgQuery[i].iMessagePart;
		ASSERT_TRUE(iMsgPart==EMsvDescription||iMsgPart== EMsvTo||iMsgPart==EMsvFrom||iMsgPart==EMsvDate||iMsgPart==EMsvSize);
		ASSERT_TRUE(0==iCacheEntry->iMsgQuery[i].iQueryString.CompareF(iSearchString)||0==iCacheEntry->iMsgQuery[i].iQueryString.CompareF(iSearchString1)||0==iCacheEntry->iMsgQuery[i].iQueryString.CompareF(iSearchString2)
		||(0 == iCacheEntry->iMsgQuery[i].iQueryString.CompareF(iSearchString31))||0==iCacheEntry->iMsgQuery[i].iQueryString.CompareF(iSearchString4));
		}
	
	ASSERT_TRUE(iCacheEntry->iQueryID == 1001); 
	delete iCacheEntry;
	CleanupStack::PopAndDestroy(1); // Query
	}

	
CTestSuite* CTestSearchSortCacheEntry::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestCreateEntryL);
	END_SUITE;
	}
