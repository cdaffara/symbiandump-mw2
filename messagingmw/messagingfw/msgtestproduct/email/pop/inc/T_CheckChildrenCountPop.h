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


#ifndef __T_CHECK_CHILDREN_COUNT_H__
#define __T_CHECK_CHILDREN_COUNT_H__

// user includes
#include"T_MsgSyncStepPop.h"
#include "T_MsgSharedDataPop.h"

// Literals Used
_LIT(KCheckChildrenCountPop,"CheckChildrenCountPop");


/**
Implements the test step which obtains the children count under the POP account
and matches it against the expected count.
*/
class CT_MsgCheckChildrenCountPop : public CT_MsgSyncStepPOP
	{
public:
	CT_MsgCheckChildrenCountPop(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgCheckChildrenCountPop();
	
	// CTestStep implementation
	TVerdict doTestStepL();

private:
	CT_MsgCheckChildrenCountPop();
	};
#endif //__T_CHECK_CHILDREN_COUNT_H__
