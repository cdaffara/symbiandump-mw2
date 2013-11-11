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
// This is the header file for CT_MsgCheckImapDLRulesSetting class
// 
//

#ifndef T_CHECK_IMAP_DL_RULES_SETTING_H
#define T_CHECK_IMAP_DL_RULES_SETTING_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KCheckImapDLRulesSetting,"CheckImapDLRulesSetting");

/* Implements a test step to check whether download rules will be used by the IMAP service
   during account synchronisation */
class CT_MsgCheckImapDLRulesSetting : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckImapDLRulesSetting(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iImapAccountName;
	TBool iExpectedDLRulesUsage;
	
	};
	
#endif /* T_CHECK_IMAP_DL_RULES_SETTING_H */
