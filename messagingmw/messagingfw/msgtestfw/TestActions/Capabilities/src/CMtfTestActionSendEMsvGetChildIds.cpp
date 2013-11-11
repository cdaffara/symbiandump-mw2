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

#include "CMtfTestActionSendEMsvGetChildIds.h"

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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvGetChildIds object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvGetChildIds::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvGetChildIds* self = new (ELeave) CMtfTestActionSendEMsvGetChildIds(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvGetChildIds
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvGetChildIds::CMtfTestActionSendEMsvGetChildIds(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvGetChildIds
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvGetChildIds::~CMtfTestActionSendEMsvGetChildIds()
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




void CMtfTestActionSendEMsvGetChildIds::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvGetChildIds);
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
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIds :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
		
		// Not sure if this will work. Can't use the same mechanism as RMsvServerSession::GetChildIdsL
		// since it uses an internal class to package the filter.
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);
		if (buffer==NULL)
			{
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIds :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}
			
		error = 0;
		
		CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
		TMsvPackedEntryFilterTest package(buffer);
		error = package.PackFilter(*filter);
		
		TPtr8 ptr = buffer->Des();
		
		error=RPSS.SendReceive(EMsvGetChildIds , TIpcArgs(&ptr, entryId));
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
				TestCase().ERR_PRINTF3(_L("CMtfTestActionSendEMsvGetChildIds :: Number of entries do not match! Entries found: [%d], Entries expected: [%d]"), temp3, numberOfEntries);
				error = KErrGeneral;
			}
			else
			{
				TestCase().INFO_PRINTF1(_L("CMtfTestActionSendEMsvGetChildIds :: Number of entries match !"));
			}
			CleanupStack::PopAndDestroy(selection);
		}
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );	
		
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(buffer);
		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvGetChildIds);
	TestCase().ActionCompletedL(*this);
	}
	
// ********************************************************************************
// ***					FUNCTIONALITY DUPLICATED FROM MSGS.DLL					***
// ********************************************************************************
// Need to steal this functionality from MSGS.DLL (defined in MSVIPC.CPP/MSVIPC.H
// since it's not exported from there, and we need to be able to pack the filter
// to call EMsvGetChildIds.

GLDEF_C void DoUnpackFilter(TUint8*& aPtr, CMsvEntryFilter& aFilter)
//
//
//
	{
	// get the entry from the start of the buffer
	const CMsvEntryFilter* pFilter = (CMsvEntryFilter*) aPtr;
	Mem::Copy(&aFilter, pFilter, sizeof(CMsvEntryFilter));
	aPtr = Align4(aPtr + sizeof(CMsvEntryFilter));
	}


GLDEF_C TInt DoPackFilter(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const CMsvEntryFilter& aFilter)
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
		
TMsvPackedEntryFilterTest::TMsvPackedEntryFilterTest(HBufC8*& aBuffer): iBuffer(aBuffer)
{}

TInt TMsvPackedEntryFilterTest::PackFilter(const CMsvEntryFilter& aFilter)
{
	// find the start and end of the buffer
	const TUint8* pS = iBuffer->Ptr();
	const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

	TInt error = DoPackFilter(pS, pE, aFilter);
	if (error==KErrNone)
		{
		// update the length of the buffer
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		}

	return error;
}

void TMsvPackedEntryFilterTest::UnpackFilter(CMsvEntryFilter& aFilter)
{
#if defined(_DEBUG)
	// check that the buffer contain a valid package
	const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(CMsvEntryFilter)));
	__ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), User::Panic(_L("MSGS"), EMsvEntryOverrunBuffer));
#endif	
	TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
	DoUnpackFilter(pS, aFilter);
}

