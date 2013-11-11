/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef HARVESTEREVENTMANAGER_H
#define HARVESTEREVENTMANAGER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32msgqueue.h>

#include "harvestercommon.h"

NONSHARABLE_CLASS( CHarvesterEventManager ) : public CBase
	{
public:
	virtual ~CHarvesterEventManager();
	
	IMPORT_C static CHarvesterEventManager* GetInstanceL();	
	IMPORT_C void ReleaseInstance();

	/*
	 * Increase event's item count
	 * @param aHEObserverType Event type
	 * @param aCount Increase to event's items
	 */
	IMPORT_C void IncreaseItemCount( HarvesterEventObserverType aHEObserverType, TUint aCount = 1 );

	/*
	 * Decrease event's item count
	 * @param aHEObserverType Event type
	 * @param aCount Decrease to event's items
	 */
	IMPORT_C TBool DecreaseItemCountL( HarvesterEventObserverType aHEObserverType, TUint aCount = 1 );
	
	/*
	 * Get event's item count
	 * @param aHEObserverType Event type
	 * @return Event's item count
	 */
	IMPORT_C TUint ItemCount( HarvesterEventObserverType aHEObserverType );

	/**
	 * Send event to all matching observers.
	 * @param aHEObserverType Event type
	 * @param aHEState Event state
	 * @param aItemsLeft Items left
	 * @return Event sending error code
	 */
	IMPORT_C TInt SendEventL( HarvesterEventObserverType aHEObserverType, 
			HarvesterEventState aHEState, TUint aItemsLeft = 0 );

	/*
	 * Register event observer. Leaves if registering fails.
	 * @param aMessage Event observer's client side information
	 */
	IMPORT_C void RegisterEventObserverL( const RMessage2& aMessage );

	/*
	 * Unregister event observer. Returns error code if unregistering fails.
	 * @param aMessage Event observer's client side information
	 */
	IMPORT_C TInt UnregisterEventObserver( const RMessage2& aMessage );
	
	/**
	 * Get current state of event by event type
	 * @param Event type
	 * @return Currect state of event
	 */
	IMPORT_C HarvesterEventState CurrentState( HarvesterEventObserverType aHEObserverType );

	/**
	 * @deprecated Client generates own "Harvester Event Observer IDs"
	 */
	IMPORT_C TUint GetLastClientId();

private:
	/**
	 * Server side event status.
	 */
	struct TEventStatus
		{
		HarvesterEventObserverType iObserverType;
		HarvesterEventState iCurrentState;
		TUint iItemsLeft;
		};
	
	// C++ constructor
	CHarvesterEventManager();

	// Second-phase constructor
	void ConstructL();

	/**
	 * Check if observer type contain wanted event type.
	 * @param aObserverType Observer type
	 * @param aEventType Event type
	 * @return Does observer type contain wanted event type
	 */
	TBool CheckObserverType( TInt aObserverType, TInt aEventType );
	
	/**
	 * Get wanted event by event type
	 * @param aHEObserverType Wanted event type
	 * @return Requested event or NULL
	 */
	TEventStatus* GetEventStatus( HarvesterEventObserverType aHEObserverType );
	
	/**
	 * Send event to single observer.
	 * @param aEventObserverInfo Observer to receive event
	 * @param aHEObserverType Event type
	 * @param aHEState Event state
	 * @param aItemsLeft Items left
	 * @return Event sending error code
	 */
	TInt SendSingleEvent( THarvesterEventObserverInfo& aEventObserverInfo, 
			HarvesterEventObserverType aObserverType, 
			HarvesterEventState aEventState, TUint aItemsLeft );

	static TInt CompareProperties(const THarvesterEventQueue& aFirst, const THarvesterEventQueue& aSecond);
	
private:

	RArray<TEventStatus> iEventStatuses;
	RPointerArray<THarvesterEventQueue> iEventQueues;
	RPointerArray<THarvesterEventObserverInfo> iRegisteredObservers;
	};

/**
* A helper class to store this singleton's static data.
*/
class CHarvesterEventManagerStaticData : public CBase
    {
    friend class CHarvesterEventManager;
        
    public:

    	CHarvesterEventManagerStaticData( CHarvesterEventManager* aHEM ) : iHEM(aHEM), iRefCount(1)
            {
            }

        virtual ~CHarvesterEventManagerStaticData()
            {
            delete iHEM;
            iHEM = NULL;
            }

    protected:

    	CHarvesterEventManager* iHEM;
        TInt iRefCount;
    };

#endif // HARVESTEREVENTMANAGER_H
