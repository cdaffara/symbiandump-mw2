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

#include <rsendasmessage.h>

#include <s32mem.h>
#include <txtrich.h>

#include "sendasserverdefs.h"

// Large enough to pass an sms message across in one transfer.
const TInt KSendAs2BodyChunkSize = 512;

/**
Create a new message in the Drafts folder.

The message type is implied by the type of the account used.
The message can be modified or deleted by this object at any
time up until the message is sent, deleted or opened in an editor.

@param	aSendAs
The connection to the SendAs server.  This RSendAs object must be
connected.

@param	aAccount
An account obtained from the CSendAsAccounts helper class.
*/
EXPORT_C void RSendAsMessage::CreateL(RSendAs& aSendAs, TSendAsAccount aAccount)
	{
	// create subsession with server.
	User::LeaveIfError(CreateSubSession((RSessionBase&)aSendAs, ESASCreateSubSession));
	
	TPckgBuf<TSendAsAccount> buf(aAccount);
	TInt createStatus = SendReceive(ESAMCreateForAccount, TIpcArgs(&buf));
	if (createStatus != KErrNone)
		{
		CloseSubSession(ESAMDestroySubSession);
		User::Leave(createStatus);
		}
	}
	
/**
Create a new message in the Drafts folder.

The message type is specified.
The message can be modified or deleted by this object at any
time up until the message is sent, deleted or opened in an editor.

@param aSendAs
The connection to the SendAs server.  This RSendAs object must be
connected.

@param aMessageType
A message type obtained from the CSendAsMessageTypes helper class.
*/
EXPORT_C void RSendAsMessage::CreateL(RSendAs& aSendAs, TUid aMessageType)
	{
	// create subsession with server
	User::LeaveIfError(CreateSubSession((RSessionBase&)aSendAs, ESASCreateSubSession));
	
	TPckgBuf<TUid> buf(aMessageType);
	TInt createStatus = SendReceive(ESAMCreateByType, TIpcArgs(&buf));
	if (createStatus != KErrNone)
		{
		CloseSubSession(ESAMDestroySubSession);
		User::Leave(createStatus);
		}
	}

/**
Close an open message and save it in the Drafts folder.
*/
EXPORT_C void RSendAsMessage::SaveMessageAndCloseL()
	{
	User::LeaveIfError(SendReceive(ESAMSaveMessage, TIpcArgs()));
	CloseSubSession(ESAMDestroySubSession);
	}

/**
Close an open message and delete it from the Drafts folder.
*/
EXPORT_C void RSendAsMessage::Close()
	{
	if (SubSessionHandle() != KNullHandle)
		{
		// don't attempt deletion if no subsession present (ie already closed/never opened)
		SendReceive(ESAMDelete, TIpcArgs());
		}
	CloseSubSession(ESAMDestroySubSession);
	}
	
/**
Set the body text of this message using a CRichText object.
The object must have an open message to use this method.

@param aBody
A CRichText object containing the body to add to the message being constructed.
*/
EXPORT_C void RSendAsMessage::SetBodyTextL(const CRichText& aBody)
	{
	TInt textLength = aBody.DocumentLength();
	TInt textPos = 0;

	HBufC* bodyBuf = HBufC::NewLC(KSendAs2BodyChunkSize);
	TPtr bodyPtr(bodyBuf->Des());
	while (textPos < textLength)
		{
		aBody.Extract(bodyPtr, textPos, KSendAs2BodyChunkSize);
		User::LeaveIfError(SendReceive(
			textPos == 0 ? ESAMSetBodyFirst : ESAMSetBodyNext, TIpcArgs(&bodyPtr)));
		textPos	+= KSendAs2BodyChunkSize;
		}
		CleanupStack::PopAndDestroy(bodyBuf);
	}
	
/**
Set the body text of this message using a plain descriptor.
The object must have an open message to use this method.

@param aBody
A descriptor containing the body to add to the message being constructed.
*/
EXPORT_C void RSendAsMessage::SetBodyTextL(const TDesC& aBody)
	{
	TInt textLength = aBody.Length();
	TInt textPos = 0;

	while (textPos < textLength)
		{
		TInt sizeLeft = textLength - textPos;
		TPtrC bodyPtr(aBody.Mid(textPos, 
			(KSendAs2BodyChunkSize >= sizeLeft) ? sizeLeft : KSendAs2BodyChunkSize));
		User::LeaveIfError(SendReceive(
			textPos == 0 ? ESAMSetBodyFirst : ESAMSetBodyNext, TIpcArgs(&bodyPtr)));

		textPos	+= KSendAs2BodyChunkSize;
		}
	}
	
/**
Set the subject of this message using a plain descriptor.
The object must have an open message to use this method.

@param aSubject
A descriptor containing the subject for the message.
*/
EXPORT_C void RSendAsMessage::SetSubjectL(const TDesC& aSubject)
	{
	User::LeaveIfError(SendReceive(ESAMSetSubject, TIpcArgs(&aSubject)));
	}
	
/**
Set the BIO type of this message.
The object must have an open message to use this method.

@param aBioType
The biotype for this message.
*/
EXPORT_C void RSendAsMessage::SetBioTypeL(TUid aBioType)
	{
	TPckgBuf<TUid> pckg(aBioType);
	User::LeaveIfError(SendReceive(ESAMSetBioType, TIpcArgs(&pckg)));
	}
	
/**
Add a recipient to this message.
The recipient's address is held in a descriptor, and is in the correct format for the
message type.

The recipient type is one of the TSendAsRecipientType constants, currently one of:
ESendAsRecipientTo	-	The recipient goes in the 'To' field.
ESendAsRecipientCc	-	The recipient goes in the 'Cc' field.
ESendAsRecipientBcc	-	The recipient goes in the 'Bcc' field.

If the 'Cc' field is not supported, the recipient will instead be added to the 'To'
field since these are functionally equivalent.
If the 'Bcc' field is not supported, this method will return KErrNotSupported.

@param aAddress
A descriptor holding the address of the recipient to add.

@param aRecipientType
The recipient type of the address being added. See above.
*/
EXPORT_C void RSendAsMessage::AddRecipientL(const TDesC& aAddress, TSendAsRecipientType aRecipientType)
	{
	TPckgBuf<TSendAsRecipientType> pckg(aRecipientType);
	User::LeaveIfError(SendReceive(ESAMAddRecipient, TIpcArgs(&aAddress, &pckg)));
	}
	
/**
Add a recipient with an alias to this message.
The recipient's address is held in a descriptor, and is in the correct format for the
message type.
The alias is the displayed recipient name.

The recipient type is one of the TSendAsRecipientType constants, currently one of:
ESendAsRecipientTo	-	The recipient goes in the 'To' field.
ESendAsRecipientCc	-	The recipient goes in the 'Cc' field.
ESendAsRecipientBcc	-	The recipient goes in the 'Bcc' field.

If the 'Cc' field is not supported, the recipient will instead be added to the 'To'
field since these are functionally equivalent.
If the 'Bcc' field is not supported, this method will return KErrNotSupported.

@param aAddress
A descriptor holding the address of the recipient to add.

@param aAlias
A descriptor holding the address alias of the recipient to add.

@param aRecipientType
The recipient type of the address being added. See above.
*/
EXPORT_C void RSendAsMessage::AddRecipientL(const TDesC& aAddress, const TDesC& aAlias, TSendAsRecipientType aRecipientType)
	{
	TPckgBuf<TSendAsRecipientType> pckg(aRecipientType);
	User::LeaveIfError(SendReceive(ESAMAddRecipientWithAlias, TIpcArgs(&aAddress, &aAlias, &pckg)));
	}

/**
Add an attachment to this message asynchronously.

A copy of the specified file is attached to message.

@param aAttachmentFile
The file being added to the message as an attachment.
The file is closed if attached successfully. Ownership is transferred .The caller must close the file handle.

@param aMimeType
The mime type for this attachment.

@param aCharset
The charset for this attachment. The value is a standard IANA charset.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddAttachment(RFile& aAttachmentFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	TInt err;
	TIpcArgs args;
	err = aAttachmentFile.TransferToServer(args, 0, 1);
	if (err != KErrNone)
		{
		TRequestStatus* p = &aStatus;
		User::RequestComplete(p, err);
		}
	else
		{
		// Perform synchronous operation to allow server to take ownership of the RFile.
		err = SendReceive(ESAMTransferAttachmentFile, args);
		if  (err != KErrNone)
			{
			TRequestStatus* p = &aStatus;
			User::RequestComplete(p, err);
			}
		else
			{
			// Now it's safe to close the file.
			aAttachmentFile.Close();
			TIpcArgs args2;
			// reuse the args
			args2.Set(0, &aMimeType);	
			args2.Set(1, aCharset); 	
				
			// Now perform async adding of transfered attachment
			SendReceive(ESAMAddAttachmentWithType, args2, aStatus);
			}
		}
	}
	
/**
Add an attachment to this message asynchronously.

A copy of the specified file is attached to message.

@param aAttachmentFile
The file being added to the message as an attachment. Ownership is transferred .The caller must close the file handle.


@param aMimeType
The mime type for this attachment.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddAttachment(RFile& aAttachmentFile, const TDesC8& aMimeType, TRequestStatus& aStatus)
	{
	AddAttachment(aAttachmentFile, aMimeType, 0, aStatus);
	}
	
/**
Add an attachment to this message.
The attachment is copied into the message store from the supplied file.

@param aAttachmentFile
The file being added to the message as an attachment. Ownership is transferred .The caller must close the file handle. 

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddAttachment(RFile& aAttachmentFile, TRequestStatus& aStatus)
	{
	TInt err;
	TIpcArgs args;
	err = aAttachmentFile.TransferToServer(args, 0, 1);
	if (err != KErrNone)
		{
		TRequestStatus* p = &aStatus;
		User::RequestComplete(p, err);
		}
	else
		{
		// Perform synchronous operation to allow server to take ownership of the RFile.
		err = SendReceive(ESAMTransferAttachmentFile, args);
		if  (err != KErrNone)
			{
			TRequestStatus* p = &aStatus;
			User::RequestComplete(p, err);
			}
		else
			{
			// Now it's safe to close the file.
			aAttachmentFile.Close();
			// Now perform async adding of transfered attachment
			SendReceive(ESAMAddAttachment, args, aStatus);
			}
		}
	}

/**
Add an attachment to this message asynchronously.

A copy of the specified file is attached to message.

@param aAttachmentFilePath
The file name and path for the file being added to the message as an attachment.

@param aMimeType
The mime type for this attachment.

@param aCharset
The charset for this attachment. The value is a standard IANA charset.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/	
EXPORT_C void RSendAsMessage::AddAttachment(const TDesC& aAttachmentFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	TInt err;
	RFs fs;
	RFile file;

	err = fs.Connect();
	if (err == KErrNone)
		{
		err = fs.ShareProtected();
		if (err == KErrNone)
			{
			err = file.Open(fs, aAttachmentFilePath, EFileRead | EFileShareReadersOnly);
			if (err == KErrNone)
				{
				AddAttachment(file, aMimeType, aCharset, aStatus);
				}
			}
		}
	// On successful adding of an attachment, ownership of the file is
	// transfered. Otherwise clean up must happen here.
	if (err != KErrNone)
		{
		TRequestStatus* p = &aStatus;
		file.Close();
		User::RequestComplete(p, err);
		}
	fs.Close();
	}

/**
Add an attachment to this message asynchronously.

A copy of the specified file is attached to message.

@param aAttachmentFilePath
The file name and path for the file being added to the message as an attachment.

@param aMimeType
The mime type for this attachment.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/	
EXPORT_C void RSendAsMessage::AddAttachment(const TDesC& aAttachmentFilePath, const TDesC8& aMimeType, TRequestStatus& aStatus)
	{
	AddAttachment(aAttachmentFilePath, aMimeType, 0, aStatus);
	}

/**
Add an attachment to this message.
The attachment is copied into the message store from the supplied filename.

@param aAttachmentFilePath
The file name and path for the file being added to the message as an attachment.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddAttachment(const TDesC& aAttachmentFilePath, TRequestStatus& aStatus)
	{
	TInt err;
	RFs fs;
	RFile file;

	err = fs.Connect();
	if (err == KErrNone)
		{
		err = fs.ShareProtected();
		if (err == KErrNone)
			{
			err = file.Open(fs, aAttachmentFilePath, EFileRead | EFileShareReadersOnly);
			if (err == KErrNone)
				{
				AddAttachment(file, aStatus);
				}
			}
		}
	// On successful adding of an attachment, ownership of the file is
	// transfered. Otherwise clean up must happen here.
	if (err != KErrNone)
		{
		TRequestStatus* p = &aStatus;
		file.Close();
		User::RequestComplete(p, err);
		}
	fs.Close();
	}

/**
Add an attachment to this message.
The attachment is linked to the message from the supplied filename.  This file must be present
when the message is sent.

@param aLinkedAttachmentFile
The file name and path for the file being added to the message as a linked attachment.

@param aMimeType
The mime type for this attachment.

@param aCharset
The charset for this attachment. The value is a standard IANA charset.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddLinkedAttachment(const TDesC& aLinkedAttachmentFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	SendReceive(ESAMAddLinkedAttachmentWithType, TIpcArgs(&aLinkedAttachmentFile, &aMimeType, aCharset), aStatus);
	}
	
/**
Add an attachment to this message asynchronously.

The attachment is linked to the message from the supplied filename. This file must be present
when the message is sent.

@param aLinkedAttachmentFile
The file name and path for the file being added to the message as a linked attachment.

@param aMimeType
The mime type for this attachment.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddLinkedAttachment(const TDesC& aLinkedAttachmentFile, const TDesC8& aMimeType, TRequestStatus& aStatus)
	{
	AddLinkedAttachment(aLinkedAttachmentFile, aMimeType, 0, aStatus);
	}
	
/**
Add an attachment to this message asynchronously.

The attachment is linked to the message from the supplied filename. This file must be present
when the message is sent.

@param aLinkedAttachmentFile
The file name and path for the file being added to the message as a linked attachment.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::AddLinkedAttachment(const TDesC& aLinkedAttachmentFile, TRequestStatus& aStatus)
	{
	SendReceive(ESAMAddLinkedAttachment, TIpcArgs(&aLinkedAttachmentFile), aStatus);
	}

/**
Create an attachment to this message.
The RFile object should not be open when this method is called.
On return, the supplied RFile is able to write into the file.

@param aFileName
The filename to assign to the newly create attachment file. 

@param aAttachmentFile
The RFile which on return will be opened on the newly created attachment. Ownership is transferred .The caller must close the file handle. 
@param aMimeType
The mime type for this attachment.

@param aCharset
The charset for this attachment. The value is a standard IANA charset.
*/
EXPORT_C void RSendAsMessage::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType, TUint aCharset)
	{
	TIpcArgs args;
	TPckgBuf<TInt> fileHandle;	
	
	User::LeaveIfError(SendReceive(ESAMCharSetInfoForAttachment, TIpcArgs(aCharset)));
		
	args.Set(0, &fileHandle);
	args.Set(1, &aFileName);
	args.Set(2, &aMimeType);

	// session handle (RFs)received from TransferToClient() in server
	TInt fileServerHandle = User::LeaveIfError(SendReceive(ESAMCreateAttachmentWithType, args));

	// Retrieve the RFile handle from the server
	// Take ownership of the new file from server
	User::LeaveIfError(aAttachmentFile.AdoptFromServer(fileServerHandle, fileHandle()));
	}
	
/**
Create an attachment to this message.
The RFile object should not be open when this method is called.
On return, the supplied RFile is able to write into the file.

@param aFileName
The filename to assign to the newly create attachment file. 

@param aAttachmentFile
The RFile which on return will be opened on the newly created attachment.Ownership is transferred . The caller must close the file handle.

@param aMimeType
The mime type for this attachment.
*/
EXPORT_C void RSendAsMessage::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType)
	{
	CreateAttachmentL(aFileName, aAttachmentFile, aMimeType, 0);
	}
	
/**
Create an attachment to this message.
The RFile object should not be open when this method is called.
On return, the supplied RFile is able to write into the file.

@param aFileName
The filename to assign to the newly create attachment file. 

@param aAttachmentFile
The RFile which on return will be opened on the newly created attachment.Ownership is transferred .The caller must close the file handle.

*/
EXPORT_C void RSendAsMessage::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile)
	{
	TIpcArgs args;
	TPckgBuf<TInt> fileHandle;
	
	args.Set(0, &fileHandle);
	args.Set(1, &aFileName);

	// session handle (RFs)received from TransferToClient() in server
	TInt fileServerHandle = User::LeaveIfError(SendReceive(ESAMCreateAttachment, args));

	// Retrieve the RFile handle from the server
	// Take ownership of the new file from server
	User::LeaveIfError(aAttachmentFile.AdoptFromServer(fileServerHandle, fileHandle()));
	}
	
/**
Asynchronously send this message.
This method requests that the SendAs server send the message without prompting the
user.  This will only be allowed to happen if the caller holds sufficient capabilities
to perform this action. If the caller does not hold these capabilities, then the
message send will be automatically demoted to a confirmed send.
The RSendAsMessage API cannot modify the message if this method fails.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::SendMessage(TRequestStatus& aStatus)
	{
	SendReceive(ESAMSendMessage, aStatus);
	}
	
/**
Asynchronously send this message.
This method requests that the SendAs server send the message, prompting the user
for confirmation.  This functionality must be supported by the UI MTM for the selected
message type.
The RSendAsMessage API cannot modify the message if this method fails.

@param aStatus
Asynchronous status object (TRequestStatus) that is signalled when the
operation completes. aStatus should be checked by the caller to ensure that the
operation was successful.
*/
EXPORT_C void RSendAsMessage::SendMessageConfirmed(TRequestStatus& aStatus)
	{
	SendReceive(ESAMSendMessageConfirmed, aStatus);	
	}
	
/**
Sends this message in the background without confirmation and close the handle.
This is a synchronous operation. This will only be allowed to happen if the
caller holds sufficient capabilities to perform this action.  If the caller does
not hold these capabilities, then the message send will be automatically demoted
to a confirmed send. The RSendAsMessage API cannot modify or resend the message
if this method fails. No progress information is available after calling this API.
*/
EXPORT_C void RSendAsMessage::SendMessageAndCloseL()
	{
	// close message
	User::LeaveIfError(SendReceive(ESAMSendMessageBackground));
	CloseSubSession(ESAMDestroySubSession);
	}
	
/**
Sends this message in the background and close the handle to it. This is a 
synchronous operation. This method requests that the SendAs server send the
message, prompting the user for confirmation.  This functionality must be
supported by the UI MTM for the selected message type.
The RSendAsMessage API cannot modify or resend the message if this method fails.
*/
EXPORT_C void RSendAsMessage::SendMessageConfirmedAndCloseL()
	{
	// close message
	User::LeaveIfError(SendReceive(ESAMSendMessageConfirmedBackground));
	CloseSubSession(ESAMDestroySubSession);
	}
	
/**
Retrieve the progress information for this message.
This method can be called at any time after message creation.

@param aProgress
Progress information on current asynchronous operation.
*/
EXPORT_C void RSendAsMessage::ProgressL(TSendAsProgress& aProgress)
	{
	if (SubSessionHandle() == KNullHandle)
		{
		User::Leave(KErrAbort);
		}
	TPckgBuf<TSendAsProgress> buf;
	User::LeaveIfError(SendReceive(ESAMGetProgress, TIpcArgs(&buf)));
	aProgress = buf();
	}
	
/**
Cancels any asynchronous request.

Only a single asynchronous request is supported at any one time. This method
cancels the current requests if any.
*/
EXPORT_C void RSendAsMessage::Cancel()
	{
	SendReceive(ESAMCancel);
	}
	
/**
Opens the message in the associated message editor for the message type. 
The RSendAsMessage handle is closed, so that the message cannot be 
futher accessed through the object.
*/
EXPORT_C void RSendAsMessage::LaunchEditorAndCloseL()
	{
	// close message
	User::LeaveIfError(SendReceive(ESAMLaunchEditor));
	CloseSubSession(ESAMDestroySubSession);
	}

/** 
Sets the character encoding value. The character encoding value options are 7-bit,
8-bit and 16-Bit Unicode.
If this functionality is not supported by the sending MTM (currently for SMS it is supported), 
it will leave with KErrExtensionNotSupported
@param aCharset 					TUint, indicating the enocding value.
@leave KErrExtensionNotSupported 	If the message is other than SMS.
@leave Other 			 			Standard system-wide error codes.
@return void
*/
   
EXPORT_C void RSendAsMessage::SetCharacterSetL(const TUint aCharset)
	{
	TPckgBuf<TInt> pckg;
	User::LeaveIfError(SendReceive(ESAMSetCharacterSet,TIpcArgs(aCharset, &pckg)));
	User::LeaveIfError(pckg());
	}
