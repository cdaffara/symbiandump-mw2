/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/
#include <e32svr.h>
#include <harvestereventenum.h>

#include "harvestereventobserverao.h"
#include "harvesterclient.h"

const TInt KHarverterEventQueueSize = 32;

CHarvesterEventObserverAO::CHarvesterEventObserverAO( 
		RHarvesterClient& aHarvesterClient ) :
	CActive(CActive::EPriorityStandard), // Standard priority
	iHarvesterClient( aHarvesterClient )
	{
	}

CHarvesterEventObserverAO* CHarvesterEventObserverAO::NewLC( 
		RHarvesterClient& aHarvesterClient )
	{
	CHarvesterEventObserverAO* self = 
		new ( ELeave ) CHarvesterEventObserverAO( aHarvesterClient );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CHarvesterEventObserverAO* CHarvesterEventObserverAO::NewL( 
		RHarvesterClient& aHarvesterClient )
	{
	CHarvesterEventObserverAO* self = 
		CHarvesterEventObserverAO::NewLC( aHarvesterClient );
	CleanupStack::Pop( self ); // self;
	return self;
	}

void CHarvesterEventObserverAO::ConstructL()
	{
	CActiveScheduler::Add( this ); // Add to scheduler
	}

CHarvesterEventObserverAO::~CHarvesterEventObserverAO()
	{
	Cancel(); // Cancel any request, if outstanding

	for(TInt i = iObservers.Count(); --i >= 0;)
		{
		THarvesterEventObserver& observer = *(iObservers[i]);

		TIpcArgs ipcArgs( observer.iObserverInfo.iObserverId, 
				iHarvesterEventQueue.Handle() );

		iHarvesterClient.Send( EUnregisterHarvesterEvent, ipcArgs );
		}

	iHarvesterEventQueue.Close();
	
	iObservers.ResetAndDestroy();
	}

void CHarvesterEventObserverAO::DoCancel()
	{
	iHarvesterEventQueue.CancelDataAvailable();
	}

void CHarvesterEventObserverAO::AddHarvesterEventObserverL( 
			MHarvesterEventObserver& aHarvesterEventObserver, 
			TInt aHEObserverType,
    		TInt aNotificationInterval )
	{
	RHandleBase& queueHandle = GetEventQueueHandleL();

	THarvesterEventObserver* observer = 
			new (ELeave) THarvesterEventObserver( aHarvesterEventObserver );
	CleanupStack::PushL( observer );

	// Init client side values
	observer->iObserverInfo.iQueueHandle = queueHandle.Handle();
	observer->iObserverInfo.iObserverId = NextObserverId();
	observer->iObserverInfo.iObserverType = aHEObserverType;
	observer->iObserverInfo.iNotificationInterval = aNotificationInterval;

	iObservers.AppendL( observer );

	CleanupStack::Pop( observer );

	TPckg<THarvesterEventObserverInfo> pckgObserverInfo( observer->iObserverInfo );
	TIpcArgs ipcArgs( &pckgObserverInfo, queueHandle );

	TInt err = iHarvesterClient.SendReceive( ERegisterHarvesterEvent, ipcArgs );

	User::LeaveIfError( err );

	if( !IsActive() )
		{
		iHarvesterEventQueue.NotifyDataAvailable( iStatus );
		SetActive(); // Tell scheduler a request is active
		}
	}

void CHarvesterEventObserverAO::RemoveHarvesterEventObserverL( 
		MHarvesterEventObserver& aHarvesterEventObserver )
	{
	// clear all observers which match to callback instance
	for(TInt i = iObservers.Count(); --i >= 0;)
		{
		THarvesterEventObserver* observer = iObservers[i];
		if( &observer->iObserver == &aHarvesterEventObserver )
			{
			TIpcArgs ipcArgs( observer->iObserverInfo.iObserverId, 
							iHarvesterEventQueue.Handle() );
			iHarvesterClient.Send( EUnregisterHarvesterEvent, ipcArgs );
			iObservers.Remove( i );
			delete observer;
			observer = NULL;
			}
		}

	// close event queue if all observers are removed
	if( iObservers.Count() == 0 )
		{
		Cancel();
		iHarvesterEventQueue.Close();
		}
	
	iObservers.Compress();
	}

void CHarvesterEventObserverAO::RunL()
	{
	User::LeaveIfError( iStatus.Int() );

	THarvesterEventNotification received;
	while( iHarvesterEventQueue.Receive( received ) != KErrUnderflow )
		{
	    const TInt count( iObservers.Count() );
		for(TInt i = count; --i >= 0;)
			{
			THarvesterEventObserver& observer = *(iObservers[i]);
			if( observer.iObserverInfo.iObserverId == received.iObserverId )
				{
				observer.iObserver.HarvestingUpdated( received.iObserverType,  
						received.iCurrentState, received.iItemsLeft );
				break;
				}
			}
		}

	if( iObservers.Count() > 0 )
		{
		SetActive();
		iHarvesterEventQueue.NotifyDataAvailable( iStatus );
		}
	}

TInt CHarvesterEventObserverAO::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

TUint CHarvesterEventObserverAO::NextObserverId()
	{
	// Sort by observer IDs
	iObservers.Sort( TLinearOrder<THarvesterEventObserver>(
			THarvesterEventObserver::CompareObserverIds ) );

	// Find smallest unused observer ID 
	const TUint count = iObservers.Count();
	for( TInt i = count - 1; i >=0; i-- )
		{
		// Return first free observer ID 
		if( iObservers[i]->iObserverInfo.iObserverId != i )
			{
			return i;
			}
		}

	if( count )
	    {
	    // No free observer ID found, so return largest observer ID + 1 
	    return iObservers[count - 1]->iObserverInfo.iObserverId + 1;
	    }
	// No observer IDs assigned
	return 0;
	}

RHandleBase& CHarvesterEventObserverAO::GetEventQueueHandleL()
	{
	// create new message queue
	if( KNullHandle == iHarvesterEventQueue.Handle() )
		{
		TInt err = iHarvesterEventQueue.CreateGlobal( KNullDesC, KHarverterEventQueueSize );
		User::LeaveIfError( err );
		}

	return iHarvesterEventQueue;
	}
