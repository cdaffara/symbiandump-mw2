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

#ifndef __MSVSEARCHSORTDBWRAPPER_H__
#define __MSVSEARCHSORTDBWRAPPER_H__
#endif


#include "MSVSTD.H"
#include <msvsearchsortquery.h>
#include "msvsearchsortcacheentry.h"
#include "msvdbadapter.h"
#include <sqldb.h>
#include <e32cmn.h>
#include <msvapi.h>

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include "msvmessagedbadapter.h"
#endif //#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)


/**
TSearchSortDbWrapper
This calss is used to convert the cache entry into a SQL DB query, which is 
passed to the DB to create, extract or manipulate data on SearchSortEntry table.

@internalAll 
@released
*/
class TSearchSortDbWrapper
	{
	public:
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		static TSearchSortDbWrapper* NewL(CMsvServer& aServer);
		void FindL(const CMsvSearchSortCacheEntry& aQuery, RArray<TMsvId>& aIdResult, const TBool aSortOnHeader/*DEFAULT = EFalse*/, TInt aTypeOfQuey, TInt& aStatusOfQuery);
		void UpdateNewIdsL(const CMsvSearchSortCacheEntry& aQuery,const RArray<TMsvId>& aIdFilter, RArray<TMsvId>& aIdResult,const TBool aSortOnHeader, TInt aTypeOfQuey, TInt& aStatusOfQuery);
		void GetIdsInIteratorNewQueryL(const CMsvSearchSortCacheEntry& aQuery, TBool aSortOnHeader, TInt aTypeOfQuery, TInt& aReturnResultType, const TInt aLimit = 1);
		void CloseResources();
#else
		static TSearchSortDbWrapper* NewL(CMsvDBAdapter& aDBadapter);
		void FindL(const CMsvSearchSortCacheEntry& aQuery, RArray<TMsvId>& aIdResult, const TBool aSortOnHeader = EFalse);
		void UpdateNewIdsL(const CMsvSearchSortCacheEntry& aQuery,const RArray<TMsvId>& aIdFilter, RArray<TMsvId>& aIdResult, const TBool aSortOnHeader = EFalse);
		void GetIdsInIteratorNewQueryL(const CMsvSearchSortCacheEntry& aQuery, const TInt aLimit = 1);
#endif
		void AddIdtoDBL(const TInt aQueryId,const RArray<TMsvIdWithSortField>& aIdSelection);
		void GetSortedTMsvIdsfromTableL(const TInt aQueryId, RArray<TMsvId>& aIdResult, TBool aOrder = EFalse, TMsvMessagePart aMessagePart = EMsvDate);
		void DeleteTMsvIdFromAllQueriesL(const RArray<TMsvId>& aIdSelection);
		void DeleteTMsvIdFromQueryL(const TInt aQueryId,const RArray<TMsvId>& aIdSelection);
		void DeleteQueryFromTableL(const TInt aQueryId);
		TInt ReturnResultCountInTableL(const TInt aQueryId);
		
		void GetIdsInIteratorQueryIdL(const TInt& aQueryId, const TBool aOrder = ETrue, TMsvMessagePart aMessagePart = EMsvDate, const TInt aLimit = 1);
		
		void GetNextIdL(TMsvId& aId, TInt& aRemainingCount);
		inline CMsvDBAdapter* GetDbAdapter();
	
	private:
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	enum THeaderBodyTableStatusAtDb 
		{
		EInvalid = 0,
		EAvailable = 1,
		EPartiallyAvailable =3,
		ENotAvailable = 4
		};
#endif

	private:

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		TSearchSortDbWrapper(CMsvServer& aServer);
#else		
		TSearchSortDbWrapper(CMsvDBAdapter& aDBadapter);
#endif	
		void CreateQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString,const TMsvId& aParentId, TBool aBypassSort=EFalse);
		void ConstructL(RSqlDatabase& aDataBase);
		
		void DoAddRelationalOperatorL(RBuf8& aQuery,TMsvRelationOp aRelationOp);
		void DoCreateSearchQueryL(const CMsvSearchSortCacheEntry& aQuery,RBuf8& aSearchQuery);
		void DoCreateSortQueryL(const CMsvSearchSortCacheEntry& aQuery,RBuf8& aSortQuery);
		void DoCreateInsertSortQueryL(const CMsvSearchSortCacheEntry& aQuery,RBuf8& aSortQuery, TMsvId aParentId);
		void DoAppendIdConstraint(const RArray<TMsvId>& aIdFilter, RBuf8& aSortQuery, TInt& aIdCount);
		void ReplaceWildCardCharacters(TDes8& aString,const CMsvSearchSortCacheEntry& aQuery );
	
		TBool IsSortAsNumRequired(const TMsvMessagePart& aMessagePart);
		TMsvRelationOp GetDateRelationalOperator(const CMsvSearchSortCacheEntry& aQuery,TInt aCurrentIndex);
		
		void IsCaseSensitiveSearch(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TInt aIndex);
		void UpdateQueryWithTMsvEntrySortFileldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)		
		void UpadeteQueryWithParentIdAndMsgType(RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId);
		void UpadeteQueryWithIndexEntryTable(RBuf8& aQueryString, TUint aDriveId);
		void UpdateQueryWithDriveNumAndIndexEntryTable(RBuf8& aQueryString, TUint aDriveId);
		void UpdateQueryWithTMsvEntrySearchFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId);
#else
		void UpadeteQueryWithParentIdAndMsgType(RBuf8& aQueryString, TMsvId aParentId);
		void UpdateQueryWithTMsvEntrySearchFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TMsvId aParentId);
#endif		

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
		void ProcessPreQueryRequestL(TMsvId aParentId, TInt& aStatusOfQuery);
		THeaderBodyTableStatusAtDb StatusOfHeaderTableL(TMsvId aParentId);
		void QueryToGetMtmIdsL(TMsvId aParentId, TUint aDriveId);
		void DoUpdateQuery (RBuf8& aQueryString);
		void DoUpdateSearchQueryForHeaderFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TMsvId aParentId);
		void DoUpdateSearchQueryForBodyFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString);
		void DoUpdateSortQueryForHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId);
		void DoCreateInsertSortOnHeaderQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString);
		void DoAddHeaderFields(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TInt aIndex, TInt aMtmIdArrayIndex);
		void CompareTMsvIds(RBuf8& aQueryString, TInt aIndex);
		void CompareTMsvIds(RBuf8& aQueryString, TUint aDriveId);
		void UpdateQueryWithSearchSortEntryTable(RBuf8& aQueryString);
		void UpdateQueryToGetQueryIdTMsvId(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId);
		void UpdateQueryWithHeaderTable(RBuf8& aQueryString, TUint aDriveId, TInt32 aMtmId);
		void UpdateQueryWithMultipleTablesL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId);
		void UpdateQueryWithSortFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId);
		void UpdateQueryWithDriveNumAndHeaderTable(TUint aDriveId, RBuf8& aQueryString);
		void UpdateHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString);
		void UpdateQueryToRemoveEmptyHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId);
		void ParseQueryForHeaderFields(const CMsvSearchSortCacheEntry& aQuery);
		void ParseCommonHederFilelds(TMsvMessagePart aMessagePart, TBool aIsSearhField);
		TBool IsSearchOnEntryAndBody(const CMsvSearchSortCacheEntry& aQuery);
		TBool IsSearchQueryOnBody(TBool aIsOnlyBody=EFalse);
#endif
		
	private:
	enum
		{
		KMsvEntryLowPriority=			0x00000003,
		KMsvEntryMediumPriority=		0x00000001,
		KMsvEntryHighPriority=			0x00000002,
		KMsvEntryNewFlag=				0x00000010,
		KMsvEntryUnreadFlag=			0x00000020,
		KMsvEntryAttachmentFlag=		0x00008000
		};
		
		CMsvDBAdapter* iDBadapter;
			
		TInt iTotalCount;
		TInt iLocation;
		TInt iProcessedIndexQueries;
		TBool 	iSortOnHeader;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		TInt 	iTypeOfQuery; // type of query 
		TInt 	iStatusOfQuery; // to send the status of the query to ChacheManager
		TInt32 	iHeaderTableForSort;  // mtmId for hedaer table 
		TInt 	iHeaderTableStdFields; // to check header fields on a given query
		TInt 	iTotalNumMsgPartsOnHeader; // total number of message parts to search on header
		TInt	iNumOfMsgParts; // to add AND or OR 
		THeaderBodyTableStatusAtDb iHeaderTableAtDB;
		THeaderBodyTableStatusAtDb iBodyTableAtDB;
		
		// need close the array by calling TSearchSortDbWrapper::CloseResources()
		RArray<TUid> iMtmIdArray;
		CMsvServer& iServer;
#endif	
		
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		friend class CTestSearchSortDBadapter;
#endif
	};

inline CMsvDBAdapter* TSearchSortDbWrapper::GetDbAdapter()
	{
	return iDBadapter;
	}
	
