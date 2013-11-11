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
// [TestStep Name]
// RemoveImapSNAPSetting
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// Removes the provisioned SNAP information for the IMAP account.
// 
//

//User includes
#include "T_RemoveImapSNAPSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literal used
_LIT(KImapAccountName,"ImapAccountName");

/**
  Function : CT_MsgRemoveImapSNAPSetting
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgRemoveImapSNAPSetting::CT_MsgRemoveImapSNAPSetting(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KRemoveImapSNAPSetting);
	}
	
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgRemoveImapSNAPSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Removes the provisioned SNAP information from the IMAP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgRemoveImapSNAPSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : RemoveImapSNAPSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImIAPPreferences* imapIapPrefs = CImIAPPreferences::NewLC();
					
		TImapAccount imapAccountId;
		CT_MsgUtilsCentralRepository::GetImapAccountL((TDes&)iImapAccountName,imapAccountId);
				
		accounts->LoadImapIapSettingsL(imapAccountId, *imapIapPrefs);	
		
		imapIapPrefs->RemoveSNAP();
		
		accounts->SaveImapIapSettingsL(imapAccountId, *imapIapPrefs);
	
		CleanupStack::PopAndDestroy(2,accounts); // imapIapPrefs, accounts
		}
	return TestStepResult();
	}
