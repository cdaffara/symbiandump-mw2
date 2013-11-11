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
// This is the header file for CT_MsgDeleteImapSyncDLRules class
// 
//

#ifndef T_DELETE_IMAP_SYNC_DL_RULES_H
#define T_DELETE_IMAP_SYNC_DL_RULES_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

class CImapSyncDownloadRules;

/* Literal used */
_LIT(KDeleteImapSyncDLRules,"DeleteImapSyncDLRules");

/* Implements a test step to delete the IMAP sync download rules for a given account */
class CT_MsgDeleteImapSyncDLRules : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgDeleteImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	void DeleteDLRulesL(CImapSyncDownloadRules& aRules);
	
private:
	TPtrC iImapAccountName;
	TPtrC iFolderDLRulesType;
	
	};
#endif /* T_DELETE_IMAP_SYNC_DL_RULES_H */
