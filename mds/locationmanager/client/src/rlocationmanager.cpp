/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  An interface to Location Manager server
*
*/

#include <f32file.h>
#include <s32mem.h>
#include <data_caging_path_literals.hrh>

#include <rlocationmanager.h>
#include <locationeventdef.h>
#include "locationmanagerdefs.h"
#include "locationmanagerdebug.h"

// --------------------------------------------------------------------------
// LaunchServer
// Launches the server.
// --------------------------------------------------------------------------
//
TInt LaunchServer()
    {
    LOG( "RLocationManager::LaunchServer begin" );

    TFindServer findLocServer( KLocServerName );
    TFullName name;

    TInt result = findLocServer.Next( name );
    if ( result == KErrNone )
        {
        LOG( "StartServer() - Server already running" );
        
        // Server already running
        return KErrNone;
        }
    
    RProcess server;
    result = server.Create( KLocServerFileName, KNullDesC );   
    if ( result != KErrNone )
        {
        LOG1( "CreateServerProcess() - failed to create server process, error: %d", result );
        return result;
        }
 
    // Process created successfully
    TRequestStatus stat( KErrNone );
    server.Rendezvous( stat );
    
    if ( stat != KRequestPending )
        {
        LOG( "RLocationManager::LaunchServer Failed" );
        server.Kill( 0 );     // Abort startup.
        }
    else
        {
        server.Resume();    // Logon OK - start the server.
        }
    
    User::WaitForRequest( stat ); // wait for start or death
    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' wehich cannot be distinguished
    // from KErrNone
    result = ( server.ExitType() == EExitPanic ) ? KErrCommsBreak : stat.Int();
    server.Close();
    
    LOG( "RLocationManager::LaunchServer end" );
    
    return result;       
    }

// --------------------------------------------------------------------------
// RLocationManager::RLocationManager
// C++ Constructor
// --------------------------------------------------------------------------
//
EXPORT_C RLocationManager::RLocationManager()
    {
    }

// --------------------------------------------------------------------------
// RLocationManager::Connect
// Creates connection to server
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationManager::Connect()
    {
    LOG( "RLocationManager::Connect(), begin" );
      
    TInt error = LaunchServer();

    if ( error == KErrNone || error == KErrAlreadyExists )
        {
        error = CreateSession( KLocServerName, Version(), KSessionSlotCount );
        }
    
    LOG( "RLocationManager::Connect(), end" );

    return error;
    }
    
// --------------------------------------------------------------------------
// RLocationManager::Close
// --------------------------------------------------------------------------
//      
EXPORT_C void RLocationManager::Close()
    {
    LOG( "RLocationManager::Close(), begin" );
    // close session    
    RSessionBase::Close();
    LOG( "RLocationManager::Close(), end" );
    }

// --------------------------------------------------------------------------
// RLocationManager::Version
// Returns the version of Location Manager.
// --------------------------------------------------------------------------
//      
TVersion RLocationManager::Version() const
    {
    TVersion version( KLocationManagerServerMajor, 
                              KLocationManagerServerMinor, 
                              KLocationManagerServerBuild );
    return version;
    }

// --------------------------------------------------------------------------
// RLocationManager::CompleteRequest
// Completes an asynchronous request with an error code.
// --------------------------------------------------------------------------
//      
void RLocationManager::CompleteRequest( TRequestStatus& aStatus, TInt aError )
	{
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, aError );
	}


//End of File
