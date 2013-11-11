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
* Sync step class owning SMTP shared data for use by derived test steps.
* 
*
*/



/**
 @file
*/


#ifndef __T_MSG_SYNC_STEP_SMTP_H__
#define __T_MSG_SYNC_STEP_SMTP_H__

//user includes
#include "T_MsgStep.h"
#include "T_MsgSharedDataSmtp.h"


/**
Base class for synchronous SMTP teststeps.
*/
class CT_MsgSyncStepSMTP : public CT_MsgStep
	{
protected:
	CT_MsgSyncStepSMTP(CT_MsgSharedDataSmtp& aSharedDataSMTP);

protected:
	CT_MsgSharedDataSmtp&	iSharedDataSMTP;
	};

#endif // __T_MSG_SYNC_STEP_SMTP_H__
