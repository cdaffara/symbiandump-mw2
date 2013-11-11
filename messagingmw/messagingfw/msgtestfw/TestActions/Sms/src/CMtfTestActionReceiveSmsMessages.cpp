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
// ReceiveSmsMessages
// [Action Parameters]
// CMsvSession  Session		<input>: Reference to the session.
// TInt		    Counter     <input>: Value of expected number of messages
// [Action Description]
// Receives a specified number of sms messages. It waits for a new
// message to appear in the inbox and decrements a count. It completes 
// once the required number of messages has been received.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::AddObserverL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <smut.h>
#include <msvapi.h>

#include "CMtfTestActionReceiveSmsMessages.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionReceiveSmsMessages::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
 	CMtfTestActionReceiveSmsMessages* self = new (ELeave) CMtfTestActionReceiveSmsMessages(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionReceiveSmsMessages::CMtfTestActionReceiveSmsMessages(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

CMtfTestActionReceiveSmsMessages::~CMtfTestActionReceiveSmsMessages()
	{
	 Cancel();
	 iMsventry->RemoveObserver(*this);
	 delete iMsventry;
	}

void CMtfTestActionReceiveSmsMessages::ExecuteActionL()
	{
    TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionReceiveSmsMessages);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	iNumberOfMessages = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	iMsventry = CMsvEntry::NewL(*paramSession,KMsvGlobalInBoxIndexEntryId,TMsvSelectionOrdering());
	iMsventry->AddObserverL(*this);
	iStatus = KRequestPending; 
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionReceiveSmsMessages);
	}

void CMtfTestActionReceiveSmsMessages::DoCancel()
	{
	TRequestStatus *status=&iStatus;
    User::RequestComplete(status,KErrCancel);
	}

 void CMtfTestActionReceiveSmsMessages::RunL()
	{
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionReceiveSmsMessages::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /* aArg2*/, TAny* /*aArg3*/)
/**
 * Called by CMsvEntry when a messaging event has occurred. It is used here to find out if any new messages have been created.
*/
	{
	CMsvEntrySelection* entries = NULL;
	switch (aEvent)
		{
		case EMsvNewChildren:
			if(aArg1 == NULL)
				{
				User::Leave(KErrGeneral);
				}
			else{
				entries = static_cast<CMsvEntrySelection*>(aArg1);
				}
			break;
              
		default:
			User::Leave(KErrGeneral);
		}
    
	iNumberOfMessages-=entries->Count(); 
    if(iNumberOfMessages>0)
		{
		return;
		} 
	else if(iNumberOfMessages == 0)
		{
        TRequestStatus *status=&iStatus;
		User::RequestComplete(status,KErrNone);
		}
	else if(iNumberOfMessages<0)
		{
		User::Leave(KErrGeneral);
		}
	}



