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
// This is the header file for CT_MsgModifyPopSettings2 class
// 
//

#ifndef T_MODIFY_POP_SETTINGS_2_H
#define T_MODIFY_POP_SETTINGS_2_H

/* User include */
#include "T_MsgSyncStepPop.h"

/* Literal used */
_LIT(KModifyPopSettings2,"ModifyPopSettings2");

/* Implements a test step that modifies the settings of the specified Pop account
   via the given config file */
class CT_MsgModifyPopSettings2 : public CT_MsgSyncStepPOP
	{
public :
	CT_MsgModifyPopSettings2(CT_MsgSharedDataPop& aSharedDataPOP);

	~CT_MsgModifyPopSettings2();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};
#endif /* T_MODIFY_POP_SETTINGS_2_H */
