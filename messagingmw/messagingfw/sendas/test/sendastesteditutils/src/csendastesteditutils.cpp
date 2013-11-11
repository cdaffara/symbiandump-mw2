// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendastesteditutils.h"
#include "csendastestedittimer.h"

#include <ecom/implementationproxy.h>
#include <msvids.h>

// local constants
const TInt KSendAsTestEditWaitTime = 5000000; // time in ms

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x1020D464, CSendAsTestEditUtils::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}


CSendAsTestEditUtils* CSendAsTestEditUtils::NewL()
	{
	CSendAsTestEditUtils* self=new (ELeave) CSendAsTestEditUtils();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CSendAsTestEditUtils::CSendAsTestEditUtils()
: CSendAsEditUtils()
	{
	}
	
CSendAsTestEditUtils::~CSendAsTestEditUtils()
	{
	delete iEditTimer;
	}
	
void CSendAsTestEditUtils::ConstructL()
	{
	iEditTimer = CSendAsTestEditTimer::NewL(*this);
	}
	
// methods from CSendAsEditUtils
void CSendAsTestEditUtils::LaunchEditorL(TMsvId /*aId*/, TRequestStatus& aStatus)
	{

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	CMsvEntry* cEntry2 = session->GetEntryL(selection->At(0));
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry = cEntry2->Entry();
	entry.SetMtmData3(234567890); // Show we've been called by touching the TMsvEntry.
	cEntry2->ChangeL(entry);
	
	CleanupStack::PopAndDestroy(5, ob1); // cEntry2, selection, cEntry, session, ob1

	iUserStatus = &aStatus;
	aStatus = KRequestPending;
	// wait a few seconds before completing
	iEditTimer->After(KSendAsTestEditWaitTime);
	}
	
void CSendAsTestEditUtils::LaunchEditorAndWaitL(TMsvId /*aId*/)
	{
	}

void CSendAsTestEditUtils::Cancel()
	{
	iEditTimer->Cancel();
	}

void CSendAsTestEditUtils::TimerDone()
	{
	// complete the request (simulates editor closing)
	User::RequestComplete(iUserStatus, KErrNone);
	}

