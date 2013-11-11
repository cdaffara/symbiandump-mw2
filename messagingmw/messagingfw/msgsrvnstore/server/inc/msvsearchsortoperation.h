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

#ifndef __MSVSEARCHSORTOPERATION_H__
#define __MSVSEARCHSORTOPERATION_H__

#include <mentact.h>
#include <msvstd.h>

#if !defined(__MSVAPI_H__)
#include <msvapi.h>
#endif

#if !defined(__MSVSEARCHSORTQUERY_H__)
#include <msvsearchsortquery.h>
#endif

#if !defined(__MSVSEARCHSORTOPONHEADERBODY_H__)
	
#endif

// forward declaration
class CMsvSession;
class CMsvSearchsortOpOnHeaderBody;

/**
* Class: CMsvSearchSortOperation
* 
* CMsvSearchSortOperation is used for extended search-sort in message store
* 
* @publishedPartner
* @released
*/

class CMsvSearchSortOperation : public CActive
	{
	public:
		// get CMsvSearchSortOperation object
		IMPORT_C static CMsvSearchSortOperation* NewL(CMsvSession& iMsvSession);
		//destructor
		IMPORT_C ~CMsvSearchSortOperation();
		
		//SearchSort request with Query
		IMPORT_C void RequestL(CMsvSearchSortQuery* aQuery, TBool aMarkQuery, TRequestStatus& aQueryStatus, TInt aIterator=0);
		//SearchSort request with QueryId
		IMPORT_C void RequestL(TInt aQueryId, TRequestStatus& aQueryStatus, TInt aIterator=0);

		IMPORT_C TInt GetResultsL (RArray<TMsvId>& aMsvIdList);  	//get results as TMsvId
		IMPORT_C TInt GetResultsL (RArray<TMsvEntry>& aMsvEntryList);	//get results as TMsvEntry
		
		IMPORT_C TInt GetNextResultL(TMsvId& aTMsvId); 			//to get results in Iterator
		IMPORT_C TInt GetNextResultL(TMsvEntry& aTMsvEntry);
		
		IMPORT_C TInt UnmarkQuery (const TInt aQueryId); 		//to unmark the query
		IMPORT_C TInt  GetQueryIdL();   			//to get the QueryId
		IMPORT_C TInt  GetResultCountL();			//to get the result count
		IMPORT_C TInt ProgressL();					//to get progress info
		
	private:
		enum TRequestType { EMsvSearchSortQuery, EMsvQueryId,  EMsvSearchSortOnHeaderBody, EMsvGetEntryForIds};
		enum TRequestQueryType { EMsvReqWithIterator, EMsvReqWithoutIterator };
		enum TSendResultToServer {ESendTMsvId, ESendTMsvIdWithSortFileld, ESendInvalidTMsvId };
		
	private:
		// constructor
		CMsvSearchSortOperation(CMsvSession& iMsvSession);
		void ConstructL(CMsvSession& aMsvSession);
		void ConstructSearchSortOperation(CMsvSearchSortQuery* aQuery, TBool aMarkQuery, TRequestStatus& aQueryStatus, TInt aIterator);
		void ConstructSearchSortOperation(TInt aQueryId, TRequestStatus& aQueryStatus, TInt aIterator);
		void SendRequestL();
		void DoSetActive();
		
		void SetRequestType(TRequestType aReqType, TRequestQueryType aQueryType);
		TInt ValidateQueryString(CMsvSearchSortQuery* aQuery);
		TBool IsSearchOnlyOnEntry();
		TBool IsSortOnlyOnEntry();
		TBool IsSearchQueryWithSortOnTMsvEntryFields();
		TBool IsInTMsvEntryField(TMsvMessagePart aMessagePart);
		TBool IsMessagePartInHeaderBody(TMsvMessagePart aMessagePart);
		void CheckWhetherSubFolderOptionIsEnabledForSortQueryL(CMsvSearchSortQuery* aQuery);
		TMsvOp OpId();
		
		void ProcessSearchSortQueryL();
		void ProcessSearchSortUsingQueryIdL();
		void ProcessSearchSortOnHeaderBodyL();
		
		void DoPorcessOnEntryResultsL();
		void DoProcessQuery();
		void DoPorcessOnNewSearchSortResultsL();
		void SendTMsvIdWithSortFieldResultsToserverL();
		void CheckIsArrayEmptyL(RArray<TMsvId>& aArray);
		void DeleteQueryData();
		//
		void RunL();
		void DoRunL();
		void DoCancel();
		void Complete();
		void DoComplete(TInt aError);
	
	protected:
		CMsvSession& iMsvSession;		
		
	private:
		TRequestStatus* iObserverRequestStatus;
		CMsvSearchSortQuery* iQuery;
		TInt iQueryId;
		TBool iMarkQuery;
		TInt iIterator;
		
		CMsvSearchsortOpOnHeaderBody *iHeaderBodySearch;
		TMsvSearchSortResultType iResultType;
		TMsvOp iOpId;   // opertaion id
		TInt iOperationProgress;	//for progrees info
		
		//results
		TInt iCount;
		TMsvId iTMsvId;
		TMsvEntry iTMsvEntry;
		RArray<TMsvId> iIdArray;
		RArray<TMsvEntry> iEntryArray;
		RArray<TMsvIdWithSortField> iTMsvIdWithSortField;
		
		//for proccesing the request
		TBool iIsSearchSortOnIndexEntry;
		TBool iIsQueryIdRequest;
		TRequestType iReqType;
  		TRequestQueryType iQueryType;
  		TSendResultToServer iResultToServer;
  		TInt iResultStatus;

	 	friend class CMsvSession;
		friend class CMsvSearchsortOpOnHeaderBody;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		friend class CTestSearchSortOperation;
#endif
	};

#endif /*__MSVSEARCHSORTOPERATION_H__*/

