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
// EventHandlerAdd
// [Paramaters]
// none
// [APIs Used]
// 
//


/* User includes */
#include "T_MsgEventHandlerAdd.h"

/**
  Function : CT_MsgEventHandlerAdd
  Description : Constructor
  @return : N/A
*/
CT_MsgEventHandlerAdd::CT_MsgEventHandlerAdd(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KMsgEventHandlerAdd);
	}

/**
  Function : doTestStepL
  Description :
  @leave  : 
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgEventHandlerAdd::doTestStepL()
	{
	
	TPtrC flagStr;
	if(!GetStringFromConfig( ConfigSection(), KMsgEventHandlerFlag, flagStr))
		{
		ERR_PRINTF1(_L("Flag is not specified"));
		SetTestStepResult(EFail);
		}	
	
	TPtrC callStr;
	if(!GetStringFromConfig( ConfigSection(), KMsgEventHandlerCall, callStr))
		{
		ERR_PRINTF1(_L("Call is not specified"));
		SetTestStepResult(EFail);
		}

	if(iSharedDataCommon.iUtilityServer.EventHandlerAdd(flagStr, callStr) != KErrNone)
		{
		ERR_PRINTF1(_L("EventHandlerAdd() failed"));
		SetTestStepResult(EFail);
		}
		
	return TestStepResult();
	}
