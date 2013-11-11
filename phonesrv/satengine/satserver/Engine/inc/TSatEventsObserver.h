/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Capsulates observer and its events
*
*/



#ifndef TSATEVENTSOBSERVER_H
#define TSATEVENTSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MSatEventObserver;

// CLASS DECLARATION

/**
*  Capsulates observer and its events.
*
*  @lib SatEngine.dll
*  @since Series 60 3.0
*/
class TSatEventsObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        * @param aObserver Observer of events. Panic will occur if aObserver is
        * NULL.
        */
        TSatEventsObserver( MSatEventObserver* aObserver );

        /**
        * Destructor.
        */
        virtual ~TSatEventsObserver();

    public: // New functions

        /**
        * Adds event to be notified for the iObserver. Same event will
        * added more than once.
        * @param aEvent Source of notification for observer.
        */
        void AddEventL( TInt aEvent );

        /**
        * Removes the event from the observable events. If aEvent is not
        * observable event, nothing is done.
        * @param aEvent Source of notification for observer.
        */
        void RemoveEvent( TInt aEvent );

        /**
        * Checks if aEvent is already in the list of events.
        * @param aEvent Event to be checked.
        * @return ETrue if aEvent is in the list. Othervise EFalse.
        */
        TBool ObservingEvent( TInt aEvent ) const;

        /**
        * Gives the observer of the iEvents.
        * @return Observer of the iEvents
        */
        MSatEventObserver* Observer();

        /**
        * Gives the number of events that has been added.
        * @return Number of events.
        */
        TInt EventCount() const;

        /**
        * Marks this observer for delete
        */
        void MarkForDelete();

        /**
        * Indicates is this observer marked for delete
        * @return True if marked for delete.
        */
        TBool IsMarkedForDelete() const;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        TSatEventsObserver( const TSatEventsObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        TSatEventsObserver& operator=( const TSatEventsObserver& );

    private:    // Data

        // Events that iObserver is observing.
        RArray<TInt> iEvents;

        // Observer of events.
        MSatEventObserver* iObserver;

        // Indicates is this observer marked to be deleted.
        // If observer is requested to delete while looping
        // in Notify events, observer cannot be deleted, but when Notify
        // loop is over, all observers marked as delete are deleted
        TBool iMarkedForDelete;

    };

#endif      // TSATEVENTSOBSERVER_H

// End of File
