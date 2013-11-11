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
//

#ifndef __T_CREATESMTPMOBILITYACCOUNT_H__
#define __T_CREATESMTPMOBILITYACCOUNT_H__

#include "T_MsgSyncStepSMTP.h"

_LIT(KCreateSmtpMobilityAccount, "CreateSmtpMobilityAccount");

class CT_MsgCreateSmtpMobilityAccount : public CT_MsgSyncStepSMTP
	{
public:	
	static CT_MsgCreateSmtpMobilityAccount* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	//	CTestStep implementation
	TVerdict doTestStepL();
private:
	CT_MsgCreateSmtpMobilityAccount(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	};

#endif //__T_CREATESMTPMOBILITYACCOUNT_H__
