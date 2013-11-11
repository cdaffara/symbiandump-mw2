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
* Description:  Capsulates the observer and events.
*
*/



// INCLUDE FILES
#include    "TSatEventsObserver.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"
#include    "SatLog.h"

// FORWARD DECLARE
class MSatEventObserver;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TSatEventsObserver::TSatEventsObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TSatEventsObserver::TSatEventsObserver(
    MSatEventObserver* aObserver ) // Observer of events.
    :
    iEvents(),
    iObserver( aObserver ),
    iMarkedForDelete( EFalse ) // This observer is not marked for delete, yet.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::TSatEventsObserver calling" )

    __ASSERT_ALWAYS( aObserver, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::TSatEventsObserver exiting" )
    }

// Destructor
TSatEventsObserver::~TSatEventsObserver()
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::~TSatEventsObserver calling" )

    iEvents.Reset();
    iEvents.Close();
    iObserver = NULL;

    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::~TSatEventsObserver exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::AddEventL
// Adds event to the list of events. If aEvent is already in the list, it will
// not be added to the list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventsObserver::AddEventL(
    TInt aEvent ) // Event to be added to list.
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::AddEventL calling" )

    // Check if the event is already added to the list.
    if ( !ObservingEvent( aEvent ) )
        {
        LOG( SIMPLE, "SATENGINE: TSatEventsObserver::AddEventL add event" )
        User::LeaveIfError( iEvents.Append( aEvent ) );
        }

    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::AddEventL exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::RemoveEvent
// Removes aEvent from the iEvents list, if aEvent is in the list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventsObserver::RemoveEvent( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::RemoveEvent calling" )

    const TInt index( iEvents.Find( aEvent ) );
    LOG2( SIMPLE, "SATENGINE: TSatEventsObserver::RemoveEvent index: %d", 
          index )

    // KErrNotFound in index means that aEvent is not in the list.
    if ( KErrNotFound != index )
        {
        iEvents.Remove( index );
        iEvents.GranularCompress();
        }

    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::RemoveEvent exiting" )
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::ObservingEvents
// Checks wether aEvent is in the iEvents list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TSatEventsObserver::ObservingEvent(
    TInt aEvent ) const // Event to be checked
    {
    LOG( DETAILED, "SATENGINE: TSatEventsObserver::ObservingEvent calling" )

    TBool result( ETrue );
    if ( KErrNotFound == iEvents.Find( aEvent ) )
        {
        LOG( DETAILED, "SATENGINE: TSatEventsObserver::ObservingEvent \
        find event" )
        result = EFalse;
        }

    LOG2( DETAILED, "SATENGINE: TSatEventsObserver::ObservingEvent exiting, \
          result: %d", result )
    return result;
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::Observer
// Getter for the observer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatEventObserver* TSatEventsObserver::Observer()
    {
    LOG( DETAILED, "SATENGINE: TSatEventsObserver::Observer calling-exiting" )
    return iObserver;
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::EventCount
// Getter for the event cound
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt TSatEventsObserver::EventCount() const
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::EventCount calling-exiting" )
    return iEvents.Count();
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::IsMarkedForDelete
// Getter for deletion mark
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TSatEventsObserver::IsMarkedForDelete() const
    {
    LOG( SIMPLE,
        "SATENGINE: TSatEventsObserver::IsMarkedForDelete calling-exiting" )
    return iMarkedForDelete;
    }

// -----------------------------------------------------------------------------
// TSatEventsObserver::MarkForDelete
// Marks this observer to be deleted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatEventsObserver::MarkForDelete()
    {
    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::MarkForDelete calling" )

    iMarkedForDelete = ETrue;

    LOG( SIMPLE, "SATENGINE: TSatEventsObserver::MarkForDelete exiting" )
    }

//  End of File
