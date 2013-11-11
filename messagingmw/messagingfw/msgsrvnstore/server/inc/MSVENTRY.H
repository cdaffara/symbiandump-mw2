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
//

#ifndef __MSVENTRY_H__
#define __MSVENTRY_H__



#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTORE_H__)
#include <msvstore.h>
#endif

//
// Forward declarations
class CMsvStore;
class CMsvCopyMoveEntriesBase;
class CMsvServer;

//**********************************
// CMsvServerEntry
//**********************************
//
//
//

class CMsvServerEntry : public CActive, public MMsvStoreObserver
/** Accesses and acts upon a particular Message Server entry. 

It provides similar 
functionality to that which CMsvEntry gives to client-side programs. The current 
entry that a CMsvServerEntry object relates is similarly referred to as its 
context. 

A difference to note is that CMsvEntry functions, when used on a remote context, 
can result in requests to Server-side MTMs to change data on a remote server, 
as well as the local Message Server index. Naturally, as CMsvServerEntry is 
designed to be used by Server-side MTMs themselves, its comparable functions 
only alter the Message Server index.

A CBaseServerMTM-derived object gets an initial CMsvServerEntry on construction. 
It can get further CMsvServerEntry objects by calling NewEntryL(). The context 
can be changed by SetEntry().

The context is locked, preventing it being accessed by other MTMs. The lock 
is released when the object is deleted, or the context changes.

As with CMsvEntry, CBaseServerMTM functions can be divided into two broad 
groups. The first provides means to access the various types of storage associated 
with an entry. The second provides a means to discover and access other entries 
that the entry owns (its children).  
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvServerEntry* NewL(CMsvServer& aServer, TMsvId aId);
	~CMsvServerEntry();
	//
	// Set and get the current context
	IMPORT_C TInt SetEntry(TMsvId aId);
	inline const TMsvEntry& Entry() const;
	IMPORT_C TMsvId OwningService() const;
	//
	// current entry only functions
	IMPORT_C TInt ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId);
	IMPORT_C TInt ChangeEntry(const TMsvEntry& aEntry);
	

	//
	// child entry only functions
	IMPORT_C TInt CreateEntry(TMsvEntry& aEntry);
	IMPORT_C TInt CreateEntry(TMsvEntry& aEntry, TSecureId aOwnerId);
	IMPORT_C TInt CreateEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aBulk);
	IMPORT_C TInt DeleteEntry(TMsvId aId);
	IMPORT_C TInt DeleteEntries(CMsvEntrySelection& aSelection);
	IMPORT_C TInt MoveEntryWithinService(TMsvId aId, TMsvId aDestination);
	IMPORT_C TInt MoveEntriesWithinService(CMsvEntrySelection& aSelection, TMsvId aDestination);
	IMPORT_C TInt ChangeAttributes(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	IMPORT_C void MoveEntryL(TMsvId aId, TMsvId aDestination, TRequestStatus& aObserverStatus);
	IMPORT_C void MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus);
	IMPORT_C void CopyEntryL(TMsvId aId, TMsvId aDestination, TRequestStatus& aObserverStatus);
	IMPORT_C void CopyEntryL(TMsvId aId, TMsvId aDestination, TMsvId& aCompletedEntry, TRequestStatus& aObserverStatus);
	IMPORT_C void CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus);
	IMPORT_C void CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, CMsvEntrySelection& aCompletedSelection, TRequestStatus& aObserverStatus);
	IMPORT_C TInt GetEntryFromId(TMsvId aId,TMsvEntry*& aEntry);
	//
	// read and write the store associated with the context
	IMPORT_C CMsvStore* ReadStoreL();
	IMPORT_C CMsvStore* EditStoreL();
	//
	// List the children of the context 
	inline const TMsvSelectionOrdering& Sort();
	inline void SetSort(TMsvSelectionOrdering& aOrdering);
	inline void SetMtm(TUid aMtm);
	IMPORT_C TInt GetChildren(CMsvEntrySelection& aSelection);
	//
	IMPORT_C TInt GetChildrenWithService(TMsvId aServiceId, CMsvEntrySelection& aSelection);
	IMPORT_C TInt GetChildrenWithMtm(TUid aMtm, CMsvEntrySelection& aSelection);
	IMPORT_C TInt GetChildrenWithType(TUid aType, CMsvEntrySelection& aSelection);
	//
	// Spawns a new CMsvServerEntry object
	IMPORT_C CMsvServerEntry* NewEntryL(TMsvId aId);
	// 
	// from MMsvStoreObserver
	void HandleStoreEvent(MMsvStoreObserver::TMsvStoreEvent aEvent, TMsvId aId);
	//
	//

	IMPORT_C TBool HasStoreL() const;
	//
	IMPORT_C RFs& FileSession();
	//
	// Methods for creating and changing entries in bulk
	// (e.g. during email header downloading)
	IMPORT_C TInt CreateEntryBulk(TMsvEntry& aEntry, TSecureId aOwnerId);

	IMPORT_C TInt CreateEntryBulk(TMsvEntry& aEntry);

	IMPORT_C TInt ChangeEntryBulk(const TMsvEntry& aEntry, TSecureId aOwnerId);
	IMPORT_C TInt ChangeEntryBulk(const TMsvEntry& aEntry);
	
	IMPORT_C void CompleteBulk();

protected:
	CMsvServerEntry(CMsvServer& aIndex);
	void ConstructL(TMsvId aId);
	//
	void RunL();
	void DoCancel();
	//
private:
	TInt IncreaseBufferSizes(TInt aNewDescriptionSize, TInt aNewDetailsSize);
	TBool AreChildren(const CMsvEntrySelection& aSelection) const;
	TBool IsAChild(TMsvId aId) const;
	void DoMoveEntryL(TMsvId aId, TMsvId aDestination);
	void DoDeleteEntryL(TMsvId aId, CMsvEntrySelection*& aDeleted, CMsvEntrySelection*& aMoved);
	void DoDeleteEntriesL(CMsvEntrySelection& aSelection, CMsvEntrySelection*& aDeleted, CMsvEntrySelection*& aMoved);
	void DoMoveEntriesL(CMsvEntrySelection& aSelection, TMsvId aDestination, CMsvEntrySelection*& aMoved);
	TInt DoGetChildren(TMsvId aServiceId, TUid aMtm, TUid aType, CMsvEntrySelection& aSelection);
	void DoGetChildrenL(TMsvId aServiceId, TUid aMtm, TUid aType, CMsvEntrySelection& aSelection);
	void DoCopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus);
	TInt DoChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate, TBool aBulk);

	//
private:
	TBool iLockedStore;
	TMsvSelectionOrdering iOrdering;
	TMsvEntry iEntry;
	HBufC* iDescription;
	HBufC* iDetails;
	CMsvServer& iServer;
	CMsvStore* iStore;
	TUid iMtm;
	CMsvCopyMoveEntriesBase* iCopyMove;
	TMsvId iSource;
	enum { EMsvIdle, EMsvMoving, EMsvCopying } iEntryState;
	TRequestStatus* iObserverStatus;
	CMsvEntrySelection* iCompletedSelection;
	TMsvId* iCompletedEntryId;
	TSecureId	iContextOwnerId;
	};


#include <msventry.inl>

#endif // __MSVENTRY_H__
