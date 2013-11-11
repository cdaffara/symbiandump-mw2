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
//

#include "ctestoomsearchsortcachemanager.h"
#include "msvsearchsortcacheentry.h"

#include "msvdbadapter.h"
#include "msvsearchsortdbwrapper.h"


#include <e32std.h>
#include "MSVSTD.H"
#include "MSVIPC.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVRUIDS.H"

#include "MTSR.H"
#include "MSVENTRY.H"
#include "MSERVER.H"
#include "MSVSERV.H"
#include "MSVUTILS.H"
#include "MSVAPI.H"
#include "indexcontext.h"
#include "CMsvCachedStore.h"
#include "CMsvBackupHandler.h"
#include "cmsvmailinitwaiter.h"
#include "msvcacheindextableentry.h"
#include "msventryfreepool.h"



CTestOOMSearchSortCacheManager::CTestOOMSearchSortCacheManager()
	{
	}

CTestOOMSearchSortCacheManager ::~CTestOOMSearchSortCacheManager()
	{
	}
	
void CTestOOMSearchSortCacheManager::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	}

void CTestOOMSearchSortCacheManager::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	}

	
// OOM test for SearchSortCacheEntry
void CTestOOMSearchSortCacheManager::TestCreateSearchSortCacheEntryOOML()
	{
	_LIT(KFunction, "TestCreateSearchSortCacheEntryOOML");
	INFO_PRINTF1(KFunction);

	TInt ret = KErrNone;
	TRAP(ret, iServer = CMsvServer::NewL());
	if(ret == KErrNoMemory)
		{
		return;		
		}
		
	//expecting the same instance, it was created by CMsvServer::ConstructL()
	CMSvSearchSortCacheManager* iSearchSortCacheManager = CMSvSearchSortCacheManager::Instance();
	ASSERT_EQUALS(iServer->iSearchSortCacheManager, iSearchSortCacheManager);

	//SearchSort Query1
		

	CMsvSearchSortQuery* searchQuery1= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(searchQuery1);
	
	searchQuery1->iParentId = 100;
	searchQuery1->iCaseSensitive = ETrue;
	searchQuery1->iWholeWord =EFalse ;
	searchQuery1->iWildCardCharacter = EFalse;
	searchQuery1->iExplicitSort = ETrue;
	searchQuery1->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString, "TEST");
	TBufC16<4> iSearchString(KSearchString);
	searchQuery1->AddSearchOptionL(EMsvDescription, iSearchString, EMsvEqual);
	
	_LIT(KEEMsgTo, "TESTEMsgTo");
	TBufC16<30> iSearchString1(KEEMsgTo);
	searchQuery1->AddSearchOptionL(EMsvTo, iSearchString1, EMsvEqual);

	_LIT(KEMsgFromString, "TESTEMsgFrom");
	TBufC16<40> iSearchString2(KEMsgFromString);
	searchQuery1->AddSearchOptionL(EMsvFrom, iSearchString2, EMsvEqual);

	//_LIT(KEMsgSize, "33");
	//TBufC16<30> iSearchString3(KEMsgSize);
	TInt iSearchString3 = 33;
	searchQuery1->AddSearchOptionL(EMsvSize, iSearchString3, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString4(KEMsgDate);
	searchQuery1->AddSearchOptionL(EMsvDate, iSearchString4, EMsvEqual);
	

	searchQuery1->iQueryCount = 5;
	searchQuery1->AddSortOptionL(EMsvTo,EMsvSortAscending );
	
	TInt err ;	
	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;
	CMsvSearchSortCacheEntry* cacheEntry2=NULL;
	
	// Testing heap failure while creating cache entry
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		
		
		TRAP(error, cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery1, ETrue));

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			
			_LIT(KPrint, "Created cache entry");
			INFO_PRINTF1(KPrint);
			
			//adding the cache entry to cache manager
			TRAP(err,iSearchSortCacheManager->AddEntryL(*cacheEntry2));
			ASSERT_EQUALS(err,KErrNone);
			}
		else
			{
			ASSERT_EQUALS(error, KErrNoMemory);
			
			_LIT(KPrint, "Met OutOfMemory condition");
			INFO_PRINTF1(KPrint);
			}
		}
	
	CleanupStack::PopAndDestroy(1); // Query
	delete iServer;
	}
	

void CTestOOMSearchSortCacheManager::TestForOutOfMemoryL()
	{
	__UHEAP_MARK;
	TestCreateSearchSortCacheEntryOOML();
	__UHEAP_MARKEND;
	}


CTestSuite* CTestOOMSearchSortCacheManager::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestForOutOfMemoryL);
	END_SUITE;
	}
