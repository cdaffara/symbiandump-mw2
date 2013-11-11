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
* Description:  Ussd Handler Send.
*
*/


// INCLUDE FILES
#include "cphsrvussdsendhandler.h" 
#include "mphsrvussdnetworkobserver.h" 
#include "mphsrvphoneinterface.h" 
#include "cphsrvussdsessioncancelwaiter.h" 
#include "phsrvdebuginfo.h" //debug prints 

#include <gsmuelem.h>
#include <f32file.h>
#include <rmmcustomapi.h>

//CONSTANTS
const TInt KPhSrvUssdSenderPriority = CActive::EPriorityLow + 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::CPhSrvUssdSendHandler
//
// Constructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
CPhSrvUssdSendHandler::CPhSrvUssdSendHandler(
    MPhSrvUssdNetworkObserver& aObserver,
    RMobileUssdMessaging& aUssdMessaging,
    MPhSrvPhoneInterface& aPhoneInterface )
    :    CActive( KPhSrvUssdSenderPriority ),
         iObserver( aObserver ),
         iUssdMessaging( aUssdMessaging ),
         iPhoneInterface( aPhoneInterface )
    {
    CActiveScheduler::Add( this );

    // R&D SOLUTION ->
    iSessionCancelWaiter = new CPhSrvUssdSessionCancelWaiter;
    __ASSERT_ALWAYS( iSessionCancelWaiter, User::Invariant() );
    // <-
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::~CPhSrvUssdSendHandler
//
// Destructor
// -----------------------------------------------------------------------------
//
CPhSrvUssdSendHandler::~CPhSrvUssdSendHandler()
    {
    _DPRINT( 4, "PhSrv.Sendhandler.Destructor.Start" );           // debug print
    Cancel();

    // R&D solution ->
    if ( iSessionCancelWaiter )
        {
        if ( iSessionCancelWaiter->IsActive() )
            {
            _DPRINT( 4, "PhSrv.Sendhandler.Destructor.IsActive.SendRelease" );
            iUssdMessaging.CancelAsyncRequest( EMobileUssdMessagingSendRelease );
            }
        delete iSessionCancelWaiter;
        iSessionCancelWaiter = NULL;
        }
    // <-

    delete iMsgData;
    iMsgData = NULL;

    delete iMsgAttribute;
    iMsgAttribute = NULL;
     _DPRINT( 4, "PhSrv.Sendhandler.Destructor.End" );           // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::SendUssdL
//
// Send the USSD data.
// -----------------------------------------------------------------------------
//
void CPhSrvUssdSendHandler::SendUssdL(
            const TDesC8& aMsgData,
            const TDesC8& aMsgAttribute )
    {
    _DPRINT( 4, "PhSrv.Sendhandler.SendUssd.Start" );           // debug print
    // R&D SOLUTION ->
    
    // If cancel is going on, don't try to send anything.
    if ( IsActive() || iSessionCancelWaiter->IsActive() )
        {
        _DPRINT( 4, "PhSrv.Sendhandler.SendUssd.KErrInUse" );    // debug print
        User::Leave( KErrInUse );
        }
    // <-

    delete iMsgData;
    iMsgData = NULL;
    iMsgData = aMsgData.AllocL();

    delete iMsgAttribute;
    iMsgAttribute = NULL;
    iMsgAttribute = aMsgAttribute.AllocL();
    iUssdMessaging.SendMessage( iStatus , *iMsgData , *iMsgAttribute );
    SetActive();
    _DPRINT( 4, "PhSrv.Sendhandler.SendUssd.End" );           // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::SendUssdCancel
//
// Cancel USSD sending.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvUssdSendHandler::SendUssdCancel()
    {
    _DPRINT( 4, "PhSrv.Sendhandler.SendUssdCancel.Start" );         // debug print
    Cancel();
     _DPRINT( 4, "PhSrv.Sendhandler.SendUssdCancel.End" );           // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::DoCancel
//
// Cancel request.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvUssdSendHandler::DoCancel()
    {
    _DDPRINT( 4, "PhSrv.Sendhandler.DoCancel.Start", iStatus.Int() );           // debug print

    iUssdMessaging.CancelAsyncRequest( EMobileUssdMessagingSendMessage );
    delete iMsgData;
    iMsgData = NULL;
    delete iMsgAttribute;
    iMsgAttribute = NULL;
    _DDPRINT( 4, "PhSrv.Sendhandler.DoCancel.End", iStatus.Int() );           // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::RunL
//
// When iStatusRequest is changed.
// -----------------------------------------------------------------------------
//
void CPhSrvUssdSendHandler::RunL()
    {
    _DDPRINT( 4, "PhSrv.SendHandler.RunL.Start", iStatus.Int() );
    iObserver.UssdNetworkObserverHandleSendEventL( iStatus.Int() );
    _DPRINT( 4, "PhSrv.Sendhandler.RunL.End" );
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdSendHandler::SendReleaseSession
//
// Terminates Ussd session.
// -----------------------------------------------------------------------------
//
void CPhSrvUssdSendHandler::SendReleaseSession()
    {
    _DPRINT( 4, "PhSrv.Sendhandler.SendReleaseSession.Start" );         // debug print
    if ( !iSessionCancelWaiter->IsActive() )
        {
        _DPRINT( 4, "PhSrv.Sendhandler.SendReleaseSession" );           // debug print
        iUssdMessaging.SendRelease(
            iSessionCancelWaiter->iStatus,
            iSessionCancelWaiter->iSSRequestPckg );
        _DPRINT( 4, "PhSrv.Sendhandler.SendReleaseSession2" ); 
        iSessionCancelWaiter->SetActive();
        _DDPRINT( 4, "PhSrv.Sendhandler.SRS.iUssdMessaging", iStatus.Int() );
        }
    _DPRINT( 4, "PhSrv.Sendhandler.SendReleaseSession.End" );           // debug print
    }

//  End of File
