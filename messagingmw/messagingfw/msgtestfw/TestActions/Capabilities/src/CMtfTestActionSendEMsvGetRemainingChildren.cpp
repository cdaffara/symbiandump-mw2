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

#include "CMtfTestActionSendEMsvGetRemainingChildren.h"




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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvGetRemainingChildren object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvGetRemainingChildren::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvGetRemainingChildren* self = new (ELeave) CMtfTestActionSendEMsvGetRemainingChildren(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvGetRemainingChildren
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvGetRemainingChildren::CMtfTestActionSendEMsvGetRemainingChildren(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvGetChildren
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvGetRemainingChildren::~CMtfTestActionSendEMsvGetRemainingChildren()
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




void CMtfTestActionSendEMsvGetRemainingChildren::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvGetRemainingChildren);
	TMsvId parentId = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(0));
	TInt numberOfEntries = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );

	TInt entriesReturned = 0;
	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetRemainingChildren :: Unable to start proxy server. !"));
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
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetRemainingChildren :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}
		
		
		error = 0;
		TPtr8 ptr=buffer->Des();
		
		TPckgBuf<TMsvChildrenDetails> details;
		details().iParentId = parentId;
		
		TMsvSelectionOrdering ordering;
		TPckgC<TMsvSelectionOrdering> pckgOrdering(ordering);
	
		// first we need to get the initial load of children indexes...
		error=RPSS.SendReceive(EMsvGetChildren , TIpcArgs( &details ,&pckgOrdering, &ptr));
		if (error != KErrOverflow)
		{
			// If we don't get KErrOverflow, it means we don't have any more indexes which means we 
			// need to fail this TA. Need more entries to be able to test this command!
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetRemainingChildren :: Not enough entries to test this command !"));
			TestCase().SetTestStepResult(EFail);
		}
		else
		{
			// Add number of entries retreived by EMsvGetChildren to total count
			entriesReturned += details().iNumberChildrenInArray;
			
			// Get the rest of the entries
			while (error == KErrOverflow)
			{
				error=RPSS.SendReceive(EMsvGetRemainingChildren , TIpcArgs( &details ,NULL, &ptr));
				if (error != KErrNone && error != KErrOverflow)
				{
					// got an error!
					TestCase().ERR_PRINTF2(_L("CMtfTestActionSendEMsvGetRemainingChildren :: EMsvGetRemainingChildren failed with error [%d] !"), error);
					break;
				}
				if (details().iNumberChildrenInArray == 0)
				{
					// The buffer isn't big enough for the current entry
					// we must resize the buffer or we're in an infinite loop
					buffer->Des().SetLength(0);
					buffer->ReAlloc(buffer->Des().MaxSize() + KMsvSessionBufferLength);
					continue;
				}
				
				// Add number of entries to total count
				entriesReturned += details().iNumberChildrenInArray;
			}
			
			// check that number of returned entries match parameter from script
			if (entriesReturned == numberOfEntries)
			{
				TestCase().INFO_PRINTF2(_L("Test Action %S: number if entries match! SUCCESS!"), &KTestActionSendEMsvGetRemainingChildren);
				error = KErrNone;
			}
			else
			{
				// some kind of error occurred but since we don't get the error code back we can't
				// be sure what it was
				TestCase().ERR_PRINTF2(_L("Test Action %S: number if entries DO NOT match! FAILURE!"), &KTestActionSendEMsvGetRemainingChildren);
				error = KErrUnknown;
			}
			
			// ok, we're done or we got an error, check the return code
			IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		}
			
		CleanupStack::PopAndDestroy(buffer);
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvGetRemainingChildren);
	TestCase().ActionCompletedL(*this);
	}

