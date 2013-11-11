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
// CheckChildrenCountPop
// [Paramaters]
// PopAccountName			:		Name of the Pop account
// ExpectedChildrenCount	:		Expected number of children to be present in the
// folder
// Obtains a selection of the children entries under the Pop account and obtains its count.
// Checks if children count is as expected. If the count does not match, the test step
// fails.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// 
//

/**
 @file
*/


// User includes
#include "T_CheckChildrenCountPop.h"
#include <t_utils.h>
#include <t_utilscentralrepository.h>


//epoc includes
#include <msvids.h>


// Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KExpCount,"Expectedcount");


/**
CT_MsgCheckChildrenCountPop()
Sets the teststep name to KCheckChildrenCountPop

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgCheckChildrenCountPop::CT_MsgCheckChildrenCountPop(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KCheckChildrenCountPop);
	}


/**
~CT_MsgCheckChildrenCountPop()
Destructor
*/
CT_MsgCheckChildrenCountPop::~CT_MsgCheckChildrenCountPop()
	{
	}

	
/**
doTestStepL()
Reads the remote foldername and the expected count from the ini file.
Obtains the count of the children entries under the given folder.
Verifies whether the expected count read from the ini file 
matches with the actual count.Fails if the count does not match

@return
Returns the test step result
*/
TVerdict CT_MsgCheckChildrenCountPop::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step :CheckChildrenCountPop "));

	// Read the POP account name from the ini file
	TPtrC	popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("POP account name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Retrieving the POP service Id based on the account name read from ini
		TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&) popAccountName);
		if(popServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid Pop account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			TInt expectedChildrenCount;			
			// Read the expected children count from ini file
			if(!GetIntFromConfig(ConfigSection(), KExpCount, expectedChildrenCount))
				{
				ERR_PRINTF1(_L("Expected number of children in the POP account not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				// Create a selection of the children entries under the given folder and verify the count
				CMsvEntrySelection* childrenSelection = 
				CT_MsgUtils::CreateChildrenSelectionL(iSharedDataPOP.iSession, popServiceId);
				CleanupStack::PushL(childrenSelection);
		
				TInt actualCount = childrenSelection->Count();

				if (expectedChildrenCount != actualCount)
					{
					ERR_PRINTF3(_L("Children count is not equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF3(_L("Children count is equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
					}
				CleanupStack::PopAndDestroy(childrenSelection);
				}
			}
		}
	return TestStepResult();
	}
