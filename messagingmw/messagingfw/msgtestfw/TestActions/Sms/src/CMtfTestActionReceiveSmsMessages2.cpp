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
// ReceiveSmsMessages2
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

#include "CMtfTestActionReceiveSmsMessages2.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionReceiveSmsMessages2::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
 	CMtfTestActionReceiveSmsMessages2* self = new (ELeave) CMtfTestActionReceiveSmsMessages2(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionReceiveSmsMessages2::CMtfTestActionReceiveSmsMessages2(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

CMtfTestActionReceiveSmsMessages2::~CMtfTestActionReceiveSmsMessages2()
	{
	}


void CMtfTestActionReceiveSmsMessages2::StopReceiveSmsMessages2()
	{
	//Cancel();
	iMsventry->RemoveObserver(*this);
	delete iMsventry;
	}


void CMtfTestActionReceiveSmsMessages2::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionReceiveSmsMessages2::ExecuteActionL IN"));

    CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	iNumberOfMessages = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	iMsventry = CMsvEntry::NewL(*paramSession,KMsvGlobalInBoxIndexEntryId,TMsvSelectionOrdering());
	iMsventry->AddObserverL(*this);
	iStatus = KRequestPending; 
	
	//StoreParameterL<CMtfTestAction>(TestCase(), (*this), ActionParameters().Parameter(2));
	
	CActiveScheduler::Add(this);
	SetActive();

	TestCase().Logger().Write(_L("CMtfTestActionReceiveSmsMessages2::ExecuteActionL OUT"));
	}

void CMtfTestActionReceiveSmsMessages2::DoCancel()
	{
	TRequestStatus *status=&iStatus;
    User::RequestComplete(status,KErrCancel);
	}

 void CMtfTestActionReceiveSmsMessages2::RunL()
	{
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionReceiveSmsMessages2::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /* aArg2*/, TAny* /*aArg3*/)
/**
 * Called by CMsvEntry when a messaging event has occurred. It is used here to find out if any new messages have been created.
*/
	{
	TestCase().Logger().Write(_L("CMtfTestActionReceiveSmsMessages2::HandleEntryEventL IN"));

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

	TestCase().Logger().WriteFormat(_L("CMtfTestActionReceiveSmsMessages2::HandleEntryEventL entries %d") , entries->Count());
    
	iNumberOfMessages-=entries->Count(); 

	TestCase().Logger().WriteFormat(_L("CMtfTestActionReceiveSmsMessages2::HandleEntryEventL entries remaining %d") , iNumberOfMessages);
	
    if(iNumberOfMessages>0)
		{
		return;
		} 
	else if(iNumberOfMessages == 0)
		{
        TRequestStatus *status=&iStatus;
		StopReceiveSmsMessages2();
		User::RequestComplete(status,KErrNone);		
		}
	else if(iNumberOfMessages<0)
		{
		User::Leave(KErrGeneral);
		}
	}



