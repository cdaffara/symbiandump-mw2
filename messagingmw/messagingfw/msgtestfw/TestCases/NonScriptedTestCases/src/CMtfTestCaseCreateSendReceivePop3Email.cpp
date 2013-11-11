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

#include "CMtfTestCaseCreateSendReceivePop3Email.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionGetBaseConstants.h"
#include "CMtfTestActionGetEntryPriorityEnums.h"
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

#include "CMtfTestActionCreatePop3Service.h"
#include "CMtfTestActionCreatePop3Mtm.h"
#include "CMtfTestActionSetPop3ServiceParameters.h"
#include "CMtfTestActionConnectPop3Server.h"
#include "CMtfTestActionDisconnectPop3Server.h"

#include "CMtfTestActionCreateSmtpService.h"
#include "CMtfTestActionCreateSmtpMtm.h"
#include "CMtfTestActionSetSmtpServiceParameters.h"
#include "CMtfTestActionCreateSmtpMessageFromEmailFile.h"

_LIT(KSession,"session");

_LIT(KInboxFolderId,	"inboxFolderId");
_LIT(KOutboxFolderId,	"outboxFolderId");
_LIT(KDraftsFolderId,	"draftsFolderId");
_LIT(KSentFolderId,		"sentFolderId");
_LIT(KDeletedFolderId,	"deletedFolderId");

_LIT(KClientMtmRegistry,"clientMtmRegistry");

_LIT(KPop3Mtm,					"Pop3Mtm");
_LIT(KPop3ServiceId,			"Pop3ServiceId");
_LIT(KPop3MessageId,			"imap4MessageId");
_LIT(KPop3MessageDescription,	"\"SimpleEmail\"");
_LIT(KPop3Details,				"\"Pop3Service\"");

_LIT(KSmtpMtm,			"smtpMtm");
_LIT(KSmtpServiceId,	"smtpServiceId");
_LIT(KSmtpMessageId,	"smtpMessageId");
_LIT(KSmtpDetails, 		"\"SmtpService\"");


_LIT(KZero,		"0");
_LIT(KOne,		"1");
	
_LIT(KSelection,"selection");

_LIT(KInboxMessageId,	"inboxMessageId");
_LIT(KSentMessageId,	"sentMessageId");




CMtfTestCase* CMtfTestCaseCreateSendReceivePop3Email::NewL(const CMtfTestServer& aTestServer)
	{
	CMtfTestCaseCreateSendReceivePop3Email* self = new (ELeave) CMtfTestCaseCreateSendReceivePop3Email(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMtfTestCaseCreateSendReceivePop3Email::CMtfTestCaseCreateSendReceivePop3Email(const CMtfTestServer& aTestServer)
	: CMtfTestCase(aTestServer,EFalse,ETrue)
	{
	}


void CMtfTestCaseCreateSendReceivePop3Email::ConstructL()
	{
	CMtfTestCase::ConstructL();
	}


void CMtfTestCaseCreateSendReceivePop3Email::SetupTestCaseL()
	{
	// Get constants and enums.
	CMtfTestActionGetBaseConstants::NewL(*this,CMtfTestActionParameters::NewL(KInboxFolderId,KOutboxFolderId,KDraftsFolderId,KSentFolderId,KDeletedFolderId));

	// Init NTRAS.
	CMtfTestActionInitNtRas::NewL(*this,CMtfTestActionParameters::NewL());

	// Create the session and reset the message store.
	CMtfTestActionCreateSession::NewL(*this,CMtfTestActionParameters::NewL(KSession));
	CMtfTestActionResetMessageStore::NewL(*this,CMtfTestActionParameters::NewL(KSession));

	// Create the POP3 and SMTP services
	CMtfTestActionCreatePop3Service::NewL(*this,CMtfTestActionParameters::NewL(KSession,_,_,_,_,KPop3Details,KPop3ServiceId));
	CMtfTestActionCreateSmtpService::NewL(*this,CMtfTestActionParameters::NewL(KSession,_,_,_,_,KSmtpDetails,KSmtpServiceId));

	// Create POP3 and SMTP client MTMs.
	CMtfTestActionCreateRegistry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KClientMtmRegistry));
	CMtfTestActionCreatePop3Mtm::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,KClientMtmRegistry,KPop3Mtm));
	CMtfTestActionCreateSmtpMtm::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,KClientMtmRegistry,KSmtpMtm));

	// Set POP3 and SMTP service parameters.
	CMtfTestActionSetPop3ServiceParameters::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,_));
	CMtfTestActionSetSmtpServiceParameters::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,_));

	// Connect and sync with POP3 server.
	CMtfTestActionConnectPop3Server::NewL(*this,CMtfTestActionParameters::NewL(KPop3ServiceId,KPop3Mtm));

	// Delete all POP3 service entries and verify that it has no entries.
	CMtfTestActionDeleteChildren::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId));
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,KZero));
	
	// Disconnect from POP3 server.
	CMtfTestActionDisconnectPop3Server::NewL(*this,CMtfTestActionParameters::NewL(KPop3ServiceId,KPop3Mtm));
	}


void CMtfTestCaseCreateSendReceivePop3Email::ExecuteTestCaseL()
	{
	// Create the email and send.
	CMtfTestActionCreateSmtpMessageFromEmailFile::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,KOutboxFolderId,_,KSmtpMessageId));
	CMtfTestActionCopyEntry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpMessageId,KSmtpServiceId));

	// Connect and sync with POP3 server.
	CMtfTestActionConnectPop3Server::NewL(*this,CMtfTestActionParameters::NewL(KPop3ServiceId,KPop3Mtm));

	// Copy POP3 service entries to local Inbox.
	CMtfTestActionCreateChildrenSelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,KSelection));
	CMtfTestActionCopySelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSelection,KInboxFolderId));

	// Disconnect from POP3 server.
	CMtfTestActionDisconnectPop3Server::NewL(*this,CMtfTestActionParameters::NewL(KPop3ServiceId,KPop3Mtm));
	}


void CMtfTestCaseCreateSendReceivePop3Email::CheckPostConditionsL()
	{
	// Check if IPOP3 service has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,KOne));

	// Check if POP3 service entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KPop3ServiceId,KPop3MessageDescription,KPop3MessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Inbox has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KOne));

	// Check if local Inbox entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KPop3MessageDescription,KInboxMessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Sent has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KOne));

	// Check if local Sent entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KPop3MessageDescription,KSentMessageId));
// COMPARE CONTENT OF THE MESSAGE
	}


