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

#ifndef __MMSVSTOREMANAGER_H__
#define __MMSVSTOREMANAGER_H__

#include <msvstd.h>

/**
Provides the File Access mechanism to the files that are stored in the message server's 
data cage in a secure manner. Presents a common interface to message store file access 
regardless of the process that its running in.
For instance, when the File Access APIs are used on the client-side process, the implementation 
uses secure IPC calls to the message server that includes capability policing, however, 
when the same API is used on the server-side, the implementation has direct access to 
the file in the message store.

@internalTechnology
*/
class MMsvStoreManager
{
public:
	/**
	Allows clients to create a new empty file and allows clients to stream data to it.
	
	Creates a new attachment file in the message store for the message entry specified by the aEntryId 
	parameter. The attachment filename is set to the filename indicated by the aFilename parameter. 
	The empty attachment file is passed back to the client using an opened file handle to the file in the message store.
	
	@param aEntryId The entry for which an attachment needs to be created in the message store
	@param aFilename The name of the attachment file to be created
	@param aFile On return, The created file handle for the attachment file.The ownership is transferred . The caller must close the file handle.
	@leave KErrAlreadyexists, if the file with the same name already existed the directory.	
	*/
	virtual TBool CreateAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile)=0;

	/**
	Allows clients to replace an existing attachment file and allows clients to stream data to it.
	
	Replaces an existing attachment file in the message store for the message entry specified by the aEntryId 
	parameter. The attachment filename is set to the filename indicated by the aFilename parameter. 
	If an attachment with the supplied name does not exist, a new file is created. The empty attachment
	file is passed back to the client using an opened file handle to the file in the message store.
	
	@param aEntryId The entry for which an attachment needs to be created in the message store
	@param aFilename The name of the attachment file to be created
	@param aFile On return, The created file handle for the attachment file.The ownership is transferred . The caller must close the file handle.
	*/
	virtual void ReplaceAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile)=0;
	
	/**
	Allows clients to view attachment files.

	Opens an existing attachment file in the message associated with the message
	entry specified by the aEntryId parameter. The attachment is identified by 
	the file path specified by the aFilePath parameter. The attachment file is 
	returned as a read-only file handle to the file in the message store. 
	
	@param aEntryId The entry whose attachment needs to be opened/read from the message store
	@param aFilePath The name and path of the attachment file to be opened
	@param aFile On return, The opened file handle for the attachment file.The ownership is transferred . The caller must close the file handle.
	*/
	virtual void OpenAttachmentL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)=0;
	
	/**
	Allows clients to open an attachment file for writing.

	Opens an existing attachment file in the message associated with the message
	entry specified by the aEntryId parameter. The attachment is identified by 
	the file path specified by the aFilePath parameter. The attachment file is
	returned as a read/write file handle to the file in the message store. 
	
	@param aEntryId The entry whose attachment needs to be opened/read from the message store
	@param aFilePath The name and path of the attachment file to be opened
	@param aFile On return, The opened file handle for the attachment file.The ownership is transferred . The caller must close the file handle.
	*/
	virtual void OpenAttachmentForWriteL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)=0;
	
	
	/**
	Allow clients to delete existing attachment files.

	Deletes an existing attachment file in the message store associated with the
	message entry specified by the aEntryId parameter. The attachment is 
	identified by the file path specified by the aFilePath parameter. 
	
	@param aEntryId The entry whose attachment needs to be deleted from the message store
	@param aFilePath The name and path of the attachment file to be deleted
	@return KErrNone if successful, otherwise any of the system wide error codes.
	*/
	virtual TInt DeleteAttachment(TMsvId aEntryId, const TDesC& aFilePath)=0;
	
	/**
	Renames an existing attachment file.
	
	Renames an existing attachment file in the message store associated with the
	message entry specified by the aEntryId parameter. The attachment is 
	identified by the file path specified by the aOldFilePath parameter and renamed to
	the name supplied in the aNewName paramter.
	
	@param aEntryId The entry whose attachment needs to be renamed from the message store
	@param aOldFilePath The name and path of the attachment file to be renamed.
	@param aNewName The new name of the attachment file.
	@return KErrNone if successful, otherwise any of the system wide error codes.
	*/
	virtual TInt RenameAttachment(TMsvId aEntryId, const TDesC& aOldFilePath, const TDesC& aNewName) = 0;
	
	/**
	Check if an attachment file with the given path and name already exists.

    @param aFilePath The full path specification of the file.
	@return ETrue if file exists, otherwise EFalse.
	*/
	virtual TBool FileExistsL(const TDesC& aFilePath)=0;
	
	/**
	Queries the message server for the file path where attachments are stored for the
	a particular message entry.
	
	@param aEntryId The message entry for which the attachment path is required.
	@param aFilePath On return, this will be set to the file path where the attachment should be stored.
	*/
	virtual void AttachmentFilePathL(TMsvId aEntryId, TDes& aFilePath) = 0;
	
	// To support CMsvStore to provide message store file management.
	/**
	Allows the message store classes to read the store file for a particular message entry.

	Opens a message store file associated with the message entry specified by the aEntryId parameter. 
	This method returns an open read-only file handle to the message store file. 
	
	@param aEntryId The entry whose store needs to be opened/read 
	@param aFile On return, The opened file hanlde to the store . The ownership is transferred . The caller must close the file handle.
	@leave KErrNotFound if file is not found else any of the system wide errors
	*/
	virtual TInt OpenFileStoreForRead(TMsvId aEntryId, RFile& aFile)=0;
	
	/**
	Opens a temporary store file associated with the message entry specified by the aEntryId parameter. 
	This method returns an open read-write file handle with an exclusive share to the temporary store file. 
	The temporary store file is defined as \<store_filename\>.new. Data can be streamed to the temporary store file. 
	It is expected that once the temporary store file has been written to and then closed, a call to 
	ReplaceFileStoreL is required to replace the old store file with this temporary one. 
	
	Along with ReplaceFileStoreL, this method allows the message store classes to write data 
	to the store file for a particular message entry.
	
	@param aEntryId The entry whose store needs to be opened/read 
	@param aFile On return, The opened file hanlde to the store . The ownership is transferred . The caller must close the file handle.
	*/
	virtual void OpenTempStoreFileL(TMsvId aEntryId, RFile& aFile)=0;

	/**
	Replaces the current store file with the temporary store file, created from OpenTempStoreFileL,
	for the message entry as specified by the aEntryId parameter. The method replaces the store file with 
	the temporary file (<store_filename.new>) and if successful, will then delete the temporary store file.
	This method assumes that the temporary file exists, otherwise a KErrNotFound error occurs. 
	
	Along with OpenTempStoreFileL, this method allows the message store classes to write data to the store 
	file for a particular message entry.
	
	@param aEntryId The entry whose store needs to be replaced/overwritten 
	*/
	virtual void ReplaceFileStoreL(TMsvId aEntryId)=0;

	/**
	Deletes the store file associated with the message entry as specified by the aEntryId. 
	Also attempts to delete the temporary store file if one exists.
	
	@param aEntryId The entry whose store needs to be deleted
	*/
	virtual void DeleteFileStoreL(TMsvId aEntryId)=0;

	/**
	Queries the message server to check if a store file exists for the message entry specified by
	the aEntryId parameter. 
	
	@param aEntryId The entry for whom the check for existance of store is being done
	@return ETrue if the store exists else EFalse 
	*/
	virtual TBool FileStoreExistsL(TMsvId aEntryId) const=0;
	
	/**
	Creates an Attachment using a ShareProtected RFs.Used in the case when a message contains a DRM attachment

	@param aEntryId The entry for which an attachment needs to be created in the message store
	@param aFilename The name of the attachment file to be created
	@param aFile On return, The created file handle for the attachment file.The ownership is transferred . The caller must close the file handle.
	@leave KErrAlreadyexists, if the file with the same name already existed the directory.
	*/
	virtual TBool CreateShareProtectedAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile) = 0;	
	
	/**
	Gets the path of the bodytext file related to a message in the message store for the message id passed to it.
	@param aBodyTextId The Id of the bodytext for which the path is required in the message store
	@param aFilename On return, The path of the file corresponding to the message Id passed.
	@leave KErrAlreadyexists, if the file with the same name already existed the directory.
	*/
	virtual void BodyTextFilePathL(TMsvId aMessageId, TDes& aFilepath) = 0;
	
	/**
	Allows the message store classes to read the store file for a particular message entry.

	Opens a message store file associated with the message entry specified by the aFilepath parameter. 
	This method returns an open read-only file handle to the message store file. 
	
	@param aFile On return, The opened file hanlde to the store . The ownership is transferred . The caller must close the file handle.
	@param aBodyTextId The Id of the bodytext.
	@param aFilepath The path of the file that needs to be opened/read
	@leave KErrNotFound if file is not found else any of the system wide errors
	*/
	virtual void OpenBodyTextFileForReadL(RFile& aFile, TMsvId aBodyTextId, const TDesC& aFilePath) = 0;
	
	/**
	Allows the message store classes to create the store file for a particular message entry.

	Creates a message store file associated with the message entry specified by the aFilepath parameter. 
	This method returns an open read-write file handle to the message store file. 
	
	@param aFile On return, The opened file hanlde to the store . The ownership is transferred . The caller must close the file handle.
	@param aBodyTextId The Id of the bodytext.
	@leave one of the other system-wide error codes.
	*/
	virtual void CreatePlainTextFileL(RFile& aFile, TMsvId aBodyTextId) = 0;
	
	/**
	Deletes the store file associated with the message entry as specified by aFilepath. 
		
	@param aBodyTextId The Id of the bodytext.
	@leave KErrNotFound if file is not found else any of the system wide errors
	*/
	virtual void DeletePlainTextFileL(TMsvId aBodyTextId) = 0;
	
	/**
	Replaces the bodytext file associated with the message specified by aEntryId. 
	@param aBodyTextId The Id of the bodytext.
	@leave KErrNotFound if file is not found else any of the system wide errors
	*/
	virtual void ReplacePlainTextFileL(TMsvId aBodyTextId) = 0;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	virtual void CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)=0;
	virtual void LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList)=0;
	virtual void DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId)=0;
	virtual void UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)=0;
	virtual TBool DoesAnyStoreExists(TMsvId aId, TUid aMtmId)=0;
	virtual TBool DoesHeaderTableExist(const TUid& aMtmId)=0;
#endif


	};

#endif // __MMSVSTOREMANAGER_H__
