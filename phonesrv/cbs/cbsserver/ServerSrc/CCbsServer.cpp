/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsServer class 
*                member functions.
*   
*                This file contains the entry point for CbsServer.exe/dll.  
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include <barsc.h>

#include "CbsCommon.h"
#include "CCbsScheduler.h"
#include "CbsServerConstants.h"
#include "CbsServerPanic.h"
#include "CCbsServer.h"
#include "CCbsSession.h"
#include "CbsUtils.h"

#include "CCbsRecEtel.h"
#include "CCbsDbImp.H"

#include "CCbsDbImp.H"
#include "CCbsRecEtel.h"

#include "CCbsDbImpSettings.H"
#include "CCbsDbImpTopicList.h"
#include "CCbsReceiverHelper.h"
#include "CCbsSatRefresh.h"

#include <e32property.h>
#include "CbsServerInternalPSKeys.h"
#include "CCbsShutdownMonitor.h"
#include "CbsLogger.h"
#include "CbsServerUid.h"


// LOCAL FUNCTION PROTOTYPES
LOCAL_C void InitServerL();

// ==================== LOCAL FUNCTIONS ====================

// -----------------------------------------------------------------------------
// InitServerL
// Initializes and starts CBS Server.
// Creates the active scheduler and an instance of the server.
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void InitServerL()
    {
    CBSLOGSTRING("CBSSERVER: >>> InitServerL()");
    // perform init code under the cleanup stack
	User::RenameThread( KCbsServerName );
    
    // create server - if one of this name does not already exist
    TFindServer findCbsServer( KCbsServerName );
    TFullName pathName;

    RMutex serverStartMutex;
    TInt createErr( serverStartMutex.CreateGlobal( _L("CBSSERVERMTX") ) );
    if ( createErr )
        {
        TInt openErr( serverStartMutex.OpenGlobal( _L("CBSSERVERMTX") ) );
        User::LeaveIfError( openErr );
        }
    serverStartMutex.Wait();

    if ( findCbsServer.Next( pathName ) != KErrNone ) 
        {
        // start scheduler and server
        CCbsScheduler* scheduler = new (ELeave) CCbsScheduler;
        CleanupStack::PushL( scheduler );
        CActiveScheduler::Install( scheduler );

        CCbsServer* server = CCbsServer::NewL();
        CleanupStack::PushL( server );
        
        // The scheduler needs access to the server instance.
        scheduler->SetServer( server );
        
        // Set the key using Publish & Subscribe
        RProperty property;                       
        _LIT_SECURITY_POLICY_PASS( KCbsServerReadPolicy );
        _LIT_SECURITY_POLICY_S0( KCbsServerWritePolicy, KCbsServerUid3 );
                
        property.Define( KPSUidCellBroadcast, KCbsServerStarted, RProperty::EInt, KCbsServerReadPolicy, KCbsServerWritePolicy );
        CBSLOGSTRING("CBSSERVER: InitServerL(): Property defined.");
                
        property.Set( KPSUidCellBroadcast, KCbsServerStarted, 1 );
        CBSLOGSTRING("CBSSERVER: InitServerL(): Property set.");
                
        property.Close();

        serverStartMutex.Signal();
        serverStartMutex.Close();

        CBSLOGSTRING("CBSSERVER: InitServerL(): CbsServer created ok, starting active schduler.");

        RProcess::Rendezvous( KErrNone );
        
        // start fielding requests from clients
        CActiveScheduler::Start();
        CBSLOGSTRING("CBSSERVER: InitServerL(): CbsServer shutting down, destroying scheduler & server instances.");

        // finished when the scheduler stops
        CleanupStack::PopAndDestroy(2); // server, scheduler    
        }
    else
        {
        serverStartMutex.Signal();
        serverStartMutex.Close();
        }
    CBSLOGSTRING("CBSSERVER: <<< InitServerL()");
    }


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsServer::CCbsServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsServer::CCbsServer( 
    TInt aPriority )
    : CServer2( aPriority )
    {
    }

// -----------------------------------------------------------------------------
// CCbsServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsServer::ConstructL()
    {
    CBSLOGSTRING( "CBSSERVER: >>> CCbsServer::ConstructL()" );
    // Use PubSub to monitor shutdown event
    iShutdownMonitor = CCbsShutdownMonitor::NewL( *this );

    iReceiver = CCbsRecEtel::NewL();

    iDatabase = CCbsDbImp::NewL();

    iHelper = CCbsReceiverHelper::NewL( *iDatabase );

    iReceiver->SetInterface( iHelper );
    InitReceiverL();
    
    // Create the container index.
    iContainerIx = CObjectConIx::NewL();
    iContainer = NewContainerL();
    iIndex = CObjectIx::NewL();

	// Create SAT refresh listener
	TInt error( KErrNone );
	TRAP( error, iSatRefresh = CCbsSatRefresh::NewL( *this ));
    CBSLOGSTRING2( "CBSSERVER: CCbsServer::ConstructL(): CCbsSatRefresh::NewL error: %d", error );

    // Start the server
    if ( Start( KCbsServerName ) != KErrNone ) 
        {
        CbsServerPanic( ECbsServerStartFailed );
        }

    CBSLOGSTRING( "CBSSERVER: <<< CCbsServer::ConstructL()" );
    }

// -----------------------------------------------------------------------------
// CCbsServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsServer* CCbsServer::NewL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsServer::NewL()");
    CCbsServer* self = new ( ELeave ) CCbsServer( KCbsServerPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    CBSLOGSTRING("CBSSERVER: <<< CCbsServer::NewL()");
    return self;
    }

    
// Destructor
CCbsServer::~CCbsServer()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsServer::~CCbsServer()");
    // Note: it is important to delete in correct order
    
    // The correct order is: 
    //        1st:    sessions, subsessions and other server related objects
    //        2nd:    receiver 
    //        3rd:    database

    if ( iIndex )
        {
        delete iIndex; 
        iIndex = NULL;        
        }
    
    if ( iContainer )
        {
        if ( iContainerIx )
            {
            iContainerIx->Remove( iContainer );
            iContainer = NULL;
            }        
        }

    // Delete subsessions
    if ( iContainerIx )
        {
        delete iContainerIx;
        iContainerIx = NULL;
        }    

    if ( iSatRefresh )
        {
        delete iSatRefresh;
        iSatRefresh = NULL;
        }
        
    if ( iReceiver )
        {
        delete iReceiver;
        iReceiver = NULL;
        }	
    
    if ( iDatabase )
        {
        delete iDatabase;
        iDatabase = NULL;
        }
        
    if ( iHelper )
        {
        delete iHelper;
        iHelper = NULL;
        }
           
    if ( iShutdownMonitor )
        {
        delete iShutdownMonitor;
        iShutdownMonitor = NULL;
        }
            
    CBSLOGSTRING("CBSSERVER: <<< CCbsServer::~CCbsServer()");
    }

// -----------------------------------------------------------------------------
// CCbsServer::NewSessionL
// Creates a new session (an instance of CCbsSession).
//   
// The method is meant to be called by the Symbian OS server framework and 
// so it should not be used in any other case.
//  
// The method leaves if the version given as parameter and the server
// version differ.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSession2* CCbsServer::NewSessionL( 
    const TVersion& aVersion,
    const RMessage2& /*aMessage*/ ) const
    {
    TVersion currentVersion( KCbsServerVersionMajor,
                             KCbsServerVersionMinor,
                             KCbsServerVersionBuild );

    // Check the client version
    if ( !User::QueryVersionSupported( currentVersion, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    // Create and return a new session
    return CCbsSession::NewL( *( CCbsServer* ) this );
    }

// -----------------------------------------------------------------------------
// CCbsServer::NewContainerL
// Creates a new session object container.
//  
// It is the responsibility of the caller to delete the object 
// container, when it is no longer used.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CObjectCon* CCbsServer::NewContainerL()
    {
    // Create the new container.
    return iContainerIx->CreateL();
    }

// -----------------------------------------------------------------------------
// CCbsServer::Database
// Returns a reference to the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CCbsDbImp& CCbsServer::Database()
    {
    return *iDatabase;
    }

// -----------------------------------------------------------------------------
// CCbsServer::Receiver
// Returns a reference to the receiver.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsRecEtel& CCbsServer::Receiver()
    {
    return *iReceiver;
    }

// -----------------------------------------------------------------------------
// CCbsServer::TotalTopicsDetected
// Returns the total number of detected (via topic detection feature)
// topics and then clears the counter.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
TInt CCbsServer::TotalTopicsDetected()
    {
    TInt count( iHelper->TopicsDetected() );
    iHelper->ClearTopicsDetectedCounter();
    return count;
    }

// -----------------------------------------------------------------------------
// CCbsServer::PanicClient
// Panics the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsServer::PanicClient( 
    TInt aPanic ) const
    {
    __DEBUGGER();   // Debugger breakpoint in server's context.
    Message().Panic( KCbsServerName, aPanic );
    }

// -----------------------------------------------------------------------------
// CCbsServer::Shutdown
// Shuts down the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsServer::Shutdown()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsServer::Shutdown()");
    Cancel();
    CActiveScheduler::Stop();
    CBSLOGSTRING("CBSSERVER: <<< CCbsServer::Shutdown()");
    }

// -----------------------------------------------------------------------------
// CCbsServer::ContainerIndex
// Returns the container index.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CObjectConIx& CCbsServer::ContainerIndex()
    {
    return *iContainerIx;
    }

// -----------------------------------------------------------------------------
// CCbsServer::ServerMessage
// Returns the current IPC message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
const RMessage2 CCbsServer::ServerMessage() const
    {
    return Message();
    }

// -----------------------------------------------------------------------------
// CCbsServer::ReturnObjectByHandle
// Returns a subsession object matching the given handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsObject* CCbsServer::ReturnObjectByHandle( 
    TUint aHandle )
    {
    return ( CCbsObject* )iIndex->At( aHandle );
    }

// -----------------------------------------------------------------------------
// CCbsServer::DeleteObjectByHandle
// Deletes a subsession 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsServer::DeleteObjectByHandle( 
    TUint aHandle )
    {    
    // Find object and then delete it
    CCbsObject* object = ReturnObjectByHandle( aHandle );

    if ( object )
        {
        // Remove from index (and it will delete the object)
        iIndex->Remove( aHandle );
        }
    else
        {
        CbsServerPanic( EInvalidSessionObject );
        }
    }

// -----------------------------------------------------------------------------
// CCbsServer::Container
// Returns the object container
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CObjectCon& CCbsServer::Container()
    {
    return *iContainer;
    }

// -----------------------------------------------------------------------------
// CCbsServer::ObjectIx
// Returns the object index
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CObjectIx& CCbsServer::ObjectIx()
    {
    return *iIndex;
    }

// -----------------------------------------------------------------------------
// CCbsServer::InitReceiverL
// Initializes the receiver with values retrieved from the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsServer::InitReceiverL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsServer::InitReceiverL()");
    
    // DB leave trapped.
    TInt ignore( KErrNone );
    TRAP( ignore, iReceiver->LoadSimTopicsL() );
    CBSLOGSTRING2("CBSSERVER: CCbsServer::InitReceiverL(): LoadSimTopicsL() TRAPped error: %d", ignore);

    iReceiver->ApplyStateChangesL();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsServer::InitReceiverL()");    
    }
    
// -----------------------------------------------------------------------------
// CCbsServer::ReLoadSimTopicsL
// Initializes the receiver with values retrieved from the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsServer::ReLoadSimTopicsL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsServer::ReLoadSimTopicsL()");

    TInt ignore( KErrNone );
    
    TRAP( ignore, iReceiver->LoadSimTopicsL() );
    CBSLOGSTRING2("CBSSERVER: CCbsServer::ReLoadSimTopicsL(): LoadSimTopicsL() TRAPped error: %d", ignore);

    iReceiver->ApplyStateChangesL();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsServer::ReLoadSimTopicsL()");    
    }    

// ================= OTHER EXPORTED FUNCTIONS ==============

// -----------------------------------------------------------------------------
// E32Main
// E32Main implements the executable entry function.
// Creates a cleanup stack and runs the server. 
// Returns: Zero
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New(); 

    TRAPD( error, InitServerL() ); 

    __ASSERT_ALWAYS( !error, User::Panic( KCbsServerName, error) );
    delete cleanup; 
    __UHEAP_MARKEND;
    return 0; 
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  
