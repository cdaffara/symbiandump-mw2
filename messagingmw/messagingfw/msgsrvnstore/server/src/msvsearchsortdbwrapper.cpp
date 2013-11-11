// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// HEADER FILES
// 
//
 
 #include "msvsearchsortdbwrapper.h"
 #include "MSVSTD.H"
 #include "msvindexadapter.h"
 
 #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
 #include "MSVSERV.H"
 #endif
 
 const TInt KMaxQuerySize = 4000;
 const TInt KMaxInCount = 50;
 
 
/**
 * LITERAL DEFINITION
 */
_LIT8(KQuote, "\"");
_LIT8(KComma, ", ");
_LIT8(KAnd, " AND ");
_LIT8(KLike, " LIKE ");
_LIT8(KPercent, "%");
_LIT8(KUnderScore, "_");
_LIT8(KDescription, "description");	
_LIT8(KDetails, "details");
_LIT8(KDate, "date");
_LIT8(KSize, "size");
_LIT8(KMtmId, "mtmId");
_LIT8(KType,"type");
_LIT8(KCloseBrace, ")");
_LIT8(KAscending," ASC");
_LIT8(KDescending," DESC");
_LIT8(KDelimiter,";");
_LIT8(KOrderBy," ORDER by ");
_LIT8(KBitwiseData,"data & ");
_LIT8(KId, "id");
_LIT8(KSelect, "SELECT ");
#if (!defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
_LIT8(KIdIn, "id IN(");
#endif
_LIT8(KWhere, " WHERE ");
_LIT8(KNull, "NULL");

_LIT8(KPrioritySort, "data & 3");
_LIT8(KAttachmentSort, "data & 32768");
_LIT8(KNewMsgSort, "data & 16");
_LIT8(KUnReadSort, "data & 32");


_LIT8(KIndexSortInsert, "INSERT OR IGNORE INTO SearchSortEntry (Qid, id, sortfield) ");

_LIT8(KGreaterThan, " > ");
_LIT8(KLessThan, " < ");
_LIT8(KGreaterThanEqualTo," >= ");
_LIT8(KLessThanEqualTo," <= ");
_LIT8(KNotEqualTo," != ");
_LIT8(KEqualTo, " = ");
	


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	_LIT8(KFromDb," FROM DB");
	_LIT8(KIndexSortSelectRemovable, ".IndexEntry WHERE ");
	_LIT8(KIndexSortSelectNoParentRemovable, ".IndexEntry ");
	_LIT8(KIdQuerySelect, "SELECT DISTINCT id FROM DB");
#else
	_LIT8(KIdQuery, "SELECT DISTINCT id ");
	_LIT8(KIdQueryNoParent, "SELECT DISTINCT id FROM IndexEntry ");
	_LIT8(KIndexSortSelect, " FROM IndexEntry WHERE parentId = ");
	_LIT8(KIndexSortSelectNoParent, " FROM IndexEntry ");
#endif

/**
 * TSearchSortDbWrapper
 */



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	

TSearchSortDbWrapper* TSearchSortDbWrapper::NewL(CMsvServer& aServer)
	{
	//Create an Search sort db adaption object 
	TSearchSortDbWrapper* self = new(ELeave) TSearchSortDbWrapper(aServer);
	return self;
	}

TSearchSortDbWrapper::TSearchSortDbWrapper(CMsvServer& aServer) : iServer(aServer)
	{
	iDBadapter = iServer.Context().IndexAdapter()->GetDbAdapter();
				 
	iTotalCount = -1;
	iProcessedIndexQueries = 0;
	}

void TSearchSortDbWrapper::CloseResources()
	{
	// relese the resources
	iMtmIdArray.Close();	
	}

#else 	// SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB

/**
 NewL()
It returns an instance of TSearchSortDbWrapper class. 

@param CMsvDBAdapter& aDBadapter: reference to CMsvDBAdapter Class.
@return The newly created Msv DBadaption Object.
@leave: KErrNone if successful or any of system wide leave codes if not.
*/
TSearchSortDbWrapper* TSearchSortDbWrapper::NewL(CMsvDBAdapter& aDBadapter)
	{
	//Create an Search sort db adaption object 
	TSearchSortDbWrapper* self = new(ELeave) TSearchSortDbWrapper(aDBadapter);
	return self;
	}

/**
 TSearchSortDbWrapper()
Constructor for the TSearchSortDbWrapper class.

@param 	None.
@return None.
*/
TSearchSortDbWrapper::TSearchSortDbWrapper(CMsvDBAdapter& aDBadapter):iDBadapter(&aDBadapter), iTotalCount(-1), iProcessedIndexQueries(0)
	{
	}

#endif // if (!defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)


/** 
  FindL()
Depending on the CMsvSearchSortCacheEntry passed to this function it constructs the DB query
and gets the resultant TMsvIds for the query so formed.

@param CMsvSearchSortCacheEntry& aQuery:	Contains all the SearchSort related information(What to search on, what to search for).
@param RArray<TMsvId>& aIdResult: RArray which holds the resultant TMsvIds.
@param TBool aSortOnHeader: This specifies whether the sort field is a part of the Header(To, Bcc, Cc) or TMsvEntry(Date, Size, priority).
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/ 
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
void TSearchSortDbWrapper::FindL(const CMsvSearchSortCacheEntry& aQuery, RArray<TMsvId>& aIdResult, const TBool aSortOnHeader, TInt aTypeOfQuery, TInt& aStatusOfQuery)
#else
void TSearchSortDbWrapper::FindL(const CMsvSearchSortCacheEntry& aQuery, RArray<TMsvId>& aIdResult, const TBool aSortOnHeader/*DEFAULT = EFalse*/)
#endif
	{
	RBuf8 queryString;
	queryString.CreateL(KMaxQuerySize);
	queryString.CleanupClosePushL();
	
	RArray<TMsvId> folderIdList;
	CleanupClosePushL(folderIdList);

	TMsvId parentId;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	parentId = UnmaskTMsvId(aQuery.iParentId);
#else
	parentId = aQuery.iParentId;
#endif  
	//Append the parentid on which the search/sort is intiated into the folder list
	folderIdList.AppendL(parentId);

	iSortOnHeader = aSortOnHeader;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	iTypeOfQuery = aTypeOfQuery;
	iTotalNumMsgPartsOnHeader = 0;
	iNumOfMsgParts = 0;
	
	ParseQueryForHeaderFields(aQuery);
#endif	
	
	for(TInt folderindex = 0; folderindex < folderIdList.Count(); ++folderindex)
		{
		//If subfolder search is enabled then all the subfolders under the parent and its 
		// childrens are searched for a match.
		if(aQuery.iSubFolderOption && parentId != KMsvRootIndexEntryIdValue)
			{
			iDBadapter->GetFolderIdsL(folderIdList[folderindex], folderIdList);	
			}
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		// get MtmIds for corresponding parentId
		TRAPD(error, ProcessPreQueryRequestL(folderIdList[folderindex], aStatusOfQuery));
		
		if(error == KErrArgument)
			{
			queryString.Delete(0,KMaxQuerySize);
			continue;
			}
#endif
		
		//Create the SQLquery from the CMsvSearchSortCacheEntry and the parentid specified
		CreateQueryL(aQuery, queryString, folderIdList[folderindex]);
		
		//If the sort is done on a Header specific part(To, Cc, Bcc), sortfield is not stored in DB
		//It has to be fetched from client side, hence sending the TMsvIds to client without storing.
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		if(iStatusOfQuery == KNewQuery)
#else
		if(iSortOnHeader)
#endif				
			{
			iDBadapter->SearchTMsvIdL(queryString, aIdResult);
			}
		//If the sort is done on a TMsvEntry specific part(size, date, mtmtype), sortfield is stored in DB
		else  // (iStatusOfQuery == KPartialResult || iStatusOfQuery == KFinalResult)
			{
			iDBadapter->AddEntriesfromIndexTableL(queryString);
			}
		queryString.Delete(0,KMaxQuerySize);
		}

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
	if(iStatusOfQuery == KPartialResult || iStatusOfQuery == KFinalResult)
#else
	if(!iSortOnHeader)
#endif
		{
		//If the sort is done on a TMsvEntry fetch all the TMsvIds matching this query and pass it to client
		iDBadapter->GetSearchSortEntriesL(aQuery.iQueryID, aIdResult, aQuery.IsAscendingSort(), IsSortAsNumRequired(aQuery.iMsgExplicitSortPart));	
		}
	
	iProcessedIndexQueries = 0;
	CleanupStack::PopAndDestroy(&folderIdList);  // for folderIdList;
	CleanupStack::PopAndDestroy(); // for  queryString;
	}


/** 
  UpdateNewIdsL()
Depending on the CMsvSearchSortCacheEntry and the TMsvIds passed to this function it constructs the DB query
and gets the resultant TMsvIds for the query so formed.

@param CMsvSearchSortCacheEntry& aQuery:	Contains all the SearchSort related information(What to search on, what to search for).
@param RArray<TMsvId>& aIdFilter: RArray holding the entries to be filtered with
@param RArray<TMsvId>& aIdResult: RArray which holds the resultant TMsvIds.
@param const TBool aSortOnHeader: This specifies whether the sort field is a part of the Header(To, Bcc, Cc) or TMsvEntry(Date, Size, priority).
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/ 

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
void TSearchSortDbWrapper::UpdateNewIdsL(const CMsvSearchSortCacheEntry& aQuery,const RArray<TMsvId>& aIdFilter, RArray<TMsvId>& aIdResult,const TBool aSortOnHeader, TInt aTypeOfQuery, TInt& aStatusOfQuery)
#else
void TSearchSortDbWrapper::UpdateNewIdsL(const CMsvSearchSortCacheEntry& aQuery,const RArray<TMsvId>& aIdFilter, RArray<TMsvId>& aIdResult,const TBool aSortOnHeader/*DEFAULT = EFalse*/)
#endif
	{
	if(aIdFilter.Count() != 0)
		{
		TInt idArrayCount = 0;
		TInt initQuerySize = 0;
		TInt idsAdded = 0;
		
		RBuf8 queryString;
		queryString.CleanupClosePushL();
		queryString.CreateL(KMaxQuerySize);
		
		//Folderlist for subfolder search
		RArray<TMsvId> folderIdList;
		TMsvId parentId;
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		parentId = UnmaskTMsvId(aQuery.iParentId);
	#else
		parentId = aQuery.iParentId;
	#endif  
		//Append the parentid on which the search/sort is intiated into the folder list
		folderIdList.AppendL(parentId);
		
		iSortOnHeader = aSortOnHeader;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		iTypeOfQuery = aTypeOfQuery;
		iTotalNumMsgPartsOnHeader = 0;
		iNumOfMsgParts = 0;
		ParseQueryForHeaderFields(aQuery);
#endif
		
		for(TInt folderindex = 0; folderindex < folderIdList.Count(); ++folderindex)
			{
			//If subfolder search is enabled then all the subfolders under the parent and its 
			// childrens are searched for a match.
			if(aQuery.iSubFolderOption && parentId != KMsvRootIndexEntryIdValue)
			{
			iDBadapter->GetFolderIdsL(folderIdList[folderindex], folderIdList);	
			}

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
			// get MtmIds for corresponding parentId
			ProcessPreQueryRequestL(folderIdList[folderindex], aStatusOfQuery);
#endif
			//Create the SQLquery from the CMsvSearchSortCacheEntry and the parentid specified
			CreateQueryL(aQuery, queryString, folderIdList[folderindex], ETrue);
			
			//initQuerySize is made one less than entire size to remove the delimiter(;)
			initQuerySize = queryString.Size()-1;
			
			//Reset Query string to Append New ID's
			queryString.Delete(initQuerySize,KMaxQuerySize);
			
			if(iProcessedIndexQueries == 0 && folderIdList[folderindex] == KMsvRootIndexEntryIdValue)
				{
				queryString.Append(KWhere);
				}
			else
				{
				queryString.Append(KAnd);
				}
				
			//initQuerySize
			initQuerySize = queryString.Size();
			
			while(idArrayCount < aIdFilter.Count())
				{
				//Reset Query string to Append New ID's
				queryString.Delete(initQuerySize,KMaxQuerySize);
				
				//Append Id's into the In statement
				DoAppendIdConstraint(aIdFilter, queryString, idArrayCount);
				//If the Sort is on Header, need not store the result as it doesnt have the sortfield
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
				if(iStatusOfQuery == KNewQuery)
#else
				if(iSortOnHeader)
#endif	
					{
					iDBadapter->SearchTMsvIdL(queryString, aIdResult);
					}
				//If the Sort is on index we need to store the result and then send the id's
				else
					{
					idsAdded += iDBadapter->AddEntriesfromIndexTableL(queryString);
					}
				}
			iProcessedIndexQueries = 0;
			idArrayCount = 0;
			queryString.Delete(0,KMaxQuerySize);
			}
		
		//Get only the updated results for this type of query
		// not the previously stored results
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		if(iStatusOfQuery == KNewQuery)
#else
		if (aQuery.iQueryType == ECombinedQuery && !aQuery.iSortOnHeader)
#endif		
			{
			iDBadapter->GetLastNTMsvIdsfromTableL(aQuery.iQueryID, aIdResult, idsAdded,aQuery.IsAscendingSort(), IsSortAsNumRequired(aQuery.iMsgExplicitSortPart));
			}
		
		folderIdList.Close();
		CleanupStack::PopAndDestroy();
		}

	//Get the final list of sorted ids from the SearchSortEntry table
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iStatusOfQuery == KPartialResult || iStatusOfQuery == KFinalResult)
#else
	if(!iSortOnHeader)
#endif
		{
		iDBadapter->GetSearchSortEntriesL(aQuery.iQueryID, aIdResult, aQuery.IsAscendingSort(), IsSortAsNumRequired(aQuery.iMsgExplicitSortPart));	
		}
	}
	
	
/** 
  CreateQueryL()
Depending on the CMsvSearchSortCacheEntry and the TMsvIds passed to this function it constructs the DB query
and gets the resultant TMsvIds for the query so formed.

@param CMsvSearchSortCacheEntry&:	Contains all the SearchSort related information(What to search on, what to search for).
@param RBuf8&: SQL query created depending on the CMsvSearchSortCacheEntry
@param TMsvId& : ParentId on which the the search/sort is performed on
@param TBool: This Specifies whether sorting is done on Header or TMsvEntry field
@param TBool: This Specifies whether the sort option should be processed or not
@leave: KErrNone if successful, KErrNotSupported if the Search/Sort message passed is not supported
		 or any of system wide leave codes or SQL leave codes.
@return none.
*/

void TSearchSortDbWrapper::CreateQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, const TMsvId& aParentId, TBool aBypassSort)
	{
	// If sort field is on Index part, result should be stored on the DB
	if(!iSortOnHeader) //it's on TMsvEntry
		{
		DoCreateInsertSortQueryL(aQuery, aQueryString, aParentId);
		}
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	// If sort field is on Header part and if Header table/s is availble or partiallly available 
	// result should be stored on the DB
	else if(iSortOnHeader && (iHeaderTableAtDB == EAvailable || iHeaderTableAtDB == EPartiallyAvailable))
		{
		DoCreateInsertSortOnHeaderQueryL(aQuery, aQueryString);
		}
#endif		
	else
		{
		if(aParentId != KMsvRootIndexEntryIdValue)
			{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aQueryString.Append(KIdQuerySelect);
			aQueryString.AppendNum(KCurrentDriveId);
			aQueryString.Append(KIndexSortSelectRemovable);
		#else
			aQueryString.Append(KIdQuery);
		#endif
			}
		else
			{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aQueryString.Append(KIdQuerySelect);
			aQueryString.AppendNum(KCurrentDriveId);
			aQueryString.Append(KIndexSortSelectNoParentRemovable);
		#else
			aQueryString.Append(KIdQueryNoParent);
		#endif
			} 
	
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		UpadeteQueryWithParentIdAndMsgType(aQueryString, KCurrentDriveId, aParentId);
	#else
		UpadeteQueryWithParentIdAndMsgType(aQueryString, aParentId);
	#endif		
		
		}
			
	//Checks whether the query has a search part specified, 
	//if yes then SQL query is created for the same
	if(!aQuery.IsSortQuery())
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		if((iTypeOfQuery & EQueryOnHeader) && (iHeaderTableAtDB == EAvailable || iHeaderTableAtDB == EPartiallyAvailable ))
			{
			DoUpdateSearchQueryForHeaderFieldsL(aQuery, aQueryString, aParentId);
			}
		
		if(iTypeOfQuery & EQueryOnIndexEntry)
#endif		
			{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
			UpdateQueryWithTMsvEntrySearchFieldsL(aQuery, aQueryString, KCurrentDriveId, aParentId);
		#else
			UpdateQueryWithTMsvEntrySearchFieldsL(aQuery, aQueryString, aParentId);
		#endif
			}
		}
	
	//Checks whether the query has a sort part specified
	//if yes then SQL query is created for the same
	if ((aQuery.IsExplicitSortReq() || aQuery.IsSortQuery()) && !aBypassSort)
		{
		if(!iSortOnHeader)
			{
			DoCreateSortQueryL(aQuery, aQueryString);
			}
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)			
		if(iSortOnHeader && (iHeaderTableAtDB == EAvailable || iHeaderTableAtDB == EPartiallyAvailable))
			{
			UpdateQueryWithSortFieldL(aQuery, aQueryString, KCurrentDriveId);
			}
#endif			
		}
	
	//Adds the delimiter(;)
	aQueryString.Append(KDelimiter);

	//Replaces Symbian WildCards(?, *) with SQL Wildcards (_, %)
	if(aQuery.IsWildCharacter())
		{
		ReplaceWildCardCharacters(aQueryString,aQuery);
		}
	}

/** 
  ReplaceWildCardCharacters()
The Function replaces the '?' in the query with '_' and '*' in the string with a '%'.

@param TDes8& aString: string on which the replace operation has to be done on.
@return none.
*/
void TSearchSortDbWrapper::ReplaceWildCardCharacters(TDes8& aString,const CMsvSearchSortCacheEntry& aQuery)
	{
	
	for(TInt pos=0; pos < aString.Size(); pos++)
		{
		if(aString[pos] == '?')
			{
			aString.Replace(pos,1,KUnderScore);
			}
		else if(aString[pos] == '*')
			{
			if(!aQuery.IsCaseSensetive())
			    aString.Replace(pos,1,KPercent);
			}
		}
	}


/** 
  DoAddRelationalOperatorL()
The function Appends the required relational operator to the query string passed 
	depending on the enum type specified

@param RBuf8& aQuery: string on which the replace operation has to be done on.
@param TMsvRelationOp aRelationOp: Relational operator which has to be appended to the string
@return none.
@leave: KErrNone if successful, KErrNotSupported if the Search/Sort message passed is not supported.
*/
void TSearchSortDbWrapper::DoAddRelationalOperatorL(RBuf8& aQuery,TMsvRelationOp aRelationOp)
	{
	switch(aRelationOp)
		{
		case EMsvEqual:
			{
			aQuery.Append(KEqualTo);
			break;
			}
			
		case EMsvNotEqual:
			{
			aQuery.Append(KNotEqualTo);
			break;							
			}
	
		case EMsvLessThan:
			{
			aQuery.Append(KLessThan);
			break;
			}
	
		case EMsvGreaterThan:
			{
			aQuery.Append(KGreaterThan);
			break;
			}
	
		case EMsvLessThanOrEqual:
			{
			aQuery.Append(KLessThanEqualTo);
			break;
			}
	
		case EMsvGreaterThanOrEqual:
			{
			aQuery.Append(KGreaterThanEqualTo);
			break;
			}
	
		default:
			{
			User::Leave(KErrNotSupported);
			}
		}
	}
	

/** 
  DoCreateSearchQueryL()
Depending on the CMsvSearchSortCacheEntry passed to this function it constructs the SQLDB search query.

@param CMsvSearchSortCacheEntry& aQuery: Search/Sort query which has to be processed
@param RBuf8& aSearchQuery: The QueryString which will be prepared depending on CMsvSearchSortCacheEntry.
@param TMsvId& aParentId: ParentId on which the the search/sort is performed on
@return none.
@leave: KErrNone if successful, KErrNotSupported if the Search/Sort message passed is not supported.
*/
void TSearchSortDbWrapper::DoCreateSearchQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aSearchQuery)
	{

	for(TInt index=0; index<aQuery.iMsgQuery.Count(); index++)
		{
		switch (aQuery.iMsgQuery[index].iMessagePart)
			{
			//Header and Body Parts are searched at client side using FindL
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
			case EMsvBody:
				{
				break;
				}
			
			//Index Entry Parts to be searched
			//Search on Description
			case EMsvDescription:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KDescription);
				
				// update query for case sensitive or case insensitive serach
				IsCaseSensitiveSearch(aQuery, aSearchQuery, index);
				
				++iProcessedIndexQueries;
				
				break;
				}
			
			//Search on Details
			case EMsvDetails:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KDetails);
				
				// update query for case sensitive or case insensitive serach
				IsCaseSensitiveSearch(aQuery, aSearchQuery, index);
				
				++iProcessedIndexQueries;
				
				break;
				}
				
			//Search on Date
			case EMsvDate:  
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				TTime time;
				User::LeaveIfError(time.Set(aQuery.iMsgQuery[index].iQueryString));
				TInt64 date=time.Int64();
				
				aSearchQuery.Append(KDate);
				DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
				aSearchQuery.AppendNum(date);
												
				++iProcessedIndexQueries;
				
				break;
				}
				
			//Search on Size	
			case EMsvSize:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KSize);
				DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
				aSearchQuery.Append(aQuery.iMsgQuery[index].iQueryString);
				
				++iProcessedIndexQueries;
				
				break;
				}
				
			//Search on Attachment Flag		
			case EMsvAttachment:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KBitwiseData);
				aSearchQuery.AppendNum(KMsvEntryAttachmentFlag);
				//Entries Without attachment
				if(!aQuery.iMsgQuery[index].iQueryString.Compare(_L("0")))
					{
					aSearchQuery.Append(KNotEqualTo);
					}
				//Entries With attachment
				else
					{
					aSearchQuery.Append(KEqualTo);
					}
				aSearchQuery.AppendNum(KMsvEntryAttachmentFlag);
				
				++iProcessedIndexQueries;
				
				break;
				}
				
			//Search on MTM type
			case EMsvMtmTypeUID:
				{					
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KMtmId);
				DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
				aSearchQuery.Append(aQuery.iMsgQuery[index].iQueryString);
				
				++iProcessedIndexQueries;
				
				break;
				}
		
			//Search on High Priority Flag
			case EMsvPriority:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KBitwiseData);
				
				//Search on High Priority Flag
				if(!aQuery.iMsgQuery[index].iQueryString.Compare(_L("0")))
					{
					aSearchQuery.AppendNum(KMsvEntryHighPriority);
					DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
					aSearchQuery.AppendNum(KMsvEntryHighPriority);
					}
					
				//Search on Normal Priority Flag
				else if(!aQuery.iMsgQuery[index].iQueryString.Compare(_L("1")))
					{
					aSearchQuery.AppendNum(KMsvEntryMediumPriority);
					DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
					aSearchQuery.AppendNum(KMsvEntryMediumPriority);
					}
					
					//Search on Low Priority Flag	
				else if(!aQuery.iMsgQuery[index].iQueryString.Compare(_L("2")))
					{
					aSearchQuery.AppendNum(KMsvEntryLowPriority);
					DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
					aSearchQuery.AppendNum(0);
					}
				
				++iProcessedIndexQueries;
				
				break;
				}

			//Search on UnRead Flag
			case EMsvUnreadMessages:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KBitwiseData);
				aSearchQuery.AppendNum(KMsvEntryUnreadFlag);
				DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
				aSearchQuery.AppendNum(KMsvEntryUnreadFlag);
				
				++iProcessedIndexQueries;
				
				break;
				}
				
			//Search on New Message Flag			
			case EMsvNew:
				{
				aSearchQuery.Append(KAnd);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				UpdateQueryWithDriveNumAndIndexEntryTable(aSearchQuery, KCurrentDriveId);
#endif				
				aSearchQuery.Append(KBitwiseData);
				aSearchQuery.AppendNum(KMsvEntryNewFlag);
				DoAddRelationalOperatorL(aSearchQuery,aQuery.iMsgQuery[index].iRelationOp);
				aSearchQuery.AppendNum(KMsvEntryNewFlag);
				
				++iProcessedIndexQueries;
				
				break;
				}
				
			default:
				{
				User::Leave(KErrNotSupported);	
				}
			}
		}
	}


/** 
  DoCreateSortQueryL()
Depending on the CMsvSearchSortCacheEntry passed to this function it constructs the SQLDB sort query.

@param CMsvSearchSortCacheEntry& aQuery: Search/Sort query which has to be processed
@param RBuf8& aSortQuery: The QueryString which will be prepared depending on CMsvSearchSortCacheEntry.
@return none.
@leave: KErrNone if successful, KErrNotSupported if the Search/Sort message passed is not supported.
*/
void TSearchSortDbWrapper::DoCreateSortQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aSortQuery )
	{
		
	switch (aQuery.iMsgExplicitSortPart)
		{
			
		//Index Entry Parts to be searched
		//Sort on Description
		case EMsvDescription:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif
			aSortQuery.Append(KDescription);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}

		//Sort on Details
		case EMsvDetails:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif
			aSortQuery.Append(KDetails);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
		
		//Sort on Date
		case EMsvDate:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			aSortQuery.Append(KDate);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
		
		//Sort on Size
		case EMsvSize:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			aSortQuery.Append(KSize);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
		
		//Sort on MTM type
		case EMsvMtmTypeUID:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			
			aSortQuery.Append(KMtmId);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
			
		//Sort on Priority Flag
		case EMsvPriority:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			
			aSortQuery.Append(KPrioritySort);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
		
		case EMsvAttachment:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			
			aSortQuery.Append(KAttachmentSort);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
			
		case EMsvNew:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			
			aSortQuery.Append(KNewMsgSort);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
			
		case EMsvUnreadMessages:
			{
			aSortQuery.Append(KOrderBy);
		#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
		#endif			
			
			aSortQuery.Append(KUnReadSort);
			if(aQuery.IsAscendingSort())
				aSortQuery.Append(KAscending);
			else
				aSortQuery.Append(KDescending);
			break;
			}
			
		case 0:
			break;
		
		default:
			{
			User::Leave(KErrNotSupported);	
			}
		
		}
	}
	
/** 
  DoCreateInsertSortQueryL()
Depending on the CMsvSearchSortCacheEntry passed to this function it constructs the SQLDB sort query.

@param CMsvSearchSortCacheEntry& aQuery: Search/Sort query which has to be processed
@param RBuf8& aSortQuery: The QueryString which will be prepared depending on CMsvSearchSortCacheEntry.
@param TMsvId& aParentId: ParentId on which the the search/sort is performed on
@return none.
@leave: KErrNone if successful, KErrNotSupported if the Search/Sort message passed is not supported.
*/	

void TSearchSortDbWrapper::DoCreateInsertSortQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aSortQuery, TMsvId aParentId)
	{
	aSortQuery.Append(KIndexSortInsert);
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	if(iMtmIdArray.Count() > 1)
		{
		// handled in UpdateQueryWithMultipleTablesL()
		return;	
		}
	else
#endif		
		{
		aSortQuery.Append(KSelect);
		aSortQuery.AppendNum(aQuery.iQueryID);
		aSortQuery.Append(KComma);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
#endif		
		aSortQuery.Append(KId);
		aSortQuery.Append(KComma);
		
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
		if(aQuery.iMsgExplicitSortPart != 0)
			{
			UpdateQueryWithDriveNumAndIndexEntryTable(aSortQuery, KCurrentDriveId);
			}
#endif						
		UpdateQueryWithTMsvEntrySortFileldL(aQuery, aSortQuery);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		TInt index = 0;
		if( iTypeOfQuery & EQueryOnIndexEntry )
			{
			UpadeteQueryWithIndexEntryTable(aSortQuery, KCurrentDriveId);
			
			if(iTypeOfQuery & EQueryOnHeader)
				{
				aSortQuery.Append(KComma);
				UpdateQueryWithHeaderTable(aSortQuery, KCurrentDriveId, iMtmIdArray[index].iUid);	
				}

			if(aQuery.IsSortQuery())
				{
				aSortQuery.Append(KWhere);
				UpadeteQueryWithParentIdAndMsgType(aSortQuery, KCurrentDriveId, aParentId);	
				}
			}
		else if( (iTypeOfQuery & EQueryOnBody) && (iStatusOfQuery == KPartialResult) )
			{
			UpadeteQueryWithIndexEntryTable(aSortQuery, KCurrentDriveId);
			aSortQuery.Append(KWhere);
			UpadeteQueryWithParentIdAndMsgType(aSortQuery, KCurrentDriveId, aParentId);
			}
		else //iTypeOfQuery & EQueryOnHeader
			{
			UpadeteQueryWithIndexEntryTable(aSortQuery, KCurrentDriveId);
			aSortQuery.Append(KComma);
			UpdateQueryWithHeaderTable(aSortQuery, KCurrentDriveId, iMtmIdArray[index].iUid);	
			}
#else	
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		UpadeteQueryWithIndexEntryTable(aSortQuery, KCurrentDriveId);
		aSortQuery.Append(KWhere);
		UpadeteQueryWithParentIdAndMsgType(aSortQuery, KCurrentDriveId, aParentId);
	#else		
		UpadeteQueryWithParentIdAndMsgType(aSortQuery, aParentId);
	#endif
#endif	
		}
	
	}
	

void TSearchSortDbWrapper::UpdateQueryWithTMsvEntrySortFileldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aSortQuery)
	{
	switch (aQuery.iMsgExplicitSortPart)
		{
		//Index Entry Parts to be searched
		//No Sort
		case 0:
			{
			aSortQuery.Append(KNull);
			break;
			}
		case EMsvDescription:
			{
			aSortQuery.Append(KDescription);
			break;
			}

		case EMsvDetails:
			{
			aSortQuery.Append(KDetails);
			break;
			}

		case EMsvDate:  
			{
			aSortQuery.Append(KDate);
			break;
			}

		case EMsvSize:
			{
			aSortQuery.Append(KSize);
			break;
			}

		case EMsvMtmTypeUID:
			{
			aSortQuery.Append(KMtmId);
			break;
			}
		
		case EMsvPriority:
			{
			aSortQuery.Append(KPrioritySort);
			break;
			}
			
		case EMsvAttachment:
			{
			aSortQuery.Append(KAttachmentSort);
			break;
			}
		
		case EMsvNew:
			{
			aSortQuery.Append(KNewMsgSort);
			break;
			}
			
		case EMsvUnreadMessages:
			{
			aSortQuery.Append(KUnReadSort);
			break;
			}

		default:
			{
			User::Leave(KErrNotSupported);	
			}
		}
	}
	
/** 
  AddIdtoDBL()
Adds the TMsvId's & sortfield specified in the RArray into the SearchSortEntry table under the query id specified

@param TInt aQueryId: Query Id under which the entries should be added into the table
@param RArray<TMsvIdWithSortField>& aEntrySelection: TMsvId and sortfield which have to be added into the SearchSortEntry table
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
void TSearchSortDbWrapper::AddIdtoDBL(const TInt aQueryId,const RArray<TMsvIdWithSortField>& aEntrySelection)
	{
	for(TInt index = 0; index < aEntrySelection.Count(); ++index)
		{
		iDBadapter->CreateQueryEntryL(aQueryId, aEntrySelection[index]);
		}
	}


/** 
  GetSortedTMsvIdsfromTableL()
Retrieves all the entries from the SearchSortEntry table under the queryid specified

@param TInt aQueryId: Query Id under which the entries should be fetched from the table
@param RArray<TMsvId>& aIdSelection: Resultant TMsvIds, retrieved from the SearchSortEntry table
@param TBool aOrder: Order of the Sort operation, default value is EFalse which performs sort in Ascending order
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
void TSearchSortDbWrapper::GetSortedTMsvIdsfromTableL(const TInt aQueryId, RArray<TMsvId>& aIdResult, TBool aOrder/*Default = EFalse*/,TMsvMessagePart aMessagePart /*Default = EMsvDate*/ )
	{
	iDBadapter->GetSearchSortEntriesL(aQueryId, aIdResult, aOrder, IsSortAsNumRequired(aMessagePart));	
	}

/** 
  DeleteTMsvIdFromQueryL()
Deletes the TMsvId's under the QueryId specified from the SearchSortEntry table

@param TInt aQueryId: Query Id under which the entries should be deleted from the table
@param RArray<TMsvId>& aIdSelection: The TMsvId to be deleted 
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
void TSearchSortDbWrapper::DeleteTMsvIdFromQueryL(const TInt aQueryId,const RArray<TMsvId>& aIdSelection)
	{
	TMsvId tid;
	for(TInt index = 0; index < aIdSelection.Count(); ++index)
		{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		tid = UnmaskTMsvId(aIdSelection[index]);
#else
		tid = aIdSelection[index];
#endif
		iDBadapter->DeleteTMsvIdFromQueryL(aQueryId, tid);
		//iDBadapter->DeleteTMsvIdFromQueryL(aQueryId, aIdSelection[index]);
		}
	}

/** 
  DeleteQueryFromTableL()
Deletes all the TMsvIds under the Query Id specified from the SearchSortEntry table

@param TInt aQueryId: Query Id under which the entries should be deleted from the table
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
void TSearchSortDbWrapper::DeleteQueryFromTableL(const TInt aQueryId)
	{
	iDBadapter->DeleteQueryFromTableL(aQueryId);
	}


/** 
  DeleteQueryFromTableL()
Deletes the TMsvId's under the all QueryId from the SearchSortEntry table

@param RArray<TMsvId>& aIdSelection: The TMsvId to be deleted 
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
void TSearchSortDbWrapper::DeleteTMsvIdFromAllQueriesL(const RArray<TMsvId>& aIdSelection)
	{
	TMsvId tid;
	for(TInt index = 0; index < aIdSelection.Count(); ++index)
		{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		tid = UnmaskTMsvId(aIdSelection[index]);
#else
		tid = aIdSelection[index];
#endif
		iDBadapter->DeleteTMsvIdFromAllQueriesL(tid);
		//iDBadapter->DeleteTMsvIdFromAllQueriesL(aIdSelection[index]);
		}
	}
	

/** 
  ReturnResultCountInTableL()
Returns the number of TMsvIds stored under the QueryId specified

@param TInt aQueryId: Query Id under which the entry count should be returned
@return none.
@leave: KErrNone if successful or any of system wide leave codes or SQL leave codes if not.
*/
TInt TSearchSortDbWrapper::ReturnResultCountInTableL(const TInt aQueryId)
	{
	return iDBadapter->ReturnIdCountInDBL(aQueryId);
	}
	
/** 
  DoAppendIdConstraint()
Appends the TMsvIds passed within the IN() SQL keyword to limit the search/sort capabilty only to these TMsvids.

@param RArray<TMsvId>& aIdFilter:  RArray holding the entries to be used inside the IN() keyword
@param RBuf8& aSortQuery: This contains the SQL query having the IN() statement with the TMsvIds specified
@param TInt& aIdCount: This contains the index in from which the TMsvIds should be appended from the array,
				This also holds the number of entries appended after the operation is performed.
		
@return none.
*/ 
void TSearchSortDbWrapper::DoAppendIdConstraint(const RArray<TMsvId>& aIdFilter, RBuf8& aSortQuery, TInt& aIdCount)
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	_LIT8(KIndexEntryIdIn, ".IndexEntry.id IN(");
	_LIT8(KDb, 		"DB");
#endif
	
	if(aIdFilter.Count() == 0)
		{
		aSortQuery.Append(KDelimiter);
		}
	else
		{
		
		TInt index=0;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		aSortQuery.Append(KDb);
		aSortQuery.AppendNum(KCurrentDriveId);
		aSortQuery.Append(KIndexEntryIdIn);
#else		
		aSortQuery.Append(KIdIn);
#endif		
		
		for(index=aIdCount; (index < aIdCount+KMaxInCount) && (index < aIdFilter.Count()) ; ++index)
			{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aSortQuery.AppendNum(UnmaskTMsvId(aIdFilter[index]));
#else			
			aSortQuery.AppendNum(aIdFilter[index]);
#endif			
			aSortQuery.Append(KComma);
			}
		//Delete the Last KComma
		aSortQuery.Delete(aSortQuery.Size()-2,KMaxQuerySize);
		aSortQuery.Append(KCloseBrace);
		aSortQuery.Append(KDelimiter);
		aIdCount = index;
		}
	}


/** 
  IsSortAsNumRequired()
Returns whether the Specified message part requires sorting as TEXT or INT

@param TMessagePart& aMessagePart: Sort Message part
@return TBool returns true if the TMessagePart is INT.
*/ 
TBool TSearchSortDbWrapper::IsSortAsNumRequired(const TMsvMessagePart& aMessagePart)
	{
	
	switch(aMessagePart)
		{
		case EMsvDate:  
		case EMsvSize:
		case EMsvMtmTypeUID:
		case EMsvPriority:
			{
			return ETrue;	
			}

		default:
			{
			return EFalse;	
			}
		}
		
	}
	
/** 
  GetIdsInIterator()
This function should be used to retrieve TMsvIds in an iterative way.
The function maintains a counter which hold the last result count sent to the client.
The fuction executes the query from the start and returns the next result every time

@param TUint32&: The Id of the Search/Sort Query
@param TInt    : The previous count in the result set that was accessed
@param TMsvId& : The result id		
@return void   
*/ 

void TSearchSortDbWrapper::GetIdsInIteratorQueryIdL(const TInt& aQueryId, const TBool aOrder/*Default = ETrue*/, TMsvMessagePart aMessagePart /*Default = EMsvDate*/, const TInt aLimit/*Default = 1*/)
	{
	//Get the total count of results
	iTotalCount=ReturnResultCountInTableL(aQueryId);
	iDBadapter->ReturnIdsInIteratorL(aQueryId, aOrder, IsSortAsNumRequired(aMessagePart), aLimit);
	iLocation = 0;
	}


/** 
  GetIdsInIteratorNewQueryL()
@param aQuery  : Search/Sort query which has to be processed
@return NONE   

Initialises iterator for a new query. It executes the query and stores the result in the DB.
Gets  total count of the result and initialises previous count to 0
*/ 

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB) 		
void TSearchSortDbWrapper::GetIdsInIteratorNewQueryL(const CMsvSearchSortCacheEntry& aQuery, TBool aSortOnHeader, TInt aTypeOfQuery, TInt& aReturnResultType,  const TInt aLimit)
#else 		

void TSearchSortDbWrapper::GetIdsInIteratorNewQueryL(const CMsvSearchSortCacheEntry& aQuery, const TInt aLimit/*Default = 1*/)
#endif
	{
	// Create a new query, Execute that query, Store results
	RArray<TMsvId> iIteratorArray;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	FindL(aQuery, iIteratorArray, aSortOnHeader, aTypeOfQuery, aReturnResultType);
#else	
	FindL(aQuery,iIteratorArray,EFalse);
#endif	
	
	// Set total count
	iTotalCount=ReturnResultCountInTableL(aQuery.iQueryID);
	iLocation = 0;
	
	//Init the iterator
	iDBadapter->ReturnIdsInIteratorL(aQuery.iQueryID, aQuery.IsAscendingSort(), IsSortAsNumRequired(aQuery.iMsgExplicitSortPart), aLimit);
	
	//Destroy the Resultant Array
	iIteratorArray.Close();
	}

/** 
* GetNextIdL()
@param aQueryId  : ID of the Search/Sort query which has to be processed
@param aId       : The next result
@return   KErrEof if the end of the result is reached

Gets the next ID for a QueryId from the DB
*/ 
void TSearchSortDbWrapper::GetNextIdL(TMsvId& aId, TInt& aRemainingCount)
	{
	User::LeaveIfError(iTotalCount);
	
	if(iTotalCount == 0)
		{
		aRemainingCount	= 0;
		aId = 0;
		User::LeaveIfError(iTotalCount = -1);
		}
		
	else if(iTotalCount == iLocation)
		{
		aRemainingCount	= 0;
		aId = 0;
		User::LeaveIfError(iTotalCount = -1);
		}
		
	else
		{
		iLocation = iDBadapter->GetNextL(aId);
		aRemainingCount = iTotalCount - iLocation;
		}
	}


/** 
  GetDateRelationalOperator()
This function is used to add date field to the query being prepared
@param CMsvSearchSortCacheEntry& aQuery: Search/Sort query which has to be processed
@param TInt aCurrentIndex: The current index of query table

@return void
*/ 
TMsvRelationOp TSearchSortDbWrapper::GetDateRelationalOperator(const CMsvSearchSortCacheEntry& aQuery,TInt aCurrentIndex)
	{
	return(aQuery.iMsgQuery[aCurrentIndex].iRelationOp);
	}


/** 
* IsCaseSensitiveSearch()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* @param aIndex: index of query table 
* return NONE
*  
*  Function to update query with case sensitive or case insensitive serach
*/ 	
void TSearchSortDbWrapper::IsCaseSensitiveSearch(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TInt aIndex)
	{
	_LIT8(KGlob, " GLOB ");
	_LIT8(KStar, "*");

	if(aQuery.IsCaseSensetive())
		{
		aQueryString.Append(KGlob);
				
		aQueryString.Append(KQuote);
		aQueryString.Append(KStar);
		aQueryString.Append(aQuery.iMsgQuery[aIndex].iQueryString);
		aQueryString.Append(KStar);
		aQueryString.Append(KQuote);
		}
	else
		{
		aQueryString.Append(KLike);
			
		aQueryString.Append(KQuote);
		aQueryString.Append(KPercent);
		aQueryString.Append(aQuery.iMsgQuery[aIndex].iQueryString);
		aQueryString.Append(KPercent);
		aQueryString.Append(KQuote);	
		}
	}	


/** 
* UpdateQueryWithTMsvEntrySearchFieldsL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* @param aDriveId: update query with Drive number 
* return NONE
*  
*  Update query with TMsvEntry Search field
*/ 	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
void TSearchSortDbWrapper::UpdateQueryWithTMsvEntrySearchFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId)
#else
void TSearchSortDbWrapper::UpdateQueryWithTMsvEntrySearchFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TMsvId aParentId)
#endif
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iMtmIdArray.Count() > 1)
		{
		if(iTypeOfQuery != EQueryOnHeaderAndBody && iTypeOfQuery != EQueryOnEntryHeaderAndBody)
			{
			UpdateQueryWithMultipleTablesL(aQuery, aQueryString, aDriveId, aParentId);
			}
		}
	else
		{
		if(!iTotalNumMsgPartsOnHeader)
			{
			aQueryString.Append(KWhere);
			UpadeteQueryWithParentIdAndMsgType(aQueryString, aDriveId, aParentId);
			}
#endif
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		aDriveId = aDriveId;    // to remove compilation warning, need to delete this line while removing macro SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB
	#endif	
		aParentId = aParentId;  // to remove compilation warning, need to delete this line while removing macro SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB
		

		DoCreateSearchQueryL(aQuery, aQueryString);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		}
#endif
	}


/** 
* UpadeteQueryWithParentIdAndMsgType()
*
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* @param aDriveId: update query with Drive number 
* return NONE
*  
*  Update query with parentId and type as KUidMsvMessageEntryValue
*/ 	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
void TSearchSortDbWrapper::UpadeteQueryWithParentIdAndMsgType(RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId)
#else
void TSearchSortDbWrapper::UpadeteQueryWithParentIdAndMsgType(RBuf8& aQueryString, TMsvId aParentId)
#endif
	{
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KParentId, 	"ParentId");
	_LIT8(KDb, 		"DB");
	_LIT8(KDot, 		".");
	#endif
	
	if(aParentId != KMsvRootIndexEntryIdValue)
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			// DB(DriveId).IndexEntry.ParentId = ParentId AND DB(DriveId).IndexEntry.type = KUidMsvMessageEntryValue
			aQueryString.Append(KDb);
			aQueryString.AppendNum(aDriveId);
			aQueryString.Append(KDot);
			aQueryString.Append(KIndexEntry);
			aQueryString.Append(KDot);
			aQueryString.Append(KParentId);
			aQueryString.Append(KEqualTo);
			aQueryString.AppendNum(aParentId);
			
			aQueryString.Append(KAnd);
			
			UpdateQueryWithDriveNumAndIndexEntryTable(aQueryString, aDriveId);
			aQueryString.Append(KType);
			aQueryString.Append(KEqualTo);
			aQueryString.AppendNum(KUidMsvMessageEntryValue);
		#else // SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT
			aQueryString.Append(KIndexSortSelect);
			aQueryString.AppendNum(aParentId);
			
			aQueryString.Append(KAnd);
		
			aQueryString.Append(KType);
			aQueryString.Append(KEqualTo);
			aQueryString.AppendNum(KUidMsvMessageEntryValue);
		#endif
		}
	else
		{
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aQueryString.Append(KType);
			aQueryString.Append(KEqualTo);
			aQueryString.AppendNum(KUidMsvMessageEntryValue);
		#else // SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT
			aQueryString.Append(KIndexSortSelectNoParent);
			aQueryString.Append(KWhere);
			aQueryString.Append(KType);
			aQueryString.Append(KEqualTo);
			aQueryString.AppendNum(KUidMsvMessageEntryValue);
		#endif
		}
	}

/** 
* UpdateQueryWithHeaderTable()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with Header table
*/ 
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
void TSearchSortDbWrapper::UpdateQueryWithDriveNumAndIndexEntryTable(RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KDb, 		"DB");
	_LIT8(KDot, 		".");
	
	// DB(DriveId).IndexEntry.
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KIndexEntry);	
	aQueryString.Append(KDot);

	}
	
/** 
* UpadeteQueryWithIndexEntryTable()
*
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* @param aDriveId: update query with Drive number 
* return NONE
*  
*  Update the Query with IndexEntrytable 
*/
void TSearchSortDbWrapper::UpadeteQueryWithIndexEntryTable(RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KDot, 		".");
	
	// FROM DB(aDriveId).IndexEntry
	aQueryString.Append(KFromDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KIndexEntry);
	}
#endif // SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	

/**
 * ParseQueryForHeaderFields()
 *
 * @param aQuery: SearchSort query sent by client
 * @return NONE
 * 
 * Function to parse the SearchSort query header filelds 
 * to check wheather it exist it in DB
 */
void TSearchSortDbWrapper::ParseQueryForHeaderFields(const CMsvSearchSortCacheEntry& aQuery)
	{
	iHeaderTableStdFields = 0;
	iTotalNumMsgPartsOnHeader = 0;
	
	for(TInt count = 0; count < aQuery.iMsgQuery.Count(); count++)
		{
		ParseCommonHederFilelds(aQuery.iMsgQuery[count].iMessagePart, ETrue);
		}
		
	ParseCommonHederFilelds(aQuery.iMsgExplicitSortPart, EFalse);
	}


/**
 * ParseCommonHederFilelds()
 *
 * @param aMessagePart: Message part that availble SearchSort query
 * @return NONE
 * 
 * Function to parse the SearchSort query header filelds 
 * to check wheather it exist it in DB
 */
void TSearchSortDbWrapper::ParseCommonHederFilelds(TMsvMessagePart aMessagePart, TBool aIsSearhField)
	{
	switch(aMessagePart)
		{
		case EMsvFrom:
			iHeaderTableStdFields |= CMsvHeaderStore::EFrom;
			if(aIsSearhField)
				{
				iTotalNumMsgPartsOnHeader++;
				}
			break;
		
		case EMsvTo:
			iHeaderTableStdFields |= CMsvHeaderStore::ETo;	
			if(aIsSearhField)
				{
				iTotalNumMsgPartsOnHeader++;
				}
			break;
			
		case EMsvCc:  
			iHeaderTableStdFields |= CMsvHeaderStore::ECC;	
			if(aIsSearhField)
				{
				iTotalNumMsgPartsOnHeader++;
				}
			break;
		
		case EMsvBcc:
			iHeaderTableStdFields |= CMsvHeaderStore::EBCC;	
			if(aIsSearhField)
				{
				iTotalNumMsgPartsOnHeader++;
				}
			break;
		
		case EMsvSubject:
			iHeaderTableStdFields |= CMsvHeaderStore::ESubject;	
			if(aIsSearhField)
				{
				iTotalNumMsgPartsOnHeader++;
				}
			break;
		
		default:
			break;
		}
	}

/**
 * ProcessPreQueryRequestL()
 *
 * @param aQuery: SearchSort query sent by client
 * @return Status of the header table
 * 
 * Function to check wheather header table and correspnding standard column are availble at DB.  
 * 
 */
void TSearchSortDbWrapper::ProcessPreQueryRequestL(TMsvId aParentId, TInt& aStatusOfQuery) 	
	{
	TBool isOnEntry = EFalse;
	iHeaderTableAtDB = EInvalid;
	iBodyTableAtDB = EInvalid;
	
	switch(iTypeOfQuery)
		{
		case EQueryOnIndexEntry:
			isOnEntry = ETrue;
			break;
		
		case EQueryOnHeader:
		case EQueryOnEntryAndHeader:
			iHeaderTableAtDB = StatusOfHeaderTableL(aParentId);
			break;

		case EQueryOnBody:
		case EQueryOnEntryAndBody:
			iBodyTableAtDB = ENotAvailable;
			break;

		case EQueryOnHeaderAndBody:
		case EQueryOnEntryHeaderAndBody:
			iHeaderTableAtDB = StatusOfHeaderTableL(aParentId);
			iBodyTableAtDB = ENotAvailable;
			break;
		}

	// if sort on header
	if(iSortOnHeader)
		{
		if(iHeaderTableAtDB == EInvalid)
			{
			iHeaderTableAtDB = StatusOfHeaderTableL(aParentId);
			}
		}
		
	// Query on TMsvEntry related fields
	if(isOnEntry && iHeaderTableAtDB == EInvalid && iBodyTableAtDB == EInvalid)
		{
		iStatusOfQuery = KFinalResult;
		}
	// Query on Header related fields	
	else if(iHeaderTableAtDB == EAvailable && iBodyTableAtDB == EInvalid )
		{
		iStatusOfQuery = KFinalResult;
		}
	// Query on Header and Body			
	else if(iHeaderTableAtDB == EAvailable && iBodyTableAtDB == ENotAvailable )
		{
		iStatusOfQuery = KPartialResult;
		}
	// Query on Header related fields (some of the Header the availble at DB and some of availble at file system )		
	else if(iHeaderTableAtDB == EPartiallyAvailable)
		{
		iStatusOfQuery = KPartialResult;
		}
	else if(iBodyTableAtDB == ENotAvailable && iTypeOfQuery == EQueryOnEntryAndBody)
	    {
	    iStatusOfQuery = KPartialResult;
	    }

	// Header table not availbe in DB
	else if(iHeaderTableAtDB == ENotAvailable || iBodyTableAtDB == ENotAvailable)
		{
		// if search only on body, get all TMsvId from IndexEntry tbale and search at client side
		if(IsSearchQueryOnBody(ETrue))
			{
			iStatusOfQuery = KPartialResult;
			}
		else
			{
			iStatusOfQuery = KNewQuery;	
			}
		}
	else
		{
		User::Leave(KErrArgument);
		}
		
	aStatusOfQuery = iStatusOfQuery;
	}


/**
 * StatusOfHeaderTableL()
 *
 * @param aQuery: SearchSort query sent by client
 * @return Status of the header table
 * 
 * Function to check wheather header table and correspnding standard column are availble at DB.  
 * 
 */
TSearchSortDbWrapper::THeaderBodyTableStatusAtDb TSearchSortDbWrapper::StatusOfHeaderTableL(TMsvId aParentId)
	{
 	
 	// to get the MtmId 
 	QueryToGetMtmIdsL(aParentId, KCurrentDriveId);
 	
 	THeaderBodyTableStatusAtDb tableStatus = EInvalid;
 	
 	for(TInt index=0; index < iMtmIdArray.Count(); ++index)
		{
		if(iServer.MessageDBAdapter().CheckHeaderTableAndStdColumnFields(iMtmIdArray[index], iHeaderTableStdFields))
			{
			if(tableStatus == ENotAvailable || tableStatus == EPartiallyAvailable)
				{
				tableStatus = EPartiallyAvailable;
				}
			else
				{
				tableStatus = EAvailable;	
				}
			}
		else
			{
			iMtmIdArray.Remove(index--);
					
			if(tableStatus == EAvailable || tableStatus == EPartiallyAvailable)
				{
				tableStatus = EPartiallyAvailable;
				}
			else
				{
				tableStatus = ENotAvailable;
				}
			}
		}
	return tableStatus;
	}
 	

 
/** 
* QueryToGetMtmIdsL()
*
* @param aQuery: SearchSort query information sent by client
* @param aDriveId:  drive number
* @leave: KErrNone if successful or SQL leave codes if not.
*  
*  To get MtmId/s for corresponding query
*/ 
void TSearchSortDbWrapper::QueryToGetMtmIdsL(TMsvId aParentId, TUint aDriveId)
	{
	_LIT8(KIdQueryForMtmId, "SELECT DISTINCT mtmId FROM DB");
	_LIT8(KIndexEntryWhereParentId, ".IndexEntry WHERE parentId = ");
	
	RBuf8 queryString;
	queryString.CleanupClosePushL();
	
	queryString.CreateL(KMaxQuerySize);
	
	iMtmIdArray.Reset();
	
	//Query string is: SELECT DISTINCT mtmid FROM DB driveid.IndexEntry WHERE parentid=aQuery.parentId;
	queryString.Append(KIdQueryForMtmId);
	queryString.AppendNum(aDriveId);
	queryString.Append(KIndexEntryWhereParentId);
	queryString.AppendNum(aParentId);
	queryString.Append(KDelimiter);

	iDBadapter->GetMtmIdsL(queryString, iMtmIdArray);
	
	CleanupStack::PopAndDestroy();  //queryString
	}
	

/** 
* DoCreateInsertSortOnHeaderQueryL()
*
* @param aQuery: SearchSort query information sent by client
* @param aQueryString:  Querystring to execute on DB
* return NONE
*  
*  Update query with sort information
*/ 
void TSearchSortDbWrapper::DoCreateInsertSortOnHeaderQueryL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString)
	{
	_LIT8(KDot, 		".");

	// INSERT OR IGNORE INTO SearchSortEntry (Qid, id, sortfield) SELECT iQueryID, DB1.IndexEntry.id, DB1.Header_268439594."To" FROM DB1.IndexEntry, DB1.Header_268439594 
	
	UpdateQueryWithSearchSortEntryTable(aQueryString);
	
	// if searching on more than one header table
	if(iMtmIdArray.Count() > 1)
		{
		// handled in UpdateQueryWithMultipleTablesL()
		return;	
		}
	else  // search query on single table
		{
		UpdateQueryToGetQueryIdTMsvId(aQuery, aQueryString, KCurrentDriveId);
		
		TInt index = 0;
		if(iMtmIdArray.Count() == 1 )
			{
			// ,DB(DriveId).Header_268439594."To"
			aQueryString.Append(KComma);
			iHeaderTableForSort = iMtmIdArray[index].iUid;
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[index].iUid);
			aQueryString.Append(KDot);
			}
		// update sort filed	
		UpdateHeaderFieldL(aQuery, aQueryString);	
		DoUpdateQuery(aQueryString);	
		}
	}


/** 
* UpdateQueryWithSearchSortEntryTable()
*
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update Query With SearchSortEntry Table
*/ 
void TSearchSortDbWrapper::UpdateQueryWithSearchSortEntryTable(RBuf8& aQueryString)
	{
	// INSERT OR IGNORE INTO SearchSortEntry (Qid, id, sortfield)
	aQueryString.Append(KIndexSortInsert);
	}



/** 
* UpdateQueryToGetQueryIdTMsvId()
*
* @param aQuery: Contains SearchSort query information sent by client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update Query to get TMsvIds
*/ 
void TSearchSortDbWrapper::UpdateQueryToGetQueryIdTMsvId(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KDb, 		"DB");
	_LIT8(KDot, 		".");

	// SELECT iQueryID, IndexEntry.id,
	aQueryString.Append(KSelect);
	aQueryString.AppendNum(aQuery.iQueryID);
	
	// ,DB1.IndexEntry.id
	aQueryString.Append(KComma);
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KIndexEntry);
	aQueryString.Append(KDot);
	aQueryString.Append(KId);
	}	


/** 
* UpdateQueryWithMultipleTablesL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
* Function to update the query for more than one Header table and/or multilevel query
*
* Example Query:
*	
* INSERT OR IGNORE INTO SearchSortEntry (Qid, id, sortfield) 
* SELECT 1001, DB1.IndexEntry.id, DB1.Header_268439592."To" FROM DB1.IndexEntry, DB1.Header_268439592 WHERE  ( DB1.IndexEntry.id = DB1.Header_268439592.id AND  
* ( DB1.Header_268439592."To" LIKE "test003@10.253.4.75" OR DB1.Header_268439592."Cc" LIKE "test005@10.253.4.75" OR DB1.Header_268439592."From" LIKE "test007@10.253.4.75" )  )  
* AND DB1.Header_268439592."To" IS NOT NULL  AND DB1.IndexEntry.ParentId = 4101 AND DB1.IndexEntry.type = 268439402 
*
* UNION 
*
* SELECT 1001, DB1.IndexEntry.id, DB1.Header_268439593."To" FROM DB1.IndexEntry, DB1.Header_268439593 WHERE  ( DB1.IndexEntry.id = DB1.Header_268439593.id AND  
* ( DB1.Header_268439593."To" LIKE "test003@10.253.4.75" OR DB1.Header_268439593."Cc" LIKE "test005@10.253.4.75" OR DB1.Header_268439593."From" LIKE "test007@10.253.4.75" )  )  
* AND DB1.Header_268439593."To" IS NOT NULL  AND DB1.IndexEntry.ParentId = 4101 AND DB1.IndexEntry.type = 268439402 
*
* ORDER by "To" ASC;
*/ 	
void TSearchSortDbWrapper::UpdateQueryWithMultipleTablesL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId, TMsvId aParentId)
	{
	_LIT8(KUnion, " UNION ");
	_LIT8(KIsNotNull, " IS NOT NULL ");
	_LIT8(KOpenBracket, " ( ");
	_LIT8(KCloseBracket, " ) ");
	_LIT8(KDot, ".");
	
	// for multiple tables
	for(TInt index = 0; index < iMtmIdArray.Count(); ++index)
		{
		// reset for other header table
		iNumOfMsgParts = 0;
		
		// SELECT 1001, IndexEntry.id, 
		UpdateQueryToGetQueryIdTMsvId(aQuery, aQueryString, aDriveId);
		aQueryString.Append(KComma);
		if(iSortOnHeader)
			{
			// Header_268439594."To"
			UpdateQueryWithHeaderTable(aQueryString, aDriveId, iMtmIdArray[index].iUid);
			aQueryString.Append(KDot);
			UpdateHeaderFieldL(aQuery, aQueryString);
			}
		else
			{
			// IndexEntry.size
			if(aQuery.iMsgExplicitSortPart != 0)
				{
				UpdateQueryWithDriveNumAndIndexEntryTable(aQueryString, aDriveId);
				}
			UpdateQueryWithTMsvEntrySortFileldL(aQuery, aQueryString);
			}
	
		// Header_268439594."To"  FROM IndexEntry, Header_268439594			
		UpadeteQueryWithIndexEntryTable(aQueryString, aDriveId);
		aQueryString.Append(KComma);
		UpdateQueryWithHeaderTable(aQueryString, aDriveId, iMtmIdArray[index].iUid);
			
		// WHERE (IndexEntry.id = Header_268439594.id AND Header_268439594."To" LIKE "%test002@10.253.4.75%")
		aQueryString.Append(KWhere);
		aQueryString.Append(KOpenBracket);
		
		// compare TMsvId of IndexEntry table and Header table
		CompareTMsvIds(aQueryString, index);
			
		// if search on header fileld
		if( (iTypeOfQuery & EQueryOnHeader) && !aQuery.IsSortQuery())
			{
			aQueryString.Append(KAnd);
			
			// inner open bracket, if search needs more than one header field
			if(iTotalNumMsgPartsOnHeader > 1)
				{
				aQueryString.Append(KOpenBracket);	
				}
			
			// multilevel query on single header table.
			for(TInt queryCount=0; queryCount < aQuery.iMsgQuery.Count(); ++queryCount)
				{
				DoAddHeaderFields(aQuery, aQueryString, queryCount, index);
				}
				
			// inner close bracket, if search needs more than one header field
			if(iTotalNumMsgPartsOnHeader > 1)
				{
				aQueryString.Append(KCloseBracket);	
				}

			aQueryString.Append(KCloseBracket);
			}
		else
			{
			aQueryString.Append(KCloseBracket);	
			}
			
		// update Query with search field
		aQueryString.Append(KAnd);
		if(iSortOnHeader)
			{
			UpdateQueryWithHeaderTable(aQueryString, aDriveId, iMtmIdArray[index].iUid);
			aQueryString.Append(KDot);
			UpdateHeaderFieldL(aQuery, aQueryString);
			aQueryString.Append(KIsNotNull);
			aQueryString.Append(KAnd);
			}
					
		// Update Query with ParentId and MsgType
		// IndexEntry.ParentId = 4098 AND IndexEntry.type = 268439402
		UpadeteQueryWithParentIdAndMsgType(aQueryString, aDriveId, aParentId);
				
		if(iTypeOfQuery & EQueryOnIndexEntry)
			{
			DoCreateSearchQueryL(aQuery, aQueryString);
			}
				
		// add UNION
		if((index+1 ) != iMtmIdArray.Count())
			{
			aQueryString.Append(KUnion);	
			}
		} //end of for
	}


/** 
* UpdateHeaderFieldL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with Header filed
*/ 
void TSearchSortDbWrapper::UpdateHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString)
	{
	
	_LIT8(KTo, 	 "To");
	_LIT8(KFrom, 	 "From");
	_LIT8(KCc, 	 "Cc");
	_LIT8(KBcc, 	 "Bcc");
	_LIT8(KSubject, "Subject");
	
	// to add header field
	switch (aQuery.iMsgExplicitSortPart)
		{
		//Sort to be done on Header fields
		case EMsvTo:
			aQueryString.Append(KQuote);
			aQueryString.Append(KTo);
			aQueryString.Append(KQuote);
			break;
			
		case EMsvFrom:
			aQueryString.Append(KQuote);
			aQueryString.Append(KFrom);
			aQueryString.Append(KQuote);
			break;
			
		case EMsvCc:  
			aQueryString.Append(KQuote);
			aQueryString.Append(KCc);
			aQueryString.Append(KQuote);
			break;
			
		case EMsvBcc:
			aQueryString.Append(KQuote);
			aQueryString.Append(KBcc);
			aQueryString.Append(KQuote);
			break;
			
		case EMsvSubject:
			aQueryString.Append(KQuote);
			aQueryString.Append(KSubject);
			aQueryString.Append(KQuote);
			break;
			
		case EMsvBody:
		default:
			{
			User::Leave(KErrNotSupported);	
			}
		}
	}	

/** 
* DoUpdateQuery()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with IndexEntry and Header table
*/ 
void TSearchSortDbWrapper::DoUpdateQuery(RBuf8& aQueryString)
	{
	
	// FROM DB(aDriveId).IndexEntry, 
	UpadeteQueryWithIndexEntryTable(aQueryString, KCurrentDriveId);
	aQueryString.Append(KComma);
	
	TInt index = 0;
	if( iMtmIdArray.Count() == 1 )
		{
		if(((iTypeOfQuery & EQueryOnHeader) || iSortOnHeader) && (iHeaderTableAtDB == EAvailable || iHeaderTableAtDB == EPartiallyAvailable))
			{
			// ,DB(DriveId).Header_268439594
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[index].iUid);
			}
		}
	}


/** 
* UpdateQueryWithHeaderTable()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with Header table
*/ 
void TSearchSortDbWrapper::UpdateQueryWithHeaderTable(RBuf8& aQueryString, TUint aDriveId, TInt32 aMtmId)
	{
	_LIT8(KHeader, 	"Header_");
	_LIT8(KDb, 		"DB");
	_LIT8(KDot, 		".");
	
	// DB(DriveId).Header_268439594
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KHeader);
	aQueryString.AppendNum(aMtmId);
	}	


/** 
* DoUpdateSearchQueryForHeaderFieldsL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with header fields
*/ 
void TSearchSortDbWrapper::DoUpdateSearchQueryForHeaderFieldsL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TMsvId aParentId)
	{
	_LIT8(KOpenBracket, " ( ");
	_LIT8(KCloseBracket, " ) ");
	
	
	// if search requies on more than one header table
	if(iMtmIdArray.Count() > 1)
		{
		UpdateQueryWithMultipleTablesL(aQuery, aQueryString, KCurrentDriveId, aParentId);
		return;	
		}
	else
		{
		// multilevel serach with single header table
		TInt index = 0;
		iNumOfMsgParts = 0;
		aQueryString.Append(KWhere);
		
		if( iMtmIdArray.Count() == 1 )
			{
			aQueryString.Append(KOpenBracket);
			
			// compare TMsvId of IndexEntry table and Header table
			CompareTMsvIds(aQueryString, index);

			aQueryString.Append(KAnd);
			
			// inner open bracket, if search needs more than one header field
			if(iTotalNumMsgPartsOnHeader > 1)
				{
				aQueryString.Append(KOpenBracket);	
				}
			
			// multilevel query on single header table.
			for(TInt queryCount=0; queryCount < aQuery.iMsgQuery.Count(); ++queryCount)
				{
				DoAddHeaderFields(aQuery, aQueryString, queryCount, index);
				}
				
			// inner close bracket, if search needs more than one header field
			if(iTotalNumMsgPartsOnHeader > 1)
				{
				aQueryString.Append(KCloseBracket);	
				}
			
			aQueryString.Append(KCloseBracket);
			}
			
		// update Query with ParentId and MsgType
		aQueryString.Append(KAnd);
		UpadeteQueryWithParentIdAndMsgType(aQueryString, KCurrentDriveId, aParentId);
		}
	}


/** 
* UpdateQueryWithSortFieldL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with sort field
*/ 
void TSearchSortDbWrapper::UpdateQueryWithSortFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId)
	{
	if(iMtmIdArray.Count() > 1)
		{
		if(aQuery.IsSortQuery() && iSortOnHeader)
			{
			UpdateQueryWithMultipleTablesL(aQuery, aQueryString, aDriveId, UnmaskTMsvId(aQuery.iParentId));
			}
		DoUpdateSortQueryForHeaderFieldL(aQuery, aQueryString, aDriveId);
		}
	else
		{
		if(aQuery.IsSortQuery() || IsSearchQueryOnBody() )
			{
			aQueryString.Append(KWhere);
			UpadeteQueryWithParentIdAndMsgType(aQueryString, aDriveId, UnmaskTMsvId(aQuery.iParentId));	
			}
		UpdateQueryToRemoveEmptyHeaderFieldL(aQuery, aQueryString, aDriveId);
		DoUpdateSortQueryForHeaderFieldL(aQuery, aQueryString, aDriveId);
		}
	}	

/** 
* IsSearchQueryOnBody()
*
* @param aIsOnlyBody: to differentiate search only on body of the message
* return NONE
*  
*  To check whether its search only on body of the message
*/ 
TBool TSearchSortDbWrapper::IsSearchQueryOnBody(TBool aIsOnlyBody)
	{
	TBool flag = EFalse;
	// is search only on body of the message?
	if(aIsOnlyBody)	
		{
		if( (iTypeOfQuery == EQueryOnBody) && !iSortOnHeader ) 
			{
			flag = ETrue;
			}
		}
	else //is search only on body of the message and sort on header field?
		{
		if( (iTypeOfQuery == EQueryOnBody) && iSortOnHeader )
			{
			flag = ETrue;
			}
		}
	return flag;	
	}

/** 
* DoUpdateSortQueryForHeaderFieldL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Update query with sort field
*/ 
void TSearchSortDbWrapper::DoUpdateSortQueryForHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KTo, 	 "To");
	_LIT8(KFrom, 	 "From");
	_LIT8(KCc, 	 "Cc");
	_LIT8(KBcc, 	 "Bcc");
	_LIT8(KSubject, "Subject");
	
	switch (aQuery.iMsgExplicitSortPart)
		{
		//Sort on EMsvTo
		case EMsvTo:
			{
			aQueryString.Append(KOrderBy);
			
			if(iMtmIdArray.Count() == 1)
				{
				UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
				}
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KTo);
			aQueryString.Append(KQuote);
			
			if(aQuery.IsAscendingSort())
				{
				aQueryString.Append(KAscending);
				}
				
			else
				{
				aQueryString.Append(KDescending);
				}
			}
			break;

		//Sort on EMsvFrom
		case EMsvFrom:
			{
			aQueryString.Append(KOrderBy);
			
			if(iMtmIdArray.Count() == 1)
				{
				UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
				}
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KFrom);
			aQueryString.Append(KQuote);
			
			if(aQuery.IsAscendingSort())
				{
				aQueryString.Append(KAscending);
				}
				
			else
				{
				aQueryString.Append(KDescending);
				}
				
			}
			break;
		
		//Sort on EMsvCc
		case EMsvCc:
			{
			aQueryString.Append(KOrderBy);
			
			if(iMtmIdArray.Count() == 1)
				{
				UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
				}
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KCc);
			aQueryString.Append(KQuote);
			
			if(aQuery.IsAscendingSort())
				{
				aQueryString.Append(KAscending);
				}
				
			else
				{
				aQueryString.Append(KDescending);
				}
			}
			break;
		
		//Sort on EMsvBcc
		case EMsvBcc:
			{
			aQueryString.Append(KOrderBy);
			
			if(iMtmIdArray.Count() == 1)
				{
				UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
				}
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KBcc);
			aQueryString.Append(KQuote);
			
			if(aQuery.IsAscendingSort())
				{
				aQueryString.Append(KAscending);
				}
				
			else
				{
				aQueryString.Append(KDescending);
				}
			}
			break;
		
		//Sort on EMsvSubject
		case EMsvSubject:
			{
			aQueryString.Append(KOrderBy);
			
			if(iMtmIdArray.Count() == 1)
				{
				UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
				}

			aQueryString.Append(KQuote);
			aQueryString.Append(KSubject);
			aQueryString.Append(KQuote);
			
			if(aQuery.IsAscendingSort())
				{
				aQueryString.Append(KAscending);
				}
			else
				{
				aQueryString.Append(KDescending);
				}
			}
			break;
		
		// No Sort body filed	
		case EMsvBody:
		case 0:
			break;
			
		default:
			{
			User::Leave(KErrNotSupported);	
			}
		}
	}


/** 
* UpdateQueryToRemoveEmptyHeaderFieldL()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Function to remove the empty header fields from search result
*/ 	
void TSearchSortDbWrapper::UpdateQueryToRemoveEmptyHeaderFieldL(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KIsNotNull, " IS NOT NULL ");
	
	// if it's sort query on header or 
	// Search on TMsvEntry fields and/or body AND sort on header field
	if( (aQuery.IsSortQuery() && iSortOnHeader) || IsSearchOnEntryAndBody(aQuery) )
		{
		CompareTMsvIds(aQueryString, aDriveId);
		}
	
	aQueryString.Append(KAnd);
	UpdateQueryWithDriveNumAndHeaderTable(aDriveId, aQueryString);
	UpdateHeaderFieldL(aQuery, aQueryString);
	aQueryString.Append(KIsNotNull);
	}
	

/** 
* IsSearchOnEntryAndBody()
*
* @param aQuery: Contains SearchSort query information sent by the client
* return NONE
*  
*  To check whether its search (search on TMsvEntry fields and/or body) query with sort on header
*/ 
TBool TSearchSortDbWrapper::IsSearchOnEntryAndBody(const CMsvSearchSortCacheEntry& aQuery)
	{
	TBool flag = EFalse;
	
	// to check whether its search query with sort on header
	if(!aQuery.IsSortQuery() && iSortOnHeader)
		{
		// to check is search on TMsvEntry fields and/or body
		switch(iTypeOfQuery)
			{
			case EQueryOnIndexEntry:
			case EQueryOnBody:
			case EQueryOnEntryAndBody:
				flag = ETrue;
				break;
		
			default:
				flag = EFalse;
			}
		}
	return flag;
	}
	

/** 
* CompareTMsvIds()
*
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* @param DriveId: Drive information
* return NONE
*  
*  Function to compare TMsvIds from IndexEntry table and Header table
*/ 
void TSearchSortDbWrapper::CompareTMsvIds(RBuf8& aQueryString, TUint aDriveId)
	{
	_LIT8(KHeader, "Header_");
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KOpenBracket, " ( ");
	_LIT8(KCloseBracket, " ) ");
	_LIT8(KDb, "DB");
	_LIT8(KDot, ".");

	// AND  ( DB(KCurrentDriveId).IndexEntry.id = DB(KCurrentDriveId).Header_268439593.id )
	aQueryString.Append(KAnd);	
	aQueryString.Append(KOpenBracket);
				
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KIndexEntry);
	aQueryString.Append(KDot);
	aQueryString.Append(KId);
	aQueryString.Append(KEqualTo);
				
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KHeader);
	aQueryString.AppendNum(iHeaderTableForSort);

	aQueryString.Append(KDot);
	aQueryString.Append(KId);
	
	aQueryString.Append(KCloseBracket);
	}


/** 
* UpdateQueryWithDriveNumAndHeaderTable()
*
* @param DriveId: Drive information
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Function to update query with header_table
*/ 
void TSearchSortDbWrapper::UpdateQueryWithDriveNumAndHeaderTable(TUint aDriveId, RBuf8& aQueryString)
	{
	_LIT8(KHeader, 	"Header_");
	_LIT8(KDb, 		"DB");
	_LIT8(KDot, 		".");

	// DB(DriveId).Header_mtmid.
	aQueryString.Append(KDb);
	aQueryString.AppendNum(aDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KHeader);
	aQueryString.AppendNum(iHeaderTableForSort);
	aQueryString.Append(KDot);	
	}

/** 
* CompareTMsvIds()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Function to compare TMsvIds from IndexEntry table and Header table
*/ 
void TSearchSortDbWrapper::CompareTMsvIds(RBuf8& aQueryString, TInt aIndex)
	{
	_LIT8(KHeader, "Header_");
	_LIT8(KIndexEntry, "IndexEntry");
	_LIT8(KDb, "DB");
	_LIT8(KDot, ".");
	
				
	aQueryString.Append(KDb);
	aQueryString.AppendNum(KCurrentDriveId);
	aQueryString.Append(KDot);
	aQueryString.Append(KIndexEntry);
	aQueryString.Append(KDot);
	aQueryString.Append(KId);
	aQueryString.Append(KEqualTo);
				
	aQueryString.Append(KDb);
	aQueryString.AppendNum(KCurrentDriveId);
	aQueryString.Append(KDot);
	
	aQueryString.Append(KHeader);
	aQueryString.AppendNum(iMtmIdArray[aIndex].iUid);

	aQueryString.Append(KDot);
	aQueryString.Append(KId);
	}

/** 
* DoAddHeaderFields()
*
* @param aQuery: Contains SearchSort query information sent by the client
* @param aQueryString: QueryString which will be prepared depending on SearchSort Query
* return NONE
*  
*  Function to update query with search fields on header
*/ 	
void TSearchSortDbWrapper::DoAddHeaderFields(const CMsvSearchSortCacheEntry& aQuery, RBuf8& aQueryString, TInt aIndex, TInt aMtmIdArrayIndex)
	{
	_LIT8(KTo, 	 "To");
	_LIT8(KFrom, 	 "From");
	_LIT8(KCc, 	 "Cc");
	_LIT8(KBcc, 	 "Bcc");
	_LIT8(KSubject, "Subject");
	_LIT8(KDot, ".");
	_LIT8(KOr, " OR ");
		
	switch (aQuery.iMsgQuery[aIndex].iMessagePart)
		{
		// Header and Body Parts are searched at client side using FindL
		case EMsvTo:
			{
			if(iNumOfMsgParts)
				{
				if(iMtmIdArray.Count() > 1)
					{
					aQueryString.Append(KOr);
					}
				else
					{
					aQueryString.Append(KAnd);
					}
				}
						
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[aMtmIdArrayIndex].iUid);
			aQueryString.Append(KDot);
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KTo);
			aQueryString.Append(KQuote);
			
			// update query for case sensitive or case insensitive serach
			IsCaseSensitiveSearch(aQuery, aQueryString, aIndex);
			
			iNumOfMsgParts++;	
			}
			break;
			
		case EMsvFrom:
			{
			if(iNumOfMsgParts)
				{
				if(iMtmIdArray.Count() > 1)
					{
					aQueryString.Append(KOr);
					}
				else
					{
					aQueryString.Append(KAnd);
					}
				}
			
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[aMtmIdArrayIndex].iUid);
			aQueryString.Append(KDot);
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KFrom);
			aQueryString.Append(KQuote);
			
			// update query for case sensitive or case insensitive serach
			IsCaseSensitiveSearch(aQuery, aQueryString, aIndex);
			
			iNumOfMsgParts++;	
			}
			break;

						
		case EMsvCc:
			{
			if(iNumOfMsgParts)
				{
				if(iMtmIdArray.Count() > 1)
					{
					aQueryString.Append(KOr);
					}	
				else
					{
					aQueryString.Append(KAnd);
					}
				}
						
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[aMtmIdArrayIndex].iUid);
			aQueryString.Append(KDot);
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KCc);
			aQueryString.Append(KQuote);
			
			// update query for case sensitive or case insensitive serach
			IsCaseSensitiveSearch(aQuery, aQueryString, aIndex);
			
			iNumOfMsgParts++;		
			}
			break;

		case EMsvBcc:
			{
			if(iNumOfMsgParts)
				{
				if(iMtmIdArray.Count() > 1)
					{
					aQueryString.Append(KOr);
					}
				else
					{
					aQueryString.Append(KAnd);
					}
				}
			
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[aMtmIdArrayIndex].iUid);
			aQueryString.Append(KDot);
			
			aQueryString.Append(KQuote);
			aQueryString.Append(KBcc);
			aQueryString.Append(KQuote);
			
			// update query for case sensitive or case insensitive serach
			IsCaseSensitiveSearch(aQuery, aQueryString, aIndex);
			
			iNumOfMsgParts++;		
			}
			break;
						
		case EMsvSubject:
			{
			if(iNumOfMsgParts)
				{
				if(iMtmIdArray.Count() > 1)
					{
					aQueryString.Append(KOr);
					}
				else
					{
					aQueryString.Append(KAnd);
					}
				}
			
			UpdateQueryWithHeaderTable(aQueryString, KCurrentDriveId, iMtmIdArray[aMtmIdArrayIndex].iUid);
			aQueryString.Append(KDot);

			aQueryString.Append(KQuote);
			aQueryString.Append(KSubject);
			aQueryString.Append(KQuote);
			
			// update query for case sensitive or case insensitive serach
			IsCaseSensitiveSearch(aQuery, aQueryString, aIndex);
						
			iNumOfMsgParts++;		
			}
			break;
		
		default:
			break;
		}
	}

#endif  // SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB
