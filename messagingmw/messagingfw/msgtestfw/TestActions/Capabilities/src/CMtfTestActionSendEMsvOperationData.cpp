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

#include "CMtfTestActionSendEMsvOperationData.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvOperationData object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvOperationData::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvOperationData* self = new (ELeave) CMtfTestActionSendEMsvOperationData(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvOperationData
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvOperationData::CMtfTestActionSendEMsvOperationData(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvOperationData
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvOperationData::~CMtfTestActionSendEMsvOperationData()
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




void CMtfTestActionSendEMsvOperationData::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvOperationData);
	// input params
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to start proxy server. !"), &KTestActionSendEMsvOperationData);
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
		FillInSimpleMessageEntry( 0, 0, 0, entry);
			// Do not care what the data is just want something to send.
			
		
		TMsvPackedEntry packedEntry(buffer);
		error = packedEntry.PackEntry(entry);
			// Note can also send TMsvPackedOperation if sending CMsvEntrySelection
			// and EMsvOperationData will still work.
		
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvOperationData );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		TMsvOp operationId=1;
		error=RPSS.SendReceive(EMsvOperationData , TIpcArgs(operationId,buffer) );
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvOperationData, error , pass );
	
		// Note appear not to need to cancel or anything when passing data. 
		// Assume server cleans up when connection closed.
	
		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
	
		}
			
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvOperationData);
	TestCase().ActionCompletedL(*this);
	}
