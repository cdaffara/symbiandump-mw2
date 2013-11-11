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
* Description:  Handles the refresh sub-session IPC.
*
*/


#include    <e32base.h>
#include    "MSatUtils.h"
#include    "CSatSRefreshSubSession.h"
#include    "SatSOpcodes.h"
#include    "CSatSSession.h"
#include    "EnginePanic.h"
#include    "CSatSServer.h"
#include    "SatLog.h"

const TInt KClientQueryDelay = 1000000; // 1 second.
const TInt KClientNotifyDelay = 60000000; // 1 minute.

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::CSatSRefreshSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSRefreshSubSession::CSatSRefreshSubSession(
    CSatSSession& aSession,
    TSatEventMediator& aEventMediator ) :
    CSatSSubSession( aSession ),
    iEventMediator( aEventMediator ),
    iClientQueryPending( EFalse ),
    iClientNotifyPending( EFalse ),
    iClientWaitingRefresh( EFalse )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::CSatSRefreshSubSession \
        calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::ConstructL calling" )

    CSatSServer* server =
        const_cast<CSatSServer*>(
            static_cast<const CSatSServer*>( iSession.Server() ) );
    server->AddL( this );

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSRefreshSubSession* CSatSRefreshSubSession::NewL(
    CSatSSession& aSession,
    TSatEventMediator& aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NewL calling" )

    CSatSRefreshSubSession* self =
        new( ELeave ) CSatSRefreshSubSession( aSession, aEventMediator );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::~CSatSRefreshSubSession()
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSRefreshSubSession::~CSatSRefreshSubSession()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::~CSatSRefreshSubSession calling" )

    CSatSServer* server =
        const_cast<CSatSServer*>(
            static_cast<const CSatSServer*>( iSession.Server() ) );
    server->Remove( this );

    // Remove Client Query timer.
    if ( iQueryTimer )
        {
        iQueryTimer->Cancel();
        delete iQueryTimer;
        iQueryTimer = NULL;
        }

    // Remove Client Notify timer.
    if ( iNotifyTimer )
        {
        iNotifyTimer->Cancel();
        delete iNotifyTimer;
        iNotifyTimer = NULL;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::~CSatSRefreshSubSession exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::HandleCommandL
// Handles the refresh sub-session requests.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::HandleCommandL( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::HandleCommandL calling" )

    switch ( aMessage.Function() )
        {
        case ESatSRefreshAllowed:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSRefreshAllowed" )
            // Client's response to refresh query.
            RefreshAllowed( aMessage );
            break;
            }

        case ESatSNotifyAllowRefresh:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSNotifyAllowRefresh" )
            // Client is ready for refresh query.
            NotifyRefreshAllowed( aMessage );
            break;
            }

        case ESatSNotityAllowRefreshCancel:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSNotityAllowRefreshCancel" )
            // Client cancels the refresh query.
            CancelNotifyAllowRefresh( aMessage );
            break;
            }

        case ESatSNotifyRefresh:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSNotifyRefresh" )
            // Client is ready for refresh.
            NotifyRefresh( aMessage );
            break;
            }

        case ESatSCancelNotifyRefresh:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSCancelNotifyRefresh" )
            // Client cancels the refresh.
            CancelNotifyRefresh( aMessage );
            break;
            }

        case ESatSRefreshEFRead:
            {
            LOG( NORMAL, "SATENGINE:   Command: ESatSRefreshEFRead" )
            // Client has completed refresh.
            CompleteRefresh( aMessage );
            break;
            }

        default:
            {
            LOG( NORMAL, "SATENGINE:   Unexpected command: ggg" )
            // A wrong message was sent.
            iSession.PanicClient( aMessage, ESatSBadDescriptor );
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::HandleCommandL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::RefreshAllowed
// Sends the client's response of refresh query to SatServer.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::RefreshAllowed( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::RefreshAllowed calling" )

    // Handle reply only when we are waiting for it.
    // It is possible, that query has timed out and we got a late response.
    if ( iClientQueryPending )
        {
        LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::RefreshAllowed \
        iClientQueryPending true" )
        iClientQueryPending = EFalse;

        // Stop timer, client replied.
        if ( iQueryTimer )
            {
            LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::RefreshAllowed \
            iQueryTimer true" )
            iQueryTimer->Cancel();
            }

        TBool allow( EFalse );
        TPckg<TBool> allowPckg( allow );
        TRAPD( err, aMessage.ReadL( 0, allowPckg ) );
        LOG2( SIMPLE, "SATENGINE: CSatSRefreshSubSession::RefreshAllowed \
              err: %i", err )
        if ( KErrNone == err )
            {
            //lint -e{774} Allow updated through allowPckg in ReadL.
            if ( allow )
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                     RefreshAllowed allow refresh" )
                iEventMediator.Notify( MSatUtils::ERefreshAllowed );
                iClientWaitingRefresh = ETrue;
                }
            else
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                     RefreshAllowed don't allow refresh" )
                iEventMediator.Notify( MSatUtils::ERefreshNotAllowed );
                }
            }
        else
            {
            iSession.PanicClient( aMessage, ESatSBadDescriptor );
            }
        }

    aMessage.Complete( KErrNone );

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::RefreshAllowed exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NotifyRefresh
// Stores the NotifyRefresh request information for later completion.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::NotifyRefresh( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyRefresh calling" )

    if ( !iNotifyRefreshPending )
        {
        iNotifyRefreshPending = ETrue;
        iNotifyRefreshMessage = aMessage;
        }
    else
        {
        LOG( SIMPLE, "SATENGINE:   Request already received" )
        aMessage.Complete( KErrInUse );
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::CancelNotifyRefresh
// Cancels the refresh notification
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::CancelNotifyRefresh( const RMessage2& aMessage )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::CancelNotifyRefresh calling" )

    if ( iNotifyRefreshPending )
        {
        LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::CancelNotifyRefresh\
        iNotifyRefreshPending true" )
        iNotifyRefreshPending = EFalse;
        iNotifyRefreshMessage.Complete( KErrCancel );
        }

    aMessage.Complete( KErrNone );

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::CancelNotifyRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NotifyRefreshAllowed
// Storest the refresh notification for later completion
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::NotifyRefreshAllowed( const RMessage2& aMessage )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::NotifyRefreshAllowed calling" )

    if ( !iRefreshAllowPending )
        {
        iRefreshAllowPending = ETrue;
        iRefreshAllowMessage = aMessage;
        }
    else
        {
        LOG( SIMPLE, "SATENGINE:   Request already received" )
        aMessage.Complete( KErrInUse );
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::NotifyRefreshAllowed exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::CancelNotifyAllowRefresh
// Cancels the refresh notification.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::CancelNotifyAllowRefresh(
    const RMessage2& aMessage )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::CancelNotifyAllowRefresh calling" )

    if ( iRefreshAllowPending )
        {
        LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
        CancelNotifyAllowRefresh iRefreshAllowPending true" )
        iRefreshAllowPending = EFalse;
        iRefreshAllowMessage.Complete( KErrCancel );
        }

    aMessage.Complete( KErrNone );

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::CancelNotifyAllowRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::CompleteRefresh
// Sends the client's completion to SatServer.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::CompleteRefresh( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::CompleteRefresh calling" )

    // Handle completion only when we are waiting for it.
    // It is possible, that it has timed out and we got a late response.
    if ( iClientNotifyPending )
        {
        LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::CompleteRefresh \
        iClientNotifyPending true" )
        iClientNotifyPending = EFalse;

        // Stop timer, client replied.
        if ( iNotifyTimer )
            {
            LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::CompleteRefresh \
            iNotifyTimer true" )
            iNotifyTimer->Cancel();
            }

        TBool additionEFRead( EFalse );
        TPckg<TBool> additionEFReadPckg( additionEFRead );
        TRAPD( err, aMessage.ReadL( 0, additionEFReadPckg ) );
        if ( KErrNone == err )
            {
            //lint -e{774} additionEFRead updated through additionEFReadPckg.
            if ( additionEFRead )
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                     CompleteRefresh additionEFRead true" )
                iEventMediator.Notify( MSatUtils::ERefreshAdditionalEFRead );
                }
            else
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                     CompleteRefresh additionEFRead false" )
                iEventMediator.Notify( MSatUtils::ERefreshEFRead );
                }
            }
        else
            {
            LOG( SIMPLE, "SATENGINE:   Bad descriptor" )
            iEventMediator.Notify( MSatUtils::ERefreshEFRead );
            }
        }

    aMessage.Complete( KErrNone );

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::CompleteRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::QueryClient
// Notifies the client that response for refresh query has to be made.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::QueryClient(
    const RSat::TRefreshV2& aRefresh )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::QueryClient calling" )
    TBool allowRefreshNow( ETrue );

    if ( iRefreshAllowPending )
        {
        if ( VerifyObservedFiles( aRefresh ) )
            {
            // Write the refresh information to client.
            TPckg<RSat::TRefreshFileList> fileListPckg( aRefresh.iFileList );
            TPckg<RSat::TRefreshType> typePckg( aRefresh.iType );

            TRAPD(
                err,
                iRefreshAllowMessage.WriteL( 0, fileListPckg );
                iRefreshAllowMessage.WriteL( 1, typePckg ); )

            if ( KErrNone == err )
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::QueryClient \
                KErrNone == err" )
                iRefreshAllowPending = EFalse;
                iRefreshAllowMessage.Complete( err );
                iClientQueryPending = ETrue;
                allowRefreshNow = EFalse;

                // Start timer to prevent jam, when client will not reply.
                if ( iQueryTimer )
                    {
                    LOG( SIMPLE, "CSatSRefreshSubSession::QueryClient \
                         iQueryTimer true" )
                    iQueryTimer->Cancel();
                    }
                else
                    {
                    // Create Client Query timer when first time used.
                    TRAP(
                        err,
                        iQueryTimer =
                            CPeriodic::NewL( CActive::EPriorityStandard ); )
                    }

                if ( ( KErrNone == err ) && iQueryTimer )
                    {
                    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                    QueryClient start iQueryTimer" )
                    TCallBack callback( TimerQueryCallback, this );
                    iQueryTimer->Start(
                        KClientQueryDelay,
                        KClientQueryDelay,
                        callback );
                    }
                }
            else
                {
                LOG2( SIMPLE, "SATENGINE:   QueryClient error: %i", err )
                }
            }
        }

    if ( allowRefreshNow )
        {
        LOG( SIMPLE, "CSatSRefreshSubSession::QueryClient \
        allowRefreshNow true" )
        iEventMediator.Notify( MSatUtils::ERefreshAllowed );
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::QueryClient exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NotifyClient
// Notifies the client that refresh has happened.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::NotifyClient(
    const RSat::TRefreshV2& aRefresh )
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyClient calling" )
    TBool completeRefreshNow( ETrue );

    if ( iNotifyRefreshPending && iClientWaitingRefresh )
        {
        TPckg<RSat::TRefreshFileList> fileListPckg( aRefresh.iFileList );

        #ifdef ENABLE_SAT_LOGGING
        TInt fileCount ( aRefresh.iFileList.Length() );
        LOG2( NORMAL, "SATENGINE:   Refresh file count: %d", fileCount )
        for ( TInt fileIndex = 0; fileIndex < fileCount; fileIndex++ )
            {
            LOG2( NORMAL, "SATENGINE:     Refresh file : %x",
                aRefresh.iFileList[ fileIndex ] )
            }
        #endif

        TPckg<RSat::TRefreshType> typePckg( aRefresh.iType );

        TRAPD(
            err,
            iNotifyRefreshMessage.WriteL( 0, fileListPckg );
            iNotifyRefreshMessage.WriteL( 1, typePckg ); )

        LOG2( SIMPLE, "SATENGINE:   NotifyClient error: %i", err )
        if ( KErrNone == err )
            {
            // ESimReset confirmation will not be waited.
            if ( RSat::ESimReset != aRefresh.iType )
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyClient\
                RSat::ESimReset != aRefresh.iType" )
                iClientNotifyPending = ETrue; // Waiting client
                completeRefreshNow = EFalse; // Client must complete this

                // Start timer to prevent jam, when client will not restart.
                if ( iNotifyTimer )
                    {
                    LOG( SIMPLE, "CSatSRefreshSubSession::NotifyClient \
                         iNotifyTimer true" )
                    iNotifyTimer->Cancel();
                    }
                else
                    {
                    // Create Client Notify timer when first time used.
                    TRAP(
                        err,
                        iNotifyTimer =
                            CPeriodic::NewL( CActive::EPriorityStandard ); )
                    }

                if ( ( KErrNone == err ) && iNotifyTimer )
                    {
                    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                    NotifyClient start iNotifyTimer" )
                    TCallBack callback( TimerNotifyCallback, this );
                    iNotifyTimer->Start(
                        KClientNotifyDelay,
                        KClientNotifyDelay,
                        callback );
                    }
                }
            }

        iNotifyRefreshPending = EFalse;
        iClientWaitingRefresh = EFalse;
        iNotifyRefreshMessage.Complete( err );
        }

    if ( completeRefreshNow )
        {
        LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyClient \
        completeRefreshNow true" )
        iEventMediator.Notify( MSatUtils::ERefreshEFRead );
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyClient exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NotifyClientCancel
// Notifies the client that refresh has been cancelled.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::NotifyClientCancel()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::NotifyClientCancel calling" )

    // Notify only clients having accepted Refresh and waiting it.
    if ( iNotifyRefreshPending && iClientWaitingRefresh )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::NotifyClientCancel wait refresh" )
        RSat::TRefreshFileList fileList;
        fileList.Zero();
        TPckg<RSat::TRefreshFileList> fileListPckg( fileList );
        TPckg<RSat::TRefreshType> typePckg( RSat::ERefreshTypeNotSet );
        iNotifyRefreshPending = EFalse;
        iClientWaitingRefresh = EFalse;

        TRAPD(
            err,
            iNotifyRefreshMessage.WriteL( 0, fileListPckg );
            iNotifyRefreshMessage.WriteL( 1, typePckg ) );
        
        LOG2( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyClientCancel error: %i", err )

        iNotifyRefreshMessage.Complete( err );
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::NotifyClientCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::VerifyObservedFiles
// Checks whether current client is interested about this refresh.
// -----------------------------------------------------------------------------
//
TBool CSatSRefreshSubSession::VerifyObservedFiles(
    const RSat::TRefreshV2& aRefresh ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::VerifyObservedFiles calling" )
    TBool isObserved( ETrue );

    // Only pure file refresh is checked.
    if ( RSat::EFileChangeNotification == aRefresh.iType )
        {
        // Fetch observed files from clients message.
        RSat::TRefreshFileList observedFiles;
        observedFiles.Zero();
        TPckg<RSat::TRefreshFileList> observedPckg( observedFiles );
        TRAPD( err, iRefreshAllowMessage.ReadL( 0, observedPckg ) );
        LOG2( SIMPLE, "SATENGINE:   ReadL failed: %i", err )
        if ( KErrNone == err )
            {
            TInt observeFileCount( observedFiles.Length() );
            LOG2( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                  VerifyObservedFiles observeFileCount: %i", observeFileCount )
            if ( observeFileCount > 0 )
                {
                LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::\
                VerifyObservedFiles observeFileCount > 0" )
                // Observed files found, check whether any of them has been
                // updated.
                isObserved = EFalse;
                for ( TInt fileIndex = 0;
                      ( fileIndex < observeFileCount ) && !isObserved;
                      fileIndex++ )
                    {
                    if ( KErrNotFound !=
                         aRefresh.iFileList.Locate(
                            observedFiles[ fileIndex ] ) )
                        {
                        isObserved = ETrue;
                        LOG( SIMPLE, "SATENGINE:   Observed file found" )
                        }
                    }
                }
            }
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::VerifyObservedFiles exiting" )
    return isObserved;
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::TimerQueryCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatSRefreshSubSession::TimerQueryCallback(
    TAny* aRefreshSubSession )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerQueryCallback calling" )

    CSatSRefreshSubSession* handler =
        static_cast<CSatSRefreshSubSession*>( aRefreshSubSession );

    if ( handler )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerQueryCallback handler true" )
        handler->QueryTimeout();
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerQueryCallback exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::QueryTimeout
// Timeout handler for the client's response of refresh query.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::QueryTimeout()
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::QueryTimeout calling" )

    if ( iQueryTimer )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::QueryTimeout iQueryTimer true" )
        iQueryTimer->Cancel();
        }

    // Handle reply timeout only when we are waiting for it.
    if ( iClientQueryPending )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::QueryTimeout \
        iClientQueryPending true" )
        iClientQueryPending = EFalse;
        iEventMediator.Notify( MSatUtils::ERefreshAllowed );
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::QueryTimeout exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::TimerNotifyCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatSRefreshSubSession::TimerNotifyCallback(
    TAny* aRefreshSubSession )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerNotifyCallback calling" )

    CSatSRefreshSubSession* handler =
        static_cast<CSatSRefreshSubSession*>( aRefreshSubSession );

    if ( handler )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerNotifyCallback handler" )
        handler->NotifyTimeout();
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSRefreshSubSession::TimerNotifyCallback exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatSRefreshSubSession::NotifyTimeout
// Timeout handler for the client's response of refresh query.
// -----------------------------------------------------------------------------
//
void CSatSRefreshSubSession::NotifyTimeout()
    {
    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyTimeout calling" )

    if ( iNotifyTimer )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSRefreshSubSession::NotifyTimeout iNotifyTimer true" )
        iNotifyTimer->Cancel();
        }

    // Handle reply timeout only when we are waiting for it.
    if ( iClientNotifyPending )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSRefreshSubSession::NotifyTimeout \
        iClientNotifyPending" )
        iClientNotifyPending = EFalse;
        // On timeout we should assume no EFs read, as most typical for S60.
        iEventMediator.Notify( MSatUtils::ERefreshEFRead );
        }

    LOG( SIMPLE, "SATENGINE: CSatSRefreshSubSession::NotifyTimeout exiting" )
    }
