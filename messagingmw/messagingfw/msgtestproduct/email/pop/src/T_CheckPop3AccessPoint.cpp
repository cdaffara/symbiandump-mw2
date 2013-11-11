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
// CheckPop3AccessPoint
// [Paramaters]
// PopAccountName : Name of the Pop account
// ConfigFileName: Configuration file name to read for account settings
// [TestStep Description]
// Checks what access point is being used by an POP account connection
// 
//

#include "T_CheckPop3AccessPoint.h"

#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>
#include <pop3set.h>
#include <iapprefs.h>

/* Literals Used */
_LIT(KAccountName,"PopAccountName");
_LIT(KConfigFileName, "PopConfigFileName");

/**
	Function : CT_MsgCheckPop3AccessPoint
	Description : Constructor
	@param : aSharedDataPOP		Reference to CT_MsgSharedDataPop
	@return : N/A
*/
CT_MsgCheckPop3AccessPoint::CT_MsgCheckPop3AccessPoint(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KCheckPop3AccessPoint);
	}

/**
	Function : ~CT_MsgCheckPop3AccessPoint
	Description : Destructor
	@return : N/A
*/
CT_MsgCheckPop3AccessPoint::~CT_MsgCheckPop3AccessPoint()
	{
	}

/**
	Function : doTestStepL
	Description : 
	@return : TVerdict - Test step result
	@leave : KMsvNullIndexEntryId	Invalid POP account name specified
*/
TVerdict CT_MsgCheckPop3AccessPoint::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckPop3AccessPoint"));

	TPtrC	accountName;
	if(!GetStringFromConfig(ConfigSection(), KAccountName, accountName))
		{
		ERR_PRINTF1(_L("Pop Account name not specified"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	TPtrC	configFileName;
	if(!GetStringFromConfig(ConfigSection(), KConfigFileName, configFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	CImIAPPreferences* popPrefs = CImIAPPreferences::NewLC();
	CImPop3Settings* popSettings = new (ELeave) CImPop3Settings;
	CleanupStack::PushL(popSettings);

	TInt expectedAccessPoint = 0;

	if(configFileName.CompareF( _L("none") ) != 0 )
		{
		TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(configFileName, *popSettings, *popPrefs));
		if(err)
			{
			ERR_PRINTF2(_L("Failure while setting the POP setting parameters, failed with error %d"), err);
			SetTestStepResult(EFail);
			}
		else
			{
			if (popPrefs->NumberOfIAPs() > 0)
				{
				TImIAPChoice iapChoice = popPrefs->IAPPreference(0);
				expectedAccessPoint = iapChoice.iIAP;
				}
			}
		}

	CleanupStack::PopAndDestroy(2, popPrefs);

	if (TestStepResult() == EPass)
		{
		TMsvId serviceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)accountName);

		TUint32 accessPoint;
		TInt err = iSharedDataPOP.iSession->ServiceAccessPointId(serviceId, accessPoint);
		
		if (err == KErrNone)
			{
			if (accessPoint == expectedAccessPoint)
				{
				INFO_PRINTF1(_L("Access points match"));
				}
			else
				{
				ERR_PRINTF3(_L("Access point %d does not match expected %d"), accessPoint, expectedAccessPoint);
				SetTestStepResult(EFail);
				}
			}
		else
			{
			ERR_PRINTF2(_L("Failure getting access point - %d"), err);
			SetTestStepResult(EFail);
			}
		}

	return TestStepResult();
	}
