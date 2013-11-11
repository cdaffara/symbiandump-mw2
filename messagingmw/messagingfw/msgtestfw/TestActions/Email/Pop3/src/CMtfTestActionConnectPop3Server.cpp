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
// ConnectPop3Server
// [Action Parameters]
// ServiceId  <input>: Value of the POP3 service id.
// Mtm        <input>: Reference to POP3 client MTM.
// [Action Description]
// Connects to the POP3 server.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CPop3ClientMtm::InvokeAsyncFunctionL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <popcmtm.h>

#include <cemailaccounts.h>

#include "CMtfTestActionConnectPop3Server.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

CMtfTestAction* CMtfTestActionConnectPop3Server::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionConnectPop3Server* self = new (ELeave) CMtfTestActionConnectPop3Server(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionConnectPop3Server::CMtfTestActionConnectPop3Server(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionConnectPop3Server::~CMtfTestActionConnectPop3Server()
	{
	}


void CMtfTestActionConnectPop3Server::ExecuteActionL()
	{	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionConnectPop3Server);
	TMsvId popServiceIdParam = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0)); 
	CPop3ClientMtm* paramMtm  = ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(popServiceIdParam);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KPOP3MTMConnect,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectPop3Server);
	}


void CMtfTestActionConnectPop3Server::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionConnectPop3Server::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	TestCase().ActionCompletedL(*this);
	}

