// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMSVDISKCHANGENOTIFIER_H__
#define __CMSVDISKCHANGENOTIFIER_H__

#include <e32base.h>
#include <f32file.h>


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
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	virtual void DiskRemoved(const TDriveUnit& aDrive, TBool aTestRemoval = EFalse)=0;
	virtual void DiskInserted(const TDriveUnit& aDrive, TBool aTestInsert = EFalse)=0;
#else
	virtual void DiskRemoved(const TDriveUnit& aDrive)=0;
	virtual void DiskInserted(const TDriveUnit& aDrive)=0;
#endif
	virtual void DiskChanged(const TDriveUnit& aDrive)=0;
	};


NONSHARABLE_CLASS (CMsvDiskChangeNotifier) : public CActive
	{
public:
	static CMsvDiskChangeNotifier* NewL(const TDriveUnit& aDrive,MMsvDiskChangeObserver& aObserver);
	void Start();
	~CMsvDiskChangeNotifier();
private:
	CMsvDiskChangeNotifier(const TDriveUnit& aDrive, MMsvDiskChangeObserver& aObserver);
	void ConstructL();
	void RunL();
	void DoCancel();	
private:
	RFs iFs;
	TDriveUnit iDrive;
	HBufC16* iDrivePath;
	MMsvDiskChangeObserver& iObserver;
	};
	
#endif /*__CMSVDISKCHANGENOTIFIER_H__*/
