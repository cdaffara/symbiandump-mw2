// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// UnloadPhoneModule
// [Action Parameters]
// HBufC	TsyName	    <input-optional>: Name of the Tsy to use. Defaults to "SIM".
// RTelServer etelServer Stored RTelserver object name is passed, using this unloading phone module is done
// [Action Description]
// Unloads Phone module and closes the object
// Should be called as the last test step for all the tests where ever symtsy is used.
// [APIs Used]
// __ACTION_INFO_END__
// 
//


#include <e32base.h>
#include <e32property.h>
#include <simtsy.h>
#include <etelmm.h>
#include <es_ini.h>
#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;
#include "CMtfTestActionUnloadPhoneModule.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

const TInt KMsvTestUidPhonePwrValue=0x100052C5;
enum TMsvTestPhoneStatus
	{
	EMsvTestPhoneOff = 0,
	EMsvTestPhoneOn
	};
	
CMtfTestAction* CMtfTestActionUnloadPhoneModule::NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters)
 	{
 	CMtfTestActionUnloadPhoneModule* self = new (ELeave) CMtfTestActionUnloadPhoneModule(aTestCase);
 	CleanupStack::PushL(self);
 	self->ConstructL(aActionParameters);
 	CleanupStack::Pop(self);
 	return self;
 	}

CMtfTestActionUnloadPhoneModule::CMtfTestActionUnloadPhoneModule(CMtfTestCase& aTestCase)
 	: CMtfSynchronousTestAction(aTestCase)
 	{
 	}

CMtfTestActionUnloadPhoneModule::~CMtfTestActionUnloadPhoneModule()
	{
	}

void CMtfTestActionUnloadPhoneModule::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionUnloadPhoneModule::ExecuteActionL IN"));
	_LIT(KDefaultTsyName, "SIM"); 
	TPtrC defaultTsyName(KDefaultTsyName);
	HBufC* defaultTsyNameBuf = defaultTsyName.AllocLC();
	HBufC* tsyName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0), defaultTsyNameBuf);
	
	UnloadPhoneModuleL(tsyName);
	CleanupStack::PopAndDestroy(1, defaultTsyNameBuf);  //tsyName
	
	TestCase().ActionCompletedL(*this);
	TestCase().Logger().Write(_L("CMtfTestActionUnloadPhoneModule::ExecuteActionL OUT"));
	}

//Unloading phone module and sets TestPhoneOff
void CMtfTestActionUnloadPhoneModule::UnloadPhoneModuleL(HBufC* aTsyName)
	{
	TInt testState;
	if(KErrNone != RProperty::Get(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidPSSimTsyCategory, KPSSimTsyTestNumber, RProperty::EInt));
		}
	TInt ret;
	ret = RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOff);
	if(ret!=KErrNone)
		{
		TestCase().Logger().Write(_L("UnloadPhoneModule: Unable to set back to EMsvTestPhoneOff"));
		}
	RTelServer etelServer = ObtainValueParameterL<RTelServer>(TestCase(),ActionParameters().Parameter(1));
	ret = etelServer.UnloadPhoneModule(aTsyName->Des());
	if(ret!=KErrNone)
		{
		TestCase().Logger().Write(_L("UnloadPhoneModule: Unable to unload the phone module"));
		}
	etelServer.Close();
	}
	
