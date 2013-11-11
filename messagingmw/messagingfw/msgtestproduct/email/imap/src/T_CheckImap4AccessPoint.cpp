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
// CheckImap4AccessPoint
// [Paramaters]
// ImapAccountName : Name of the Imap account
// ConfigFileName: Configuration file name to read for account settings
// [TestStep Description]
// Checks what access point is being used by an IMAP account connection
// 
//

#include "T_CheckImap4AccessPoint.h"

#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>
#include <imapset.h>
#include <iapprefs.h>

/* Literals Used */
_LIT(KAccountName,"ImapAccountName");
_LIT(KConfigFileName, "ImapConfigFileName");

/**
	Function : CT_MsgCheckImap4AccessPoint
	Description : Constructor
	@param : aSharedDataIMAP		Reference to CT_MsgSharedDataImap
	@return : N/A
*/
CT_MsgCheckImap4AccessPoint::CT_MsgCheckImap4AccessPoint(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckImap4AccessPoint);
	}

/**
	Function : ~CT_MsgCheckImap4AccessPoint
	Description : Destructor
	@return : N/A
*/
CT_MsgCheckImap4AccessPoint::~CT_MsgCheckImap4AccessPoint()
	{
	}

/**
	Function : doTestStepL
	Description : 
	@return : TVerdict - Test step result
	@leave : KMsvNullIndexEntryId	Invalid IMAP account name specified
*/
TVerdict CT_MsgCheckImap4AccessPoint::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckImap4AccessPoint"));

	TPtrC	accountName;
	if(!GetStringFromConfig(ConfigSection(), KAccountName, accountName))
		{
		ERR_PRINTF1(_L("Imap Account name not specified"));
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

	CImIAPPreferences* imapPrefs = CImIAPPreferences::NewLC();
	CImImap4Settings* imapSettings = new (ELeave) CImImap4Settings;
	CleanupStack::PushL(imapSettings);

	TInt expectedAccessPoint = 0;

	if(configFileName.CompareF( _L("none") ) != 0 )
		{
		TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(configFileName, *imapSettings, *imapPrefs));
		if(err)
			{
			ERR_PRINTF2(_L("Failure while setting the IMAP setting parameters, failed with error %d"), err);
			SetTestStepResult(EFail);
			}
		else
			{
			if (imapPrefs->NumberOfIAPs() > 0)
				{
				TImIAPChoice iapChoice = imapPrefs->IAPPreference(0);
				expectedAccessPoint = iapChoice.iIAP;
				}
			}
		}

	CleanupStack::PopAndDestroy(2, imapPrefs);

	if (TestStepResult() == EPass)
		{
		TMsvId serviceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)accountName);

		TUint32 accessPoint;
		TInt err = iSharedDataIMAP.iSession->ServiceAccessPointId(serviceId, accessPoint);
		
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
