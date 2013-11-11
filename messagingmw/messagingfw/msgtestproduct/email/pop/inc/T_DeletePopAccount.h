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

#ifndef __T_DELETE_POP_ACCOUNT_H__
#define __T_DELETE_POP_ACCOUNT_H__


// User include
#include "T_MsgSyncStepPop.h"

// Literals Used
_LIT(KDeletePopAccount,"DeletePopAccount");


// This implements a test step to delete the pop account mentioned or to delete all the pop accounts
class CT_MsgDeletePopAccount : public CT_MsgSyncStepPOP
	{
public :
	CT_MsgDeletePopAccount(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgDeletePopAccount();
	
	// CTestStep implementation
	TVerdict doTestStepL();	
	};
#endif //__T_DELETE_POP_ACCOUNT_H__
