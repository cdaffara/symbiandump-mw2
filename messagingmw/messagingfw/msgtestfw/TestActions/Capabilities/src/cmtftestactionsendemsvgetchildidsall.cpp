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

#include "cmtftestactionsendemsvgetchildidsall.h"

#include <e32std.h>
#include <e32base.h>

#include <msvstd.h>
#include "MCLIENT.H"
#include "MSERVER.H"
#include "MSVPANIC.H"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvGetChildIdsAll object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvGetChildIdsAll::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvGetChildIdsAll* self = new (ELeave) CMtfTestActionSendEMsvGetChildIdsAll(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvGetChildIdsAll
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvGetChildIdsAll::CMtfTestActionSendEMsvGetChildIdsAll(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendEMsvGetChildIdsAll
  Description : Destructor
  @internalTechnology
  @param :
  @return :
  @pre
  @post
*/
CMtfTestActionSendEMsvGetChildIdsAll::~CMtfTestActionSendEMsvGetChildIdsAll()
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




void CMtfTestActionSendEMsvGetChildIdsAll::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvGetChildIdsAll);
	// input params
	TInt numberOfEntries = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );
	TMsvId entryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1) );
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2) );


	RProxyServerSession RPSS;

	// Will auto start the server if not started.
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);

	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIdsAll :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);

		// Not sure if this will work. Can't use the same mechanism as RMsvServerSession::GetChildIdsAllL
		// since it uses an internal class to package the filter.
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);
		if (buffer==NULL)
			{
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIdsAll :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}

		error = 0;

		CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
		TMsvPackedEntryFilterTest2 package(buffer);
		error = package.PackFilter(*filter);

		TPtr8 ptr = buffer->Des();

		error=RPSS.SendReceive(EMsvGetChildIdsALL , TIpcArgs(&ptr, entryId));
		if (error == KErrNone)
		{
			// need to unpack the returned data so we can check the number of returned entries
			TInt temp1, temp2;
			TMsvPackedOperation op(buffer);
			CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);

			op.UnpackL(*selection, temp1, temp2);
			TInt temp3 = selection->Count();
			if (temp3 != numberOfEntries)
			{
				TestCase().ERR_PRINTF3(_L("CMtfTestActionSendEMsvGetChildIdsAll :: Number of entries do not match! Entries found: [%d], Entries expected: [%d]"), temp3, numberOfEntries);
				error = KErrGeneral;
			}
			else
			{
				TestCase().INFO_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIdsAll :: Number of entries match !"));
			}
			CleanupStack::PopAndDestroy(selection);
		}
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );

		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(buffer);
		}

	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvGetChildIdsAll);
	TestCase().ActionCompletedL(*this);
	}

// ********************************************************************************
// ***					FUNCTIONALITY DUPLICATED FROM MSGS.DLL					***
// ********************************************************************************
// Need to steal this functionality from MSGS.DLL (defined in MSVIPC.CPP/MSVIPC.H
// since it's not exported from there, and we need to be able to pack the filter
// to call EMsvGetChildIdsAll.

GLDEF_C void DoUnpackFilter2(TUint8*& aPtr, CMsvEntryFilter& aFilter)
//
//
//
	{
	// get the entry from the start of the buffer
	const CMsvEntryFilter* pFilter = (CMsvEntryFilter*) aPtr;
	Mem::Copy(&aFilter, pFilter, sizeof(CMsvEntryFilter));
	aPtr = Align4(aPtr + sizeof(CMsvEntryFilter));
	}


GLDEF_C TInt DoPackFilter2(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const CMsvEntryFilter& aFilter)
//
//
//
	{
	// make sure the entry can fit into the memory area defined by the two pointers
	TInt size = Align4(sizeof(CMsvEntryFilter));
	if ((aPtrStart + size)>aPtrEnd)
		{
		aPtrStart += size;
		return KErrOverflow;
		}

	// copy the filter into the memory area
	Mem::Copy((void*)aPtrStart, &aFilter, size);
	aPtrStart += size;

	return KErrNone;
	}

TMsvPackedEntryFilterTest2::TMsvPackedEntryFilterTest2(HBufC8*& aBuffer): iBuffer(aBuffer)
{}

TInt TMsvPackedEntryFilterTest2::PackFilter(const CMsvEntryFilter& aFilter)
{
	// find the start and end of the buffer
	const TUint8* pS = iBuffer->Ptr();
	const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

	TInt error = DoPackFilter2(pS, pE, aFilter);
	if (error==KErrNone)
		{
		// update the length of the buffer
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		}

	return error;
}

void TMsvPackedEntryFilterTest2::UnpackFilter(CMsvEntryFilter& aFilter)
{
#if defined(_DEBUG)
	// check that the buffer contain a valid package
	const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(CMsvEntryFilter)));
	__ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), User::Panic(_L("MSGS"), EMsvEntryOverrunBuffer));
#endif
	TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
	DoUnpackFilter2(pS, aFilter);
}

