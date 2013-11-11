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
// [TestStep Name]
// SearchSortResultByQueryId
// [Paramaters]
// RepeatedQueryID		: Query ID previously stored by SearchSortByQueryObject.
// IteratorLimit		: Number of entries to be returned in each Iteration(supported values are 0 and 1).
// ResultAsTMsvEntry	: Type of the result(TMsvEntry ot TMsvId) need to be returned by search/sort operation.
// Search/sort the messages with respect to a specific query ID that has been issued previously.
// Options for getting the results can be added to the query ID as well.
// [APIs Used]
// CMsvSearchSortOperation::RequestL
// CMsvSearchSortOperation::GetResultCount
// 
//

/* User Includes */
#include "t_searchsortresultbyqueryid.h"

/* Epoc includes */
#include <msvsearchsortoperation.h>


// Literals used 
_LIT(KRepeatedQueryID, "RepeatedQueryID");
_LIT(KIteratorLimit, "IteratorLimit");
_LIT(KResultAsTMsvEntry, "ResultAsTMsvEntry");
_LIT(KCountOfResult, "CountOfResult");

/**
 Function : NewL
 Description : Creates an object of CT_MsgSearchSortResultByQueryId
 @param : aSharedDataCommon		A reference to CT_MsgSharedDataCommon
 @return : A pointer to an object of type CT_MsgSearchSortResultByQueryId
 @leave  : KErrNoMemory if	sufficient memory is not available
 */
CT_MsgSearchSortResultByQueryId* CT_MsgSearchSortResultByQueryId::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgSearchSortResultByQueryId* self = new(ELeave) CT_MsgSearchSortResultByQueryId(aSharedDataCommon);
	CleanupStack::PushL (self);
	self->ConstructL ();
	CleanupStack::Pop (self);
	return self;
	}

/**
 Function : CT_MsgSearchSortResultByQueryId
 Description : Constructor
 @param : aSharedDataCommon A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgSearchSortResultByQueryId::CT_MsgSearchSortResultByQueryId(CT_MsgSharedDataCommon& aSharedDataCommon) 
	: CT_MsgAsyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KSearchSortResultByQueryId);
	}

/**
 Function : ConstructL
 Description : Calls the ConstrutL of CT_MsgAsyncStepCommon
 @return : N/A
 */
void CT_MsgSearchSortResultByQueryId::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL ();
	}

/**
 Function : ~CT_MsgSearchSortResultByQueryId
 Description : Destructor
 @return : N/A
 */
CT_MsgSearchSortResultByQueryId::~CT_MsgSearchSortResultByQueryId()
	{
	}

/**
 Function : doTestStepL
 Description : Get the count of message entries satisfying serach-sort criteria.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgSearchSortResultByQueryId::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : SearchSortResultByQueryId"));
	TInt queryId = 0;
	if(!GetIntFromConfig(ConfigSection(), KRepeatedQueryID, queryId))
		{
		ERR_PRINTF1(_L("Can not find any Query ID for Execution"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Set the preferred result type flag, default value is TMsvId 
		TBool resInTMsvEntry = EFalse;
		GetBoolFromConfig(ConfigSection(), KResultAsTMsvEntry, resInTMsvEntry);	
		TMsvSearchSortResultType resultType = EMsvResultAsTMsvId;
		if(resInTMsvEntry)
			{
			resultType = EMsvResultAsTMsvEntry;
			}
		
		// Set the iteration limit for getting the results
		TInt iteratorLimit = 0;
		GetIntFromConfig(ConfigSection(), KIteratorLimit, iteratorLimit);

		// Execute the search/sort request
		iSharedDataCommon.iSearchSortOperation = CMsvSearchSortOperation::NewL(*iSharedDataCommon.iSession);
		CT_MsgActive& active=Active();
		TRAPD(err, iSharedDataCommon.iSearchSortOperation->RequestL(queryId, active.iStatus, iteratorLimit));

		if(err == KErrNone)
			{
			active.Activate();
			CActiveScheduler::Start();
			
			//Check Search/Sort operation for errors
			TInt error = active.Result();
			if (error != KErrNone)
				{
				ERR_PRINTF2(_L("Search/Sort request failed with %d error"), error);
				SetTestStepError(error);
				}
			else
				{
				RetriveSearchSortResultL(iteratorLimit, resultType);
				}
			}
		else
			{
			SetTestStepError(err);	
			}
		}
		
	return TestStepResult();
	}

/**
 Function : RetriveSearchSortResultL
 Description : Retrieves the search/sort result
 @return : none
 */
void CT_MsgSearchSortResultByQueryId::RetriveSearchSortResultL(const TInt aIterationLimit, TMsvSearchSortResultType aResultType)
	{
	// Get the number of messages satisfying the search/sort request
	TInt resultCount = iSharedDataCommon.iSearchSortOperation->GetResultCountL();
	WriteIntToConfig(ConfigSection(), KCountOfResult, resultCount); 
	// Get the search/sort result according to user preference
	TMsvId messageId, serviceId;
	TMsvEntry messageEntry;
	iSharedDataCommon.iSearchSortResultArray.Reset(); // Flush out the existing search/sort result
	if (aIterationLimit > 0 && aResultType == EMsvResultAsTMsvId) // Iteration limit is one and result type is TMsvId
		{
		for(TInt index = 0; index < resultCount; ++index)
			{
			iSharedDataCommon.iSearchSortOperation->GetNextResultL(messageId);
			 // Get the corresponding index entry and append the same to array of entries.
			iSharedDataCommon.iSession->GetEntry(messageId, serviceId, messageEntry);
			iSharedDataCommon.iSearchSortResultArray.Append(messageEntry);
			}
		}
	else if(aIterationLimit > 0 && aResultType == EMsvResultAsTMsvEntry) // Iteration limit is one and result type is TMsvEntry
		{
		for(TInt index = 0; index < resultCount; ++index)
			{
			iSharedDataCommon.iSearchSortOperation->GetNextResultL(messageEntry);
			iSharedDataCommon.iSearchSortResultArray.Append(messageEntry);
			}
		}
	else if (aIterationLimit == 0 && aResultType == EMsvResultAsTMsvId) // No iteration and result type is TMsvId
		{
		RArray<TMsvId> idArray;
		TRAPD(error, iSharedDataCommon.iSearchSortOperation->GetResultsL(idArray));
		if(error == KErrNone)
			{
			// Get the corresponding index entries and create an array of entries.
			for(TInt index = 0; index < idArray.Count(); ++index)
				{
				iSharedDataCommon.iSession->GetEntry(idArray[index], serviceId, messageEntry);
				iSharedDataCommon.iSearchSortResultArray.Append(messageEntry);
				}
			}
		else
			{
			SetTestStepError(error);
			}
		}
	else // No iteration and result type is TMsvEntry
		{
//		RArray<TMsvEntry> entryArray;
		TRAPD(error, iSharedDataCommon.iSearchSortOperation->GetResultsL(iSharedDataCommon.iSearchSortResultArray));
		SetTestStepError(error);
		}
	// Ensure Number of Entries in iSharedDataCommon.iSearchSortResultArray is same as the result count
	if(iSharedDataCommon.iSearchSortResultArray.Count() != resultCount)
		{
		ERR_PRINTF1(_L("MisMatch in Result count and Entries retrieved"));
		ERR_PRINTF3(_L("Search-sort result count= %d Entries retrieved= %d"), resultCount, iSharedDataCommon.iSearchSortResultArray.Count());
		SetTestStepResult(EFail);
		}
	}

/**
 Function : ProgressL
 Description : Provides the progress information of the asynchronous operation
 @param :	aFinal	Used to display the status of the asynchronous operation
 @return : none
 */
void CT_MsgSearchSortResultByQueryId::ProgressL(TBool aFinal)
	{
	//	Stub
	if ( aFinal )
		{
		/*	Display final progress */
		}
	else
		{
		/*	Display current progress*/
		}
	}

/**
 Function : CancelStep
 Description : Used to cancel an asynchronous operation
 @return : none
 */
void CT_MsgSearchSortResultByQueryId::CancelStep()
	{
	//	Stub
	}

