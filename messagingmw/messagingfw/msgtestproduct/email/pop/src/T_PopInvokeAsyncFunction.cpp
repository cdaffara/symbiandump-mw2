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
// PopInvokeAsyncFunction
// [Parameters]
// PopAccountName		:		Name of the pop account
// EmailSize			:		The maximum emailsize
// PopLimit			:		The population limit
// PopCommand			:		The POP command to be executed
// Reads the Pop account name,maximun email size,population limit and the pop command 
// name to be executed.Based on the command it calls a utility to establish connection 
// the POP server, executes the POP command and then based on the command again
// disconnects with the POP server.
// [APIs Used]
// CMsvEntry:: GetEntryL
// CBaseMtm :: SwitchCurrentEntryL
// CMsvEntry:: ChildrenL
// CBaseMtm :: InvokeAsyncFunctionL
// 
//

/**
 @file
*/



// User Includes
#include "T_PopInvokeAsyncFunction.h"
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include "T_PopServer.h"
#include "T_MsgSharedDataPop.h"

// Epoc includes
#include <miutset.h>


// Literals Used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KEmailSize,"EmailSize");
_LIT(KPopLimit,"PopLimit");
_LIT(KPopCommand,"PopCommand");


typedef TPckgBuf<TImPop3PopulateOptions> TPop3MailInfoBuf;


/**
CT_MsgModifyPopSettings()
Sets the teststep name to KPopInvokeAsyncFunction

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgPopInvokeAsyncFunction::CT_MsgPopInvokeAsyncFunction(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP)
,	iOperation(NULL)
	{
	SetTestStepName(KPopInvokeAsyncFunction);
	}


/**
NewL()
Allocates and creates a new CT_MsgPopInvokeAsyncFunction object

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgPopInvokeAsyncFunction
*/
CT_MsgPopInvokeAsyncFunction* CT_MsgPopInvokeAsyncFunction::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgPopInvokeAsyncFunction* self = new(ELeave) CT_MsgPopInvokeAsyncFunction(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}	
	

/**
~CT_MsgPopInvokeAsyncFunction()
Destructor
*/
CT_MsgPopInvokeAsyncFunction::~CT_MsgPopInvokeAsyncFunction()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
ProgressL()

@param
*/
void CT_MsgPopInvokeAsyncFunction::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
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
void CT_MsgPopInvokeAsyncFunction::CancelStep()
	{
	iOperation->Cancel();
	}


/**
doTestStepL()
Reads the POP account name,population limit,maximum email size from the ini file
Based on the command decides whether a connection is required.
It then calls the InvokeAsyncFunctionL with the given command.
Based on the command again calls the utility to disconnect fromt the POP server

@return
Returns the test step result
*/
TVerdict CT_MsgPopInvokeAsyncFunction::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : Invoke Async Function"));

	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Account name not specified"));
		SetTestStepResult(EFail);
		}
	else
		{

		// Retrieving the Pop service Id for the given Pop account
		TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);

		// Read the maximum email size from the ini file
		TInt maxEmailSize;
		GetIntFromConfig(ConfigSection(), KEmailSize, maxEmailSize); 

		// Read the population limit from the ini file
		TInt popLimit;
		GetIntFromConfig(ConfigSection(), KPopLimit, popLimit);

		// Read the pop command to be executed from the ini file
		TPtrC popCmdName;
		if(!GetStringFromConfig(ConfigSection(),KPopCommand,popCmdName))
			{
			ERR_PRINTF1(_L("Pop command not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			
			// Changes the entry on which later actions are performed to the entry of the Pop Service Id.
			iSharedDataPOP.iMtm->SwitchCurrentEntryL(popServiceId); //base Class Member iMtm
			TPop3Cmds  popCommand = CT_MsgUtilsEnumConverter::ConvertDesToTPop3Cmds(popCmdName);
			// Accesses the entry for the specified Pop Service Id.
			CMsvEntry* entry = iSharedDataPOP.iSession->GetEntryL(popServiceId); //base Class Member iSession
			entry->SetEntryL(popServiceId);
			CleanupStack::PushL(entry);

			// Create a selection of the children entries  
			CMsvEntrySelection* selection = entry->ChildrenL();
			CleanupStack::PushL(selection); 
			
	
			//setting the max email size and populationlimit to the values read from the ini file
			iMailInfo.SetMaxEmailSize(static_cast<TInt32>(maxEmailSize));
			iMailInfo.SetPopulationLimit(static_cast<TInt32>(popLimit));

			TPop3MailInfoBuf pack(iMailInfo);
			selection->InsertL(0, popServiceId);

			CT_MsgActive&	active=Active();
			iOperation = iSharedDataPOP.iMtm->InvokeAsyncFunctionL( popCommand,
											*selection, pack, active.iStatus ); //base Class Member iMtm, waiter
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
			CleanupStack::PopAndDestroy(2,entry);   // operation,selection,entry
			User::LeaveIfError(active.Result());
			}
		}	
	return TestStepResult();
	}
