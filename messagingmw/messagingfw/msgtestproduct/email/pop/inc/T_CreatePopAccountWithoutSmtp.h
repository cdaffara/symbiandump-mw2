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
// This is the header file for CT_MsgCreatePopAccountWithoutSmtp class
// 
//

#ifndef T_CREATE_POP_ACCOUNT_WITHOUT_SMTP_H
#define T_CREATE_POP_ACCOUNT_WITHOUT_SMTP_H

/* User include */
#include "T_MsgSyncStepPop.h"

/* Literal used */
_LIT(KCreatePopAccountWithoutSmtp,"CreatePopAccountWithoutSmtp");

/* Implements a test step to create an POP account reading the settings from a config file */
class CT_MsgCreatePopAccountWithoutSmtp : public CT_MsgSyncStepPOP
	{
public:
	CT_MsgCreatePopAccountWithoutSmtp(CT_MsgSharedDataPop& aSharedDataPOP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();

private:
	TPtrC iPopAccountName;
	TPtrC iConfigFileName;
	
	};
#endif /* T_CREATE_POP_ACCOUNT_WITHOUT_SMTP_H */
