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
*/

#ifndef HARVESTEREVENTOBSERVERAO_H
#define HARVESTEREVENTOBSERVERAO_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <e32msgqueue.h>

#include "harvestercommon.h"

class RHarvesterClient;
class MHarvesterEventObserver;

struct THarvesterEventObserver
	{
	MHarvesterEventObserver& iObserver;
	THarvesterEventObserverInfo iObserverInfo;
	
	THarvesterEventObserver(MHarvesterEventObserver& aObserver) : iObserver(aObserver) {}
	
	static TInt CompareObserverIds(
			const THarvesterEventObserver& aFirst, 
			const THarvesterEventObserver& aSecond)
		{
		if( aFirst.iObserverInfo.iObserverId < aSecond.iObserverInfo.iObserverId )
			{
			return -1;
			}
		else if( aFirst.iObserverInfo.iObserverId > aSecond.iObserverInfo.iObserverId )
			{
			return 1;
			}
		else
			{
			return 0;
			}
		}
	};

NONSHARABLE_CLASS( CHarvesterEventObserverAO ) : public CActive
	{
public:
	// Cancel and destroy
	~CHarvesterEventObserverAO();

	// Two-phased constructor.
	static CHarvesterEventObserverAO* NewL( RHarvesterClient& aHC );

	// Two-phased constructor.
	static CHarvesterEventObserverAO* NewLC( RHarvesterClient& aHC );

public:
	// New functions
	// Function for making the initial request
	void AddHarvesterEventObserverL( 
			MHarvesterEventObserver& aHarvesterEventObserver, 
			TInt aHEObserverType,
    		TInt aEventInterval );
	
	void RemoveHarvesterEventObserverL( MHarvesterEventObserver& aHarvesterEventObserver );

private:	
	// C++ constructor
	CHarvesterEventObserverAO( RHarvesterClient& aHC );

	// Second-phase constructor
	void ConstructL();
	
	/**
	 * Get next free observer ID
	 * @return Observer ID
	 */
	TUint NextObserverId();

	/**
	 * Get handle to event queue. Creates event queue if it doesn't exist. 
	 * @return Event queue handle
	 */
	RHandleBase& GetEventQueueHandleL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);

private:	
	RPointerArray<THarvesterEventObserver> iObservers;
	THarvesterEventQueue iHarvesterEventQueue;
	RHarvesterClient& iHarvesterClient;
	};

#endif // HARVESTEREVENTOBSERVERAO_H
