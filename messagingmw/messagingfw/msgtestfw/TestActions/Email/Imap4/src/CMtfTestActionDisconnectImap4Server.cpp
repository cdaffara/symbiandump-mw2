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
// DisconnectImap4Server
// [Action Parameters]
// ServiceId  <input>: Value of the Imap4 service id.
// Mtm        <input>: Reference to Imap4 client MTM.
// [Action Description]
// Discoonnects from the Imap4 server.
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


#include "CMtfTestActionDisconnectImap4Server.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <impcmtm.h>


CMtfTestAction* CMtfTestActionDisconnectImap4Server::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDisconnectImap4Server* self = new (ELeave) CMtfTestActionDisconnectImap4Server(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDisconnectImap4Server::CMtfTestActionDisconnectImap4Server(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDisconnectImap4Server::~CMtfTestActionDisconnectImap4Server()
	{
	}


void CMtfTestActionDisconnectImap4Server::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDisconnectImap4Server);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CImap4ClientMtm* paramMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDisconnectImap4Server);
	}


void CMtfTestActionDisconnectImap4Server::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDisconnectImap4Server::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	if( err != KErrNone )
		{
		TestCase().ERR_PRINTF2(_L("CMtfTestActionDisconnectImap4Server failed with code %d"), err );	
		User::Leave(err);	
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDisconnectImap4Server);
	TestCase().ActionCompletedL(*this);
	}

