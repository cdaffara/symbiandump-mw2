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
// @file 
// [TestStep Name]
// UnitTestCMsvSession
// [Paramaters]
// Covers all the API for functional coverage and it is a unit test case.
// [APIs Used]
// CEmailAccounts::XXXXXXXXX All the API for code coverage.
// 
//



/* User include*/
#include "t_cmsvsession.h"
#include <t_utilsreademailsettingsfromconfigfile.h>

/* epoc inlcudes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <smtpset.h>
#include <msvfind.h>


/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapConfigFileName,"ImapConfigFileName");
_LIT(KDataComponentFilename, "c:\\system\\mtm\\testsmtm.dat");

const TUid KUidTestServerMtmType={0x10001304};
/**
  Function : CT_MsgCreateImapAccount
  Description : Constructor
  @return : N/A
*/
CT_CMsvSession::CT_CMsvSession(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KUnitTestCMsvSession);
	}

void CT_CMsvSession::TestOpenSyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of synchronous session to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session to the server -- ended"));
	
	error = KErrGeneral;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	INFO_PRINTF1(_L("Testing: creation of synchronous session with fileserver to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this, fs));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with fileserver to the server -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority to the server -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority and fileserver to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this, fs, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority and fileserver to the server -- ended"));

	CleanupStack::PopAndDestroy(&fs);
	}

void CT_CMsvSession::TestOpenAsyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session to the server -- ended"));
	
	error = KErrGeneral;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with fileserver to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this, fs));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with fileserver to the server -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority to the server -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority and fileserver to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this, fs, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority and fileserver to the server -- ended"));

	CleanupStack::PopAndDestroy(&fs);
	}

void CT_CMsvSession::TestOpenAsObserverL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer -- ended"));
	
	error = KErrGeneral;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with file session -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this, fs));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with file session -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with priority -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with priority -- ended"));
	
	error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with file session and priority -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this, fs, CMsvSession::EPriorityStandard));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Creates a session that is used only as an observer with file session and priority -- ended"));
	CleanupStack::PopAndDestroy(&fs);

	}

void CT_CMsvSession::TestAddObserverL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Registering a new Session Observer -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this));
	TEST(error == KErrNone);
	iSession->AddObserverL(*this);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Registering a new Session Observer -- ended"));	
	}

void CT_CMsvSession::TestRemoveObserver()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: De Registering a previously registered Observer -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsObserverL(*this));
	TEST(error == KErrNone);
	iSession->AddObserverL(*this);
	iSession->RemoveObserver(*this);
	delete iSession;
	INFO_PRINTF1(_L("Testing: De Registering a previously registered Observer -- ended"));	
	}

void CT_CMsvSession::TestSetReceiveEntryEvents()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: SetReceiveEntryEvents -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);
	iSession->SetReceiveEntryEvents(EFalse);
	delete iSession;
	INFO_PRINTF1(_L("Testing: SetReceiveEntryEvents -- ended"));
	}

void CT_CMsvSession::TestGetEntryL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: GetEntry -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);
	iSession->SetReceiveEntryEvents(EFalse);
	TRAP(error, iSession->GetEntryL(KMsvRootIndexEntryId));
	TEST(error == KErrNone);
	delete iSession;
	INFO_PRINTF1(_L("Testing: GetEntry -- ended"));
	}

void CT_CMsvSession::TestTransferCommandL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Transfer Commands -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	CMsvOperation* operation = NULL;	
	active->Start();
	operation = iSession->TransferCommandL(*selection, 1, param, active->iStatus);
	CActiveScheduler::Start();
	TEST(operation->iStatus.Int()==KErrNone);
	cEntry->DeleteL(service.Id());
	delete operation;
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Transfer Commands -- ended"));
	}

void CT_CMsvSession::TestIncPcSyncCountL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Increment PC Sync Count -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	TRAP(error, iSession->IncPcSyncCountL(*selection);)
	TEST(error == KErrNone);
	cEntry->DeleteL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Increment PC Sync Count -- ended"));
	}

void CT_CMsvSession::TestDecPcSyncCountL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Decrement PC Sync Count -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	TRAP(error, iSession->IncPcSyncCountL(*selection);)
	TEST(error == KErrNone);
	TRAP(error, iSession->DecPcSyncCountL(*selection);)
	TEST(error == KErrNone);
	cEntry->DeleteL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Decrement PC Sync Count  -- ended"));
	}

void CT_CMsvSession::TestGetChildIdsL()
	{

	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: GetChildId -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TRAP(error ,iSession->GetChildIdsL(KMsvLocalServiceIndexEntryId, *filter, *selection));
	TEST(selection->Find(KMsvGlobalInBoxIndexEntryId) >= 0);
	TEST(selection->Find(KMsvGlobalOutBoxIndexEntryId) >= 0);
	TEST(selection->Find(KMsvDraftEntryId) >= 0);
	TEST(selection->Find(KMsvSentEntryId) >= 0);

	TInt count = selection->Count();
	while(count--)
		{
		TMsvEntry entry;
		TMsvId service;
		TEST(iSession->GetEntry(selection->At(count), service, entry) == KErrNone);
		TEST(service == KMsvLocalServiceIndexEntryId);

		CMsvEntry* cEntry = iSession->GetEntryL(selection->At(count));
		delete cEntry;
		}

	CleanupStack::PopAndDestroy(selection); // selection
	CleanupStack::PopAndDestroy(filter);

	delete iSession;
	INFO_PRINTF1(_L("Testing: GetChildId  -- ended"));	
	}

void CT_CMsvSession::TestChangeAttributesL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Changing Attributes -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	TRAP(error, iSession->ChangeAttributesL(*selection, KMsvUnreadAttribute, KMsvInPreparationAttribute);)
	TEST(error == KErrNone);
	cEntry->DeleteL(service.Id());
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Changing Attributes -- ended"));

	}

void CT_CMsvSession::TestChangeDriveL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Change Drive -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	active->Start();
	cEntry->CreateL(service, active->iStatus);
	CActiveScheduler::Start();
	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	CMsvOperation* operation = NULL;	
	active->Start();
	TRAP(error, operation = iSession->ChangeDriveL(EDriveE, active->iStatus));
	TEST(error == KErrNone);
	CActiveScheduler::Start();
	delete operation;
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Change Drive -- ended"));

	}

void CT_CMsvSession::TestOutstandingOperationsL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Outstanding Operations -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	CMsvOperation* operation = NULL;
	TInt noOfOutstandingOperations = 0;
	active->Start();
	TRAP(error, operation = iSession->ChangeDriveL(EDriveC, active->iStatus));
	TEST(error == KErrNone);
	TRAP(error, noOfOutstandingOperations = iSession->OutstandingOperationsL());
	TEST(error == KErrNone);
	INFO_PRINTF2(_L("Expected Error is : 0, Error Occuered is %d"),error);
	CActiveScheduler::Start();
	TRAP(error, noOfOutstandingOperations = iSession->OutstandingOperationsL());
	TEST(error == KErrNone);
	TEST(noOfOutstandingOperations == 0);
	delete operation;
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Outstanding Operations -- ended"));
	}

void CT_CMsvSession::TestCopyStoreL()
	{
		TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Copy Store -- started"));
	TRAP(error, iSession = CMsvSession::OpenAsyncL(*this));
	TEST(error == KErrNone);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt ret = iSession->InstallMtmGroup(KDataComponentFilename);
	TEST(ret==KErrNone|| ret==KErrAlreadyExists);

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
//	cEntry->CreateL(service);

	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	CMsvOperation* operation = NULL;
	TInt noOfOutstandingOperations = 0;
	active->Start();
	TRAP(error, operation = iSession->CopyStoreL(EDriveE, active->iStatus));
	TEST(error == KErrNone);
	CActiveScheduler::Start();
	TEST(operation->iStatus.Int()==KErrNone);
	delete operation;
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: Copy Store -- ended"));
	}


void CT_CMsvSession::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	INFO_PRINTF1(_L("Entered Handlesession EventL"));
	switch(aEvent)
		{
			case EMsvEntriesCreated:
			INFO_PRINTF1(_L("Entries created"));
			break;
			case EMsvEntriesChanged:
			INFO_PRINTF1(_L("Entries Changed"));
			break;
			case EMsvEntriesDeleted:
			INFO_PRINTF1(_L("Entried Deleted"));
			break;
			case EMsvEntriesMoved:
			INFO_PRINTF1(_L("Entried Moved"));
			break;
			case EMsvMtmGroupInstalled:
			INFO_PRINTF1(_L("Mtm Group Installed"));
			break;
			case EMsvMtmGroupDeInstalled:
			INFO_PRINTF1(_L("Mtm Group De-installed"));
			break;
			case EMsvGeneralError:
			INFO_PRINTF1(_L("General Error occured"));
			break;
			case EMsvCloseSession:
			INFO_PRINTF1(_L("Session Closed"));
			break;
			case EMsvServerReady:
			INFO_PRINTF1(_L("Server Ready"));
			break;
			case EMsvServerFailedToStart:
			INFO_PRINTF1(_L("Failed to start"));
			break;
			case EMsvCorruptedIndexRebuilt:
			INFO_PRINTF1(_L("Corrupted Intex rebuilt"));
			break;
			case EMsvServerTerminated:
			INFO_PRINTF1(_L("Server terminated"));
			break;
			case EMsvMediaChanged:
			INFO_PRINTF1(_L("Media Changed"));
			break;
			EMsvMediaUnavailable:
			INFO_PRINTF1(_L("Media Unavailable"));
			break;
			case EMsvMediaAvailable:
			INFO_PRINTF1(_L("Media Available"));
			break;
			case EMsvMediaIncorrect:
			INFO_PRINTF1(_L("Media Incorrect"));
			break;
			case EMsvCorruptedIndexRebuilding:
			INFO_PRINTF1(_L("Corrupted Index Rebuilding"));
			break;
		}
	INFO_PRINTF1(_L("Exited Handlesession EventL"));
	}
	
/**
  Function : doTestStepL
  Description : Reads the Imap ccount name and the settings file name from the ini file.
				Creates an Imap account by reading the settings from the given settings file
  @return : TVerdict - Test step result
*/
TVerdict CT_CMsvSession::doTestStepL()
	{
//Running only in debug mode for code coverage reasons
	INFO_PRINTF1(_L("Test Step: CMsvSession Unit Test Start"));
#if defined (__DEBUG__)
	TInt error = KErrGeneral;
	TRAP(error, TestOpenSyncL());
	TRAP(error, TestOpenAsyncL());
	TRAP(error, TestOpenAsObserverL());
	TRAP(error, TestAddObserverL());
	TRAP(error, TestRemoveObserver());
	TRAP(error, TestSetReceiveEntryEvents());
	TRAP(error, TestGetEntryL());
	TRAP(error, TestTransferCommandL());
	TRAP(error, TestIncPcSyncCountL());
	TRAP(error, TestDecPcSyncCountL());
	TRAP(error, TestGetChildIdsL());
	TRAP(error, TestChangeAttributesL());
	TRAP(error, TestChangeDriveL());
	TRAP(error, TestOutstandingOperationsL());
	TRAP(error, TestCopyStoreL());
	INFO_PRINTF1(_L("Test Step: CMsvSession Unit Test Completed"));
	return TestStepResult();
#else	
	return TestStepResult();
#endif
	}
