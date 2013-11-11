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

#include "CMtfTestActionSendEMsvOpenAttachment.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvOpenAttachment object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvOpenAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvOpenAttachment* self = new (ELeave) CMtfTestActionSendEMsvOpenAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvOpenAttachment
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvOpenAttachment::CMtfTestActionSendEMsvOpenAttachment(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvOpenAttachment
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvOpenAttachment::~CMtfTestActionSendEMsvOpenAttachment()
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




void CMtfTestActionSendEMsvOpenAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvOpenAttachment);
	// input params
	TMsvId entryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0) );
	HBufC* fileName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );


	RProxyServerSession RPSS;
	
	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvOpenAttachment :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		error = 0;
		
		TPtr pFileName = fileName->Des();
		TPckg<TInt> pckgSubSessionHandle(0);
		
		TInt sessionHandle=RPSS.SendReceive(EMsvOpenAttachment , TIpcArgs(entryId, &pFileName, &pckgSubSessionHandle));
		if (sessionHandle < 0)
			{
			error = sessionHandle;
			}
		else
			{
			error = KErrNone;
			}
		
		// We may want to do something with the returned file handle.
		// RFile file;
		// file.AdoptFromServer(sessionHandle,pckgSubSessionHandle());
		
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvOpenAttachment);
	TestCase().ActionCompletedL(*this);
	}
