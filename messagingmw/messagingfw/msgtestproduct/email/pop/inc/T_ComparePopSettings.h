/**
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
*
*/



/**
 @file
*/


#ifndef __T_COMPARE_POP_SETTINGS_H__
#define __T_COMPARE_POP_SETTINGS_H__



// User include
#include "T_MsgSyncStepPop.h"

//Epoc includes
#include <cemailaccounts.h>

// Forward Declaration
class CImPop3Settings;
class CImSmtpSettings;
class CImIAPPreferences;

// Literals Used
_LIT(KComparePopSettings,"ComparePopSettings");


/**
Implements a test step to compare the POP and SMTP settings objects
fora POP account
*/
class CT_MsgComparePopSettings : public CT_MsgSyncStepPOP
	{	
public:
	CT_MsgComparePopSettings(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgComparePopSettings();
	
	// CTestStep implementation
	TVerdict doTestStepL();
	
public:	
	TVerdict CompareSettings(CImPop3Settings& aPopSettings1, CImPop3Settings& aPopSettings2,
				CImSmtpSettings& aSmtpSettings1, CImSmtpSettings& aSmtpSettings2,
				CImIAPPreferences& aPopIapPrefs1, CImIAPPreferences& aPopIapPrefs2,
				CImIAPPreferences& aSmtpIapPrefs1, CImIAPPreferences& aSmtpIapPrefs2);
	// This function loads the SMTP and POP settings objects of the specified POP account
	void LoadAccountSettingsL(CEmailAccounts& aEmailAccount, const TPopAccount& aAccount, CImPop3Settings& aPopSettings, 
	CImIAPPreferences& aPopIapSettings,CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings);
	};
#endif // __T_COMPARE_POP_SETTINGS_H__
