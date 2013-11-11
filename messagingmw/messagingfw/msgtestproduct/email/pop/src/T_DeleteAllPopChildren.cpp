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
// DeleteAllPopChildren
// [Paramaters]
// PopAccountName			:			Name of the Pop Account
// [Action Description]
// Deletes all children entries for the remote Pop account
// [APIs Used]
// 
//

/**
 @file
*/


//User Includes
#include "T_DeleteAllPopChildren.h"
#include <t_utilsdeleteallchildren.h>
#include <t_utilscentralrepository.h>
#include "T_MsgSharedDataPop.h"


//epoc includes
#include <barsc.h>
#include <bautils.h>
#include <msgs.rsg>


// Literals Used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KBlockDelete, "BlockDelete");


/**
CT_MsgDeleteAllPopChildren()
Sets the teststep name to KDeleteAllPopChildren

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgDeleteAllPopChildren::CT_MsgDeleteAllPopChildren(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP)
,	iUtil(NULL)
	{
	SetTestStepName(KDeleteAllPopChildren);
	}


/**
NewL()
Allocates and creates a new CT_MsgDeleteAllPopChildren object

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@return
Returns pointer to an object of CT_MsgDeleteAllPopChildren
*/
CT_MsgDeleteAllPopChildren* CT_MsgDeleteAllPopChildren::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgDeleteAllPopChildren* self = new(ELeave) CT_MsgDeleteAllPopChildren(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_MsgDeleteAllPopChildren()
Destructor
*/
CT_MsgDeleteAllPopChildren::~CT_MsgDeleteAllPopChildren()
	{
	if(iUtil!=NULL)
		{
		delete iUtil;
		iUtil=NULL;
		}
	}


/**
ProgressL()

@param
*/
void CT_MsgDeleteAllPopChildren::ProgressL(TBool bFinal)
	{
	//	TODO
	if (bFinal)
		{
		//	Display final progress iUtil
		}
	else
		{
		//	Display current progress iUtil
		}
	}


/**
CancelStep()

@param
*/
void CT_MsgDeleteAllPopChildren::CancelStep()
	{
	//	TODO cancel iUtil
	}


/**
doTestStepL()
Reads the Pop account name from the ini file. It obtains a selection of the 
enries under the account and calls theT_UtilsDeleteAllChildren to delete them

@return
Returns the test step result
*/
TVerdict CT_MsgDeleteAllPopChildren::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteAllChildren "));
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
			if(iUtil==NULL)
				{
				iUtil = new (ELeave) CT_MsgUtilsDeleteAllChildren(iSharedDataPOP.iSession,popServiceId);
				}

			CT_MsgActive&	active=Active();
			// Deletes all the entries under the remote folder

			TBool blockDelete = EFalse;
			GetBoolFromConfig(ConfigSection(), KBlockDelete, blockDelete);
			if (blockDelete)
				{
				INFO_PRINTF1(_L("Using block delete"));
				}

			iUtil->StartL(blockDelete, active.iStatus);
			active.Activate();
			CActiveScheduler::Start();
			delete iUtil;
			iUtil=NULL;
			
			TInt err = active.Result();
			if (err != KErrNone)
				{
				SetTestStepResult(EFail);
				SetTestStepError(err);
				}
			}
		}
	return TestStepResult();
	}
