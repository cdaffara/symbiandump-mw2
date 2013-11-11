// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <msvids.h>
#include <msvuids.h>
#include <msgs.rsg>
#include "msvindexadapter.h"
#include "Testsetup.h"

#pragma warning( disable : 4710 )

RTest test(_L("MSV Index Testrig"));
CPermanentFileStore* DummyStore;

_LIT(KMsvTestFile, "c:\\Index testing file 1");
_LIT(KMsvTestDummyFile, "c:\\Index testing file 2");

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");

_LIT(KLongDescription, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDetails, "A very long details string containing rubbish and junk but has more than sixty four characters");


const TUid KUidTestMsgsIndex = { 0x10274014 };
_LIT(KMsvTestIndexFile, "\\Server MTM testing\\Index");

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
	
LOCAL_C void CreateEntryL(CMsvSession& aSession, TTestMsvEntry& aEntry, TInt aError, TSecureId aOwnerId)
	{	
	CMsvEntry* msventry = CMsvEntry::NewL(aSession, aEntry.Parent(), TMsvSelectionOrdering());
	CleanupStack::PushL(msventry);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	CleanupStack::PushL(active);
	active->StartL();	

	CMsvOperation* opert = NULL;
	if( aOwnerId != NULL )
		{
		// An owner ID has been specified...
		opert = msventry->CreateL(aEntry, aOwnerId, active->iStatus);
		}
	else
		{
		// No owner ID has been specified...
		opert = msventry->CreateL(aEntry, active->iStatus);			
		}
	CleanupStack::PushL(opert);

	CActiveScheduler::Start(); // operation complete

	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalNew);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberRemaining==0);
	test(package().iError==aError);
	if( aError != KErrNone )
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		aEntry.SetId(package().iId);
		if (aEntry.iType==KUidMsvServiceEntry)
			aEntry.iServiceId = package().iId;
		}

	CleanupStack::PopAndDestroy(opert);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(msventry);
	}
	
LOCAL_C void ChangeEntryL(CMsvSession& aSession, TTestMsvEntry& aEntry, TInt aError, TSecureId aOwnerId)
	{
	CMsvEntry* msventry = CMsvEntry::NewL(aSession, aEntry.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(msventry);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	CleanupStack::PushL(active);
	active->StartL();	

	CMsvOperation* opert = NULL;
	if( aOwnerId != NULL )
		{
		// An owner ID has been specified...
		opert = msventry->ChangeL(aEntry, aOwnerId, active->iStatus);
		}
	else
		{
		// No owner ID has been specified...
		opert = msventry->ChangeL(aEntry, active->iStatus);			
		}
	CleanupStack::PushL(opert);

	CActiveScheduler::Start(); // operation complete

	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberRemaining==0);
	test(package().iError==aError);
	test(package().iId==aEntry.Id());
	if( aError != KErrNone )
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		aEntry.SetId(package().iId);
		if (aEntry.iType==KUidMsvServiceEntry)
			aEntry.iServiceId = package().iId;
		}

	CleanupStack::PopAndDestroy(opert);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(msventry);
	}
	
LOCAL_C void MoveCopyEntryL(CMsvSession& aSession, TTestMsvEntry& aEntry, TMsvId aDestination, TInt aError, TBool aMove)
	{	
	CMsvEntry* msventry = CMsvEntry::NewL(aSession, aEntry.Parent(), TMsvSelectionOrdering());
	CleanupStack::PushL(msventry);

	COperationCompleteActive* active = new (ELeave) COperationCompleteActive;
	CleanupStack::PushL(active);
	active->StartL();	

	CMsvOperation* opert = NULL;
	if( aMove )
		{
		// Do move...
		opert = msventry->MoveL(aEntry.Id(), aDestination, active->iStatus);
		}
	else
		{
		// Do copy...
		opert = msventry->CopyL(aEntry.Id(), aDestination, active->iStatus);
		}
	CleanupStack::PushL(opert);

	CActiveScheduler::Start(); // operation complete

	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());
	if( aMove )
		{
		test(package().iType==TMsvLocalOperationProgress::ELocalMove);		
		test(package().iId==aEntry.Id());
		}
	else
		test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
	test(package().iTotalNumberOfEntries==1);
	test(package().iNumberRemaining==0);
	test(package().iError==aError);
	if( aError != KErrNone )
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		aEntry.SetId(package().iId);
		}

	CleanupStack::PopAndDestroy(opert);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(msventry);
	}
	



//

LOCAL_C TInt LockAndDeleteEntryL(CMsvIndexAdapter* aIndex, TMsvId aId)
	{
	TInt error = aIndex->LockEntryAndStore(aId);
	if (error==KErrNone)
		{
		error = aIndex->DeleteEntry(aId);
		if (error)
			aIndex->ReleaseEntryAndStore(aId);
		}
	return error;
	}


LOCAL_C void CreateDefaultIndex(CMsvIndexAdapter& aIndex)
	{
	// find the resource file
	TFileName fileName = KServerIndexResourceFileName();
	TParse parse;
	TFindFile findfile(theUtils->FileSession());
	CDir* dirs=NULL;
	User::LeaveIfError(findfile.FindWildByDir(fileName, KServerIndexResourceFilePath, dirs));
	fileName=(*dirs)[0].iName;
	delete dirs;
	parse.Set(fileName, &findfile.File(), NULL);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(theUtils->FileSession(), fileName);
	
	// load the initial index structure from resource file
	RResourceFile rf;
	rf.OpenL(theUtils->FileSession(),fileName);
	CleanupClosePushL(rf);
	TResourceReader reader;
	HBufC8* buf=rf.AllocReadLC(R_SERVER_INDEX_STARTUP);
	reader.SetBuffer(buf);

	const TInt numberOfEntries=reader.ReadInt16();
	for (TInt ii=0; ii<numberOfEntries; ii++)
		{
		TTestMsvEntry newEntry;
		// values from resource file
		newEntry.SetId(reader.ReadInt32());
		newEntry.SetParent(reader.ReadInt32());
		newEntry.iServiceId = reader.ReadInt32();
		newEntry.iType.iUid = reader.ReadInt32();
		newEntry.iMtm.iUid = reader.ReadInt32();
		newEntry.SetData(reader.ReadInt32());
		newEntry.iDescription.Set(reader.ReadTPtrC());
		newEntry.iDetails.Set(reader.ReadTPtrC());
		// plus other ones
		newEntry.iDate.HomeTime();
		newEntry.iSize=0;
		// populate it
		User::LeaveIfError(aIndex.AddEntry(newEntry, KMsvServerId, EFalse));
		}

	CleanupStack::PopAndDestroy(2); // reader(buf), rf

	}



LOCAL_C void TestIndexAllocL()
//
// Test there is no memory leakage from  CMsvServerIndex
// Important to find any orphaned entries
//
	{
#if (!defined SYMBIAN_MESSAGESTORE_USING_SQLDB)
	CMsvServerIndex* index;
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);

	// test c'tor and d'tor with no additions
	index = CMsvServerIndex::NewL(*DummyStore);
	delete index;

	// test c'tor and d'tor with no additions
	index = CMsvServerIndex::NewL(*DummyStore);
	CreateDefaultIndex(*index);
	delete index;

	// test c'tor and d'tor with single additions
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
	CleanupStack::PopAndDestroy();
	
	// test c'tor and d'tor with multiple additions
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TInt i;
	for (i=0; i<20; i++)
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	CleanupStack::PopAndDestroy();

	// test c'tor and d'tor with multiple additions and deletions
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	for (i=0; i<20; i++)
		{
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry.Id()));
		}
	CleanupStack::PopAndDestroy();

	// test c'tor and d'tor with multiple additions then multiple deletions
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	TTestMsvEntry entry3;
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	TTestMsvEntry entry4;
	entry4.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	TTestMsvEntry entry5;
	entry5.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	TTestMsvEntry entry6;
	entry6.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry4.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry5.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry6.Id()));
	CleanupStack::PopAndDestroy();

	// test c'tor and d'tor with multiple additions then multiple deletions in reverse order
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry6.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry5.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry4.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	CleanupStack::PopAndDestroy();

	// test c'tor and d'tor with multiple additions then multiple deletions in random order
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry5.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry6.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry4.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	CleanupStack::PopAndDestroy();
#endif
	}

LOCAL_C void TestIndexIdsL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	
	// create three entries
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));

	TTestMsvEntry entry3;
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));

	// delete them out of order
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));

	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));

	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));

	CleanupStack::PopAndDestroy();
	}

LOCAL_C void TestChildrenSelectionL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);
	TInt i;

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));


	// selection all children of root with none existing
	index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering());
	test(selection->Count()==0);
	selection->Reset();

	// selection all children of root with none existing - with sort order
	index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByDateReverse));
	test(selection->Count()==0);
	selection->Reset();

	// selection all children of root with none existing - with grouping, sort order
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByDateReverse)));
	test(selection->Count()==0);
	selection->Reset();


	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	test(entry1.Id()==selection->At(0)->Id());
	selection->Reset();
	

	//selection all children of root with many existing
	TTestMsvEntry entry2;
	entry2.SetParent(entry.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	TTestMsvEntry entry3;
	entry3.SetParent(entry.Id());
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	TTestMsvEntry entry4;
	entry4.SetParent(entry.Id());
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==4);
	for (i=0; i<4; i++)
		test(selection->At(i)->Parent()==entry.Id());
	selection->Reset();


	//selection all children of root with many existing but some with children themselves
	TTestMsvEntry entry5;
	entry5.SetParent(entry1.Id());
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	TTestMsvEntry entry6;
	entry6.SetParent(entry5.Id());
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==4);
	for (i=0; i<selection->Count(); i++)
		test(selection->At(i)->Parent()==entry.Id());
	selection->Reset();


	// adding somemore to the root
	TTestMsvEntry entry7;
	entry7.SetParent(entry.Id());
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==5);
	for (i=0; i<selection->Count(); i++)
		test(selection->At(i)->Parent()==entry.Id());
	selection->Reset();

	// getting other selections
	(index->GetChildrenL(entry1.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	test(selection->At(0)->Parent()==entry1.Id());
	selection->Reset();

	// getting other selections
	(index->GetChildrenL(entry5.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	test(selection->At(0)->Parent()==entry5.Id());
	selection->Reset();


	// delete an entry
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==4);
	for (i=0; i<selection->Count(); i++)
		test(selection->At(i)->Parent()==entry.Id());
	selection->Reset();

	// delete an entry
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry4.Id()));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==3);
	for (i=0; i<selection->Count(); i++)
		test(selection->At(i)->Parent()==entry.Id());
	selection->Reset();
	
	// delete that child
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry6.Id()));
	// make sure it is deleted
	(index->GetChildrenL(entry5.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();

	CleanupStack::PopAndDestroy(2);
	}



LOCAL_C void TestInvisibleSelectionL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// selection all children of root with none existing
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();

	// selection all children of root with none existing - with invisible
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(selection->Count()==0);
	selection->Reset();

	// selection all children of root with none existing - with grouping, sort order, and invisible
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByDateReverse, ETrue)));
	test(selection->Count()==0);
	selection->Reset();

	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.SetVisible(EFalse);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse)));
	test(selection->Count()==0);
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(selection->Count()==1);
	test(entry1.Id()==selection->At(0)->Id());
	selection->Reset();
	
	TTestMsvEntry entry2;
	entry2.SetParent(entry.Id());
	TTestMsvEntry entry3;
	entry3.SetParent(entry.Id());
	entry3.SetVisible(EFalse);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse)));
	test(selection->Count()==1);
	test(entry2.Id()==selection->At(0)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue)));
	test(selection->Count()==3);
	test(entry1.Id()==selection->At(0)->Id());
	test(entry2.Id()==selection->At(1)->Id());
	test(entry3.Id()==selection->At(2)->Id());
	selection->Reset();
	
	CleanupStack::PopAndDestroy(2);
	}	
	
	
LOCAL_C void TestMtmSelectionL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);

	TUid uid1={0x100};
	TUid uid2={0x101};
//	TUid uid3={0x102};

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// selection all children of root with none existing
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(), uid1));
	test(selection->Count()==0);
	selection->Reset();


	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iMtm=uid1;
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse), uid1));
	test(selection->Count()==1);
	test(entry1.Id()==selection->At(0)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse), uid2));
	test(selection->Count()==1);
	test(entry1.Id()==selection->At(0)->Id());
	selection->Reset();
	

	
	TTestMsvEntry entry2;
	entry2.SetParent(entry.Id());
	entry2.iMtm=uid2;
	entry2.iSize=1000;
	TTestMsvEntry entry3;
	entry3.SetParent(entry.Id());
	entry3.iMtm=uid2;
	entry3.iSize=10;
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone, EFalse), uid1));
	test(selection->Count()==3);
	test(entry1.Id()==selection->At(0)->Id());
	test(entry2.Id()==selection->At(1)->Id());
	test(entry3.Id()==selection->At(2)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone, EFalse), uid2));
	test(selection->Count()==3);
	test(entry2.Id()==selection->At(0)->Id());
	test(entry3.Id()==selection->At(1)->Id());
	test(entry1.Id()==selection->At(2)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortBySize, EFalse), uid1));
	test(selection->Count()==3);
	test(entry1.Id()==selection->At(0)->Id());
	test(entry3.Id()==selection->At(1)->Id());
	test(entry2.Id()==selection->At(2)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortBySizeReverse, EFalse), uid1));
	test(selection->Count()==3);
	test(entry1.Id()==selection->At(0)->Id());
	test(entry2.Id()==selection->At(1)->Id());
	test(entry3.Id()==selection->At(2)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortBySize, EFalse), uid2));
	test(selection->Count()==3);
	test(entry3.Id()==selection->At(0)->Id());
	test(entry2.Id()==selection->At(1)->Id());
	test(entry1.Id()==selection->At(2)->Id());
	selection->Reset();
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortBySizeReverse, EFalse), uid2));
	test(selection->Count()==3);
	test(entry2.Id()==selection->At(0)->Id());
	test(entry3.Id()==selection->At(1)->Id());
	test(entry1.Id()==selection->At(2)->Id());
	selection->Reset();

	CleanupStack::PopAndDestroy(2);
	}	


LOCAL_C void TestLockingL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TInt error;

	// create entry
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// ENTRY
	// locking an entry
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));

	// try to lock entry again
	error = index->LockEntry(entry1.Id());
	test(error==KErrLocked);

	// try to lock unknown entry
	error = index->LockEntry(1212);
	test(error==KErrNotFound);

	// try to delete locked entry
	error = LockAndDeleteEntryL(index,entry1.Id());
	test(error==KErrLocked);
	
	// try to change locked entry
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	
	// releasing an entry
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));

	// try to change unlocked entry
//	error = index->ChangeEntry(entry1);
//	test(error==KErrAccessDenied);
	
	// delete unlocked entry
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));

	// STORE
	// create new entry
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// locking an store
	REPORT_IF_ERROR(index->LockStore(entry1.Id()));

	// try to lock store again
	error = index->LockStore(entry1.Id());
	test(error==KErrLocked);

	// try to lock unknown store
	error = index->LockStore(1212);
	test(error==KErrNotFound);

	// try to delete entry with locked store
	error = LockAndDeleteEntryL(index,entry1.Id());
	test(error==KErrLocked);
	
	// releasing an store
	REPORT_IF_ERROR(index->ReleaseStore(entry1.Id()));

	// delete entry with unlocked store
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));

	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestChangingL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// create entry
	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	entry1.iServiceId = entry.Id();
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// check entry via children selection
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	test(selection->At(0)->iServiceId == entry1.iServiceId);
	selection->Reset();;

	// check entry by getting it
// TO DO
//	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
//	test(entryPtr->iServiceId == entry1.iServiceId);
	
	// change entry
	entry1.iServiceId = 101;
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, ETrue));
	
	// check entry via children selection
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	test(selection->At(0)->iServiceId == entry1.iServiceId);
	selection->Reset();;

	// check entry by getting it
// TO DO
//	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
//	test(entryPtr->iServiceId == entry1.iServiceId);

	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));

	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C void TestFlagsL()
	{
	
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TMsvEntry* entryPtr;

	//
	// owner flag

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner()==EFalse);
	

	// create entry
	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());
	

	// create entry
	TTestMsvEntry entry2;
	entry2.SetParent(entry.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());
	

	// delete first sibling
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	
	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());
	

	// delete second sibling
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner()==EFalse);
	

	// add entry
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());
	

	// delete second sibling
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());
	

	// delete first sibling
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));

	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner()==EFalse);
	

	// delete the index and reload from file
	CleanupStack::PopAndDestroy(); // index	
	TRAPD(ignore, index = CMsvServerIndex::OpenL(*testStore));
	CleanupStack::PushL(index);
	
	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner()==EFalse);
	
	
	// add entry
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// delete the index and reload from file
	CleanupStack::PopAndDestroy(); // index	
	
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	
	// test owner flag
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr));
	test(entryPtr->Owner());

	

	CleanupStack::PopAndDestroy(2); // index, testStore
	}

	

LOCAL_C void TestFileSyncingL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);
	TTestMsvEntry entry1,entry2,entry3;

	// create the test index with no entries
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(); 
	CleanupStack::PopAndDestroy();
	
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(2); 

	// create the test index with no entries
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(2); 

	// create the test index with one entry
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 


	// create the test index with two entries under the root
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 

	// create the test index with two entries under each other
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(entry1.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	const TMsvEntry* testEntry1 = selection->At(0);
	selection->Reset();;
	(index->GetChildrenL(testEntry1->Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	const TMsvEntry* testEntry2 = selection->At(0);
	selection->Reset();;
	(index->GetChildrenL(testEntry2->Id(), *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 


	// create the test index with three entries
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(entry1.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// create entry
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 

	
	// create the test index with three sibling entries and delete one
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// create entry
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// delete entry
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==2);
	selection->Reset();;
	// close the index
	CleanupStack::PopAndDestroy(2); 

	
	// create the test index with three sibling entries and delete all
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// create entry
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// delete entries
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(2); 

	
	// create the test index with three sibling entries and delete all
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// create entry
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// delete entries
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(2); 

	
	// create the test index with three sibling entries and delete all
	theUtils->FileSession().Delete(KMsvTestFile);	
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	// create entry
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// create entry
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	// create entry
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// delete entries
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(2); 
	// reopen the index
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	// check entry
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// close the index
	CleanupStack::PopAndDestroy(3); 
	}






LOCAL_C void TestLongDescriptorsL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TTestMsvEntry entry1,entry2,entry3;

	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);

	// adding then deleting entries with various long strings
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry2.Id()));
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry3.Id()));
	CleanupStack::PopAndDestroy(2);

	// read it back in to check it is OK
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(2);

	// check the read back of long strings from file
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KLongDescription().Length());
	test(selection->At(0)->iDetails.Length() == KShortDetails().Length());
	selection->Reset();
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	
	//
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KShortDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	
	//
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KLongDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	REPORT_IF_ERROR(LockAndDeleteEntryL(index,entry1.Id()));
	

	// test changing the string length
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KShortDescription().Length());
	test(selection->At(0)->iDetails.Length() == KShortDetails().Length());
	selection->Reset();
	
	//
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	//
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KShortDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	
	//
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KLongDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	
	//
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KShortDescription().Length());
	test(selection->At(0)->iDetails.Length() == KShortDetails().Length());
	selection->Reset();
	
	//
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KLongDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	
	//
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KLongDescription().Length());
	test(selection->At(0)->iDetails.Length() == KShortDetails().Length());
	selection->Reset();
	
	//
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->At(0)->iDescription.Length() == KShortDescription().Length());
	test(selection->At(0)->iDetails.Length() == KLongDetails().Length());
	selection->Reset();
	
	
	CleanupStack::PopAndDestroy(3);
	}



LOCAL_C void TestLocalL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	
	TBool isLocal;

	// non-existant entry
	test(index->IsLocal(1234, isLocal)==KErrNotFound);

	// test local service and in/outboxes
	REPORT_IF_ERROR(index->IsLocal(KMsvLocalServiceIndexEntryId, isLocal));
	test(isLocal);
	REPORT_IF_ERROR(index->IsLocal(KMsvGlobalInBoxIndexEntryId, isLocal));
	test(isLocal);
	REPORT_IF_ERROR(index->IsLocal(KMsvGlobalOutBoxIndexEntryId, isLocal));
	test(isLocal);

	// create an entry in inbox
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->IsLocal(entry1.Id(), isLocal));
	test(isLocal);

	// create an entry under entry in inbox
	TTestMsvEntry entry2;
	entry2.SetParent(entry1.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->IsLocal(entry2.Id(), isLocal));
	test(isLocal);

	// create another service (does not count as local)
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId); // CHANGE to root
	service.iType=KUidMsvServiceEntry;
	service.iDescription.Set(KShortDescription);
	service.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(service, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->IsLocal(service.Id(), isLocal));
	test(isLocal==EFalse);

	// create an entry under other service (is no longer local)
	TTestMsvEntry entry3;
	entry3.SetParent(service.Id());
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->IsLocal(entry3.Id(), isLocal));
	test(isLocal==EFalse);

	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C void TestDeletingL()
//
// Test DeletingEntries() using a selection
// 
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CArrayPtrFlat<const TMsvEntry>* childrenSelection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(childrenSelection);

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// create some entries
	TTestMsvEntry entry1,entry2,entry3;
	entry1.SetParent(entry.Id());
	entry1.iDescription.Set(KLongDescription);
	entry1.iDetails.Set(KShortDetails);
	entry2.SetParent(entry.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry3.SetParent(entry.Id());
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));

	// test current index
	(index->GetChildrenL(entry.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 3);
	childrenSelection->Reset();

	// test current index
	(index->GetChildrenL(entry.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 3);
	childrenSelection->Reset();

	// delete a selection with one entry
	REPORT_IF_ERROR(index->LockEntryAndStore(entry2.Id()));
	REPORT_IF_ERROR(index->DeleteEntry(entry2.Id()));

	// test current index
	(index->GetChildrenL(entry.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 2);
	childrenSelection->Reset();

	// delete a selection with two entry
	REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
	REPORT_IF_ERROR(index->LockEntryAndStore(entry3.Id()));
	REPORT_IF_ERROR(index->DeleteEntry(entry1.Id()));
	REPORT_IF_ERROR(index->DeleteEntry(entry3.Id()));

	// test current index
	(index->GetChildrenL(entry.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 0);
	childrenSelection->Reset();
	
	
	
	
	
	CleanupStack::PopAndDestroy(4);
	}





LOCAL_C void TestMovingL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	CArrayPtrFlat<const TMsvEntry>* childrenSelection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(childrenSelection);
	
	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// create two folders
	TTestMsvEntry folder1;
	folder1.SetParent(entry.Id());
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder1, KMsvServerId, ETrue));
	TTestMsvEntry folder2;
	folder2.SetParent(entry.Id());
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder2, KMsvServerId, ETrue));

	// moving to non-existant target
	test(index->MoveEntry(12, 23456)==KErrPathNotFound);

	// moving non-existant entry
	test(index->MoveEntry(12, folder2.Id())==KErrNotFound);

	// create entry in folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 0);
	childrenSelection->Reset();

	// move single entry
	REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
	REPORT_IF_ERROR(index->MoveEntry(entry1.Id(), folder2.Id()));
	REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry1.Id()));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 0);
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	childrenSelection->Reset();

	// create entry in folder2
	TTestMsvEntry entry2;
	entry2.SetParent(folder2.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 0);
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 2);
	childrenSelection->Reset();

	// move two entries
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
	REPORT_IF_ERROR(index->LockEntryAndStore(entry2.Id()));
	REPORT_IF_ERROR(index->MoveEntry(entry1.Id(), folder1.Id()));
	REPORT_IF_ERROR(index->MoveEntry(entry2.Id(), folder1.Id()));
	REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry1.Id()));
	REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry2.Id()));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 2);
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 0);
	childrenSelection->Reset();

	// move one of two entries
	REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
	REPORT_IF_ERROR(index->MoveEntry(entry1.Id(), folder2.Id()));
	REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry1.Id()));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	test(childrenSelection->At(0)->Id() == entry2.Id());
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	test(childrenSelection->At(0)->Id() == entry1.Id());
	childrenSelection->Reset();

	// try to move to a descendent
	REPORT_IF_ERROR(index->LockEntryAndStore(folder2.Id()));
	test(index->MoveEntry(folder2.Id(), entry1.Id())==KErrArgument);
	REPORT_IF_ERROR(index->ReleaseEntryAndStore(folder2.Id()));

	// test current index
	(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	test(childrenSelection->At(0)->Id() == entry2.Id());
	childrenSelection->Reset();
	(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
	test(childrenSelection->Count() == 1);
	test(childrenSelection->At(0)->Id() == entry1.Id());
	childrenSelection->Reset();


	CleanupStack::PopAndDestroy(4);
	}

LOCAL_C void TestDescendentsL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));


	// create two folders
	TTestMsvEntry folder1;
	folder1.SetParent(entry.Id());
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder1, KMsvServerId, ETrue));

	TTestMsvEntry folder2;
	folder2.SetParent(entry.Id());
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder2, KMsvServerId, ETrue));

	TTestMsvEntry entry2;
	entry2.SetParent(folder1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));

	// test the descendent logic
	TBool des;
	test(index->IsADescendent(entry.Id(), folder1.Id(), des)==KErrNone);
	test(des);
	test(index->IsADescendent(entry.Id(), folder2.Id(), des)==KErrNone);
	test(des);
	test(index->IsADescendent(folder1.Id(), entry.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(folder2.Id(), entry.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(entry.Id(), entry2.Id(), des)==KErrNone);
	test(des);
	test(index->IsADescendent(entry2.Id(), entry.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(folder1.Id(), entry2.Id(), des)==KErrNone);
	test(des);
	test(index->IsADescendent(entry2.Id(), folder1.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(folder2.Id(), entry2.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(entry2.Id(), folder2.Id(), des)==KErrNone);
	test(!des);
	test(index->IsADescendent(KMsvGlobalInBoxIndexEntryId, entry2.Id(), des)==KErrNone);
	test(des);
	test(index->IsADescendent(KMsvLocalServiceIndexEntryId, entry2.Id(), des)==KErrNone);
	test(des);

	// test errors
	test(index->IsADescendent(entry2.Id(), 34342, des)==KErrNotFound);


	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C void TestReadOnlyL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TMsvEntry* entryPtr=NULL;

	// create read only entry 
	TTestMsvEntry entry;
	entry.SetReadOnly(ETrue);
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// try to change from a read only entry 
	entry.iMtm=KUidMsvLocalServiceMtm;
	entry.SetReadOnly(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(*entryPtr==entry);
	REPORT_IF_ERROR(index->ReleaseEntry(entry.Id()));

	// try to change to a read only entry 
	entry.iMtm=KUidMsvNullEntry;
	entry.SetReadOnly(ETrue);
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(*entryPtr==entry);
	REPORT_IF_ERROR(index->ReleaseEntry(entry.Id()));

	// create standard entry 
	entry.SetStandardFolder(ETrue);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// try to change a standard entry 
	entry.iMtm=KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	test(index->ChangeEntry(entry, KMsvServerId, EFalse)==KErrAccessDenied);
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(!(*entryPtr==entry));
	REPORT_IF_ERROR(index->ReleaseEntry(entry.Id()));

	// try to change from a standard entry 
	entry.iMtm=KUidMsvLocalServiceMtm;
	entry.SetStandardFolder(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	test(index->ChangeEntry(entry, KMsvServerId, EFalse)==KErrAccessDenied);
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(!(*entryPtr==entry));

	// create non-standard entry 
	entry.SetStandardFolder(EFalse);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
	
	// try to change to a read only entry 
	entry.iMtm=KUidMsvNullEntry;
	entry.SetStandardFolder(ETrue);
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(*entryPtr==entry);
	REPORT_IF_ERROR(index->ReleaseEntry(entry.Id()));

	// try to change from a standard entry 
	entry.iMtm=KUidMsvLocalServiceMtm;
	entry.SetStandardFolder(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));
	test(index->ChangeEntry(entry, KMsvServerId, EFalse)==KErrAccessDenied);
	REPORT_IF_ERROR(index->GetEntry(entry.Id(),entryPtr));
	test(!(*entryPtr==entry));

	CleanupStack::PopAndDestroy(); // index
	}


LOCAL_C void TestTemporaryFlagsL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TMsvEntry* entryPtr=NULL;

	// create connected entry 
	TTestMsvEntry entry1;
	entry1.SetConnected(ETrue);
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	// create unconnected entry 
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(),entryPtr));
	test(!entryPtr->Connected());
	// close the index
	CleanupStack::PopAndDestroy(); // index

	// open the index
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	
	// test the temporary flags
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(),entryPtr));
	test(!entryPtr->Connected());

	// change to temporary
	entry1.SetConnected(ETrue);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	entry1.SetConnected(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(!entryPtr->Connected());

	// change to temporary & permanent
	entry1.SetConnected(ETrue);
	entry1.SetAttachment(ETrue);
	entry1.iSize=12345;
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	test(entryPtr->Attachment());
	test(entryPtr->iSize==12345);
	entry1.SetConnected(EFalse);
	entry1.SetAttachment(EFalse);
	entry1.iSize=67890;
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(!entryPtr->Connected());
	test(!entryPtr->Attachment());
	test(entryPtr->iSize==67890);

	// change to permanent
	entry1.SetConnected(ETrue);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	entry1.SetAttachment(ETrue);
	entry1.iSize=12345;
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	test(entryPtr->Attachment());
	test(entryPtr->iSize==12345);
	entry1.SetAttachment(EFalse);
	entry1.iSize=67890;
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->ChangeEntry(entry1, KMsvServerId, EFalse));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());
	test(!entryPtr->Attachment());
	test(entryPtr->iSize==67890);

	CleanupStack::PopAndDestroy(2); //index, testStore
	}





class CIndexOb : public CBase, public MMsvIndexObserver
	{
public:
	CIndexOb() {};
	void IndexLoadingCompleted(TInt aError) {iError=aError; CActiveScheduler::Stop();};
	void IndexRebuildingCompleted(TInt aError) {iError=aError; CActiveScheduler::Stop();};
public:
	TInt iError;
	};



LOCAL_C void TestAsyncL()
	{
	CIndexOb* ob = new(ELeave) CIndexOb;
	CleanupStack::PushL(ob);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Create blank filter
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();

	theUtils->FileSession().Delete(KMsvTestDummyFile);
	
	CPermanentFileStore* testStore;
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);

	
	// basic index
	CMsvServerIndex* index = CMsvServerIndex::NewL(*testStore);
	delete index;

	TRAPD(ignore, index = CMsvServerIndex::OpenL(*testStore, *ob));
	CActiveScheduler::Start();
	CleanupStack::PushL(index);

	// increasing number of entries - flat
	TInt i;
	TTestMsvEntry entry;
	entry.SetParent(KMsvRootIndexEntryId);
	for (i=1; i<=50; i++)
		{
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		CleanupStack::PopAndDestroy(); // index

		index = CMsvServerIndex::OpenL(*testStore, *ob);
		CActiveScheduler::Start();
		CleanupStack::PushL(index);

		REPORT_IF_ERROR(index->GetChildrenId(KMsvRootIndexEntryId, *filter, *selection));
		test(selection->Count()==i);
		selection->Reset();
		}

	// increasing number of entries - hierrachical
	for (i=1; i<=50; i++)
		{
		TMsvId parent = entry.Id();
		entry.SetParent(parent);
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		CleanupStack::PopAndDestroy(); // index

		index = CMsvServerIndex::OpenL(*testStore, *ob);
		CActiveScheduler::Start();
		CleanupStack::PushL(index);

		REPORT_IF_ERROR(index->GetChildrenId(KMsvRootIndexEntryId, *filter, *selection));
		test(selection->Count()==50);
		selection->Reset();

		REPORT_IF_ERROR(index->GetChildrenId(parent, *filter, *selection));
		test(selection->Count()==1);
		test(selection->At(0)==entry.Id());
		selection->Reset();
		}
	CleanupStack::PopAndDestroy(5, ob); // index, testStore, filter, selection, ob
	}




LOCAL_C void TestAttributesL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	// create entry to work with
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	TTestMsvEntry entry3;
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));

	TMsvEntry* entryPtr;

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(!entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(!entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(!entryPtr->InPreparation());

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->LockEntry(entry2.Id()));
	REPORT_IF_ERROR(index->LockEntry(entry3.Id()));

	// change nothing
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, 0));
	test(selection->Count()==0);

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());

	// change nothing that is not already set
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute, 0));
	test(selection->Count()==0);

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());

	// change nothing that is not already set
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute, KMsvUnreadAttribute));
	test(selection->Count()==0);

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());

	// set invisible
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());

	// set connected
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvConnectedAttribute, 0));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());

	// set in preparation
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvInPreparationAttribute, 0));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());

	// set visible clear connected
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute, KMsvConnectedAttribute));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected()); test(entryPtr->InPreparation());

	// set connected & unread
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvUnreadAttribute|KMsvConnectedAttribute, 0));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(entryPtr->InPreparation());

	// Clear In preparation
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvInPreparationAttribute));
	test(selection->Count()==3); selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(!entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(!entryPtr->InPreparation());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); test(!entryPtr->InPreparation());

	// entry1
	selection->AppendL(entry1.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute));
	test(selection->Count()==1); 
	test(selection->At(0)==entry1.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected()); 
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());

	// entry2&3
	selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute));
	test(selection->Count()==2);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());


	// entry2
	selection->AppendL(entry2.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0));
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());

	// entry2&3
	selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0));
	test(selection->Count()==1);
	test(selection->At(0)==entry3.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());

	// entry1&2&3
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0));
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());

	// entry1
	selection->AppendL(entry1.Id()); 
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute));
	test(selection->Count()==1);
	test(selection->At(0)==entry1.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());

	// entry1&2
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute));
	test(selection->Count()==2);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());


	// entry1&2&3
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute));
	test(selection->Count()==1);
	test(selection->At(0)==entry3.Id());
	selection->Reset();

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(entryPtr->Connected());

	// test the long descriptors
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvVisibilityAttribute, KMsvUnreadAttribute|KMsvConnectedAttribute));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(*entryPtr==(TMsvEntry)entry1);
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(*entryPtr==(TMsvEntry)entry2);
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(*entryPtr==(TMsvEntry)entry3);
	

	CleanupStack::PopAndDestroy(3); // selection, index, testStore
	}





LOCAL_C TInt DoDeleteSelection(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection, TMsvId aId)
	{
	// expand the selection
	aSelection.Reset();
	aSelection.AppendL(aId);
	REPORT_IF_ERROR(aIndex.ExpandSelectionRecursively(aSelection));

	// lock'm
	TInt count=aSelection.Count();
	while (count--)
		REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));

	// delete them
	TInt error = aIndex.DeleteSelection(aSelection);

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));
		}

	return error;
	}

LOCAL_C void TestSelectionDeletion1L()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);

	//
	// delete entry with one child
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));


	//
	// vertical 
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));

	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));


	//
	// horizontal
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));
	
	
	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry3.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry5.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));

	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry5.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	REPORT_IF_ERROR(DoDeleteSelection(*index, *selection, folder.Id()));
	// test
	test(!index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));



	CleanupStack::PopAndDestroy(3);
	}


LOCAL_C TInt DoDeleteSelection2(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection)
	{
	// lock'm
	TInt count=aSelection.Count();
	while (count--)
		REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));

	// delete them
	TInt error = aIndex.DeleteSelection(aSelection);

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));
		}

	return error;
	}



LOCAL_C void TestSelectionDeletion2L()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));

	
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);


	//
	// complex 1
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));

	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-1);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));
	


	//
	// complex 2
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-1);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(!index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));


	//
	// complex 2
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-2,2);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(index->EntryExists(entry1.Id()));
	test(!index->EntryExists(entry2.Id()));
	test(!index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));

	//
	// complex 2
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));

	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-4,4);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(index->EntryExists(entry1.Id()));
	test(index->EntryExists(entry2.Id()));
	test(index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));

	//
	// complex 2
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));

	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-4,4);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(index->EntryExists(entry1.Id()));
	test(index->EntryExists(entry2.Id()));
	test(index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));

	//
	// complex 2
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));

	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry3.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	selection->Reset();
	selection->AppendL(folder.Id());
	REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
	selection->Delete(selection->Count()-4,4);
	REPORT_IF_ERROR(DoDeleteSelection2(*index, *selection));
	// test
	test(index->EntryExists(folder.Id()));
	test(index->EntryExists(entry1.Id()));
	test(index->EntryExists(entry2.Id()));
	test(index->EntryExists(entry3.Id()));
	test(!index->EntryExists(entry4.Id()));
	test(!index->EntryExists(entry5.Id()));
	test(!index->EntryExists(entry6.Id()));
	test(!index->EntryExists(entry7.Id()));


	CleanupStack::PopAndDestroy(3);
	}
	

LOCAL_C void TestOwnerIdL()
	{
	CPermanentFileStore* testStore;
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TSecureId entryOwnerId;
	TMsvEntry* entryPtr = NULL;
	
	// CMsvServerIndex testing -
	//
	// 1. Create entry with given owner ID - test for correct ID.
	// 2. Change entry with forced change in ID - test for correct ID.
	// 3. Change entry, no forced change - test ID same.
	// 4. Change entry, no forced change, but private data changed - test for correct ID.

	// Test 1 - create entry with specific ID
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	TSecureId owner_1 = 1234;
	REPORT_IF_ERROR(index->AddEntry(entry, owner_1, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_1);

	// Lock the entry for the change tests...
	REPORT_IF_ERROR(index->LockEntry(entry.Id()));

	// Test 2 - change entry with forced change in ID.
	TSecureId owner_2 = 4321;
	REPORT_IF_ERROR(index->ChangeEntry(entry, owner_2, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_2);
	
	// Test 3 - change entry without forced change in ID and no change to
	// private data.
	
	REPORT_IF_ERROR(index->ChangeEntry(entry, owner_1, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_2);
	
	// Test 4a - change entry without forced change in ID but with a change to
	// private data (description only).
	entry.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(index->ChangeEntry(entry, owner_1, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_1);

	// Test 4b - change entry without forced change in ID but with a change to
	// private data (details only).
	entry.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry, owner_2, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_2);

	// Test 4c - change entry without forced change in ID but with a change to
	// private data (description and details).
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->ChangeEntry(entry, owner_1, EFalse));
	REPORT_IF_ERROR(index->GetEntry(entry.Id(), entryPtr, entryOwnerId));
	test(entryOwnerId == owner_1);
	
	// Release the entry...
	REPORT_IF_ERROR(index->ReleaseEntry(entry.Id()));

	CleanupStack::PopAndDestroy(index);
	index = NULL;
	
	// CMsvEntry testing.
	// 
	// 1. Create entry without a specified ID - created entry has ID of process,
	// 2. Create entry with specified ID - created entry has specified ID.
	// 3. Change entry without specified ID - no forced change, ID unchanged.
	// 4. Change entry with specifed server ID - forced change, ID updated.
	// 5. Move entry - ID unchanged.
	// 6. Copy entry - ID unchanged.
	
	// NOTE - cannot do actual checks until after all actions complete.
	
	CDummyObserver* dummyOb = new (ELeave) CDummyObserver;
	CleanupStack::PushL(dummyOb);
	CMsvSession* session = CMsvSession::OpenSyncL(*dummyOb);
	CleanupStack::PushL(session);

	// Test 1 - create entry without a specified ID.
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iType= KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	
	CreateEntryL(*session, entry1, KErrNone, NULL);
	
	// Test 2 - create entry with a specified ID
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iType= KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);

	CreateEntryL(*session, entry2, KErrNone, KMsvServerId);

	// Test 3 - change entry without a specified ID
	TTestMsvEntry entry3;
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iType= KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	
	CreateEntryL(*session, entry3, KErrNone, NULL);	// entry owned by this process
	ChangeEntryL(*session, entry3, KErrNone, NULL);
	
	// Test 4 - change entry with non-message server ID (implies forced update).
	TTestMsvEntry entry4;
	entry4.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry4.iType= KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	
	CreateEntryL(*session, entry4, KErrNone, NULL);
	ChangeEntryL(*session, entry4, KErrNone, KMsvServerId);
	
	// Create folders for the copy/move tests...
	TTestMsvEntry folder1;
	folder1.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder1.iType=KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDetails);
	CreateEntryL(*session, folder1, KErrNone, NULL);
	TTestMsvEntry folder2;
	folder2.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder2.iType=KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KLongDetails);
	CreateEntryL(*session, folder2, KErrNone, NULL);
	
	// Test 5 - move an entry.
	TTestMsvEntry entry5;
	entry5.SetParent(folder1.Id());
	entry5.iType= KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	
	CreateEntryL(*session, entry5, KErrNone, KMsvServerId);
	MoveCopyEntryL(*session, entry5, folder2.Id(), KErrNone, ETrue);
	
	// Test 6 - copy an entry.
	TTestMsvEntry entry6;
	entry6.SetParent(folder1.Id());
	entry6.iType= KUidMsvMessageEntry;
	entry6.iMtm = KUidMsvLocalServiceMtm;
	entry6.iServiceId = KMsvLocalServiceIndexEntryId;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KShortDetails);

	CreateEntryL(*session, entry6, KErrNone, KMsvServerId);
	MoveCopyEntryL(*session, entry6, folder1.Id(), KErrNone, EFalse);

	// Close the message server to check the results.
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(dummyOb);
	CMsgsTestUtils::WaitForServerClose();

	// CMvsServerEntry testing
	//
	// 1. Create entry with no specified owner ID - owner ID is KMsvServerId.
	// 2. Create entry with specified owner ID - owner ID is that specified.
	// 3. Change entry with no specified owner ID - no change in owner ID.
	// 4. Change entry with specified (different) owner ID - owner ID changes.
	
	theUtils->CleanMessageFolderL(KMsvTestIndexFile);
	
	CMsvServer* server = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(server);
	CleanupStack::PopAndDestroy(2); // server, testStore
	index = NULL;
	}
	
LOCAL_C void TestFilteredGetChildrenL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);
	TSecureId entryOwnerId;
	TMsvEntry* entryPtr = NULL;	
	TInt i;
	TSecureId owner1 = 0x0001;
	TSecureId owner2 = 0x0002;
	
	// Testing CMsvServerIndex::GetChildrenL - filtering against a specified
	// owner ID.
	// NOTE - no need to test different ordering as this is done else where.
	// Also, no need to test CMsvServerIndex::GetChildrenIdL as the work is for
	// this function is done by the same internal method as for GetChildrenL.
	
	// Test 1 - parent entry has no children.
	//
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, owner1, ETrue));

	// selection all children of root with none existing
	index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner1);
	test(selection->Count()==0);
	selection->Reset();

	// Test 2 - parent has several entries, all with same owner ID.
	//
	TTestMsvEntry entry1;
	entry1.SetParent(entry.Id());
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, owner1, ETrue));
	TTestMsvEntry entry2;
	entry2.SetParent(entry.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, owner1, ETrue));
	TTestMsvEntry entry3;
	entry3.SetParent(entry.Id());
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, owner1, ETrue));
	TTestMsvEntry entry4;
	entry4.SetParent(entry.Id());
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry4, owner1, ETrue));
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner1));
	test(selection->Count()==4);
	for (i=0; i<4; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==entry.Id());
		test(entryOwnerId==owner1);		
		}
	selection->Reset();
	
	// Test 3 - parent has several entries, NOT all with same owner ID. The
	// returned selection should only have entries with the same owner ID.
	//
	// Add a few of entries with a different owner ID.
	TTestMsvEntry entry5;
	entry5.SetParent(entry.Id());
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry5, owner2, ETrue));
	TTestMsvEntry entry6;
	entry6.SetParent(entry.Id());
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry6, owner2, ETrue));
	TTestMsvEntry entry7;
	entry7.SetParent(entry.Id());
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry7, owner2, ETrue));
	// 
	// Filter against owner1...
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner1));
	test(selection->Count()==4);
	for (i=0; i<4; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==entry.Id());
		test(entryOwnerId==owner1);		
		}
	selection->Reset();
	// 
	// Filter against owner2...
	//
	(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner2));
	test(selection->Count()==3);
	for (i=0; i<3; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==entry.Id());
		test(entryOwnerId==owner2);		
		}
	selection->Reset();
	
	// Test 4 - when filtering by owner ID, service entries are always included.
	// Get the children of the root entry.
	//
	TTestMsvEntry entry8;
	entry8.SetParent(KMsvRootIndexEntryId);
	entry8.iDescription.Set(KShortDescription);
	entry8.iDetails.Set(KShortDetails);
	entry8.iType = KUidMsvServiceEntry;
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	//
	(index->GetChildrenL(KMsvRootIndexEntryId, *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner1));
	test(selection->Count()==2);
	for (i=0; i<2; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==KMsvRootIndexEntryId);
		test(entryPtr->iType==KUidMsvServiceEntry);		
		}
	selection->Reset();
	
	// Test 5 - when filtering by owner ID, standard folders are always included.
	// Get children of the local service entry.
	//
	TTestMsvEntry entry9;
	entry9.SetParent(KMsvLocalServiceIndexEntryId);
	entry9.iDescription.Set(KShortDescription);
	entry9.iDetails.Set(KShortDetails);
	entry9.iType = KUidMsvFolderEntry;
	REPORT_IF_ERROR(index->AddEntry(entry9, owner1, ETrue));
	TTestMsvEntry entry10;
	entry10.SetParent(KMsvLocalServiceIndexEntryId);
	entry10.iDescription.Set(KShortDescription);
	entry10.iDetails.Set(KShortDetails);
	entry10.iType = KUidMsvFolderEntry;
	REPORT_IF_ERROR(index->AddEntry(entry10, owner2, ETrue));
	TTestMsvEntry entry11;
	entry11.SetParent(KMsvLocalServiceIndexEntryId);
	entry11.iDescription.Set(KShortDescription);
	entry11.iDetails.Set(KShortDetails);
	entry11.iType = KUidMsvFolderEntry;
	REPORT_IF_ERROR(index->AddEntry(entry11, owner2, ETrue));
	// 
	// Filter against owner1...
	//
	(index->GetChildrenL(KMsvLocalServiceIndexEntryId, *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner1));
	test(selection->Count()==5);
	for (i=0; i<5; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==KMsvLocalServiceIndexEntryId);
		test(entryPtr->StandardFolder() || entryOwnerId==owner1);		
		}
	selection->Reset();
	// 
	// Filter against owner2...
	//
	(index->GetChildrenL(KMsvLocalServiceIndexEntryId, *selection, TMsvSelectionOrdering(), KUidMsvNullEntry, ETrue, owner2));
	test(selection->Count()==6);
	for (i=0; i<6; ++i)
		{
		REPORT_IF_ERROR(index->GetEntry(selection->At(i)->Id(), entryPtr, entryOwnerId));
		test(entryPtr->Parent()==KMsvLocalServiceIndexEntryId);
		test(entryPtr->StandardFolder() || entryOwnerId==owner2);		
		}
	selection->Reset();
	
	CleanupStack::PopAndDestroy(2, index);	
	}
	
LOCAL_C TBool TestEntryInfo(CMsvServerIndex::TMsvServerEntryInfo aInfo,
							TMsvId			aId,
							TMsvId			aTopFolder,
							TMsvId			aService,
							TUid			aMtm,
							TUid			aType,
							TUid			aServiceMtm,
							TSecureId		aEntryOwnerId,
							TSecureId		aParentOwnerId,
							TBool			aPartOfMessage)
	{
	if( aInfo.iId != aId )
		return EFalse;
	if( aInfo.iTopFolder != aTopFolder )
		return EFalse;
	if( aInfo.iService != aService )
		return EFalse;
	if( aInfo.iMtm != aMtm )
		return EFalse;
	if( aInfo.iType != aType )
		return EFalse;
	if( aInfo.iServiceMtm != aServiceMtm )
		return EFalse;
	if( aInfo.iEntryOwnerId != aEntryOwnerId )
		return EFalse;
	if( aInfo.iParentOwnerId != aParentOwnerId )
		return EFalse;
	if( !((aInfo.iPartOfMessage && aPartOfMessage) || (!aInfo.iPartOfMessage && !aPartOfMessage)) )
		return EFalse;
	
	return ETrue;
	}
	
LOCAL_C void TestEntryTreeInfoL()
	
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	CMsvServerIndex::TMsvServerEntryInfo info1;
	TSecureId owner1 = 0x0001;
	TSecureId owner2 = 0x0002;
	TSecureId owner3 = 0x0003;
	TSecureId owner4 = 0x0004;

	// non-existant entry
	test(index->EntryTreeInfo(1234, info1)==KErrNotFound);
	
	// test root entry.
	REPORT_IF_ERROR(index->EntryTreeInfo(KMsvRootIndexEntryId, info1));
	test(TestEntryInfo(info1, 
		KMsvRootIndexEntryId,			// Id
		KMsvRootIndexEntryId,	 		// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvLocalServiceMtm,			// MTM
		KUidMsvRootEntry,				// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		KMsvServerId,					// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	// test local service and in/outboxes
	REPORT_IF_ERROR(index->EntryTreeInfo(KMsvLocalServiceIndexEntryId, info1));
	test(TestEntryInfo(info1, 
		KMsvLocalServiceIndexEntryId,	// Id
		KMsvNullIndexEntryIdValue, 		// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvLocalServiceMtm,			// MTM
		KUidMsvServiceEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		KMsvServerId,					// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));
	REPORT_IF_ERROR(index->EntryTreeInfo(KMsvGlobalInBoxIndexEntryId, info1));
	test(TestEntryInfo(info1, 
		KMsvGlobalInBoxIndexEntryId,	// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvLocalServiceMtm,			// MTM
		KUidMsvFolderEntry,				// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		KMsvServerId,					// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));
	REPORT_IF_ERROR(index->EntryTreeInfo(KMsvGlobalOutBoxIndexEntryId, info1));
	test(TestEntryInfo(info1, 
		KMsvGlobalOutBoxIndexEntryId,	// Id
		KMsvGlobalOutBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvLocalServiceMtm,			// MTM
		KUidMsvFolderEntry,				// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		KMsvServerId,					// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	// create an entry in inbox
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry1, owner1, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry1.Id(), info1));
	test(TestEntryInfo(info1, 
		entry1.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner1,							// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	// create an entry under entry in inbox
	TTestMsvEntry entry2;
	entry2.SetParent(entry1.Id());
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry2, owner2, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry2.Id(), info1));
	test(TestEntryInfo(info1, 
		entry2.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner2,							// Entry Owner ID
		owner1,							// Parent Owner ID
		ETrue							// Part of Message
		));
	
	// create a folder under inbox
	TTestMsvEntry folder;
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	folder.iType = KUidMsvFolderEntry;
	REPORT_IF_ERROR(index->AddEntry(folder, owner1, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(folder.Id(), info1));
	test(TestEntryInfo(info1, 
		folder.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvFolderEntry,				// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner1,							// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));
	
	// create a entry under folder in inbox
	TTestMsvEntry entry3;
	entry3.SetParent(folder.Id());
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry3, owner2, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry3.Id(), info1));
	test(TestEntryInfo(info1, 
		entry3.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner2,							// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	// create a entry under entry under folder in inbox
	TTestMsvEntry entry4;
	entry4.SetParent(entry3.Id());
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	entry4.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry4, owner3, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry4.Id(), info1));
	test(TestEntryInfo(info1, 
		entry4.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner3,							// Entry Owner ID
		owner2,							// Parent Owner ID
		ETrue							// Part of Message
		));

	// create an entry under entry under entry under folder in inbox!!
	TTestMsvEntry entry5;
	entry5.SetParent(entry4.Id());
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	entry5.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry5, owner4, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry5.Id(), info1));
	test(TestEntryInfo(info1, 
		entry5.Id(),					// Id
		KMsvGlobalInBoxIndexEntryId, 	// TopFolder
		KMsvLocalServiceIndexEntryId,	// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner4,							// Entry Owner ID
		owner2,							// Parent Owner ID
		ETrue							// Part of Message
		));

	// create another service (does not count as local)
	TTestMsvEntry service;
	service.SetParent(KMsvRootIndexEntryId); // CHANGE to root
	service.iType=KUidMsvServiceEntry;
	service.iMtm=KUidMsvLocalServiceMtm;
	service.iDescription.Set(KShortDescription);
	service.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(service, owner1, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(service.Id(), info1));
	test(TestEntryInfo(info1, 
		service.Id(),					// Id
		KMsvNullIndexEntryIdValue, 		// TopFolder
		service.Id(),					// Service
		KUidMsvLocalServiceMtm,			// MTM
		KUidMsvServiceEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner1,							// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	// create an entry under other service (is no longer local)
	TTestMsvEntry entry6;
	entry6.SetParent(service.Id());
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KShortDetails);
	entry6.iType = KUidMsvMessageEntry;
	REPORT_IF_ERROR(index->AddEntry(entry6, owner2, ETrue));
	REPORT_IF_ERROR(index->EntryTreeInfo(entry6.Id(), info1));
	test(TestEntryInfo(info1, 
		entry6.Id(),					// Id
		KMsvNullIndexEntryIdValue, 		// TopFolder
		service.Id(),					// Service
		KUidMsvNullEntry,				// MTM
		KUidMsvMessageEntry,			// Type
		KUidMsvLocalServiceMtm,			// Service MTM
		owner2,							// Entry Owner ID
		KMsvServerId,					// Parent Owner ID
		EFalse							// Part of Message
		));

	CleanupStack::PopAndDestroy(2, testStore);	
	}

LOCAL_C void TestStoreManagerL()
	{
  	TFileName fileName(_L("amethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethatneedstobetrimmedalongnamethangextens.aloona"));
 
	CMsvServer* server = CMsgsTestUtils::CreateServerL();
  	CleanupStack::PushL(server);
  	RFs fs;
  	fs.Connect();
  		
  	server->ServerStoreManager().SetMsvServerAndFileSession(*server,fs);
    	
  	RFile file;
  	TBool fileNameChanged = server->ServerStoreManager().CreateAttachmentForWriteL(0, fileName, file);
  	CleanupStack::PopAndDestroy(server);
  	}

	
LOCAL_C void doTestsL()
	{
	theUtils->FileSession().Delete(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestDummyFile);

	DummyStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestDummyFile, EFileWrite|EFileShareAny);
	DummyStore->SetTypeL(KPermanentFileStoreLayoutUid);

	// testing normal behaviours

	CMsgsTestUtils::Start(_L("Normal behaviour"));
	TestIndexAllocL();
	TestIndexIdsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Selections"));
	TestChildrenSelectionL();
	TestInvisibleSelectionL();
	TestMtmSelectionL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Locking"));
	TestLockingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Changing"));
	TestChangingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("File syncing"));
	TestFileSyncingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Long descriptors"));
	TestLongDescriptorsL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Local"));
	TestLocalL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Deleting"));
	TestDeletingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Moving"));
	TestMovingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Flags"));
	TestFlagsL();
	TestTemporaryFlagsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Read only"));
	TestReadOnlyL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Descendents"));
	TestDescendentsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Attributes"));
	TestAttributesL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asynchronous Opening"));
	TestAsyncL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Selection deletion"));
	TestSelectionDeletion1L();
	TestSelectionDeletion2L();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Owner ID"));
	TestOwnerIdL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("GetChildren filtered against owner ID"));
	TestFilteredGetChildrenL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("EntryTreeInfo"));
	TestEntryTreeInfoL();
	CMsgsTestUtils::Complete();

 	CMsgsTestUtils::Next(_L("Store manager"));
 	TestStoreManagerL();
 	CMsgsTestUtils::Complete();

	theUtils->FileSession().Delete(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestDummyFile);

	CleanupStack::PopAndDestroy(); // DummyStore
	}

#pragma warning( default : 4710 )
