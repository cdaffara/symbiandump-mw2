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
* References POP shared data for use by derived test steps.
* 
*
*/



/**
 @file
*/

#ifndef __T_MSG_ASYNCSTEPPOP_H__
#define __T_MSG_ASYNCSTEPPOP_H__

//user includes
#include "T_MsgAsyncStep.h"
#include "T_MsgSharedDataPop.h"


/**
Base class for the asynchronous POP test steps
*/
class CT_MsgAsyncStepPOP : public CT_MsgAsyncStep
	{
protected:
	CT_MsgAsyncStepPOP(CT_MsgSharedDataPop& iSharedDataPop);

	//	MT_MsgActiveCallback implementation
//	virtual void RunL() =0;
//	virtual void DoCancel() =0;

protected:
	CT_MsgSharedDataPop& iSharedDataPOP;
	};

#endif // __T_MSG_ASYNCSTEPPOP_H__
