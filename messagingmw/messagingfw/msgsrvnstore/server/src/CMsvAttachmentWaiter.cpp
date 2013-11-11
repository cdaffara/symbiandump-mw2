// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CMsvAttachmentWaiter.h"
#include <mmsvattachmentmanager.h>
#include "MSVSTORE.H"

CMsvAttachmentWaiter* CMsvAttachmentWaiter::NewL()
	{
	CMsvAttachmentWaiter* self = new(ELeave) CMsvAttachmentWaiter();
	return self;
	}
	
CMsvAttachmentWaiter::CMsvAttachmentWaiter()
	: CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

CMsvAttachmentWaiter::~CMsvAttachmentWaiter()
	{
	Cancel();
	delete iStore;
	}	

void CMsvAttachmentWaiter::StartWaitingL(TRequestStatus& aStatus, CMsvStore* aStore, MMsvAttachmentManager* aAttachmentManager)
	{
	if( IsActive() )
		{
		aAttachmentManager->CancelRequest();
		User::Leave(KErrInUse);
		}
	
	aStatus = KRequestPending;
	iReportStatus = &aStatus;
	iStore = aStore;
	iAttachmentManager = aAttachmentManager;
	SetActive();
	}

void CMsvAttachmentWaiter::Reset()
	{
	iReportStatus = NULL;
	iAttachmentManager = NULL;
	delete iStore;
	iStore = NULL;
	}

void CMsvAttachmentWaiter::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	iStore->CommitL();
	User::RequestComplete(iReportStatus, KErrNone);
	Reset();
	}
	
void CMsvAttachmentWaiter::DoCancel()
	{
	iAttachmentManager->CancelRequest();
	User::RequestComplete(iReportStatus, KErrCancel);
	Reset();
	}

TInt CMsvAttachmentWaiter::RunError(TInt aError)
	{
	User::RequestComplete(iReportStatus, aError);
	Reset();
	return KErrNone;
	}
