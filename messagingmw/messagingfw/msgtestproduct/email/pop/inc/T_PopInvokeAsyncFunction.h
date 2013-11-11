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

#ifndef __T_POP_INVOKE_ASYNC_FUNCTION_H__
#define __T_POP_INVOKE_ASYNC_FUNCTION_H__

// User includes
#include "T_MsgAsyncStepPOP.h"
#include "T_MsgSharedDataPop.h"


// Epoc includes
#include <pop3set.h>

// Forward Declaration
class CMsvOperation;
class CT_MsgPopServer;

// Literals Used
_LIT(KPopInvokeAsyncFunction,"PopInvokeAsyncFunction");


/**
Implements a test step to populate the messages from the POP server
*/
class CT_MsgPopInvokeAsyncFunction : public CT_MsgAsyncStepPOP
	{
public:
	static CT_MsgPopInvokeAsyncFunction* NewL(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgPopInvokeAsyncFunction();

	//	CTestStep implementation
	virtual TVerdict doTestStepL();

private:
	CT_MsgPopInvokeAsyncFunction(CT_MsgSharedDataPop& aSharedDataPOP);

	//	CT_MsgAsyncStep implementation
	virtual void ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

private:
	// For the async operation
	CMsvOperation* iOperation;
	// For the mail populate options
	TImPop3PopulateOptions iMailInfo;
	};
#endif //__T_POP_INVOKE_ASYNC_FUNCTION_H__

