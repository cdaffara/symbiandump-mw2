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


#ifndef T_MSV_SERVER_STEP_H
#define T_MSV_SERVER_STEP_H

#include <test/tefunit.h>
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


/**
@internalTechnology
@prototype
*/
class CTestMsvServer : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestMsvServer();
	virtual ~CTestMsvServer();
	void SetupL();
	void TearDownL();
	
	void TestAddDriveToListL();
	void TestRemoveDriveFromListL();
	void TestUpdateDrivePriorityL();
	void TestUpdateRepositoryL();
	void TestCreatePreferredDriveListL();
	void TestChangeDriveL();
	void TestDriveOperationsL();
	void TestMsvUtils();
	void TestStoreManager();
	void TestIsMessageStoreDrivePresent();
	void TestDriveContainsStore();
	void TestIsMessageStoreSupported();
	void TestContextComplete();
	void TestChangeDriveWithFullDriveListL();

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	void TestHeaderTableMetadata();
#endif
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	void CreateEntryL(TDriveNumber aDriveNum, TMsvId aId);

private:
	CMsvServer* iServer;
	CMsvIndexContext* iContext;
	CActiveWaiter* iActiveWaiter;
	TMsvServerStoreManager iStoreManager;
	};
	 
#endif // T_MSV_SERVER_STEP_H
