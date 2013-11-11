// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//



#ifndef __CMTFTESTACTIONGETIMAPACCOUNTSETTINGS_H__
#define __CMTFTESTACTIONGETIMAPACCOUNTSETTINGS_H__

#include "CMtfSynchronousTestAction.h"

class CEmailAccounts;
class TImapAccount;
class CImImap4Settings;
class CImSmtpSettings;
class CImIAPPreferences;

_LIT( KTestActionGetImapAccountSettings,"GetImapAccountSettings");

class CMtfTestActionGetImapAccountSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionGetImapAccountSettings();
	
private:
	CMtfTestActionGetImapAccountSettings(CMtfTestCase& aTestCase);
	
	void LoadSettingsL(CEmailAccounts& aAccounts, const TImapAccount& aAccount, CImImap4Settings& aImapSettings, CImIAPPreferences& aImapIapSettings, CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings);
	};


#endif  // __CMTFTESTACTIONGETIMAPACCOUNTSETTINGS_H__
