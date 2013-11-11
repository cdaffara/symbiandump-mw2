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
* Description:  Harvester server*
*/

#include <e32svr.h>
#include <apgcli.h>
#include <apmrec.h>

#include "harvesterserver.h"
#include "harvesterserversession.h"
#include "harvestercommon.h"
#include "harvesterlog.h"
#include "harvesterao.h"
#include "harvesterblacklist.h"
#include "mdsactivescheduler.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesterserverTraces.h"
#endif


// ----------------------------------------------------------------------------------------
// Server's policy here
// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------
// Total number of ranges
// ----------------------------------------------------------------------------------------
const TUint KHarvesterServerRangeCount = 10;
 
// ----------------------------------------------------------------------------------------
// Definition of the ranges
// ----------------------------------------------------------------------------------------
const TInt KHarvesterServerRanges[KHarvesterServerRangeCount] = 
{
	EResumeHarvester,        // Resume/start Harvester server
	EPauseHarvester,          // Pause Harvester server
	EHarvestFile,
    ERegisterProcessOrigin,
    EUnregisterProcessOrigin,
    ERegisterHarvestComplete,
    EUnregisterHarvestComplete,
    ERegisterHarvesterEvent,
    EUnregisterHarvesterEvent,
    EHarvestFileWithUID,
};

// ----------------------------------------------------------------------------------------
// Policy to implement for each of the above ranges 
// ----------------------------------------------------------------------------------------      
const TUint8 KHarvesterServerElementsIndex[KHarvesterServerRangeCount] = 
	{
	CPolicyServer::ECustomCheck,    // EResumeHarvester
    CPolicyServer::ECustomCheck,    // EPauseHarvester
    CPolicyServer::ECustomCheck,    // EHarvestFile
    CPolicyServer::ECustomCheck,    // ERegisterProcessOrigin
    CPolicyServer::ECustomCheck,    // EUnregisterProcessOrigin
    CPolicyServer::ECustomCheck,    // ERegisterHarvestComplete
    CPolicyServer::ECustomCheck,    // EUnregisterHarvestComplete
    CPolicyServer::ECustomCheck,    // ERegisterHarvesterEvent
    CPolicyServer::ECustomCheck,    // EUnregisterHarvesterEvent
    CPolicyServer::ECustomCheck,    // EHarvestFileWithUID
	};

// ----------------------------------------------------------------------------------------
// Package all the above together into a policy 
// ---------------------------------------------------------------------------------------- 
const CPolicyServer::TPolicy KHarvesterServerPolicy =
	{
	CPolicyServer::EAlwaysPass,
	KHarvesterServerRangeCount,      // number of ranges
	KHarvesterServerRanges,          // ranges array
	KHarvesterServerElementsIndex,   // elements<->ranges index
	NULL 
	                           // array of elements
	};

// ---------------------------------------------------------------------------
// CustomSecurityCheckL
// ---------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CHarvesterServer::CustomSecurityCheckL(
        const RMessage2& aMsg, TInt& /*aAction*/, TSecurityInfo& /*aMissing*/ )
	{
    WRITELOG( "CHarvesterServer::CustomSecurityCheckL()" );
    
    CPolicyServer::TCustomResult securityCheckResult = EFail;
    
    switch ( aMsg.Function() )
    	{
        case ERegisterHarvestComplete:
        case EUnregisterHarvestComplete:
        case ERegisterHarvesterEvent:
        case EUnregisterHarvesterEvent:
            {
            if( aMsg.HasCapability( ECapabilityReadUserData ) )
                {
                securityCheckResult = EPass;
                }
            break;
            }
    	
        case EHarvestFileWithUID:
            {
            if( aMsg.HasCapability( ECapabilityWriteUserData ) )
                {
                securityCheckResult = EPass;
                }
            break;
            }
            
        case EHarvestFile:
    	case EResumeHarvester:
    	case EPauseHarvester:
    	case ERegisterProcessOrigin:
    	case EUnregisterProcessOrigin:
    			{
            if( aMsg.HasCapability( ECapabilityWriteDeviceData ) )
                {
                securityCheckResult = EPass;
                }
    		}
        break;
        
        default:
            {
            securityCheckResult = EFail;
            }
   	    }
    
    return securityCheckResult;
	}
// ---------------------------------------------------------------------------
// CustomFailureActionL
// ---------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CHarvesterServer::CustomFailureActionL(
        const RMessage2& /*aMsg*/, TInt /*aAction*/, const TSecurityInfo& /*aMissing*/ )
	{
    // Not used
    return EFail;
	}
 
// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CHarvesterServer* CHarvesterServer::NewLC()
	{
    WRITELOG( "CHarvesterServer::NewLC() - begin" );
    
	CHarvesterServer* self = new (ELeave) CHarvesterServer(
	        KHarvesterServerHighPriority, KHarvesterServerPolicy, 
			ESharableSessions );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterServer* CHarvesterServer::NewL()
	{
    WRITELOG( "CHarvesterServer::NewL() - begin" );
    
	CHarvesterServer* self = CHarvesterServer::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// CHarvesterServer
// ---------------------------------------------------------------------------
//
CHarvesterServer::CHarvesterServer( TInt aPriority, const TPolicy& aPolicy, 
		TServerType aType )
 	: CPolicyServer( aPriority, aPolicy, aType )
 	{
    WRITELOG( "CHarvesterServer::CHarvesterServer() - begin" );
 	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterServer::ConstructL()
	{
	WRITELOG( "CHarvesterServer::ConstructL() - begin" );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_CONSTRUCTL, "CHarvesterServer::ConstructL - begin" );
	
	StartL( KHarvesterServerName );
	iHarvesterAO = CHarvesterAO::NewL();
	
    RProcess process;
    process.SetPriority( EPriorityBackground );
    process.Close();
	
	iHarvesterAO->SetHarvesterStatusObserver( this );	
	iPauseObserverAO = CPauseObserverAO::NewL( *this );
	
    // create shutdown observer
    iShutdownObserver = CHarvesterShutdownObserver::NewL( *this );                                                
	
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_CONSTRUCTL, "CHarvesterServer::ConstructL - end" );    
	WRITELOG( "CHarvesterServer::ConstructL() - end" );
	}

// ---------------------------------------------------------------------------
// Pause
// ---------------------------------------------------------------------------
//
void CHarvesterServer::Pause( const RMessage2& aMessage )
	{
	WRITELOG( "CHarvesterServer::Pause() - begin" );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_PAUSE, "CHarvesterServer::Pause" );	

	if ( !iHarvesterAO->IsServerPaused() )
		{
		iMessage = &aMessage;
		iHarvesterAO->SetNextRequest( CHarvesterAO::ERequestPause );
		}
	else
		{
		aMessage.Complete( KErrInUse );
		}
	}


// ---------------------------------------------------------------------------
// Resume
// ---------------------------------------------------------------------------
//
void CHarvesterServer::Resume( const RMessage2& aMessage )
	{
	WRITELOG( "CHarvesterServer::Resume()" ); 
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_RESUME, "CHarvesterServer::Resume" );	
	
	if ( iHarvesterAO->IsServerPaused() )
		{
		iMessage = &aMessage;
		iHarvesterAO->SetNextRequest( CHarvesterAO::ERequestResume );
		}
	else
		{
		aMessage.Complete( KErrInUse );
		}
	}

// ---------------------------------------------------------------------------
// Pause
// ---------------------------------------------------------------------------
//
void CHarvesterServer::Pause()
	{
	WRITELOG( "CHarvesterServer::Pause()" );
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_PAUSE, "CHarvesterServer::Pause" );
    
	if ( !iHarvesterAO->IsServerPaused() )
		{
		iHarvesterAO->SetNextRequest( CHarvesterAO::ERequestPause );
		}
	}

// ---------------------------------------------------------------------------
// Resume
// ---------------------------------------------------------------------------
//
void CHarvesterServer::Resume()
	{
	WRITELOG( "CHarvesterServer::Resume()" ); 
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_RESUME, "CHarvesterServer::Resume" );
    
	if ( iHarvesterAO->IsServerPaused() )
		{
		iHarvesterAO->SetNextRequest( CHarvesterAO::ERequestResume );
		}
	}


void CHarvesterServer::PauseReady( TInt aError )
	{
	WRITELOG1( "CHarvesterServer::PauseReady( %d )", aError );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_PAUSEREADY, "CHarvesterServer::PauseReady" );
	
	if( iMessage )
		{
		iMessage->Complete( aError );
		iMessage = NULL;
		}
	}

void CHarvesterServer::ResumeReady( TInt aError )
	{
	WRITELOG1( "CHarvesterServer::ResumeReady( %d )", aError );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_RESUMEREADY, "CHarvesterServer::ResumeReady" );
	
	if( iMessage )
		{
		iMessage->Complete( aError );
		iMessage = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CHarvesterServer::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CHarvesterServer::ShutdownNotification()
    {
    WRITELOG( "CHarvesterServer::ShutdownNotification" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_SHUTDOWNNOTIFICATION, "CHarvesterServer::ShutdownNotification" );
    
    User::RenameThread( KHarvesterServerNameIAD );
    
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CHarvesterServer::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CHarvesterServer::RestartNotification()
    {
    WRITELOG( "CHarvesterServer::RestartNotification" );    
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_RESTARTNOTIFICATION, "CHarvesterServer::RestartNotification" );
    
    }


// ---------------------------------------------------------------------------
// Harvest File
// ---------------------------------------------------------------------------
//
void CHarvesterServer::HarvestFile( const RMessage2& aMessage )
	{
	WRITELOG( "CHarvesterServer::HarvestFile()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_HARVESTFILE, "CHarvesterServer::HarvestFile" );
	
	iHarvesterAO->HarvestFile( aMessage );
	}

// ---------------------------------------------------------------------------
// Harvest File with UID
// ---------------------------------------------------------------------------
//
void CHarvesterServer::HarvestFileWithUID( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::HarvestFile()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_HARVESTFILEWITHUID, "CHarvesterServer::HarvestFileWithUID" );
    
    iHarvesterAO->HarvestFileWithUID( aMessage );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::RegisterProcessOrigin()
// ---------------------------------------------------------------------------
//
void CHarvesterServer::RegisterProcessOrigin( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::RegisterProcessOrigin()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_REGISTERPROCESSORIGIN, "CHarvesterServer::RegisterProcessOrigin" );
    
    iHarvesterAO->RegisterProcessOrigin( aMessage );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::UnregisterProcessOrigin()
// ---------------------------------------------------------------------------
//
void CHarvesterServer::UnregisterProcessOrigin( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::UnregisterProcessOrigin()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_UNREGISTERPROCESSORIGIN, "CHarvesterServer::UnregisterProcessOrigin" );
    
    iHarvesterAO->UnregisterProcessOrigin( aMessage );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::RegisterHarvestComplete()
// ---------------------------------------------------------------------------
//
TInt CHarvesterServer::RegisterHarvestComplete( const CHarvesterServerSession& aSession, const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::RegisterHarvestComplete()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_REGISTERHARVESTCOMPLETE, "CHarvesterServer::RegisterHarvestComplete" );
    
    return iHarvesterAO->RegisterHarvestComplete( aSession, aMessage );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::UnregisterHarvestComplete()
// ---------------------------------------------------------------------------
//
TInt CHarvesterServer::UnregisterHarvestComplete( const CHarvesterServerSession& aSession )
    {
    WRITELOG( "CHarvesterServer::UnregisterHarvestComplete()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_UNREGISTERHARVESTCOMPLETE, "CHarvesterServer::UnregisterHarvestComplete" );
    
    return iHarvesterAO->UnregisterHarvestComplete( aSession );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::RegisterHarvesterEvent()
// ---------------------------------------------------------------------------
//
void CHarvesterServer::RegisterHarvesterEvent( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::RegisterHarvesterEvent()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_REGISTERHARVESTEREVENT, "CHarvesterServer::RegisterHarvesterEvent" );
    
    iHarvesterAO->RegisterHarvesterEvent( aMessage );
    }

// ---------------------------------------------------------------------------
// CHarvesterServer::UnregisterHarvesterEvent()
// ---------------------------------------------------------------------------
//
void CHarvesterServer::UnregisterHarvesterEvent( const RMessage2& aMessage )
    {
    WRITELOG( "CHarvesterServer::UnregisterHarvesterEvent()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_UNREGISTERHARVESTEREVENT, "CHarvesterServer::UnregisterHarvesterEvent" );
    
    iHarvesterAO->UnregisterHarvesterEvent( aMessage );
    }

// ---------------------------------------------------------------------------
// ~CHarvesterServer
// ---------------------------------------------------------------------------
//
CHarvesterServer::~CHarvesterServer()
    {
    WRITELOG( "CHarvesterServer::~CHarvesterServer()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_CHARVESTERSERVER, "CHarvesterServer::~CHarvesterServer" );
    
    delete iHarvesterAO;
    iHarvesterAO = NULL;
    delete iPauseObserverAO;
    iPauseObserverAO = NULL;
    delete iShutdownObserver;
    iShutdownObserver = NULL;
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_CHARVESTERSERVER, "CHarvesterServer::~CHarvesterServer end" );
    
    WRITELOG( "CHarvesterServer::~CHarvesterServer() end" );
    }

// ---------------------------------------------------------------------------
// PanicClient
// ---------------------------------------------------------------------------
//
void CHarvesterServer::PanicClient( const RMessage2& aMessage, TInt aPanic, 
											const TDesC& aPanicDescription )
	{
    WRITELOG( "CHarvesterServer::PanicClient()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_PANICCLIENT, "CHarvesterServer::PanicClient" );
    
    aMessage.Panic( aPanicDescription, aPanic );
 	}

// ---------------------------------------------------------------------------
// PanicServer
// ---------------------------------------------------------------------------
//
void CHarvesterServer::PanicServer( TInt aPanic, const TDesC& aPanicDescription )
	{   
    WRITELOG( "CHarvesterServer::PanicServer() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_PANICSERVER, "CHarvesterServer::PanicServer" );
    
    User::Panic( aPanicDescription, aPanic );
 	}

// ---------------------------------------------------------------------------
// NewSessionL
// ---------------------------------------------------------------------------
//
CSession2* CHarvesterServer::NewSessionL(
    const TVersion& aVersion, const RMessage2& ) const
	{
	WRITELOG( "CHarvesterServer::NewSessionL() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_NEWSESSIONL, "CHarvesterServer::NewSessionL" );
    
    //If there isn't connection to mde, we can't do much. Inform client about situation.
    //This doesn't leave on first client because Process::Rendezcvouz is called when mde:s
    //HandleSessionOpened is called.
	if (!iHarvesterAO->IsConnectedToMde())
	    {
	    User::Leave(KErrNotReady);
	    }
    
	if ( iShutdownObserver->UpdateInProgress() )
	    {
	    WRITELOG( "CHarvesterServer::NewSessionL - iad update in progress: KErrLocked");
	    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_NEWSESSIONL, "CHarvesterServer::NewSessionL- iad update in progress: KErrLocked" );
	    
	    User::Leave(KErrLocked);
	    }

    // Check we are the right version
    if ( !User::QueryVersionSupported( TVersion( KHarvesterServerMajorVersion,
                                                 KHarvesterServerMinorVersion,
                                                 KHarvesterServerBuildVersion ),
                                       aVersion ) )
    	{
        User::Leave( KErrNotSupported );
    	}

    return CHarvesterServerSession::NewL( *const_cast<CHarvesterServer*>( this ) );
	}

	
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CHarvesterServer::RunError( TInt aError )
	{
	WRITELOG1( "CHarvesterServer::RunError - %d()", aError );
	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_RUNERROR, "CHarvesterServer::RunError" );
	
    if ( aError == KErrBadDescriptor )
 		{
        // A bad descriptor error implies a badly programmed client,
        // so panic it; otherwise report the error to the client
        PanicClient( Message(), KErrBadDescriptor );
    	}
    else
    	{
        Message().Complete( aError );
    	}

    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();

    return KErrNone;    // Handled the error fully
 	}

// ---------------------------------------------------------------------------
// ThreadFunctionL
// ---------------------------------------------------------------------------
//
void CHarvesterServer::ThreadFunctionL()
	{
	WRITELOG( "CHarvesterServer::ThreadFunctionL() - begin" );
    OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_THREADFUNCTIONL, "CHarvesterServer::ThreadFunctionL - begin" );
    
    User::LeaveIfError( User::RenameThread( KHarvesterServerName ) );
    // Construct active scheduler
    CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );

    // Install active scheduler
    // We don't need to check whether an active scheduler is already installed
    // as this is a new thread, so there won't be one
    CActiveScheduler::Install( activeScheduler );
    
    CHarvesterServer::NewLC();

    RProcess::Rendezvous(KErrNone);

    // Start handling requests
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy( 2, activeScheduler );
    
	OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_THREADFUNCTIONL, "CHarvesterServer::ThreadFunctionL - end" );
	
    WRITELOG( "CHarvesterServer::ThreadFunctionL() - end" );
	}

// ---------------------------------------------------------------------------
// ThreadFunction
// ---------------------------------------------------------------------------
//
TInt CHarvesterServer::ThreadFunction( TAny* /*aNone*/ )
	{    
 	WRITELOG( "CHarvesterServer::ThreadFunction() - TAny - begin" );
 	OstTrace0( TRACE_NORMAL, CHARVESTERSERVER_THREADFUNCTION, "CHarvesterServer::ThreadFunction - TAny - begin" );
 	
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if ( !cleanupStack )
		{
        PanicServer( KErrServerTerminated );
	 	}

    TRAPD( err, ThreadFunctionL() );

    if ( cleanupStack )
    	{
        delete cleanupStack;
        cleanupStack = NULL;	
    	}
    
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERSERVER_THREADFUNCTION, "CHarvesterServer::ThreadFunction - TAny - end" );    
    WRITELOG( "CHarvesterServer::ThreadFunction() - TAny - end" );
    
    return err;
	}

// ---------------------------------------------------------------------------
// E32Main
// ---------------------------------------------------------------------------
//
TInt E32Main()
	{    
    WRITELOG( "CHarvesterServer::E32Main() - begin" );
    OstTrace0( TRACE_NORMAL, _E32MAIN, "CHarvesterServer::E32Main() - begin" );
    
    __UHEAP_MARK;

    const TInt result = CHarvesterServer::ThreadFunction( NULL );

    __UHEAP_MARKEND;

    return result;
	}

// End of file


