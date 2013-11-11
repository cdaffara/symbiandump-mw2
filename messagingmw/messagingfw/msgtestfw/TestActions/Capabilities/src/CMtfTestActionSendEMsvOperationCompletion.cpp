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

#include "CMtfTestActionSendEMsvOperationCompletion.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvOperationCompletion object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvOperationCompletion::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvOperationCompletion* self = new (ELeave) CMtfTestActionSendEMsvOperationCompletion(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvOperationCompletion
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvOperationCompletion::CMtfTestActionSendEMsvOperationCompletion(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvOperationCompletion
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvOperationCompletion::~CMtfTestActionSendEMsvOperationCompletion()
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




void CMtfTestActionSendEMsvOperationCompletion::ExecuteActionL()
	{
	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvOperationCompletion);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt useProxyServerSecurityId = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) , 1);
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3) );


	TSecureId ownerId = 0;
		// Assume set caps had set this correctly.
	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.

	// Allow us to change the owning process of the message.
	if (useProxyServerSecurityId)
		{	// Use the security Id of  the proxy server.		
		RPSS.GetServerSecureId( ownerId  );
		}
	else
		{	// Use the security id of the test server. 
	 	ownerId = RProcess().SecureId();
		}
		
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvOperationCompletion);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		TInt error = 0;
		CleanupClosePushL(RPSS);
							
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);						
		TPtr8 ptr=buffer->Des();
		
		TMsvEntryForward entry;
		FillInSimpleMessageEntry( 0, paramParentId, paramServiceId, entry);

		TMsvPackedEntry packedEntry(buffer);
		error = packedEntry.PackEntry(entry);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvOperationCompletion );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
						
		TMsvOp operationId=1;
		
		
		
		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the message data required to create the message") , &KTestActionSendEMsvOperationCompletion );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}

				
		error=RPSS.SendReceive(EMsvCreateEntry , TIpcArgs(operationId, ownerId));		
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message or create the data.") , &KTestActionSendEMsvOperationCompletion );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
				

		TMsvLocalOperationProgress progress;
		TPckg<TMsvLocalOperationProgress> progressPack(progress);
		TInt ret=RPSS.SendReceive(EMsvOperationCompletion , TIpcArgs(operationId, &progressPack)); // Test this message.
			// We need to indicate that we have finished and clean up otherwise we will get 
			// a panic.
			
		if (ret>=0)
			{ // We are happy some sort of length returned.
			error=KErrNone;
			}
		else if (ret<0)
			{	
			error=KErrGeneral; // Just force an error.
			}
				
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvOperationCompletion, error , pass );
		
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
		}
	
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvOperationCompletion);
	TestCase().ActionCompletedL(*this);
				
	}
