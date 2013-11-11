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

#ifndef __MMSVATTACHMENTMANAGERSYNC_H__
#define __MMSVATTACHMENTMANAGERSYNC_H__

#include <e32base.h>
#include <msvstd.h>
#include <cmsvattachment.h>

class MMsvAttachmentManagerSync
/**
Defines the attachment management extensions interface.

This class is a pure virtual interface class that defines the APIs to be used for
by MTMs and implementors of MMsvAttachmentManager.

This API extends the MMsvAttachmentManager with synchronous calls for deleting
and creating attachments. 

@see CMsvAttachment
@publishedAll
@released
*/
	{
public:	
	/**
	Creates a new empty attachment file.
	
	The caller is returned an open writable file handle to an empty attachment file in the message
	store. The caller must pass in an uninitialised file handle. If the request is sucessful the file handle
	is open and must close by the caller once the data has been written to it.
	
	@param aFileName The filename to assign to the newly create attachment file.
	@param aAttachmentFile An uninitialised file handle. This is opened and can be written to if the
		   request is successful. The ownership is transferred . The caller must close the file handle.
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo) = 0;



	/**
	Renames the physical filename of an attachment.
	
	@param aIndex The array index position of the attachment to be renamed.
	@param aNewName The new name of the attachment.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	@leave KErrAlreadyExists If the supplied attachment filename already exists.
	*/
	virtual void RenameAttachmentL(TInt aIndex, const TDesC& aNewName) = 0;

	
	/**
	Removes the attachment from the message entry.
	
	This changes the array index values of all the attachments after the removed one.
	Attachment files stored in the message store are deleted. Linked files and message entry 
	attachments are not deleted, this is left to the caller to do if required.
	
	@param aParam The array index position of the attachment to be removed.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void RemoveAttachmentL(TInt aIndex) = 0;
	
	/**
	Modify existing attachment
	@param aAttachmentInfo The attachment info associated with the file.
	  If the routine does not leave, then ownership will be transferred to the
	  attachment manager. If the routine does leave then ownership will not have
	  been transfered and the caller is responsible for cleanup.
	@leave KErrAccessDenied If attachment manager is in read-only mode.
	*/
	virtual void ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo) = 0;
	
	};

#endif // __MMSVATTACHMENTMANAGERSYNC_H__
