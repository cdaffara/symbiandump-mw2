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


#ifndef T_CMSVENTRY_STEP_H
#define T_CMSVENTRY_STEP_H

#include <test\tefunit.h>
#include <msvapi.h>
#include <centralrepository.h>
#include "cactivewaiter.h"

#include "t_msv_std_header_step.h"


/**
@internalTechnology
@prototype
*/
class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};
	
	

/**
@internalTechnology
@prototype
*/
class CTestCMsvEntry : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestCMsvEntry();
	virtual ~CTestCMsvEntry();
	void SetupL();
	void TearDownL();

	void CTestGlobalEntryL();
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	void SetupINIFile(TInt aPref);
	void OpenASession();
	void CreateEntry(CMsvEntry *aEntry);
	static void ResetRepositoryL();

private:
	CDummyObserver* iMsvSessionObserver;
	CMsvSession* iSession;
	CActiveWaiter* iActiveWaiter;
	};
#endif // T_CMSVENTRY_STEP_H

