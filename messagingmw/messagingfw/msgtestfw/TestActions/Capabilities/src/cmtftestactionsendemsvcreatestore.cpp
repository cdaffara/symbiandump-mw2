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
// Description: Implementation of CMtfTestActionSendEMsvCreateStore
//

#include "cmtftestactionsendemsvcreatestore.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvCreateStore object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvCreateStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvCreateStore* self = new (ELeave) CMtfTestActionSendEMsvCreateStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvCreateStore
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvCreateStore::CMtfTestActionSendEMsvCreateStore(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvCreateStore
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvCreateStore::~CMtfTestActionSendEMsvCreateStore()
	{}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none
  @post none
*/
void CMtfTestActionSendEMsvCreateStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvCreateStore);
	// input params
	TUid mtmUid = KUidMsgTypeIMAP4 ; 
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	// Will auto start the server if not started.
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvCreateStore :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		RPointerArray<CFieldPair> fieldPairList;
		CleanupClosePushL(fieldPairList);

		CFieldPair *fieldPair = new(ELeave) CFieldPair();
		CleanupStack::PushL(fieldPair);

		fieldPair->iFieldType = ETextField;

		_LIT16(KFrom, "From");
		fieldPair->iFieldName = KFrom().AllocL();
		_LIT16(KTo, "To");
		fieldPair->iFieldName = KTo().AllocL();
		_LIT16(KCc, "CC");
		fieldPair->iFieldName = KCc().AllocL();
		_LIT16(KBcc, "BCC");
		fieldPair->iFieldName = KBcc().AllocL();
		_LIT16(KSubject, "Subject");
		fieldPair->iFieldName = KSubject().AllocL();

		fieldPairList.AppendL(fieldPair);

		HBufC8* buffer = HBufC8::New(KMsvSessionBufferLength);

		TMsvPackedHeaderStructure packedHeaderStruct(buffer);
		TInt error = packedHeaderStruct.Pack(fieldPairList);
		while(error!=KErrNone)
			{
			// increase the size of the buffer and try again
			buffer->Des().SetLength(0); // to avoid copying contents
			buffer = buffer->ReAllocL(buffer->Des().MaxSize() + KMsvSessionBufferLength);
			error = packedHeaderStruct.Pack(fieldPairList);
		}

		TInt err = RPSS.SendReceive(EMsvCreateStore, TIpcArgs(mtmUid.iUid, buffer));
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, err , pass );

		CleanupStack::Pop(); // CFieldPair
		delete fieldPair;
		CleanupStack::PopAndDestroy();
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvCreateStore);
	TestCase().ActionCompletedL(*this);
	}
