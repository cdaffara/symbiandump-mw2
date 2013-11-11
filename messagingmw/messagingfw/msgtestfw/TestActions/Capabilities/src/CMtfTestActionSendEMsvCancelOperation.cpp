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

#include "CMtfTestActionSendEMsvCancelOperation.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCancelOperation object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCancelOperation::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCancelOperation* self = new (ELeave) CMtfTestActionSendEMsvCancelOperation(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCancelOperation
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCancelOperation::CMtfTestActionSendEMsvCancelOperation(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvCancelOperation
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvCancelOperation::~CMtfTestActionSendEMsvCancelOperation()
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




void CMtfTestActionSendEMsvCancelOperation::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCancelOperation);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt useProxyServerSecurityId = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) , 1);
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3) );

	TSecureId ownerId = 0;
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
		TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to start proxy server. !"), &KTestActionSendEMsvCancelOperation);
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

		TMsvEntryForward entry;
		FillInSimpleMessageEntry( 0, paramParentId, paramServiceId, entry);
					
		TMsvPackedEntry packedEntry(buffer);
		error = packedEntry.PackEntry(entry);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvCancelOperation );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		TMsvOp operationId=1;
		
		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));
			// Lets pretend we are going to perform some operation 1st we send some data.
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message data") , &KTestActionSendEMsvCancelOperation );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		
		RPSS.SendReceive(EMsvCreateEntry , TIpcArgs(operationId, ownerId) );
		
		TBuf8<64> complete;	
		TInt ret =RPSS.SendReceive(EMsvCancelOperation , 
						TIpcArgs(operationId,  &complete ) );
			// Now lets cancel.
		if (ret>=0)
			{ // We are happy some sort of length returned or KErrNone.
			error=KErrNone;
			}
		else 
			{
			error=KErrGeneral; // Just force an error.
			}
		
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvCancelOperation, error , pass );
			// Note no need to use real data as we are just setting it up and sending it across.
			// When we actually executed the command we would expect some sort of checking.
	
		// Note appear not to need to cancel or anything when passing data. 
		// Assume server cleans up when connection closed.
	

		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
	
		}
			
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCancelOperation);
	TestCase().ActionCompletedL(*this);
		
	
	}
