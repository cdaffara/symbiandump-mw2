/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The Sat Server session manages a connection to a client and
*                handles client requests.
*
*/


#include    <fbs.h>
#include    "MSatUtils.h"
#include    "CSatIconHandler.h"
#include    "CSatSSession.h"
#include    "CSatSUiSubSession.h"
#include    "CSatSServer.h"
#include    "EnginePanic.h"
#include    "SatLog.h"
#include    "TSatEventMediator.h"
#include    "SatSOpcodes.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "CSatSRefreshSubSession.h"
#include    "MThreadDeathNotifier.h"
#include    "CSatSIconSubSession.h"

const TUint8 KHandleParam( 3 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Class constructor
// -----------------------------------------------------------------------------
//
CSatSSession::CSatSSession(
    CSatSServer* aServer,
    TSatEventMediator& aEventMediator,
    MSatApi& aSatApi,
    MThreadDeathNotifier& aThreadDeathNotifier ) :
    CSession2(),
    iSatServer( aServer ),
    iEventMediator( aEventMediator ),
    iSatApi( aSatApi ),
    iThreadDeathNotifier( aThreadDeathNotifier )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::CSatSSession calling-exiting" )
    }

// -----------------------------------------------------------------------------
// Two-phased constructor
// -----------------------------------------------------------------------------
//
CSatSSession* CSatSSession::NewL(
    CSatSServer* aServer,
    TSatEventMediator& aEventMediator,
    MSatApi& aSatApi,
    MThreadDeathNotifier& aThreadDeathNotifier )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::NewL calling" )

    // Perform construction.
    CSatSSession* self =
        new ( ELeave ) CSatSSession(
            aServer,
            aEventMediator,
            aSatApi,
            aThreadDeathNotifier );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSSession::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSatSSession::~CSatSSession()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::~CSatSSession calling" )

    delete iSubSessionIx;
    delete iSubSessionCon;

    iSatServer = NULL;
    iUiSubSession = NULL;

    LOG( SIMPLE, "SATENGINE: CSatSSession::~CSatSSession exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::ConstructL
// -----------------------------------------------------------------------------
//
void CSatSSession::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::ConstructL calling" )

    iSubSessionCon = iSatServer->CreateNewContainerL();
    iSubSessionIx = CObjectIx::NewL();

    LOG( SIMPLE, "SATENGINE: CSatSSession::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::UiSubSession
// -----------------------------------------------------------------------------
//
MSatUiSession* CSatSSession::UiSubSession()
    {
    LOG( DETAILED, "SATENGINE: CSatSSession::UiSubSession calling" )

    MSatUiSession* session = iUiSubSession;

    LOG( DETAILED, "SATENGINE: CSatSSession::UiSubSession exiting" )
    return session;
    }

// -----------------------------------------------------------------------------
// CSatSSession::CloseSubSession
// -----------------------------------------------------------------------------
TBool CSatSSession::CloseSubSession( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::CloseSubSession calling" )

    // If subssession exists, return TRUE. Otherwise, return FALSE
    TInt result = ETrue;
    // Read the handle from client.
    const TInt handle( aMessage.Int3() );

    // Check that subsession exists before deleting the subsession
    if ( iSubSessionIx->At( handle ) )
        {
        LOG(
        SIMPLE, "SATENGINE: CSatSSession::CloseSubSession subsession exists" )
        // Delete the sub-session.
        iSubSessionIx->Remove( handle );
        }
    else
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSSession::CloseSubSession subsession doesn't exist" )
        // Sub-session handle was not valid.
        PanicClient( aMessage, ESatSBadDescriptor );
        result = EFalse;
        }

    LOG( SIMPLE, "SATENGINE: CSatSSession::CloseSubSession exiting" )
    return result;
    }

// -----------------------------------------------------------------------------
// CSatSSession::GetSubSessionFromHandle
// Returns the subsession which handle is aHandle.
// -----------------------------------------------------------------------------
CSatSSubSession* CSatSSession::GetSubSessionFromHandle( TInt aHandle ) const
    {
    LOG( DETAILED, "SATENGINE: CSatSSession::GetSubSessionFromHandle calling" )

    CSatSSubSession* session =
        static_cast<CSatSSubSession*>( iSubSessionIx->At( aHandle ) );

    LOG( DETAILED, "SATENGINE: CSatSSession::GetSubSessionFromHandle exiting" )
    return session;
    }

// -----------------------------------------------------------------------------
// CSatSSession::AddSubSessionL
// -----------------------------------------------------------------------------
void CSatSSession::AddSubSessionL(
    CSatSSubSession* aSubSession,
    const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::AddSubSessionL calling" )

    __ASSERT_ALWAYS( aSubSession, PanicSatEngine( ESatEngineNullPointer ) );

    CleanupStack::PushL( aSubSession );
    iSubSessionCon->AddL( aSubSession );
    const TInt handle( iSubSessionIx->AddL( aSubSession ) );
    CleanupStack::Pop( aSubSession );

    // Send the handle to client.
    TPckg<TInt> handlePckg( handle );
    TRAPD( err, aMessage.WriteL( KHandleParam, handlePckg ) );

    if ( KErrNone != err )
        {
        // Remove the handle, will also cause destruction
        // of session.
        iSubSessionIx->Remove( handle );
        LOG2( SIMPLE, 
        "SATENGINE: CSatSSession::AddSubSessionL panic: %i", err )
        PanicClient( aMessage, ESatSBadDescriptor );
        User::Leave( err ); 
        }

    LOG( SIMPLE, "SATENGINE: CSatSSession::AddSubSessionL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::ServiceL
// Dispatches the commands to their appropriate handlers. For asynchronous
// messages, the message object is save for future processing.
// -----------------------------------------------------------------------------
//
void CSatSSession::ServiceL( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::ServiceL calling" )

    if ( !aMessage.HasCapability( ECapabilityReadDeviceData ) )
        {
        // Legal client should at least have ReadDeviceData capability
        LOG( SIMPLE, "SATENGINE: CSatSSession::ServiceL client \
        has no ReadDeviceData capability. Panic it" )
        PanicClient( aMessage, ESatSBadDescriptor );
        }
    else
        {
        // Client has enough capability
        
        LOG2( NORMAL, "SATENGINE: CSatSSession::ServiceL \
              aMessage.Function is %d", aMessage.Function() )

        switch ( aMessage.Function() )
            {
            //  Ui Sub-session indicator.
            case ESatSOpenUiSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSOpenUiSubSession" )
                CSatSUiSubSession* subsession = CSatSUiSubSession::NewL(
                    *this,
                    iSatApi,
                    iEventMediator );

                TRAPD( err, AddSubSessionL( subsession, aMessage ) );
                if ( KErrNone == err )
                    {
                    // Store the ui clients thread handle.
                    RThread uiClientThread;
                    User::LeaveIfError( aMessage.Client( uiClientThread ) );

                    iUiSubSession = subsession;

                    // Start observing threads state
                    TThreadId threadId = uiClientThread.Id();
                    iThreadDeathNotifier.SetObserveredThread( threadId );
                    iThreadDeathNotifier.StartObservingThread();
                    uiClientThread.Close();

                    // Complete message before notifying SAT Server since notifying may
                    // be very time consuming operation dependening on the situation
                    aMessage.Complete( KErrNone );

                    // Event has to be send after the iUiSubSession has been set.
                    // Otherwise NULL pointer error in commandhandlers.
                    iEventMediator.Notify( MSatUtils::ESatUiLaunched );
                    }
                break;
                }

            case ESatSOpenRefreshSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSOpenRefreshSubSession" )
                CSatSRefreshSubSession* subsession = CSatSRefreshSubSession::NewL(
                    *this,
                    iEventMediator );
                
                TRAPD( err, AddSubSessionL( subsession, aMessage ) );
                if ( KErrNone == err )
                    {
                    aMessage.Complete( KErrNone );
                    }
                break;
                }

            case ESatSOpenIconSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSOpenIconSubSession" )
                CSatSIconSubSession* subsession = CSatSIconSubSession::NewL(
                    *this,
                    iEventMediator );

                TRAPD( err, AddSubSessionL( subsession, aMessage ) );
                if ( KErrNone == err )
                    {
                    aMessage.Complete( KErrNone );
                    }
                break;
                }

            case ESatSCloseUiSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSCloseUiSubSession" )
                iEventMediator.Notify( MSatUtils::ESatUiClosed );
                iSatUiSubSessionClosed = ETrue;
                iUiSubSession = NULL;
                if ( CloseSubSession( aMessage ) )
                    {
                    aMessage.Complete( KErrNone );
                    }
                break;
                }

            case ESatSCloseRefreshSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSCloseRefreshSubSession" )
                if ( CloseSubSession( aMessage ) )
                    {
                    aMessage.Complete( KErrNone );
                    }
                break;
                }

            case ESatSCloseIconSubSession:
                {
                LOG( SIMPLE, 
                "SATENGINE: CSatSSession::ServiceL ESatSCloseIconSubSession" )
                if ( CloseSubSession( aMessage ) )
                    {
                    aMessage.Complete( KErrNone );
                    }
                break;
                }

            default:
                {
                LOG( SIMPLE, "SATENGINE: CSatSSession::ServiceL others" )
                CSatSSubSession* subsession =
                    GetSubSessionFromHandle( aMessage.Int3() );

                if ( NULL != subsession )
                    {
                    LOG( SIMPLE, 
                    "SATENGINE: CSatSSession::ServiceL NULL != subsession" )
                    subsession->HandleCommandL( aMessage );
                    }
                else
                    {
                    PanicClient( aMessage, ESatSBadDescriptor );
                    }
                }
            }
        }
        LOG( SIMPLE, "SATENGINE: CSatSSession::ServiceL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::PanicClient
// Panics the client.
// -----------------------------------------------------------------------------
//
void CSatSSession::PanicClient( const RMessage2& aMessage, TInt aPanic ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::PanicClient calling" )

    _LIT( KTxtServer, "SatS server" );
    aMessage.Panic( KTxtServer, aPanic );

    LOG( SIMPLE, "SATENGINE: CSatSSession::PanicClient exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::NotifyThreadDeathMonitor
// Notifies ThreadDeathMonitor for thread is dying
// -----------------------------------------------------------------------------
//
void CSatSSession::NotifyThreadDeathMonitor()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::NotifyThreadDeathMonitor calling" )

    iThreadDeathNotifier.NotifyThreadDeath();

    LOG( SIMPLE, "SATENGINE: CSatSSession::NotifyThreadDeathMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::SatServer
// Returns pointer to SatServer - class
// -----------------------------------------------------------------------------
//
CSatSServer* CSatSSession::SatServer()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::SatServer calling-exiting" )
    return iSatServer;
    }

// -----------------------------------------------------------------------------
// CSatSSession::UiSessionClosed
// Notifies SatEngine that UI session has closed
// -----------------------------------------------------------------------------
//
void CSatSSession::UiSessionClosed()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::UiSessionClosed calling" )

    if ( !iSatUiSubSessionClosed )
        {
        LOG( SIMPLE, "SATENGINE: CSatSSession::UiSessionClosed close satui" )
        iEventMediator.Notify( MSatUtils::ESatUiClosed );
        iSatUiSubSessionClosed = ETrue;
        }

    LOG( SIMPLE, "SATENGINE: CSatSSession::UiSessionClosed exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSession::CreateIconHandlerL
// Creates and returns CSatIconHandler
// -----------------------------------------------------------------------------
//
MSatSIconAPI* CSatSSession::CreateIconHandlerL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSSession::CreateIconHandlerL calling-exiting" )
    return new( ELeave ) CSatIconHandler( iSatApi );
    }
