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

#ifndef MSVSEARCHSORTCACHEENTRY_H_
#define MSVSEARCHSORTCACHEENTRY_H_

#include <msvsearchsortquery.h>
#include <e32base.h>
#include <msvstd.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
#endif


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
enum TTypeOfSearchQuery
	{
	EQueryInvalid = 0,       
	EQueryOnIndexEntry = 1,  
 	EQueryOnHeader = 2,      
 	EQueryOnEntryAndHeader =3,
 	EQueryOnBody = 4,         
 	EQueryOnEntryAndBody = 5,
 	EQueryOnHeaderAndBody = 6,
 	EQueryOnEntryHeaderAndBody = 7
	};


#else

//Type of Query : To indicate the type of query.
//This enum is used by manager class alos so kept it out side the Cache entry clas..
enum TTypeOfQuery
	{
	ENotValid=0,
	EIndexQuery,
	EHeaderBodyQuery,
	ECombinedQuery,
	};
	
#endif

// Forward Declaration
class CMSvSearchSortCacheManager;

/**********************************
 * CMsvSearchSortCacheEntry :
 * Description : Create a  Search or Sort Cache Entry using the client Query.
 * @internalComponent
 * @prototype
 ***********************************/ 
NONSHARABLE_CLASS (CMsvSearchSortCacheEntry) : public CBase
 	{
public:
	static CMsvSearchSortCacheEntry* CreateQueryEntryL(const CMsvSearchSortQuery* aQuery,const TBool aMarkQuery,TInt aiteratorCount = 0);
	~CMsvSearchSortCacheEntry(); //Destructor.
public:
	inline TMsvId& ReturnParentId();
	inline TMsvSearchSortResultType& GetResultType();
	inline void DoSortQuery(); //Set if Query is a Sort Query.
	inline void DoCaseSensetive();	//Case Sensitive  bit is set.
	inline void DoWholeWord();	//Whole Word bit is set.
	inline void DoAscendingSort();	//Ascending / Descending Sort option bit is set. by default it is by date and time.
	inline void DoWildCharacter(); //set Wild Character option .
	inline void DoExplicitSortReq(); // Set the flag for Explicit sorting .
			
	inline TBool IsSortQuery() const;//Get Search or Sort Query .
	inline TBool IsCaseSensetive() const;	//Retrive  Case Sensitive  bit is set or not.
	inline TBool IsWholeWord() const;	//Retrive  Whole Word bit is set or not.
	inline TBool IsAscendingSort() const;	//Retrive  Ascending / Descending Sort option bit is set or not.
	inline TBool IsWildCharacter() const; //Get Wild Character option .
	inline TBool IsExplicitSortReq() const; //Get Explicit Sorting Option.
private:
	//CMsvSearchSortCacheEntry();//Default Constructor
	CMsvSearchSortCacheEntry(const CMsvSearchSortQuery* aQuery,const TBool aMarkQuery,TInt aiteratorCount); 
	void ConstructQueryNodeL(const CMsvSearchSortQuery* aQuery);
private:
	enum TQueryFlag
		{
		ESortQuery = 		0x0001,
		ECaseSensitive = 	0x0002,
		EWholeWord = 		0x0004,
		EAscendingSort = 	0x0008,
		EWildCharacter = 	0x0100,
		EExplicitSort = 	0x1000
		};

	TBool iSubFolderOption;
	TBool iFileAccess;  //This  will indicate the Query is on index entry or header/Body .
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)			
	TInt iTypeOfQuery;  // what kind of search/sort query (i.e. Search on Entry, Header, body fileds or combined query )
#else
	TTypeOfQuery iQueryType;
#endif	
	TBool iSortOnHeader;
	TInt iteratorCount;
	TMsvId iParentId; 
	TInt iQueryID; //This is a unique 32bit integer value generated after Result ID's found by manager class.
	TInt iQueryOptions; //32bit interger value for different query options like.. Query Type,KCaseSensitive  , KWholeWord etc;
	TInt iQueryLevel;
	// Structure for storing the Client Query. 		
	typedef struct 
		{
		TMsvMessagePart iMessagePart;
		TMsvRelationOp iRelationOp;
		TBuf<KMsvMaxSearchTextLength> iQueryString;
		}TQueryData;
	TQueryData iQueryData;
	RArray<TQueryData> iMsgQuery; // The RArray owning the TQueryData. 
  	TBool iMarkQuery;
 	TMsvMessagePart iMsgExplicitSortPart;
	TMsvSearchSortResultType iResultType;
	friend class CMSvSearchSortCacheManager;
	friend class TSearchSortDbWrapper;
	friend class CMsvServerSession;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestSearchSortCacheEntry;
	friend class CTestSearchSortCacheManager;
	friend class CTestSearchSortDBadapter;
#endif
	};	
	
#include "msvsearchsortcache.inl"

#endif /*MSVSEARCHSORTCACHEENTRY_H_*/
