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

#include "CMtfTestActionSendEMsvGetChildren.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>
#include "MCLIENT.H"
#include "MSERVER.H"
#include "MSVSTD.H"
#include "MCLENTRY.H"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvGetChildren object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvGetChildren::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvGetChildren* self = new (ELeave) CMtfTestActionSendEMsvGetChildren(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvGetChildren
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvGetChildren::CMtfTestActionSendEMsvGetChildren(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionSendEMsvGetChildren::~CMtfTestActionSendEMsvGetChildren()
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
void CMtfTestActionSendEMsvGetChildren::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvGetChildren);
	TMsvId parentId = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(0));
	TInt numberOfEntries = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );
	
	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildren :: Unable to start proxy server. !"));
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
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildren :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}
		
		TInt entriesReturned = 0;
		error = 0;
		TPtr8 ptr=buffer->Des();
		
		TPckgBuf<TMsvChildrenDetails> details;
		details().iParentId = parentId;
		
		TMsvSelectionOrdering ordering;
		TPckgC<TMsvSelectionOrdering> pckgOrdering(ordering);
	
		error=RPSS.SendReceive(EMsvGetChildren , TIpcArgs( &details ,&pckgOrdering, &ptr));
		if (error == KErrOverflow)
		{
			// KErrOverflow is ok only means there are more entries than fits in the buffer
			TestCase().INFO_PRINTF2(_L("Test Action %S: Got KErrOverflow, more entries are available. Getting remaining entries."), &KTestActionSendEMsvGetChildren);
			
			// Add number of entries retreived by EMsvGetChildren to total count
			entriesReturned += details().iNumberChildrenInArray;
			
			// Get the rest of the entries
			while (error == KErrOverflow)
			{
				error=RPSS.SendReceive(EMsvGetRemainingChildren , TIpcArgs( &details ,NULL, &ptr));
				if (error != KErrNone && error != KErrOverflow)
				{
					// got an error!
					TestCase().ERR_PRINTF2(_L("CMtfTestActionSendEMsvGetChildren :: EMsvGetRemainingChildren failed with error [%d] !"), error);
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
		}
		else if (error == KErrNone)
		{
			entriesReturned = details().iNumberChildrenInArray;
		}

/*		** Data validity check. Since EMsvGetChildren does not return an error even when it gets **
		** KErrPermissionDenied, the only way to check if it failed is to count the entries. If  **
		** the call failed there should be 0 entries. Unfortunately there seems to be no way to  **
		** verify what error code we got so we can't guarantee that it failed because of caps!	 **
*/		   
		// Check number of returned entires. Should match parameter value from script.
		if (entriesReturned != numberOfEntries)
		{
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildren :: Number of children returned does not match !"));
			TestCase().SetTestStepResult(EFail);
		}
		else
		{
			IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		}
		
		CleanupStack::PopAndDestroy(buffer);
		}
		
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvGetChildren);
	TestCase().ActionCompletedL(*this);	
	
	}
