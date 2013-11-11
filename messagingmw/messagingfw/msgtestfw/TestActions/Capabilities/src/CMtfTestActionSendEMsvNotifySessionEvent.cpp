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

#include "CMtfTestActionSendEMsvNotifySessionEvent.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvNotifySessionEvent object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvNotifySessionEvent::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvNotifySessionEvent* self = new (ELeave) CMtfTestActionSendEMsvNotifySessionEvent(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvNotifySessionEvent
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvNotifySessionEvent::CMtfTestActionSendEMsvNotifySessionEvent(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvNotifySessionEvent
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvNotifySessionEvent::~CMtfTestActionSendEMsvNotifySessionEvent()
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




void CMtfTestActionSendEMsvNotifySessionEvent::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvNotifySessionEvent);
	// input params
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );


	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvNotifySessionEvent);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		TInt error = 0;
		TMsvNotifBuffer change;	
		TPckgBuf<TUint32> sequence;
				
		error = RPSS.SendReceiveProxyAsync(EMsvNotifySessionEvent , TIpcArgs(&change, &sequence) );	
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvNotifySessionEvent, error , pass );

		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvNotifySessionEvent);
	TestCase().ActionCompletedL(*this);
	}
