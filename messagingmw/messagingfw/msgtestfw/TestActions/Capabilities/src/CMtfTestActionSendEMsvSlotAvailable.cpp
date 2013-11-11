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
//

#include "CMtfTestActionSendEMsvSlotAvailable.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>
#include "MCLIENT.H"
#include "MSERVER.H"


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "IPCCommandForCapsPermissions.h"
#include "SendProxyClient.h"
#include "SendProxyserver.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvSlotAvailable object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvSlotAvailable::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvSlotAvailable* self = new (ELeave) CMtfTestActionSendEMsvSlotAvailable(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvSlotAvailable
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvSlotAvailable::CMtfTestActionSendEMsvSlotAvailable(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvSlotAvailable
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvSlotAvailable::~CMtfTestActionSendEMsvSlotAvailable()
	{
	}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/




void CMtfTestActionSendEMsvSlotAvailable::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvSlotAvailable);
	// input params
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );

	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvSlotAvailable);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		error = 0;
				
		error=RPSS.SendReceive(EMsvSlotAvailable);
		if (error == KErrServerBusy)
			{
			// we're just interested in being able to make the call, so if the server is
			// busy that's ok for our test.
			error = KErrNone;
			}
		
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvSlotAvailable, error , pass );
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvSlotAvailable);
	TestCase().ActionCompletedL(*this);
	}
