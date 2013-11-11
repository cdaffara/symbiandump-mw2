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
// CMsvAttachmentManager.h
//
/**
 * @file 
 * @publishedAll
 * @released
 */

#ifndef __MMSVATTACHMENTMANAGER_H__
#define __MMSVATTACHMENTMANAGER_H__

#include <e32base.h>
#include <msvstd.h>
#include <cmsvattachment.h>

class MMsvAttachmentManager
/**
Defines the attachment management interface.

This class is a pure virtual interface class that defines the APIs to be used for
attachment management in the Messaging Framework. It is expected that the clients of
this interface will be MTMs implementations that require attachment management.

The API allows is based around the use of the CMsvAttachment object that represents
any type of attachment that is supported by the Messaging Framework. The CMsvAttachment
object provides users with various attributes about the attachment without having to
actually load or retrieve the attachment.

This attachment manager API supports the following different types of attachments:
1 - File attachments, file based attachments that are copied or created in the message 
	store.
2 - Linked file attachments, file based attachments that are not copied and are simply 
	linked to the file location on disk.
3 - Message entry attachments, existing message entries that can be registered as
	attachments of another message entry.

For file based attachments, this API also supports the retrieval of the files as
open read-only file handles.

All functionality that requires the attachment manager to be in edit mode have been
defined as asynchronous. The attachment manager does not allow multiple asynchronous
requests to be made at the same time.

@see CMsvAttachment
@publishedAll
@released
*/
	{
public:	
	/**
	Adds an attachment to the message store by file path.
	
	The attachment file must be located in a public area. The file path must also be a
	full path specification. The file is copied into the message store.
	
	@param aFilePath The absolute file path of the attachment file.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@param aStatus The client's request status to complete when the operation has completed.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void AddAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;
		
	/**
	Adds an attachment to the message store by file handle.
	
	This is used to add an attachment from an open file handle such as when adding a file
	from the caller's private area. The file is copied into the message store. The message
	server is responsible for closing the file handle once copied.
	
	@param aFileHandle An open file handle for the attachment file. Ownership is transferred. The caller must close the file handle.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@param aStatus The client's request status to complete.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void AddAttachmentL(RFile& aFileHandle, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;
	
	/**
	Adds an attachment as a linked file attachment.
	
	The attachment is added as a link by its file path. The attachment is not copied to the
	message store. The attachment is linked and therefore must be in a public location.
	
	It is possible to change a linked attachment file after it has been attached to a message
	entry. No integrity checking is carried out. This is left to individual MTMs or implementors
	of this interface to decide if any checking is required such as checking if the size has changed.
	
	@param aFilePath The absolute file path of the linked attachment file.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@param aStatus The client's request status to complete.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void AddLinkedAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;
	
	/**
	Adds a message entry as an attachment.
	
	The message entry is registered as an attachment of the	current message entry. The attachment
	message entry is not copied.
	
	@param aEntryId The message Id of the entry to register as an attachment.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@param aStatus The client's request status to complete.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void AddEntryAsAttachmentL(TMsvId aEntryId, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;
	
	/**
	Creates a new empty attachment file.
	
	The caller is returned an open writable file handle to an empty attachment file in the message
	store. The caller must pass in an uninitialised file handle. The file handle cannot be used
	until the asynchronous request completes successfully. If the request is sucessful the file handle
	is open and must close by the caller once the data has been written to it.
	
	@param aFileName The filename to assign to the newly create attachment file.
	@param aAttachmentFile An uninitialised file handle. This is opened and can be written to if the
		   request is successful. Ownership is transferred . The caller must close the file handle.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;
	
	
	/**
	Renames the physical filename of an attachment.
		
	@param aIndex The array index position of the attachment to be renamed.
	@param aNewName The new name of the attachment.
	@param aStatus The client's request status to complete.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	@leave KErrAlreadyExists If the supplied attachment filename already exists.
	*/	
	virtual void RenameAttachmentL(TInt aIndex, const TDesC& aNewName, TRequestStatus& aStatus) = 0;

	
	/**
	Counts the number of attachments.
	
	Returns the number of attachments associated with the message entry. This includes all the
	attachments that have not been commited yet.
	@return The number of attachments.
	*/
	virtual TInt AttachmentCount() const = 0;
	
	/**
	Returns an attachment info object.
	
	This object contains attributes and information about the attachment without having actually
	retrieving the actual attachment. The caller assumes ownership of the returned object.
	
	@param aIndex The array index position of the attachment.
	@return The attachment info for the attachment. Ownership is passed to caller,
	*/
	virtual CMsvAttachment* GetAttachmentInfoL(TInt aIndex) = 0;
	
	/**
	Returns an attachment info object.
	
	This object contains attributes and information about the attachment without having actually
	retrieving the actual attachment. The caller assumes ownership of the returned object.
	
	@param aId The attachment Id of the attachment.
	@return The attachment info for the attachment. Ownership is passed to caller,
	@leave KErrNotFound If an attachment with the specified attachment Id is not found.
	*/
	virtual CMsvAttachment* GetAttachmentInfoL(TMsvAttachmentId aId) = 0;

	/**
	Modifies the attachment info for a particular attachment.
	
	This allows callers to modify an existing attachment info object for a particular attachment.
	The attachment info object passed in replaces the existing attachment info object and takes
	ownership for the object passed in. It is expected that callers will use GetAttachmentInfoL 
	to get the object, make some changes and pass it back into this method.
	
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@param aStatus The client's request status to complete.
	@leave KErrNotFound If the attachment that is trying to be modified cannot be found.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus) = 0;

	/**
	Returns an open file handle for the attachment file.
	
	Returns a read-only open file handle for the attachment file. This only applies to file based
	attachments ie. file attachment and linked file attachments. The caller is responsible for
	closing the returned file handle.
	
	@param aIndex The array index position of the attachment.
	@return A read-only open file handle for the attachment file. Caller must close the handle.
	@leave KErrNotSupported If the attachment is not a file attachment in the message store.
	*/
	virtual RFile GetAttachmentFileL(TInt aIndex) = 0;
	
	/**
	Returns an open file handle for the attachment file.
	
	Returns a read-only open file handle for the attachment file. This only applies to file based
	attachments ie. file attachment and linked file attachments. The caller is responsible for
	closing the returned file handle.
	
	@param aIndex The array index position of the attachment.
	@return A read-only open file handle for the attachment file. Caller must close the handle.
	@leave KErrNotSupported If the attachment is not a file attachment in the message store.
	@leave KErrNotFound If an attachment with the specified attachment Id is not found.
	*/
	virtual RFile GetAttachmentFileL(TMsvAttachmentId aId) = 0;
	
	/**
	Returns an open writable file handle for the attachment file.
	
	Returns a writable open file handle for the attachment file. This only applies to file based
	attachments ie. file attachment and linked file attachments. The caller is responsible for
	closing the returned file handle.
	
	@param aIndex The array index position of the attachment.
	@return A writable open file handle for the attachment file. Caller must close the handle.
	@leave KErrNotSupported If the attachment is not a file attachment in the message store.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual RFile GetAttachmentFileForWriteL(TInt aIndex) = 0;
	
	/**
	Returns an open writable file handle for the attachment file.
	
	Returns a writable open file handle for the attachment file. This only applies to file based
	attachments ie. file attachment and linked file attachments. The caller is responsible for
	closing the returned file handle.
	
	@param aIndex The array index position of the attachment.
	@return A writable open file handle for the attachment file. Caller must close the handle.
	@leave KErrNotSupported If the attachment is not a file attachment in the message store.
	@leave KErrNotFound If an attachment with the specified attachment Id is not found.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual RFile GetAttachmentFileForWriteL(TMsvAttachmentId aId) = 0;
	
	/**
	Removes the attachment from the message entry.
	
	This changes the array index values of all the attachments after the removed one.
	Attachment files stored in the message store are deleted. Linked files and message entry 
	attachments are not deleted, this is left to the caller to do if required.
	
	@param aParam The array index position of the attachment to be removed.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void RemoveAttachmentL(TInt aIndex, TRequestStatus& aStatus) = 0;
	
	/**
	Removes the attachment from the message entry.
	
	This changes the array index values of all the attachments after the removed one.
	Attachment files stored in the message store are deleted. Linked files and message entry 
	attachments are not deleted, this is left to the caller to do if required.
	
	@param aParam The array index position of the attachment to be removed.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	@leave KErrNotFound If an attachment with the specified attachment Id is not found.
	*/
	virtual void RemoveAttachmentL(TMsvAttachmentId aId, TRequestStatus& aStatus) = 0;
	
	/**
	Cancels the last asynchronous operation that was requested.
	
	Allows callers to cancel the last asynchronous operation. This will have no effect
	if an asynchronous is not waiting to complete.
	*/
	virtual void CancelRequest() = 0;
	};

#endif // __MMSVATTACHMENTMANAGER_H__
