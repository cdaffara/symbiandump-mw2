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

#ifndef T_MSV_ENTRYFREEPOOL_STEP_H
#define T_MSV_ENTRYFREEPOOL_STEP_H

#include "msventryfreepool.h"
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#include "msvpreferreddrivelist.h"
#endif
#include <test/tefunit.h>

/**
@internalTechnology
@prototype
*/
class CTestEntryFreePool : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestEntryFreePool();
	virtual ~CTestEntryFreePool();
	void SetupL();
	void TearDownL();

	void TestCreate();
	void TestInstance();
	void TestEntry();
	void TestAllocateMemory();
	void TestIsAllocationRequired();
	
	void TestMissingIniFile();

	static CTestSuite* CreateSuiteL(const TDesC& aName);
private:
	RFs iFileHandle;
	TInt iMsvMaximumCacheSize;
	CMsvEntryFreePool* iFreePool;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsvPreferredDriveList* iDriveList;
#endif	
	};
#endif // T_MSV_ENTRYFREEPOOL_STEP_H
