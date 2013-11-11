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

#if !defined(__DISKCHANGE_H__)
#define __DISKCHANGE_H__

#if !defined(__F32FILE_H__)
#include <f32file.h>
#endif

#if !defined (__MENTACT_H__)
#include <mentact.h>
#endif

//**********************************
// MMsvDiskChangeObserver
//**********************************

class MMsvDiskChangeObserver
/**
@internalComponent
@released
*/
	{
public:
	virtual TBool DiskRemoved(const TDriveUnit& aDrive)=0;
	virtual TBool DiskInserted(const TDriveUnit& aDrive)=0;
	virtual TBool DiskChanged(const TDriveUnit& aDrive, TUint aUniqueId)=0;
	};

//**********************************
// CMsvNotifyDiskChange
//**********************************

class CMsvNotifyDiskChange : public CMsgActive
/**
@internalComponent
@released
*/
	{
public:
	CMsvNotifyDiskChange(RFs& aFs, MMsvDiskChangeObserver& aObserver);
	~CMsvNotifyDiskChange();
	//
	TInt Start(const TDriveUnit& aDrive, TUint aUniqueId);
	//
	inline void SetDiskMissing(TBool aMissing);
	inline void SetWrongId(TBool aWrongId);
	//
private:
	void DoCancel();
	void DoRunL();
	//
private:
	RFs& iFs;
	TDriveUnit iDrive;
	TUint iUniqueId;
	MMsvDiskChangeObserver& iObserver;
	TBool iDiskRemoved;
	//
	TBool iDiskMissing;
	TBool iWrongId;
	};

inline void CMsvNotifyDiskChange::SetDiskMissing(TBool aMissing)
	{
	iDiskMissing = aMissing;
	}

inline void CMsvNotifyDiskChange::SetWrongId(TBool aWrongId)
	{
	iWrongId = aWrongId;
	}

#endif
