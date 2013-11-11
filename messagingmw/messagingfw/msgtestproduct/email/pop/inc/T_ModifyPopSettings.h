/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/

#ifndef __T_MODIFY_POP_SETTINGS_H__
#define __T_MODIFY_POP_SETTINGS_H__


// User include
#include "T_MsgSyncStepPop.h"

// Literals Used
_LIT(KModifyPopSettings,"ModifyPopSettings");


/**
Implements a test step to modify the settings of a POP account
by reading from the config file specified
*/
class CT_MsgModifyPopSettings : public CT_MsgSyncStepPOP
	{
public :
	CT_MsgModifyPopSettings(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgModifyPopSettings();
	
	// CTestStep implementation
	TVerdict doTestStepL();
	};
#endif //__T_MODIFY_POP_SETTINGS_H__
