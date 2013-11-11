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

#ifndef T_MSV_CACHEENTRY_STEP_H
#define T_MSV_CACHEENTRY_STEP_H

#include <test/tefunit.h>

/**
@internalTechnology
@prototype
*/
class CTestCacheEntry : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestCacheEntry();
	virtual ~CTestCacheEntry();
	void SetupL();
	void TearDownL();

	void TestEntryL();
	void TestLockL();
	void TestEntryLockL();
	void TestStoreLockL();
	void TestStoreAndEntryLockL();
	void TestReaderCountL();
	void TestDateL();
	void TestCreatedDateL();
	void TestLastChangeDateL();
	void TestCopyEntryL();
	void TestOwnerIdL();
	void TestRollBackCopyEntryL();
	void TestChildArrayL();
	void TestSetEntryL();
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	};
#endif // T_MSV_CACHEENTRY_STEP_H
