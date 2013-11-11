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

#if !defined(__MSVDELET_H__)
#define __MSVDELET_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

class CMsvServer;

//**********************************
// CMsvDelete
//**********************************
//
//
//

class CMsvDelete : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvDelete* NewL(CMsvServer& aServer);
	~CMsvDelete();
	//
	void StartL(TMsvId aEntryId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TRequestStatus& aObserverStatus, TBool aPCSyncOverride=EFalse);
	void StartL(TMsvId aEntryId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TBool aPCSyncOverride=EFalse);
	//
	
private:
	CMsvDelete(CMsvServer& aServer);
	void ConstructL();
	void Complete(TInt aError);
	void DoRunL();
	void CheckEntriesL();
	void DeleteFilesL();
	void DeleteAllIndexEntries();
	void DeleteIndividualIndexEntries();	
	void FindParent();
	void DoStartL(TMsvId aId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TBool aPCSyncOverride);
	// from CActive
	void RunL();
	void DoCancel();
	//
private:
	enum TState {ECheck, EFiles, EIndex, EIndexIndividually, ECompleted};
private:
	class TMsvDelete
		{
		public:
			TMsvDelete(TMsvId aEntryId, TBool aOwner, TInt aPcSyncCount, TUid aType, TBool aComplete)
			:iEntryId(aEntryId), iOwner(aOwner), iPcSyncCount(aPcSyncCount), iType(aType), iIsComplete(aComplete) {}
			TMsvId iEntryId;
			TBool iOwner;
			TInt iPcSyncCount;
			TUid iType;
			TBool iIsComplete;
		};
	CMsvServer& iServer;
	TRequestStatus* iObserverStatus;
	TMsvId iId;
	CMsvEntrySelection* iDeletedEntries;
	CMsvEntrySelection* iMovedEntries;
	CMsvEntrySelection* iDescendents;
	TState iState;
	TInt iLockedIndex;
	TInt iDeletionIndex;
	TBool iPCSynced;
	TBool iPCSyncOverride;
	TInt iError;
	CFileMan *iFileMan;
	RArray<TMsvDelete> iDeleteArray;
	};



#endif
