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
// SetUnRead
// [Paramaters]
// ImapAccountName		:	Name of the IMAP account
// FolderName			:	Name of the remote folder
// Subject				:   The  subject of the message which needs to be set.
// FlagUpdate			:   The value for the message Unread flag.	
// Sets the message unread flag.
// 
//


/* User includes */
#include "T_SetUnReadFlag.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

/* Literals Used */
_LIT(KFolderName,"FolderName");
_LIT(KSubject,"Subject");
_LIT(KFlagUpdate,"FlagUpdate");
_LIT(KImapAccountName,"ImapAccountName");


/**
  Function : CT_MsgSetUnReadFlag
  Description : Constructor
  @param : aSharedDataIMAP	A reference to CT_MsgSharedDataImap
*/
CT_MsgSetUnReadFlag::CT_MsgSetUnReadFlag(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KSetUnReadFlag);
	}


/**
  Function 		: doTestStepL
  Description 	: Reads the IMAP account name, folder name , subject and flagupdate from the ini file.
  				  For the message whose subject is mentioned in the ini file,
  				  sets the Unread flag.
  @return 		: TVerdict - Test step result
*/
TVerdict CT_MsgSetUnReadFlag::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: SetUnRead"));
	/* Reads the folder name from ini file */
	TPtrC	folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Reads the IMAP account name from the ini file */
		TPtrC	imapAccountName;
		if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
			{
			ERR_PRINTF1(_L("Imap account name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/* Gets the Id of the remote folder based on the name specified */
			TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
 						
			/* Gets the Id of the remote folder */
			TMsvId folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,folderName);	
			INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

			/* If the folder Id is KErrNotFound,then the folder name is invalid */
			if ( folderId == KErrNotFound )
				{
				ERR_PRINTF1(_L("Invalid folder name specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TBool flagUpdate;
				if( !GetBoolFromConfig(ConfigSection(), KFlagUpdate, flagUpdate ))
					{
					ERR_PRINTF1(_L("The update for the UnRead flag not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					TPtrC	subject;	
					if( !GetStringFromConfig(ConfigSection(), KSubject, subject ))
						{
						ERR_PRINTF1(_L("The subject of the message is not specified"));
						SetTestStepResult(EFail);
						}
					else
						{
						/* Retrieves the message Id based ont he message subject from the given local folder */
						TMsvId	messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataIMAP.iSession, folderId , subject);
						if( messageId == KMsvNullIndexEntryId)
							{
							ERR_PRINTF1(_L("The given message is not found"));
							SetTestStepResult(EFail);
							}
						 /* Message found */
						else
							{
							INFO_PRINTF2(_L("The Message Id is %d"),messageId );
						
							/* Setting the current context to the parent of the mesage */
							CMsvEntry*	msgEntry = CMsvEntry::NewL(*iSharedDataIMAP.iSession, messageId , TMsvSelectionOrdering());
							CleanupStack::PushL(msgEntry);
							
							msgEntry->SetEntryL(messageId);
							
							TMsvEmailEntry entry = msgEntry->Entry();		
							
							/* Sets the message Unread flag */
							entry.SetUnread(flagUpdate);
							CleanupStack::PopAndDestroy(msgEntry);
							}	
						}	
					}	
				}
			}
		}
	return TestStepResult();
	}
