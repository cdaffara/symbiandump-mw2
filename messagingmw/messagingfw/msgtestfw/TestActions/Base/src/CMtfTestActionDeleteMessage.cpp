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
// DeleteMessage
// [Action Parameters]
// Session   <input>: Reference to the session.
// MessageId <input>: Value of the message id.
// [Action Description]
// Deletes the message.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::DeleteL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionDeleteMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteMessage* self = new (ELeave) CMtfTestActionDeleteMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteMessage::CMtfTestActionDeleteMessage(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteMessage::~CMtfTestActionDeleteMessage()
	{
	}


void CMtfTestActionDeleteMessage::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteMessage);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramMessageId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramMessageId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramMessageId);
	entry->SetEntryL(entry->Entry().Parent());

	if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
		{
		entry->DeleteL(paramMessageId);
		DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = entry->DeleteL(paramMessageId,iStatus);
		SetActive();
		}
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteMessage);
	}


void CMtfTestActionDeleteMessage::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDeleteMessage::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);
	DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	TestCase().ActionCompletedL(*this);
	}

