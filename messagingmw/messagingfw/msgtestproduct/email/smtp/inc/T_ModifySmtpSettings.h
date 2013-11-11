// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the header file for CT_MsgModifySmtpSettings class
// 
//

#ifndef T_MODIFY_SMTP_SETTINGS_H
#define T_MODIFY_SMTP_SETTINGS_H

/* User include */
#include "T_MsgSyncStepSMTP.h"

/* Literal used */
_LIT(KModifySmtpSettings,"ModifySmtpSettings");

/* Implements a test step that modifies the settings of the specified Smtp account
   via the given config file */
class CT_MsgModifySmtpSettings : public CT_MsgSyncStepSMTP
	{
public :
	CT_MsgModifySmtpSettings(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	~CT_MsgModifySmtpSettings();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};
#endif /* T_MODIFY_SMTP_SETTINGS_H */
