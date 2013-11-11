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
* Description: AlwaysOnline server
*
*/


#include "AlwaysOnlineManagerServer.h"
#include "AlwaysOnlineManagerServerSession.h"
#include "AlwaysOnlineManagerStartupSignaller.h"
#include "AlwaysOnlineManagerLogging.h"



//constants

//---------------------------------------------
// CAlwaysOnlineManagerServer()
//---------------------------------------------
CAlwaysOnlineManagerServer::CAlwaysOnlineManagerServer( TInt aPriority )
	: CPolicyServer( aPriority, KAOMServerPolicy )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::CAlwaysOnlineManagerServer");
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::CAlwaysOnlineManagerServer");
    }

//---------------------------------------------
// NewL()
//---------------------------------------------
EXPORT_C CAlwaysOnlineManagerServer* CAlwaysOnlineManagerServer::NewL()
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::NewL");
    CAlwaysOnlineManagerServer* self = new (ELeave) 
    	CAlwaysOnlineManagerServer( EAlwaysOnlineManagerServerPriority );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	self->StartL( KAlwaysOnlineManagerServerName );
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::NewL");
	return self;
    }


//---------------------------------------------
// ConstructL()
//---------------------------------------------
void CAlwaysOnlineManagerServer::ConstructL()
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::ConstructL");
    iManager = NULL;

    TRAPD(err, iManager = CAlwaysOnlineManager::NewL(this) );
    
    if( err==KErrNone )
        {
        TRAPD(err2, iManager->DoStartL() );
        if(err2!=KErrNone)
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("Manager Start failed, err2:%d", err2 );
            PanicServer( EAOMServerStartFail );
            }
        }
    else
        {
        KAOMANAGER_LOGGER_WRITE_FORMAT("Manager NewL failed, err:%d", err );
        PanicServer( EAOMServerStartFail );
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::ConstructL");
    }

//---------------------------------------------
// ~CAlwaysOnlineManagerServer()
//---------------------------------------------
CAlwaysOnlineManagerServer::~CAlwaysOnlineManagerServer()
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::~");
    //we should never come here while phone is switched on
    delete iManager;
    //delete iSession;
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::~");
    }

//---------------------------------------------
// HandleClientCommandL()
//---------------------------------------------
//
EXPORT_C void CAlwaysOnlineManagerServer::HandleClientCommandL(
	TAlwaysOnlineServerAPICommands aCommand, TDes8& aParameter )
    {
    iManager->BroadcastClientCommandL( aCommand, aParameter );
    }

//---------------------------------------------
// NewSessionL()
//---------------------------------------------
CSession2* CAlwaysOnlineManagerServer::NewSessionL(const TVersion &aVersion,
												const RMessage2& /*aMessage*/) const
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::NewSessionL");
    // check we're the right version
	TVersion v( KAlwaysOnlineMajorVersionNumber,
				KAlwaysOnlineMinorVersionNumber,
				KAlwaysOnlineVersionNumber );

	if (!User::QueryVersionSupported( v, aVersion ))
		User::Leave( KErrNotSupported );

	// make new session
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::NewSessionL");
	return CAlwaysOnlineManagerServerSession::NewL( (CAlwaysOnlineManagerServer*)this );
    }

//---------------------------------------------
// PanicServer()
//---------------------------------------------
void CAlwaysOnlineManagerServer::PanicServer( TAlwaysOnlineManagerPanic aPanic )
    {
    _LIT(KTxtAlwaysOnlineManagerServer,"CAlwaysOnlineManagerServer");
	User::Panic(KTxtAlwaysOnlineManagerServer, aPanic);
    }

//**********************************
//Global functions
//**********************************


//---------------------------------------------
// RunServer()
//---------------------------------------------
TInt CAlwaysOnlineManagerServer::RunServer(TOAManagerStartupSignaller& aSignaller)
	{
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::RunServer");
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt ret = KErrNoMemory;
	if	(cleanup)
		{
		TRAP(ret, CAlwaysOnlineManagerServer::RunServerL(aSignaller));
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::RunServer");
	return ret;
	}



//---------------------------------------------
// RunServer()
//---------------------------------------------
void CAlwaysOnlineManagerServer::RunServerL(TOAManagerStartupSignaller& aSignaller)
	{
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::RunServerL");
	// Naming the server thread after the server helps to debug panics
	User::LeaveIfError( User::RenameThread( KAlwaysOnlineManagerServerName ) );

	// Create and install the active scheduler we need
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
    CleanupStack::PushL( scheduler );
	__ASSERT_ALWAYS( scheduler,PanicServer( EMainSchedulerError ) );
	CActiveScheduler::Install( scheduler );

	// construct server, an active object
    CAlwaysOnlineManagerServer* server = CAlwaysOnlineManagerServer::NewL();
    CleanupStack::PushL( server );

	// Initialisation complete, now signal the client
	aSignaller.SignalL();

	// Ready to run
	CActiveScheduler::Start();		// CSI: 3 # This is only easy way...

	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy( 2, scheduler ); // CSI: 47,12 # scheduler, server
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::RunServerL");
	}

//*************************************************************************************
TInt CAlwaysOnlineManagerServer::ServerThreadFunction(TAny* aParameters)
	{
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::ServerThreadFunction");
	TOAManagerStartupSignaller* parameters = static_cast<TOAManagerStartupSignaller*>(aParameters);
    TInt ret = RunServer(*parameters);
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::ServerThreadFunction");
	return ret;
	}


///////////////////////////////////////////////////////////////////////////////////////
// ----> TOAManagerStartupSignaller (source)
///////////////////////////////////////////////////////////////////////////////////////

//*************************************************************************************
void TOAManagerStartupSignaller::SignalL()
	{
	// Signal the owning thread that the server has started successfully
	// This may itself fail
    KAOMANAGER_LOGGER_FN1("TOAManagerStartupSignaller::SignalL");
	RThread startingThread;
	User::LeaveIfError(startingThread.Open(iThreadId));
	startingThread.RequestComplete(iStatus, KErrNone);
	startingThread.Close();
    KAOMANAGER_LOGGER_FN2("TOAManagerStartupSignaller::SignalL");
	}


///////////////////////////////////////////////////////////////////////////////////////
// ----> Global kick-off methods (source)
///////////////////////////////////////////////////////////////////////////////////////


//*************************************************************************************
IMPORT_C TInt WinsMain();
EXPORT_C TInt WinsMain()
	{
	// WINS DLL entry-point. Just return the real thread function 
	// cast to TInt
	return reinterpret_cast<TInt>(&CAlwaysOnlineManagerServer::ServerThreadFunction);
	}


//*************************************************************************************


//*************************************************************************************
TInt TOAManagerStartupSignaller::GetCommand()
	{
	// Validate the size of the command line.
    KAOMANAGER_LOGGER_FN1("TOAManagerStartupSignaller::GetCommand");

	if ( User::CommandLineLength() != 
            sizeof( TOAManagerStartupSignaller ) / sizeof( TText ) )
        {
        KAOMANAGER_LOGGER_FN2( "TOAManagerStartupSignaller::GetCommand error" );
		return KErrGeneral;
        }

	// Get the command line
	TPtr pCommandLine(
        reinterpret_cast<TText*>( this ), 0, 
        sizeof( TOAManagerStartupSignaller ) / sizeof( TText ) );
	User::CommandLine( pCommandLine );
	// 
    KAOMANAGER_LOGGER_FN2( "TOAManagerStartupSignaller::GetCommand" );
	return KErrNone;
    }

//*************************************************************************************
TInt E32Main()
	{
	// Fetch the startup parameters from the command line.
    KAOMANAGER_LOGGER_FN1("E32Main");
	TOAManagerStartupSignaller signaller;
	TInt error = signaller.GetCommand();
	
	// If there wasn't an error fetching the command line parameters
	// then we attempt to start the server.
	if	(error == KErrNone)
        {
		error = CAlwaysOnlineManagerServer::RunServer(signaller);
        }
    KAOMANAGER_LOGGER_FN2("E32Main");
	return error;
	}

//#endif


EXPORT_C GLDEF_C TInt CAlwaysOnlineManagerServer::ThreadFunction( TAny* aParameters )
	{
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManagerServer::ThreadFunction");
	TOAManagerStartupSignaller* parameters = static_cast<TOAManagerStartupSignaller*>(aParameters);
	TInt retVal = RunServer(*parameters);
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManagerServer::ThreadFunction");
	return retVal;
	}	

