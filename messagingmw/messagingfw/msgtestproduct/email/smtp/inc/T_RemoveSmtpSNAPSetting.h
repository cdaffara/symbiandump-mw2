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
// This is the header file for CT_MsgRemoveSmtpSNAPSetting class
// 
//

#ifndef T_REMOVE_SMTP_SNAP_SETTING_H
#define T_REMOVE_SMTP_SNAP_SETTING_H

/* User include */
#include "T_MsgSyncStepSMTP.h"

/* Literal used */
_LIT(KRemoveSmtpSNAPSetting,"RemoveSmtpSNAPSetting");

/* Implements a test step to remove the SMTP SNAP information for the account */
class CT_MsgRemoveSmtpSNAPSetting : public CT_MsgSyncStepSMTP
	{
public:	
	CT_MsgRemoveSmtpSNAPSetting(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iSmtpAccountName;
	
	};
	
#endif /* T_REMOVE_SMTP_SNAP_SETTING_H */
