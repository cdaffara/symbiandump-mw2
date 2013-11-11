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
// PruneMessage
// [Action Parameters]
// Session   <input>: Reference to the session.
// Selection <input>: Selection of messages to prune
// [Action Description]
// Unpopulates all the email messages under a service
// [APIs Used]
// CImCacheManager
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionPruneMessages.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <cacheman.h>


class CPruneTester: public CImCacheManager
	{
	public:
		static CPruneTester* NewL(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus);

	private:
		CPruneTester(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus);
		TBool Filter() const;
	};

CPruneTester::CPruneTester(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus) : CImCacheManager(aSession, aObserverRequestStatus)
	{
	}

CPruneTester* CPruneTester::NewL(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus)
	{
	CPruneTester* self = new (ELeave) CPruneTester(aSession, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

TBool CPruneTester::Filter() const
	{
	return ETrue;
	}



CMtfTestAction* CMtfTestActionPruneMessages::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionPruneMessages* self = new (ELeave) CMtfTestActionPruneMessages(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionPruneMessages::CMtfTestActionPruneMessages(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

CMtfTestActionPruneMessages::~CMtfTestActionPruneMessages()
	{
	}


void CMtfTestActionPruneMessages::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionPruneMessages);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	CMsvEntrySelection* selection = ObtainParameterReferenceL<CMsvEntrySelection> (TestCase(), ActionParameters().Parameter(1)); 


	TRequestStatus observerStatus;
	iPruneTester = CPruneTester::NewL(*paramSession, observerStatus);	
	iPruneTester->StartL(*selection, iStatus);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionPruneMessages);
	}


void CMtfTestActionPruneMessages::DoCancel()
	{
	iPruneTester->Cancel();
	}


void CMtfTestActionPruneMessages::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	delete iPruneTester;
	iPruneTester=NULL;
	TestCase().ActionCompletedL(*this);
	}
