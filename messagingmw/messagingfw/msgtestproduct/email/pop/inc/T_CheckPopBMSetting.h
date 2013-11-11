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
// This is the header file for CT_MsgCheckPopBMSetting class
// 
//

#ifndef T_CHECK_POP_BM_SETTING_H
#define T_CHECK_POP_BM_SETTING_H

/* User include */
#include "T_MsgSyncStepPop.h"

/* Literal used */
_LIT(KCheckPopBMSetting,"CheckPopBMSetting");

/* Implements a test step to check if bearer mobility is supported by POP service */
class CT_MsgCheckPopBMSetting : public CT_MsgSyncStepPOP
	{
public:
	CT_MsgCheckPopBMSetting(CT_MsgSharedDataPop& aSharedDataPOP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iPopAccountName;
	TBool iExpectedBMSupport;
	
	};
	
#endif /* T_CHECK_POP_BM_SETTING_H */
