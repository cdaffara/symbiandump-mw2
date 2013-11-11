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

#ifndef T_MSV_CACHE_INDEXTABLEENTRY_STEP_H
#define T_MSV_CACHE_INDEXTABLEENTRY_STEP_H

#include <test\tefunit.h>
#include "msvcacheindextableentry.h"
#include "msvcacheentry.h"
#include "msventryfreepool.h"
#include "msvindexadapter.h"
#include "msvcachevisiblefolder.h"

/**
@internalTechnology
@prototype
*/
class CTestIndexTableEntry : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestIndexTableEntry();
	virtual ~CTestIndexTableEntry();
	void SetupL();
	void TearDownL();

	void TestConstructorL();
	void TestAddEntry();
	void TestAddEntrySet();
	void TestGetEntry();
	void TestGetChildren();
	void TestDeleteEntry();
	void TestSortBlock();
	void TestReleaseBlock();
	void TestIsInRange();
	void TestEntryExists();
	void TestIsDirty();
	void TestSetDirty();
	void TestClearDirty();
	void TestSetMaxMsvIdRange();
	void TestSetMinMsvIdRange();
	void TestGetMaxMsvIdRange();
	void TestGetMinMsvIdRange();
	void TestUpdateChildMsvIds();
	void TestSize();
    void TestSetGrandChildPresent();
    void TestClearGrandChildPresent();
    void TestIsGrandChildPresent();
    void TestBlockPtr();
    void TestAccessTime();
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:

	CMsvCacheEntry *entryA, *entryB, *entryC,*entryD, *parent;
	RFs iFileHandle;
	CMsvCacheIndexTableEntry* iIndexTableEntry;
	RPointerArray<CMsvCacheEntry>* iEntryPointer;
	CMsvEntryFreePool *iMsvEntryFreePool;
	};
	 
#endif // T_MSV_INDEXTABLEENTRY_STEP_H
