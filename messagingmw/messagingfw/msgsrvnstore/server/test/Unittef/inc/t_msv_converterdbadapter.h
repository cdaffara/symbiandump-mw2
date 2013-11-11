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


#ifndef T_MSV_CONVERTERDBADAPTER_H
#define T_MSV_CONVERTERDBADAPTER_H



#include "msvdbadapter.h"

#include "cactivewaiter.h"

/**
@internalTechnology
@prototype
*/
class CTestConverterDBAdapter : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:


	CTestConverterDBAdapter();
	virtual ~CTestConverterDBAdapter();
	void SetupL();
	void TearDownL();
	void SetUpTest();
	
	void TestCreateIndexEntryL();
	void TestChangeVersionL();	
	void TestGetAllMessageIdsL();	
	void TestGetAllServiceIDsForMtmL();	
	void TestGetAllIndexEntryTableIdsL();	
	void TestGetMessageCountL();	
	void TestCreateConversionStatusTableL();	
	void TestRemoveConversionStatusTableL();	
	void TestGetConversionStatusL();	
	void TestAddConversionStatusEntryL();	
	void TestUpdateConversionStatusEntryL();	
	void TestRemoveConversionStatusEntriesL();	
		
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	void CreateEntryL(TDriveNumber aDriveNum, TMsvId aId);

private:
		
	CMsvDBAdapter* iDbAdapter;
	
	};
	 
#endif // T_MSV_CONVERTERDBADAPTER_H
