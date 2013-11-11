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
// SearchSortByQueryObject
// [Paramaters]
// MarkQuery
// IteratorLimit
// CountOfResult
// IsRepetitionRequired
// LastQueryID
// RepeatedQueryID
// Performs search/sort request and gets the corresponding result.
// [APIs Used]
// CMsvSearchSortOperation::RequestL
// CMsvSearchSortOperation::GetQueryIdL
// CMsvSearchSortOperation::GetResultCountL
// CMsvSearchSortOperation::GetNextResultL
// CMsvSearchSortOperation::GetResultsAsTMsvId
// CMsvSearchSortOperation::GetResultsAsTMsvEntry
// 
//

/* User Includes */
#include "t_msgperfsearchusing1667qo.h"

/* Epoc includes */
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>

// Literals used 
_LIT(KMarkQuery, "MarkQuery");
_LIT(KIteratorLimit, "IteratorLimit");
_LIT(KCountOfResult, "CountOfResult");
_LIT(KLastQueryID, "LastQueryID");
_LIT(KIsRepetitionRequired, "IsRepetitionRequired");
_LIT(KRepeatedQueryID, "RepeatedQueryID");
_LIT(KResultAsTMsvEntry, "ResultAsTMsvEntry");

/**
 Function : NewL
 Description : Creates an object of CT_MsgPerfSearchUsing1667QO
 @param : aSharedDataCommon	A reference to CT_MsgSharedDataCommon
 @return : A pointer to an object of type CT_MsgPerfSearchUsing1667QO
 @leave  : KErrNoMemory if	sufficient memory is not available
 */
CT_MsgPerfSearchUsing1667QO* CT_MsgPerfSearchUsing1667QO::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgPerfSearchUsing1667QO* self = new(ELeave) CT_MsgPerfSearchUsing1667QO(aSharedDataCommon);
	CleanupStack::PushL (self);
	self->ConstructL ();
	CleanupStack::Pop (self);
	return self;
	}

/**
 Function : CT_MsgPerfSearchUsing1667QO
 Description : Constructor
 @param : aSharedDataCommon	A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgPerfSearchUsing1667QO::CT_MsgPerfSearchUsing1667QO(CT_MsgSharedDataCommon& aSharedDataCommon) 
	:CT_MsgAsyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KPerfSearchUsing1667QO);
	}

/**
 Function : ConstructL
 Description : Calls the ConstrutL of CT_MsgAsyncStepCommon
 @return : N/A
 */
void CT_MsgPerfSearchUsing1667QO::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL ();
	}

/**
 Function : ~CT_MsgPerfSearchUsing1667QO
 Description : Destructor
 @return : N/A
 */
CT_MsgPerfSearchUsing1667QO::~CT_MsgPerfSearchUsing1667QO()
	{
	}

/**
 Function : doTestStepL
 Description : Get the count of message entries satisfying serach-sort criteria and get the entries.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgPerfSearchUsing1667QO::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : SearchSortByQueryObject"));
	
	// Read query marking option
	TBool markQuery = EFalse;
	GetBoolFromConfig(ConfigSection(), KMarkQuery, markQuery);
	markQuery ? INFO_PRINTF1(_L("Query is marked")) : INFO_PRINTF1(_L("Query is to not marked"));

	// Read the iteration limit for getting the results
	TInt iteratorLimit = 0;
	GetIntFromConfig(ConfigSection(), KIteratorLimit, iteratorLimit);
	TUint32 startTest = User::NTickCount();
	// Execute the search/sort request
	iSharedDataCommon.iSearchSortOperation = CMsvSearchSortOperation::NewL(*iSharedDataCommon.iSession);
	CT_MsgActive& active=Active();
	TRAPD(err, iSharedDataCommon.iSearchSortOperation->RequestL(iSharedDataCommon.iSearchSortQuery, markQuery, active.iStatus, iteratorLimit));
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
			// Get the query ID for the above search/sort request
			TInt quryId = iSharedDataCommon.iSearchSortOperation->GetQueryIdL();
			TBool isRepetitionRequired = EFalse;
			GetBoolFromConfig(ConfigSection(), KIsRepetitionRequired, isRepetitionRequired);
			// Save the query ID to INI file
			isRepetitionRequired ? WriteIntToConfig(ConfigSection(), KRepeatedQueryID, quryId):WriteIntToConfig(ConfigSection(), KLastQueryID, quryId);

			TBool resInTMsvEntry = EFalse;
			GetBoolFromConfig(ConfigSection(), KResultAsTMsvEntry, resInTMsvEntry);	
			TMsvSearchSortResultType resultType = EMsvResultAsTMsvId;
			if(resInTMsvEntry)
				{
				resultType = EMsvResultAsTMsvEntry;
				}
			RetriveSearchSortResultL(iteratorLimit, resultType);
			}
		active.Cancel();
		}
	else
		{
		SetTestStepError(err);	
		}
	TUint32 endTest = User::NTickCount();
	INFO_PRINTF2(_L("<font color=660099><b>Time take to search the List is %d milliseconds</b></font>"), (endTest-startTest));
	return TestStepResult();
	}

/**
 Function : RetriveSearchSortResultL
 Description : Retrieves the search/sort result
 @return : none
 */
void CT_MsgPerfSearchUsing1667QO::RetriveSearchSortResultL(const TInt aIterationLimit, TMsvSearchSortResultType aResultType)
	{
	// Get the number of messages satisfying the search/sort request
	TInt resultCount = iSharedDataCommon.iSearchSortOperation->GetResultCountL();
	//WriteIntToConfig(ConfigSection(), KCountOfResult, resultCount); 
	
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
void CT_MsgPerfSearchUsing1667QO::ProgressL(TBool aFinal)
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
void CT_MsgPerfSearchUsing1667QO::CancelStep()
	{
	//	Stub
	}
