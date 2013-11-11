// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <txtrich.h>
#include <txtfmlyr.h>
#include "testsetup.h"
#include "msvstore.h"
#include "../support/inc/backupserver.h"
#include <babackup.h>


_LIT(KMsgDir,"C:\\private\\1000484b\\Mail\\");
_LIT(KMsgLocalService,"C:\\private\\1000484b\\Mail\\00001001_S\\");
_LIT(KMsgBackupDir,"C:\\MailBackup\\");
_LIT(KAnAddress,"Test@psion7.demon.co.uk");
_LIT(KADescription,"A Message");

// local variables//

RTest test(_L("MSV Backup Testrig"));

//

const TUid KUidMsgTypePOP3			= {0x10001029};	// 268439593

const TUid KMsvEntryFile={0x10003C68};


class CSessionObserver : public CBase, public MMsvSessionObserver
	{
	class TEvents
		{
		public:
			TMsvSessionEvent iType;
			TInt iArg1;
			TInt iArg2;
		};
		public:
			void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*);
		public:
			TEvents iTheEvents[10];
			TInt iCurrent;
	};

void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	TBuf<128> event;
	_LIT(KEventFormat,"Event %d, (%d,%d,%d)");
	event.Format(KEventFormat,aEvent,(TInt) aArg1,(TInt) aArg2,(TInt) aArg3);
	theUtils->WriteComment(event);
	if(iCurrent<10)
		{
		iTheEvents[iCurrent].iType = aEvent;
		if (aArg1)
			iTheEvents[iCurrent].iArg1 = *(TInt*)aArg1;
		if (aArg2)
			iTheEvents[iCurrent].iArg2 = *(TInt*)aArg2;
		}
	iCurrent++;
	}



LOCAL_C void TestInitialStructureL(CMsvSession& aSession)
	{
	TInt services=1;
	TInt localFolders=5;
	
	CMsvEntry* cEntry = aSession.GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry);
	
	// root
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	test(cEntry->Entry().iDetails==KNullDesC);
	test(cEntry->Entry().iDescription==KNullDesC);
	test(cEntry->Count()==services);
	
	// local service
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry->SetSortTypeL(TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	test(cEntry->Entry().iDetails==_L("Local"));
	test(cEntry->Count()==localFolders);
	
	// standard folders
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	test(cEntry->Entry().iDetails==_L("Inbox"));
	cEntry->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	test(cEntry->Entry().iDetails==_L("Outbox"));
	cEntry->SetEntryL(KMsvDraftEntryId);
	test(cEntry->Entry().iDetails==_L("Drafts"));
	cEntry->SetEntryL(KMsvSentEntryId);
	test(cEntry->Entry().iDetails==_L("Sent"));
	cEntry->SetEntryL(KMsvDeletedEntryFolderEntryId);
	test(cEntry->Entry().iDetails==_L("Deleted"));
	
	// check that only the index file and the local services dir are present
	CDir* dir;
	const TUidType type(KMsvEntryFile, KMsvEntryFile, KNullUid);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(KMsgDir,type , ESortNone, dir));
	TInt count=dir->Count();
	test(count==0) ;
	delete dir;
	
	// check that the local services dir is empty
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(KMsgLocalService,type , ESortNone, dir));
	count=dir->Count();
	test(count==0) ;
	delete dir;
	
	CleanupStack::PopAndDestroy(cEntry);
	}


class RTestBackupSession : public RSessionBase
	{
	public:
		TInt Connect();
		TInt CloseFile(MBackupObserver::TFileLockFlags aFlags) const;
		void GetFileName(TDes& aFileName) const;
		void RestartFile() const;
	};

TInt RTestBackupSession::Connect()
	{
	TInt ret=CreateSession(KBackupServerName,TVersion(0,0,0),5);
	return ret;
	}

TInt RTestBackupSession::CloseFile(MBackupObserver::TFileLockFlags aFlags) const
	{
	TInt p[KMaxMessageArguments];
	p[0]=(TInt)aFlags;
	const TInt err=SendReceive(EBakOpCodeCloseFile,&p[0]);
	return err;
	}

void RTestBackupSession::RestartFile() const
	{
	TInt p[KMaxMessageArguments];
	SendReceive(EBakOpCodeRestartFile,&p[0]);
	}

void RTestBackupSession::GetFileName(TDes& aFileName) const
	{
	TInt p[KMaxMessageArguments];
	TBuf<KMaxFileName+1> buf;
	p[0]=(TInt)(&buf);
	if (SendReceive(EBakGetRegisteredFile,&p[0])!=KErrServerTerminated)
		aFileName=buf;
	}


TInt CreateMessageTest(CMsvEntry &aEntry, TMsvId& aMessageId)
	{	
	TMsvEntry message;
	message.iDetails.Set(KAnAddress);
	message.iDescription.Set(KADescription);
	
	message.iServiceId=KMsvLocalServiceIndexEntryId;
	message.iType=KUidMsvMessageEntry;
	message.iMtm=KUidMsgTypePOP3;
	
	TRequestStatus status;
	CMsvOperation *anop=aEntry.CreateL(message,status);
	CleanupStack::PushL(anop);
	TInt err=RunWhilePending(status);		

	if(err==KErrNone)
		{
		TMsvLocalOperationProgress progress=McliUtils::GetLocalProgressL(*anop);
		test(progress.iError==KErrNone);
		aMessageId=progress.iId;
		}
	
	CleanupStack::PopAndDestroy(anop);
	return(err);
	}



void GetLockL(RTestBackupSession &aBackup,TFileName &aRegistered,MBackupObserver::TFileLockFlags aFlags) 
	{
	aBackup.GetFileName(aRegistered);
	theUtils->WriteComment(aRegistered);
	TBool open;
	test(theUtils->FileSession().IsFileOpen(aRegistered,open)==KErrNone);
	test(open!=EFalse);
	aBackup.CloseFile(aFlags);
	CTestTimer *timer=CTestTimer::NewL();
	timer->After(2000000);
	CActiveScheduler::Start();
	delete timer;
	timer=NULL;
	test(theUtils->FileSession().IsFileOpen(aRegistered,open)==KErrNone);
	test(open==EFalse);
	}


void ReleaseLock(RTestBackupSession &aBackup) 
	{
	aBackup.RestartFile();
	CTestTimer *timer=CTestTimer::NewL();
	timer->After(2000000);
	CActiveScheduler::Start();
	delete timer;
	timer=NULL;
	}	

void ReleaseLockCheckReloaded(RTestBackupSession &aBackup,TFileName &aRegistered) 
	{
	ReleaseLock(aBackup);
	TBool open;
	test(theUtils->FileSession().IsFileOpen(aRegistered,open)==KErrNone);
	test(open!=EFalse);
	}	

LOCAL_C void doTestsL()
	{
	
	CFileMan *fileMan=CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);
	TFileName registered;
	
	CSessionObserver *ob=new (ELeave) CSessionObserver();
	CleanupStack::PushL(ob);

	CMsgsTestUtils::Start(_L("Setting up for the tests"));
	CMsgsTestUtils::Start(_L("Starting backup server"));
	User::LeaveIfError(StartThread());
	RTestBackupSession backup;
	backup.Connect();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Opening message server"));
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Opening CMsvEntry on drafts"));
	CMsvEntry *cEntry=session->GetEntryL(KMsvDraftEntryId);	
	CleanupStack::PushL(cEntry);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Creating message in drafts"));
	TMsvId msgId;
	test(CreateMessageTest(*cEntry,msgId)==KErrNone);
	test(cEntry->Count()==1);
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvDraftEntryId);
	test(cEntry->Count()==1);	
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();
	
	CMsgsTestUtils::Next(_L("backing up the message store"));
	CMsgsTestUtils::Start(_L("Asking for a read lock"));
	GetLock(backup,registered,MBackupObserver::EReleaseLockReadOnly);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Backing up index"));
	test(fileMan->Copy(KMsgDir,KMsgBackupDir,CFileMan::EOverWrite|CFileMan::ERecurse)==KErrNone);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Release read lock"));
	ReleaseLockCheckReloaded(backup,registered);
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();
	
	CMsgsTestUtils::Next(_L("Restoring the a corupt index"));
	CMsgsTestUtils::Start(_L("Asking to get a write lock"));
	GetLock(backup,registered,MBackupObserver::EReleaseLockNoAccess);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Trashing index"));
	CMsgsTestUtils::CorruptIndexFileL(0);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Release a write lock"));
	ReleaseLockCheckReloaded(backup,registered);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Checking we have a new blank message store (drafts empty)"));
	test(cEntry->Count()==0);
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvDraftEntryId);
	test(cEntry->Count()==0);
	TestInitialStructureL(*session);
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();
	

	CMsgsTestUtils::Next(_L("Restoring the backed up index"));
	CMsgsTestUtils::Start(_L("Asking to get a write lock"));
	GetLock(backup,registered,MBackupObserver::EReleaseLockNoAccess);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Restoring index"));
	test(fileMan->Copy(KMsgBackupDir,KMsgDir,CFileMan::EOverWrite|CFileMan::ERecurse)==KErrNone);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Release a write lock"));
	ReleaseLockCheckReloaded(backup,registered);
	CMsgsTestUtils::Complete();	
	
	CMsgsTestUtils::Next(_L("Checking we have the old message store (drafts contains message)"));
	test(cEntry->Count()==1);
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvDraftEntryId);
	test(cEntry->Count()==1);
	TestInitialStructureL(*session);
	cEntry->SetEntryL(msgId);
	test(cEntry->Entry().iDescription==KADescription);
	test(cEntry->Entry().iDetails==KAnAddress);
	cEntry->SetEntryL(KMsvDraftEntryId);
	CleanupStack::PopAndDestroy(cEntry);	
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();
	

	
	CMsgsTestUtils::Next(_L("Checking we get the correct notifications"));
	CMsgsTestUtils::Start(_L("Asking for a read lock - checking we get media unavailable"));
	ob->iCurrent=0;
	GetLock(backup,registered,MBackupObserver::EReleaseLockReadOnly);	
	test(ob->iCurrent==1);
	test(ob->iTheEvents[0].iType==MMsvSessionObserver::EMsvMediaUnavailable);	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Release read lock - checking we get media available"));
	ob->iCurrent=0;	
	ReleaseLockCheckReloaded(backup,registered);
	test(ob->iCurrent==1);
	test(ob->iTheEvents[0].iType==MMsvSessionObserver::EMsvMediaAvailable);	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asking for a write lock - checking we get media unavailable"));
	ob->iCurrent=0;
	GetLock(backup,registered,MBackupObserver::EReleaseLockNoAccess);	
	test(ob->iCurrent=1);
	test(ob->iTheEvents[0].iType==MMsvSessionObserver::EMsvMediaUnavailable);	
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Release write lock - checking we get server ready and media changed"));
	ob->iCurrent=0;
	ReleaseLockCheckReloaded(backup,registered);
	test(ob->iCurrent==2);
	test(ob->iTheEvents[0].iType==MMsvSessionObserver::EMsvServerReady);	
	test(ob->iTheEvents[1].iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iTheEvents[1].iArg1==EDriveC);
	test(ob->iTheEvents[1].iArg2==EDriveC);	
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();
	

	CMsgsTestUtils::Next(_L("Checking the server waits till after a backup has completed before closing"));
	CMsgsTestUtils::Start(_L("Asking for a write lock"));
	GetLock(backup,registered,MBackupObserver::EReleaseLockNoAccess);	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Closing our session, wait 5 secs check the message server hasn't gone"));
	CleanupStack::PopAndDestroy(session);
	User::After(5*1000000);
	test(CMsgsTestUtils::IsServerRunning()!=EFalse);
	CMsgsTestUtils::Complete();	

	CMsgsTestUtils::Next(_L("Release write lock, wait 5 secs check the message server has closed"));
	ReleaseLock(backup);
	User::After(5*1000000);
	test(CMsgsTestUtils::IsServerRunning()==EFalse);	
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();

	
	CMsgsTestUtils::Next(_L("Checking the server stays open if a session is created while backing up"));
	CMsgsTestUtils::Start(_L("Opening a new session"));
	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Start(_L("Asking for a write lock"));
	GetLock(backup,registered,MBackupObserver::EReleaseLockNoAccess);	
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Closing our session, wait 5 secs check the message server hasn't gone"));
	CleanupStack::PopAndDestroy(session);
	User::After(5*1000000);
	test(CMsgsTestUtils::IsServerRunning()!=EFalse);
	CMsgsTestUtils::Complete();	

	CMsgsTestUtils::Next(_L("Opening a new session"));
	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Release write lock, wait 5 secs check the message server hasn't gone"));
	ReleaseLock(backup);
	User::After(5*1000000);
	test(CMsgsTestUtils::IsServerRunning()!=EFalse);	
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::End();

	

	backup.Close();
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(ob);	
	CleanupStack::PopAndDestroy(fileMan);
	CMsgsTestUtils::WaitForServerClose();
	}

