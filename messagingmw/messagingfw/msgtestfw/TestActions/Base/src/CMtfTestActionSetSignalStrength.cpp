// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CMtfTestActionSetNetRegStatus.cpp
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SetSignalStrength
// [Action Parameters]
// TInt		strength		<input>: The new signal strength.
// [Action Description]
// Asks SimTsy to change the signal strength to the value specified.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

#include <e32base.h>
#include <e32property.h>
#include "CMtfTestActionSetSignalStrength.h"
#include "CMtfTestActionSetNetRegStatus.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include <simtsy.h>

const TUint KMtfFakeSignalStrengthChange = 0x1FFFFFFF;
 
CMtfTestAction* CMtfTestActionSetSignalStrength::NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters)
 	{
 	CMtfTestActionSetSignalStrength* self = new (ELeave) CMtfTestActionSetSignalStrength(aTestCase);
 	CleanupStack::PushL(self);
 	self->ConstructL(aActionParameters);
 	CleanupStack::Pop(self);
 	return self;
 	}
 
CMtfTestActionSetSignalStrength::CMtfTestActionSetSignalStrength(CMtfTestCase& aTestCase)
 	: CMtfSynchronousTestAction(aTestCase)
 	{
 	TInt temp;
 	TInt ret = RProperty::Get(KUidPSSimTsyCategory, KPSSimTsySignalStrengthChange, temp);
 	if (ret == KErrNotFound)
 		{
 		RProperty::Define(KUidPSSimTsyCategory, KPSSimTsySignalStrengthChange, KPSSimTsySignalStrengthChangeKeyType);
 		}
  	ret = RProperty::Get(KUidSystemCategory, KMtfFakeSignalStrengthChange, temp);
 	if( ret == KErrNotFound )
 		{
 		RProperty::Define(KUidSystemCategory, KMtfFakeSignalStrengthChange, RProperty::EInt);
 		} 			
 	}

void CMtfTestActionSetSignalStrength::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetSignalStrength);
	TInt status = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	
	TestCase().Logger().WriteFormat(_L("CMtfTestActionSetSignalStrength:: new state: %d"), status);
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsySignalStrengthChange, status));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMtfFakeSignalStrengthChange, status));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetSignalStrength);
	TestCase().ActionCompletedL(*this);
	}

