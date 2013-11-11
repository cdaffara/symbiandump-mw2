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
// This is the header file for CT_MsgStartUp
// 
//

/* User includes */
#include "T_StartUp.h"


/**
  Function : CT_MsgStartUp
  Description : Constructor
  @return : none
*/
CT_MsgStartUp::CT_MsgStartUp()
	{
	SetTestStepName(KStartUp);
	}


/**
  Function : doTestStepL
  Description : Dummy step
  @return : TVerdict Test result
*/
TVerdict CT_MsgStartUp::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep :StartUp"));
	return TestStepResult();
	}
