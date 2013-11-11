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


#include "CMtfTestActionConnectAndSyncImap4Server.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <impcmtm.h>


CMtfTestAction* CMtfTestActionConnectAndSyncImap4Server::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionConnectAndSyncImap4Server* self = new (ELeave) CMtfTestActionConnectAndSyncImap4Server(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionConnectAndSyncImap4Server::CMtfTestActionConnectAndSyncImap4Server(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionConnectAndSyncImap4Server::~CMtfTestActionConnectAndSyncImap4Server()
	{
	}


void CMtfTestActionConnectAndSyncImap4Server::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionConnectAndSyncImap4Server);
	TMsvId paramServiceId     = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CImap4ClientMtm* paramMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	paramMtm->SwitchCurrentEntryL(paramServiceId);
	
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSynchronise,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectAndSyncImap4Server);
	}


void CMtfTestActionConnectAndSyncImap4Server::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionConnectAndSyncImap4Server::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectAndSyncImap4Server);
	TestCase().ActionCompletedL(*this);
	}

