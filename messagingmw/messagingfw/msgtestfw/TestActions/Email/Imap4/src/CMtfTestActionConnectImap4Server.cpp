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
// ConnectImap4Server
// [Action Parameters]
// ServiceId  <input>: Value of the Imap4 service id.
// Mtm        <input>: Reference to Imap4 client MTM.
// [Action Description]
// Connects to the Imap4 server.
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


#include "CMtfTestActionConnectImap4Server.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <cemailaccounts.h>
#include <iapprefs.h>


#include <impcmtm.h>


CMtfTestAction* CMtfTestActionConnectImap4Server::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionConnectImap4Server* self = new (ELeave) CMtfTestActionConnectImap4Server(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionConnectImap4Server::CMtfTestActionConnectImap4Server(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionConnectImap4Server::~CMtfTestActionConnectImap4Server()
	{
	}


void CMtfTestActionConnectImap4Server::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionConnectImap4Server);		
	TMsvId paramServiceId     = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CImap4ClientMtm* paramMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	// Make sure an account exists for this service id
	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();
	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(paramServiceId, imapAccount);
	CleanupStack::PopAndDestroy(emailAccounts);	
	
	paramMtm->SwitchCurrentEntryL(paramServiceId);
	
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KIMAP4MTMConnect,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectImap4Server);
	}


void CMtfTestActionConnectImap4Server::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionConnectImap4Server::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectImap4Server);
	TestCase().ActionCompletedL(*this);
	}

