// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "test_erroractions_step.h"
#include <msvuids.h>
#include <msvids.h>
#include <s32mem.h>

/**
Test Step class implementation
*/		
CMsvSendErrorActionsStep::CMsvSendErrorActionsStep()
	{
	SetTestStepName(KMsvSendErrorActionsStep);
	}

TVerdict CMsvSendErrorActionsStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;
	User::LeaveIfError(iFs.Connect());
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	iSession = CMsvSession::OpenSyncL(*this);
	iErrorActions = CMsvSendErrorActions::NewL();
	SetConfigurationL();
	return TestStepResult();
	}

TVerdict CMsvSendErrorActionsStep::doTestStepPostambleL()
	{
	delete iErrorActions;
	delete iSession;
	CActiveScheduler::Install(NULL);
	delete iScheduler;	
	iFs.Close();
	__UHEAP_MARKEND;
	return TestStepResult();
	}
			
TVerdict CMsvSendErrorActionsStep::doTestStepL()
	{
	TestDefaultErrorAction();
	TestManipulateErrorActionL();
	TestSetGetErrorsL();
	TestRestoreFromResourceL();	
	return TestStepResult();
	}	


void CMsvSendErrorActionsStep::TestDefaultErrorAction()
	{
	TMsvSendErrorAction action;
	action.iError = -10;
	action.iAction = ESendActionSentAlready;
	action.iRetries = ESendRetriesInfinite;
	action.iRetrySpacing = ESendRetrySpacingVariable;
	action.SetMaxRetries(2);
	
	iErrorActions->SetDefault(action);
	if(!CompareErrorAction(action, iErrorActions->Default()))
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions:: SetDefault/Default failed"));
		SetTestStepResult(EFail);
		}	
	}
	
void CMsvSendErrorActionsStep::TestManipulateErrorActionL()
	{
	TMsvSendErrorAction action;
	action.iError = -15;
	action.iAction = ESendActionSentAlready;
	action.iRetries = ESendRetriesInfinite;
	action.iRetrySpacing = ESendRetrySpacingVariable;
	action.SetMaxRetries(2);	
	iErrorActions->AddSendErrorActionL(action);
	
	TMsvSendErrorAction action1;
	if(iErrorActions->GetSendErrorAction(action.iError, action1) != KErrNone ||
		!CompareErrorAction(action, action1))
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions:: GetSendErrorAction failed"));
		SetTestStepResult(EFail);
		}
	
	if(iErrorActions->RemoveSendErrorAction(action.iError) != KErrNone)
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions:: RemoveSendErrorAction failed"));
		SetTestStepResult(EFail);		
		}
	
	if(iErrorActions->GetSendErrorAction(action.iError, action1) == KErrNone)
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions:: GetSendErrorAction (or RemoveSendErrorAction) failed"));
		SetTestStepResult(EFail);
		}		
	}

void CMsvSendErrorActionsStep::TestSetGetErrorsL()		
	{
	const CArrayFixFlat<TMsvSendErrorAction>& errors = iErrorActions->Errors();
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	errorActions->SetErrorsL(errors);
	if(!CompareErrorActions(*errorActions, EFalse, ETrue))
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions::Errors/SetErrorsL failed"));
		SetTestStepResult(EFail);
		}
	CleanupStack::PopAndDestroy(errorActions);
	}

void CMsvSendErrorActionsStep::TestRestoreFromResourceL()
	{
	CBufFlat *buf = CBufFlat::NewL(32);
	CleanupStack::PushL(buf);
	buf->ResizeL(0x10000);		
	RBufWriteStream writer(*buf);
	writer.PushL();	
	
	//default error action
	writer.WriteInt8L(0); //flag, action + retry + retry spacing
	writer.WriteInt16L(3); //max retries
	writer.WriteInt16L(1); //error code count
	writer.WriteInt32L(-1); //error code
	
	//error action count
	writer.WriteInt16L(1);
	
	//error action
	writer.WriteInt8L(1);
	writer.WriteInt16L(2);
	writer.WriteInt16L(2);	//error code count
	writer.WriteInt32L(-2);	//error code -2
	writer.WriteInt32L(-3);	//error code -3
	
	writer.CommitL();
	CleanupStack::PopAndDestroy(&writer);
		
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);

	TResourceReader reader;
	TPtr8 bufPtr = buf->Ptr(0);
	reader.SetBuffer(&bufPtr);
	errorActions->RestoreFromResourceL(reader);
	
	TMsvSendErrorAction errorAction;
	if(errorActions->GetSendErrorAction(-2, errorAction) != KErrNone ||
		errorActions->GetSendErrorAction(-3, errorAction) != KErrNone ||
		errorActions->Errors().Count() != 2)
		{
		INFO_PRINTF1(_L("CMsvSendErrorActions::RestoreFromResourceL failed"));
		SetTestStepResult(EFail);
		}
	CleanupStack::PopAndDestroy(errorActions); 
	CleanupStack::PopAndDestroy(buf);
	}
	
	
TBool CMsvSendErrorActionsStep::CompareErrorActions(const CMsvSendErrorActions& aDest, TBool aCompareDefault, TBool aCompareIndividual)
	{
	if(aCompareDefault && !CompareErrorAction(iErrorActions->Default(), aDest.Default()))
		{
		return EFalse;
		}
	const CArrayFixFlat<TMsvSendErrorAction>& errorsSource = iErrorActions->Errors();
	const CArrayFixFlat<TMsvSendErrorAction>& errorsDest = aDest.Errors();
	TInt count = errorsSource.Count();
	if(count != errorsDest.Count())
		{
		return EFalse;
		}
		
	while(aCompareIndividual && count--)
		{
		if(!CompareErrorAction(errorsSource.At(count), errorsDest.At(count)))
			{
			return EFalse;
			}
		}	
		
	return ETrue;	
	}
	
				
TBool CMsvSendErrorActionsStep::CompareErrorAction(const TMsvSendErrorAction& aAction1, const TMsvSendErrorAction& aAction2)
	{
	if(aAction1.iAction != aAction2.iAction ||
		aAction1.iError != aAction2.iError ||
		aAction1.iRetries != aAction2.iRetries ||
		aAction1.iRetrySpacing != aAction2.iRetrySpacing ||
		aAction1.MaxRetries() != aAction2.MaxRetries())
		{
		return EFalse;
		}
	return ETrue;	
	}
	 			
void CMsvSendErrorActionsStep::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
	{
	//do nothing
	}

_LIT(KCfgErrorActionsTotal, "ErrorActionsTotal");
_LIT(KCfgDefaultErrorActionIndex, "DefaultErrorActionIndex");

_LIT(KCfgErrorAction, "ErrorAction");
_LIT(KCfgErrorActionError, ".Error");
_LIT(KCfgErrorActionAction, ".Action");
_LIT(KCfgErrorActionRetries, ".Retries");
_LIT(KCfgErrorActionRetrySpacing, ".RetrySpacing");
_LIT(KCfgErrorActionMaxRetries, ".MaxRetries");

const TInt KMaxSectionLength = 100;

void CMsvSendErrorActionsStep::SetConfigurationL()
	{	
	TBuf<KMaxSectionLength> sectionName;	
	TInt count = 0;
	TInt index = 0;
	if(GetIntFromConfig(ConfigSection(), KCfgErrorActionsTotal, count))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: Actions Total: [%d]."), count);
		}
	if(GetIntFromConfig(ConfigSection(), KCfgDefaultErrorActionIndex, index))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: Default error action index: [%d]."), index);
		}

	
	TInt i=0;	
	while(i < count)
		{		
		TMsvSendErrorAction action;
		sectionName.Copy(KCfgErrorAction);
		sectionName.AppendNum(i);
		TInt pos = sectionName.Length();
		sectionName.Append(KCfgErrorActionError);
		if(GetIntFromConfig(ConfigSection(), sectionName, action.iError))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, action.iError);
			}
		
		TInt variable = 0;
		sectionName.Replace(pos, KCfgErrorActionError().Length(), KCfgErrorActionAction);
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iAction = static_cast<TMsvSendAction>(variable);	
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionAction().Length(), KCfgErrorActionRetries);			
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iRetries = static_cast<TMsvSendRetries>(variable);
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionRetries().Length(), KCfgErrorActionRetrySpacing);	
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iRetrySpacing = static_cast<TMsvSendRetrySpacing>(variable);
		
		sectionName.Replace(pos, KCfgErrorActionRetrySpacing().Length(), KCfgErrorActionMaxRetries);
		TInt maxRetry = 0;
		if(GetIntFromConfig(ConfigSection(), sectionName, maxRetry))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName,maxRetry);
			}
		action.SetMaxRetries((TInt16)maxRetry);
		
		iErrorActions->AddSendErrorActionL(action);
		
		if(i == index)
			{
			iErrorActions->SetDefault(action);
			}
		++i;					 					
		}
		
	}
	

