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
* Description:  The UI Client API of the SAT Server.
*
*/



//  INCLUDE FILES
#include <e32def.h>
#include <rsatsession.h>
#include <data_caging_path_literals.hrh>

#include "RSatUiSession.h"
#include "MSatUiObserver.h"
#include "MSatUiAdapter.h"
#include "SatSOpcodes.h"
#include "CSatCCommandProcessor.h"
#include "SatServerFactory.h"
#include "SatLog.h"

// CONSTANTS

#if defined( __WINS__ )
const TInt KHeapMinSize = 0x10000;
const TInt KHeapMaxSize = 0x10000;
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RSatUiSession::RSatUiSession
// Class constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C RSatUiSession::RSatUiSession() :
    RSubSessionBase(),
    iCommandProcessor( NULL ),
    iSatUiObserver( NULL )
    {
    }

// -----------------------------------------------------------------------------
// RSatUiSession::RegisterL
// Registers a new client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::RegisterL(
    const RSatSession& aSession,
    MSatUiObserver* aSatUiObserver )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::RegisterL calling" )

    // If the server isn't already started, we can't do anything.
    if ( !IsServerStarted() )
        {
        LOG( SIMPLE, 
        "SATINTERNALCLIENT: RSatUiSession::RegisterL server not started" )
        User::Leave( KErrCouldNotConnect );
        }

    iSatUiObserver = aSatUiObserver;
    iCommandProcessor = CSatCCommandProcessor::NewL( this );

    // Connect to the server side session.
    TIpcArgs arguments( 0 );
    User::LeaveIfError(
        CreateSubSession(
            aSession,
            ESatSOpenUiSubSession, arguments ) );

    // Start the handlers.
    iCommandProcessor->Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::RegisterL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::Adapter
// Returns a pointer to an adapter object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C MSatUiAdapter* RSatUiSession::Adapter() const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Adapter calling" )

    if ( iCommandProcessor )
        {
        MSatUiAdapter* satUiAdapter = iCommandProcessor->ActiveCommandHandler();

        LOG( SIMPLE,
            "SATINTERNALCLIENT: RSatUiSession::Adapter (iCommandProcessor) exiting" )
        return satUiAdapter;
        }

    else
        {
        LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Adapter exiting" )
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// RSatUiSession::Unregister
// Unregisters a client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RSatUiSession::Unregister()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Unregister calling" )

     if ( iCommandProcessor )
        {
        LOG( SIMPLE, 
        "SATINTERNALCLIENT: RSatUiSession::Unregister iCommandProcessor true" )
        delete iCommandProcessor;
        iCommandProcessor = NULL;
        }

    iSatUiObserver = NULL;

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Unregister exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::Version
// Returns the current version of the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TVersion RSatUiSession::Version() const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Version calling-exiting" )
    return TVersion(
        KSatServerMajorVersionNumber,
        KSatServerMinorVersionNumber,
        KSatServerBuildVersionNumber );
    }

// -----------------------------------------------------------------------------
// RSatUiSession::Close
// Closes the server session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::Close()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Close calling" )

    Unregister();
    CloseSubSession( ESatSCloseUiSubSession );

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Close exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::Panic
// Panics the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::Panic(
    TSatClientPanic aReason ) const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::Panic" )

    User::Panic( KSatCliName, aReason );
    }

// -----------------------------------------------------------------------------
// RSatUiSession::KillServer
// Kills the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::KillServer() const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::KillServer calling" )

    // Use a dummy arguments table.
    TIpcArgs arguments( 0 );

    // Request the server shutdown.
    SendReceive( ESatSShutdown, arguments );

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::KillServer exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::IsServerStarted
// Checks if the server is already started.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool RSatUiSession::IsServerStarted() const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::IsServerStarted calling" )

    TFindServer findServer( KSatServerName );
    TFullName name;
    TBool started = ( KErrNone == findServer.Next( name ) );

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::IsServerStarted exiting" )
    return started;
    }

// -----------------------------------------------------------------------------
// RSatUiSession::CreateRequest
// Creates Send Receive request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::CreateRequest(
    TInt aFunction,
    const TIpcArgs& aArgs,
    TRequestStatus& aStatus ) const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::CreateRequest calling" )

    SendReceive( aFunction, aArgs, aStatus );

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::CreateRequest exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::CreateRequest
// Creates Send Receive request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatUiSession::CreateRequest(
    TInt aFunction,
    const TIpcArgs& aArgs ) const
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::CreateRequest calling" )

    SendReceive( aFunction, aArgs );

    LOG( SIMPLE, "SATINTERNALCLIENT: RSatUiSession::CreateRequest exiting" )
    }

// -----------------------------------------------------------------------------
// RSatUiSession::SatUiObserver
// Returns a pointer to a client observer object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C MSatUiObserver* RSatUiSession::SatUiObserver()
    {
    LOG( SIMPLE, 
    "SATINTERNALCLIENT: RSatUiSession::SatUiObserver calling-exiting" )
    return iSatUiObserver;
    }

// -----------------------------------------------------------------------------
// RSatUiSession::CommandProcessor
// Returns a pointer to a command processor object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSatCCommandProcessor* RSatUiSession::CommandProcessor()
    {
    LOG( SIMPLE, 
    "SATINTERNALCLIENT: RSatUiSession::CommandProcessor calling-exiting" )
    return iCommandProcessor;
    }

//  End of File
