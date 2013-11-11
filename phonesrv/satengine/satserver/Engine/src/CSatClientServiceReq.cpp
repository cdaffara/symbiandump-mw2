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
* Description:  Handles client requests.
*
*/



// INCLUDE FILES
#include    "CSatClientServiceReq.h"
#include    "MSatUtils.h"
#include    "MSatSUiClientHandler.h"
#include    "EnginePanic.h"
#include    "SatLog.h"

_LIT( KTxtServer, "SatServer" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatClientServiceReq::CSatClientServiceReq
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatClientServiceReq::CSatClientServiceReq(
    TSatServerRequest aRequest,
    TSatServerRequest aResponse,
    MSatCommand* aCommand,
    MSatUtils& aUtils ) :
    iHandledRequest( aRequest ),
    iHandledResponse( aResponse ),
    iResponseObserver( aCommand ),
    iUtils( aUtils )
    {
    LOG( SIMPLE, "SATENGINE: \
        CSatClientServiceReq::CSatClientServiceReq calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatClientServiceReq* CSatClientServiceReq::NewL(
    TSatServerRequest aRequest,
    TSatServerRequest aResponse,
    MSatCommand* aCommand,
    MSatUtils& aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::NewL calling-exiting" )
    return new ( ELeave ) CSatClientServiceReq(
        aRequest, aResponse, aCommand, aUtils );
    }


// Destructor
CSatClientServiceReq::~CSatClientServiceReq()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatClientServiceReq::~CSatClientServiceReq calling" )

    iResponseObserver = NULL;
    iCmdData = 0;
    iCmdRsp = 0;

    LOG( SIMPLE,
        "SATENGINE: CSatClientServiceReq::~CSatClientServiceReq exiting" )
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::HandleCommand
// Handles the SIM command. If client has not yet requested service for this
// SIM command, command is stored for later use.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatClientServiceReq::HandleCommand(
    TDesC8* aCmdData, // Data to be sent to client.
    TDes8* aCmdRsp, // Client is response is written to this.h
    TSatServerRequest aRequest ) // Command from SIM
    {
    LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleCommand calling" )

    __ASSERT_ALWAYS( iResponseObserver,
        PanicSatEngine( ESatEngineNullPointer ) );

    TBool handled( EFalse );
    // Handle the command here if the request op codes match.
    if ( iHandledRequest == aRequest )
        {
        LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleCommand \
             request op codes match" )
        // Store the descriptors
        iCmdData = aCmdData;
        iCmdRsp = aCmdRsp;

        handled = ETrue;

        // Is the client already waiting for the command
        if ( iRequestPending )
            {
            LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleCommand \
                 RequestPending" )
            // Request is served.
            iRequestPending = EFalse;
            SendDataToClient();
            }
        else
            {
            LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleCommand \
                 no RequestPending" )
            // Client request was not available, so command is waiting for
            // client request.
            iCmdPending = ETrue;
            }
        }

    LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleCommand exiting" )
    return handled;
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::HandleRequest
// Handles the client's request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatClientServiceReq::HandleRequest(
    const RMessage2& aRequest ) // Request from client.
    {
    LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest calling" )

    TBool handled( EFalse );

    // Is the request handled here.
    const TInt request( aRequest.Function() );
    if ( request == iHandledRequest )
        {
        LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest \
             handle request" )
        handled = ETrue;
        iRequest = aRequest;

        // Is the command waiting for client.
        if ( iCmdPending )
            {
            LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest \
            iCmdPending" )
            iCmdPending = EFalse;
            SendDataToClient();
            }
        else
            {
            // Command has not yet arrived from SIM, so
            // wait for the command.
            iRequestPending = ETrue;
            }
        }
    // Is the response handled here.
    else if ( request == iHandledResponse )
        {
        LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest \
             handle response" )
        handled = ETrue;

        TRAPD( res, aRequest.ReadL( 0, *iCmdRsp ) );

        // Send the response further only if IPC succeeded.
        if ( KErrNone != res )
            {
            LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest \
            KErrNone != res" )
            iRequest.Panic( KTxtServer, ESatSBadDescriptor );
            }

        // Release client first, then handle the response
        aRequest.Complete( KErrNone );
        // Notify command that response is available.
        iResponseObserver->ClientResponse();
        }
    else
        {
        }

    LOG( DETAILED, "SATENGINE: CSatClientServiceReq::HandleRequest exiting" )
    return handled;
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::SendDataToClient
// Writes the data to client address space and completes the iRequest.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatClientServiceReq::SendDataToClient()
    {
    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::SendDataToClient calling" )

    TRAPD( res,iRequest.WriteL( 0, *iCmdData ) );
    LOG2( SIMPLE, "SATENGINE: CSatClientServiceReq::SendDataToClient res: %d",
          res )
    if ( KErrNone != res )
        {
        iRequest.Panic( KTxtServer, ESatSBadDescriptor );
        }

    iRequest.Complete( KErrNone );

    // If UI is not launched by user, make sure UI is in foreground
    if ( !iUtils.SatUiHandler().UiLaunchedByUser() )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatClientServiceReq::SendDataToClient \
        EBringSatUiToForeGround" )
        iUtils.NotifyEvent( MSatUtils::EBringSatUiToForeGround );
        }

    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::SendDataToClient exiting" )
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::IsMyRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatClientServiceReq::IsMyRequest( const TSatServerRequest aRequest )
    {
    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::IsMyRequest calling" )
    TBool myRequest( EFalse );

    if ( iHandledRequest == aRequest )
        {
        LOG( SIMPLE,
            "SATENGINE: CSatClientServiceReq::IsMyRequest" )

        myRequest = ETrue;
        }
    LOG2( SIMPLE, "SATENGINE: CSatClientServiceReq::IsMyRequest exiting, \
          return: %d", myRequest )
    return myRequest;
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::SetCommandHandler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatClientServiceReq::SetCommandHandler( MSatCommand* aCommand )
    {
    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::SetCommandHandler calling" )

    iResponseObserver = aCommand;

    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::SetCommandHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatClientServiceReq::Reset
// Resets member data
// -----------------------------------------------------------------------------
//
void CSatClientServiceReq::Reset()
    {
    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::Reset calling" )

    // Check is request pending
    if ( !iRequestPending )
        {
        // Request not pending meaning that command is executing.
        // Send client response to prevent SIM to lock
        LOG( NORMAL, " Request is not pending" )
        // Notify command that response is available.
        iResponseObserver->ClientResponse();
        }

    // Clean data
    iCmdData = NULL;
    iCmdRsp = NULL;
    iCmdPending = EFalse;
    iRequestPending = EFalse;
    // We don't reset iResponseObserver

    LOG( SIMPLE, "SATENGINE: CSatClientServiceReq::Reset exiting" )
    }

//  End of File
