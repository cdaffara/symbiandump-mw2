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
* Sync step class owning POP shared data for use by derived test steps.
* 
*
*/



/**
 @file
*/

#ifndef __T_MSG_SYNC_STEP_POP_H__
#define __T_MSG_SYNC_STEP_POP_H__

//user includes
#include "T_MsgStep.h"
#include "T_MsgSharedDataPop.h"

/**
Base class for synchronous POP teststeps.
*/
class CT_MsgSyncStepPOP : public CT_MsgStep
	{
protected:
	CT_MsgSyncStepPOP(CT_MsgSharedDataPop& iSharedDataPop);
	virtual ~CT_MsgSyncStepPOP();

private:
	CT_MsgSyncStepPOP(); //Force iSharedData to be set correctly.

protected:
	CT_MsgSharedDataPop& iSharedDataPOP;
	};

#endif // __T_MSG_SYNC_STEP_POP_H__
