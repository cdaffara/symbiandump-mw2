// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef T_MSV_DISKCHANGENOTIFIER_H
#define T_MSV_DISKCHANGENOTIFIER_H

#include <test/tefunit.h>
#include "cmsvdiskchangenotifier.h"

enum TNotification
	{
	EMsvTestDiskRemoveNotification,
	EMsvTestDiskInsertNotification,
	EMsvTestDiskChangeNotification
	};

class CTestDiskChangeObserver: public MMsvDiskChangeObserver
	{
public:
	virtual void DiskRemoved(const TDriveUnit& aDrive, TBool aTestRemoval = EFalse);
	virtual void DiskInserted(const TDriveUnit& aDrive, TBool aTestInsert = EFalse);
	virtual void DiskChanged(const TDriveUnit& aDrive);
	TNotification iType;
	TDriveUnit iDrive;
	};
	

/**
@internalTechnology
@prototype
*/
class CTestDiskChangeNotifier : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestDiskChangeNotifier();
	~CTestDiskChangeNotifier();
	void SetupL();
	void TearDownL();

	void TestNotification();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CTestDiskChangeObserver *iDiskChangeObserver;
	CMsvDiskChangeNotifier *iDiskChangeNotifierD;
	CMsvDiskChangeNotifier *iDiskChangeNotifierE;
	CMsvDiskChangeNotifier *iDiskChangeNotifierF;
	
	RBuf iFileSystemD;
	RBuf iFileSystemE;
	RBuf iFileSystemF;
	RFs iFs;
	};
	 
#endif // T_MSV_DISKCHANGENOTIFIER_H

