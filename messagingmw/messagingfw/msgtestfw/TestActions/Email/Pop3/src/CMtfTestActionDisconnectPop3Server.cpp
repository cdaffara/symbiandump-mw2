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
// DisconnectPop3Server
// [Action Parameters]
// ServiceId  <input>: Value of the POP3 service id.
// Mtm        <input>: Reference to POP3 client MTM.
// [Action Description]
// Discoonnects from the POP3 server.
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

#include "CMtfTestActionDisconnectPop3Server.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"



CMtfTestAction* CMtfTestActionDisconnectPop3Server::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDisconnectPop3Server* self = new (ELeave) CMtfTestActionDisconnectPop3Server(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDisconnectPop3Server::CMtfTestActionDisconnectPop3Server(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase), iOperation(NULL)
	{
	}


CMtfTestActionDisconnectPop3Server::~CMtfTestActionDisconnectPop3Server()
	{
	delete iOperation;
	}


void CMtfTestActionDisconnectPop3Server::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDisconnectPop3Server);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	CPop3ClientMtm* paramMtm = ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(paramServiceId);
	TBuf8<1> param;
	iOperation = paramMtm->InvokeAsyncFunctionL(KPOP3MTMDisconnect,*selection,param,iStatus);
	CleanupStack::PopAndDestroy(selection);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDisconnectPop3Server);
	}


void CMtfTestActionDisconnectPop3Server::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDisconnectPop3Server::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus( *iOperation, iStatus );

	delete iOperation;
	iOperation = NULL;

	if( err != KErrNone )
		{
		TestCase().INFO_PRINTF2(_L("CMtfTestActionDisconnectPop3Server::RunL ERROR = %d"), err );
		User::Leave(err);
		}
	TestCase().ActionCompletedL(*this);
	}

