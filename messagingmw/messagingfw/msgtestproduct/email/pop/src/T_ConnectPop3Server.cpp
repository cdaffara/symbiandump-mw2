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
// ConnectPop3Server
// [Paramaters]
// PopAccountName			<input>	: Name of the Pop account
// Connects to the Pop3 Server
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
#include "T_ConnectPop3Server.h"
#include <t_utilscentralrepository.h>


// Epoc includes
#include <miutset.h>
#include <pop3cmds.h>
#include <mtclbase.h>
#include <pop3set.h>

// Literals Used
_LIT(KPopAccountName,"PopAccountName");

const TInt KMaxLenghtOfChar = 1;

/**
CT_MsgConnectPop3Server()
Sets the teststep name to KConnectPop3Server

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgConnectPop3Server::CT_MsgConnectPop3Server(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP) 
,	iOperation(NULL)
	{
	SetTestStepName(KConnectPop3Server);
	}

/**
NewL()
Allocates and creates a new CT_MsgConnectPop3Server object

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgComparePopEmailMsgs
*/
CT_MsgConnectPop3Server* CT_MsgConnectPop3Server::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgConnectPop3Server* self = new(ELeave) CT_MsgConnectPop3Server(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}		


/**
~CT_MsgConnectPop3Server()
Destructor
*/
CT_MsgConnectPop3Server::~CT_MsgConnectPop3Server()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
ProgressL()

@param
*/
void CT_MsgConnectPop3Server::ProgressL(TBool bFinal)
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
void CT_MsgConnectPop3Server::CancelStep()
	{
	iOperation->Cancel();
	}


/**
doTestStepL()
It establishes connection with the Pop server using a POP account.
The account is identified by the account name that is read from the INI file.

@return
Returns the test step result
*/
TVerdict CT_MsgConnectPop3Server::doTestStepL()
	{	
	INFO_PRINTF1(_L(" Test Step : ConnectPop3Server"));
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

		CT_MsgActive&	active=Active();
		// Attempts to connect to the Pop3 Service
		iOperation = iSharedDataPOP.iMtm->InvokeAsyncFunctionL(KPOP3MTMConnect,*selection,param, active.iStatus);

		active.Activate();
		CActiveScheduler::Start();
	
		// Get the error code from TPop3Progress
		TPop3Progress temp;
		TPckgC<TPop3Progress> paramPack(temp);
		paramPack.Set(iOperation->ProgressL());
		TPop3Progress progress=paramPack();
		SetTestStepError(progress.iErrorCode);

		delete iOperation;
		iOperation=NULL;
		
		CleanupStack::PopAndDestroy(selection); //operation,selection
	//	User::LeaveIfError(active.Result());
		}
	return TestStepResult();
	}
