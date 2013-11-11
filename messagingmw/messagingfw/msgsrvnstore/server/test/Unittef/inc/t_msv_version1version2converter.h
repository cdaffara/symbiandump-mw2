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


#ifndef T_MSG_TESTVERSION1VERSION2CONVERTER_STEP_H
#define T_MSG_TESTVERSION1VERSION2CONVERTER_STEP_H

//#include <tefunit.h>
#include "msvdbadapter.h"

#include "cactivewaiter.h"
#include "cmsvversion1version2converter.h"


enum TTestFieldList 
	{ 
	EReplyTo, 	
	};

enum TTestEncodingType 
	{ 
	EBase64, 		
	EQP, 			
	ENoEncoding, 	
	EUU				
	};


/**
@internalTechnology
@prototype
*/
class CTestVersion1Version2Converter : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:


	CTestVersion1Version2Converter();
	virtual ~CTestVersion1Version2Converter();
	void SetupL();
	void TearDownL();
	
	void TestStartEMailMessageConversionL();
	void TestAddCommonheaderFieldFieldL();	
	void TestAddFieldL();	
	void TestReadEMailMessageHeaderL();	
	void TestReadEMailMessageEncodedHeaderL();	
	void TestReadEmailMimeHeaderL();	
	void TestDeleteMessageFromFileStore();
	void TestResumeIMAPMessageConversionL();
	void TestResumePOPMessageConversionL();
	void TestResumeSMTPMessageConversionL();
	void TestTImHeaderEncodingInfoL();
	void TestCoverageL();
	void SetUpTest();	
	static CTestSuite* CreateSuiteL(const TDesC& aName);

private:
	void CreateEntryL(TDriveNumber aDriveNum, TMsvId aId);

private:
	TDriveNumber iDrive;
	
	CMsvDBAdapter* iDbAdapter;
	CMsvMessageDBAdapter* iMessageDbAdapter;
	
	CMsvVersion1Version2Converter* iVersion1Version2Converter;
	TBool iConversionResume;
	};
	 
#endif // T_MSG_TESTVERSION1VERSION2CONVERTER_STEP_H
