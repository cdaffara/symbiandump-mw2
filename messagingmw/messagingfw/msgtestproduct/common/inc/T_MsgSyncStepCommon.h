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
* Sync step class owning Common shared data for use by derived test steps.
* 
*
*/



/**
 @file 
*/



#ifndef __T_MSG_SYNC_STEP_COMMON_H__
#define __T_MSG_SYNC_STEP_COMMON_H__


/* User includes */
#include "T_MsgStep.h"
#include "T_MsgSharedDataCommon.h"


/* 
 * Serves as a base class to create test steps which require access to the
 * shared data object.
 */
class CT_MsgSyncStepCommon : public CT_MsgStep
	{
protected:
	CT_MsgSyncStepCommon(CT_MsgSharedDataCommon& iSharedDataCommon);
protected:
	/* Reference to an object of CT_MsgSharedDataCommon */
	CT_MsgSharedDataCommon& iSharedDataCommon;
	};
#endif /* __T_MSG_SYNC_STEP_COMMON_H__*/
