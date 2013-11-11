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

#include "CMtfTestActionSendEMsvOperationProgress.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvOperationProgress object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvOperationProgress::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvOperationProgress* self = new (ELeave) CMtfTestActionSendEMsvOperationProgress(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvOperationProgress
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvOperationProgress::CMtfTestActionSendEMsvOperationProgress(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvOperationProgress
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvOperationProgress::~CMtfTestActionSendEMsvOperationProgress()
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




void CMtfTestActionSendEMsvOperationProgress::ExecuteActionL()
	{
	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvOperationProgress);
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
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvOperationProgress);
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
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvOperationProgress );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
						
		TMsvOp operationId=1;
		
		
		
		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the message data required to create the message") , &KTestActionSendEMsvOperationProgress );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}

				
		error=RPSS.SendReceive(EMsvCreateEntry , TIpcArgs(operationId, ownerId));		
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message or create the data.") , &KTestActionSendEMsvOperationProgress );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
			
		TBuf8<64> progress;
		TInt ret=RPSS.SendReceive(EMsvOperationProgress , TIpcArgs(operationId, &progress));
			// Should never generate an error.
		if (ret>=0)
			{ // We are happy some sort of length returned or KErrNone.
			error=KErrNone;
			}
		else 
			{
			error=KErrGeneral; // Just force an error.
			}

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvOperationProgress, error , pass );
		
		
		if ( (pass == 0) && (error!=KErrNone ) )
			{ // We expected it to fail and it did so do nothing else basically ignore the error. 
			  // Appears to be no need to do EMsvOperationCompletion. 
			}
		else
			{ // Now just commit the message so that we do not get a panic.

			TMsvLocalOperationProgress progress;
			TPckg<TMsvLocalOperationProgress> progressPack(progress);
			TInt ret=RPSS.SendReceive(EMsvOperationCompletion , TIpcArgs(operationId, &progressPack));
				// We need to indicate that we have finished and clean up otherwise we will get 
				// a panic.
				
			if (ret>0)
				{ // We are happy some sort of length returned.
				}
			else if (ret<0)
				{	
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the modified message") , &KTestActionSendEMsvOperationProgress );
				TestCase().SetTestStepResult(EFail);						
				User::LeaveIfError(error);			
				}
			// else KErrNone
			}
		
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
		}
	
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvOperationProgress);
	TestCase().ActionCompletedL(*this);		
	}
