//
// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef T_MSV_STORE_STEP_H
#define T_MSV_STORE_STEP_H

#include <test\tefunit.h>
#include "msvindexadapter.h"
#include "msvdbadapter.h"
#include "MSVSERV.H"
#include "centralrepository.h"
#include "cactivewaiter.h"
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>
#include <bautils.h>
#include "msvutils.h"
#include <msvstore.h>


/**
@internalTechnology
@prototype
*/
class CTestStore: public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestStore();
	virtual ~CTestStore();
	void SetupL();
	void TearDownL();
	
	void TestOpenForReadL();
	void TestOpenForWriteL();
	void TestStoreHeaderL();
	void TestReStoreheaderL();
	void TestCommitL();
	void TestStoreBodyTextL();
	void TestReStoreBodyTextL();
	void TestHasBodyTextL();
	void TestIsPresentL();
	void TestDeleteBodyTextL();
	void TestDeleteL();
	void TestRemoveL();
	void TestSizeL();
	void TestRevertL();
		
	static CTestSuite* CreateSuiteL(const TDesC& aName);

	};
	 
#endif // T_MSV_STORE_STEP_H
