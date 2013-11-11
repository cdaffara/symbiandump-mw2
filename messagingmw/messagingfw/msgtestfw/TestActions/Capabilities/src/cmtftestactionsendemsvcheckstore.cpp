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
// Description: Implementation of CMtfTestActionSendEMsvCheckStore
//

#include "cmtftestactionsendemsvcheckstore.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCheckStore object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCheckStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCheckStore* self = new (ELeave) CMtfTestActionSendEMsvCheckStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCheckStore
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCheckStore::CMtfTestActionSendEMsvCheckStore(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvCheckStore
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvCheckStore::~CMtfTestActionSendEMsvCheckStore()
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
void CMtfTestActionSendEMsvCheckStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCheckStore);
	// input params
	TUid mtmUid = KUidMsgTypeIMAP4;
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	// Will auto start the server if not started.
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvCheckStore :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		TPckg<TBool> isStoreExists = EFalse;		
		TInt err = RPSS.SendReceive(EMsvCheckStoreExists, TIpcArgs(mtmUid.iUid,&isStoreExists));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, err , pass );
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCheckStore);
	TestCase().ActionCompletedL(*this);
	}
