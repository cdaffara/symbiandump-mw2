/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Wrapper for usat api.
*
*/


#include    <satcs.h>
#include    "TUSatAPI.h"
#include    "SatLog.h"
#include    "msatmultimodeapi.h"
#include    "csatsactivewrapper.h"

const TInt KSatSLoopTimeout = 3000000;
const TInt KLoopMaxTryouts = 5; // Max tryouts for loops

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TUSatAPI::TUSatAPI
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TUSatAPI::TUSatAPI() :
    iSat(),
    iRSatConnected( EFalse ),
    iWrapper( NULL )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::TUSatAPI calling - exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::Connect
// Connects to USAT API.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt TUSatAPI::Connect( MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::Connect calling" )

    TInt numberOfTries( 1 );        // Indicates loop tryouts
    TBool loopSuccess( EFalse );    // Loop stopper, if this is ETrue
    TInt error( KErrNotSupported ); // Error code from iSat.Open()
    iRSatConnected = EFalse;

    // Create active wrapper
    iWrapper = new CSatSActiveWrapper();

    if ( iWrapper )
        {        
        while ( !loopSuccess && numberOfTries <= KLoopMaxTryouts )
            {
#ifndef SAT_USE_DUMMY_TSY
            LOG( NORMAL, "SATENGINE: TUSatAPI::Connect connect Phone" )
            error = iSat.Open( *( aPhone.Phone() ) );
#else
            LOG( NORMAL, "SATENGINE: TUSatAPI::Connect connect DummyPhone" )
            error = iSat.Open( *( aPhone.DummyPhone() ) );
#endif
            if ( KErrNone == error )
                {
                LOG( SIMPLE, "SATENGINE: TUSatAPI::Connect KErrNone == error" )
                loopSuccess = ETrue;
                iRSatConnected = ETrue;
                }
            else
                {
                numberOfTries++;
                iWrapper->After( KSatSLoopTimeout );
                }
            }
        LOG2( SIMPLE, "SATENGINE: TUSatAPI::Connect numberOfTries: %d", 
              numberOfTries )
        }

    LOG2( SIMPLE, "SATENGINE: TUSatAPI::Connect exiting with code: %i", error )
    return error;
    }

// -----------------------------------------------------------------------------
// TUSatAPI::Close
// Closes the connection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::Close()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::Close calling" )

    iSat.Close();
    iRSatConnected = EFalse;

    if ( iWrapper )
        {
        LOG( SIMPLE, "SATENGINE: TUSatAPI::Close iWrapper true" )
        iWrapper->CancelWrapper();
        delete iWrapper;
        iWrapper = NULL;
        }

    LOG( SIMPLE, "SATENGINE: TUSatAPI::Close exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpMenu
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpMenu(
    TRequestStatus& aStatus,
    TDes8& aPCmd )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenu calling" )

    iSat.NotifySetUpMenuPCmd( aStatus, aPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenu exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpMenuCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpMenuCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenuCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySetUpMenuPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenuCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLanguageNotification
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLanguageNotification(
    TRequestStatus& aStatus,
    TDes8& aPCmd )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLanguageNotification calling" )

    iSat.NotifyLanguageNotificationPCmd( aStatus, aPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLanguageNotification exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLanguageNotification
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLanguageNotificationCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenuCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyLanguageNotificationPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpMenuCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpCall(
    TRequestStatus& aStatus,
    TDes8& aPCmd )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpCall calling" )

    iSat.NotifySetUpCallPCmd( aStatus, aPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpCall exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpCallCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpCallCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpCallCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySetUpCallPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpCallCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySelectItem
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySelectItem(
    TRequestStatus& aStatus,
    TDes8& aPCmd )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySelectItem calling" )

    iSat.NotifySelectItemPCmd( aStatus, aPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySelectItem exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySelectItemCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySelectItemCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySelectItemCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySelectItemPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySelectItemCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendSm
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendSm(
    TRequestStatus& aStatus,
    TDes8& aPCmd )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSm calling" )

    iSat.NotifySendSmPCmd( aStatus, aPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSm exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendSmCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendSmCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSmCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySendSmPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSmCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::SendMessageNoLogging
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::SendMessageNoLogging(
    TRequestStatus& aStatus,
    TDes8& aData,
    TUint16& aRefMsg )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMessageNoLogging calling" )

    iSat.SendMessageNoLogging( aStatus, aData, aRefMsg );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMessageNoLogging exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::SendMessageNoLoggingCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::SendMessageNoLoggingCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMessageNoLoggingCancel calling" )

    iSat.CancelAsyncRequest( ESatSendMessageNoLogging );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMessageNoLoggingCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLaunchBrowser
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLaunchBrowser(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLaunchBrowser calling" )

    iSat.NotifyLaunchBrowserPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLaunchBrowser exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLaunchBrowserCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLaunchBrowserCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLaunchBrowserCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyLaunchBrowserPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLaunchBrowserCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyDisplayText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyDisplayText(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyDisplayText calling" )

    iSat.NotifyDisplayTextPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyDisplayText exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyDisplayTextCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyDisplayTextCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyDisplayTextCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyDisplayTextPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyDisplayTextCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetInput
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetInput(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInput calling" )

    iSat.NotifyGetInputPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInput exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetInputCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetInputCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInputCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyGetInputPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInputCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetInkey
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetInkey(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInkey calling" )

    iSat.NotifyGetInkeyPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInkey exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetInkeyCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetInkeyCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInkeyCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyGetInkeyPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetInkeyCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyCallControl
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyCallControl(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCallControl calling" )

    iSat.NotifyCallControlRequest( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCallControl exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyCallControlCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyCallControlCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCallControlCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyCallControlRequest );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCallControlCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyMoSmControl
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyMoSmControl(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyMoSmControl calling" )

    iSat.NotifyMoSmControlRequest( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyMoSmControl exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyMoSmControlCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyMoSmControlCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyMoSmControlCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyMoSmControlRequest );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyMoSmControlCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpEventList
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpEventList(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpEventList calling" )

    iSat.NotifySetUpEventListPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpEventList exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpEventListCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpEventListCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpEventListCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySetUpEventListPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpEventListCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpIdleModeText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpIdleModeText(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpIdleModeText calling" )

    iSat.NotifySetUpIdleModeTextPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpIdleModeText exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySetUpIdleModeTextCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySetUpIdleModeTextCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpIdleModeTextCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySetUpIdleModeTextPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySetUpIdleModeTextCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendDtmf
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendDtmf(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDtmf calling" )

    iSat.NotifySendDtmfPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDtmf exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendDtmfCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendDtmfCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDtmfCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySendDtmfPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDtmfCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendSs
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendSs(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSs calling" )

    iSat.NotifySendSsPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSs exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendSsCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendSsCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSsCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySendSsPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendSsCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendUssd
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendUssd(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendUssd calling" )

    iSat.NotifySendUssdPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendUssd exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendUssdCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendUssdCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendUssdCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySendUssdPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendUssdCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyPlayTone
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyPlayTone(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyPlayTone calling" )

    iSat.NotifyPlayTonePCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyPlayTone exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyPlayToneCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyPlayToneCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyPlayToneCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyPlayTonePCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyPlayToneCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyRefreshRequired
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyRefreshRequired(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshRequired calling" )

    iSat.NotifyRefreshRequired( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshRequired exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyRefreshRequiredCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyRefreshRequiredCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshRequiredCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyRefreshRequiredParam );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshRequiredCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::RefreshAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::RefreshAllowed(
    const TDesC8& aPCkg )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::RefreshAllowed calling" )

    if ( iWrapper )
        {
        LOG( SIMPLE, "SATENGINE: TUSatAPI::RefreshAllowed iWrapper true" )
        iSat.RefreshAllowed( iWrapper->RequestStatus(), aPCkg );
        iWrapper->SetActiveAndWait();
        }

    LOG( SIMPLE, "SATENGINE: TUSatAPI::RefreshAllowed exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyRefresh
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyRefresh(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefresh calling" )

    iSat.NotifyRefreshPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyRefreshCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyRefreshCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyRefreshPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyRefreshCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySimSessionEnd
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySimSessionEnd( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySimSessionEnd calling" )

    iSat.NotifyProactiveSimSessionEnd( aStatus );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySimSessionEnd exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySimSessionEndCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySimSessionEndCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySimSessionEndCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyProactiveSimSessionEnd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySimSessionEndCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLocalInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLocalInfo(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLocalInfo calling" )

    iSat.NotifyLocalInfoPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLocalInfo exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyLocalInfoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyLocalInfoCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLocalInfoCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyLocalInfoPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyLocalInfoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyOpenChannel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyOpenChannel(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyOpenChannel calling" )

    iSat.NotifyOpenChannelPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyOpenChannel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyOpenChannelCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyOpenChannelCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyOpenChannelCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyOpenChannelPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyOpenChannelCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyCloseChannel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyCloseChannel(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCloseChannel calling" )

    iSat.NotifyCloseChannelPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCloseChannel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyCloseChannelCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyCloseChannelCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCloseChannelCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyCloseChannelPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyCloseChannelCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetChannelStatus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetChannelStatus(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetChannelStatus calling" )

    iSat.NotifyGetChannelStatusPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetChannelStatus exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyGetChannelStatusCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyGetChannelStatusCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetChannelStatusCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyGetChannelStatusPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyGetChannelStatusCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendData(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendData calling" )

    iSat.NotifySendDataPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendData exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifySendDataCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifySendDataCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDataCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifySendDataPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifySendDataCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyReceiveData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyReceiveData(
    TRequestStatus& aStatus,
    TDes8& aData )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyReceiveData calling" )

    iSat.NotifyReceiveDataPCmd( aStatus, aData );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyReceiveData exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::NotifyReceiveDataCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::NotifyReceiveDataCancel()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyReceiveDataCancel calling" )

    iSat.CancelAsyncRequest( ESatNotifyReceiveDataPCmd );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::NotifyReceiveDataCancel exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::SendMenuSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::SendMenuSelection( const TDes8& aMenuSelection )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMenuSelection calling" )

    if ( iWrapper )
        {
        LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMenuSelection iWrapper true" )
        iSat.MenuSelection( iWrapper->RequestStatus(), aMenuSelection );
        iWrapper->SetActiveAndWait();
        }

    LOG( SIMPLE, "SATENGINE: TUSatAPI::SendMenuSelection exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatApi::TerminalRsp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::TerminalRsp(
    RSat::TPCmd aPCmd, // Identifies the command, which is sending the response
    const TDesC8& aRsp ) // Resposen data package
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::TerminalRsp calling" )

    if ( iWrapper )
        {
        LOG( SIMPLE, "SATENGINE: TUSatAPI::TerminalRsp iWrapper true" )
        iSat.TerminalRsp( iWrapper->RequestStatus(), aPCmd, aRsp );
        iWrapper->SetActiveAndWait();
        }

    LOG( SIMPLE, "SATENGINE: TUSatAPI::TerminalRsp exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::GetIcon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::GetIcon(
    TRequestStatus& aReqStatus,
    TUint8 aRecordNumber,
    RSat::TIcon& aIconEf )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetIcon calling" )

    iSat.GetIcon( aReqStatus, aRecordNumber, aIconEf );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetIcon exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::GetImageInstance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::GetImageInstance(
    TRequestStatus& aReqStatus,
    const TDesC8& aInstanceInfo,
    TDes8& aInstance )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetImageInstance calling" )

    iSat.GetImageInstance( aReqStatus, aInstanceInfo, aInstance );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetImageInstance exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::GetClut
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::GetClut(
    TRequestStatus& aReqStatus,
    TUint aInstanceNumber,
    TUint aOffset,
    TDes8& aClut )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetClut calling" )

    iSat.GetClut( aReqStatus, aInstanceNumber, aOffset, aClut );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::GetClut exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::EventDownload
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TUSatAPI::EventDownload(
    TRequestStatus& aReqStatus,
    RSat::TEventList aSingleEvent,
    const TDesC8& aEventInfo )
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::EventDownload calling" )

    iSat.EventDownload( aReqStatus, aSingleEvent, aEventInfo );

    LOG( SIMPLE, "SATENGINE: TUSatAPI::EventDownload exiting" )
    }

// -----------------------------------------------------------------------------
// TUSatAPI::IsRSatConnected
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TUSatAPI::IsRSatConnected() const
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::IsRSatConnected calling-exiting" )
    return iRSatConnected;
    }
// -----------------------------------------------------------------------------
// TUSatAPI::UsatClientReadyIndication
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt TUSatAPI::UsatClientReadyIndication()
    {
    LOG( SIMPLE, "SATENGINE: TUSatAPI::UsatClientReadyIndication calling-exit")

    return iSat.UsatClientReadyIndication();
    }
