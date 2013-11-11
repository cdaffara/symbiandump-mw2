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


#include "CMtfTestCaseCreateSendReceiveImap4Email.h"
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

#include "CMtfTestActionCreateImap4Service.h"
#include "CMtfTestActionCreateImap4Mtm.h"
#include "CMtfTestActionSetImap4ServiceParameters.h"
#include "CMtfTestActionConnectImap4Server.h"
#include "CMtfTestActionDisconnectImap4Server.h"
#include "CMtfTestActionDoFullImap4Synchronisation.h"

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

_LIT(KImap4Mtm,					"imap4Mtm");
_LIT(KImap4ServiceId,			"imap4ServiceId");
_LIT(KImap4InboxFolder,			"\"Inbox\"");
_LIT(KImap4FolderId,			"imap4FolderId");
_LIT(KImap4MessageId,			"imap4MessageId");
_LIT(KImap4MessageDescription,	"\"SimpleEmail\"");
_LIT(KImap4Details,				"\"Imap4Service\"");

_LIT(KSmtpMtm,			"smtpMtm");
_LIT(KSmtpServiceId,	"smtpServiceId");
_LIT(KSmtpMessageId,	"smtpMessageId");
_LIT(KSmtpDetails, 		"\"SmtpService\"");


_LIT(KZero,	"0");
_LIT(KOne, 	"1");
	
_LIT(KSelection,"selection");

_LIT(KInboxMessageId,	"inboxMessageId");
_LIT(KSentMessageId,	"sentMessageId");


CMtfTestCase* CMtfTestCaseCreateSendReceiveImap4Email::NewL(const CMtfTestServer& aTestServer)
	{
	CMtfTestCaseCreateSendReceiveImap4Email* self = new (ELeave) CMtfTestCaseCreateSendReceiveImap4Email(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMtfTestCaseCreateSendReceiveImap4Email::CMtfTestCaseCreateSendReceiveImap4Email(const CMtfTestServer& aTestServer)
	: CMtfTestCase(aTestServer,EFalse,ETrue)
	{
	}


void CMtfTestCaseCreateSendReceiveImap4Email::ConstructL()
	{
	CMtfTestCase::ConstructL();
	}


void CMtfTestCaseCreateSendReceiveImap4Email::SetupTestCaseL()
	{
	// Get constants and enums.
	CMtfTestActionGetBaseConstants::NewL(*this,CMtfTestActionParameters::NewL(KInboxFolderId,KOutboxFolderId,KDraftsFolderId,KSentFolderId,KDeletedFolderId));

	// Init NTRAS.
	CMtfTestActionInitNtRas::NewL(*this,CMtfTestActionParameters::NewL());

	// Create the session and reset the message store.
	CMtfTestActionCreateSession::NewL(*this,CMtfTestActionParameters::NewL(KSession));
	CMtfTestActionResetMessageStore::NewL(*this,CMtfTestActionParameters::NewL(KSession));

	// Create the IMAP4 and SMTP services
	CMtfTestActionCreateImap4Service::NewL(*this,CMtfTestActionParameters::NewL(KSession,_,_,_,_,KImap4Details,KImap4ServiceId));
	CMtfTestActionCreateSmtpService::NewL(*this,CMtfTestActionParameters::NewL(KSession,_,_,_,_,KSmtpDetails,KSmtpServiceId));

	// Create IMAP4 and SMTP client MTMs.
	CMtfTestActionCreateRegistry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KClientMtmRegistry));
	CMtfTestActionCreateImap4Mtm::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4ServiceId,KClientMtmRegistry,KImap4Mtm));
	CMtfTestActionCreateSmtpMtm::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,KClientMtmRegistry,KSmtpMtm));

	// Set IMAP4 and SMTP service parameters.
	CMtfTestActionSetImap4ServiceParameters::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4ServiceId,_));
	CMtfTestActionSetSmtpServiceParameters::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,_));

	// Connect and do full sync with IMAP4 server.
	CMtfTestActionConnectImap4Server::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));
	CMtfTestActionDoFullImap4Synchronisation::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));

	// Get IMAP4 server Inbox id, delete all entries and verify that it has no entries.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4ServiceId,KImap4InboxFolder,KImap4FolderId));
	CMtfTestActionDeleteChildren::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4FolderId));
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4FolderId,KZero));
	
	// Disconnect from IMAP4 server.
	CMtfTestActionDisconnectImap4Server::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));
	}


void CMtfTestCaseCreateSendReceiveImap4Email::ExecuteTestCaseL()
	{
	// Create the email and send.
	CMtfTestActionCreateSmtpMessageFromEmailFile::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpServiceId,KOutboxFolderId,_,KSmtpMessageId));
	CMtfTestActionCopyEntry::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSmtpMessageId,KSmtpServiceId));

	// Connect and full sync with IMAP4 server.
	CMtfTestActionConnectImap4Server::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));
	CMtfTestActionDoFullImap4Synchronisation::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));

	// Copy IMAP4 server Inbox entries to local Inbox.
	CMtfTestActionCreateChildrenSelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4FolderId,KSelection));
	CMtfTestActionCopySelection::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSelection,KInboxFolderId));

	// Disconnect from IMAP4 server.
	CMtfTestActionDisconnectImap4Server::NewL(*this,CMtfTestActionParameters::NewL(KImap4ServiceId,KImap4Mtm));
	}


void CMtfTestCaseCreateSendReceiveImap4Email::CheckPostConditionsL()
	{
	// Check if IMAP4 server Inbox has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4FolderId,KOne));

	// Check if IMAP4 server Inbox entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KImap4FolderId,KImap4MessageDescription,KImap4MessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Inbox has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KOne));

	// Check if local Inbox entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KInboxFolderId,KImap4MessageDescription,KInboxMessageId));
// COMPARE CONTENT OF THE MESSAGE

	// Check if local Sent has 1 entry.
	CMtfTestActionCheckChildrenCount::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KOne));

	// Check if local Sent entry is the one we have sent.
	CMtfTestActionFindEntryByName::NewL(*this,CMtfTestActionParameters::NewL(KSession,KSentFolderId,KImap4MessageDescription,KSentMessageId));
// COMPARE CONTENT OF THE MESSAGE
	}


