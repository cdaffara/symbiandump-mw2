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

#include "imaptests.h"
#include <imapconnectionobserver.h>
#include <cemailaccounts.h>
#include <iapprefs.h>


//Provides access to Settings flag for seen flag tests 
LOCAL_D CImImap4Settings* isaveSettings;
_LIT(KImapAccountName, "IMAP");

// Strings....
_LIT(KErrUseService, "[%4d] Error! you have to select a service for use before you can access it!");


//
//
// CImapClientTest
//

CImapClientTest* CImapClientTest::NewL(CEmailTestUtils& aTestUtils, RTest& aTest)
	{
	CImapClientTest* self = new (ELeave) CImapClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImapClientTest::~CImapClientTest()
	{
	}

void CImapClientTest::ConstructL()
	{
	CEmailClientTest::ConstructL();
	}

CImapClientTest::CImapClientTest(CEmailTestUtils& aTestUtils, RTest& aTest) : CEmailClientTest(aTestUtils, aTest)
	{
	}



//
//
// CCreateImapService
//

void CCreateImapService::StartL(TRequestStatus& aStatus)
	{
	// create imap account
	CEmailAccounts* account = CEmailAccounts::NewLC();

	CImIAPPreferences* imapIAP = CImIAPPreferences::NewLC();
	
	iSettings->SetDisconnectedUserMode(ETrue);
	iSettings->SetSynchronise(EUseLocal);
	iSettings->SetSubscribe(EUpdateNeither);	
	TImapAccount accountId = account->CreateImapAccountL(*iDetails, *iSettings, *imapIAP,EFalse);

	//save settings address
	isaveSettings = iSettings;

	// get imap service for the account
	TMsvId service = accountId.iImapService;
	
	// select this service
	CMsvEntry& msvEntry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	msvEntry.SetEntryL(service);
	iEntry = msvEntry.Entry();
	iTestHarness.iCurrentServiceId = service;
	iTestHarness.EmailTestUtils().iImapServiceId = service;

	CleanupStack::PopAndDestroy(2, account); // imapIAP, account

	if(iTestHarness.EmailTestUtils().iImapClientMtm == NULL)
		iTestHarness.EmailTestUtils().InstantiateImapClientMtmL();
	else // no need to create a new one, reuse the existing one
		iTestHarness.EmailTestUtils().iImapClientMtm->SwitchCurrentEntryL(iTestHarness.iCurrentServiceId);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CCreateImapService* CCreateImapService::NewL(CEmailClientTest& aTestHarness)
	{
	CCreateImapService* self = new (ELeave) CCreateImapService(aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CCreateImapService::CCreateImapService(CEmailClientTest& aTestHarness) : CCreateEmailService(aTestHarness), iTestHarness(aTestHarness)
	{
	}

void CCreateImapService::ConstructL()
	{
	iSettings = new(ELeave) CImImap4Settings();
	}

CCreateImapService::~CCreateImapService()
	{
	delete iSettings;
	}

CImImap4Settings& CCreateImapService::Settings()
	{
	return *iSettings;
	}


//
//
// CImapUseService
//

CImapUseService::CImapUseService(CImapClientTest& aTestHarness) : iTestHarness(aTestHarness)
	{
	}

void CImapUseService::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	// Select current folder
	CMsvEntry& entry = *(iTestHarness.EmailTestUtils().iMsvEntry);
	entry.SetEntryL(iTestHarness.iSelectedFolder);
		
	// first make sure it is a service which is selected
	//  then change the test utils service id
	if(entry.Entry().iType == KUidMsvServiceEntry)
		{
		iTestHarness.EmailTestUtils().iImapServiceId = entry.Entry().Id();
		if(iTestHarness.EmailTestUtils().iImapClientMtm == NULL)
			iTestHarness.EmailTestUtils().InstantiateImapClientMtmL();
		else // no need to create a new one, reuse the existing one
			iTestHarness.EmailTestUtils().iImapClientMtm->SwitchCurrentEntryL(entry.Entry().Id());
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
// CCreateImapClientMtm
//

CCreateImapClientMtm::CCreateImapClientMtm(CEmailClientTest& aTest) : iTest(aTest)
	{
	}

void CCreateImapClientMtm::StartL(TRequestStatus& aStatus)
	{
	iTest.EmailTestUtils().InstantiatePopClientMtmL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

//
//
// CImapConnect
//

ImapConnectionObserver::ImapConnectionObserver(CImapConnect* aImapConnect) : iImapConnect(aImapConnect), iCount(0)
	{
	}

void ImapConnectionObserver::HandleImapConnectionEvent(TImapConnectionEvent)
	{
//	if (aConnectionState == ESynchronisationComplete)
//		if (++iCount == 2)
		if (++iCount == 1)
			iImapConnect->DummyRun();
	}

CImapConnect::CImapConnect(CEmailClientTest& aParentTestHarness, TSyncStatus aSynchronise) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSynchronise(aSynchronise)
	{
	CActiveScheduler::Add(this);
	}

void CImapConnect::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);

	if(iSynchronise == EConnectAndSync) 
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMConnectAndSynchronise, *selection, param, iStatus));
	else if (iSynchronise == EConnectAndPeriodicSync) 
		{
		iObserver = new(ELeave) ImapConnectionObserver(this);
		TPckg<MMsvImapConnectionObserver*> paramPeriodic(iObserver);
		iParentTestHarness.SetCurrentOperation(imapMtm.
		InvokeAsyncFunctionL(KIMAP4MTMConnectAndSyncCompleteAfterDisconnect, *selection, paramPeriodic, iStatus));
		}

//	KIMAP4MTMConnectAndSyncCompleteAfterConnect,
//	KIMAP4MTMConnectAndSyncCompleteAfterFullSync,
//	KIMAP4MTMConnectAndSyncCompleteAfterDisconnect,
	else
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMConnect, *selection, param, iStatus));

	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapConnect::RunL()
	{
	if (iDummyComplete == EFalse)
		User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapConnect::DummyRun()
	{
	User::RequestComplete(iReportStatus, KErrNone);
	iDummyComplete = ETrue;
	}

void CImapConnect::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapConnect::~CImapConnect()
	{
	if (iObserver)
		delete iObserver;
	iObserver=0; 
	}


//
//
// CImapConnect
//
// Connect an monitor connection progress.
//
CImapConnectAndMonitor::CImapConnectAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSynchronise(aSynchronise)
	{
	CActiveScheduler::Add(this);
	}

void CImapConnectAndMonitor::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iParentTestHarness.EmailTestUtils().WriteComment(buf);
	}

void CImapConnectAndMonitor::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;

	// Check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	TPckg<MMsvImapConnectionObserver*> parameter(this);

	if (iSynchronise)
		{
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMConnectAndSyncCompleteAfterFullSync, *selection, parameter, iStatus));
		}
	else
		{
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMConnect, *selection, parameter, iStatus));
		}

	CleanupStack::PopAndDestroy(selection);
	*iReportStatus = KRequestPending;
	SetActive();

	MonitorConnectionL();
	}

void CImapConnectAndMonitor::MonitorConnectionL()
	{
	//Initialise to unused values
	TInt progressState = KInvalidState;
	TInt connectState = KInvalidState;
	TInt iap = KInvalidIAP;
	TInt connection_progress_state = 0;

	RTimer timer; 
	TRequestStatus timerStatus; // Request status associated with timer
	timer.CreateLocal(); // Create for this thread

	do
		{	
		// Call funtion to return IAP and Poll to find connection state 
		TImap4GenericProgress temp;	
		TPckgC<TImap4GenericProgress> paramPack(temp);
		paramPack.Set(iParentTestHarness.CurrentOperation().ProgressL());
		TImap4GenericProgress progress = paramPack();	

		progressState = progress.iState;
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
	
		} while (progressState == TImap4GenericProgress::EConnecting);

	timer.Close();
	if (connection_progress_state != 3)
		{
		LogCommentFormat(_L("[%4d] Local Connection Progress state =  %d, Connect State = %d, IAP = %d"), iDebugInfo.LineNumber(), connection_progress_state, connectState, iap);
		iConnectionError = ETrue;
		}
	}

void CImapConnectAndMonitor::RunL()
	{
	if (iConnectionError)
		User::RequestComplete(iReportStatus, KErrUnknown);	
	else
		User::RequestComplete(iReportStatus, iStatus.Int());
	}

// HandleImapConnectionEvent Callback (from MMsvImapConnectionObserver)
void CImapConnectAndMonitor::HandleImapConnectionEvent(TImapConnectionEvent)
	{
	}

void CImapConnectAndMonitor::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}

CImapConnectAndMonitor::~CImapConnectAndMonitor()
	{
	}

//
//
// CImapConnectAndCancel
//
// Connect then disconnect before sync completes
//
CImapConnectAndCancel::CImapConnectAndCancel(CEmailClientTest& aParentTestHarness, TUint aCancelDelay) : CActive(EPriorityNormal),iParentTestHarness(aParentTestHarness), iCancelDelay(aCancelDelay)
	{
	CActiveScheduler::Add(this);
	}

void CImapConnectAndCancel::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iParentTestHarness.EmailTestUtils().WriteComment(buf);
	}


void CImapConnectAndCancel::StartL(TRequestStatus& aStatus)
	{
	iReportStatus = &aStatus;

	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	TBuf8<1> param;
	iSelection = new (ELeave) CMsvEntrySelection;
	iSelection->AppendL(serviceId);

	// Call the Async Connect & Sync funtion
	iParentTestHarness.SetCurrentOperation(iParentTestHarness.EmailTestUtils().iImapClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnect, 
											*iSelection, param, iStatus));

	iCommandState=EFirstConnect;
	aStatus = KRequestPending;
	SetActive();
	LogCommentFormat(_L("First connect started"));
	}

void CImapConnectAndCancel::RunL()
	{
	TBuf8<1> param;
	//LogCommentFormat(_L("CImapConnectAndCancel::RunL().  State = %d"), iCommandState);	

	switch (iCommandState)
		{
		case EFirstConnect:
			{
			// The first connect has completed.  Now Disconnect.
			iParentTestHarness.SetCurrentOperation(iParentTestHarness.EmailTestUtils().iImapClientMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,  
													*iSelection, param, iStatus));
			
			iCommandState = EDisconnect;
			*iReportStatus = KRequestPending;
			SetActive();
			LogCommentFormat(_L("Disconnecting"));
			break;
			}
		case EDisconnect:
			{
			// We have now disconnected after the first sync. 
			// Start a new connect and sync which we will cancel after 
			//  the specified interval
			iParentTestHarness.SetCurrentOperation(iParentTestHarness.EmailTestUtils().iImapClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSynchronise, 
													*iSelection, param, iStatus));

			iCommandState=ESecondSyncCancelled;		// This sync is about to be cancelled
			*iReportStatus = KRequestPending;
			SetActive();
			LogCommentFormat(_L("Second Connect and Sync started"));

			// This is where is gets a bit messy...

			// Define a timer so we can cancel after a specified period.
			// We can't wait for the connect to complete because it's too late by then.
			// We need to cancel during the stage when the IMAP server Capabilities 
			//  are being returned
			RTimer timer; 
			TRequestStatus timerStatus; // Request status associated with timer
			timer.CreateLocal(); // Create for this thread
			LogCommentFormat(_L("Waiting for %d msecs"), iCancelDelay);

			// Wait for the defined interval
			// OK, so this suspends the current thread which isn't great but the
			//  server is running in a different thread, so that continues as normal
			timer.After(timerStatus, iCancelDelay * 1000);
			User::WaitForRequest(timerStatus); 
			timer.Close();

			// Cancel the connect & sync
			DoCancel();
			LogCommentFormat(_L("Second connect & sync cancelled"));
			break;
			}
		case ESecondSyncCancelled: 
			// The second sync should have completed when the connect occured, but
			// we cancelled it before it had chance to complete.
			// Now start the final sync to check that all is still OK
			{
			iParentTestHarness.SetCurrentOperation(iParentTestHarness.EmailTestUtils().iImapClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSynchronise, 
													*iSelection, param, iStatus));

			iCommandState=EThirdSync;
			*iReportStatus = KRequestPending;
			SetActive();
			LogCommentFormat(_L("Third Connect and Sync started"));

			break;
			}
		case EThirdSync:
			{
			// At this point the third ConnectAndSync reports that it is completed, but actually 
			//  only the connect has.  The sync continues in background, so create an operation
			//  to wait for it to complete.
			iParentTestHarness.SetCurrentOperation(iParentTestHarness.EmailTestUtils().iImapClientMtm->InvokeAsyncFunctionL(KIMAP4MTMWaitForBackground,  
													*iSelection, param, iStatus));
			
			iCommandState = EComplete;
			*iReportStatus = KRequestPending;
			SetActive();
			LogCommentFormat(_L("Waiting for third sync to complete"));
			break;
			}
		case EComplete:
			{
			// Everything has completed.  Success!
			LogCommentFormat(_L("Completed"));
			User::RequestComplete(iReportStatus, KErrNone);
			break;
			}
		default:
			{
			//  This shouldn't happen...
			LogCommentFormat(_L("Unknown state in ConnectAndCancel"));
			User::RequestComplete(iReportStatus, KErrUnknown);
			break;
			}
		}
	}



void CImapConnectAndCancel::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapConnectAndCancel::~CImapConnectAndCancel()
	{
	delete iSelection;
	}


//
//
// CImapDisconnect
//

CImapDisconnect::CImapDisconnect(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapDisconnect::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	msvEntry.SetEntryL(serviceId);
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMDisconnect, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapDisconnect::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapDisconnect::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapDisconnect::~CImapDisconnect()
	{
	}


//
//
// CImapSetSyncLimits
//

CImapSetSyncLimits::CImapSetSyncLimits(TInt aInboxSyncLimit, TInt aFolderSyncLimit, CImapClientTest& aTestHarness) : 
	iTestHarness(aTestHarness), iInboxSyncLimit(aInboxSyncLimit), iFolderSyncLimit(aFolderSyncLimit)
	{
	}

void CImapSetSyncLimits::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestHarness.EmailTestUtils().WriteComment(buf);
	}

void CImapSetSyncLimits::StartL(TRequestStatus& aStatus)
	{
// Assumes that IMAP service currently selected.

	LogCommentFormat(_L("[%4d] Setting sync limits (inbox=%d) (folders=%d)"), iDebugInfo.LineNumber(), iInboxSyncLimit, iFolderSyncLimit);
	
	CImImap4Settings* settings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(settings);	
	
	CEmailAccounts* account = CEmailAccounts::NewLC();
	
	TImapAccount imapAccountId;
	account->GetImapAccountL(iTestHarness.EmailTestUtils().iMsvEntry->EntryId(),imapAccountId); 
	account->LoadImapSettingsL(imapAccountId, *settings);
						
	settings->SetInboxSynchronisationLimit(iInboxSyncLimit);
	settings->SetMailboxSynchronisationLimit(iFolderSyncLimit);
	account->SaveImapSettingsL(imapAccountId, *settings);

	CleanupStack::PopAndDestroy(account);
	CleanupStack::PopAndDestroy(settings);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

//
//
// CCheckImapOperation
//

CCheckImapOperation::CCheckImapOperation(TInt aExpectedStatus, CImapClientTest& aTestHarness) : iTestHarness(aTestHarness), iExpectedStatus(aExpectedStatus)
	{
	}

void CCheckImapOperation::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	TImap4GenericProgress temp;	
	TPckgC<TImap4GenericProgress> paramPack(temp);
	paramPack.Set(iTestHarness.CurrentOperation().ProgressL());
	TImap4GenericProgress progress=paramPack();	

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


//
//
// CImapSyncTree
//

CImapSyncTree::CImapSyncTree(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapSyncTree::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMSyncTree, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapSyncTree::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapSyncTree::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapSyncTree::~CImapSyncTree()
	{
	}



//
//
// CImapInboxNewSync
//

CImapInboxNewSync::CImapInboxNewSync(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapInboxNewSync::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMInboxNewSync, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapInboxNewSync::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapInboxNewSync::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapInboxNewSync::~CImapInboxNewSync()
	{
	}

//
//
// CImapFolderSync
//

CImapFolderSync::CImapFolderSync(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapFolderSync::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMFolderFullSync, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapFolderSync::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapFolderSync::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapFolderSync::~CImapFolderSync()
	{
	}

//
//
// CImapPerformFullSync
//

CImapPerformFullSync::CImapPerformFullSync(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapPerformFullSync::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMFullSync, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}														

void CImapPerformFullSync::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapPerformFullSync::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}

CImapPerformFullSync::~CImapPerformFullSync()
	{
	}

//
//
// CImapPopulateFolder
//

CImapPopulateFolder::CImapPopulateFolder(CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CImapPopulateFolder::StartL(TRequestStatus& aStatus)
	{
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMPopulate, *(iParentTestHarness.iCurrentSelection), param, iStatus));
	aStatus = KRequestPending;
	SetActive();
	}

void CImapPopulateFolder::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapPopulateFolder::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapPopulateFolder::~CImapPopulateFolder()
	{
	}



//
//
// CImapSubscribeFolder
//

CImapSubscribeFolder::CImapSubscribeFolder(TBool aSubscribe, CEmailClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSubscribe(aSubscribe)
	{
	CActiveScheduler::Add(this);
	}

void CImapSubscribeFolder::StartL(TRequestStatus& aStatus)
	{
	TMsvId serviceId = iParentTestHarness.iSelectedFolder;
	iReportStatus = &aStatus;
	// check that current entry is a service
	CImap4ClientMtm& imapMtm = *(iParentTestHarness.EmailTestUtils().iImapClientMtm);
	TBuf8<1> param;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(serviceId);
	if (iSubscribe)
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMLocalSubscribe, *selection, param, iStatus));
	else
		iParentTestHarness.SetCurrentOperation(imapMtm.InvokeAsyncFunctionL(KIMAP4MTMLocalUnsubscribe, *selection, param, iStatus));
	CleanupStack::PopAndDestroy(selection);
	aStatus = KRequestPending;
	SetActive();
	}

void CImapSubscribeFolder::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CImapSubscribeFolder::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CImapSubscribeFolder::~CImapSubscribeFolder()
	{
	}


//
//
// CImapMarkSelectionUnRead
//

CImapMarkSelection::CImapMarkSelection(TBool aUnreadflag, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection),
  iUnreadflag(aUnreadflag)
	{
	}

void CImapMarkSelection::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CImapMarkSelection::StartL(TRequestStatus& aStatus)
	{
	// check that current entry is a service
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);	
	
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	LogCommentFormat(_L("[%4d] Unread mark Folder contains %d children"), iDebugInfo.LineNumber(), iSelection.Count());

	TInt count=iSelection.Count();
	// Set the Unread flag of each message
	while(count--)
		{
		msvEntry.SetEntryL(iSelection.At(count));
		TMsvEntry message = msvEntry.Entry();
		
		message.SetUnread(iUnreadflag);	// true - sets message to unread and false to read.

		CMsvOperation *op=msvEntry.ChangeL(message,active->iStatus);
		CleanupStack::PushL(op);
		active->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(op);

		}

	LogCommentFormat(_L("[%4d] Unread mark: Set things read"), iDebugInfo.LineNumber());

	CleanupStack::PopAndDestroy(active);

	LogCommentFormat(_L("[%4d] Unread mark: Completing parent"), iDebugInfo.LineNumber());

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}														


CImapMarkSelection::~CImapMarkSelection()
	{
	}


//
//
// CImapCheckSelectionUnread
//

CImapCheckSelectionUnread::CImapCheckSelectionUnread(TInt aTestType, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection),
  iTestType(aTestType)
  	{
	}

void CImapCheckSelectionUnread::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CImapCheckSelectionUnread::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	// check that current entry is a service
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);	
	
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	LogCommentFormat(_L("[%4d] Unread mark Folder contains %d children"), iDebugInfo.LineNumber(), iSelection.Count());

	TInt count=iSelection.Count();

	//Check unseen and imap4 flag
	TBool imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent;
	TBool Unreadflag;	
	// Check the Unread flag of each message

	while(count--)
		{
		msvEntry.SetEntryL(iSelection.At(count));
		TMsvEmailEntry emailmessage = msvEntry.Entry();

		/* get seen flag */
		emailmessage.GetIMAP4Flags(imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent);
		/* get relevant unread flag */
		Unreadflag = emailmessage.Unread();
		
		switch (iTestType)
			{
			case 0:
				/* read test */	
				if (imapUnread)
					{
					LogCommentFormat(_L("[%4d] Error: Unread flag wrongly set to true "), iDebugInfo.LineNumber());
					err = KErrUnknown;
					}
			break;
			case 1:	
				/* unread test */
				if (!Unreadflag)
					{
					LogCommentFormat(_L("[%4d] Error: Unread flag wrongly set to false "), iDebugInfo.LineNumber());
					err = KErrUnknown;
					}	
				break;
			}
		}
	
	LogCommentFormat(_L("[%4d] all flags checked"), iDebugInfo.LineNumber());

	CleanupStack::PopAndDestroy(active);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}														


CImapCheckSelectionUnread::~CImapCheckSelectionUnread()
	{
	}


//
//
// CImapCheckSelectionSeen
//

CImapCheckSelectionSeen::CImapCheckSelectionSeen(TInt aSeenflag, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection),
  iSeenflag(aSeenflag)
  	{
	}

void CImapCheckSelectionSeen::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CImapCheckSelectionSeen::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	// check that current entry is a service
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);	
	
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	LogCommentFormat(_L("[%4d] Unread mark Folder contains %d children"), iDebugInfo.LineNumber(), iSelection.Count());

	TInt count=iSelection.Count();

	//Check unseen and imap4 flag
	TBool imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent;

	TBool updateSeenflag = isaveSettings->UpdatingSeenFlags();
	
	if (updateSeenflag)
	{
		while(count--)
			{
			msvEntry.SetEntryL(iSelection.At(count));
			TMsvEmailEntry emailmessage = msvEntry.Entry();

			/* get seen flag */
			emailmessage.GetIMAP4Flags(imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent);
			
			switch (iSeenflag)
				{
				case 0:	
					/* unread test */
					if(Seen)
						{
						LogCommentFormat(_L("[%4d] Error: seenImap4 flag set to true "), iDebugInfo.LineNumber());
						err = KErrUnknown;
						}		
				break;
				case 1:
					/* read test */		
					if(Seen)
						{
						LogCommentFormat(_L("[%4d] Error: seenImap4 flag set to false "), iDebugInfo.LineNumber());
						err = KErrUnknown;
						}		
				break;	
				}
			}
	}
	else
	{
		LogCommentFormat(_L("[%4d] Error: this test should not be called when updateseenflag false "), iDebugInfo.LineNumber());
		err = KErrUnknown;
	}
	
	LogCommentFormat(_L("[%4d] all flags checked"), iDebugInfo.LineNumber());

	CleanupStack::PopAndDestroy(active);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}														


CImapCheckSelectionSeen::~CImapCheckSelectionSeen()
	{
	}




//
//
// CImapMarkSelectedMail
//

CImapMarkSelectedMail::CImapMarkSelectedMail(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection)
	{
	}

void CImapMarkSelectedMail::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CImapMarkSelectedMail::StartL(TRequestStatus& aStatus)
	{
	// check that current entry is a service
	
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);	
	
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	LogCommentFormat(_L("[%4d] Unread mark Folder contains %d children"), iDebugInfo.LineNumber(), iSelection.Count());

	TInt count=iSelection.Count();

	while(count--)
		{
		msvEntry.SetEntryL(iSelection.At(count));
		TMsvEntry message = msvEntry.Entry();
		
		//Set every other message to read and unread!
		if ((count & 1) == 0)
			message.SetUnread(ETrue);
		else
			message.SetUnread(EFalse);	

		CMsvOperation *op=msvEntry.ChangeL(message,active->iStatus);
		CleanupStack::PushL(op);
		active->StartL();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(op);

		}
	LogCommentFormat(_L("[%4d] Flag settings done"), iDebugInfo.LineNumber());

	CleanupStack::PopAndDestroy(active);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}														


CImapMarkSelectedMail::~CImapMarkSelectedMail()
	{
	}



//
//
// CImapCheckSelectedMail
//

CImapCheckSelectedMail::CImapCheckSelectedMail(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection)
	{
	}

void CImapCheckSelectedMail::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}
void CImapCheckSelectedMail::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	// check that current entry is a service
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);	
	
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	LogCommentFormat(_L("[%4d] Selection test. Unread mark Folder contains %d children"), iDebugInfo.LineNumber(), iSelection.Count());

	TInt count=iSelection.Count();

	//Check unseen and imap4 flag
	TBool imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent;
	TBool Unreadflag;	
	
	TBool updateSeenflag = isaveSettings->UpdatingSeenFlags();

	while(count--)
		{
		msvEntry.SetEntryL(iSelection.At(count));
		
		TMsvEmailEntry emailmessage = msvEntry.Entry();
		emailmessage.GetIMAP4Flags(imapUnread, Seen, Answered, Flagged, Deleted, Draft, Recent);
		
		Unreadflag = emailmessage.Unread();

		if ((count & 1) == 0)
			{
			if (Unreadflag)
				{
				LogCommentFormat(_L("[%4d] Error: Unread flag wrongly set to false, Message No. = %d  "), iDebugInfo.LineNumber(), count);
				err = KErrUnknown;
				}
			if (updateSeenflag)
				{
				if(!Seen)
					{
					LogCommentFormat(_L("[%4d] Error: seenImap4 flag wrongly set to true, Message No. = %d  "), iDebugInfo.LineNumber(), count);
					err = KErrUnknown;
					}
				}
			}
		else
			{
			if (!Unreadflag)
				{
				LogCommentFormat(_L("[%4d] Error: Unread flag wrongly set to true, Message No. = %d "), iDebugInfo.LineNumber(), count);
				err = KErrUnknown;
				}
			if (updateSeenflag)
				{
				if(Seen)
					{
					LogCommentFormat(_L("[%4d] Error: seenImap4 flag wrongly set to true, Message No. = %d  "), iDebugInfo.LineNumber(), count);
					err = KErrUnknown;
					}
				}		
			}
		}
	
	LogCommentFormat(_L("[%4d] all flags checked"), iDebugInfo.LineNumber());

	CleanupStack::PopAndDestroy(active);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}														


CImapCheckSelectedMail::~CImapCheckSelectedMail()
	{
	}


//
//
// CImapSetSyncRate
//

CImapSetSyncRate::CImapSetSyncRate(TInt aSyncRate, CImapClientTest& aTestHarness) : iTestHarness(aTestHarness), iSyncRate(aSyncRate)
	{
	}

void CImapSetSyncRate::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	CImImap4Settings* settings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(settings);

	CEmailAccounts* account = CEmailAccounts::NewLC();				
		
	TImapAccount imapAccountId;
	account->GetImapAccountL(iTestHarness.EmailTestUtils().iMsvEntry->EntryId(),imapAccountId); 
	account->LoadImapSettingsL(imapAccountId, *settings);
		
	settings->SetSyncRateL(iSyncRate);
	account->SaveImapSettingsL(imapAccountId, *settings);

	CleanupStack::PopAndDestroy(2, settings); // account, settings

	TBuf<100> logString;
	logString.Format(_L("Set sync rate: %d seconds"), iSyncRate);
	iTestHarness.LogComment(logString);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

//
//
// CImapGetSyncRate
//

CImapGetSyncRate::CImapGetSyncRate(TInt aSyncRate, CImapClientTest& aTestHarness) : iTestHarness(aTestHarness), iSyncRate(aSyncRate)
	{
	}

void CImapGetSyncRate::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	CImImap4Settings* settings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(settings);

	CEmailAccounts* account = CEmailAccounts::NewLC();				
	
	TImapAccount imapAccountId;
	account->GetImapAccountL(iTestHarness.EmailTestUtils().iMsvEntry->EntryId(),imapAccountId); 
	account->LoadImapSettingsL(imapAccountId, *settings);
			
	TUint syncRate = settings->SyncRate();
	
	account->SaveImapSettingsL(imapAccountId, *settings);

	CleanupStack::PopAndDestroy(2, settings); // account, settings

	TBuf<100> logString;
	logString.Format(_L("Expected sync rate: %d seconds Obtained: %d"), iSyncRate,syncRate);
	iTestHarness.LogComment(logString);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

