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
// CheckImapBMSetting
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// ExpectedBMSupport		:ETrue => Bearer mobility is expected to be supported by the IMAP service.
// EFalse => Bearer mobility is not expected to be supported by the IMAP service.
// Checks if bearer mobility is supported by the IMAP service.
// 
//

//User includes
#include "T_CheckImapBMSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedBMSupport,"ExpectedBMSupport");

/**
  Function : CT_MsgCheckImapBMSetting
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCheckImapBMSetting::CT_MsgCheckImapBMSetting(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckImapBMSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckImapBMSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KExpectedBMSupport, iExpectedBMSupport))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for BM support"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks the provisioned BM support for the IMAP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckImapBMSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckImapBMSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
		CleanupStack::PushL(imapSettings);
					
		TImapAccount imapAccountId;
		CT_MsgUtilsCentralRepository::GetImapAccountL((TDes&)iImapAccountName, imapAccountId);
				
		accounts->LoadImapSettingsL(imapAccountId, *imapSettings);
		
		TBool actualBMSupport = imapSettings->BearerMobility();
		
		if (actualBMSupport == iExpectedBMSupport)
			{
			INFO_PRINTF3(_L("Actual Bearer Mobility Support [%d] does equals Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			}
		else
			{
			ERR_PRINTF3(_L("Actual Bearer Mobility Support [%d] does not equal Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // imapSettings, accounts
		}
	return TestStepResult();
	}
