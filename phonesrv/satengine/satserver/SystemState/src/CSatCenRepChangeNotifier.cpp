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
* Description:  Notifies the changes in central repository values.
*
*/



// INCLUDE FILES
#include    <centralrepository.h>
#include    "CSatCenRepChangeNotifier.h"
#include    "MSatSystemStateObserver.h"
#include    "Panic.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::CSatCenRepChangeNotifier
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCenRepChangeNotifier::CSatCenRepChangeNotifier(
    const TUid& aCenRepCategory,
    TUint32 aCenRepId,
    MSatSystemStateObserver& aObserver,
    TInt aCenRepType ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iCenRepCategory( aCenRepCategory ),
    iCenRepId( aCenRepId ),
    iCenRepType( aCenRepType )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        CSatCenRepChangeNotifier calling" )
    // Log parameters
    LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        CSatCenRepChangeNotifier Category: %d", aCenRepCategory )
    LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        CSatCenRepChangeNotifier ID: %d", aCenRepId )
    LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        CSatCenRepChangeNotifier Type: %d ", aCenRepType )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        CSatCenRepChangeNotifier exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCenRepChangeNotifier* CSatCenRepChangeNotifier::NewL(
    const TUid& aCenRepCategory,
    TUint32 aCenRepId,
    MSatSystemStateObserver& aObserver,
    TInt aCenRepType )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::NewL calling" )

    CSatCenRepChangeNotifier* self =
        new( ELeave ) CSatCenRepChangeNotifier(
            aCenRepCategory, aCenRepId, aObserver, aCenRepType );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::NewL exiting" )
    return self;
    }


// Destructor
CSatCenRepChangeNotifier::~CSatCenRepChangeNotifier()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        ~CSatCenRepChangeNotifier calling" )

    Cancel();
    if ( iCenRepClient )
        {
        delete iCenRepClient;
        iCenRepClient = NULL;
        }

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::\
        ~CSatCenRepChangeNotifier exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::ConstructL
// Creates central repository object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::ConstructL()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::ConstructL calling" )

    iCenRepClient = CRepository::NewL( iCenRepCategory );

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::NotifyChangeL
// Issues notification request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::NotifyChangeL()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::NotifyChangeL calling" )

    __ASSERT_ALWAYS( !IsActive(), Panic( ESatSystemStateAllReadyActive ) );
    ActivateL();

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::NotifyChangeL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::CancelNotify
// Cancels outstanding request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::CancelNotify()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::CancelNotify calling" )

    Cancel();

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::CancelNotify exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::DoCancel()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::DoCancel calling" )

    // Cancel all notifications. This causes notification mechanism to return
    // KInvalidNotificationId in the RunL function, so it must be handled there.
    iCenRepClient->NotifyCancelAll();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::RunL
// Activates the request again and calls the observer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::RunL()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL calling" )

    // In CenRep CRepository returns the key for the changed value in iStatus.
    const TUint32 cenRepId( iStatus.Int() );
    LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
    iStatus: %d", iStatus.Int() )
    // Makes sure that all the changes are notified.
    if ( KErrCancel != iStatus.Int() )
        {
        ActivateL();
        }
        
    // Check that notification came from that ID we are observing.
    if ( cenRepId == iCenRepId )
        {
        LOG( SIMPLE, 
        "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL cenRepId == iCenRepId" )
        switch ( iCenRepType )
            {
            case ECRTypeInteger:
                {
                TInt value;
                iCenRepClient->Get( iCenRepId, value );
                LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
                    Notifying TInt value: %d", value )
                iObserver.StateChanged( value );
                break;
                }
            case ECRTypeReal:
                {
                TReal value;
                iCenRepClient->Get( iCenRepId, value );
                LOG2( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
                    Notifying TReal value: %d", value )
                iObserver.StateChanged( value );
                break;
                }
            case ECRTypeDesC8:
                {
                TBuf8<NCentralRepositoryConstants::KMaxBinaryLength> value;
                iCenRepClient->Get( iCenRepId, value );
                LOG( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
                    Notifying string value" )
                iObserver.StateChanged( value );
                break;
                }
            case ECRTypeDesC16:
                {
                TBuf16<NCentralRepositoryConstants::KMaxUnicodeStringLength> value;
                iCenRepClient->Get( iCenRepId, value );
                LOG( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
                    Notifying Unicode value" )
                iObserver.StateChanged( value );
                break;
                }
            case ECRTypeUnknown:
            default:
                {
                LOG( NORMAL, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL \
                    Notifying without value" )
                // If type is not defined, call StateChanged without parameters
                iObserver.StateChanged();
                break;
                }
            }
        }

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCenRepChangeNotifier::ActivateL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCenRepChangeNotifier::ActivateL()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::ActivateL calling" )

    // This request notifies all changes in this category, not just the one we
    // are observing. Thus we have to read the value from CenRep before
    // notification.
    User::LeaveIfError( iCenRepClient->NotifyRequest( iCenRepId, iStatus ) );
    SetActive();

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatCenRepChangeNotifier::ActivateL exiting" )
    }

//  End of File
