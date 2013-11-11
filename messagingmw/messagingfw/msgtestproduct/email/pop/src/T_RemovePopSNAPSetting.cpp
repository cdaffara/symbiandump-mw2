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
// RemovePopSNAPSetting
// [Paramaters]
// PopAccountName			:Name of the Pop account.
// Removes the provisioned SNAP information for the POP account.
// 
//

//User includes
#include "T_RemovePopSNAPSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literal used
_LIT(KPopAccountName,"PopAccountName");

/**
  Function : CT_MsgRemovePopSNAPSetting
  Description : Constructor 
  @param : aSharedDataPOP Reference to CT_MsgSharedDataPop
  @return : N/A
*/
CT_MsgRemovePopSNAPSetting::CT_MsgRemovePopSNAPSetting(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KRemovePopSNAPSetting);
	}
	
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgRemovePopSNAPSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, iPopAccountName))
		{
		ERR_PRINTF1(_L("Pop account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Removes the provisioned SNAP information from the POP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgRemovePopSNAPSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : RemovePopSNAPSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImIAPPreferences* popIapPrefs = CImIAPPreferences::NewLC();
					
		TPopAccount popAccountId;
		CT_MsgUtilsCentralRepository::GetPopAccountL((TDes&)iPopAccountName, popAccountId);
				
		accounts->LoadPopIapSettingsL(popAccountId, *popIapPrefs);	
		
		popIapPrefs->RemoveSNAP();
		
		accounts->SavePopIapSettingsL(popAccountId, *popIapPrefs);
	
		CleanupStack::PopAndDestroy(2,accounts); // popIapPrefs, accounts
		}
	return TestStepResult();
	}
