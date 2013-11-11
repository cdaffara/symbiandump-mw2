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

#include <e32uid.h>
#include <txtrich.h>

#include "Testsetup.h"
#include <msvipc.h>
#include "MSERVER.H"
#include <msvreg.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvfind.h>
#include <mtmdef.h>
#include <mtclreg.h>

RTest test(_L("Find Testrig"));

_LIT(KShortDescription, "find some text in here");
_LIT(KShortDetails, "find some text in here");
_LIT(KMatchingDescription, "abcDEFghi Jkl mno");
_LIT(KMatchingDetails, "abcDEFghi Jkl mno");

_LIT(KFindMtmDataFile, "c:\\system\\mtm\\FINDMTM.dat");
_LIT(KFindMtmName, "Find test MTM");
_LIT(KBrokenFindMtmDataFile, "c:\\system\\mtm\\BROKENFINDMTM.dat");
_LIT(KBrokenFindMtmName, "Broken find MTM");
_LIT(KMessageStorePath, "c:\\private\\1000484b\\mail2\\00001001_S\\");

_LIT(KFindMtmFilename, "z:\\system\\libs\\findmtm.dll");
_LIT(KBrokenFindMtmFilename, "z:\\system\\libs\\brokenfindmtm.dll");

// Note, these UIDs are used as the MTM type UID, the technology type uid,
// the MTM dll 3rd UID *and* the registry data file 3rd UID
const TUid KUidFindTestMtm       = {0x10005126};
const TUid KUidBrokenFindTestMtm = {0x10005127};

//

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iEvent;
	};

void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	}

//**********************************
// TMsvFindResultDateRange
//**********************************
//
// An enumeration that specifies the date comparison to perform
//

enum TMsvFindResultDateRange
	{
	EMsvOnDate,
	EMsvBeforeDate,
	EMsvAfterDate
	};

//**********************************
// CMsvFindDateOperation
//**********************************
//
// An operation that extends global find to restrict searching by date
//

class CMsvFindDateOperation : public CMsvFindOperation
	{
public:
	static CMsvFindDateOperation* FindInSelectionL(CMsvSession& aSession, const TDesC& aTextToFind, const CMsvEntrySelection& aSel, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus);
	static CMsvFindDateOperation* FindInChildrenL(CMsvSession& aSession, const TDesC& aTextToFind, TMsvId aId, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus);
	//
protected:
	CMsvFindDateOperation(CMsvSession& aSession, const TDesC& aTextToFind, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus);
	//
private:
	TBool IsValid(const TMsvEntry& aEntry) const;
	//
private:
	TTime iDate;
	TMsvFindResultDateRange iRange;
	};

CMsvFindDateOperation* CMsvFindDateOperation::FindInSelectionL(CMsvSession& aSession, const TDesC& aTextToFind, const CMsvEntrySelection& aSel, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus)
//
//
//
	{
	CMsvFindDateOperation* self = new(ELeave)CMsvFindDateOperation(aSession, aTextToFind, aPartList, aDate, aRange, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructFindInSelectionL(aSel);
	CleanupStack::Pop(); // self
	return self;	
	}

CMsvFindDateOperation* CMsvFindDateOperation::FindInChildrenL(CMsvSession& aSession, const TDesC& aTextToFind, TMsvId aId, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus)
//
// 
//
	{
	CMsvFindDateOperation* self = new(ELeave)CMsvFindDateOperation(aSession, aTextToFind, aPartList, aDate, aRange, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructFindInChildrenL(aId);
	CleanupStack::Pop(); // self
	return self;	
	}

CMsvFindDateOperation::CMsvFindDateOperation(CMsvSession& aSession, const TDesC& aTextToFind, TMsvPartList aPartList, const TTime& aDate, TMsvFindResultDateRange aRange, TRequestStatus& aObserverRequestStatus)
: CMsvFindOperation(aSession, aTextToFind, aPartList, aObserverRequestStatus), iDate(aDate), iRange(aRange)
	{
	}

TBool CMsvFindDateOperation::IsValid(const TMsvEntry& aEntry) const
//
//
//
	{
	TDateTime dt;
		
	dt = aEntry.iDate.DateTime();
	TTime date1(TDateTime(dt.Year(), dt.Month(), dt.Day(), 0, 0, 0, 0));

	dt = iDate.DateTime();
	TTime date2(TDateTime(dt.Year(), dt.Month(), dt.Day(), 0, 0, 0, 0));

	switch (iRange)
		{
		case EMsvOnDate:
			return date1 == date2;
		case EMsvBeforeDate:
			return date1 < date2;
		case EMsvAfterDate:
			return date1 > date2;
		}
	return EFalse;
	}

//

LOCAL_C void SaveMessageL(CMsvSession& aSession, TMsvEntry aEntry, TBool aRichText = EFalse)
	{

	CClientMtmRegistry* clientRegistry = CClientMtmRegistry::NewL(aSession);
	CleanupStack::PushL(clientRegistry);

	CBaseMtm* baseMtm = clientRegistry->NewMtmL(aEntry.iMtm);
	CleanupStack::PushL(baseMtm);

	// SetCurrentEntryL takes ownership of this
	CMsvEntry* entry = CMsvEntry::NewL(aSession, aEntry.Id(), TMsvSelectionOrdering());
	baseMtm->SetCurrentEntryL(entry);
	if (aRichText)
		baseMtm->Body().InsertL(0, _L("Some Rich Text"));
	baseMtm->SaveMessageL();

	CleanupStack::PopAndDestroy(2); // baseMtm, clientRegistry
	}

LOCAL_C void TestFindInSelectionL(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvFindOperation* find = NULL;
	TInt error;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(folder.Id());
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	// Test invalid arguments
	TRAP(error, CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus));
	test(error == KErrArgument);

	selection->Delete(0);
	selection->InsertL(1, folder.Id());

	// Test find in children delete straight away.
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus);
	delete find;	

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrArgument);
	test(progress().iCompleted == 1);
	test(progress().iRemaining == 3);

	selection->Delete(1);
	selection->InsertL(2, folder.Id());

	// Test results
	test(find->GetFindResult().Count() == 1);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrArgument);
	test(progress().iCompleted == 2);
	test(progress().iRemaining == 2);

	selection->Delete(2);
	selection->InsertL(3, folder.Id());

	// Test results
	test(find->GetFindResult().Count() == 2);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription,  active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrArgument);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 1);

	selection->Delete(3);

	// Test results
	test(find->GetFindResult().Count() == 3);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription,  active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);

	TInt count;
	for(count = 0; count < selection->Count(); count++)
		{
		test(selection->At(count) == find->GetFindResult().At(count).iId);
		test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);
		}

	CleanupStack::PopAndDestroy(6); // find, selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInSelectionWithHeapFail1L(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvFindOperation* find = NULL;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TInt error;
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// Test find in selection
		TRAP(error, find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus));
		
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				__UHEAP_RESET;
	
				finished = ETrue;

				// Test the progress
				test(progress().iCompleted == 3);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrNoMemory);				

			TInt count;

			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 3);

			for(count = 0; count < find->GetFindResult().Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);
				}

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInSelectionWithHeapFail2L(TUid aMtm)
//
//
//
	{
	RMsvServerSession server;
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvFindOperation* find = NULL;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		// Test find in selection
		TRAP(error, find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus));
		
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				server.SetFailure(EHeapFailure, RHeap::ENone);
	
				finished = ETrue;

				// Test the progress
				test(progress().iCompleted == 3);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrNoMemory);

			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 3);

			TInt count;
			for(count = 0; count < find->GetFindResult().Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);
				}

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInSelectionWithFileFailL(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvFindOperation* find = NULL;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Test find in selection
		TRAP(error, find = CMsvFindOperation::FindInSelectionL(*session, _L("Text"), *selection, KMsvMessagePartDescription, active->iStatus));
		
		theUtils->FileSession().SetErrorCondition(KErrNone);
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);
	
				finished = ETrue;

				// Test the progress
				test(progress().iCompleted == 3);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrGeneral);

			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 3);

			TInt count;
			for(count = 0; count < find->GetFindResult().Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);
				}

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrGeneral);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenL(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under (Created on heap because I was running out of stack)
	TMsvEntry* folderX = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderX);
	folderX->iType = KUidMsvFolderEntry;
	folderX->iMtm = KUidMsvLocalServiceMtm;
	folderX->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderX);
	cEntry->SetEntryL(folderX->Id());

	TMsvEntry* entryX1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX1);
	entryX1->iDescription.Set(KShortDescription);
	entryX1->iDetails.Set(KShortDetails);
	entryX1->iType = KUidMsvMessageEntry;
	entryX1->iMtm = aMtm;
	entryX1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX1);
	SaveMessageL(*session, *entryX1);

	TMsvEntry* entryX2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX2);
	entryX2->iDescription.Set(KShortDescription);
	entryX2->iDetails.Set(KShortDetails);
	entryX2->iType = KUidMsvMessageEntry;
	entryX2->iMtm = aMtm;
	entryX2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX2);
	SaveMessageL(*session, *entryX2);

	TMsvEntry* entryX3 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX3);
	entryX3->iDescription.Set(KShortDescription);
	entryX3->iDetails.Set(KShortDetails);
	entryX3->iType = KUidMsvMessageEntry;
	entryX3->iMtm = aMtm;
	entryX3->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX3);
	SaveMessageL(*session, *entryX3);

	TMsvEntry* folderY = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderY);
	folderY->iType = KUidMsvFolderEntry;
	folderY->iMtm = KUidMsvLocalServiceMtm;
	folderY->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderY);
	cEntry->SetEntryL(folderY->Id());

	TMsvEntry* entryY1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY1);
	entryY1->iDescription.Set(KShortDescription);
	entryY1->iDetails.Set(KShortDetails);
	entryY1->iType = KUidMsvMessageEntry;
	entryY1->iMtm = aMtm;
	entryY1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY1);
	SaveMessageL(*session, *entryY1);

	TMsvEntry* entryY2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY2);
	entryY2->iDescription.Set(KShortDescription);
	entryY2->iDetails.Set(KShortDetails);
	entryY2->iType = KUidMsvMessageEntry;
	entryY2->iMtm = aMtm;
	entryY2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY2);
	SaveMessageL(*session, *entryY2);

	selection->AppendL(entryY1->Id());
	selection->AppendL(entryY2->Id());
	selection->AppendL(entryX1->Id());
	selection->AppendL(entryX2->Id());
	selection->AppendL(entryX3->Id());

	CMsvFindOperation* find = NULL;

	// Test invalid arguments
	TRequestStatus status;
	TRAPD(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), entryX1->Id(), KMsvMessagePartDescription, status));
	test(error == KErrArgument);

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX->Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 5);

	CMsvFindResultSelection* resSel = find->GetFindResult().CopyL();
	test(resSel->Count() == find->GetFindResult().Count());

	TInt count;
	for(count = 0; count < resSel->Count(); count++)
		{
		test(selection->At(count) == find->GetFindResult().At(count).iId);
		test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);

		test(resSel->At(count).iPartList == find->GetFindResult().At(count).iPartList);
		test(resSel->At(count).iId == find->GetFindResult().At(count).iId);
		test(resSel->Find(find->GetFindResult().At(count).iId) == count);
		}
	delete resSel;

	CleanupStack::PopAndDestroy(7); // All TMsvEntry's
	CleanupStack::PopAndDestroy(6); // selection, find, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenWithCorruptFileL(TUid aMtm)
// Test that the find operation will continue when a corrupt store
// is encountered. This is to test the fix for defect 
// DEF035985 - Find does not search Sent/Draft folder for SMS
	{
	TInt totalEntries  = 0;
	TInt positiveFinds = 0;

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under (Created on heap because I was running out of stack)
	TMsvEntry* folderX = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderX);
	folderX->iType = KUidMsvFolderEntry;
	folderX->iMtm = KUidMsvLocalServiceMtm;
	folderX->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderX);
	cEntry->SetEntryL(folderX->Id());

	TMsvEntry* entryX1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX1);
	entryX1->iDescription.Set(KShortDescription);
	entryX1->iDetails.Set(KShortDetails);
	entryX1->iType = KUidMsvMessageEntry;
	entryX1->iMtm = aMtm;
	entryX1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX1);
	SaveMessageL(*session, *entryX1);
	totalEntries++;  // Increment total entries
	positiveFinds++; // Increment expected successes on a find.

	TMsvEntry* entryX2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX2);
	entryX2->iDescription.Set(KShortDescription);
	entryX2->iDetails.Set(KShortDetails);
	entryX2->iType = KUidMsvMessageEntry;
	entryX2->iMtm = aMtm;
	entryX2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX2);
	SaveMessageL(*session, *entryX2);
	totalEntries++;  // Increment total entries
	positiveFinds++; // Increment expected successes on a find.

	// Get the filename for this entry
	TMsvId id;
	_LIT(KFormatHash, "%x");
	_LIT(KFormatEntry, "\\00%6x");
	TBuf<50> fileName;
	TBuf<20> fileTemp;
	fileName.Append(KMessageStorePath);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	id = (entryX2->Id() & 0x0FFFFFFF);
#else
	id = entryX2->Id();
#endif
	TInt hash = (id % 16);
	fileTemp.Format(KFormatHash, hash);
	fileName.Append(fileTemp);
	fileTemp.SetLength(0);
	fileTemp.Format(KFormatEntry, id);
	fileName.Append(fileTemp);

	// Delete the original file and replace with a corrupt one
	_LIT8(KCorruptData, "This is a corrupt message");
	RFs& fs = session->FileSession();
	fs.Delete(fileName);
	RFileWriteStream ws;
	ws.Create(fs, fileName, EFileWrite);
	ws.WriteL(KCorruptData);
	ws.Close();

	TMsvEntry* entryX3 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX3);
	entryX3->iDescription.Set(KShortDescription);
	entryX3->iDetails.Set(KShortDetails);
	entryX3->iType = KUidMsvMessageEntry;
	entryX3->iMtm = aMtm;
	entryX3->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX3);
	SaveMessageL(*session, *entryX3);
	totalEntries++; // Increment total entries

	TMsvEntry* folderY = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderY);
	folderY->iType = KUidMsvFolderEntry;
	folderY->iMtm = KUidMsvLocalServiceMtm;
	folderY->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderY);
	cEntry->SetEntryL(folderY->Id());

	TMsvEntry* entryY1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY1);
	entryY1->iDescription.Set(KShortDescription);
	entryY1->iDetails.Set(KShortDetails);
	entryY1->iType = KUidMsvMessageEntry;
	entryY1->iMtm = aMtm;
	entryY1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY1);
	SaveMessageL(*session, *entryY1);
	totalEntries++;  // Increment total entries
	positiveFinds++; // Increment expected successes on a find.

	TMsvEntry* entryY2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY2);
	entryY2->iDescription.Set(KShortDescription);
	entryY2->iDetails.Set(KShortDetails);
	entryY2->iType = KUidMsvMessageEntry;
	entryY2->iMtm = aMtm;
	entryY2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY2);
	SaveMessageL(*session, *entryY2);
	totalEntries++;  // Increment total entries
	positiveFinds++; // Increment expected successes on a find.

	selection->AppendL(entryY1->Id());
	selection->AppendL(entryY2->Id());
	selection->AppendL(entryX1->Id());
	// do not add X2 to the selection as this is the corrupt one
	selection->AppendL(entryX3->Id());

	CMsvFindOperation* find = NULL;

	// Test invalid arguments
	TRequestStatus status;
	TRAPD(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), entryX1->Id(), KMsvMessagePartDescription, status));
	test(error == KErrArgument);

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX->Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());

	test(progress().iError == KErrNone);
	test(progress().iCompleted == totalEntries);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == positiveFinds);

	TInt count;
	for(count = 0; count < positiveFinds; count++)
		{
		test(selection->At(count) == find->GetFindResult().At(count).iId);
		test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);
		}

	CleanupStack::PopAndDestroy(7); // All TMsvEntry's
	CleanupStack::PopAndDestroy(6); // selection, find, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenBrokenMtmL(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under (Created on heap because I was running out of stack)
	TMsvEntry* folderX = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderX);
	folderX->iType = KUidMsvFolderEntry;
	folderX->iMtm = KUidMsvLocalServiceMtm;
	folderX->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderX);
	cEntry->SetEntryL(folderX->Id());

	TMsvEntry* entryX1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX1);
	entryX1->iDescription.Set(KShortDescription);
	entryX1->iDetails.Set(KShortDetails);
	entryX1->iType = KUidMsvMessageEntry;
	entryX1->iMtm = aMtm;
	entryX1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX1);
	SaveMessageL(*session, *entryX1);

	TMsvEntry* entryX2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX2);
	entryX2->iDescription.Set(KShortDescription);
	entryX2->iDetails.Set(KShortDetails);
	entryX2->iType = KUidMsvMessageEntry;
	entryX2->iMtm = aMtm;
	entryX2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX2);
	SaveMessageL(*session, *entryX2);

	TMsvEntry* entryX3 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryX3);
	entryX3->iDescription.Set(KShortDescription);
	entryX3->iDetails.Set(KShortDetails);
	entryX3->iType = KUidMsvMessageEntry;
	entryX3->iMtm = aMtm;
	entryX3->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryX3);
	SaveMessageL(*session, *entryX3);

	TMsvEntry* folderY = new(ELeave)TMsvEntry;
	CleanupStack::PushL(folderY);
	folderY->iType = KUidMsvFolderEntry;
	folderY->iMtm = KUidMsvLocalServiceMtm;
	folderY->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*folderY);
	cEntry->SetEntryL(folderY->Id());

	TMsvEntry* entryY1 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY1);
	entryY1->iDescription.Set(KShortDescription);
	entryY1->iDetails.Set(KShortDetails);
	entryY1->iType = KUidMsvMessageEntry;
	entryY1->iMtm = aMtm;
	entryY1->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY1);
	SaveMessageL(*session, *entryY1);

	TMsvEntry* entryY2 = new(ELeave)TMsvEntry;
	CleanupStack::PushL(entryY2);
	entryY2->iDescription.Set(KShortDescription);
	entryY2->iDetails.Set(KShortDetails);
	entryY2->iType = KUidMsvMessageEntry;
	entryY2->iMtm = aMtm;
	entryY2->iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(*entryY2);
	SaveMessageL(*session, *entryY2);

	selection->AppendL(entryY1->Id());
	selection->AppendL(entryY2->Id());
	selection->AppendL(entryX1->Id());
	selection->AppendL(entryX2->Id());
	selection->AppendL(entryX3->Id());

	CMsvFindOperation* find = NULL;

	// Test invalid arguments
	TRequestStatus status;
	TRAPD(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), entryX1->Id(), KMsvMessagePartDescription, status));
	test(error == KErrArgument);

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX->Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iRemaining == 0);

	CleanupStack::PopAndDestroy(7); // All TMsvEntry's
	CleanupStack::PopAndDestroy(6); // selection, find, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenWithHeapFail1L(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KShortDescription);
	entryX1.iDetails.Set(KShortDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KShortDescription);
	entryX3.iDetails.Set(KShortDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	SaveMessageL(*session, entryY1);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KShortDescription);
	entryY2.iDetails.Set(KShortDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	SaveMessageL(*session, entryY2);

	selection->AppendL(entryY1.Id());
	selection->AppendL(entryY2.Id());
	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	CMsvFindOperation* find = NULL;

	TInt error;
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// Test find in children
		TRAP(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX.Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus));
		
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				__UHEAP_RESET;

				finished = ETrue;
				
				// Test the progress
				test(progress().iCompleted == 5);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrNoMemory);
			
			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 5);

			CMsvFindResultSelection* resSel = find->GetFindResult().CopyL();
			test(resSel->Count() == find->GetFindResult().Count());

			TInt count;
			for(count = 0; count < resSel->Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);

				test(resSel->At(count).iPartList == find->GetFindResult().At(count).iPartList);
				test(resSel->At(count).iId == find->GetFindResult().At(count).iId);
				test(resSel->Find(find->GetFindResult().At(count).iId) == count);
				}
			delete resSel;

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenWithHeapFail2L(TUid aMtm)
//
//
//
	{
	RMsvServerSession server;
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KShortDescription);
	entryX1.iDetails.Set(KShortDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KShortDescription);
	entryX3.iDetails.Set(KShortDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	SaveMessageL(*session, entryY1);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KShortDescription);
	entryY2.iDetails.Set(KShortDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	SaveMessageL(*session, entryY2);

	selection->AppendL(entryY1.Id());
	selection->AppendL(entryY2.Id());
	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	CMsvFindOperation* find = NULL;

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		// Test find in children
		TRAP(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX.Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus));
		
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				server.SetFailure(EHeapFailure, RHeap::ENone);

				finished = ETrue;
				
				// Test the progress
				test(progress().iCompleted == 5);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrNoMemory);
			
			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 5);

			CMsvFindResultSelection* resSel = find->GetFindResult().CopyL();
			test(resSel->Count() == find->GetFindResult().Count());

			TInt count;
			for(count = 0; count < resSel->Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);

				test(resSel->At(count).iPartList == find->GetFindResult().At(count).iPartList);
				test(resSel->At(count).iId == find->GetFindResult().At(count).iId);
				test(resSel->Find(find->GetFindResult().At(count).iId) == count);
				}
			delete resSel;

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindInChildrenWithFileFailL(TUid aMtm)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KShortDescription);
	entryX1.iDetails.Set(KShortDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KShortDescription);
	entryX3.iDetails.Set(KShortDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	SaveMessageL(*session, entryY1);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KShortDescription);
	entryY2.iDetails.Set(KShortDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	SaveMessageL(*session, entryY2);

	selection->AppendL(entryY1.Id());
	selection->AppendL(entryY2.Id());
	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	CMsvFindOperation* find = NULL;

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Test find in children
		TRAP(error, find = CMsvFindOperation::FindInChildrenL(*session, _L("Text"), folderX.Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus));
		theUtils->FileSession().SetErrorCondition(KErrNone);
		
		if (error == KErrNone)
			{
			CleanupStack::PushL(find);

			active->Start();
			CActiveScheduler::Start();

			TPckgBuf<TMsvFindOperationProgress> progress;
			progress.Copy(find->FinalProgress());
			test(active->iStatus == progress().iError);

			if (progress().iError == KErrNone)
				{
				theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

				finished = ETrue;
				
				// Test the progress
				test(progress().iCompleted == 5);
				test(progress().iRemaining == 0);
				}
			else
				test(progress().iError == KErrGeneral);
			
			if (progress().iError == KErrNone)
				test(find->GetFindResult().Count() == 5);

			CMsvFindResultSelection* resSel = find->GetFindResult().CopyL();
			test(resSel->Count() == find->GetFindResult().Count());

			TInt count;
			for(count = 0; count < resSel->Count(); count++)
				{
				test(selection->At(count) == find->GetFindResult().At(count).iId);
				test(find->GetFindResult().At(count).iPartList == KMsvMessagePartDescription);

				test(resSel->At(count).iPartList == find->GetFindResult().At(count).iPartList);
				test(resSel->At(count).iId == find->GetFindResult().At(count).iId);
				test(resSel->Find(find->GetFindResult().At(count).iId) == count);
				}
			delete resSel;

			CleanupStack::PopAndDestroy(); // find
			}
		else
			test(error == KErrGeneral);
		}

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindDescriptionSelectionL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3);

	CMsvFindOperation* find = NULL;

	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("abc"), *selection, KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("def"), *selection, KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("def"), *selection, KMsvMessagePartDescription | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("ghi"), *selection, KMsvMessagePartDescription | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("jkl"), *selection, KMsvMessagePartDescription | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("jkl"), *selection, KMsvMessagePartDescription | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Jkl"), *selection, KMsvMessagePartDescription | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindDescriptionChildrenL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	SaveMessageL(*session, entryY1);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KMatchingDescription);
	entryY2.iDetails.Set(KMatchingDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	SaveMessageL(*session, entryY2);

	CMsvFindOperation* find = NULL;

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("abc"), folderX.Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("def"), folderX.Id(), KMsvMessagePartDescription, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartDescription);


	CleanupStack::PopAndDestroy(); // find

	// Test find in children - case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("def"), folderX.Id(), KMsvMessagePartDescription | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in chidlren - match whole word
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("ghi"), folderX.Id(), KMsvMessagePartDescription | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in chidlren - match whole word
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("jkl"), folderX.Id(), KMsvMessagePartDescription | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartDescription);

	CleanupStack::PopAndDestroy(); // find

	// Test find in children - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("jkl"), folderX.Id(), KMsvMessagePartDescription | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in children - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Jkl"), folderX.Id(), KMsvMessagePartDescription | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartDescription);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartDescription);


	CleanupStack::PopAndDestroy(); // find

	//

	CleanupStack::PopAndDestroy(4); // active, cEntry, session, ob
	}

LOCAL_C void CreateRecipientsL(CMsvSession& aSession, TMsvEntry& aEntry, TBool aFlag)
	{
	CClientMtmRegistry* clientRegistry = CClientMtmRegistry::NewL(aSession);
	CleanupStack::PushL(clientRegistry);

	CBaseMtm* baseMtm = clientRegistry->NewMtmL(aEntry.iMtm);
	CleanupStack::PushL(baseMtm);

	// SetCurrentEntryL takes ownership of this
	CMsvEntry* entry = CMsvEntry::NewL(aSession, aEntry.Id(), TMsvSelectionOrdering());
	baseMtm->SetCurrentEntryL(entry);

	baseMtm->AddAddresseeL(_L("joe.blogs@symbian.com"));
	baseMtm->AddAddresseeL(_L("test@test.com"), _L("Tester"));
	baseMtm->AddAddresseeL(_L("peter.harper@symbian.com"));

	if (aFlag)
		baseMtm->AddAddresseeL(_L("test.find@match.com"));

	baseMtm->SaveMessageL();

	CleanupStack::PopAndDestroy(2); // baseMtm, clientRegistry
	}

LOCAL_C void TestFindRecipientSelectionL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	CreateRecipientsL(*session, entryX1, ETrue);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	CreateRecipientsL(*session, entryX2, EFalse);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	CreateRecipientsL(*session, entryX3, ETrue);

	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	CMsvFindOperation* find = NULL;

	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Find"), *selection, KMsvMessagePartRecipient, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartRecipient);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("zzz"), *selection, KMsvMessagePartRecipient, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Find"), *selection, KMsvMessagePartRecipient | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Fin"), *selection, KMsvMessagePartRecipient | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("find"), *selection, KMsvMessagePartRecipient | KMsvFindCaseSensitive | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartRecipient);

	CleanupStack::PopAndDestroy(); // find

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindRecipientChildrenL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	CreateRecipientsL(*session, entryX1, ETrue);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	CreateRecipientsL(*session, entryX2, EFalse);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	CreateRecipientsL(*session, entryX3, ETrue);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	CreateRecipientsL(*session, entryY1, EFalse);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KMatchingDescription);
	entryY2.iDetails.Set(KMatchingDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	CreateRecipientsL(*session, entryY2, ETrue);

	CMsvFindOperation* find = NULL;

	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Find"), folderX.Id(), KMsvMessagePartRecipient, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartRecipient);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("zzz"), folderX.Id(), KMsvMessagePartRecipient, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Find"), folderX.Id(), KMsvMessagePartRecipient | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Fin"), folderX.Id(), KMsvMessagePartRecipient | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("find"), folderX.Id(), KMsvMessagePartRecipient | KMsvFindCaseSensitive | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartRecipient);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartRecipient);

	CleanupStack::PopAndDestroy(); // find

	CleanupStack::PopAndDestroy(4); // active, cEntry, session, ob
	}

LOCAL_C void TestFindBodySelectionL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1, ETrue);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3, ETrue);

	CMsvFindOperation* find = NULL;

	selection->AppendL(entryX1.Id());
	selection->AppendL(entryX2.Id());
	selection->AppendL(entryX3.Id());

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("rich"), *selection, KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in selection
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("RICH"), *selection, KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("rich"), *selection, KMsvMessagePartBody | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("ric"), *selection, KMsvMessagePartBody | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("rich"), *selection, KMsvMessagePartBody | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("rich"), *selection, KMsvMessagePartBody | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in selection - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInSelectionL(*session, _L("Rich"), *selection, KMsvMessagePartBody | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iId == entryX1.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX3.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	CleanupStack::PopAndDestroy(5); // selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindBodyChildrenL(TUid aMtm)
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folderX;
	folderX.iType = KUidMsvFolderEntry;
	folderX.iMtm = KUidMsvLocalServiceMtm;
	folderX.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderX);
	cEntry->SetEntryL(folderX.Id());

	TMsvEntry entryX1;
	entryX1.iDescription.Set(KMatchingDescription);
	entryX1.iDetails.Set(KMatchingDetails);
	entryX1.iType = KUidMsvMessageEntry;
	entryX1.iMtm = aMtm;
	entryX1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX1);
	SaveMessageL(*session, entryX1, ETrue);

	TMsvEntry entryX2;
	entryX2.iDescription.Set(KShortDescription);
	entryX2.iDetails.Set(KShortDetails);
	entryX2.iType = KUidMsvMessageEntry;
	entryX2.iMtm = aMtm;
	entryX2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX2);
	SaveMessageL(*session, entryX2);

	TMsvEntry entryX3;
	entryX3.iDescription.Set(KMatchingDescription);
	entryX3.iDetails.Set(KMatchingDetails);
	entryX3.iType = KUidMsvMessageEntry;
	entryX3.iMtm = aMtm;
	entryX3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryX3);
	SaveMessageL(*session, entryX3, ETrue);

	TMsvEntry folderY;
	folderY.iType = KUidMsvFolderEntry;
	folderY.iMtm = KUidMsvLocalServiceMtm;
	folderY.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folderY);
	cEntry->SetEntryL(folderY.Id());

	TMsvEntry entryY1;
	entryY1.iDescription.Set(KShortDescription);
	entryY1.iDetails.Set(KShortDetails);
	entryY1.iType = KUidMsvMessageEntry;
	entryY1.iMtm = aMtm;
	entryY1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY1);
	SaveMessageL(*session, entryY1);

	TMsvEntry entryY2;
	entryY2.iDescription.Set(KMatchingDescription);
	entryY2.iDetails.Set(KMatchingDetails);
	entryY2.iType = KUidMsvMessageEntry;
	entryY2.iMtm = aMtm;
	entryY2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entryY2);
	SaveMessageL(*session, entryY2, ETrue);

	CMsvFindOperation* find = NULL;

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("rich"), folderX.Id(), KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in children
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("RICH"), folderX.Id(), KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in children - case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("rich"), folderX.Id(), KMsvMessagePartBody | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in chidlren - match whole word
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("ric"), folderX.Id(), KMsvMessagePartBody | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in chidlren - match whole word
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Rich"), folderX.Id(), KMsvMessagePartBody | KMsvFindWholeWord, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results


	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	// Test find in children - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("rich"), folderX.Id(), KMsvMessagePartBody | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find

	// Test find in children - match whole word and case sensitive
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*session, _L("Rich"), folderX.Id(), KMsvMessagePartBody | KMsvFindWholeWord | KMsvFindCaseSensitive, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 5);
	test(progress().iRemaining == 0);

	// Test results

	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iId == entryY2.Id());
	test(find->GetFindResult().At(0).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(1).iId == entryX1.Id());
	test(find->GetFindResult().At(1).iPartList == KMsvMessagePartBody);
	test(find->GetFindResult().At(2).iId == entryX3.Id());
	test(find->GetFindResult().At(2).iPartList == KMsvMessagePartBody);


	CleanupStack::PopAndDestroy(); // find

	CleanupStack::PopAndDestroy(4); // active, cEntry, session, ob
	}

LOCAL_C void TestFindOnDateSelectionL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = 0;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = 0;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = 0;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TTime date;
	date.HomeTime();

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = date;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = date;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = date;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(6); // find, selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindOnDateChildrenL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = 0;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = 0;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = 0;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	TTime date;
	date.HomeTime();

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = date;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = date;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = date;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvOnDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(5); // find, active, cEntry, session, ob
	}

LOCAL_C void TestFindBeforeDateSelectionL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	TTime date;
	date.HomeTime();
	
	TDateTime dt(date.DateTime());
	dt.SetYearLeapCheck(dt.Year() + 1);

	date = dt;

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = date;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = date;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = date;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	date.HomeTime();

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = 0;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = 0;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = 0;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(6); // find, selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindBeforeDateChildrenL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	TTime date;
	date.HomeTime();
	
	TDateTime dt(date.DateTime());
	dt.SetYearLeapCheck(dt.Year() + 1);

	date = dt;

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = date;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = date;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = date;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	date.HomeTime();

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = 0;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = 0;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = 0;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvBeforeDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(5); // find, active, cEntry, session, ob
	}

LOCAL_C void TestFindAfterDateSelectionL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	TTime date;
	date.HomeTime();
	
	TDateTime dt(date.DateTime());
	dt.SetYearLeapCheck(dt.Year() + 1);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = date;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = date;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = date;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = dt;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = dt;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = dt;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInSelectionL(*session, aText, *selection, aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(6); // find, selection, active, cEntry, session, ob
	}

LOCAL_C void TestFindAfterDateChildrenL(TUid aMtm, const TDesC& aText, TMsvPartList aParts)
//
//
//
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvOperationWait* active = CMsvOperationWait::NewLC();

	TTime date;
	date.HomeTime();
	
	TDateTime dt(date.DateTime());
	dt.SetYearLeapCheck(dt.Year() + 1);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = aMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDate = date;
	cEntry->CreateL(entry1);
	SaveMessageL(*session, entry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDate = date;
	cEntry->CreateL(entry2);
	SaveMessageL(*session, entry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDate = date;
	cEntry->CreateL(entry3);
	SaveMessageL(*session, entry3);

	date.HomeTime();

	// Test find in selection
	active->Start();
	CMsvFindDateOperation* find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	TPckgBuf<TMsvFindOperationProgress> progress;
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 0);

	CleanupStack::PopAndDestroy(); // find
	entry1.iDate = dt;
	cEntry->SetEntryL(entry1.Id());
	cEntry->ChangeL(entry1);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 1);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());

	CleanupStack::PopAndDestroy(); // find
	entry2.iDate = dt;
	cEntry->SetEntryL(entry2.Id());
	cEntry->ChangeL(entry2);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 2);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());

	CleanupStack::PopAndDestroy(); // find
	entry3.iDate = dt;
	cEntry->SetEntryL(entry3.Id());
	cEntry->ChangeL(entry3);

	// Test find in selection
	active->Start();
	find = CMsvFindDateOperation::FindInChildrenL(*session, aText, folder.Id(), aParts, date, EMsvAfterDate, active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();
	
	// Test the progress
	progress.Copy(find->FinalProgress());
	test(progress().iError == KErrNone);
	test(progress().iCompleted == 3);
	test(progress().iRemaining == 0);

	// Test results
	test(find->GetFindResult().Count() == 3);
	test(find->GetFindResult().At(0).iPartList == aParts);
	test(find->GetFindResult().At(0).iId == entry1.Id());
	test(find->GetFindResult().At(1).iPartList == aParts);
	test(find->GetFindResult().At(1).iId == entry2.Id());
	test(find->GetFindResult().At(2).iPartList == aParts);
	test(find->GetFindResult().At(2).iId == entry3.Id());

	CleanupStack::PopAndDestroy(5); // find, active, cEntry, session, ob
	}

LOCAL_C void doTestsL()
	{
	PanicSchedulerOnError = EFalse;
	CMsgsTestUtils::InstallMtmL(KFindMtmDataFile, KFindMtmName, KUidFindTestMtm, KFindMtmFilename);
	CMsgsTestUtils::InstallMtmL(KBrokenFindMtmDataFile, KBrokenFindMtmName, KUidBrokenFindTestMtm, KBrokenFindMtmFilename);

	CMsgsTestUtils::Start(_L("Finding text in children with unsupported LoadMessageL() in mtm"));
	TestFindInChildrenBrokenMtmL(KUidBrokenFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text in selection"));
	TestFindInSelectionL(KUidFindTestMtm);
	TestFindInSelectionWithHeapFail1L(KUidFindTestMtm);
	TestFindInSelectionWithHeapFail2L(KUidFindTestMtm);
	TestFindInSelectionWithFileFailL(KUidFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text in children"));
	TestFindInChildrenL(KUidFindTestMtm);
	TestFindInChildrenWithHeapFail1L(KUidFindTestMtm);
	TestFindInChildrenWithHeapFail2L(KUidFindTestMtm);
	TestFindInChildrenWithFileFailL(KUidFindTestMtm);
	TestFindInChildrenWithCorruptFileL(KUidFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text in description"));
	TestFindDescriptionSelectionL(KUidFindTestMtm);
	TestFindDescriptionChildrenL(KUidFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text in recipient"));
	TestFindRecipientSelectionL(KUidFindTestMtm);
	TestFindRecipientChildrenL(KUidFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text in body"));
	TestFindBodySelectionL(KUidFindTestMtm);
	TestFindBodyChildrenL(KUidFindTestMtm);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding text using derived class"));
	TestFindOnDateSelectionL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	TestFindOnDateChildrenL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	TestFindBeforeDateSelectionL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	TestFindBeforeDateChildrenL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	TestFindAfterDateSelectionL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	TestFindAfterDateChildrenL(KUidFindTestMtm, _L("Text"), KMsvMessagePartDescription);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Finding empty text using derived class"));
	TestFindOnDateSelectionL(KUidFindTestMtm, _L(""), 0);
	TestFindOnDateChildrenL(KUidFindTestMtm, _L(""), 0);
	TestFindBeforeDateSelectionL(KUidFindTestMtm, _L(""), 0);
	TestFindBeforeDateChildrenL(KUidFindTestMtm, _L(""), 0);
	TestFindAfterDateSelectionL(KUidFindTestMtm, _L(""), 0);
	TestFindAfterDateChildrenL(KUidFindTestMtm, _L(""), 0);
	CMsgsTestUtils::Complete();
	}

