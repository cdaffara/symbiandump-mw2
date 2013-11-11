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
// EventHandlerInit
// [Paramaters]
// none
// [APIs Used]
// 
//


/* User includes */
#include "T_MsgEventHandlerInit.h"

/**
  Function : CT_MsgEventHandlerInit
  Description : Constructor
  @return : N/A
*/
CT_MsgEventHandlerInit::CT_MsgEventHandlerInit(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KMsgEventHandlerInit);
	}

/**
  Function : doTestStepL
  Description :
  @leave  : 
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgEventHandlerInit::doTestStepL()
	{
	iSharedDataCommon.iUtilityServer.EventHandlerInit();
	
	return TestStepResult();
	}
