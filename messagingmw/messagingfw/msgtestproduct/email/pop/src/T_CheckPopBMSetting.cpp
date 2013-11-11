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
// CheckPopBMSetting
// [Paramaters]
// PopAccountName			:Name of the Pop account.
// ExpectedBMSupport		:ETrue => Bearer mobility is expected to be supported by the POP service.
// EFalse => Bearer mobility is not expected to be supported by the POP service.
// Checks if bearer mobility is supported by the POP service.
// 
//

//User includes
#include "T_CheckPopBMSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KExpectedBMSupport,"ExpectedBMSupport");

/**
  Function : CT_MsgCheckPopBMSetting
  Description : Constructor 
  @param : aSharedDataPOP Reference to CT_MsgSharedDataPop
  @return : N/A
*/
CT_MsgCheckPopBMSetting::CT_MsgCheckPopBMSetting(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KCheckPopBMSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckPopBMSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, iPopAccountName))
		{
		ERR_PRINTF1(_L("Pop account name is not specified"));
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
  Description : Checks the provisioned BM support for the POP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckPopBMSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckPopBMSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
		CleanupStack::PushL(popSettings);
					
		TPopAccount popAccountId;
		CT_MsgUtilsCentralRepository::GetPopAccountL((TDes&)iPopAccountName, popAccountId);
				
		accounts->LoadPopSettingsL(popAccountId, *popSettings);
		
		TBool actualBMSupport = popSettings->BearerMobility();
		
		if (actualBMSupport == iExpectedBMSupport)
			{
			INFO_PRINTF3(_L("Actual Bearer Mobility Support [%d] does equals Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			}
		else
			{
			ERR_PRINTF3(_L("Actual Bearer Mobility Support [%d] does not equal Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // popSettings, accounts
		}
	return TestStepResult();
	}
