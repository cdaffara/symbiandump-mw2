// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CheckConnected
// [Action Parameters]
// EmailMtmUid <input>:  Value of any IMAP4 MTM uid.
// State       <input>:  The state to test (0 = not connected, 1 = connected)
// [Action Description]
// Checks whether the MTM is connected or not connected to an external server
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionImap4Connected.h"
//#include "CMtfTestCase.h"
//#include "CMtfTestActionParameters.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

//#include <miutset.h>
#include <impcmtm.h>


CMtfTestAction* CMtfTestActionImap4Connected::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionImap4Connected* self = new (ELeave) CMtfTestActionImap4Connected(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionImap4Connected::CMtfTestActionImap4Connected(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionImap4Connected::~CMtfTestActionImap4Connected()
	{
	}


void CMtfTestActionImap4Connected::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionImap4Connected);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));

	// First parameter is the MTM
	CImap4ClientMtm* pMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	// Second parameter is the boolean state
	TBool connected = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(2));
	TInt  expected = KErrNone;
	if(connected == 0)
		expected = KErrDisconnected;

	// Get the state of the MTM
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<16> param;
	pMtm->InvokeSyncFunctionL(KIMAP4MTMIsConnected, *selection, param);
	CleanupStack::PopAndDestroy();	// selection
	TInt err = param[0];
	
	if((err != KErrNone && connected != 0 ) || (err == KErrNone && connected == 0 ))
		{
		TestCase().ERR_PRINTF3( _L("CMtfTestActionImap4Connected err(%d) != expected(%d)"), err, expected );
		TestCase().SetTestStepResult(EFail);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionImap4Connected);
	TestCase().ActionCompletedL(*this);
	}


void CMtfTestActionImap4Connected::DoCancel()
	{
	// No longer asynchronous
	}


void CMtfTestActionImap4Connected::RunL()
	{
	// No longer asynchronous
	}
