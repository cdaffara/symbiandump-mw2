// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// GetBtMTMCommandParameters
// [Action Parameters]
// CconnectionTiemout	<input>: Connection timeout value.
// PutTimeout			<input>: Put timeout value.
// RemotePort			<input>: Remote OBEX Port.
// Password				<input>: Password for authentication.
// CommandParameters	<output>: Reference to the command parameters.
// [Action Description]
// Returns the Bt MTM command parameters.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionGetBtMTMCommandParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <btcmtm.h>

CMtfTestAction* CMtfTestActionGetBtMTMCommandParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetBtMTMCommandParameters* self = new (ELeave) CMtfTestActionGetBtMTMCommandParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetBtMTMCommandParameters::CMtfTestActionGetBtMTMCommandParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetBtMTMCommandParameters::~CMtfTestActionGetBtMTMCommandParameters()
	{
	}


void CMtfTestActionGetBtMTMCommandParameters::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetBtMTMCommandParameters);
	TInt paramConnectionTimeout = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	TInt paramPutTimeout = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	TInt paramRemotePort = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	HBufC* paramPassword   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(3));

	CBtClientMtm::SBtcCmdSendParams sendParams;

	sendParams.iTimeouts.iConnectTimeout = paramConnectionTimeout;
	sendParams.iTimeouts.iPutTimeout = paramPutTimeout;	
	sendParams.iRemoteObexPort = paramRemotePort;
	sendParams.iConnectPassword = paramPassword;

	TPckgBuf<CBtClientMtm::SBtcCmdSendParams> sendParamsBuf(sendParams);

	HBufC8* paramCommandParameters = sendParamsBuf.AllocL();
	
	CleanupStack::PushL(paramCommandParameters);
		
	StoreParameterL<HBufC8>(TestCase(),*paramCommandParameters,ActionParameters().Parameter(4));
	CleanupStack::Pop(paramCommandParameters);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetBtMTMCommandParameters);
	TestCase().ActionCompletedL(*this);
	}


