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
// Description: Implementation of CMtfTestActionSendEMsvUpdateHeaderEntry
//

#include "cmtftestactionsendemsvupdateheaderentry.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvUpdateHeaderEntry object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvUpdateHeaderEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvUpdateHeaderEntry* self = new (ELeave) CMtfTestActionSendEMsvUpdateHeaderEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvUpdateHeaderEntry
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvUpdateHeaderEntry::CMtfTestActionSendEMsvUpdateHeaderEntry(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvUpdateHeaderEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvUpdateHeaderEntry::~CMtfTestActionSendEMsvUpdateHeaderEntry()
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
void CMtfTestActionSendEMsvUpdateHeaderEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvUpdateHeaderEntry);
	// input params
	TUid mtmUid = KUidMsgTypeIMAP4;
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );

	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	// Will auto start the server if not started.
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvUpdateHeaderEntry :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		RPointerArray<CHeaderFields> fieldPairList;
		CleanupClosePushL(fieldPairList);

		CHeaderFields* headerField = new(ELeave) CHeaderFields();
		CleanupStack::PushL(headerField);
		headerField->iUid = TUid::Uid(1001);

		CFieldPair* fieldPair = new(ELeave) CFieldPair();
		CleanupStack::PushL(fieldPair);

		fieldPair->iFieldType = ETextField;

		_LIT(KFieldText1, "!¬\"£$%^ &*()_+-=");
		fieldPair->iFieldTextValue = KFieldText1().AllocL();
		headerField->iFieldPairList.AppendL(fieldPair);

		fieldPairList.AppendL(headerField);

		HBufC8* buffer = HBufC8::New(KMsvSessionBufferLength);

		TMsvPackedHeaderData packedHeaderData(buffer);
		TInt error = packedHeaderData.Pack(fieldPairList);
		while(error!=KErrNone)
			{
			// increase the size of the buffer and try again
			buffer->Des().SetLength(0); // to avoid copying contents
			buffer = buffer->ReAllocL(buffer->Des().MaxSize() + KMsvSessionBufferLength);
			error = packedHeaderData.Pack(fieldPairList);
			}

		TInt err = RPSS.SendReceive(EMsvUpdateHeaderEntry, TIpcArgs(mtmUid.iUid, (TMsvId)40000, buffer));

		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, err , pass );

		CleanupStack::Pop(); // CFieldPair

		CleanupStack::Pop(); // CHeaderFields
		delete headerField;

		CleanupStack::PopAndDestroy();
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvUpdateHeaderEntry);
	TestCase().ActionCompletedL(*this);
	}
