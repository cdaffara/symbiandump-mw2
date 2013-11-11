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

#if !defined(__MSVLOPS_H__)
#define __MSVLOPS_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVOP_H__)
#include "msvop.h"
#endif


class CMsvCopy;
class CMsvDelete;
class CMsvMove;

//**********************************
// CMsvLocalOperation
//**********************************
//
// Base class for all local operations
//

class CMsvLocalOperation : public CMsvServerOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalOperation(const RMessage2& aMessage, TMsvOp aId, CMsvServer& aMsvServer);
	//
	const TDesC8& Progress();
	//
	TInt SystemProgress(TMsvSystemProgress& aSysProgress);
protected:
	CMsvServer& iMsvServer;
	TPckgBuf<TMsvLocalOperationProgress> iProgress;
	};



//**********************************
// CMsvLocalCopyOperation
//**********************************
//
// Operation used to control server side local copy
//

class CMsvLocalCopyOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalCopyOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, TMsvId aTarget, CMsvServer& aMsvServer);
	~CMsvLocalCopyOperation();
	//
	void StartL();
	//
private:
	TInt CopyNextEntry();
	void Completed();
	// from CActive
	void DoCancel();
	void RunL();
	//
private:
	CMsvCopy* iCopy;
	CMsvEntrySelection* iOrigEntries;
	CMsvEntrySelection* iNewEntries;
	TInt iIndex;
	TMsvId iTarget;
	TBool iDescendentCopying;
	TMsvId iDescendentId;
	};


//**********************************
// CMsvLocalMoveOperation
//**********************************
//
// Operation used to control server side local move
//

class CMsvLocalMoveOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalMoveOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, TMsvId aTarget, CMsvServer& aMsvServer);
	~CMsvLocalMoveOperation();
	//
	void StartL();
	//
private:
	void DoCancel();
	void RunL();
	void Completed();
	TInt MoveNextEntry();
	void StartFailed(TInt aError);
	//
private:
	TMsvId iTarget;
	TMsvId iParent;
	CMsvEntrySelection* iOrigEntries;
	CMsvMove* iMove;
	TMsvId iNextEntry;
	};


//**********************************
// CMsvLocalDeleteOperation
//**********************************
//
// Operation used to control server side local delete
//

class CMsvLocalDeleteOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalDeleteOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, CMsvServer& aMsvServer);
	~CMsvLocalDeleteOperation();
	//
	void StartL();
	//
private:
	void DoCancel();
	void RunL();
	void Completed();
	void DoRunL();
	void PartialCompletionNotificationL();
	//
private:
	CMsvEntrySelection* iSelection;
	CMsvEntrySelection* iMovedEntries;
	CMsvEntrySelection* iDeletedEntries;
	CMsvEntrySelection* iWorkSelection1;
	CMsvEntrySelection* iWorkSelection2;
	CMsvDelete* iDelete;
	TMsvId iParent;
	TInt iIndex;
	};


//**********************************
// CMsvLocalChangeOperation
//**********************************
//
// Operation used to control server side local change
//

class CMsvLocalChangeOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalChangeOperation(const RMessage2& aMessage, TMsvOp aId, const TMsvEntry& aEntry, CMsvServer& aMsvServer);
	~CMsvLocalChangeOperation();
	//
	void StartL(TSecureId aOwnerId, TBool aForcedUpdate);
	//
private:
	void DoCancel();
	void RunL();
	void Completed();
	//
private:
	const TMsvEntry& iEntry;
	CMsvEntrySelection* iWorkSelection1;
	CMsvEntrySelection* iWorkSelection2;
	CMsvDelete* iDelete;
	TMsvId iId;
	TMsvId iParent;
	};
///
//**********************************
// CMsvLocalChangeOperation
//**********************************
//
// Operation used to control server side local change od selection of ids.
//

class CMsvLocalChangeEntriesOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
    {
public:
    CMsvLocalChangeEntriesOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, CMsvServer& aMsvServer, TInt aMark);
    ~CMsvLocalChangeEntriesOperation();
    //
    void StartL(TSecureId aOwnerId, TBool aForcedUpdate);
    //
private:
    void DoCancel();
    void RunL();
    void Completed();
    //
private:
    CMsvEntrySelection* iSelection;
    CMsvEntrySelection* iWorkSelection1;
    CMsvEntrySelection* iWorkSelection2;
    CMsvDelete* iDelete;
    TMsvId iId;
    TMsvId iParent;
    TInt iMark;
    };


///
//**********************************
// CMsvLocalCreateOperation
//**********************************
//
// Operation used to control server side local create
//

class CMsvLocalCreateOperation : public CMsvLocalOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvLocalCreateOperation(const RMessage2& aMessage, TMsvOp aId, const TMsvEntry& aEntry, CMsvServer& aMsvServer);
	~CMsvLocalCreateOperation();
	//
	void Start(TSecureId aOwnerId);
	//
protected:
	void DoCancel();
	void RunL();
	//
private:
	const TMsvEntry& iEntry;
	};

//**********************************
// CMsvChangeDriveOperation
//**********************************
class CMsvChangeDriveOperation : public CMsvServerOperation
/**
@internalComponent
@released
*/
	{
public:
	CMsvChangeDriveOperation(const RMessage2& aMessage, TMsvOp aId, TInt aDrive, CMsvServer& aServer);
	~CMsvChangeDriveOperation();
	//
	const TDesC8& Progress();
	TInt Start();
	//
private:
	void DoCancel();
	void RunL();
	void Completed();
	//
private:
	TPckgBuf<TMsvIndexLoadProgress> iProgress;
	TInt iDrive;
	CMsvServer& iServer;
	};
#endif


