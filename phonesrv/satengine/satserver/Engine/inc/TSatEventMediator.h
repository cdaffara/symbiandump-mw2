/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides way to notify events and register observers
*                for events
*
*/



#ifndef TSATEVENTMEDIATOR_H
#define TSATEVENTMEDIATOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MSatEventObserver;
class TSatEventsObserver;

// CLASS DECLARATION

/**
*  Provides way to notify events and register observers for events.
*  Observers are not owned by this class and therefore observers are
*  not deleted when TSatEventMediator instance is destroyed.
*
*  @lib SatEngine.dll
*  @since Series 60 3.0
*/
class TSatEventMediator
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TSatEventMediator();

    public: // New functions

        /**
        * Notifies possible observers who are observing the event.
        * @param aEvent Integer identifying the event.
        * @return None
        */
        void Notify( TInt aEvent );

        /**
        * Registers observer for event. Same observer will not be added
        * more than once for one event.
        * @param aObserver Pointer of observer instance. Panic will occur if
        * aObserver is NULL.
        * @param aEvent Event, which will cause the notification of observer.
        * @return None
        */
        void RegisterL( MSatEventObserver* aObserver, TInt aEvent );

        /**
        * Unregisters the notification of single event.
        * @param aObserver Observer, whom event registeration will be
        * unregistered.
        * @param aEvent Event, which will be unregistered.
        * @return None
        */
        void Unregister( const MSatEventObserver* aObserver, TInt aEvent );

        /**
        * Unregisters the event observer.
        * @param aObserver Observer, whom event registeration will be
        * unregistered.
        */
        void Unregister( const MSatEventObserver* aObserver );

        /**
        * Resets the list of event observers. Used instead of destructor
        */
        void Reset();

    private:  // New functions

        /**
        * Checks if aObserver is already in the list of observers.
        * @param aObserver Observer that is checked from the list.
        * @return Pointer of the observer, where aObserver is registered.
        * Contains NULL pointer if aObserver is not already registered.
        */
        TSatEventsObserver* HasObserverInstance(
            const MSatEventObserver* aObserver ) const;

        /**
        * Deletes all observers marked as deleted during notify loop
        */
        void DeleteMarkedObservers();

    private:

        // Prohibit copy constructor if not deriving from CBase.
        TSatEventMediator( const TSatEventMediator& );

        // Prohibit assigment operator if not deriving from CBase.
        TSatEventMediator& operator=( const TSatEventMediator& );

    private:    // Data

        // event observers.
        RPointerArray<TSatEventsObserver> iEventsObservers;

        // Indicates are we notifying observers. If this flag is on,
        // observers cannot be removed from iEventsObservers. After
        // notify loop is done, observers can be replaced again
        TBool iIsNotifying;

        // Indicates are there any observers marked as deleted during
        // Notify loop
        TBool iDeletionsMarked;
    };

#endif      // TSATEVENTMEDIATOR_H

// End of File
