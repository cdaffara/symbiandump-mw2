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

#include "CMtfTestActionSendEMsvCreateEntry.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCreateEntry object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCreateEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCreateEntry* self = new (ELeave) CMtfTestActionSendEMsvCreateEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCreateEntry
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCreateEntry::CMtfTestActionSendEMsvCreateEntry(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvCreateEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvCreateEntry::~CMtfTestActionSendEMsvCreateEntry()
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
 



void CMtfTestActionSendEMsvCreateEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCreateEntry);
	// input params
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0), 0);
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1), 0);
	TInt createEntryType = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	TInt useProxyServerSecurityId = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3) , 1);
	TInt canReadBack = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(5) );


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
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !"), &KTestActionSendEMsvCreateEntry);
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

		
		TMsvEntryForward entry;
		if ( createEntryType == ECreateMessage )
			{						
			FillInSimpleMessageEntry( 0, paramParentId, paramServiceId, entry);
				// aMessageId=0 will be filled in when we attempt to create the message.
			}
		else if (createEntryType == ECreateRemote)
			{
			FillInRemoteMessageEntry(0, paramParentId, paramServiceId, entry);
			}
		else if (createEntryType == ECreateService)
			{
			FillInServiceEntry(0, entry);
			}
		else if ( createEntryType == ECreateMessageSMS )
			{						
			FillInSimpleMessageEntry( 0, paramParentId, paramServiceId, entry);
				// aMessageId=0 will be filled in when we attempt to create the message.
			entry.iMtm = KUidMsgTypeSMS;
			}
		else if (createEntryType == ECreateServiceSMS)
			{
			FillInServiceEntry(0, entry);
			entry.iMtm = KUidMsgTypeSMS;
			}
		else if (createEntryType == ECreateRemoteSMS)
			{
			FillInRemoteMessageEntry(0, paramParentId, paramServiceId, entry);
			entry.iMtm = KUidMsgTypeSMS;
			}
		else
			{
			User::Leave(KErrGeneral);
			}

		TMsvPackedEntry packedEntry(buffer);
		error = packedEntry.PackEntry(entry);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvCreateEntry );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
						
		TMsvOp operationId=1;
		error= RPSS.SendReceive(EMsvOperationData, TIpcArgs(operationId,buffer));
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the created message") , &KTestActionSendEMsvCreateEntry );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
				
		error=RPSS.SendReceive(EMsvCreateEntry , TIpcArgs(operationId, ownerId));
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		
		
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
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to finish sending the modified message") , &KTestActionSendEMsvCreateEntry );
				TestCase().SetTestStepResult(EFail);						
				User::LeaveIfError(error);			
				}
			// else KErrNone
			
			if ( pass && canReadBack )
				{	// Lets just check that we get the same results out as an extra check. 
					// If it passes we know we must have suffient rights.
				CheckWentInStoreL( TestCase(), KTestActionSendEMsvCreateEntry, RPSS, progress.iId, entry, operationId);
				}
			}
			
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
		}
	
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCreateEntry);
	TestCase().ActionCompletedL(*this);
	}
