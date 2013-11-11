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



#include <test\tefunit.h>
#include "cmessageconvertermanager.h"
#include "cactivewaiter.h"
#include "msvdbadapter.h"



/**
@internalTechnology
@prototype
*/
class CTestMessageConverterManager : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	
	CTestMessageConverterManager();
	virtual ~CTestMessageConverterManager();
	void SetupL();
	void TearDownL();
	
	void TestStartConversion();
	void TestResumeConversionL();
	void TestResumeConversionFunctionL();
	void InitialiseMessageStoreConversionL();
	void TestRemoveIndexFileL();
	void TestRollBackVersion0Version1ConversionL();
	void TestRollBackVersion1Version2ConversionL();
	void TestVersion0Version2ConversionResumeL();
	void TestVersion1Version2ConversionResumeL();
	void TestRemoveDatabaseFileL();
	
		
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	RThread iConverterThread;	
	CActiveWaiter* iActiveWaiter;
	};
	 

