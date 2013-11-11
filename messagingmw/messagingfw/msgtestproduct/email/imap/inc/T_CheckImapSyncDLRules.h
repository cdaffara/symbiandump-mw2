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
// This is the header file for CT_MsgCheckImapSyncDLRules class
// 
//

#ifndef T_CHECK_IMAP_SYNC_DL_RULES_H
#define T_CHECK_IMAP_SYNC_DL_RULES_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

class TImImap4GetPartialMailInfo;

/* Literal used */
_LIT(KCheckImapSyncDLRules,"CheckImapSyncDLRules");

/* Implements a test step to check the IMAP sync download rules for a given account
   against expected rules */
class CT_MsgCheckImapSyncDLRules : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	TUint32 ConvertBearerTypes();
	TBool CompareDLRules(TImImap4GetPartialMailInfo& aExpectedMailInfo, TImImap4GetPartialMailInfo& aLoadedMailInfo);
	
private:
	TPtrC iImapAccountName;
	TInt iExpectedTotalSizeLimit;
	TInt iExpectedBodyTextSizeLimit;
	TInt iExpectedAttachmentSizeLimit;
	TPtrC iExpectedPartialMailOptions;
	TPtrC iExpectedGetMailBodyParts;
	TPtrC iImapBearerTypes;
	TPtrC iFolderDLRulesType;
	
	};
#endif /* T_CHECK_IMAP_SYNC_DL_RULES_H */
