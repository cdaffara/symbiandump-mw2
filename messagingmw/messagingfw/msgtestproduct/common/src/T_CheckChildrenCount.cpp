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
// @file 
// [TestStep Name]
// CheckChildrenCount
// [Paramaters]
// ParentFolder					: Parent folder name
// Foldername					: Folder in which children count is to be verified
// Expectedcount				: Expected number of children to be present in the
// folder
// [Action Description]
// Checks if children count is as expected. If the count does not match, the test step
// fails.NOTE: The current implementation supports checking the children count for only
// the local folders.It needs to be enhanced to check the children count for remote 
// folders
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// 
//


/* User includes */
#include "T_CheckChildrenCount.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>

/* Literals used */
_LIT(KExpCount,		"Expectedcount");
_LIT(KFolderName,	"FolderName");
_LIT(KParentFolderName,	"ParentFolderName");


/**
  Function : CT_MsgCheckChildrenCount
  Description : Constructor
  @param :
  @return : N/A
*/
CT_MsgCheckChildrenCount::CT_MsgCheckChildrenCount(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KCheckChildrenCount);
	}



/**
  Function    : doTestStepL
  Description : Reads the foldername and the expected count from the ini file.
				Obtains the count of the children entries under the given folder.
				Verifies whether the expected count read from the ini file 
				matches with the actual count.
  @return     : TVerdict - Test step result
			    EPass, if the count matches
			    EFail, if the count does not match
  @leave  : KErrNotFound	Invalid local folder name specified
*/
TVerdict CT_MsgCheckChildrenCount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : Check children count"));

	TInt expectedChildrenCount;
	/* Reads the expected children count from ini file */
	if(!GetIntFromConfig( ConfigSection(), KExpCount, expectedChildrenCount))
		{
		ERR_PRINTF1(_L("Expected number of children not specified"));
		SetTestStepResult(EFail);
		}		
	else
		{
		/* 
		 * Reads the folder name whose entries need to be matched against 
		 * the expected count 
		 */
		TPtrC folderName;
		if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
			{
			ERR_PRINTF1(_L("Folder is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			TMsvId	folderId = NULL;
			TPtrC parentFolderName;
			if(GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
				{
					TInt err = KErrNone;
					// Retrives the Id of the local folder
					TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataCommon.iSession, KMsvLocalServiceIndexEntryId,folderName,parentFolderName));
				}
				else
				{
					/* Retrieves the folder Id based on the folder name specified */
					folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(folderName);
				}
			
			/* 
			 * If the folder Id is KErrNotFound ,then the folder name is invalid
			 * The test step fails 
			 */
			if( folderId == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid local folder name specified"));
				SetTestStepResult(EFail);
				}				
			/* 
			 * If a valid folder name is specified then it verifies the children count
			 */
			else
				{
				/* Creates a selection of the children entries under the specified folder 
				  and obtains the count */
				CMsvEntrySelection*	childrenSelection = 
					CT_MsgUtils::CreateChildrenSelectionL(iSharedDataCommon.iSession, folderId);
				CleanupStack::PushL(childrenSelection);

				TInt actualCount = childrenSelection->Count();

				/* 
				 * Matches the expected count with the actual count 
				 * If the count does not match, fails the test
				 */
				if ( expectedChildrenCount != actualCount )
					{
					ERR_PRINTF3(_L("Children count is not equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
					SetTestStepResult(EFail);
					}
				/* If the count matches the expected count then the test step passes */
				else
					{
					INFO_PRINTF3(_L("Children count is equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
					}
				CleanupStack::PopAndDestroy(childrenSelection);  /*childrenSelection*/
				}
			}
		}
	return TestStepResult();
	}
