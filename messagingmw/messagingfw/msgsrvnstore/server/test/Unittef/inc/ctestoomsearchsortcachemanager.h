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


#ifndef CTESTOOMSEARCHSORTCACHEMANAGER_H
#define CTESTOOMSEARCHSORTCACHEMANAGER_H

#include <test\tefunit.h>
#include "msvsearchsortcachemanager.h"
#include "msvsearchsortdeltacache.h"
#include "msvindexadapter.h"

class CMsvDBAdapter;

/**
@internalTechnology
@prototype
*/
class CTestOOMSearchSortCacheManager : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestOOMSearchSortCacheManager(); 
	virtual ~CTestOOMSearchSortCacheManager();
	void SetupL();
	void TearDownL();
	void TestForOutOfMemoryL();
	void TestCreateSearchSortCacheEntryOOML();
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);
private:
	CMsvServer* iServer;
	};
#endif // CTESTOOMSEARCHSORTCACHEMANAGER_H
