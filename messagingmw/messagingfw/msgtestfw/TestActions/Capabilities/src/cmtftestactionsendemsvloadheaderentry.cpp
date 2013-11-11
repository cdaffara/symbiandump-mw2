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
// Description: Implementation of CMtfTestActionSendEMsvLoadHeaderEntry
//

#include "cmtftestactionsendemsvloadheaderentry.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvLoadHeaderEntry object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvLoadHeaderEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvLoadHeaderEntry* self = new (ELeave) CMtfTestActionSendEMsvLoadHeaderEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvLoadHeaderEntry
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvLoadHeaderEntry::CMtfTestActionSendEMsvLoadHeaderEntry(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvLoadHeaderEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvLoadHeaderEntry::~CMtfTestActionSendEMsvLoadHeaderEntry()
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
void CMtfTestActionSendEMsvLoadHeaderEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvLoadHeaderEntry);
	// input params
	TUid mtmUid = KUidMsgTypeIMAP4;
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	// Will auto start the server if not started.
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvLoadHeaderEntry :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		RPointerArray<CHeaderFields> fieldPairList;
		CleanupClosePushL(fieldPairList);

		HBufC8* buffer = HBufC8::New(KMsvSessionBufferLength);

		TPtr8 ptr = buffer->Des();
		TInt err = RPSS.SendReceive(EMsvLoadHeaderEntry, TIpcArgs(mtmUid.iUid, (TMsvId)40000, &ptr));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, err , pass );

		TMsvPackedHeaderData packedHeaderData(buffer);
		TRAPD(error, packedHeaderData.UnpackL(fieldPairList));
		while(error!=KErrNone)
			{
			// increase the size of the buffer and try again
			buffer->Des().SetLength(0); // to avoid copying contents
			buffer = buffer->ReAllocL(buffer->Des().MaxSize() + KMsvSessionBufferLength);
			TRAP(error, packedHeaderData.UnpackL(fieldPairList));
			}

		CleanupStack::PopAndDestroy();
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvLoadHeaderEntry);
	TestCase().ActionCompletedL(*this);
	}