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


#include <e32std.h>
#include <e32base.h>
#include <msvipc.h>
#include <impcmtm.h>


#include "MCLIENT.H"
#include "MSERVER.H"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"
#include "IPCCommandForCapsPermissions.h"
#include "SendProxyClient.h"
#include "SendProxyserver.h"
#include "CMtfTestActionSendEMsvStopService.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvStopService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvStopService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvStopService* self = new (ELeave) CMtfTestActionSendEMsvStopService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvStopService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvStopService::CMtfTestActionSendEMsvStopService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
																																														
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvStopService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvStopService::~CMtfTestActionSendEMsvStopService()
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
void CMtfTestActionSendEMsvStopService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvStopService);
	// for this test action we need a pending operation on the MTM
	// we will create a spurious imap4 disconnect operation
	TMsvId serviceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CImap4ClientMtm* paramMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );
	
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL( selection );
	selection->AppendL( serviceId );
	TBuf8<1> param;

	CMsvOperation* operation = paramMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,*selection,param,iStatus);
	CleanupStack::PushL( operation );

	RProxyServerSession RPSS;
	
	TInt error = RPSS.Connect();
		
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvStopService :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		error = 0;			
		error=RPSS.SendReceive(EMsvStopService , TIpcArgs( serviceId ) );
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		}

	CleanupStack::PopAndDestroy( 2, selection );	
	TestCase().INFO_PRINTF2(_L(" (<: Test Action %S completed :>) "), &KTestActionSendEMsvStopService );
	TestCase().ActionCompletedL(*this);
	}


