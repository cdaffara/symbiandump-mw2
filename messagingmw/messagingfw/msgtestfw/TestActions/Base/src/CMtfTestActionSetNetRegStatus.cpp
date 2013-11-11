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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SetNetworkRegistrationStatus
// [Action Parameters]
// TInt		status		<input>: The new network registration status.
// [Action Description]
// Asks SimTsy to change the network registration status to the value specified.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

#include <e32base.h>
#include <e32property.h>
#include "CMtfTestActionSetNetRegStatus.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include <simtsy.h>

const TUint KMtfFakeRegStatChange = 0x1FFFFFFE;
 
CMtfTestAction* CMtfTestActionSetNetRegStatus::NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters)
 	{
 	CMtfTestActionSetNetRegStatus* self = new (ELeave) CMtfTestActionSetNetRegStatus(aTestCase);
 	CleanupStack::PushL(self);
 	self->ConstructL(aActionParameters);
 	CleanupStack::Pop(self);
 	return self;
 	}
 
CMtfTestActionSetNetRegStatus::CMtfTestActionSetNetRegStatus(CMtfTestCase& aTestCase)
 	: CMtfSynchronousTestAction(aTestCase)
 	{
 	TInt temp;
 	TInt ret = RProperty::Get(KUidPSSimTsyCategory, KPSSimTsyRegStatChange, temp);
 	if (ret == KErrNotFound)
 		{
 		RProperty::Define(KUidPSSimTsyCategory, KPSSimTsyRegStatChange, KPSSimTsyRegStatChangeKeyType);
 		}
 	ret = RProperty::Get(KUidSystemCategory, KMtfFakeRegStatChange, temp);
 	if( ret == KErrNotFound )
 		{
 		RProperty::Define(KUidSystemCategory, KMtfFakeRegStatChange, RProperty::EInt);
 		} 			
 	}

void CMtfTestActionSetNetRegStatus::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetNetRegStatus);
	TInt status = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	
	TestCase().Logger().WriteFormat(_L("CMtfTestActionSetNetRegStatus:: new state: %d"), status);
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyRegStatChange, status));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMtfFakeRegStatChange, status));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetNetRegStatus);
	TestCase().ActionCompletedL(*this);
	}

