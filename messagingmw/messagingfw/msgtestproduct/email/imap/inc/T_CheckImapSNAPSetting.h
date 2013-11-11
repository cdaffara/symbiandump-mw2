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
// This is the header file for CT_MsgCheckImapSNAPSetting class
// 
//

#ifndef T_CHECK_IMAP_SNAP_SETTING_H
#define T_CHECK_IMAP_SNAP_SETTING_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KCheckImapSNAPSetting,"CheckImapSNAPSetting");

/* Implements a test step to check the IMAP SNAP information for the account */
class CT_MsgCheckImapSNAPSetting : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckImapSNAPSetting(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iImapAccountName;
	TBool iExpectedSNAPDefinition;
	
	};
	
#endif /* T_CHECK_IMAP_SNAP_SETTING_H */
