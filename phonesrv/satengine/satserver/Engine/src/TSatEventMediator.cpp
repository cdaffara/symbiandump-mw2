/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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



// INCLUDE FILES
#include    "TSatEventMediator.h"
#include    "TSatEventsObserver.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"
#include    "MSatEventObserver.h"
#include    "SatLog.h"

// FORWARD DECLARE
class MSatEventObserver;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TSatEventMediator::TSatEventMediator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TSatEventMediator::TSatEventMediator() :
    iEventsObservers(),
    iIsNotifying( EFalse ),
    iDeletionsMarked( EFalse )
    {
    LOG( SIMPLE,
        "SATENGINE: TSatEventMediator::TSatEventMediator calling - exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::Reset
// Replaces destructor since T classes cannot have one
// -----------------------------------------------------------------------------
//
void TSatEventMediator::Reset()
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Reset calling" )

    iEventsObservers.ResetAndDestroy();

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Reset exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::Notify
// All observers, who are registered to get the notification of aEvent, are
// notified
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventMediator::Notify(
    TInt aEvent ) // Event
    {
    LOG( DETAILED, "SATENGINE: TSatEventMediator::Notify calling" )

    // Go through the list of observers and call notify for observers who are
    // observing aEvent.

    // Set the flag on. No one can remove observers when this flag is on
    iIsNotifying = ETrue;

    TInt count( iEventsObservers.Count() );
    LOG2( DETAILED, "SATENGINE: TSatEventMediator::Notify observers \
          count: %d", count )
    for ( TInt i = 0; i < count; i++ )
        {
        TSatEventsObserver* eventsObserver =
            iEventsObservers[i];
        __ASSERT_ALWAYS( eventsObserver,
            PanicSatEngine( ESatEngineNullPointer ) );

        if ( eventsObserver->ObservingEvent( aEvent ) )
            {
            MSatEventObserver* observer =
                eventsObserver->Observer();

            if ( observer )
                {
                LOG( DETAILED, "TSatEventMediator::Notify observer found" )
                // Observer found, notify it
                observer->Event( aEvent );

                // Check is iEventObservers list changed. Some observers might
                // remove themselves and notify a new event (new Notify loop)
                // and marked observers are deleted there. When it comes back 
                // here iEventsObserver has changed           
                if ( count > iEventsObservers.Count() )
                    {
                    LOG( DETAILED, "TSatEventMediator::Notify \
                    count > iEventsObservers.Count()" )
                    TInt newCount( iEventsObservers.Count() );
                    i = i - ( count - newCount );
                    count = newCount;
                    }
                }
            else
                {
                LOG( DETAILED, "TSatEventMediator::Notify observer not found" )
                // Found NULL observer -> Mark fo deleted. This is deleted after
                // this notification loop
                eventsObserver->RemoveEvent( aEvent );
                eventsObserver->MarkForDelete();
                }
            }
        }

    // Set the flag off. Now observers can be removed
    iIsNotifying = EFalse;

    // Check was there any deletions during notify loop
    if ( iDeletionsMarked )
        {
        LOG( DETAILED, "TSatEventMediator::Notify iDeletionsMarked true" )
        // Delete marked observers
        DeleteMarkedObservers();
        iDeletionsMarked = EFalse;
        }

    LOG( DETAILED, "SATENGINE: TSatEventMediator::Notify exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::RegisterL
// Registers observer for getting the event when event is notified using
// Notify( TInt aEvent ).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventMediator::RegisterL(
    MSatEventObserver* aObserver, // Observer of aEvent.
    TInt aEvent ) // Event, which will be trigger of notification.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::RegisterL calling" )

    __ASSERT_ALWAYS( aObserver, PanicSatEngine( ESatEngineNullPointer ) );
    // Check if aObserver instance is already registered, so that we dont
    // create a new TSatEventsObserver entry in to the iEventsObserver list.
    TSatEventsObserver* eventsObserver = HasObserverInstance( aObserver );

    if ( NULL != eventsObserver )
        {
        LOG( SIMPLE, "SATENGINE: TSatEventMediator::RegisterL eventsObserver \
             not NULL" )
        // Add the event for the observer.
        eventsObserver->AddEventL( aEvent );
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: TSatEventMediator::RegisterL eventsObserver \
             NULL" )
        // Observer is registering for the first time.
        eventsObserver = new( ELeave )TSatEventsObserver( aObserver );
        CleanupStack::PushL( eventsObserver );
        eventsObserver->AddEventL( aEvent );
        User::LeaveIfError( iEventsObservers.Append( eventsObserver ) );
        CleanupStack::Pop( eventsObserver );
        }

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::RegisterL exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::Unregister
// Removes aEvent from aObservers observable events.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventMediator::Unregister(
    const MSatEventObserver* aObserver, // Observer of aEvent.
    TInt aEvent ) // Event, which the observer is observing.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister calling" )

    // Check if observer is registered.
    TSatEventsObserver* eventsObserver = HasObserverInstance( aObserver );
    if ( NULL != eventsObserver )
        {
        LOG( SIMPLE, "SATENGINE: TSatEventMediator::RegisterL eventsObserver \
             not NULL" )
        eventsObserver->RemoveEvent( aEvent );

        // Remove the eventsObserver from the list if the observer has
        // no longer any events to observe.
        if ( 0 == eventsObserver->EventCount() )
            {
            // Prevent delete if we are notifying observers
            if ( !iIsNotifying )
                {
                LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister \
                     iIsNotifying false" )
                const TInt index = iEventsObservers.Find( eventsObserver );
                // Does not destroy the object.
                iEventsObservers.Remove( index ); 
                iEventsObservers.GranularCompress();
                delete eventsObserver;
                }
            else
                {
                LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister \
                     iIsNotifying true" )
                // If we are notifying, mark this observer as deleted
                eventsObserver->MarkForDelete();
                iDeletionsMarked = ETrue;
                }
            }
        }

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::Unregister
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventMediator::Unregister(
    const MSatEventObserver* aObserver ) // Observer of aEvent.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister calling" )

    // Check if observer is registered.
    TSatEventsObserver* eventsObserver = HasObserverInstance( aObserver );
    if ( NULL != eventsObserver )
        {
        // Prevent delete if we are notifying observers
        if ( !iIsNotifying )
            {
            LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister \
                 iIsNotifying false" )
            const TInt index = iEventsObservers.Find( eventsObserver );
            iEventsObservers.Remove( index ); // Does not destroy the object.
            iEventsObservers.GranularCompress();
            delete eventsObserver;
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister \
                 iIsNotifying true" )
            // If we are notifying, mark this observer as deleted
            eventsObserver->MarkForDelete();
            iDeletionsMarked = ETrue;
            }
        }

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::Unregister exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::HasObserverInstance
// Checks if aObserver is already registered to this mediator and returns the
// result.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TSatEventsObserver* TSatEventMediator::HasObserverInstance(
    const MSatEventObserver* aObserver ) const // Observer looked from the list.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::HasObserverInstance calling" )

    // Go through the list until same observer instance is found or
    // all entries have been checked.
    TSatEventsObserver* foundEventsObserver = NULL;
    const TInt count( iEventsObservers.Count() );
    LOG2( NORMAL, "SATENGINE: TSatEventMediator::HasObserverInstance \
          observer count: %d", count )
    for ( TInt i = 0; ( i < count ) && ( !foundEventsObserver ); i++ )
        {
        TSatEventsObserver* eventsObserver = iEventsObservers[i];
        __ASSERT_ALWAYS( eventsObserver,
            PanicSatEngine( ESatEngineNullPointer ) );

        if ( aObserver == eventsObserver->Observer() )
            {
            LOG( SIMPLE, "SATENGINE: TSatEventMediator::HasObserverInstance \
            aObserver == eventsObserver->Observer()" )
            foundEventsObserver = eventsObserver;
            }
        }

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::HasObserverInstance exiting" )
    return foundEventsObserver;
    }

// -----------------------------------------------------------------------------
// TSatEventMediator::DeleteMarkedObservers
// Deletes all observers marked as deleted during Notify loop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventMediator::DeleteMarkedObservers()
    {
    LOG( SIMPLE, "SATENGINE: TSatEventMediator::DeleteMarkedObservers calling" )

    // Go through the list and delete all observers wich are
    // marked as deleted
    TInt count( iEventsObservers.Count() );
    LOG2( NORMAL, "SATENGINE: TSatEventMediator::DeleteMarkedObservers \
          observer count: %d", count )
    for ( TInt i = 0; i < count; i++ )
        {
        TSatEventsObserver* eventsObserver =
            iEventsObservers[i];
        if ( eventsObserver )
            {
            LOG( SIMPLE, "SATENGINE: TSatEventMediator::DeleteMarkedObservers\
            eventsObserver true" )
            // Check is obserer marked and it has no events
            if ( eventsObserver->IsMarkedForDelete() &&
                 ( 0 == eventsObserver->EventCount() ) )
                {
                LOG( SIMPLE, "SATENGINE: TSatEventMediator::\
                DeleteMarkedObservers delete Observer" )
                iEventsObservers.Remove( i ); // Does not destroy the object.
                iEventsObservers.GranularCompress();
                delete eventsObserver;

                // Update count value. We just removed an object from list
                count = iEventsObservers.Count();
                i--;
                }
            }
        }

    LOG( SIMPLE, "SATENGINE: TSatEventMediator::DeleteMarkedObservers exiting" )
    }

//  End of File
