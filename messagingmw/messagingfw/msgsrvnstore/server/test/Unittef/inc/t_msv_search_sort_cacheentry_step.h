// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// t_msv_serach_sort_cacheentry_step.h
// 
//

#ifndef T_MSV_SEARCH_SORT_CACHEENTRY_STEP_H
#define T_MSV_SEARCH_SORT_CACHEENTRY_STEP_H

#include <test/tefunit.h>

/**************************
* CTestSearchSortCacheEntry
* @internalTechnology
* @prototype
****************************/
class CMSvSearchSortCacheManager;
class CMsvServer;

class CTestSearchSortCacheEntry : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestSearchSortCacheEntry(); 
	virtual ~CTestSearchSortCacheEntry();
	void SetupL();
	void TearDownL();
	void TestCreateEntryL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
private:
	CMSvSearchSortCacheManager* iSearchSortCacheManager;
	CMsvServer* iServer;
	};
#endif // T_MSV_SEARCH_SORT_CACHEENTRY_STEP_H
