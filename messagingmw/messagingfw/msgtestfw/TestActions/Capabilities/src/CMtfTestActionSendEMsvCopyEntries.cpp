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

#include "CMtfTestActionSendEMsvCopyEntries.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCopyEntries object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCopyEntries::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCopyEntries* self = new (ELeave) CMtfTestActionSendEMsvCopyEntries(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCopyEntries
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCopyEntries::CMtfTestActionSendEMsvCopyEntries(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvCopyEntries
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvCopyEntries::~CMtfTestActionSendEMsvCopyEntries()
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




void CMtfTestActionSendEMsvCopyEntries::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCopyEntries);
	TMsvId multiMsg1 = 0; // need these for testing special case of copying to Outbox
						  // must have several entries with different caps requirements
						  
	TMsvId messageIdFrom = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(0) );
	TMsvId parentIdTo = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(1) );										
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	
	if (ActionParameters().Count() > 3)
	{
		multiMsg1 = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(3) );
	}
	
	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !") , &KTestActionSendEMsvCopyEntries );
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
		
		// add more entries for testing special outbox case
		if (multiMsg1 != 0)
		{
			entrySelection->AppendL(multiMsg1);
		}
		
		TMsvPackedOperation packedOperation(buffer);
		error = packedOperation.Pack(*entrySelection, parentIdTo);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvCopyEntries );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		
		TMsvOp operationId=1;

		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));				
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message") , &KTestActionSendEMsvCopyEntries );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
			
		error=RPSS.SendReceive(EMsvCopyEntries , TIpcArgs(operationId));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvCopyEntries, error , pass );
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
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the modified message") , &KTestActionSendEMsvCopyEntries );
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

		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCopyEntries);
	TestCase().ActionCompletedL(*this);		
	
	}

