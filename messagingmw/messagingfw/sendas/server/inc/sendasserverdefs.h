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

#ifndef __SENDASSERVERDEFS_H__
#define __SENDASSERVERDEFS_H__

// Types and Classes Shared between the server and client interfaces.

/**
@internalComponent
@released
*/
const TInt KSendAsServerVersionMajor = 1;

/**
@internalComponent
@released
*/
const TInt KSendAsServerVersionMinor = 0;

/**
@internalComponent
@released
*/
const TInt KSendAsServerVersionBuild = 0;

/**
@internalComponent
@released
*/
enum TSendAsSessionMethods
	{
	ESendAsSessionMethodBase = 0x1000,	// start of session methods

	// Message Type Filter Messages
	ESASGetMessageTypeListLength,	// request the size required to receive a filtered message type list, value written into TInt pointed to by param 0.
	ESASGetMessageTypes,			// fetch the list.  Server copies the data into the descriptor pointed to by param 0.
	ESASSetFilter,					// fills a TSendAsMessageTypeFilter and sends it in parameter 0.
	ESASClearFilter,				// remove the message type filter
	
	// Message Account Methods
	ESASGetAccountListLength,		// request the size of the buffer required to transfer the account list
	ESASGetAccountList,				// ask the server to fill the supplied buffer with the account list
	ESASCreateAttachment,			// ask the server to create an attachment file
	
	// SubSession Creation
	ESASCreateSubSession,
	
	ESendAsSessionMethodMax			// end of session methods
	};

/**
@internalComponent
@released
*/
enum TSendAsMessageMethods
	{
	ESendAsMessageMethodBase = 0x2000,	// start of message methods
	ESAMCreateForAccount,
	ESAMCreateByType,
	ESAMDelete,
	ESAMSetBodyFirst,
	ESAMSetBodyNext,
	ESAMSetSubject,
	ESAMSetBioType,
	ESAMAddRecipient,
	ESAMAddRecipientWithAlias,
	ESAMTransferAttachmentFile,
	ESAMAddAttachment,
	ESAMAddAttachmentWithType,
	ESAMAddLinkedAttachment,
	ESAMAddLinkedAttachmentWithType,
	ESAMCreateAttachment,
	ESAMCreateAttachmentWithType,
	ESAMCancel,
	ESAMLaunchEditor,
	ESAMDestroySubSession,
	ESendAsMessageMethodSendBase,	// methods after this have a custom security policy applied to them
	ESAMSendMessage,
	ESAMSendMessageBackground,
	ESAMSendMessageConfirmed,
	ESAMSendMessageConfirmedBackground,
	ESAMGetProgress,
	ESAMSaveMessage,
	ESendAsMessageMethodMax,	
	ESAMSetCharacterSet,
	ESAMCharSetInfoForAttachment			// end of message methods
	};

#endif	// __SENDASSERVERDEFS_H__
