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
#include <mtsr.h>
#include <msventry.h>


#define _LOCAL_L(a) (TPtrC8((const TText8 *)(a)))


NONSHARABLE_CLASS  (CMsvWait): public CActive
/**
@internalComponent
@released
*/
	{
public:
	CMsvWait();
	~CMsvWait();
	inline void Start();
protected:
	void RunL();
	void DoCancel();
	};

inline void CMsvWait::Start()
	{
	SetActive();
	}


CMsvWait::CMsvWait() : CActive(EPriorityHigh)
	{
	CActiveScheduler::Add(this);
	}

CMsvWait::~CMsvWait()
	{
	Cancel();
	}

void CMsvWait::RunL()
	{
	CActiveScheduler::Stop();
	}

void CMsvWait::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}


class CTestServerMtm : public CBaseServerMtm
	{
public:
	IMPORT_C static CTestServerMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	~CTestServerMtm();

	void CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void CopyFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void CopyWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void MoveWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	void DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	void CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	void ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	//
	void StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
	//
	TBool CommandExpected();
	//
	const TDesC8& Progress();
	//
	void SetInitialEntry(CMsvServerEntry* aEntry);
	//

protected:
	void DoCancel();
	void DoRunL();
	void DoComplete(TInt aError);

private:
	RTimer iTimer;
	CTestServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	void ConstructL();
	void DoCopyEntriesL();
	void DoCopyEntryL(TMsvId aId);
	void DoMoveEntriesL();

private:
	const CMsvEntrySelection* iSelection;
	TInt iIntParam;
	TRequestStatus* iObserverStatus;
	CMsvServerEntry* iNewParent;
	TBuf8<32> iProgress;
	TBuf8<32> iPersistentBuffer;
	TBool iPersistent;
	RFs iFs;
	};


EXPORT_C CTestServerMtm* CTestServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry)
	{
	CleanupStack::PushL(aInitialEntry); // Take ownership of aInitialEntry
	CTestServerMtm* mysvrmtm=new(ELeave) CTestServerMtm(aRegisteredMtmDll, aInitialEntry);
	CleanupStack::Pop(); // aInitialEntry - Entry now safely stored in member

	CleanupStack::PushL(mysvrmtm);
	mysvrmtm->ConstructL();
	CleanupStack::Pop();
	return mysvrmtm;
	}

CTestServerMtm::CTestServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry):
	CBaseServerMtm(aRegisteredMtmDll, aInitialEntry), iPersistent(EFalse)
	{
	__DECLARE_NAME(_S("CTestServerMtm"));
	}
		
void CTestServerMtm::ConstructL()
	{
	CActiveScheduler::Add(this);
	User::LeaveIfError(iFs.Connect());
	TUint att;
	if (iFs.Att(_L("c:\\block test mtm start"), att)==KErrNone)
		User::Leave(-101);
	}

CTestServerMtm::~CTestServerMtm()
	{
	Cancel();
	iFs.Close();
	}

void CTestServerMtm::CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)
	{
	iSelection = &aSelection;
	iIntParam = aDestination;
	iObserverStatus = &aStatus;
	DoCopyEntriesL();
	iProgress = _LOCAL_L("copied to local");
	}

void CTestServerMtm::CopyFromLocalL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aStatus)
	{
	iSelection = &aSelection;
	iIntParam = aDestination;
	iObserverStatus = &aStatus;
	DoCopyEntriesL();
	iProgress = _LOCAL_L("copied from local");
	}

void CTestServerMtm::CopyWithinServiceL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aStatus)
	{
	iSelection = &aSelection;
	iIntParam = aDestination;
	iObserverStatus = &aStatus;
	DoCopyEntriesL();
	iProgress = _LOCAL_L("copied within service");
	}

void CTestServerMtm::MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)
	{
	iSelection = &aSelection;
	iIntParam = aDestination;
	iObserverStatus = &aStatus;
	DoMoveEntriesL();
	iProgress = _LOCAL_L("moved to local");
	}

void CTestServerMtm::MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)
	{
	iSelection = &aSelection;
	iIntParam = aDestination;
	iObserverStatus = &aStatus;
	DoMoveEntriesL();
	iProgress = _LOCAL_L("moved from local");
	}

void CTestServerMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, KErrNone);
	iProgress = _LOCAL_L("moved within service");
	}


void CTestServerMtm::DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
	{
	// set the main server entry up to the parent
	TInt error = iServerEntry->SetEntry(aSelection.At(0));

	if (error==KErrNone)
		{
		iServerEntry->SetEntry(iServerEntry->Entry().Parent());
		// delete the entry recursively
		error = iServerEntry->DeleteEntry(aSelection.At(0));
		}

	iProgress = _LOCAL_L("deleted entry recursively");

	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, error);
	}

void CTestServerMtm::CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus)
	{
	TInt error = iServerEntry->SetEntry(aNewEntry.Parent());
	if (!error)
		error = iServerEntry->CreateEntry(aNewEntry);

	iProgress = _LOCAL_L("added entry");
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, error);
	}

void CTestServerMtm::ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus)
	{
	TInt error = iServerEntry->SetEntry(aNewEntry.Id());
	if (!error)
		error = iServerEntry->ChangeEntry(aNewEntry);
	
	iProgress = _LOCAL_L("changed entry");
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, error);
	}

void CTestServerMtm::StartCommandL(CMsvEntrySelection& /*aSelection*/, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)
	{
	iObserverStatus = &aStatus;
	
	if (aCommand<5 || aCommand>100)
		{
		// completes after aCommand second
		SetActive();
		iTimer.CreateLocal();
		iTimer.After(iStatus, aCommand*1000000);
		//
		iProgress = _LOCAL_L("timer running");
		*iObserverStatus=KRequestPending;
		return;
		}

	switch (aCommand)
		{
		case 10:
			// makes the MTM persistent, store the parameter
			iPersistent = ETrue;
			iPersistentBuffer = aParameter;
			iProgress = _LOCAL_L("persistent");
			break;
		case 11:
			// makes the MTM non-persistent, returns the persisted buffer as progress
			iPersistent = EFalse;
			iProgress = iPersistentBuffer;
			iPersistentBuffer = TPtrC8();
			break;
		case 12:
			// sets the progress to the paaremter
			iProgress = aParameter;
			break;
		case 13:
			// always leave
			User::Leave(-1300);
			return;
		default:
			break;
		}
	*iObserverStatus=KRequestPending;
	User::RequestComplete(iObserverStatus, KErrNone);
	}


void CTestServerMtm::DoRunL()
	{
	User::RequestComplete(iObserverStatus, iStatus.Int());
	iProgress = _LOCAL_L("timer completed");
	}

void CTestServerMtm::DoComplete(TInt /*aError*/)
	{
	}

TBool CTestServerMtm::CommandExpected()
	{
	return iPersistent;
	}

const TDesC8& CTestServerMtm::Progress()
	{
	return iProgress;
	}

void CTestServerMtm::DoCancel()
	{
	iProgress = _LOCAL_L("cancelled");
	iTimer.Cancel();
	User::RequestComplete(iObserverStatus, KErrCancel);	
	}





void CTestServerMtm::DoCopyEntriesL()
//
// just copies the index entries
//
	{
	// set the main server entry up to the curent parent
	User::LeaveIfError(iServerEntry->SetEntry(iServerEntry->Entry().Parent()));

	// create a new server entry for the new parent
	iNewParent = iServerEntry->NewEntryL(iIntParam);

	// copy each index entry
	for (TInt count=0; count<iSelection->Count(); count++)
		{
		TRAPD(leave, DoCopyEntryL(iSelection->At(count)));
		}
	
	User::RequestComplete(iObserverStatus, KErrNone);
	
	delete iNewParent;
	iNewParent=NULL;
	}



void CTestServerMtm::DoCopyEntryL(TMsvId aId)
//
//
//
	{
	User::LeaveIfError(iServerEntry->SetEntry(aId));
	
	// create a new entry
	TMsvEntry newEntry(iServerEntry->Entry());

	// add the new entry to the index under the new parent
	User::LeaveIfError(iNewParent->CreateEntry(newEntry));
	}



void CTestServerMtm::DoMoveEntriesL()
//
// just copies the index entries
//
	{
	User::LeaveIfError(iServerEntry->SetEntry(iSelection->At(0)));
	User::LeaveIfError(iServerEntry->SetEntry(iServerEntry->Entry().Parent()));
	CMsvWait* wait = new(ELeave)CMsvWait;
	CleanupStack::PushL(wait);
	for (TInt count=0; count<iSelection->Count(); count++)
		{
		TRAPD(leave, iServerEntry->MoveEntryL(iSelection->At(count), iIntParam, wait->iStatus));
		if (leave==KErrNone)
			{
			wait->Start();
			CActiveScheduler::Start();
			}
		}
	CleanupStack::PopAndDestroy();
	User::RequestComplete(iObserverStatus, KErrNone);
	}

