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
// ChangeImap4SubAndSyncSettings
// [Paramaters]
// ImapAccountName			:Name of the Imap account
// ImapSubscribeMethod		:Sets the method for synchronising IMAP4 subscription information with a server.
// ImapSynchroniseMethod	:Sets the method for synchronising folder information with the server.
// Change the IMAP4 Subscribe and Synchronise settings of a test case through the .ini file
// 
//

//User includes
#include "T_ChangeImap4SubAndSyncSettings.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapSubscribeMethod,"ImapSubscribeMethod");
_LIT(KImapSynchroniseMethod,"ImapSynchroniseMethod");

/**
  Function : CT_MsgChangeImap4SubAndSyncSettings
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgChangeImap4SubAndSyncSettings::CT_MsgChangeImap4SubAndSyncSettings(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KChangeImap4SubAndSyncSettings);
	}
	
/**
  Function : ~CT_MsgChangeImap4SubAndSyncSettings
  Description : Destructor
  @return : N/A
*/
CT_MsgChangeImap4SubAndSyncSettings::~CT_MsgChangeImap4SubAndSyncSettings()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgChangeImap4SubAndSyncSettings 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgChangeImap4SubAndSyncSettings
*/	
CT_MsgChangeImap4SubAndSyncSettings* CT_MsgChangeImap4SubAndSyncSettings::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgChangeImap4SubAndSyncSettings* self = new(ELeave) CT_MsgChangeImap4SubAndSyncSettings(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgChangeImap4SubAndSyncSettings::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapSubscribeMethod, iImapSubscribeMethod))
		{
		ERR_PRINTF1(_L("Imap subscribe method is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapSynchroniseMethod, iImapSynchroniseMethod))
		{
		ERR_PRINTF1(_L("Imap synchronise method is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Change the IMAP4 Subscribe and Synchronise settings of a test case through the .ini file
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgChangeImap4SubAndSyncSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ChangeImap4SubAndSyncSettings"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
			
		CImImap4Settings *imapSettings = new (ELeave) CImImap4Settings;
		CleanupStack::PushL(imapSettings);
				
		TImapAccount imapAccountId;
		CT_MsgUtilsCentralRepository::GetImapAccountL((TDes&)iImapAccountName,imapAccountId);
				
		accounts->LoadImapSettingsL(imapAccountId,*imapSettings);
			
		TFolderSubscribeType setFolderSubscribeType = CT_MsgUtilsEnumConverter::ConvertDesToTFolderSubscribeType(iImapSubscribeMethod);
		imapSettings->SetSubscribe(setFolderSubscribeType);
		TFolderSyncType setFolderSyncType = CT_MsgUtilsEnumConverter::ConvertDesToTFolderSyncType(iImapSynchroniseMethod);
		imapSettings->SetSynchronise(setFolderSyncType);
				
		accounts->SaveImapSettingsL(imapAccountId,*imapSettings);
	
		CleanupStack::PopAndDestroy(2,accounts); // imapSettings,accounts
		}
	return TestStepResult();
	}
	
/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgChangeImap4SubAndSyncSettings::ProgressL(TBool bFinal)
	{
	//	Stub
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation */
		}
	}

/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : void
*/
void CT_MsgChangeImap4SubAndSyncSettings::CancelStep()
	{
	iOperation->Cancel();
	}
