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
// t_msv_performance.cpp
// 
//


#ifndef T_MSV_PERFORMANCE_H
#define T_MSV_PERFORMANCE_H

#include <tefunit.h>
#include "msvserv.h"
#include <msvids.h>
#include "msvstd.h"
#include <e32std.h>


#include "indexcontext.h"
#include "emailtestutils.h"
//#include "msvtestutils.h"

/**
@internalTechnology
@prototype
*/
class CTestPerformance1 : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestPerformance1();
	virtual ~CTestPerformance1();
	void SetupL();
	void TearDownL();

	void TestGetSingleEntry();
	void TestAddMoveDeleteEntry();
	void TestMoveMultipleEntry();
	void TestCopyAndUpdateEntry();
	void TestDeleteMultipleEntry();
	void TestGetAllEntries();
	void TestFunc();
	
	void CreateFileNames();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CEmailTestUtils* iTestUtils;
	CMsvIndexAdapter* iIndexAdapter;
	CMsvServer* iServer;
	TBool iServerSide;
	};
	 
#endif // T_MSV_PERFORMANCE_H

