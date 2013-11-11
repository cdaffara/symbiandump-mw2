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
// t_msv_search_sort_dbadapter_step.cpp

#include "t_msv_search_sort_dbadapter_step.h"
#include <e32svr.h>

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
#include "cactivewaiter.h"
#include "msvsearchsortoperation.h"
#include "msvdbadapter.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

//TInt query id
//#define QUERYID 12343545575
const TInt K_QUERYID = 12343545;
#define MAX_IN_COUNT 50//ref. development code

 
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
_LIT(KTestDbFileName, "c:\\messaging.db"); //c:[1000484b]testDB.db
_LIT(KMainDB, "c:\\messaging_master.db");


class CSessionObserverDb : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iEvent;
	};

void CSessionObserverDb::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	}


CTestSearchSortDBadapter::CTestSearchSortDBadapter()
	{
	}

CTestSearchSortDBadapter ::~CTestSearchSortDBadapter()
	{
	
	}


void CTestSearchSortDBadapter::SetupL()
	{
	_LIT(KSetupL,"SetupL");
	INFO_PRINTF1(KSetupL);
	
	TFileName dbName(KTestDbFileName);
	
		
	RSqlDatabase::Delete(dbName);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	TFileName DBfilename(KMainDB);
	
	//Delete if the Db is still existing
	TInt error = RSqlDatabase::Delete(DBfilename);
#endif  //SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT 
	
	
	//Create , Intialize and open the database
	iServer = CMsvServer::NewL();
	iDBadapter = iServer->Context().IndexAdapter()->GetDbAdapter();
	
	CMSvSearchSortCacheManager::Instance()->InstantiateDBwrapperL();
	iSearchDbAdapter=CMSvSearchSortCacheManager::Instance()->GetDbWrapper();
	}

void CTestSearchSortDBadapter::TearDownL()
	{
	_LIT(KTearDownL,"TearDownL");
	INFO_PRINTF1(KTearDownL);
	delete iServer;
	}
	
	
	
void CTestSearchSortDBadapter::CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible /* (Default = EFalse)*/, TBool isPreparationFlag /*(Default=EFalse)*/, TBool iTestValue/*(Default=EFalse)*/)
	{
	TMsvEntry sampleTMsvEntry;
	
	sampleTMsvEntry.SetId(aId);
	sampleTMsvEntry.SetParent(aParentId);
	
	sampleTMsvEntry.SetVisible(aIsVisible);
	sampleTMsvEntry.SetVisibleFolderFlag(aIsVisible);
	
	// Extra Information (Needed only in TestGetEntry()
	if(iTestValue)
		{	
		sampleTMsvEntry.SetUnread(ETrue);
		sampleTMsvEntry.SetAttachment(ETrue);
		sampleTMsvEntry.SetNew(ETrue);
		sampleTMsvEntry.SetPriority(EMsvLowPriority);
		sampleTMsvEntry.iSize = 64;
		sampleTMsvEntry.iDate = 1234;
		sampleTMsvEntry.iMtm = KUidMsvServiceEntry;
		}	
	else
		{
		sampleTMsvEntry.SetUnread(EFalse);
		sampleTMsvEntry.SetAttachment(EFalse);
		sampleTMsvEntry.SetNew(EFalse);
		sampleTMsvEntry.SetPriority(EMsvMediumPriority);
		sampleTMsvEntry.iSize = 32;
		sampleTMsvEntry.iDate.UniversalTime();
		sampleTMsvEntry.iMtm = aUid;
		}
		
	sampleTMsvEntry.iType = aUid;
	sampleTMsvEntry.iServiceId = aId+10;		// Some Random Id.
	sampleTMsvEntry.iRelatedId = aId+20;		// Some Random Id.
			

	sampleTMsvEntry.iError = NULL;		
	sampleTMsvEntry.iBioType = 64;		
	sampleTMsvEntry.iMtmData1 = 64;		
	sampleTMsvEntry.iMtmData2 = 64;		
	sampleTMsvEntry.iMtmData3 = 64;	
	sampleTMsvEntry.SetInPreparation(isPreparationFlag);
	
	if(iTestValue)
		{
		_LIT(KName, "TEST");
		HBufC* data = HBufC::NewL(15); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
		
		iDBadapter->CreateEntryL(sampleTMsvEntry, aVisibleFolder);	
		
		delete data;
		data = NULL;
		}
	else
		{
		_LIT(KName, "Hello World");
		HBufC* data = HBufC::NewL(15); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
		
		iDBadapter->CreateEntryL(sampleTMsvEntry, aVisibleFolder);	
		
		delete data;
		data = NULL;
		}
	}
	
void CTestSearchSortDBadapter::CreateSortEntriesL(TInt Iter)
	{
	TMsvEntry sampleTMsvEntry;
	TMsvId visibleFolder = NULL;

	sampleTMsvEntry.SetId(KFirstFreeEntryId + Iter);
	sampleTMsvEntry.SetParent(KMsvRootIndexEntryIdValue);
	

		
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	
	// Extra Information (Needed only in TestGetEntry()
	sampleTMsvEntry.SetUnread(ETrue);
	sampleTMsvEntry.SetAttachment(ETrue);
	sampleTMsvEntry.SetNew(ETrue);
	sampleTMsvEntry.SetPriority(EMsvHighPriority);
	sampleTMsvEntry.iSize = 64+Iter;
	sampleTMsvEntry.iDate = 1234+Iter;
	sampleTMsvEntry.iServiceId = KMsvRootIndexEntryIdValue+10;		// Some Random Id.
	sampleTMsvEntry.iRelatedId = KMsvRootIndexEntryIdValue+20;		// Some Random Id.
		

	sampleTMsvEntry.iError = NULL;		
	sampleTMsvEntry.iBioType = 64;		
	sampleTMsvEntry.iMtmData1 = 64;		
	sampleTMsvEntry.iMtmData2 = 64;		
	sampleTMsvEntry.iMtmData3 = 64;	
	sampleTMsvEntry.SetInPreparation(EFalse);
	

	if((Iter%3) == 0 )
		{
		sampleTMsvEntry.iType = KUidMsvMessageEntry;
		
		_LIT(KName, "ABC");
		HBufC* data = HBufC::NewL(3); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
		sampleTMsvEntry.iMtm = KUidMsvFolderEntry;
		iDBadapter->CreateEntryL(sampleTMsvEntry, visibleFolder);	
		
		delete data;
		data = NULL;
		
		}
	else if((Iter%3) == 1 )
		{
		sampleTMsvEntry.iType = KUidMsvMessageEntry;
		
		_LIT(KName, "GHI");
		HBufC* data = HBufC::NewL(3); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
		sampleTMsvEntry.iMtm = KUidMsvAttachmentEntry;
		iDBadapter->CreateEntryL(sampleTMsvEntry, visibleFolder);
		
		delete data;
		data = NULL;
		}
	else
		{
		sampleTMsvEntry.iType = KUidMsvMessageEntry;
		
		_LIT(KName, "DEF");
		HBufC* data = HBufC::NewL(3); 
		*data = KName;
		sampleTMsvEntry.iDescription.Set(data->Des());
		sampleTMsvEntry.iDetails.Set(data->Des());
		sampleTMsvEntry.iMtm = KUidMsvMessageEntry;
		iDBadapter->CreateEntryL(sampleTMsvEntry, visibleFolder);
		
		delete data;
		data = NULL;		
		}
	
	}
		
/**
 * TestDoAddRelationalOperator()
 * @param none.
 * @return none.
 *
 * This Function Tests the DoAddRelationalOperator Functionalities
 */
void CTestSearchSortDBadapter::TestDoAddRelationalOperatorL()
	{
	_LIT(KFunction,"TestDoAddRelationalOperator");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatBefore, "Before Appending Relational Operator - [%S]");
	_LIT(KFormatAfter, "After Appending Relational Operator - [%S]");
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf16 printString;
	
	printString.CleanupClosePushL();
	whereClause.CleanupClosePushL();
		
	printString.CreateL(20);
	whereClause.CreateL(20);
		
	// 01. Check the EMsvEqual Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	TRAPD(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvEqual));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	
	// 02. Check the EMsvNotEqual Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatBefore, &printString);
		
	TRAP(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvNotEqual));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
	

	// 03. Check the EMsvLessThan Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatBefore, &printString);
		
	TRAP(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvLessThan));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	
	// 04. Check the EMsvGreaterThan Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatBefore, &printString);
		
	iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvGreaterThan);
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	// 05. Check the EMsvLessThanOrEqual Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatBefore, &printString);
		
	TRAP(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvLessThanOrEqual));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	
	// 06. Check the EMsvGreaterThanOrEqual Relational Operator	
	whereClause.Copy(_L("TEMP"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatBefore, &printString);
		
	TRAP(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,EMsvGreaterThanOrEqual));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
	

	// 07. Check other Value
	TRAP(err,iSearchDbAdapter->DoAddRelationalOperatorL(whereClause,TMsvRelationOp(7)));
	MY_ASSERT_EQUALS(err,KErrNotSupported);

	CleanupStack::PopAndDestroy(2);
	}


/**
 * TestIsSortAsNumRequired()
 * @param none.
 * @return none.
 *
 * This Function Tests the IsSortAsNumRequired Functionalities
 */
void CTestSearchSortDBadapter::TestIsSortAsNumRequired()
	{
	_LIT(KFunction,"TestIsSortAsNumRequired");
	INFO_PRINTF1(KFunction);
	
	TMsvMessagePart aMessagePart;
	
	aMessagePart = EMsvTo;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvFrom;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvCc;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvBcc;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvDetails;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvDescription;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvSubject;
	MY_ASSERT_FALSE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvDate;
	MY_ASSERT_TRUE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvSize;
	MY_ASSERT_TRUE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	
	aMessagePart = EMsvMtmTypeUID;
	MY_ASSERT_TRUE(iSearchDbAdapter->IsSortAsNumRequired(aMessagePart));
	}


/**
 * TestAddIdtoDBL()
 * @param none.
 * @return none.
 *
 * This Function Tests the AddIdtoDBL Functionalities
 */
void CTestSearchSortDBadapter::TestAddIdtoDBL()
	{
	_LIT(KFunction,"TestAddIdtoDBL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
	RArray<TMsvId> idSelection;
		
	//01. Try to Add Null Array
	TRAPD(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 0);
		
	//02.Fill the Sort Data to be added into DB
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);
		#endif
		}
		
	sortedDataArray.Close();
	idSelection.Close();
	}


/**
 * TestGetSortedTMsvIdsfromTableL()
 * @param none.
 * @return none.
 *
 * This Function Tests the GetSortedTMsvIdsfromTableL Functionalities
 */
void CTestSearchSortDBadapter::TestGetSortedTMsvIdsfromTableL()
	{
	_LIT(KFunction,"TestGetSortedTMsvIdsfromTableL");
	INFO_PRINTF1(KFunction);

	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt queryId2 = K_QUERYID+666;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
	RArray<TMsvId> idSelection;
		
	//01. Try to Get a Non-Existing Query
	TRAPD(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), NULL);
	
	//02. Fetch a Single Entry
	sortData.iMessageId = KFirstFreeEntryId;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 1);
	
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		ASSERT_EQUALS(KFirstFreeEntryId, UnmaskTMsvId(idSelection[NULL]));
	#else
		ASSERT_EQUALS(KFirstFreeEntryId, idSelection[NULL]);
	#endif
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//03.Fetch Entries in Ascending Order
	for(TInt index = 1; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);		
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//04.Fetch Entries in Descending Order
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//05.Fetch Entries in Descending Order sortaschar
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection, EFalse, EMsvFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//06.Fetch Entries in Descending Order sortaschar
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection, EFalse, EMsvFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);	
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//07. Fetch Scatterd Entries
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 100; index < 200; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 200);
	
	for(TInt index = 0; index < 200; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[index]);
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	sortedDataArray.Close();
	idSelection.Close();
	}


/**
 * TestGetLastNTMsvIdsfromTableL()
 * @param none.
 * @return none.
 *
 * This Function Tests the GetLastNTMsvIdsfromTableL Functionalities
 */
void CTestSearchSortDBadapter::TestGetLastNTMsvIdsfromTableL()
	{
	_LIT(KFunction,"TestGetLastNTMsvIdsfromTableL");
	INFO_PRINTF1(KFunction);

	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt counter = 0;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
	RArray<TMsvId> idSelection;
		
	//01. Try to Get a Non-Existing Query
	TRAPD(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), NULL);

	//02. Fetch a Single Entry
	sortData.iMessageId = KFirstFreeEntryId;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 1));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 1);
	
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		ASSERT_EQUALS(KFirstFreeEntryId, UnmaskTMsvId(idSelection[NULL]));
	#else
		ASSERT_EQUALS(KFirstFreeEntryId, idSelection[NULL]);
	#endif
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//03.Fetch Entries in Ascending Order
	for(TInt index = 1; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 10));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 10);
	
	for(TInt index = 90; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
		
	//Check if the Ids are present in DB
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 30));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 30);
	
	for(TInt index = 70; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
		
	//04.Fetch Entries in Descending Order
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 10, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 10);
	
	for(TInt index = 90; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
	
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 60, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 60);
	
	for(TInt index = 40; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//05.Fetch Entries in Descending Order sortaschar
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 10, EFalse, EMsvFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 10);
	
	for(TInt index = 90; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
	
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 70, EFalse, EMsvFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 70);
	
	for(TInt index = 30; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//06.Fetch Entries in Descending Order sortaschar
	TRAP(err,iDBadapter->GetLastNTMsvIdsfromTableL(queryId, idSelection, 70, EFalse, EMsvFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 70);
	
		for(TInt index = 30; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index, UnmaskTMsvId(idSelection[counter]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index, idSelection[counter]);
		#endif
		++counter;
		}
	counter = 0;
	sortedDataArray.Reset();
	idSelection.Reset();
	}

/**
 * TestReturnResultCountInTableL()
 * @param none.
 * @return none.
 *
 * This Function Tests the ReturnResultCountInTableL Functionalities
 */
void CTestSearchSortDBadapter::TestReturnResultCountInTableL()
	{
	_LIT(KFunction,"TestReturnResultCountInTableL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt queryId2 = K_QUERYID+666;
	TInt countOfEntries;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
			
	//01. Get Count on a Non-Existing Entry
	TRAPD(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	
	//02. Get Count on a Single Entry
	sortData.iMessageId = KFirstFreeEntryId;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 1);
	
	sortedDataArray.Reset();
	
	//03.Get 100 Entry count
	for(TInt index = 1; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	sortedDataArray.Reset();
	
	//04. Get Scattered Entry count
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 100; index < 200; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 200);
	
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	sortedDataArray.Reset();
	sortedDataArray.Close();
	}


/**
 * TestDeleteQueryFromTableL()
 * @param none.
 * @return none.
 *
 * This Function Tests the ReturnResultCountInTableL Functionalities
 */
void CTestSearchSortDBadapter::TestDeleteQueryFromTableL()
	{
	_LIT(KFunction,"TestDeleteQueryFromTableL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt queryId2 = K_QUERYID+666;
	TInt countOfEntries;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
			
	//01. Delete a Non-Existing Query
	TRAPD(err,iSearchDbAdapter->DeleteQueryFromTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	//02. Delete a Single Entry Query
	sortData.iMessageId = KFirstFreeEntryId;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Delete and Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 1);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	
	sortedDataArray.Reset();
	
	//03.Delete a Query having 100 ids
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	
	sortedDataArray.Reset();
	
	//04. Delete a Query having scattered entries
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 0; index < 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 100; index < 200; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 200);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	sortedDataArray.Reset();
	sortedDataArray.Close();
	}


/**
 * TestDeleteTMsvIdFromAllQueriesL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DeleteTMsvIdFromAllQueriesL Functionalities
 */
void CTestSearchSortDBadapter::TestDeleteTMsvIdFromAllQueriesL()
	{
	_LIT(KFunction,"TestDeleteTMsvIdFromAllQueriesL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt queryId2 = K_QUERYID+666;
	TInt countOfEntries = 0;
	TInt idIndex = 0;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
	RArray<TMsvId> idSelection;
			
	//01. Delete a Null array
	TRAPD(err,iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//02. Delete a Non Existing id
	idSelection.Append(KFirstFreeEntryId);

	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	idSelection.Reset();
	
	//03.Delete an Array of entries having all existant entries
	for(TInt index = 1; index <= 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		
		if(index%2 == 0)
			{
			idSelection.Append(KFirstFreeEntryId+index);
			}
		}
	
	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	//Delete the Ids from the DB
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 50);
	idIndex = 1;
	
	for(TInt index = 0; index < 50; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, idSelection[index]);
		#endif
		idIndex+=2;
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	
	//04.Delete an Array of entries having non-existant entries
	for(TInt index = 1; index <= 100; ++index)
		{
		idSelection.Append(KFirstFreeEntryId+index);
		}
	idSelection.Append(KFirstFreeEntryId);
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 50);
	
	//Delete the Ids from the DB
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);

	sortedDataArray.Reset();
	idSelection.Reset();
	
	//05. Delete entries on a Scattered DB
	for(TInt index = 1; index < 50; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		
		if(index%2 == 0)
			{
			idSelection.Append(KFirstFreeEntryId+index);
			}
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
		
	for(TInt index = 1; index <= 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 50; index <= 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		
		if(index%2 == 0)
			{
			idSelection.Append(KFirstFreeEntryId+index);
			}
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	//Delete the Ids from the DB
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 50);
	idIndex = 1;
	
	for(TInt index = 0; index < 50; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, idSelection[index]);
		#endif
		idIndex+=2;
		}

	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId2, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 50);
	idIndex = 1;
	
	for(TInt index = 0; index < 50; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, idSelection[index]);
		#endif
		idIndex+=2;
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	sortedDataArray.Close();
	idSelection.Close();
	}


/**
 * TestDeleteTMsvIdFromQueryL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DeleteTMsvIdFromQueryL Functionalities
 */
void CTestSearchSortDBadapter::TestDeleteTMsvIdFromQueryL()
	{
	_LIT(KFunction,"TestDeleteTMsvIdFromQueryL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSortedField,"This is A Description to Fill in 50 Characters !!");
	
	TInt queryId = K_QUERYID;
	TInt queryId2 = K_QUERYID+666;
	TInt countOfEntries = 0;
	TInt idIndex = 0;
	TMsvIdWithSortField sortData;
	RArray<TMsvIdWithSortField> sortedDataArray;
	RArray<TMsvId> idSelection;
		
	//01. Delete a Non Existing QueryId
	idSelection.Append(KFirstFreeEntryId);
	TRAPD(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	idSelection.Reset();
	
	//02. Delete a Null array
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//03. Delete a TMsvid present under different Query Id
	sortData.iMessageId = KFirstFreeEntryId;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	sortData.iMessageId = KFirstFreeEntryId+1;
	sortData.iContentMessagePart = KSortedField;
	sortedDataArray.Append(sortData);

	//Add the data to DB
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//Delete and Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 1);
	
	idSelection.Append(KFirstFreeEntryId);
	
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId2, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 1);
	
	sortedDataArray.Reset();
	
	//04. Delete a TMsvid present under the same queryid
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	
	idSelection.Reset();
	idSelection.Append(KFirstFreeEntryId+1);
	
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId2, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, NULL);
	idSelection.Reset();
	
	//05. Delete entries on a Scattered DB
	for(TInt index = 1; index < 50; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		
		if(index%2 == 0)
			{
			idSelection.Append(KFirstFreeEntryId+index);
			}
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
		
	for(TInt index = 1; index <= 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId2, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	for(TInt index = 50; index <= 100; ++index)
		{
		sortData.iMessageId = KFirstFreeEntryId+index;
		sortData.iContentMessagePart = KSortedField;
		sortedDataArray.Append(sortData);
		
		if(index%2 == 0)
			{
			idSelection.Append(KFirstFreeEntryId+index);
			}
		}
	
	//Add the data to DB	
	TRAP(err,iSearchDbAdapter->AddIdtoDBL(queryId, sortedDataArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	sortedDataArray.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	TRAP(err,countOfEntries=iSearchDbAdapter->ReturnResultCountInTableL(queryId2));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(countOfEntries, 100);
	
	//Delete the Ids from the DB
	TRAP(err,iSearchDbAdapter->DeleteTMsvIdFromQueryL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 50);
	idIndex = 1;
	
	for(TInt index = 0; index < 50; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+idIndex, idSelection[index]);	
		#endif
		idIndex+=2;
		}

	idSelection.Reset();
	
	//Check if the Ids are present in DB
	TRAP(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL(queryId2, idSelection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(idSelection.Count(), 100);
	
	for(TInt index = 1; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(idSelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index+1, idSelection[index]);	
		#endif
		}
		
	sortedDataArray.Reset();
	idSelection.Reset();
	sortedDataArray.Close();
	idSelection.Close();
	}
	
	
/**
 * TestDoAppendIdConstraintL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DoAppendIdConstraint Functionalities
 */
void CTestSearchSortDBadapter::TestDoAppendIdConstraintL()
	{
	_LIT(KFunction,"TestDoAppendIdConstraint");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatAfter, "After Appending ID Constraints - [%S]");
	
	RArray<TMsvId> idSelection;
	TInt referenceCount;
	TInt initQuerySize;
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf printString;
	
	printString.CleanupClosePushL();	
	whereClause.CleanupClosePushL();
		
	printString.CreateL(500);
	whereClause.CreateL(500);
			
	// 01. Append Empty Array
	whereClause.Copy(_L("CREATED UPDATE QUERY - "));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	TRAPD(err,iSearchDbAdapter->DoAppendIdConstraint(idSelection, whereClause, referenceCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	

			
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	
	
	//01. Append a Single entry
	whereClause.Copy(_L("CREATED UPDATE QUERY - "));
	referenceCount = NULL;

	idSelection.Append(KFirstFreeEntryId);
	
	TRAP(err,iSearchDbAdapter->DoAppendIdConstraint(idSelection, whereClause, referenceCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(referenceCount, 1);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	idSelection.Reset();
	
	//03. Append less than MAX_IN_COUNT(ref. Development code) entries
	whereClause.Copy(_L("CREATED UPDATE QUERY - "));
	referenceCount = NULL;
		
	for(TInt index = 0; index < MAX_IN_COUNT-10; ++index)
		{
		idSelection.Append(KFirstFreeEntryId+index);
		}
	
	TRAP(err,iSearchDbAdapter->DoAppendIdConstraint(idSelection, whereClause, referenceCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(referenceCount, MAX_IN_COUNT-10);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	idSelection.Reset();
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	
	//04. Append Exact MAX_IN_COUNT(ref. Development code) no. of entries
	whereClause.Copy(_L("CREATED UPDATE QUERY - "));
	referenceCount = NULL;
		
	for(TInt index = 0; index < MAX_IN_COUNT; ++index)
		{
		idSelection.Append(KFirstFreeEntryId+index);
		}
	
	TRAP(err,iSearchDbAdapter->DoAppendIdConstraint(idSelection, whereClause, referenceCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(referenceCount, MAX_IN_COUNT);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	idSelection.Reset();
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	
	//05. Append More MAX_IN_COUNT(ref. Development code) no. of entries
	whereClause.Copy(_L("CREATED UPDATE QUERY - "));
	referenceCount = NULL;
		
	for(TInt index = 0; index < MAX_IN_COUNT+10; ++index)
		{
		idSelection.Append(KFirstFreeEntryId+index);
		}
	
	initQuerySize = whereClause.Size();//Remove Delimiter
	
	while(referenceCount < idSelection.Count())
		{
		TRAP(err,iSearchDbAdapter->DoAppendIdConstraint(idSelection, whereClause, referenceCount));
		MY_ASSERT_EQUALS(err, KErrNone);
		convBufa.Copy(whereClause);
		convBufb.Copy(convBufa);
		printString.Copy(convBufb);
		INFO_PRINTF2(KFormatAfter, &printString);
		printString.Delete(0,500);
		whereClause.Delete(initQuerySize,500);
		}
	
	idSelection.Reset();
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	
	idSelection.Close();
	CleanupStack::PopAndDestroy(2);
	}
		

/**
 * TestDoCreateInsertSortQueryL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DoCreateInsertSortQueryL Functionalities
 */
void CTestSearchSortDBadapter::TestDoCreateInsertSortQueryL()
	{
	_LIT(KFunction,"TestDoCreateInsertSortQueryL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatAfter, "INSERT QUERY formed is - [%S]");
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf16 printString;
	
	printString.CleanupClosePushL();	
	whereClause.CleanupClosePushL();
		
	printString.CreateL(500);
	whereClause.CreateL(500);
	
	
	// 03. Insert Query for EMsvSize
	//Create the Query as per requirement
	CMsvSearchSortQuery* query2 = CMsvSearchSortQuery::NewL();
	query2->iParentId = KMsvRootIndexEntryIdValue;
	query2->SetParentId(KFirstFreeEntryId);
	query2->AddSortOptionL(EMsvSize, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry2;
	cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query2, EFalse, 10);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)			
	iSearchDbAdapter->iTypeOfQuery = EQueryOnIndexEntry;
#endif
		
	TRAPD(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry2, whereClause, query2->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry2;
	delete query2;
	
	// 04. Insert Query for EMsvDate
	//Create the Query as per requirement
	CMsvSearchSortQuery* query3 = CMsvSearchSortQuery::NewL();
	query3->iParentId = KMsvRootIndexEntryIdValue;
	query3->SetParentId(KFirstFreeEntryId);
	query3->AddSortOptionL(EMsvDate, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry3;
	cacheEntry3 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query3, EFalse, 10);
		
	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry3, whereClause, query2->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry3;
	delete query3;
	
	// 05. Insert Query for EMsvDetails
	//Create the Query as per requirement
	CMsvSearchSortQuery* query4 = CMsvSearchSortQuery::NewL();
	query4->iParentId = KMsvRootIndexEntryIdValue;
	query4->SetParentId(KFirstFreeEntryId);
	query4->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry4;
	cacheEntry4 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query4, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry4, whereClause, query4->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry4;
	delete query4;
	
	// 06. Insert Query for EMsvDescription
	//Create the Query as per requirement
	CMsvSearchSortQuery* query5 = CMsvSearchSortQuery::NewL();
	query5->iParentId = KMsvRootIndexEntryIdValue;
	query5->SetParentId(KFirstFreeEntryId);
	query5->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry5;
	cacheEntry5 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query5, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry5, whereClause, query5->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry5;
	delete query5;
	
	// 07. Insert Query for EMsvDescription
	//Create the Query as per requirement
	CMsvSearchSortQuery* query6 = CMsvSearchSortQuery::NewL();
	query6->iParentId = KMsvRootIndexEntryIdValue;
	query6->SetParentId(KFirstFreeEntryId);
	query6->AddSortOptionL(EMsvCc, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry6;
	cacheEntry6 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query6, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry6, whereClause, query6->iParentId));
	MY_ASSERT_EQUALS(err, KErrNotSupported);
	
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry6;
	delete query6;

	// 10. Insert Query for EMsvSize
	//Create the Query as per requirement
	CMsvSearchSortQuery* query9 = CMsvSearchSortQuery::NewL();
	query9->iParentId = KMsvRootIndexEntryIdValue;
	query9->SetParentId(KMsvRootIndexEntryIdValue);
	query9->AddSortOptionL(EMsvSize, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry9;
	cacheEntry9 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query9, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry9, whereClause, query9->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry9;
	delete query9;
	
	// 11. Insert Query for EMsvDate
	//Create the Query as per requirement
	CMsvSearchSortQuery* query10 = CMsvSearchSortQuery::NewL();
	query10->iParentId = KMsvRootIndexEntryIdValue;
	query10->SetParentId(KMsvRootIndexEntryIdValue);
	query10->AddSortOptionL(EMsvDate, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry10;
	cacheEntry10 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query10, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry10, whereClause, query10->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry10;
	delete query10;
	
	// 12. Insert Query for EMsvDetails
	//Create the Query as per requirement
	CMsvSearchSortQuery* query11 = CMsvSearchSortQuery::NewL();
	query11->iParentId = KMsvRootIndexEntryIdValue;
	query11->SetParentId(KMsvRootIndexEntryIdValue);
	query11->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry11;
	cacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query11, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry11, whereClause, query11->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry11;
	delete query11;
	
	// 13. Insert Query for EMsvDescription
	//Create the Query as per requirement
	CMsvSearchSortQuery* query12 = CMsvSearchSortQuery::NewL();
	query12->iParentId = KMsvRootIndexEntryIdValue;
	query12->SetParentId(KMsvRootIndexEntryIdValue);
	query12->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry12;
	cacheEntry12 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query12, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry12, whereClause, query12->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry12;
	delete query12;
	
	// 14. Insert Query for EMsvDescription
	//Create the Query as per requirement
	CMsvSearchSortQuery* query13 = CMsvSearchSortQuery::NewL();
	query13->iParentId = KMsvRootIndexEntryIdValue;
	query13->SetParentId(KMsvRootIndexEntryIdValue);
	query13->AddSortOptionL(EMsvCc, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry13;
	cacheEntry13 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query13, EFalse, 10);
	
	TRAPD(errr,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry13, whereClause, query13->iParentId));
	MY_ASSERT_EQUALS(errr, KErrNotSupported);
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry13;
	delete query13;
	
	// 15. Insert Query for EMsvDescription
	//Create the Query as per requirement
	CMsvSearchSortQuery* query15 = CMsvSearchSortQuery::NewL();
	query15->iParentId = KMsvRootIndexEntryIdValue;
	query15->SetParentId(KMsvRootIndexEntryIdValue);
	query15->AddSortOptionL(TMsvMessagePart(3), EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry15;
	cacheEntry15 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query15, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry15, whereClause, query15->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry15;
	delete query15;
	
	// 16. Insert Query for EMsvMtmTypeUID	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query16 = CMsvSearchSortQuery::NewL();
	query16->iParentId = KMsvRootIndexEntryIdValue;
	query16->SetParentId(KFirstFreeEntryId);
	query16->AddSortOptionL(EMsvMtmTypeUID, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry16;
	cacheEntry16 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query16, EFalse, 10);

	TRAP(err,iSearchDbAdapter->DoCreateInsertSortQueryL(*cacheEntry16, whereClause, query16->iParentId));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry16;
	delete query16;
	CleanupStack::PopAndDestroy(2);
	}
	
/**
 * TestReplaceWildCardCharactersL()
 * @param none.
 * @return none.
 *
 * This Function Tests the ReplaceWildCardCharacters Functionalities
 */
void CTestSearchSortDBadapter::TestReplaceWildCardCharactersL()
	{	
	_LIT(KFunction,"TestReplaceWildCardCharacters");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatAfter, "After Replacing Wild Cards - [%S]");
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf16 printString;
	
	printString.CleanupClosePushL();	
	whereClause.CleanupClosePushL();
		
	printString.CreateL(20);
	whereClause.CreateL(20);
		
	// 01. NULL data
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	
	CMsvSearchSortQuery* ssquery = CMsvSearchSortQuery::NewL();

	//Create the sscacheEntry from the ssquery
	CMsvSearchSortCacheEntry* sscacheEntry;
	sscacheEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(ssquery, EFalse, 10);

	TRAPD(err,iSearchDbAdapter->ReplaceWildCardCharacters(whereClause,*sscacheEntry));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
			
	// 02. Single *	
	whereClause.Copy(_L("*"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	TRAP(err,iSearchDbAdapter->ReplaceWildCardCharacters(whereClause,*sscacheEntry));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	// 03. Single ?	
	whereClause.Copy(_L("?"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);

	TRAP(err,iSearchDbAdapter->ReplaceWildCardCharacters(whereClause, *sscacheEntry));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	// 04. Combination of ? and *	
	whereClause.Copy(_L("?*??**?*"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	TRAP(err,iSearchDbAdapter->ReplaceWildCardCharacters(whereClause,*sscacheEntry));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
		
	// 04. Valid text and combinatin of ? and *	
	whereClause.Copy(_L("THIS*IS?A?TEST*"));
		
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
	
	TRAP(err,iSearchDbAdapter->ReplaceWildCardCharacters(whereClause,*sscacheEntry));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	printString.Delete(0,20);
	whereClause.Delete(0, 20);
	
	delete sscacheEntry;
	delete ssquery;
	CleanupStack::PopAndDestroy(2);
	}

/**
 * TestDoCreateSortQueryL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DoCreateSortQueryL Functionalities
 */
void CTestSearchSortDBadapter::TestDoCreateSortQueryL()
	{	
	_LIT(KFunction,"TestDoCreateSortQueryL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatAfter, "SORT QUERY formed is - [%S]");
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf16 printString;
	
	printString.CleanupClosePushL();	
	whereClause.CleanupClosePushL();
		
	printString.CreateL(500);
	whereClause.CreateL(500);
		
	// 05. Insert query5 for EMsvSize ASC	
	//Create the query5 as per requirement
	CMsvSearchSortQuery* query5 = CMsvSearchSortQuery::NewL();
	query5->iParentId = KMsvRootIndexEntryIdValue;
	query5->SetParentId(KFirstFreeEntryId);
	query5->AddSortOptionL(EMsvSize, EMsvSortAscending);

	//Create the cacheTEntry from the query5
	CMsvSearchSortCacheEntry* cacheEntry5;
	cacheEntry5 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query5, EFalse, 10);
	
	TRAPD(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry5, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry5;
	delete query5;
	
	// 06. Insert query5 for EMsvSize DESC	
	//Create the query5 as per requirement
	CMsvSearchSortQuery* query6 = CMsvSearchSortQuery::NewL();
	query6->iParentId = KMsvRootIndexEntryIdValue;
	query6->SetParentId(KFirstFreeEntryId);
	query6->AddSortOptionL(EMsvSize, EMsvSortDescending);
		
	//Create the cacheTEntry from the query5
	CMsvSearchSortCacheEntry* cacheEntry6;
	cacheEntry6 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query6, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry6, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry6;
	delete query6;
	
	// 07. Insert query7 for EMsvDate ASC	
	//Create the query7 as per requirement
	CMsvSearchSortQuery* query7 = CMsvSearchSortQuery::NewL();
	query7->iParentId = KMsvRootIndexEntryIdValue;
	query7->SetParentId(KFirstFreeEntryId);
	query7->AddSortOptionL(EMsvDate, EMsvSortAscending);

	//Create the cacheTEntry from the query7
	CMsvSearchSortCacheEntry* cacheEntry7;
	cacheEntry7 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query7, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry7, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry7;
	delete query7;
	
	// 08. Insert query7 for EMsvDate DESC	
	//Create the query7 as per requirement
	CMsvSearchSortQuery* query8 = CMsvSearchSortQuery::NewL();
	query8->iParentId = KMsvRootIndexEntryIdValue;
	query8->SetParentId(KFirstFreeEntryId);
	query8->AddSortOptionL(EMsvDate, EMsvSortDescending);
		
	//Create the cacheTEntry from the query7
	CMsvSearchSortCacheEntry* cacheEntry8;
	cacheEntry8 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query8, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry8, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry8;
	delete query8;
	
	// 09. Insert query9 for EMsvDetails ASC	
	//Create the query9 as per requirement
	CMsvSearchSortQuery* query9 = CMsvSearchSortQuery::NewL();
	query9->iParentId = KMsvRootIndexEntryIdValue;
	query9->SetParentId(KFirstFreeEntryId);
	query9->AddSortOptionL(EMsvDetails, EMsvSortAscending);

	//Create the cacheTEntry from the query9
	CMsvSearchSortCacheEntry* cacheEntry9;
	cacheEntry9 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query9, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry9, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry9;
	delete query9;
	
	// 10. Insert query9 for EMsvDetails DESC	
	//Create the query9 as per requirement
	CMsvSearchSortQuery* query10 = CMsvSearchSortQuery::NewL();
	query10->iParentId = KMsvRootIndexEntryIdValue;
	query10->SetParentId(KFirstFreeEntryId);
	query10->AddSortOptionL(EMsvDetails, EMsvSortDescending);
		
	//Create the cacheTEntry from the query9
	CMsvSearchSortCacheEntry* cacheEntry10;
	cacheEntry10 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query10, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry10, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry10;
	delete query10;
	
	// 11. Insert query11 for EMsvDescription ASC	
	//Create the query11 as per requirement
	CMsvSearchSortQuery* query11 = CMsvSearchSortQuery::NewL();
	query11->iParentId = KMsvRootIndexEntryIdValue;
	query11->SetParentId(KFirstFreeEntryId);
	query11->AddSortOptionL(EMsvDescription, EMsvSortAscending);

	//Create the cacheTEntry from the query11
	CMsvSearchSortCacheEntry* cacheEntry11;
	cacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query11, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry11, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry11;
	delete query11;
	
	// 12. Insert query11 for EMsvDescription DESC	
	//Create the query11 as per requirement
	CMsvSearchSortQuery* query12 = CMsvSearchSortQuery::NewL();
	query12->iParentId = KMsvRootIndexEntryIdValue;
	query12->SetParentId(KFirstFreeEntryId);
	query12->AddSortOptionL(EMsvDescription, EMsvSortDescending);
		
	//Create the cacheTEntry from the query11
	CMsvSearchSortCacheEntry* cacheEntry12;
	cacheEntry12 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query12, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry12, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry12;
	delete query12;
		
	// 20. Insert query11 for EMsvDescription DESC	
	//Create the query11 as per requirement
	CMsvSearchSortQuery* query20 = CMsvSearchSortQuery::NewL();
	query20->iParentId = KMsvRootIndexEntryIdValue;
	query20->SetParentId(KFirstFreeEntryId);
	query20->AddSortOptionL(TMsvMessagePart(3), EMsvSortDescending);
		
	//Create the cacheTEntry from the query11
	CMsvSearchSortCacheEntry* cacheEntry20;
	cacheEntry20 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query20, EFalse, 10);
	
	TRAP(err,iSearchDbAdapter->DoCreateSortQueryL(*cacheEntry20, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry20;
	delete query20;
	CleanupStack::PopAndDestroy(2);
	}


/**
 * TestDoCreateSearchQueryL()
 * @param none.
 * @return none.
 *
 * This Function Tests the DoCreateSearchQueryL Functionalities
 */
void CTestSearchSortDBadapter::TestDoCreateSearchQueryL()
	{	
	_LIT(KFunction,"TestDoCreateSearchQueryL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFormatAfter, "SEARCH QUERY formed is - [%S]");
	_LIT(KSearchString, "SearchString");
	TBufC<12> searchString(KSearchString);
	
	RBuf8 whereClause;
		
	TBuf8<500> convBufa;
	TBuf16<500> convBufb;
	
	RBuf16 printString;
	
	printString.CleanupClosePushL();	
	whereClause.CleanupClosePushL();
	
	printString.CreateL(500);
	whereClause.CreateL(500);
		
	// 01. Insert Query for EMsvNew	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query1 = CMsvSearchSortQuery::NewL();
	query1->iParentId = KMsvRootIndexEntryIdValue;
	query1->SetParentId(KFirstFreeEntryId);
	query1->AddSearchOptionL(EMsvNew, ETrue);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry1;
	cacheEntry1 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query1, EFalse, 10);
	
	TRAPD(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry1, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry1;
	delete query1;
	
	// 02. Insert Query for EMsvNew
	//Create the Query as per requirement
	CMsvSearchSortQuery* query2 = CMsvSearchSortQuery::NewL();
	query2->iParentId = KMsvRootIndexEntryIdValue;
	query2->SetParentId(KFirstFreeEntryId);
	query2->AddSearchOptionL(EMsvNew, EFalse);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry2;
	cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query2, EFalse, 20);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry2, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry2;
	delete query2;
	
	// 03. Insert Query for EMsvUnreadMessages	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query3 = CMsvSearchSortQuery::NewL();
	query3->iParentId = KMsvRootIndexEntryIdValue;
	query3->SetParentId(KFirstFreeEntryId);
	query3->AddSearchOptionL(EMsvUnreadMessages, ETrue);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry3;
	cacheEntry3 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query3, EFalse, 30);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry3, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry3;
	
	// 04. Insert Query for EMsvUnreadMessages	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query4 = CMsvSearchSortQuery::NewL();
	query4->iParentId = KMsvRootIndexEntryIdValue;
	query4->SetParentId(KFirstFreeEntryId);
	query4->AddSearchOptionL(EMsvUnreadMessages, EFalse);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry4;
	cacheEntry4 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query4, EFalse, 30);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry4, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry4;
	
	// 05. Insert Query for EMsgNormalPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query5 = CMsvSearchSortQuery::NewL();
	query5->iParentId = KMsvRootIndexEntryIdValue;
	query5->SetParentId(KFirstFreeEntryId);
	query5->AddSearchOptionL(EMsvPriority, 1, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry5;
	cacheEntry5 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query5, EFalse, 50);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry5, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry5;
	delete query5;
	
	// 06. Insert Query for EMsgNormalPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query6 = CMsvSearchSortQuery::NewL();
	query6->iParentId = KMsvRootIndexEntryIdValue;
	query6->SetParentId(KFirstFreeEntryId);
	query6->AddSearchOptionL(EMsvPriority, 1, EMsvNotEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry6;
	cacheEntry6 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query6, EFalse, 50);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry6, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry6;
	delete query6;
	
	// 07. Insert Query for EMsgLowPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query7 = CMsvSearchSortQuery::NewL();
	query7->iParentId = KMsvRootIndexEntryIdValue;
	query7->SetParentId(KFirstFreeEntryId);
	query7->AddSearchOptionL(EMsvPriority, 0, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry7;
	cacheEntry7 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query7, EFalse, 70);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry7, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry7;
	delete query7;
	
	// 08. Insert Query for EMsgLowPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query8 = CMsvSearchSortQuery::NewL();
	query8->iParentId = KMsvRootIndexEntryIdValue;
	query8->SetParentId(KFirstFreeEntryId);
	query8->AddSearchOptionL(EMsvPriority, 0, EMsvNotEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry8;
	cacheEntry8 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query8, EFalse, 70);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry8, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry8;
	delete query8;
	
	// 09. Insert Query for EMsgHighPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query9 = CMsvSearchSortQuery::NewL();
	query9->iParentId = KMsvRootIndexEntryIdValue;
	query9->SetParentId(KFirstFreeEntryId);
	query9->AddSearchOptionL(EMsvPriority, 2, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry9;
	cacheEntry9 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query9, EFalse, 90);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry9, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry9;
	delete query9;
	
	// 10. Insert Query for EMsgHighPriority 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query10 = CMsvSearchSortQuery::NewL();
	query10->iParentId = KMsvRootIndexEntryIdValue;
	query10->SetParentId(KFirstFreeEntryId);
	query10->AddSearchOptionL(EMsvPriority, 2, EMsvNotEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry10;
	cacheEntry10 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query10, EFalse, 90);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry10, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry10;
	delete query10;
	
	// 11. Insert Query for EMsvAttachment 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query11 = CMsvSearchSortQuery::NewL();
	query11->iParentId = KMsvRootIndexEntryIdValue;
	query11->SetParentId(KFirstFreeEntryId);
	query11->AddSearchOptionL(EMsvAttachment, ETrue);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry11;
	cacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query11, EFalse, 112);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry11, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry11;
	delete query11;
	
	// 12. Insert Query for EMsvAttachment 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query12 = CMsvSearchSortQuery::NewL();
	query12->iParentId = KMsvRootIndexEntryIdValue;
	query12->SetParentId(KFirstFreeEntryId);
	query12->AddSearchOptionL(EMsvAttachment, EFalse);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry12;
	cacheEntry12 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query12, EFalse, 112);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry12, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry12;
	delete query12;
	
	// 19. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query19 = CMsvSearchSortQuery::NewL();
	query19->iParentId = KMsvRootIndexEntryIdValue;
	query19->SetParentId(KFirstFreeEntryId);
	query19->AddSearchOptionL(EMsvSize, 57, EMsvEqual);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry19;
	cacheEntry19 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query19, EFalse, 192);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry19, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry19;
	delete query19;
	
	// 20. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query20 = CMsvSearchSortQuery::NewL();
	query20->iParentId = KMsvRootIndexEntryIdValue;
	query20->SetParentId(KFirstFreeEntryId);
	query20->AddSearchOptionL(EMsvSize, 57, EMsvNotEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry20;
	cacheEntry20 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query20, EFalse, 192);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry20, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry20;
	delete query20;
	
	// 21. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query21 = CMsvSearchSortQuery::NewL();
	query21->iParentId = KMsvRootIndexEntryIdValue;
	query21->SetParentId(KFirstFreeEntryId);
	query21->AddSearchOptionL(EMsvSize, 57, EMsvLessThan);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry21;
	cacheEntry21 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query21, EFalse, 212);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry21, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry21;
	delete query21;
	
	// 22. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query22 = CMsvSearchSortQuery::NewL();
	query22->iParentId = KMsvRootIndexEntryIdValue;
	query22->SetParentId(KFirstFreeEntryId);
	query22->AddSearchOptionL(EMsvSize, 57, EMsvGreaterThan);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry22;
	cacheEntry22 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query22, EFalse, 212);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry22, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry22;
	delete query22;
	
	// 23. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query23 = CMsvSearchSortQuery::NewL();
	query23->iParentId = KMsvRootIndexEntryIdValue;
	query23->SetParentId(KFirstFreeEntryId);
	query23->AddSearchOptionL(EMsvSize, 57, EMsvLessThanOrEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry23;
	cacheEntry23 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query23, EFalse, 232);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry23, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry23;
	delete query23;
	
	// 24. Insert Query for EMsvSize 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query24 = CMsvSearchSortQuery::NewL();
	query24->iParentId = KMsvRootIndexEntryIdValue;
	query24->SetParentId(KFirstFreeEntryId);
	query24->AddSearchOptionL(EMsvSize, 57, EMsvGreaterThanOrEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry24;
	cacheEntry24 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query24, EFalse, 232);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry24, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry24;
	delete query24;
	
	// 31. Insert Query for EMsvDetails 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query31 = CMsvSearchSortQuery::NewL();
	query31->iParentId = KMsvRootIndexEntryIdValue;
	query31->SetParentId(KFirstFreeEntryId);
	query31->SetWholeWord(ETrue);
	query31->AddSearchOptionL(EMsvDetails, searchString, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry31;
	cacheEntry31 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query31, EFalse, 312);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry31, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry31;
	delete query31;
	
	// 32. Insert Query for EMsvDetails 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query32 = CMsvSearchSortQuery::NewL();
	query32->iParentId = KMsvRootIndexEntryIdValue;
	query32->SetParentId(KFirstFreeEntryId);
	query32->SetWholeWord(EFalse);
	query32->AddSearchOptionL(EMsvDetails, searchString, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry32;
	cacheEntry32 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query32, EFalse, 312);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry32, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry32;
	delete query32;
	
	// 33. Insert Query for EMsvDescription 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query33 = CMsvSearchSortQuery::NewL();
	query33->iParentId = KMsvRootIndexEntryIdValue;
	query33->SetParentId(KFirstFreeEntryId);
	query33->SetWholeWord(ETrue);
	query33->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry33;
	cacheEntry33 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query33, EFalse, 334);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry33, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry33;
	delete query33;
	
	// 34. Insert Query for EMsvDescription 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query34 = CMsvSearchSortQuery::NewL();
	query34->iParentId = KMsvRootIndexEntryIdValue;
	query34->SetParentId(KFirstFreeEntryId);
	query34->SetWholeWord(EFalse);
	query34->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry34;
	cacheEntry34 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query34, EFalse, 334);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry34, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);

	convBufa.Copy(whereClause);
	convBufb.Copy(convBufa);
	printString.Copy(convBufb);
	INFO_PRINTF2(KFormatAfter, &printString);
		
	whereClause.Delete(0, 500);
	printString.Delete(0, 500);
	delete cacheEntry34;
	delete query34;
	
	// 35. Insert Query for EMsvDescription 	
	//Create the Query as per requirement
	CMsvSearchSortQuery* query35 = CMsvSearchSortQuery::NewL();
	query35->iParentId = KMsvRootIndexEntryIdValue;
	query35->SetParentId(KFirstFreeEntryId);
	query35->SetWholeWord(ETrue);
	query35->AddSearchOptionL(EMsvSubject, searchString, EMsvEqual);

	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry35;
	cacheEntry35 = CMsvSearchSortCacheEntry::CreateQueryEntryL(query35, EFalse, 352);
	
	TRAP(err,iSearchDbAdapter->DoCreateSearchQueryL(*cacheEntry35, whereClause));
	MY_ASSERT_EQUALS(err, KErrNone);
	delete cacheEntry35;
	delete query35;
	
	CleanupStack::PopAndDestroy(2);
	}
	

/**
 * TestFindL()
 * @param none.
 * @return none.
 *
 * This Function Tests the FindL Functionalities
 */
void CTestSearchSortDBadapter::TestFindL()
	{	
	_LIT(KFunction,"TestFindL");
	INFO_PRINTF1(KFunction);
	
	//Initialization
	_LIT(KSearchString, "TEST");//Search String
	_LIT(KSearchString2, "EST");//Search String2
	_LIT(KSearchString3, "test");//Search String3
	_LIT(KSearchString4, "TE*");//Search String3
	_LIT(KSearchString5, "?ES?");//Search String3
	TBufC<4> searchString(KSearchString);
	TBufC<3> searchString2(KSearchString2);
	TBufC<4> searchString3(KSearchString3);
	TBufC<3> searchString4(KSearchString4);
	TBufC<4> searchString5(KSearchString5);
	
	TMsvId visibleFolder;
	TInt counter;
	RArray<TMsvId> entrySelection;
	TInt err;
	
	visibleFolder = KMsvRootIndexEntryIdValue;
		// Creating a folder under root

	
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryIdValue, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryIdValue, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		}
		
				
	
	/* 
	01. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery1 = CMsvSearchSortQuery::NewL();
	searchQuery1->iParentId = KMsvRootIndexEntryIdValue;
	
//	searchQuery1->iParentId=MaskTMsvId(GetDriveId(searchQuery1->iParentId),searchQuery1->iParentId);
	searchQuery1->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual);
	searchQuery1->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	searchQuery1->SetWholeWord(ETrue);
	searchQuery1->SetWildCardSearch(EFalse);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry1;
	cacheEntry1 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery1, EFalse);
	cacheEntry1->iQueryID = K_QUERYID+1;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt typeOfQuery = EQueryOnIndexEntry;
	TInt returnResultType = KFinalResult;
	
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry1, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry1, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry1;
	delete searchQuery1;
	
	/* 
	02. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery2 = CMsvSearchSortQuery::NewL();
	searchQuery2->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery2->AddSearchOptionL(EMsvDescription, searchString2, EMsvEqual);
	searchQuery2->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	searchQuery2->SetWholeWord(EFalse);
	searchQuery2->SetWildCardSearch(EFalse);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry2;
	cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery2, EFalse);
	cacheEntry2->iQueryID = K_QUERYID+2;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry2, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry2, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry2;
	delete searchQuery2;
	
	/* 
	03. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = ETrue;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery3 = CMsvSearchSortQuery::NewL();
	searchQuery3->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery3->AddSearchOptionL(EMsvDescription, searchString4, EMsvEqual);
	searchQuery3->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	searchQuery3->SetWholeWord(ETrue);
	searchQuery3->SetWildCardSearch(ETrue);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry3;
	cacheEntry3 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery3, EFalse);
	cacheEntry3->iQueryID = K_QUERYID+3;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry3, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry3, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry3;
	delete searchQuery3;
	
	/* 
	05. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = ETrue;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery5 = CMsvSearchSortQuery::NewL();
	searchQuery5->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery5->AddSearchOptionL(EMsvDescription, searchString4, EMsvEqual);
	searchQuery5->AddSortOptionL(EMsvDescription, EMsvSortDescending);
	searchQuery5->SetWholeWord(ETrue);
	searchQuery5->SetWildCardSearch(ETrue);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry5;
	cacheEntry5 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery5, EFalse);
	cacheEntry5->iQueryID = K_QUERYID+5;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry5, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry5, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);

	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry5;
	delete searchQuery5;
	
	/* 
	06. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery6 = CMsvSearchSortQuery::NewL();
	searchQuery6->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery6->AddSearchOptionL(EMsvDetails, searchString, EMsvEqual);
	searchQuery6->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	searchQuery6->SetWholeWord(ETrue);
	searchQuery6->SetWildCardSearch(EFalse);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry6;
	cacheEntry6 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery6, EFalse);
	cacheEntry6->iQueryID = K_QUERYID+6;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry6, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry6, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry6;
	delete searchQuery6;
	
	/* 
	07. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery7 = CMsvSearchSortQuery::NewL();
	searchQuery7->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery7->AddSearchOptionL(EMsvDetails, searchString2, EMsvEqual);
	searchQuery7->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	searchQuery7->SetWholeWord(EFalse);
	searchQuery7->SetWildCardSearch(EFalse);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry7;
	cacheEntry7 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery7, EFalse);
	cacheEntry7->iQueryID = K_QUERYID+7;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry7, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry7, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry7;
	delete searchQuery7;
	
	/* 
	08. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = ETrue;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery8 = CMsvSearchSortQuery::NewL();
	searchQuery8->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery8->AddSearchOptionL(EMsvDetails, searchString4, EMsvEqual);
	searchQuery8->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	searchQuery8->SetWholeWord(ETrue);
	searchQuery8->SetWildCardSearch(ETrue);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry8;
	cacheEntry8 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery8, EFalse);
	cacheEntry8->iQueryID = K_QUERYID+8;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry8, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry8, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry8;
	delete searchQuery8;
	
	/* 
	010. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = EFalse;
	CaseSensitive = EFalse;
	WildCard = ETrue;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery10 = CMsvSearchSortQuery::NewL();
	searchQuery10->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery10->AddSearchOptionL(EMsvDetails, searchString5, EMsvEqual);
	searchQuery10->AddSortOptionL(EMsvDetails, EMsvSortDescending);
	searchQuery10->SetWholeWord(ETrue);
	searchQuery10->SetWildCardSearch(ETrue);	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry10;
	cacheEntry10 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery10, EFalse);
	cacheEntry10->iQueryID = K_QUERYID+10;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry10, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry10, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry10;
	delete searchQuery10;
	
	/* 
	11. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery11 = CMsvSearchSortQuery::NewL();
	searchQuery11->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery11->AddSearchOptionL(EMsvSize, 64, EMsvEqual);
	searchQuery11->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry11;
	cacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery11, EFalse);
	cacheEntry11->iQueryID = K_QUERYID+11;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry11, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry11, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry11;
	delete searchQuery11;	
	
	/* 
	12. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery12 = CMsvSearchSortQuery::NewL();
	searchQuery12->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery12->AddSearchOptionL(EMsvSize, 64, EMsvNotEqual);
	searchQuery12->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry12;
	cacheEntry12 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery12, EFalse);
	cacheEntry12->iQueryID = K_QUERYID+12;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry12, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry12, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry12;
	delete searchQuery12;
	
	/* 
	13. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvLessThan
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery13 = CMsvSearchSortQuery::NewL();
	searchQuery13->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery13->AddSearchOptionL(EMsvSize, 64, EMsvLessThan);
	searchQuery13->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry13;
	cacheEntry13 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery13, EFalse);
	cacheEntry13->iQueryID = K_QUERYID+13;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry13, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry13, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry13;
	delete searchQuery13;
	
	/* 
	14. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvGreaterThan
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery14 = CMsvSearchSortQuery::NewL();
	searchQuery14->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery14->AddSearchOptionL(EMsvSize, 32, EMsvGreaterThan);
	searchQuery14->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry14;
	cacheEntry14 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery14, EFalse);
	cacheEntry14->iQueryID = K_QUERYID+14;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry14, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry14, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry14;
	delete searchQuery14;
	
	/* 
	15. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvLessThanOrEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery15 = CMsvSearchSortQuery::NewL();
	searchQuery15->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery15->AddSearchOptionL(EMsvSize, 32, EMsvLessThanOrEqual);
	searchQuery15->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry15;
	cacheEntry15 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery15, EFalse);
	cacheEntry15->iQueryID = K_QUERYID+15;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry15, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry15, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);	
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry15;
	delete searchQuery15;
	
	/* 
	16. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvGreaterThanOrEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery16 = CMsvSearchSortQuery::NewL();
	searchQuery16->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery16->AddSearchOptionL(EMsvSize, 64, EMsvGreaterThanOrEqual);
	searchQuery16->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry16;
	cacheEntry16 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery16, EFalse);
	cacheEntry16->iQueryID = K_QUERYID+16;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry16, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry16, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry16;
	delete searchQuery16;
	
	/* 
	23. Search query Creation for
	EMsvAttachment = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery23 = CMsvSearchSortQuery::NewL();
	searchQuery23->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery23->AddSearchOptionL(EMsvAttachment, ETrue);
	searchQuery23->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry23;
	cacheEntry23 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery23, EFalse);
	cacheEntry23->iQueryID = K_QUERYID+23;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry23, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry23, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry23;
	delete searchQuery23;
	
	/* 
	24. Search query Creation for
	EMsvAttachment = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery24 = CMsvSearchSortQuery::NewL();
	searchQuery24->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery24->AddSearchOptionL(EMsvAttachment, EFalse);
	searchQuery24->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry24;
	cacheEntry24 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery24, EFalse);
	cacheEntry24->iQueryID = K_QUERYID+24;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry24, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry24, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);	
			#endif
			}
		}
		
	entrySelection.Reset();
	delete cacheEntry24;
	delete searchQuery24;
	
	/* 
	26. Search query Creation for
	EMsvAttachment = TEST;
	Sorting = EMsvAttachment;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery26 = CMsvSearchSortQuery::NewL();
	searchQuery26->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery26->AddSearchOptionL(EMsvAttachment, EFalse);
	searchQuery26->AddSortOptionL(EMsvSize, EMsvSortDescending);
			
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry26;
	cacheEntry26 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery26, EFalse);
	cacheEntry26->iQueryID = K_QUERYID+26;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err, iSearchDbAdapter->FindL(*cacheEntry26, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry26, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);	
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry26;
	delete searchQuery26;
	
	/* 
	27. Search query Creation for
	EMsvUnreadMessages = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery27 = CMsvSearchSortQuery::NewL();
	searchQuery27->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery27->AddSearchOptionL(EMsvUnreadMessages, ETrue);
	searchQuery27->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry27;
	cacheEntry27 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery27, EFalse);
	cacheEntry27->iQueryID = K_QUERYID+27;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry27, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry27, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry27;
	delete searchQuery27;
	
	/* 
	28. Search query Creation for
	EMsvUnreadMessages = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery28 = CMsvSearchSortQuery::NewL();
	searchQuery28->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery28->AddSearchOptionL(EMsvUnreadMessages, EFalse);
	searchQuery28->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry28;
	cacheEntry28 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery28, EFalse);
	cacheEntry28->iQueryID = K_QUERYID+28;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry28, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry28, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
		
	entrySelection.Reset();
	delete cacheEntry28;
	delete searchQuery28;
	
	/* 
	30. Search query Creation for
	EMsvUnreadMessages = TEST;
	Sorting = EMsvUnreadMessages;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery30 = CMsvSearchSortQuery::NewL();
	searchQuery30->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery30->AddSearchOptionL(EMsvUnreadMessages, EFalse);
	searchQuery30->AddSortOptionL(EMsvSize, EMsvSortDescending);
	
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry30;
	cacheEntry30 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery30, EFalse);
	cacheEntry30->iQueryID = K_QUERYID+30;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry30, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry30, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1,UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry30;
	delete searchQuery30;
	
	/* 
	31. Search query Creation for
	EMsvNew = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery31 = CMsvSearchSortQuery::NewL();
	searchQuery31->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery31->AddSearchOptionL(EMsvNew, ETrue);
	searchQuery31->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry31;
	cacheEntry31 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery31, EFalse);
	cacheEntry31->iQueryID = K_QUERYID+31;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry31, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry31, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
		
	entrySelection.Reset();
	delete cacheEntry31;
	delete searchQuery31;
	
	/* 
	32. Search query Creation for
	EMsvNew = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery32 = CMsvSearchSortQuery::NewL();
	searchQuery32->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery32->AddSearchOptionL(EMsvNew, EFalse);
	searchQuery32->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry32;
	cacheEntry32 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery32, EFalse);
	cacheEntry32->iQueryID = K_QUERYID+32;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry32, entrySelection, EFalse, typeOfQuery, returnResultType));
#else		
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry32, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry32;
	delete searchQuery32;
	
	/* 
	34. Search query Creation for
	EMsvNew = TEST;
	Sorting = EMsvNew;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery34 = CMsvSearchSortQuery::NewL();
	searchQuery34->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery34->AddSearchOptionL(EMsvNew, EFalse);
	searchQuery34->AddSortOptionL(EMsvSize, EMsvSortDescending);
			
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry34;
	cacheEntry34 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery34, EFalse);
	cacheEntry34->iQueryID = K_QUERYID+34;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry34, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry34, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry34;
	delete searchQuery34;
	
	/* 
	35. Search query Creation for
	EMsgLowPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery35 = CMsvSearchSortQuery::NewL();
	searchQuery35->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery35->AddSearchOptionL(EMsvPriority, 2, EMsvEqual);
	searchQuery35->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry35;
	cacheEntry35 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery35, EFalse);
	cacheEntry35->iQueryID = K_QUERYID+35;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry35, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry35, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter,UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry35;
	delete searchQuery35;
	
	/* 
	36. Search query Creation for
	EMsgLowPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery36 = CMsvSearchSortQuery::NewL();
	searchQuery36->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery36->AddSearchOptionL(EMsvPriority, 2, EMsvNotEqual);
	searchQuery36->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry36;
	cacheEntry36 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery36, EFalse);
	cacheEntry36->iQueryID = K_QUERYID+36;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry36, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry36, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1,UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry36;
	delete searchQuery36;
	
	/* 
	38. Search query Creation for
	EMsgLowPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery38 = CMsvSearchSortQuery::NewL();
	searchQuery38->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery38->AddSearchOptionL(EMsvPriority, 2, EMsvEqual);
	searchQuery38->AddSortOptionL(EMsvSize, EMsvSortDescending);
			
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry38;
	cacheEntry38 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery38, EFalse);
	cacheEntry38->iQueryID = K_QUERYID+38;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry38, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry38, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry38;
	delete searchQuery38;
	
	/* 
	39. Search query Creation for
	EMsgNormalPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery39 = CMsvSearchSortQuery::NewL();
	searchQuery39->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery39->AddSearchOptionL(EMsvPriority, 1, EMsvEqual);
	searchQuery39->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry39;
	cacheEntry39 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery39, EFalse);
	cacheEntry39->iQueryID = K_QUERYID+39;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry39, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry39, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			}
		}
	
	entrySelection.Reset();
	delete cacheEntry39;
	delete searchQuery39;
	
	/* 
	40. Search query Creation for
	EMsgNormalPriority = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery40 = CMsvSearchSortQuery::NewL();
	searchQuery40->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery40->AddSearchOptionL(EMsvPriority, 1, EMsvNotEqual);
	searchQuery40->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry40;
	cacheEntry40 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery40, EFalse);
	cacheEntry40->iQueryID = K_QUERYID+40;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry40, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry40, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter,UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry40;
	delete searchQuery40;
	
	/* 
	42. Search query Creation for
	EMsgNormalPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery42 = CMsvSearchSortQuery::NewL();
	searchQuery42->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery42->AddSearchOptionL(EMsvPriority, 1, EMsvEqual);
	searchQuery42->AddSortOptionL(EMsvSize, EMsvSortDescending);
			
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry42;
	cacheEntry42 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery42, EFalse);
	cacheEntry42->iQueryID = K_QUERYID+42;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry42, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry42, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 90);
	
	for(TInt index=0; counter < entrySelection.Count(); ++index)
		{
		if(index%10 != 0)
			{
			#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));	
			#else
				ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
			#endif
			
			}
		}
		
	entrySelection.Reset();
	delete cacheEntry42;
	delete searchQuery42;
	
	/* 
	46. Search query Creation for
	EMsgHighPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery46 = CMsvSearchSortQuery::NewL();
	searchQuery46->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery46->AddSearchOptionL(EMsvPriority, 0, EMsvEqual);
	searchQuery46->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry46;
	cacheEntry46 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery46, EFalse);
	cacheEntry46->iQueryID = K_QUERYID+46;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry46, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry46, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), NULL);
	entrySelection.Reset();
	delete cacheEntry46;
	delete searchQuery46;
	
	/* 
	47. Search query Creation for
	EMsgHighPriority = TEST;
	Sorting = EMsvSize;
	EMsvNotEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery47 = CMsvSearchSortQuery::NewL();
	searchQuery47->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery47->AddSearchOptionL(EMsvPriority, 0, EMsvNotEqual);
	searchQuery47->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry47;
	cacheEntry47 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery47, EFalse);
	cacheEntry47->iQueryID = K_QUERYID+47;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry47, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry47, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	entrySelection.Reset();
	delete cacheEntry47;
	delete searchQuery47;
	
	/* 
	51. Search query Creation for
	EMsvMtmTypeUID = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery51 = CMsvSearchSortQuery::NewL();
	searchQuery51->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery51->AddSearchOptionL(EMsvMtmTypeUID, KUidMsvServiceEntry.iUid, EMsvEqual);
	searchQuery51->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry51;
	cacheEntry51 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery51, EFalse);
	cacheEntry51->iQueryID = K_QUERYID+51;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry51, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry51, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry51;
	delete searchQuery51;
	
	/* 
	53. Search query Creation for
	EMsvMtmTypeUID = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery53 = CMsvSearchSortQuery::NewL();
	searchQuery53->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery53->AddSearchOptionL(EMsvMtmTypeUID, KUidMsvServiceEntry.iUid, EMsvEqual);
	searchQuery53->AddSortOptionL(EMsvSize, EMsvSortDescending);
			
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry53;
	cacheEntry53 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery53, EFalse);
	cacheEntry53->iQueryID = K_QUERYID+53;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry53, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry53, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif	
		counter+=10;
		}
	delete cacheEntry53;
	delete searchQuery53;
	
	entrySelection.Reset();
	entrySelection.Close();
	}


/**
 * TestUpdateNewIdsL()
 * @param none.
 * @return none.
 *
 * This Function Tests the UpdateNewIdsL Functionalities
 */
void CTestSearchSortDBadapter::TestUpdateNewIdsL()
	{	
	_LIT(KFunction,"TestUpdateNewIdsL");
	INFO_PRINTF1(KFunction);
	
	//Initialization
	_LIT(KSearchString, "TEST");//Search String
	_LIT(KSearchString2, "EST");//Search String2
	_LIT(KSearchString3, "test");//Search String3
	_LIT(KSearchString4, "TE*");//Search String3
	_LIT(KSearchString5, "?ES?");//Search String3
	TBufC<4> searchString(KSearchString);
	TBufC<3> searchString2(KSearchString2);
	TBufC<4> searchString3(KSearchString3);
	TBufC<3> searchString4(KSearchString4);
	TBufC<4> searchString5(KSearchString5);
	
	_LIT(KDateString, "1234");//Date Search String
	TBufC<4> dateSearch(KDateString);
	
	TMsvId visibleFolder;
	TInt counter;
	RArray<TMsvId> entrySelection;
	RArray<TMsvId> idFilter;
	
	TInt err;
	//Add Root Entry
/*	TRAPD(err, CreateEntryL(KMsvRootIndexEntryIdValue+1, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);*/
	
	visibleFolder =  KMsvRootIndexEntryIdValue+1;
	//Add Entries Under Root
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId+1, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId+1, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		}
	
	//Add a Folder entry
	TRAP(err, CreateEntryL(KFirstFreeEntryId+300, KMsvRootIndexEntryId+1, KUidMsvFolderEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);
	
	visibleFolder =  KFirstFreeEntryId+300;
	for(TInt index=1; index<=200; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+300+index, KFirstFreeEntryId+300, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+300, visibleFolder);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+300+index, KFirstFreeEntryId+300, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+300, visibleFolder);
			}
		idFilter.Append(KFirstFreeEntryId+300+index);
		}
	
	//Add Updated Entries into the folder
	
	
	/* 
	01. Search query Creation for
	EMsvSize = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery01 = CMsvSearchSortQuery::NewL();
	searchQuery01->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery01->AddSearchOptionL(EMsvSize, 64, EMsvEqual);
	searchQuery01->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry01;
	cacheEntry01 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery01, EFalse);
	cacheEntry01->iQueryID = K_QUERYID+01;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt typeOfQuery = EQueryOnIndexEntry;
	TInt returnResultType;
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry01, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry01, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	
	//Update the selection
	cacheEntry01->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else
	cacheEntry01->iQueryType = EIndexQuery;
#endif	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry01, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry01, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	delete cacheEntry01;
	delete searchQuery01;
	
	/* 
	02. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery02 = CMsvSearchSortQuery::NewL();
	searchQuery02->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery02->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual);
	searchQuery02->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	searchQuery02->SetWholeWord(ETrue);
	searchQuery02->SetWildCardSearch(EFalse);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry02;
	cacheEntry02 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery02, EFalse);
	cacheEntry02->iQueryID = K_QUERYID+02;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry02, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry02, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	
	//Update the selection
	cacheEntry02->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else	
	cacheEntry02->iQueryType = EIndexQuery;
#endif	

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry02, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry02, idFilter, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	delete cacheEntry02;
	delete searchQuery02;
	
	/* 
	03. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery03 = CMsvSearchSortQuery::NewL();
	searchQuery03->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery03->AddSearchOptionL(EMsvDetails, searchString, EMsvEqual);
	searchQuery03->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	searchQuery03->SetWholeWord(ETrue);
	searchQuery03->SetWildCardSearch(EFalse);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry03;
	cacheEntry03 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery03, EFalse);
	cacheEntry03->iQueryID = K_QUERYID+03;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry03, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry03, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	
	//Update the selection
	cacheEntry03->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else	
	cacheEntry03->iQueryType = EIndexQuery;
#endif	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry03, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry03, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	delete cacheEntry03;
	delete searchQuery03;
	
	/* 
	05. Search query Creation for
	EMsvAttachment = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery05 = CMsvSearchSortQuery::NewL();
	searchQuery05->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery05->AddSearchOptionL(EMsvAttachment, ETrue);
	searchQuery05->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry05;
	cacheEntry05 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery05, EFalse);
	cacheEntry05->iQueryID = K_QUERYID+05;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry05, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry05, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);		
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	
	//Update the selection
	cacheEntry05->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else	
	cacheEntry05->iQueryType = EIndexQuery;
#endif	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry03, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry05, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	delete cacheEntry05;
	delete searchQuery05;
	
	/* 
	06. Search query Creation for
	EMsvUnreadMessages = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery06 = CMsvSearchSortQuery::NewL();
	searchQuery06->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery06->AddSearchOptionL(EMsvUnreadMessages, ETrue);
	searchQuery06->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry06;
	cacheEntry06 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery06, EFalse);
	cacheEntry06->iQueryID = K_QUERYID+06;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry06, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry06, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	
	//Update the selection
	cacheEntry06->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else	
	cacheEntry06->iQueryType = EIndexQuery;
#endif	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry06, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry06, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	delete cacheEntry06;
	delete searchQuery06;
	
	/* 
	07. Search query Creation for
	EMsgLowPriority = TEST;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery07 = CMsvSearchSortQuery::NewL();
	searchQuery07->iParentId = KMsvRootIndexEntryIdValue+1;
	searchQuery07->AddSearchOptionL(EMsvPriority, 2, EMsvEqual);
	searchQuery07->AddSortOptionL(EMsvSize, EMsvSortAscending);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry07;
	cacheEntry07 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery07, EFalse);
	cacheEntry07->iQueryID = K_QUERYID+07;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry07, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry07, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 10);
	
	counter=10;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	
	//Update the selection
	cacheEntry07->iSubFolderOption = ETrue;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	cacheEntry01->iTypeOfQuery = EQueryOnIndexEntry;
#else	
	cacheEntry07->iQueryType = EIndexQuery;
#endif	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry07, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry07, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 30);
	entrySelection.Reset();
	
	idFilter.Reset();
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)								
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry07, idFilter, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Update the selection
	TRAP(err,iSearchDbAdapter->UpdateNewIdsL(*cacheEntry07, idFilter, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	entrySelection.Reset();
	delete cacheEntry07;
	delete searchQuery07;
	
	entrySelection.Close();
	idFilter.Close();
	
	}


/**
 * TestDoSortL()
 * @param none.
 * @return none.
 *
 * This Function Tests the TFindL sort Functionalities
 */
void CTestSearchSortDBadapter::TestDoSortL()
	{
	_LIT(KFunction,"TestDoSortL");
	INFO_PRINTF1(KFunction);
	
	
	RArray<TMsvId> entrySelection;
//	TMsvId visibleFolder;
	TInt counter=0;
	
//	TRAPD(err, CreateEntryL(KMsvRootIndexEntryIdValue, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	
	for(TInt index = 1; index <=100; ++index)
		{
		CreateSortEntriesL(index);	
		}
	
		
	// 01. Sort query Creation for EMsvSize Asc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery1 = CMsvSearchSortQuery::NewL();
	sortQuery1->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery1->AddSortOptionL(EMsvSize, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry1;
	sortcacheEntry1 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery1, EFalse);
	sortcacheEntry1->iQueryID = K_QUERYID+1;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt typeOfQuery = EQueryOnIndexEntry;
	TInt returnResultType;
	
	//Pass the cacheEntry to create the query required
	TRAPD(err,iSearchDbAdapter->FindL(*sortcacheEntry1, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAPD(err,iSearchDbAdapter->FindL(*sortcacheEntry1, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(sortcacheEntry1->iQueryID));
	
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+index+1, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+index+1, entrySelection[index]);
		#endif
		}
		
	entrySelection.Reset();
	delete sortcacheEntry1;
	delete sortQuery1;
	
	// 02. Sort query Creation for EMsvSize Desc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery2 = CMsvSearchSortQuery::NewL();
	sortQuery2->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery2->AddSortOptionL(EMsvSize, EMsvSortDescending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry2;
	sortcacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery2, EFalse);
	sortcacheEntry2->iQueryID = K_QUERYID+2;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry2, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry2, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(sortcacheEntry2->iQueryID));
	
	
	counter=100;
	for(TInt index=0; index < 100; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);	
		#endif
		--counter;
		}
			
	entrySelection.Reset();
	delete sortcacheEntry2;
	delete sortQuery2;
	
	// 07. Sort query Creation for EMsvDetails Asc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery10 = CMsvSearchSortQuery::NewL();
	sortQuery10->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery10->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry10;
	sortcacheEntry10 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery10, EFalse);
	sortcacheEntry10->iQueryID = K_QUERYID+10;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry10, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry10, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(sortcacheEntry10->iQueryID));
	
	counter=3;
	for(TInt index=0; index < entrySelection.Count()-1; ++index)
		{
		if(index == 33)
			{
			counter = 2;
			}
		
		if(index == 66)
			{
			counter = 1;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry10;
	delete sortQuery10;

	// 08. Sort query Creation for EMsvDetails Desc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery11 = CMsvSearchSortQuery::NewL();
	sortQuery11->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery11->AddSortOptionL(EMsvDetails, EMsvSortDescending);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry11;
	sortcacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery11, EFalse);
	sortcacheEntry11->iQueryID = K_QUERYID+11;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry11, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry11, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	TRAP(err,iSearchDbAdapter->DeleteQueryFromTableL(sortcacheEntry11->iQueryID));
	
	counter=1;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		if(index == 34)
			{
			counter = 2;
			}
		
		if(index == 67)
			{
			counter = 3;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry11;
	delete sortQuery11;
	
	// 09. Sort query Creation for EMsvDescription Asc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery12 = CMsvSearchSortQuery::NewL();
	sortQuery12->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery12->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry12;
	sortcacheEntry12 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery12, EFalse);
	sortcacheEntry12->iQueryID = K_QUERYID+12;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry12, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry12, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	
	counter=3;
	for(TInt index=0; index < entrySelection.Count()-1; ++index)
		{
		if(index == 33)
			{
			counter = 2;
			}
		
		if(index == 66)
			{
			counter = 1;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter,UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry12;
	delete sortQuery12;
	
	// 10. Sort query Creation for EMsvDescription Desc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery13 = CMsvSearchSortQuery::NewL();
	sortQuery13->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery13->AddSortOptionL(EMsvDescription, EMsvSortDescending);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry13;
	sortcacheEntry13 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery13, EFalse);
	sortcacheEntry13->iQueryID = K_QUERYID+13;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry13, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry13, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	
	counter=1;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		if(index == 34)
			{
			counter = 2;
			}
		
		if(index == 67)
			{
			counter = 3;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry13;
	delete sortQuery13;
	
	// 14. Sort query Creation for EMsvMtmTypeUID Asc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery14 = CMsvSearchSortQuery::NewL();
	sortQuery14->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery14->AddSortOptionL(EMsvMtmTypeUID, EMsvSortAscending);
	
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry14;
	sortcacheEntry14 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery14, EFalse);
	sortcacheEntry14->iQueryID = K_QUERYID+14;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry14, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry14, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	
	counter=3;
	for(TInt index=0; index < entrySelection.Count(); ++index)
		{
		if(index == 33)
			{
			counter = 2;
			}
		
		if(index == 66)
			{
			counter = 1;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry14;
	delete sortQuery14;
	
	// 15. Sort query Creation for EMsvMtmTypeUID Desc;
	//Create the Query as per requirement
	CMsvSearchSortQuery* sortQuery15 = CMsvSearchSortQuery::NewL();
	sortQuery15->iParentId = KMsvRootIndexEntryIdValue;
	sortQuery15->AddSortOptionL(EMsvMtmTypeUID, EMsvSortDescending);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* sortcacheEntry15;
	sortcacheEntry15 = CMsvSearchSortCacheEntry::CreateQueryEntryL(sortQuery15, EFalse);
	sortcacheEntry15->iQueryID = K_QUERYID+15;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry15, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheTEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*sortcacheEntry15, entrySelection, EFalse));
#endif
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 100);
	
	counter=1;
	for(TInt index=0; index < entrySelection.Count()-1; ++index)
		{
		if(index == 34)
			{
			counter = 2;
			}
		
		if(index == 67)
			{
			counter = 3;
			}
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+counter, entrySelection[index]);
		#endif
		counter+=3;
		}
		
	entrySelection.Reset();
	delete sortcacheEntry15;
	delete sortQuery15;
	entrySelection.Close();
	}
	

/**
 * TestFindLSubFoldersL()
 * @param none.
 * @return none.
 *
 * This Function Tests the FindL Subfolder Search Functionalities
 */
void CTestSearchSortDBadapter::TestFindLSubFoldersL()
	{
	_LIT(KFunction,"TestFindLSubFoldersL");
	INFO_PRINTF1(KFunction);

	//Initialization
	_LIT(KSearchString, "TEST");//Search String
	TBufC<4> searchString(KSearchString);

	TMsvId visibleFolder;
	TInt counter;
	TInt err;
	
	RArray<TMsvId> entrySelection;
	
/*	TRAPD(err, CreateEntryL(KMsvRootIndexEntryIdValue, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);
*/	//Create a Folder Under Root
		
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryIdValue, KUidMsvFolderEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);
	//Creat a Folder under the above Folder
	TRAP(err, CreateEntryL(KFirstFreeEntryId+1, KFirstFreeEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);
	//Creat a Folder under the above Folder
	TRAP(err, CreateEntryL(KFirstFreeEntryId+2, KFirstFreeEntryId+1, KUidMsvFolderEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);
	
	visibleFolder = KFirstFreeEntryId+1;
	
		
	//Add entries under folder KFirstFreeEntryId+1
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+100+index, KFirstFreeEntryId+1, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+1, visibleFolder);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+100+index, KFirstFreeEntryId+1, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+1, visibleFolder);
			}
		}
	
	visibleFolder = KFirstFreeEntryId+2;
	//Add entries under folder KFirstFreeEntryId+2
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+200+index, KFirstFreeEntryId+2, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+2, visibleFolder);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+200+index, KFirstFreeEntryId+2, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			ASSERT_EQUALS(KFirstFreeEntryId+2, visibleFolder);
			}
		}
	
	/*
	01. Search query Creation for
	EMsvDescription = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDescription;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery1 = CMsvSearchSortQuery::NewL();
	searchQuery1->iParentId = KFirstFreeEntryId;
	searchQuery1->AddSearchOptionL(EMsvDescription, searchString, EMsvEqual);
	searchQuery1->AddSortOptionL(EMsvDescription, EMsvSortAscending);
	searchQuery1->SetWholeWord(ETrue);
	searchQuery1->SetWildCardSearch(EFalse);
	searchQuery1->SetSubFolderSearch(ETrue);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry1;
	cacheEntry1 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery1, EFalse);
	cacheEntry1->iQueryID = K_QUERYID+1;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt typeOfQuery = EQueryOnIndexEntry;
	TInt returnResultType;
	
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry1, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry1, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 20);
	
	counter=10;
	for(TInt index=0; index < 10; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	counter=10;
	for(TInt index=10; index < 20; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry1;
	delete searchQuery1;
	
	/* 
	02. Search query Creation for
	EMsvDetails = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsvDetails;
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery2 = CMsvSearchSortQuery::NewL();
	searchQuery2->iParentId = KFirstFreeEntryId;
	searchQuery2->AddSearchOptionL(EMsvDetails, searchString, EMsvEqual);
	searchQuery2->AddSortOptionL(EMsvDetails, EMsvSortAscending);
	searchQuery2->SetWholeWord(ETrue);
	searchQuery2->SetWildCardSearch(EFalse);
	searchQuery2->SetSubFolderSearch(ETrue);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry2;
	cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery2, EFalse);
	cacheEntry2->iQueryID = K_QUERYID+2;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry2, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry2, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 20);
	
	counter=10;
	for(TInt index=0; index < 10; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	counter=10;
	for(TInt index=10; index < 20; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry2;
	delete searchQuery2;
	
	/* 
	03. Search query Creation for
	EMsvSize = 64;
	Sorting = EMsvSize;
	EMsvEqual
	*/
	
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery3 = CMsvSearchSortQuery::NewL();
	searchQuery3->iParentId = KFirstFreeEntryId;
	searchQuery3->AddSearchOptionL(EMsvSize, 64, EMsvEqual);
	searchQuery3->AddSortOptionL(EMsvSize, EMsvSortAscending);
	searchQuery3->SetSubFolderSearch(ETrue);
		
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry3;
	cacheEntry3 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery3, EFalse);
	cacheEntry3->iQueryID = K_QUERYID+3;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry3, entrySelection, EFalse, typeOfQuery, returnResultType));
#else	
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry3, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 20);
	
	counter=10;
	for(TInt index=0; index < 10; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	counter=10;
	for(TInt index=10; index < 20; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	
	entrySelection.Reset();
	delete cacheEntry3;
	delete searchQuery3;
	
	/* 
	07. Search query Creation for
	EMsvMtmTypeUID = KUidMsvServiceEntry;
	EMsvEqual
	*/
	//Create the Query as per requirement
	CMsvSearchSortQuery* searchQuery7 = CMsvSearchSortQuery::NewL();
	searchQuery7->iParentId = KFirstFreeEntryId;
	searchQuery7->AddSearchOptionL(EMsvMtmTypeUID, KUidMsvServiceEntry.iUid, EMsvEqual);
	searchQuery7->SetSubFolderSearch(ETrue);
	
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry7;
	cacheEntry7 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery7, EFalse);
	cacheEntry7->iQueryID = K_QUERYID+7;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry3, entrySelection, EFalse, typeOfQuery, returnResultType));
#else
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->FindL(*cacheEntry7, entrySelection, EFalse));
#endif	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection.Count(), 20);
	
	counter=10;
	for(TInt index=0; index < 10; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+100+counter, entrySelection[index]);
		#endif
		counter+=10;
		}
	counter=10;
	for(TInt index=10; index < 20; ++index)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, UnmaskTMsvId(entrySelection[index]));
		#else
			ASSERT_EQUALS(KFirstFreeEntryId+200+counter, entrySelection[index]);
		#endif
		counter+=10;
		}

	entrySelection.Reset();
	delete cacheEntry7;
	delete searchQuery7;
	entrySelection.Close();
	}


void CTestSearchSortDBadapter::TestCreateQueryDateL()
	{
	_LIT(KFunction,"TestCreateQueryDateL");
	INFO_PRINTF1(KFunction);
	
	//Initialization
	_LIT(KFormatSerial, "Query Created : [%S]");
	_LIT(KSearchString, "20080228:");//Search String Feb 28 2009
	_LIT(KSearchString1, "20080328:");//Search String March 28 2009
	_LIT(KSearchString2, "details");//Search String 
	TBufC<10> searchString(KSearchString);
	TBufC<10> searchString1(KSearchString1);
	TBufC<10> searchString2(KSearchString2);
	
	RBuf8 queryBuf;
	queryBuf.Create(500);
	CleanupClosePushL(queryBuf);
	
	RBuf8 compareString;
	compareString.CreateL(500);
	CleanupClosePushL(compareString);
	
	RBuf8 compareString1;
	compareString.CreateL(500);
	CleanupClosePushL(compareString1);
	
	/*******************************************************/
	TInt err;
	_LIT(KFunction1,"EEqual");
	INFO_PRINTF1(KFunction1);

	TMsvId aParentId=4096;
	CMsvSearchSortQuery *searchQuery1= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery1);
	searchQuery1->AddSearchOptionL (EMsvDate, searchString, EMsvEqual);
	searchQuery1->SetWholeWord(ETrue);
	searchQuery1->iIsSearchQuery=ETrue;
	searchQuery1->iSortOnMessagePart=EMsvDate;
	iSearchDbAdapter->iSortOnHeader = EFalse;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry1;
	searchQuery1->SetWildCardSearch(EFalse);	
	cacheEntry1 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery1, EFalse, EFalse);
	#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		iSearchDbAdapter->QueryToGetMtmIdsL(aParentId, 1);
	#endif
	//Pass the cacheEntry to create the query required
	TRAP(err,iSearchDbAdapter->CreateQueryL(*cacheEntry1, queryBuf, aParentId,EFalse));
	MY_ASSERT_EQUALS(err, KErrNone );
	
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1001, id, date FROM IndexEntry WHERE parentId = 4096 AND date >= 63375004800000000 AND date <= 63375004800086399;"));
	TBuf8<500> aBuf;
	aBuf.Copy(queryBuf);
	TBuf16<500> bBuf;
	bBuf.Copy(aBuf);
	RBuf16 compareString16;
	compareString16.Create(500);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);

	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry1;
	
	/***************  Query 2  *************************/
	_LIT(KFunction2,"ENotEqual");
	INFO_PRINTF1(KFunction2);
	CMsvSearchSortQuery *searchQuery2= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery2);
	searchQuery2->AddSearchOptionL (EMsvDate, searchString, EMsvNotEqual);
	searchQuery2->SetWholeWord(ETrue);
	searchQuery2->iIsSearchQuery=ETrue;
	searchQuery2->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry2;
	searchQuery2->SetWildCardSearch(EFalse);	
	cacheEntry2 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery2, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	TRAPD(err1,iSearchDbAdapter->CreateQueryL(*cacheEntry2, queryBuf, aParentId,EFalse));
	MY_ASSERT_EQUALS(err1, KErrNone );
		
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1002, id, date FROM IndexEntry WHERE parentId = 4096 AND date < 63375004800000000 AND date > 63375004800086399;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry2;
	
	/***********************************************************************/
	_LIT(KFunction3,"EGreaterThan");
	INFO_PRINTF1(KFunction3);
	CMsvSearchSortQuery *searchQuery3= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery3);
	searchQuery3->AddSearchOptionL (EMsvDate, searchString, EMsvGreaterThan);
	searchQuery3->SetWholeWord(ETrue);
	searchQuery3->iIsSearchQuery=ETrue;
	searchQuery3->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry3;
	searchQuery3->SetWildCardSearch(EFalse);	
	cacheEntry3 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery3, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry3, queryBuf, aParentId,EFalse);
	
	MY_ASSERT_EQUALS(err, KErrNone );
		
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1003, id, date FROM IndexEntry WHERE parentId = 4096 AND date > 63375004800086399;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry3;
	
	/************************************************************************/
	_LIT(KFunction4,"ELessThan");
	INFO_PRINTF1(KFunction4);
	CMsvSearchSortQuery *searchQuery4= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery4);
	searchQuery4->AddSearchOptionL (EMsvDate, searchString, EMsvLessThan);
	searchQuery4->SetWholeWord(ETrue);
	searchQuery4->iIsSearchQuery=ETrue;
	searchQuery4->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry4;
	searchQuery4->SetWildCardSearch(EFalse);	
	cacheEntry4 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery4, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry4, queryBuf, aParentId,EFalse);
	
	MY_ASSERT_EQUALS(err, KErrNone );
		
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1004, id, date FROM IndexEntry WHERE parentId = 4096 AND date < 63375004800000000;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry4;
	
	/*******************************************************/
	_LIT(KFunction5,"ELessThanOrEqual");
	INFO_PRINTF1(KFunction5);
	CMsvSearchSortQuery *searchQuery5= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery5);
	searchQuery5->AddSearchOptionL (EMsvDate, searchString, EMsvLessThanOrEqual);
	searchQuery5->SetWholeWord(ETrue);
	searchQuery5->iIsSearchQuery=ETrue;
	searchQuery5->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry5;
	searchQuery5->SetWildCardSearch(EFalse);	
	cacheEntry5 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery5, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry5, queryBuf, aParentId,EFalse);
	
	MY_ASSERT_EQUALS(err, KErrNone );
		
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1005, id, date FROM IndexEntry WHERE parentId = 4096 AND date <= 63375004800086399;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry5;
	
	/**************************************************************/
	_LIT(KFunction6,"EGreaterThanOrEqual");
	INFO_PRINTF1(KFunction6);
	CMsvSearchSortQuery *searchQuery6= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery6);
	searchQuery6->AddSearchOptionL (EMsvDate, searchString, EMsvLessThanOrEqual);
	searchQuery6->SetWholeWord(ETrue);
	searchQuery6->iIsSearchQuery=ETrue;
	searchQuery6->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry6;
	searchQuery6->SetWildCardSearch(EFalse);	
	cacheEntry6 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery6, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry6, queryBuf, aParentId,EFalse);
	MY_ASSERT_EQUALS(err, KErrNone );
		
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1006, id, date FROM IndexEntry WHERE parentId = 4096 AND date >= 63375004800000000;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry6;
	
	/*******************************************************************/
	_LIT(KFunction7,"Date Test with two options EGreaterThan    ELessThan");
	INFO_PRINTF1(KFunction7);
	
	CMsvSearchSortQuery *searchQuery7= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery7);
	searchQuery7->AddSearchOptionL(EMsvDate, searchString, EMsvGreaterThan);
	searchQuery7->AddSearchOptionL(EMsvDate, searchString1, EMsvLessThan);
	searchQuery7->SetWholeWord(ETrue);
	searchQuery7->iIsSearchQuery=ETrue;
	searchQuery7->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry7;
	searchQuery7->SetWildCardSearch(EFalse);	
	cacheEntry7 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery7, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry7, queryBuf, aParentId,EFalse);	
	MY_ASSERT_EQUALS(err, KErrNone );
			
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1003, id, date FROM IndexEntry WHERE parentId = 4096 AND date > 63375004800086399;"));

	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry7;
	
	/*************************************************************************/
	_LIT(KFunction8,"Date Test with two options EGreaterThan    ELessThan");
	INFO_PRINTF1(KFunction8);
	

	CMsvSearchSortQuery *searchQuery8= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery8);
	searchQuery8->AddSearchOptionL(EMsvDate, searchString, EMsvGreaterThanOrEqual);
	searchQuery8->AddSearchOptionL(EMsvDate, searchString1, EMsvLessThanOrEqual);
	searchQuery8->SetWholeWord(ETrue);
	searchQuery8->iIsSearchQuery=ETrue;
	searchQuery8->iSortOnMessagePart=EMsvDate;
	//Create the cacheEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry8;
	searchQuery8->SetWildCardSearch(EFalse);	
	cacheEntry8 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery8, EFalse, EFalse);
	
	//Pass the cacheEntry to create the query required
	iSearchDbAdapter->CreateQueryL(*cacheEntry8, queryBuf, aParentId,EFalse);
	MY_ASSERT_EQUALS(err, KErrNone );
			
	//Query Comparison
	compareString.Copy(_L("INSERT INTO SearchSortEntry (Qid, id, sortfield) SELECT 1003, id, date FROM IndexEntry WHERE parentId = 4096 AND date > 63375004800086399;"));
	aBuf.Copy(queryBuf);
	bBuf.Copy(aBuf);
	compareString16.Copy(bBuf);
	INFO_PRINTF2(KFormatSerial, &compareString16);

	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);
	compareString16.Delete(0,500);
	delete cacheEntry8;
	
	
	
	//Reset the Buffers
	queryBuf.Delete(0, 500);
	compareString.Delete(0,500);	
	compareString16.Delete(0,500);
	CleanupStack::PopAndDestroy(11);
	
	}
	
void CTestSearchSortDBadapter::TestIteratorFindL()
	{
	_LIT(KFunction,"TestIteratorFindL");
	INFO_PRINTF1(KFunction);
	_LIT(KFormatAfter, "TmsvId retrieved - [%d]");
	//Initialization
	_LIT(KSearchString, "TEST");//Search String
	_LIT(KSearchString2, "EST");//Search String2
	_LIT(KSearchString3, "test");//Search String3
	_LIT(KSearchString4, "TE*");//Search String3
	_LIT(KSearchString5, "?ES?");//Search String3
	TBufC<4> searchString(KSearchString);
	TBufC<3> searchString2(KSearchString2);
	TBufC<4> searchString3(KSearchString3);
	TBufC<3> searchString4(KSearchString4);
	TBufC<4> searchString5(KSearchString5);
	
	_LIT(KSizeString, "64");//Size Search String
	TBufC<2> sizeSearch(KSizeString);
	
	_LIT(KSizeString2, "32");//Size Search String
	TBufC<2> sizeSearch2(KSizeString2);
	
	_LIT(KDateString, "1234");//Date Search String
	TBufC<4> dateSearch(KDateString);
	
	TMsvId visibleFolder;
	RArray<TMsvId> entrySelection;
	TMsvId id;
	TInt remainingCount;
	
	TInt err;
	/*TRAPD(err, CreateEntryL(KMsvRootIndexEntryIdValue, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	ASSERT_EQUALS(KErrNone, err);*/
	
	for(TInt index=1; index<=100; ++index)
		{
		if((index%10) == 0)
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, EFalse,EFalse, ETrue));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
			//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
			ASSERT_EQUALS(KErrNone, err);
			}
		}
	
	//With Init
	TRAP(err, iSearchDbAdapter->GetNextIdL(id,remainingCount));
	MY_ASSERT_EQUALS(err, KErrNotFound);


	/* 
	After Init
	Search query Creation for
	EMsgDescription = TEST;
	WholeWord = ETrue;
	CaseSensitive = EFalse;
	WildCard = NotSpecified;
	Sorting = EMsgDescription;
	*/

	//Create the Query as per requirement
	CMsvSearchSortQuery *searchQuery1= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery1);
	searchQuery1->iParentId = KMsvRootIndexEntryIdValue;
	searchQuery1->AddSearchOptionL (EMsvDescription, searchString, EMsvEqual);
	searchQuery1->AddSortOptionL(EMsvDate,EMsvSortAscending);
	searchQuery1->SetWholeWord(EFalse);
	searchQuery1->SetWildCardSearch(EFalse);
		
	//Create the cacheTEntry from the query
	CMsvSearchSortCacheEntry* cacheEntry11;
	cacheEntry11 = CMsvSearchSortCacheEntry::CreateQueryEntryL(searchQuery1, EFalse,EFalse);
	
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt typeOfQuery = EQueryOnIndexEntry;
	TInt returnResultType;
	
	iSearchDbAdapter->GetIdsInIteratorNewQueryL(*cacheEntry11, EFalse, typeOfQuery, returnResultType);
#else
	iSearchDbAdapter->GetIdsInIteratorNewQueryL(*cacheEntry11);
#endif	
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	_LIT(KPrintinfo,"----TestIteratorFindL with QueryId ----");
	INFO_PRINTF1(KPrintinfo);
	
	//Iter by QueryId
	iSearchDbAdapter->GetIdsInIteratorQueryIdL(cacheEntry11->iQueryID);
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	iSearchDbAdapter->GetNextIdL(id,remainingCount);
	INFO_PRINTF2(KFormatAfter,id);
	INFO_PRINTF2(KFormatAfter,remainingCount);
	remainingCount=0;
	
	//After Completion
	TRAP(err, iSearchDbAdapter->GetNextIdL(id,remainingCount));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	delete cacheEntry11;
	entrySelection.Close();
	CleanupStack::PopAndDestroy(1);	
	}	


CTestSuite* CTestSearchSortDBadapter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestDoAddRelationalOperatorL);
	ADD_ASYNC_TEST_STEP(TestIsSortAsNumRequired);
	ADD_ASYNC_TEST_STEP(TestAddIdtoDBL);
	ADD_ASYNC_TEST_STEP(TestGetSortedTMsvIdsfromTableL);
	ADD_ASYNC_TEST_STEP(TestGetLastNTMsvIdsfromTableL);
	ADD_ASYNC_TEST_STEP(TestReturnResultCountInTableL);
	ADD_ASYNC_TEST_STEP(TestDeleteQueryFromTableL);
	ADD_ASYNC_TEST_STEP(TestDeleteTMsvIdFromAllQueriesL);
	ADD_ASYNC_TEST_STEP(TestDeleteTMsvIdFromQueryL);
	ADD_ASYNC_TEST_STEP(TestDoAppendIdConstraintL);
	ADD_ASYNC_TEST_STEP(TestDoCreateInsertSortQueryL);
	ADD_ASYNC_TEST_STEP(TestReplaceWildCardCharactersL);
	ADD_ASYNC_TEST_STEP(TestDoCreateSortQueryL);
	ADD_ASYNC_TEST_STEP(TestDoCreateSearchQueryL);
	ADD_ASYNC_TEST_STEP(TestFindL);
	ADD_ASYNC_TEST_STEP(TestUpdateNewIdsL);
	ADD_ASYNC_TEST_STEP(TestDoSortL);
	ADD_ASYNC_TEST_STEP(TestFindLSubFoldersL);
	ADD_ASYNC_TEST_STEP(TestCreateQueryDateL);
	ADD_ASYNC_TEST_STEP(TestIteratorFindL);
	END_SUITE;
	}
