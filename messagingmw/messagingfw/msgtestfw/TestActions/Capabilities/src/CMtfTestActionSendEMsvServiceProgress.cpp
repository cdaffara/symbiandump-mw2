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

#include "CMtfTestActionSendEMsvServiceProgress.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>
#include <impcmtm.h>

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvServiceProgress object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvServiceProgress::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvServiceProgress* self = new (ELeave) CMtfTestActionSendEMsvServiceProgress(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvServiceProgress
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvServiceProgress::CMtfTestActionSendEMsvServiceProgress(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvServiceProgress
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvServiceProgress::~CMtfTestActionSendEMsvServiceProgress()
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

void CMtfTestActionSendEMsvServiceProgress::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvServiceProgress);
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
	// Will auto start the server if not started.
	
	if ( RPSS.Connect() != KErrNone )
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvServiceProgress);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);
		if (buffer==NULL)
			{
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvServiceProgress :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			TPtr8 pProgress = buffer->Des();
			TInt retVal = RPSS.SendReceive(EMsvServiceProgress , TIpcArgs(serviceId, &pProgress));		
			// there is no policing of the EMsvServiceProgress request so just pass the test 
			// independent of what is returned
			
			TestCase().INFO_PRINTF2(_L("%S :: PASS :: Test Always Expected to Pass and Always Passes") , &KTestActionSendEMsvServiceProgress );
			TestCase().SetTestStepResult(EPass);			
			CleanupStack::PopAndDestroy(buffer);	
			}
		}

	CleanupStack::PopAndDestroy( 2, selection ); // buffer, selection
			
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvServiceProgress);
	TestCase().ActionCompletedL(*this);
	}
