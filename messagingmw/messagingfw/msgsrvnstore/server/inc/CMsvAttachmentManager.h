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

#ifndef __CMSVATTACHMENTMANAGER_H__
#define __CMSVATTACHMENTMANAGER_H__

#include <e32base.h>

#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>
#include <cmsvattachment.h>

class CMsvStore;
class MMsvStoreManager;
class CCopyOneFile;
class RFs;
class CMsvAttachmentRename;

/**
Provides the implementation of the Attachment API.

The attachment API interface class manages all the attachment management and
uses MMsvStoreManager to provide access to the message store to manage the physical
attachment files and manipulate the attachment stream in the message store.

@see MMsvAttachmentManager
@internalComponent
*/
class CMsvAttachmentManager : public CActive,
							  public MMsvAttachmentManager,
							  public MMsvAttachmentManagerSync
	{
public:
	static CMsvAttachmentManager* NewL(CMsvStore& aStore, MMsvStoreManager& aStoreManager, const TMsvId aEntryId, RFs& aFs, TBool aReadOnly);
	~CMsvAttachmentManager();
	
	void RestoreL();
	void StoreL();
	void HandleCommitComplete();
	
	void CreateShareProtectedAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo);
	
private:
	CMsvAttachmentManager(CMsvStore& aStore, MMsvStoreManager& aStoreManager, const TMsvId aEntryId, RFs& aFs, TBool aReadOnly);
	void ConstructL();
	
	TMsvAttachmentId GenerateAttachmentId();
	TInt IndexPositionOfAttachmentL(TMsvAttachmentId aId) const;
	void CompleteSelf();
	void CreateAttachmentInStoreLC(TDes& aFileName, RFile& aFile, CMsvAttachment* aAttachmentInfo, TBool aIsShareProtected);
	void DoAttachmentRenameL(TInt aIndex, const TDesC& aNewName);
	TInt InternaliseRenameLockL();
	void ExternaliseRenameLockL();
		
	// from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
	// from MMsvAttachmentManager
	virtual void AddAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual void AddAttachmentL(RFile& aFileHandle, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual void AddLinkedAttachmentL(const TDesC& aFilePath, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual void AddEntryAsAttachmentL(TMsvId aEntryId, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual void RenameAttachmentL(TInt aIndex, const TDesC& aNewName, TRequestStatus& aStatus);
	virtual TInt AttachmentCount() const;
	virtual CMsvAttachment* GetAttachmentInfoL(TInt aIndex);
	virtual CMsvAttachment* GetAttachmentInfoL(TMsvAttachmentId aId);
	virtual void ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo, TRequestStatus& aStatus);
	virtual RFile GetAttachmentFileL(TInt aIndex);
	virtual RFile GetAttachmentFileL(TMsvAttachmentId aId);
	virtual RFile GetAttachmentFileForWriteL(TInt aIndex);
	virtual RFile GetAttachmentFileForWriteL(TMsvAttachmentId aId);
	virtual void RemoveAttachmentL(TInt aIndex, TRequestStatus& aStatus);
	virtual void RemoveAttachmentL(TMsvAttachmentId aId, TRequestStatus& aStatus);
	virtual void CancelRequest();

	// from MMsvAttachmentManagerSync
	virtual void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo);
	virtual void RenameAttachmentL(TInt aIndex, const TDesC& aNewName);
	virtual void RemoveAttachmentL(TInt aIndex);
	virtual void ModifyAttachmentInfoL(CMsvAttachment* aAttachmentInfo);
		
private:
	/** The message store representing the current message entry. */
	CMsvStore& iStore;
	
	/** The store manager providing access to the message store. */
	MMsvStoreManager& iStoreManager;
	
	/** The current message entry that the attachment manages. */
	const TMsvId iEntryId;
	
	/** A connected file server session. */
	RFs& iFs;
	
	/** Array of attachments that are associated with the entry. */
	RPointerArray<CMsvAttachment> iAttachments;
	
	/** A persisted value used to generate attachment Ids to assign to new attachments. */
	TUint iIdGenerator;
	
	/** An array to list the attachments that are due for deletion. */
	RPointerArray<HBufC> iDeletedAttachments;
	
	/** An array to list the attachment files that have been copied into the store
		but not yet commited. */
	RPointerArray<HBufC> iAddedAttachments;
	
	/** Utility for copying a single file. */
	CCopyOneFile* iCopyOneFile;
	
	/** Flag indicating whether the store is Read-Only. */
	TBool iReadOnly;
	
	/** Caller's request status to complete. */
	TRequestStatus* iReportStatus;
	
	/** An array to manage the renaming of attachments. */
	RPointerArray<CMsvAttachmentRename> iRenamed;
	};

#endif // __CMSVATTACHMENTMANAGER_H__
