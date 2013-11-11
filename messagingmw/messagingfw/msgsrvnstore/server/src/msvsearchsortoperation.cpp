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

#include <msvsearchsortoperation.h>
#include <msvuids.h>
#include "msvsearchsortoponheaderbody.h"

/** 
Allocates and constructs a CMsvSearchSortOperation object.

@param aMsvSession: The client’s Message Server session 
@return If the function succeeds, this is a pointer to a newly allocated 
and initialised object. 
@leave KErrNotFound The requested entry does not exist 
@leave KErrNoMemory A memory allocation failed 
*/
EXPORT_C CMsvSearchSortOperation* CMsvSearchSortOperation::NewL(CMsvSession& aMsvSession)
	{
	CMsvSearchSortOperation* self = new(ELeave) CMsvSearchSortOperation(aMsvSession);
	CleanupStack::PushL(self);
	self->ConstructL(aMsvSession);
	CleanupStack::Pop();
	return self;
	}

/**
Second phase construction
*/
void CMsvSearchSortOperation::ConstructL(CMsvSession& aMsvSession)
	{
	iHeaderBodySearch = CMsvSearchsortOpOnHeaderBody::NewL(aMsvSession, EPriorityStandard);
	CActiveScheduler::Add(this);
	}

/**
CMsvSearchSortOperation Constructor
@param aMsvSession: The client’s Message Server session 
*/
CMsvSearchSortOperation::CMsvSearchSortOperation(CMsvSession& aMsvSession) : CActive(EPriorityStandard), iMsvSession(aMsvSession)
	{
	iOpId = iMsvSession.OperationId();
	}

/** 
Destructor.
*/
EXPORT_C CMsvSearchSortOperation::~CMsvSearchSortOperation()
	{
	Cancel();
	delete iHeaderBodySearch;
	delete iQuery;
	iIdArray.Close();
	iEntryArray.Close();
	iTMsvIdWithSortField.Close();
	}

/**
Search-sort operation is performed on a specified folder (e.g, on INBOX, 
SENT-ITEMS, DRAFT, OUTBOX or any user created folder).

@param aQuery: A search-sort query object with search-sort options. 
The ownership of this pointer is passed to CMsvSearchSortOperation

@param aMarkQuery:Marks the query to indicate the framework to store this 
query in the internal search-sort cache for future use.  The queries are 
stored by default in the search-sort cache, but they are disposed when 
the cache reaches a maximum limit to accommodate the fresh query in the cache. 
The marked queries are less preferred to be disposed from the cache than the 
queries which are not marked during such a cache full condition unless all 
the queries in the cache are marked.
@param aQueryStatus: Provides the status of the asynchronous operation

@param aIterator: The number of result values to be returned at a time. 
By default the value is zero which indicates the results are expected in chunk 
and not using iterative mechanism. Currently the only other value this parameter can have is '1', due to limitation from SQLite.
@leave If iterator count is morethan one, it will with leave with KErrArgument
@return: None
*/	
EXPORT_C void CMsvSearchSortOperation::RequestL(CMsvSearchSortQuery* aQuery, TBool aMarkQuery, TRequestStatus& aQueryStatus, TInt aIterator)	
	{
	//Validate aQuery object
	if(aQuery->GetMaxQuerySize() <= 0)
		{
		//whether is it a sort query
		if(aQuery->iCountMultilevelSort <= 0)
			{
			//aQuery object doesn't contains any query fields to search/sort, so delete CMsvSearchSortQuery object.
			delete aQuery;
			User::Leave(KErrArgument);
			}
		}	
	/** 
	 For enabling the Iterator: aIterator value should be 1, 
	 if aIterator value is 0, then there is no Iterator mechanisam.
	 If iterator count is morethan one, it will with leave with KErrArgument.
	 */
	if(aIterator > 1)
		{
		delete aQuery;
		User::Leave(KErrArgument);
		}
	
	//Iterator is not supported for Header and/or Body or Combined (Header and/or Body and TMsvEntry) Query.
	//Do query validation when aIterator value is equal to one. 
	if(aIterator == 1)
		{
		TInt error = ValidateQueryString(aQuery);
		if(error != KErrNone)
			{
			delete aQuery;
			User::LeaveIfError(error);
			}
		}
	
	//If aQuery is a sort query, then sorting in subfolder is not supported
	CheckWhetherSubFolderOptionIsEnabledForSortQueryL(aQuery);
	
	ConstructSearchSortOperation(aQuery, aMarkQuery, aQueryStatus, aIterator);
	SendRequestL();
	}

/**
Getting Search-sort results for  specified QueryId.  

@param aQueryId: A unique 32 bit integer value associated with a query which is 
already executed ones in the past and which is assumed to be present in the 
search-sort cache.
@param aQueryStatus: Provides the status of the asynchronous operation
@param aIterator: Whether client requested results in Iterator
@leave If iterator count is morethan one, it will with leave with KErrArgument
@return: None
*/	
EXPORT_C void CMsvSearchSortOperation::RequestL(TInt aQueryId, TRequestStatus& aQueryStatus, TInt aIterator)
	{
	/* For enabling the Iterator: aIterator value should be 1, 
	 * if aIterator value is 0, then there is no Iterator mechanisam.
	 * If iterator count is morethan one, it will with leave with KErrArgument.
	 */
	if(aIterator > 1)
		{
		User::Leave(KErrArgument);
		}
	
	ConstructSearchSortOperation(aQueryId, aQueryStatus, aIterator);
	SendRequestL();	
	}

/** 
After the completion of asynchronous function RequestL(), this function provides 
the query Id corresponding to a particular search-sort query.

@param None
@return A unique integer query Id which can be used later to refer to the same 
search-sort query.
@leave KErrRequestPending if the asynchronous function RequestL() is not yet completed. 
*/
EXPORT_C TInt CMsvSearchSortOperation::GetQueryIdL()
	{
	//send query id to client
	iMsvSession.Session().GetSearchSortRequestQueryIdL(iQueryId);
	return iQueryId;
	}

/**
Unmarks the query listed in search-sort cache. The unmarked queries are preferred 
over marked queries for being disposed from the search-sort cache when the 
search-sort cache reaches a maximum limit

@param aQueryId: An unique queryId  associated with a query in search-sort cache. 
The query will be considered for being disposed off the search-sort cache when 
the search-sort cache reaches the maximum limit. The query will be actually 
disposed if it was unmarked, sometime in future when there is not enough space 
to store the newer query in the cache.
@leave KErrNotFound The requested queryId does not exist 
*/
EXPORT_C TInt CMsvSearchSortOperation::UnmarkQuery (const TInt aQueryId)
	{
	//unmark the query id
	TRAPD(error, iMsvSession.Session().UnmarkSearchSortRequestQueryIdL(aQueryId));
	return error;
	}


/** 
Returns the count of search-sort results, without returning the actual 
search-sort results.

@param None
@return The count of search-sort results
*/
EXPORT_C TInt CMsvSearchSortOperation::GetResultCountL()
	{
	if(*iObserverRequestStatus == KRequestPending)
		{
		User::Leave(KRequestPending);
		}

	iMsvSession.Session().GetResultCountL(iCount);
	return iCount;
	}



/**
After the completion of asynchronous function RequestL(), this function provides 
a list of index entry Id objects (TMsvId). 

@param aMsvIdList: output parameter containing the list of index entry Id objects 
which are the result of search-sort operation.
@return KErrRequestPending - GetResults() function is called before the completion 
of asynchronous function, else KErrNone.
@leave  KErrMsvInvalidResultRequest - If the result type requested during execution 
of query was set to TMsvEntry, but the messaging client is trying to receive TMsvId 
objects
*/
EXPORT_C TInt CMsvSearchSortOperation::GetResultsL (RArray<TMsvId>& aMsvIdList)
	{
	if(*iObserverRequestStatus == KRequestPending)
		{
		return KRequestPending;
		}

	if ((iResultType != EMsvResultAsTMsvId) && (iIterator != 0))
		{
		User::Leave(KErrMsvInvalidResultRequest);
		}
		
	TInt count = iIdArray.Count();
	for(TInt index = 0; index < count; ++index)
		{
		aMsvIdList.AppendL(iIdArray[index]);
		}
	return KErrNone;
	}
	
/** 
After the completion of asynchronous function RequestL(), this function provides 
a list of index entry objects (TMsvEntry).

@param aMsvEntryList: output parameter containing the list of index entry objects 
which are the result of search-sort operation.
@return KErrRequestPending - GetResults() function is called before the completion 
of asynchronous function, else KErrNone.
@leave  KErrMsvInvalidResultRequest - If the result type requested during execution 
of query was set to TMsvEntry, but the messaging client is trying to receive TMsvId 
objects
*/
EXPORT_C TInt CMsvSearchSortOperation::GetResultsL (RArray<TMsvEntry>& aMsvEntryList)
	{
	if(*iObserverRequestStatus == KRequestPending)
		{
		return KRequestPending;
		}

	if((iResultType != EMsvResultAsTMsvEntry) && (iIterator != 0))
		{
		User::Leave(KErrMsvInvalidResultRequest);
		}

	TInt count = iEntryArray.Count();
	for(TInt index = 0; index < count; ++index)
		{
		aMsvEntryList.AppendL(iEntryArray[index]);
		}
	return KErrNone;	
	}


/** 
Gets the TMsvId of a single index entry object at a time. This function is used 
when getting the results using iterative mechanism.

@param aTMsvId:  A reference of index entry Id of the index entry, which is one 
of the results of search-sort operation. All the TMsvId objects could be received 
one after the other using this function.
@return  Count of remaining search-sort results
*/
EXPORT_C TInt CMsvSearchSortOperation::GetNextResultL(TMsvId& aTMsvId)
	{
	if(*iObserverRequestStatus == KRequestPending)
		{
		return KRequestPending;
		}

	if ((iResultType != EMsvResultAsTMsvId) && (iIterator != 1))
		{
		User::Leave(KErrMsvInvalidResultRequest);
		}
	
	iMsvSession.Session().GetNextIdL(aTMsvId, iCount);
	return iCount;
	}

/** 
Gets one index entry object at a time. This function is used when getting the results 
using iterative mechanism.

@param aTMsvId: next result as TMsvEntry 
@return Count of remaining search-sort results
*/
EXPORT_C TInt CMsvSearchSortOperation::GetNextResultL(TMsvEntry& aTMsvEntry)
	{
	if(*iObserverRequestStatus == KRequestPending)
		{
		return KRequestPending;
		}
	
	if((iResultType != EMsvResultAsTMsvEntry) && (iIterator != 1))
		{
		User::Leave(KErrMsvInvalidResultRequest);
		}

	iMsvSession.Session().GetNextEntryL(aTMsvEntry, iCount);
	return iCount;
	}	


/** 
Returns progress information on the current search-sort request.
@return The progress information on an asynchronous search-sort operation.
@leave  
*/
EXPORT_C TInt CMsvSearchSortOperation::ProgressL()
	{
	iOperationProgress = KMsvSearchSortOpNone;
	if(*iObserverRequestStatus == KRequestPending)
		{
		iMsvSession.Session().SearchSortOperationProgressL(OpId(), iOperationProgress);
		return iOperationProgress;
		}
	return iOperationProgress;
	}


// Requet is SearchSort Query 	
void CMsvSearchSortOperation::ConstructSearchSortOperation(CMsvSearchSortQuery* aQuery, TBool aMarkQuery, TRequestStatus& aQueryStatus, TInt aIterator)
	{
	iQuery = aQuery;
	iMarkQuery = aMarkQuery;
	iIterator = aIterator;
	iObserverRequestStatus = &aQueryStatus;
	iResultType = iQuery->GetResultType();
	
	if(iIterator == 1)
		{
		SetRequestType(EMsvSearchSortQuery, EMsvReqWithIterator);
		}
	else
		{
		SetRequestType(EMsvSearchSortQuery, EMsvReqWithoutIterator);
		}
	}

// Requet is SearchSort QueryId
void CMsvSearchSortOperation::ConstructSearchSortOperation(TInt aQueryId, TRequestStatus& aQueryStatus, TInt aIterator)
	{
	iQueryId = aQueryId;
	iIterator = aIterator;
	iObserverRequestStatus = &aQueryStatus;
	iIsQueryIdRequest = ETrue;
	
	if(iIterator == 1)
		{
		SetRequestType(EMsvQueryId, EMsvReqWithIterator);
		}
	else
		{
		SetRequestType(EMsvQueryId, EMsvReqWithoutIterator);
		}
	}


// send a request to server according to the Request given by client
void CMsvSearchSortOperation::SendRequestL()
	{
	switch(iReqType)
		{
		case EMsvSearchSortQuery:
			//search and sort both are TMsvEntry fields 
			//or sort query only on TMsvEntry fields (sort query)
			if (IsSearchOnlyOnEntry() || IsSortOnlyOnEntry())
				{
				iIsSearchSortOnIndexEntry = ETrue;
				iMsvSession.Session().SearchSortRequestOnEntryL(iQuery, OpId(), iMarkQuery, iIterator, iStatus);
				}
			else
				{
				// Iterator is not supported for Header and/or Body or Combination of Header and/or Body with TMsvEntry fields.
				iIsSearchSortOnIndexEntry = EFalse;
				iMsvSession.Session().SearchSortRequestOnHeaderBodyL(iQuery, OpId(), iMarkQuery, iStatus);	
				}
			break;
			
		case EMsvQueryId:
			// send request to server to get resuls from this query id. 
			iMsvSession.Session().GetResultsForQueryId(iQueryId, OpId(), iIterator, iStatus);
			break;
		}
	// setActive
	DoSetActive();
	}

// do setActive
void CMsvSearchSortOperation::DoSetActive()
	{
	*iObserverRequestStatus = KRequestPending;
	SetActive();
	}


void CMsvSearchSortOperation::RunL()
	{
	TInt error = iStatus.Int();
		
	//do process
	if(error == KErrNone)
		{
		TRAP(error, DoRunL());
		}
	
	//complete with error	
	if(error != KErrNone)
		{
		//KErrNotFound comes when results are zero for a query, completed with KErrNone, 
		//so that client can show '0' results found
		if(error == KErrNotFound)
			{
			error = KErrNone;
			}
		
		//KErrUnknown comes when query id doesn't exist, so completing with KErrNotFound
		if(error == KErrUnknown)
			{
			error = KErrNotFound;
			}
		DoComplete(error);
		}
	}
	

void CMsvSearchSortOperation::DoRunL()
	{
	switch(iReqType)
		{
	case EMsvSearchSortQuery:
		{
		ProcessSearchSortQueryL();
		}
		break;
		
	case EMsvQueryId:
		{
		ProcessSearchSortUsingQueryIdL();
		}
		break;
		
	case EMsvSearchSortOnHeaderBody:
		{
		ProcessSearchSortOnHeaderBodyL();
		}
		break;
			
	case EMsvGetEntryForIds:
		{
		iEntryArray.Reset();
		iHeaderBodySearch->GetResultAsTmsvEntryL(iEntryArray);
		Complete();
		}
		break;
			
	default:
		break;
		}
	}



void CMsvSearchSortOperation::ProcessSearchSortQueryL()
	{
	if(iIsSearchSortOnIndexEntry)
		{
		DoPorcessOnEntryResultsL();
		} 
	else // seraching on Header and/or Body
		{
		/**
		If search query on Header and/or Body or combined (TMsvEntry and Header and/or Body) query
		call GetResultAsIdL(iIdArray, resultStatus), 
		from the resultStatus, will come to know the ResultType
		i) final result (complete results from cache)
		ii) partial result (cache results + delat cache entries)
		iii) It's new search for all id's
		Iterator is not supported for Header and/or Body search
		*/
		iIdArray.Reset();
		
		iMsvSession.Session().GetResultAsIdL(iIdArray, iResultStatus);
						
		switch(iResultStatus)
			{
		case KFinalResult:
			{
			CheckIsArrayEmptyL(iIdArray);
			
			if (iQuery->GetResultType() == EMsvResultAsTMsvEntry)
				{
				iReqType = EMsvGetEntryForIds;
				iHeaderBodySearch->DoGetEntryForAllIdL(iIdArray, iStatus);
				DoSetActive();
				}
			else
				{
				Complete();	
				}
			}
			break;						
											
		case KPartialResult:
		case KNewQuery:
			{
			/**
			* GetResultAsIdL(iIdArray, iResultStatus);
			* needs to search these Id's against Header and/or Body according to query request. 
			* send results to server, so that results will be sorted at (along with existing messages) DB 
			* and server will send final result.
			*/
			
			iReqType = EMsvSearchSortOnHeaderBody;
			CheckIsArrayEmptyL(iIdArray);
			DoProcessQuery();
			
			//sortOnTMsvEnty should set to ETrue,  when we want to send invalid TMsvId to Server
			TBool sortOnTMsvEnty = EFalse;
			if(iResultToServer == ESendInvalidTMsvId)
				{
				sortOnTMsvEnty = ETrue;
				}		
			iHeaderBodySearch->SearchSortOnHeaderBodyL(iQuery, iIdArray, sortOnTMsvEnty, iStatus);	
			DoSetActive();
			
			}
			break;
		
		default:
			break;
			}
		}
	}

void CMsvSearchSortOperation::ProcessSearchSortUsingQueryIdL()
	{
	iIdArray.Reset();
	iMsvSession.Session().GetResultAsIdL(iIdArray, iResultStatus);
		
	if( iResultStatus == KMsvSearchSortQueryIdNotFound)
		{
		//inform to client
		User::Leave(KErrUnknown);
		}
	else if(iResultStatus == KFinalResult)
		{
		//check whether result array is empty
		CheckIsArrayEmptyL(iIdArray);
		
		//make sure previous Query object get deleted before allocate new one
		DeleteQueryData();
	
		iQuery = CMsvSearchSortQuery::NewL();
		CleanupStack::PushL(iQuery);
		
		// get the query from server
		iMsvSession.Session().GetQueryFromServerL(OpId(), iQueryId, iQuery);
		iResultType = iQuery->GetResultType();
		CleanupStack::Pop(iQuery);
		
		if (iResultType == EMsvResultAsTMsvId)
			{
			Complete();	
			}
		else
			{
			iReqType = EMsvGetEntryForIds;
			iHeaderBodySearch->DoGetEntryForAllIdL(iIdArray, iStatus);
			DoSetActive();
			}
		}
	else if(iResultStatus == KPartialResult)
		{
		iReqType = EMsvSearchSortOnHeaderBody;
		CheckIsArrayEmptyL(iIdArray);
		
		//make sure previous Query object get deleted before allocate new one
		DeleteQueryData();
		iQuery = CMsvSearchSortQuery::NewL();
		CleanupStack::PushL(iQuery);
		
		// get the query from server
		iMsvSession.Session().GetQueryFromServerL(OpId(), iQueryId, iQuery);
		iResultType = iQuery->GetResultType();
		DoProcessQuery();

		CleanupStack::Pop(iQuery);
		//sortOnTMsvEnty should set to ETrue,  when we want to send invalid TMsvId to Server
		TBool sortOnTMsvEnty = EFalse;
		if(iResultToServer == ESendInvalidTMsvId)
			{
			sortOnTMsvEnty = ETrue;
			}		
		iHeaderBodySearch->SearchSortOnHeaderBodyL(iQuery, iIdArray, sortOnTMsvEnty, iStatus);	
		DoSetActive();
		}
	}

void CMsvSearchSortOperation::ProcessSearchSortOnHeaderBodyL()
	{
	//change the state according to type of Request
	if(iIsQueryIdRequest)
		{
		iReqType = EMsvQueryId;
		}
	else
		{
		iReqType = EMsvSearchSortQuery;	
		}
	
	//processing header or/and Body search results 
	if(iResultToServer == ESendInvalidTMsvId)  //sort on TMsvEntry fields
		{
		iIdArray.Reset();
		iHeaderBodySearch->GetResultAsInvalidTMsvIdL(iIdArray);
		DoPorcessOnNewSearchSortResultsL();
		}
	else if(iResultToServer == ESendTMsvIdWithSortFileld)
		{
		iTMsvIdWithSortField.Reset();
		iHeaderBodySearch->GetResultAsTMsvIdWithSortFieldL(iTMsvIdWithSortField);
		SendTMsvIdWithSortFieldResultsToserverL();
		}
	else
		{
		//iResultToServer == ESendTMsvId
		}
	}
	

void CMsvSearchSortOperation::DoCancel()
	{
	// cancel the header body search
	if(iHeaderBodySearch->IsActive())
		{
		iHeaderBodySearch->Cancel();
		}
	
	TRAPD(ret, iMsvSession.Session().CancelSearchSortOperation(OpId()));
	
	// signal the observer that the operation has cancelled
	if(ret == KErrNone) // For removing warning
		{
		TRequestStatus* status = iObserverRequestStatus;
		User::RequestComplete(status, KErrCancel);
		}
	}

void CMsvSearchSortOperation::DoProcessQuery()
	{
	iResultToServer = ESendTMsvId;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if( iResultStatus == KPartialResult )	
#else
	if(IsSearchQueryWithSortOnTMsvEntryFields() && (iResultStatus == KNewQuery || iResultStatus == KPartialResult))
#endif
		{
		iResultToServer = ESendInvalidTMsvId;
		}
	else
		{
		iResultToServer = ESendTMsvIdWithSortFileld;
		}
	}

void CMsvSearchSortOperation::DoPorcessOnEntryResultsL()
	{
	// In case of Index entry search, resultant query sending to client is updated with 
	// delta cache entries.
	if(iQueryType == EMsvReqWithIterator)
		{
		iMsvSession.Session().GetResultCountL(iCount);
		if(iCount <= 0)
			{
			User::Leave(KErrNotFound);
			}
		Complete();	
		}
	else // EMsvReqWithoutIterator
		{
		//get the result from server
		iIdArray.Reset();
		iMsvSession.Session().GetResultAsIdL(iIdArray);
		CheckIsArrayEmptyL(iIdArray);
		
		if (iQuery->GetResultType() == EMsvResultAsTMsvId)
			{
			Complete();	
			}
		else
			{
			iReqType = EMsvGetEntryForIds;
			iHeaderBodySearch->DoGetEntryForAllIdL(iIdArray, iStatus);
			DoSetActive();
			}
		}
	}

void CMsvSearchSortOperation::DoPorcessOnNewSearchSortResultsL()
	{
	//send invalid array list to server an send valid array list to clent
	iMsvSession.Session().SendNewResultsToServerForSortL(OpId(), iIdArray, iStatus);
	DoSetActive();
	}
	

void CMsvSearchSortOperation::SendTMsvIdWithSortFieldResultsToserverL()
	{
	// TMsvId along with sort field
	iMsvSession.Session().SendResultantListL(OpId(), iTMsvIdWithSortField, iStatus);
	DoSetActive();
	}
	

void CMsvSearchSortOperation::Complete()
	{
	DeleteQueryData();
	
	// signal the observer that the operation has completed
	TRequestStatus* status = iObserverRequestStatus;
	User::RequestComplete(status, iStatus.Int());
	}


void CMsvSearchSortOperation::DoComplete(TInt aError)
	{
	DeleteQueryData();
	
	// signal the observer that the operation has completed with error
	TRequestStatus* status = iObserverRequestStatus;
	User::RequestComplete(status, aError);
	}


// Is search query only on TMsvEntry
TBool CMsvSearchSortOperation::IsSearchOnlyOnEntry()
	{
	// get maxiumun query size
	TInt maxquerysize = iQuery->GetMaxQuerySize();
	
	// flag to decide whether it's index query or header/body or combination
	TBool isOnlyOnEntry = EFalse;
	
	TMsvMessagePart messagepart;
	for(TInt count = 0; count < maxquerysize; ++count)
		{
		// get the message part
		messagepart = iQuery->GetMessagePart(count);
				
		// To check is this message part belongs to Index entry table..
		if(IsInTMsvEntryField(messagepart))
			{
			/*
			* Check specifically below condition for attachment.
			* To check whether attachment filed is present or not, will be seraching on Index Entry table.
			* To check CMsvAttachment::TMsvAttachmentType will be searching in header and/or body.
			*/
			if(messagepart == EMsvAttachment)
				{
				TInt value = 0;
				TLex iLex(iQuery->iQueryTable[count].iQueryString);
				iLex.Val(value);
				if(value == 0 || value == 1)
					{
					isOnlyOnEntry = ETrue;
					}
				else
					{
					isOnlyOnEntry = EFalse;	
					}
				}//end of EMsvAttachment condition
			else
				{
				isOnlyOnEntry = ETrue;	
				}
			}
		
		//Is search message part belongs to header and/or body
		if(IsMessagePartInHeaderBody(messagepart))
			{
			return EFalse;
			}
		}
	
	//check whether sort is on Header or Body
	messagepart = iQuery->GetMessagePart();	
	if(isOnlyOnEntry && (IsMessagePartInHeaderBody(messagepart)))
		{
		isOnlyOnEntry = EFalse;
		}
	
	return isOnlyOnEntry;
	}


// It's sort query, sort only on TMsvEntry fields
TBool CMsvSearchSortOperation::IsSortOnlyOnEntry()
	{
	TMsvMessagePart messagepart = iQuery->GetMessagePart();
	
	if(!iQuery->IsSearchQuery() && (IsInTMsvEntryField(messagepart)))
		{
		return ETrue;
		}
	return EFalse;
	}


//This functin to check whether given query is sort on TMsvEntry fields 
//it's search query on Header and/or Body and sort on TMsvEntry fields
TBool CMsvSearchSortOperation::IsSearchQueryWithSortOnTMsvEntryFields()
	{
	TMsvMessagePart messagepart = iQuery->GetMessagePart();
		
	if(IsInTMsvEntryField(messagepart))
		{
		return ETrue;
		}
	return EFalse;
	}


// for setting the QueryType (with or witout iterator) and RequestType (it's a Query or QueryId)
void CMsvSearchSortOperation::SetRequestType(TRequestType aReqType, TRequestQueryType aQueryType)
	{
	iReqType = aReqType;
	iQueryType = aQueryType;
	}


// get operation id
TMsvOp CMsvSearchSortOperation::OpId()
	{
	return iOpId;
	}

//Check is Array is Empty
void CMsvSearchSortOperation::CheckIsArrayEmptyL(RArray<TMsvId>& aArray)
	{
	if(aArray.Count() <= 0)
		{
		//reset the result array
		iIdArray.Reset();
		iEntryArray.Reset();
		
		User::Leave(KErrNotFound);
		}
	}

TInt CMsvSearchSortOperation::ValidateQueryString(CMsvSearchSortQuery* aQuery)
	{	
	// get maxiumun query size
	TInt maxquerysize = aQuery->GetMaxQuerySize();
	
	TMsvMessagePart messagepart;
	for(TInt count = 0; count < maxquerysize; ++count)
		{
		// get the message part
		messagepart = aQuery->GetMessagePart(count);
		//check whether message part belongs to Header and/or body filed
		if(IsMessagePartInHeaderBody(messagepart))
			{
			return KErrNotSupported;
			}
		}
	return KErrNone;
	}
	
void CMsvSearchSortOperation::DeleteQueryData()
	{
	if(iQuery != NULL)
		{
		delete iQuery;
		iQuery = NULL;
		}
	}	

//If it's sort query, then subfolder option should not be set
void CMsvSearchSortOperation::CheckWhetherSubFolderOptionIsEnabledForSortQueryL(CMsvSearchSortQuery* aQuery)
	{
	if((!aQuery->IsSearchQuery()) && (aQuery->IsSubFolderSearch()))
		{
		//delete CMsvSearchSortQuery object
		delete aQuery;
		User::Leave(KErrNotSupported);
		}
	}

//If query is made with only follwing parts, then needs to be search at DB side
TBool CMsvSearchSortOperation::IsInTMsvEntryField(TMsvMessagePart aMessagePart)
	{
	TBool isEntryField = EFalse;
	
	switch(aMessagePart)
		{
		case TMsvMessagePart(0):
		case EMsvDate:
		case EMsvSize:
		case EMsvAttachment:
		case EMsvDetails:
		case EMsvMtmTypeUID: 
		case EMsvUnreadMessages:
		case EMsvNew:
		case EMsvDescription:
		case EMsvPriority:
			isEntryField = ETrue;
			break;
			
		default:
			isEntryField = EFalse;
		}
	return isEntryField;
	}


//Is Message parts belongs to Header and/or Body fields
TBool CMsvSearchSortOperation::IsMessagePartInHeaderBody(TMsvMessagePart aMessagePart)
	{	
	TBool isHeaderbodyField = EFalse;
	
	switch(aMessagePart)
		{
		case EMsvTo:
		case EMsvFrom:
		case EMsvCc:
		case EMsvBcc:
		case EMsvSubject:
		case EMsvBody: 
			isHeaderbodyField = ETrue;
			break;
			
		default:
			isHeaderbodyField = EFalse;
		}
	return isHeaderbodyField;
	}
