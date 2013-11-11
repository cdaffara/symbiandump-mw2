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
* References Common shared data for use by derived test steps.
* 
*
*/



/**
 @file 
*/


#ifndef __T_MSG_ASYNCSTEPCOMMON_H__
#define __T_MSG_ASYNCSTEPCOMMON_H__

/* User includes */
#include "T_MsgAsyncStep.h"
#include "T_MsgSharedDataCommon.h"


/* Implements a common test step from which other test steps which require 
 * a shared data object can be derived
 */
class CT_MsgAsyncStepCommon : public CT_MsgAsyncStep
	{
public:
	enum TVerdict CT_MsgAsyncStepCommon::doTestStepPostambleL();
	
protected:
	CT_MsgAsyncStepCommon(CT_MsgSharedDataCommon& iSharedDataCommon);

protected:
	/* Reference to object of shared data*/
	CT_MsgSharedDataCommon&	iSharedDataCommon;
	};
#endif /* __T_MSG_ASYNCSTEPCOMMON_H__ */
