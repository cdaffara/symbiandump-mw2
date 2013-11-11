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
// InitNtRas
// [Paramaters]
// none
// Creates the serial drivers required by NTRAS.
// [APIs Used]
// none
// 
//


/* User includes */
#include "T_InitNtRas.h"



#if defined (__WINS__)
	#define PDD_NAME		_L("ECDRV")
	#define LDD_NAME		_L("ECOMM")
#else
	#define PDD_NAME		_L("EUART1")
	#define LDD_NAME		_L("ECOMM")
#endif


/**
  Function : CT_MsgInitNtRas
  Description : Constructor
  @return : N/A
*/
CT_MsgInitNtRas::CT_MsgInitNtRas(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KInitNtRas);
	}

	
	
/**
  Function : doTestStepL
  Description : It loads the physical drive and logical drive
				and .
  @leave  : KErrAlreadyExists if the load drive already exists
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgInitNtRas::doTestStepL()
	{
	INFO_PRINTF1(_L("InitNtRas"));	

	/* Loads the physical device driver (PDD) DLL with the specified filename */
	TInt	driverErr=User::LoadPhysicalDevice(PDD_NAME);
	
	if (driverErr!=KErrAlreadyExists)
		{
		User::LeaveIfError(driverErr);
		}
	
	/* Loads the logical device driver (LDD) DLL with the specified filename */
	driverErr=User::LoadLogicalDevice(LDD_NAME);
	
	if (driverErr!=KErrAlreadyExists)
		{
		User::LeaveIfError(driverErr);
		}
	return TestStepResult();
	}
