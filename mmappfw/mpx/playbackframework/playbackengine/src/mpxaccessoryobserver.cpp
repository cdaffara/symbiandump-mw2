/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Accessory observer
*
*/

#include <mpxlog.h>
#include "mpxaccessoryobserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::NewL
// Two phase constructor
// -----------------------------------------------------------------------------
//
CMPXAccessoryObserver* CMPXAccessoryObserver::NewL(
    MMPXAccessoryEventObserver& aObs)
    {
    CMPXAccessoryObserver* self = new ( ELeave ) CMPXAccessoryObserver(aObs);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMPXAccessoryObserver::ConstructL()
    {
    MPX_DEBUG1("CMPXAccessoryObserver::ConstructL()" );
#ifdef __ACCESSORY_FW
    User::LeaveIfError(iServer.Connect());
    User::LeaveIfError(iMode.CreateSubSession(iServer));
#endif // __ACCESSORY_FW
    StartL();
    }

// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::CMPXAccessoryObserver
// -----------------------------------------------------------------------------
//
#ifdef __ACCESSORY_FW
CMPXAccessoryObserver::CMPXAccessoryObserver(
    MMPXAccessoryEventObserver& aObs)
:   CActive( CActive::EPriorityStandard ),
    iObs(aObs)
    {
    CActiveScheduler::Add(this);
    }

#else
CMPXAccessoryObserver::CMPXAccessoryObserver(
    MMPXAccessoryEventObserver& aObs)
:   CDosEventListenerBase(),
    iObs(aObs)
    {
    }
#endif // __ACCESSORY_FW


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMPXAccessoryObserver::~CMPXAccessoryObserver()
    {
    MPX_DEBUG1("CMPXAccessoryObserver::~CMPXAccessoryObserver()");
#ifdef __ACCESSORY_FW
    Cancel();
    iMode.CloseSubSession();
    iServer.Disconnect();
#endif // __ACCESSORY_FW
    }

#ifndef __ACCESSORY_FW
// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::AccessoryModeChangedL
// Receives event on accessory mode change.
// -----------------------------------------------------------------------------
//
void CMPXAccessoryObserver::AccessoryModeChangedL(
    TDosAccessoryMode aAccessoryState )
    {
    MPX_FUNC("CMPXAccessoryObserver::AccessoryModeChangedL");

    TMPXPlaybackAccessoryMode mode( ConvertToMpxMode( aAccessoryState ));
    if ( EPbAccessoryModeUnknown != mode )
        { //inform parent
        iObs.HandleAccesoryEventL(mode);
        }
    }
#endif // __ACCESSORY_FW

// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::StartL
// -----------------------------------------------------------------------------
//
void CMPXAccessoryObserver::StartL()
    {
    MPX_DEBUG1("CMPXAccessoryObserver::StartL()");

#ifdef __ACCESSORY_FW
    if (!IsActive())
        {
        iMode.NotifyAccessoryModeChanged(iStatus, iAccessory_mode);
        SetActive();
        }
#else
    StartListeningL(KAccessoryModeChanged, sizeof (TDosAccessoryMode), EQueue );
#endif // __ACCESSORY_FW

    }
// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::AccessoryModeL
// Gets current accessory mode
// -----------------------------------------------------------------------------
//
TMPXPlaybackAccessoryMode CMPXAccessoryObserver::AccessoryModeL()
    {
    MPX_DEBUG1("CMPXAccessoryObserver::AccessoryModeL() entering");

#ifdef __ACCESSORY_FW
    RAccessoryServer     accessoryServer;
    User::LeaveIfError( accessoryServer.Connect () );
    CleanupClosePushL( accessoryServer );
    RAccessoryMode       accessory;
    accessory.CreateSubSession( accessoryServer );
    TAccPolAccessoryMode accMode;
    TInt err = accessory.GetAccessoryMode( accMode );
    accessory.CloseSubSession();
    CleanupStack::PopAndDestroy( &accessoryServer );
#else
    RDosServer dosServer;
    User::LeaveIfError( dosServer.Connect () );
    CleanupClosePushL( dosServer );
    RDosAccessory dosAccessory;
    User::LeaveIfError( dosAccessory.Open( dosServer ) );
    CleanupClosePushL( dosAccessory );
    TDosAccessoryMode accMode( EDosAccNotConnected );
    TInt err = dosAccessory.GetAccessoryMode( accMode );
    CleanupStack::PopAndDestroy( 2, dosServer ); // close dosAccessory and dosServer
#endif // __ACCESSORY_FW

    MPX_DEBUG1("CMPXAccessoryObserver::AccessoryModeL() exiting");
    return ConvertToMpxMode( accMode );
    }

// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::ConvertToMpxMode
// Converts from accessory mode to internal MPX mode enum
// -----------------------------------------------------------------------------
//
TMPXPlaybackAccessoryMode CMPXAccessoryObserver::ConvertToMpxMode(
#ifdef __ACCESSORY_FW
    TAccPolAccessoryMode& aMode )
#else
    TDosAccessoryMode& aMode )
#endif // __ACCESORY_FW
    {
    MPX_DEBUG1("CMPXAccessoryObserver::ConvertToMpxMode() entering");

    TMPXPlaybackAccessoryMode mode( EPbAccessoryModeUnknown );

#ifdef __ACCESSORY_FW
    MPX_DEBUG2("CMPXAccessoryObserver::ConvertToMpxMode(): aMode.iAccessoryMode = %d", aMode.iAccessoryMode);
    switch ( aMode.iAccessoryMode )
        {
        case EAccModeHandPortable:
            mode = EPbAccessoryHandPortable;
            break;
        case EAccModeWiredHeadset:
            mode = EPbAccessoryWiredHeadset;
            break;
        case EAccModeWirelessHeadset:
            mode = EPbAccessoryWirelessHeadset;
            break;
        case EAccModeWiredCarKit:
            mode = EPbAccessoryWiredCarKit;
            break;
        case EAccModeWirelessCarKit:
            mode = EPbAccessoryWirelessCarKit;
            break;
        case EAccModeTextDevice:
            mode = EPbAccessoryTextDevice;
            break;
        case EAccModeLoopset:
            mode = EPbAccessoryLoopset;
            break;
        case EAccModeMusicStand:
            mode = EPbAccessoryMusicStand;
            break;
        case EAccModeTVOut:
            mode = EPbAccessoryTVOut;
            break;
        case EAccModeHeadphones:
            mode = EPbAccessoryHeadphones;
            break;
        case EAccModeHDMI:
            mode = EPbAccessoryHDMI;
            break;
        default:
            break;
        }
#else
    MPX_DEBUG2("CMPXAccessoryObserver::ConvertToMpxMode(): aMode = %d", aMode);
    switch ( aMode )
        {
        case EDosAccNotConnected:
            mode = EPbAccessoryNone;
            break;
        case EDosAccUnsupportedConnected:
            mode = EPbAccessoryUnsupported;
            break;
        case EDosAccModeHeadset:
            mode = EPbAccessoryHeadset;
            break;
        case EDosAccModeLoopset:
            mode = EPbAccessoryLoopset;
            break;
        case EDosAccModeTty:
            mode = EPbAccessoryTty;
            break;
        case EDosAccModeCarKit:
            mode = EPbAccessoryCarKit;
            break;
        case EDosAccModeBtHeadset:
            mode = EPbAccessoryBtHeadset;
            break;
        case EDosAccModeBtCarKit:
            mode = EPbAccessoryBtCarKit;
            break;
        default:
            break;
        }
#endif // __ACCESORY_FW

    MPX_DEBUG2("CMPXAccessoryObserver::ConvertToMpxMode() exiting: %d", mode);
    return mode;
    }

#ifdef __ACCESSORY_FW
// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::RunL
// -----------------------------------------------------------------------------
//
void CMPXAccessoryObserver::RunL()
    {
    MPX_DEBUG2("CMPXAccessoryObserver::RunL() status %d ",iStatus.Int() );

    switch ( iStatus.Int() )
        {
        case KErrCancel:
            return;
        case KErrNone:
            {
            TMPXPlaybackAccessoryMode mode = ConvertToMpxMode( iAccessory_mode );
            MPX_DEBUG2("CMPXAccessoryObserver::AccessoryModeChangedL() mode %d",mode );

            //inform parent
            if ( EPbAccessoryModeUnknown != mode )
                {
                iObs.HandleAccesoryEventL(mode);
                }
            // Issue request again
            StartL();
            }
            break;
        case KErrNotSupported:
            break;
        default:
             StartL();
            break;
        }

    }

// -----------------------------------------------------------------------------
// CMPXAccessoryObserver::DoCancel
// -----------------------------------------------------------------------------
//
void CMPXAccessoryObserver::DoCancel()
    {
    MPX_FUNC("CMPXAccessoryObserver::DoCancel()");
    iMode.CancelNotifyAccessoryModeChanged();
    }
#endif // __ACCESSORY_FW

// End of file
