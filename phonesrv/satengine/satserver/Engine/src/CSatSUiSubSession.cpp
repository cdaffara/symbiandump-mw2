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
* Description:  Ui sub session.
*
*/



// INCLUDE FILES
#include    <e32property.h>
#include    <satdomainpskeys.h>

#include    "CSatSUiSubSession.h"
#include    "CSatSSession.h"
#include    "CSatSServer.h"
#include    "CSatCommandHandler.h"
#include    "CSatCommandContainer.h"
#include    "MSatApi.h"
#include    "EnginePanic.h"
#include    "SatLog.h"
#include    "SATInternalPSKeys.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatSUiSubSession::CSatSUiSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSatSUiSubSession::CSatSUiSubSession(
    CSatSSession& aSession,
    MSatApi& aSatApi,
    TSatEventMediator& aEventMediator ) // Session
    :
    CSatSSubSession( aSession ),
    iEventData(),
    iEventDataPckg( iEventData ),
    iSatApi( aSatApi ),
    iEventMediator( aEventMediator )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::CSatSUiSubSession calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::ConstructL calling" )

    iEventMediator.RegisterL( this, MSatUtils::ESmsSent );

    // Get registered service requests from MSatUtils.
    iServiceRequest = iSession.SatServer()->SatUtils()->ServiceRequests();

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSUiSubSession* CSatSUiSubSession::NewL(
    CSatSSession& aSession,
    MSatApi& aSatApi,
    TSatEventMediator& aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::NewL calling" )

    CSatSUiSubSession* self =
        new( ELeave ) CSatSUiSubSession( aSession, aSatApi, aEventMediator );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::NewL exiting" )
    return self;
    }

// Destructor
CSatSUiSubSession::~CSatSUiSubSession()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::~CSatSUiSubSession calling" )

    iServiceRequest = NULL;

    // Unregister to listen events
    iEventMediator.Unregister( this );

    // Inform session that UI Session is closed.
    // Session tells to container of iServiceRequest to reset
    // request handlers.
    iSession.UiSessionClosed();

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::~CSatSUiSubSession exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::HandleCommandL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::HandleCommandL(
    const RMessage2& aMessage ) // Message from ui client.
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL calling" )
    LOG2( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
          command: %i", aMessage.Function() )
    switch ( aMessage.Function() )
        {
        // Ui client is ready to handle setupmenu command.
        case ESatSProactiveSetUpMenu:
            {
            // Notifies that SetUpMenu command has been made because SAT Client
            // side application is returning to the main menu after SelectItem.
            // For example: Next action is LaunchBrowser command and after
            // Browser is launched then SAT Client side application has been
            // opened top of Browser. In some cases SAT client side application
            // stays topmost.
            iEventMediator.Notify( MSatUtils::ESetUpMenuRequested );
            // This service request is handled also like commands listed below.
            }
            //lint -fallthrough intended here
        case ESatSProactiveDisplayText:
        case ESatSProactiveGetInkey:
        case ESatSProactiveGetInput:
        case ESatSProactiveSelectItem:
        case ESatSProactivePlayTone:
        case ESatSProactiveRefresh:
        case ESatSProactiveDisplayTextResponse:
        case ESatSProactiveGetInkeyResponse:
        case ESatSProactiveGetInputResponse:
        case ESatSProactiveSelectItemResponse:
        case ESatSProactivePlayToneResponse:
        case ESatSProactiveQuery:
        case ESatSProactiveQueryResponse:
        case ESatSProactiveNotification:
        case ESatSProactiveNotificationResponse:
            {
            const TInt count( iServiceRequest->Count() );
            TBool requestHandled( EFalse );
            TInt index( 0 );
            // Loop until request is handled or no request handler found.
            while ( !requestHandled && index < count )
                {
                requestHandled = iServiceRequest->At( index )->
                    HandleRequest( aMessage );
                index++;
                }
            LOG2( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                  index: %i", index )
            break;
            }

        // Menu selection envelope.
        case ESatSActiveMenuSelection:
            {
            LOG( SIMPLE, "SATENGINE: CSatSUiSubSession: MenuSelection" )
            // Declare the IPC data structures.
            RSat::TMenuSelectionV1 menuSelection;
            RSat::TMenuSelectionV1Pckg menuSelectionPckg( menuSelection );

            // Perform the IPC transfer of response data.
            TRAPD( res, aMessage.ReadL( 0, menuSelectionPckg ) );

            // Send the response further only if IPC succeeded.
            if ( KErrNone != res )
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession: KErrNone != res" )
                iSession.PanicClient( aMessage, ESatSBadDescriptor );
                }
            iSatApi.SendMenuSelection( menuSelectionPckg );

            // Complete the client request.
            aMessage.Complete( KErrNone );
            LOG( SIMPLE,
                "SATENGINE: CSatSUiSubSession::HandleCommandL exiting" )
            return;
            }

        case ESatSProactiveEvent:
            {
            LOG( SIMPLE, "SATENGINE: CSatSUiSubSession: Event" )
            iEventRequest = aMessage;
            iEventRequestAvailable = ETrue;
            if ( iCloseUiRequested )
                {
                LOG( SIMPLE,
                    "SATENGINE: CSatSUiSubSession::HandleCommandL closeui" )
                CloseUiSession();
                }
            else if ( iSmsSent )
                {
                LOG( SIMPLE,
                    "SATENGINE: CSatSUiSubSession::HandleCommandL smssent" )
                SendSmsSentEventToClient();
                }
            else if ( iClearScreenRequested )
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                     clearscreen" )
                ClearScreen();
                }
            else if ( iUiEventRequested )
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                     UiEventRequested" )
                HandleUiEventNotification();
                }
            else
                {
                LOG( SIMPLE, "SATENGINE:   Event request available" )
                }

            break;
            }

        case ESatSProactiveEventResponse:
            {
            LOG( SIMPLE, "SATENGINE: CSatSUiSubSession: EventResponse" )
            // Declare the IPC data structures.
            TSatEventV1 eventRsp;
            //lint -e{603} eventRsp read through eventRspPckg in ReadL.
            TSatEventV1Pckg eventRspPckg( eventRsp );

            // Perform the IPC transfer of response data.
            TRAPD( res, aMessage.ReadL( 0, eventRspPckg ) );
            if ( KErrNone != res )
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                     read response data error" )
                iSession.PanicClient( aMessage, ESatSBadDescriptor );
                }
            else
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                     no error" )
                aMessage.Complete( KErrNone );
                }
            break;
            }

        case ESatSSessionTerminated:
            {
            LOG( SIMPLE, "SATENGINE: CSatSUiSubSession: SessionTerminated" )
            TSatTermination temp;
            //lint -e{603} temp read through tempPckg in ReadL.
            TSatTerminationPckg tempPckg ( temp );
            TRAPD( res, aMessage.ReadL( 0, tempPckg ) );
            if ( KErrNone != res )
                {
                LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                     read tempPckg error" )
                iSession.PanicClient( aMessage, ESatSBadDescriptor );
                }
            else
                {
                LOG2( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL \
                      temp.type: %i", temp.type )
                switch ( temp.type )
                    {
                    case ETerminatedByCancel:
                        {
                        iEventMediator.Notify(
                            MSatUtils::ECommandCancelled );
                        break;
                        }
                    case ETerminatedByEndKey:
                        {
                        iEventMediator.Notify(
                            MSatUtils::ECancelledUsingEndKey );

                        PublishSatAppClosedUsingEndKey();
                            
                        // Inform session that UI Session is closed.
                        // Session is not actually closed yet but this prevents
                        // sending data through this session.
                        // Notify thread death monitor
                        iSession.NotifyThreadDeathMonitor();
                        break;
                        }
                    default:
                        {
                        LOG( SIMPLE, "SATENGINE:   Unknown termination type" )
                        }
                    }
                }
            // Complete the client request.
            aMessage.Complete( KErrNone );
            break;
            }

        default:
            {
            LOG( SIMPLE, "  Unhandled command")
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::HandleCommandL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::CloseUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::CloseUiSession()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::CloseUiSession calling" )

    // Set request flag to ETrue. Updated when command is successfully sent.
    iCloseUiRequested = ETrue;

    // Notify thread death monitor
    iSession.NotifyThreadDeathMonitor();

    iEventData.iEvent = ESatSCloseSatUiAppEvent;

    const TBool commandSent( SendCommandDataToClient( iEventRequestAvailable,
                             iEventDataPckg,
                             iEventRequest ) );

    if ( commandSent )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSUiSubSession::CloseUiSession commandSent" )
        // If command was sent, set flag to false, so when next Event request
        // arrives, command data is not sent.
        iCloseUiRequested = EFalse;
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::CloseUiSession exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::ClearScreen
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::ClearScreen()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::ClearScreen calling" )

    // Set request flag to ETrue. Updated when command is successfully sent.
    iClearScreenRequested = ETrue;

    iEventData.iEvent = ESatSClearScreenEvent;

    const TBool commandSent( SendCommandDataToClient( iEventRequestAvailable,
                             iEventDataPckg,
                             iEventRequest ) );

    if ( commandSent )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSUiSubSession::ClearScreen commandSent" )
        // If command was sent, set flag to false, so when next Event request
        // arrives, command data is not sent.
        iClearScreenRequested = EFalse;
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::ClearScreen exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::UiEventNotification
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::UiEventNotification(
    TInt aEventId, TInt aEventStatus, TInt aEventError )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::UiEventNotification calling" )

    // Set request flag to ETrue. Updated when command is successfully sent.
    iUiEventRequested = ETrue;
    iEventData.iEvent = static_cast< TSatSEvent >( aEventId );
    iEventData.iStatus = static_cast< TSatSEventStatus >( aEventStatus );
    iEventData.iError = aEventError;

    HandleUiEventNotification();

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::UiEventNotification exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::HandleUiEventNotification
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::HandleUiEventNotification()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::HandleUiEventNotification calling" )

    // Send Command to client
    const TBool commandSent( SendCommandDataToClient( iEventRequestAvailable,
                             iEventDataPckg,
                             iEventRequest ) );

    // Check the status of command sending
    if ( commandSent )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSUiSubSession::HandleUiEventNotification commandSent" )
        // If command was sent, set flag to false, so when next Event request
        // arrives, command data is not sent.
        iUiEventRequested = EFalse;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::HandleUiEventNotification exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::SendCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::SendCommand(
    TDesC8* aCmdData, // Data from SIM.
    TDes8* aCmdRsp, // Response.
    TSatServerRequest aRequest ) // Request, which the aData is sent.
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::SendCommand calling" )

    const TInt count( iServiceRequest->Count() );
    TBool requestHandled( EFalse );
    TInt index( 0 );

    // Loop until request is handled or no request handler found.
    while ( !requestHandled && ( index < count ) )
        {
        requestHandled = iServiceRequest->At( index )->HandleCommand(
            aCmdData, aCmdRsp, aRequest );
        index++;
        }
    LOG2( SIMPLE, "SATENGINE: CSatSUiSubSession::SendCommand \
          index: %i", index )
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::SendCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::Event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::Event calling" )

    if ( MSatUtils::ESmsSent == aEvent )
        {
        LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::Event ESmsSent" )
        iSmsSent = ETrue;
        SendSmsSentEventToClient();
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiSubSession::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::SendCommandDataToClient
// Writes descriptor to clients address space and
// completes the request.
// -----------------------------------------------------------------------------
//
TBool CSatSUiSubSession::SendCommandDataToClient(
    TBool& aRequestAvailable, // Is request available
    const TDesC8& aData, // Data to be sent to client
    const RMessage2& aMessage ) const // Message of the request
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::SendCommandDataToClient calling" )

    TBool commandSent( EFalse );

    // Is ui client ready to receive the command data.
    if ( aRequestAvailable )
        {
        LOG( SIMPLE, "SATENGINE: SendCommandDataToClient: Sending to Client" )
        TRAPD( res, aMessage.WriteL( 0, aData ) );
        if ( KErrNone != res )
            {
            LOG( SIMPLE, 
            "SATENGINE: SendCommandDataToClient: KErrNone != res" )
            iSession.PanicClient( aMessage, ESatSBadDescriptor );
            }
        aMessage.Complete( KErrNone );
        aRequestAvailable = EFalse;
        commandSent = ETrue;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::SendCommandDataToClient exiting" )
    return commandSent;
    }

// -----------------------------------------------------------------------------
// CSatSUiSubSession::SendSmsSentEventToClient
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::SendSmsSentEventToClient()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::SendSmsSentEventToClient calling" )

    iEventData.iEvent = ESatSSmEndEvent;
    const TBool commandSent( SendCommandDataToClient( iEventRequestAvailable,
        iEventDataPckg, iEventRequest ) );

    if ( commandSent )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSUiSubSession::SendSmsSentEventToClient commandSent" )
        // If command was sent, set flag to false, so when next Event request
        // arrives, command data is not sent.
        iSmsSent = EFalse;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSUiSubSession::SendSmsSentEventToClient exiting" )
    }
    
// -----------------------------------------------------------------------------
// Publish the P&S key KSatAppClosedUsingEndKey to indicate the occurred
// End Key action
// -----------------------------------------------------------------------------
//
void CSatSUiSubSession::PublishSatAppClosedUsingEndKey()
    {
    LOG( SIMPLE,
        "SATENGINE:CSatSUiSubSession::PublishSatAppClosedUsingEndKey calling" )

    TInt error = RProperty::Set( KPSUidSatServerInternal,
        KSatAppClosedUsingEndKey, KSatAppTerminatedUsingEndKey );
    LOG2( SIMPLE, "CSatSUiSubSession::PublishSatAppClosedUsingEndKey \
          P&S key: %i", KSatAppTerminatedUsingEndKey )    
    LOG2(NORMAL, 
        "SATENGINE: CSatSUiSubSession::PublishSatAppClosedUsingEndKey -  \
        publish KSapAppClosedUsingEndKey and return: %d ", error );

    LOG( SIMPLE,
        "SATENGINE:CSatSUiSubSession::PublishSatAppClosedUsingEndKey exiting" )
    }
