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
// This is the header file for CT_MsgCompareEmailMessages class
// 
//

#ifndef T_COMPARE_EMAIL_MESSAGES_H
#define T_COMPARE_EMAIL_MESSAGES_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KCompareEmailMessages,"CompareEmailMessages");

 /* Implements a test step which compares the messages in the Sent Items folder
    the with the contents of the remote folder mentioned in the .ini file */
class CT_MsgCompareEmailMessages : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCompareEmailMessages(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();

private:
	TBool ReadIni();

private:
	TPtrC iImapAccountName;
	TPtrC iFolderName;
	TBool iDownloadLimits;
	TBool iDLRulesUsage;
	TPtrC iImapBearerTypes;

	};
#endif /* T_COMPARE_EMAIL_MESSAGES_H */
