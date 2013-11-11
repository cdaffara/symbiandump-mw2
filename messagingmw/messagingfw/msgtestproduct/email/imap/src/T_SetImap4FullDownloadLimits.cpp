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
// SetImap4FullDownloadLimits
// [Paramaters]
// ImapAccountName			: Name of the IMAP account 
// FolderName				: Destination folder name 
// MaxEmailSize				: Value of the total size limit
// Default value is KMaxTInt
// ImapMailOption			: Enumerated values defined in 
// TImap4GetMailOptions
// Default value is EGetImap4EmailHeaders
// [Optional Parameters]
// DeleteFlag 			:  Checks whether the caller has enabled messages marked to be deleted 
// while offline to be expunged from the server when disconnecting from the next online session. 
// UpdateFlag 			:	Retrieves whether the IMAP4 seen flag of new messages will be set on the server only when 
// the message has been marked as read, or when the message has been synchronised.
// FetchSizeLimit		:	The IMAP fetch size.
// InboxSyncLimit		:	Specifies whether or not the IMAP4 service will synchronise the Inbox folder after
// connecting, or the maximum number of new messages that will be synchronised if there
// are new messages on the server.
// MailboxSyncLimit	:	Specifies whether or not the IMAP4 service will synchronise folders other than the
// Inbox folder after connecting, or the maximum number of messages that will be
// synchronised in the folders.
// FolderSubType		:	Sets the method for synchronising IMAP4 subscription information with a server
// FolderSyncType		:	Sets the method for synchronising folder information with the server.
// SearchString		:	Specifies the search string.
// [TestStep Description]
// Sets the limits for full downloading mails.
// [APIs Used]
// 
//



/* User includes */
#include "T_SetImap4FullDownloadLimits.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <t_utils.h>


/* epoc includes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <msvapi.h>
#include <msvstd.h>



/* Literals used */
_LIT(KImapAccountName, "ImapAccountName");
_LIT(KFolderName, "FolderName");
_LIT(KMaxEmailSize, "MaxEmailSize");
_LIT(KImapMailOption, "ImapMailOption");
_LIT(KDeleteFlag,"DeleteFlag");
_LIT(KUpdateFlag,"UpdateFlag");
_LIT(KFetchSizeLimit,"FetchSizeLimit");
_LIT(KInboxSyncLimit,"InboxSyncLimit");
_LIT(KMailboxSyncLimit,"MailboxSyncLimit");
_LIT(KFolderSubType,"FolderSubType");
_LIT(KFolderSyncType,"FolderSyncType");
_LIT(KSearchString,"SearchString");


_LIT(KInfoTestStepName, "Test Step: Set Imap4 Full Download Limits");
_LIT(KAcntNameNtSpecified, "IMAP Account name not specified");
_LIT(KDestFolderNtSpecified, "Destination Folder Name is not specified");
_LIT(KGetMailOptNtSpecified, "Get mail option is not specified");
_LIT(KAcntNtExist, "Account with name = %S does not exist");
_LIT(KFolderId, " The folder Id is %d");
_LIT(KInfoCompletionStatus, "Test Step %S completed.");



/**
  Function : CT_MsgSetImap4FullDownloadLimits
  Description : Constructor
  @param	:	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgSetImap4FullDownloadLimits::CT_MsgSetImap4FullDownloadLimits(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KSetImap4FullDownloadLimits);
	}


/**
  Function : ~CT_MsgSetImap4FullDownloadLimits
  Description : Destructor
  @return : N/A
*/
CT_MsgSetImap4FullDownloadLimits::~CT_MsgSetImap4FullDownloadLimits()
	{
	}


/**
  Function : doTestStepL
  Description : Fetches the srvice ID using the IMAP account name and sets
  				the full download limit fields of the setting object corresponding to it. 
  @return : TVerdict - Test step result
  @leave  :	KMsvNullIndexEntryId	If valid IMAP account name is not specified or
									If valid remote folder name is not specified
*/
TVerdict CT_MsgSetImap4FullDownloadLimits::doTestStepL()
	{
	INFO_PRINTF1(KInfoTestStepName);
	TPtrC imapAccountName;
	
	/* Reads IMAP Account Name from ini file */
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(KAcntNameNtSpecified);
		SetTestStepResult(EFail);
		}		
	else
		{
		TPtrC destFolderName;

		/* Reads the destination Folder Name */
		if(!GetStringFromConfig(ConfigSection(), KFolderName, destFolderName))
			{
			ERR_PRINTF1(KDestFolderNtSpecified);
			SetTestStepResult(EFail);
			}	
		else
			{
			TInt maxEmailSize = KMaxTInt;
			/* Reads the size of the Email to be set,where default is KMaxTInt */
			GetIntFromConfig(ConfigSection(), KMaxEmailSize, maxEmailSize);
			
			TPtrC mailOption;
			/* Reads the mail option to be set from the ini file */
			if(!GetStringFromConfig(ConfigSection(), KImapMailOption, mailOption))
				{
				ERR_PRINTF1(KGetMailOptNtSpecified);
				SetTestStepResult(EFail);
				}
			else
				{
				TMsvId destFolderId = -1;
				/* Retrieves the IMAP service Id based on the IMAP account name*/
				TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&) imapAccountName);
				if(imapServiceId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF2(KAcntNtExist, &imapAccountName);
					SetTestStepResult(EFail);
					}
				else
					{
					/* 
					 * Retrieves the mail options and the remote destination folder Id
					 * based on the IMAP account name and the remote folder name given
					 */
					TImap4GetMailOptions imapMailOption = CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(mailOption);
		
					destFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
												imapAccountName,destFolderName);
			
					if(destFolderId == KMsvNullIndexEntryId)
						{
						ERR_PRINTF1(_L("Invalid remote folder name specified"));
						SetTestStepResult(EFail);
						}
					else
						{																						
						INFO_PRINTF2(KFolderId,destFolderId);		
						
						/* Stores the fulll donwload limit values */
						TImImap4GetMailInfo  imap4GetMailInfo;
						imap4GetMailInfo.iMaxEmailSize      = maxEmailSize;	
						imap4GetMailInfo.iGetMailBodyParts  = imapMailOption;
						imap4GetMailInfo.iDestinationFolder = destFolderId;
						
						/* Loads the Imap account settings and save the download limits */
						CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
						CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
						CleanupStack::PushL(imapSettings);

						TImapAccount imapAccount;
						emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
						emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);
						
						
						TBool deleteFlag;
						if(GetBoolFromConfig(ConfigSection(),KDeleteFlag,deleteFlag))	
							{
							imapSettings->SetDeleteEmailsWhenDisconnecting(deleteFlag);
							}
						TBool updateFlag;
						if(GetBoolFromConfig(ConfigSection(),KUpdateFlag,updateFlag))	
							{
							imapSettings->SetUpdatingSeenFlags(updateFlag);
							}
							
						TInt fetchSizeLimit = 20480;
						if(GetIntFromConfig(ConfigSection(),KFetchSizeLimit,fetchSizeLimit))
							{
							imapSettings->SetFetchSizeL(fetchSizeLimit);		
							}
							
						TInt inboxSyncLimit;
						if(GetIntFromConfig(ConfigSection(),KInboxSyncLimit,inboxSyncLimit))
							{
							imapSettings->SetInboxSynchronisationLimit(inboxSyncLimit);
							}
						
						TInt mailboxSyncLimit;
						if(GetIntFromConfig(ConfigSection(),KMailboxSyncLimit,mailboxSyncLimit))
							{
							imapSettings->SetMailboxSynchronisationLimit(mailboxSyncLimit);
							}
						
						TPtrC searchString;
						if(GetStringFromConfig(ConfigSection(),KSearchString,searchString))
							{
							HBufC8* string = HBufC8::NewLC(searchString.Length());
							string->Des().Copy(searchString);
							imapSettings->SetSearchStringL(string->Des());
							CleanupStack::PopAndDestroy(string);
							}
						
						TPtrC folderSubTypeName;
						if(GetStringFromConfig(ConfigSection(),KFolderSubType,folderSubTypeName))
							{
							TFolderSubscribeType folderSubType =  CT_MsgUtilsEnumConverter::ConvertDesToTFolderSubscribeType
												((TDesC&)folderSubTypeName);
							imapSettings->SetSubscribe(folderSubType);
							}
							
						TPtrC folderSyncTypeName;
						if(GetStringFromConfig(ConfigSection(),KFolderSyncType,folderSyncTypeName))
							{
							TFolderSyncType  folderSyncType = CT_MsgUtilsEnumConverter::ConvertDesToTFolderSyncType
												((TDesC&)folderSyncTypeName);
							imapSettings->SetSynchronise(folderSyncType);
							}
										
						/* Sets the download limts in the settings object created */
						imapSettings->SetMaxEmailSize( imap4GetMailInfo.iMaxEmailSize );
						imapSettings->SetGetMailOptions( imap4GetMailInfo.iGetMailBodyParts );
						
						/* Saves the settings */
						emailAccounts->SaveImapSettingsL(imapAccount, *imapSettings);
						
						CleanupStack::PopAndDestroy(2,emailAccounts);/* imapSettings, emailAccounts */
						
						INFO_PRINTF2(KInfoCompletionStatus, &KSetImap4FullDownloadLimits);
						}
					}
				}
			}
		}
	return TestStepResult();
	}
			
	
	

