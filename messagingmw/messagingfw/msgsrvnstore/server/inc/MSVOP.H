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

#if !defined(__MSVOP_H__)
#define __MSVOP_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

// forward declarations
class CBaseServerMtm;
class CMsvMtmOperation;
class TMsvSystemProgress;

//**********************************
// MMsvOperationObserver
//**********************************
//
// 
//

class MMsvOperationObserver
/**
@internalComponent
@released
*/
	{
public: 
	virtual void OperationCompleted(TMsvId aServiceId, TMsvOp aOpId)=0;
	virtual void OperationCancelled(const CMsvMtmOperation& aOperation)=0;
	};


//**********************************
// CMsvServerOperation
//**********************************
//
// Abstract base class for server side operations
//

/**
@internalComponent
@released
*/
enum TMsvOperationState {	EMsvOperationNull,
							EMsvOperationQueued,
							EMsvOperationRunning,
							EMsvOperationCompleted,
							EMsvOperationFailed};

class CMsvServerOperation : public CActive
/**
@internalComponent
@released
*/
	{
public:
	~CMsvServerOperation();
	//
	virtual const TDesC8& Progress()=0;
	virtual TInt SystemProgress(TMsvSystemProgress& aSysProgress);
	//
	inline TMsvOp Id() const;
	inline TUid MtmUid() const;
	inline TMsvId ServiceId() const;
	inline TMsvId SessionId() const;
	inline TMsvOperationState State() const;
	inline void SetState(TMsvOperationState aState);

protected:
	CMsvServerOperation(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId);
	//
protected:
	RMessage2 iMessage;
private:
	TMsvOp	iId;
	TUid	iMtmUid;
	TMsvId	iServiceId;
	TInt	iSessionId;
	TMsvOperationState iState;
	};

#include "MSVOP.INL"

#endif
