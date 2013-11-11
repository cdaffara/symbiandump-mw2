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

#include <msvstore.h>
#include <f32file.h>

#include "CMsvAttachmentManager.h"
#include <mmsvstoremanager.h>
#include "CCopyOneFile.h"
#include "CMsvAttachmentRename.h"
#include "TMsvServerStoreManager.h"

const TUid KMsvEntryAttachmentInfo={0x10202D55};
_LIT(KTxtRenameLockFile, "_Rename_._");

CMsvAttachmentManager* CMsvAttachmentManager::NewL(CMsvStore& aStore, MMsvStoreManager& aStoreManager, const TMsvId aEntryId, RFs& aFs, TBool aReadOnly)
	{
	CMsvAttachmentManager* me = new(ELeave) CMsvAttachmentManager(aStore, aStoreManager, aEntryId, aFs, aReadOnly);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}
	
CMsvAttachmentManager::CMsvAttachmentManager(CMsvStore& aStore, MMsvStoreManager& aStoreManager, const TMsvId aEntryId, RFs& aFs, TBool aReadOnly)
	:CActive(EPriorityStandard), iStore(aStore), iStoreManager(aStoreManager), iEntryId(aEntryId), iFs(aFs), iReadOnly(aReadOnly)
	{
	CActiveScheduler::Add(this);
	}
	
void CMsvAttachmentManager::ConstructL()
	{
	// Only need the CCopyOneFile if attachment is not readonly
	if( !iReadOnly )
		iCopyOneFile = CCopyOneFile::NewL(iFs);
	
	// Populate the array of attachments from store file that contains all of the data
	RestoreL();
	}
	
CMsvAttachmentManager::~CMsvAttachmentManager()
	{
	Cancel();
	
	// if we have any added attachments, these must be deleted as they have not been committed
	TInt addedAttachmentCount = iAddedAttachments.Count();
	for( TInt ii=0; ii<addedAttachmentCount; ++ii )
		{
		iStoreManager.DeleteAttachment(iEntryId, *iAddedAttachments[ii]); // ignore return code
		}
		
	iAddedAttachments.ResetAndDestroy();
	iDeletedAttachments.ResetAndDestroy();	
	iAttachments.ResetAndDestroy();
	iRenamed.ResetAndDestroy();
	delete iCopyOneFile;
	}

void CMsvAttachmentManager::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	User::RequestComplete(iReportStatus, KErrNone);
	}
	
void CMsvAttachmentManager::DoCancel()
	{
	__ASSERT_DEBUG(iCopyOneFile!=NULL, User::Invariant());
	
	if( iCopyOneFile->IsActive() )
		iCopyOneFile->Cancel();
	}

TInt CMsvAttachmentManager::RunError(TInt aError)
	{
	// Asynchronous request only occur when an attachment is added
	// so delete the last added attachment
	TInt count = iAttachments.Count();
	--count;
	if (count > 0)
		{
		delete iAttachments[count];
		iAttachments.Remove(count);
		}

	count = iAddedAttachments.Count();
	--count;
	if (count > 0)
		{
		delete iAddedAttachments[count];
		iAddedAttachments.Remove(count);
		}
	User::RequestComplete(iReportStatus, aError);
	return KErrNone;
	}

void CMsvAttachmentManager::CompleteSelf()
	{
	iStatus=KRequestPending;
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	}
	
void CMsvAttachmentManager::RestoreL()
	{
	// Before loading, ensure that array is flushed out, all changes since last
	// store commit will be lost
	iAttachments.ResetAndDestroy();
	iDeletedAttachments.ResetAndDestroy();
	
	// if any added attachment file were added since the last commit, these must be removed
	TInt addedAttachmentCount = iAddedAttachments.Count();
	for( TInt ii=0; ii<addedAttachmentCount; ++ii )
		{
		iStoreManager.DeleteAttachment(iEntryId, *iAddedAttachments[ii]); // ignore return error
		}
	// All the added attachments have been cleaned up, reset the list
	iAddedAttachments.ResetAndDestroy();
	
	// Only load the attachment info stream if one exists
	if(iStore.IsPresentL(KMsvEntryAttachmentInfo))
		{	
		RMsvReadStream readStream;
		readStream.OpenLC(iStore, KMsvEntryAttachmentInfo);
		
		// Read the data...
		// 1. the number of attachments
		TInt attachmentCount = readStream.ReadInt32L();
		
		TFileName attachmentFilePath;
		
		// 2. populate the attachment array contents
		// for every object create and populate it from the stream
		for(TInt ii=0; ii<attachmentCount; ++ii)
			{
			// create empty attachment object, doesn't matter what type as the
			// correct attachment type is set when it is internalised
			CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
			CleanupStack::PushL(attachment);
			readStream >> *attachment;
			if( attachment->IsPathRequired() )
				{
				// The attachment path needs to be dynamically set, only get the file path if needed
				if( attachmentFilePath.Length()==0 )
					{
					iStoreManager.AttachmentFilePathL(iEntryId, attachmentFilePath);
					}
				
				TParse fullPath;
				User::LeaveIfError(fullPath.Set(attachmentFilePath, &attachment->FilePath(), NULL));
				attachment->SetFilePathL(fullPath.FullName());
				}
				
			User::LeaveIfError(iAttachments.Append(attachment));
			CleanupStack::Pop(attachment);
			}
			
		// 3. attachment id generator
		iIdGenerator = readStream.ReadUint32L();
		
		CleanupStack::PopAndDestroy(&readStream);
		}
		
	// Restore any rename info from the rename lock file
	TInt err = InternaliseRenameLockL();
	if( err==KErrNone )
		{
		// lock file exists
		TInt currentIndex = iRenamed.Count();
		while( currentIndex>0 )
			{
			// We have some files that have been renamed and need to
			// be renamed back to the old name, these are renamed in
			// reverse order in case any one single attachment was
			// renamed twice before committing.
			--currentIndex;
			CMsvAttachmentRename* renameInfo = iRenamed[currentIndex];
			iStoreManager.RenameAttachment(iEntryId, renameInfo->NewName(), renameInfo->OldName()); // ignore errors
			}
			
		iRenamed.ResetAndDestroy();
		
		// delete the lock file as it no longer needed
		iStoreManager.DeleteAttachment(iEntryId, KTxtRenameLockFile); // ignore return error
		}
	}
	
void CMsvAttachmentManager::HandleCommitComplete()
	{
	// The attachments have been commited, the cache of added files can be removed
	iAddedAttachments.ResetAndDestroy();
	
	// Any attachment files that are marked for delete, must be deleted from the message store
	TInt deleteAttachmentCount = iDeletedAttachments.Count();
	for( TInt ii=0; ii<deleteAttachmentCount; ++ii )
		{
		iStoreManager.DeleteAttachment(iEntryId, *iDeletedAttachments[ii]); // ignore errors
		}
	iDeletedAttachments.ResetAndDestroy();
	
	// Remove the rename info as changes have been committed
	iStoreManager.DeleteAttachment(iEntryId, KTxtRenameLockFile); // ignore return error
	iRenamed.ResetAndDestroy();
	}
	
void CMsvAttachmentManager::StoreL()
	{
	__ASSERT_DEBUG(iReadOnly==EFalse, User::Invariant());
	
	TInt attachmentCount = iAttachments.Count();
	
	// If there are no attachments, remove the stream
	if(attachmentCount==0)
		{
		iStore.Remove(KMsvEntryAttachmentInfo);
		}
	else
		{
		RMsvWriteStream writeStream;
		writeStream.AssignLC(iStore, KMsvEntryAttachmentInfo);
		
		// Write the data...
		// 1. the number of attachments
		writeStream.WriteInt32L(attachmentCount);
		
		// 2. save the attachment array contents
		// for every object create and populate it from the stream
		for(TInt ii=0; ii<attachmentCount; ++ii)
			{
			CMsvAttachment* attachment = iAttachments[ii];
			writeStream << *attachment;
			}
		
		// 3. attachment id generator
		writeStream.WriteUint32L(iIdGenerator);
		
		writeStream.CommitL();
		CleanupStack::PopAndDestroy(&writeStream);
		}
	}

TMsvAttachmentId CMsvAttachmentManager::GenerateAttachmentId()
	{
	return ++iIdGenerator;
	}
	
void CMsvAttachmentManager::CreateAttachmentInStoreLC(TDes& aFileName, RFile& aFile, CMsvAttachment* aAttachmentInfo, TBool aIsShareProtected)
	{
	if( aFileName.CompareF(KTxtRenameLockFile)==0 )
		{
		// Cant add attachments using the same name as the rename lock file
		User::Leave(KErrBadName);
		}
	
	if(aIsShareProtected)
		{
		iStoreManager.CreateShareProtectedAttachmentForWriteL(iEntryId, aFileName, aFile);
		}
	else
		{				
		iStoreManager.CreateAttachmentForWriteL(iEntryId, aFileName, aFile);
		}
	
	CleanupClosePushL(aFile);
	
	HBufC* addedFilePath = aFileName.AllocLC();
	User::LeaveIfError(iAddedAttachments.Append(addedFilePath));
	CleanupStack::Pop(addedFilePath);
	
	// initailise attachment info and take ownership
	aAttachmentInfo->SetFilePathL(aFileName);
	User::LeaveIfError(iAttachments.Append(aAttachmentInfo));
	aAttachmentInfo->SetId(GenerateAttachmentId());
	}

void CMsvAttachmentManager::AddAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvFile, User::Invariant());

	RFile fileToCopy;
	User::LeaveIfError(fileToCopy.Open(iFs, aFilePath, EFileRead|EFileShareReadersOnly));
	CleanupClosePushL(fileToCopy);
	
	TFileName fileName;
	User::LeaveIfError(fileToCopy.Name(fileName));
	
	RFile newFileHandle;
	CreateAttachmentInStoreLC(fileName, newFileHandle, aAttachmentInfo, EFalse);

	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	
	iCopyOneFile->Copy(fileToCopy, newFileHandle, iStatus);
	CleanupStack::Pop(2, &fileToCopy); // newFileHandle, fileToCopy
	SetActive();
	}
		
void CMsvAttachmentManager::AddAttachmentL(RFile& aFileHandle, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvFile, User::Invariant());
	
	TFileName fileName;
	User::LeaveIfError(aFileHandle.Name(fileName));
	
	RFile newFileHandle;
	CreateAttachmentInStoreLC(fileName, newFileHandle, aAttachmentInfo, EFalse);
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	
	iCopyOneFile->Copy(aFileHandle, newFileHandle, iStatus);
	CleanupStack::Pop(&newFileHandle);
	SetActive();
	}

void CMsvAttachmentManager::AddLinkedAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvLinkedFile, User::Invariant());
	
	// initailise attachment info and take ownership
	aAttachmentInfo->SetFilePathL(aFilePath);
	User::LeaveIfError(iAttachments.Append(aAttachmentInfo));
	aAttachmentInfo->SetId(GenerateAttachmentId());
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	CompleteSelf();
	}
	
void CMsvAttachmentManager::AddEntryAsAttachmentL(TMsvId aEntryId, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvMessageEntry, User::Invariant());
	
	// initailise attachment info and take ownership
	User::LeaveIfError(iAttachments.Append(aAttachmentInfo));
	aAttachmentInfo->SetId(GenerateAttachmentId());
	aAttachmentInfo->SetEntryAttachmentId(aEntryId);
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	User::RequestComplete(iReportStatus, KErrNone);
	}
	
void CMsvAttachmentManager::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	CreateAttachmentL(aFileName, aAttachmentFile, aAttachmentInfo);
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	CompleteSelf();
	}
	
	
TInt CMsvAttachmentManager::AttachmentCount() const
	{
	return iAttachments.Count();
	}
	
CMsvAttachment* CMsvAttachmentManager::GetAttachmentInfoL(TInt aIndex)
	{
	// Create a deep copy the CMsvAttachment object to pass back to the client
	CMsvAttachment* currentAttachment = iAttachments[aIndex];
	
	return CMsvAttachment::NewL(*currentAttachment);
	}

CMsvAttachment* CMsvAttachmentManager::GetAttachmentInfoL(TMsvAttachmentId aId)
	{
	return GetAttachmentInfoL(IndexPositionOfAttachmentL(aId));
	}

void CMsvAttachmentManager::ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	// Get the index position of the attachment info with the same Id
	TInt attachmentIndex = IndexPositionOfAttachmentL(aAttachmentInfo->Id());
	__ASSERT_ALWAYS(aAttachmentInfo->Type()==iAttachments[attachmentIndex]->Type(), User::Invariant());
	
	// Insert the replaced attachment info object into the same position and then
	// remove the old one
	User::LeaveIfError(iAttachments.Insert(aAttachmentInfo, attachmentIndex));
	++attachmentIndex;
	delete iAttachments[attachmentIndex];
	iAttachments.Remove(attachmentIndex);
	}
	
void CMsvAttachmentManager::ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus)
	{
	ModifyAttachmentInfoL(aAttachmentInfo);	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	CompleteSelf();
	}

RFile CMsvAttachmentManager::GetAttachmentFileL(TInt aIndex)
	{
	CMsvAttachment* attachment = iAttachments[aIndex];
	
	// Make sure that the attachment is a file attachment
	if( attachment->Type() == CMsvAttachment::EMsvMessageEntry )
		User::Leave(KErrNotSupported);
	
	RFile attachmentFile;
	
	if( attachment->Type() == CMsvAttachment::EMsvFile )
		{		
		iStoreManager.OpenAttachmentL(iEntryId, attachment->FilePath(), attachmentFile);
		}
	else // attachment->iType == ELinkedFile
		{
		RFs fs;
		User::LeaveIfError(fs.Connect());
		User::LeaveIfError(fs.ShareProtected());  		
		User::LeaveIfError(attachmentFile.Open(fs, attachment->FilePath(), EFileRead | EFileShareReadersOnly));
		}
	
	// handle to attachment file allocated, return to caller
	return attachmentFile;
	}

RFile CMsvAttachmentManager::GetAttachmentFileL(TMsvAttachmentId aId)
	{
	return GetAttachmentFileL(IndexPositionOfAttachmentL(aId));
	}

RFile CMsvAttachmentManager::GetAttachmentFileForWriteL(TInt aIndex)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	CMsvAttachment* attachment = iAttachments[aIndex];
	
	// Make sure that the attachment is a file attachment
	if( attachment->Type() == CMsvAttachment::EMsvMessageEntry )
		User::Leave(KErrNotSupported);
	
	RFile attachmentFile;
	
	if( attachment->Type() == CMsvAttachment::EMsvFile )
		{		
		iStoreManager.OpenAttachmentForWriteL(iEntryId, attachment->FilePath(), attachmentFile);
		}
	else // attachment->iType == ELinkedFile
		{
		RFs fs;
		User::LeaveIfError(fs.Connect());
		User::LeaveIfError(fs.ShareProtected());
		User::LeaveIfError(attachmentFile.Open(fs, attachment->FilePath(), EFileWrite));
		}
	
	// handle to attachment file allocated, return to caller
	return attachmentFile;
	}

RFile CMsvAttachmentManager::GetAttachmentFileForWriteL(TMsvAttachmentId aId)
	{
	return GetAttachmentFileForWriteL(IndexPositionOfAttachmentL(aId));
	}
	
void CMsvAttachmentManager::RemoveAttachmentL(TInt aIndex, TRequestStatus& aStatus)
	{
	RemoveAttachmentL(aIndex);
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	CompleteSelf();
	}
	
void CMsvAttachmentManager::RemoveAttachmentL(TMsvAttachmentId aId, TRequestStatus& aStatus)
	{
	RemoveAttachmentL(IndexPositionOfAttachmentL(aId), aStatus);
	}

TInt CMsvAttachmentManager::IndexPositionOfAttachmentL(TMsvAttachmentId aId) const
	{
	TInt foundPosition = KErrNotFound;
	TInt attachmentCount = iAttachments.Count();
	// Go through the attachment list to find the attachment by its id
	for( TInt ii=0; ii<attachmentCount; ++ii )
		{
		if( iAttachments[ii]->Id() == aId )
			{
			foundPosition = ii;
			break;
			}
		}
		
	// Leave with KErrNotFound if the attachment id is not found
	User::LeaveIfError(foundPosition);
	
	// return the index position of attachment
	return foundPosition;
	}

void CMsvAttachmentManager::CancelRequest()
	{
	Cancel();
	User::RequestComplete(iReportStatus, KErrCancel);
	}
	
void CMsvAttachmentManager::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvFile, User::Invariant());
	
	// File name must be provided
	if( aFileName.Length() == 0 || aAttachmentFile.SubSessionHandle() != KNullHandle)
		User::Leave(KErrArgument);
	
	TFileName fileName = aFileName;
	
	CreateAttachmentInStoreLC(fileName, aAttachmentFile, aAttachmentInfo, EFalse);
	CleanupStack::Pop(&aAttachmentFile);
	}

void CMsvAttachmentManager::CreateShareProtectedAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	__ASSERT_ALWAYS(aAttachmentInfo->Type() == CMsvAttachment::EMsvFile, User::Invariant());
	
	// File name must be provided
	if( aFileName.Length() == 0 || aAttachmentFile.SubSessionHandle() != KNullHandle)
		User::Leave(KErrArgument);
	
	TFileName fileName = aFileName;
	
	CreateAttachmentInStoreLC(fileName, aAttachmentFile, aAttachmentInfo, ETrue);
	CleanupStack::Pop(&aAttachmentFile);
	}
	
TInt CMsvAttachmentManager::InternaliseRenameLockL()
	{
	iRenamed.ResetAndDestroy();
	
	// if the rename lock file exists internalise it
	RFile renameLockFile;
	TRAPD(err, iStoreManager.OpenAttachmentL(iEntryId, KTxtRenameLockFile, renameLockFile));
	if( err==KErrNone )
		{
		// got the rename lock file
		RFileReadStream renameReadStream(renameLockFile);
		renameReadStream.PushL();
		TInt renamedCount = renameReadStream.ReadInt32L();
		for( TInt ii=0; ii<renamedCount; ++ii )
			{
			CMsvAttachmentRename* renameInfo = CMsvAttachmentRename::NewLC();
			renameReadStream >> *renameInfo;
			iRenamed.AppendL(renameInfo);
			CleanupStack::Pop(renameInfo);
			}
		CleanupStack::PopAndDestroy(); // renameReadStream
		}
	else if( err==KErrNotFound || err==KErrPathNotFound )
		{
		err = KErrNotFound; // rename lock file not found
		}
	else
		{
		User::Leave(err);
		}
		
	return err; 
	}
	
void CMsvAttachmentManager::ExternaliseRenameLockL()
	{
	__ASSERT_DEBUG(iRenamed.Count()>0, User::Invariant());
	
	// Get the rename lock file to store the rename information
	RFile renameLockFile;
	TFileName lockFileName(KTxtRenameLockFile());
	iStoreManager.ReplaceAttachmentForWriteL(iEntryId, lockFileName, renameLockFile);
	RFileWriteStream renameWriteStream(renameLockFile);
	renameWriteStream.PushL();
	
	// Externalise the count and each rename file info object
	TInt renamedCount = iRenamed.Count();
	renameWriteStream.WriteInt32L(renamedCount);
	for( TInt ii=0; ii<renamedCount; ++ii )
		{
		renameWriteStream << *(iRenamed[ii]);
		}
	
	renameWriteStream.CommitL();	
	CleanupStack::PopAndDestroy(); // renameWriteStream
	}
	
void CMsvAttachmentManager::DoAttachmentRenameL(TInt aIndex, const TDesC& aNewName)
	{
	if(iReadOnly)
		{
		User::Leave(KErrAccessDenied);
		}
	
	CMsvAttachment* renameAttachment = iAttachments[aIndex];	
	__ASSERT_ALWAYS(renameAttachment->Type() == CMsvAttachment::EMsvFile, User::Invariant());	
	
	// Build the attachment rename infomation and store in array
	TPtrC oldFilePath(renameAttachment->FilePath());
	CMsvAttachmentRename* renameInfo = CMsvAttachmentRename::NewLC(oldFilePath, aNewName);
	iRenamed.AppendL(renameInfo);
	CleanupStack::Pop(renameInfo);
	
	// Save the rename infomation
	ExternaliseRenameLockL();
	
	// Rename and update the attachment info
	TParse newFilePath;
	User::LeaveIfError(newFilePath.Set(aNewName, &oldFilePath, NULL));
	User::LeaveIfError(iStoreManager.RenameAttachment(iEntryId, oldFilePath, aNewName));
	renameAttachment->SetFilePathL(newFilePath.FullName());
	}
	
void CMsvAttachmentManager::RenameAttachmentL(TInt aIndex, const TDesC& aNewName)
	{
    DoAttachmentRenameL(aIndex, aNewName);
	}

void CMsvAttachmentManager::RenameAttachmentL(TInt aIndex, const TDesC& aNewName, TRequestStatus& aStatus)
	{
	DoAttachmentRenameL(aIndex, aNewName);
	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	User::RequestComplete(iReportStatus, KErrNone);			
	}

void CMsvAttachmentManager::RemoveAttachmentL(TInt aIndex)
	{
	if(iReadOnly)
		User::Leave(KErrAccessDenied);
	
	// If the attachment is a file attachment, we need remove the attachment object
	// and mark the file for deletion so that it is deleted only when SaveL is called.
	// This ensures that if the store is reverted, the attachment files are still in
	// the message store
	CMsvAttachment* deleteAttachment = iAttachments[aIndex];
	
	// only need to mark the file for deletion if it is an attachment stored in the
	// message server, ie. dont delete linked attachments
	if( deleteAttachment->Type() == CMsvAttachment::EMsvFile )
		{
		HBufC* deleteFilePath = deleteAttachment->FilePath().AllocLC();
		User::LeaveIfError(iDeletedAttachments.Append(deleteFilePath));
		CleanupStack::Pop(deleteFilePath);
		} 
	
	iAttachments.Remove(aIndex);
	delete deleteAttachment;
	}
	
