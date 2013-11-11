// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ChangeImap4FolderSyncSettings
// [Paramaters]
// ImapAccountName			:Name of the Imap account
// FolderSyncFlag			:Folder sync flag can be set to ETrue or EFalse
// ETrue -  to disable the folder sync 
// EFalse - to enable the folder sync, while syncronisation
// Change the IMAP4 folder sync setting of a test case through the .ini file
// 
//

//User includes
#include "T_ChangeImap4FolderSyncSettings.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderSyncFlag,"FolderSyncFlag");

/**
  Function		: CT_MsgChangeImap4FolderSyncSettings
  Description	: Constructor 
  @param		: aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return		: N/A
*/
CT_MsgChangeImap4FolderSyncSettings::CT_MsgChangeImap4FolderSyncSettings(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KChangeImap4FolderSyncSettings);
	}
	
/**
  Function		: ~CT_MsgChangeImap4FolderSyncSettings
  Description	: Destructor
  @return		: N/A
*/
CT_MsgChangeImap4FolderSyncSettings::~CT_MsgChangeImap4FolderSyncSettings()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function		: NewL
  Description	: Creates an object of CT_MsgChangeImap4FolderSyncSettings 				 
  @param		: aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return		: Pointer to CT_MsgChangeImap4FolderSyncSettings
*/	
CT_MsgChangeImap4FolderSyncSettings* CT_MsgChangeImap4FolderSyncSettings::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgChangeImap4FolderSyncSettings* self = new(ELeave) CT_MsgChangeImap4FolderSyncSettings(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function		: ReadIni
  Description	: Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return		: TBool
*/
TBool CT_MsgChangeImap4FolderSyncSettings::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	else if(!GetBoolFromConfig(ConfigSection(), KFolderSyncFlag, iFolderSyncFlag))
		{
		ERR_PRINTF1(_L("Folder sync flag is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function		: doTestStepL
  Description	: Change the IMAP4 folder sync settings of a test case through the .ini file
  @return		: TVerdict - Test step result
*/		
TVerdict CT_MsgChangeImap4FolderSyncSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ChangeImap4FolderSyncSettings"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
			
		CImImap4Settings *imapSettings = new (ELeave) CImImap4Settings;
		CleanupStack::PushL(imapSettings);
				
		TImapAccount imapAccountId;
		CT_MsgUtilsCentralRepository::GetImapAccountL((TDes&)iImapAccountName,imapAccountId);
				
		accounts->LoadImapSettingsL(imapAccountId,*imapSettings);
		imapSettings->SetFolderSyncDisabled(iFolderSyncFlag);
		accounts->SaveImapSettingsL(imapAccountId,*imapSettings);
	
		CleanupStack::PopAndDestroy(2,accounts); // imapSettings,accounts
		}
	return TestStepResult();
	}
	
/**
  Function		: ProgressL
  Description	: Displays the progress information of the asynchronous operation
  @return		: void
*/
void CT_MsgChangeImap4FolderSyncSettings::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function		: CancelStep
  Description	: Cancels the asynchronous operation
  @return		: void
*/
void CT_MsgChangeImap4FolderSyncSettings::CancelStep()
	{
	iOperation->Cancel();
	}
