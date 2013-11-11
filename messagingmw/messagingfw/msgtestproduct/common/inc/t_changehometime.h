// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// Changes the home time of the email messages as mentioned in <test-case>.ini file.
// 
//

#ifndef __T_CHANGE_HOME_TIME_H__
#define __T_CHANGE_HOME_TIME_H__
/* User includes */
#include "T_MsgCommonServer.h"
#include "T_MsgSharedDataCommon.h"
#include "T_MsgSyncStepCommon.h"

/* Epoc includes */
#include <msvapi.h>


/* Literals Used */
_LIT(KChangeHomeTime,"ChangeHomeTime");


/* 
 * Implements the test step which obtains the children count under the specified folder
 * and verifies if it matches with the expected count
 */
class CT_MsgChangeHomeTime : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgChangeHomeTime(CT_MsgSharedDataCommon& aSharedDataCommon);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	};
#endif /*__T_CHANGE_HOME_TIME_H__*/
