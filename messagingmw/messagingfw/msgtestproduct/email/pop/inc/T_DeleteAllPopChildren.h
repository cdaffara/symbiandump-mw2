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

#ifndef __T_DELETE_ALL_POP_CHILDREN_H__
#define __T_DELETE_ALL_POP_CHILDREN_H__


// User include
#include "T_MsgAsyncStepPOP.h"

//	Forward decleration
class CT_MsgUtilsDeleteAllChildren;

//Literals Used
_LIT(KDeleteAllPopChildren,"DeleteAllPopChildren");


/**
Implements a test step to delete all the children entries for the remote 
POP account.
*/
class CT_MsgDeleteAllPopChildren : public CT_MsgAsyncStepPOP
	{
public:
	static CT_MsgDeleteAllPopChildren* NewL(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgDeleteAllPopChildren();

	//	CTestStep implementation
	virtual TVerdict	doTestStepL();

private:
	CT_MsgDeleteAllPopChildren(CT_MsgSharedDataPop& aSharedDataPOP);

	//	CT_MsgAsyncStep implementation
	virtual void	ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

private:
	CT_MsgUtilsDeleteAllChildren*	iUtil;
	};

#endif //__T_DELETE_ALL_POP_CHILDREN_H__

