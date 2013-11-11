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
#include "MSERVER.H"
#include <msvreg.h>
#include <msvuids.h>
#include <msvids.h>
#include "MSERVER.H"
#include "msvutils.h"
#include <mtclreg.h>
#include "MSGQRY.H"
#include "MsvSecurityCapabilitySet.h"
#include "Testsetup.h"

#include "MsvSecurityCapabilitySet.h"
#include "CAsyncWaiter.h"

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

#if !defined(_DEBUG)
#pragma warning( disable : 4702 ) //  unreachable code
#endif
#pragma warning( disable : 4238 ) //nonstandard extension used : class rvalue used as lvalue

RTest test(_L("MSV Client Testrig"));
LOCAL_D CMsvEntrySelection* theRemovedEntries;

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters");
_LIT(KLongDetails, "A very long details string containing rubbish and junk");
_LIT(KDrive, "C:");
_LIT(KBlockServerMtmStartFile, "c:\\block test mtm start");
_LIT(KDataComponentFilename, "c:\\system\\mtm\\testsmtm.dat");
_LIT(KMtmDataFile1, "c:\\system\\mtm\\REGMTM1.dat");
_LIT(KMtmDataFile2, "c:\\system\\mtm\\REGMTM2.dat");
_LIT(KMtmName1, "Reg control test MTM 1");
_LIT(KMtmName2, "Reg control test MTM 2");
_LIT(KMtmRegistry, "C:\\private\\1000484b\\Mtm Registry v2");

const TUid KMsvTestUid1 = {0x102001D6};
const TUid KMsvTestUid2 = {0x102001D7};
const TUid KUidMsvTestMtm = {0x100012C2};
const TUid KUidTestServerMtmType = {0x10001304};
const TUid KUidTestServerMtmVersion1 = {0x10001305};
const TUid KUidTestServerMtmTech = {0x10001304};
const TUid KUidMsvHiddenEntryStream = {0x10003C73};
const TUid KUidTestMtm1 = {0x10005097};
const TUid KUidTestMtm2 = {0x10005098};
_LIT(KTestServerMtmFilename, "z:\\system\\libs\\testservermtm.dll");
_LIT(KTestMtm1Filename, "z:\\system\\libs\\regmtm1.dll");
_LIT(KTestMtm2Filename, "z:\\system\\libs\\regmtm2.dll");

//

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	CSessionObserver();
	~CSessionObserver();
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
protected:
	void DoHandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iType;
	CMsvEntrySelection iSelection;
	TMsvId iParameter;
	TMsvId iParameter2;
	};

CSessionObserver::CSessionObserver() 
	{
	}

CSessionObserver::~CSessionObserver() 
	{
	}

void CSessionObserver::DoHandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	CMsvEntrySelection* selection=NULL;
	iSelection.Reset();
	iParameter=0;
	iType = aEvent;
	switch (iType)
		{
		case EMsvEntriesCreated:
			selection= (CMsvEntrySelection*)aArg1;
			iParameter = *(TMsvId*)aArg2;
			break;
		case EMsvEntriesChanged:
			selection= (CMsvEntrySelection*)aArg1;
			break;
		case EMsvEntriesDeleted:
			selection= (CMsvEntrySelection*)aArg1;
			iParameter = *(TMsvId*)aArg2;
			break;
		case EMsvEntriesMoved:
			selection = (CMsvEntrySelection*)aArg1;
			iParameter = *(TMsvId*)aArg2;
			iParameter2 = *(TMsvId*)aArg3;
			break;
		default:
			break;
		}
	if (selection)
		{
		for (TInt count=0; count<selection->Count(); count++)
			iSelection.AppendL(selection->At(count));
		}
	}


void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
	CActiveScheduler::Stop();
	}

//
//
//
class CSpecialSessionObserver : public CSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	};


void CSpecialSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
	}


//
//
//
class CDestructSessionObserver : public CSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	CMsvSession* iSession;
	};

void CDestructSessionObserver::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	delete iSession;
	CActiveScheduler::Stop();
	}

//
//
//
class COtherSessionObserver : public CSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void Start() {iActive=ETrue;};
private:
	TBool iActive;
	};


void COtherSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
	if (iActive)
		{
		iActive=EFalse;
		CActiveScheduler::Stop();
		}
	}

//

class CMultipleSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	CMultipleSessionObserver();
	~CMultipleSessionObserver();
	void Start(TMsvSessionEvent aType, TInt aParameter);
	void Start(TMsvSessionEvent aType, TInt aParameter, TInt aParameter2);
	void Finish() {iActive=EFalse;};
	void Wait() {iWait=ETrue;};
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TBool iWait;
	TBool iActive;
	TInt iEvents;
	TMsvSessionEvent iType;
	CMsvEntrySelection iSelection;
	TMsvId iParameter;
	TMsvId iParameter2;
	};

CMultipleSessionObserver::CMultipleSessionObserver() 
	{
	}

CMultipleSessionObserver::~CMultipleSessionObserver() 
	{
	}

void CMultipleSessionObserver::Start(TMsvSessionEvent aType, TInt aParameter) 
	{
	iActive=ETrue;
	iEvents=0;
	iType=aType; 
	iSelection.Reset();
	iParameter=aParameter;
	}

void CMultipleSessionObserver::Start(TMsvSessionEvent aType, TInt aParameter, TInt aParameter2) 
	{
	iActive=ETrue;
	iEvents=0;
	iType=aType; 
	iSelection.Reset();
	iParameter=aParameter;
	iParameter2=aParameter2;
	}

void CMultipleSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	if (iActive)
		{
		CMsvEntrySelection* selection=NULL;
		iEvents++;
		__ASSERT_ALWAYS(iType==aEvent,User::Panic(_L("T_CLIENT"),1));
		switch (iType)
			{
			case EMsvEntriesMoved:
				__ASSERT_ALWAYS(iParameter2==*(TMsvId*)aArg3, User::Panic(_L("T_CLIENT"),2));
			case EMsvEntriesDeleted:
			case EMsvEntriesCreated:
				selection= (CMsvEntrySelection*)aArg1;
				__ASSERT_ALWAYS(iParameter==*(TMsvId*)aArg2, User::Panic(_L("T_CLIENT"),2));
				break;
			default:
				User::Panic(_L("T_CLIENT"),3);
				break;
			}
		if (selection)
			{
			for (TInt count=0; count<selection->Count(); count++)
				iSelection.AppendL(selection->At(count));
			}
		if (iWait)
			{
			CActiveScheduler::Stop();
			iWait=EFalse;
			}
		}
	}

// test classes //

class CSpecialEntryObserver : public CEntryObserver
	{
public:
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	};


void CSpecialEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleEntryEventL(aEvent, aArg1, aArg2, aArg3);
	}

	
// test classes //

class CCancelTimer : public CTimer
	{
public:
	static CCancelTimer* NewL(CMsvOperation*& aOperation);
	~CCancelTimer();
	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest(TInt aWait);
protected:
	CCancelTimer(CMsvOperation*& aOperation);
private:
	CMsvOperation*& iOperation;
	};

CCancelTimer* CCancelTimer::NewL(CMsvOperation*& aOperation)
	{
	CCancelTimer* self = new(ELeave) CCancelTimer(aOperation);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CCancelTimer::CCancelTimer(CMsvOperation*& aOperation)
: CTimer(-3), iOperation(aOperation)
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

void CCancelTimer::IssueRequest(TInt aWait)
	{
	After(aWait*1000000);
	}

void CCancelTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void CCancelTimer::RunL()
	{
	iOperation->Cancel();
	}

// test classes //#


class CDeleteTimer : public CTimer
	{
public:
	static CDeleteTimer* NewL(CMsvOperation*& aOperation);
	~CDeleteTimer();
	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest(TInt aWait);
protected:
	CDeleteTimer(CMsvOperation*& aOperation);
private:
	CMsvOperation*& iOperation;
	};

CDeleteTimer* CDeleteTimer::NewL(CMsvOperation*& aOperation)
	{
	CDeleteTimer* self = new(ELeave) CDeleteTimer(aOperation);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CDeleteTimer::CDeleteTimer(CMsvOperation*& aOperation)
: CTimer(-3), iOperation(aOperation)
	{
	}

CDeleteTimer::~CDeleteTimer()
	{
	Cancel();
	}

void CDeleteTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add( this );
	}

void CDeleteTimer::IssueRequest(TInt aWait)
	{
	After(aWait*1000000);
	}

void CDeleteTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void CDeleteTimer::RunL()
	{
	delete iOperation;
	}

// test classes //#

class CProgressTimer : public CTimer
	{
public:
	static CProgressTimer* NewL(CMsvOperation*& aOperation);
	~CProgressTimer();
	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest(TInt aWait);
protected:
	CProgressTimer(CMsvOperation*& aOperation);
public:
	HBufC* iBuffer;
private:
	CMsvOperation*& iOperation;
	};

CProgressTimer* CProgressTimer::NewL(CMsvOperation*& aOperation)
	{
	CProgressTimer* self = new(ELeave) CProgressTimer(aOperation);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CProgressTimer::CProgressTimer(CMsvOperation*& aOperation)
: CTimer(-3), iOperation(aOperation) 
	{
	}

CProgressTimer::~CProgressTimer()
	{
	delete iBuffer;
	DoCancel();
	}

void CProgressTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add( this );
	iBuffer= HBufC::NewL(32);
	}

void CProgressTimer::IssueRequest(TInt aWait)
	{
	After(aWait*1000000);
	}

void CProgressTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void CProgressTimer::RunL()
	{
	iBuffer->Des().Copy(iOperation->ProgressL());
	}

//

LOCAL_C void CreateMtmFilesL()
	{
	TInt error = theUtils->FileSession().MkDirAll(KDataComponentFilename);
	test(error==KErrNone || error == KErrAlreadyExists);

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=mtmdllinfo1=CMtmDllInfo::NewL(_L("Test Server Mtm"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KTestServerMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	capSet.AddCapability(ECapabilityLocalServices);
	capSet.AddCapability(ECapabilityPowerMgmt);
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidTestServerMtmType, KUidTestServerMtmTech, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(),KDataComponentFilename,EFileShareExclusive|EFileStream|EFileWrite);
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


LOCAL_C void CreateEntry(TMsvEntry& aNewEntry, CMsvEntry& aClientEntry, TInt aError, CEntryObserver& aObserver)
	{
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	active->StartL();
	aObserver.Start();
	CMsvOperation*  opert = aClientEntry.CreateL(aNewEntry, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry changed
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalNew);
	test(package().iTotalNumberOfEntries==1);
	test(package().iError==aError);
	if (aError)
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		TTestMsvEntry* ptr = (TTestMsvEntry*)&aNewEntry; 
		ptr->SetId(package().iId);
		ptr->SetParent(aClientEntry.EntryId());
		if (aNewEntry.iType==KUidMsvServiceEntry)
			aNewEntry.iServiceId = aNewEntry.Id();
		}
	test(package().iNumberRemaining==0);

	// Check final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalNew);
	test(package().iTotalNumberOfEntries==1);
	test(package().iError==aError);
	if (aError)
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		TTestMsvEntry* ptr = (TTestMsvEntry*)&aNewEntry; 
		ptr->SetId(package().iId);
		ptr->SetParent(aClientEntry.EntryId());
		if (aNewEntry.iType==KUidMsvServiceEntry)
			aNewEntry.iServiceId = aNewEntry.Id();
		}
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;
	CleanupStack::PopAndDestroy(); // active
	}


LOCAL_C void DeleteEntry(TMsvId aId, CMsvEntry& aClientEntry, CEntryObserver& aObserver)
	{
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	active->StartL();
	aObserver.Start();
	CMsvOperation*  opert = aClientEntry.DeleteL(aId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry changed
	test(opert->iStatus.Int()==KErrNone);
	TMsvLocalOperationProgress details = McliUtils::GetLocalProgressL(*opert);
	test(details.iError==KErrNone);
	delete opert; opert=NULL;
	CleanupStack::PopAndDestroy(); // active
	}


LOCAL_C void DeleteEntries(const CMsvEntrySelection& selection, CMsvEntry& aClientEntry, CEntryObserver& aObserver)
	{
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	active->StartL();
	aObserver.Start();
	CMsvOperation*  opert = aClientEntry.DeleteL(selection, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry changed
	test(opert->iStatus.Int()==KErrNone);
	TMsvLocalOperationProgress details = McliUtils::GetLocalProgressL(*opert);
	test(details.iError==KErrNone);
	delete opert; opert=NULL;
	CleanupStack::PopAndDestroy(); // active
	}
	
LOCAL_C void TestClientCreationL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	// get hold of the root entry
	CMsvEntry* entry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	CleanupStack::PopAndDestroy(2);

	// starting the server from scratch with a file
	session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	// get hold of the root entry
	entry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	CleanupStack::PopAndDestroy(2);

	// adding session to server already active
	session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	// get hold of the root entry
	entry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	// adding a second session
	CMsvSession* session2 = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session2);
	// get hold of the root entry
	CMsvEntry* entry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry2);
	CleanupStack::PopAndDestroy(4);
	}



LOCAL_C void TestEntryCreationL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvOperation* opert;
	
	// add an entry - short description
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Entry().Id()==entry1.Id());

		test(cEntry->HasStoreL()==EFalse);

	// add an entry - long description
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry2.Id());
	test(cEntry->Entry().Id()==entry2.Id());
		test(cEntry->HasStoreL()==EFalse);

	// delete the entries
	cEntry->SetEntryL(entry1.Id());
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;


	// SW1-280
	// create a local entry with the store flag set
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	CMsvStore* store=cEntry->EditStoreL();
	store->CommitL();
	delete store;
	// create a copy entry
	TMsvEntry entryN=cEntry->Entry();
	CreateEntry(entryN, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entryN.Id());
		test(!cEntry->HasStoreL());

	// create a service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	test(cEntry->Entry().iSize == 0);

	TMsvEntry service;
	service.iDescription.Set(KShortDescription);
	service.iDetails.Set(KShortDetails);
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	CreateEntry(service, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(service.Id());
	test(cEntry->Entry()==service);
		test(cEntry->HasStoreL()==EFalse);

	// Create a service synchronously - EDNPHAR-4ENKRE, EDNPHAR-4ENKW3
	TTestMsvEntry testEntry;
	testEntry.iDescription.Set(KShortDescription);
	testEntry.iDetails.Set(KShortDetails);
	testEntry.iType = KUidMsvServiceEntry;
	testEntry.iMtm = KMsvTestUid1;
	testEntry.iServiceId = 0x123;
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->CreateL(testEntry);
	test(testEntry.iServiceId == testEntry.Id());
	cEntry->SetEntryL(testEntry.Id());

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}





LOCAL_C void TestEntryChangingL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	


	// add an entry - short description
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Entry().Id()==entry1.Id());

	// alter entry
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KLongDetails);
	entry1.iMtm = KUidMsvMessageEntry;

	// change the sever copy
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;
	test(cEntry->Entry().Id()==entry1.Id());
	
	// add another entry
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	// delete entry2
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;
	// delete entry1
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;
	
	// test chaning different parts
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Entry().Id()==entry1.Id());

	// change nothing
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// change entry only
	entry1.iType = KUidMsvAttachmentEntry;
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// change description
	entry1.iDescription.Set(KLongDescription);
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// change details
	entry1.iDetails.Set(KLongDetails);
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// change details & description
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// add a child
	cEntry->SetEntryL(entry1.Id());
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	// test the child
	test(cEntry->Count()==1);
	test((*cEntry)[0].Id()==entry3.Id());

	// change details & description
	entry1=cEntry->Entry();
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KLongDetails);
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(cEntry->Entry().Id()==entry1.Id());
	delete opert; opert=NULL;

	// test the child
	test(cEntry->Count()==1);
	test((*cEntry)[0].Id()==entry3.Id());

	// add an servcie
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iDescription.Set(KShortDescription);
	service.iDetails.Set(KShortDetails);
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidMsvTestMtm;
	CreateEntry(service, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(service.Id());
	service.iServiceId = service.Id(); 
	test(cEntry->Entry()==service);

	// alter entry
	service.iDescription.Set(KLongDescription);
	service.iDetails.Set(KLongDetails);
	
	// change the sever copy
	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(service, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;
	test(cEntry->Entry()==service);

	
	// SW1-254
	// changing root
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	if (cEntry->HasStoreL())
		{
		CMsvStore* store = cEntry->EditStoreL();
		store->DeleteL();
		delete store;
		test(cEntry->HasStoreL()==EFalse);
		store = cEntry->EditStoreL();
		RMsvWriteStream out;
		out.AssignLC(*store,KNullUid);
		out.WriteInt32L(10);
		out.CommitL();
		CleanupStack::PopAndDestroy(&out);
		store->CommitL();
		delete store;
		test(cEntry->HasStoreL());
		}
	else
		{
		CMsvStore* store = cEntry->EditStoreL();
		RMsvWriteStream out;
		out.AssignLC(*store,KNullUid);
		out.WriteInt32L(10);
		out.CommitL();
		CleanupStack::PopAndDestroy(&out);
		store->CommitL();
		delete store;
		test(cEntry->HasStoreL());
		store = cEntry->EditStoreL();
		store->DeleteL();
		delete store;
		test(cEntry->HasStoreL()==EFalse);
		}

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}

LOCAL_C void TestChangeAttributeNotifyL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry2);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);
	cEntry1->AddObserverL(*entryOb1);

	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb2);
	cEntry2->AddObserverL(*entryOb2);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);	

	TMsvEntry folder;
	folder.iType = KUidMsvMessageEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;

	// Create two folders
	cEntry1->CreateL(folder);
	cEntry1->SetEntryL(folder.Id());
	cEntry2->CreateL(folder);
	cEntry2->SetEntryL(folder.Id());

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Create an entry in each folder
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry1->CreateL(entry1);
	selection->AppendL(entry1.Id());

	test(!cEntry1->ChildDataL(entry1.Id()).Unread());
	test(cEntry1->ChildDataL(entry1.Id()).Visible());

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry2->CreateL(entry2);
	selection->AppendL(entry2.Id());

	test(!cEntry2->ChildDataL(entry2.Id()).Unread());
	test(cEntry2->ChildDataL(entry2.Id()).Visible());

	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry1->CreateL(entry3);
	selection->AppendL(entry3.Id());

	test(!cEntry1->ChildDataL(entry3.Id()).Unread());
	test(cEntry1->ChildDataL(entry3.Id()).Visible());

	TMsvEntry entry4;
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry2->CreateL(entry4);
	selection->AppendL(entry4.Id());

	test(!cEntry2->ChildDataL(entry4.Id()).Unread());
	test(cEntry2->ChildDataL(entry4.Id()).Visible());

	test(entryOb1->iSelection.Count() == 0);
	test(entryOb2->iSelection.Count() == 0);

	// Change the attributes of the entries
	entryOb1->Start();
	entryOb2->Start();
	session->ChangeAttributesL(*selection, KMsvUnreadAttribute, KMsvVisibilityAttribute);
	CActiveScheduler::Start();
	CActiveScheduler::Start();

	// Check the notifications were correct
	test(entryOb1->iSelection.Count() == 2);
	test(entryOb1->iSelection.Find(entry1.Id()) >= 0);
	test(entryOb1->iSelection.Find(entry3.Id()) >= 0);
	test(entryOb1->iEvent == MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb2->iSelection.Count() == 2);
	test(entryOb2->iSelection.Find(entry2.Id()) >= 0);
	test(entryOb2->iSelection.Find(entry4.Id()) >= 0);
	test(entryOb2->iEvent == MMsvEntryObserver::EMsvChildrenChanged);

	// Check the entries have been changed
	test(cEntry1->ChildDataL(entry1.Id()).Unread());
	test(!cEntry1->ChildDataL(entry1.Id()).Visible());
	test(cEntry2->ChildDataL(entry2.Id()).Unread());
	test(!cEntry2->ChildDataL(entry2.Id()).Visible());
	test(cEntry1->ChildDataL(entry3.Id()).Unread());
	test(!cEntry1->ChildDataL(entry3.Id()).Visible());
	test(cEntry2->ChildDataL(entry4.Id()).Unread());
	test(!cEntry2->ChildDataL(entry4.Id()).Visible());

	CleanupStack::PopAndDestroy(8); // selection, active, entryOb2, entryOb1, cEntry2, cEntry1, session, ob
	}

LOCAL_C void TestChildrenChangingL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);
	cEntry1->AddObserverL(*entryOb1);
	
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb2);
	cEntry2->AddObserverL(*entryOb2);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	TBuf<32> temp;
	TMsvSelectionOrdering order;
	
//	TMsvLocalOperationProgress details;
//	TPckgC<TMsvLocalOperationProgress> package(details);	


	// create a folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvMessageEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(folder1.Id());

	// create a entry
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);

	// set the second entry to the child and alter it from there
	cEntry2->SetEntryL(entry1.Id());

	// test entries
	test((*cEntry1)[0].Id()==entry1.Id());
	test(cEntry2->Entry().Id()==entry1.Id());

	// change entry
	entry1.iDescription.Set(_L("mmmmmmmmmmmm"));
	entry1.iDetails.Set(KLongDetails);
	entryOb2->Start();
	active->StartL();
	opert = cEntry2->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;

	// test entries
	test((*cEntry1)[0].Id()==entry1.Id());
	test(cEntry2->Entry().Id()==entry1.Id());

	// create another entry
	TMsvEntry entry2;
	temp = _L("zzzzzzzzz");
	entry2.iDescription.Set(temp);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry1, KErrNone, *entryOb1);

	// test the order of the children
	order.SetSorting(EMsvSortByDescription);
	cEntry1->SetSortTypeL(order);
	test((*cEntry1)[0].Id()==entry1.Id());
	test((*cEntry1)[1].Id()==entry2.Id());

	// change entry2
	cEntry2->SetEntryL(entry2.Id());
	entry2.iDescription.Set(_L("aaaaaaaaaa"));
	entryOb2->Start();
	active->StartL();
	opert = cEntry2->ChangeL(entry2, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete opert; opert=NULL;

	// test the order of the children
	order.SetSorting(EMsvSortByDescription);
	cEntry1->SetSortTypeL(order);
	test((*cEntry1)[1].Id()==entry1.Id());
	test((*cEntry1)[0].Id()==entry2.Id());

	// test entries
	test((*cEntry1)[0].Id()==entry2.Id());
	test(cEntry2->Entry().Id()==entry2.Id());

	CleanupStack::PopAndDestroy(6);
	}

	
LOCAL_C void TestChildrenSelectionL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvOperation* opert;
	TRequestStatus status;

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	// add entry as a child of another
	cEntry->SetEntryL(entry1.Id());	
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KLongDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);	

	// get the selections of Ids
	CMsvEntrySelection* selection=NULL;

	(cEntry->SetEntryL(folder.Id()));
	selection = cEntry->ChildrenL();
	test(selection!=NULL);
	test(selection->Count()==3);
	delete selection;
	selection=NULL;

	(cEntry->SetEntryL(entry1.Id()));
	selection = cEntry->ChildrenL();
	test(selection!=NULL);
	test(selection->Count()==1);
	delete selection;
	selection=NULL;

	(cEntry->SetEntryL(entry2.Id()));
	selection = cEntry->ChildrenL();
	test(selection!=NULL);
	test(selection->Count()==0);

	// delete all the entries
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());
	(cEntry->SetEntryL(folder.Id()));
	opert = cEntry->DeleteL(*selection, status);
	User::WaitForAnyRequest();
	delete opert; opert=NULL;
	delete selection; selection=NULL;

	
	
	//
	// change the sort ordering

	TMsvSelectionOrdering ordering;

	// create an entry to work under
	(cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId));
	folder.iType=KUidMsvFolderEntry;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	(cEntry->SetEntryL(folder.Id()));
	
	// create lots of entries
	entry1.iType = KUidMsvFolderEntry;
	entry1.iDate = TTime(TDateTime(1999,EApril,1,8,30,0,0));
	entry1.iSize = 0;
	entry1.SetPriority(EMsvHighPriority);
	TBuf<8> det1(_L("ZZZ"));
	entry1.iDetails.Set(det1);
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	entry2.iDate = TTime(TDateTime(1998,EJanuary,7,8,30,0,0));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iSize = 100;
	entry2.SetPriority(EMsvHighPriority);
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	entry3.iDate = TTime(TDateTime(2000,EJanuary,1,4,30,0,0));
	entry3.iType = KUidMsvFolderEntry;
	entry3.SetPriority(EMsvLowPriority);
	entry3.iSize = 10000;
	TBuf<8> det3(_L("AAA"));
	entry3.iDetails.Set(det3);
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	entry4.iDate = TTime(TDateTime(1999,EJanuary,1,8,30,0,0));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iSize = 101;
	entry4.SetPriority(EMsvLowPriority);
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry5;
	entry5.iDate = TTime(TDateTime(1999,EJanuary,1,8,32,0,0));
	entry5.iType = KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	entry5.iSize = 20;
	entry5.SetPriority(EMsvMediumPriority);
	CreateEntry(entry5, *cEntry, KErrNone, *entryOb);



	// sort by date reversed
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	cEntry->SetSortTypeL(ordering);
	selection = cEntry->ChildrenL();
	test(selection->Count()==5);
	test(selection->At(4)==entry2.Id());
	test(selection->At(3)==entry4.Id());
	test(selection->At(2)==entry5.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(0)==entry3.Id());
	delete selection;
	selection=NULL;

	// group by type - sort by size
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	cEntry->SetSortTypeL(ordering);
	selection = cEntry->ChildrenL();
	test(selection->Count()==5);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry5.Id());
	test(selection->At(3)==entry2.Id());
	test(selection->At(4)==entry4.Id());
	delete selection;
	selection=NULL;

	// group by priority and type
	ordering.SetSorting(EMsvSortByNone);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	cEntry->SetSortTypeL(ordering);
	selection = cEntry->ChildrenL();
	test(selection->Count()==5);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry2.Id());
	test(selection->At(3)==entry5.Id());
	test(selection->At(4)==entry4.Id());
	delete selection;
	selection=NULL;
	
	
	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestChildrenAccessorL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	// get the selections of Ids
	CMsvEntrySelection* selection=NULL;
	(cEntry->SetEntryL(folder.Id()));
	selection = cEntry->ChildrenL();
	test(selection!=NULL);
	test(selection->Count()==3);

	// test the count
	test(cEntry->Count()==3);

	// test the access via id
	test(cEntry->ChildDataL(entry1.Id()).Id()==entry1.Id());
	test(cEntry->ChildDataL(entry2.Id()).Id()==entry2.Id());
	test(cEntry->ChildDataL(entry3.Id()).Id()==entry3.Id());
	
	// test index operator
	for (TInt count=0; count<3; count++)
		test((*cEntry)[count].Id()==selection->At(count));
	test((*cEntry)[0].Id() == entry1.Id());
	test((*cEntry)[1].Id() == entry2.Id());
	test((*cEntry)[2].Id()== entry3.Id());

	// delete middle entry
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();	
	delete opert; opert=NULL;

	// test the count
	test(cEntry->Count()==2);

	// test the access via id
	test(cEntry->ChildDataL(entry1.Id()).Id()==entry1.Id());
	test(cEntry->ChildDataL(entry3.Id()).Id()==entry3.Id());
	TRAPD(leave,cEntry->ChildDataL(entry2.Id()));
	test(leave==KErrNotFound);

	// test index operator
	test((*cEntry)[0].Id() == entry1.Id());
	test((*cEntry)[1].Id() == entry3.Id());
	
	// delete first entry
	entryOb->Start();
	active->StartL();
	opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();	
	delete opert; opert=NULL;

	// test index operator
	test((*cEntry)[0].Id() == entry3.Id());
		
	delete selection;

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}



LOCAL_C void TestChildrenSorting1L()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(_L("a first in the list"));
	entry1.iDetails.Set(_L("z last"));
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry2;
	entry2.iDescription.Set(_L("b first in the list"));
	entry2.iDetails.Set(_L("a first"));
	entry2.iType = KUidMsvFolderEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry3;
	entry3.iDescription.Set(_L("z last in the list"));
	entry3.iDetails.Set(_L("m for middle"));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.SetPriority(EMsvHighPriority);
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	TMsvSelectionOrdering order;

	// no order
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry1.Id());
	test((*cEntry)[1].Id()==entry2.Id());
	test((*cEntry)[2].Id()==entry3.Id());
	// no order
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry1.Id());
	test((*cEntry)[1].Id()==entry2.Id());
	test((*cEntry)[2].Id()==entry3.Id());

	// des
	order.SetSorting(EMsvSortByDescription);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry1.Id());
	test((*cEntry)[1].Id()==entry2.Id());
	test((*cEntry)[2].Id()==entry3.Id());

	// des reversed
	order.SetSorting(EMsvSortByDescriptionReverse);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry3.Id());
	test((*cEntry)[1].Id()==entry2.Id());
	test((*cEntry)[2].Id()==entry1.Id());

	// det reversed
	order.SetSorting(EMsvSortByDetailsReverse);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry1.Id());
	test((*cEntry)[1].Id()==entry3.Id());
	test((*cEntry)[2].Id()==entry2.Id());


	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}

LOCAL_C void TestChildrenSorting2L()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(_L("the same3"));
	entry1.iDetails.Set(_L("again3"));
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry2;
	entry2.iDescription.Set(_L("the same2"));
	entry2.iDetails.Set(_L("again2"));
	entry2.iType = KUidMsvFolderEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry3;
	entry3.iDescription.Set(_L("the same1"));
	entry3.iDetails.Set(_L("again1"));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.SetPriority(EMsvHighPriority);
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	TMsvSelectionOrdering order;

	// no order
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==entry1.Id());
	test((*cEntry)[1].Id()==entry2.Id());
	test((*cEntry)[2].Id()==entry3.Id());

	// des
	order.SetSorting(EMsvSortByDescription);
	cEntry->SetSortTypeL(order);
	TMsvId first  = (*cEntry)[0].Id();
	TMsvId second = (*cEntry)[1].Id();
	TMsvId third  = (*cEntry)[2].Id();
	
	// des reversed
	order.SetSorting(EMsvSortByDescriptionReverse);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==third);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==first);

	// des 
	order.SetSorting(EMsvSortByDescription);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==first);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==third);

	// des reversed
	order.SetSorting(EMsvSortByDescriptionReverse);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==third);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==first);

	// det
	order.SetSorting(EMsvSortByDetails);
	cEntry->SetSortTypeL(order);
	first  = (*cEntry)[0].Id();
	second = (*cEntry)[1].Id();
	third  = (*cEntry)[2].Id();

	// det reversed
	order.SetSorting(EMsvSortByDetailsReverse);
	cEntry->SetSortTypeL(order);

	test((*cEntry)[0].Id()==third);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==first);

	// det 
	order.SetSorting(EMsvSortByDetails);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==first);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==third);

	// det reversed
	order.SetSorting(EMsvSortByDetailsReverse);
	cEntry->SetSortTypeL(order);
	test((*cEntry)[0].Id()==third);
	test((*cEntry)[1].Id()==second);
	test((*cEntry)[2].Id()==first);

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestChildrenSelectionsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(_L("a first in the list"));
	entry1.iDetails.Set(_L("z last"));
	entry1.iType = KUidMsvFolderEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry2;
	entry2.iDescription.Set(_L("b first in the list"));
	entry2.iDetails.Set(_L("a first"));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = 123456;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	TMsvEntry entry3;
	entry3.iDescription.Set(_L("z last in the list"));
	entry3.iDetails.Set(_L("m for middle"));
	entry3.iType = KUidMsvAttachmentEntry;
	entry3.iMtm = KUidMsvTestMtm;
	entry3.iServiceId = 123456;
	entry3.SetPriority(EMsvHighPriority);
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	// get the different selections (with a fixed order)
	CMsvEntrySelection* selection;
	TMsvSelectionOrdering order;
	order.SetSorting(EMsvSortByDetails);
	cEntry->SetSortTypeL(order);

	// all
	selection = cEntry->ChildrenL();
	test(selection->Count()==3);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	delete selection;

	// type
	selection = cEntry->ChildrenWithTypeL(KUidMsvFolderEntry);
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	delete selection;
	selection = cEntry->ChildrenWithTypeL(KUidMsvMessageEntry);
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	delete selection;
	selection = cEntry->ChildrenWithTypeL(KUidMsvAttachmentEntry);
	test(selection->Count()==1);
	test(selection->At(0)==entry3.Id());
	delete selection;

	// service
	selection = cEntry->ChildrenWithServiceL(KMsvLocalServiceIndexEntryId);
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	delete selection;
	selection = cEntry->ChildrenWithServiceL(123456);
	test(selection->Count()==2);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	delete selection;

	// mtm
	selection = cEntry->ChildrenWithMtmL(KUidMsvLocalServiceMtm);
	test(selection->Count()==2);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry1.Id());
	delete selection;
	selection = cEntry->ChildrenWithMtmL(KUidMsvTestMtm);
	test(selection->Count()==1);
	test(selection->At(0)==entry3.Id());
	delete selection;


	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}




LOCAL_C void TestEntryDeletionL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
//	TRequestStatus status;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	// add entry as a child of another
	cEntry->SetEntryL(entry1.Id());	
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KLongDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);	

	// delete the entries
	active->StartL();
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	opert = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	active->StartL();
	cEntry->SetEntryL(entry1.Id());
	opert = cEntry->DeleteL(entry4.Id(), active->iStatus);
	CActiveScheduler::Start();
	// test the operation
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	test(opert->iStatus.Int()==KErrNone);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	
	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	selection->AppendL(entry1.Id());
	selection->AppendL(entry3.Id());
	active->StartL();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	// test the operation
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	test(opert->iStatus.Int()==KErrNone);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	//
	// Recursive

	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry2.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry3.Id());
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);

	// delete all
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	selection->Reset();
	selection->AppendL(entry1.Id());
	active->StartL();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	// test the operation
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	test(opert->iStatus.Int()==KErrNone);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	// CHANGE continue



	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // selection
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestServiceDeletionL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);
	
	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvServiceEntry;
	entry1.iMtm = KMsvTestUid1;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvServiceEntry;
	entry2.iMtm = KMsvTestUid1;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvServiceEntry;
	entry3.iMtm = KMsvTestUid2;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	// delete the entries
	active->StartL();
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	opert = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	selection->AppendL(entry1.Id());
	selection->AppendL(entry3.Id());
	active->StartL();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	// test the operation
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	test(opert->iStatus.Int()==KErrNone);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;
	
	//
	// Recursive

	cEntry->SetEntryL(KMsvRootIndexEntryId);
	entry1 = TMsvEntry();
	entry1.iType = KUidMsvServiceEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId=0;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidTestServerMtmType;
	entry2.iServiceId = entry1.Id();
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	// copy from local to remote
	active->StartL();
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	opert = cEntry->CopyL(entry2.Id(), entry1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidTestServerMtmType);
	test(opert->Service()==entry1.Id());
	test(opert->ProgressL()==_L8("copied from local"));
	test(opert->FinalProgress()==_L8("copied from local"));
	delete opert;

	// delete all
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	selection->Reset();
	selection->AppendL(entry1.Id());
	active->StartL();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	// test the operation
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	test(opert->iStatus.Int()==KErrNone);

	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	// Test final progress
	package.Set(opert->FinalProgress());
	test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);

	delete opert; opert=NULL;

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // selection
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}
	

LOCAL_C void TestRecursiveDeletionL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvOperation* opert;
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	TInt count;

	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	TMsvEntry child;
	child.iType = KUidMsvMessageEntry;
	child.iMtm = KUidMsvLocalServiceMtm;
	child.iServiceId = KMsvLocalServiceIndexEntryId;


	for (count=0; count<10; count++)
		{
		// add entries
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
		// create lots of recursive children
		cEntry->SetEntryL(entry1.Id());
		CreateEntry(child, *cEntry, KErrNone, *entryOb);
		TInt ii=count;
		while (ii--)
			{
			cEntry->SetEntryL(child.Id());
			CreateEntry(child, *cEntry, KErrNone, *entryOb);
			}

		// delete the entries
		active->StartL();
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
		CActiveScheduler::Start();
		test(opert->iStatus.Int()==KErrNone);
		test(opert->Mtm()==KUidMsvLocalServiceMtm);
		test(opert->Service()==KMsvLocalServiceIndexEntryId);

		package.Set(opert->ProgressL());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		test(package().iNumberRemaining==0);

		// Test final progress
		package.Set(opert->FinalProgress());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		test(package().iNumberRemaining==0);

		delete opert; opert=NULL;
		}

	for (count=0; count<6; count++)
		{
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
		cEntry->SetEntryL(entry1.Id());
		CreateEntry(child, *cEntry, KErrNone, *entryOb);
		TInt ii=count;
		while (ii--)
			{
			cEntry->SetEntryL(child.Id());
			CreateEntry(child, *cEntry, KErrNone, *entryOb);
			}

		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
		cEntry->SetEntryL(entry2.Id());
		CreateEntry(child, *cEntry, KErrNone, *entryOb);
		ii=count;
		while (ii--)
			{
			cEntry->SetEntryL(child.Id());
			CreateEntry(child, *cEntry, KErrNone, *entryOb);
			}

		// delete the entries
		selection->Reset();
		selection->AppendL(entry1.Id());
		selection->AppendL(entry2.Id());
		active->StartL();
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		opert = cEntry->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start();
		test(opert->iStatus.Int()==KErrNone);
		test(opert->Mtm()==KUidMsvLocalServiceMtm);
		test(opert->Service()==KMsvLocalServiceIndexEntryId);

		package.Set(opert->ProgressL());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==2);
		test(package().iNumberCompleted==2);
		test(package().iNumberFailed==0);
		test(package().iNumberRemaining==0);

		// Test final progress
		package.Set(opert->FinalProgress());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==2);
		test(package().iNumberCompleted==2);
		test(package().iNumberFailed==0);
		test(package().iNumberRemaining==0);

		delete opert; opert=NULL;
		}


	for (count=0; count<6; count++)
		{
		// add entries
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
		// create lots of recursive children
		cEntry->SetEntryL(entry1.Id());
		CreateEntry(child, *cEntry, KErrNone, *entryOb);
		TInt ii=0;
		TMsvId aId = 0;
		while (ii<=count)
			{
			cEntry->SetEntryL(child.Id());
			CreateEntry(child, *cEntry, KErrNone, *entryOb);
			if (ii++==count)
				aId = child.Id();
			}

		cEntry2->SetEntryL(aId);
		CMsvStore* store = cEntry2->EditStoreL();

		// delete the entries
		active->StartL();
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
		CActiveScheduler::Start();
		test(opert->iStatus.Int()==KErrNone);
		test(opert->Mtm()==KUidMsvLocalServiceMtm);
		test(opert->Service()==KMsvLocalServiceIndexEntryId);

		package.Set(opert->ProgressL());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		test(package().iNumberRemaining==0);
		test(package().iError==KErrLocked);

		// Test final progress
		package.Set(opert->FinalProgress());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		test(package().iNumberRemaining==0);
		test(package().iError==KErrLocked);

		delete opert; opert=NULL;

		delete store;
		}

	for (count=0; count<6; count++)
		{
		// add entries
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
		// create lots of recursive children
		cEntry->SetEntryL(entry1.Id());
		CreateEntry(child, *cEntry, KErrNone, *entryOb);
		TInt ii=0;
		TMsvId aId = 0;
		while (ii<=count)
			{
			cEntry->SetEntryL(child.Id());
			CreateEntry(child, *cEntry, KErrNone, *entryOb);
			if (ii++==count)
				aId = child.Id();
			}

		cEntry2->SetEntryL(aId);
		CMsvStore* store = cEntry2->EditStoreL();
		RMsvWriteStream out;
		out.AssignLC(*store,KNullUid);
		out.WriteInt32L(10);
		out.CommitL();
		CleanupStack::PopAndDestroy(&out);
		store->CommitL();
		delete store;
		store = cEntry2->ReadStoreL();

		// delete the entries
		active->StartL();
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
		CActiveScheduler::Start();
		test(opert->iStatus.Int()==KErrNone);
		test(opert->Mtm()==KUidMsvLocalServiceMtm);
		test(opert->Service()==KMsvLocalServiceIndexEntryId);

		package.Set(opert->ProgressL());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		test(package().iNumberRemaining==0);
		test(package().iError==KErrInUse);

		// Test final progress
		package.Set(opert->FinalProgress());
		test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
		test(package().iTotalNumberOfEntries==1);
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		test(package().iNumberRemaining==0);
		test(package().iError==KErrInUse);

		delete opert; opert=NULL;

		delete store;
		}


	CleanupStack::PopAndDestroy(6); 
	}




LOCAL_C void TestEntryRemoveL()
	{
	COtherSessionObserver* ob = new(ELeave) COtherSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* createEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(createEntry);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	createEntry->AddObserverL(*entryOb);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	CMsvEntry* lockEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(lockEntry);
	CMsvEntry* mainEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(mainEntry);

	CMsvStore* store;

	// create folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(folder1.Id());

	//
	// RemoveEntry which is not loacked
	// create entry
	createEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	// remove entry
	ob->Start();
	session->RemoveEntry(entry1.Id());
	CActiveScheduler::Start();
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());
	
	
	//
	// RemoveEntry which are not locked (recursive)
	// create entry with grandchild
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(entry1.Id());
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==1);
	// remove entry
	ob->Start();
	session->RemoveEntry(entry1.Id());
	CActiveScheduler::Start();
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==2);
	test(ob->iSelection.At(0)==entry2.Id());
	test(ob->iSelection.At(1)==entry1.Id());

	//
	// RemoveEntry which grandchild is locked (recursive)
	// create entry with grandchild
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(entry1.Id());
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==1);
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	// lock child
	lockEntry->SetEntryL(entry1.Id());
	store = lockEntry->EditStoreL();
	// remove entry
	session->RemoveEntry(entry1.Id());
	User::After(100000);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==1);
	// release child
	delete store;
	// test HAS TO BE DONE LATER
	theRemovedEntries->AppendL(entry1.Id());
	theRemovedEntries->AppendL(entry2.Id());

	//
	// create entry with two grandchild
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(entry1.Id());
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==2);
	// lock first grandchild
	lockEntry->SetEntryL(entry2.Id());
	store = lockEntry->EditStoreL();
	// remove entry
	session->RemoveEntry(entry1.Id());
	User::After(100000);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	// release child
	delete store;
	// test HAS TO BE DONE LATER
	theRemovedEntries->AppendL(entry1.Id());
	theRemovedEntries->AppendL(entry2.Id());
	theRemovedEntries->AppendL(entry3.Id());


	//
	// create entry with two grandchild
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(entry1.Id());
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	CreateEntry(entry3, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==2);
	// lock second grandchild
	lockEntry->SetEntryL(entry3.Id());
	store = lockEntry->EditStoreL();
	// remove entry
	session->RemoveEntry(entry1.Id());
	User::After(100000);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	// release child
	delete store;
	// test HAS TO BE DONE LATER
	theRemovedEntries->AppendL(entry1.Id());
	theRemovedEntries->AppendL(entry2.Id());
	theRemovedEntries->AppendL(entry3.Id());

	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);

	//
	// create entry with two grandchild
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(entry1.Id());
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	CreateEntry(entry3, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==2);
	// lock child
	lockEntry->SetEntryL(entry1.Id());
	store = lockEntry->EditStoreL();
	// remove entry
	session->RemoveEntry(entry1.Id());
	User::After(100000);
	// test
	mainEntry->SetEntryL(entry1.Id());
	test(mainEntry->Count()==2);
	// release child
	delete store;
	// test HAS TO BE DONE LATER
	theRemovedEntries->AppendL(entry1.Id());
	theRemovedEntries->AppendL(entry2.Id());
	theRemovedEntries->AppendL(entry3.Id());


	// SW1-288
	// removing a service entry
	//
	// RemoveEntry
	// create entry
	createEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	CreateEntry(service, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(service.Id());
	mainEntry->SetEntryL(KMsvRootIndexEntryId);
	// remove entry
	ob->Start();
	session->RemoveEntry(service.Id());
	CActiveScheduler::Start();
	// test
	TRAPD(error, mainEntry->SetEntryL(service.Id()));
	test(error==KErrNotFound);



	CleanupStack::PopAndDestroy(7); // ob,session,entry,entryob,active.lockEntry,mainEntry
	}


LOCAL_C void TestEntryCleanupL()
	{
	COtherSessionObserver* ob = new(ELeave) COtherSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* createEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(createEntry);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	createEntry->AddObserverL(*entryOb);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	CMsvEntry* lockEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(lockEntry);
	CMsvEntry* mainEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(mainEntry);

	TInt leave;

	// create folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *createEntry, KErrNone, *entryOb);
	createEntry->SetEntryL(folder1.Id());


	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;

		
	//
	// push and pop
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	// push and pop
	session->CleanupEntryPushL(entry1.Id());
	session->CleanupEntryPop();
	TRAP(leave,User::Leave(KErrGeneral));
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);

	//
	// push and leave
	ob->Start();
	TRAP(leave,
		{
		session->CleanupEntryPushL(entry1.Id());
		User::Leave(KErrGeneral);});
	
	CActiveScheduler::Start();
	// test
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());

	//
	// push, push and leave
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	TRAP(leave,
		{
		session->CleanupEntryPushL(entry1.Id());
		session->CleanupEntryPushL(entry2.Id());
		User::Leave(KErrGeneral);});
	
	// test
	ob->Start();
	CActiveScheduler::Start();
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry2.Id());
	// test
	ob->Start();
	CActiveScheduler::Start();
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());

	//
	// push, push, pop and leave
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	CreateEntry(entry2, *createEntry, KErrNone, *entryOb);
	TRAP(leave,
		{
		session->CleanupEntryPushL(entry1.Id());
		session->CleanupEntryPushL(entry2.Id());
		session->CleanupEntryPop();
		User::Leave(KErrGeneral);});
	
	// test
	ob->Start();
	CActiveScheduler::Start();
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==1);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());

	//
	// push, push, pop pop and leave
	createEntry->SetEntryL(folder1.Id());
	CreateEntry(entry1, *createEntry, KErrNone, *entryOb);
	TRAP(leave,
		{
		session->CleanupEntryPushL(entry1.Id());
		session->CleanupEntryPushL(entry2.Id());
		session->CleanupEntryPop();
		session->CleanupEntryPop();
		User::Leave(KErrGeneral);});
	// test
	mainEntry->SetEntryL(entry1.Id());
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==2);

	//
	// push, push, pop(2) and leave
	TRAP(leave,
		{
		session->CleanupEntryPushL(entry1.Id());
		session->CleanupEntryPushL(entry2.Id());
		session->CleanupEntryPop(2);
		User::Leave(KErrGeneral);});
	// test
	mainEntry->SetEntryL(entry1.Id());
	mainEntry->SetEntryL(folder1.Id());
	test(mainEntry->Count()==2);

	CleanupStack::PopAndDestroy(7); // ob,session,entry,entryob,active.lockEntry,mainEntry
	}


LOCAL_C void TestScheduledSendingL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;
	
	// add an entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry.Id());
	test(cEntry->Entry().Id()==entry.Id());

	test(!cEntry->Entry().OffPeak());
	test(!cEntry->Entry().Scheduled());
	test(cEntry->Entry().SendingState() == 0);

	entry.SetOffPeak(ETrue);

	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry, active->iStatus);
	delete opert; opert=NULL;
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated	
	
	test(cEntry->Entry().OffPeak());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());
	test(cEntry->Entry().OffPeak());

	entry.SetOffPeak(EFalse);

	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry, active->iStatus);
	delete opert; opert=NULL;
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated	

	test(!cEntry->Entry().OffPeak());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());
	test(!cEntry->Entry().OffPeak());

	entry.SetScheduled(ETrue);

	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry, active->iStatus);
	delete opert; opert=NULL;
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated	

	test(cEntry->Entry().Scheduled());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());
	test(cEntry->Entry().Scheduled());

	entry.SetScheduled(EFalse);

	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry, active->iStatus);
	delete opert; opert=NULL;
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated	

	test(!cEntry->Entry().Scheduled());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());
	test(!cEntry->Entry().Scheduled());

	TUint state;
	for(state = 1; state < 16; state++)
		{
		entry.SetSendingState(state);

		entryOb->Start();
		active->StartL();
		opert = cEntry->ChangeL(entry, active->iStatus);
		delete opert; opert=NULL;
		CActiveScheduler::Start(); // operation complete
		CActiveScheduler::Start(); // cEntry updated	

		test(cEntry->Entry().SendingState() == state);
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		cEntry->SetEntryL(entry.Id());
		test(cEntry->Entry().SendingState() == state);
		}

	entry.SetSendingState(0);

	entryOb->Start();
	active->StartL();
	opert = cEntry->ChangeL(entry, active->iStatus);
	delete opert; opert=NULL;
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated	

	test(cEntry->Entry().SendingState() == 0);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());
	test(cEntry->Entry().SendingState() == 0);

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}

LOCAL_C void TestMessageStoresL()
	{
	RMsvReadStream readStream,readStream1,readStream2;
	RMsvWriteStream writeStream;
	TInt error,value;
	CMsvStore* store;
	CMsvStore* store1;
	CMsvStore* store2;

	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);;
	cEntry1->AddObserverL(*entryOb1);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb2);;
	cEntry2->AddObserverL(*entryOb2);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	// add entry
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(entry1.Id());
	cEntry2->SetEntryL(entry1.Id());


	// create store for writing
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==4321);
	readStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// open store for reading
	store = cEntry1->ReadStoreL();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==4321);
	readStream.Close();
	delete store; store=NULL;

	// try to open store being writen to
	store1 = cEntry1->EditStoreL();
	TRAP(error, store2 = cEntry2->ReadStoreL());
	test(error==KErrAccessDenied);
	delete store1; store1=NULL;

	// open multiple read on a store
	store1 = cEntry1->ReadStoreL();
	store2 = cEntry2->ReadStoreL();
	readStream1.OpenL(*store1, KMsvTestUid1);
	readStream2.OpenL(*store1, KMsvTestUid1);
	value = readStream1.ReadInt32L();
	test(value==4321);
	value = readStream2.ReadInt32L();
	test(value==4321);
	readStream2.Close();
	readStream1.Close();
	delete store1; store1=NULL;
	delete store2; store2=NULL;

	// open read then open write on store
	store1 = cEntry1->ReadStoreL();
	store2 = cEntry2->EditStoreL();
	// check read stream
	readStream1.OpenL(*store1, KMsvTestUid1);
	value = readStream1.ReadInt32L();
	test(value==4321);
	readStream1.Close();
	// rewrite the stream
	writeStream.AssignL(*store2, KMsvTestUid1);
	writeStream.WriteInt32L(1234);
	writeStream.CommitL();
	writeStream.Close();
	// check read stream hasn't changed
	readStream1.OpenL(*store1, KMsvTestUid1);
	value = readStream1.ReadInt32L();
	test(value==4321);
	readStream1.Close();
	// commit the write stream
	store2->CommitL();
	delete store2; store2=NULL;
	// check that the read stream is blocked
	// CHANGE TO DO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	delete store1; store1=NULL;
	// reload the read store
	store1 = cEntry1->ReadStoreL();
	readStream1.OpenL(*store1, KMsvTestUid1);
	value = readStream1.ReadInt32L();
	test(value==1234);
	readStream1.Close();
	delete store1; store1=NULL;


	// open read then open write on store and delete store


	// read only entries
	TMsvEntry folder1;
	folder1.SetReadOnly(ETrue);
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(folder1.Id());
	error=KErrNone;
	TRAP(error, store1 = cEntry1->EditStoreL());
	test(error==KErrAccessDenied);

	// change to write access
	cEntry1->SetEntryL(folder1.Id());
	folder1 = cEntry1->Entry();
	folder1.SetReadOnly(EFalse);
	opert = cEntry1->ChangeL(folder1, active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	delete opert;

	// remove the entry and store
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	opert = cEntry1->DeleteL(entry1.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	delete opert;

	// add service store
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidMsvTestMtm;
	CreateEntry(service, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(service.Id());

	// create store for writing
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==4321);
	readStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// remove the service and store
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	opert = cEntry1->DeleteL(service.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	delete opert;
	
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // entryOb2
	CleanupStack::PopAndDestroy(); // cEntry2
	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry1
	CleanupStack::PopAndDestroy(); // session	
	}


LOCAL_C void TestCorruptMessageStoreDeleteL()
	{
	// create service, add file, delete service.
	_LIT(KMailBox,"private\\1000484b\\mail2\\");

	TFileName mailStoreDir;
	TFileName mailStoreCorruptDir;
	TDriveUnit drive(MessageServer::CurrentDriveL(theUtils->FileSession()));
	mailStoreDir.Append(drive.Name());
	mailStoreDir.Append(KPathDelimiter);
	mailStoreDir.Append(KMailBox);

	//cleanmailbox
	theUtils->FileSession().RmDir(mailStoreDir);
	theUtils->FileSession().MkDirAll(mailStoreDir);

	CDummyObserver ob;
	CMsvOperation* opert;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb1);
	cEntry1->AddObserverL(*entryOb1);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	// add service store
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidMsvTestMtm;
	CreateEntry(service, *cEntry1, KErrNone, *entryOb1);

	// test that the new mailbox is their
	TRAPD(error, cEntry1->SetEntryL(service.Id()));
	test (error == KErrNone);

	// corrupt the mailbox
	mailStoreCorruptDir = mailStoreDir;
	mailStoreCorruptDir.AppendFormat(_L("%08x_S\\a\\"),service.Id());
	error = theUtils->FileSession().MkDir(mailStoreCorruptDir);
	test (error == KErrNone);
	mailStoreCorruptDir.Append(_L("00100100")); //make corrupt dir into a file
	RFile file;
	file.Create(theUtils->FileSession(), mailStoreCorruptDir, EFileShareAny|EFileWrite);
	file.Write(_L8("hello"));
	file.Close();

	// remove the currupt mailbox
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	opert = cEntry1->DeleteL(service.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	delete opert;
	opert = NULL;

	User::After(10000000);
	// test that the corrupt mailbox is removed
	TRAPD(err, cEntry1->SetEntryL(service.Id()));
	test (err == KErrNotFound);

	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(entryOb1);
	CleanupStack::PopAndDestroy(cEntry1);
	CleanupStack::PopAndDestroy(session);
	}


LOCAL_C void TestLocalMovingL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);
	
	// get the selections of Ids
	CMsvEntrySelection* selection=NULL;

	(cEntry->SetEntryL(folder1.Id()));

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	// add entry as a child of another
	cEntry->SetEntryL(entry1.Id());	
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KLongDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);	

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==3);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==0);
	delete selection; selection=NULL;

	// move single entry
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==2);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	delete selection; selection=NULL;
	(cEntry->SetEntryL(entry1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;

	// create mpove selection
	CMsvEntrySelection* moveSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(moveSelection);

	// move single selection entry
	active->StartL();
	moveSelection->AppendL(entry3.Id());
	(cEntry->SetEntryL(folder1.Id()));
	opert = cEntry->MoveL(*moveSelection, folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;
	moveSelection->Reset();

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==2);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(entry1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;

	// copy mutiple selection entry
	moveSelection->AppendL(entry1.Id());
	moveSelection->AppendL(entry3.Id());
	(cEntry->SetEntryL(folder2.Id()));
	active->StartL();
	opert = cEntry->MoveL(*moveSelection, folder1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;
	moveSelection->Reset();

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==3);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==0);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(entry1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // moveSelection
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session	
	}



LOCAL_C void TestLocalMovingWithStoreFilesOpenL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob); CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering()); CleanupStack::PushL(cEntry);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering()); CleanupStack::PushL(cEntry2);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);
	CMsvOperation* opert=NULL;
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);
	CMsvStore* store=NULL;
	RFile file;
	TFileName filename;
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);
	

	// add entries
	cEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());	
	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	// open entry1 store for write
	cEntry2->SetEntryL(entry1.Id());	
	store = cEntry2->EditStoreL();
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrLocked);
	delete opert;
	//
	store->CommitL();
	delete store;

	// open entry1 store for read
	cEntry2->SetEntryL(entry1.Id());	
	store = cEntry2->ReadStoreL();
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	delete store;

	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrLocked);
	delete opert;
	//
	store->CommitL();
	delete store;

	// open entry2 store for read
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->ReadStoreL();
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	delete store;

	
	// open entry1 file
	cEntry2->SetEntryL(entry1.Id());	
	store = cEntry2->EditStoreL();
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	
	CleanupStack::PopAndDestroy(2, store); // waiter, store
	waiter = NULL;
	file.Write(_L8("hello"));
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	file.Close();

	// open another entry1 file
	cEntry2->SetEntryL(entry1.Id());
	store = cEntry2->EditStoreL();
	CleanupStack::PushL(store);
	
	waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	filename.Zero();
	filename.Append(_L("file2"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::Pop(attachment); // ownership passed
	
	CleanupStack::PopAndDestroy(2, store); // waiter, store
	waiter = NULL;
	file.Write(_L8("hello"));
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	file.Close();

	// open entry2 file
	cEntry2->SetEntryL(entry2.Id());
	store = cEntry2->EditStoreL();
	CleanupStack::PushL(store);

	waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	filename.Zero();
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::Pop(attachment); // ownership passed
	
	CleanupStack::PopAndDestroy(2, store); // waiter, store
	file.Write(_L8("hello"));
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	file.Close();

	// open another entry2 file
	cEntry2->SetEntryL(entry2.Id());
	store = cEntry2->EditStoreL();
	CleanupStack::PushL(store);
	
	waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	filename.Zero();
	filename.Append(_L("file2"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::Pop(attachment); // ownership passed
	
	CleanupStack::PopAndDestroy(2, store); // waiter, store
	file.Write(_L8("hello"));
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==0);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrInUse);
	delete opert;
	//
	file.Close();


	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;

	// add another entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry3.Id());
	selection->AppendL(entry1.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry3.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;

	// add another entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry3.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry3.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;


	TMsvEntry entry4;
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;

	// add two other entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry3.Id());
	selection->AppendL(entry4.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==3);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry3.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;

	// add two other entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry3.Id());
	selection->AppendL(entry1.Id());
	selection->AppendL(entry4.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==3);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry3.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;


	// add two other entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry3.Id());
	selection->AppendL(entry4.Id());
	selection->AppendL(entry1.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==3);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry3.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;

	// add two other entry  
	cEntry->SetEntryL(folder1.Id());
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);
	// open entry2 store for write
	cEntry2->SetEntryL(entry2.Id());	
	store = cEntry2->EditStoreL();
	// move them
	cEntry->SetEntryL(folder1.Id());
	selection->Reset();
	selection->AppendL(entry4.Id());
	selection->AppendL(entry3.Id());
	selection->AppendL(entry1.Id());
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==3);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==1);
	test(package().iNumberRemaining==0);
	test(package().iId==entry4.Id());
	test(package().iError==KErrLocked);
	delete opert;
	//
	delete store;

	CleanupStack::PopAndDestroy(6); 
	}


LOCAL_C void TestMovingAfterReadStoreLeavesL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver;
	CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	CMsvOperation* opert=NULL;
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);
	
	// add entry
	cEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());	

	// fail to open an entry for read, then check you can move it.
	cEntry2->SetEntryL(entry1.Id());
	TRAPD(error, cEntry2->ReadStoreL());
	test(error==KErrNotFound);
	// try to move
	cEntry->SetEntryL(folder1.Id());
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());

	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	test(package().iError==KErrNone);
	delete opert;	

	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entryOb);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(cEntry2);
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);
	}


LOCAL_C void TestSessionObserversL()
	{
	CSpecialSessionObserver* ob = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CSpecialSessionObserver* ob2 = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob2);
	session->AddObserverL(*ob2);
	
	CSpecialSessionObserver* ob3 = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob3);
	session->AddObserverL(*ob3);
	
	CSpecialSessionObserver* ob4 = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob4);
	
	CSpecialSessionObserver* ob5 = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob5);
	session->AddObserverL(*ob5);

	session->RemoveObserver(*ob3);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	CTestActive* active2 = new (ELeave) CTestActive;
	CleanupStack::PushL(active2);

	CMsvOperation* opert;
	CMsvOperation* opert2;
	
	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==folder1.Id());
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob2->iSelection.At(0)==folder1.Id());
	test(ob2->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob5->iSelection.At(0)==folder1.Id());
	test(ob5->iParameter==KMsvGlobalInBoxIndexEntryId);

	// create and delete an CMsvEntry (check it removes it's observer)
	CMsvEntry* temp = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	delete temp;

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==folder2.Id());
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob2->iSelection.At(0)==folder2.Id());
	test(ob2->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob5->iSelection.At(0)==folder2.Id());
	test(ob5->iParameter==KMsvGlobalInBoxIndexEntryId);


	// get the selections of Ids
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	(cEntry->SetEntryL(folder1.Id()));

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==entry1.Id());
	test(ob->iParameter==folder1.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob2->iSelection.At(0)==entry1.Id());
	test(ob2->iParameter==folder1.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob5->iSelection.At(0)==entry1.Id());
	test(ob5->iParameter==folder1.Id());

	// move single entry
	(cEntry->SetEntryL(folder1.Id()));
	active->StartL();
	entryOb->Start();
	opert = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob->iParameter==folder2.Id());
	test(ob->iParameter2==folder1.Id());
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob2->iParameter==folder2.Id());
	test(ob2->iParameter2==folder1.Id());
	test(ob2->iSelection.Count()==1);
	test(ob2->iSelection.At(0)==entry1.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob5->iParameter==folder2.Id());
	test(ob5->iParameter2==folder1.Id());
	test(ob5->iSelection.Count()==1);
	test(ob5->iSelection.At(0)==entry1.Id());

	// add another entries
	(cEntry->SetEntryL(folder2.Id()));
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==entry2.Id());
	test(ob->iParameter==folder2.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob2->iSelection.At(0)==entry2.Id());
	test(ob2->iParameter==folder2.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob5->iSelection.At(0)==entry2.Id());
	test(ob5->iParameter==folder2.Id());

	// move two entries
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	(cEntry->SetEntryL(folder2.Id()));
	active->StartL();
	entryOb->Start();
	opert = cEntry->MoveL(*selection, folder1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;
	selection->Reset();

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob->iParameter==folder1.Id());
	test(ob->iParameter2==folder2.Id());
	test(ob->iSelection.Count()==2);
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob2->iParameter==folder1.Id());
	test(ob2->iParameter2==folder2.Id());
	test(ob2->iSelection.Count()==2);
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob5->iParameter==folder1.Id());
	test(ob5->iParameter2==folder2.Id());
	test(ob5->iSelection.Count()==2);

	// move one entries
	selection->AppendL(entry1.Id());
	(cEntry->SetEntryL(folder1.Id()));
	active->StartL();
	entryOb->Start();
	opert = cEntry->MoveL(*selection, folder2.Id(), active->iStatus);
	selection->Reset();
	// move another entries
	selection->AppendL(entry2.Id());
	active2->StartL();
	entryOb->Start();
	opert2 = cEntry->MoveL(*selection, folder2.Id(), active2->iStatus);
	selection->Reset();

	CActiveScheduler::Start();
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	// test the second callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob->iParameter==folder2.Id());
	test(ob->iParameter2==folder1.Id());
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry2.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob2->iParameter==folder2.Id());
	test(ob2->iParameter2==folder1.Id());
	test(ob2->iSelection.Count()==1);
	test(ob2->iSelection.At(0)==entry2.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesMoved);
	test(ob5->iParameter==folder2.Id());
	test(ob5->iParameter2==folder1.Id());
	test(ob5->iSelection.Count()==1);
	test(ob5->iSelection.At(0)==entry2.Id());

	delete opert; opert=NULL;
	delete opert2; opert2=NULL;

	// delete a entry
	(cEntry->SetEntryL(folder2.Id()));
	active->StartL();
	entryOb->Start();
	opert = cEntry->DeleteL(entry1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iParameter==folder2.Id());
	test(ob->iSelection.Count()==1);
	test(ob->iSelection.At(0)==entry1.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob2->iParameter==folder2.Id());
	test(ob2->iSelection.Count()==1);
	test(ob2->iSelection.At(0)==entry1.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob5->iParameter==folder2.Id());
	test(ob5->iSelection.Count()==1);
	test(ob5->iSelection.At(0)==entry1.Id());


	// add another entries
	(cEntry->SetEntryL(folder2.Id()));
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	
	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==entry3.Id());
	test(ob->iParameter==folder2.Id());
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob2->iSelection.At(0)==entry3.Id());
	test(ob2->iParameter==folder2.Id());
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob5->iSelection.At(0)==entry3.Id());
	test(ob5->iParameter==folder2.Id());

	// delete a selection of entries entry
	selection->Reset();
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());
	(cEntry->SetEntryL(folder2.Id()));
	active->StartL();
	entryOb->Start();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iParameter==folder2.Id());
	test(ob->iSelection.Count()==2);
	test((ob->iSelection.At(0)==entry2.Id() && ob->iSelection.At(1)==entry3.Id()) ||
		 (ob->iSelection.At(0)==entry3.Id() && ob->iSelection.At(1)==entry2.Id())  );
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob2->iParameter==folder2.Id());
	test(ob2->iSelection.Count()==2);
	test((ob2->iSelection.At(0)==entry2.Id() && ob2->iSelection.At(1)==entry3.Id()) ||
		 (ob2->iSelection.At(0)==entry3.Id() && ob2->iSelection.At(1)==entry2.Id())  );
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob5->iParameter==folder2.Id());
	test(ob5->iSelection.Count()==2);
	test((ob5->iSelection.At(0)==entry2.Id() && ob5->iSelection.At(1)==entry3.Id()) ||
		 (ob5->iSelection.At(0)==entry3.Id() && ob5->iSelection.At(1)==entry2.Id())  );


	// nested entries
	(cEntry->SetEntryL(folder2.Id()));
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	(cEntry->SetEntryL(entry2.Id()));
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);

	// open store on entry to lock
	cEntry2->SetEntryL(entry3.Id());
	CMsvStore* store = cEntry2->EditStoreL();

	// delete top to folder recursively
	selection->Reset();
	selection->AppendL(folder1.Id());
	selection->AppendL(folder2.Id());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	active->StartL();
	entryOb->Start();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;

	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob->iSelection.Count()==1);
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob2->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob2->iSelection.Count()==1);
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob5->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob5->iSelection.Count()==1);

	// close store on entry to unlock
	delete store;;

	// delete top to folder recursively
	selection->Reset();
	selection->AppendL(folder2.Id());
	(cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId));
	active->StartL();
	entryOb->Start();
	opert = cEntry->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete opert; opert=NULL;
	
	// test the callback
	test(ob->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob->iSelection.Count()==4);
	test(ob2->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob2->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob2->iSelection.Count()==4);
	test(ob3->iParameter==0);// ob3 was added then deleted to session so should have default values
	test(ob4->iParameter==0);// ob4 was not added to session so should have default values
	test(ob5->iType==MMsvSessionObserver::EMsvEntriesDeleted);
	test(ob5->iParameter==KMsvGlobalInBoxIndexEntryId);
	test(ob5->iSelection.Count()==4);


	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // active2
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // selection
	CleanupStack::PopAndDestroy(); // cEntry2
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session	
	CleanupStack::PopAndDestroy(); // ob	
	CleanupStack::PopAndDestroy(); // ob2
	CleanupStack::PopAndDestroy(); // ob3
	CleanupStack::PopAndDestroy(); // ob4
	CleanupStack::PopAndDestroy(); // ob5
	}


LOCAL_C void TestSessionObserversDestructingL()
	{
	CDestructSessionObserver* ob = new(ELeave) CDestructSessionObserver;
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	ob->iSession=session;

	session->CloseMessageServer();
	CActiveScheduler::Start();
	
	delete ob;
	ob=NULL;
	}


LOCAL_C void TestEntryObserversL()
	{
	CSpecialSessionObserver* ob = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);
	cEntry->AddObserverL(*entryOb1);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

	// create two fodlers to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb1);
	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb1);

	// add additional observers
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb2);
	cEntry->AddObserverL(*entryOb2);
	CEntryObserver* entryOb3 = new(ELeave) CEntryObserver;
	cEntry->AddObserverL(*entryOb3);
	CEntryObserver* entryOb4=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb4);
	cEntry->AddObserverL(*entryOb4);
	// remove one
	cEntry->RemoveObserver(*entryOb3);
	delete entryOb3; entryOb3=NULL;
	
	// set the context to folder1
	cEntry->SetEntryL(folder1.Id());

	// create another client on folder1
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, folder1.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	// with it's own observer
	CEntryObserver* entryOb5=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb5);
	cEntry2->AddObserverL(*entryOb5);

	//
	// change the entry
	TMsvEntry entry = cEntry->Entry();
	entry.iSize = 2143234;
	//
	entryOb1->Start();
	active->StartL();
	operation = cEntry->ChangeL(entry, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->Entry().iSize==entry.iSize);
	test(cEntry2->Entry().iSize==entry.iSize);
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvEntryChanged);

	//
	// create a child under folder1
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvFolderEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb4);
	// test the result
	test(cEntry->Count()==1);
	test(cEntry2->Count()==1);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry1.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry1.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry1.Id());

	//
	// change the child under folder1
	CMsvEntry* cEntryA = CMsvEntry::NewL(*session, entry1.Id(), TMsvSelectionOrdering());
	entry1.iDescription.Set(KLongDescription);
	entryOb1->Start();
	active->StartL();
	operation = cEntryA->ChangeL(entry1, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->ChildDataL(entry1.Id())==entry1);
	test(cEntryA->Entry().Id()==entry1.Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry1.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry1.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry1.Id());
	delete cEntryA; cEntryA=NULL;
	
	//	
	// move entry to folder2
	entryOb4->Start();
	active->StartL();
	operation = cEntry->MoveL(entry1.Id(), folder2.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->Count()==0);
	test(cEntry2->Count()==0);
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry1.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry1.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry1.Id());

	//	
	// copy entry back
	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, folder2.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);
	active->StartL();
	operation = cEntry3->CopyL(entry1.Id(), folder1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->Count()==1);
	test(cEntry2->Count()==1);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	TMsvId newChildId = (*cEntry)[0].Id();
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==(*cEntry)[0].Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==(*cEntry)[0].Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==(*cEntry)[0].Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==(*cEntry)[0].Id());

	//	
	// move entry back
	active->StartL();
	operation = cEntry3->MoveL(entry1.Id(), folder1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->Count()==2);
	test(cEntry2->Count()==2);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	test((*cEntry)[1].Id()==(*cEntry2)[1].Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry1.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry1.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry1.Id());

	//
	// Delete a child
	active->StartL();
	operation = cEntry2->DeleteL(newChildId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the result
	test(cEntry->Count()==1);
	test(cEntry2->Count()==1);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==newChildId);
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==newChildId);
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==newChildId);
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==newChildId);

	//
	// delete the entry
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	active->StartL();
	operation = cEntry2->DeleteL(folder1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvEntryDeleted);
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvEntryDeleted);
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvEntryDeleted);
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==folder1.Id());


	//
	// add three depths of entries
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvFolderEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb4);
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry2.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry2.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry2.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry2.Id());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	//
	cEntry->SetEntryL(entry2.Id());
	cEntry2->SetEntryL(entry2.Id());
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvFolderEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry3, *cEntry2, KErrNone, *entryOb5);
	// test the result
	test(cEntry->Count()==1);
	test(cEntry2->Count()==1);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry3.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry3.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry3.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry3.Id());
	//
	cEntry->SetEntryL(entry3.Id());
	cEntry2->SetEntryL(entry3.Id());
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	entry4.iType = KUidMsvFolderEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry4, *cEntry2, KErrNone, *entryOb5);
	// test the result
	test(cEntry->Count()==1);
	test(cEntry2->Count()==1);
	test((*cEntry)[0].Id()==(*cEntry2)[0].Id());
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry4.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry4.Id());
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb4->iSelection.Count()==1);
	test(entryOb4->iSelection.At(0)==entry4.Id());
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb5->iSelection.Count()==1);
	test(entryOb5->iSelection.At(0)==entry4.Id());


	//
	// delete the top level
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	active->StartL();
	operation = cEntry->DeleteL(entry2.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidMsvLocalServiceMtm);
	test(operation->Service()==KMsvLocalServiceIndexEntryId);
	delete operation; operation=NULL;
	// test the callback
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry2.Id());
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb4->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb5->iEvent==MMsvEntryObserver::EMsvEntryDeleted);


	// CHANGE

	//
	// test context invalid

	//
	// test missing children


	CleanupStack::PopAndDestroy(); // centry3
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(2); // entry2 entrOb5
	CleanupStack::PopAndDestroy(3); // entryOb1,2,4
	CleanupStack::PopAndDestroy(3); // sessiom, sessionOb, entry
	}


LOCAL_C void TestLocalCopyingL()
	{
	CSpecialSessionObserver* ob = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

//	TRequestStatus status;
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==folder1.Id());
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==folder2.Id());
	test(ob->iParameter==KMsvGlobalInBoxIndexEntryId);
	
	// get the selections of Ids
	CMsvEntrySelection* selection=NULL;

	(cEntry->SetEntryL(folder1.Id()));

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==entry1.Id());
	test(ob->iParameter==folder1.Id());

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==0);
	delete selection; selection=NULL;

	CSessionObserver* ob2 = new(ELeave) CSessionObserver;
	CleanupStack::PushL(ob2);
	session->AddObserverL(*ob2);

	// copy single entry
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(entry1.Id(), folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	package.Set(operation->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iParameter==folder2.Id());
	test(ob->iSelection.Count()==1);
	delete operation;

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;

	session->RemoveObserver(*ob2);

	// add entries
	(cEntry->SetEntryL(folder1.Id()));
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iSelection.At(0)==entry2.Id());
	test(ob->iParameter==folder1.Id());

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==2);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;

	session->AddObserverL(*ob2);

	// copy two entries
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(*selection, folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	package.Set(operation->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iParameter==folder2.Id());
	test(ob->iSelection.Count()==2);
	delete operation;
	delete selection; selection=NULL;

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==2);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==3);
	delete selection; selection=NULL;

	// copy two entries to the same parent
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(*selection, folder1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	package.Set(operation->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	test(ob->iType==MMsvSessionObserver::EMsvEntriesCreated);
	test(ob->iParameter==folder1.Id());
	test(ob->iSelection.Count()==2);
	delete operation;
	delete selection; selection=NULL;

	// test index
	(cEntry->SetEntryL(folder1.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==4);
	delete selection; selection=NULL;
	(cEntry->SetEntryL(folder2.Id()));
	selection = cEntry->ChildrenL();
	test(selection->Count()==3);
	delete selection; selection=NULL;

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(5); // ob2, observer, session, cEntry, active
	}

LOCAL_C void TestCancelCopyL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Create the source folder
	TMsvEntry folderSrc;
	folderSrc.iType = KUidMsvFolderEntry;
	folderSrc.iMtm = KUidMsvLocalServiceMtm;
	folderSrc.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderSrc);

	// Create the destination folder
	TMsvEntry folderDst;
	folderDst.iType = KUidMsvFolderEntry;
	folderDst.iMtm = KUidMsvLocalServiceMtm;
	folderDst.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderDst);

	// Now create a message for us to copy
	cEntry->SetEntryL(folderSrc.Id());
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);

	active->StartL();
	CMsvOperation* op = cEntry->CopyL(entry.Id(), folderDst.Id(), active->iStatus);
	op->Cancel();
	CActiveScheduler::Start();
	test(active->iStatus == KErrCancel);
	delete op;

	CleanupStack::PopAndDestroy(4); // active, cEntry, session, ob
	}



LOCAL_C void TestLocalCopyingToDescendentL()
//
//
//
	{
	CSpecialSessionObserver* ob = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	CSessionObserver* ob2 = new(ELeave) CSessionObserver;
	CleanupStack::PushL(ob2);

	CMsvOperation* operation;

	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	session->AddObserverL(*ob2);
	cEntry->SetEntryL(folder1.Id());

	// test
	cEntry->SetEntryL(entry1.Id());
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	test(selection->Count()==0);
	delete selection; selection=NULL;
	cEntry->SetEntryL(folder1.Id());

	// copy single entry into itself
	active->StartL();
	operation = cEntry->CopyL(entry1.Id(), entry1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete operation;

	// test
	cEntry->SetEntryL(entry1.Id());
	selection = cEntry->ChildrenL();
	test(selection->Count()==1);
	delete selection; selection=NULL;
	cEntry->SetEntryL(folder1.Id());

	// copy single entry again
	active->StartL();
	operation = cEntry->CopyL(entry1.Id(), entry1.Id(), active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	delete operation;
	
	// test
	cEntry->SetEntryL(entry1.Id());
	selection = cEntry->ChildrenL();
	test(selection->Count()==2);
	delete selection; selection=NULL;
	cEntry->SetEntryL(folder1.Id());

	CleanupStack::PopAndDestroy(6);
	}


LOCAL_C void TestLocalComplexCopyingL()
	{
	CSpecialSessionObserver* ob = new(ELeave) CSpecialSessionObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;
	CMsvEntrySelection* selection;

	// create two folders to work with
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb);


	// create a complex child
	cEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry1.Id());
	TMsvEntry attach1;
	attach1.iType = KUidMsvAttachmentEntry;
	attach1.iMtm = KUidMsvLocalServiceMtm;
	attach1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(attach1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry2.Id());
	TMsvEntry attach2;
	attach2.iType = KUidMsvAttachmentEntry;
	attach2.iMtm = KUidMsvLocalServiceMtm;
	attach2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(attach2, *cEntry, KErrNone, *entryOb);

	// test
	cEntry->SetEntryL(folder1.Id());
	test(cEntry->Count()==2);
	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Count()==1);
	cEntry->SetEntryL(entry2.Id());
	test(cEntry->Count()==1);
	cEntry->SetEntryL(folder2.Id());
	test(cEntry->Count()==0);

	// copy folder to fodler2
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	active->StartL();
	operation = cEntry->CopyL(folder1.Id(), folder2.Id(), active->iStatus);
	CActiveScheduler::Start();
	delete operation;

	// test old structure
	cEntry->SetEntryL(folder1.Id());
	test(cEntry->Count()==2);
	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Count()==1);
	cEntry->SetEntryL(entry2.Id());
	test(cEntry->Count()==1);

	// test new structure
	cEntry->SetEntryL(folder2.Id());
	test(cEntry->Count()==1);
	selection = cEntry->ChildrenL();
	TMsvId id0 = selection->At(0);
	delete selection;
	cEntry->SetEntryL(id0);
	test(cEntry->Count()==2);
	selection = cEntry->ChildrenL();
	TMsvId id1 = selection->At(0);
	TMsvId id2 = selection->At(1);
	delete selection;
	cEntry->SetEntryL(id1);
	test(cEntry->Count()==1);
	cEntry->SetEntryL(id2);
	test(cEntry->Count()==1);


	CleanupStack::PopAndDestroy(5);	
	}


LOCAL_C void TestNonExistantServerMtmL()
//
// 
//
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CMsvEntrySelection* childSelect=NULL;

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	//
	// none existant Server Mtm

	// create the service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidMsvTestMtm;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);

	// add folder and entry under local
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvTestMtm;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNotFound);
	test(operation->Mtm()==KUidMsvTestMtm);
	test(operation->Service()==service1.Id());
	delete operation;

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==0);
	delete childSelect; childSelect=NULL;

	// start command
	TInt command=1234;
	TBuf8<32> param = _L8("not a lot");
	selection->AppendL(entry1.Id());
	active->StartL();
	operation = session->TransferCommandL(*selection, command, param, active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNotFound);
	test(operation->Mtm()==KUidMsvTestMtm);
	test(operation->Service()==service1.Id());
	delete operation;
	selection->Reset();

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(4); // session, active, selection, cEntry

	}


LOCAL_C void TestServerMtmL()
//
// 
//
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CMsvEntrySelection* childSelect=NULL;

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);

	
	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);
	// another folder
	(cEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	// another entry 
	(cEntry->SetEntryL(folder1.Id()));
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("copied from local"));
	delete operation;

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	// copy from remote to local
	active->StartL();
	(cEntry->SetEntryL(service1.Id()));
	operation = cEntry->CopyL(remoteId, folder1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("copied to local"));
	delete operation;

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==2);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;

	// copy within server
	active->StartL();
	(cEntry->SetEntryL(service1.Id()));
	operation = cEntry->CopyL(remoteId, service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("copied within service"));
	delete operation;

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==2);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==2);
	delete childSelect; childSelect=NULL;

	// start command - entry under local
	TInt command=1;
	TBuf8<32> param;
	selection->AppendL(entry1.Id());
	active->StartL();
	operation = session->TransferCommandL(*selection, command, param, active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("timer completed"));
	delete operation;
	selection->Reset();

	// start command - entry under service
	param = _L8("Hmmmmmmmmmmmm");
	selection->AppendL(remoteId);
	active->StartL();
	operation = session->TransferCommandL(*selection, command, param, active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("timer completed"));
	delete operation;
	selection->Reset();

	// start command - service
	selection->AppendL(service1.Id());
	active->StartL();
	operation = session->TransferCommandL(*selection, command, TPtrC8(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("timer completed"));
	delete operation;
	selection->Reset();


	CCancelTimer* cancel = CCancelTimer::NewL(operation);
	CleanupStack::PushL(cancel);
	CProgressTimer* progress = CProgressTimer::NewL(operation);
	CleanupStack::PushL(progress);

	// cancelling
	command = 100000; // hang the test code :-)
	selection->AppendL(service1.Id());
	active->StartL();
	cancel->IssueRequest(2);
	operation = session->TransferCommandL(*selection, command, TPtrC8(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrCancel);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("cancelled"));
	delete operation;
	selection->Reset();

	// progress
	command = 2; 
	selection->AppendL(service1.Id());
	active->StartL();
	progress->IssueRequest(1);
	operation = session->TransferCommandL(*selection, command, TPtrC8(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(progress->iBuffer->Des()==_L("timer running"));
	test(operation->ProgressL()==_L8("timer completed"));
	delete operation;

	// create another remote entry 
	(cEntry->SetEntryL(service1.Id()));
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	active->StartL();
	progress->IssueRequest(1);
	operation = cEntry->CreateL(entry1, active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("added entry"));
	delete operation;

	// test the index
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==3);
	TMsvId remId = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	// changing an entry
	(cEntry->SetEntryL(remId));
	TMsvEntry remEntry = cEntry->Entry();
	remEntry.iDescription.Set(KLongDescription);
	remEntry.iDetails.Set(KLongDescription);
	entryOb->Start();
	active->StartL();
	operation = cEntry->ChangeL(remEntry, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("changed entry"));
	delete operation; operation=NULL;
	test(cEntry->Entry()==remEntry);


	// changing an entry sync.
	(cEntry->SetEntryL(remId));
	remEntry = cEntry->Entry();
	remEntry.iDescription.Set(KShortDescription);
	remEntry.iDetails.Set(KShortDescription);
	cEntry->ChangeL(remEntry);
	test(cEntry->Entry()==remEntry);
	cEntry->SetEntryL(service1.Id());
	(cEntry->SetEntryL(remId));
	test(cEntry->Entry()==remEntry);



	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(2); // cancel, progress
	CleanupStack::PopAndDestroy(4); // session, active, selection, cEntry
	}


LOCAL_C void TestServerMtmSyncL()
//
// 
//
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CMsvEntrySelection* childSelect=NULL;

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);

	
	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);

	// another folder
	(cEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);

	// another entry 
	(cEntry->SetEntryL(folder1.Id()));
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote
	active->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation = cEntry->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	test(operation->iStatus.Int()==KErrNone);
	test(operation->Mtm()==KUidTestServerMtmType);
	test(operation->Service()==service1.Id());
	test(operation->ProgressL()==_L8("copied from local"));
	delete operation;

	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	TBuf8<256> progress;

	// start command - entry under local
	TInt command=1;
	TBuf8<32> param;
	selection->AppendL(entry1.Id());
	session->TransferCommandL(*selection, command, param, progress);
	test(progress==_L8("timer completed"));
	selection->Reset();

	// start command - entry under service
	param = _L8("Hmmmmmmmmmmmm");
	selection->AppendL(remoteId);
	session->TransferCommandL(*selection, command, param, progress);
	test(progress==_L8("timer completed"));
	selection->Reset();

	// start command - service
	selection->AppendL(service1.Id());
	session->TransferCommandL(*selection, command, TPtrC8(), progress);
	test(progress==_L8("timer completed"));
	selection->Reset();

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(4); // session, active, selection, cEntry
	}


LOCAL_C void TestContextMovingServicesL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb2);;
	cEntry2->AddObserverL(*entryOb2);

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);
	CMsvOperation* operation;
	

	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);
	
	// create remote entry 
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	
	// test the entry
	cEntry2->SetEntryL(entry1.Id());
	test(cEntry2->OwningService()==KMsvLocalServiceIndexEntryId);
	
	// move the entry
	entryOb2->Start();
	active->StartL();
	operation = cEntry->MoveL(entry1.Id(), service1.Id(), active->iStatus);

	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvEntryMoved);
	delete operation;

	// test the entry
	cEntry2->SetEntryL(entry1.Id());
	test(cEntry2->OwningService()==service1.Id());
	
	CleanupStack::PopAndDestroy(6);	
	}


LOCAL_C void TestServerMtmFailsToLoadL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active1 = new (ELeave) CTestActive;
	CleanupStack::PushL(active1);
	CTestActive* active2 = new (ELeave) CTestActive;
	CleanupStack::PushL(active2);
	CTestActive* active3 = new (ELeave) CTestActive;
	CleanupStack::PushL(active3);
	CTestActive* active4 = new (ELeave) CTestActive;
	CleanupStack::PushL(active4);

	CMsvOperation* operation1;
	CMsvOperation* operation2;
	CMsvOperation* operation3;
	CMsvOperation* operation4;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);
	// another folder
	(cEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);


	// create the block file
	RFile file;
	file.Create(theUtils->FileSession(), KBlockServerMtmStartFile, EFileShareAny|EFileWrite);
	file.Write(_L8("hello"));
	file.Close();

	// copy from local to remote
	active1->StartL();
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	operation1 = cEntry->CopyL(folder1.Id(), service1.Id(), active1->iStatus);
	//
	// complete first operation
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==-101);
	test(operation1->Mtm()==KUidTestServerMtmType);
	test(operation1->Service()==service1.Id());
	delete operation1;

	theUtils->FileSession().Delete(KBlockServerMtmStartFile);

	// three copies from local to remote
	active1->StartL();
	active2->StartL();
	active3->StartL();
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	operation1 = cEntry->CopyL(folder1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(folder1.Id(), service1.Id(), active2->iStatus);
	operation3 = cEntry->CopyL(folder1.Id(), service1.Id(), active3->iStatus);
	//
	// complete first operation
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	test(operation1->Mtm()==KUidTestServerMtmType);
	test(operation1->Service()==service1.Id());
	delete operation1;
	//
	// set up block for third
	file.Create(theUtils->FileSession(), KBlockServerMtmStartFile, EFileShareAny|EFileWrite);
	file.Write(_L8("hello"));
	file.Close();
	//
	// complete second operation
	CActiveScheduler::Start();
	test(operation2->iStatus.Int()==KErrNone);
	test(operation2->Mtm()==KUidTestServerMtmType);
	test(operation2->Service()==service1.Id());
	delete operation2;
	//
	// complete third operation
	CActiveScheduler::Start();
	test(operation3->iStatus.Int()==-101);
	test(operation3->Mtm()==KUidTestServerMtmType);
	test(operation3->Service()==service1.Id());
	delete operation3;
	
	theUtils->FileSession().Delete(KBlockServerMtmStartFile);
	
	// four copies from local to remote
	active1->StartL();
	active2->StartL();
	active3->StartL();
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	operation1 = cEntry->CopyL(folder1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(folder1.Id(), service1.Id(), active2->iStatus);
	operation3 = cEntry->CopyL(folder1.Id(), service1.Id(), active3->iStatus);
	//
	// complete first operation
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	test(operation1->Mtm()==KUidTestServerMtmType);
	test(operation1->Service()==service1.Id());
	delete operation1;
	//
	// set up block for third & fourth
	file.Create(theUtils->FileSession(), KBlockServerMtmStartFile, EFileShareAny|EFileWrite);
	file.Write(_L8("hello"));
	file.Close();
	//
	active4->StartL();
	operation4 = cEntry->CopyL(folder1.Id(), service1.Id(), active4->iStatus);
	//
	// complete second operation
	CActiveScheduler::Start();
	test(operation2->iStatus.Int()==KErrNone);
	test(operation2->Mtm()==KUidTestServerMtmType);
	test(operation2->Service()==service1.Id());
	delete operation2;
	//
	// complete third operation
	CActiveScheduler::Start();
	test(operation3->iStatus.Int()==-101);
	test(operation3->Mtm()==KUidTestServerMtmType);
	test(operation3->Service()==service1.Id());
	delete operation3;
	//
	// complete fourth operation
	CActiveScheduler::Start();
	test(operation4->iStatus.Int()==-101);
	test(operation4->Mtm()==KUidTestServerMtmType);
	test(operation4->Service()==service1.Id());
	delete operation4;

	theUtils->FileSession().Delete(KBlockServerMtmStartFile);
	
	CleanupStack::PopAndDestroy(8); // active, selection, cEntry, session, entryOb
	}


LOCAL_C void TestServerMtmMultipleOpsL()
//
// 
//
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active1 = new (ELeave) CTestActive;
	CleanupStack::PushL(active1);
	CTestActive* active2 = new (ELeave) CTestActive;
	CleanupStack::PushL(active2);
	CTestActive* active3 = new (ELeave) CTestActive;
	CleanupStack::PushL(active3);

	CMsvOperation* operation1;
	CMsvOperation* operation2;
	CMsvOperation* operation3;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CMsvEntrySelection* childSelect=NULL;


	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	TInt error;

	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);
	// another folder
	(cEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	// another entry 
	(cEntry->SetEntryL(folder1.Id()));
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote 3 times
	active1->StartL();
	active2->StartL();
	active3->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation1 = cEntry->CopyL(entry1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(entry1.Id(), service1.Id(), active2->iStatus);
	operation3 = cEntry->CopyL(entry1.Id(), service1.Id(), active3->iStatus);
	//
	// complete first operation
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	test(operation1->Mtm()==KUidTestServerMtmType);
	test(operation1->Service()==service1.Id());
	test(operation1->ProgressL()==_L8("copied from local"));
	delete operation1;
	test(operation2->IsActive());
	test(operation2->ProgressL()==_L8("copied from local"));
	test(operation3->IsActive());
	TRAP(error, operation3->ProgressL());
	test(error==KErrNotReady);
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==2);
	delete childSelect; childSelect=NULL;
	//
	// complete second operation
	CActiveScheduler::Start();
	test(operation2->iStatus.Int()==KErrNone);
	test(operation2->Mtm()==KUidTestServerMtmType);
	test(operation2->Service()==service1.Id());
	test(operation2->ProgressL()==_L8("copied from local"));
	delete operation2;
	test(operation3->IsActive());
	test(operation3->ProgressL()==_L8("copied from local"));
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==3);
	delete childSelect; childSelect=NULL;
	//
	// complete third operation
	CActiveScheduler::Start();
	test(operation3->iStatus.Int()==KErrNone);
	test(operation3->Mtm()==KUidTestServerMtmType);
	test(operation3->Service()==service1.Id());
	test(operation3->ProgressL()==_L8("copied from local"));
	delete operation3;
	// test the index
	(cEntry->SetEntryL(folder1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==1);
	delete childSelect; childSelect=NULL;
	(cEntry->SetEntryL(service1.Id()));
	childSelect = cEntry->ChildrenL();
	test(childSelect->Count()==3);
	delete childSelect; childSelect=NULL;

	// CHANGE - access viloation in server
/*	// have outstanding operations when exiting
	// copy from local to remote 3 times
	active1->StartL();
	active2->StartL();
	active3->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation1 = cEntry->CopyL(entry1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(entry1.Id(), service1.Id(), active2->iStatus);
	operation3 = cEntry->CopyL(entry1.Id(), service1.Id(), active3->iStatus);
	delete session;
*/

	// have persisted mtm when exiting
	selection->AppendL(service1.Id());
	active1->StartL();
	operation1 = session->TransferCommandL(*selection, 10, TPtrC8(), active1->iStatus);
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	delete operation1;
	

	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(6); // active, selection, cEntry, session
	}


LOCAL_C void TestServiceFunctionsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	
	CTestActive* active1 = new (ELeave) CTestActive;
	CleanupStack::PushL(active1);
	CTestActive* active2 = new (ELeave) CTestActive;
	CleanupStack::PushL(active2);

	CMsvOperation* operation1;
	CMsvOperation* operation2;

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);

	TBuf8<32> progress;
	
	//
	// Test Server Mtm

	// create the new service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry, KErrNone, *entryOb);
	// another folder
	(cEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	TMsvEntry folder1;
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry, KErrNone, *entryOb);
	// another entry 
	(cEntry->SetEntryL(folder1.Id()));
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);

	// test the mtm is not active
	test(!session->ServiceActive(service1.Id()));

	// copy twice
	active1->StartL();
	active2->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation1 = cEntry->CopyL(entry1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(entry1.Id(), service1.Id(), active2->iStatus);
	// first copy is done
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	test(operation1->Mtm()==KUidTestServerMtmType);
	test(operation1->Service()==service1.Id());
	test(operation1->ProgressL()==_L8("copied from local"));
	delete operation1;

	// test the mtm is active
	test(session->ServiceActive(service1.Id()));
	// test the service progress
	REPORT_IF_ERROR(session->ServiceProgress(service1.Id(), progress));
	test(progress==_L8("copied from local"));

	// second copy is done
	CActiveScheduler::Start();
	delete operation2;

	// copy twice
	active1->StartL();
	active2->StartL();
	(cEntry->SetEntryL(folder1.Id()));
	operation1 = cEntry->CopyL(entry1.Id(), service1.Id(), active1->iStatus);
	operation2 = cEntry->CopyL(entry1.Id(), service1.Id(), active2->iStatus);
	// stop it
	REPORT_IF_ERROR(session->StopService(service1.Id()));
	// test
	CActiveScheduler::Start();
	test(operation1->iStatus.Int()==KErrNone);
	delete operation1;
	//
	CActiveScheduler::Start();
	test(operation2->iStatus.Int()==KErrCancel);
	delete operation2;

	
	CleanupStack::PopAndDestroy(6);
	}



LOCAL_C void TestDogL()
	{
	CMsvOperationWait* dog;
	RTimer timer;
	timer.CreateLocal();

	// c'tor - d'tor
	dog = CMsvOperationWait::NewLC();
	CleanupStack::PopAndDestroy();

	// c'tor - start() -  d'tor
	dog = CMsvOperationWait::NewLC(CActive::EPriorityHigh);
	dog->Start();
	CleanupStack::PopAndDestroy();

	// c'tor - start() - complete -  d'tor
	dog = CMsvOperationWait::NewLC();
	timer.After(dog->iStatus, 1000000);
	dog->Start();
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy();

	// c'tor - start() - complete -  d'tor
	dog = CMsvOperationWait::NewLC();
	dog->iStatus = KRequestPending;
	dog->Start();
	dog->Cancel();
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestCompletedOperationL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	TBuf8<256> prog;
	CMsvOperation* compOp;

	TMsvId service=12345678;

	compOp = CMsvCompletedOperation::NewL(*session, KUidMsvLocalServiceMtm, TPtrC8(), service, active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(compOp->iStatus==KErrNone);
	test(active->iStatus==KErrNone);
	test(compOp->ProgressL()==TPtrC8());
	test(compOp->Mtm()==KUidMsvLocalServiceMtm);
	test(compOp->Service()==service);
	TMsvOp id = compOp->Id();
	delete compOp;

	prog = _L8("ddfgfdg");
	compOp = CMsvCompletedOperation::NewL(*session, KUidMsvLocalServiceMtm, prog, service, active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	test(compOp->iStatus==KErrNone);
	test(active->iStatus==KErrNone);
	test(compOp->ProgressL()==prog);
	test(compOp->Mtm()==KUidMsvLocalServiceMtm);
	test(compOp->Service()==service);
	test(compOp->Id()==id+1);
	delete compOp;

	const TUid uid={0x0};
	prog = _L8("dsgffdgfd dfgfdgfdfd dfgdfgfdgfd rstrdtggdf rgfdgrrrrrrrrrrrr dfgggggggggggggggr rtrtr45");
	compOp = CMsvCompletedOperation::NewL(*session, uid, prog, service, active->iStatus, KErrGeneral);
	active->StartL();
	CActiveScheduler::Start();
	test(compOp->iStatus==KErrGeneral);
	test(active->iStatus==KErrGeneral);
	test(compOp->ProgressL()==prog);
	test(compOp->Mtm()==uid);
	test(compOp->Service()==service);
	test(compOp->Id()==id+2);
	delete compOp;

	TRequestStatus status;
 	compOp = CMsvCompletedOperation::NewL(*session, uid, prog, service, status, KErrGeneral);
   	delete compOp;
 	User::WaitForRequest(status);
 
 	compOp = CMsvCompletedOperation::NewL(*session, KUidMsvLocalServiceMtm, TPtrC8(), service, active->iStatus);
 	CleanupStack::PushL(compOp);
 	compOp->Cancel();
 	active->StartL();
	CActiveScheduler::Start();
	test(compOp->iStatus==KErrNone);
	test(active->iStatus==KErrNone);
	test(compOp->ProgressL()==TPtrC8());
	test(compOp->Mtm()==KUidMsvLocalServiceMtm);
	test(compOp->Service()==service);
	test(compOp->Id()==id+4);
	CleanupStack::PopAndDestroy(compOp);

	CleanupStack::PopAndDestroy(2); // session, active
	}


LOCAL_C void TestMcliUtilsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	//
	// GetLocalProgressL
	CMsvOperation* compOp;
	TPckgBuf<TMsvLocalOperationProgress> progressPack;
	progressPack().iTotalNumberOfEntries = 45;
	progressPack().iNumberRemaining = 13;
	progressPack().iId = 456;

	//
	active->StartL();
	compOp = CMsvCompletedOperation::NewL(*session, KUidMsvLocalServiceMtm, progressPack, TMsvId(1234), active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);
	test(compOp->Mtm()==KUidMsvLocalServiceMtm);
	//
	TMsvLocalOperationProgress prog = McliUtils::GetLocalProgressL(*compOp);
	test(prog.iType					== progressPack().iType);
	test(prog.iTotalNumberOfEntries	==progressPack().iTotalNumberOfEntries);
	test(prog.iNumberCompleted		== progressPack().iNumberCompleted);
	test(prog.iNumberRemaining		== progressPack().iNumberRemaining);
	test(prog.iError				== progressPack().iError);
	test(prog.iId					== progressPack().iId);

	delete compOp;


	CleanupStack::PopAndDestroy(2); // session, active
	}


LOCAL_C void TestLocalStoresL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	// add folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb);

	// create entry
	cEntry1->SetEntryL(folder1.Id());
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	cEntry1->SetEntryL(entry1.Id());
	cEntry2->SetEntryL(entry1.Id());
	
	CMsvStore* store;
	RMsvWriteStream writeStream;

		test(!cEntry1->HasStoreL());
		test(!cEntry2->HasStoreL());

	// create store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(folder1.Id());
	cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->HasStoreL());
	// test other clients
	test(cEntry2->HasStoreL());

	// change store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(folder1.Id());
	cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->HasStoreL());
	// test other clients
	test(cEntry2->HasStoreL());

	// delete the store
	store = cEntry1->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	// Note: We check that the stores exist in a later test

	CleanupStack::PopAndDestroy(5); // ob,session,cEntry1,entryob,centry2
	}


LOCAL_C void TestServiceStoresL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	// add service 	TMsvEntry service;
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	CreateEntry(service, *cEntry1, KErrNone, *entryOb);
	cEntry1->SetEntryL(service.Id());
	cEntry2->SetEntryL(service.Id());

	CMsvStore* store;
	RMsvWriteStream writeStream;

		test(!cEntry1->HasStoreL());
		test(!cEntry2->HasStoreL());

	// create store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(service.Id());
	test(cEntry1->HasStoreL());
	// test other clients
	entryOb->Start();
//	CActiveScheduler::Start();
//	test(cEntry2->HasStoreL());

	// change store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(service.Id());
	test(cEntry1->HasStoreL());
	// test other clients
	entryOb->Start();
//	CActiveScheduler::Start();
//	test(cEntry2->HasStoreL());

	// delete the store
	store = cEntry1->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	// Note: We check that the stores exist in a later test

	CleanupStack::PopAndDestroy(5); // ob,session,cEntry1,entryob,centry2
	}


LOCAL_C void TestRemoteStoresL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CMsvEntrySelection* childSelect = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(childSelect);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvStore* store;
	RMsvWriteStream writeStream;

	// create the new service
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry1, KErrNone, *entryOb);

	// create local entry
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	
	// check the store exists
	cEntry1->SetEntryL(entry1.Id());
		test(!cEntry1->HasStoreL());
	
	// copy from local to remote
	active->StartL();
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	CMsvOperation* operation = cEntry1->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	delete operation;

	// test the index to get the new id
	cEntry1->SetEntryL(service1.Id());
	childSelect = cEntry1->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	// test the new entry does not have a store
	cEntry1->SetEntryL(remoteId);
	cEntry2->SetEntryL(remoteId);
	test(cEntry1->HasStoreL()==EFalse);
	test(cEntry2->HasStoreL()==EFalse);
	
	// create store
	cEntry1->SetEntryL(remoteId);
	store = cEntry1->EditStoreL();

	writeStream.AssignL(*store, KNullUid);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();

	// test other clients
	test(cEntry2->HasStoreL());

	// use the store
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	// test the immediate copy
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(service1.Id());
	cEntry1->SetEntryL(remoteId);
	test(cEntry1->HasStoreL());
	// test other clients

	// change store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL());
	// test the server copy
	cEntry1->SetEntryL(service1.Id());
	cEntry1->SetEntryL(remoteId);
	test(cEntry1->HasStoreL());
	// test other clients
	test(cEntry2->HasStoreL());

	// delete the store
	store = cEntry1->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;
	//
	test(cEntry1->HasStoreL()==EFalse);
	// test the server copy
	cEntry1->SetEntryL(service1.Id());
	cEntry1->SetEntryL(remoteId);
	test(cEntry1->HasStoreL()==EFalse);
	// test other clients
	test(cEntry2->HasStoreL()==EFalse);

	CleanupStack::PopAndDestroy(7); // ob,session,cEntry1,entryob,centry2,childSleect,active
	}

	
LOCAL_C void TestRemoteContextStoreFlagL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);
	CMsvEntry* cEntry4 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry4);
	CMsvEntrySelection* childSelect = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(childSelect);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvStore* store;
	RMsvWriteStream writeStream;

	// create the new service
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry1, KErrNone, *entryOb);

	// create local entry
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	
	// copy from local to remote
	active->StartL();
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	CMsvOperation* operation = cEntry1->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	delete operation;
	// test the index to get the new id
	cEntry1->SetEntryL(service1.Id());
	childSelect = cEntry1->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId1 = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote
	active->StartL();
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	operation = cEntry1->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	delete operation;
	// test the index to get the new id
	cEntry1->SetEntryL(service1.Id());
	childSelect = cEntry1->ChildrenL();
	test(childSelect->Count()==2);
	TMsvId remoteId2 = childSelect->At(1);
	test(remoteId1!=remoteId2);
	delete childSelect; childSelect=NULL;
	
	// two clients look at entry - two don't
	cEntry1->SetEntryL(remoteId1);
	cEntry2->SetEntryL(remoteId1);
	cEntry3->SetEntryL(remoteId2);
	cEntry4->SetEntryL(service1.Id());

	// test
	test(cEntry1->HasStoreL()==EFalse);
	test(cEntry2->HasStoreL()==EFalse);
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());

	// create store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// test
	test(cEntry1->HasStoreL());
	test(cEntry2->HasStoreL());
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());

	// delete the store
	store = cEntry1->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	// test
	test(cEntry1->HasStoreL()==EFalse);
	test(cEntry2->HasStoreL()==EFalse);
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());

	// create store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// test
	test(cEntry1->HasStoreL());
	test(cEntry2->HasStoreL());
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());

	// change store
	store = cEntry1->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(-657);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	
	// test
	test(cEntry1->HasStoreL());
	test(cEntry2->HasStoreL());
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());

	// delete the store
	store = cEntry1->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	// test
	test(cEntry1->HasStoreL()==EFalse);
	test(cEntry2->HasStoreL()==EFalse);
	test(cEntry3->HasStoreL()==EFalse);
		test(!cEntry4->HasStoreL());



	CleanupStack::PopAndDestroy(9); // ob,session,cEntry1,entryob,centry2,centry3,centry3,active,childselect
	}
	

	
LOCAL_C void TestRemoteChildrenStoreFlagL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);

	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	CMsvEntry* cEntryChild = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntryChild);

	CMsvEntrySelection* childSelect = NULL;

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvStore* store;
	RMsvWriteStream writeStream;

	// create the new service
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service1;
	service1.iType=KUidMsvServiceEntry;
	service1.iMtm = KUidTestServerMtmType;
	CreateEntry(service1, *cEntry1, KErrNone, *entryOb);

	// create local entry
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	TMsvEntry entry1;
	entry1.iType=KUidMsvMessageEntry;
	entry1.iMtm = KUidTestServerMtmType;
	entry1.iServiceId = service1.Id();
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	
	// copy from local to remote
	active->StartL();
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	CMsvOperation* operation = cEntry1->CopyL(entry1.Id(), service1.Id(), active->iStatus);
	CActiveScheduler::Start();
	delete operation;

	// test the index to get the new id
	cEntry1->SetEntryL(service1.Id());
	childSelect = cEntry1->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId1 = childSelect->At(0);
	delete childSelect; childSelect=NULL;

	// copy from local to remote
	active->StartL();
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	operation = cEntry1->CopyL(entry1.Id(), remoteId1, active->iStatus);
	CActiveScheduler::Start();
	delete operation;

	// test the index to get the new id
	cEntry1->SetEntryL(remoteId1);
	childSelect = cEntry1->ChildrenL();
	test(childSelect->Count()==1);
	TMsvId remoteId2 = childSelect->At(0);
	test(remoteId1!=remoteId2);
	delete childSelect; childSelect=NULL;

	cEntry1->SetEntryL(remoteId1);
	cEntry2->SetEntryL(remoteId2);

	// test the child store flag
	cEntryChild->SetEntryL(remoteId2);
	test(cEntryChild->HasStoreL()==EFalse);

	// create store
	store = cEntry2->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	cEntryChild->SetEntryL(remoteId2);
	test(cEntryChild->HasStoreL());

	// delete the store
	store = cEntry2->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	cEntryChild->SetEntryL(remoteId2);
	test(cEntryChild->HasStoreL()==EFalse);

	// create store
	store = cEntry2->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	cEntryChild->SetEntryL(remoteId2);
	test(cEntryChild->HasStoreL());

	// change store
	store = cEntry2->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(-657);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// this should not have been notified

	// delete the store
	store = cEntry2->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	cEntryChild->SetEntryL(remoteId2);
	test(cEntryChild->HasStoreL()==EFalse);

	CleanupStack::PopAndDestroy(7); // ob,session,cEntry1,entryob,centry2,cEntryChild, active
	}
	
	
LOCAL_C void TestContextOwnerFlagL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb2);
	cEntry2->AddObserverL(*entryOb2);
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;
	TMsvId id1,id2;
	
	// add folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb);
	cEntry1->SetEntryL(folder1.Id());

	// test
	test(cEntry1->Entry().Owner()==EFalse);

	// add a child
	TMsvEntry entry1;
	entry1.iType = KUidMsvFolderEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);

	// test
	test(cEntry1->Entry().Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner());

	// delete a child
	DeleteEntry(entry1.Id(), *cEntry1, *entryOb);

	// test
	test(cEntry1->Entry().Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner()==EFalse);

	// create two children
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	id1 = entry1.Id();
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	id2 = entry1.Id();

	// delete a child
	DeleteEntry(id1, *cEntry1, *entryOb);

	// test
	test(cEntry1->Entry().Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner());

	// delete a child
	DeleteEntry(id2, *cEntry1, *entryOb);

	// test
	test(cEntry1->Entry().Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner()==EFalse);

	// create two children
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	id1 = entry1.Id();
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);
	id2 = entry1.Id();

	// delete both children
	selection->AppendL(id1);
	selection->AppendL(id2);
	DeleteEntries(*selection, *cEntry1, *entryOb);
	selection->Reset();

	// test
	test(cEntry1->Entry().Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner()==EFalse);

	// add another entry elsewhere
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	TMsvEntry entry2;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);

	// copy the entry to the folder
	entryOb->Start();
	active->StartL();
	operation = cEntry2->CopyL(entry2.Id(), folder1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	test(operation->iStatus.Int()==KErrNone);
	TMsvLocalOperationProgress details = McliUtils::GetLocalProgressL(*operation);
	test(details.iError==KErrNone);
	id1 = details.iId;
	delete operation; operation=NULL;
	
	// test
	test(cEntry1->Entry().Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner());

	// move it out
	entryOb2->Start();
	active->StartL();
	operation = cEntry1->MoveL(id1, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(cEntry1->Entry().Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner()==EFalse);

	// move two back in
	selection->AppendL(entry2.Id());
	selection->AppendL(id1);
	operation = cEntry2->MoveL(*selection, folder1.Id(), active->iStatus);
	entryOb->Start();
	active->StartL();
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(cEntry1->Entry().Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner());

	// move one out
	entryOb2->Start();
	active->StartL();
	operation = cEntry1->MoveL(id1, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(cEntry1->Entry().Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner());

	// move other out
	entryOb2->Start();
	active->StartL();
	operation = cEntry1->MoveL(entry2.Id(), KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(cEntry1->Entry().Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Entry().Owner()==EFalse);

	CleanupStack::PopAndDestroy(8); //
	}


LOCAL_C void TestChildrenOwnerFlagL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry1->AddObserverL(*entryOb);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb2);
	cEntry2->AddObserverL(*entryOb2);
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;
	TMsvId id1,id2;
	
	// add folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb);
	cEntry1->SetEntryL(folder1.Id());

	// add a child to alter
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb);

	// test
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);

	//
	// add a grandchild
	cEntry2->SetEntryL(entry1.Id());
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// delete grandchild
	DeleteEntry(entry2.Id(), *cEntry2, *entryOb2);
	
	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);


	// create an entry elsewhere
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);

	// move it to under entry1
	entryOb->Start();
	active->StartL();
	operation = cEntry2->MoveL(entry2.Id(), entry1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// move it out
	cEntry2->SetEntryL(entry1.Id());
	entryOb2->Start();
	active->StartL();
	operation = cEntry2->MoveL(entry2.Id(), KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;
	
	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);

	// create anothe entry under folder
	TMsvEntry null;
	null.iType = KUidMsvMessageEntry;
	null.iMtm = KUidMsvLocalServiceMtm;
	null.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(null, *cEntry1, KErrNone, *entryOb);
	
	//
	// add two grandchildren
	cEntry2->SetEntryL(entry1.Id());
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);
	id1 = entry2.Id();
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);
	id2 = entry2.Id();
	
	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// move it out
	selection->AppendL(id1);
	selection->AppendL(id2);
	cEntry2->SetEntryL(entry1.Id());
	entryOb2->Start();
	active->StartL();
	operation = cEntry2->MoveL(*selection, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);

	// move them back
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	entryOb2->Start();
	active->StartL();
	operation = cEntry2->MoveL(*selection, entry1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// delete them
	cEntry2->SetEntryL(entry1.Id());
	DeleteEntries(*selection, *cEntry2, *entryOb2);

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);


	//
	// add two grandchildren
	cEntry2->SetEntryL(entry1.Id());
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);
	id1 = entry2.Id();
	CreateEntry(entry2, *cEntry2, KErrNone, *entryOb2);
	id2 = entry2.Id();
	
	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// reset
	entryOb->iSelection.Reset();

	// move one out
	cEntry2->SetEntryL(entry1.Id());
	entryOb2->Start();
	active->StartL();
	operation = cEntry2->MoveL(id1, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb->iSelection.Count()==0);
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// move them back
	cEntry2->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	entryOb2->Start();
	active->StartL();
	operation = cEntry2->MoveL(id1, entry1.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb->iSelection.Count()==0);
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// delete one
	cEntry2->SetEntryL(entry1.Id());
	DeleteEntry(id1, *cEntry2, *entryOb2);

	// test
	test(entryOb->iSelection.Count()==0);
	test(cEntry1->ChildDataL(entry1.Id()).Owner());
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner());

	// delete other one
	DeleteEntry(id2, *cEntry2, *entryOb2);

	// test
	test(entryOb->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb->iSelection.Count()==1);
	test(entryOb->iSelection.At(0)==entry1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->ChildDataL(entry1.Id()).Owner()==EFalse);
	
	
	CleanupStack::PopAndDestroy(8); //
	}



LOCAL_C void TestMtmListL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CArrayFix<TUid>* mtmList=new(ELeave) CArrayFixFlat<TUid>(8);
	CleanupStack::PushL(mtmList);

	TUid uid1={0x100};
	TUid uid2={0x101};
	TUid uid3={0x102};
	TUid uid4={0x103};
	TUid uid5={0x104};

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iType = KUidMsvFolderEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm=uid1;
	CreateEntry(entry1, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm=uid2;
	CreateEntry(entry2, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry3;
	entry3.iType = KUidMsvAttachmentEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm=uid3;
	CreateEntry(entry3, *cEntry, KErrNone, *entryOb);
	TMsvEntry entry4;
	entry4.iType = KUidMsvAttachmentEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm=uid4;
	CreateEntry(entry4, *cEntry, KErrNone, *entryOb);

	CMsvEntrySelection* selection;
	cEntry->SetSortTypeL(TMsvSelectionOrdering());

	// no sorting
	selection = cEntry->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	delete selection;

	cEntry->SetSortTypeL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));

	// mtm grouping but no list
	selection = cEntry->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	delete selection;

	// list provided
	mtmList->Reset();
	mtmList->AppendL(uid1);
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid3);
	mtmList->AppendL(uid4);
	cEntry->SetMtmListL(*mtmList);
	selection = cEntry->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	delete selection;

	// list changed
	mtmList->Reset();
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid5);
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid1);
	mtmList->AppendL(uid3);
	cEntry->SetMtmListL(*mtmList);
	selection = cEntry->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry3.Id());
	delete selection;

	// add another entry
	TMsvEntry entry5;
	entry5.iType = KUidMsvAttachmentEntry;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	entry5.iMtm=uid1;
	CreateEntry(entry5, *cEntry, KErrNone, *entryOb);

	// check it is placed in the correct position
	selection = cEntry->ChildrenL();
	test(selection->Count()==5);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry5.Id());
	test(selection->At(4)==entry3.Id());
	delete selection;


	CleanupStack::PopAndDestroy(); // mtmList
	CleanupStack::PopAndDestroy(); // entryOb
	CleanupStack::PopAndDestroy(); // cEntry
	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestInvisibleEntriesL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	
	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);;
	cEntry1->AddObserverL(*entryOb1);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(folder.Id());

	// add entries
	cEntry1->SetSortTypeL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	TMsvEntry entry1;
	entry1.iType = KUidMsvFolderEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.SetVisible(ETrue);
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.SetVisible(EFalse);
	CreateEntry(entry2, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry entry3;
	entry3.iType = KUidMsvAttachmentEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.SetVisible(EFalse);
	CreateEntry(entry3, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry entry4;
	entry4.iType = KUidMsvAttachmentEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.SetVisible(ETrue);
	CreateEntry(entry4, *cEntry1, KErrNone, *entryOb1);

	CMsvEntrySelection* selection;

	// test - all entries
	selection = cEntry1->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	delete selection;

	// test - visible entries
	cEntry1->SetSortTypeL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse));
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry4.Id());
	delete selection;

	// test - all entries
	cEntry1->SetSortTypeL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	selection = cEntry1->ChildrenL();
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	delete selection;

	// create new client (visible entries only)
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, folder.Id(), TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse));
	CleanupStack::PushL(cEntry2);
	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb2);;
	cEntry2->AddObserverL(*entryOb2);

	// test
	selection = cEntry1->ChildrenL();
	test(selection->Count()==4);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==2);
	delete selection;

	// create a visible entry
	TMsvEntry entry5;
	entry5.iType = KUidMsvAttachmentEntry;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry5, *cEntry1, KErrNone, *entryOb1);

	// test
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry5.Id());
	selection = cEntry1->ChildrenL();
	test(selection->Count()==5);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==3);
	delete selection;
	
	// create an invisible entry
	TMsvEntry entry6;
	entry6.iType = KUidMsvAttachmentEntry;
	entry6.iServiceId = KMsvLocalServiceIndexEntryId;
	entry6.iMtm = KUidMsvLocalServiceMtm;
	entry6.SetVisible(EFalse);
	CreateEntry(entry6, *cEntry1, KErrNone, *entryOb1);

	// test
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry5.Id()); // should not have changed
	selection = cEntry1->ChildrenL();
	test(selection->Count()==6);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==3);
	delete selection;

	// delete a visible entry
	entryOb1->Start();
	active->StartL();
	operation = cEntry1->DeleteL(entry5.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry5.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry5.Id());
	selection = cEntry1->ChildrenL();
	test(selection->Count()==5);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==2);
	delete selection;

	// delete an invisible entry
	entryOb1->Start();
	active->StartL();
	operation = cEntry1->DeleteL(entry6.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry6.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry5.Id()); // should not have changed
	selection = cEntry1->ChildrenL();
	test(selection->Count()==4);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==2);
	delete selection;

	// delete a visible amd an invisible entry
	selection = new(ELeave) CMsvEntrySelection;
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	entryOb1->Start();
	active->StartL();
	operation = cEntry1->DeleteL(*selection, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;
	delete selection; selection=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==2);
	test(entryOb1->iSelection.At(0)==entry1.Id()); 
	test(entryOb1->iSelection.At(1)==entry2.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==1);
	delete selection;

	// create new client 
	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, entry3.Id(), TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry3);
	CEntryObserver* entryOb3=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb3);
	cEntry3->AddObserverL(*entryOb3);

	// change an invisible entry
	entry3.iSize=1000;
	entryOb3->Start();
	active->StartL();
	operation = cEntry3->ChangeL(entry3, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry3.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry1.Id());  // should not have changed
	test(entryOb3->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==1);
	test(selection->At(0)==entry4.Id());
	delete selection;

	// change an invisible entry to visible
	entry3.SetVisible(ETrue);
	entryOb3->Start();
	active->StartL();
	operation = cEntry3->ChangeL(entry3, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry3.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry3.Id());  
	test(entryOb3->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==2);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry3.Id());
	delete selection;

	// change a visible entry to invisible
	entry3.SetVisible(EFalse);
	entryOb3->Start();
	active->StartL();
	operation = cEntry3->ChangeL(entry3, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvChildrenChanged);
	test(entryOb1->iSelection.Count()==1);
	test(entryOb1->iSelection.At(0)==entry3.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry3.Id());  
	test(entryOb3->iEvent==MMsvEntryObserver::EMsvEntryChanged);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==1);
	test(selection->At(0)==entry4.Id());
	delete selection;

	// move two entries out
	selection = new(ELeave) CMsvEntrySelection;
	selection->AppendL(entry3.Id());
	selection->AppendL(entry4.Id());
	entryOb1->Start();
	active->StartL();
	operation = cEntry1->MoveL(*selection, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;
	delete selection; selection=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb1->iSelection.Count()==2);
	test(entryOb1->iSelection.At(0)==entry3.Id()); 
	test(entryOb1->iSelection.At(1)==entry4.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvDeletedChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry4.Id());  
	selection = cEntry1->ChildrenL();
	test(selection->Count()==0);
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==0);
	delete selection;

	// move two entries back in
	selection = new(ELeave) CMsvEntrySelection;
	selection->AppendL(entry3.Id());
	selection->AppendL(entry4.Id());
	entryOb3->Start();
	active->StartL();
	cEntry3->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	operation = cEntry3->MoveL(*selection, folder.Id(), active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry updated
	delete operation; operation=NULL;
	delete selection; selection=NULL;

	// test
	test(entryOb1->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb1->iSelection.Count()==2);
	test(entryOb1->iSelection.At(0)==entry3.Id()); 
	test(entryOb1->iSelection.At(1)==entry4.Id()); 
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvNewChildren);
	test(entryOb2->iSelection.Count()==1);
	test(entryOb2->iSelection.At(0)==entry4.Id());  
	selection = cEntry1->ChildrenL();
	test(selection->Count()==2);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry3.Id());
	delete selection;
	selection = cEntry2->ChildrenL();
	test(selection->Count()==1);
	test(selection->At(0)==entry4.Id());
	delete selection;


	CleanupStack::PopAndDestroy(); // entryOb3
	CleanupStack::PopAndDestroy(); // cEntry3
	CleanupStack::PopAndDestroy(); // active
	CleanupStack::PopAndDestroy(); // entryOb2
	CleanupStack::PopAndDestroy(); // cEntry2
	CleanupStack::PopAndDestroy(); // entryOb1
	CleanupStack::PopAndDestroy(); // cEntry1
	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestPcSyncingL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);
	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, KMsvDeletedEntryFolderEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);
	CMsvEntry* cEntry4 = CMsvEntry::NewL(*session, KMsvDeletedEntryFolderEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry4);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection; CleanupStack::PushL(selection);

	TMsvEntry entryD;

	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	CMsvOperation*  opert;

	cEntry3->SetSortTypeL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	cEntry4->SetSortTypeL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	
	// need to remove any entries from the deleted fodler
	CMsvEntrySelection* select=cEntry3->ChildrenL();
	TInt count=select->Count();
	while (count--)
		{
		cEntry3->SetEntryL(select->At(count));
		entryD = cEntry3->Entry();
		TInt tt=entryD.PcSyncCount();
		while (tt--)
			entryD.DecPcSyncCount();
		active->StartL();
		opert = cEntry3->ChangeL(entryD, active->iStatus);
		CActiveScheduler::Start();
		test(opert->iStatus==KErrNone);
		delete opert;
		}
	cEntry3->SetEntryL(KMsvDeletedEntryFolderEntryId);
	delete select;

	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);;
	cEntry1->AddObserverL(*entryOb1);
	CEntryObserver* entryOb3=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb3);;
	cEntry3->AddObserverL(*entryOb3);

	// create a folder to work with
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntry(folder, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(folder.Id());
	cEntry2->SetEntryL(folder.Id());

	// create entry with sync
	TMsvEntry entry1;
	entry1.iType = KUidMsvFolderEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.IncPcSyncCount();
	entry1.iDescription.Set(KLongDescription);
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);

	// test
	test(cEntry1->Count()==1);
	test((*cEntry1)[0].Id()==entry1.Id());
	test((*cEntry1)[0].iDescription==KLongDescription);
	test(cEntry2->Count()==1);
	test((*cEntry2)[0].Id()==entry1.Id());
	test((*cEntry2)[0].iDescription==KLongDescription);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);

	// delete the entry
	DeleteEntry(entry1.Id(), *cEntry1, *entryOb1);

	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==1);
	test((*cEntry3)[0].Id()==entry1.Id());
	test((*cEntry3)[0].Deleted());
	test((*cEntry3)[0].iDescription==TPtrC());
	test(!(*cEntry3)[0].Visible());
	test(cEntry4->Count()==1);
	test((*cEntry4)[0].Id()==entry1.Id());
	test((*cEntry4)[0].Deleted());
	test(!(*cEntry4)[0].Visible());
	test((*cEntry4)[0].iDescription==TPtrC());

	// change/remove the entry
	entryD = (*cEntry3)[0];
	entryD.DecPcSyncCount();
	cEntry3->SetEntryL(entryD.Id());
	active->StartL();
	entryOb3->Start();
	opert = cEntry3->ChangeL(entryD, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(opert->iStatus==KErrNone);
	delete opert;
	cEntry3->SetEntryL(KMsvDeletedEntryFolderEntryId);

	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);

	// create two entries - one with sync
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry entry2;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry2, *cEntry1, KErrNone, *entryOb1);

	// test
	test(cEntry1->Count()==2);
	test(cEntry2->Count()==2);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);

	// delete the entry
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	DeleteEntries(*selection, *cEntry1, *entryOb1);
	
	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==1);
	test((*cEntry3)[0].Id()==entry1.Id());
	test(cEntry4->Count()==1);
	test((*cEntry4)[0].Id()==entry1.Id());

	// change/remove the entry
	entryD = (*cEntry3)[0];
	entryD.DecPcSyncCount();
	cEntry3->SetEntryL(entryD.Id());
	active->StartL();
	entryOb3->Start();
	opert = cEntry3->ChangeL(entryD, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(opert->iStatus==KErrNone);
	delete opert;
	cEntry3->SetEntryL(KMsvDeletedEntryFolderEntryId);

	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);
	
	// create two entries - both with sync
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);
	entry2.IncPcSyncCount();
	CreateEntry(entry2, *cEntry1, KErrNone, *entryOb1);

	// test
	test(cEntry1->Count()==2);
	test(cEntry2->Count()==2);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);

	// delete the entry
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	DeleteEntries(*selection, *cEntry1, *entryOb1);
	
	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==2);
	test((*cEntry3)[0].Id()==entry2.Id());
	test((*cEntry3)[1].Id()==entry1.Id());
	test(cEntry4->Count()==2);
	test((*cEntry4)[0].Id()==entry2.Id());
	test((*cEntry4)[1].Id()==entry1.Id());

	// change/remove the entry
	entryD = (*cEntry3)[0];
	entryD.DecPcSyncCount();
	cEntry3->SetEntryL(entryD.Id());
	active->StartL();
	entryOb3->Start();
	opert = cEntry3->ChangeL(entryD, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(opert->iStatus==KErrNone);
	delete opert;
	cEntry3->SetEntryL(KMsvDeletedEntryFolderEntryId);

	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==1);
	test(cEntry4->Count()==1);
	
	// change/remove the entry
	entryD = (*cEntry3)[0];
	entryD.DecPcSyncCount();
	cEntry3->SetEntryL(entryD.Id());
	active->StartL();
	entryOb3->Start();
	opert = cEntry3->ChangeL(entryD, active->iStatus);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(opert->iStatus==KErrNone);
	delete opert;
	cEntry3->SetEntryL(KMsvDeletedEntryFolderEntryId);

	// test
	test(cEntry1->Count()==0);
	test(cEntry2->Count()==0);
	test(cEntry3->Count()==0);
	test(cEntry4->Count()==0);
	
	CleanupStack::PopAndDestroy(9); // centry1-4, session, entryOb1/3, active, selection
	}


LOCAL_C void TestRemovingAllFilesL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	CMsvEntrySelection* selection;
	CMsvOperation* opert;
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	CDir* dir;
	TFileName filename;
	TInt count;


	// delete everything under the outbox
	cEntry1->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// delete everything under the inbox
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;
	
	// delete everything under the draft
	cEntry1->SetEntryL(KMsvDraftEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;
	
	// delete everything under the sent
	cEntry1->SetEntryL(KMsvSentEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// delete everything under the local service
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	selection = cEntry1->ChildrenL();
	count=selection->Count();
	while (count--)
		{
		TMsvId id = selection->At(count);
		if (id==KMsvGlobalOutBoxIndexEntryId || 
			id==KMsvGlobalInBoxIndexEntryId || 
			id==KMsvDraftEntryId || 
			id==KMsvSentEntryId || 
			id==KMsvDeletedEntryFolderEntryId)
			selection->Delete(count);
		}
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// test
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==5);
	delete selection;
	TestNumberOfFilesInService(KMsvLocalServiceIndexEntryId,0);

	// everything under the root
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	count=selection->Count();
	while (count--)
		{
		TMsvId id = selection->At(count);
		if (id==KMsvLocalServiceIndexEntryId)
				selection->Delete(count);
		}
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// test
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==1);
	delete selection;
	filename=KDrive;
	filename.Append(KMsvDefaultFolder2);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
	test(dir->Count()==1);
	delete dir;


	CleanupStack::PopAndDestroy(4); 
	}


LOCAL_C void TestEntryStream(CMsvEntry& aEntry, TMsvId aId, TMsvId aService, TBool aAllDescendents)
	{
	// get the store
	aEntry.SetEntryL(aId);
	CMsvStore* store=NULL;
	if(aEntry.HasStoreL()) store = aEntry.ReadStoreL();
		test(store==NULL || store->IsPresentL(KUidMsvHiddenEntryStream)==EFalse);
		delete store;

	// recurse into the children
	if (aAllDescendents)
		{
		CMsvEntrySelection* selection = aEntry.ChildrenL();
		TInt count=selection->Count();
		while (count--)
			TestEntryStream(aEntry, selection->At(count), aService, EFalse);
		delete selection;
		}
	}


LOCAL_C void TestHiddenEntryStreamsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);
	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
	cEntry1->SetSortTypeL(sort);

	// all services
	CMsvEntrySelection* selection = cEntry1->ChildrenL();
	TInt count=selection->Count();
	while (count--)
		TestEntryStream(*cEntry1, selection->At(count), selection->At(count), EFalse);
	delete selection;
	
	// local entries
	TestEntryStream(*cEntry1, KMsvLocalServiceIndexEntryId, KMsvLocalServiceIndexEntryId, ETrue);

	CleanupStack::PopAndDestroy(2); // cEntry1,session
	}


LOCAL_C void TestWaitL()
	{
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(0x00B00000); // has to be longer than KMsvDelayTime
	CActiveScheduler::Start();
	delete timer;
	}


LOCAL_C void TestRemovedEntriesL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	TInt count = theRemovedEntries->Count();
	while (count--)
		{
		TRAPD(error, cEntry1->SetEntryL(theRemovedEntries->At(count)));
		test(error==KErrNotFound);
		}

	CleanupStack::PopAndDestroy(2); // cEntry1,session
	}


LOCAL_C void TestAsyncOpeningL()
	{
	CDummyObserver ob;
	CSessionObserver* ob1 = new(ELeave) CSessionObserver;
	CSessionObserver* ob2 = new(ELeave) CSessionObserver;
	CMsvSession* session;
	CMsvSession* session2;

	// async when starting the server from nothing
	CMsgsTestUtils::CleanMessageFolderL();
	session = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;

	// async when starting the server has already started
	session = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;

	// two async starts
	session = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	session2 = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;
	delete session2;
	
	// close the server
	session = CMsvSession::OpenSyncL(ob);
	session->CloseMessageServer();
	delete session;

	// Wait until the server closes
	User::After(500000);

	// two async starts
	session = CMsvSession::OpenAsyncL(*ob1);
	session2 = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(ob1->iType==MMsvSessionObserver::EMsvServerReady);
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;
	delete session2;
	
	// close the server
	session = CMsvSession::OpenSyncL(ob);
	session->CloseMessageServer();
	delete session;

	// Wait until the server closes
	User::After(500000);

	// async when starting the server has previously run
	session = CMsvSession::OpenAsyncL(*ob2);
	CActiveScheduler::Start();
	test(ob2->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;

	// delete before the notification
	session = CMsvSession::OpenAsyncL(*ob2);
	delete session;

	User::After(5000000);
	delete ob1;
	delete ob2;
	}


LOCAL_C void TestSyncOpeningL()
	{
	CDummyObserver ob;
	CMsvSession* session;
	CMsvSession* session2;
	
	// close the server
	session	= CMsvSession::OpenSyncL(ob);
	session->CloseMessageServer();
	delete session;

	// Wait until the server closes
	User::After(500000);

	// async when starting the server from nothing
	CMsgsTestUtils::CleanMessageFolderL();
	session = CMsvSession::OpenSyncL(ob);
	delete session;

	// async when starting the server has already started
	session = CMsvSession::OpenSyncL(ob);
	delete session;

	// start two session
	session = CMsvSession::OpenSyncL(ob);
	session2= CMsvSession::OpenSyncL(ob);
	delete session;
	delete session2;

	// close the server
	session	= CMsvSession::OpenSyncL(ob);
	session->CloseMessageServer();
	delete session;

	// Wait until the server closes
	User::After(500000);

	// async when starting the server has previously run
	session = CMsvSession::OpenSyncL(ob);
	delete session;

	}



LOCAL_C void TestLocalMoveContextProblemL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb1);;
	cEntry1->AddObserverL(*entryOb1);

	CEntryObserver* entryOb2=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb2);;
	cEntry2->AddObserverL(*entryOb2);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// create two folders
	TMsvEntry folder1;	
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry folder2;	
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(folder2, *cEntry1, KErrNone, *entryOb1);

	// create an entry
	cEntry1->SetEntryL(folder1.Id());
	TMsvEntry entry;	
	entry.iType = KUidMsvFolderEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry, *cEntry1, KErrNone, *entryOb1);

	// set cEntry2 to the entry
	cEntry2->SetEntryL(entry.Id());

	// move entry to folder2
	opert = cEntry1->MoveL(entry.Id(), folder2.Id(), active->iStatus);
	active->StartL();
	entryOb2->Start();
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	// test the operation
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;
	// test the observer
	test(entryOb2->iEvent==MMsvEntryObserver::EMsvEntryMoved);	

	CleanupStack::PopAndDestroy(6); // active entryOb2, entryOb1 cEntry2, cEntry1, session
	}



LOCAL_C void TestLocalMovingWithDescendentsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb1);;
	cEntry1->AddObserverL(*entryOb1);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// create two folders
	TMsvEntry folder1;	
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(folder1, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry folder2;	
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(folder2, *cEntry1, KErrNone, *entryOb1);

	// create two entries
	cEntry1->SetEntryL(folder1.Id());
	TMsvEntry entry1;	
	entry1.iType = KUidMsvFolderEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry1, *cEntry1, KErrNone, *entryOb1);
	TMsvEntry entry2;	
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry2, *cEntry1, KErrNone, *entryOb1);

	// create two attachments
	cEntry1->SetEntryL(entry1.Id());
	TMsvEntry attach1;	
	attach1.iType = KUidMsvFolderEntry;
	attach1.iServiceId = KMsvLocalServiceIndexEntryId;
	attach1.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(attach1, *cEntry1, KErrNone, *entryOb1);
	cEntry1->SetEntryL(entry2.Id());
	TMsvEntry attach2;	
	attach2.iType = KUidMsvFolderEntry;
	attach2.iServiceId = KMsvLocalServiceIndexEntryId;
	attach2.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(attach2, *cEntry1, KErrNone, *entryOb1);


	// test 
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Count()==2);
	cEntry1->SetEntryL(folder2.Id());
	test(cEntry1->Count()==0);

	// move entries to folder2
	cEntry1->SetEntryL(folder1.Id());
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	opert = cEntry1->MoveL(*selection, folder2.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	// test the operation
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==2);
	test(package().iNumberCompleted==2);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;
	delete selection;

	// test 
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Count()==0);
	cEntry1->SetEntryL(folder2.Id());
	test(cEntry1->Count()==2);


	// create another entry
	cEntry1->SetEntryL(folder2.Id());
	TMsvEntry entry3;	
	entry3.iType = KUidMsvFolderEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(entry3, *cEntry1, KErrNone, *entryOb1);

	// create two attachments
	cEntry1->SetEntryL(entry3.Id());
	TMsvEntry attach3;	
	attach3.iType = KUidMsvFolderEntry;
	attach3.iServiceId = KMsvLocalServiceIndexEntryId;
	attach3.iMtm = KUidMsvLocalServiceMtm;
	CreateEntry(attach3, *cEntry1, KErrNone, *entryOb1);

	// test 
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Count()==0);
	cEntry1->SetEntryL(folder2.Id());
	test(cEntry1->Count()==3);

	// move entries to folder1
	cEntry1->SetEntryL(folder2.Id());
	selection = new(ELeave)CMsvEntrySelection;
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());
	opert = cEntry1->MoveL(*selection, folder1.Id(), active->iStatus);
	active->StartL();
	CActiveScheduler::Start();
	// test the operation
	test(opert->iStatus.Int()==KErrNone);
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalMove);
	test(package().iTotalNumberOfEntries==3);
	test(package().iNumberCompleted==3);
	test(package().iNumberFailed==0);
	test(package().iNumberRemaining==0);
	delete opert;
	delete selection;

	// test 
	cEntry1->SetEntryL(folder1.Id());
	test(cEntry1->Count()==3);
	cEntry1->SetEntryL(folder2.Id());
	test(cEntry1->Count()==0);

	CleanupStack::PopAndDestroy(4); // active entryOb1 cEntry1, session
	}

LOCAL_C void CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aMtmFilename)
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(aUid, aUid, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(), aFilename, EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent, aUid);
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

LOCAL_C TInt GetMtmCountL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CClientMtmRegistry* reg = CClientMtmRegistry::NewL(*session);
	TInt count = reg->NumRegisteredMtmDlls();
	delete reg;

	CleanupStack::PopAndDestroy(2); // session, ob
	return count;
	}

LOCAL_C void TestMtmCountL(TInt count)
	{
	TInt mtmCount = GetMtmCountL();
	test(mtmCount == count);
	}

LOCAL_C void TestInstallUninstallL()
	{
	theUtils->FileSession().Delete(KMtmDataFile1);
	theUtils->FileSession().Delete(KMtmDataFile2);
	theUtils->FileSession().Delete(KMtmRegistry);

	CreateDataFilesL(KMtmDataFile1, KMtmName1, KUidTestMtm1, KTestMtm1Filename); 
	CreateDataFilesL(KMtmDataFile2, KMtmName2, KUidTestMtm2, KTestMtm2Filename); 

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session;

	TInt mtmCount = GetMtmCountL();

	session = CMsvSession::OpenSyncL(*ob);
	session->InstallMtmGroup(KMtmDataFile1);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount + 1);

	session = CMsvSession::OpenSyncL(*ob);
	session->InstallMtmGroup(KMtmDataFile2);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount + 2);

	session = CMsvSession::OpenSyncL(*ob);
	session->DeInstallMtmGroup(KMtmDataFile2);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount + 1);

	session = CMsvSession::OpenSyncL(*ob);
	session->DeInstallMtmGroup(KMtmDataFile1);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount);

	session = CMsvSession::OpenSyncL(*ob);
	session->InstallMtmGroup(KMtmDataFile1);
	session->InstallMtmGroup(KMtmDataFile2);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount + 2);

	session = CMsvSession::OpenSyncL(*ob);
	session->DeInstallMtmGroup(KMtmDataFile1);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount + 1);

	session = CMsvSession::OpenSyncL(*ob);
	session->DeInstallMtmGroup(KMtmDataFile2);
	session->CloseMessageServer();
	delete session;

	TestMtmCountL(mtmCount);

	CleanupStack::PopAndDestroy(); // ob
	}


/**

  This tests that sending states of messages in the outbox are changed to 
  suspeneded on startup. Those that should change are


enum TMsvSendState
{
KMsvSendStateUnknown		= 0x0,
KMsvSendStateUponRequest	= 0x1,
KMsvSendStateWaiting		= 0x2,
KMsvSendStateSending		= 0x3,  <---
KMsvSendStateScheduled		= 0x4,
KMsvSendStateResend			= 0x5,
KMsvSendStateSuspended		= 0x6,
KMsvSendStateFailed			= 0x7,
KMsvSendStateSent			= 0x8,
KMsvSendStateNotApplicable	= 0x9

  and these is type is fax
enum TFaxSendingState
{
EFaxSendingStateCalling = (KMsvSendStateLast + 1),
EFaxSendingStateNegotiating,
EFaxSendingStatePreparing
};
};


  */
  
  const TUid KUidMsgTypeFax =	{0x1000102B};
  
  // copied from fax code.
  enum TFaxSendingState
	  {
	  EFaxSendingStateCalling = (KMsvSendStateLast + 1),
		  EFaxSendingStateNegotiating,
		  EFaxSendingStatePreparing
	  };




LOCAL_C void TestSuspendedEntriesL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalOutBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	
	TMsvEntry entryKMsvSendStateUnknown;
	entryKMsvSendStateUnknown.iType = KUidMsvMessageEntry;
	entryKMsvSendStateUnknown.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateUnknown.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateUnknown.SetSendingState(KMsvSendStateUnknown);
	
	TMsvEntry entryKMsvSendStateUponRequest;
	entryKMsvSendStateUponRequest.iType = KUidMsvMessageEntry;
	entryKMsvSendStateUponRequest.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateUponRequest.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateUponRequest.SetSendingState(KMsvSendStateUponRequest);
	
	TMsvEntry entryKMsvSendStateWaiting;
	entryKMsvSendStateWaiting.iType = KUidMsvMessageEntry;
	entryKMsvSendStateWaiting.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateWaiting.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateWaiting.SetSendingState(KMsvSendStateWaiting);
	
	TMsvEntry entryKMsvSendStateSending;
	entryKMsvSendStateSending.iType = KUidMsvMessageEntry;
	entryKMsvSendStateSending.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateSending.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateSending.SetSendingState(KMsvSendStateSending);
	
	TMsvEntry entryKMsvSendStateScheduled;
	entryKMsvSendStateScheduled.iType = KUidMsvMessageEntry;
	entryKMsvSendStateScheduled.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateScheduled.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateScheduled.SetSendingState(KMsvSendStateScheduled);
	
	TMsvEntry entryKMsvSendStateResend;
	entryKMsvSendStateResend.iType = KUidMsvMessageEntry;
	entryKMsvSendStateResend.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateResend.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateResend.SetSendingState(KMsvSendStateResend);
	
	TMsvEntry entryKMsvSendStateSuspended;
	entryKMsvSendStateSuspended.iType = KUidMsvMessageEntry;
	entryKMsvSendStateSuspended.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateSuspended.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateSuspended.SetSendingState(KMsvSendStateSuspended);
	
	TMsvEntry entryKMsvSendStateFailed;
	entryKMsvSendStateFailed.iType = KUidMsvMessageEntry;
	entryKMsvSendStateFailed.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateFailed.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateFailed.SetSendingState(KMsvSendStateFailed);
	
	TMsvEntry entryKMsvSendStateSent;
	entryKMsvSendStateSent.iType = KUidMsvMessageEntry;
	entryKMsvSendStateSent.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateSent.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateSent.SetSendingState(KMsvSendStateSent);
	
	TMsvEntry entryKMsvSendStateNotApplicable;
	entryKMsvSendStateNotApplicable.iType = KUidMsvMessageEntry;
	entryKMsvSendStateNotApplicable.iMtm = KUidMsvLocalServiceMtm;
	entryKMsvSendStateNotApplicable.iServiceId = KMsvLocalServiceIndexEntryId;
	entryKMsvSendStateNotApplicable.SetSendingState(KMsvSendStateNotApplicable);
	
	TMsvEntry entryEFaxSendingStateCalling;
	entryEFaxSendingStateCalling.iType = KUidMsvMessageEntry;
	entryEFaxSendingStateCalling.iMtm = KUidMsgTypeFax;
	entryEFaxSendingStateCalling.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStateCalling.SetSendingState(EFaxSendingStateCalling);
	
	TMsvEntry entryEFaxSendingStateNegotiating;
	entryEFaxSendingStateNegotiating.iType = KUidMsvMessageEntry;
	entryEFaxSendingStateNegotiating.iMtm = KUidMsgTypeFax;
	entryEFaxSendingStateNegotiating.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStateNegotiating.SetSendingState(EFaxSendingStateNegotiating);
	
	TMsvEntry entryEFaxSendingStatePreparing;
	entryEFaxSendingStatePreparing.iType = KUidMsvMessageEntry;
	entryEFaxSendingStatePreparing.iMtm = KUidMsgTypeFax;
	entryEFaxSendingStatePreparing.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStatePreparing.SetSendingState(EFaxSendingStatePreparing);
	

	TMsvEntry entryEFaxSendingStateCallingNonFax;
	entryEFaxSendingStateCallingNonFax.iType = KUidMsvMessageEntry;
	entryEFaxSendingStateCallingNonFax.iMtm = KUidMsvLocalServiceMtm;
	entryEFaxSendingStateCallingNonFax.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStateCallingNonFax.SetSendingState(EFaxSendingStateCalling);
	
	TMsvEntry entryEFaxSendingStateNegotiatingNonFax;
	entryEFaxSendingStateNegotiatingNonFax.iType = KUidMsvMessageEntry;
	entryEFaxSendingStateNegotiatingNonFax.iMtm = KUidMsvLocalServiceMtm;
	entryEFaxSendingStateNegotiatingNonFax.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStateNegotiatingNonFax.SetSendingState(EFaxSendingStateNegotiating);
	
	TMsvEntry entryEFaxSendingStatePreparingNonFax;
	entryEFaxSendingStatePreparingNonFax.iType = KUidMsvMessageEntry;
	entryEFaxSendingStatePreparingNonFax.iMtm = KUidMsvLocalServiceMtm;
	entryEFaxSendingStatePreparingNonFax.iServiceId = KMsvLocalServiceIndexEntryId;
	entryEFaxSendingStatePreparingNonFax.SetSendingState(EFaxSendingStatePreparing);
		
	// create entries with each possible return state.

	cEntry->CreateL(entryKMsvSendStateUnknown);
	cEntry->CreateL(entryKMsvSendStateUponRequest);
	cEntry->CreateL(entryKMsvSendStateWaiting);
	cEntry->CreateL(entryKMsvSendStateSending);
	cEntry->CreateL(entryKMsvSendStateScheduled);
	cEntry->CreateL(entryKMsvSendStateResend);
	cEntry->CreateL(entryKMsvSendStateSuspended);
	cEntry->CreateL(entryKMsvSendStateFailed);
	cEntry->CreateL(entryKMsvSendStateSent);
	cEntry->CreateL(entryKMsvSendStateNotApplicable);
	cEntry->CreateL(entryEFaxSendingStateCalling);
	cEntry->CreateL(entryEFaxSendingStateNegotiating);
	cEntry->CreateL(entryEFaxSendingStatePreparing);
	cEntry->CreateL(entryEFaxSendingStateCallingNonFax);
	cEntry->CreateL(entryEFaxSendingStateNegotiatingNonFax);
	cEntry->CreateL(entryEFaxSendingStatePreparingNonFax);
	
	
	session->CloseMessageServer();
	// close session and wait for server to shut down;
	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);
	
	User::After(20000000);


		// start server again. 
	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	cEntry = CMsvEntry::NewL(*session, KMsvGlobalOutBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// delay because the server does the change on idle after it starts.
	User::After(10000);
	// these should have changed
	cEntry->SetEntryL(entryKMsvSendStateSending.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	cEntry->SetEntryL(entryEFaxSendingStateCalling.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	cEntry->SetEntryL(entryEFaxSendingStateNegotiating.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	cEntry->SetEntryL(entryEFaxSendingStatePreparing.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	cEntry->SetEntryL(entryKMsvSendStateWaiting.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	
	// these shouldn't
	cEntry->SetEntryL(entryKMsvSendStateUnknown.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateUnknown);
	cEntry->SetEntryL(entryKMsvSendStateUponRequest.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateUponRequest);
	cEntry->SetEntryL(entryKMsvSendStateScheduled.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateScheduled);
	cEntry->SetEntryL(entryKMsvSendStateResend.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateResend);
	cEntry->SetEntryL(entryKMsvSendStateSuspended.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSuspended);
	cEntry->SetEntryL(entryKMsvSendStateFailed.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateFailed);
	cEntry->SetEntryL(entryKMsvSendStateSent.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateSent);
	cEntry->SetEntryL(entryKMsvSendStateNotApplicable.Id());
	test(cEntry->Entry().SendingState()==KMsvSendStateNotApplicable);
	// non fax messages with sending states > KMsvSendStateLast
	// should stay unchanged
	cEntry->SetEntryL(entryEFaxSendingStateCallingNonFax.Id());
	test(cEntry->Entry().SendingState()==EFaxSendingStateCalling);
	cEntry->SetEntryL(entryEFaxSendingStateNegotiatingNonFax.Id());
	test(cEntry->Entry().SendingState()==EFaxSendingStateNegotiating);
	cEntry->SetEntryL(entryEFaxSendingStatePreparingNonFax.Id());
	test(cEntry->Entry().SendingState()==EFaxSendingStatePreparing);
	

	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(ob);
	}

LOCAL_C void TestChangedEntriesL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry entry1;
	entry1.iType = KUidMsvFolderEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry entry2;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry entry3;
	entry3.iType = KUidMsvFolderEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry other;
	other.iType = KUidMsvMessageEntry;
	other.iMtm = KUidMsvLocalServiceMtm;
	other.iServiceId = KMsvLocalServiceIndexEntryId;

	// Create 3 folders with 2 entries in each

	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());
	cEntry->CreateL(entry1);
	cEntry->CreateL(other);

	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());
	cEntry->CreateL(entry2);
	cEntry->CreateL(other);

	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());
	cEntry->CreateL(entry3);
	cEntry->CreateL(other);

	// Get current time and add one minute
	TTime now;
	now.UniversalTime();

	// Wait until minute changes
	TInt wait = 60 - now.DateTime().Second();
	test.Printf(_L("** Waiting %d seconds **\n"), wait);
	User::After(wait * 1000000);
	
	CMsvEntrySelection* result = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(result);

	CMessageQuery* query = CMessageQuery::NewLC(*session);

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	now.UniversalTime();
	filter->SetLastChangeDate(now);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Get list of changed entries (should be none)
	active->StartL();
	query->GetItemIdsL(*filter, *result, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus == KErrNone);
	test(result->Count() == 0);

	entry1.iDescription.Set(KLongDescription);
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Get list of changed entries
	active->StartL();
	query->GetItemIdsL(*filter, *result, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus == KErrNone);
	test(result->Count() == 1);

	// Test we got the expected results
	test(result->At(0) == entry1.Id());
	result->Reset();

	entry2.iDescription.Set(KLongDescription);
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Get list of changed entries
	active->StartL();
	query->GetItemIdsL(*filter, *result, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus == KErrNone);
	test(result->Count() == 2);

	// Test we got the expected results
	test(result->At(0) == entry1.Id());
	test(result->At(1) == entry2.Id());
	result->Reset();

	entry3.iDescription.Set(KLongDescription);
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Get list of changed entries
	active->StartL();
	query->GetItemIdsL(*filter, *result, active->iStatus);
	CActiveScheduler::Start();
	test(active->iStatus == KErrNone);
	test(result->Count() == 3);

	// Test we got the expected results
	test(result->At(0) == entry1.Id());
	test(result->At(1) == entry2.Id());
	test(result->At(2) == entry3.Id());
	result->Reset();

	CleanupStack::PopAndDestroy(7); // active, filter, query, result, cEntry, session, ob
	}


LOCAL_C void TestRetrievingMtmCapabilitySetL()
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	TInt ret=session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);

	TCapabilitySet mtmRequiredCaps;
	session->GetMtmRequiredCapabilitiesL(KUidTestServerMtmType,mtmRequiredCaps);
	for(TUint ii=ECapabilityTCB ; ii<ECapability_HardLimit; ++ii)
		{
		TCapability cap = static_cast<TCapability>(ii);
		if(cap == ECapabilityLocalServices || cap == ECapabilityPowerMgmt)
			{
			test(mtmRequiredCaps.HasCapability(cap));
			}
		else
			{			
			test(!mtmRequiredCaps.HasCapability(cap));
			}
		}
	
	CleanupStack::PopAndDestroy(2, ob); // session, ob
	}


void doTestsL()
	{
	theUtils->FileSession().Delete(KBlockServerMtmStartFile);
	theRemovedEntries = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(theRemovedEntries);

	CreateMtmFilesL();

	User::After(20000000);
	CMsgsTestUtils::Start(_L("Connecting to server"));
	TestAsyncOpeningL();	
	TestSyncOpeningL();	
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Utility classes"));
	TestDogL();
	TestCompletedOperationL();
	TestMcliUtilsL();
	CMsgsTestUtils::Complete();

	User::After(20000000);
	CMsgsTestUtils::Next(_L("Entry creation"));
	TestClientCreationL();	
	TestEntryCreationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry changing"));
	TestEntryChangingL();
	TestChangeAttributeNotifyL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Children selection"));
	TestChildrenSelectionL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Children accessors"));
	TestChildrenAccessorL();
	TestChildrenSorting1L();
	TestChildrenSorting2L();
	TestChildrenSelectionsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Children changing"));
	TestChildrenChangingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry deletion"));
	TestEntryDeletionL();
	TestServiceDeletionL();
	TestRecursiveDeletionL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Message stores"));
	TestMessageStoresL();
	CMsgsTestUtils::Complete();


	CMsgsTestUtils::Next(_L("Local entry moving"));
	TestLocalMovingL();
	TestLocalMovingWithStoreFilesOpenL();
	TestLocalMoveContextProblemL();
	TestLocalMovingWithDescendentsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Local moving entry after CMsvEntry::ReadStoreL fails"));
	TestMovingAfterReadStoreLeavesL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Observers"));
	TestSessionObserversL();
	TestEntryObserversL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Local entry copying"));
	TestLocalCopyingL();
	TestLocalCopyingToDescendentL();
	TestLocalComplexCopyingL();
	TestCancelCopyL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Installing and uninstalling mtm's"));
	TestInstallUninstallL();
	CMsgsTestUtils::Complete();
	
	
	test.Printf(_L("** Waiting for delayed updating **\n"));
	TestWaitL();
	TestRemovedEntriesL();

	CMsgsTestUtils::Next(_L("Server Mtm"));
	TestNonExistantServerMtmL();
	TestServerMtmL();
	TestServerMtmSyncL();
	TestServerMtmMultipleOpsL();
	TestServiceFunctionsL();
	TestContextMovingServicesL();
	#ifdef _DEBUG_	//Panics as the MTM gets deleted in Debug mode
	TestServerMtmFailsToLoadL();
	#endif
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Stores"));
	TestLocalStoresL();
	TestServiceStoresL();
	TestRemoteStoresL();
	TestRemoteContextStoreFlagL();
	TestRemoteChildrenStoreFlagL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Flags"));
	TestContextOwnerFlagL();
	TestChildrenOwnerFlagL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mtm list & invisible entries"));
	TestMtmListL();
	TestInvisibleEntriesL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("PC syncing"));
	TestPcSyncingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry removal"));
	TestEntryRemoveL();
	TestEntryCleanupL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Scheduled Sending"));
	TestScheduledSendingL();
	CMsgsTestUtils::Complete();

	test.Printf(_L("** Waiting 10s for delayed updating **\n"));
	TestWaitL();
	TestRemovedEntriesL();

	CMsgsTestUtils::Next(_L("Hidden entry stream"));
	TestHiddenEntryStreamsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Destructive Observer"));
	TestSessionObserversDestructingL();
	CMsgsTestUtils::Complete();
	
	test.Printf(_L("** Waiting for delayed updating **\n"));
	TestWaitL();
	TestRemovedEntriesL();
		
	CMsgsTestUtils::Next(_L("Changing state of messages in inbox on msgs startup"));
	TestSuspendedEntriesL();
	CMsgsTestUtils::Complete();
	
	test.Printf(_L("** Waiting for delayed updating **\n"));
	TestWaitL();

	CMsgsTestUtils::Next(_L("Installing and uninstalling mtm's"));
	TestInstallUninstallL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Retrieving changed entries"));
	TestChangedEntriesL();
	CMsgsTestUtils::Complete();

	User::After(20000000);
	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...and checking for orphan files\n"));
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Corrupt Mailbox"));
	TestCorruptMessageStoreDeleteL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Retrieving MTM capability set"));
	TestRetrievingMtmCapabilitySetL();
	CMsgsTestUtils::Complete();
	
	CleanupStack::PopAndDestroy(); // theRemovedEntries		
	}

#pragma warning( default : 4702 ) 
#pragma warning( default : 4238 ) 

