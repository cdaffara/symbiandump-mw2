/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the handler for the SIM Application Toolkit to detect
*                the SIM card subscriber ID.
*
*/


#include    <e32base.h>
#include    <e32svr.h>
#include    "msatmultimodeapi.h"
#include    "MSatUtils.h"
#include    "CSatSSimSubscriberId.h"
#include    "TSatEventMediator.h"
#include    "csatsactivewrapper.h"
#include    "SatLog.h"

// MCC and MNC values can be found from numbering plans
const TInt KMinImsiLength( 5 );         // MNC + MNC consists of five numbers
const TInt KTimeBeforeRetry( 500000 );  // 0.5 seconds.
const TInt KMaxRetry( 20 );

// Special IMSI
_LIT( KCmccImsiIdentity1, "46000" );
_LIT( KCmccImsiIdentity2, "46002" );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::CSatSSimSubscriberId
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSSimSubscriberId::CSatSSimSubscriberId(
    MSatMultiModeApi& aPhone,
    TSatEventMediator& aEventMediator ) :
    CActive( EPriorityStandard ),
    iPhone( aPhone ),
    iEventMediator( aEventMediator ),
    iSubscriberIdValid( EFalse ),
    iRetryCounter( 0 )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSSimSubscriberId::CSatSSimSubscriberId calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATENGINE: CSatSSimSubscriberId::CSatSSimSubscriberId exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSSimSubscriberId::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::ConstructL calling" )

    // Used to suspends the current thread until a 0.5 seconds
    // interval has expired.
    iWrapper = new ( ELeave ) CSatSActiveWrapper();

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSSimSubscriberId* CSatSSimSubscriberId::NewL(
    MSatMultiModeApi& aPhone,
    TSatEventMediator& aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::NewL calling" )

    // Perform construction.
    CSatSSimSubscriberId* self = new ( ELeave ) CSatSSimSubscriberId(
        aPhone,
        aEventMediator );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::NewL exiting" )
    return self;
    }


// Destructor
CSatSSimSubscriberId::~CSatSSimSubscriberId()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSSimSubscriberId::~CSatSSimSubscriberId calling" )

    if ( iWrapper )
        {
        iWrapper->CancelWrapper();
        delete iWrapper;
        iWrapper = NULL;
        }

    // Cancel any outstanding requests.
    Cancel();

    LOG( SIMPLE,
        "SATENGINE: CSatSSimSubscriberId::~CSatSSimSubscriberId exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::Start
// Starts the handler initially.
// -----------------------------------------------------------------------------
//
void CSatSSimSubscriberId::Start()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::Start calling" )

    iRetryCounter = KMaxRetry;
    DoStart();

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::DoStart
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatSSimSubscriberId::DoStart()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::DoStart calling" )

    if ( !IsActive() )
        {
        iSubscriberIdValid = EFalse;
        iPhone.GetSubscriberId( iStatus, iSubscriberId );
        LOG( SIMPLE, "SATENGINE:   GetSubscriberId called" )
        SetActive();
        }

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::DoStart exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatSSimSubscriberId::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::RunL calling" )

    // Check the status of the asnychronous operation.
    TInt status( iStatus.Int() );
    if ( KErrNone == status )
        {
        // Indicate id successfully read.
        iSubscriberIdValid = ETrue;

        // Handle special SIM.
        if ( IsCmccSIM() )
            {
            LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::RunL Cmcc SIM" )
            // Activate SetUpMenuHandler for special SIM.
            iEventMediator.Notify( MSatUtils::ECmccSimDetected );
            }
        }
    else if ( ( KErrServerBusy == status ) ||
              ( KErrNotFound == status ) )
        {
        LOG2( SIMPLE,
            "SATENGINE: CSatSSimSubscriberId::RunL Server busy (%d)", status )

        if ( iRetryCounter > 0 )
            {
            LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::RunL \
            iRetryCounter > 0" )
            iRetryCounter--;

            // Restart subscriber ID reading after short period of time.
            if ( iWrapper )
                {
                LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::RunL \
                iWrapper true" )
                iWrapper->After( KTimeBeforeRetry );
                }

            DoStart();
            }
        }
    else
        {
        LOG2( SIMPLE,
            "SATENGINE: CSatSSimSubscriberId::RunL error: %d", status )
        }

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatSSimSubscriberId::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::DoCancel calling" )

    // Cancel an outstanding request.
    iPhone.CancelAsyncRequest( EMobilePhoneGetSubscriberId );

    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSimSubscriberId::IsCmccSIM
// Check if the used SIM card is special SIM
// -----------------------------------------------------------------------------
//
TBool CSatSSimSubscriberId::IsCmccSIM() const
    {
    LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::IsCmccSIM calling" )
    TBool isCmcc( EFalse );

    // Check validity.
    if ( iSubscriberIdValid )
        {
        if ( iSubscriberId.Length() >= KMinImsiLength )
            {
            LOG( SIMPLE, "SATENGINE: \
            CSatSSimSubscriberId::IsCmccSIM length valid" )
            isCmcc = (
                ( iSubscriberId.Left( KMinImsiLength ).
                    Compare( KCmccImsiIdentity1 ) == KErrNone ) ||
                ( iSubscriberId.Left( KMinImsiLength ).
                    Compare( KCmccImsiIdentity2 ) == KErrNone ) );
            }
        }

    else
        {
        LOG( SIMPLE, "SATENGINE: CSatSSimSubscriberId::IsCmccSIM Not valid" )
        }

    LOG2(
        SIMPLE,
        "SATENGINE: CSatSSimSubscriberId::IsCmccSIM exiting: %i",
        isCmcc )

    return isCmcc;
    }
