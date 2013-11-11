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

#ifndef __T_VERIFY_POP_ACCOUNTS_COUNT_H__
#define __T_VERIFY_POP_ACCOUNTS_COUNT_H__


// User include
#include "T_MsgSyncStepPop.h"

// Literals Used
_LIT(KVerifyPopAccountsCount,"VerifyPopAccountsCount");


/**
This class Implements a test step to verify the count of the Pop accounts created
*/
class CT_MsgVerifyPopAccountsCount: public CT_MsgSyncStepPOP
	{
public :
	CT_MsgVerifyPopAccountsCount(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgVerifyPopAccountsCount();
	
	// CTestStep implementation
	TVerdict doTestStepL();
	};
#endif //__T_VERIFY_POP_ACCOUNTS_COUNT_H__
