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


#ifndef T_MSV_INDEXCONTEXT_STEP_H
#define T_MSV_INDEXCONTEXT_STEP_H

#include <test/tefunit.h>
#include "msvindexadapter.h"
#include "msvdbadapter.h"
#include "MSVSERV.H"
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>
#include <bautils.h>


/**
@internalTechnology
@prototype
*/
class CTestIndexContext : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestIndexContext();
	virtual ~CTestIndexContext();
	void SetupL();
	void TearDownL();
	void CreateEntryL(TDriveNumber aDriveNum, TMsvId aId, TMsvId aParentId);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void TestCreateStoreDeleteFileL();
	void TestUpdateDriveStatusL();
	void TestChangeDriveL();
	void TestGetInPreparationIdsL();
	void TestLocalizeStandardFoldersL();
	void TestProgress();
#else
	void TestValidationL();
#endif

	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	CMsvServer* iServer;
	CMsvIndexContext* iContext;
	};
	 
#endif // T_MSV_INDEXCONTEXT_STEP_H
