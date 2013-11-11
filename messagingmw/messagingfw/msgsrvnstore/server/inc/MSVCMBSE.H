// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__MSVCMBSE_H__)
#define __MSVCMBSE_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include "MSVSTD.H"
#endif

//**********************************
// CMsvCopyMoveEntriesBase
//**********************************
//
//
//

class CMsvCopyMoveEntriesBase : public CActive
/**
@internalComponent
@released
*/
	{
public:
	~CMsvCopyMoveEntriesBase();
	//
	void StartL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aObserverStatus);
	//
	inline const CMsvEntrySelection& FailedIds() const;
	inline const CMsvEntrySelection& CompletedIds() const;
	inline TMsvId TargetId() const;
	//
protected:
	CMsvCopyMoveEntriesBase();
	void ConstructL();
	//
	virtual void DoStartL(TMsvId aSourceId, TMsvId aTargetId, TRequestStatus& aObserverStatus)=0;
	void RunL();
	//
private:
	TBool StartNext();
	void DoStartNextL();
	//
protected:
	TRequestStatus* iObserverStatus;
private:
	CMsvEntrySelection* iFailedIds;
	CMsvEntrySelection* iCompletedIds;
	TInt iCurrentIndex;
	TMsvId iTargetId;
	TInt iError;
	};

inline const CMsvEntrySelection& CMsvCopyMoveEntriesBase::FailedIds() const
	{
	return *iFailedIds;
	}

inline const CMsvEntrySelection& CMsvCopyMoveEntriesBase::CompletedIds() const
	{
	return *iCompletedIds;
	}

inline TMsvId CMsvCopyMoveEntriesBase::TargetId() const
	{
	return iTargetId;
	}

#endif
