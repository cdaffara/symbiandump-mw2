// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "CCloseSimTsyTestStep.h"

#include <simtsy.h>
#include <commsdattypesv1_1.h>
#include <etel.h>
#include <etelmm.h>


using namespace CommsDat;
using namespace SharedData;

const TInt KMsvTestUidPhonePwrValue=0x100052C5;
const TInt KUidCmtPowerValue=0x100052D9;

enum TSACmtPower
	{	
	ECmtOn,		//< Specifies that the power is on
	ECmtOff		//< Specifies that the power is off
	};

enum TMsvTestPhoneStatus
	{
	EMsvTestPhoneOff = 0,
	EMsvTestPhoneOn
	};
/**
 Constructor to set test step name
*/
CCloseSimTsyTestStep::CCloseSimTsyTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KCloseSimTsyTestStep);
	}


CCloseSimTsyTestStep::~CCloseSimTsyTestStep()
	{
	}

// Switch off the test phone (Rproperty)
TVerdict CCloseSimTsyTestStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	TName tsy;
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOff));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidCmtPowerValue, ECmtOff));
	return TestStepResult();
	}

