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
//


#include <msvipc.h>
#include <msvreg.h>
#include <msvids.h>
#include <msvuids.h>
#include <mtsr.h>
#include "MSERVER.H"
#include "MCLENTRY.H"
#include "msvutils.h"
#include "Testsetup.h"

#include <mmsvstoremanager.h>

#if !defined(_DEBUG)
#pragma warning( disable : 4710 ) 
#endif
#pragma warning( disable : 4238 ) //nonstandard extension used : class rvalue used as lvalue

RTest test(_L("MSV Server Testrig"));

_LIT8(hello8, "Hello");
_LIT(KDataComponentFileName, "c:\\system\\mtm\\testsmtm.dat");
_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters");
_LIT(KLongDetails, "A very long details string containing rubbish and junk");

_LIT(KMsvDefaultFolder2WithDrive, "C:\\Private\\1000484b\\Mail2\\");
const TUid KUidMsvTestMtm={0x100012C2};
const TUid KUidTestServerMtmType={0x10001304};
const TUid KUidTestServerMtmTech={0x10001304};
const TUid KUidTestServerMtmVersion1={0x10001305};
const TUid KUidMsvHiddenEntryStream={0x10003C73};
_LIT(KTestServerMtmFilename, "z:\\system\\libs\\testservermtm.dll");
_LIT(KThreadPattern, "*::!MsvServer*");

//

class CNullMsvStoreObserver : public CBase, public MMsvStoreObserver
	{
public:
	void HandleStoreEvent(TMsvStoreEvent, TMsvId) {};
	};

class CTestMsvStore : public CMsvStore
	{
public:

	static CMsvStore* OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);
	static CMsvStore* OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId);

private:

	CTestMsvStore(MMsvStoreObserver& aObserver, RFs& aFs, TMsvId aId, MMsvStoreManager& aStoreManager) : CMsvStore(aObserver, aFs, aId, aStoreManager) {};
	};

CMsvStore* CTestMsvStore::OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	return CMsvStore::OpenForReadL(aObserver, aFs, aStoreManager, aId);
	}

CMsvStore* CTestMsvStore::OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	return CMsvStore::OpenForWriteL(aObserver, aFs, aStoreManager, aId);
	}

//

class COperationCompleteActive : public CActive
	{
public:
	COperationCompleteActive();
	~COperationCompleteActive();
	void StartL();
protected:
	void DoCancel();
	void RunL();
	};

COperationCompleteActive::COperationCompleteActive()
: CActive(0)
	{
	CActiveScheduler::Add(this);
	}

COperationCompleteActive::~COperationCompleteActive()
	{
	Cancel();
	}

void COperationCompleteActive::StartL()
	{
	SetActive();
	}

void COperationCompleteActive::RunL() 
	{
	CActiveScheduler::Stop();
	}

void COperationCompleteActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}

//

class CChangeNotification : public CActive
	{
public:
	CChangeNotification();
	void StartL();
protected:
	void DoCancel() {};
	void RunL();
public:
	TMsvNotifBuffer iChange;	
	TPckgBuf<TUint32> iNotifySequence;
private:
	RMsvServerSession iSession;
	};

CChangeNotification::CChangeNotification()
: CActive(0)
	{
	iSession.Connect(theUtils->FileSession());
	CActiveScheduler::Add(this);
	}

void CChangeNotification::StartL()
	{
	iSession.QueueSessionEventRequest(iChange, iNotifySequence, iStatus);
	SetActive();
	}

void CChangeNotification::RunL() 
	{
	CActiveScheduler::Stop();
	}

//

class CCancelTimer : public CTimer
	{
public:
	static CCancelTimer* NewL(RMsvServerSession& aSession);
	~CCancelTimer();
	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest(TMsvOp aId, TInt aWait);
protected:
	CCancelTimer(RMsvServerSession& aSession);
public:
	TBuf8<64> iBuffer;
private:
	RMsvServerSession& iSession;
	TMsvOp iId;
	};

CCancelTimer* CCancelTimer::NewL(RMsvServerSession& aSession)
	{
	CCancelTimer* self = new(ELeave) CCancelTimer(aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CCancelTimer::CCancelTimer(RMsvServerSession& aSession)
: CTimer(-3), iSession(aSession)
	{
	}

CCancelTimer::~CCancelTimer()
	{
	DoCancel();
	}

void CCancelTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add( this );
	}

void CCancelTimer::IssueRequest(TMsvOp aId, TInt aWait)
	{
	iId = aId;
	After(aWait*1000000);
	}

void CCancelTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void CCancelTimer::RunL()
	{
	iSession.CancelOperation(iId, iBuffer);
	}

//

class CProgressTimer : public CTimer
	{
public:
	static CProgressTimer* NewL(RMsvServerSession& aSession);
	~CProgressTimer();
	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest(TMsvOp aId, TInt aWait);
protected:
	CProgressTimer(RMsvServerSession& aSession);
public:
	TBuf8<256> iProgress;
private:
	RMsvServerSession& iSession;
	TMsvOp iId;
	};

CProgressTimer* CProgressTimer::NewL(RMsvServerSession& aSession)
	{
	CProgressTimer* self = new(ELeave) CProgressTimer(aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CProgressTimer::CProgressTimer(RMsvServerSession& aSession)
: CTimer(-3), iSession(aSession)
	{
	}

CProgressTimer::~CProgressTimer()
	{
	DoCancel();
	}

void CProgressTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add( this );
	}

void CProgressTimer::IssueRequest(TMsvOp aId, TInt aWait)
	{
	iId = aId;
	After(aWait*1000000);
	}

void CProgressTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void CProgressTimer::RunL()
	{
	iSession.OperationProgress(iId, iProgress);
	}
	
//

LOCAL_C void CreateEntryL(RMsvServerSession& aSession, TInt aOperationNumber, TTestMsvEntry& aEntry, TInt aError)
	{
	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	TPckgBuf<TMsvLocalOperationProgress> progress;
	
	active->StartL();
	aSession.CreateEntryL(aEntry, aOperationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone);
	// test the operation completion
	REPORT_IF_ERROR(aSession.OperationCompletion(aOperationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalNew);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==aError);
	if (aError)
		{
		test(progress().iNumberCompleted==0);
		test(progress().iNumberFailed==1);
		}
	else
		{
		test(progress().iNumberCompleted==1);
		test(progress().iNumberFailed==0);
		aEntry.SetId(progress().iId);
		if (aEntry.iType==KUidMsvServiceEntry)
			aEntry.iServiceId = progress().iId;
		}

	delete active;
	}


LOCAL_C void GetEntryDirectory(TMsvId aId, TMsvId aService, TFileName& aFileName)
	{
	aFileName = KMsvDefaultFolder2;
	MsvUtils::ConstructEntryName(aService, aId, aFileName, MsvUtils::EFolder);
	}

LOCAL_C void GetStoreFileName(TMsvId aId, TMsvId aService, TFileName& aFileName)
	{
	aFileName = KMsvDefaultFolder2;
	MsvUtils::ConstructEntryName(aService, aId, aFileName, MsvUtils::EStore);
	}


LOCAL_C void TestServerCreationL()
	{
	RMsvServerSession session;

	// starting the server with no index file
	REPORT_IF_ERROR(session.Connect(theUtils->FileSession()));
	session.Close();

	// starting the server with the simplest index file
	REPORT_IF_ERROR(session.Connect(theUtils->FileSession()));
	session.Close();
	}


LOCAL_C void TestThreadRenameL() 
	{
	TBool threadExists = EFalse;
	TFindThread tf1;
	tf1.Find(KThreadPattern); 
	TFullName name2;
	if (tf1.Next(name2)==KErrNone) //check if pattern is found
		{
		threadExists=ETrue; // Thread has been renamed
		}
	test(threadExists);
	}


LOCAL_C void TestServerAsyncOpeningL()
	{
	RMsvServerSession session;
	RMsvServerSession session2;

	// close the session
	session.Connect(theUtils->FileSession());
	session.CloseMessageServer();
	session.Close();
	User::After(100000);

	// notification stuff
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; 
	TRequestStatus status;
	TMsvNotifBuffer change;	
	TPckgBuf<TUint32> sequence;
	TMsvPackedChangeNotification changeNotBuf(change);
	TMsvServerChangeNotificationType changeType;
	TInt parameter,parameter2;

	// start the server
	REPORT_IF_ERROR(session.Connect(theUtils->FileSession()));
	User::After(100000);
	session.QueueSessionEventRequest(change, sequence, status);
	User::WaitForRequest(status);
	// test the notification
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvIndexLoaded);

	// create another session
	REPORT_IF_ERROR(session.Connect(theUtils->FileSession()));
	session.QueueSessionEventRequest(change, sequence, status);
	User::WaitForRequest(status);
	// test the notification
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvIndexLoaded);

	session.Close();
	session2.Close();
	delete obsrSelection;
	}


LOCAL_C void TestMessageDirectoryL()
	{
	RMsvServerSession session;
	TFileName dir;
	REPORT_IF_ERROR(session.Connect(theUtils->FileSession()));
	REPORT_IF_ERROR(session.GetMessageDirectory(dir));
	test(dir==KMsvDefaultFolder2WithDrive);
	session.Close();
	}

LOCAL_C void TestEntryCreationL()
	{
	TTestMsvEntry entry;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iType=KUidMsvServiceEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iDescription.Set(KLongDescription);
	entry.iDetails.Set(KShortDetails);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	
	RMsvServerSession session1;
	session1.Connect(theUtils->FileSession());
	// crate an entry
	CreateEntryL(session1, 1, entry, KErrNone);
	// get the entry back directly
	TTestMsvEntry newEntry;
	TMsvId service;
	REPORT_IF_ERROR(session1.GetEntry(entry.Id(), service, newEntry));
	test(entry==newEntry);
	session1.Close();

	delete active;
	}

LOCAL_C void TestEntryLockingReleasingL()
	{
	TInt error;
	TTestMsvEntry entry;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iMtm = KUidTestServerMtmType;
	entry.iType=KUidMsvServiceEntry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KLongDetails);

	// set up the session
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	// lock a non-existant entry
	error = session.LockEntry(1234);
	test(error==KErrNotFound);

	// release a non-existant entry
	error = session.ReleaseEntry(1234);
	test(error==KErrNotFound);


	// add entry
	CreateEntryL(session, operationNumber++, entry, KErrNone);

	// lock the entry
	REPORT_IF_ERROR(session.LockEntry(entry.Id()));

	// try to lock again
	error = session.LockEntry(entry.Id());
	test(error==KErrLocked);

	// release the entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry.Id()));

	// try to release again **ASSERT thrown in DEBUG**
#if !defined(_DEBUG)
	error = session.ReleaseEntry(entry.Id());
	test(error==KErrNone);
#endif

	session.Close();
	}



LOCAL_C void TestEntryDeletionL()
	{
	// set up the session
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	COperationCompleteActive* delActive = new (ELeave) COperationCompleteActive;
	TPckgBuf<TMsvLocalOperationProgress> progress;

	TMsvOp operationNumber=1;

	CMsvEntrySelection* deleteSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(deleteSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(obsrSelection);

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	//
	// create folder to work under

	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	//
	// deletion of local single entry

	// add the entry
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	

	// try to delete a locked entry
	deleteSelection->AppendL(entry1.Id());
	REPORT_IF_ERROR(session.LockEntry(entry1.Id()));
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;
	deleteSelection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();

	// release the entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry1.Id()));

	// delete the entry
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	deleteSelection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// try to delete entry again (non-existant entry)
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNotFound); // CHANGE  no operation feedback
	deleteSelection->Reset();

	//
	// deletion of local selections

	TTestMsvEntry entry2;
	entry2.SetParent(folder.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);

	TTestMsvEntry entry3;
	entry3.SetParent(folder.Id());
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	
	TTestMsvEntry entry4;
	entry4.SetParent(folder.Id());
	entry4.iType=KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);

	// add the entries
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	CreateEntryL(session, operationNumber++, entry4, KErrNone);

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();

	// try to delete with all locked entries
	deleteSelection->AppendL(entry2.Id());
	deleteSelection->AppendL(entry3.Id());
	deleteSelection->AppendL(entry4.Id());
	REPORT_IF_ERROR(session.LockEntry(entry2.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry3.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry4.Id()));
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==3);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();

	// try to delete with all locked entries and one non-existant
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==4);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==3);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();

	// try to delete with only one unlocked
	deleteSelection->Delete(3);
	REPORT_IF_ERROR(session.ReleaseEntry(entry2.Id()));
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==2);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// delete the remainng two
	deleteSelection->Reset();
	deleteSelection->AppendL(entry3.Id());
	deleteSelection->AppendL(entry4.Id());
	REPORT_IF_ERROR(session.ReleaseEntry(entry4.Id()));
	REPORT_IF_ERROR(session.ReleaseEntry(entry3.Id()));
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// try to delete again
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNotFound); 
	deleteSelection->Reset();

	//
	// recursive deletion of single entry

	// add child to folder
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	// which has a child
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	// which has two children
	entry3.SetParent(entry2.Id());
	entry4.SetParent(entry2.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	CreateEntryL(session, operationNumber++, entry4, KErrNone);

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// lock entry
	REPORT_IF_ERROR(session.LockEntry(entry4.Id()));
	
	// try to delete
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	
	// unlock entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry4.Id()));

	// try to delete
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;


	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	//
	// recursive deletion of selections
	
	// add three children to folder
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	entry2.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	entry3.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// one has a child
	entry4.SetParent(entry2.Id());
	CreateEntryL(session, operationNumber++, entry4, KErrNone);

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	
	// lock entries
	REPORT_IF_ERROR(session.LockEntry(entry4.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry3.Id()));
	
	// try to delete
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	deleteSelection->AppendL(entry2.Id());
	deleteSelection->AppendL(entry3.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==2);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;
	
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	
	// unlock entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry4.Id()));
	REPORT_IF_ERROR(session.ReleaseEntry(entry3.Id()));

	// try to delete
	deleteSelection->Reset();
	deleteSelection->AppendL(entry2.Id());
	deleteSelection->AppendL(entry3.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	//
	// deletion of entry with empty file directory

	TFileName directory;

	// create entry
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	
	// create the directory
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().MkDirAll(directory)==KErrNone);

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test the directory has been removed
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().RmDir(directory)==KErrPathNotFound);
	
	//
	// deletion of entry with file directory contains a closed file

	// create entry
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	
	// create the directory
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().MkDirAll(directory)==KErrNone);

	// create a file
	RFile file;
	TFileName filename;
	file.Temp(theUtils->FileSession(), directory, filename,EFileShareAny|EFileWrite);
	file.Write(hello8);
	file.Close();

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test the directory has been removed
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().RmDir(directory)==KErrPathNotFound);

	//
	// deletion of entry with file directory contains an open file

	// create entry
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	
	// create the directory
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().MkDirAll(directory)==KErrNone);

	// create a file
	file.Temp(theUtils->FileSession(), directory, filename,EFileShareAny|EFileWrite);
	file.Write(hello8);

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrInUse);
	// reset
	operationNumber++;

	// test the directory is there
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().MkDir(directory)==KErrAlreadyExists);

	// close the file
	file.Close();

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test the directory has been removed
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, directory);
	test(theUtils->FileSession().RmDir(directory)==KErrPathNotFound);

	
	//
	// deletion of entry with store locked

	// create entry
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	
	// lock the store
	REPORT_IF_ERROR(session.LockStore(entry1.Id()));

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;

	// unlock the store
	REPORT_IF_ERROR(session.ReleaseStore(entry1.Id()));

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	//
	// deletion of entry with store

	// create entry with a store
	entry1.SetParent(folder.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	delActive->StartL();
	session.ChangeEntryL(entry1, operationNumber, KMsvServerId, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));

	
	// create the store
	GetStoreFileName(entry1.Id(), KMsvLocalServiceIndexEntryId, filename);
	theUtils->FileSession().Delete(filename);
	TInt err=theUtils->FileSession().MkDirAll(filename);
	test(err==KErrNone || err==KErrAlreadyExists);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrNone);
	file.Write(hello8);
	file.Close();

	// test file exists
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);

	// delete the entry
	deleteSelection->Reset();
	deleteSelection->AppendL(entry1.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	// test file has been removed
	test(file.Open(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrPathNotFound);


	//
	// deletion of a service

	// create a service
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// delete the service
	deleteSelection->Reset();
	deleteSelection->AppendL(service.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	
	
	
	session.Close();

	delete delActive;

	CleanupStack::PopAndDestroy(3);
	}



LOCAL_C void TestEntryDeleteAllL()
	{
	// set up the session
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	COperationCompleteActive* delActive = new (ELeave) COperationCompleteActive;
	TPckgBuf<TMsvLocalOperationProgress> progress;

	TMsvOp operationNumber=1;

	CMsvEntrySelection* deleteSelection = new (ELeave) CMsvEntrySelection; 
	CleanupStack::PushL(deleteSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; 
	CleanupStack::PushL(obsrSelection);
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); 
	CleanupStack::PushL(entries);

	//
	// create folder to work under

	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	// add chidlren
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	// read only child
	TTestMsvEntry entry2;
	entry2.SetReadOnly(ETrue);
	entry2.SetParent(folder.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	

	// try to delete read only entry
	deleteSelection->Reset();
	deleteSelection->AppendL(folder.Id());
	delActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, delActive->iStatus);
	CActiveScheduler::Start();
	test(delActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;

	delete delActive;


	CleanupStack::PopAndDestroy(3);
	}
	

LOCAL_C void TestEntryRemoveL()
	{
	// set up the session
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	TMsvOp operationNumber=1;

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	//
	// create folder to work under

	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	//
	// add child
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// remove child
	session.RemoveEntry(entry1.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	/*
	//
	// add child
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// lock child
	REPORT_IF_ERROR(session.LockStore(entry1.Id()));
	// remove child
	session.RemoveEntry(entry1.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock store
	REPORT_IF_ERROR(session.ReleaseStore(entry1.Id()));
	User::After(100000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	//
	// add child and grandchild
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	TTestMsvEntry entry2;
	entry2.SetParent(entry1.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// lock grandchild
	REPORT_IF_ERROR(session.LockStore(entry2.Id()));
	// remove child
	session.RemoveEntry(entry1.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock store
	REPORT_IF_ERROR(session.ReleaseStore(entry2.Id()));
	User::After(100000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	
	//
	// add child and two grandchild
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	TTestMsvEntry entry3;
	entry3.SetParent(entry1.Id());
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// lock first grandchild
	REPORT_IF_ERROR(session.LockStore(entry2.Id()));
	// remove child
	session.RemoveEntry(entry1.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock store
	REPORT_IF_ERROR(session.ReleaseStore(entry2.Id()));
	User::After(100000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	//
	// add child and two grandchild
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	entry3.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// lock second grandchild
	REPORT_IF_ERROR(session.LockStore(entry3.Id()));
	// remove child
	session.RemoveEntry(entry1.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock store
	REPORT_IF_ERROR(session.ReleaseStore(entry3.Id()));
	User::After(100000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	//
	// add child and two grandchild
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	entry3.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// lock second grandchild
	REPORT_IF_ERROR(session.LockStore(entry3.Id()));
	// remove second grandchild
	session.RemoveEntry(entry3.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// remove first grandchild
	session.RemoveEntry(entry2.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock second grandchild
	REPORT_IF_ERROR(session.ReleaseStore(entry3.Id()));
	User::After(200000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	//
	// add two grandchild
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	entry3.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// lock first grandchild
	REPORT_IF_ERROR(session.LockStore(entry2.Id()));
	// remove first grandchild
	session.RemoveEntry(entry2.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// remove second grandchild
	session.RemoveEntry(entry3.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	// unlock first grandchild
	REPORT_IF_ERROR(session.ReleaseStore(entry2.Id()));
	User::After(200000);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	//
	// add child and two grandchild
	entry2.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	entry3.SetParent(entry1.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// lock second grandchild
	REPORT_IF_ERROR(session.LockStore(entry3.Id()));
	// remove second grandchild
	session.RemoveEntry(entry3.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// unlock second grandchild
	REPORT_IF_ERROR(session.ReleaseStore(entry3.Id()));
	// remove first grandchild
	session.RemoveEntry(entry2.Id());
	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
*/

	session.Close();
	CleanupStack::PopAndDestroy(); // entries
	}

	
LOCAL_C void TestEntryChangingL()
	{
	TTestMsvEntry entry;
	entry.SetParent(KMsvLocalServiceIndexEntryId);
	entry.iServiceId = 101;
	entry.iType=KUidMsvFolderEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KLongDetails);

	COperationCompleteActive* changeActive = new (ELeave) COperationCompleteActive;	
	TMsvOp operationNumber=1;
	TPckgBuf<TMsvLocalOperationProgress> progress;

	// set up the session
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// change a non-existant entry
	changeActive->StartL();
	session.ChangeEntryL(entry, operationNumber, KMsvServerId, changeActive->iStatus);
	CActiveScheduler::Start();
	test(changeActive->iStatus==KErrNotFound);

	// add the entry
	CreateEntryL(session, operationNumber++, entry, KErrNone);

	// try to change the entry
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KLongDetails);

	// when locked by someone else
	REPORT_IF_ERROR(session.LockEntry(entry.Id()));
	changeActive->StartL();
	session.ChangeEntryL(entry, operationNumber, KMsvServerId, changeActive->iStatus);
	CActiveScheduler::Start();
	test(changeActive->iStatus==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	test(progress().iId==entry.Id());
	
	// when locked by someone else
	REPORT_IF_ERROR(session.ReleaseEntry(entry.Id()));
	changeActive->StartL();
	session.ChangeEntryL(entry, operationNumber, KMsvServerId, changeActive->iStatus);
	CActiveScheduler::Start();
	test(changeActive->iStatus==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	test(progress().iId==entry.Id());

	// try to change a stanard folder
	TMsvEntry outbox;
	TMsvId service;
	REPORT_IF_ERROR(session.GetEntry(KMsvGlobalOutBoxIndexEntryId, service, outbox));
	TBuf<32> des(outbox.iDescription);
	TBuf<32> det(outbox.iDetails);
	outbox.iDescription.Set(des);
	outbox.iDetails.Set(det);
	session.ChangeEntryL(outbox, operationNumber, KMsvServerId, changeActive->iStatus);
	changeActive->StartL();
	CActiveScheduler::Start();
	test(changeActive->iStatus==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	test(progress().iId==outbox.Id());



	// CHANGE should add more tests
	
	session.Close();
	
	delete changeActive;
	}


LOCAL_C void TestChildrenSelectionL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CArrayPtrFlat<CMsvClientEntry>* entries = new CArrayPtrFlat<CMsvClientEntry>(16);
	CleanupStack::PushL(entries);

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	// create an entry to work under
	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	// get a selection containing a single entry
	TTestMsvEntry entry;
	entry.SetParent(folder.Id());
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iType=KUidMsvFolderEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, entry, KErrNone);
	//
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry()==entry);
	entries->ResetAndDestroy();

	
	// add another entry
	TTestMsvEntry entry2;
	entry2.SetParent(folder.Id());
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType=KUidMsvFolderEntry;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	//
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	if (entries->At(0)->Entry().Id()==entry.Id())
		{
		test(entries->At(0)->Entry()==entry);
		test(entries->At(1)->Entry()==entry2);
		}
	else
		{
		test(entries->At(0)->Entry()==entry2);
		test(entries->At(1)->Entry()==entry);
		}
	entries->ResetAndDestroy();
	//
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();


	
	//
	// change the sort ordering

	// create an entry to work under
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iServiceId = 101;
	folder.iType=KUidMsvFolderEntry;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder, KErrNone);
	
	// create lots of entries
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType = KUidMsvFolderEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = TTime(TDateTime(1999,EApril,1,8,30,0,0));
	entry1.iSize = 0;
	entry1.SetPriority(EMsvHighPriority);
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	entry2.SetParent(folder.Id());
	entry2.iDate = TTime(TDateTime(1998,EJanuary,7,8,30,0,0));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iSize = 100;
	entry2.SetPriority(EMsvHighPriority);
	CreateEntryL(session, operationNumber++, entry2, KErrNone);

	TTestMsvEntry entry3;
	entry3.SetParent(folder.Id());
	entry3.iDate = TTime(TDateTime(2000,EJanuary,1,4,30,0,0));
	entry3.iType = KUidMsvFolderEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.SetPriority(EMsvLowPriority);
	entry3.iSize = 10000;
	CreateEntryL(session, operationNumber++, entry3, KErrNone);

	TTestMsvEntry entry4;
	entry4.SetParent(folder.Id());
	entry4.iDate = TTime(TDateTime(1999,EJanuary,1,8,30,0,0));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iSize = 20000;
	entry4.SetPriority(EMsvLowPriority);
	CreateEntryL(session, operationNumber++, entry4, KErrNone);

	TTestMsvEntry entry5;
	entry5.SetParent(folder.Id());
	entry5.iDate = TTime(TDateTime(1999,EJanuary,1,8,32,0,0));
	entry5.iType = KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	entry5.iSize = 20;
	entry5.SetPriority(EMsvMediumPriority);
	CreateEntryL(session, operationNumber++, entry5, KErrNone);


	TMsvSelectionOrdering ordering;

	
	// sort by date reversed
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, ordering));
	test(entries->Count()==5);
	test(entries->At(4)->Entry().Id()==entry2.Id());
	test(entries->At(3)->Entry().Id()==entry4.Id());
	test(entries->At(2)->Entry().Id()==entry5.Id());
	test(entries->At(1)->Entry().Id()==entry1.Id());
	test(entries->At(0)->Entry().Id()==entry3.Id());
	entries->ResetAndDestroy();

	// group by type - sort by size
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, ordering));
	test(entries->Count()==5);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	test(entries->At(1)->Entry().Id()==entry3.Id());
	test(entries->At(2)->Entry().Id()==entry5.Id());
	test(entries->At(3)->Entry().Id()==entry2.Id());
	test(entries->At(4)->Entry().Id()==entry4.Id());
	entries->ResetAndDestroy();

	// group by priority and type
	ordering.SetSorting(EMsvSortByNone);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, ordering));
	test(entries->Count()==5);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	test(entries->At(1)->Entry().Id()==entry3.Id());
	test(entries->At(2)->Entry().Id()==entry2.Id());
	test(entries->At(3)->Entry().Id()==entry5.Id());
	test(entries->At(4)->Entry().Id()==entry4.Id());
	entries->ResetAndDestroy();

	//
	// get the children of root - check the grouping is ignored :-)
	TMsvSelectionOrdering ordering2;
	// no grouping, no sort
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, ordering2));
	entries->ResetAndDestroy();
	selection->Reset();
	// no grouping, sort
	ordering.SetSorting(EMsvSortByDate);
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, ordering2));
	entries->ResetAndDestroy();
	selection->Reset();
	// no grouping, sort reversed
	ordering.SetSorting(EMsvSortByDateReverse);
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, ordering2));
	entries->ResetAndDestroy();
	// group by type
	ordering2.SetGroupByType(ETrue);
	ordering2.SetGroupByPriority(EFalse);
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, ordering2));
	entries->ResetAndDestroy();
	// group by priority
	ordering2.SetGroupByType(ETrue);
	ordering2.SetGroupByPriority(ETrue);
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, ordering2));
	entries->ResetAndDestroy();


	session.Close();
	CleanupStack::PopAndDestroy(2); // selection, entries
	}

LOCAL_C	void TestChildrenGettingL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CArrayPtrFlat<CMsvClientEntry>* entries = new CArrayPtrFlat<CMsvClientEntry>(16);
	CleanupStack::PushL(entries);

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	// create an entry to work under
	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iDescription.Set(KLongDescription);
	folder.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	TTestMsvEntry entry;
	entry.SetParent(folder.Id());
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iType=KUidMsvFolderEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iDescription.Set(KLongDescription);
	entry.iDetails.Set(KLongDetails);

	TMsvSelectionOrdering ordering;

	// add another child and get all chidren
	for (TInt count=1; count<200; count++)
		{
		if ((count&0x03)==0x03)
			entry.iDescription.Set(KLongDescription);
		else
			entry.iDescription.Set(KShortDescription);
		TBuf<32> number;
		number.Num(count);
		entry.iDetails.Set(number);

		if ((count&0x0f)==0x0f)
			test.Printf(_L("."));

		// ad another child
		CreateEntryL(session, operationNumber++, entry, KErrNone);

		// get all the children
		REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, ordering));

		test(entries->Count()==count);
		TBuf<32> temp;
		for (TInt ii=0; ii<count; ii++)
			{
			// des
			if (((ii+1)&0x03)==0x03)
				test(entries->At(ii)->Entry().iDescription==KLongDescription);
			else
				test(entries->At(ii)->Entry().iDescription==KShortDescription);

			// details
			number.Num(ii+1);
			temp.Copy(entries->At(ii)->Entry().iDetails);
			test(entries->At(ii)->Entry().iDetails==number);
			}

		entries->ResetAndDestroy();
		}
	
	session.Close();

	test.Printf(_L("\n"));

	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C	void TestSW1_98L()
	{
	CArrayPtrFlat<CMsvClientEntry>* entries = new CArrayPtrFlat<CMsvClientEntry>(16);
	CleanupStack::PushL(entries);

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TMsvSelectionOrdering ordering;

	// get a non-existant entry's children
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// we can't have negative TMsvId's, especially after PREQ557
	test(session.GetChildren(1234, *entries, ordering)==KErrNotFound);
#else
	test(session.GetChildren(-1234, *entries, ordering)==KErrNotFound);
#endif
	// get outbox children
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, ordering));

	CleanupStack::PopAndDestroy();
	}



LOCAL_C void TestStoreLockingReleasingL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// get the id of an entry in the index
	TMsvId id = KMsvGlobalInBoxIndexEntryId;

	// LOCKING & RELEASING

	// lock a non-existant store
	test(session.LockStore(1234)==KErrNotFound);

	// release a non-existant store
#if !defined(_DEBUG)
	test(session.ReleaseStore(1234)==KErrNotFound);
#endif

	// lock the store
	REPORT_IF_ERROR(session.LockStore(id));

	// try to lock again
	test(session.LockStore(id)==KErrLocked);

	// release the store
	REPORT_IF_ERROR(session.ReleaseStore(id));

	// try to release again
#if !defined(_DEBUG)
	test(session.ReleaseStore(id)==KErrNone);
#endif

	
	// READING

	// read a non-existant store
	test(session.ReadStore(1234)==KErrNotFound);

	// read an unlocked store
	REPORT_IF_ERROR(session.ReadStore(id));

	// read the store again
	REPORT_IF_ERROR(session.ReadStore(id));

	// read a locked store
	REPORT_IF_ERROR(session.LockStore(id));
	test(session.ReadStore(id)==KErrAccessDenied);
	REPORT_IF_ERROR(session.ReleaseStore(id));

#if !defined(_DEBUG)
	// bad exits
	REPORT_IF_ERROR(session.LockStore(id));
#endif
	
	//
	session.Close(); // this should release the store
	session.Connect(theUtils->FileSession());
	//
	REPORT_IF_ERROR(session.LockStore(id));
	REPORT_IF_ERROR(session.ReleaseStore(id));


	session.Close();
	}

LOCAL_C void TestMovingLocalL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	COperationCompleteActive* moveActive = new (ELeave) COperationCompleteActive;
	CChangeNotification* notifActive = new (ELeave) CChangeNotification;

	TPckgBuf<TMsvLocalOperationProgress> progress;

	TMsvOp operationNumber=1;

	CMsvEntrySelection* moveSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(moveSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(obsrSelection);

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);
	
	// create two folders under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	TTestMsvEntry folder2;
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	// allow notification
	notifActive->StartL();
	CActiveScheduler::Start();
	notifActive->StartL();
	CActiveScheduler::Start();

	// moving to non-existant target
	// CHANGE
//	test(session.MoveLocalEntries(*moveSelection, 12345, package)==KErrNotFound);

	// moving single file
	moveSelection->AppendL(12345);
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder2.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNotFound); // CHANGE no operation produced
	// test the operation completion
/*	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound);
	// reset
	operationNumber++;
*/	moveSelection->Reset();

	// create entry in folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// moving single file
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder2.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();

	// create entry in folder2
	TTestMsvEntry entry2;
	entry2.SetParent(folder2.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// move two entries
	moveSelection->AppendL(entry2.Id());
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder1.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// move one of two entries
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder2.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry2.Id());
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	entries->ResetAndDestroy();

	// create new session
	RMsvServerSession session2;
	session2.Connect(theUtils->FileSession());

	// try to move entry already locked
	REPORT_IF_ERROR(session2.LockEntry(entry1.Id()));
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder1.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// release entry	
	REPORT_IF_ERROR(session2.ReleaseEntry(entry1.Id()));

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry2.Id());
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	entries->ResetAndDestroy();

	// moving entry
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder1.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// lock one of two entries
	REPORT_IF_ERROR(session2.LockEntry(entry1.Id()));

	// try to move two entries (one already locked)
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry2.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder2.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrLocked);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry2.Id());
	entries->ResetAndDestroy();

	// unlock entry
	REPORT_IF_ERROR(session2.ReleaseEntry(entry1.Id()));

	// try to move two entries (first non-existant)
	moveSelection->AppendL(2345);
	moveSelection->AppendL(entry2.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder1.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	// try to move three entries (middle non-existant) 
	moveSelection->AppendL(entry2.Id());
	moveSelection->AppendL(2345);
	moveSelection->AppendL(entry1.Id());
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder2.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound);
	// reset
	operationNumber++;
	moveSelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// try to move three entries (last non-existant) 
	moveSelection->AppendL(entry2.Id());
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(2345);
	moveActive->StartL();
	session.MoveEntriesL(*moveSelection, folder1.Id(), operationNumber, moveActive->iStatus);
	CActiveScheduler::Start();
	test(moveActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalMove);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound);
	// reset
	operationNumber++;
	moveSelection->Reset();


	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	session.Close();
	
	delete moveActive;
	delete notifActive;
	CleanupStack::PopAndDestroy(3);
	}

LOCAL_C void TestLocalL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TMsvOp operationNumber=1;
	TMsvId serviceId;
	TUid mtm;

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	// install the MTM
	TInt error = session.InstallMtmGroup(KDataComponentFileName);
	test(error==KErrNone || error==KErrAlreadyExists);

	// local entries
	session.OperationMtmL(KMsvLocalServiceIndexEntryId, mtm, serviceId);
	test(mtm==KUidMsvLocalServiceMtm);
	test(serviceId==KMsvLocalServiceIndexEntryId);
	session.OperationMtmL(KMsvGlobalInBoxIndexEntryId, mtm, serviceId);
	test(mtm==KUidMsvLocalServiceMtm);
	test(serviceId==KMsvLocalServiceIndexEntryId);
	
	// create a service
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// create entry under service
	TTestMsvEntry entry;
	entry.SetParent(service.Id());
	entry.iType=KUidMsvMessageEntry;
	entry.iServiceId = service.Id();
	entry.iMtm = KUidTestServerMtmType;
	active->StartL();
	session.CreateEntryL(entry, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone);
	// test the operation completion
	TBuf8<32> rProgress;
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, rProgress));
	test(rProgress==_L8("added entry"));

	// remote entries
	session.OperationMtmL(service.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entry.SetId(entries->At(0)->Entry().Id());
	session.OperationMtmL(entry.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	entries->ResetAndDestroy();

	// create an entry under the inbox
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iType=KUidMsvMessageEntry;
	entry2.iServiceId = service.Id();
	entry2.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);

	// two local entries
	entries->ResetAndDestroy();
	session.OperationMtmL(entry2.Id(), KMsvGlobalOutBoxIndexEntryId, mtm, serviceId);
	test(mtm==KUidMsvLocalServiceMtm);
	test(serviceId==KMsvLocalServiceIndexEntryId);
	session.OperationMtmL(KMsvGlobalOutBoxIndexEntryId, entry2.Id(), mtm, serviceId);
	test(mtm==KUidMsvLocalServiceMtm);
	test(serviceId==KMsvLocalServiceIndexEntryId);

	// two remote entries
	session.OperationMtmL(service.Id(), entry.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	session.OperationMtmL(entry.Id(), service.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());

	// two mtm entries - one in local
	session.OperationMtmL(entry2.Id(), service.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	session.OperationMtmL(service.Id(), entry2.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	session.OperationMtmL(entry2.Id(), entry.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	session.OperationMtmL(entry.Id(), entry2.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());

	// one mtm entry one local entry
	session.OperationMtmL(KMsvGlobalOutBoxIndexEntryId, service.Id(), mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());
	session.OperationMtmL(service.Id(), KMsvGlobalOutBoxIndexEntryId, mtm, serviceId);
	test(mtm==KUidTestServerMtmType);
	test(serviceId==service.Id());

	session.Close();

	delete active;
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestNotificationL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	COperationCompleteActive* opActive = new (ELeave) COperationCompleteActive;
	CChangeNotification* active = new (ELeave) CChangeNotification;

	TMsvPackedChangeNotification changeNotBuf(active->iChange);
	TMsvServerChangeNotificationType changeType;
	TInt parameter,parameter2;

	TPckgBuf<TMsvLocalOperationProgress> package;

	CMsvEntrySelection* moveSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(moveSelection);
	CMsvEntrySelection* testSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(testSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(obsrSelection);

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	TInt opId = 1000;

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvIndexLoaded);

	//
	// creating

	// create two folders under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder1, KErrNone);

	TTestMsvEntry folder2;
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	
	
	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==KMsvGlobalInBoxIndexEntryId);
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder1.Id());
	
	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==KMsvGlobalInBoxIndexEntryId);
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder2.Id());
	

	// create entry in folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry1.Id());

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	//
	// moving
	// start the observer for a queued change request 
	active->StartL();
	opActive->StartL();
		
	// move the entry
	moveSelection->AppendL(entry1.Id());
	session.MoveEntriesL(*moveSelection, folder2.Id(), opId, opActive->iStatus);
	moveSelection->Reset();
	entry1.SetParent(folder2.Id());

	// wait for the observer to report and op to complete
	CActiveScheduler::Start();
	CActiveScheduler::Start();

	// complete the operation
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrNone);
	test(package().iId==entry1.Id());

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==folder2.Id());
	test(parameter2==folder1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry1.Id());

	//
	// queued changed information

	// move the entry
	opActive->StartL();
	moveSelection->AppendL(entry1.Id());
	session.MoveEntriesL(*moveSelection, folder1.Id(), opId, opActive->iStatus);
	moveSelection->Reset();
	
	// wait for the observer to report and op to complete
	CActiveScheduler::Start();

	// complete the operation
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	entry1.SetParent(folder1.Id());

	// start the observer for an immediately serviced change request
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==folder1.Id());
	test(parameter2==folder2.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry1.Id());


	//
	// moving two entries

	// create another entry in folder1
	TTestMsvEntry entry2;
	entry2.SetParent(folder1.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);

	// start the observer for an immediately serviced change request
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry2.Id());
	
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// start the observer for a queued change request 
	active->StartL();
	
	// move the entry
	opActive->StartL();
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry2.Id());
	session.MoveEntriesL(*moveSelection, folder2.Id(), opId, opActive->iStatus);
	moveSelection->Reset();
	entry1.SetParent(folder2.Id());
	entry2.SetParent(folder2.Id());
	
	// wait for the op to complete
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	test(package().iId==entry1.Id());

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==folder2.Id());
	test(parameter2==folder1.Id());
	test(obsrSelection->Count()==2);
	test((obsrSelection->At(0)==entry1.Id() && obsrSelection->At(1)==entry2.Id()) ||
		 (obsrSelection->At(0)==entry2.Id() && obsrSelection->At(1)==entry1.Id())    );


	//
	// two different moves

	// move two entry
	opActive->StartL();
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry2.Id());
	session.MoveEntriesL(*moveSelection, folder1.Id(), opId, opActive->iStatus);
	moveSelection->Reset();
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	entry1.SetParent(folder1.Id());
	entry2.SetParent(folder1.Id());

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// move one entry back
	opActive->StartL();
	moveSelection->AppendL(entry2.Id());
	session.MoveEntriesL(*moveSelection, folder2.Id(), opId, opActive->iStatus);
	moveSelection->Reset();
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	entry2.SetParent(folder2.Id());

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	
	// start the observer for first queued change request 
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==folder1.Id());
	test(parameter2==folder2.Id());
	test(obsrSelection->Count()==2);
	test((obsrSelection->At(0)==entry1.Id() && obsrSelection->At(1)==entry2.Id()) ||
		 (obsrSelection->At(0)==entry2.Id() && obsrSelection->At(1)==entry1.Id())    );


	// start the observer for first second change request 
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==folder2.Id());
	test(parameter2==folder1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry2.Id());


	//
	// changing entries

	// change entry2
	entry2.iServiceId = 2345;
	opActive->StartL();
	session.ChangeEntryL(entry2, opId, KMsvServerId, opActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesChanged);
	test(parameter==entry2.Parent());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry2.Id());

	// change folder2
	folder2.iSize = 45;
	opActive->StartL();
	session.ChangeEntryL(folder2, opId, KMsvServerId, opActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesChanged);
	test(parameter==folder2.Parent());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder2.Id());

	//
	// deleting entries
	opActive->StartL();
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry2.Id());
	session.DeleteEntriesL(*moveSelection, opId, opActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	moveSelection->Reset();

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==2);
	test(obsrSelection->At(0)==entry1.Id() && obsrSelection->At(1)==entry2.Id());

	//
	// recursive deletion of selections
	TTestMsvEntry entry3, entry4;
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iType=KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	
	// add three children to folder
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	active->StartL(); CActiveScheduler::Start();
	entry1.SetParent(folder1.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	active->StartL(); CActiveScheduler::Start();
	entry2.SetParent(folder1.Id());
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	active->StartL(); CActiveScheduler::Start();
	entry3.SetParent(folder1.Id());
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	active->StartL(); CActiveScheduler::Start();
	// one has a child
	entry4.SetParent(entry2.Id());
	CreateEntryL(session, operationNumber++, entry4, KErrNone);
	active->StartL(); CActiveScheduler::Start();


	// unlock entry
	REPORT_IF_ERROR(session.LockEntry(entry4.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry3.Id()));

	// try to delete
	opActive->StartL();
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry2.Id());
	moveSelection->AppendL(entry3.Id());
	session.DeleteEntriesL(*moveSelection, opId, opActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	moveSelection->Reset();

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==entry1.Id());

	// unlock entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry4.Id()));
	REPORT_IF_ERROR(session.ReleaseEntry(entry3.Id()));

	// try to delete
	opActive->StartL();
	moveSelection->AppendL(entry2.Id());
	moveSelection->AppendL(entry3.Id());
	session.DeleteEntriesL(*moveSelection, opId, opActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(opId++, package));
	moveSelection->Reset();

	// wait for the oberserver to report
	active->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==3);

	//
	// notifcation of > 28 entries
	TPckgBuf<TMsvLocalOperationProgress> progress;

// CHANGE back when compression is added
//	TInt max = 2*TMsvPackedChangeNotification::KMsvPackedChangeLimit+4;
	TInt max = TMsvPackedChangeNotification::KMsvPackedChangeLimit+1;
	for (TInt count=1; count<max; count++)
		{
		test.Printf(_L("."));
		// create folder to work under
		TTestMsvEntry folder;
		folder.SetParent(KMsvLocalServiceIndexEntryId);
		folder.iType=KUidMsvMessageEntry;
		folder.iMtm = KUidMsvLocalServiceMtm;
		folder.iServiceId = KMsvLocalServiceIndexEntryId;
		CreateEntryL(session, operationNumber++, folder, KErrNone);
		// start the observer for an immediately serviced change request
		active->StartL();
		CActiveScheduler::Start();
		// test observer feedback
		obsrSelection->Reset();
		changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
		test(changeType==EMsvEntriesCreated);
		test(parameter==KMsvLocalServiceIndexEntryId);
		test(obsrSelection->Count()==1);
		test(obsrSelection->At(0)==folder.Id());

		// create all the children
		TTestMsvEntry entry;
		entry.SetParent(folder.Id());
		entry.iType=KUidMsvMessageEntry;
		entry.iMtm = KUidMsvLocalServiceMtm;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		for (TInt ii=0; ii<count; ii++)
			{
			CreateEntryL(session, operationNumber++, entry, KErrNone);
			// start the observer for an immediately serviced change request
			active->StartL();
			CActiveScheduler::Start();
			// test observer feedback
			obsrSelection->Reset();
			changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
			test(changeType==EMsvEntriesCreated);
			test(parameter==folder.Id());
			test(obsrSelection->Count()==1);
			test(obsrSelection->At(0)==entry.Id());
			}
		
		// delete everything
		testSelection->Reset();
		testSelection->AppendL(folder.Id());
		active->StartL();
		opActive->StartL();
		session.DeleteEntriesL(*testSelection, operationNumber++, opActive->iStatus);
		CActiveScheduler::Start();
		CActiveScheduler::Start();
		// complete the operation
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, progress));
		// test observer feedback
		obsrSelection->Reset();
		changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
		test(changeType==EMsvEntriesDeleted);
		if (count<TMsvPackedChangeNotification::KMsvPackedChangeLimit)
			test(obsrSelection->Count()==count+1);
		else
			{
			test(obsrSelection->Count()==TMsvPackedChangeNotification::KMsvPackedChangeLimit);
			active->StartL();
			CActiveScheduler::Start();
			// test observer feedback
			obsrSelection->Reset();
			changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
			test(changeType==EMsvEntriesDeleted);
			if (count<2*TMsvPackedChangeNotification::KMsvPackedChangeLimit)
				test(obsrSelection->Count()==count+1-TMsvPackedChangeNotification::KMsvPackedChangeLimit);
			else
				{
				test(obsrSelection->Count()==TMsvPackedChangeNotification::KMsvPackedChangeLimit);
				active->StartL();
				CActiveScheduler::Start();
				// test observer feedback
				obsrSelection->Reset();
				changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
				test(changeType==EMsvEntriesDeleted);
				test(obsrSelection->Count()==count+1-2*TMsvPackedChangeNotification::KMsvPackedChangeLimit);
				}
			}
		}
	test.Printf(_L("\n"));

	CleanupStack::PopAndDestroy(4); // selections;
	delete active;
	delete opActive;

	session.Close();
	}


LOCAL_C void TestCopyingLocalL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	COperationCompleteActive* copyActive = new (ELeave) COperationCompleteActive;
	CChangeNotification* notifActive = new (ELeave) CChangeNotification;

	TMsvPackedChangeNotification changeNotBuf(notifActive->iChange);
	TMsvServerChangeNotificationType changeType;
	TInt parameter, parameter2;

	TPckgBuf<TMsvLocalOperationProgress> progress;

	TMsvOp operationNumber=1;

	CMsvEntrySelection* copySelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(copySelection);
	CMsvEntrySelection* testSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(testSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(obsrSelection);

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	RFile file;
	TFileName filename;

	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvIndexLoaded);

	// create two folders under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	TTestMsvEntry folder2;
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==KMsvGlobalInBoxIndexEntryId);
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder1.Id());
	notifActive->StartL();
	CActiveScheduler::Start();
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==KMsvGlobalInBoxIndexEntryId);
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder2.Id());


	// create entry under folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
		CreateEntryL(session, operationNumber++, entry1, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// copying to non-existant target // CHANGE
	copySelection->AppendL(12345);
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, 12345, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNotFound); // CHANGE currently the operation is not created

	// copying non-existant first file // CHANGE
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNotFound); // CHANGE currently the operation is not created
	copySelection->Reset();
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	//
	// index entries only
		
	// copying single entry which is locked
	copySelection->AppendL(entry1.Id());
	REPORT_IF_ERROR(session.LockEntry(entry1.Id()));
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	test(progress().iId==KMsvNullIndexEntryId);
	// reset/update
	REPORT_IF_ERROR(session.ReleaseEntry(entry1.Id()));
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	// copying single file
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	TMsvId newId = progress().iId;
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder2.Id());
	test(obsrSelection->Count()==1);
		
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	test(newId==entries->At(0)->Entry().Id());
	entries->ResetAndDestroy();

	// create entry under folder2
	TTestMsvEntry entry2;
	entry2.SetParent(folder2.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// copying two file
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	copySelection->AppendL(entries->At(0)->Entry().Id());
	copySelection->AppendL(entries->At(1)->Entry().Id());
	entries->ResetAndDestroy();
	//
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder1.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder1.Id());
	test(obsrSelection->Count()==2);

	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();


	// copying three files - one does not exist
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	copySelection->AppendL(entries->At(0)->Entry().Id());
	copySelection->AppendL(entries->At(2)->Entry().Id());
	copySelection->AppendL(12345);
	entries->ResetAndDestroy();
	//
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder2.Id());
	test(obsrSelection->Count()==2);

	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==4);
	entries->ResetAndDestroy();

	//
	// index entries and stores

	// create two new folders and entries to work under
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	entry1.SetParent(folder1.Id());
		CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	// copying single entry whose store is locked
	copySelection->AppendL(entry1.Id());
	REPORT_IF_ERROR(session.LockStore(entry1.Id()));
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	test(progress().iId==KMsvNullIndexEntryId);
	// reset/update
	REPORT_IF_ERROR(session.ReleaseStore(entry1.Id()));
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create the store for entry 1
	GetStoreFileName(entry1.Id(), KMsvLocalServiceIndexEntryId, filename);
	theUtils->FileSession().Delete(filename); // could be left from previous running
	TInt err=theUtils->FileSession().MkDirAll(filename);
	test(err==KErrNone || err==KErrAlreadyExists);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrNone);
	file.Write(hello8);
	file.Close();
	// test file exists
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	// mark the index accordingly - normally server/client will do ths
	copyActive->StartL();
	session.ChangeEntryL(entry1, operationNumber, KMsvServerId, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));

	// copying single entry
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	newId = progress().iId;
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test the stores exist and delete the entries
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	testSelection->AppendL(entries->At(0)->Entry().Id());
	GetStoreFileName(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	copyActive->StartL();
	session.DeleteEntriesL(*testSelection, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	entries->ResetAndDestroy();
	testSelection->Reset();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	testSelection->AppendL(entries->At(0)->Entry().Id());
	test(newId==entries->At(0)->Entry().Id());
	GetStoreFileName(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	copyActive->StartL();
	session.DeleteEntriesL(*testSelection, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	entries->ResetAndDestroy();
	testSelection->Reset();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));

	
	//
	// index entries and files

	// create two new folders and entries to work under
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	entry1.SetParent(folder1.Id());
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create the file for entry 1
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, filename);
	test(theUtils->FileSession().MkDirAll(filename)==KErrNone);

	filename.Append(_L("my_file"));
	theUtils->FileSession().Delete(filename);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrNone);
	file.Write(hello8);
	file.Close();
	// test file exists
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);

	// copying single entry
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test the files exist
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	GetEntryDirectory(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("my_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	entries->ResetAndDestroy();
	//
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	GetEntryDirectory(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("my_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	entries->ResetAndDestroy();

	// create the file for entry 1
	GetEntryDirectory(entry1.Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("their_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrNone);
	file.Write(hello8);
	file.Close();
	// test file exists
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);

	// copying single entry
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test the files exist
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	GetEntryDirectory(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("my_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	GetEntryDirectory(entries->At(0)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("their_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	GetEntryDirectory(entries->At(1)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("my_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	GetEntryDirectory(entries->At(1)->Entry().Id(), KMsvLocalServiceIndexEntryId, filename);
	filename.Append(_L("their_file"));
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);
	entries->ResetAndDestroy();
	
	// delete all entries
	CMsvEntrySelection* deleteSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(deleteSelection);
	deleteSelection->AppendL(folder1.Id());
	deleteSelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.DeleteEntriesL(*deleteSelection, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	CleanupStack::PopAndDestroy(); // deleteSelection


	//
	// recursive copy

	// create two folders under local
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iServiceId = 101;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iServiceId = 101;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	notifActive->StartL();
	CActiveScheduler::Start();

	// create entry under folder1
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iServiceId = 101;
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// create attachment under entry
	TTestMsvEntry attach1;
	attach1.SetParent(entry1.Id());
	attach1.iType=KUidMsvAttachmentEntry;
	attach1.iMtm = KUidMsvLocalServiceMtm;
	attach1.iServiceId = KMsvLocalServiceIndexEntryId;
	attach1.iDescription.Set(KLongDescription);
	attach1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber++, attach1, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// copy the entry
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	newId = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(newId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();


	// create another folder under local
	TTestMsvEntry folder3;
	folder3.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder3.iType=KUidMsvFolderEntry;
	folder3.iMtm = KUidMsvLocalServiceMtm;
	folder3.iServiceId = KMsvLocalServiceIndexEntryId;
	folder3.iDescription.Set(KShortDescription);
	folder3.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder3, KErrNone);
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder3.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// copy the two folders into the third
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder3.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	TMsvId t1,t2;
	REPORT_IF_ERROR(session.GetChildren(folder3.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	t1 = entries->At(0)->Entry().Id();
	t2 = entries->At(1)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	t1 = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	t1 = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t2, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	t2 = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t2, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	t2 = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(t2, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	// create another folder under local
	TTestMsvEntry folder4;
	folder4.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder4.iType=KUidMsvFolderEntry;
	folder4.iMtm = KUidMsvLocalServiceMtm;
	folder4.iServiceId = KMsvLocalServiceIndexEntryId;
	folder4.iDescription.Set(KShortDescription);
	folder4.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder4, KErrNone);
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder4.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// lock one of the entries
	REPORT_IF_ERROR(session.LockEntry(entry1.Id()));
	
	// copy the one folders into the fourth
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder4.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder4.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	// create another folder under local
	TTestMsvEntry folder5;
	folder5.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder5.iType=KUidMsvFolderEntry;
	folder5.iMtm = KUidMsvLocalServiceMtm;
	folder5.iServiceId = KMsvLocalServiceIndexEntryId;
	folder5.iDescription.Set(KShortDescription);
	folder5.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder5, KErrNone);
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder5.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// copy the one and two folders into the fifth
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder5.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();
	
	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder5.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();

	// copy the one and two and non-existant folders into the fifth
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(folder2.Id());
	copySelection->AppendL(324344);
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder5.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==2);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound || progress().iError==KErrAccessDenied);
	// reset/update
	progress() = TMsvLocalOperationProgress();
	operationNumber++;
	copySelection->Reset();

	// copy the one and two and non-existant folders into the fifth
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(324344);
	copySelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder5.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==2);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound || progress().iError==KErrAccessDenied);
	// reset/update
	progress() = TMsvLocalOperationProgress();
	operationNumber++;
	copySelection->Reset();

	// copy the one and two and non-existant folders into the fifth
	copySelection->AppendL(324344);
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder5.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==2);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound || progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// lock another entry
	REPORT_IF_ERROR(session.LockEntry(folder2.Id()));

	// copy the one and two and non-existant folders into the fifth
	copySelection->AppendL(324344);
	copySelection->AppendL(folder1.Id());
	copySelection->AppendL(folder2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder5.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==3);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==3);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNotFound || progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// create another entry under folder1
	entry2.SetParent(folder1.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iServiceId = 101;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// create another folder under local
	TTestMsvEntry folder6;
	folder6.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder6.iType=KUidMsvFolderEntry;
	folder6.iMtm = KUidMsvLocalServiceMtm;
	folder6.iServiceId = KMsvLocalServiceIndexEntryId;
	folder6.iDescription.Set(KShortDescription);
	folder6.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder6, KErrNone);


	// copy the two entries
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder6.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	newId = progress().iId ;
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder6.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	// change the lock entry
	REPORT_IF_ERROR(session.LockEntry(entry2.Id()));
	REPORT_IF_ERROR(session.ReleaseEntry(entry1.Id()));

	// copy the two entries
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder6.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder6.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create another entry under folder1
	TTestMsvEntry entry3;
	entry3.SetParent(folder1.Id());
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, entry3, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// change the lock entry
	REPORT_IF_ERROR(session.LockEntry(entry1.Id()));
	REPORT_IF_ERROR(session.ReleaseEntry(entry2.Id()));

	// copy the two entries
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder6.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// change the lock entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry2.Id()));

	// copy the two entries
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder6.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// change the lock entry
	REPORT_IF_ERROR(session.ReleaseEntry(entry2.Id()));
	REPORT_IF_ERROR(session.LockEntry(entry3.Id()));


	// copy the two entries
	copySelection->AppendL(folder1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder6.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==0);
	test(progress().iNumberFailed==1);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrAccessDenied);
	// reset/update
	operationNumber++;
	copySelection->Reset();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder6.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	REPORT_IF_ERROR(session.ReleaseEntry(folder2.Id())); 

	//
	// notification of new entry

	// create two folders under local
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iServiceId = 101;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iServiceId = 101;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(session, operationNumber++, folder2, KErrNone);

	// create entry under folder1
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iServiceId = 101;
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber++, entry1, KErrNone);


	// copy the entry to folder 2
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	test(progress().iId==entries->At(0)->Entry().Id());
	// reset/update
	operationNumber++;
	copySelection->Reset();
	entries->ResetAndDestroy();


	// delete the new entry
	copySelection->AppendL(progress().iId);
	copyActive->StartL();
	session.DeleteEntriesL(*copySelection, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	operationNumber++;
	copySelection->Reset();
	
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	
	// create attachment under entry
	attach1.SetParent(entry1.Id());
	attach1.iType=KUidMsvAttachmentEntry;
	attach1.iMtm = KUidMsvLocalServiceMtm;
	attach1.iServiceId = KMsvLocalServiceIndexEntryId;
	attach1.iDescription.Set(KLongDescription);
	attach1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber++, attach1, KErrNone);

	// copy the entry and attachment to folder 2
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, folder2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	newId = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	test(progress().iId==newId);
	// reset/update
	operationNumber++;
	copySelection->Reset();
	// test the index
	REPORT_IF_ERROR(session.GetChildren(newId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	
	REPORT_IF_ERROR(session.ReleaseEntry(entry3.Id()));


	session.Close();

	delete copyActive;
	delete notifActive;
	CleanupStack::PopAndDestroy(4);
	}


LOCAL_C void TestCopyingLocalIntoDescendantL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	COperationCompleteActive* copyActive = new (ELeave) COperationCompleteActive;
	CChangeNotification* notifActive = new (ELeave) CChangeNotification;

	TMsvPackedChangeNotification changeNotBuf(notifActive->iChange);
	TMsvServerChangeNotificationType changeType;
	TInt parameter, parameter2;

	TPckgBuf<TMsvLocalOperationProgress> progress;

	TMsvOp operationNumber=1;

	CMsvEntrySelection* copySelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(copySelection);
	CMsvEntrySelection* testSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(testSelection);
	CMsvEntrySelection* obsrSelection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(obsrSelection);

	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	TMsvId id1,id2,id3,id4,id5,id6, id7;

	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvIndexLoaded);


	// create a folders under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder1, KErrNone);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==KMsvGlobalInBoxIndexEntryId);
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==folder1.Id());

	// create entries under folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry1, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();
	TTestMsvEntry entry2;
	entry2.SetParent(folder1.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, entry2, KErrNone);
	notifActive->StartL();
	CActiveScheduler::Start();

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();

	// copying single file into itself
	copySelection->Reset();
	copySelection->AppendL(entry1.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, entry1.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	id1 = progress().iId;
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==entry1.Id());
	test(obsrSelection->Count()==1);
	test(obsrSelection->At(0)==id1);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();

	// copying two entries into one of them
	copySelection->Reset();
	copySelection->AppendL(entry1.Id());
	copySelection->AppendL(entry2.Id());
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, entry2.Id(), operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==entry2.Id());
	test(obsrSelection->Count()==2);
	id2 = obsrSelection->At(0);
	id3 = obsrSelection->At(1);
//	id4 = obsrSelection->At(2);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id2, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id3, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	id4 = entries->At(0)->Entry().Id(); entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id4, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();

	// copying two entries into a child of one them
	copySelection->Reset();
	copySelection->AppendL(id2);
	copySelection->AppendL(id3);
	copyActive->StartL();
	session.CopyEntriesL(*copySelection, id4, operationNumber, copyActive->iStatus);
	CActiveScheduler::Start();
	test(copyActive->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(progress().iTotalNumberOfEntries==2);
	test(progress().iNumberCompleted==2);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	// check the notification
	notifActive->StartL();
	CActiveScheduler::Start();
	// test observer feedback
	obsrSelection->Reset();
	changeNotBuf.UnpackL(changeType, *obsrSelection, parameter, parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==id4);
	test(obsrSelection->Count()==2);
	id5 = obsrSelection->At(0);
	id6 = obsrSelection->At(1);
//	id7 = obsrSelection->At(2);

	// test current index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(entry2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id1, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id2, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id3, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id4, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id5, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);	id7 = entries->At(0)->Entry().Id(); entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id6, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(id7, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);	entries->ResetAndDestroy();

	session.Close();
	
	delete copyActive;
	delete notifActive;
	CleanupStack::PopAndDestroy(4);
	}






LOCAL_C void CreateMtmFilesL()
	{
	TInt error = theUtils->FileSession().MkDirAll(KDataComponentFileName);
	test(error==KErrNone || error == KErrAlreadyExists);

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=mtmdllinfo1=CMtmDllInfo::NewL(_L("Test Server Mtm"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KTestServerMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidTestServerMtmType, KUidTestServerMtmTech, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(),KDataComponentFileName,EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent,KUidTestServerMtmVersion1);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);							// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(); // filestore
	
	CleanupStack::PopAndDestroy();  // mtmgroupdata
	}


LOCAL_C void TestNonExistantServerMtmL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	
	TInt operationNumber=1;

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// create the service to work under
	TTestMsvEntry service1;
	service1.SetParent(KMsvRootIndexEntryId);
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidMsvTestMtm;
	CreateEntryL(session, operationNumber++, service1, KErrNone);

	// add folder under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvLocalServiceIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder1, KErrNone);

	// add entry in local
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvTestMtm;
	entry1.iServiceId = service1.Id();
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// copy from local to remote
	selection->AppendL(entry1.Id());
	active->StartL();
	session.CopyEntriesL(*selection, service1.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNotFound);
	operationNumber++;
	selection->Reset();

	TInt command=2;

	// none existant Server Mtm
	selection->AppendL(service1.Id());
	active->StartL();
	session.TransferCommandL(*selection, command, _L8("not a lot"), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNotFound);
	operationNumber++;
	selection->Reset();

	delete active;
	CleanupStack::PopAndDestroy(); // selection
	}


LOCAL_C void TestServerMtmSingleOperationsL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	TBuf8<64> progress;

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	//
	// Test Server Mtm

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// add folder under local
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvLocalServiceIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder1, KErrNone);

	// add entry in local
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service.Id();
	CreateEntryL(session, operationNumber++, entry1, KErrNone);

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// copy from local to remote
	selection->AppendL(entry1.Id());
	active->StartL();
	session.CopyEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress==_L8("copied from local"));
	selection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	TMsvId remoteId = entries->At(0)->Entry().Id();
	entries->ResetAndDestroy();

	// copy from remote to local
	selection->AppendL(remoteId);
	active->StartL();
	session.CopyEntriesL(*selection, folder1.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("copied to local"));
	selection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();

	// copy within remote
	selection->AppendL(remoteId);
	active->StartL();
	session.CopyEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("copied within service"));
	selection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();

	// delete entry
	selection->AppendL(remoteId);
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("deleted entry recursively"));
	selection->Reset();
	
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	remoteId = entries->At(0)->Entry().Id();
	
	// delete recursively
	selection->AppendL(remoteId);
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("deleted entry recursively"));
	selection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==3);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create folder in remote
	TTestMsvEntry folder2;
	folder2.SetParent(service.Id());
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidTestServerMtmType;
	folder2.iServiceId = service.Id();
	active->StartL();
	session.CreateEntryL(folder2, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("added entry"));
	selection->Reset();

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	folder2.SetId(entries->At(0)->Entry().Id());
	entries->ResetAndDestroy();

	// move to local
	selection->AppendL(folder2.Id());
	active->StartL();
	session.MoveEntriesL(*selection, folder1.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("moved to local"));
	selection->Reset();
	folder2.SetParent(folder1.Id());


	// move from local
	selection->AppendL(folder1.Id());
	active->StartL();
	session.MoveEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("moved from local"));
	selection->Reset();
	folder1.SetParent(service.Id());

	// move within service
	selection->AppendL(folder2.Id());
	active->StartL();
	session.MoveEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("moved within service"));
	selection->Reset();
	folder2.SetParent(service.Id());


	// changing an entry
	folder2.iDescription.Set(KLongDescription);
	folder2.SetParent(service.Id());
	folder2.iType=KUidMsvMessageEntry;
	folder2.iMtm = KUidTestServerMtmType;
	folder2.iServiceId = service.Id();
	active->StartL();
	session.ChangeEntryL(folder2, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("changed entry"));
	selection->Reset();


	//
	// start command

	TBuf8<32> param;
	TInt command;

	// actual Server Mtm
	param =  _L8("not a lot");
	command=12;
	selection->AppendL(service.Id());
	active->StartL();
	session.TransferCommandL(*selection, command, param, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("not a lot"));
	selection->Reset();
	
	// cancelling
	CCancelTimer* cancel = CCancelTimer::NewL(session);
	command=10000;
	selection->AppendL(service.Id());
	active->StartL();
	cancel->IssueRequest(operationNumber, 2);
	session.TransferCommandL(*selection, command, param, operationNumber, active->iStatus);
	test.Printf(_L("10000 second wait (cancelled after two)..."));	
	CActiveScheduler::Start();
	test.Printf(_L("cancelled\n"));	
	test(active->iStatus.Int()==KErrCancel);
	test(session.OperationCompletion(operationNumber++, progress)==KErrNotFound);
	selection->Reset();


	CProgressTimer* getProgress = CProgressTimer::NewL(session);	

	// getting progress
	command=2;
	selection->AppendL(service.Id());
	active->StartL();
	getProgress->IssueRequest(operationNumber, 1);
	session.TransferCommandL(*selection, command, param, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone);
	test(getProgress->iProgress == _L8("timer running"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("timer completed"));
	selection->Reset();


	// always fails to start
	command=13;
	selection->AppendL(service.Id());
	active->StartL();
	session.TransferCommandL(*selection, command, param, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==-1300);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	selection->Reset();


	session.Close();

	delete getProgress;
	delete cancel;
	delete active;
	CleanupStack::PopAndDestroy(2); // selection
	}



	
LOCAL_C void TestServerMtmMultipleOperationsL()
	{
	CMsvEntrySelection* selection1 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection1);
	CMsvEntrySelection* selection2 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection2);
	CMsvEntrySelection* selection3 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection3);
	COperationCompleteActive* active1 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active1);
	COperationCompleteActive* active2 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active2);
	COperationCompleteActive* active3 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active3);
	
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	TInt operationNumber=1;

	TBuf8<64> progress;
	TBuf8<64> complete;

	TMsvId serviceId;

	//
	// Test Server Mtm

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// create two entries
	TTestMsvEntry folder1;
	folder1.SetParent(service.Id());
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidTestServerMtmType;
	folder1.iServiceId = service.Id();
	TTestMsvEntry folder2;
	folder2.SetParent(service.Id());
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidTestServerMtmType;
	folder2.iServiceId = service.Id();
	active1->StartL();
	active2->StartL();
	session.CreateEntryL(folder1, operationNumber++, KMsvServerId, active1->iStatus);
	session.CreateEntryL(folder2, operationNumber, KMsvServerId, active2->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("added entry"));
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, complete));
	test(complete== _L8("added entry"));

	// test the index
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	folder1.SetId(entries->At(0)->Entry().Id());
	folder2.SetId(entries->At(1)->Entry().Id());
	entries->ResetAndDestroy();


	// create three entries
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service.Id();
	TTestMsvEntry entry2;
	entry2.SetParent(folder2.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidTestServerMtmType;
	entry2.iServiceId = service.Id();
	TTestMsvEntry entry3;
	entry3.SetParent(folder1.Id());
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidTestServerMtmType;
	entry3.iServiceId = service.Id();
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.CreateEntryL(entry1, operationNumber++, KMsvServerId, active1->iStatus);
	session.CreateEntryL(entry2, operationNumber++, KMsvServerId, active2->iStatus);
	session.CreateEntryL(entry3, operationNumber++, KMsvServerId, active3->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("added entry"));
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("added entry"));
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("added entry"));

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	selection1->AppendL(entries->At(0)->Entry().Id());
	selection3->AppendL(entries->At(1)->Entry().Id());
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	selection2->AppendL(entries->At(0)->Entry().Id());
	entries->ResetAndDestroy();
	
	// check progress of queued operations (delete three entries)
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.DeleteEntriesL(*selection1, operationNumber++, active1->iStatus);
	session.DeleteEntriesL(*selection2, operationNumber++, active2->iStatus);// HERE delete all doesn't work?
	session.DeleteEntriesL(*selection3, operationNumber++, active3->iStatus);
	selection1->Reset();selection2->Reset();selection3->Reset();
	//
	// 1st operation
	CActiveScheduler::Start();
	// check the progress of the operations
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-3, progress));
	test(progress == _L8("deleted entry recursively"));
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotReady);
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// complete 1st entry
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("deleted entry recursively"));
	//
	// 2nd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("deleted entry recursively"));
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// complete 2nd entry
	test(active2->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("deleted entry recursively"));
	//
	// 3rd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("deleted entry recursively"));
	// complete 2nd entry
	test(active3->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("deleted entry recursively"));

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// delete everything in outbox
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	if (entries->Count())
		{
		TInt count=entries->Count();
		while (count--)
			selection1->AppendL(entries->At(count)->Entry().Id());
		active1->StartL();
		session.DeleteEntriesL(*selection1, operationNumber, active1->iStatus);
		CActiveScheduler::Start();
		test(active1->iStatus.Int()==KErrNone); 
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, complete));
		entries->ResetAndDestroy();
		}
	// test
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create two entries under the local folder
	TTestMsvEntry entry4;
	entry4.SetParent(KMsvGlobalOutBoxIndexEntryId);
	entry4.iType=KUidMsvFolderEntry;
	entry4.iMtm = KUidTestServerMtmType;
	entry4.iServiceId = service.Id();
	active1->StartL();
	session.CreateEntryL(entry4, operationNumber, KMsvServerId, active1->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, complete));
	active1->StartL();
	session.CreateEntryL(entry4, operationNumber, KMsvServerId, active1->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, complete));

	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	selection1->AppendL(entries->At(0)->Entry().Id());
	selection2->AppendL(entries->At(1)->Entry().Id());
	entries->ResetAndDestroy();

	// do 2 copies
	active1->StartL();
	active2->StartL();
	session.CopyEntriesL(*selection1, folder1.Id(), operationNumber++, active1->iStatus);
	session.CopyEntriesL(*selection2, folder2.Id(), operationNumber++, active2->iStatus);
	selection1->Reset();selection2->Reset();
	//
	// 1st operation
	CActiveScheduler::Start();
	// check the progress of the operations
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("copied from local"));
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// complete 1st entry
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("copied from local"));
	//
	// 2nd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("copied from local"));
	// test the index
	REPORT_IF_ERROR(session.GetChildren(folder1.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder2.Id(), *entries, TMsvSelectionOrdering()));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	// complete 2nd entry
	test(active2->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("copied from local"));


	// cancelling a command
	CCancelTimer* cancel = CCancelTimer::NewL(session);
	TBuf8<32> param =  _L8("not a lot");
	// actual Server Mtm
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	cancel->IssueRequest(operationNumber, 2);
	session.TransferCommandL(*selection1, 10000, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 2, param, operationNumber++, active2->iStatus);
	//
	// wait for 1st to be cancelled
	test.Printf(_L("10000 second wait (cancelled after two)..."));	
	CActiveScheduler::Start();
	test.Printf(_L("cancelled\n"));	
	test(active1->iStatus.Int()==KErrCancel);
	test(session.OperationCompletion(operationNumber-2, complete)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("timer running"));
	//
	// wait for 2nd to complete
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("timer completed"));
	delete cancel;	

	// create an entry under the service 
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	TInt index = entries->Count();
	entries->ResetAndDestroy();
	TTestMsvEntry entryR;
	entryR.SetParent(service.Id());
	entryR.iType=KUidMsvMessageEntry;
	entryR.iMtm = KUidTestServerMtmType;
	entryR.iServiceId = service.Id();
	active1->StartL();
	session.CreateEntryL(entryR, operationNumber++, KMsvServerId, active1->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("added entry"));
	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering()));
	entryR.SetId(entries->At(index)->Entry().Id());
	entries->ResetAndDestroy();


	// create an entry under local
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalInBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	index = entries->Count();
	entries->ResetAndDestroy();
	TTestMsvEntry entryL;
	entryL.SetParent(KMsvGlobalInBoxIndexEntryId);
	entryL.iType=KUidMsvMessageEntry;
	entryL.iMtm = KUidTestServerMtmType;
	entryL.iServiceId = service.Id();
	active1->StartL();
	session.CreateEntryL(entryL, operationNumber++, KMsvServerId, active1->iStatus);
	
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalInBoxIndexEntryId, *entries, TMsvSelectionOrdering()));
	entryL.SetId(entries->At(index)->Entry().Id());
	entries->ResetAndDestroy();

	// do 3 moves
	selection1->Reset();
	selection1->AppendL(entryR.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.MoveEntriesL(*selection1, service.Id(), operationNumber++, active1->iStatus);
	session.MoveEntriesL(*selection1, KMsvGlobalInBoxIndexEntryId, operationNumber++, active2->iStatus);
	selection1->Reset();
	selection1->AppendL(entryL.Id());
	session.MoveEntriesL(*selection1, service.Id(), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	// 1st operation
	CActiveScheduler::Start();
	// check the progress of the operations
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-3, progress));
	test(progress == _L8("moved within service"));
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotReady);
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// complete 1st
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("moved within service"));
	//
	// 2nd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("moved to local"));
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// complete 1st
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("moved to local"));
	//
	// 3rd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("moved from local"));
	// complete 3rd
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("moved from local"));

	// do 3 changes
	TMsvEntry testEntry;
	REPORT_IF_ERROR(session.GetEntry(folder1.Id(), serviceId, testEntry));
	testEntry.iDescription.Set(TPtrC());
	testEntry.iDetails.Set(TPtrC());
	//
	active1->StartL();
	active2->StartL();
	active3->StartL();
	testEntry.iDescription.Set(_L("change 1"));
	session.ChangeEntryL(testEntry, operationNumber++, KMsvServerId, active1->iStatus);
	testEntry.iDescription.Set(_L("change 2"));
	session.ChangeEntryL(testEntry, operationNumber++, KMsvServerId, active2->iStatus);
	testEntry.iDescription.Set(_L("change 3"));
	session.ChangeEntryL(testEntry, operationNumber++, KMsvServerId, active3->iStatus);
	testEntry.iDescription.Set(TPtrC());
	//
	// 1st operation
	CActiveScheduler::Start();
	// check the progress of the operations
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-3, progress));
	test(progress == _L8("changed entry"));
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotReady);
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// check change
	REPORT_IF_ERROR(session.GetEntry(folder1.Id(), serviceId, testEntry));
	test(testEntry.iDescription==_L("change 1"));
	// complete 1st
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("changed entry"));
	//
	// 2nd operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("changed entry"));
	test(session.OperationProgress(operationNumber-1, progress)==KErrNotReady);
	// check change
	REPORT_IF_ERROR(session.GetEntry(folder1.Id(), serviceId, testEntry));
	test(testEntry.iDescription==_L("change 2"));
	// complete 1st
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("changed entry"));
	//
	// 1st operation
	CActiveScheduler::Start();
	// check the progress of the operations
	test(session.OperationProgress(operationNumber-3, progress)==KErrNotFound);
	test(session.OperationProgress(operationNumber-2, progress)==KErrNotFound);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("changed entry"));
	// check change
	REPORT_IF_ERROR(session.GetEntry(folder1.Id(), serviceId, testEntry));
	test(testEntry.iDescription==_L("change 3"));
	// complete 1st
	test(active1->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("changed entry"));

	//
	// test persistent MTMs	- when another command is queued 
	param =  _L8("my buffer");
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	session.TransferCommandL(*selection1, 10, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 11, TPtrC8(), operationNumber++, active2->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("persistent"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("persistent"));
	//
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("my buffer"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("my buffer"));
	
	//
	// test persistent MTMs	- when no other commands are queued 
	selection1->AppendL(service.Id());
	param =  _L8("YUK YUK YUK");
	active1->StartL();
	session.TransferCommandL(*selection1, 10, param, operationNumber++, active1->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("persistent"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("persistent"));
	//
	// do another command
	selection1->AppendL(service.Id());
	param = TPtrC8();
	active1->StartL();
	session.TransferCommandL(*selection1, 11, TPtrC8(), operationNumber++, active1->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("YUK YUK YUK"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("YUK YUK YUK"));

	//
	// cancelling running operation with queued ops
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	session.CancelOperation(operationNumber-3, complete);
	test(complete== _L8("one")); // had already completed
	test(session.OperationCompletion(operationNumber-3, complete)==KErrNotFound);
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("two"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));

	//
	// cancelling running operation with queued ops
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.TransferCommandL(*selection1, 2,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	session.CancelOperation(operationNumber-3, complete);
	test(complete== _L8("cancelled"));
	test(session.OperationCompletion(operationNumber-3, complete)==KErrNotFound);
	CActiveScheduler::Start(); // allow cancel to be propogated
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("two"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));

	//
	// cancelling queued operation
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	session.CancelOperation(operationNumber-2, complete);
	test(complete== _L8(""));
	test(session.OperationCompletion(operationNumber-2, complete)==KErrNotFound);
	//
	CActiveScheduler::Start(); // allow cancel to be propogated
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("one"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));

	//
	// cancelling queued operation
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	session.CancelOperation(operationNumber-1, complete);
	test(complete== _L8(""));
	test(session.OperationCompletion(operationNumber-1, complete)==KErrNotFound);
	CActiveScheduler::Start(); // allow cancel to be propogated
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("one"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("two"));


	//
	// cancelling queued operation with multiple sessions the operationNumber might be the same

	RMsvServerSession session1;
	session1.Connect(theUtils->FileSession());
	RMsvServerSession session2;
	session2.Connect(theUtils->FileSession());
	RMsvServerSession session3;
	session3.Connect(theUtils->FileSession());
	
	
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session1.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber, active1->iStatus);
	session2.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber, active2->iStatus);
	session3.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	session3.CancelOperation(operationNumber-1, complete);
	test(complete== _L8(""));
	test(session3.OperationCompletion(operationNumber-1, complete)==KErrNotFound);
	CActiveScheduler::Start(); // allow cancel to be propogated
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("one"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("two"));
	

	//
	// cancelling queued operation with multiple sessions the operationNumber might be the same
	//
	

	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session1.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber, active1->iStatus);
	session2.TransferCommandL(*selection1, 12,  _L8("two"), operationNumber, active2->iStatus);
	session3.TransferCommandL(*selection1, 12,  _L8("three"), operationNumber++, active3->iStatus);
	selection1->Reset();
	//
	CActiveScheduler::Start();
	session2.CancelOperation(operationNumber-1, complete);
	test(complete== _L8(""));
	test(session2.OperationCompletion(operationNumber-1, complete)==KErrNotFound);
	//
	CActiveScheduler::Start(); // allow cancel to be propogated
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("one"));
	//
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));
	
	session1.Close();
	session2.Close();
	session3.Close();
	


	// second operation fails to start
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	session.TransferCommandL(*selection1, 10, _L8(""), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 13, _L8(""), operationNumber++, active2->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==-1300);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	selection1->Reset();

	// second operation fails to start
	selection1->AppendL(service.Id());
	active1->StartL();
	active2->StartL();
	active3->StartL();
	session.TransferCommandL(*selection1, 10, _L8(""), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection1, 13, _L8(""), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection1, 13, _L8(""), operationNumber++, active3->iStatus);
	CActiveScheduler::Start();
	test(active1->iStatus.Int()==KErrNone);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==-1300);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==-1300);
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	selection1->Reset();

	session.Close();
	CleanupStack::PopAndDestroy(7); // selection, active, nullActive
	}

/*
LOCAL_C void TestServerMtmOutstandingOperationsL()
	{
	CMsvEntrySelection* selection1 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection1);
	CMsvEntrySelection* selection2 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection2);
	CMsvEntrySelection* selection3 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection3);
	COperationCompleteActive* active1 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active1);
	COperationCompleteActive* active2 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active2);
	COperationCompleteActive* active3 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active3);

	TBuf8<64> complete;
	TInt operationNumber=1;
	
	RMsvServerSession session;
	RMsvServerSession session1;
	session.Connect(theUtils->FileSession());

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	selection1->AppendL(service.Id());

	//
	// have outstanding completed operation when exiting
#if !defined(_DEBUG)
	session1.Connect(theUtils->FileSession());
	//
	active1->StartL();
	session1.TransferCommandL(*selection1, 12, TPtrC8(), operationNumber++, active1->iStatus);
	CActiveScheduler::Start();
	//
	session1.Close();
#endif
	
	//
	// have outstanding completed and queued operations when exiting
#if !defined(_DEBUG)
	session1.Connect(theUtils->FileSession());
	//
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection1, 12, TPtrC8(), operationNumber++, active1->iStatus);
	session1.TransferCommandL(*selection1, 12, TPtrC8(), operationNumber++, active2->iStatus);
	CActiveScheduler::Start();
	//
	session1.Close();
	//
	CActiveScheduler::Start(); // Complete active2
#endif

	//
	// have outstanding completed operation with a persisted mtm
#if !defined(_DEBUG)
	session1.Connect(theUtils->FileSession());
	//
	active1->StartL();
	session1.TransferCommandL(*selection1, 10, TPtrC8(), operationNumber++, active1->iStatus);
	CActiveScheduler::Start();
	//
	session1.Close();
#endif

	//
	// have outstanding completed and queued operation with a persisted mtm
#if !defined(_DEBUG)
	session1.Connect(theUtils->FileSession());
	//
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection1, 10, TPtrC8(), operationNumber++, active1->iStatus);
	session1.TransferCommandL(*selection1, 10, TPtrC8(), operationNumber++, active2->iStatus);
	CActiveScheduler::Start();
	//
	session1.Close();
	//
	CActiveScheduler::Start(); // Complete active2
#endif

	//
	// have a persisted mtm
	session1.Connect(theUtils->FileSession());
	//
	active1->StartL();
	session1.TransferCommandL(*selection1, 10, TPtrC8(), operationNumber++, active1->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber-1, complete));
	//
	session1.Close();
	
	session.Close();

	
	CleanupStack::PopAndDestroy(6); 
	}
*/


LOCAL_C void TestServerMtmMultipleMtms()
	{
	CMsvEntrySelection* selection1 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection1);
	CMsvEntrySelection* selection2 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection2);
	CMsvEntrySelection* selection3 = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection3);
	COperationCompleteActive* active1 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active1);
	COperationCompleteActive* active2 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active2);
	COperationCompleteActive* active3 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active3);

	TBuf8<64> progress;
	TBuf8<64> complete;
	TInt operationNumber=1;
	
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// create the service to work under
	TTestMsvEntry service1;
	service1.SetParent(KMsvRootIndexEntryId);
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service1, KErrNone);
	TTestMsvEntry service2;
	service2.SetParent(KMsvRootIndexEntryId);
	service2.iType=KUidMsvServiceEntry;
	service2.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service2, KErrNone);
	TTestMsvEntry service3;
	service3.SetParent(KMsvRootIndexEntryId);
	service3.iType=KUidMsvServiceEntry;
	service3.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service3, KErrNone);

	selection1->AppendL(service1.Id());
	selection2->AppendL(service2.Id());
	selection3->AppendL(service3.Id());

	TBufC8<32> param =  _L8("YUK YUK YUK");


	// start three different waits
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 2, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 2, param, operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 2, param, operationNumber++, active3->iStatus);

	//  get the progress
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-3, progress));
	test(progress == _L8("timer running"));
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("timer running"));
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-1, progress));
	test(progress == _L8("timer running"));

	// cancel them
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-1, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-2, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-3, complete));
	test(complete== _L8("cancelled"));
	CActiveScheduler::Start();CActiveScheduler::Start();CActiveScheduler::Start();

	// start three different waits and cancel in different ways
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 2, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 2, param, operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 2, param, operationNumber++, active3->iStatus);
	// cancel them
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-2, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-3, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-1, complete));
	test(complete== _L8("cancelled"));
	CActiveScheduler::Start();CActiveScheduler::Start();CActiveScheduler::Start();

	// start three different waits and cancel in different ways
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 2, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 2, param, operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 2, param, operationNumber++, active3->iStatus);
	// cancel them
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-3, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-2, complete));
	test(complete== _L8("cancelled"));
	REPORT_IF_ERROR(session.CancelOperation(operationNumber-1, complete));
	test(complete== _L8("cancelled"));
	CActiveScheduler::Start();CActiveScheduler::Start();CActiveScheduler::Start();

	// start three different waits and let them complete
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 2, param, operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 3, param, operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 2, param, operationNumber++, active3->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("timer completed"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("timer completed"));	
	REPORT_IF_ERROR(session.OperationProgress(operationNumber-2, progress));
	test(progress == _L8("timer running"));	
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("timer completed"));	
	
	// three mtm only one is persisted
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 10,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 12,  _L8("three"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("one"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("persistent"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));	
	
	// three mtm closed persisted one
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("ein"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 11,  _L8("should not return"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 12,  _L8("drei"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("ein"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("two"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("drei"));	

	// three mtm only one is persisted
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("one"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 12,  _L8("two"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 10,  _L8("three"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("one"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("two"));
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("persistent"));	
	
	// three mtm switch persisted one
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 12,  _L8("ein"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 10,  _L8("zwei"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 11,  _L8("drei"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("ein"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("persistent"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("three"));	

	// three mtm switch persisted one
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 10,  _L8("onan"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 11,  _L8("dew"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 12,  _L8("tri"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("persistent"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("zwei"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("tri"));	

	// three mtm add another persisted
	active1->StartL();active2->StartL();active3->StartL();
	session.TransferCommandL(*selection1, 10,  _L8("une"), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection2, 12,  _L8("deux"), operationNumber++, active2->iStatus);
	session.TransferCommandL(*selection3, 10,  _L8("tois"), operationNumber++, active3->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); CActiveScheduler::Start();
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-3, complete));
	test(complete== _L8("persistent"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("deux"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("persistent"));	

	// two mtms close persisted
	active1->StartL();active2->StartL();
	session.TransferCommandL(*selection1, 11,  _L8(""), operationNumber++, active1->iStatus);
	session.TransferCommandL(*selection3, 11,  _L8("ya"), operationNumber++, active2->iStatus);
	CActiveScheduler::Start(); CActiveScheduler::Start(); 
	// completed
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-2, complete));
	test(complete== _L8("une"));	
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber-1, complete));
	test(complete== _L8("tois"));	
	
	session.Close();
	CleanupStack::PopAndDestroy(6);
 	}

LOCAL_C void TestServerMtmMutipleClients()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active1 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active1);
	COperationCompleteActive* active2 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active2);
	COperationCompleteActive* active3 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active3);

	TBuf8<64> progress;
	TBuf8<64> complete;
	TInt operationNumber1=9;
	TInt operationNumber2=20;
	TInt operationNumber3=30;

	// connect three sessions
	RMsvServerSession session1;
	session1.Connect(theUtils->FileSession());
	RMsvServerSession session2;
	session2.Connect(theUtils->FileSession());
	RMsvServerSession session3;
	session3.Connect(theUtils->FileSession());

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session1, operationNumber1++, service, KErrNone);

	selection->AppendL(service.Id());

	// queue different operations - mtm NOT persisted
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 12,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 12,  _L8("three"), operationNumber3++, active3->iStatus);
	// allow one op to complete
	CActiveScheduler::Start();
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("one"));
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotReady);
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("one"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// allow two op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("three"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("three"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("three"));	


	// queue different operations - mtm persisted
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// allow one op to complete
	CActiveScheduler::Start();
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotReady);
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("persistent"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// allow two op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("one"));	


	// queue different operations - close first session
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// close session (but operation will have been completed anyway)
	CActiveScheduler::Start();
//#if defined(_DEBUG)
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
//#endif
	session1.Close();
	// progress
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// allow two op to complete
	CActiveScheduler::Start();
	// progress
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	
	// progress
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("one"));	
	// reconnect
	session1.Connect(theUtils->FileSession());

	// queue different operations - close second session
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// close session 
//#if defined(_DEBUG)
	REPORT_IF_ERROR(session2.CancelOperation(operationNumber2-1, complete));
//#endif
	session2.Close();
//#if defined(_DEBUG)
//	active2->Cancel();
	CActiveScheduler::Start();
//#else
//	active2->Cancel();
//	CActiveScheduler::Start();
//#endif
	// allow first operation to complete
	CActiveScheduler::Start();	
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("persistent"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("one"));	
	// reconnect
	session2.Connect(theUtils->FileSession());

	
	// queue different operations - close third session
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// close session 
//#if defined(_DEBUG)
	REPORT_IF_ERROR(session3.CancelOperation(operationNumber3-1, complete));
//#endif
	session3.Close();
//#if defined(_DEBUG)
//	active3->Cancel();
	CActiveScheduler::Start();
//#else
//	active3->Cancel();
//	CActiveScheduler::Start();
//#endif
	// allow first operation to complete
	CActiveScheduler::Start();	
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("persistent"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	
	// reconnect
	session3.Connect(theUtils->FileSession());

	
	// queue different operations - cancel first op
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// cancel first op 
	session1.CancelOperation(operationNumber1-1, complete);
	CActiveScheduler::Start();
	test(complete== _L8("persistent")); // CHANGE operation has already completed?
	// progress
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// allow two op to complete
	CActiveScheduler::Start();
	// progress
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	
	// progress
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("one"));	

	// queue different operations - cancel second session
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// cancel second op 
	session2.CancelOperation(operationNumber2-1, complete);
	test(complete== _L8(""));
	// allow first operation to complete and secodn to be cancelled
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(active2->iStatus.Int()==KErrCancel);
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session3.OperationProgress(operationNumber3-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("persistent"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session3.OperationProgress(operationNumber3-1, progress));
	test(progress == _L8("one"));
	// complete
	REPORT_IF_ERROR(session3.OperationCompletion(operationNumber3-1, complete));
	test(complete== _L8("one"));	

	
	// queue different operations - cancel third session
	active1->StartL();active2->StartL();active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 12,  _L8("two"), operationNumber2++, active2->iStatus);
	session3.TransferCommandL(*selection, 11,  _L8("three"), operationNumber3++, active3->iStatus);
	// cancel second op 
	session3.CancelOperation(operationNumber3-1, complete);
	test(complete== _L8(""));
	// allow first operation to complete
	CActiveScheduler::Start();	
	CActiveScheduler::Start();	
	test(active3->iStatus.Int()==KErrCancel);
	// progress
	REPORT_IF_ERROR(session1.OperationProgress(operationNumber1-1, progress));
	test(progress == _L8("persistent"));
	test(session2.OperationProgress(operationNumber2-1, progress)==KErrNotReady);
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, complete));
	test(complete== _L8("persistent"));	
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	// allow three op to complete
	CActiveScheduler::Start();
	// progress
	test(session1.OperationProgress(operationNumber1-1, progress)==KErrNotFound);
	REPORT_IF_ERROR(session2.OperationProgress(operationNumber2-1, progress));
	test(progress == _L8("two"));
	// complete
	REPORT_IF_ERROR(session2.OperationCompletion(operationNumber2-1, complete));
	test(complete== _L8("two"));	

	

	
	session1.Close();
	session2.Close();
	session3.Close();
	CleanupStack::PopAndDestroy(4); 
	}



LOCAL_C void TestServiceCommandsL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active1 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active1);
	COperationCompleteActive* active2 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active2);
	COperationCompleteActive* active3 = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active3);
	TInt operationNumber1=9;

	// connect three sessions
	RMsvServerSession session1;
	session1.Connect(theUtils->FileSession());
	RMsvServerSession session2;
	session2.Connect(theUtils->FileSession());

	TBuf8<256> progress;

	// non-existant service
	test(session1.StopService(-12345)==KErrNotFound);
	test(session1.ServiceActive(-12345)==EFalse);
	test(session1.ServiceProgress(-12345, progress)==KErrNotFound);

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session1, operationNumber1++, service, KErrNone);
	selection->AppendL(service.Id());

	//
	// persisted (1) server mtm with no current operation (stoppped (1))
	active1->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("persistent"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("persistent"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);

	//
	// persisted (1) server mtm with no current operation (stoppped (2))
	active1->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("one"), operationNumber1++, active1->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-1, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("persistent"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("persistent"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);


	//
	// server mtm with current operation (1) (not persisted) (stoppped (1))
	active1->StartL();
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active1->iStatus);
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	test(active1->iStatus==KErrCancel);

	//
	// server mtm with current operation (1) (not persisted) (stoppped (2))
	active1->StartL();
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active1->iStatus);
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	test(active1->iStatus==KErrCancel);


	//
	// server mtm with current operation (1) (persisted (1)) (stoppped (1))
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("dsfbids"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-2, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	test(active2->iStatus==KErrCancel);

	
	//
	// server mtm with current operation (1) (persisted (1)) (stoppped (2))
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("dsfbids"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-2, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	test(active2->iStatus==KErrCancel);


	//
	// server mtm with current(1) & queued(1) operation (not persisted) (stoppped (1))
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	CActiveScheduler::Start(); // allow active2 to be completed
	test(active1->iStatus==KErrCancel);
	test(active2->iStatus==KErrCancel);

	//
	// server mtm with current(1) & queued(1) operation (not persisted) (stoppped (2))
	active1->StartL();
	active2->StartL();
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active1 to be completed
	CActiveScheduler::Start(); // allow active2 to be completed
	test(active1->iStatus==KErrCancel);
	test(active2->iStatus==KErrCancel);


	//
	// server mtm with current(1) & queued(1) operation (persisted(1)) (stoppped (1))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);

	//
	// server mtm with current(1) & queued(1) operation (persisted(1)) (stoppped (2))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);

	//
	// server mtm with current(2) & queued(1) operation (persisted(1)) (stoppped (1))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);

	//
	// server mtm with current(2) & queued(1) operation (persisted(1)) (stoppped (2))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session2.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);
	
	//
	// server mtm with current(1) & queued(2) operation (persisted(1)) (stoppped (1))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session2.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session1.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);

	//
	// server mtm with current(1) & queued(2) operation (persisted(1)) (stoppped (2))
	active1->StartL(); active2->StartL(); active3->StartL();
	session1.TransferCommandL(*selection, 10,  _L8("zvfxcv"), operationNumber1++, active1->iStatus);
	session1.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active2->iStatus);
	session2.TransferCommandL(*selection, 10000,  _L8("never returned"), operationNumber1++, active3->iStatus);
	CActiveScheduler::Start();
	// complete
	REPORT_IF_ERROR(session1.OperationCompletion(operationNumber1-3, progress));
	test(progress== _L8("persistent")); progress=TPtrC8();
	// test service
	test(session1.ServiceActive(service.Id()));
	test(session2.ServiceActive(service.Id()));
	test(session1.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	test(session2.ServiceProgress(service.Id(), progress)==KErrNone);
	test(progress== _L8("timer running"));	progress=TPtrC8();
	// stop the service
	test(session2.StopService(service.Id())==KErrNone);
	// test the service
	test(session1.ServiceActive(service.Id())==EFalse);
	test(session2.ServiceActive(service.Id())==EFalse);
	test(session1.ServiceProgress(service.Id(), progress)==KErrNotFound);
	test(session2.ServiceProgress(service.Id(), progress)==KErrNotFound);
	// test the operation
	CActiveScheduler::Start(); // allow active2 to be completed
	CActiveScheduler::Start(); // allow active3 to be completed
	test(active2->iStatus==KErrCancel);
	test(active3->iStatus==KErrCancel);
	
	session1.Close();
	session2.Close();

	CleanupStack::PopAndDestroy(4); // selection, active1, active2, active3
	}
	

LOCAL_C void TestRegistryL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	CRegisteredMtmDllArray* registeredmtmdllarray=new(ELeave) CRegisteredMtmDllArray();
	CleanupStack::PushL(registeredmtmdllarray);

	User::LeaveIfError(session.FillRegisteredMtmDllArray(KUidMtmClientComponent,*registeredmtmdllarray,KMsvDefaultTimeoutMicroSeconds32));
//	TInt count=registeredmtmdllarray->Count();
	CleanupStack::PopAndDestroy();

	TInt ret=session.DeInstallMtmGroup(KDataComponentFileName);
	test(ret==KErrNone || ret==KErrNotFound);

	ret=session.InstallMtmGroup(KDataComponentFileName);
	test(ret==KErrNone);

	if (ret==KErrNone)
		{
		CMtmGroupData* mtmgroupdata=session.GetMtmGroupDataL(KUidTestServerMtmType);
		CleanupStack::PushL(mtmgroupdata);
		CleanupStack::PopAndDestroy();
		}

	test(session.DeInstallMtmGroup(KDataComponentFileName)==KErrNone);

	session.Close();
	}


LOCAL_C void TestPcSyncL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active);
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);
	
	TInt operationNumber=1;
	TPckgBuf<TMsvLocalOperationProgress> progress;
	TMsvEntry entryD;
	TMsvId service;	

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// need to remove any entries from the deleted folder
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	TInt count=entries->Count();
	while (count--)
		{
		entryD = entries->At(count)->Entry();
		TInt tt=entryD.PcSyncCount();
		while (tt--)
			entryD.DecPcSyncCount();
		active->StartL();
		session.ChangeEntryL(entryD, operationNumber, KMsvServerId, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus.Int()==KErrNone); 
		// test the operation completion
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
		// reset
		operationNumber++;
		selection->Reset();
		}
	entries->ResetAndDestroy();

	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create the folder to work under
	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	// create entry without sync
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber, entry1, KErrNone);

	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// delete entry without sync
	selection->AppendL(entry1.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// delete entry with sync
	entry1.IncPcSyncCount();
	CreateEntryL(session, operationNumber, entry1, KErrNone);
	selection->AppendL(entry1.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry1.Id());
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetEntry(entry1.Id(), service, entryD));
	test(!entryD.Visible());
	test(entryD.Deleted());
	test(entryD.iDescription==TPtrC());
	test(entryD.iDetails==TPtrC());

	// change/remove deleted entry
	entryD.DecPcSyncCount();
	active->StartL();
	session.ChangeEntryL(entryD, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();


	// create entry and child - child sync'd
	entry1.DecPcSyncCount();
	CreateEntryL(session, operationNumber, entry1, KErrNone);
	TTestMsvEntry entry2;
	entry2.SetParent(entry1.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.IncPcSyncCount();
	CreateEntryL(session, operationNumber, entry2, KErrNone);

	// delete the entries
	selection->AppendL(entry1.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	test(entries->At(0)->Entry().Id()==entry2.Id());
	entries->ResetAndDestroy();

	// change/remove deleted entry
	REPORT_IF_ERROR(session.GetEntry(entry2.Id(), service, entryD));
	entryD.DecPcSyncCount();
	active->StartL();
	session.ChangeEntryL(entryD, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();



	session.Close();

	CleanupStack::PopAndDestroy(3); // selection, active1, entries
	}


LOCAL_C void TestPcSyncDeletingServiceL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active);
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);
	
	TInt operationNumber=1;
	TPckgBuf<TMsvLocalOperationProgress> progress;
	TMsvEntry entryD;

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// need to remove any entries from the deleted folder
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	TInt count=entries->Count();
	while (count--)
		{
		entryD = entries->At(count)->Entry();
		TInt tt=entryD.PcSyncCount();
		while (tt--)
			entryD.DecPcSyncCount();
		active->StartL();
		session.ChangeEntryL(entryD, operationNumber, KMsvServerId, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus.Int()==KErrNone); 
		// test the operation completion
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
		// reset
		operationNumber++;
		selection->Reset();
		}
	entries->ResetAndDestroy();

	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// create entry without sync
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvLocalServiceIndexEntryId);
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service.Id();
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber, entry1, KErrNone);
	// copy from local to remote
	selection->AppendL(entry1.Id());
	active->StartL();
	session.CopyEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("copied from local"));
	selection->Reset();

	// create entry with sync
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvLocalServiceIndexEntryId);
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidTestServerMtmType;
	entry2.iServiceId = service.Id();
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.IncPcSyncCount();
	CreateEntryL(session, operationNumber, entry2, KErrNone);
	// copy from local to remote
	selection->AppendL(entry2.Id());
	active->StartL();
	session.CopyEntriesL(*selection, service.Id(), operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber++, progress));
	test(progress== _L8("copied from local"));
	selection->Reset();

	REPORT_IF_ERROR(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==2);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// delete entry without sync
	selection->AppendL(service.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
	test(progress().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(progress().iTotalNumberOfEntries==1);
	test(progress().iNumberCompleted==1);
	test(progress().iNumberFailed==0);
	test(progress().iNumberRemaining==0);
	test(progress().iError==KErrNone);
	// reset
	operationNumber++;
	selection->Reset();

	// test 
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	test(session.GetChildren(service.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue))==KErrNotFound);

	CleanupStack::PopAndDestroy(3); // selection, active1, entries
	}


LOCAL_C void TestPcSyncObserversL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active);
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);
	CChangeNotification* changeActive = new (ELeave) CChangeNotification; CleanupStack::PushL(changeActive);

	TMsvPackedChangeNotification changeNotBuf(changeActive->iChange);
	TMsvServerChangeNotificationType changeType;
	TInt parameter,parameter2;
	
	TInt operationNumber=1;
	TPckgBuf<TMsvLocalOperationProgress> progress;
	TMsvEntry entryD;
	TMsvId service;	

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	User::After(100000);

	// create the folder to work under
	TTestMsvEntry folder;
	folder.SetParent(KMsvLocalServiceIndexEntryId);
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(session, operationNumber++, folder, KErrNone);

	// create entry with sync
	TTestMsvEntry entry1;
	entry1.SetParent(folder.Id());
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.IncPcSyncCount();
	CreateEntryL(session, operationNumber, entry1, KErrNone);

	// wait for the oberserver to report
	changeActive->StartL();
	CActiveScheduler::Start();
	changeActive->StartL();
	CActiveScheduler::Start();
	changeActive->StartL();
	CActiveScheduler::Start();

	// test observer feedback
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter,parameter2);
	test(changeType==EMsvEntriesCreated);
	test(parameter==folder.Id());
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());

	// test folders
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// delete the entries
	selection->Reset();
	selection->AppendL(entry1.Id());
	active->StartL();
	changeActive->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));

	// test observer feedback
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter2==folder.Id());
	test(parameter==KMsvDeletedEntryFolderEntryId);
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	// test folders
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	entries->ResetAndDestroy();

	// change/remove deleted entry
	REPORT_IF_ERROR(session.GetEntry(entry1.Id(), service, entryD));
	entryD.DecPcSyncCount();
	active->StartL();
	changeActive->StartL();
	session.ChangeEntryL(entryD, operationNumber, KMsvServerId, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	// test the operation completion
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));

	// test observer feedback
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==KMsvDeletedEntryFolderEntryId);
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	// test folders
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();

	// create two entry - one with sync
	CreateEntryL(session, operationNumber, entry1, KErrNone);
	TTestMsvEntry entry2;
	entry2.SetParent(folder.Id());
	entry2.iType=KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KShortDetails);
	CreateEntryL(session, operationNumber, entry2, KErrNone);

	// wait for the oberserver to report
	changeActive->StartL();
	CActiveScheduler::Start();
	changeActive->StartL();
	CActiveScheduler::Start();

	// delete the entries
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));

	// test observer feedback
	changeActive->StartL();
	CActiveScheduler::Start();
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==folder.Id());
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	// test observer feedback
	changeActive->StartL();
	CActiveScheduler::Start();
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter2==folder.Id());
	test(parameter==KMsvDeletedEntryFolderEntryId);
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	// test folders
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==1);
	entries->ResetAndDestroy();


	// create three entry - two with sync
	CreateEntryL(session, operationNumber, entry1, KErrNone);
	CreateEntryL(session, operationNumber, entry2, KErrNone);
	TTestMsvEntry entry3;
	entry3.SetParent(folder.Id());
	entry3.iType=KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.IncPcSyncCount();
	CreateEntryL(session, operationNumber, entry3, KErrNone);

	// wait for the oberserver to report
	changeActive->StartL();
	CActiveScheduler::Start();
	changeActive->StartL();
	CActiveScheduler::Start();
	changeActive->StartL();
	CActiveScheduler::Start();

	// delete the entries
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());
	active->StartL();
	session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus.Int()==KErrNone); 
	REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));

	// test observer feedback
	changeActive->StartL();
	CActiveScheduler::Start();
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesDeleted);
	test(parameter==folder.Id());
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	// test observer feedback
	changeActive->StartL();
	CActiveScheduler::Start();
	selection->Reset();
	changeNotBuf.UnpackL(changeType, *selection, parameter, parameter2);
	test(changeType==EMsvEntriesMoved);
	test(parameter==KMsvDeletedEntryFolderEntryId);
	test(parameter2==folder.Id());
	test(selection->Count()==2);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry3.Id());
	// test folders
	REPORT_IF_ERROR(session.GetChildren(folder.Id(), *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==0);
	entries->ResetAndDestroy();
	REPORT_IF_ERROR(session.GetChildren(KMsvDeletedEntryFolderEntryId, *entries, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(entries->Count()==3);
	entries->ResetAndDestroy();

	session.Close();
	CleanupStack::PopAndDestroy(4); // selection, active1, entries, changeActive
	}

LOCAL_C void TestWaitL()
	{
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(0x00B00000); // has to be longer than KMsvDelayTime
	CActiveScheduler::Start();
	delete timer;
	}

LOCAL_C void TestRemovingAllFilesL()
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);
	COperationCompleteActive* active = new (ELeave) COperationCompleteActive; CleanupStack::PushL(active);
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); CleanupStack::PushL(entries);

	CDir* dir;
	TFileName filename;
	TInt count;
	TInt operationNumber=1;
	TPckgBuf<TMsvLocalOperationProgress> progress;

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);

	// everything under the inbox
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalInBoxIndexEntryId, *entries, sort));
	count=entries->Count();
	while (count--)
		selection->AppendL(entries->At(count)->Entry().Id());
	if (selection->Count())
		{
		active->StartL();
		session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus.Int()==KErrNone); 
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
		}
	selection->Reset();
	entries->ResetAndDestroy();
	
	// everything under the outbox
	REPORT_IF_ERROR(session.GetChildren(KMsvGlobalOutBoxIndexEntryId, *entries, sort));
	count=entries->Count();
	while (count--)
		selection->AppendL(entries->At(count)->Entry().Id());
	if (selection->Count())
		{
		active->StartL();
		session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
		CActiveScheduler::Start();
		test(active->iStatus.Int()==KErrNone); 
		REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
		}
	selection->Reset();
	entries->ResetAndDestroy();

	// everything under the local service
	REPORT_IF_ERROR(session.GetChildren(KMsvLocalServiceIndexEntryId, *entries, sort));

	for (count=0; count<entries->Count(); count++)
		{
		TMsvId id = entries->At(count)->Entry().Id();
		if (id!=KMsvGlobalOutBoxIndexEntryId && id!=KMsvGlobalInBoxIndexEntryId && id!=KMsvDraftEntryIdValue && id!=KMsvGlobalOutBoxIndexEntryId && id!=KMsvDeletedEntryFolderEntryId)
			{
			selection->AppendL(id);
			active->StartL();
			session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
			CActiveScheduler::Start();
			test(active->iStatus.Int()==KErrNone); 
			REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
			selection->Reset();
			}
		}
	entries->ResetAndDestroy();

	// test
	REPORT_IF_ERROR(session.GetChildren(KMsvLocalServiceIndexEntryId, *entries, sort));

	test(entries->Count()==5);
	entries->ResetAndDestroy();

	TestNumberOfFilesInService(KMsvLocalServiceIndexEntryId,0);

	// everything under the root
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, sort));
	for (count=0; count<entries->Count(); count++)
		{
		TMsvId id = entries->At(count)->Entry().Id();
		if (id!=KMsvLocalServiceIndexEntryId)
			{
			selection->AppendL(id);
			active->StartL();
			session.DeleteEntriesL(*selection, operationNumber, active->iStatus);
			CActiveScheduler::Start();
			test(active->iStatus.Int()==KErrNone); 
			REPORT_IF_ERROR(session.OperationCompletion(operationNumber, progress));
			selection->Reset();
			}
		}
	entries->ResetAndDestroy();

	// test
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, sort));
	test(entries->Count()==1);
	entries->ResetAndDestroy();
	filename=_L("C:");
	filename.Append(KMsvDefaultFolder2);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
		test(dir->Count()==1);
	delete dir;

	session.Close();
	CleanupStack::PopAndDestroy(3); // selection, active, entries
	}

LOCAL_C void TestEntryStream(RMsvServerSession& aSession, TMsvId aId, TMsvId aService, TBool aAllDescendents)
	{
	CNullMsvStoreObserver ob;
	
	// get the store
	TFileName filename;
	GetStoreFileName(aId, aService, filename);
	CMsvStore* store = NULL;
	
	TRAPD(error,store=CTestMsvStore::OpenForReadL(ob, theUtils->FileSession(), aSession, aId));

		test(store==NULL || store->IsPresentL(KUidMsvHiddenEntryStream)==EFalse);
		delete store;
	// recurse into the children
	if (aAllDescendents)
		{
		TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
		CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); 
		CleanupStack::PushL(entries);
		// all services
		REPORT_IF_ERROR(aSession.GetChildren(aId, *entries, sort));
		TInt count=entries->Count();
		while (count--)
			TestEntryStream(aSession, aId, aService, EFalse);
		entries->ResetAndDestroy();
		CleanupStack::PopAndDestroy(); // entries
		}
	}


LOCAL_C void TestHiddenEntryStreamsL()
	{
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	CArrayPtrFlat<CMsvClientEntry>* entries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(8); 
	CleanupStack::PushL(entries);

	// all services
	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
	REPORT_IF_ERROR(session.GetChildren(KMsvRootIndexEntryId, *entries, sort));
	TInt count=entries->Count();
	while (count--)
		TestEntryStream(session, entries->At(count)->Entry().Id(), entries->At(count)->Entry().Id(), EFalse);
	entries->ResetAndDestroy();
	CleanupStack::PopAndDestroy(); // entries
	
	// local entries
	TestEntryStream(session, KMsvLocalServiceIndexEntryId, KMsvLocalServiceIndexEntryId, ETrue);

	session.Close();
	}



LOCAL_C void TestAvailableSlotsL()
	{
	CArrayPtrFlat<COperationCompleteActive>* activeArray = new(ELeave)CArrayPtrFlat<COperationCompleteActive>(32);

	TInt count;
	for (count=0; count<KMsvNumberOfSlots; count++)
		activeArray->AppendL(new (ELeave) COperationCompleteActive);

	TInt operationNumber=1;
	TBuf8<64> progress;
	TInt leave;

	RMsvServerSession session;
	session.Connect(theUtils->FileSession());

	// install the MTM
	TInt error = session.InstallMtmGroup(KDataComponentFileName);
	test(error==KErrNone || error==KErrAlreadyExists);

	// create the service to work under
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId);
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntryL(session, operationNumber++, service, KErrNone);

	// entry to create
	TTestMsvEntry folder1;
	folder1.SetParent(service.Id());
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidTestServerMtmType;
	folder1.iServiceId = service.Id();

	// fill all the slots
	TInt startOp = operationNumber;
	for (count=0; count<KMsvMaxSlotsAvailable; count++)
		{
		activeArray->At(count)->StartL();
		session.CreateEntryL(folder1, operationNumber++, KMsvServerId, activeArray->At(count)->iStatus);
		}
	// test they are full
	TRAP(leave, session.CreateEntryL(folder1, operationNumber, KMsvServerId, activeArray->At(count)->iStatus));
	test(leave=KErrServerBusy);

	// test we can still get progress
	REPORT_IF_ERROR(session.OperationProgress(startOp, progress));

	// test they are full
	TRAP(leave, session.CreateEntryL(folder1, operationNumber, KMsvServerId, activeArray->At(count)->iStatus));
	test(leave=KErrServerBusy);

	// complete one
	CActiveScheduler::Start();
	REPORT_IF_ERROR(session.OperationCompletion(startOp++, progress));
	
	// add another
	activeArray->At(count)->StartL();
	session.CreateEntryL(folder1, operationNumber++, KMsvServerId, activeArray->At(count)->iStatus);

	// complete them all
	for (count=0; count<KMsvMaxSlotsAvailable; count++)
		{
		CActiveScheduler::Start();
		REPORT_IF_ERROR(session.OperationCompletion(startOp++, progress));
		}

	session.Close();
	activeArray->ResetAndDestroy();
	delete activeArray;
	}


void doTestsL()
	{
	CreateMtmFilesL();

#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	_LIT(KDBFile, "C:\\messaging.db");
	TInt err = RSqlDatabase::Delete(KDBFile);
	test(err == KErrNone || err == KErrNotFound || err == KErrPathNotFound);
#endif

	CMsgsTestUtils::Start(_L("Server creation"));
	TestServerCreationL();	
	TestServerAsyncOpeningL();	
	TestMessageDirectoryL();	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Thread renaming"));
	TestThreadRenameL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry creation/getting"));
	TestEntryCreationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry locking/releasing"));
	TestEntryLockingReleasingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry deletion"));
	TestEntryDeletionL();
	TestEntryDeleteAllL();
	TestEntryRemoveL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry changing"));
	TestEntryChangingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Children selection"));
	TestChildrenSelectionL();
	TestChildrenGettingL();
	TestSW1_98L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Store locking/releasing"));
	TestStoreLockingReleasingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Operations"));
	TestLocalL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Change notification"));
	TestNotificationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Moving local entries"));
	TestMovingLocalL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Copying local entries"));
	TestCopyingLocalL();
	TestCopyingLocalIntoDescendantL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Server Mtm"));
	TestNonExistantServerMtmL();
	TestServerMtmSingleOperationsL();
	TestServerMtmMultipleOperationsL();
//	TestServerMtmOutstandingOperationsL();
	TestServerMtmMultipleMtms();
	TestServerMtmMutipleClients();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("PC Sync'ing"));
	TestPcSyncL();
	TestPcSyncObserversL();
	TestPcSyncDeletingServiceL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Services"));
	TestServiceCommandsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Registry"));
	TestRegistryL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Available slots"));
	TestAvailableSlotsL();
	CMsgsTestUtils::Complete();


	test.Printf(_L("** Waiting 10s for delayed updating of files **\n"));
	TestWaitL();

	CMsgsTestUtils::Next(_L("Hidden entry stream"));
	TestHiddenEntryStreamsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...and checking for orphan files\n"));
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();

	// check the server for alloc heaven
	RMsvServerSession session;
	session.Connect(theUtils->FileSession());
	session.CloseMessageServer();
	session.Close();
	
	}

#pragma warning( default : 4710 ) 
#pragma warning( default : 4238 ) 

