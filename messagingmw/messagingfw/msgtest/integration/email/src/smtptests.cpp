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

#include "smtptests.h"
#include <cemailaccounts.h>
#include <iapprefs.h>

// Strings....
_LIT(KErrUseService, "[%4d] Error! you have to select a service for use before you can access it!");
_LIT(KErrOperation,  "Operation didn't complete as expected. Expected status: [%d], actual status: [%d]");
_LIT(KPopAccountName, "POP");

const TInt KMaxErrorText = 100;

//
//
// CSmtpClientTest
//

CSmtpClientTest* CSmtpClientTest::NewL(CEmailTestUtils& aTestUtils, RTest& aTest)
	{
	CSmtpClientTest* self = new (ELeave) CSmtpClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSmtpClientTest::~CSmtpClientTest()
	{
	}

void CSmtpClientTest::ConstructL()
	{
	CEmailClientTest::ConstructL();
	}

CSmtpClientTest::CSmtpClientTest(CEmailTestUtils& aTestUtils, RTest& aTest) : CEmailClientTest(aTestUtils, aTest)
	{
	}



//
//
// CCreateSmtpService
//

void CCreateSmtpService::StartL(TRequestStatus& aStatus)
	{
	// create pop account (pop account also creates smtp service)
	CEmailAccounts* account = CEmailAccounts::NewLC();

	CImIAPPreferences* smtpIAP = CImIAPPreferences::NewLC();
				
	TSmtpAccount accountId = account->CreateSmtpAccountL(*iDetails,*iSettings, *smtpIAP, EFalse);

	// get pop service for the account
	TMsvId service = accountId.iSmtpService;
	
	// select this service
	CMsvEntry& msvEntry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	msvEntry.SetEntryL(service);
	iEntry = msvEntry.Entry();
	iEntry.SetVisible(ETrue);
	iTestHarness.iCurrentServiceId = service;
	iTestHarness.EmailTestUtils().iSmtpServiceId = service;

	CleanupStack::PopAndDestroy(2, account); // smtpIAP, account

	// If the SMTP Client MTM doesn't already exist, then Instantiate it 
	if (!iTestHarness.EmailTestUtils().iSmtpClientMtm)
		iTestHarness.EmailTestUtils().InstantiateSmtpClientMtmL();

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CCreateSmtpService* CCreateSmtpService::NewL(CEmailClientTest& aTestHarness)
	{
	CCreateSmtpService* self = new (ELeave) CCreateSmtpService(aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CCreateSmtpService::CCreateSmtpService(CEmailClientTest& aTestHarness) : CCreateEmailService(aTestHarness), iTestHarness(aTestHarness)
	{
	}

void CCreateSmtpService::ConstructL()
	{
	iSettings = new(ELeave) CImSmtpSettings();
	}

CCreateSmtpService::~CCreateSmtpService()
	{
	delete iSettings;
	}

CImSmtpSettings& CCreateSmtpService::Settings()
	{
	return *iSettings;
	}


//
//
// CCreateSmtpClientMtm
//

CCreateSmtpClientMtm::CCreateSmtpClientMtm(CEmailClientTest& aTest) : iTest(aTest)
	{
	}

void CCreateSmtpClientMtm::StartL(TRequestStatus& aStatus)
	{
	iTest.EmailTestUtils().InstantiatePopClientMtmL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}



//
//
// CCheckSmtpOperation
//

CCheckSmtpOperation::CCheckSmtpOperation(TInt aExpectedStatus, CSmtpClientTest& aTestHarness) : iTestHarness(aTestHarness), iExpectedStatus(aExpectedStatus)
	{
	}

void CCheckSmtpOperation::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	TImSmtpProgress temp;	
	TPckgC<TImSmtpProgress> paramPack(temp);
	paramPack.Set(iTestHarness.CurrentOperation().ProgressL());
	TImSmtpProgress progress=paramPack();	

	if (progress.Error() != iExpectedStatus)
		{
		TBuf<KMaxErrorText> logString;
		logString.Format(KErrOperation, iExpectedStatus, progress.Error());
		iTestHarness.LogComment(logString);
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}


//
//
// CSmtpUseService
//

CSmtpUseService::CSmtpUseService(CSmtpClientTest& aTestHarness) : iTestHarness(aTestHarness)
	{
	}

void CSmtpUseService::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	// Select current folder
	CMsvEntry& entry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	entry.SetEntryL(iTestHarness.iSelectedFolder);
		
	// Check it is a service which is selected
	if(entry.Entry().iType == KUidMsvServiceEntry)
		{
		// Make sure that the Root folder is selected
		iTestHarness.iSelectedFolder = KMsvRootIndexEntryId;

		// Change the Harness & Test Utils to use this SMTP service id
		iTestHarness.EmailTestUtils().iSmtpServiceId = entry.Entry().Id();
		iTestHarness.iCurrentServiceId = entry.Entry().Id();

		// If the Smtp Client MTM doesn't already exist, then Instantiate it 
		if (!iTestHarness.EmailTestUtils().iSmtpClientMtm)
			iTestHarness.EmailTestUtils().InstantiateSmtpClientMtmL();
		}
	else
		{
		TBuf<KMaxErrorText> logString;
		logString.Format(KErrUseService, iDebugInfo.LineNumber());
		iTestHarness.LogComment(logString);

		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

//
//
// CCopySelectionAndMonitor
//
CCopySelectionAndMonitor::CCopySelectionAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSynchronise(aSynchronise)
	{
	CActiveScheduler::Add(this);
	}



void CCopySelectionAndMonitor::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iParentTestHarness.EmailTestUtils().WriteComment(buf);
	}


void CCopySelectionAndMonitor::StartL(TRequestStatus& aStatus)
	{
	iParentTestHarness.MsvTestUtils().iMsvEntry->SetEntryL(iParentTestHarness.iSelectedFolder);
	iReportStatus = &aStatus;
	iParentTestHarness.SetCurrentOperation(iParentTestHarness.MsvTestUtils().iMsvEntry->CopyL(*(iParentTestHarness.iCurrentSelection), iParentTestHarness.iDestinationFolder, iStatus));
	
	// Call funtion to return IAP and Poll to find connection state 
	RTimer timer; 
	TRequestStatus timerStatus; // Request status associated with timer
	timer.CreateLocal(); // Create for this thread
	
	//initialise to unused values
	TInt progressState = KInvalidState;
	TInt connectState = KInvalidState;
	TInt iap = KInvalidIAP;
	TInt connection_progress_state = 0;

	do
		{
		TImSmtpProgress temp;	
		TPckgC<TImSmtpProgress> paramPack(temp);
		paramPack.Set(iParentTestHarness.CurrentOperation().ProgressL());
		TImSmtpProgress progress = paramPack();	

		progressState = progress.Status();
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

		
		// wait 500 microseconds and then re-test progress!
		timer.After(timerStatus,500);
		User::WaitForRequest(timerStatus); 

	
		} while (progressState == EMsgOutboxProgressConnecting);

	timer.Close();

	if (connection_progress_state != 3)
		{
		LogCommentFormat(_L("[%4d] Local Connection Progress state =  %d, Connect State = %d, IAP = %d"), iDebugInfo.LineNumber(), connection_progress_state, connectState, iap);
		iConnectionError = ETrue;
		}

	aStatus = KRequestPending;
	SetActive();
	}

void CCopySelectionAndMonitor::RunL()
	{
	if (iConnectionError)
		User::RequestComplete(iReportStatus, KErrUnknown);	
	else
		User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CCopySelectionAndMonitor::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CCopySelectionAndMonitor::~CCopySelectionAndMonitor()
	{
	}



