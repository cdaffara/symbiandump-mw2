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

#if !defined(__MSVMOVE_H__)
#define __MSVMOVE_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#include "MSVCMBSE.H"

class CMsvServer;

//**********************************
// CMsvMove
//**********************************
//
//
//

class CMsvMove : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvMove* NewL(CMsvServer& aServer);
	~CMsvMove();
	//
	void StartL(TMsvId aId, TMsvId aTarget, TRequestStatus& aObserverStatus);
	void StartL(TMsvId aId, TMsvId aTarget);
	void Reset();
	//
protected:
	CMsvMove(CMsvServer& aServer);
	void ConstructL();
	// from CActive
	void RunL();
	void DoCancel();
	//
private:
	void CheckEntriesL();
	void CleanupFiles(TMsvId aService);
	void FindNextStep();
	void StartNextStep();
	TInt SetupFileCopy();
	TInt SetupStoreCopy();
	void DoRunL();
	//
private:
	enum TState {EFolder, EStore, EIndex};
	//
private:
	CMsvServer& iServer;
	TRequestStatus* iObserverStatus;
	CFileMan* iFileMan;
	TState iState;
	TMsvEntry* iCurrentEntry;
	TMsvId iTargetId;
	TMsvId iSourceId;
	TMsvId iParentId;
	TMsvId iTargetService;
	TMsvId iSourceService;
	HBufC* iTargetPath;
	CMsvEntrySelection* iDescendents;
	TInt iNextDescendent;
	TInt iLockedIndex;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TBool iIsDBStore;
#endif
	};


//**********************************
// CMsvMoveEntries
//**********************************
//
//
//

class CMsvMoveEntries : public CMsvCopyMoveEntriesBase
/**
@internalComponent
@released
*/
	{
public:
	static CMsvMoveEntries* NewL(CMsvServer& aServer);
	~CMsvMoveEntries();
	//
private:
	void DoStartL(TMsvId aSourceId, TMsvId aTargetId, TRequestStatus& aObserverStatus);
	CMsvMoveEntries();
	void ConstructL(CMsvServer& aServer);
	void DoCancel();
	//
private:
	CMsvMove* iMove;
	};


#endif
