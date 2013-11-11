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
// GetIrMTMCommandParameters
// [Action Parameters]
// CconnectionTiemout	<input>: Connection timeout value.
// PutTimeout			<input>: Put timeout value.
// CommandParameters	<output>: Reference to the command parameters.
// [Action Description]
// Returns the Ir MTM command parameters.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionGetIrMTMCommandParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <ircmtm.h>

CMtfTestAction* CMtfTestActionGetIrMTMCommandParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetIrMTMCommandParameters* self = new (ELeave) CMtfTestActionGetIrMTMCommandParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetIrMTMCommandParameters::CMtfTestActionGetIrMTMCommandParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetIrMTMCommandParameters::~CMtfTestActionGetIrMTMCommandParameters()
	{
	}


void CMtfTestActionGetIrMTMCommandParameters::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetIrMTMCommandParameters);
	TInt paramConnectionTimeout = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	TInt paramPutTimeout = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));

	CIrClientMtm::STimeouts sendParams;

	sendParams.iConnectTimeout = paramConnectionTimeout;
	sendParams.iPutTimeout = paramPutTimeout;

	TPckgBuf<CIrClientMtm::STimeouts> sendParamsBuf(sendParams);
	
	HBufC8* paramCommandParameters = sendParamsBuf.AllocL();
	
	CleanupStack::PushL(paramCommandParameters);
		
	StoreParameterL<HBufC8>(TestCase(),*paramCommandParameters,ActionParameters().Parameter(2));
	CleanupStack::Pop(paramCommandParameters);	
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetIrMTMCommandParameters);

	TestCase().ActionCompletedL(*this);
	}


