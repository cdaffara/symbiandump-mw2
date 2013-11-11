// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cmtftestactionsendemsvdrivelist.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvDriveList object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvDriveList::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvDriveList* self = new (ELeave) CMtfTestActionSendEMsvDriveList(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvDriveList
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvDriveList::CMtfTestActionSendEMsvDriveList(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvDriveList
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvDriveList::~CMtfTestActionSendEMsvDriveList()
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

void CMtfTestActionSendEMsvDriveList::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvDriveList);
	// input params
	TInt expDrives = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;

	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvDriveList :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		RArray<TDriveNumber> driveList;
		const TInt KMsvBufferLength=0x1000;
		HBufC8* buffer = HBufC8::New(KMsvBufferLength);
		TPtr8 ptr = buffer->Des();	
		TInt error = RPSS.SendReceive(EMsvGetDriveList, TIpcArgs(&ptr));
		// Unpack the entries from the buffer.
		TMsvPackedDriveIdOperation packedDriveIdOperation(buffer);
		packedDriveIdOperation.UnpackL(driveList);
		TInt actualDrives = driveList.Count();
		if( actualDrives != expDrives)
			{
			TestCase().ERR_PRINTF3(_L("Expected drives No: %d is not equal to Actual No: %d"), expDrives, actualDrives);
			TestCase().SetTestStepResult(EFail);
			}
		driveList.Close();
		delete buffer;
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvDriveList);
	TestCase().ActionCompletedL(*this);
	}
