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

#include "CMtfTestActionSendEMsvCopyStore.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>
#include "MCLIENT.H"
#include "MSERVER.H"

#include "SendProxyClient.h"
#include "SendProxyserver.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "IPCCommandForCapsPermissions.h"



/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCopyStore object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCopyStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCopyStore* self = new (ELeave) CMtfTestActionSendEMsvCopyStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCopyStore
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCopyStore::CMtfTestActionSendEMsvCopyStore(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvCopyStore
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvCopyStore::~CMtfTestActionSendEMsvCopyStore()
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




void CMtfTestActionSendEMsvCopyStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCopyStore);
	// input params
	TInt drive = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvCopyStore :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
		
		error = 0;
		TDriveUnit driveUnit(drive);
		TMsvOp operationId=1;

		// initiate CopyStore
		error=RPSS.SendReceive(EMsvCopyStore , TIpcArgs(operationId, drive));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );

		// we're not interested in the result, just the policing check.
		// if successful we need to cancel the operation so we don't get a panic on exit.
		if (error == KErrNone)
			{
			TBuf8<64> complete;	
			TInt ret =RPSS.SendReceive(EMsvCancelOperation, TIpcArgs(operationId,  &complete ) );
			if (ret>=0)
				{ 
				TestCase().INFO_PRINTF2(_L("Test Action %S: Cancelling outstanding operation successfully completed."), &KTestActionSendEMsvCopyStore);
				}
			else 
				{ // do we care if cancelling the operation fails? most likely not since we're only testing the
				TestCase().ERR_PRINTF3(_L("Test Action %S: Cancelling outstanding operation failed: [%d]!"), &KTestActionSendEMsvCopyStore, ret);
				}
			}
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy();
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCopyStore);
	TestCase().ActionCompletedL(*this);
	}
