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


#include "ctestsearchsortcachemanager.h"
#include "msvsearchsortcacheentry.h"

#include "msvdbadapter.h"
#include "msvsearchsortdbwrapper.h"


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



CTestSearchSortCacheManager::CTestSearchSortCacheManager()
	{
	}

CTestSearchSortCacheManager ::~CTestSearchSortCacheManager()
	{
	}
	
void CTestSearchSortCacheManager::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	}

void CTestSearchSortCacheManager::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
	delete iServer;
	}

void CTestSearchSortCacheManager::TestCreateManagerL()
	{
	_LIT(KFunction, "TestCreateManagerL");
	INFO_PRINTF1(KFunction);

	TRAPD(err, iServer = CMsvServer::NewL());


	
	TestInstanceL();
	TestQueryExistEntryL();
	TestAddEntryL();
	TestRemoveEndEntryL();
	TestTypeOfSortQueryL();
	TestRetrunQueryL();
	TestMoveEntryAtEndL();
	TestUnMarkedQueryL();
	TestOnDemandUpdateCacheEntryL();
	TestResultCountL();
	TestFinalResultAsIdL();
	TestStoreResultL();
	TestDoProcessQueryL();
	TestRemoveEntryL();
	TesUpdateQueryResultOnDemandInDBL();
	
	}

void CTestSearchSortCacheManager::TestInstanceL()
	{
	_LIT(KFunction, "TestInstance");
	INFO_PRINTF1(KFunction);
	iSearchSortCacheManager= CMSvSearchSortCacheManager::CreateManagerL(*iServer);
	CMSvSearchSortCacheManager* searchSortCacheManager = CMSvSearchSortCacheManager::Instance();
	ASSERT_EQUALS(searchSortCacheManager, iSearchSortCacheManager);
	}


void CTestSearchSortCacheManager::TestQueryExistEntryL()
	{
	_LIT(KFunction, "TestQueryExistEntryL");
	INFO_PRINTF1(KFunction);
	
	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery = CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	iSearchQuery->iParentId = 100;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	TInt iSearchString3 = 33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString3, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	TRAP(err,CMSvSearchSortCacheManager::Instance()->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	TInt aTest = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();
	CMSvSearchSortCacheManager::Instance()->QueryExists(*iCacheEntry);
	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestAddEntryL()
	{

	_LIT(KFunction, "TestAddEntryL");
	INFO_PRINTF1(KFunction);
	
	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	iSearchQuery->iParentId = 14567780;
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

	//_LIT(KEMsgSize, "33");
	//TBufC16<30> iSearchString3(KEMsgSize);
	TInt iSearchString3 = 33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString3, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString4(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString4, EMsvEqual);
	

	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	TInt aTest = iSearchSortCacheManager->iManagerEntry->Count();
	
	//ADDENTRY TEST:-
	ASSERT_TRUE(aTest == 2);

	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestRemoveEndEntryL()
	{
	_LIT(KFunction, "TestRemoveEndEntryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	//CACHE ENTRY 1:-
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	iSearchQuery->iParentId = 503;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	TInt   iSearchString14 = 33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	
	TInt atest=-1;
	atest = CMSvSearchSortCacheManager::Instance()->QueryExists(*iCacheEntry);
	iSearchSortCacheManager->RemoveEndEntry(atest);
	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestTypeOfSortQueryL()
	{
	_LIT(KFunction, "TypeOfSortQueryL");
	INFO_PRINTF1(KFunction);

	//CACHE ENTRY 1:-
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	iSearchQuery->iParentId = 600;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	_LIT(KSearchString, "SEARCH SORT TEST");
	TBufC16<40> iSearchString(KSearchString);
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	iSearchSortCacheManager->TypeOfSortQuery(*iCacheEntry);
	ASSERT_TRUE(iCacheEntry->iSortOnHeader == TRUE);
	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestRetrunQueryL()
	{	

	_LIT(KFunction, "TestRetrunQueryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	//CACHE ENTRY 1:-
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	iSearchQuery->iParentId = 72100;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "JustTEST");
	TBufC16<10> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt iSearchString14 = 73; 
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080422:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending);
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	
	
	//TMsvSearchSortQuery:-
			
	
	//TMsvSearchSortQuery:-
	CMsvSearchSortQuery* returnSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(returnSearchQuery);

	TInt queryId = iSearchSortCacheManager->QueryID();
	
	iSearchSortCacheManager->RetrunQuery(iSearchSortCacheManager->QueryExists(queryId), returnSearchQuery);
	ASSERT_TRUE(returnSearchQuery->iParentId == 72100);


	CleanupStack::PopAndDestroy(2); // Query
	}


void CTestSearchSortCacheManager::TestMoveEntryAtEndL()
	{
	_LIT(KFunction, "TestMoveEntryAtEndL");
	INFO_PRINTF1(KFunction);
	
	//CACHE ENTRY 1:-
	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	
	iSearchQuery->iParentId = 900;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	_LIT(KEMsgSize1, "33");
	TBufC16<30> iSearchString14(KEMsgSize1);
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	
	
	TInt tt = CMSvSearchSortCacheManager::Instance()->QueryExists(*iCacheEntry);
	CMSvSearchSortCacheManager::Instance()->MoveEntryAtEnd(iCacheEntry,iSearchSortCacheManager->QueryExists(*iCacheEntry));
	tt = CMSvSearchSortCacheManager::Instance()->QueryExists(*iCacheEntry);
	ASSERT_TRUE(tt =6)

	CleanupStack::PopAndDestroy(1); // Query
	}
	

void CTestSearchSortCacheManager::TestUnMarkedQueryL()
	{
	_LIT(KFunction, "TestUnMarkedQueryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);


	iSearchQuery->iParentId = 111;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt iSearchString14 =33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, ETrue);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);

	ASSERT_TRUE(iCacheEntry->iMarkQuery == TRUE );
	iSearchSortCacheManager->UnMarkedQuery(CMSvSearchSortCacheManager::Instance()->QueryExists(*iCacheEntry));
	ASSERT_TRUE(iCacheEntry->iMarkQuery == FALSE );
	CleanupStack::PopAndDestroy(1); // Query
	}
	
	
void CTestSearchSortCacheManager::TestOnDemandUpdateCacheEntryL()
	{
	_LIT(KFunction, "TestOnDemandUpdateCacheEntryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	iSearchQuery->iParentId = 104325;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt iSearchString14 =33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);

	TInt tt = iSearchSortCacheManager->DoProcessQueryL(iSearchSortCacheManager->QueryExists(*iCacheEntry));
	ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy(1); // Query
	}

	
void CTestSearchSortCacheManager::TestResultCountL()
	{
	//ResultCount
	_LIT(KFunction, "TestResultCountL");
	INFO_PRINTF1(KFunction);
	
	TInt err= EFalse;
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	
	iSearchQuery->iParentId = 1023460;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt iSearchString14 = 33;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);

	TInt tt = iSearchSortCacheManager->ResultCountL(iSearchSortCacheManager->QueryExists(*iCacheEntry));
	ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestFinalResultAsIdL()
	{
	
	_LIT(KFunction, "TestFinalResultAsIdL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	

	iSearchQuery->iParentId = 10234780;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt  iSearchString14 = 33 ;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo,EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);

	TInt tt = iSearchSortCacheManager->FinalResultAsIdL(*iCacheEntry);
	ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy(1); // Query
	}


void CTestSearchSortCacheManager::TestStoreResultL()
	{
	_LIT(KFunction, "TestStoreResultL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);


	iSearchQuery->iParentId =2345800;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);


	TInt iSearchString14 =33;  
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);

	RArray<TMsvId> idlist;
	iSearchSortCacheManager->StoreResultL(1,idlist);
	
	RArray<TMsvIdWithSortField> Sortidlist;
	iSearchSortCacheManager->StoreSortResultL(1,Sortidlist);
	
	Sortidlist.Close();
	idlist.Close();
	ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy(1); // Query
	}
	
	

void CTestSearchSortCacheManager::TestDoProcessQueryL()
	{
	_LIT(KFunction, "TestDoProcessQueryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	
	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	iSearchQuery->iParentId = 100788;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	_LIT(KEMsgSize1, "33");
	TBufC16<30> iSearchString14(KEMsgSize1);
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	iSearchQuery->iQueryCount = 4;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	
	TInt tt = iSearchSortCacheManager->DoProcessQueryL(*iCacheEntry ,0);
	ASSERT_EQUALS(err, KErrNone);
	
	iSearchSortCacheManager->ReturnProgressInfo();
	iSearchSortCacheManager->CancelSearchSortOperation();
	iSearchSortCacheManager->FirstResultForInteraratorQueryIdL(iSearchSortCacheManager->QueryExists(*iCacheEntry));
	TRAP(err,iSearchSortCacheManager->NextResultForInteraratorL(iSearchSortCacheManager->QueryExists(*iCacheEntry))) ;
	ASSERT_EQUALS(err, -1);
	CleanupStack::PopAndDestroy(1); // Query
	}
	
void CTestSearchSortCacheManager::TestRemoveEntryL()
	{
	
	_LIT(KFunction, "TestRemoveEntryL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;

	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);

	iSearchQuery->iParentId = 6009;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	TInt iSearchString14 = 33 ;
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	
	//#Required size
	TInt requiredsize =sizeof(*iCacheEntry);

	iSearchSortCacheManager->RemoveEntry(requiredsize);
	CleanupStack::PopAndDestroy(1); // Query
	}
	
	
void CTestSearchSortCacheManager::TesUpdateQueryResultOnDemandInDBL()
	{
	_LIT(KFunction, "TesUpdateQueryResultOnDemandInDBL");
	INFO_PRINTF1(KFunction);

	TInt err= EFalse;
	
	CMsvSearchSortDeltaCache::Instance()->iDeltaCacheDirtyFlag = ETrue;

	CMsvSearchSortQuery* iSearchQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(iSearchQuery);
	
	
	iSearchQuery->iParentId = 122100;
	iSearchQuery->iCaseSensitive = ETrue;
	iSearchQuery->iWholeWord =EFalse ;
	iSearchQuery->iWildCardCharacter = EFalse;
	iSearchQuery->iExplicitSort = ETrue;
	iSearchQuery->iSortOnMessagePart = EMsvSize;
	
	_LIT(KSearchString1, "TEST");
	TBufC16<4> iSearchString11(KSearchString1);
	iSearchQuery->AddSearchOptionL(EMsvDescription, iSearchString11, EMsvEqual);
	
	_LIT(KEEMsgTo1, "TESTEMsgTo");
	TBufC16<30> iSearchString12(KEEMsgTo1);
	iSearchQuery->AddSearchOptionL(EMsvTo, iSearchString12, EMsvEqual);

	_LIT(KEMsgFromString1, "TESTEMsgFrom");
	TBufC16<40> iSearchString13(KEMsgFromString1);
	iSearchQuery->AddSearchOptionL(EMsvFrom, iSearchString13, EMsvEqual);

	_LIT(KEMsgSize1, "33");
	TBufC16<30> iSearchString14(KEMsgSize1);
	iSearchQuery->AddSearchOptionL(EMsvSize, iSearchString14, EMsvEqual);

	_LIT(KEMsgDate, "20080423:");
	TBufC16<30> iSearchString15(KEMsgDate);
	iSearchQuery->AddSearchOptionL(EMsvDate, iSearchString15, EMsvEqual);
	iSearchQuery->iQueryCount = 5;
	iSearchQuery->AddSortOptionL(EMsvTo, EMsvSortAscending );
	CMsvSearchSortCacheEntry* iCacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(iSearchQuery, EFalse);
	//ADDENTRY :-
	TRAP(err,iSearchSortCacheManager->AddEntryL(*iCacheEntry));

	RArray<TMsvId> aNewEntry;
	RArray<TMsvId> aUpdateEntry;
	RArray<TMsvId> aDeleteEntry;

	iSearchSortCacheManager->UpdateQueryResultOnDemandInDBL(*iCacheEntry,aNewEntry,aUpdateEntry,aDeleteEntry);
	iSearchSortCacheManager->ResultCountL(*iCacheEntry);
	TRAP(err,iSearchSortCacheManager->FirstResultForInteraratorNewQueryL(*iCacheEntry));
	ASSERT_EQUALS(err,KErrNone);
	iSearchSortCacheManager->FirstResultForInteraratorQueryIdL(iSearchSortCacheManager->QueryExists(*iCacheEntry));
	TRAP(err,iSearchSortCacheManager->ReturnIdCountL(iSearchSortCacheManager->QueryExists(*iCacheEntry)));
	ASSERT_EQUALS(err,KErrNone);

	aNewEntry.Close();
	aUpdateEntry.Close();
	aDeleteEntry.Close();
	CleanupStack::PopAndDestroy(1); // Query
	}
	
	
CTestSuite* CTestSearchSortCacheManager::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestCreateManagerL);
	END_SUITE;
	}

