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
// This is the header file for CT_MsgShutDown
// 
//

/* User includes */
#include "T_ShutDown.h"



/**
  Function : CT_MsgShutDown
  Description : Constructor
  @return : none
*/
CT_MsgShutDown::CT_MsgShutDown()
	{
	SetTestStepName(KShutDown);
	}



/**
  Function : doTestStepL
  Description : Dummy step
  @return : TVerdict Test result
*/
TVerdict CT_MsgShutDown::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep :ShutDown"));
	return TestStepResult();
	}
