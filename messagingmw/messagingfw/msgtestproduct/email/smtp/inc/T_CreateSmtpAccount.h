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
// This is the header file for CT_MsgCreateSmtpAccount class
// 
//

#ifndef T_CREATE_SMTP_ACCOUNT_H
#define T_CREATE_SMTP_ACCOUNT_H

/* User include */
#include "T_MsgSyncStepSMTP.h"

/* Literal used */
_LIT(KCreateSmtpAccount,"CreateSmtpAccount");

/* Implements a test step to create an SMTP account reading the settings from a config file */
class CT_MsgCreateSmtpAccount : public CT_MsgSyncStepSMTP
	{
public:
	CT_MsgCreateSmtpAccount(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();

private:
	TPtrC iSmtpAccountName;
	TPtrC iConfigFileName;
	
	};
#endif /* T_CREATE_SMTP_ACCOUNT_H */
