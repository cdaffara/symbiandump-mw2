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
//

/**
 @file
*/

#include "CMtfTestCaseCreateSendReceiveMms.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionGetBaseConstants.h"
#include "CMtfTestActionCreateSession.h"
#include "CMtfTestActionResetMessageStore.h"
#include "CMtfTestActionCreateRegistry.h"
#include "CMtfTestActionCopyEntry.h"
#include "CMtfTestActionFindEntryByName.h"
#include "CMtfTestActionDeleteChildren.h"
#include "CMtfTestActionCheckChildrenCount.h"
#include "CMtfTestActionCreateChildrenSelection.h"
#include "CMtfTestActionCopySelection.h"

#include "CMtfTestActionInitNtRas.h"
#include "CMtftestActionSleep.h"

#include "CMtfTestActionCreateMmsService.h"
#include "CMtfTestActionCreateMmsMtm.h"
#include "CMtfTestActionSetMmsServiceParameters.h"
#include "CMtfTestActionCreateMmsMessageFromSmilFile.h"

_LIT(KSession,"session");

_LIT(KInboxFolderId,	"inboxFolderId");
_LIT(KOutboxFolderId,	"outboxFolderId");
_LIT(KDraftsFolderId,	"draftsFolderId");
_LIT(KSentFolderId,		"sentFolderId");
_LIT(KDeletedFolderId,	"deletedFolderId");

_LIT(KClientMtmRegistry,"clientMtmRegistry");

_LIT(KMmsMtm,	 			"MmsMtm");
_LIT(KMmsServiceId,			"MmsServiceId");
_LIT(KMmsMessageId,			"MmsMessageId");
_LIT(KMmsDetails,			"\"MmsService\"");

_LIT(KZero,		"0");


CMtfTestCase* CMtfTestCaseCreateSendReceiveMms::NewL(const CMtfTestServer& aTestServer)
	{
	CMtfTestCaseCreateSendReceiveMms* self = new (ELeave) CMtfTestCaseCreateSendReceiveMms(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMtfTestCaseCreateSendReceiveMms::CMtfTestCaseCreateSendReceiveMms(const CMtfTestServer& aTestServer)
	: CMtfTestCase(aTestServer,EFalse,ETrue)
	{
	}


void CMtfTestCaseCreateSendReceiveMms::ConstructL()
	{
	CMtfTestCase::ConstructL();
	}


void CMtfTestCaseCreateSendReceiveMms::SetupTestCaseL()
	{
	// Get constants and enums.
	CMtfTestActionGetBaseConstants::NewL(*this,CMtfTestActionParameters::NewL(KInboxFolderId,KOutboxFolderId,KDraftsFolderId,KSentFolderId,KDeletedFolderId));

	// Init NTRAS.
	CMtfTestActionInitNtRas::NewL(*this,CMtfTestActionParameters::NewL());

	// Create the session and reset the message store.
	CMtfTestActionCreateSession::NewL(*this,CMtfTestActionParameters::NewL(KSession));
	CMtfTestActionResetMessageStore::NewL(*this,CMtfTestActionParameters::NewL(KSession));

	// Create the MMS service
	CMtfTestActionCreateMmsService::NewL(*this,CMtfTestActionParameters::NewL(KSession,_,_,_,_,KMmsDetails,KMmsServiceId));

	// Create MMS client MTM.
	CMtfTestActionCreateRegistry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KClientMtmRegistry));
	CMtfTestActionCreateMmsMtm::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KClientMtmRegistry,KMmsMtm));

	// Set MMS service parameters.
	CMtfTestActionSetMmsServiceParameters::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,_));

	// Get Mms server Inbox id, delete all entries and verify that it has no entries.
	CMtfTestActionDeleteChildren::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId));
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KZero));
	}


void CMtfTestCaseCreateSendReceiveMms::ExecuteTestCaseL()
	{
	// Create the message and send.
	CMtfTestActionCreateMmsMessageFromSmilFile::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KOutboxFolderId,_,_,KMmsMessageId));
	CMtfTestActionCopyEntry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsMessageId,KMmsServiceId));

	// Wait till message arrives back.
//	CMtfTestActionSleep::NewL(*this,CMtfTestActionParameters::NewL(KSixty));

	// Copy Mms service entries to local Inbox.
//	CMtfTestActionCreateChildrenSelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KSelection));
//	CMtfTestActionCopySelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSelection,KInboxFolderId));
	}


void CMtfTestCaseCreateSendReceiveMms::CheckPostConditionsL()
	{
	// Check if MMS service has 1 entry.
//	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KOne));

	// Check if MMS service entry is the one we have sent.
//	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KMmsServiceId,KMmsMessageDescription,KMmsMessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Inbox has 1 entry.
//	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KOne));

	// Check if local Inbox entry is the one we have sent.
//	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KMmsMessageDescription,KInboxMessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Sent has 1 entry.
//	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KOne));

	// Check if local Sent entry is the one we have sent.
//	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KMmsMessageDescription,KSentMessageId));
// COMPARE CONTENT OF THE MESSAGE
	}


