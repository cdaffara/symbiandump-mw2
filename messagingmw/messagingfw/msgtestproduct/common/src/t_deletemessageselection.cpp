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
// DeleteMessageSelection
// [Paramaters]
// NumberOfEntries <IN> Number of message entries to be deleted form the list
// A -ve number implies that all the messages need to be deleted.
// Deletes the messages coresponding to the entries enlisted in the array.
// [APIs Used]
// 
//

/* User Includes */
#include "t_deletemessageselection.h"

/* Epoc includes */
#include <msvids.h>

// Literals used 
_LIT(KNumberOfEntries, "NumberOfEntries");

/**
 Function : NewL
 Description : Creates an object of CT_MsgDeleteMessageSelection
 @param : aSharedDataCommon	A reference to CT_MsgSharedDataCommon
 @return : A pointer to an object of type CT_MsgDeleteMessageSelection
 @leave  : KErrNoMemory if	sufficient memory is not available
 */
CT_MsgDeleteMessageSelection* CT_MsgDeleteMessageSelection::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgDeleteMessageSelection* self = new(ELeave) CT_MsgDeleteMessageSelection(aSharedDataCommon);
	CleanupStack::PushL (self);
	self->ConstructL ();
	CleanupStack::Pop (self);
	return self;
	}

/**
 Function : CT_MsgDeleteMessageSelection
 Description : Constructor
 @param : aSharedDataCommon	A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgDeleteMessageSelection::CT_MsgDeleteMessageSelection(CT_MsgSharedDataCommon& aSharedDataCommon) 
	:CT_MsgAsyncStepCommon(aSharedDataCommon), iOperation(NULL)
	{
	SetTestStepName (KDeleteMessageSelection);
	}

/**
 Function : ConstructL
 Description : Calls the ConstrutL of CT_MsgAsyncStepCommon
 @return : N/A
 */
void CT_MsgDeleteMessageSelection::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL ();
	}

/**
 Function : ~CT_MsgDeleteMessageSelection
 Description : Destructor
 @return : N/A
 */
CT_MsgDeleteMessageSelection::~CT_MsgDeleteMessageSelection()
	{
	delete iOperation;
	iOperation = NULL;
	}

/**
 Function : doTestStepL
 Description : Get the entry list cntaining a selection of TMsvEntries and deletes few/all  
 			   corresponding messages.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgDeleteMessageSelection::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteMessageSelection"));
	
	// Read the number of message entries to be deleted fom the selection
	TInt numberOfEntries = -1;
	GetIntFromConfig(ConfigSection(), KNumberOfEntries, numberOfEntries);
	TInt countOfSelection = iSharedDataCommon.iSearchSortResultArray.Count();
	if(numberOfEntries < 0)
		{
		numberOfEntries = countOfSelection;
		}
	else if(numberOfEntries > countOfSelection)
		{
		ERR_PRINTF3(_L("Number of entries to be deleted = %d : Total count in selction = %d"), numberOfEntries, countOfSelection);
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("Number of entries to be deleted = %d"), numberOfEntries);
		CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);

		while(numberOfEntries)
			{
			TMsvId messageId = iSharedDataCommon.iSearchSortResultArray[numberOfEntries - 1].Id();
			INFO_PRINTF2(_L("The Message Id is %d"),messageId );
			entry->SetEntryL(iSharedDataCommon.iSearchSortResultArray[numberOfEntries - 1].Parent());
			CT_MsgActive&	active=Active();
			delete iOperation;
			iOperation = NULL;
			iOperation = entry->DeleteL(messageId, active.iStatus);
			active.Activate();
			CActiveScheduler::Start();
			User::LeaveIfError(active.Result());
			iSharedDataCommon.iSearchSortResultArray.Remove(numberOfEntries - 1);
			--numberOfEntries;
			}
		CleanupStack::PopAndDestroy(entry);
		}
	return TestStepResult();	
	}

/**
 Function : ProgressL
 Description : Provides the progress information of the asynchronous operation
 @param :	aFinal	Used to display the status of the asynchronous operation
 @return : none
 */
void CT_MsgDeleteMessageSelection::ProgressL(TBool aFinal)
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
void CT_MsgDeleteMessageSelection::CancelStep()
	{
	iOperation->Cancel();
	}
