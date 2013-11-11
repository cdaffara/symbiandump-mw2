// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// DoFullImap4Synchronisation
// [Action Parameters]
// Mtm        <input>: Reference to Imap4 client MTM.
// [Action Description]
// Performs full sync with IPAP4 server.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDoFullImap4Synchronisation.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <impcmtm.h>


CMtfTestAction* CMtfTestActionDoFullImap4Synchronisation::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDoFullImap4Synchronisation* self = new (ELeave) CMtfTestActionDoFullImap4Synchronisation(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDoFullImap4Synchronisation::CMtfTestActionDoFullImap4Synchronisation(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDoFullImap4Synchronisation::~CMtfTestActionDoFullImap4Synchronisation()
	{
	}


void CMtfTestActionDoFullImap4Synchronisation::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDoFullImap4Synchronisation);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CImap4ClientMtm* paramMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KIMAP4MTMFullSync,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDoFullImap4Synchronisation);
	}


void CMtfTestActionDoFullImap4Synchronisation::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDoFullImap4Synchronisation::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;
	
	if( err != KErrNone )
		{
		TestCase().ERR_PRINTF3( _L("Test Action %S failed with code %d"), &KTestActionDoFullImap4Synchronisation, err );		
		User::Leave( err );
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDoFullImap4Synchronisation);
	TestCase().ActionCompletedL(*this);
	}

