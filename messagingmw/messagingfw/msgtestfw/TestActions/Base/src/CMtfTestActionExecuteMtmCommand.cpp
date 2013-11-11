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
// ExecuteMtmCommand
// [Action Parameters]
// Command           <input>: Value of the command.
// Mtm               <input>: Reference to the client MTM.
// Selection         <input>: Reference to the selection.
// CommandParameters <input>: Reference to the command parameters.
// (SynchronousFlag) <input>: Value of the flag which specifies if the command is executed synchronously. 
// Default is false (asynchronous).
// [Action Description]
// Executes the specific MTM command.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionExecuteMtmCommand.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <impcmtm.h>


CMtfTestAction* CMtfTestActionExecuteMtmCommand::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionExecuteMtmCommand* self = new (ELeave) CMtfTestActionExecuteMtmCommand(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionExecuteMtmCommand::CMtfTestActionExecuteMtmCommand(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionExecuteMtmCommand::~CMtfTestActionExecuteMtmCommand()
	{
	}


void CMtfTestActionExecuteMtmCommand::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionExecuteMtmCommand);
	TInt paramCommand = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	CBaseMtm* paramMtm = ObtainParameterReferenceL<CBaseMtm>(TestCase(),ActionParameters().Parameter(1));
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(2));
	HBufC8* paramCommandParameters = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(3));
	TInt paramSynchronousFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4),EFalse);

	TPtr8 commandParameters = paramCommandParameters->Des();

	if (paramSynchronousFlag)
		{
		paramMtm->InvokeSyncFunctionL(paramCommand,*paramSelection,commandParameters);
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = paramMtm->InvokeAsyncFunctionL(paramCommand,*paramSelection,commandParameters,iStatus);
		CActiveScheduler::Add(this);
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionExecuteMtmCommand);
	}


void CMtfTestActionExecuteMtmCommand::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionExecuteMtmCommand::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);

	TestCase().ActionCompletedL(*this);
	}

