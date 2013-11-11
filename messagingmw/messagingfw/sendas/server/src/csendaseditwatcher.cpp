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

#include "csendaseditwatcher.h"

#include <msvids.h>
#include <mtclbase.h>
#include <csendaseditutils.h>

#include "msendaseditobserver.h"
#include "tsendasserverpanic.h"

CSendAsEditWatcher* CSendAsEditWatcher::NewL(MSendAsEditObserver& aObserver, TUid aUid)
	{
	CSendAsEditWatcher* self = new (ELeave) CSendAsEditWatcher(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aUid);
	CleanupStack::Pop(self);
	return self;
	}

CSendAsEditWatcher::~CSendAsEditWatcher()
	{
	// do not notify observer during deletion.
	iObserver = NULL;
	Cancel();

	delete iUtils;
	}

CSendAsEditWatcher::CSendAsEditWatcher(MSendAsEditObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(&aObserver)
	{
	CActiveScheduler::Add(this);
	}
	
void CSendAsEditWatcher::ConstructL(TUid aUid)
	{
	// Create the edit utils (loads Ecom plugin specified by aUId)
	iUtils = CSendAsEditUtils::NewL(aUid);
	}
	
void CSendAsEditWatcher::LaunchEditorL(TMsvId aEntryId)
	{
	__ASSERT_ALWAYS( iState == EEditWatcherIdle, User::Invariant() );
	
	// launch the editor
	iUtils->LaunchEditorL(aEntryId, iStatus);
	SetActive();
		
	iState = EEditWatcherEditing;
	}

void CSendAsEditWatcher::Complete(TInt aError)
	{
	iState = EEditWatcherDone; // This must be set before notifying observer
	if( iObserver != NULL )
		{
		// notify observer that the editor thread has closed.
		iObserver->EditComplete(aError);
		}
	}
	
/*
 *	Methods from CActive
 */
 
void CSendAsEditWatcher::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch( iState )
		{
	case EEditWatcherEditing:
		{
		Complete(KErrNone);
		} break;
	case EEditWatcherIdle:
	case EEditWatcherDone:
	default:
		User::Invariant();
		}
	}
	
void CSendAsEditWatcher::DoCancel()
	{
	switch( iState )
		{
	case EEditWatcherEditing:
		{
		iUtils->Cancel();
		}
	case EEditWatcherIdle:
	case EEditWatcherDone:
	default:
		// do nothing...
		break;
		}
	Complete(KErrCancel);
	}
	
TInt CSendAsEditWatcher::RunError(TInt aError)
	{
	Complete(aError);
	return KErrNone;
	}
