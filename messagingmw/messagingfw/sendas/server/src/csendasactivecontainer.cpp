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

#include "csendasactivecontainer.h"

#include "csendassender.h"
#include "csendaseditwatcher.h"
#include "csendasserver.h"

CSendAsActiveContainer* CSendAsActiveContainer::NewL(CSendAsServer& aServer)
	{
	CSendAsActiveContainer* self = new(ELeave) CSendAsActiveContainer(aServer);
	return self;
	}
	
CSendAsActiveContainer::~CSendAsActiveContainer()
	{
	// clean up
	Cancel();
	iActiveList.ResetAndDestroy();
	iActiveList.Close();
	}

CSendAsActiveContainer::CSendAsActiveContainer(CSendAsServer& aServer)
: CActive(EPriorityStandard), iServer(aServer)
	{
	CActiveScheduler::Add(this);
	}

void CSendAsActiveContainer::AddSenderL(CSendAsSender& aSender)
	{
	// need to take ownership of the sender - place on cleanup stack.
	CleanupStack::PushL(&aSender);
	iActiveList.AppendL(&aSender);
	CleanupStack::Pop(&aSender);
	}
	
void CSendAsActiveContainer::AddEditWatcherL(CSendAsEditWatcher& aEditWatcher)
	{
	// need to take ownership of the edit watcher - place on cleanup stack.
	CleanupStack::PushL(&aEditWatcher);
	iActiveList.AppendL(&aEditWatcher);
	CleanupStack::Pop(&aEditWatcher);
	}
	
TBool CSendAsActiveContainer::IsEmpty() const
	{
	return (iActiveList.Count() == 0);
	}
	
void CSendAsActiveContainer::PurgeInactive()
	{
	// self-complete if not already active - the RunL will then delete any 
	// completed active objects in the list.
	if( !IsActive() )
		{
		TRequestStatus* status=&iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}
	
/*
 *	Methods from CActive
 */

void CSendAsActiveContainer::RunL()
	{
	// remove all completed operations from lists
	TInt count = iActiveList.Count();
	while( count-- > 0 )
		{
		if( !iActiveList[count]->IsActive() )
			{
			delete iActiveList[count];
			iActiveList.Remove(count);
			}
		}
	// notify the server if there are no more actions waiting to complete.
	if( iActiveList.Count() == 0 )
		{
		iServer.ContainerEmpty();
		}
	}
	
void CSendAsActiveContainer::DoCancel()
	{
	// nothing to do really...
	}

/*
 *	Methods from MSendAsSenderObserver
 */

void CSendAsActiveContainer::SenderComplete(TInt /*aError*/)
	{
	// self-complete if not already active - the RunL will then delete all 
	// completed active objects in the list.
	if( !IsActive() )
		{
		TRequestStatus* status=&iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}
	
/*
 *	Methods from MSendAsEditObserver
 */

void CSendAsActiveContainer::EditComplete(TInt /*aError*/)
	{
	// self-complete if not already active - the RunL will then delete all 
	// completed active objects in the list.
	if( !IsActive() )
		{
		TRequestStatus* status=&iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}
