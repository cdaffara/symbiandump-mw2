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
* Description:  Notifies the changes in publish and suscripe properties.
*
*/



// INCLUDE FILES
#include    "CSatPSChangeNotifier.h"
#include    "MSatSystemStateObserver.h"
#include    "Panic.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::CSatPSChangeNotifier
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatPSChangeNotifier::CSatPSChangeNotifier(
    const TUid& aPropertyKey,
    TUint aPropertyValue,
    MSatSystemStateObserver& aObserver,
    TInt aNotifyValue ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iPSKey( aPropertyKey ),
    iPSValue( aPropertyValue ),
    iNotifyValue( aNotifyValue )
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::CSatPSChangeNotifier calling" )
    // Log parameters
    LOG2( NORMAL,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::CSatPSChangeNotifier PS Uid: %d",
        aPropertyKey )
    LOG2( NORMAL,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::CSatPSChangeNotifier PS Key: %d",
        aPropertyValue )
    LOG2( NORMAL,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::CSatPSChangeNotifier Needed \
        value: %d ", aNotifyValue )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::CSatPSChangeNotifier exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatPSChangeNotifier* CSatPSChangeNotifier::NewL(
    const TUid& aPropertyKey,
    TUint aPropertyValue,
    MSatSystemStateObserver& aObserver,
    TInt aNotifyValue )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::NewL calling" )

    CSatPSChangeNotifier* self =
        new( ELeave ) CSatPSChangeNotifier(
            aPropertyKey, aPropertyValue, aObserver, aNotifyValue );

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::NewL exiting" )
    return self;
    }


// Destructor
CSatPSChangeNotifier::~CSatPSChangeNotifier()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::~CSatPSChangeNotifier calling" )

    Cancel();
    iPSClient.Close();

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatPSChangeNotifier::~CSatPSChangeNotifier exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::NotifyChangeL
// Issues notification request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatPSChangeNotifier::NotifyChangeL()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::NotifyChangeL calling" )

    __ASSERT_ALWAYS( !IsActive(), Panic( ESatSystemStateAllReadyActive ) );
    User::LeaveIfError( iPSClient.Attach( iPSKey, iPSValue ) );
    Activate();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::NotifyChangeL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::CancelNotify
// Cancels outstanding request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatPSChangeNotifier::CancelNotify()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::CancelNotify calling" )

    Cancel();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::CancelNotify exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatPSChangeNotifier::DoCancel()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::DoCancel calling" )

    iPSClient.Cancel();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::RunL
// Activates the request again and calls the observer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatPSChangeNotifier::RunL()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::RunL calling" )
    // Makes sure that all the changes are notified.
    LOG2( SIMPLE, "SATSYSTEMSTATE:   Failed: %i", iStatus.Int() )
    if ( KErrNone == iStatus.Int() )
        {
        NotifyChangeL();

        // Check the value
        TInt value = KNotifyValueNotSet;

        // Get the value from P&S
        RProperty::Get( iPSKey, iPSValue, value );

        // If notify value is set, check the value before notifying observer
        if ( KNotifyValueNotSet != iNotifyValue )
            {
            LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::RunL \
            KNotifyValueNotSet != iNotifyValue" )
            // If changed P&S value is same as notify value, notify observer
            if ( value == iNotifyValue )
                {
                LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::RunL \
                value == iNotifyValue" )
                iObserver.StateChanged();
                }
            }
        else
            {
            LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::RunL \
            KNotifyValueNotSet == iNotifyValue" )
            // Notify value not specified -> Send the changed value
            iObserver.StateChanged( value );
            }
        }
    else if ( KErrCancel == iStatus.Int() )
        {
        Activate();
        }
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::Activate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatPSChangeNotifier::Activate()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::Activate calling" )

    iPSClient.Subscribe( iStatus );
    SetActive();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::Activate exiting" )
    }

// -----------------------------------------------------------------------------
// CSatPSChangeNotifier::GetValueInt
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatPSChangeNotifier::GetValueInt()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatPSChangeNotifier::GetValueInt calling" )

    // Gets the value from P&S.
    TInt value( KNotifyValueNotSet );
    RProperty::Get( iPSKey, iPSValue, value );

    LOG2( SIMPLE, 
        "SATSYSTEMSTATE: CSatPSChangeNotifier::GetValueInt exiting %i",
        value )
    return value;
    }
    
//  End of File
