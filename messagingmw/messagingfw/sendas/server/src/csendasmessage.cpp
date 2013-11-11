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

#include "csendasmessage.h"

#include <u32std.h>
#include <mtclbase.h>
#include <msvuids.h>
#include <mtmuids.h>
#include <txtrich.h>
#include <mturutils.h>

#include "csendassender.h"
#include "csendassession.h"
#include "csendasattachment.h"
#include "csendasmtmmanager.h"
#include "sendasserverdefs.h"
#include "csendasactivecontainer.h"
#include "csendaseditwatcher.h"
#include "tsendasserverpanic.h"

// max index for incoming RMessage parameters
const TInt KSendAsMaxMessageIndex = 3;

CSendAsMessage* CSendAsMessage::NewL(CSendAsSession& aSession)
	{
	CSendAsMessage* self = new(ELeave) CSendAsMessage(aSession);
	return self;
	}
	
CSendAsMessage::~CSendAsMessage()
	{
	delete iSender;
	delete iClientMtm;
	delete iAttachment;
	}
	
CSendAsMessage::CSendAsMessage(CSendAsSession& aSession)
: iSession(aSession)
	{
	// the data members of iProgress are defined such that when they
	// are zero initialised (like being embedded in a C-Class) this is
	// the same as the default progress structure.
	}

/** Dispatch the subsession message to the correct handler.
 
The return value indicates to the owner session if we are doing an
asynchronous operation.  ETrue is async: EFalse is sync.
Errors can only be returned by leaving.  This method should be encapsulated
in a trap harness to prevent the server AO from leaving during a ServiceL.

@param	aMesasge
The IPC message.

@return
A value of true if the request is asynchronous, otherwise a value of false
indicating that the request is synchronous.
*/
TBool CSendAsMessage::DoSubSessionServiceL(const RMessage2& aMessage)
	{
	TBool async = EFalse;

	switch( aMessage.Function() )
		{
	case ESAMCreateForAccount:
		{
		CreateMessageForAccountL(aMessage);
		} break;
	case ESAMCreateByType:
		{
		CreateMessageByTypeL(aMessage);
		} break;
	case ESAMGetProgress:
		{
		ProgressL(aMessage);
		} break;
	case ESAMDelete:
		{
		DeleteMessageL(aMessage);
		} break;
	case ESAMSetBodyFirst:
		{
		SetBodyTextL(aMessage, ETrue);
		} break;
	case ESAMSetBodyNext:
		{
		SetBodyTextL(aMessage, EFalse);
		} break;
	case ESAMSetSubject:
		{
		SetSubjectL(aMessage);
		} break;
	case ESAMSetBioType:
		{
		SetBioTypeL(aMessage);
		} break;
	case ESAMAddRecipient:
		{
		AddRecipientL(aMessage);
		} break;
	case ESAMAddRecipientWithAlias:
		{
		AddRecipientWithAliasL(aMessage);
		} break;
	case ESAMTransferAttachmentFile:
		{
		TransferAttachmentFileL(aMessage);
		} break;
	case ESAMAddAttachment:
		{
		AddAttachmentL(aMessage);
		async = ETrue;
		} break;
	case ESAMAddAttachmentWithType:
		{
		AddAttachmentWithMimeTypeL(aMessage);
		async = ETrue;
		} break;
	case ESAMAddLinkedAttachment:
		{
		AddLinkedAttachmentL(aMessage);
		async = ETrue;
		} break;
	case ESAMAddLinkedAttachmentWithType:
		{
		AddLinkedAttachmentWithMimeTypeL(aMessage);
		async = ETrue;
		} break;
	case ESAMCreateAttachment:
		{
		// Message will complete upon transfering ownership of file to client.
		CreateAttachmentL(aMessage);
		async = ETrue;
		} break;
	case ESAMCreateAttachmentWithType:
		{
		// Message will complete upon transfering ownership of file to client.
		CreateAttachmentWithMimeTypeL(aMessage);
		async = ETrue;
		} break;
	case ESAMCancel:
		{
		Cancel();
		} break;
	case ESAMLaunchEditor:
		{
		LaunchEditorL(aMessage);
		} break;
	case ESAMSendMessageConfirmed:
		{
		async = ETrue;
		// drop through to next case...
		}
	case ESAMSendMessageConfirmedBackground:
		{
		SendMessageL(aMessage, ETrue, !async);
		} break;
	case ESAMSendMessage:
		{
		async = ETrue;
		// drop through to next case...
		}
	case ESAMSendMessageBackground:
		{
		SendMessageL(aMessage, EFalse, !async);
		} break;
	case ESAMSaveMessage:
		{
		__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage));
		__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );
		
		DoSaveMessageL();
		} break;
	case ESAMSetCharacterSet:
		{
		// For setting the charset encoding value for the message.
		SetMessageCharacterSetL(aMessage);	
		}break;
	
	case ESAMCharSetInfoForAttachment:
		{
		// For setting the charset encoding value for the message.
		SetCharsetInfoForAttachment(aMessage);	
		}break;
	default:
		PanicClientL(aMessage, ESendAsClientPanicBadRequest);
		break;
		}
	return async;
	}
	
/** Cancels any asynchronous activity being done by the message.
*/
void CSendAsMessage::CancelMessage()
	{
	// cancelling is a safe thing to do, like closing.
	switch( iState )
		{
	case EAddingOrCreatingAttachment:
		{
		__ASSERT_DEBUG( iAttachment != NULL, User::Invariant() );
		
		// cancel and then delete the attachment object.
		iAttachment->Cancel();
		
		delete iAttachment;
		iAttachment = NULL;
		} break;
	case ESendingMessage:
		{
		__ASSERT_DEBUG( iSender != NULL, User::Invariant() );

		// cancel and then delete the sender object.
		iSender->Cancel();
		
		delete iSender;
		iSender = NULL;
		} break;
	case EIdle:
	case EMessageCreated:
	case ESendingMessageComplete:
	case EMessageDeleted:
	case EPendingClose:
	default:
		// do nothing as cancel is a safe thing...
		break;		
		}
	}

/** Allocate a heap buffer to hold a descriptor parameter and copy it in.

This method leaves if the desciptor is not valid. This method pushes the buffer
onto the cleanup stack and leaves it there when it returns.

@param	aMessage
The IPC message.

@param	aIndex
The index of the parameter to read from the IPC message.

@return
A pointer to an array populated with the descriptor data. A copy of the pointer
is left on the cleanup stack.
*/
HBufC* CSendAsMessage::GetDesParamLC(const RMessage2& aMessage, TInt aIndex)
	{
	__ASSERT_DEBUG((aIndex >=0 && aIndex <=KSendAsMaxMessageIndex), PanicClientL(aMessage, ESendAsClientPanicBadRequestArgument));
	
	TInt len = User::LeaveIfError(aMessage.GetDesLength(aIndex));
	HBufC* buffer = HBufC::NewLC(len);
	TPtr ptr(buffer->Des());
	aMessage.Read(aIndex, ptr);
	return buffer;
	}

/** Allocate a heap buffer to hold a descriptor parameter and copy it in.

This method leaves if the desciptor is not valid. This method pushes the buffer
onto the cleanup stack and leaves it there when it returns.

@param	aMessage
The IPC message.

@param	aIndex
The index of the parameter to read from the IPC message.

@return
A pointer to an array populated with the descriptor data. A copy of the pointer
is left on the cleanup stack.
*/
HBufC8* CSendAsMessage::GetDesParam8LC(const RMessage2& aMessage, TInt aIndex)
	{
	__ASSERT_DEBUG((aIndex >=0 && aIndex <=KSendAsMaxMessageIndex), PanicClientL(aMessage, ESendAsClientPanicBadRequestArgument));
	
	TInt len = User::LeaveIfError(aMessage.GetDesLength(aIndex));
	HBufC8* buffer = HBufC8::NewLC(len);
	TPtr8 ptr(buffer->Des());
	aMessage.Read(aIndex, ptr);
	return buffer;
	}

/**
Create a message in the drafts folder and set up this CSendAsMessage instance to
operate on it.

This method requires either an MTM UID or a valid account ID. 

If the MTM UID is NULL, then the account ID must be valid otherwise the method will
leave. If the account ID is valid, then the MTM of the account is used.

@param	aMtm
The UID of the MTM for the new message.

@param	aAccount
The account ID under which the message is created.

@leave	KErrNotFound
The MTM UID is NULL and the account ID is not valid.
*/
void CSendAsMessage::DoCreateMessageL(TUid aMtm, TSendAsAccount aAccount)
	{
	// new context 
	CMsvEntry* msvEntry = CMsvEntry::NewL(iSession.GetMsvSessionL(), aAccount, TMsvSelectionOrdering());
	CleanupStack::PushL(msvEntry);
	if( aMtm == KNullUid )
		{
		if( aAccount == KMsvRootIndexEntryId || msvEntry->Entry().iType != KUidMsvServiceEntry )
			{
			// not a valid account
			User::Leave(KErrNotFound);
			}
		// get associated mtm uid
		aMtm = msvEntry->Entry().iMtm;
		}

	// create mtm by uid	
	DeleteClientMtm();
	iClientMtm = iSession.GetClientMtmL(aMtm);
	
	// if no account provided, get default for this MTM
	if( aAccount == KMsvRootIndexEntryId )
		{
		aAccount = iClientMtm->DefaultServiceL();
		}
	
	// create message in drafts folder
	msvEntry->SetEntryL(KMsvDraftEntryId);
	
	// mtm takes ownership of entry context	
	CleanupStack::Pop(msvEntry);
	iClientMtm->SetCurrentEntryL(msvEntry);

	// create message
	iClientMtm->CreateMessageL(aAccount);
	iMessageType = aMtm;

	// Change the context of the client MTM to the newly created message entry
	iClientMtm->SwitchCurrentEntryL(iClientMtm->Entry().EntryId());

	// Load the message created so far into the client MTMs cache. This ensures
	// that SendAs will not overwrite any body text added during the
	// CreateMessageL routine (such as signature text).
	// Some MTMs don't actually write anything to store during the
	// CreateMessageL phase, so the LoadMessageL can leave with KErrNotFound.
	// It is safe to trap and ignore this as it just means there is no message
	// body text to be loaded.
	TRAPD(err, iClientMtm->LoadMessageL());

	if (err != KErrNone && err != KErrNotFound)
		{
		User::Leave(err);
		}

	iState = EMessageCreated;
	}

/** Create a new message in the drafts folder, using the specified account.

The account to use is passed from the client in a package buffer, held in
parameter 0 of the message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::CreateMessageForAccountL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EIdle, PanicClientL(aMessage, ESendAsClientPanicSubsessionInUse) );

	TPckgBuf<TSendAsAccount> accountBuf;
	aMessage.Read(0, accountBuf);

	DoCreateMessageL(KNullUid, accountBuf()); 
	}
	
/** Create a new message in the drafts folder, of the specified type.

The message type (a TUid) to create is passed from the client in a package buffer,
held in parameter 0 of the message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::CreateMessageByTypeL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EIdle, PanicClientL(aMessage, ESendAsClientPanicSubsessionInUse) );
	
	// extract mtm uid
	TPckgBuf<TUid> uidBuf;
	aMessage.Read(0, uidBuf);

	// create message using default account for this mtm
	DoCreateMessageL(uidBuf());
	}
	
/** Delete the open message from the drafts folder.
@param aMessage
The IPC message.
*/
void CSendAsMessage::DeleteMessageL(const RMessage2& aMessage)
	{
	// Client code should cancel any outstanding requests prior to deleting
	// the message. Failure to do so will result in the client being panicked.
	switch (iState)
		{
	case EMessageCreated:
		{
		__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );
		
		iClientMtm->Session().RemoveEntry(iClientMtm->Entry().EntryId());
		iState = EMessageDeleted;
		} break;
	case EIdle:
	case ESendingMessageComplete:
	case EPendingClose:
		break;
	case ESendingMessage:
		{
		PanicClientL(aMessage, ESendAsClientPanicSendingMessage);
		} break;
	case EAddingOrCreatingAttachment:
		{
		PanicClientL(aMessage, ESendAsClientPanicAddingCreatingAttachment);
		} break;
	case EMessageDeleted:
	default:
		PanicClientL(aMessage, ESendAsClientPanicMessageAlreadyDeleted);
		break;
		}

	DeleteClientMtm();
	}
	
/** Set the body text of the open message.

The body text is specified using a CRichText object. This object is streamed 
into a descriptor which is passed over the IPC boundary in parameter 0 of the
message. The CRichText object is then internalised by the server.

@param	aMessage
The IPC message.

@param	aFirstChunk
A flag that indicates whether this is the first chunk of rich text received 
from the client.
*/
void CSendAsMessage::SetBodyTextL(const RMessage2& aMessage, TBool aFirstChunk)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

	TInt response = 0;
	User::LeaveIfError(iClientMtm->QueryCapability(KUidMtmQuerySupportedBody, response));

	// This location in the body text at which the text is to be inserted.
	TInt insertLoc = 0;

	if( aFirstChunk )
		{
		// Store the size of the signature text. This is added to the body
		// text on creation of the message by the client MTM.
		// All added body text is to be added before the signature text.
		iSignatureSize = iClientMtm->Body().DocumentLength();
		}
	else
		{
		// insert added text before the signature text.
		insertLoc = iClientMtm->Body().DocumentLength() - iSignatureSize;
		}

	TInt chunkLength = User::LeaveIfError(aMessage.GetDesLength(0));
	HBufC* textBuffer = GetDesParamLC(aMessage, 0);
	// Append the next chunk to the mtm's CRichText object.
	TRAPD(insertError, iClientMtm->Body().InsertL(insertLoc, *textBuffer));
	if( insertError != KErrNone )
		{
		iClientMtm->Body().Reset();
		User::Leave(insertError);
		}
	CleanupStack::PopAndDestroy(textBuffer);
	}

/** Set the subject of the open message.

The subject is plain text, held in a descriptor passed in parameter 0 of the
message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::SetSubjectL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

	HBufC* subject = GetDesParamLC(aMessage, 0);
	iClientMtm->SetSubjectL(*subject);
		
	CleanupStack::PopAndDestroy(subject);
	}
	
/** Set the BIO type of the message.

The BIO type is passed from the client in a package buffer, pointed to by 
parameter 0 of the message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::SetBioTypeL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

	TPckgBuf<TUid> uidBuf;
	aMessage.Read(0, uidBuf);
	iClientMtm->BioTypeChangedL(uidBuf());
	
	TUid bioTypeUid = uidBuf();
	iClientMtm->SaveMessageL();

	TMsvEntry tempEntry = iClientMtm->Entry().Entry(); 
	tempEntry.iBioType = bioTypeUid.iUid;
	iClientMtm->Entry().ChangeL(tempEntry);	
	}

/** Add a recipient (plus alias) to the message.

The recipient's address is passed in a descriptor, pointed to by parameter 0 of
the message.  The alias is also passed in as a descriptor, this time pointed to
by parameter 1.

The recipient type (which indicates if a recipient is a To, Cc or Bcc type) is
passed inside a package buffer pointed to by parameter 2.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddRecipientWithAliasL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );
	
	HBufC* address = GetDesParamLC(aMessage, 0);
	HBufC* alias = GetDesParamLC(aMessage, 1);

	TPckgBuf<RSendAsMessage::TSendAsRecipientType> recipientTypeBuf;
	aMessage.Read(2, recipientTypeBuf);

	// client MTM manages handling of Bcc recipients
	iClientMtm->AddAddresseeL(recipientTypeBuf(), *address, *alias);
	
	CleanupStack::PopAndDestroy(2, address); // alias, address
	}
	
/** Add a recipient to the message.

The recipient's address is passed in a descriptor, pointed to by parameter 0 of
the message.

The recipient type (which indicates if a recipient is a To, Cc or Bcc type) is
passed inside a package buffer pointed to by parameter 1.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddRecipientL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

	HBufC* address = GetDesParamLC(aMessage, 0);
	
	TPckgBuf<RSendAsMessage::TSendAsRecipientType> recipientTypeBuf;
	aMessage.Read(1, recipientTypeBuf);

	// call new implementation - let it manage handling of Bcc recipients
	iClientMtm->AddAddresseeL(recipientTypeBuf(), *address);

	CleanupStack::PopAndDestroy(address);
	}
	
/** Prepare the message for subsequent add or create attachment operations.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::PrepareAddCreateAttachmentL(const RMessage2& aMessage)
	{
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );
	
	// delete any orphaned attachment object.
	delete iAttachment;
	iAttachment = NULL;

	// queue the IPC message - used to notify the client when the attachment 
	// operation is complete.
	iQueuedMessage = aMessage;
	iAttachment = CSendAsAttachment::NewL(*this, *iClientMtm);
	}

/** Take ownership of the RFile attachment

@param	aMessage
The IPC message.
*/
void CSendAsMessage::TransferAttachmentFileL(const RMessage2& aMessage)
	{
	// Close if left open previously.
	iAttachmentFile.Close();

	// need to adopt the file from the client.
	User::LeaveIfError(iAttachmentFile.AdoptFromClient(aMessage, 0, 1));
	}
	
/** Add an attachment to the message.

The attachment added to the message is copied into a new attachment by the
message server.  The new attachment lives in the message store which is private
to the msg server.

The client must pass a file handle over to the server.  The server assumes that
this handle allows it to have read access to the open file once it has been adopted.

The file handles are passed over in the usual manner.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddAttachmentL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->AddExistingFileAttachmentL(iAttachmentFile);
	iState = EAddingOrCreatingAttachment;
	}
	
/** Add an attachment to the message.

The attachment added to the message is copied into a new attachment by the
message server.  The new attachment lives in the message store which is private
to the msg server.

The client must pass a file handle over to the server.  The server assumes that
this handle allows it to have read access to the open file once it has been adopted.

The mime type of the attachment is specified in aMessage parameter 2.

The file handles are passed over in the usual manner.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddAttachmentWithMimeTypeL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	HBufC8* mimeType = GetDesParam8LC(aMessage, 0);
	TUint charset = aMessage.Int1();

	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->AddExistingFileAttachmentWithMimeTypeL(iAttachmentFile, *mimeType, charset);
	iState = EAddingOrCreatingAttachment;

	CleanupStack::PopAndDestroy(mimeType);
	}
	
/** Add an attachment to the message.

The filename of the attachment is linked to the message.

The file must be in a location which the message server is able to read and it
must also remain until the message has been sent or deleted.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddLinkedAttachmentL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	HBufC* fileName = GetDesParamLC(aMessage, 0);
	
	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->AddFileLinkAttachmentL(*fileName);
	iState = EAddingOrCreatingAttachment;
	
	CleanupStack::PopAndDestroy(fileName);
	}

/** Add an attachment to the message.

The filename of the attachment is linked to the message.

The file must be in a location which the message server is able to read and it
must also remain until the message has been sent or deleted.

The mime type of the linked file is supplied.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::AddLinkedAttachmentWithMimeTypeL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	HBufC* fileName = GetDesParamLC(aMessage, 0);
	HBufC8* mimeType = GetDesParam8LC(aMessage, 1);
	TUint charset = aMessage.Int2();
	
	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->AddFileLinkAttachmentWithMimeTypeL(*fileName, *mimeType, charset);
	iState = EAddingOrCreatingAttachment;
	
	CleanupStack::PopAndDestroy(2, fileName); // mimeType, fileName
	}
	
/** Create an empty attachment in the open message.

The SendAs server makes the empty attachment and returns a file handle to the
client ready for writing.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::CreateAttachmentL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	HBufC* fileName = GetDesParamLC(aMessage, 1);

	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->CreateNewFileAttachmentL(iAttachmentFile, *fileName);
	iState = EAddingOrCreatingAttachment;

	CleanupStack::PopAndDestroy(fileName);
	}

/** Create an empty attachment in the open message.

The mime type of the attachment is specified.

The SendAs server makes the empty attachment and returns a file handle to the
client ready for writing.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::CreateAttachmentWithMimeTypeL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );

	HBufC* fileName = GetDesParamLC(aMessage, 1);
		
	HBufC8* mimeType = GetDesParam8LC(aMessage, 2);
		
	PrepareAddCreateAttachmentL(aMessage);
	iAttachment->CreateNewFileAttachmentWithMimeTypeL(iAttachmentFile, *fileName, *mimeType, iCharSet);
	iState = EAddingOrCreatingAttachment;
		
	CleanupStack::PopAndDestroy(2, fileName); // mimeType, fileName
	}	

void CSendAsMessage::SetCharsetInfoForAttachment(const RMessage2& aMessage)
	{
	iCharSet = aMessage.Int0();
	}	


/** Cancel any asynchronous activity being done by the message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::Cancel()
	{
	CancelMessage();
	}
	
/** Launch the message editor for the open message.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::LaunchEditorL(const RMessage2& aMessage)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

	iClientMtm->SaveMessageL();
		
	// the observer and owner is the SendAs Server, via active container.
	CSendAsEditWatcher* editWatcher = CSendAsEditWatcher::NewL(iSession.ActiveContainer(), iSession.EditUtilsPluginUid());
	iSession.ActiveContainer().AddEditWatcherL(*editWatcher);

	TMsvId entryId = iClientMtm->Entry().EntryId();	
	DeleteClientMtm();
	editWatcher->LaunchEditorL(entryId);

	iState = EPendingClose;
	}
		
/** Send the message.

This method handles both confirmed and unconfirmed sends. If an unconfirmed send
is attempted but the client does not have the MTM-required capabilities then the
send will be downgraded to a confirmed send.

@param	aMessage
The IPC message.

@param	aConfirmed
A value of true indicates that the send should be confirmed.

@param	aBackground
A value of true indicates that the send should be done in the background. This
implies that the client does not wish to be notified of the progress of the send.

@leave KErrNotSupported
The client MTM does not support SendAs message sending.
*/
void CSendAsMessage::SendMessageL(const RMessage2& aMessage, TBool aConfirmed, TBool aBackground)
	{
	__ASSERT_ALWAYS( iState == EMessageCreated, PanicClientL(aMessage, ESendAsClientPanicNoCurrentMessage) );
	__ASSERT_DEBUG( iClientMtm != NULL, User::Invariant() );

#if (defined SYMBIAN_USER_PROMPT_SERVICE)		
	TBool hasCapability = EFalse;	
#endif
	// for unconfirmed send, check capabilities of client
	if( !aConfirmed )
		{
		VerifyCallerCapabilitiesL(aMessage, aConfirmed);
#if (defined SYMBIAN_USER_PROMPT_SERVICE)		
		if(!aConfirmed)
			{
			hasCapability = ETrue;
			}
#endif			
		}
		

	TBool result = KErrNone;
	if( iClientMtm->QueryCapability(KUidMtmQuerySendAsMessageSendSupport, result) == KErrNotSupported )
		{
		User::Leave(KErrNotSupported);
		}
	// save message	
	DoSaveMessageL();

	// create the sender obejct
	CSendAsSender* sender = NULL;
	if( aBackground )
		{
		// this a background send - the observer and owner is the active container.
		sender = CSendAsSender::NewL(iSession.ActiveContainer());
		iSession.ActiveContainer().AddSenderL(*sender);
		
		iState = ESendingMessageComplete;
		}
	else
		{
		// this is a non-background send - the observer and owner is the send-as
		// message.
		delete iSender;
		iSender = NULL;
		
		sender	= CSendAsSender::NewL(*this);
		iSender = sender;
		
		// queue the IPC message - used to notify the client when the send is complete
		iQueuedMessage = aMessage;
		
		iState = ESendingMessage;
		}
	// new sender takes ownership of the client MTM
	sender->SetClientMtm(*iClientMtm);
		
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
	TRAPD(err, sender->SendMessageL(aMessage, hasCapability));
	if(err!=KErrNone)
		{
		// There was a error while sending the message... so delete the entry
		iClientMtm->Session().RemoveEntry(iClientMtm->Entry().EntryId());
		iClientMtm = NULL;
		iState = ESendingMessageComplete;
		User::Leave(err);
		}
	iClientMtm = NULL;	
#else
	iClientMtm = NULL;
	if( aConfirmed )
		{
		TSecurityInfo securityInfo(aMessage);
		sender->SendMessage(securityInfo, iSession.NotifierUid());
		}
	else
		{
		sender->SendMessage();
		}
#endif	
	}
	
void CSendAsMessage::DeleteClientMtm()
	{
	delete iClientMtm;
	iClientMtm = NULL;
	}

/** Get progress information 

Used to provide progress information to the client. 

1. Before sending, the progress is set to ESendStateInPreparation.
 
2. Whilst sending, the progress is obtained from the send operation.

3. After sending, the final progress is returned to the client (always a TMsvSendOperationProgress).

@param	aMessage
The IPC message.
*/
void CSendAsMessage::ProgressL(const RMessage2& aMessage)
	{
	switch( iState )
		{
	case EIdle:
	case EMessageCreated:
	case EAddingOrCreatingAttachment:
	case EMessageDeleted:
	case EPendingClose:
		{
		iProgress().iState = CMsvSendOperation::ESendStateInPreparation;
		iProgress().iError = KErrNone;
		iProgress().iProgressMax = 0;
		iProgress().iProgress = 0;
		} break;
	case ESendingMessage:
		{
		__ASSERT_DEBUG( iSender != NULL, User::Invariant() );

		iSender->ProgressL(iProgress);
		} break;
	case ESendingMessageComplete:
		{
		// return the final progress 
		if( iProgress().iError == KErrNone )
			{
			iProgress().iState = CMsvSendOperation::ESendStateDone;
			}
		else
			{
			iProgress().iState = CMsvSendOperation::ESendStateFailed;
			}
		} break;
	default:
		User::Invariant();
		break;
		}

	aMessage.WriteL(0, iProgress);
	}

/** Verify that the client has the required capabilities to use the server MTM
to send this message unconfirmed. Otherwise changes request to confirmed send.

@param	aMessage
The IPC message.
*/
void CSendAsMessage::VerifyCallerCapabilitiesL(const RMessage2& aMessage, TBool& aConfirmed)
	{
	// get required capabilities
	TCapabilitySet caps;
	iClientMtm->Session().GetMtmRequiredCapabilitiesL(iMessageType, caps);

	// get client thread capabilities
	TSecurityInfo securityInfo(aMessage);
	
	// check client capabilities with required capabilities
	if( !securityInfo.iCaps.HasCapabilities(caps) )
		{
		// fail capability check...
		caps.Remove(securityInfo.iCaps);
		PlatSec::CapabilityCheckFail(aMessage, caps, __PLATSEC_DIAGNOSTIC_STRING("Unconfirmed send downgraded to confirmed by CSendAsMessage::VerifyCallerCapabilitiesL"));
		aConfirmed = ETrue;
		}
	}

/** Panic the client

@param	aMessage
The IPC message.

@param aPanic
The panic code.
*/
void CSendAsMessage::PanicClientL(const RMessage2& aMessage, TSendAsClientPanic aPanic) const
	{
	iSession.PanicClient(aMessage, aPanic);
	}

/*
 *	Methods from MSendAsSenderObserver
 */
 
/** Notify the client that the send has completed

@param	aError
The error code.
*/
void CSendAsMessage::SenderComplete(TInt aError)
	{
	__ASSERT_ALWAYS( iState == ESendingMessage, User::Invariant() );
	__ASSERT_DEBUG( iSender != NULL, User::Invariant() );

	// get final progress from the sender.
	iSender->FinalProgress(iProgress);
	
	// capture error
	if( iProgress().iError == KErrNone )
		{
		iProgress().iError = aError;
		}
	// notify the client that the send has completed - always complete with 
	// KErrNone as actual error is in the progress.
	iQueuedMessage.Complete(KErrNone);
	
	// Set the state to complete so we can call FinalProgress
	iState = ESendingMessageComplete;
	}

/*
 *	Methods from MSendAsAttachmentObserver
 */

/** Notify the client that the attachment operation has completed

If the message is complete, the client is notified, otherwise the created
attachment is transferred to the client.

@param	aError
The error code.

@param	aCompleteMessage
Indicates if the message is complete.
*/
void CSendAsMessage::AttachmentCompleteL(TInt aError, TBool aCompleteMessage)
	{
	__ASSERT_ALWAYS( iState == EAddingOrCreatingAttachment, User::Invariant() );
	
	// notify the client that the attachment operation has completed - if an
	// error has occurred the error code is returned in the complete status.
	if (aCompleteMessage)
		{
		iQueuedMessage.Complete(aError);
		}
	else
		{
		// transfer the created file to the client. Argument 0 of iQueuedMessage
		// contains the address to which the client should receive the file handle.
		User::LeaveIfError(iAttachmentFile.TransferToClient(iQueuedMessage, 0));
		iAttachmentFile.Close();
		}
	// the attachment file is now closed - set it to a blank RFile so that any 
	// later closes don't panic.
	iAttachmentFile = RFile();
	
	// The client can continue editing the message - move to appropriate state
	// and delete attachment object

	// Delete the attachment object after use. If this is a notification of a 
	// cancel then the delete should happen after the cancel operation to avoid
	// a second cancel in the attachment object's destructor.
	if( aError != KErrCancel )
		{
		delete iAttachment;
		iAttachment = NULL;
		}

	iState = EMessageCreated;
	}

/** Sets the message entry's Visible flag to ETrue and inPreparation flag to EFalse 
saves the message, commits the changes  
*/

void CSendAsMessage::DoSaveMessageL() 
    {     
    iClientMtm->SaveMessageL();
  
    // set the InPreparation to false and visible to true
    TMsvEntry tempEntry= iClientMtm->Entry().Entry();
    tempEntry.iDate.UniversalTime();
    tempEntry.SetVisible(ETrue);
    tempEntry.SetInPreparation(EFalse);
     
    iClientMtm->Entry().ChangeL(tempEntry);
    }

/** 
Sets the character encoding value. The character encoding value options are 7-bit,
8-bit and 16-Bit Unicode. By default the character set encoding is 7 bit encoding.
It call the MTM functionality for setting the charset value.
@param	aMessage	The IPC message.
@return void
*/
   
void CSendAsMessage::SetMessageCharacterSetL(const RMessage2& aMessage)
	{
	TInt err =	iClientMtm->SetMessageCharacterSet(aMessage.Int0());
	TPckgBuf<TInt> pckg = err;
	aMessage.WriteL(1,pckg);
	}
