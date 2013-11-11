// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// A test wrapper over the CSuplWdpWatcher class.
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include <wapmessage.h>

#include "Te_WatcherLog.h"
#include "Te_WapBoundDatagramService.h"


#define CWatcherLog CTe_WatcherLog
#define TWatcherParams TTe_WatcherParams
#define CLbsSuplPush CTe_LbsSuplPush
#define CWapBoundDatagramService CTe_WapBoundDatagramService


#include "Te_SuplWdpWatcher.h"

TBool CTe_LbsSuplWdpWatcher::iEnableAsserts = ETrue;

/** Redefines the ASSERT_DEBUG macro to be able to turn ot onn/off at run-time. */
#undef __ASSERT_DEBUG
#define __ASSERT_DEBUG(c,p) (void)((c || !CTe_LbsSuplWdpWatcher::AssertsEnabled()) ||(p,0)) 


#include "LbsSuplWdpWatcher.cpp"


/**
Static factory method for creating an instance of the CLbsSuplWdpWatcher class and
leaving it on the cleanup stack.

@param aWatcherLog [In] A pointer on the test watcher log simulator.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CLbsSuplWdpWatcher* CLbsSuplWdpWatcher::NewLC(CTe_WatcherLog* aWatcherLog)
	{
	__ASSERT_ALWAYS(aWatcherLog!=0, User::Invariant());
	//Two lines below - to avoid a false Leavescan error 
	CLbsSuplWdpWatcher* self= new CLbsSuplWdpWatcher(CActive::EPriorityStandard, *aWatcherLog);
	User::LeaveIfNull(self); //A false Leavescan error because of the __ASSERT_DEBUG redefinition above
	self->iLocalPort = KSuplSmsTriggerWdpPort+1;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	
	}

/**
Checks that the object in one of the possible states and check that the internal state of the object
is in the agreement with the state it is currently in. 
	
@param aPossibleStates  [In] The bitmask of the possible states.
	
@leave If the object is not in one of the possible states or the internal state of the object
is not in the agreement with the state it is currently in, this method will leave with error 
code KErrGeneral.

@see CLbsSuplWdpWatcher::TState
*/
void CLbsSuplWdpWatcher::CheckStateL(TInt aPossibleStates)
	{
	
	if((iState & aPossibleStates)==0)
		{
		User::Leave(KErrGeneral);
		}
	
	if(EFalse==IsAdded() || !iTimer || iTruncated)
		{
		User::Leave(KErrGeneral);
		}
	
	switch(iState)
		{
		case EDisconnected:
			if(IsActive() || EFalse==iTimer->IsActive() || iData.MaxLength()>0 || iWdpConn)
				{
				User::Leave(KErrGeneral);
				}
			break;
		case EAwaitingDataSize:
		case EAwaitingData:
			if(EFalse==IsActive() || iTimer->IsActive() || !iWdpConn)
				{
				User::Leave(KErrGeneral);
				}
			
			if(iState==EAwaitingDataSize)
				{
				if(iData.MaxLength()>0)
					{
					User::Leave(KErrGeneral);
					}
				}
			else //EAwaitingData
				{
				if(iDataSize==0 || iData.MaxLength()<iDataSize)
					{
					User::Leave(KErrGeneral);
					}
				}
			break;
		default:
			__ASSERT_ALWAYS(0, User::Invariant());
			break;
		}	
	}

