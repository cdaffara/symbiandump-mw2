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

#if !defined(__MSVCOPY_H__)
#define __MSVCOPY_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#include "MSVCMBSE.H"

class CCopyOneFile;
class CCopyFiles;
class CMsvServer;


//**********************************
// CMsvCopyEntry
//**********************************
//
//
//

class CMsvCopyEntry : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvCopyEntry* NewL(CMsvServer& aServer);
	~CMsvCopyEntry();
	//
	void StartL(TMsvId aOrigEntryId, TMsvId aTargetId, TRequestStatus& aObserverStatus);
	inline TMsvId NewEntryId() const;
	//
private:
	CMsvCopyEntry(CMsvServer& aServer);
	void ConstructL();
	//
	void Completed(TInt aError);
	void NextState();
	void CopyStoreFile();
	void CopyFolder();

	// from CActive
	void RunL();
	void DoCancel();


private:

	enum TState {ENotYetStarted=0, EEntryCreated, ECopyingStore, ECopyingFolder, ECompleted };

private:
	CMsvServer& iServer;
	TRequestStatus* iObserverStatus;
	CCopyOneFile* iCopyOneFile;
	CCopyFiles* iCopyFiles;
	TState iState;
	TMsvId iOrigEntryId;
	TMsvEntry* iNewEntry;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TMsvEntry* iOrigEntry;
#endif

	TFileName *iFromName;
	TFileName *iToName;
	};


//**********************************
// CMsvCopy
//**********************************
//
//
//

class CMsvCopy : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvCopy* NewL(CMsvServer& aServer);
	~CMsvCopy();
	//
	void StartL(TMsvId aOrigEntryId, TMsvId aTargetId, TRequestStatus& aObserverStatus, TBool aDescendentCopying);
	//
	inline TMsvId NewEntryId() const;
	inline void DeleteAllOnFailure();
	//
private:
	CMsvCopy(CMsvServer& aServer);
	void ConstructL();
	void Completed();
	void DoRunL(TBool aTopLevel);
	// from CActive
	void RunL();
	void DoCancel();
	//
private:
	CMsvServer& iServer;
	TRequestStatus* iObserverStatus;
	TMsvId iOrigTargetId;
	TMsvId iOrigSourceId;
	TMsvId iTopLevelId;
	TMsvId iTempTargetId;
	TMsvId iTempSourceId;
	TBool iDescendentCopying;
	TMsvId iDescendentId;
	CMsvEntrySelection* iFailedEntryParents;
	CMsvEntrySelection* iRecursionList;
	CMsvCopyEntry* iCopyEntry;
	TInt iError;
	TBool iDeleteAllOnFailure;
	};


//**********************************
// CMsvCopyEntries
//**********************************
//
//
//

class CMsvCopyEntries : public CMsvCopyMoveEntriesBase
/**
@internalComponent
@released
*/
	{
public:
	static CMsvCopyEntries* NewL(CMsvServer& aServer);
	~CMsvCopyEntries();
	//
	inline const CMsvEntrySelection& NewEntryIds() const;
	//
private:
	void DoStartL(TMsvId aSourceId, TMsvId aTargetId, TRequestStatus& aObserverStatus);
	CMsvCopyEntries();
	void ConstructL(CMsvServer& aServer);
	void DoCancel();
	void RunL();
	//
private:
	CMsvCopy* iCopy;
	CMsvEntrySelection* iNewEntries;
	};


// inline functions

inline TMsvId CMsvCopyEntry::NewEntryId() const
	{
	return iNewEntry->Id();
	}

inline TMsvId CMsvCopy::NewEntryId() const
	{
	return iTopLevelId;
	}

inline void CMsvCopy::DeleteAllOnFailure()
	{
	iDeleteAllOnFailure = ETrue;
	}

inline const CMsvEntrySelection& CMsvCopyEntries::NewEntryIds() const
	{
	return *iNewEntries;
	}

#endif
