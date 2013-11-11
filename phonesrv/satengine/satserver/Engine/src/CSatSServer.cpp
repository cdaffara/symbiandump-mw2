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
* Description:  Manages the sessions.
*
*/



// INCLUDE FILES
#include    "CSatSServer.h"
#include    "SatSOpcodes.h"
#include    "CSatCommandContainer.h"
#include    "CSatSSession.h"
#include    "SatLog.h"
#include    "SatServerFactory.h"
#include    "CSatSUiClientHandler.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatSServer::CSatSServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSServer::CSatSServer() :
CPolicyServer( EPriorityHigh, satServerPolicy, ESharableSessions )
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::CSatSServer calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSServer::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::ConstructL calling" )

    iObjectConIx = CObjectConIx::NewL();
    iUiHandler = CSatSUiClientHandler::NewL( *this, iEventMediator );
    iCmdContainer = CSatCommandContainer::NewL(
        *this, iEventMediator, *iUiHandler );

    iUiHandler->SetUtils( iCmdContainer );

    const TInt err( Start( KSatServerName ) );
    __ASSERT_ALWAYS( err == KErrNone, PanicSatEngine( ESatSStartServer ) );

    LOG( SIMPLE, "SATENGINE: CSatSServer::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSServer* CSatSServer::NewL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::NewL calling" )
    CSatSServer* self = new( ELeave ) CSatSServer();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSServer::NewL exiting" )
    return self;
    }

// Destructor
CSatSServer::~CSatSServer()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::~CSatSServer calling" )

    iEventMediator.Reset();

    iRefreshSubSessions.Close();
    delete iCmdContainer;
    delete iUiHandler;
    delete iObjectConIx;

    LOG( SIMPLE, "SATENGINE: CSatSServer::~CSatSServer exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::CompleteMessage
// Completes current message from client with an error code.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSServer::CompleteMessage(
    TInt aError ) const// Error code, which is used to complete the message.
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::CompleteMessage calling" )

    Message().Complete( aError );

    LOG( SIMPLE, "SATENGINE: CSatSServer::CompleteMessage exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::CreateNewContainerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
CObjectCon* CSatSServer::CreateNewContainerL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::CreateNewContainerL calling" )

    CObjectCon* objCon = iObjectConIx->CreateL();

    LOG( SIMPLE, "SATENGINE: CSatSServer::CreateNewContainerL exiting" )
    return objCon;
    }

// -----------------------------------------------------------------------------
// CSatSServer::AddL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CSatSServer::AddL( const MSatRefreshSubSession* aSubSession )
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::AddL calling" )

    User::LeaveIfError( iRefreshSubSessions.Append( aSubSession ) );

    LOG( SIMPLE, "SATENGINE: CSatSServer::AddL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::Remove
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CSatSServer::Remove( const MSatRefreshSubSession* aSubSession )
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::Remove calling" )
    const TInt index( iRefreshSubSessions.Find( aSubSession ) );

    if ( KErrNotFound != index )
        {
        LOG( SIMPLE, "SATENGINE: CSatSServer::Remove KErrNotFound != index" )
        iRefreshSubSessions.Remove( index );
        }

    LOG( SIMPLE, "SATENGINE: CSatSServer::Remove exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSServer::NewSessionL
// Creates new session if the clients version number matches with server
// version number.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSession2* CSatSServer::NewSessionL(
    const TVersion& aVersion, // Clients version of the server
    const RMessage2& /*aMessage*/ ) const // Message
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::NewSessionL calling" )

    // Check we're the right version.
    TVersion satVersionInformation( KSatServerMajorVersionNumber,
        KSatServerMinorVersionNumber,KSatServerBuildVersionNumber );

    if ( !User::QueryVersionSupported( satVersionInformation, aVersion ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatSServer::NewSessionL KErrNotSupported" )
        User::Leave( KErrNotSupported );
        }

    // Create a new session.
    CSatSSession* session =
        CSatSSession::NewL( const_cast<CSatSServer*>( this ),
            const_cast<TSatEventMediator&>( iEventMediator ),
            const_cast<MSatApi&>( iCmdContainer->USatAPI() ),
            *iUiHandler );

    LOG( SIMPLE, "SATENGINE: CSatSServer::NewSessionL exiting" )
    return session;
    }

// -----------------------------------------------------------------------------
// CSatSServer::UiSession
// Getter for ui session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatUiSession* CSatSServer::UiSession()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::UiSession calling" )
    MSatUiSession* result = NULL;

    //lint -e{1757} Post increment acceptable here.
    for ( iSessionIter.SetToFirst();
          iSessionIter.operator CSession2*() && ( !result );
          iSessionIter++ )
        {
        CSatSSession* session =
            static_cast<CSatSSession*>( iSessionIter.operator CSession2*() );

        // Has session ui subsession
        result = session->UiSubSession();
        }

    if ( NULL != result )
        {
        LOG( SIMPLE,
            "SATENGINE: CSatSServer::UiSession exiting (Ui session found)" )
        }
    else
        {
        LOG( SIMPLE,
            "SATENGINE: CSatSServer::UiSession exiting (Ui session not found)" )
        }
    LOG( SIMPLE, "SATENGINE: CSatSServer::UiSession exiting" )
    return result;
    }

// -----------------------------------------------------------------------------
// CSatSServer::RefreshSubSessions
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const RPointerArray<MSatRefreshSubSession>&
    CSatSServer::RefreshSubSessions()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::RefreshSubSessions calling-exiting" )
    return iRefreshSubSessions;
    }

// -----------------------------------------------------------------------------
// CSatSServer::SatUtils
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatUtils* CSatSServer::SatUtils()
    {
    LOG( SIMPLE, "SATENGINE: CSatSServer::SatUtils calling-exiting" )
    return iCmdContainer;
    }

//  End of File
