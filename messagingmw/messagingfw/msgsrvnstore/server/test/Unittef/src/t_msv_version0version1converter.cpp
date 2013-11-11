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
// Contributors: Ashok Kumar
//
// Description:
//
 
#include "t_msv_version0version1converter.h"




#define DEFAULT_RETURN KErrNone
#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);


_LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
_LIT(KDbFileName, "D:\\messaging.db");
_LIT(KConverterMail2Folder, "c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2");


CTestVersion0Version1Converter::CTestVersion0Version1Converter()
	{
	}



CTestVersion0Version1Converter::~CTestVersion0Version1Converter()
	{	
	}



void CTestVersion0Version1Converter::SetupL()
	{
	_LIT(KFunction, "*_*SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	SetUpTest();

	}

void CTestVersion0Version1Converter::SetUpTest()
	{
	_LIT(KFunction, "SetUpTest");
	INFO_PRINTF1(KFunction);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// delete messaging db
	// create the directories
	
	// delete db
	
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);

	//copy mail2
	err = fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2);
		
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	
	CMsvDBAdapter::CreateDBL(EDriveD);
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);		
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);
	iDbAdapter->CreateConversionStatusTableL();
	iDbAdapter->InitializeL();
		
	iVersion0Version1Converter = CMsvVersion0Version1Converter::NewL(*iDbAdapter,EDriveD,EFalse);
	}


void CTestVersion0Version1Converter::TearDownL()
	{
	_LIT(KFunction, "*_*TearDownL");
	INFO_PRINTF1(KFunction);
	
	delete iDbAdapter;
	delete iVersion0Version1Converter;
		
	__UHEAP_MARKEND;
	}


void CTestVersion0Version1Converter::TestConvertMessageStoreL()
	{
	_LIT(KFunction,"TestConvertMessageStoreL");
	INFO_PRINTF1(KFunction);
	
	TRAPD(err, iVersion0Version1Converter->ConvertMessageStoreL());		
	MY_ASSERT_EQUALS(err, KErrNone);		
	
	}


CTestSuite* CTestVersion0Version1Converter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestConvertMessageStoreL);
	END_SUITE;
	}

