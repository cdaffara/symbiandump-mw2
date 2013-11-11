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

#ifndef T_MSV_SMSMESSAGE_V1V2_CONVERTER_STEP_H
#define T_MSV_SMSMESSAGE_V1V2_CONVERTER_STEP_H

#include "cmsvversion1version2converter.h"
#include <sqldb.h>
#include <msvstd.h>
#include <test\tefunit.h>

#include "msvdbadapter.h"
#include "msvindexadapter.h"
#include "msvmessagedbadapter.h"
#include "msvpreferreddrivelist.h"		// Preferred drive list.



class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};


class CTestSmsMessageV1toV2Converter : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestSmsMessageV1toV2Converter();
	virtual ~CTestSmsMessageV1toV2Converter();
	void SetupL();
	void TearDownL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);

	
private:
		
	void TestFillEmptyEosBufferL();
	void TestConvertMessageStoreL();
	void TestCleanAllTestDataL();
	
private:
	CMsvDBAdapter* iDbAdapter;
	TDriveNumber iDrive;
	CMsvMessageDBAdapter* iMessageDbAdapter;
	TFileName dbFileName;
	CMsvVersion1Version2Converter* iMessageConverter;
	
	TMsvId iTmsvId;
	
		
	CDummyObserver* iOb;
	CMsvSession* iSession;
	CMsvEntry* iMsvEntry;
	
	
	
	};

	
	
#endif // T_MSV_SMSMESSAGE_V1V2_CONVERTER_STEP_H
