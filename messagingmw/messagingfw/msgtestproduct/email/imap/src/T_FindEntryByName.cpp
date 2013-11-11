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
// FindEntryByName
// [Paramaters]
// ImapAccountName				:	Name of the Imap account
// FolderName					:   Name of the remote parent folder of the entry to be searched
// EntryName					:	Name of the entry which needs to be searched
// Finds and returns a specified entry.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::CopyL
// 
//

/* User includes*/
#include "T_FindEntryByName.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>


/* Epoc includes */
#include <msvids.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderName,"FolderName");
_LIT(KEntryName,"EntryName");
_LIT(KCaseSensitive,"CaseSensitive");


/**
  Function : CT_MsgFindEntryByName
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgFindEntryByName::CT_MsgFindEntryByName(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KFindEntryByName);
	}

	
/**
  Function : ~CT_MsgFindEntryByName
  Description : Destructor
  @return : N/A
*/
CT_MsgFindEntryByName::~CT_MsgFindEntryByName()
	{
	}


/**
  Function : doTestStepL
  Description : Reads the Imap account name, folder name ,the entry name from the ini file.
				It the then searches for the given entry under the specified folder.
				The test passes if the entry is found else it fail.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgFindEntryByName::doTestStepL()
	{
	INFO_PRINTF1(_L(" TestStep :  FindEntryByName"));
	/* Reads the IMAP account name from the ini file */
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the IMAP service Id for the given IMAP account */
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId);

		/* Reads the name of the remote folder from the ini file */
		TPtrC folderName;
		if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
			{
			ERR_PRINTF1(_L("Remote Parent Folder name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/* Reads the entry name that is to be verified from the ini file */
			TPtrC entryName;
			if(!GetStringFromConfig( ConfigSection(), KEntryName, entryName))
				{
				ERR_PRINTF1(_L("Entry name to search is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TBool caseSensitive = EFalse;
				GetBoolFromConfig(ConfigSection(), KCaseSensitive, caseSensitive);

				/* Gets the Id of the given entry to be searched */
				TMsvId  entryId;
				TRAPD(err,entryId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,entryName,folderName,caseSensitive));
				INFO_PRINTF2(_L(" The entry Id is %d"),entryId);
				SetTestStepError(err);
				}
			}
		}		
	return TestStepResult();
	}
