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
* Description:  Session class to connect SatServer.
*
*/


#include    <rsatsession.h>
#include    "SatSOpcodes.h"
#include    "SatServerFactory.h"
#include    "SatLog.h"

_LIT( KSatSessionMtx, "SATSESSIONMTX" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RSatSession::RSatSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSatSession::RSatSession() :
    RSessionBase()
    {
    LOG( SIMPLE, "SATCLIENT: RSatSession::RSatSession calling-exiting" )
    }

// -----------------------------------------------------------------------------
// RSatSession::ConnectL
// Connects to SatServer.
// -----------------------------------------------------------------------------
//
EXPORT_C void RSatSession::ConnectL()
    {
    LOG( SIMPLE, "SATCLIENT: RSatSession::ConnectL calling" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatSession::ConnectL UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif

    RMutex sessionStartMutex;
    TInt createErr( sessionStartMutex.CreateGlobal( KSatSessionMtx ) );
    if ( createErr )
        {
        TInt openErr( sessionStartMutex.OpenGlobal( KSatSessionMtx ) );
        User::LeaveIfError( openErr );
        LOG( SIMPLE, "SATCLIENT:   Opened SATSESSIONMTX" )
        }

    LOG( SIMPLE, "SATCLIENT:   Asking ownership of SATSESSIONMTX" )
    sessionStartMutex.Wait();
    LOG( SIMPLE, "SATCLIENT:   Got ownership of SATSESSIONMTX" )

    // Create new session for SatServer.
    TInt error( CreateSession( KSatServerName, Version(), KSatMessageSlots ) );

    // Server may not be fully started, so let's retry after synchronization.
    if ( KErrNone != error )
        {
        LOG2( SIMPLE,
              "SATCLIENT: RSatSession::ConnectL CreateSession %d", error )

        // Starts a new process, loading SAT Server.
        TBuf<1> arguments;
        RProcess satProcess;

        if ( KErrNone == satProcess.Create( KSatServerNameAndPath, arguments ) )
            {
            TRequestStatus rendezvousStatus;

            satProcess.Rendezvous( rendezvousStatus );
            satProcess.Resume();
            User::WaitForRequest( rendezvousStatus );
            LOG2( SIMPLE,
                  "SATCLIENT: RSatSession::ConnectL Rendezvous %d",
                   rendezvousStatus.Int() )
            satProcess.Close();
            }

        // Retry session creation.
        error = CreateSession( KSatServerName,
                               Version(), KSatMessageSlots );
        LOG2( DETAILED,
              "SATCLIENT: RSatSession::ConnectL CreateSession %d",
              error )
        }

    LOG( SIMPLE, "SATCLIENT:   Releasing ownership of SATSESSIONMTX" )
    sessionStartMutex.Signal();
    sessionStartMutex.Close();
    User::LeaveIfError( error );

    LOG( SIMPLE, "SATCLIENT: RSatSession::ConnectL exiting" )
    }

// -----------------------------------------------------------------------------
// RSatSession::Version
// Returns the version of this API.
// -----------------------------------------------------------------------------
//
EXPORT_C TVersion RSatSession::Version() const
    {
    LOG( SIMPLE, "SATCLIENT: RSatSession::Version calling-exiting" )
    
    #ifdef ENABLE_SAT_LOGGING
    RProcess test;
    LOG2( SIMPLE, 
    "SATCLIENT: RSatSession::Version UID of calling process: 0x%x",
    test.SecureId().iId )
    test.Close();
    #endif
    
    return TVersion(
        KSatServerMajorVersionNumber,
        KSatServerMinorVersionNumber,
        KSatServerBuildVersionNumber );
    }
