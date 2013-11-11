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
// SetImap4PartialDownloadLimits
// [Paramaters]
// ImapAccountName			: Name of the IMAP account 
// FolderName				: Destnation folder name 
// MaxEmailSize				: Maximum size limit for the total message 
// Default value is KMaxTInt
// BodyTextSizeLimit    	: Maximum size limit for body of the message 
// Default value is KMaxTInt
// AttachmentSizeLimit  	: Maximum size limit for the attachments 
// Default value is KMaxTInt
// PartialMailSizeOptions 	: Enumerated values defined in 
// TImImap4PartialMailOptions
// Default value is ENoSizeLimits
// [TestStep Description]
// Set the limits for partial downloading.
// 
//

/* User includes */
#include "T_SetImap4PartialDownloadLimits.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <t_utils.h>


/* Epoc includes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <msvapi.h>
#include <msvstd.h>


/* Literals used */
_LIT(KImapAccountName, "ImapAccountName");
_LIT(KFolderName, "FolderName");
_LIT(KMaxEmailSize, "MaxEmailSize");
_LIT(KBodyTextSizeLimit, "BodyTextSizeLimit");
_LIT(KAttachmentSizeLimit, "AttachmentSizeLimit");
_LIT(KImapPartialMailOption, "ImapPartialMailOption");

_LIT(KInfoTestStepName, "Test Step: Set Imap4 Partial Download Limits");
_LIT(KAcntNtSpecified, "IMAP Account name not specified");
_LIT(KDestFolderNtSpecified, "Destination Folder Name is not specified");
_LIT(KPartialMailOptNtSpecified, "Partial Mail option is not specified");
_LIT(KAccountNtExist, "Account with name = %S does not exist");
_LIT(KFolderId, " The folder Id is %d");
_LIT(KInfoCompletionStatus, "Test Step %S completed.");



/**
  Function : CT_MsgSetImap4PartialDownloadLimits
  Description : Constructor
  @param	:	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgSetImap4PartialDownloadLimits::CT_MsgSetImap4PartialDownloadLimits(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KSetImap4PartialDownloadLimits);
	}

	
/**
  Function : ~CT_MsgSetImap4PartialDownloadLimits
  Description : Destructor
  @return : N/A
*/
CT_MsgSetImap4PartialDownloadLimits::~CT_MsgSetImap4PartialDownloadLimits()
	{
	}


/**
  Function : doTestStepL
  Description : Fetches the srvice ID using the IMAP account name and sets
  				the partial download limit fields of the setting object corresponding to it. 
  @return : TVerdict - Test step result
  @leave  :	KMsvNullIndexEntryId	If valid IMAP account name is not specified or
									If valid remote folder name is not specified
*/
TVerdict CT_MsgSetImap4PartialDownloadLimits::doTestStepL()
	{
	INFO_PRINTF1(KInfoTestStepName);
	
	/* Reads IMAP Account Name from ini file*/
	TPtrC imapAccountName;
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(KAcntNtSpecified);
		SetTestStepResult(EFail);
		}
	else
		{
		TPtrC destFolderName;
		/* Reads the destination Folder Name to where the mails need to be populated*/
		if(!GetStringFromConfig(ConfigSection(), KFolderName, destFolderName))
			{
			ERR_PRINTF1(KDestFolderNtSpecified);
			SetTestStepResult(EFail);
			}
		/* 
		 * Reads the maximum email size, body test size limit, attachment size limt 
		 * from the ini. If these options are not specified, the default values are taken
		 */
		else
			{
			/* Default values */
			TInt maxEmailSize = KMaxTInt;
			TInt bodyTextSizeLimit = KMaxTInt;
			TInt attachmentSizeLimit = KMaxTInt;
			
			/* Reads size of the Email, default is KMaxTInt */
			GetIntFromConfig(ConfigSection(), KMaxEmailSize, maxEmailSize);
		
			/* Reads size of the Body Text, default is KMaxTInt */
			GetIntFromConfig(ConfigSection(), KBodyTextSizeLimit, bodyTextSizeLimit);
			
			/* Reads size of the Body Text, default is KMaxTInt */
			GetIntFromConfig(ConfigSection(), KAttachmentSizeLimit, attachmentSizeLimit);
	
			TPtrC partialMailOption;		
			/* Reads the partial mail download options */
			if(!GetStringFromConfig(ConfigSection(), KImapPartialMailOption, partialMailOption))
				{
				ERR_PRINTF1(KPartialMailOptNtSpecified);
				SetTestStepResult(EFail);
				}
			
			/* Retrieves the IMAP service Id */
			else
				{
				TMsvId destFolderId = -1;
				TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes &) imapAccountName);
				if(imapServiceId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF2(KAccountNtExist, &imapAccountName);
					SetTestStepResult(EFail);
					}
				else
					{
					/* 
					 * Retrieves the partial mail options and the remote destination folder Id
					 * based on the IMAP account name and the remote folder name given
					 */
					TImImap4PartialMailOptions imapPartialMailOption = CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(partialMailOption);

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
						
						/* Stores the partial donwload limit values */
						TImImap4GetPartialMailInfo imap4GetPartialMailInfo;		
						imap4GetPartialMailInfo.iBodyTextSizeLimit   = bodyTextSizeLimit;
						imap4GetPartialMailInfo.iAttachmentSizeLimit = attachmentSizeLimit;
						imap4GetPartialMailInfo.iTotalSizeLimit      = maxEmailSize;
						imap4GetPartialMailInfo.iPartialMailOptions  = imapPartialMailOption;
						imap4GetPartialMailInfo.iDestinationFolder   = destFolderId;	
							
						/* Loads the Imap account settings and saves the download limits set */
						CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
						CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
						CleanupStack::PushL(imapSettings);

						TImapAccount imapAccount;
						emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
						emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);
							
						/* Sets the download limts in the settings object created */
						imapSettings->SetBodyTextSizeLimitL( imap4GetPartialMailInfo.iBodyTextSizeLimit );
						imapSettings->SetAttachmentSizeLimitL( imap4GetPartialMailInfo.iAttachmentSizeLimit );
						imapSettings->SetMaxEmailSize( imap4GetPartialMailInfo.iTotalSizeLimit );
						imapSettings->SetPartialMailOptionsL( imap4GetPartialMailInfo.iPartialMailOptions );		
						
						/* Saves the settings */
						emailAccounts->SaveImapSettingsL(imapAccount, *imapSettings);
						CleanupStack::PopAndDestroy( 2, emailAccounts );   /* imapSettings, emailSettings */
						
						INFO_PRINTF2(KInfoCompletionStatus, &KSetImap4PartialDownloadLimits);
						}
					}
				}
			}
		}
	return TestStepResult();
	}
				
			
		
	
	

