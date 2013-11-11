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
*     Alwaysonline manager implementation file
*
*/


#include <ecom/ecom.h>
#include <ecom/resolver.h>
#include <AlwaysOnlineManagerCommon.h>
#include <CoreApplicationUIsSDKCRKeys.h>   // Offline key

#include "AlwaysOnlineManager.h"
#include "AlwaysOnlineManagerServer.h"
#include "HandleServerCommandOperation.h"
#include "AlwaysOnlineManagerLogging.h"
#include "AOCenRepControl.h"
#include "AOCommandParser.h"

const TInt KPluginArrayGranularity = 3;
const TInt KNWOperationsAllowed = 1;
const TInt KManagerOperationsGranularity = 6;

const TInt KDisabledPluginsGranulatiry = 3;

const TUid KAOEmailPluginUid = {0x101F85ED};

_LIT8( KStopPlugin, "1" );
#ifdef _DEBUG
_LIT( KAOManagerParsePanic, "AO parse" );
#endif

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager()
// ----------------------------------------------------------------------------
//
CAlwaysOnlineManager::CAlwaysOnlineManager()
    {
    }

// ----------------------------------------------------------------------------
// NewL()
// ----------------------------------------------------------------------------
//
CAlwaysOnlineManager* CAlwaysOnlineManager::NewL(
    CAlwaysOnlineManagerServer* aServer )
    {
    CAlwaysOnlineManager* self = new(ELeave) CAlwaysOnlineManager;
    CleanupStack::PushL( self );
    self->ConstructL( aServer );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// ~CAlwaysOnlineManager()
// ----------------------------------------------------------------------------
//
CAlwaysOnlineManager::~CAlwaysOnlineManager()
    {
    if ( iOperations )
        {
        iOperations->ResetAndDestroy();
        }
        
    delete iOperations;
    iRfs.Close();
    iConMon.Close();
    
    //we shouldn't never come here while phone is switched on
    if ( iPluginArray )
        {
        TInt idx = KErrNotFound;
        while( iPluginArray->Count() )
            {
            idx = iPluginArray->Count() - 1;
            // Delete object
            delete iPluginArray->At( idx );
            // Delete element
            iPluginArray->Delete( idx );
            }
        }
        
    delete iCenRepControl;
    delete iPluginArray;       
    delete iDisabledPluginUidsArray;
    delete iDiskSpaceObserver;
    delete iSession;
    }
    
// ----------------------------------------------------------------------------
// ConstructL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::ConstructL( CAlwaysOnlineManagerServer* aServer )
    {
    // Create operation array to heap
    iOperations = new ( ELeave ) 
        CMsvSingleOpWatcherArray( KManagerOperationsGranularity );
        
    iCenRepControl = CAOCenRepControl::NewL( *this );
    
    iSession = CMsvSession::OpenSyncL(*this);
    iServer = aServer;
    iNetworkStatus = KErrNotFound;
    iOffline = ETrue;
    iLastDiskEvent = EAOManagerEmailCommandBase;
    iPluginsLoaded = EFalse;
    iPluginArray = new(ELeave)
        CArrayFixFlat<CAlwaysOnlineEComInterface*>(KPluginArrayGranularity);
    iDisabledPluginUidsArray = new ( ELeave ) 
        CArrayFixFlat<TUid>( KDisabledPluginsGranulatiry );

    User::LeaveIfError( iRfs.Connect() );
    iConMon.ConnectL();
    }

// ----------------------------------------------------------------------------
// DoStartL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::DoStartL()
    {
    if( !iPluginsLoaded )
        {
        TRAPD( err, LoadPluginsL() );
        if ( err != KErrNone )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::DoStartL() Errors in LoadPluginsL. Err = %d", err );
            }
        else
            {
            // Nowadays, we don't care about the plugin count, we will start all
            // which are not disabled. At this point iPluginArray contains those
            // plugins which are enabled.
            TRAPD( err, StartPluginsL() );
            if ( err != KErrNone )
                {
                KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::DoStartL() Errors in StartPluginsL. Err = %d", err );
                }
            else
                {
                //don't start observer unless plugins have been started                  
                iDiskSpaceObserver = 
                    CAlwaysOnlineDiskSpaceObserver::NewL( this, *iSession );
                iDiskSpaceObserver->SetLimitAndActivateL();
                //check network status and start observing it
                CheckNetworkStatusAndSetObserverL();
                
                //get state of KCoreAppUIsNetworkConnectionAllowed key from 
                //CenRep and relay it to plugins
                TInt state = KErrNotFound;
                TInt result = iCenRepControl->GetCenRepKey(
                    KCRUidCoreApplicationUIs,
                    KCoreAppUIsNetworkConnectionAllowed, 
                    state );
                    
                if ( result == KErrNone )
                    {
                    HandleOfflineEventL( state );
                    }
                else
                    {
                    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::DoStartL() Could not get KCoreAppUIsNetworkConnectionAllowed from CenRep! Result = %d", result );
                    }
                }
                
            // Send suicide query to all started plugins.
            KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::DoStartL() Sending suicide queries.");
            QuerySuicideL();
            }
        }
    }

// ----------------------------------------------------------------------------
// PointerArrayCleanup()
//
// An utility function to handle cleanup of RImplInfoPtrArray instances
// @param aArray Pointer to RImpInfoPtrArray to be deleted. 
// 		  Guaranteed not to be NULL.
// ----------------------------------------------------------------------------
//
static void PointerArrayCleanup( TAny* aArray )
	{
	static_cast< RImplInfoPtrArray* >( aArray )->ResetAndDestroy();
	}

// ----------------------------------------------------------------------------
// LoadPluginsL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::LoadPluginsL()
    {
	RImplInfoPtrArray pluginArray;
    TCleanupItem arrayCleanup( PointerArrayCleanup, &pluginArray );
    CleanupStack::PushL( arrayCleanup );

    //List all plugins which implement AlwaysOnlineManagerInterface
    REComSession::ListImplementationsL( KCEComInterfaceUid, pluginArray );

    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::LoadPluginsL() Count of plugins found: %d", pluginArray.Count() );
    
    // Update list of disabled plugins from cenrep
    iCenRepControl->UpdateDisabledPluginsUids( *iDisabledPluginUidsArray );
    
    if ( pluginArray.Count() )
        {
        TInt index = -1;
        TKeyArrayFix key( 0, ECmpTInt32 );
        
        for( TInt i = 0; i < pluginArray.Count(); i++ )
            {
            CImplementationInformation* info = pluginArray[ i ];

            TUid id = info->ImplementationUid();    

            TInt  result( iDisabledPluginUidsArray->Find( id, key, index ) );
            // if id is found from disabled plugins list, then we don't
            // load it.
            if ( result == 0 )
                {
                 KAOMANAGER_LOGGER_WRITE_FORMAT( "CAlwaysOnlineManager::LoadPluginsL() disabled plugin: 0x%x", id );
                }
            else
                {
                CAlwaysOnlineEComInterface* implementation( NULL );

                //be sure that if plugin leaves, manager doesn't. 
                //This applies in every place!
                TRAP_IGNORE( implementation = CAlwaysOnlineEComInterface::NewL( id ) );
                CleanupStack::PushL( implementation );
                if ( implementation )                    
				{                    
				    implementation->SetStatusQueryObject( this );
                    iPluginArray->AppendL( implementation );
                    
                    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::LoadPluginsL() plugin loaded succesfully: 0x%x", id);
                }
                else
                    {
                    // Just write to debug log, that all the plugins could not be loaded.
                    // Perhaps there should be some nice way to acknoledge the user or the system!
                    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::LoadPluginsL() plugin load failed: 0x%x", id);
                    }
                CleanupStack::Pop( implementation );
                }
            }//for

        //plugins should be loaded
        iPluginsLoaded = ETrue;
        
        }//if
    else
        {
        KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::LoadPluginsL() NO PLUGINS FOUND!");
        //no plugins. This shouldn't be, there's something wrong with build.
        //if plugins are not in build, server shouldn't be either!        
        }
	  CleanupStack::PopAndDestroy(); // arrayCleanup
    }

// ----------------------------------------------------------------------------
// StartPluginsL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::StartPluginsL()
    {
    TInt count = iPluginArray->Count();

    TBuf8<1> dummyParam;
    for ( TInt i = 0; i < count; i++ )
        {
        InvokeCommandHandlerL( EAOManagerPluginStart, dummyParam, i );
        }
    KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::StartPluginsL() All plugins ordered to start");
    }

// ----------------------------------------------------------------------------
// QuerySuicideL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::QuerySuicideL()
    {
    TInt count = iPluginArray->Count();

    TBuf8<1> dummyParam;
    for ( TInt i = 0; i < count; i++ )
        {
        InvokeCommandHandlerL( EAOManagerSuicideQuery, dummyParam, i );
        }
    KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::QuerySuicideL() Suicide query made for all plugins");
    }

// ----------------------------------------------------------------------------
// QueryStatusL()
// ----------------------------------------------------------------------------
//
TAny* CAlwaysOnlineManager::QueryStatusL( TInt aQuery )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::QueryStatusL()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::QueryStatusL() Query received: %d", aQuery);
    
    switch( aQuery )
        {
        case EAOManagerStatusQueryRoaming:
            return reinterpret_cast<TAny*>(
                iNetworkStatus == ENetworkRegistrationRoaming );
            
        case EAOManagerStatusQueryOffline:
            return reinterpret_cast<TAny*>( iOffline );

        case EAOManagerStatusQueryNetwork:
            return reinterpret_cast<TAny*>( iNetworkStatus );
        
        default:           
            return reinterpret_cast<TAny*>( KErrNotSupported );//unknown query
            
        }
    }

// ----------------------------------------------------------------------------
// HandleOfflineEventL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleOfflineEventL( TInt aEvent)
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleOfflineEventL()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandeOfflineEventL() aEvent: %d", aEvent);
    TInt count = iPluginArray->Count();

    TInt command = KErrNotFound;

    if( aEvent == KNWOperationsAllowed )
        {
        command = EAOManagerNWOpsAllowed;
        iOffline = EFalse;
        }
    else 
        {
        command = EAOManagerNWOpsNotAllowed;
        iOffline = ETrue;
        }

    TBuf8<1> dummyParam;
    for( TInt i = 0; i<count; i++ )
        {
        InvokeCommandHandlerL( 
            static_cast<TManagerServerCommands>(command), dummyParam, i );
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleOfflineEventL()");
    }

// ----------------------------------------------------------------------------
// CheckNetworkStatusAndSetObserverL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::CheckNetworkStatusAndSetObserverL()
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::CheckNetworkStatusAndSetObserverL()");
    TRequestStatus status;
    TInt registrationStatus( 0 );

    //check network status
    iConMon.GetIntAttribute(
        EBearerIdGSM, 0, KNetworkRegistration, 
        registrationStatus, status );

    User::WaitForRequest( status );		// CSI: 94 # This is server side context

    if ( status.Int() == KErrNone )
        {
        HandleRegistrationStatusL( registrationStatus );
        }

    //sets this as an observer for networkstatus
    iConMon.NotifyEventL(*this);
    
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::CheckNetworkStatusAndSetObserverL()");
    }

// ----------------------------------------------------------------------------
// EventL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::EventL( const CConnMonEventBase &aConnMonEvent )
    {
    switch ( aConnMonEvent.EventType() )
		{
        case EConnMonNetworkRegistrationChange:
            {
			CConnMonNetworkRegistrationChange* eventNetworkRegChange;
			eventNetworkRegChange = ( CConnMonNetworkRegistrationChange* ) &aConnMonEvent;
		        
			TInt regStatus = eventNetworkRegChange->RegistrationStatus();

            HandleRegistrationStatusL( regStatus );
    
            }
			break;
		default:
			break;
		}; 

    }

// ----------------------------------------------------------------------------
// HandleRegistrationStatusL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleRegistrationStatusL( TInt aStatus )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleRegistrationStatusL()");
    TInt command;

    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleRegistrationStatusL Reporting Network Status to plugins. aStatus: %d", aStatus );
    iNetworkStatus = aStatus;

    if ( aStatus == ENetworkRegistrationHomeNetwork )
        {
        command = EAOManagerStoppedRoaming;
        }
    else if ( aStatus == ENetworkRegistrationRoaming )
        {
        command = EAOManagerStartedRoaming;
        }
    else
        {
        // lets tell plugins we are offline while we don't know network status so that they
        // won't try any connections...
        KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleRegistrationStatusL() 2");
        return; //we don't care about other network states than these two
        }

    TInt count = iPluginArray->Count();

    //send changed status to all plugins
    TBuf8<1> dummyParam;
    for ( TInt i = 0; i<count; i++ )
        {
        InvokeCommandHandlerL( 
            static_cast<TManagerServerCommands>(command), dummyParam, i );
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleRegistrationStatusL()");
    }

// ----------------------------------------------------------------------------
// InvokeCommandHandlerL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::InvokeCommandHandlerL(
    const TManagerServerCommands aCommand,
    TDes8& aParameter,
    TInt aIndex )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::InvokeCommandHandlerL()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::InvokeCommandHandlerL() CommandHandler invoked for TManagerServerCommand: %d", aCommand);
    CMsvSingleOpWatcher* watcher = CMsvSingleOpWatcher::NewL(*this);
    CleanupStack::PushL( watcher );

    if ( !iSession )
        {
        KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::InvokeCommandHandlerL() iSession is NULL");
        iSession = CMsvSession::OpenSyncL(*this);
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::InvokeCommandHandlerL() iSession created to 0x%x", iSession );
        }

    CAOServerCommandHandler* op = CAOServerCommandHandler::NewL( 
        *iSession,
        watcher->iStatus,
        aCommand,
        aParameter,
        *iPluginArray->At(aIndex) );
    CleanupStack::PushL( op );
    iOperations->AppendL( watcher );
    watcher->SetOperation( op ); // takes immediately ownership

    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::InvokeCommandHandlerL() Async command invoked. Operation ID: %d ",op->Id()  );

    CleanupStack::Pop( 2, watcher ); // CSI: 47,12 # op, watcher
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::InvokeCommandHandlerL()");
    }

// ----------------------------------------------------------------------------
// HandleDiskSpaceEventL()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleDiskSpaceEventL( TInt aEvent )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleDiskSpaceEventL()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleDiskSpaceEventL() Got DiskSpace event: %d, Forwarding to plugins", aEvent );

    // Make sure there actually is a reason to send the event
    if ( aEvent == iLastDiskEvent )
        {
        return;
        }
        
    iLastDiskEvent = aEvent;       

    TInt count = iPluginArray->Count();

    //send event to all plugins
    TBuf8<1> dummyParam;
    for( TInt i = 0; i<count; i++ )
        {
        InvokeCommandHandlerL( 
            static_cast<TManagerServerCommands>(aEvent), dummyParam, i );
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleDiskSpaceEventL()");
    }


// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleSessionEventL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleSessionEventL(
    TMsvSessionEvent aEvent,
    TAny* /*aArg1*/,
    TAny* /*aArg2*/,
    TAny* /*aArg3*/)
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleSessionEventL");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleSessionEventL() aEvent %d", aEvent);
    switch( aEvent )
        {
        case EMsvMediaUnavailable:
            //delete diskspace observer
            if( iDiskSpaceObserver )
                {
                iDiskSpaceObserver->Cancel();
                delete iDiskSpaceObserver;
                iDiskSpaceObserver = NULL;            
                }
            break;
        case EMsvMediaAvailable:
            //restart diskspace observer
            if( !iDiskSpaceObserver )
                {
                iDiskSpaceObserver = 
                    CAlwaysOnlineDiskSpaceObserver::NewL( this, *iSession );
                iDiskSpaceObserver->SetLimitAndActivateL();
                }            
            break;
        case EMsvMediaChanged:
            //if media changed, old observer is obsolete, delete and restart.
            if( iDiskSpaceObserver )
                {
                iDiskSpaceObserver->Cancel();
                delete iDiskSpaceObserver;
                iDiskSpaceObserver = NULL;

                iDiskSpaceObserver = 
                    CAlwaysOnlineDiskSpaceObserver::NewL( this, *iSession );
                iDiskSpaceObserver->SetLimitAndActivateL();
                }
            break;

        case EMsvCloseSession:
        case EMsvServerTerminated:
            iOperations->ResetAndDestroy();
            delete iSession;
            iSession = NULL;
            break;


        default:
            break;
        }
        
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleSessionEventL");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::BroadcastClientCommandL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::BroadcastClientCommandL( 
    TAlwaysOnlineServerAPICommands aCommand, 
    TDes8&                         aParameter )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::BroadcastClientCommandL()");
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::BroadcastClientCommandL() aCommand: %d", aCommand);
    // Translate TAlwaysOnlineServerAPICommands to TManagerServerCommands
    TManagerServerCommands command = (TManagerServerCommands)KErrNotFound;
    TBool invoke = ETrue;

    switch( aCommand )
        {
        
        // Start a plugin
        case EServerAPIBaseCommandStart:
            HandleStartPluginL( aParameter );
            command = EAOManagerPluginStart;
            invoke = EFalse;
            break;
        
        // Stop a plugin
        case EServerAPIBaseCommandStop:
            HandleStopPluginL( aParameter );
            command = EAOManagerPluginStop;
            invoke = EFalse;
            break;
        
        case EServerAPIBaseCommandAoSchedulerError:
            KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineManager::BroadcastClientCommandL() AOScheduler ERROR!" );
            command = EAOManagerAOSchdulerError;
            break;
            
        // Suspend email agent
        case EServerAPIEmailTurnOff:
            command = EAOManagerMailboxAgentSuspend;
            break;

        // Turn the email agent back on
        case EServerAPIEmailTurnOn:
            command = EAOManagerMailboxAgentResume;
            break;
        
        case EServerAPIBaseAllowNWOperations:
            invoke = EFalse;
            break;
        case EServerAPIBaseSuspendNWOperations:
            invoke = EFalse;
            break;
        // Remove the agent completely
        case EServerAPIEmailAgentRemove:
            command = EAOManagerMailboxAgentRemove;
            break;

        // Handle the OMA Email Notification
        case EServerAPIEmailEMNReceived:
            command = EAOManagerEMNReceived;
            break;

        // Start temporary agent for manual IMAP4 connection
        case EServerAPIEmailUpdateMailWhileConnected:
            command = EAOManagerMailboxAgentUpdateMailWhileConnected;
            break;
        case EServerAPIEmailQueryState:
            command = EAOManagerQueryState;
            invoke = EFalse;
            BroadcastCommandDirectL( command, aParameter );
            break;
        case EServerAPIEmailCancelAllAndDisconnect:
            command = EAOManagerCancelAllAndDisconnect;
            break;
        case EServerAPIEmailCancelAllAndDoNotDisconnect:
            command = EAOManagerCancelAllAndDoNotDisconnect;
            break;
        case EServerAPIEmailSuspend:
            command = EAOManagerSuspend;
            break;
        case EServerAPIEmailContinue:
            command = EAOManagerContinue;
            break;
        case EServerAPIEmailDoNotDisconnect:
            command = EAOManagerDoNotDisconnect;
            break;
        case EServerAPIEmailSessionClosed:
            command = EAOManagerSessionClosed;
            invoke = EFalse;
            BroadcastCommandDirectL( command, aParameter );
            break;
        case EServerAPIEmailDisableAOEmailPlugin:
        	// Remove UID from plugin array and delete the instance
        	// of the email plugin
        	iCenRepControl->SetPluginStatus( KAOEmailPluginUid, ETrue );
        	for( TInt i = 0; i < iPluginArray->Count(); i++ )
	        	{
	        	if( KAOEmailPluginUid == iPluginArray->At(i)->InstanceUid() )
	        		{
	                delete iPluginArray->At( i );
	                // Delete element
	                iPluginArray->Delete( i );	        		
	        		}
	        	}
        	break;
        case EServerAPIEmailEnableAOEmailPlugin:
        	iCenRepControl->SetPluginStatus( KAOEmailPluginUid, EFalse );
        	break;
            
        // Ignore unrecognized command
        default:
            invoke = EFalse;
        };

    // EServerAPIBaseCommandStart and EServerAPIBaseCommandStop is only
    // sent to one plugin and at this point, it is already handled in
    // their own cases.
    if ( invoke )
        {
        TInt count = iPluginArray->Count();

        // Send event to all plugins
        for( TInt i = 0; i<count; i++ )
            {
            InvokeCommandHandlerL( 
                static_cast<TManagerServerCommands>(command), aParameter, i );
            }
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::BroadcastClientCommandL()");
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::BroadcastCommandDirectL( 
    TManagerServerCommands aCommand, 
    TDes8&                         aParameter )
    {
    TInt count = iPluginArray->Count();
    TInt ret = KErrNone;
    // Send event to all plugins
    for( TInt i = 0; i<count; i++ )
        {
        CAlwaysOnlineEComInterface* plugin = iPluginArray->At(i);
        TAny* result = plugin->HandleServerCommandL( aCommand, &aParameter );
        ret = reinterpret_cast<TInt>(result);
        }
    
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::OpCompleted
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::OpCompleted(
    CMsvSingleOpWatcher& aOpWatcher,
    TInt /*aCompletionCode*/ )
    {
    TMsvOp opId = aOpWatcher.Operation().Id();
    const TInt count = iOperations->Count();  

    for ( TInt i=0; i < count; i++ )
        {
        CMsvOperation& op = (*iOperations)[i]->Operation();

        if ( op.Id() == opId )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::OpCompleted() Async operation %d completed ", opId );
            CMsvSingleOpWatcher* operation = (*iOperations)[i];

            // Attempt to handle operation
            TRAPD( err, HandleOpCompletionL( op.FinalProgress() ) );
            if ( err != KErrNone )
                {
                KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::OpCompleted() failed err:%d", err );
                }
            
            delete operation;
            iOperations->Delete(i);
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::OpCompleted() Operations remaining: %d ", iOperations->Count() );
            break;
            }
        }
    }
    
// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleOpCompletionL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleOpCompletionL( const TDesC8& aProgress )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleOpCompletionL");
    
    TUid id;
    TInt command;
    TBuf8<KAOFinalProgressLength> result;
    
    if ( ParseProgressL( aProgress, id, command, result ) )
        {
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleOpCompletionL() : plugin: 0x%x", id);
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleOpCompletionL() : command: %d", command);
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleOpCompletionL() : result: %S", &result);
        
        switch ( static_cast<TManagerServerCommands>( command ) )
            {
            case EAOManagerSuicideQuery:
                if ( result.Match( KStopPlugin ) > KErrNotFound )
                    {
                    StopPluginL( id );
                    }
                break;
            case EAOManagerPluginStop:
                DisablePlugin( id );
                break;
            default:
                // Currently, there is no other commands for plugins, which 
                // returns something useful to server.
                break;
            }    
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleOpCompletionL");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::ParseProgressL
// ----------------------------------------------------------------------------
//
TBool CAlwaysOnlineManager::ParseProgressL( 
        const TDesC8& aProgress, 
        TUid& aUid,
        TInt& aCommand, 
        TDes8& aResult )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::ParseProgressL");
    TBool parseOk = EFalse;
    
    // Parse final progress, received from CAOServerCommandHandler::FinalProgress()
    CAOCommandParser* parser = NULL;
    TRAPD( err, parser = CAOCommandParser::NewL( aProgress ) );
    
    if ( err == KErrNone )
        {
        CleanupStack::PushL( parser );
        
        TInt error = parser->Parse();

        __ASSERT_DEBUG( error != KErrNotFound, User::Panic( KAOManagerParsePanic, error ) );
        
        if ( error != KErrNotFound )
            {
            aUid = parser->Uid();
            aCommand = parser->Command();
            aResult = parser->Result().Left( aResult.MaxLength() );
            parseOk = ETrue;
            }
        
        CleanupStack::PopAndDestroy( parser );
        }
        
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::ParseProgressL");
    
    return parseOk;
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleStartPluginL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleStartPluginL( const TDes8& aParameter )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleStartPluginL");
    
    TPckgBuf<TUid> paramPack;
    TUid pluginId;
    TInt error = KErrNone;
    
    // Make sure that the parameter length matches Id length
    if ( aParameter.Length() == sizeof( TUid ) )
        {
        paramPack.Copy( aParameter );
        
        // Get the mailbox id from the packet
        pluginId = paramPack();
        error = KErrNone;
        }
    else
        {
        error = KErrNotFound;
        }

    // Check if plugin already loaded.
    for ( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        if ( pluginId == iPluginArray->At( i )->InstanceUid() )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleStartPluginL() plugin already loaded: 0x%x", pluginId);
            // No need to load a plugin again.
            error = KErrAlreadyExists;
            break;
            }
        }

    // Load the plugin and call start for it.            
    CAlwaysOnlineEComInterface* implementation = NULL;

    if ( error == KErrNone )
        {
        TRAPD( err, implementation = 
            CAlwaysOnlineEComInterface::NewL( pluginId ) );
        
        if ( err == KErrNone )
            {
            implementation->SetStatusQueryObject( this );
            iPluginArray->AppendL( implementation );
            
            // Set the loaded plugin to be enabled (== delete UID from cenrep).
            iCenRepControl->SetPluginStatus( pluginId, EFalse );
            
            // Refresh the list of disabled plugins UIDs.
            iCenRepControl->UpdateDisabledPluginsUids( 
                *iDisabledPluginUidsArray );
            
            TBuf8<1> dummyParam;
            // AppendL puts object to the end of the array, 
            // so we can start the last plugin
            TInt idx = iPluginArray->Count() - 1;
            InvokeCommandHandlerL( EAOManagerPluginStart, dummyParam, idx );
            
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleStartPluginL() plugin loaded succesfully: 0x%x", pluginId);
            }
        else
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleStartPluginL() plugin load failed: 0x%x", pluginId);
            }
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleStartPluginL");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleStopPluginL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::HandleStopPluginL( const TDes8& aParameter )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::HandleStopPluginL");
    
    TPckgBuf<TUid> paramPack;
    TUid pluginId;
    
    // Make sure that the parameter length matches Id length
    if ( aParameter.Length() == sizeof( TUid ) )
        {
        paramPack.Copy( aParameter );
        
        // Get the mailbox id from the packet
        pluginId = paramPack();
        StopPluginL( pluginId );
        }
        
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::HandleStopPluginL");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::StopPluginL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::StopPluginL( const TUid& aPluginImplementationUid )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::StopPluginL");
    
    // Find the plugin and send stop command.
    for ( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        TUid id = ( iPluginArray->At( i ) )->InstanceUid();

        // Stop plugin if it has matching implemention UID.
        // Will stop all plugin instances with matching implementation.
        if ( REComSession::GetImplementationUidL(id) == aPluginImplementationUid )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT(
                "CAlwaysOnlineManager::StopPluginL() Calling stop to plugin: 0x%x", 
                id );
            TBuf8<1> dummyParam;
            InvokeCommandHandlerL( 
                static_cast<TManagerServerCommands>( EAOManagerPluginStop ), 
                dummyParam, 
                i );    
            }
        }
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::StopPluginL");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::DisablePlugin
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineManager::DisablePlugin( const TUid& aPluginInstanceUid )
    {
    KAOMANAGER_LOGGER_FN1("CAlwaysOnlineManager::DisablePlugin");
    
    // Delete the plugin and mark it disabled.
    for ( TInt i = 0; i < iPluginArray->Count(); i++ )
        {
        TUid id = ( iPluginArray->At( i ) )->InstanceUid();
        
        // Delete plugin if it has matching instance UID.
        if ( id == aPluginInstanceUid )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT(
                "CAlwaysOnlineManager::DisablePlugin() Deleting plugin from array: 0x%x", 
                aPluginInstanceUid);
            // Delete object
            delete iPluginArray->At( i );
            // Delete element
            iPluginArray->Delete( i );
            }
        }
    // Set this plugin as disabled.
    iCenRepControl->SetPluginStatus( aPluginInstanceUid, ETrue );
    
    // Refresh the list of disabled plugins UIDs.
    iCenRepControl->UpdateDisabledPluginsUids( *iDisabledPluginUidsArray );
    
    
    KAOMANAGER_LOGGER_FN2("CAlwaysOnlineManager::DisablePlugin");
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleNotifyInt
// ----------------------------------------------------------------------------    
//
void CAlwaysOnlineManager::HandleNotifyInt( 
    TUint32 /* aId */, 
    TInt aNewValue )
    {   
    TRAPD( err, HandleOfflineEventL( aNewValue ) );
    if( err != KErrNone )
        {
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleNotifyInt() failed err:%d", err );
        }
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineManager::HandleNotifyError
// ----------------------------------------------------------------------------    
//
void CAlwaysOnlineManager::HandleNotifyError( 
    TUint32 /* aId */, 
    TInt /* aError */, 
    CCenRepNotifyHandler* /* aHandler */ )
    {
    TRAPD( err, iCenRepControl->ReCreateCoreAppUiCenRepSessionL() );
    if( err != KErrNone )
        {
        KAOMANAGER_LOGGER_WRITE_FORMAT("CAlwaysOnlineManager::HandleNotifyError() failed err:%d", err );
        }
    }

//EOF
