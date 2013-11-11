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
// AsyncStepCommon
// References Common shared data for use by derived asynchronous test steps.
// 
//

/**
 @file 
*/

/* User includes */
#include "T_MsgAsyncStepCommon.h"


/**
  Function : CT_AsyncStepCommon
  Description : Constructor
  @return : N/A
*/
CT_MsgAsyncStepCommon::CT_MsgAsyncStepCommon(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgAsyncStep()
,	iSharedDataCommon(aSharedDataCommon)
	{
	}

enum TVerdict CT_MsgAsyncStepCommon::doTestStepPostambleL()
	{
		//TBD: Check the handler fail flag?
		//SetTestStepResult(EFail);
		//INFO_PRINTF1(_L("TNTSHandler : Fail"));
		return TestStepResult();
	}
