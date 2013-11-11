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
// T_ModifyImapSNAPSettings.h
// This is the header file for CT_MsgModifyImapSettings class
// 
//

#ifndef T_MODIFY_IMAP_SETTINGS_H
#define T_MODIFY_IMAP_SETTINGS_H

/* User include */
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KModifyImapSettings,"ModifyImapSettings");

/* Implements a test step that modifies the settings of the specified Imap account
   via the given config file */
class CT_MsgModifyImapSettings : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgModifyImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgModifyImapSettings();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	
	};
#endif /* T_MODIFY_IMAP_SETTINGS_H */
