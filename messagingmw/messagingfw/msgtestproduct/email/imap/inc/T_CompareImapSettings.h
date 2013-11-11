/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* @file 
* This contains the class for CT_MsgCompareImapSettings
* 
*
*/




#ifndef __T_COMPARE_IMAP_SETTINGS_H__
#define __T_COMPARE_IMAP_SETTINGS_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Forward Declaration */
class CImIAPPreferences;
class CImSmtpSettings;
class CImImap4Settings;
class CEmailAccounts;
class TImapAccount;


/* Literals Used*/
_LIT(KCompareImapSettings,	"CompareImapSettings");


/* Implements a test step to Compare the Imap settings */
class CT_MsgCompareImapSettings : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCompareImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	TVerdict doTestStepL();

private:	
	/* 
	 * This function loads the Imap and Smtp settings of the given Imap account
	 * with the settings specified
	 */
	void LoadAccountSettingsL(
				CEmailAccounts& aEmailAccount,
				const TImapAccount& aAccount,
				CImImap4Settings& aImapSettings, 
				CImIAPPreferences& aImapIapSettings,
				CImSmtpSettings& aSmtpSettings, 
				CImIAPPreferences& aSmtpIapSettings);
	};
#endif /* __T_COMPARE_IMAP_SETTINGS_H__*/
