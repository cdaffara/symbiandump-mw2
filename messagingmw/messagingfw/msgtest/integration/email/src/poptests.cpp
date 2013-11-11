// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "poptests.h"
#include <cemailaccounts.h>
#include <iapprefs.h>

// Strings....
_LIT(KErrUseService, "[%4d] Error! you have to select a service for use before you can access it!");
_LIT(KPopAccountName, "POP");

//
//
// CPop3ClientTest
//

CPop3ClientTest* CPop3ClientTest::NewL(CEmailTestUtils& aTestUtils, RTest& aTest)
	{
	CPop3ClientTest* self = new (ELeave) CPop3ClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPop3ClientTest::~CPop3ClientTest()
	{
	}

void CPop3ClientTest::ConstructL()
	{
	CEmailClientTest::ConstructL();
	}

CPop3ClientTest::CPop3ClientTest(CEmailTestUtils& aTestUtils, RTest& aTest) : CEmailClientTest(aTestUtils, aTest)
	{
	}


//
//
// CCreatePop3Service
//

void CCreatePop3Service::StartL(TRequestStatus& aStatus)
	{

	// create pop account				
	TPopAccount accountId = iAccount->CreatePopAccountL(*iDetails, *iSettings, *iPopIAP,EFalse);
	
	// select this service
	CMsvEntry& msvEntry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	msvEntry.SetEntryL(accountId.iPopService);
	iEntry = msvEntry.Entry();

	iTestHarness.iCurrentServiceId = accountId.iPopService;
	iTestHarness.EmailTestUtils().iPopServiceId = accountId.iPopService;
 
 	// If the Pop Client MTM doesn't already exist, then Instantiate it 
 	if (!iTestHarness.EmailTestUtils().iPopClientMtm)
 		iTestHarness.EmailTestUtils().InstantiatePopClientMtmL();
 	
 	// The account needs to be deleted to close the session with the message server
 	// If this session is kept open, the test will get in an infinite loop when sending
 	// an email (because of implementation of CMsvTestUtils::GoServerSideL())
 	delete iAccount;
 	iAccount = NULL;
 
 	TRequestStatus* status = &aStatus;
 	User::RequestComplete(status, KErrNone);

	}

CCreatePop3Service* CCreatePop3Service::NewL(CEmailClientTest& aTestHarness)
	{
	CCreatePop3Service* self = new (ELeave) CCreatePop3Service(aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CCreatePop3Service::CCreatePop3Service(CEmailClientTest& aTestHarness) : CCreateEmailService(aTestHarness), iTestHarness(aTestHarness)
	{
	}

void CCreatePop3Service::ConstructL()
	{
	iAccount = CEmailAccounts::NewL();
	iSettings = new(ELeave) CImPop3Settings();		
	iPopIAP = CImIAPPreferences::NewLC();
	CleanupStack::Pop();
	
	iAccount->PopulateDefaultPopSettingsL(*iSettings, *iPopIAP);
	iSettings->SetDisconnectedUserMode(ETrue);
	iSettings->SetDeleteEmailsWhenDisconnecting(ETrue);

	}

CCreatePop3Service::~CCreatePop3Service()
	{
	delete iAccount;
 	delete iSettings;
	delete iPopIAP;
	}

CImPop3Settings& CCreatePop3Service::Settings()
	{
	return *iSettings;
	}



//
//
// CCreatePop3ClientMtm
//

CCreatePop3ClientMtm::CCreatePop3ClientMtm(CEmailClientTest& aTest) : iTest(aTest)
	{
	}

void CCreatePop3ClientMtm::StartL(TRequestStatus& aStatus)
	{
	iTest.EmailTestUtils().InstantiatePopClientMtmL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}




//
//
// CPop3Connect
//

CPop3Connect::CPop3Connect(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CPop3Connect::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;

	// If the Pop Client MTM doesn't already exist, then Instantiate it 
	if (!iParentTestHarness.EmailTestUtils().iPopClientMtm)
		iParentTestHarness.EmailTestUtils().InstantiatePopClientMtmL();

	// check that current entry is a service
	CPop3ClientMtm& pop3Mtm = *(iParentTestHarness.EmailTestUtils().iPopClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(pop3Mtm.InvokeAsyncFunctionL(KPOP3MTMConnect, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CPop3Connect::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CPop3Connect::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CPop3Connect::~CPop3Connect()
	{
	}




//
//
// CPop3Disconnect
//

CPop3Disconnect::CPop3Disconnect(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CPop3Disconnect::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;
	// check that current entry is a service
	CPop3ClientMtm& pop3Mtm = *(iParentTestHarness.EmailTestUtils().iPopClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(pop3Mtm.InvokeAsyncFunctionL(KPOP3MTMDisconnect, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CPop3Disconnect::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CPop3Disconnect::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CPop3Disconnect::~CPop3Disconnect()
	{
	}





//
//
// CCheckPop3Operation
//

CCheckPop3Operation::CCheckPop3Operation(TInt aExpectedStatus, CPop3ClientTest& aTestHarness) : iTestHarness(aTestHarness), iExpectedStatus(aExpectedStatus)
	{
	}

void CCheckPop3Operation::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	TPop3Progress temp;	
	TPckgC<TPop3Progress> paramPack(temp);

	//if(iTestHarness.CurrentOperation().Id() != NULL)
		{
		paramPack.Set(iTestHarness.CurrentOperation().ProgressL());
			
		TPop3Progress progress=paramPack();	

		if (progress.iErrorCode != iExpectedStatus)
			{
			TBuf<100> logString;
			logString.Format(_L("Operation didn't complete as expected. Expected status: %d, actual status: %d"), iExpectedStatus, progress.iErrorCode);
			iTestHarness.LogComment(logString);
			err = KErrUnknown;
			}

		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, err);
		}
	}


//
//
// CPop3UseService
//

CPop3UseService::CPop3UseService(CPop3ClientTest& aTestHarness) : iTestHarness(aTestHarness)
	{
	}

void CPop3UseService::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	// Select current folder
	CMsvEntry& entry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	entry.SetEntryL(iTestHarness.iSelectedFolder);
		
	// first make sure it is a service which is selected
	//  then change the test utils service id
	if(entry.Entry().iType == KUidMsvServiceEntry)
		{
		iTestHarness.EmailTestUtils().iPopServiceId = entry.Entry().Id();

		// If the Pop Client MTM doesn't already exist, then Instantiate it 
		if (!iTestHarness.EmailTestUtils().iPopClientMtm)
			iTestHarness.EmailTestUtils().InstantiatePopClientMtmL();
		}
	else
		{
		TBuf<100> logString;
		logString.Format(KErrUseService, iDebugInfo.LineNumber());
		iTestHarness.LogComment(logString);

		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}



//
//
//
//
// CPop3Connect
//
// Connect an monitor connection progress.
//
CPop3ConnectAndMonitor::CPop3ConnectAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSynchronise(aSynchronise)
	{
	CActiveScheduler::Add(this);
	}

void CPop3ConnectAndMonitor::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iParentTestHarness.EmailTestUtils().WriteComment(buf);
	}

void CPop3ConnectAndMonitor::StartL(TRequestStatus& aStatus)
	{
	iConnectionError = EFalse;
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;
	// check that current entry is a service
	CPop3ClientMtm& pop3Mtm = *(iParentTestHarness.EmailTestUtils().iPopClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);

	
	iParentTestHarness.SetCurrentOperation(pop3Mtm.InvokeAsyncFunctionL(KPOP3MTMConnect, *selection, param, iStatus));

	// Call funtion to return IAP and Poll to find connection state 

	RTimer timer; 
	TRequestStatus timerStatus; // Request status associated with timer
	timer.CreateLocal(); // Create for this thread

	TInt progressState = KInvalidState;
	TInt connectState = KInvalidState;
	TInt iap = KInvalidIAP;
	TInt connection_progress_state = 0;


	do
		{
		TPop3Progress temp;	
		TPckgC<TPop3Progress> paramPack(temp);
		paramPack.Set(iParentTestHarness.CurrentOperation().ProgressL());
		TPop3Progress progress = paramPack();	

		progressState = progress.iPop3Progress;
				
		if((connectState != progress.ConnectionState()) || (iap != progress.ConnectionIAP()) )
			{
			connectState = progress.ConnectionState();
			iap = progress.ConnectionIAP();
			LogCommentFormat(_L("[%4d] Progress state =  %d, Connect State = %d, IAP = %d"), iDebugInfo.LineNumber(), progressState, connectState, iap);
			
			//Check that the relevant states are reached in the correct order
			if ((connectState == ECsdStartingDialling) || (connectState == ECsdStartingConnect))
					connection_progress_state = 1;
			if (connectState == ECsdConnectionOpen && connection_progress_state == 1)
					connection_progress_state = 2;
			if (connectState == KLinkLayerOpen && connection_progress_state == 2)
					connection_progress_state = 3;
				
			}

		// wait 50 microseconds and then re-test progress!
		timer.After(timerStatus,50);
		User::WaitForRequest(timerStatus); 
	
		} while (progressState == TPop3Progress::EPopConnecting);
	
	    timer.Close();

	if (connection_progress_state != 3)
		{
		LogCommentFormat(_L("[%4d] Local Connection Progress state =  %d, Connect State = %d, IAP = %d"), iDebugInfo.LineNumber(), connection_progress_state, connectState, iap);
		iConnectionError = ETrue;
		}
	
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CPop3ConnectAndMonitor::RunL()
	{
	if (iConnectionError)
		User::RequestComplete(iReportStatus, KErrUnknown);	
	else
		User::RequestComplete(iReportStatus, iStatus.Int());
	}	

void CPop3ConnectAndMonitor::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CPop3ConnectAndMonitor::~CPop3ConnectAndMonitor()
	{
	}
// CPop3SetSyncLimits
//

CPop3SetSyncLimits::CPop3SetSyncLimits(TInt aInboxSyncLimit, CPop3ClientTest& aTestHarness) : 
	iTestHarness(aTestHarness), iInboxSyncLimit(aInboxSyncLimit)
	{
	}

void CPop3SetSyncLimits::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestHarness.EmailTestUtils().WriteComment(buf);
	}

void CPop3SetSyncLimits::StartL(TRequestStatus& aStatus)
	{
	// Assumes that Pop3 service currently selected.

	LogCommentFormat(_L("[%4d] Setting sync limits (inbox=%d)"), iDebugInfo.LineNumber(), iInboxSyncLimit);

	CImPop3Settings* settings = new(ELeave) CImPop3Settings();
	CleanupStack::PushL(settings);

	CEmailAccounts* account = CEmailAccounts::NewLC();				

	TPopAccount popAccountId;
	account->GetPopAccountL(iTestHarness.EmailTestUtils().iMsvEntry->EntryId(),popAccountId); 
	account->LoadPopSettingsL(popAccountId, *settings);
	settings->SetInboxSynchronisationLimit(iInboxSyncLimit);		
	account->SavePopSettingsL(popAccountId, *settings);

	CleanupStack::PopAndDestroy(account);
	CleanupStack::PopAndDestroy(settings);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}


