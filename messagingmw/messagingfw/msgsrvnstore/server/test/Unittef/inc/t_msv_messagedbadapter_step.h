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

#ifndef T_MSV_MESSAGEDBADAPTER_STEP_H
#define T_MSV_MESSAGEDBADAPTER_STEP_H


#include <sqldb.h>
#include <msvstd.h>
#include <test\tefunit.h>

#include "msvdbadapter.h"
#include "msvindexadapter.h"
#include "msvmessagedbadapter.h"
#include "msvpreferreddrivelist.h"		// Preferred drive list.

/**
@internalTechnology
@prototype
*/

class CTestMessageDbAdapter : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestMessageDbAdapter();
	virtual ~CTestMessageDbAdapter();
	void SetupL();
	void TearDownL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	void TestHeaderEntry();
	void TestHeaderTableCreation();
	void TestUpdateHeaderEntry();
	void TestIsHeaderTableExists();
	void TestCopyHeaderEntry();
private:
	CMsvDBAdapter* iDatabase;
	RPointerArray<CFieldPair> iFieldStruct;
	CMsvMessageDBAdapter* iMsgDBAdapter;
	};
	
	
	
#endif // T_MSV_MESSAGEDBADAPTER_STEP_H
