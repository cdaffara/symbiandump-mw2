// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TMSVSERVERSTOREMANAGER_H__
#define __TMSVSERVERSTOREMANAGER_H__

#include <mmsvstoremanager.h>
/**
Provides implementation of the File Access APIs at the server-side.
This class has direct access to the Message store, unlike the client version of it.
This can be used by the server side classed to access the store and attachment entries.

@see MMsvStoreManager , TMsvServerStoreManager implements MMsvStoreManager
@internalComponent
*/
class TMsvServerStoreManager : public MMsvStoreManager 
	{
public:
	TMsvServerStoreManager();
	void SetMsvServerAndFileSession(CMsvServer& aMsvServer,RFs& aFs);
	
	// from MMsvStoreManager
	virtual TBool CreateAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	virtual void ReplaceAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	virtual void OpenAttachmentL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile);
	virtual void OpenAttachmentForWriteL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile);
	virtual TInt DeleteAttachment(TMsvId aEntryId, const TDesC& aFilePath);
	virtual TInt RenameAttachment(TMsvId aEntryId, const TDesC& aOldFilePath, const TDesC& aNewName);
	virtual TBool FileExistsL(const TDesC& aFilePath);
	virtual void AttachmentFilePathL(TMsvId aEntryId, TDes& aFilePath);
	virtual TInt OpenFileStoreForRead(TMsvId aEntryId, RFile& aFile);
	virtual void OpenTempStoreFileL(TMsvId aEntryId, RFile& aFile);
	virtual void ReplaceFileStoreL(TMsvId aEntryId);
	virtual void DeleteFileStoreL(TMsvId aEntryId);
	virtual TBool FileStoreExistsL(TMsvId aEntryId) const;
	
	TBool CreateShareProtectedAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	
	void BodyTextFilePathL(TMsvId aBodyTextId, TDes& aFilePath);
	void OpenBodyTextFileForReadL(RFile& aFile, TMsvId aBodyTextId, const TDesC& aFilePath);
	void CreatePlainTextFileL(RFile& aFile, TMsvId aBodyTextId);
	void DeletePlainTextFileL(TMsvId aBodyTextId);
	void ReplacePlainTextFileL(TMsvId aBodyTextId);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	//PREQ2073
	void CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList);
	void LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList);
	void DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId);
	void UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList);
	TBool DoesAnyStoreExists(TMsvId aId, TUid aMtmId);
	TBool DoesHeaderTableExist(const TUid& aMtmId);
#endif

		
private:
	TInt GetFilePath(TMsvId aEntryId,TFileName& aDirectory) const;
	void GetStoreFileName(TMsvId aEntryId, TFileName& aFileName) const;	
	TBool CheckFileNameLengthAndTrimL(const TDesC& aFilePath,TDes& aFileName);
	TBool GenerateFileNameIfExistsL(TDes& aFileName);
	void GetOwningServiceAndFilePath(TMsvId aEntryId,TDes& aFilePath,TMsvId& aOwningService) const;	
	TBool CreateAttachmentL(TMsvId aEntryId, TDes& aFilename, RFile& aFile, RFs& aFs,TBool aSharedAccess = EFalse);

private:
	// Does not own these members. They are set to CMsvServer and RFs 
	// references in the class where the TMsvServerStoreManager object is created.
	CMsvServer* iMsvServer;	
	RFs* iFs;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestMsvServer;
#endif	
	};

#endif // __TMSVSERVERSTOREMANAGER_H__
