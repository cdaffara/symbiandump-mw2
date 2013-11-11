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
* Description:  Harvester Client implementation*
*/

#include <e32property.h> 

#include "harvesterclient.h"
#include "harvestercommon.h"
#include "harvesterrequestqueue.h"
#include "harvestereventobserverao.h"
#include "harvesterlog.h"
#include "harvesterclientao.h"
#include "harvestersessionwatcher.h"
#include "mdsutils.h"
#include "harvesterrequestactive.h"
#include "mdscommoninternal.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesterclientTraces.h"
#endif

/** @var Message slots */
const TInt KDefaultMessageSlots = -1;  // Global pool

/* Server name */
_LIT( KHarvesterServerName, "HarvesterServer" );

/* Harvester Server process location */
_LIT( KHarvesterServerExe, "harvesterserver.exe" );

// FUNCTION PROTOTYPES
static TInt StartServer();
static TInt CreateServerProcess();


// ----------------------------------------------------------------------------------------
// RHarvesterClient
// ----------------------------------------------------------------------------------------
//
EXPORT_C RHarvesterClient::RHarvesterClient() : RSessionBase() 
    {
    WRITELOG( "RHarvesterClient::RHarvesterClient() - Constructor" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_RHARVESTERCLIENT, "RHarvesterClient::RHarvesterClient" );
    
    iObserver = NULL;
    iHEO = NULL;
    iRequestQueue = NULL;
    iHarvesterClientAO = NULL;
    iSessionWatcher = NULL;
    }

// ----------------------------------------------------------------------------------------
// Connect
// ----------------------------------------------------------------------------------------
//
EXPORT_C TInt RHarvesterClient::Connect()
    {
    WRITELOG( "RHarvesterClient::Connect()" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_CONNECT, "RHarvesterClient::Connect" );
    
    RProperty property;
    const TInt error( property.Attach( KHarvesterPSShutdown, KShutdown, EOwnerThread ) );
    TInt value = 0;
    property.Get( value );
    property.Close();

    if ( error != KErrNone || value > KErrNone )
        {
        return KErrLocked;
        }
    
    if( iHarvesterClientAO )
    	{
    	return KErrAlreadyExists;
    	}
    
    TRAPD( err, iHarvesterClientAO = CHarvesterClientAO::NewL(*this) );
    if ( err != KErrNone )
        {
        WRITELOG( "RHarvesterClient::RHarvesterClient() - Couldn't create active object" );
        return err;
        }
    
    // request processor
    TRAP( err, iRequestQueue = CHarvesterRequestQueue::NewL() )
        {
        if ( err != KErrNone )
            {
            WRITELOG( "RHarvesterClient::RHarvesterClient() - Couldn't create harvester request queue" );
            delete iHarvesterClientAO;
            iHarvesterClientAO = NULL;
            return err;
            }
        }
    
    err = ::StartServer();

    if ( err == KErrNone || err == KErrAlreadyExists )
        {
        WRITELOG( "RHarvesterClient::Connect() - creating session" );
        err = CreateSession( KHarvesterServerName, Version(), KDefaultMessageSlots );
        }
    else
        {
        WRITELOG1( "RHarvesterClient::Connect() - Server is not running or could not be started, error &d", err );
        delete iRequestQueue;
        iRequestQueue = NULL;
        delete iHarvesterClientAO;
        iHarvesterClientAO = NULL;
        return err;
        }

    if( err != KErrNone )
        {
        WRITELOG1( "RHarvesterClient::Connect() - Session creation failed, error &d", err );
        delete iRequestQueue;
        iRequestQueue = NULL;
        delete iHarvesterClientAO;
        iHarvesterClientAO = NULL;    
        }
    
    iHEO = NULL;
    
    iSessionWatcher = NULL;
    
    WRITELOG( "RHarvesterClient::Connect() - end" );
    
    return err;
    }

// ----------------------------------------------------------------------------------------
// Pause
// ----------------------------------------------------------------------------------------
//
EXPORT_C TInt RHarvesterClient::Pause()
    {
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_PAUSE, "RHarvesterClient::Pause" );
    
    WRITELOG( "RHarvesterClient::Pause() -  sending command EPauseHarvester" );
    if( iHandle )
    	{
    	return SendReceive( EPauseHarvester );
       	}
    return KErrDisconnected;
    }

// ----------------------------------------------------------------------------------------
// Resume
// ----------------------------------------------------------------------------------------
//
EXPORT_C TInt RHarvesterClient::Resume()
    {
    WRITELOG( "RHarvesterClient::Resume() -  sending command EResumeHarvester" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_RESUME, "RHarvesterClient::Resume" );
    
    if( iHandle )
    	{
    	return SendReceive( EResumeHarvester );
    	}
    return KErrDisconnected;
    }

// ----------------------------------------------------------------------------------------
// Close
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::Close()
    {
    WRITELOG( "RHarvesterClient::Close()" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_CLOSE, "RHarvesterClient::Close" );
    
    delete iSessionWatcher;
    iSessionWatcher = NULL;
    
    WRITELOG( "RHarvesterClient::Close() - cancelling pending harvesting requests" );
    if( iRequestQueue && iRequestQueue->RequestsPending() )
        {
        iRequestQueue->Cancel();
        }
    
    WRITELOG( "RHarvesterClient::Close() - removing harvester client observer" );
    if( iObserver )
        {
        RemoveObserver( iObserver );
        }
    UnregisterHarvestComplete();
    
    WRITELOG( "RHarvesterClient::Close() - UnregisterHarvest done" );
    
    WRITELOG( "RHarvesterClient::Close() - forcing remaining harvesting requests" );
    if( iRequestQueue && iRequestQueue->RequestsPending() )
        {
        WRITELOG( "RHarvesterClient::Close() - Pending harvesting requests detected -> ForceHarvest" );
        iRequestQueue->ForceRequests();
        }
    
    WRITELOG( "RHarvesterClient::Close() - ForceHarvest done" );
    
    delete iRequestQueue;
    iRequestQueue = NULL;
    
    delete iHEO;
    iHEO = NULL;
    
    WRITELOG( "RHarvesterClient::Close() - deleting harvester client AO" );
    delete iHarvesterClientAO;
    iHarvesterClientAO = NULL;
    
    WRITELOG( "RHarvesterClient::Close() - Closing session" );
    
    RSessionBase::Close();
    }

// ----------------------------------------------------------------------------------------
// SetObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::SetObserver( MHarvestObserver* aObserver )
    {
    WRITELOG( "RHarvesterClient::SetObserver()" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_SETOBSERVER, "RHarvesterClient::SetObserver" );

    if ( iHarvesterClientAO )
        {
       iHarvesterClientAO->SetObserver( aObserver );
       }
	iObserver = aObserver;
    }

// ----------------------------------------------------------------------------------------
// RemoveObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::RemoveObserver( MHarvestObserver* aObserver )
    {
    WRITELOG( "RHarvesterClient::RemoveObserver()" );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_REMOVEOBSERVER, "RHarvesterClient::RemoveObserver" );
    
	if ( aObserver == iObserver && iObserver )
		{
		WRITELOG( "RHarvesterClient::RemoveObserver() - deleting observer" );
		
	    if ( iHarvesterClientAO )
	        {
	        WRITELOG( "RHarvesterClient::RemoveObserver() - calling iHarvesterClientAO->RemoveObserver" );
	        iHarvesterClientAO->RemoveObserver( aObserver );
	        }
		
		iObserver = NULL;
		}
    }

// ----------------------------------------------------------------------------------------
// AddHarvesterEventObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C TInt RHarvesterClient::AddHarvesterEventObserver( 
	MHarvesterEventObserver& aHarvesterEventObserver, 
	TInt aHEObserverType,
	TInt aEventInterval )
	{
	TInt err( 0 );
	if( !iHEO )
	    {
        TRAP( err, iHEO = CHarvesterEventObserverAO::NewL( *this ) );
        if ( err != KErrNone )
            {
            WRITELOG( "RHarvesterClient::RHarvesterClient() - Couldn't create harvester event observer" );
            return err;
            }
	    }
	
	TRAP(err, iHEO->AddHarvesterEventObserverL(
			aHarvesterEventObserver,
			aHEObserverType,
			aEventInterval ));
	
	return err;
	}

// ----------------------------------------------------------------------------------------
// RemoveObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C TInt RHarvesterClient::RemoveHarvesterEventObserver( MHarvesterEventObserver& aHarvesterEventObserver )
	{
    if( iHEO )
          {
          TRAPD( err, iHEO->RemoveHarvesterEventObserverL( aHarvesterEventObserver ) );
          return err;
          }
	
	return KErrNone;
	}

// ----------------------------------------------------------------------------------------
// HarvestFile
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::HarvestFile( const TDesC& aURI, RArray<TItemId>& aAlbumIds, TBool aAddLocation )
    {
    WRITELOG1( "RHarvesterClient::HarvestFile() - file %S", &aURI );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_HARVESTFILE, "RHarvesterClient::HarvestFile" );
    
    HBufC8* paramBuf = NULL;
    TRAPD( err, paramBuf = SerializeArrayL( aAlbumIds ) );
    if ( err )
    	{
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot create serialized array, error: %d", err );
        if( iObserver )
            {
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), err );  
            }
    	return;
    	}
        
    CHarvesterRequestActive* harvestFileActive( NULL );
    TRAP( err, harvestFileActive = CHarvesterRequestActive::NewL( *this, iObserver, (TInt)EHarvestFile, aURI, 
                                                                                                   paramBuf, aAddLocation, iRequestQueue ) );
    if( err )
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot create harvesting request, error: %d", err );
        if( iObserver )
            {
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), err );  
            }
        return;
        }

    // send actually harvest request to server
    if( iHandle )
        {
        TRAP( err, iRequestQueue->AddRequestL( harvestFileActive ) );
        if( err && iObserver)
            {
            WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrServerBusy );
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), KErrServerBusy );  
            delete harvestFileActive;
            harvestFileActive = NULL;
            }
        else if( err )
            {
            WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrServerBusy );
            delete harvestFileActive;
            harvestFileActive = NULL;
            }
        else
            {
            WRITELOG( "RHarvesterClient::HarvestFile() - harvesting request added to queue" );
            iRequestQueue->Process();
            }
        }
    else if( iObserver )
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrDisconnected );
        iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), KErrDisconnected );  
        delete harvestFileActive;
        harvestFileActive = NULL;
        }
    else
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrDisconnected );
        delete harvestFileActive;
        harvestFileActive = NULL;
        }
    WRITELOG( "RHarvesterClient::HarvestFile() - end" );
    }

// ----------------------------------------------------------------------------------------
// HarvestFileWithUID
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::HarvestFileWithUID( const TDesC& aURI, 
                                                                                         RArray<TItemId>& aAlbumIds, 
                                                                                         TBool aAddLocation,
                                                                                         TUid /*aUid*/ )
    {
    WRITELOG1( "RHarvesterClient::HarvestFileWithUID() - file %S", &aURI );
    OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_HARVESTFILEWITHUID, "RHarvesterClient::HarvestFileWithUID" );
    
    
    HBufC8* paramBuf = NULL;
    TRAPD( err, paramBuf = SerializeArrayL( aAlbumIds ) );
    if ( err )
        {
        WRITELOG1( "RHarvesterClient::HarvestFileWithUID() - cannot create serialized array, error: %d", err );
        if( iObserver )
            {
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), err );  
            }
        return;
        }

    CHarvesterRequestActive* harvestFileActive( NULL );
    TRAP( err, harvestFileActive = CHarvesterRequestActive::NewL( *this, iObserver, (TInt)EHarvestFile, aURI, 
                                                                                                   paramBuf, aAddLocation, iRequestQueue ) );
    if( err )
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot create harvesting request, error: %d", err );
        if( iObserver )
            {
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), err );  
            }
        return;
        }

    // send actually harvest request to server
    if( iHandle )
        {
        TRAP( err, iRequestQueue->AddRequestL( harvestFileActive ) );
        if( err && iObserver)
            {
            WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrServerBusy );
            iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), KErrServerBusy );  
            delete harvestFileActive;
            harvestFileActive = NULL;
            }
        else if( err )
            {
            WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrServerBusy );
            delete harvestFileActive;
            harvestFileActive = NULL;
            }
        else
            {
            iRequestQueue->Process();
            }
        }
    else if( iObserver )
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrDisconnected );
        iObserver->HarvestingComplete( const_cast<TDesC&>(aURI), KErrDisconnected );  
        delete harvestFileActive;
        harvestFileActive = NULL;
        }
    else
        {
        WRITELOG1( "RHarvesterClient::HarvestFile() - cannot not send harvest request to server, error: %d", KErrDisconnected );
        delete harvestFileActive;
        harvestFileActive = NULL;
        }
    }

// ----------------------------------------------------------------------------------------
// AddSessionObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::AddSessionObserverL( MHarvesterSessionObserver& aObserver  )
    {
    if( iSessionWatcher )
        {
        delete iSessionWatcher;
        iSessionWatcher = NULL;
        }
    iSessionWatcher = CHarvesterSessionWatcher::NewL( aObserver );
    }

// ----------------------------------------------------------------------------------------
// RemoveSessionObserver
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RHarvesterClient::RemoveSessionObserver()
    {
    if( iSessionWatcher )
        {
        delete iSessionWatcher;
        iSessionWatcher = NULL;
        }
    }

// ----------------------------------------------------------------------------------------
// RegisterHarvestComplete
// ----------------------------------------------------------------------------------------
//
void RHarvesterClient::RegisterHarvestComplete(TDes& aURI, TRequestStatus& aStatus)
	{
	TIpcArgs ipcArgs( &aURI );
	OstTrace0( TRACE_NORMAL, RHARVESTERCLIENT_REGISTERHARVESTCOMPLETE, "RHarvesterClient::RegisterHarvestComplete" );
	
	if( !iHandle )
		{
		return;
		}
	SendReceive( ERegisterHarvestComplete, ipcArgs, aStatus);
	}


// ----------------------------------------------------------------------------------------
// UnregisterHarvestComplete
// ----------------------------------------------------------------------------------------
//
void RHarvesterClient::UnregisterHarvestComplete()
	{
	if( !iHandle )
		{
		return;
		}	
	
	SendReceive( EUnregisterHarvestComplete );
	}

// ----------------------------------------------------------------------------------------
// HarvestFile
// ----------------------------------------------------------------------------------------
//
void RHarvesterClient::HarvestFile( TInt& aService, TIpcArgs& aArgs, TRequestStatus& aStatus )
    {
    // send to server harvesting complete observer
    if( iObserver && iHarvesterClientAO )
        {
        iHarvesterClientAO->Active();
        }
    SendReceive( aService, aArgs, aStatus );
    }

// ----------------------------------------------------------------------------------------
// ForceHarvestFile
// ----------------------------------------------------------------------------------------
//
void RHarvesterClient::ForceHarvestFile( TInt& aService, TIpcArgs& aArgs )
    {
    SendReceive( aService, aArgs );
    }

// ----------------------------------------------------------------------------------------
// Version
// ----------------------------------------------------------------------------------------
//
TVersion RHarvesterClient::Version() const
    {
    WRITELOG( "RHarvesterClient::Version()" );
        
    TVersion version( KHarvesterServerMajorVersion, KHarvesterServerMinorVersion,
                              KHarvesterServerBuildVersion );
    return version;
    }

// ----------------------------------------------------------------------------------------
// StartServer
// ----------------------------------------------------------------------------------------
//
static TInt StartServer()
    {
    WRITELOG( "StartServer() - begin" );
    OstTrace0( TRACE_NORMAL, _STARTSERVER, "::StartServer" );    
    
    TFindServer findHarvesterServer( KHarvesterServerName );
    TFullName name;

    TInt result = findHarvesterServer.Next( name );
    if ( result == KErrNone )
        {
        WRITELOG( "StartServer() - Server allready running" );
        
        // Server already running
        return KErrNone;
        }
#ifdef _DEBUG
    else
        {
        if( result == KErrNotFound )
            {
            WRITELOG( "StartServer() - server not found running" );
            }
        else
            {
            WRITELOG1( "StartServer() error - error code: %d", result );
            }
        }
#endif
    
    result = CreateServerProcess();
    if ( result != KErrNone )
        {
        WRITELOG1( "StartServer() - creating process failed, error: %d", result );
        }
    
    WRITELOG( "StartServer() - end" ); 
    return result;
    }

// ----------------------------------------------------------------------------------------
// CreateServerProcess
// ----------------------------------------------------------------------------------------
//
static TInt CreateServerProcess()
    {
    WRITELOG( "CreateServerProcess() - begin" );
    OstTrace0( TRACE_NORMAL, _CREATESERVERPROCESS, "::CreateServerProcess" );
    
    RProcess server;
    TInt result = server.Create( KHarvesterServerExe, KNullDesC );   
    if ( result != KErrNone )
        {
        WRITELOG1( "CreateServerProcess() - failed to create server process, error: %d", result );
        return result;
        }
 
    // Process created successfully
    TRequestStatus stat( 0 );
    server.Rendezvous( stat );
    
    if ( stat != KRequestPending )
        {
        server.Kill( 0 );     // abort startup
        }
    else
        {
        server.Resume();    // logon OK - start the server
        }        
    
    User::WaitForRequest( stat ); // wait for start or death
    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' wehich cannot be distinguished
    // from KErrNone
    result = ( server.ExitType() == EExitPanic ) ? KErrCommsBreak : stat.Int();
    server.Close();
    
    WRITELOG( "CreateServerProcess() - end" );
    
    return result;
    }

// End of file

