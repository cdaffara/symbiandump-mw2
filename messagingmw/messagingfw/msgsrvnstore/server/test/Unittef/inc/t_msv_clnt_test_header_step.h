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


#ifndef T_MSV_CLNT_TEST_HEADER_STEP_H
#define T_MSV_CLNT_TEST_HEADER_STEP_H

#include <test\tefunit.h>
#include <msvapi.h>
#include <msvuids.h>
#include <msvids.h>
#include <centralrepository.h>

#include "t_msv_std_header_step.h"

	

/**
@internalTechnology
@prototype
*/

class CTestHeaderEntry:public CActiveTestFixture
{
public:
	void SetupL();
	void TearDownL();	
	CTestHeaderEntry();
	~CTestHeaderEntry();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	void CTestHeaderTableCreation();
	void CTestHeaderEntryCreation();
	void TestUpdateHeaderEntry();
	void TestIsHeaderTableExists();

private:
	TMsvId CreateEntryL(TMsvId aParentId = KMsvGlobalInBoxIndexEntryId, TUid aType = KUidMsvMessageEntry);

private:
	CTestSessionObserver* iMsvSesionObserver;
	CTestSessionObserver* iMsvExtraSesionObserver;
	CTestEntryObserver* iMsvEntryObserver;
	CTestEntryObserver* iMsvExtraEntryObserver;
	CMsvEntry* iEntry;
	CMsvEntry* iExtraEntry;
	CMsvSession* iSession;
	CMsvSession* iExtraSession;
};

#endif		// T_MSV_CLNT_TEST_HEADER_STEP_H
