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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// GetEntry
// [Action Parameters]
// Session			<input>: Reference to the session.
// MsgId			<input>: Value of the message Id.
// Entry			<output>: Value of the created entry.
// [Action Description]
// Provides the entry for message.
// [APIs Used]
// CMsvSession::GetEntryL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionGetEntry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>

CMtfTestAction* CMtfTestActionGetEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetEntry* self = new(ELeave) CMtfTestActionGetEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetEntry::CMtfTestActionGetEntry(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetEntry::~CMtfTestActionGetEntry()
	{
	}

void CMtfTestActionGetEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddGetEntry);
	if(TestCase().TestStepResult() == EPass)
		{
		CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
		TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
		
		CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
		CleanupStack::PushL(entry);

		entry->SetEntryL(messageEntry);
		
		StoreParameterL<CMsvEntry>(TestCase(),*entry,ActionParameters().Parameter(2));
		CleanupStack::Pop(entry);

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddGetEntry);
		}
	TestCase().ActionCompletedL(*this);
	}



