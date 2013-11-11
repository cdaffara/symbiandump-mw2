// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MsvSysAgentCondAction.CPP
// 
//

#ifdef _DEBUG
#undef _MSG_NO_LOGGING
#endif

#include <e32base.h>
#include <e32std.h>

#include <e32property.h>

#include <msvsenderroraction.h>
#include <msvschedulesend.h>
#include <msvsysagentaction.h>
#include <schsend_panic.h>

#ifndef _MSG_NO_LOGGING
#include <flogger.h>
_LIT(KSysAgentActionsLog, "sysagent.txt");
#endif


/**
Default constructor.
*/

EXPORT_C TMsvSysAgentConditionAction::TMsvSysAgentConditionAction()
	{
	iUseDefaultSysAgentAction = ETrue;
	}




/**
Constructor.
*/


EXPORT_C CMsvSysAgentActions::CMsvSysAgentActions()
: CArrayFixFlat<TMsvSysAgentConditionAction>(KMsvSchsendArrayGrowth)

	{
	}


/**
Destructor.
*/


EXPORT_C CMsvSysAgentActions::~CMsvSysAgentActions()
	{
	}


/**
Tests if system agent conditions are met.

@param aErrorAction
On return, error action to use.

@return True if conditions are met, else false.

@leave One of the system-wide error codes
RSystemAgent::Connect() returned an error.
*/

EXPORT_C TBool CMsvSysAgentActions::ConditionsMetL(TMsvSendErrorAction& aErrorAction)
	{
	aErrorAction = iDefault;
	TBool retVal = ETrue;
	

	CArrayFixFlat<TUid>* uids = new (ELeave) CArrayFixFlat<TUid>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(uids);

	CArrayFixFlat<TInt>* states = new (ELeave) CArrayFixFlat<TInt>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(states);

	GetConditionVariablesL(*uids);

	TInt count = uids->Count();
	
    states->ResizeL(count);
	for(TInt i = 0; i < count; ++i)
		{
		TInt val = 0;
		TInt uid = (*uids)[i].iUid;	
	
		User::LeaveIfError(RProperty::Get(KUidSystemCategory, uid, val));

		states->At(i) = val; 
		}

	retVal = CheckStates(*states, aErrorAction);

	CleanupStack::PopAndDestroy(2);

	return retVal;
	}


void CMsvSysAgentActions::GetConditionVariablesL(CArrayFix<TUid>& aUids) const
	{
	TInt count = Count();

	for (TInt curCond = 0; curCond < count; curCond++) //order important, therefore not a while loop
		{
		TMsvSysAgentConditionAction condAction = At(curCond);
		aUids.AppendL(condAction.iCondition.iVariable);
		}
	}


TBool CMsvSysAgentActions::CheckStates(const CArrayFix<TInt>& aStates, TMsvSendErrorAction& aErrorAction) const
	{
	aErrorAction = iDefault;
	TBool retVal = ETrue;
	TBool useDefault = ETrue;
	TInt count = Count();

	for (TInt curCond = 0; curCond < count; curCond++) //order important, therefore not a while loop
		{
		TMsvSysAgentConditionAction condAction = At(curCond);
		TInt state = aStates.At(curCond);
		
#ifndef _MSG_NO_LOGGING
		RFileLogger::WriteFormat(KSchSendLogDir, KSysAgentActionsLog, EFileLoggingModeAppend, _L("Cond: %d, Type %d, Expected %d, Actual %d"), condAction.iCondition.iVariable.iUid, condAction.iCondition.iType, condAction.iCondition.iState, state);
#endif
		
		if (!ConditionOK(condAction, state))
			{
			retVal = EFalse;

			if (useDefault && !condAction.iUseDefaultSysAgentAction)
				{
				aErrorAction = condAction.iErrorAction;
				}
			else if (!useDefault && !condAction.iUseDefaultSysAgentAction)
				{
				if (condAction.iErrorAction.iAction == ESendActionFail)
					{
					aErrorAction = condAction.iErrorAction;
					}
				else if (condAction.iErrorAction.iAction == ESendActionRetryLater && aErrorAction.iAction == ESendActionRetryImmediately)
					{
					aErrorAction = condAction.iErrorAction;
					}
				else if (condAction.iErrorAction.iRetries == ESendRetriesFixed && aErrorAction.iRetries == ESendRetriesInfinite)
					{
					aErrorAction = condAction.iErrorAction;
					}
				else
					{
					//use the current aErrorAction
					}
				}
			else
				{
				//use the default error action (iDefault)
				}

			useDefault &= condAction.iUseDefaultSysAgentAction;
			}
		} //end for

	return retVal;
	}


TBool CMsvSysAgentActions::ConditionOK(const TMsvSysAgentConditionAction& aCondAction, const TInt aState) const
	{
	// If system agent hasn't defined the variable the condition should be satisfied
	if (aState == KErrUnknown)
		return ETrue;

	switch (aCondAction.iCondition.iType)
		{
		case TMsvCondition::EMsvSchSendEquals:
			{
			return (aState == aCondAction.iCondition.iState);
			}
		case TMsvCondition::EMsvSchSendNotEquals:
			{
			return (aState != aCondAction.iCondition.iState);
			}
		case TMsvCondition::EMsvSchSendGreaterThan:
			{
			return (aState > aCondAction.iCondition.iState);
			}
		case TMsvCondition::EMsvSchSendLessThan:
			{
			return (aState < aCondAction.iCondition.iState);
			}


		default:
			gPanic(ESysAgentConditionTypeNotSupported);
			break;
		} //end switch

	return ETrue; //should not be reached
	}
