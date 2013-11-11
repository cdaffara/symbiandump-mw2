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

#include "CMtfTestActionSendEMsvChangeEntry.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvChangeEntry object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvChangeEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvChangeEntry* self = new (ELeave) CMtfTestActionSendEMsvChangeEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvChangeEntry
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvChangeEntry::CMtfTestActionSendEMsvChangeEntry(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvChangeEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvChangeEntry::~CMtfTestActionSendEMsvChangeEntry()
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




void CMtfTestActionSendEMsvChangeEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvChangeEntry);
	// input params	
	
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0), 0);
	TMsvId messageId = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(1) );										
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2), 0);
	TInt createEntryType = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3));
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	  // Will auto start the server if not started.
		
	TSecureId ownerId = 0;
	RPSS.GetServerSecureId( ownerId );
	
				
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !") , &KTestActionSendEMsvChangeEntry);
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

		
		TMsvEntryForward entry;
		if ( createEntryType == ECreateMessage )
			{						
			FillInSimpleMessageEntry( messageId, paramParentId, paramServiceId, entry);
			}
		else if ( createEntryType == ECreateService )
			{
			FillInServiceEntry(messageId, entry);
			}
		else if ( createEntryType == ECreateRemote)
			{
			FillInRemoteMessageEntry(messageId, paramParentId, paramServiceId, entry);
			}
		else
			{
			User::Leave(KErrGeneral);
			}
								
		
		TMsvPackedEntry packedEntry(buffer);
		error = packedEntry.PackEntry(entry);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvChangeEntry );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
						
		TMsvOp operationId=1;
			
		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the modified message") , &KTestActionSendEMsvChangeEntry );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
						
		error= RPSS.SendReceive(EMsvChangeEntry, TIpcArgs(operationId, ownerId));			
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvChangeEntry, error , pass );
			// EMsvChangeEntry is what we are testing.
		
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
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the modified message") , &KTestActionSendEMsvChangeEntry );
				TestCase().SetTestStepResult(EFail);						
				User::LeaveIfError(error);			
				}
			// else KErrNone
			
			if (pass)
				{	// Lets just check that we get the same results out as an extra check. 
					// If it passes we know we must have suffient rights.
								
				CheckWentInStoreL( TestCase(), KTestActionSendEMsvChangeEntry, RPSS, entry.Id() , entry, operationId);
				}
			}
			
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvChangeEntry);
	TestCase().ActionCompletedL(*this);
	}
	


