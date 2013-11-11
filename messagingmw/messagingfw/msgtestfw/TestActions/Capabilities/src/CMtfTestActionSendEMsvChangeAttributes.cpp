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

#include "CMtfTestActionSendEMsvChangeAttributes.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvChangeAttributes object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvChangeAttributes::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvChangeAttributes* self = new (ELeave) CMtfTestActionSendEMsvChangeAttributes(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvChangeAttributes
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvChangeAttributes::CMtfTestActionSendEMsvChangeAttributes(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvChangeAttributes
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvChangeAttributes::~CMtfTestActionSendEMsvChangeAttributes()
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




void CMtfTestActionSendEMsvChangeAttributes::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvChangeAttributes);
	// input params
	TMsvId entryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0) );
	TInt setAttributes = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );
	TInt clearAttributes = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3) );


	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: Unable to start proxy server. !") , &KTestActionSendEMsvChangeAttributes);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
		
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);
		if (buffer==NULL)
			{
			TestCase().ERR_PRINTF2(_L("%S :: Unable to allocate mem. !"), &KTestActionSendEMsvChangeAttributes);
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			error = 0;
			CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);
			selection->AppendL(entryId);
			
			TMsvPackedOperation packedOperation(buffer);
			error = packedOperation.Pack(*selection, setAttributes, clearAttributes);
			while(error!=KErrNone)
				{
				// increase the size of the buffer and try again
				buffer->Des().SetLength(0); // to avoid copying contents
				buffer = buffer->ReAllocL(buffer->Des().MaxSize() + KMsvSessionBufferLength);
				if (buffer==NULL)
					{
					TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvChangeAttributes :: Unable to allocate mem. !"));
					TestCase().SetTestStepResult(EFail);
					TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvChangeAttributes);
					TestCase().ActionCompletedL(*this);
					return;
					}
				error = packedOperation.Pack(*selection, setAttributes, clearAttributes);
				}
			
			error=RPSS.SendReceive(EMsvChangeAttributes , TIpcArgs(buffer));
			IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvChangeAttributes, error , pass );	
					
			CleanupStack::PopAndDestroy(selection);	
			CleanupStack::PopAndDestroy(buffer);	
			}
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvChangeAttributes);
	TestCase().ActionCompletedL(*this);
	}
