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
// GetBaseConstants
// [Action Parameters]
// InboxFolderId   <output>: Value of the local inbox folder id.
// OutboxFolderId  <output>: Value of the local outbox folder id.
// DraftsFolderId  <output>: Value of the local drafts folder id.
// SentFolderId    <output>: Value of the local sent folder id.
// DeletedFolderId <output>: Value of the local deleted folder id.
// [Action Description]
// Initialises base related constants.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetBaseConstants.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionGetBaseConstants::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetBaseConstants* self = new (ELeave) CMtfTestActionGetBaseConstants(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetBaseConstants::CMtfTestActionGetBaseConstants(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetBaseConstants::~CMtfTestActionGetBaseConstants()
	{
	}

 
void CMtfTestActionGetBaseConstants::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetBaseConstants);
	TMsvId paramInboxFolderId = KMsvGlobalInBoxIndexEntryId;
	StoreParameterL<TMsvId>(TestCase(),paramInboxFolderId,ActionParameters().Parameter(0));

	TMsvId paramOutboxFolderId = KMsvGlobalOutBoxIndexEntryId;
	StoreParameterL<TMsvId>(TestCase(),paramOutboxFolderId,ActionParameters().Parameter(1));

	TMsvId paramDraftsFolderId = KMsvDraftEntryId;
	StoreParameterL<TMsvId>(TestCase(),paramDraftsFolderId,ActionParameters().Parameter(2));

	TMsvId paramSentFolderId = KMsvSentEntryId;
	StoreParameterL<TMsvId>(TestCase(),paramSentFolderId,ActionParameters().Parameter(3));

	TMsvId paramDeletedFolderId = KMsvDeletedEntryFolderEntryId;
	StoreParameterL<TMsvId>(TestCase(),paramDeletedFolderId,ActionParameters().Parameter(4));

	if (ActionParameters().Count() > 5)
		{	// Add extra default params for root index, without having to change all the ini files.
		TMsvId paramRootIndexEntryId = KMsvRootIndexEntryId;
		StoreParameterL<TMsvId>(TestCase(),paramRootIndexEntryId,ActionParameters().Parameter(5));
		}
	

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetBaseConstants );
	TestCase().ActionCompletedL(*this);
	}


