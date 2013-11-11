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


#ifndef T_MSV_INIDATA_H
#define T_MSV_INIDATA_H

#include <test/tefunit.h>
#include "msvinifile.h"


/**
@internalTechnology
@prototype
*/
class CTestIniData : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestIniData();
	virtual ~CTestIniData();
	void SetupL();
	void TearDownL();

	void TestFindVarL();
	void TestFindVarWithSectionsL();
	void TestCreation();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CMsvIniData* iIniFile;
	RFs iFileHandle;
	};
	 
#endif // T_MSV_INIDATA_H

