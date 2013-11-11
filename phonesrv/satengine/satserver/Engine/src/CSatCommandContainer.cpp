/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains SAT commands.
*
*/



#include    <f32file.h>
#include    <barsc.h>
#include    <bautils.h>
#include    <ecom.h>
#include    <e32property.h>
#include    <data_caging_path_literals.hrh>
#include    <startupdomainpskeys.h>
#include    <satdomainpskeys.h>
#include    <hbtextresolversymbian.h>

#include    "MSatSystemState.h"
#include    "TSatSystemStateFactory.h"
#include    "MSatSystemStateChangeNotifier.h"
#include    "CSatCommandContainer.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"
#include    "TUSatAPI.h"
#include    "MSatSSessions.h"
#include    "MSatShellController.h"
#include    "SatServer.hrh"
#include    "SatLog.h"
#include    "MSatSUiClientHandler.h"
#include    "CSatClientServiceReq.h"
#include    "CSatEventMonitorContainer.h"
#include    "CSatSAPChangeObserver.h"
#include    "CSatSSimSubscriberId.h"
#include    "CSatBIPUtils.h"
#include    "TSatExtErrorUtils.h"
#include    "MSatUiSession.h"
#include    "csatmediatoreventprovider.h"
#include    "csatprofilechangeobserver.h"
#include    "SATInternalPSKeys.h"
#include    "csatmultimodeapi.h"
#include    "csatsactivewrapper.h"

_LIT( KResourceDrive, "z:\\resource\\qt\\translations" );
_LIT( KSatServerRsc, "satapp_");
_LIT( KSatLogTitle, "txt_simatk_title_sim_services");
_LIT( KSatCmccTitle, "txt_simatk_titlw_cmcc_sim_services");

const TUid KSatInterfaceDefinitionUid = { 0x1000f001 };


// Important plugins UIDs. These are started on startup
// Implementation UID is from the <plugin>.rss
const TUid KSetUpEventListUid = { 0x10202993 };

// ======== LOCAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// CleanupPointerArray
// Cleanup RPointerArray objects by using the cleanup stack. Function will be 
// called when application leaves while a RPointerArray object still alive, 
// or when CleanupStack::PopAndDestroy is explicitly called to release a 
// RPointerArray. See CleanupStack::PushL( TCleanupItem ) for more details.
// -----------------------------------------------------------------------------
//
static void CleanupPointerArray( TAny* aArray )
    {
    LOG2( NORMAL, "SATENGINE: CSatCommandContainer::CleanupPointerArray \
        calling array = 0x%08x", aArray )
    
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>( aArray );
    if ( array )
        {
        array->ResetAndDestroy();
        }
    
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::CleanupPointerArray \
        exiting" )
    }

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCommandContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCommandContainer* CSatCommandContainer::NewL(
    MSatSSessions& aSessions,
    TSatEventMediator& aEventMediator,
    MSatSUiClientHandler& aSatUiHandler )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NewL calling" )

    CSatCommandContainer* self =
        new ( ELeave ) CSatCommandContainer(
            aSessions,
            aEventMediator,
            aSatUiHandler );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::~CSatCommandContainer
// Destructor
// -----------------------------------------------------------------------------
//
CSatCommandContainer::~CSatCommandContainer()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::~CSatCommandContainer calling" )

    // Stop and Delete all command handlers
    StopCommandHandlers();

    if ( iUSatAPI )
        {
        iUSatAPI->Close();
        delete iUSatAPI;
        iUSatAPI = NULL;
        }

    if ( iServiceReqs )
        {
        iServiceReqs->Reset();
        delete iServiceReqs;
        iServiceReqs = NULL;
        }

    delete iSatAppName;
    delete iSystemState;
    delete iBipUtils;
    delete iSatMediatorEvent;    
    delete iMultiModeApi;
    
    delete iSapObserver;
    delete iProfileObserver;

    delete iSimSubscriberId;
    delete iEventMonitors;

    delete iCmdHandlers;
    delete iStartupChangeObserver;

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::~CSatCommandContainer exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::StartCommandHandlersL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::StartCommandHandlersL()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::StartCommandHandlersL calling" )

    // If in startup phase, start the rest of the commands
    if ( iStartupPhase )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
             in startup phase" )
        // Create command handlers.
        RImplInfoPtrArray satCommandImplementations;
        CleanupStack::PushL( 
            TCleanupItem( CleanupPointerArray, &satCommandImplementations ) );
        REComSession::ListImplementationsL( KSatInterfaceDefinitionUid,
            satCommandImplementations );

        // Container for commands
        const TInt cmdCount( satCommandImplementations.Count() );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
            Command handler count: %d", cmdCount )

        // Check are there any command implementations
        if ( cmdCount > 0 )
            {
            // Check array. Should not be NULL as it is started earlier
            if ( !iCmdHandlers )
                {
                LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
                StartCommandHandlersL iCmdHandlers false" )
                iCmdHandlers = new ( ELeave )
                    CArrayPtrFlat<CSatCommandHandler>( cmdCount );
                }

            // Add all found command handlers to list
            for ( TInt i = 0; i < cmdCount; i++ )
                {
                const TUid KImplementationUid(
                    satCommandImplementations[i]->ImplementationUid() );

                // Check important command handlers. They are already started
                if ( KImplementationUid != KSetUpEventListUid )
                    {
                    CSatCommandHandler* cmd =
                        CSatCommandHandler::NewL( KImplementationUid, this );
                    cmd->Start();
                    CleanupStack::PushL( cmd );
                    iCmdHandlers->AppendL( cmd );
                    CleanupStack::Pop( cmd );
                    }
                }
            }
        CleanupStack::PopAndDestroy( &satCommandImplementations );
        
        // Notify TSY about readiness i.e. all nofies are sent
        // Done only once in startup phase
        TInt ret = iUSatAPI->UsatClientReadyIndication();
        LOG2( NORMAL, "SATENGINE: UsatClientReadyIndication() gives %d", ret )
        }

    // Else check that there are no command handlers created and
    // BT SAP is not active
    else if ( !iCmdHandlers && !( SystemState().IsBtSapActive() ) )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
             no command handlers created and BT SAP is not active" )
        // This is the case when
        // SAP is first enabled and then disabled
        if ( !iUSatAPI->IsRSatConnected() )
            {
            User::LeaveIfError( iUSatAPI->Connect( *iMultiModeApi ) );
            }

        // Create command handlers.
        RImplInfoPtrArray satCommandImplementations;
        CleanupStack::PushL( 
            TCleanupItem( CleanupPointerArray, &satCommandImplementations ) );
        REComSession::ListImplementationsL( KSatInterfaceDefinitionUid,
            satCommandImplementations );

        // Container for commands
        const TInt cmdCount( satCommandImplementations.Count() );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
            Command handler count: %d", cmdCount )

        // Check are there any command implementations
        if ( cmdCount > 0 )
            {
            iCmdHandlers =
                new ( ELeave ) CArrayPtrFlat<CSatCommandHandler>( cmdCount );

            // Add all found command handlers to list
            for ( TInt i = 0; i < cmdCount; i++ )
                {
                const TUid KImplementationUid(
                    satCommandImplementations[i]->ImplementationUid() );
                CSatCommandHandler* cmd =
                    CSatCommandHandler::NewL( KImplementationUid, this );
                cmd->Start();
                CleanupStack::PushL( cmd );
                iCmdHandlers->AppendL( cmd );
                CleanupStack::Pop( cmd );
                }
            }
        else
            {
            LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
            StartCommandHandlersL No commands found" )
            // No commands, remove SAT Icon from shell
            iSatUiHandler.ShellController().RemoveSatUiL();
            }
        CleanupStack::PopAndDestroy( &satCommandImplementations );
        }
    else
        {
        LOG( NORMAL, " Not starting any command handlers" )
        }

    // Create SAP State observer.
    if ( !iSapObserver )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
        iSapObserver false" )
        // Create SAP change observer only if there are command handlers.
        iSapObserver = CSatSAPChangeObserver::NewL( *this );
        }

    // Create Profile change observer.
    if ( !iProfileObserver )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StartCommandHandlersL \
        iProfileObserver false" )
        iProfileObserver = CSatProfileChangeObserver::NewL( *this );
        }

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::StartCommandHandlersL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::StopCommandHandlersL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::StopCommandHandlers()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::StopCommandHandlers calling" )

    if ( iCmdHandlers )
        {
         // Remove every command handler from event observers
        const TInt count( iCmdHandlers->Count() );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::StopCommandHandlers \
              Command handler count: %d", count )
        for ( TInt j = 0; j < count; j++ )
            {
            CSatCommandHandler* cmd = iCmdHandlers->At( j );
            UnregisterEventObserver( cmd );
            }

        // Delete command handlers
        iCmdHandlers->ResetAndDestroy();

        delete iCmdHandlers;
        iCmdHandlers = NULL;
        }

    // This must be called when all ECom Plug-Ins has been deleted. Otherwise
    // there might be memory leaks.
    REComSession::FinalClose();

    // Reset ClientService request handlers since there are no command handlers
    // to handle client responses. If SAT UI / Proactive command is executing
    // while Bluetooth SAP is enabled, SAT UI sends ClientResponse and that must
    // be preveted
    if ( iServiceReqs )
        {
        iServiceReqs->Reset();
        }

    // Close all reserved BIP Data channels
    if ( iBipUtils )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StopCommandHandlers \
        iBipUtils true" )
        iBipUtils->CloseAllChannels();
        }

    // Cancel all event monitors
    if ( iEventMonitors )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::StopCommandHandlers \
        iEventMonitors true" )
        iEventMonitors->CancelAllMonitors();
        }

#ifdef SAT_USE_DUMMY_TSY
    // Close ATSY. This is needed when testin BT SAP in emulator
    if ( iUSatAPI )
        {
        iUSatAPI->Close();
        }
#endif

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::StopCommandHandlers exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::Event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::Event( TInt aEvent )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::Event calling" )

    if ( MSatUtils::ECmccSimDetected == aEvent )
        {
        LOG( NORMAL, "SATENGINE:   Event: ECmccSimDetected" )
        iIsCmccSim = ETrue;

        // Update default name read in ConstructL.
        TRAPD( err, CreateSatAppNameL( KSatCmccTitle ) );
        LOG2( NORMAL, "SATENGINE:   Error: %i", err )
        if ( KErrNone == err )
            {
            // Notify application name update.
            NotifyEvent( EApplNameUpdated );
            }
        }
    else if ( MSatUtils::ESatUiClosed == aEvent )
        {
        LOG( NORMAL, "SATENGINE:   Event: ESatUiClosed" )
        // Reset service requests. If this is not done,
        // Some command may try to send data to UiClient,
        // even if there is no request.
        const TInt reqs( iServiceReqs->Count() );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::Event reqs: %i", 
              reqs )
        for ( TInt i = 0; i < reqs; i++ )
            {
            iServiceReqs->At( i )->Reset();
            }
        }
    else
        {
        LOG( NORMAL, "SATENGINE:   Unexpected event" )
        }

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::NotifyUiEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::NotifyUiEvent(
    TInt aEventId, TInt aEventStatus, TInt aEventError )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NotifyUiEvent calling" )

    MSatUiSession* session = iSatUiHandler.UiSession();
    if ( session )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::NotifyUiEvent \
        session" )
        session->UiEventNotification( aEventId, aEventStatus, aEventError );
        session = NULL;
        }

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NotifyUiEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::USatAPI
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatApi& CSatCommandContainer::USatAPI()
    {
    LOG( DETAILED, "SATENGINE: CSatCommandContainer::USatAPI calling" )

    __ASSERT_ALWAYS( iUSatAPI, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( DETAILED, "SATENGINE: CSatCommandContainer::USatAPI exiting" )
    return *iUSatAPI;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::SystemState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatSystemState& CSatCommandContainer::SystemState()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::SystemState calling-exiting" )
    return *iSystemState;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::SatUiHandler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatSUiClientHandler& CSatCommandContainer::SatUiHandler()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::SatUiHandler calling-exiting" )
    return iSatUiHandler;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::RegisterL
// Forwards the registeration to TSatEventMediator
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::RegisterL(
    MSatEventObserver* aObserver, // Event observer
    TSatEvent aEvent ) // Event which is observerd
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::RegisterL calling" )

    iEventMediator.RegisterL( aObserver, aEvent );

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::RegisterL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::NotifyEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::NotifyEvent( TSatEvent aEvent )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NotifyEvent calling" )

    iEventMediator.Notify( aEvent );

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::NotifyEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::UnregisterEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::UnregisterEvent(
    MSatEventObserver* aObserver, // Observer of the event
    TSatEvent aEvent ) // Event
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::UnregisterEvent calling" )

    iEventMediator.Unregister( aObserver, aEvent );

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::UnregisterEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::UnregisterEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::UnregisterEventObserver(
    MSatEventObserver* aObserver )
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::UnregisterEventObserver calling" )

    iEventMediator.Unregister( aObserver );

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::UnregisterEventObserver exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::NumberOfExecutingCommandHandlers
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatCommandContainer::NumberOfExecutingCommandHandlers()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::NumberOfExecutingCommandHandlers \
        calling" )

    TInt result( 0 );
    TInt count( 0 );
    if ( iCmdHandlers )
        {
        LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::NumberOfExecutingCommandHandlers \
        iCmdHandlers true" )
        // Gets the count of command handlers
        // if the list is already available.
        count = iCmdHandlers->Count();
        }
    LOG2( NORMAL, "CSatCommandContainer::NumberOfExecutingCommandHandlers \
          count: %i", count )
    for ( TInt i = 0; i < count; i++ )
        {
        CSatCommandHandler* handler = iCmdHandlers->At( i );
        __ASSERT_ALWAYS( handler, PanicSatEngine( ESatEngineNullPointer ) );

        // If command handler is not active then it has received command
        // from sim.
        if ( handler->IsCommandExecuting() )
            {
            result++;
            }
        }

    LOG2( NORMAL,
        "SATENGINE: CSatCommandContainer::NumberOfExecutingCommandHandlers \
        exiting: %i", result )
    return result;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::SatAppName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CSatCommandContainer::SatAppName() const
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::SatAppName calling-exiting" )
    return *iSatAppName;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::BipApnName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CSatCommandContainer::BipApnName() const
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::BipApnName calling-exiting" )
    return iSatBipName;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::RestoreSatAppNameL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::RestoreSatAppNameL()
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::RestoreSatAppNameL calling" )
    if ( !iIsCmccSim )
        {
        LOG( NORMAL,
            "SATENGINE: CSatCommandContainer::RestoreSatAppNameL name reset" )
        CreateSatAppNameL( KSatLogTitle );
        }
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::RestoreSatAppNameL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::SetSatAppNameL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::SetSatAppNameL(
    const TDesC& aName ) // New sat app name
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::SetSatAppNameL calling" )

    if ( !iIsCmccSim )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::SetSatAppNameL \
        iIsCmccSim false" )
        HBufC* temp = aName.AllocL();
        delete iSatAppName;
        iSatAppName = temp;
        }

    LOG( NORMAL, "SATENGINE: CSatCommandContainer::SetSatAppNameL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::RefreshSubSessions
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const RPointerArray<MSatRefreshSubSession>&
    CSatCommandContainer::RefreshSubSessions()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::RefreshSubSessions calling-exiting" )
    return iSessions.RefreshSubSessions();
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::RegisterServiceRequestL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::RegisterServiceRequestL(
    TSatServerRequest aRequest,
    TSatServerRequest aResponse,
    MSatCommand* aCommand )
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::RegisterServiceRequestL calling" )

    // First, check is there service request handler already with the same
    // request.If it exists, change command handler
    const TInt reqs( iServiceReqs->Count() );
    TBool requestHandled( EFalse );
    LOG2( NORMAL, "CSatCommandContainer::RegisterServiceRequestL \
          reqs: %i", reqs )
    for ( TInt i = 0; i < reqs; i++ )
        {
        CSatClientServiceReq* req = iServiceReqs->At( i );
        __ASSERT_ALWAYS( req, PanicSatEngine( ESatEngineNullPointer ) );

        if ( req->IsMyRequest( aRequest ) )
            {
            req->SetCommandHandler( aCommand );
            requestHandled = ETrue;
            i = reqs; // to end for-loop
            }
        }

    // Check was service request found
    if ( !requestHandled )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
        RegisterServiceRequestL requestHandled false" )
        // Not found, add new
        CSatClientServiceReq* servReq = CSatClientServiceReq::NewL( 
            aRequest, aResponse, aCommand, *this );
        CleanupStack::PushL( servReq );
        iServiceReqs->AppendL( servReq );
        CleanupStack::Pop( servReq );
        }

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::RegisterServiceRequestL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::ServiceRequests
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CArrayPtrFlat<CSatClientServiceReq>*
    CSatCommandContainer::ServiceRequests()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::ServiceRequests calling-exiting" )
    return iServiceReqs;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::EventMonitors
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatEventMonitorContainer* CSatCommandContainer::EventMonitors()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::EventMonitors calling-exiting" )
    return iEventMonitors;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::SimSubscriberId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatSSimSubscriberId& CSatCommandContainer::SimSubscriberId()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::SimSubscriberId calling-exiting" )
    return *iSimSubscriberId;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::BipUtils
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatBIPUtils& CSatCommandContainer::BipUtils()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::BipUtils calling-exiting" )
    return *CreateAndGetBIPUtils();
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::BipEventNotifier
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatBIPEventNotifier& CSatCommandContainer::BipEventNotifier()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::BipEventNotifier calling-exiting" )
    return *CreateAndGetBIPUtils();
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::MultiModeApi
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatMultiModeApi& CSatCommandContainer::MultiModeApi()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::MultiModeApi calling-exiting" )
    return *iMultiModeApi;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CustomApi
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMmCustomAPI* CSatCommandContainer::CustomApi()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::CustomApi calling-exiting" )
    return iMultiModeApi->CustomApi();
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::RaiseSatEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatCommandContainer::RaiseSatEvent( const TDesC8& aData )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::RaiseSatEvent calling" )

    CSatMediatorEventProvider* medEvent =
        CreateAndGetSatMediatorEventProvider();
    TInt retVal( KErrNone );
    if ( medEvent )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::RaiseSatEvent \
        medEvent true" )
        retVal = medEvent->RaiseSatEvent( aData );
        }

    LOG2( NORMAL,"SATENGINE: CSatCommandContainer::RaiseSatEvent exiting, \
          retVal: %d", retVal )
    return retVal;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CoverUiSupported()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatCommandContainer::CoverUiSupported()
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::CoverUiSupported calling" )

    CSatMediatorEventProvider* medEvent = CreateAndGetSatMediatorEventProvider();
    TBool supported( EFalse );
    if ( medEvent )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::CoverUiSupported \
        medEvent true" )
        supported = medEvent->CoverUiSupported();
        }

    LOG2( NORMAL, "SATENGINE: CSatCommandContainer::CoverUiSupported exiting,\
          supported: %d", supported )
    return supported;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CreateAsyncToSyncHelperL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MSatAsyncToSync* CSatCommandContainer::CreateAsyncToSyncHelper()
    {
    MSatAsyncToSync* wrapper = new CSatSActiveWrapper();
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
            CreateAsyncToSyncHelperL calling - exiting" )
    return wrapper;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::StateChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::StateChanged( const TInt aValue )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::StateChanged calling" )
    CheckStartupState( aValue );
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::StateChanged exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CSatCommandContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCommandContainer::CSatCommandContainer(
    MSatSSessions& aSessions,
    TSatEventMediator& aEventMediator,
    MSatSUiClientHandler& aSatUiHandler ) :
    iSessions( aSessions ),
    iEventMediator( aEventMediator ),
    iSatUiHandler( aSatUiHandler ),
    iSatMediatorEvent( NULL )
    {
    LOG( NORMAL,
       "SATENGINE: CSatCommandContainer::CSatCommandContainer calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::ConstructL()
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::ConstructL calling" )

    iIsCmccSim = EFalse;
    CreateSatAppNameL( KSatLogTitle );

    // Register for ui events in order to keep track if user or command
    // has launched the ui.
    iEventMediator.RegisterL( this, MSatUtils::ESatUiClosed );
    iEventMediator.RegisterL( this, MSatUtils::ECmccSimDetected );

    // Set these pointers to NULL because their value is checked
    // before initialization...
    iSapObserver = NULL;
    iCmdHandlers = NULL;
    iProfileObserver = NULL;

    // Create pointer array for service request handlers
    iServiceReqs = new( ELeave )CArrayPtrFlat<CSatClientServiceReq>( 1 );

    iMultiModeApi = CSatMultiModeApi::NewL();
    
    // By default, we use extended errors
    iMultiModeApi->RaiseErrorGranularity();

    iUSatAPI = new( ELeave )TUSatAPI();
    TInt error = iUSatAPI->Connect( *iMultiModeApi );

    // Create SystemState.
    iSystemState = 
            TSatSystemStateFactory::CreateSystemStateL( *iMultiModeApi );

    // Create event monitor container.
    iEventMonitors = CSatEventMonitorContainer::NewL( *iUSatAPI, *this );

    // Check did the conneciton to phone passed.
    if ( KErrNone == error )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::ConstructL connection \
             to phone" )
        iStartupPhase = ETrue;
        // Start important plugins
        StartImportantCommandHandlersL();

        // Start listening system's state
        iStartupChangeObserver =
            TSatSystemStateFactory::CreateStartupChangeNotifierL( *this );
        CheckStartupState( iStartupChangeObserver->GetValueInt() );

        if ( iStartupPhase )
            {
            LOG( NORMAL, "SATENGINE: CSatCommandContainer::ConstructL \
            iStartupPhase true" )
            iStartupChangeObserver->NotifyChangeL();
            }

        }

    // Create Subscriber id handler.
    iSimSubscriberId = 
              CSatSSimSubscriberId::NewL( *iMultiModeApi, iEventMediator );
    iSimSubscriberId->Start();

    // Create SatMediatorEventProvider. Will register SAT Cover UI events
    // if cover UI is supported.
    CreateAndGetSatMediatorEventProvider();

    TSecurityPolicy readPolicy( ECapabilityReadUserData );
    TSecurityPolicy writePolicy( ECapabilityWriteUserData );

    // Define properties in P&S key for icon support
    error = RProperty::Define( KPSUidSatServerInternal,
        KSatAppClosedUsingEndKey, RProperty::EInt, readPolicy, writePolicy );
        
    LOG2( NORMAL, "CSatUiActionPerformer::ConstructL \
        define KSatAppClosedUsingEndKey and return: %d", error )
    
    // Publish P&S key for icon support
    error = RProperty::Set( KPSUidSatServerInternal, KSatAppClosedUsingEndKey,
        KSatAppTerminatedNormal );
    
    LOG2( SIMPLE, "SATENGINE: CSatCommandContainer::ConstructL \
          P&S key: %i", KSatAppTerminatedNormal )
        
    LOG2( NORMAL, "CSatUiActionPerformer::ConstructL \
        publish KSatAppClosedUsingEndKey and return: %d", error )
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::ConstructL exiting" )
    }
// -----------------------------------------------------------------------------
// CSatCommandContainer::CreateSatAppNameL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::CreateSatAppNameL( const TDesC& aResourceId )
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::CreateSatAppNameL calling" )
    delete iSatAppName;
    iSatAppName = NULL;
    const TBool textResolver = HbTextResolverSymbian::Init( 
        KSatServerRsc, KResourceDrive );
    LOG2(NORMAL,"SATENGINE: CSatCommandContainer::\
        CreateSatAppNameL textResolver = %d", textResolver ) 
    LOG2(NORMAL,"SATENGINE: CSatCommandContainer::\
        CreateSatAppNameL aResourceId = %S", &aResourceId )

    iSatAppName = HbTextResolverSymbian::LoadL( aResourceId );
    LOG2(NORMAL,"SATENGINE: CSatCommandContainer::\
        CreateSatAppNameL iSatAppName = %S", iSatAppName )
    iSatBipName.Copy( SatAppName() );
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::CreateSatAppNameL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CreateAndGetBIPUtilsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSatBIPUtils* CSatCommandContainer::CreateAndGetBIPUtils()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::CreateAndGetBIPUtils calling" )
    // If pointer is null, create new
    if ( !iBipUtils )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::CreateAndGetBIPUtilsL \
            Creating BIP Utils" )
        // TRAP this, We will panic, if this is NULL
        TInt err( KErrNone );
        TRAP( err, iBipUtils = CSatBIPUtils::NewL( *this ) );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::\
                CreateAndGetBIPUtilsL BIP Utils creation status: %i", err )
        }

    __ASSERT_ALWAYS( iBipUtils, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::CreateAndGetBIPUtils exiting" )
    return iBipUtils;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CreateAndGetMediatorEventProvider
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSatMediatorEventProvider* CSatCommandContainer::
                                        CreateAndGetSatMediatorEventProvider()
    {
    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::CreateAndGetSatMediatorEventProvider \
        calling" )

    // If pointer is null, create new
    if ( !iSatMediatorEvent )
        {
        LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
                        CreateAndGetSatMediatorEventProvider \
                            Creating CreateAndGetSatMediatorEventProvider" )
        // TRAP this, We will panic, if this is NULL
        TInt err( KErrNone );
        TRAP( err, iSatMediatorEvent = CSatMediatorEventProvider::NewL() );
        LOG2( NORMAL, "SATENGINE: CSatCommandContainer::\
            CreateAndGetCSatMediatorEventProvider status: %i", err )
        }

    __ASSERT_ALWAYS(
        iSatMediatorEvent, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( NORMAL,
        "SATENGINE: CSatCommandContainer::CreateAndGetSatMediatorEventProvider \
        exiting" )
    return iSatMediatorEvent;
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::StartImportantCommandHandlersL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::StartImportantCommandHandlersL()
    {
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
        StartImportantCommandHandlersL calling" )

    // Create command handlers.
    RImplInfoPtrArray satCommandImplementations;
    CleanupStack::PushL( 
        TCleanupItem( CleanupPointerArray, &satCommandImplementations ) );
    REComSession::ListImplementationsL( KSatInterfaceDefinitionUid,
        satCommandImplementations );
    
    // Container for commands
    const TInt cmdCount( satCommandImplementations.Count() );
    LOG2( NORMAL, "SATENGINE: CSatCommandContainer::\
        StartImportantCommandHandlersL Command handler count: %d", cmdCount )

    // Check are there any command implementations
    if ( cmdCount > 0 )
        {
        iCmdHandlers
            = new ( ELeave ) CArrayPtrFlat<CSatCommandHandler>( cmdCount );

        CSatCommandHandler* setUpEventListCmd = NULL;
        // Add all important command handlers to list.

        // This will return KErrNotFound if the implementation is not included
        // in configurations
        TRAPD( err, setUpEventListCmd =
            CSatCommandHandler::NewL( KSetUpEventListUid, this ) );

        // Check does the command exist.
        if ( setUpEventListCmd && KErrNone == err )
            {
            LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
            StartImportantCommandHandlersL KErrNone == err" )
            setUpEventListCmd->Start();
            CleanupStack::PushL( setUpEventListCmd );
            iCmdHandlers->AppendL( setUpEventListCmd );
            CleanupStack::Pop( setUpEventListCmd );
            }
        }
    
    CleanupStack::PopAndDestroy( &satCommandImplementations );
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::\
        StartImportantCommandHandlersL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandContainer::CheckStartupState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCommandContainer::CheckStartupState( const TInt aValue )
    {
    LOG2( NORMAL, "SATENGINE: CSatCommandContainer::CheckStartupState calling\
        with value %d", aValue )
    if ( ESwStateNormalRfOn == aValue )
        {
        TRAP_IGNORE( StartCommandHandlersL() )
        iStartupChangeObserver->CancelNotify();
        // Startup phase is over
        iStartupPhase = EFalse;
        }
    LOG( NORMAL, "SATENGINE: CSatCommandContainer::CheckStartupState exiting" )
    }

// End Of File
