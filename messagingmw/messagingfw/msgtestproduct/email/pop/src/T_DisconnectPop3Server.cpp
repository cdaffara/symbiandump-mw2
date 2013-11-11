// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// [TestStep Name]
// DisConnectPop3Server
// [Paramaters]
// PopAccountName			<input>	: Name of the Pop account
// DisConnects to the Pop3 Server
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CPop3ClientMtm::InvokeAsyncFunctionL
// 
//

/**
 @file
*/

// User Includes
#include "T_DisConnectPop3Server.h"
#include <t_utilscentralrepository.h>


// Epoc includes
#include <pop3cmds.h>


// Literals Used
_LIT(KPopAccountName,"PopAccountName");
const TInt KMaxLenghtOfChar= 1;

/**
CT_MsgDisConnectPop3Server()
Sets the teststep name to KDisConnectPop3Server
  
@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgDisConnectPop3Server::CT_MsgDisConnectPop3Server(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP) 
,	iOperation(NULL)
	{
	SetTestStepName(KDisConnectPop3Server);
	}


/**
NewL()
Allocates and creates a new CT_MsgDisConnectPop3Server object 
  
@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgDisConnectPop3Server
*/
CT_MsgDisConnectPop3Server* CT_MsgDisConnectPop3Server::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgDisConnectPop3Server* self = new(ELeave) CT_MsgDisConnectPop3Server(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}		


/**
~CT_MsgDisConnectPop3Server()
Destructor
*/
CT_MsgDisConnectPop3Server::~CT_MsgDisConnectPop3Server()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
ProgressL()

@param
*/
void CT_MsgDisConnectPop3Server::ProgressL(TBool bFinal)
	{
	//	TODO
	if (bFinal)
		{
		//	Display final progress iOperation
		}
	else
		{
		//	Display current progress iOperation
		}
	}


/**
CancelStep()

@param
*/
void CT_MsgDisConnectPop3Server::CancelStep()
	{
	iOperation->Cancel();
	}

/**
doTestStepL()
Reads the  Pop account name from the ini file. It establishes disconnection with the Pop server.

@return
Returns the test step result
*/
TVerdict CT_MsgDisConnectPop3Server::doTestStepL()
	{	
	INFO_PRINTF1(_L(" Test Step : DisConnectPop3Server"));
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Pop Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Retrieving the Pop service Id for the given Pop account
		TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
		INFO_PRINTF2(_L("Pop service id is %d"),popServiceId);

		// Change the current context
		iSharedDataPOP.iMtm->SwitchCurrentEntryL(popServiceId);

		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		// Appends the popServiceId onto the end of the array
		selection->AppendL(popServiceId);
		
		TBuf8<KMaxLenghtOfChar> param;

		// Attempts to DisConnect to the Pop3 Service
		CT_MsgActive&	active=Active();
		iOperation = iSharedDataPOP.iMtm->InvokeAsyncFunctionL(KPOP3MTMDisconnect, *selection, param, active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		delete iOperation;
		iOperation=NULL;
		CleanupStack::PopAndDestroy(selection); //operation,selection
		User::LeaveIfError(active.Result());
		}
	return TestStepResult();
	}


