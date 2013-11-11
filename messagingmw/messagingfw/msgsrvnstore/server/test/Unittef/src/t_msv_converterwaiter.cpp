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

#include "t_msv_converterwaiter.h"
#include "MSVSERV.H"
#include "cmsvconverterwaiter.h"
 

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


//Literals
_LIT(KDbFileName, "D:\\messaging.db");
_LIT(KDbFileNameE, "E:\\messaging.db");
_LIT(KConverterMail2Folder, "c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2");
_LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
_LIT(KMail2FolderPathE, "e:\\private\\1000484b\\Mail2");



void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	if(iEvent == EMsvMessageStoreNotSupported)
		{
		CActiveScheduler::Stop();
		}
	}



CMyActiveWaiter::CMyActiveWaiter()
	: CActive(CActive::EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}
	
CMyActiveWaiter::~CMyActiveWaiter()
	{ Cancel(); }

void CMyActiveWaiter::SetActiveDontWait()
	{
	SetActive();
	}

void CMyActiveWaiter::WaitActive()
	{ 
	ASSERT(!iScheduler.IsStarted());
	SetActive();
	
	iScheduler.Start();
	}

void CMyActiveWaiter::RunL()
	{
	if (iScheduler.IsStarted())
		{
		ASSERT(iScheduler.CanStopNow());
		iScheduler.AsyncStop();
		}
	}

void CMyActiveWaiter::DoCancel()
	{
	if (iScheduler.IsStarted())
		{
		ASSERT(iScheduler.CanStopNow());
		iScheduler.AsyncStop();
		}
	}




void CTestMessageConverterWaiter::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	}

CTestMessageConverterWaiter::CTestMessageConverterWaiter()
	{
	}

CTestMessageConverterWaiter ::~CTestMessageConverterWaiter()
	{

	}


void CTestMessageConverterWaiter::SetupL()
	{
	_LIT(KSetupL,"SetupL");
	INFO_PRINTF1(KSetupL);
	
	__UHEAP_MARK;
	
	iActiveWaiter = new (ELeave) CMyActiveWaiter();
	iActiveWaiter2 = new (ELeave) CMyActiveWaiter();
	iActiveWaiter3 = new (ELeave) CMyActiveWaiter();
	iActiveWaiter4 = new (ELeave) CMyActiveWaiter();
	}

void CTestMessageConverterWaiter::TearDownL()
	{
	_LIT(KTearDownL,"TearDownL");
	INFO_PRINTF1(KTearDownL);


	
	if(iServer)
		{
		delete iServer;
		iServer = NULL;
		}
	if(iActiveWaiter)
		{
		delete iActiveWaiter;
		iActiveWaiter = NULL;
		}
	if(iActiveWaiter2)	
		{
		delete iActiveWaiter2;
		iActiveWaiter = NULL;
		}
	if(iActiveWaiter3)	
		{
		delete iActiveWaiter3;
		iActiveWaiter = NULL;
		}
	if(iActiveWaiter4)	
		{
		delete iActiveWaiter4;
		iActiveWaiter = NULL;
		}
	
	
	
	__UHEAP_MARKEND;
	}
	
	

void CTestMessageConverterWaiter::SetupTest()
	{
	_LIT(KFunction,"SetupTest");
	INFO_PRINTF1(KFunction);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// create the directories
	fs.MkDirAll(_L("D:\\private\\1000484b\\mail2\\"));
	fs.MkDirAll(_L("E:\\private\\1000484b\\mail2\\"));
	
	TParse parse;
	TPtrC drive;
	
	// delete persists cre file
	_LIT16(KPersistsFile,"c:\\private\\10202be9\\persists\\10286a26.cre");	
	TRAPD(err,fileMngr->Delete(KPersistsFile));	
	_LIT(KFunction1,"Deleting cre file %d");
	INFO_PRINTF2(KFunction1,err);	
	MY_ASSERT_EQUALS(err, KErrNone);

	//copy mail2
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	_LIT(KFunction2,"COPYING MAIL2 folder in drive D %d");
	INFO_PRINTF2(KFunction2,err);	
	MY_ASSERT_EQUALS(err, KErrNone);
		
	//copy mail2
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathE,2));
	_LIT(KFunction3,"COPYING MAIL2 folder in drive E %d");
	INFO_PRINTF2(KFunction3,err);	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(2);
	}
	
void CTestMessageConverterWaiter::TestCheckServerStartUpNotification()
	{
	_LIT(KFunction,"TestMessageServerStartUp");
	INFO_PRINTF1(KFunction);	
	
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
		
	CActiveScheduler::Start();
	
	MY_ASSERT_EQUALS(ob->iEvent, EMsvMessageStoreNotSupported);
		
	CleanupStack::PopAndDestroy(2);
	}
	
void CTestMessageConverterWaiter::TestGetUnsupportedDriveListL()
	{
	_LIT(KFunction,"TestGetUnsupportedDriveListL");
	INFO_PRINTF1(KFunction);
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 
	
	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	RArray<TDriveNumber> driveArray;
	
	session->GetConvertibleDriveListL(driveArray);
	
	MY_ASSERT_TRUE(driveArray.Count() != 0);

	driveArray.Close();
	CleanupStack::PopAndDestroy(2); // session
	
	}
	
	
void CTestMessageConverterWaiter::TestStartConverterWaiterL()
	{

	_LIT(KFunction,"TestMessageConverterWaiterStartUp");
	INFO_PRINTF1(KFunction);
	
	iServer = NULL;
	CMsvConverterWaiter* converterWaiter = NULL;
		// Instantiate converter waiter
	TRAPD(err,converterWaiter=CMsvConverterWaiter::InstanceL(iServer));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	delete converterWaiter;
	}
	
void CTestMessageConverterWaiter::TestConvertMessageStoreL()
	{
	_LIT(KFunction,"TestConvertMessageStoreL");
	INFO_PRINTF1(KFunction);
	
	// delete the db
	TestDeleteMessagingDBL();
	
	SetupTest();
		
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
		
	TRAPD(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->SetActiveDontWait();

	
	User::After(1000000);
	
	TRAP(err, session->ConvertMessageStore(EDriveE,iActiveWaiter2->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter2->SetActiveDontWait();
	
		
	User::After(1000000);

	
	TRAP(err, session->GetConversionStatusL(EDriveK));
	MY_ASSERT_EQUALS(err, -1);
	
	
	TRAP(err, session->GetConversionStatusL(EDriveE));
	MY_ASSERT_EQUALS(err, 0);
		
	User::After(50000000);
	session->CloseMessageServer();
	
	CleanupStack::PopAndDestroy(2); // session , ob
		
	}
	


void CTestMessageConverterWaiter::TestDeleteMessagingDBL()
	{
	_LIT(KFunction,"TestDeleteMessagingDBL");
	INFO_PRINTF1(KFunction);
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// delete messaging db
	TInt err = fileMngr->Delete(KDbFileName);
		
	err = fileMngr->Delete(KDbFileNameE);
		
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	

	}		

void CTestMessageConverterWaiter::TestCancelConversionL()
	{
	_LIT(KFunction,"TestCancelConversionL");
	INFO_PRINTF1(KFunction);
	
	TestDeleteMessagingDBL();
	
	SetupTest();
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	// fire conversion request	
	TRAPD(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->SetActiveDontWait();
	
	User::After(1000000);

	// this request will be queued
	TRAP(err, session->ConvertMessageStore(EDriveE,iActiveWaiter2->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter2->SetActiveDontWait();
	
	
	// this request will be queued
	TRAP(err, session->ConvertMessageStore(EDriveF,iActiveWaiter3->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter3->SetActiveDontWait();
	
	

	TRAP(err, session->ConvertMessageStore(EDriveG,iActiveWaiter4->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter4->SetActiveDontWait();

	err = session->CancelConversion(EDriveE);
	MY_ASSERT_EQUALS(err, KErrNone);

	err = session->CancelConversion(EDriveK);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	err = session->CancelConversion(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);

	User::After(1000000);
	err= session->CancelConversion(EDriveF);
	MY_ASSERT_EQUALS(err, KErrNotFound);	
	
	User::After(1000000);
	err= session->CancelConversion(EDriveG);
	MY_ASSERT_EQUALS(err, KErrNotFound);

	User::After(2000000);

	CleanupStack::PopAndDestroy(2); // session , ob
	}



void CTestMessageConverterWaiter::TestInstanceL()
	{
	_LIT(KFunction,"TestInstanceL");
	INFO_PRINTF1(KFunction);
	
	TRAPD(err, iServer = CMsvServer::NewL(EFalse));
	ASSERT_EQUALS(err, KErrNone);	
	
	CMsvConverterWaiter* converterWaiter=CMsvConverterWaiter::InstanceL(iServer);
	MY_ASSERT_TRUE(converterWaiter != NULL);
	
	CMsvConverterWaiter *waiter = CMsvConverterWaiter::InstanceL(iServer);
	MY_ASSERT_TRUE(waiter != NULL);
	
	
	}

void CTestMessageConverterWaiter::TestValidateDriveInPreferredDriveListL()
	{
	_LIT(KFunction,"TestValidateDriveInPreferredDriveListL");
	INFO_PRINTF1(KFunction);
	
	SetupTest();
	
	// we also need to create server	
	TRAPD(err, iServer = CMsvServer::NewL());
	_LIT(KFunction1,"Starting Message server %d");
	INFO_PRINTF2(KFunction1,err);	

	ASSERT_EQUALS(err, KErrNone);	
	
	CMsvConverterWaiter* converterWaiter=CMsvConverterWaiter::InstanceL(iServer);
	err = converterWaiter->ValidateDriveInPreferredDriveListL(EDriveD);
	INFO_PRINTF2(KFunction1,err);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = converterWaiter->ValidateDriveInPreferredDriveListL(EDriveE);
	INFO_PRINTF2(KFunction1,err);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = converterWaiter->ValidateDriveInPreferredDriveListL(EDriveT);
	INFO_PRINTF2(KFunction1,err);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	iServer->ResetRepositoryL();
	}
	
void CTestMessageConverterWaiter::TestValidateRequestedDriveL()
	{
	_LIT(KFunction,"TestValidateRequestedDriveL");
	INFO_PRINTF1(KFunction);
	
	// we also need to create server\
	
	TRAPD(err, iServer = CMsvServer::NewL(EFalse));
	ASSERT_EQUALS(err, KErrNone);	
	
	CMsvConverterWaiter* converterWaiter=CMsvConverterWaiter::InstanceL(iServer);
	err = converterWaiter->ValidateRequestedDriveL(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveE);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveF);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveP);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveQ);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveR);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveS);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = converterWaiter->ValidateRequestedDriveL(EDriveT);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// Copy version 1 message store 
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	
	// copy version 1 db in drive D
	_LIT(KDriveD,"D:\\");
	_LIT(KVersion1MessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\messaging.db");		
	
	TRAP(err, fileMngr->Copy(KVersion1MessageStore,KDriveD,2));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	// version 1 check
	err = converterWaiter->ValidateRequestedDriveL(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	// dummy implementation. Just for code coverage
	TRAP(err, converterWaiter->DoCancel());
	MY_ASSERT_EQUALS(err, KErrNone);
	iServer->ResetRepositoryL();
	}

	
void CTestMessageConverterWaiter::TestCancelVersion0Version1ConversionL()
	{
	_LIT(KFunction,"TestCancelVersion0Version1ConversionL");
	INFO_PRINTF1(KFunction);
	
	TestDeleteMessagingDBL();
	
	SetupTest();
	
		
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	// fire conversion request	
	TRAPD(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->SetActiveDontWait();
	
	User::After(1000000);

	err = session->CancelConversion(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	User::After(2000000);

	CleanupStack::PopAndDestroy(2); // session , ob
	}

void CTestMessageConverterWaiter::TestCancelVersion1Version2ConversionL()
	{
	_LIT(KFunction,"TestCancelVersion1Version2ConversionL");
	INFO_PRINTF1(KFunction);
	
	TestDeleteMessagingDBL();
	
	SetupTest();
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	// fire conversion request	
	TRAPD(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->SetActiveDontWait();
	
	User::After(1000000);

	err= session->CancelConversion(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);

	// wait for the above active request to complete
	User::After(4000000);
	
	CleanupStack::PopAndDestroy(2); // session , ob
	}

void CTestMessageConverterWaiter::TestCancelVersion1Version2NewConversionL()
	{
	_LIT(KFunction,"TestCancelVersion1Version2NewConversionL");
	INFO_PRINTF1(KFunction);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	//copy version 1 db
	TRAPD(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
		
		// copy version 1 db in drive D
	_LIT(KResumeIMAPDBPath, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\imapresume\\messaging.db");
	_LIT(KDriveD,"D:\\");
	TRAP(err, fileMngr->Copy(KResumeIMAPDBPath,KDriveD));	
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	// fire conversion request	
	TRAP(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->SetActiveDontWait();
	
	// wait for the above active request to complete
	User::After(100000);

	err = session->CancelConversion(EDriveD);
	MY_ASSERT_EQUALS(err, KErrNone);

	// wait for the above active request to complete
	User::After(2000000);
	
	CleanupStack::PopAndDestroy(2); // session , ob
	}

void CTestMessageConverterWaiter::TestPerformanceConvertMessageStoreL()
	{
	_LIT(KFunction,"TestPerformanceConvertMessageStoreL");
	INFO_PRINTF1(KFunction);
	
	// delete the db
	TestDeleteMessagingDBL();
	
	SetupTest();
		
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob); 

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	CleanupStack::PushL(session);
	
	TUint32 startTest = User::NTickCount();
		
	TRAPD(err, session->ConvertMessageStore(EDriveD,iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(err, KErrNone);
	iActiveWaiter->WaitActive();

	
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to Convert is %d milliseconds"),(endTest-startTest));
	
	session->CloseMessageServer();
	
	CleanupStack::PopAndDestroy(2); // session , ob
		
	}


CTestSuite* CTestMessageConverterWaiter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
		
	ADD_ASYNC_TEST_STEP(TestValidateDriveInPreferredDriveListL);
	ADD_ASYNC_TEST_STEP(TestValidateRequestedDriveL);
	ADD_ASYNC_TEST_STEP(TestInstanceL);	
	ADD_ASYNC_TEST_STEP(TestCheckServerStartUpNotification);
	ADD_ASYNC_TEST_STEP(TestGetUnsupportedDriveListL);
	ADD_ASYNC_TEST_STEP(TestStartConverterWaiterL);
	ADD_ASYNC_TEST_STEP(TestCancelConversionL);	
	ADD_ASYNC_TEST_STEP(TestConvertMessageStoreL);
	ADD_ASYNC_TEST_STEP(TestCancelVersion0Version1ConversionL);
	ADD_ASYNC_TEST_STEP(TestCancelVersion1Version2ConversionL);
	ADD_ASYNC_TEST_STEP(TestCancelVersion1Version2NewConversionL);
	ADD_ASYNC_TEST_STEP(TestPerformanceConvertMessageStoreL);
	END_SUITE;
	}

