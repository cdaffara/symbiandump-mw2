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


#ifndef T_MSV_CMSVVERSION0VERSION1CONVERTER_STEP_H
#define T_MSV_CMSVVERSION0VERSION1CONVERTER_STEP_H

#include <test\tefunit.h>
#include "msvdbadapter.h"
#include "cmsvversion0version1converter.h"
#include "cactivewaiter.h"



/**
@internalTechnology

*/
class CTestVersion0Version1Converter : public  CActiveTestFixture
	{
public:
	CTestVersion0Version1Converter();
	virtual ~CTestVersion0Version1Converter();
	void SetupL();
	void TearDownL();
	void TestConvertMessageStoreL();
	void SetUpTest();
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	
private:
	CMsvVersion0Version1Converter* iVersion0Version1Converter;
	CMsvDBAdapter* iDbAdapter;
	};
	 
#endif // T_MSV_CMSVVERSION0VERSION1CONVERTER_STEP_H
