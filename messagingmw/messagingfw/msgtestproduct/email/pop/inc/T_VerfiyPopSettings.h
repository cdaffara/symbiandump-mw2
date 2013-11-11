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

#ifndef __T_VERIFY_POP_SETTINGS_H__
#define __T_VERIFY_POP_SETTINGS_H__


// User include
#include "T_MsgSyncStepPop.h"

// Literals used
_LIT(KVerifyPopSettings,"VerifyPopSettings");


/**
Implements a test step that verifies the settings of an POP account
created with the default settings
*/
class CT_MsgVerifyPopSettings : public CT_MsgSyncStepPOP
	{
public :
	CT_MsgVerifyPopSettings(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgVerifyPopSettings();
	
	// CTestStep implementation
	TVerdict doTestStepL();
	};
#endif //__T_VERIFY_POP_SETTINGS_H__
