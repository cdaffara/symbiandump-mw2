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

#include "CMtfTestActionSendEMsvCommandData.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCommandData object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCommandData::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCommandData* self = new (ELeave) CMtfTestActionSendEMsvCommandData(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCommandData
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCommandData::CMtfTestActionSendEMsvCommandData(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvCommandData
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvCommandData::~CMtfTestActionSendEMsvCommandData()
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




void CMtfTestActionSendEMsvCommandData::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCommandData);
	// input params
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to start proxy server. !"), &KTestActionSendEMsvCommandData);
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
		
		CMsvEntrySelection* entrySelection =  new (ELeave) (CMsvEntrySelection);
		CleanupDeletePushL(entrySelection);
		
		entrySelection->SetReserveL(1);
		entrySelection->AppendL(KMsvDraftEntryId);			
					
		TMsvPackedOperation packedOperation(buffer);
		error = packedOperation.Pack(*entrySelection, KMsvLocalServiceIndexEntryId);
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvCommandData );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		TMsvOp operationId=1;
		_LIT8(KTestData , "TestData" );
		
		error=RPSS.SendReceive(EMsvCommandData , TIpcArgs(operationId, buffer, &KTestData) );
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvCommandData, error , pass );
			// Note no need to use real data as we are just setting it up and sending it across.
			// When we actually executed the command we would expect some sort of checking.
	
		// Note appear not to need to cancel or anything when passing data. 
		// Assume server cleans up when connection closed.
	

		CleanupStack::Check(entrySelection);
		CleanupStack::PopAndDestroy();

		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();
	
		}
			
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCommandData);
	TestCase().ActionCompletedL(*this);
	

	}
