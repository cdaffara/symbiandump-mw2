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
// CheckPopSNAPSetting
// [Paramaters]
// PopAccountName			:Name of the Pop account.
// ExpectedSNAPDefinition	:ETrue => A SNAP has expected to been provisioned for the POP account.
// EFalse => A SNAP has not expected to been provisioned for the POP account.
// ExpectedSNAPPreference	:Expected SNAP ID (Assuming the SNAP has been defined for the POP a/c)
// This parameter can be ignored if the "ExpectedSNAPDefinition" is EFalse.
// Checks the provisioned SNAP information for the POP account.
// 
//

//User includes
#include "T_CheckPopSNAPSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KSNAPDefined,"ExpectedSNAPDefinition");
_LIT(KSNAPPreference,"ExpectedSNAPPreference");

/**
  Function : CT_MsgCheckPopSNAPSetting
  Description : Constructor 
  @param : aSharedDataPOP Reference to CT_MsgSharedDataPop
  @return : N/A
*/
CT_MsgCheckPopSNAPSetting::CT_MsgCheckPopSNAPSetting(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KCheckPopSNAPSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckPopSNAPSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, iPopAccountName))
		{
		ERR_PRINTF1(_L("Pop account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KSNAPDefined, iExpectedSNAPDefinition))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for the SNAP definition"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks the provisioned SNAP information for the POP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckPopSNAPSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckPopSNAPSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImIAPPreferences* popIapPrefs = CImIAPPreferences::NewLC();
					
		TPopAccount popAccountId;
		CT_MsgUtilsCentralRepository::GetPopAccountL((TDes&)iPopAccountName,popAccountId);
				
		accounts->LoadPopIapSettingsL(popAccountId, *popIapPrefs);	
		
		TBool actualSNAPDefintion = popIapPrefs->SNAPDefined();
		
		if (actualSNAPDefintion == iExpectedSNAPDefinition)
			{
			if (actualSNAPDefintion)
				{
				TInt expectedSNAPPreference = 0;
				if(!GetIntFromConfig(ConfigSection(), KSNAPPreference, expectedSNAPPreference))
					{
					ERR_PRINTF1(_L("An expected SNAP ID value is not specified"));
					SetTestStepResult(EFail);
					CleanupStack::PopAndDestroy(2,accounts); // popIapPrefs, accounts
					return TestStepResult();
					}
				TInt actualSNAPPreference = popIapPrefs->SNAPPreference();
				
				if (actualSNAPPreference != expectedSNAPPreference)
					{
					ERR_PRINTF3(_L("Actual SNAP ID [%d] does not equal Expected SNAP ID [%d]"),actualSNAPPreference,expectedSNAPPreference);
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF3(_L("Actual SNAP ID [%d] equals Expected SNAP ID [%d]"),actualSNAPPreference,expectedSNAPPreference);
					}
				}
			}
		else
			{
			ERR_PRINTF3(_L("Actual SNAP Defintion [%d] does not equal Expected SNAP Defintion [%d]"),actualSNAPDefintion,iExpectedSNAPDefinition);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // popIapPrefs, accounts
		}
	return TestStepResult();
	}
