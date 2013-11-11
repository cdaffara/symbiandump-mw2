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
// msvsearchsortquery.h
// 
//
/**
 * @file 
 * @publishedPartner
 * @released
 */
#ifndef __MSVSEARCHSORTQUERY_H__
#define __MSVSEARCHSORTQUERY_H__

#include <mentact.h>
#include <msvstd.h>
#include <msgerr.h>

// forward declaration
class CMsvSearchSortOperation;

/** Search Sort progress information
@publishedPartner
@released
*/
const TInt KMsvSearchSortOpNone=0;
const TInt KMsvSearchSortOpInProgress=1;
const TInt KMsvSearchSortOpCompleted=2;
const TInt KMsvSearchSortOpCanceled=3;
const TInt KMsvSearchSortOpFailed=4;
const TInt KMsvSearchSortOpLowMemory=5;
const TInt KMsvSearchSortOpIterationEnabled=6;
const TInt KMsvSearchSortOpIterationInProgress=7;

/** Search and Sort results
@publishedPartner
@released
*/
enum TMsvSearchSortResultType
	{
	EMsvResultAsTMsvEntry = 0,
	EMsvResultAsTMsvId
	};


/** Search or sort will be in Ascending/Descending order
@publishedPartner
@released
*/
enum TMsvSortOrder
	{
	EMsvSortAscending = 0, // Sorting in Ascending order
	EMsvSortDescending // Sorting in Descending order.
	};


/** Following enum's can be used along with TMsvMessagePart to create the query string
@publishedPartner
@released
*/	
enum TMsvRelationOp
	{
	EMsvEqual = 0, 
	EMsvNotEqual, 
	EMsvLessThan , 
	EMsvLessThanOrEqual,
	EMsvGreaterThan, 
	EMsvGreaterThanOrEqual,
	};

/** Searching or Sorting will be based on TMsvMessagePart
@publishedPartner
@released
*/	
enum TMsvMessagePart
	{
	EMsvAttachment = 1,
	EMsvDetails,
	EMsvDescription,
	EMsvSubject,
	EMsvDate,
	EMsvSize,
	EMsvPriority,		
	EMsvMtmTypeUID,
	EMsvUnreadMessages,
	EMsvNew,
	EMsvTo,
	EMsvFrom,
	EMsvCc,
	EMsvBcc,
	EMsvBody
	};

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
#endif

struct TMsvQueryTable;

/**
* Class: CMsvSearchSortQuery
* 
* CMsvSearchSortQuery is used to create Searchsort Query
* 
* @publishedPartner
* @released
*/
class CMsvSearchSortQuery : public CBase
	{
public:
	// get CMsvSearchSortQuery object
	IMPORT_C static CMsvSearchSortQuery* NewL();
	IMPORT_C static CMsvSearchSortQuery* NewLC();
	//destructor
	IMPORT_C ~CMsvSearchSortQuery();
	
	//to add search options
	IMPORT_C void AddSearchOptionL (TMsvMessagePart aMsgPart, const TDesC& aQueryDesValue, TMsvRelationOp aRelOp);
	IMPORT_C void AddSearchOptionL (TMsvMessagePart aMsgPart, const TInt  aQueryIntValue, TMsvRelationOp aRelOp);
	IMPORT_C void AddSearchOptionL (TMsvMessagePart aMsgPart, TBool aMessagePartFlagStatus);
	//to add sort option
	IMPORT_C void AddSortOptionL (TMsvMessagePart aMessagePart, TMsvSortOrder aSortOder);
	
	IMPORT_C void SetParentId(const TMsvId aParentId); 	//set ParentId, so that search/sort will be done on this
	IMPORT_C void SetCaseSensitiveOption(TBool aCaseSensitive);	//For CaseSensitive search
	IMPORT_C void SetWholeWord(TBool aWholeWord);				//For Whole World search
	IMPORT_C void SetWildCardSearch (TBool aWildCard);		//For Wild Character 
	IMPORT_C void SetResultType (TMsvSearchSortResultType aResultType);	//for setting result type
	IMPORT_C void SetSubFolderSearch(TBool aSubFolderSearch);	//to enable sub folder search
	
private:
	//constructor
	CMsvSearchSortQuery();
	void ConstructL();
	
	TBool IsWholeWord();				// is whole word search enabled
	TBool IsCaseSensitive();			// is case sensitive
	TBool IsWildCardSearch();		// is wildcard search enabled
	TBool IsSubFolderSearch();		// is sub folder search enabled
	TBool IsSearchQuery();
	
	TMsvSearchSortResultType GetResultType ();		//get result type
	TMsvMessagePart GetMessagePart(TInt aIndex);	// get the message part
	TMsvMessagePart GetMessagePart();	// get the message part
	TMsvRelationOp GetRelationOp(TInt aIndex);
	TInt GetMaxQuerySize();		// to get max query size 
	TInt ValidateQueryString(TMsvMessagePart aMsgPart, const TDesC& aQueryString, TMsvRelationOp aRelOp);
	TBool ValidateMessagePartAgainstEMsvEqualOperator(TMsvMessagePart aMessagePart);
	TBool ValidateMessagePartAgainstRelationOp(TMsvRelationOp aRelationOp);
			
private:
	TMsvId iParentId;			//search or sort will be on this Id
	TBool iCaseSensitive; 		//for case sensitive search
	TBool iWholeWord;			//for whole world search
	TBool iWildCardCharacter; 	//for Wild Character 
	TInt iQueryCount;		//for counting level of search
	TInt iCountMultilevelSort; //single level sort supported			
	TMsvMessagePart iSortOnMessagePart; //sort based on this message field
	TBool iExplicitSort;    	//explicit sort on searched results
	TBool iIsSearchQuery;		//to check whether is this search query 
	TMsvSearchSortResultType iSearchSortResultType;	// to set result type
	TBool iSubFolderSearch;			// flag for sub folder search
	TMsvSortOrder iSortOrder;		// search sort results will be in Ascending or Descending order
	TInt iQueryId;                  //to store query id value
	TMsvQueryTable *iQueryTable;  //iQueryTable must be kept at end of class to simplify pack & unpack query
	
	friend class CMsvSearchSortCacheEntry;
	friend class CMsvSearchSortOperation;
	friend class CMSvSearchSortCacheManager;
	friend class CMsvSearchsortOpOnHeaderBody;
	friend class TMsvPackQuery;
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestSearchSortCacheEntry;
	friend class CTestSearchSortDBadapter;
	friend class CTestSearchSortCacheManager;
	friend class CTestSearchSortOperation;
	friend class CTestHeaderSearchSortOperation;
	friend class CTestSearchSortDeltaCache;
	friend class CTestOOMSearchSortCacheManager;
	friend class CTestMsvIpc;
	friend class CTestAttachmentSearch;
#endif
	};

#endif /*__MSVSEARCHSORTQUERY_H__*/	
