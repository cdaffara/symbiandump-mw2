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
// @file 
// [TestStep Name]
// CT_MsgEventHandlerCheck
// [Paramaters]
// none
// [APIs Used]
// 
//


/* User includes */
#include "T_MsgEventHandlerCheck.h"

/**
  Function : CT_MsgEventHandlerCheck
  Description : Constructor
  @return : N/A
*/
CT_MsgEventHandlerCheck::CT_MsgEventHandlerCheck(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KMsgEventHandlerCheck);
	}

/**
  Function : doTestStepL
  Description :
  @leave  : 
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgEventHandlerCheck::doTestStepL()
	{
	TInt eventsLeft = iSharedDataCommon.iUtilityServer.EventHandlerCheck();
	
	if(eventsLeft != 0)
		{
		ERR_PRINTF2(_L("Last %d events not matched"), eventsLeft);
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}
