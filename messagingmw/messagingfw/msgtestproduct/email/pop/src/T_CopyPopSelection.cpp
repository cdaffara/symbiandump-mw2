// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CopyPopSelection
// [Paramaters]
// PopAccountName				:	Name of the Pop account
// TargetFolderName				:   Name of the target folder to which the entries need to be copied 				
// Copies the entries to the target.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::CopyL
// 
//

/**
 @file
*/

// User includes
#include "T_CopyPopSelection.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include "T_MsgSharedDataPop.h"


// Literals Used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KTargetFolderName,"TargetFolderName");
_LIT(KMove, "Move");
_LIT(KCopyWithinService, "CopyWithinService");

/**
CT_MsgCopyPopSelection()
Sets the teststep name to KCopyPopSelection

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgCopyPopSelection::CT_MsgCopyPopSelection(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP) 
,	iOperation(NULL)
	{
	SetTestStepName(KCopyPopSelection);
	}

/**
NewL()
Allocates and creates a new CT_MsgCopyPopSelection object
  
@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgComparePopEmailMsgs
*/
CT_MsgCopyPopSelection* CT_MsgCopyPopSelection::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgCopyPopSelection* self = new(ELeave) CT_MsgCopyPopSelection(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_MsgCopyPopSelection()
Destructor
*/
CT_MsgCopyPopSelection::~CT_MsgCopyPopSelection()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
ProgressL()

@param
*/
void CT_MsgCopyPopSelection::ProgressL(TBool bFinal)
	{
	//	TODO
	if (bFinal)
		{
		//	Display final progress iOperation
		}
	else
		{
		//	Display current progress iOperation
		}
	}


/**
CancelStep()

*/
void CT_MsgCopyPopSelection::CancelStep()
	{
	iOperation->Cancel();
	}


/**
doTestStepL()
A connection with the POP server is established in the doTestStepPreambleL of the 
base class. Reads the Pop account name from the ini file. It obtains a selection of
the entires under POP account given. It copies these entries into the specified target folder.
 
@return
Returns the test step result
*/
TVerdict CT_MsgCopyPopSelection::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep : CopyPopSelection"));
	// Read the POP account name from the ini file
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Pop Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Retrieving the Pop service Id for the given Pop account
		TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
		INFO_PRINTF2(_L("Pop service id is %d"),popServiceId);

		if(popServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid POP account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			TMsvId targetFolderId = KErrNotFound;
			TBool copyWithinService = EFalse;
			GetBoolFromConfig(ConfigSection(), KCopyWithinService, copyWithinService);

			if (!copyWithinService)
				{
				TPtrC targetFolderName;
				if(!GetStringFromConfig(ConfigSection(), KTargetFolderName, targetFolderName))
					{
					ERR_PRINTF1(_L("Target Folder is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					// Retrieves the folder Id based on the local folder name read from the ini file
					targetFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(targetFolderName);
					if(targetFolderId == KErrNotFound)
						{
						ERR_PRINTF1(_L("Invalid local folder name specified"));
						SetTestStepResult(EFail);
						}
					}
				}

			if (copyWithinService || targetFolderId != KErrNotFound)
				{
				// Get a selection of the children entries for the Pop account
				CMsvEntrySelection* selection =  CT_MsgUtils::CreateChildrenSelectionL(iSharedDataPOP.iSession,
																			popServiceId);
				CleanupStack::PushL(selection);
					
				// Check if there are no entries under the remote folder 
				TInt count = selection->Count();
				if (count == 0)
					{
					User::Leave(KErrArgument);
					}
				// Fetch the Id of the first entry
				TMsvId entryId = (*selection)[0];

				// Creates a new CMsvEntry for the specified entry ID.
				CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataPOP.iSession,entryId,TMsvSelectionOrdering());
				CleanupStack::PushL(entry);
						
				// Sets the context to the parent of the specified entry.
				entry->SetEntryL(entryId);
				entry->SetEntryL(entry->Entry().Parent());

				TBool isMove = EFalse;
				if(GetBoolFromConfig(ConfigSection(), KMove, isMove))
					{
					INFO_PRINTF1(_L("Is move"));
					}

				CT_MsgActive&	active=Active();

				if (copyWithinService)
					{
					if (isMove)
						{
						iOperation = entry->MoveL(*selection,popServiceId, active.iStatus);
						}
					else
						{
						iOperation = entry->CopyL(*selection,popServiceId, active.iStatus);						
						}
					}
				else
					{
					if (isMove)
						{
						iOperation = entry->MoveL(*selection,targetFolderId, active.iStatus);
						}
					else
						{
						iOperation = entry->CopyL(*selection,targetFolderId, active.iStatus);						
						}
					}

				active.Activate();
				CActiveScheduler::Start();

				// Get the error code from TPop3Progress
				TPop3Progress temp;
				TPckgC<TPop3Progress> paramPack(temp);
				paramPack.Set(iOperation->ProgressL());
				TPop3Progress progress=paramPack();
				SetTestStepError(progress.iErrorCode);

				delete iOperation;
				iOperation=NULL;
				CleanupStack::PopAndDestroy(2,selection); //operation,entry,selection
				User::LeaveIfError(active.Result());
				}		
			}
		}
	return TestStepResult();
	}
