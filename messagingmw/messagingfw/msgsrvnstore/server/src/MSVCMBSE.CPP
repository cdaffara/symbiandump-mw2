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

#include <e32base.h>

#include "MSVCMBSE.H"
#include "MSVPANIC.H"

//**********************************
// CMsvCopyMoveEntriesBase
//**********************************


CMsvCopyMoveEntriesBase::CMsvCopyMoveEntriesBase()
: CActive(EPriorityStandard)
//
//
//
	{}


CMsvCopyMoveEntriesBase::~CMsvCopyMoveEntriesBase()
//
//
//
	{
	Cancel();

	delete iFailedIds;
	delete iCompletedIds;
	}


void CMsvCopyMoveEntriesBase::ConstructL()
//
//
//
	{
	iFailedIds = new(ELeave) CMsvEntrySelection;
	iCompletedIds = new(ELeave) CMsvEntrySelection;

	CActiveScheduler::Add(this);
	}

void CMsvCopyMoveEntriesBase::StartL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aObserverStatus)
//
//
//
	{
	__ASSERT_DEBUG(aSelection.Count() > 0, PanicServer(EMsvEmptySelection));

	iCompletedIds->Reset();
	iCompletedIds->SetReserveL(aSelection.Count());

	iFailedIds->Reset();
	iFailedIds->AppendL(aSelection.Back(0), aSelection.Count());

	iCurrentIndex = iFailedIds->Count();
	iError = KErrNone;

	iTargetId = aTargetId;

	if (!StartNext())
		{
		__ASSERT_DEBUG(iError, PanicServer(EMsvNoErrorSet));
		User::Leave(iError);
		}

	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;
	}

void CMsvCopyMoveEntriesBase::RunL()
//
//
//
	{
	if (iStatus != KErrNone)
		{
		if (iError == KErrNone)
			iError = iStatus.Int();
		}
	else
		{
		iCompletedIds->AppendL(iFailedIds->At(iCurrentIndex));
		iFailedIds->Delete(iCurrentIndex);
		}

	if (!StartNext())
		User::RequestComplete(iObserverStatus, iError);
	}

TBool CMsvCopyMoveEntriesBase::StartNext()
	{
	while(iCurrentIndex > 0)
		{
		TRAPD(error, DoStartNextL());
		if (error)
			{
			if (iError == KErrNone)
				iError = error;
			}
		else
			return ETrue;
		}

	return EFalse;
	}

void CMsvCopyMoveEntriesBase::DoStartNextL()
	{
	__ASSERT_DEBUG(iCurrentIndex > 0, PanicServer(EMsvInvalidCurrentIndex));

	iCurrentIndex--;
	DoStartL(iFailedIds->At(iCurrentIndex), iTargetId, iStatus);
	SetActive();
	}
