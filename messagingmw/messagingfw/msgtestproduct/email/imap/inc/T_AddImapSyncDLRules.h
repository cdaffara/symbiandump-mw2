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
// This is the header file for CT_MsgAddImapSyncDLRules class
// 
//

#ifndef T_ADD_IMAP_SYNC_DL_RULES_H
#define T_ADD_IMAP_SYNC_DL_RULES_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

class CImapSyncDownloadRules;
class TImImap4GetPartialMailInfo;

/* Literal used */
_LIT(KAddImapSyncDLRules,"AddImapSyncDLRules");

/* Implements a test step to add the specified IMAP sync download rules for a given account */
class CT_MsgAddImapSyncDLRules : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgAddImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	void AddDLRulesL(CImapSyncDownloadRules& aRules, TUint32 aBearerTypes, TImImap4GetPartialMailInfo& aMailInfo);
	
private:
	TPtrC iImapAccountName;
	TInt iTotalSizeLimit;
	TInt iBodyTextSizeLimit;
	TInt iAttachmentSizeLimit;
	TPtrC iPartialMailOptions;
	TPtrC iGetMailBodyParts;
	TPtrC iBearerTypes;
	TPtrC iFolderDLRulesType;
	
	};
#endif /* T_ADD_IMAP_SYNC_DL_RULES_H */
