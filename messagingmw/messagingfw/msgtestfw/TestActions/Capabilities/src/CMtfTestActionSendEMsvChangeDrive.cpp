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

#include "CMtfTestActionSendEMsvChangeDrive.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvChangeDrive object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvChangeDrive::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvChangeDrive* self = new (ELeave) CMtfTestActionSendEMsvChangeDrive(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvChangeDrive
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvChangeDrive::CMtfTestActionSendEMsvChangeDrive(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvChangeDrive
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvChangeDrive::~CMtfTestActionSendEMsvChangeDrive()
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



TBool Swap( CMtfTestCase& aTestCase, RProxyServerSession& RPSS, TMsvOp operationId , TInt drive  )
	{
	
	TInt error = 0;				
	error = RPSS.SendReceiveProxyAsync(EMsvChangeDrive , TIpcArgs(operationId, drive));

	if (error)
		{
		aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Unable to to swap drive. Potential all following tests broken.!!!!!! ") , &KTestActionSendEMsvChangeDrive );				
		return EFalse;
		}
	
	TMsvLocalOperationProgress progress;
	TPckg<TMsvLocalOperationProgress> progressPack(progress);
	TInt ret=RPSS.SendReceive(EMsvOperationCompletion , TIpcArgs(operationId, &progressPack));
		// Clean up to stop panic.
				
	if (ret < 0)
		{ // Should prbably never happen.
		aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Problem with swap drive. Potential all following tests broken.!!!!!! ") , &KTestActionSendEMsvChangeDrive );				
		return EFalse;
		}
		
	return ETrue;
	}



TInt SwapTest(CMtfTestCase& aTestCase, RProxyServerSession& RPSS, TMsvOp operationId , TInt drive , TInt pass )
	{

	
	TInt swapped=EFalse;
	
	TInt error = 0;				
	error = RPSS.SendReceiveProxyAsync(EMsvChangeDrive , TIpcArgs(operationId, drive));
		// This will have completed by now. 
		// IMPORTANT 
		// Uses the E drive to switch to so need to ensure one is set up. 
		// And it must be read/write.
	IPCCommandReportPassOrFailForCapsPermissions( aTestCase, KTestActionSendEMsvChangeDrive, error , pass );
	
	
	if ( (pass == 0) && (error!=KErrNone ) )
		{ // We expected it to fail and it did so do nothing else basically ignore the error. 
		  // Appears to be no need to do EMsvOperationCompletion.
		swapped=EFalse;
		}
	else
		{

		TMsvLocalOperationProgress progress;
		TPckg<TMsvLocalOperationProgress> progressPack(progress);
		TInt ret=RPSS.SendReceive(EMsvOperationCompletion , TIpcArgs(operationId, &progressPack));
			// We need to indicate that we have finished and clean up otherwise we will get 
			// a panic.
			
		if (ret>0)
			{ // We are happy some sort of length returned.
		  	swapped=ETrue;
			}
		else if (ret<0)
			{	
			aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the message") , &KTestActionSendEMsvChangeDrive );
			aTestCase.SetTestStepResult(EFail);						
		  	swapped=EFalse;
			}
		else 
			{
		  	swapped=ETrue;
			}
		}			
			
	return swapped;
	}


void CMtfTestActionSendEMsvChangeDrive::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvChangeDrive);
	// input params
	TInt drive_start = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );
	TInt drive_end = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );

	TestCase().SetTestStepResult(EFail);
		// Assume failure.

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	TInt swapped=0;
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvChangeDrive :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
		
				
		TMsvOp operationId=1;
						
		TInt initial_drive = RPSS.SendReceive(EMsvGetMessageDrive, TIpcArgs() );		
		if (initial_drive==drive_start)
			{ // On the drive we want to start from.
			
			operationId++;
			swapped=SwapTest(TestCase() , RPSS, operationId, drive_end, pass);
				// Now do the test.
			}				
		else
			{ // We are on a different drive than the one we want to start from.
			
			operationId++;
			swapped=SwapTest(TestCase() , RPSS, operationId, drive_start, pass); 
			
			if (swapped)
				{ 				
				operationId++;
				SwapTest(TestCase() , RPSS, operationId, drive_end, pass); // Now do the switch.
					// Just in case something strange.
				}
			}
			

		TInt current_drive = RPSS.SendReceive(EMsvGetMessageDrive, TIpcArgs() );

		if ( (swapped) && (initial_drive!=current_drive) )
			{ // Swap back. Try not to disturb the environment othewise this will break all the other tests.
			operationId++;
			Swap( TestCase(), RPSS, operationId, initial_drive);
			}			
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvChangeDrive);
	TestCase().ActionCompletedL(*this);
	}
