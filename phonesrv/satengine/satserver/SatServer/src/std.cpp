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
* Description:  General functions
*
*/


#include    <e32base.h>
#include    <data_caging_path_literals.hrh>
#include    "SatServerFactory.h"
#include    "CSatSScheduler.h"
#include    "SatSPanic.h"
#include    "SatLog.h"

const TInt8 KInitServerPop( 2 );
_LIT( KSatServerMtx, "SATSERVERMTX" );

// ======== LOCAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// PanicServer
// Panics the SatServer.
// -----------------------------------------------------------------------------
//
void PanicServer(
    TSatSPanicCode aPanicCode ) // Panic code
    {
    LOG( SIMPLE, "SATSERVER: PanicServer" )
    User::Panic( KSatServerPanic, aPanicCode );
    }

// -----------------------------------------------------------------------------
// InitServerL
// Initialises the SatServer.
// -----------------------------------------------------------------------------
//
LOCAL_C void InitServerL()
    {
    LOG( SIMPLE, "SATSERVER: InitServerL calling" )

    RMutex serverStartMutex;
    TInt createErr( serverStartMutex.CreateGlobal( KSatServerMtx ) );
    if ( createErr )
        {
        TInt openErr( serverStartMutex.OpenGlobal( KSatServerMtx ) );
        User::LeaveIfError( openErr );
        LOG( SIMPLE, "SATSERVER:   Opened SATSERVERMTX" )
        }

    LOG( SIMPLE, "SATSERVER:   Asking ownership of SATSERVERMTX" )
    serverStartMutex.Wait();
    LOG( SIMPLE, "SATSERVER:   Got ownership of SATSERVERMTX" )

    // create server - if one of this name does not already exist
    TFindServer findSatServer( KSatServerName );
    TFullName pathName;

    // Search for the server.
    if ( KErrNone != findSatServer.Next( pathName ) )
        {
        // We don't already exist.
        // Start scheduler and server.
        CSatSScheduler* scheduler = new ( ELeave ) CSatSScheduler;
        __ASSERT_ALWAYS( scheduler !=
            NULL, PanicServer( ESatSMainSchedulerError ) );

        CleanupStack::PushL( scheduler );
        CActiveScheduler::Install( scheduler );

        // Rename the thread.
        User::RenameThread( KSatServerName );

        // Create the server and connect to external interfaces.
        CSatSServer* server = CreateSatServerL();
        CleanupStack::PushL( server );

        // The scheduler needs access to the server instance.
        //lint -e{613} scheduler cannot be null, due assertion in creation.
        scheduler->SetServer( server );

        // Call Rendezvous to improve startup time
        RProcess::Rendezvous( KErrNone );

        LOG( SIMPLE,
            "SATSERVER:   Releasing ownership of SATSERVERMTX, Starting.." )
        serverStartMutex.Signal();

        // start fielding requests from clients
        CActiveScheduler::Start();

        // finished when the scheduler stops
        CleanupStack::PopAndDestroy( KInitServerPop ); // scheduler, server
        }
    else
        {
        LOG( SIMPLE,
            "SATSERVER:   Releasing ownership of SATSERVERMTX, Already started" )
        serverStartMutex.Signal();
        }
    serverStartMutex.Close();
    LOG( SIMPLE, "SATSERVER: InitServerL exiting" )
    }

// ======== GLOBAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// E32Main implements the executable entry function.
// Target type of the SATServer module is EXE.
// Creates a cleanup stack and runs the server.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;

    // Get a new clean-up stack.
    CTrapCleanup* cleanup = CTrapCleanup::New();

    // Initialize and run the server.
    TRAPD( error, InitServerL() );
    __ASSERT_ALWAYS( !error, User::Panic( KSatSInitError, error ) );

    // Destroy clean-up stack.
    delete cleanup;

    __UHEAP_MARKEND;
    // Not called unless server is already running. This server is always on
    LOG( SIMPLE, "SATSERVER: E32Main exiting" )
    return KErrAlreadyExists;
    }
