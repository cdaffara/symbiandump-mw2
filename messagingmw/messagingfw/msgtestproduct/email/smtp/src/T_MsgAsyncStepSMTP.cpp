// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// [Test Step] :
// AsyncStepSMTP
// References SMTP shared data for use by derived test steps.
// 
//

/**
 @file
*/

// User includes
#include "T_MsgAsyncStepSMTP.h"


/**
CT_AsyncStepSMTP()
Constructor

@param aSharedDataSMTP
*/
CT_MsgAsyncStepSMTP::CT_MsgAsyncStepSMTP(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgAsyncStep()
,	iSharedDataSMTP(aSharedDataSMTP)
	{
	}
