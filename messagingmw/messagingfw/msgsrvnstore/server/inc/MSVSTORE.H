// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MSVSTORE.H
//
/**
 * @file 
 * @publishedAll
 * @released
 */
#if !defined(__MSVSTORE_H__)
#define __MSVSTORE_H__

#include <e32base.h>
#include <s32file.h>
#include <msvstd.h>

class CMsvCachedStore;
class CMsvBodyText;
class CMsvAttachment;

//**********************************
// MMsvStoreObserver
//**********************************
//
// 
//

class MMsvStoreObserver
/** Observer interface to get events relating to a message store.

This is used in the derivation of CMsvServerEntry and CMsvEntry.

@publishedAll
@released
*/
	{
public:
/**
Defines message store event types.

@publishedAll
@released
*/
	enum TMsvStoreEvent {	/** A read-only message store was closed. */
							EMsvReadStoreClosed,	// read only CMsvStore closed
							/** An editable message store was closed. */
							EMsvEditStoreClosed		// edit only CMsvStore closed
							};	
public:
	/** Receives a message store event.

	@param aEvent Event type
	@param aId ID of the message to which the event relates
	*/
	virtual void HandleStoreEvent(TMsvStoreEvent aEvent, TMsvId aId)=0;
	};


//**********************************
// CMsvStore
//**********************************
//
// The entry structure that the client uses
//

/*
Uid which are not needed to be known outside CMsvStore
@publishedAll
@released
*/
const TUid KMsvEntryRichTextBody={0x10000F70};

class MMsvAttachmentManager;
class MMsvAttachmentManagerSync;
class CMsvAttachmentManager;
class MMsvStoreManager;
class CMsvPlainBodyText;


class CMsvStore : public CBase
/** Provides an interface over the message store that is associated with a message 
entry. It is similar to the dictionary store in supporting the concept of 
streams referenced by UID. 

This class is intended for use by MTM implementations. Message client applications 
access the store through the higher-level functions provided by Client-side 
and User Interface MTMs.

A CMsvStore object is created by a CMsvEntry object and passed to a client 
process. The client process then becomes responsible for deleting the CMsvStore 
object. The store can be opened in two modes, read or edit, and only one CMsvStore 
object can have edit access to a store. 

CMsvStore provides functions for general manipulation of the store, and for 
accessing the standard body text stream. RMsvReadStream and RMsvWriteStream 
must be used to access other streams. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C ~CMsvStore();
	//
	IMPORT_C void StoreBodyTextL(const CRichText& aRichTextBody);
	
	IMPORT_C void RestoreBodyTextL(CRichText& aRichTextBody);
	IMPORT_C void RestoreBodyTextL(CRichText& aRichTextBody, TUint aCharsetOverride);
	
	IMPORT_C void DeleteBodyTextL();
	IMPORT_C TBool HasBodyTextL() const;
	//
	IMPORT_C void DeleteL(); // deletes the message store
	IMPORT_C TInt SizeL(); // the size of the store
	//
	// The following give the Dictionary store interface
	IMPORT_C TBool IsNullL() const;
	IMPORT_C TBool IsPresentL(TUid aUid) const;
	IMPORT_C void Remove(TUid aUid);
	IMPORT_C void RemoveL(TUid aUid);
	IMPORT_C void Revert();
	IMPORT_C void RevertL();
	IMPORT_C TInt Commit(); 
	IMPORT_C void CommitL();

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	IMPORT_C TBool IsDbStore();
	void CommitHeaderL();
#endif

	// Attachment Management
	IMPORT_C MMsvAttachmentManager& AttachmentManagerL();
	IMPORT_C MMsvAttachmentManagerSync& AttachmentManagerExtensionsL();
	
	// APIs to get the CMsvPlainBodyText interface.
	IMPORT_C CMsvPlainBodyText* InitialisePlainBodyTextForWriteL(TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId);
	IMPORT_C CMsvPlainBodyText* InitialisePlainBodyTextForReadL(TInt aChunkLength);

	IMPORT_C void CreateShareProtectedAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo);
	void Restore8BitBodyTextL(RFileReadStream& aInputStream);
protected:
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	IMPORT_C static CMsvStore* OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId);
	IMPORT_C static CMsvStore* OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId);
#endif
	IMPORT_C CMsvStore(MMsvStoreObserver& aObserver, RFs& aFs, TMsvId aId, MMsvStoreManager& aStoreManager);
	IMPORT_C static CMsvStore* OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);
	IMPORT_C static CMsvStore* OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);
	
private:
	
	void ConstructL(TBool aReadOnly);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	void ConstructDbL(TUid aMtmId,TBool aReadOnly);
	void RestoreL();
	void LoadHeaderEntryL(const TUid aMtmId, TBool aUid);
#endif

	//Methods to handle plain bodytext.
	void RestorePlainBodyTextL(CRichText& aRichText, TUint aCharsetOverride);
	void GetRichTextFrom8BitL(RFile& aBodyTextFile, CRichText& aRichText, TUint aCharSet, TUint aDefaultCharSet);
	void GetRichTextFrom16BitL(RFile& aBodyTextFile, CRichText& aRichText);
	void Convert8BitToRichTextL(RFile& aBodyTextFile, CRichText& aRichText, TUint aCharSet, TUint aDefaultCharSet);

	void Lock();
	inline const CMsvCachedStore& Store() const;
	inline CMsvCachedStore& Store();
private:
	enum {EMsvStoreUnlocked, EMsvStoreLocked} iLockStatus;

	RFs& iFs;
	MMsvStoreObserver& iObserver;
	

	const TMsvId iId;
	CMsvCachedStore* iStore;
	TBool iConstructed;
	CMsvBodyText* iBodyText;
	
	MMsvStoreManager& iStoreManager;
	CMsvAttachmentManager* iAttachmentManager;

friend class CMsvEntry;
friend class CMsvServerEntry;
friend class RMsvReadStream;
friend class RMsvWriteStream;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
friend class TMsvWriteStore;
friend class TMsvReadStore;
#endif
	};


class RMsvReadStream : public RReadStream
/** Accesses the streams in a message store with read access. 

Before it is used, 
a CMsvStore must have been opened on the message store with read or read/write 
access.

This class is intended for use by MTM implementations to store MTM-specific 
information. Message client applications access the store through the higher-level 
functions provided by Client-side and User Interface MTMs.

Most of its functionality is provided by the base class RReadStream. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C void OpenL(const CMsvStore& aMsvStore, TUid aUid);
	IMPORT_C void OpenLC(const CMsvStore& aMsvStore,TUid aUid);
	void OpenLC(CMsvCachedStore& aStore,TUid aUid);
	};


class RMsvWriteStream : public RWriteStream
/** Accesses the streams in a message store with write access, or creates new streams. 
Before it is used, a CMsvStore must have been opened on the message store 
with write access.

This class is intended for use by MTM implementations to store MTM-specific 
information. Message client applications access the store through the higher-level 
functions provided by Client-side and User Interface MTMs.

Most of its functionality is provided by the base class RWriteStream. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C void AssignLC(CMsvStore &aMsvStore, TUid aUid);
	IMPORT_C void AssignL(CMsvStore &aMsvStore, TUid aUid);
	void AssignLC(CMsvCachedStore &aStore, TUid aUid);
	};



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/** Accesses the message store with write access, or creates new header entry. 
Before it is used, a CMsvStore must have been opened on the message store 
with write access.

This class is intended for use by MTM implementations to store MTM-specific 
information. Message client applications access the store through the higher-level 
functions provided by Client-side and User Interface MTMs.

@publishedAll
@released
*/
class TMsvWriteStore
	{
public:	
	IMPORT_C TMsvWriteStore(CMsvStore& aMsvStore);
	IMPORT_C void AssignL(CHeaderFields* aHeaderFields);
	IMPORT_C void CommitL();	
public:
	CMsvStore& iMsvStore;
	};
	
/** Accesses the a message store with read access. 

Before it is used, 
a CMsvStore must have been opened on the message store with read or read/write 
access.

This class is intended for use by MTM implementations to store MTM-specific 
information. Message client applications access the store through the higher-level 
functions provided by Client-side and User Interface MTMs.

@publishedAll
@released
*/
	
class TMsvReadStore
	{
public:	
	IMPORT_C TMsvReadStore(CMsvStore& aMsvStore, TUid aUid);
	IMPORT_C void LoadL(CHeaderFields*& aHeaderFields);
	IMPORT_C void ReadL(CHeaderFields*& aHeaderFields);
public:
	CMsvStore& iMsvStore;
	TUid iUid;
	};
#endif

#endif
