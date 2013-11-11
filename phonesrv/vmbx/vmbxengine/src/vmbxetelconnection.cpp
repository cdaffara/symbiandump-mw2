/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CVmbxETelConnection class
*
*/


// INCLUDE FILES
#include <mmtsy_names.h>

#include "vmbxlogger.h"
#include "vmbxetelconnection.h"

// CONSTANTS
// ETel connections attemps
const TInt KTriesToConnectServer( 7 );
// time between ETel connections attemps
const TInt KTimeBeforeRetryingServerConnection( 100000 );
// Index to the first phone info
const TInt KFirstPhoneIndex = 0;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxETelConnection::CVmbxETelConnection
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxETelConnection::CVmbxETelConnection()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::CVmbxETelConnection <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxETelConnection::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxETelConnection::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::ConstructL =>" );
    TInt errorCode( KErrNone );
    TInt thisTry( 0 );
    RTelServer::TPhoneInfo info;
    // connect to ETel server
    while ( ( errorCode = iTelServer.Connect() ) != KErrNone &&
            ( thisTry++ ) <= KTriesToConnectServer )
        {
        User::After( KTimeBeforeRetryingServerConnection );
        }
    VMBLOGSTRING2( "VMBX: CVmbxETelConnection::ConstructL: errorCode %I", 
                                                    errorCode );
    User::LeaveIfError( errorCode );

    // load TSY
    errorCode = iTelServer.LoadPhoneModule( KMmTsyModuleName );
    VMBLOGSTRING2( "VMBX: CVmbxETelConnection::ConstructL: err%I", errorCode );
    if ( KErrNone != errorCode && KErrAlreadyExists != errorCode )
        {
        User::Leave( errorCode );
        }

    // This enables extended errors from low level which are needed
    // to get detailed information when saving contacts
    TInt errExtrnded = iTelServer.SetExtendedErrorGranularity(
                                RTelServer::EErrorExtended );
    VMBLOGSTRING2( "VMBX: CVmbxETelConnection::ConstructL: errExtrnded %I", 
                                                        errExtrnded );
    User::LeaveIfError( errExtrnded );
    // get info about default phone
    TInt errGetInfo = iTelServer.GetPhoneInfo( KFirstPhoneIndex, info );
    VMBLOGSTRING2( "VMBX: CVmbxETelConnection::ConstructL: errGetInfo %I", 
                                                        errGetInfo );
    User::LeaveIfError( errGetInfo );

    TInt errInfoName = iPhone.Open( iTelServer, info.iName );
    VMBLOGSTRING2( "VMBX: CVmbxETelConnection::ConstructL: errInfoName %I", 
                                                        errInfoName );
    User::LeaveIfError( errInfoName );

    VMBLOGSTRING( "VMBX: CVmbxETelConnection::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxETelConnection::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxETelConnection* CVmbxETelConnection::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::NewL =>" );
    CVmbxETelConnection* self = new( ELeave ) CVmbxETelConnection();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxETelConnection::~CVmbxETelConnection()
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxETelConnection::~CVmbxETelConnection()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::~CVmbxETelConnection =>" );
    // close ETel connection
    iPhone.Close();
    iTelServer.Close();
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::~CVmbxETelConnection <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxETelConnection::TelServer
// Returns a reference to the ETel RTelServer
// ---------------------------------------------------------------------------
//
RTelServer& CVmbxETelConnection::TelServer()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::TelServer <=>" );
    return iTelServer;
    }

// ---------------------------------------------------------------------------
// CVmbxETelConnection::Phone
// Returns a reference to the ETel RMobilePhone
// ---------------------------------------------------------------------------
//
RMobilePhone& CVmbxETelConnection::Phone()
    {
    VMBLOGSTRING( "VMBX: CVmbxETelConnection::Phone <=>" );
    return iPhone;
    }

// End of file
