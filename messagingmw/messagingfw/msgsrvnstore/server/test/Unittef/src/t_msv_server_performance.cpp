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

/**
 @file 
 @internalTechnology
*/
 
#include "t_msv_server_performance.h"
#include <msvapi.h>
#include <hal.h>


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


_LIT(KDbFileName, "\\messaging.db");
_LIT(KBaseDBFileName, "C:\\messaging_base.db");
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");

_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile1, "C:\\private\\1000484b\\msgPriorityDriveList1.ini"); //empty config file
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgPriorityDriveList2.ini"); //empty config file


CTestMsvServerPerformance::CTestMsvServerPerformance()
	{
	}



CTestMsvServerPerformance::~CTestMsvServerPerformance()
	{	
	}



void CTestMsvServerPerformance::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	// Delete database from default system drive (C:\)
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	// Create thr dir
	TInt err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	err = file->Copy(KBaseDBFileName, dbName); //ignore error
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)		
	// Use only one drive.
	err = file->Copy(KMsgTestPriorityDriveListFile1, KPreferredDriveListFilePath); //ignore error
#endif

	CleanupStack::PopAndDestroy(file);
	fs.Close();
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)		
	CMsvServer::ResetRepositoryL();
#endif
	}



void CTestMsvServerPerformance::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	// Delete database from default system drive (C:\)
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// restore the msgPriorityDriveList.ini file
	TInt err = file->Copy(KMsgTestPriorityDriveListFile2, KPreferredDriveListFilePath); //ignore error
#endif

	CleanupStack::PopAndDestroy(file);
	fs.Close();
	}




void CTestMsvServerPerformance::TestServerPerformance()
	{
	_LIT(KFunction, "TestServerPerformance");
	INFO_PRINTF1(KFunction);
	
	TInt frequency;
	HAL::Get(HALData::EFastCounterFrequency, frequency);
	INFO_PRINTF2(_L("<b>FastCounter frequency: %d</b>"), frequency);

	TInt err = KErrNone;
	TUint32 start, stop;
	CMsvServer *server = NULL;
	
	INFO_PRINTF1(_L("<b>Starting Message Server...</b>"));
	start = User::FastCounter();
	TRAP(err, server = CMsvServer::NewL());
	stop = User::FastCounter();
	MY_ASSERT_EQUALS(err, KErrNone);
	INFO_PRINTF1(_L("<b>Message Server started.</b>"));
	TReal64 diff = (TReal64)(stop-start)/frequency;
	INFO_PRINTF2(_L("---Time taken: <b>%f seconds.</b>"), diff);
	
	delete server;
	}
	
	

CTestSuite* CTestMsvServerPerformance::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestServerPerformance);
	END_SUITE;
	}

