/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     Main class
*
*/


#include "AlwaysOnlineManagerClient.h"
#include "AlwaysOnlineManagerLogging.h"
#include "AlwaysOnlineManagerStartupSignaller.h"
#include "AlwaysOnlineManagerServer.h"
#include "AOActiveScheduler.h"

// ---------------------------------------------------------------------------
// RAlwaysOnlineClientSession::RunServerL()
// Called from AlwaysOnlineStarterApp
// ---------------------------------------------------------------------------
//
EXPORT_C void RunServerL()
    {
    KAOMANAGER_LOGGER_FN1("RunServerL()");

    //first thing to do, rename the thread. If construct fails, 
    //it is easier to debug when it's been named.
    User::LeaveIfError( User::RenameThread( KAlwaysOnlineManagerServerName ) );
    
    
    // Create and install the active scheduler we need
	CAOActiveScheduler* AOScheduler = new (ELeave) CAOActiveScheduler( );
    CleanupStack::PushL( AOScheduler );
	CActiveScheduler::Install( AOScheduler );
    
    CAlwaysOnlineManagerServer* AOserver = CAlwaysOnlineManagerServer::NewL();
    CleanupStack::PushL( AOserver );
    
    // set AOserver object to AOActiveScheduler
    AOScheduler->SetServerRef( AOserver );
    
    // Rendezvous with starter
    RProcess::Rendezvous( KErrNone );
    
    //
    // Ready to run
    CActiveScheduler::Start();
    //
    // Cleanup the server and scheduler
    CleanupStack::PopAndDestroy( 2, AOScheduler );	// AOserver
    KAOMANAGER_LOGGER_FN2("RunServerL()");
    }


// ---------------------------------------------------------------------------
// StartThread
// ---------------------------------------------------------------------------
//
TInt StartThread()
    {
    KAOMANAGER_LOGGER_FN1("StartThread");

	TInt ret = KErrNone;
	TRequestStatus startedRequestStatus;
	TOAManagerStartupSignaller start(startedRequestStatus);

    // check server not already started
    TFindServer findAlwaysOnlineServer( KAlwaysOnlineManagerServerName );
    TFullName name;
    if (findAlwaysOnlineServer.Next(name)==KErrNone)
        { // found server already - bad news
        KAOMANAGER_LOGGER_FN2("StartThread1");
        return KErrAlreadyExists;
        }
    const TUid KAlwaysOnlineManagerServerUid = {0x101F85EB};
    const TUidType serverUid( KNullUid,KNullUid,KAlwaysOnlineManagerServerUid );

        // We are on target HW.
        RProcess server;
        ret =server.Create(
            KAlwaysOnlineManagerServerName, // Full path to BT server
            start.AsCommand(), // Descriptor of parameters
            serverUid);                // Triplet UID of executable

	// Did we manage to create the thread/process?
	if	(ret != KErrNone)
        {
        KAOMANAGER_LOGGER_FN2("StartThread4");
		return ret;
        }

	// Wait to see if the thread/process died during construction
	TRequestStatus serverDiedRequestStatus;
	server.Logon(serverDiedRequestStatus);
	
	if	(serverDiedRequestStatus != KRequestPending)
		{
		//
		// Logon failed - server is not yet running, so cannot 
		// have terminated
		//

		// Eat signal
		User::WaitForRequest(serverDiedRequestStatus);	// CSI: 94 # This is server side context
		
		// Abort startup
		server.Kill(KErrNone);
		server.Close();

        KAOMANAGER_LOGGER_FN2("StartThread5");
		return serverDiedRequestStatus.Int();
		}

	// Logon OK - start the server
	server.Resume();

	// Wait for either the server to start or for it to die
	User::WaitForRequest(startedRequestStatus, serverDiedRequestStatus);	// CSI: 94 # This is server side context
	if	(startedRequestStatus == KRequestPending)
		{
		// Server has died - never made it to the startup signal
		server.Close();
        KAOMANAGER_LOGGER_FN2("StartThread6");
		return serverDiedRequestStatus.Int();
		}

	//
	// Server started (at last). Cancel and consume the death-notification
	// before reporting success
	//
	server.LogonCancel(serverDiedRequestStatus);
	server.Close();

	// Eat the signal (from the cancel)
	User::WaitForRequest(serverDiedRequestStatus);	// CSI: 94 # This is server side context
    KAOMANAGER_LOGGER_FN2("StartThread7");
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// RMailboxPollingSession
// ---------------------------------------------------------------------------
//
EXPORT_C RAlwaysOnlineClientSession::RAlwaysOnlineClientSession()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C RAlwaysOnlineClientSession::~RAlwaysOnlineClientSession()
    {
    }

// ---------------------------------------------------------------------------
// Connect
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RAlwaysOnlineClientSession::Connect()
    {
    KAOMANAGER_LOGGER_FN1("RAlwaysOnlineClientSession::Connect");

    if ( Handle() )
        {
        KAOMANAGER_LOGGER_FN2("RAlwaysOnlineClientSession::Connect1");
        return KErrAlreadyExists;
        }

    FOREVER
        {
        // Try to make session with server
        TInt ret = CreateSession( KAlwaysOnlineManagerServerName,
        						Version(), 
        						KDefaultMessageSlots );
        
        if ( ret != KErrNotFound && ret != KErrServerTerminated )
            {
            KAOMANAGER_LOGGER_FN2("RAlwaysOnlineClientSession::Connect2");
            return ret;
            }
        
        // Try to start the server
        ret=StartThread();
        
        if ( ret != KErrNone && ret != KErrAlreadyExists )
            {
            // Error can't be handled.
            KAOMANAGER_LOGGER_FN2("RAlwaysOnlineClientSession::Connect3");
            return ret;
            }
        }
    }

// ---------------------------------------------------------------------------
// RAlwaysOnlineClientSession::Version()
// ---------------------------------------------------------------------------
//
EXPORT_C TVersion RAlwaysOnlineClientSession::Version() const
    {
    return(TVersion(KAlwaysOnlineMajorVersionNumber,
        KAlwaysOnlineMinorVersionNumber,
        KAlwaysOnlineVersionNumber));
    }

// ---------------------------------------------------------------------------
// RAlwaysOnlineClientSession::RelayCommandL
// ---------------------------------------------------------------------------
//
EXPORT_C void RAlwaysOnlineClientSession::RelayCommandL( 
    TAlwaysOnlineServerAPICommands aCommand, 
    TDes8& aParameter )
    {
    User::LeaveIfError( SendReceive( aCommand, TIpcArgs( &aParameter ) ) );
    }

// ---------------------------------------------------------------------------
//  RAlwaysOnlineClientSession::AoApiSendMessageL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RAlwaysOnlineClientSession::SendSinglePacketL(        
    const TAlwaysOnlineServerAPICommands aCommand, 
    TDes8& aParameter ) 
    {
    // Connect client and close the connection after the message is sent
    User::LeaveIfError( Connect() );

    // Send message to server and close it
    RelayCommandL( aCommand, aParameter );
    Close();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void RAlwaysOnlineClientSession::SendCommandAsync( 
    const TAlwaysOnlineServerAPICommands aCommand,
    TDes8& aParameter,
    TRequestStatus& aStatus )
    {

    aStatus = KRequestPending;
    SendReceive( aCommand, TIpcArgs( &aParameter ), aStatus );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RAlwaysOnlineClientSession::SendReceiveSync(
    const TAlwaysOnlineServerAPICommands aCommand,
    TDes8& aParameter,
    TDes8& aResultParameter )
    {
    TRequestStatus status = KRequestPending;
    SendReceive( aCommand, 
            TIpcArgs( &aParameter, &aResultParameter ), status );
    
    User::WaitForRequest( status );
    
    return status.Int();
    }

//#endif
