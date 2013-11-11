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

#include "CMtfTestActionSendEMsvMoveEntries.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvMoveEntries object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvMoveEntries::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvMoveEntries* self = new (ELeave) CMtfTestActionSendEMsvMoveEntries(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvMoveEntries
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvMoveEntries::CMtfTestActionSendEMsvMoveEntries(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvMoveEntries
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvMoveEntries::~CMtfTestActionSendEMsvMoveEntries()
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




void CMtfTestActionSendEMsvMoveEntries::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvMoveEntries);
	TMsvId multiMsg = 0;
	
	TMsvId messageIdFrom = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(0) );										
	TMsvId parentIdTo = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(1) );										
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));

	if (ActionParameters().Count() > 3)
	{
		multiMsg = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(3) );
	}
	
	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !") , &KTestActionSendEMsvMoveEntries);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		error = 0;
		CleanupClosePushL(RPSS);
		
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);						
		TPtr8 ptr=buffer->Des();
		
		CMsvEntrySelection* entrySelection =  new (ELeave) (CMsvEntrySelection);
		CleanupDeletePushL(entrySelection);
		
		entrySelection->SetReserveL(1);
		entrySelection->AppendL(messageIdFrom);

		if (multiMsg != 0)
		{
			entrySelection->AppendL(multiMsg);
		}
		
		TMsvPackedOperation packedOperation(buffer);
		error = packedOperation.Pack(*entrySelection, parentIdTo);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvMoveEntries );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		
		TMsvOp operationId=1;

		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));				
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message") , &KTestActionSendEMsvMoveEntries  );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
			
		error=RPSS.SendReceive(EMsvMoveEntries , TIpcArgs(operationId));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvMoveEntries, error , pass );
		if ( (pass == 0) && (error!=KErrNone ) )
			{ // We expected it to fail and it did so do nothing else basically ignore the error. 
			  // Appears to be no need to do EMsvOperationCompletion.
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
				}
			else if (ret<0)
				{	
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the modified message") , &KTestActionSendEMsvMoveEntries );
				TestCase().SetTestStepResult(EFail);						
				User::LeaveIfError(error);			
				}
			// else KErrNone
			

			}

		CleanupStack::Check(entrySelection);
		CleanupStack::PopAndDestroy();
			
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
		}

		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvMoveEntries);
	TestCase().ActionCompletedL(*this);
	}

