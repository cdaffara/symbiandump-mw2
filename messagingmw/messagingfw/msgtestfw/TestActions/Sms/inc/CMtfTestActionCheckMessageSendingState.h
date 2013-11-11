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


#ifndef __CMTF_TEST_ACTION_CHECK_MESSAGE_SENDINGSTATE_H__
#define __CMTF_TEST_ACTION_CHECK_MESSAGE_SENDINGSTATE_H__

#include "CMtfTestAction.h"

class CMsvEntry;
class MMsvEntryObserver;
class RTimer;

_LIT(KTestActionCheckMessageSendingState,"CheckMessageSendingState");

/**
This test action waits for the expected message states.

It checks for the following message states: Failed, SendingState, Scheduled,
and PendingConditions.  The action first gets the settings (expected states and 
time out value) from the configuration.  It then listens on the target message 
entry for any entry events.  If the message states match the expected states within
the time out limit, the action passes.  Otherwise, the action fails.

@internalTechnology
*/
class CMtfTestActionCheckMessageSendingState : public CMtfTestAction,public MMsvEntryObserver
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCheckMessageSendingState(); 

public:
	//method from CMtfTestAction
	virtual void ExecuteActionL();

protected:
	//methods from CActive
	virtual void DoCancel();
	virtual void RunL();
	
private:
	//method from MMsvEntryObserver
	virtual void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	
	TBool IsMessageStateMet();
	CMtfTestActionCheckMessageSendingState(CMtfTestCase& aTestCase);

private:
	RTimer		iExpireTimer;
	CMsvEntry* 	iMsvEntry;
			
	TInt 		iExpectedSendingState;
	TInt 		iExpectedPendingConditionMet;
	TInt		iExpectedFailed;
	TInt		iExpectedScheduled;
	};

#endif //__CMTF_TEST_ACTION_CHECK_MESSAGE_SENDINGSTATE_H__
