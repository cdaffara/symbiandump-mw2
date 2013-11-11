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

#include <msvstd.h>
#include <msvapi.h>
#include <msvids.h>
#include "MSGQRY.H"

CMessageQuery* CMessageQuery::NewLC(CMsvSession& aSession, TInt aPriority)
	{
	CMessageQuery* self = new(ELeave)CMessageQuery(aSession, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMessageQuery* CMessageQuery::NewL(CMsvSession& aSession, TInt aPriority)
	{
	CMessageQuery* self = NewLC(aSession, aPriority);
	CleanupStack::Pop(); // self
	return self;
	}

CMessageQuery::CMessageQuery(CMsvSession& aSession, TInt aPriority)
: CMsgActive(aPriority), iSession(aSession)
	{
	}

void CMessageQuery::ConstructL()
	{
	iSelection = new(ELeave)CMsvEntrySelection;
	iChildSelection = new(ELeave)CMsvEntrySelection;
	iBlankFilter = CMsvEntryFilter::NewL();

	CActiveScheduler::Add(this);
	}

CMessageQuery::~CMessageQuery()
	{
	delete iSelection;
	delete iChildSelection;
	delete iBlankFilter;
	}

void CMessageQuery::GetItemIdsL(const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
	{
	__ASSERT_ALWAYS(aSelection.Count() == 0, User::Invariant());
	
	// Remember the parameters
	iResultSelection = &aSelection;
	iFilter = &aFilter;

	// Get children of local service
	iSession.GetChildIdsL(KMsvLocalServiceIndexEntryId, *iBlankFilter, *iChildSelection);

	// Get children of local service that satisfy the filter
	iSession.GetChildIdsL(KMsvLocalServiceIndexEntryId, *iFilter, *iResultSelection);

	// Start the asynchronous operation
	Queue(aStatus);
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

void CMessageQuery::DoRunL()
	{
	// Is there anything else todo?
	if (iChildSelection->Count())
		{
		// Get next entry to process
		TMsvId id = iChildSelection->At(0);
		iChildSelection->Delete(0);

		// Get child entries and add to child list
		iSession.GetChildIdsL(id, *iBlankFilter, *iSelection);
		if (iSelection->Count())
			{
			iChildSelection->AppendL(iSelection->Back(0), iSelection->Count());
			iSelection->Reset();
			}

		// Get child entries that satisfy the filter
		iSession.GetChildIdsL(id, *iFilter, *iSelection);
		if (iSelection->Count())
			{
			iResultSelection->AppendL(iSelection->Back(0), iSelection->Count());
			iSelection->Reset();
			}

		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}
