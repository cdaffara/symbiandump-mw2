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
// This is the header file for CT_MsgModifyImapSyncDLRules class
// 
//

#ifndef T_MODIFY_IMAP_SYNC_DL_RULES_H
#define T_MODIFY_IMAP_SYNC_DL_RULES_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KModifyImapSyncDLRules,"ModifyImapSyncDLRules");

/* Implements a test step to modify the IMAP sync download rules for a given account */
class CT_MsgModifyImapSyncDLRules : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgModifyImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	TUint32 ConvertBearerTypes();
	
private:
	TPtrC iImapAccountName;
	TInt iNewTotalSizeLimit;
	TInt iNewBodyTextSizeLimit;
	TInt iNewAttachmentSizeLimit;
	TPtrC iNewPartialMailOptions;
	TPtrC iNewGetMailBodyParts;
	TPtrC iImapBearerTypes;
	TPtrC iFolderDLRulesType;
	TInt iDLRulesPos;
	
	};
#endif /* T_MODIFY_IMAP_SYNC_DL_RULES_H */
