// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef T_MSV_SESSION_STEP_H
#define T_MSV_SESSION_STEP_H

#include <test\tefunit.h>
#include <msvapi.h>
#include <centralrepository.h>

#include "t_msv_std_header_step.h"

/**
@internalTechnology
@prototype
*/

class CTestMsvSession:public CActiveTestFixture
{
public:
	void SetupL();
	void TearDownL();	
	CTestMsvSession();
	~CTestMsvSession();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
	void CTestDriveListWithNoDriveL();
	void CTestDriveListWithSingleDriveL();
	void CTestDriveListWithMultipleDriveL();
	void CTestGetChildrenAll();	
	void CTestAddDriveWithNoDriveL();
	void CTestUpdateDrivePriorityWithNoDriveL();
	void CTestGetEntry();
		
private:	
	void SetupINIFile(TInt aPref);
	void InitialiseSessionObserver();
	void CreateEntry(CMsvEntry *aEntry);
	static void ResetRepositoryL();
	
private:
	CTestSessionObserver* iMsvSesionObserver;
	CTestSessionObserver* iMsvExtraSesionObserver;
	CTestEntryObserver* iMsvEntryObserver;
	CTestEntryObserver* iMsvExtraEntryObserver;
	CMsvEntry* iEntry;
	CMsvEntry* iExtraEntry;
	CMsvSession* iSession;
	CMsvSession* iExtraSession;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	// Post PREQ2073, the server throws a notification for unsupported message stores
	// that it previously didn't. This TBool is to handle that but also not interfere with
	// Pre2073 behaviour.
	TBool iUnsupportedDrivesPresent;
#endif
};

#endif
