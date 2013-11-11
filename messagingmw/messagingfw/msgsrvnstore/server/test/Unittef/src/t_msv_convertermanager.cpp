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

#include "t_msv_convertermanager.h"
#include "cactivewaiter.h"
#include "cmessageconvertermanager.h"

 
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


_LIT(KConverterMail2Folder, "c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2");		
_LIT(KDbFileName, "D:\\messaging.db");
_LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
_LIT(KVersion1MessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\messaging.db");
_LIT(KVersion0ResumeMessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\version0version1resume\\messaging.db");
_LIT(KVersion1ResumeMessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\version1version2resume\\messaging.db");
_LIT(KDriveD,"D:\\");
_LIT(KIndexFile,"c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2\\index");

CTestMessageConverterManager::CTestMessageConverterManager()
	{
	}

CTestMessageConverterManager ::~CTestMessageConverterManager()
	{
	}


void CTestMessageConverterManager::SetupL()
	{
	_LIT(KSetupL,"SetupL");
	INFO_PRINTF1(KSetupL);
	
	__UHEAP_MARK;
	
	iActiveWaiter = new(ELeave) CActiveWaiter();	
	
	
	}

void CTestMessageConverterManager::TearDownL()
	{
	_LIT(KTearDownL,"TearDownL");
	INFO_PRINTF1(KTearDownL);
	iConverterThread.Close();
	delete iActiveWaiter;
	
	__UHEAP_MARKEND;	
	}
	

void CTestMessageConverterManager::TestStartConversion()
	{
	_LIT(KFunction,"TestStartConversion");
	INFO_PRINTF1(KFunction);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	TPtrC drive;
	
		// create the directories
	fs.MkDirAll(_L("D:\\private\\1000484b\\mail2\\"));
	fs.MkDirAll(_L("E:\\private\\1000484b\\mail2\\"));
		
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//copy mail2
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	_LIT(KStartMessageStoreConversionL,"Starting Conversion on Drive D, this request will be cancelled after some time so that we can resume conversion in the next step");
	INFO_PRINTF1(KStartMessageStoreConversionL);
	
	//Conversion request
	TDriveNumber driveNumber = EDriveD;
	TAny* conversionStatus = &driveNumber;
	
	_LIT(KConverterTestThread,"StartMessageStoreConversion");
	err = iConverterThread.Create(KConverterTestThread, &CMessageConverterManager::StartConversion,KDefaultStackSize*8,KMinHeapSize,0x8000000,conversionStatus,EOwnerThread);

	// Request for notification
	TRAPD(error, iConverterThread.Logon(iActiveWaiter->iStatus);)	
	
	//Activate converter Thread
	iConverterThread.Resume();
	
	//my asserts
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(error, KErrNone);
	
	iActiveWaiter->SetActiveDontWait();
	// Cancel the request after some time, so that we resume
	// in the next test step. The next test step requires presence of conversion 
	// status table
	User::After(3000000);
	iConverterThread.Kill(KErrCancel);
			
	//Close the existing handle to the thread
	
	
	//
	
	}
	
void CTestMessageConverterManager::TestResumeConversionL()
	{
	_LIT(KFunction,"TestResumeConversionL");
	INFO_PRINTF1(KFunction);
	
	
	//Conversion request
	TDriveNumber driveNumber = EDriveD;
	TAny* conversionStatus = &driveNumber;
	
	_LIT(KConverterTestThread,"StartMessageStoreConversion");
	TInt err = iConverterThread.Create(KConverterTestThread, &CMessageConverterManager::StartConversion,KDefaultStackSize*8,KMinHeapSize,0x8000000,conversionStatus,EOwnerThread);

	// Request for notification
	TRAPD(error, iConverterThread.Logon(iActiveWaiter->iStatus);)	
	
	
	_LIT(KResumeConversion,"Resuming Message Store Conversion on Drive D");
	INFO_PRINTF1(KResumeConversion);
	
	//Activate converter Thread
	iConverterThread.Resume();
	
	//my asserts
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(error, KErrNone);

	//
	iActiveWaiter->WaitActive();
	}
	
	
	
void CTestMessageConverterManager::TestRemoveIndexFileL()
	{
	_LIT(KFunction,"TestRemoveIndexFileL");
	INFO_PRINTF1(KFunction);
			
	CMessageConverterManager* self  = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
		
	TRAPD(err,self->RemoveIndexFileL(EDriveD));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);
	}
	
void CTestMessageConverterManager::TestRemoveDatabaseFileL()
	{
	_LIT(KFunction,"TestRemoveDatabaseFileL");
	INFO_PRINTF1(KFunction);
			
	// we have a version 1 db from the previous test step
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);	
	
		// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();
	}

void CTestMessageConverterManager::TestRollBackVersion0Version1ConversionL()
	{
	_LIT(KFunction,"TestRollBackVersion0Version1ConversionL");
	INFO_PRINTF1(KFunction);
	
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
	
	TRAPD(err,self->RollBackVersion0Version1ConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	self->iDeleteDatabase = 0;
	
	CleanupStack::PopAndDestroy(self);
	}


void CTestMessageConverterManager::TestRollBackVersion1Version2ConversionL()
	{
	_LIT(KFunction,"TestRollBackVersion1Version2ConversionL");
	INFO_PRINTF1(KFunction);
	
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
		
	TRAPD(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err,self->RollBackVersion1Version2ConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);
	
	}
	
void CTestMessageConverterManager::TestResumeConversionFunctionL()
	{
	_LIT(KFunction,"TestResumeConversionFunctionL");
	INFO_PRINTF1(KFunction);
	
	// we have a version 1 db from the previous test step
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);	
	
		// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);

	TRAP(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
		
	err = self->ResumeConversionL();
	MY_ASSERT_TRUE(err = ETrue);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KVersion1MessageStore,KDriveD));
			
	err = self->ResumeConversionL();
	MY_ASSERT_TRUE(err =  ETrue);
	
	// create resume table
	CMsvDBAdapter* iDbAdapter;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	CleanupStack::PushL(iDbAdapter);	
	iDbAdapter->CreateConversionStatusTableL();
	
	err = self->ResumeConversionL();
	MY_ASSERT_TRUE(err = ETrue);
	
	// remove conversion status table
	iDbAdapter->RemoveConversionStatusTableL();
	
	err = self->ResumeConversionL();
	MY_ASSERT_TRUE(err = ETrue);
	
	CleanupStack::PopAndDestroy(iDbAdapter);
	
	CleanupStack::PopAndDestroy(self);
	
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();
	}

void CTestMessageConverterManager::InitialiseMessageStoreConversionL()
	{
	_LIT(KFunction,"InitialiseMessageStoreConversionL");
	INFO_PRINTF1(KFunction);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);	
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
			
	TRAP(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);

	CleanupStack::PopAndDestroy(self);
			
		// create resume table
	CMsvDBAdapter* iDbAdapter;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	CleanupStack::PushL(iDbAdapter);	
	
	// v0 to v1 entries in conversion db
	TConversionResumeStatus conversionResumeStatus;
	conversionResumeStatus.lastId = 124353;
	conversionResumeStatus.serviceId = 1048765;
	conversionResumeStatus.sourceVersion = 0;
	conversionResumeStatus.targetVersion = 1;
	TUid mtmId;
	mtmId.iUid = 287438383;
	conversionResumeStatus.mtmId = mtmId;
	conversionResumeStatus.remainingCount = 233;
	
	iDbAdapter->AddConversionStatusEntryL(conversionResumeStatus);
	
	self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
	//test
	TRAP(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);
	
	// remove conversion status table
	iDbAdapter->RemoveConversionStatusEntriesL();
	
	// v1 to v2 entries in conversion db
	conversionResumeStatus.lastId = 1243353;
	conversionResumeStatus.serviceId = 10448765;
	conversionResumeStatus.sourceVersion = 1;
	conversionResumeStatus.targetVersion = 2;
	conversionResumeStatus.mtmId = mtmId;
	conversionResumeStatus.remainingCount = 233;
	
	iDbAdapter->AddConversionStatusEntryL(conversionResumeStatus);
	
	
	self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
	//test
	TRAP(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);

	CleanupStack::PopAndDestroy(self);
		
	// delete db
	// cleanup mesasging.db in drive D
	
	iDbAdapter->RemoveConversionStatusTableL();
	
	self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
	//test
	TRAP(err, self->InitialiseMessageStoreConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);

	CleanupStack::PopAndDestroy(iDbAdapter);

	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	}
	
void CTestMessageConverterManager::TestVersion0Version2ConversionResumeL()
	{
	_LIT(KFunction,"TestVersion0Version2ConversionResumeL");
	INFO_PRINTF1(KFunction);
	
	// copy version0version1 resume db
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);	
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KVersion0ResumeMessageStore,KDriveD));
	
		//copy mail2
	_LIT(KMail2Folder, "d:\\private\\1000484b\\Mail2\\index");	
	TRAP(err, fileMngr->Copy(KIndexFile,KMail2Folder));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
		
	TRAP(err,self->DoConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	}

void CTestMessageConverterManager::TestVersion1Version2ConversionResumeL()
	{
	_LIT(KFunction,"TestVersion1Version2ConversionResumeL");
	INFO_PRINTF1(KFunction);
	
	// copy version1version2 resume db
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);	
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KVersion1ResumeMessageStore,KDriveD));
	
		
	_LIT(KMail2Folder, "d:\\private\\1000484b\\Mail2\\index");	
	//copy index file
	TRAP(err, fileMngr->Copy(KIndexFile,KMail2Folder));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMessageConverterManager* self = CMessageConverterManager::NewL(EDriveD); 
	CleanupStack::PushL(self);
		
	TRAP(err,self->DoConversionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(self);
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	}
	
CTestSuite* CTestMessageConverterManager::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestStartConversion);
	ADD_ASYNC_TEST_STEP(TestResumeConversionL);
	ADD_ASYNC_TEST_STEP(TestResumeConversionFunctionL);
	ADD_ASYNC_TEST_STEP(InitialiseMessageStoreConversionL);
	ADD_ASYNC_TEST_STEP(TestRemoveIndexFileL);
	ADD_ASYNC_TEST_STEP(TestRemoveDatabaseFileL);
	ADD_ASYNC_TEST_STEP(TestRollBackVersion1Version2ConversionL);
	ADD_ASYNC_TEST_STEP(TestRollBackVersion0Version1ConversionL);
	ADD_ASYNC_TEST_STEP(TestVersion0Version2ConversionResumeL);
	ADD_ASYNC_TEST_STEP(TestVersion1Version2ConversionResumeL);
		
	END_SUITE;
	}
