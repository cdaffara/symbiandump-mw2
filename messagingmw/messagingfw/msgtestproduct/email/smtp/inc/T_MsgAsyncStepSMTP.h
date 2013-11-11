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
* References SMTP shared data for use by derived test steps.
* 
*
*/



/**
 @file
*/

#ifndef __T_MSG_ASYNCSTEPSMTP_H__
#define __T_MSG_ASYNCSTEPSMTP_H__

//user includes
#include "T_MsgAsyncStep.h"
#include "T_MsgSharedDataSmtp.h"


/**
Base class for asynchronous SMTP teststeps.
*/
class CT_MsgAsyncStepSMTP : public CT_MsgAsyncStep
	{
protected:
	CT_MsgAsyncStepSMTP(CT_MsgSharedDataSmtp& aSharedDataSMTP);

protected:
	CT_MsgSharedDataSmtp&	iSharedDataSMTP;
	};

#endif // __T_MSG_ASYNCSTEPSMTP_H__
