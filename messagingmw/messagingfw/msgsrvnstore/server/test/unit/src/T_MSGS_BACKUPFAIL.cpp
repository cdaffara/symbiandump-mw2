// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// T_SEND.CPP
// 
//

#include <e32base.h>
#include <e32test.h>
#include <e32uid.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <msvapi.h>
#include <msvruids.h>
#include <msvuids.h>
#include <msvids.h>
#include <mtmuids.h>
#include <mtmdef.h>
#include <msvstd.h>
#include "T_MSGS_BACKUPFAILTEST.H"
#include <smut.h>
#include <smutset.h>
#include <smsclnt.h>
#include <miutset.h>
#include <babackup.h>
#include "T_MSGS_BACKUPFAIL.h"
#include "smstestutils.h"
#include <smuthdr.h>

RTest test(_L("Backup/Restore Testrig"));
const TInt KWaitForWatchersToStart = 15000000; //10 secs

// This test harness creates a mail folder with one sms in the outbox,
// it then does a backup, deletes the sms and then restores the mail folder.
// It then checks to see if the sms is present then waits for an sms message to be
// received by the phone. If the index has not be reloaded properly, the test harness
// will panic before the sms is received

//

//class CRenderingSendAsObserver : public CBase , public MSendAsObserver
//	{
//public:
//	TBool CapabilityOK(TUid /*aCapabilty*/, TInt /*aResponse*/) {return ETrue;}
//	TInt RenderImage(TUid aPrinterUid, const TDesC& aFileName);
//	TInt RenderImage(TUid aPrinterUid, const TDesC& aFileName, TRequestStatus& iCompletionStatus);
//public:
//	TFileName iFileName;
//	TUid iUid;
//	TInt iError;
//	TInt iCompletion;
//	};
/*
TInt CRenderingSendAsObserver::RenderImage(TUid aPrinterUid, const TDesC& aFileName) 
	{
	iFileName=aFileName; 
	iUid=aPrinterUid; 
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(fs, iFileName, iUid);
	store->CommitL();
	CleanupStack::PopAndDestroy();
	fs.Close();
	return iError;
	}

TInt CRenderingSendAsObserver::RenderImage(TUid aPrinterUid, const TDesC& aFileName, TRequestStatus& iCompletionStatus) 
	{
	TInt error = RenderImage(aPrinterUid, aFileName);
	if (error==KErrNone)
		{
		iCompletionStatus=KRequestPending;
		TRequestStatus* st=&iCompletionStatus;
		User::RequestComplete(st, iCompletion);
		}
	return error;
	}
*/
//

void CBackupTest::StartBackupL()
	{
	theUtils->WriteComment(_L("Starting backup"));
	if (iBackup)
		StopBackupRestoreL();

	iBackup = CBaBackupSessionWrapper::NewL();
	TRequestStatus status;
	iBackup->CloseAll(MBackupObserver::EReleaseLockReadOnly,status);
	User::WaitForRequest(status);
	User::After(1000000);
	}

void CBackupTest::StartRestoreL()
	{
	theUtils->WriteComment(_L("Starting restore"));
	if (iBackup)
		StopBackupRestoreL();

	iBackup = CBaBackupSessionWrapper::NewL();
	TRequestStatus status;
	iBackup->CloseAll(MBackupObserver::EReleaseLockNoAccess,status);
	User::WaitForRequest(status);
	User::After(1000000);
	}

void CBackupTest::StopBackupRestoreL()
	{
	theUtils->WriteComment(_L("Stoping backup/restore"));
	delete iBackup;
	iBackup = NULL;
	User::After(1000000);
	}

void CBackupTest::ReplaceMailFolderL()
	{
	theUtils->WriteComment(_L("Replacing mail folder"));
	iFileman->RmDir(_L("c:\\private\\1000484b\\mail\\"));
	iFileman->Copy(_L("c:\\msgtest\\T_BACKUP\\mail.copy\\"),_L("c:\\private\\1000484b\\mail\\"),CFileMan::ERecurse);
	}

void CBackupTest::CreateInitialMailFolderL()
	{
	theUtils->WriteComment(_L("Creating initial mail folder"));
	
	// Create the session, get the SMS mtm and get the outbox entry
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
 	CClientMtmRegistry* baseMtmRegistry=CClientMtmRegistry::NewL(*session,0);
	CleanupStack::PushL(baseMtmRegistry);
	CBaseMtm* smsMtm = baseMtmRegistry->NewMtmL(KUidMsgTypeSMS);
	CSmsClientMtm* smsClient = static_cast<CSmsClientMtm*>(smsMtm);
	CleanupStack::PushL(smsMtm);
	CMsvEntry* outbox = session->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(outbox);

	// Create Rich Text Object
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);
	richText->InsertL(0, _L("Test Message"));

	// Create a blank Entry
	TMsvEntry newEntry;
	newEntry.iMtm = KUidMsgTypeSMS;
	newEntry.iType = KUidMsvMessageEntry;
	newEntry.iServiceId = KMsvLocalServiceIndexEntryId;
	// work out how many characters of the original message, to use as a description of the message.
	TInt nGrab=30;
	if (richText->DocumentLength()<30)
		{
		nGrab = richText->DocumentLength();
		}

	newEntry.iDate.HomeTime();
	newEntry.iDescription.Set(richText->Read(0,nGrab));
	newEntry.iDetails.Set(_L("07674123456"));
    newEntry.SetSendingState(KMsvSendStateWaiting);

	outbox->CreateL(newEntry);
	CMsvEntry* entry = session->GetEntryL(newEntry.Id());
	smsClient->SetCurrentEntryL(entry);

	// Create the SMS header
	CSmsNumber* 		smsNumber 		= NULL;
	CSmsServiceCenter*	sc 				= NULL;

	// Create SMS Header
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*richText);
	CleanupStack::PushL(header);

    //restore send options
    CSmsSettings* sendOptions = CSmsSettings::NewL();
    CleanupStack::PushL(sendOptions);

    // first copy the default values to sendOptions
    sendOptions->CopyL(smsClient->ServiceSettings());

    // then get the per-message options
    CSmsHeader& cheader = smsClient->SmsHeader();
    cheader.GetSmsSettingsL(*sendOptions);
    // must see if we have to change sc address matching the address on current sim
    if (sendOptions->ServiceCenterCount()>0)
		{
        // if default sc is defined and is found from the sc addresses list we'll use it
        if ((sendOptions->DefaultServiceCenter()) >=0 && (sendOptions->DefaultServiceCenter() < sendOptions->ServiceCenterCount()))
            sc = &(sendOptions->GetServiceCenter(sendOptions->DefaultServiceCenter()));
        else
            sc = &(sendOptions->GetServiceCenter(0));

        header->Message().SetServiceCenterAddressL(sc->Address());
        }

	CleanupStack::PopAndDestroy();	// sendOptions

	smsNumber = CSmsNumber::NewL();
	CleanupStack::PushL(smsNumber);
	smsNumber->SetAddressL(_L("07929459948"));
	header->Recipients().AppendL(smsNumber);
	CleanupStack::Pop();

	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	header->StoreL(*store);
	store->StoreBodyTextL(*richText);
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store

	CleanupStack::PopAndDestroy(9);  
	// header, paraformatlayer, charformatlayer, richtext, outbox, session, ob1, registry, smsMtm
	}

void CBackupTest::CopyMailFolderL()
	{
	theUtils->WriteComment(_L("Backing up mail folder"));
	iFileman->RmDir(_L("c:\\msgtest\\T_BACKUP\\mail.copy\\"));
	iFileman->Copy(_L("c:\\private\\1000484b\\mail\\"),_L("c:\\msgtest\\T_BACKUP\\mail.copy\\"),CFileMan::ERecurse);
	}

void CBackupTest::RemoveSMSMessageL()
	{
	theUtils->WriteComment(_L("Removing SMS message from outbox"));
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	CMsvEntry* parentEntry	= NULL;
	CMsvEntrySelection* sel = NULL;

	parentEntry = session->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(parentEntry);
	sel = parentEntry->ChildrenWithMtmL(KUidMsgTypeSMS); // Get the sms messages in that folder
	CleanupStack::PushL(sel);
	if (sel->Count() == 0) 
		{
		theUtils->WriteComment(_L("Need at least one sms message in the inbox"));
		User::Leave(KErrGeneral);
		}
	for (TInt j=0;j<sel->Count();j++)
		{
		parentEntry->DeleteL(sel->At(j));
		}
	CleanupStack::PopAndDestroy();
	sel = parentEntry->ChildrenWithMtmL(KUidMsgTypeSMS); // Get the sms messages in that folder
	CleanupStack::PushL(sel);
	if (sel->Count() != 0) 
		{
		theUtils->WriteComment(_L("Could not delete all the sms messages"));
		User::Leave(KErrGeneral);
		}

	CleanupStack::PopAndDestroy(4);
	}

CBackupTest::CBackupTest() : CActive(0), iCount(0)
	{
	CActiveScheduler::Add(this);
	}


CBackupTest::~CBackupTest()
	{
	delete iBackup;
	delete iFileman;
	iFs.Close();
	delete iSmsTest;
	delete iWatchers;
	delete iInboxEntry;
	delete iSession;
	}

void CBackupTest::ConstructL()
	{
	iTimer.CreateLocal();
	iFs.Connect();
	iFileman = CFileMan::NewL(iFs);
	}

void CBackupTest::WaitForSMSL()
	{
	theUtils->WriteComment(_L("Waiting for an SMS message"));
	iSmsTest = CSmsTestUtils::NewL(test);
	iSmsTest->iSmokeTest = ETrue;

	iSmsTest->NotifySaPhoneOnL();
	iSmsTest->WaitForInitializeL();

	//Start the watchers
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	iSmsTest->Printf(_L("Waiting %d secs for watchers to start\n"), KWaitForWatchersToStart / 1000000); 
	iWatchers = CTestUtilsWatcherStarter::NewL(wait->iStatus, KWaitForWatchersToStart);
	wait->Start();
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(wait);
	iSession = CMsvSession::OpenSyncL(*this);

	iTimer.After(iStatus,500000);
	SetActive();

	CActiveScheduler::Start();
	}

void CBackupTest::CheckForSMSL()
	{
	theUtils->WriteComment(_L("Checking for one sms in the outbox"));
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	CMsvEntry* parentEntry	= NULL;
	CMsvEntrySelection* sel = NULL;

	parentEntry = session->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(parentEntry);
	sel = parentEntry->ChildrenWithMtmL(KUidMsgTypeSMS); // Get the sms messages in that folder
	CleanupStack::PushL(sel);
	if (sel->Count() != 1) 
		{
		theUtils->WriteComment(_L("Error: wrong number of messages in outbox"));
		User::Leave(KErrGeneral);
		}
	else
		theUtils->WriteComment(_L("Found one SMS message in outbox!"));

	CleanupStack::PopAndDestroy(4);
	}

void CBackupTest::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvEntriesCreated)
		{
		iCount++;
		}
	}

void CBackupTest::RunL()
	{
	if (iCount!=0)
		{
		theUtils->WriteComment(_L("Messages Received"));
		delete iWatchers;
		delete iSmsTest;
		delete iInboxEntry;
		iInboxEntry = NULL;
		iWatchers = NULL;
		iSmsTest = NULL;
		CActiveScheduler::Stop();
		}
	else
		{
		iTimer.After(iStatus,500000);
		SetActive();
		}
	}

void CBackupTest::DoCancel()
	{
	iTimer.Cancel();
	}

void RemoveMailFolderL(TInt aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);

	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
	test(parse.Set(ptr, &KMsvDefaultIndexFile(), NULL) == KErrNone);

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	test(error == KErrNone || error == KErrPathNotFound);

	CleanupStack::PopAndDestroy();
	}

void doTestsL()
	{
	RemoveMailFolderL(EDriveC);
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CleanupStack::PopAndDestroy(2);
	theUtils->Start(_L("Testing backup/restore with sms"));
	CBackupTest* test = new (ELeave) CBackupTest;
	CleanupStack::PushL(test);
	test->ConstructL();
	test->CreateInitialMailFolderL();
	test->StartBackupL();
	test->CopyMailFolderL();
	test->StopBackupRestoreL();
	test->RemoveSMSMessageL();
	test->StartRestoreL();
	test->ReplaceMailFolderL();
	test->StopBackupRestoreL();
	test->CheckForSMSL();
	test->WaitForSMSL();
	CleanupStack::PopAndDestroy();
	theUtils->Complete();
	}


