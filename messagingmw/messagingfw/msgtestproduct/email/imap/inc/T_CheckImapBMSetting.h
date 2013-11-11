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
// This is the header file for CT_MsgCheckImapBMSetting class
// 
//

#ifndef T_CHECK_IMAP_BM_SETTING_H
#define T_CHECK_IMAP_BM_SETTING_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KCheckImapBMSetting,"CheckImapBMSetting");

/* Implements a test step to check if bearer mobility is supported by IMAP service */
class CT_MsgCheckImapBMSetting : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckImapBMSetting(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iImapAccountName;
	TBool iExpectedBMSupport;
	
	};
	
#endif /* T_CHECK_IMAP_BM_SETTING_H */
