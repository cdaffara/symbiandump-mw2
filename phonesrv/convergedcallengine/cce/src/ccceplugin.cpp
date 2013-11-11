/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles plugin
*
*/


//  INCLUDE FILES
#include <badesca.h>
#include "ccceplugin.h"
#include "cconvergedcallprovider.h"
#include "cccelogger.h"
#include "cccecallcontainer.h"
#include "cccetransfercontroller.h"
#include "mccpcall.h"
#include "mccpcscall.h"
#include "mcceobserver.h"
#include "cccecall.h"
#include "mccecallobserver.h"
#include "ccceconferencecall.h"

#include "mccpplugindeathobserver.h" 
#include "mccpssobserver.h"
#include "mccessobserver.h"
#include "mccptransferprovider.h"
#include "tccecallindex.h"

#include <featmgr.h>
#include "mccpdtmfprovider.h"



// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// NewL()
// -----------------------------------------------------------------------------
//
CCCEPlugin* CCCEPlugin::NewL( const TUid& aPluginUID,
                              MCCEObserver& aObserver,
                              CCCECallContainer& aCallContainer,
                              MCCPPluginDeathObserver& aDeathObserver,
                              CCCETransferController& aTransferController )
    {
    CCCEPlugin* self = new (ELeave) CCCEPlugin( aPluginUID,
                                                aObserver,
                                                aCallContainer,
                                                aDeathObserver,
                                                aTransferController );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCCEPluginManager()
// -----------------------------------------------------------------------------
//
CCCEPlugin::CCCEPlugin( const TUid& aPluginUID,
                        MCCEObserver& aObserver,
                        CCCECallContainer& aCallContainer,
                        MCCPPluginDeathObserver& aDeathObserver,
                        CCCETransferController& aTransferController )
                        : iObserver( aObserver ),
                          iCallContainer( aCallContainer ),
                          iTransferController( aTransferController ),
                          iDeathObserver( aDeathObserver ),
                          iPluginId( aPluginUID ),
                          iReleaseWhenIdle( ETrue )
    {
        
    }

// -----------------------------------------------------------------------------
// ConstructL()
// -----------------------------------------------------------------------------
//
void CCCEPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();

    iPlugin = CConvergedCallProvider::NewL( iPluginId );

    if(iPlugin)
        {
        CCELOGSTRING("CCCEPlugin::GetPluginL: Initialize" );
        iPlugin->InitializeL( *this, *this );
        }
    }

// -----------------------------------------------------------------------------
// ~CCCEPluginManager()
// -----------------------------------------------------------------------------
//
CCCEPlugin::~CCCEPlugin()
    {
    delete iPlugin;
    FeatureManager::UnInitializeLib();
    }

// -----------------------------------------------------------------------------
// Dtmf
// -----------------------------------------------------------------------------
//
MCCPDTMFProvider& CCCEPlugin::GetDtmfProviderL(
    const MCCPDTMFObserver& aObserver )
    {
    if( !iDtmfProvider )
        {
        iDtmfProvider = iPlugin->DTMFProviderL( aObserver );

        if( !iDtmfProvider )
            {
            CCELOGSTRING("CCCEPlugin::GetDtmfProviderL: Plugin does not support dtmf, leaving with KErrNotSupported" );
            // maybe api should be changed to leave with error code instead of returning null
            User::Leave(KErrNotSupported);
            }
        }

    return *iDtmfProvider;
    }

// -----------------------------------------------------------------------------
// Return protocol interface object of wanted type.
// -----------------------------------------------------------------------------
//
CConvergedCallProvider* CCCEPlugin::GetPluginL() const
    {
    return iPlugin;
    }

// -----------------------------------------------------------------------------
// Primary emergency call objects are created construct time
// -----------------------------------------------------------------------------
//
void CCCEPlugin::InitialiseEmergencyCallL(MCCPCallObserver& aEmergencyCallObserver)
    {
    iEmergencyCall = 
        iPlugin->NewEmergencyCallL( 0, KNullDesC, aEmergencyCallObserver );
    }
    
// -----------------------------------------------------------------------------
// Get emergency call handle.
// -----------------------------------------------------------------------------
//
MCCPEmergencyCall* CCCEPlugin::GetEmergencyCall() const
    {
    return iEmergencyCall;
    }
    
// ---------------------------------------------------------------------------
// CCCEPlugin::Type()
// ---------------------------------------------------------------------------
//
TUid CCCEPlugin::Type() const
    {
    return iPluginId;
    }
    
// ---------------------------------------------------------------------------
// Returns ETrue if plugin can be relesed if idle
// ---------------------------------------------------------------------------
//
TBool CCCEPlugin::ReleaseWhenIdle()
    {
    return iReleaseWhenIdle;
    }

// ---------------------------------------------------------------------------
// Set ETrue if plugin should stay permanently in memory
// ---------------------------------------------------------------------------
//
void CCCEPlugin::SetReleaseWhenIdle(TBool aReleaseWhenIdle )
    {
    iReleaseWhenIdle = aReleaseWhenIdle;
    }    
    
// ---------------------------------------------------------------------------
// From class MCCPCSObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::MoCallCreated( MCCPCall& aCall )
    {
    CCELOGSTRING("CCCEPlugin::MoCallCreated" );

    TBool callAllowed = EFalse;
    
    //CCELOGSTRING2("  cceparams->iCallType = %d", aCall->Parameters().CallType() );
    
    callAllowed = ( IsCallAllowed() >= 0 );     
    
    
    CCCECall* call = NULL;
    if( callAllowed )
        {
        call = iCallContainer.GetNextFreeCall();
        }
        
    if( call )
        {
        TRAPD( error,
            call->SetConvergedCallL( &aCall, iPluginId ) );

        // set observer for the passed call
        TRAP_IGNORE( aCall.AddObserverL(*call) );

        // Error handling
        if( error )
            {
            CCELOGSTRING(
               "CCCEPlugin::MoCallCreated:Canceled" );
            aCall.Cancel();
            call->Release();
            }
        else
            {
            CCELOGSTRING(
               "CCCEPlugin::MoCallCreated:Forwarded" );
            // Sets new call index. Normally the call itself
            // sets the value, but in the case when external client has
            // created the call we need to set it here, because
            // the call is already ongoing.
            call->SetNewCallIndex();
            iObserver.MOCallCreated( *call );        
            }
            
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::MoCallCreated:Error, CCECall not found" );
        aCall.Cancel();
        iCallContainer.ReleaseCall( aCall, iPluginId );
        }
    }

// ---------------------------------------------------------------------------
// From class MCCPCSObserver
// CCCEPlugin::DataPortName()
// ---------------------------------------------------------------------------
//
void CCCEPlugin::DataPortName( TName& aPortName )
    {
    CCELOGSTRING( "CCCEPlugin::DataPortName" );
    iObserver.DataPortName( aPortName );
    }

// ---------------------------------------------------------------------------
// From class MCCPCSObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::ConferenceCallCreated( MCCPConferenceCall& aConferenceCall )
    {
    CCELOGSTRING("CCCEPlugin::ConferenceCallCreated IN" );
    if( !iCallContainer.ConferenceCall().SetOngoingConferenceCall( 
        aConferenceCall, iPluginId ) )
        {
        iObserver.ConferenceCallCreated( iCallContainer.ConferenceCall() );
        }
    else
        {
        iPlugin->ReleaseConferenceCall( aConferenceCall );
        }
    
    CCELOGSTRING("CCCEPlugin::ConferenceCallCreated OUT" );
    }

// ---------------------------------------------------------------------------
// CCCEPlugin::IsCallAllowed()
// ---------------------------------------------------------------------------
//
TInt CCCEPlugin::IsCallAllowed() const
    {
    CCCECall* call;
    TInt activeNonConferenceCalls = 0;
    CCPCall::TCallState callState;
    
    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = iCallContainer.GetCall( i );
        
        if( call->IsInUse() &&
            !iCallContainer.ConferenceCall().IsPartOfConference( *call ) )
            {
            callState = call->State();
            
            CCELOGSTRING2( "CCCEPlugin::IsCallAllowed:Active call found: State = %d",
                callState );
            
            switch( callState )
                {
                case CCPCall::EStateQueued:
                case CCPCall::EStateRinging:
                case CCPCall::EStateConnecting:
                    {
                    CCELOGSTRING( "CCCEPlugin::IsCallAllowed: KErrGeneral" );
                    // No other calls allowed
                    }
                    return KErrGeneral;
                case CCPCall::EStateDialling:
                    {
                    // Dialling -> Cancel it
                    // Telephone UI spec: chapter 3.1.4
                    call->HangUp();
                    }
                    break;
                case CCPCall::EStateDisconnecting:
                    {
                    // Disconnecting -> do nothing
                    }
                    break;
                default:
                    {
                    // Active/Hold -> Call can be on waiting state
                    activeNonConferenceCalls++;
                    }
                    break;
                }
            
            if( activeNonConferenceCalls >= KCCEMaxNumberOfCalls )
                {
                CCELOGSTRING( "CCCEPlugin::IsCallAllowed: KErrOverflow" );
                return KErrOverflow;
                }

            }
        }
    // In addition to checks made here call should not be allowed 
    // when emergency call is ongoing. This is done in 
    // CCCECallContainer::GetNextFreeCall()
        
    CCELOGSTRING( "CCCEPlugin::IsCallAllowed: ETrue" );
    return activeNonConferenceCalls;
    }
    
// ---------------------------------------------------------------------------
// CCCEPlugin::IsDataCallAllowed()
// ---------------------------------------------------------------------------
//
TBool CCCEPlugin::IsDataCallAllowed() const
    {
    CCELOGSTRING( "CCCEPlugin::IsDataCallAllowed()" );
    CCCECall* call = NULL;
    TInt callCount = 0;
    CCPCall::TCallState callState;
    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = iCallContainer.GetCall( i );
        callState = call->State();
        if( CCPCall::EStateIdle != callState &&
            !iCallContainer.ConferenceCall().IsPartOfConference( *call ) )
            {
            callCount++;
            }
        }
    
    if( iCallContainer.ConferenceCall().State() != 
        MCCEConferenceCallObserver::ECCEConferenceIdle )
        {
        callCount++;
        }

    // In addition to checks made here call should not be allowed 
    // when emergency call is ongoing. This is done in 
    // CCCECallContainer::GetNextFreeCall()

    CCELOGSTRING2( "CCCEPlugin::IsDataCallAllowed() callCount=%d", callCount );       
    // Data call cannot be allowed if there is three or more calls ongoing
    return ( callCount < KCCEMaxNumberOfCalls );
    }
    
// ---------------------------------------------------------------------------
// From class MCCPObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::ErrorOccurred( const TCCPError aError )
    {
    CCELOGSTRING2("CCCEPlugin::ErrorOccurred: Error = %d", aError  );
    
    switch (aError)
        {
        case ECCPIncomingCallFailed:
            {
            if( iReleaseWhenIdle )
                {
                iDeathObserver.CCPPluginInitialisationFailed(iPluginId, aError);
                }
            }
            break;
        case ECCPRegistrationFailed:
            {
            iDeathObserver.CCPPluginInitialisationFailed(iPluginId, aError);   
            }
            break;
        default:
            {
            CCELOGSTRING("CCCEPlugin::ErrorOccurred: Default");
            }
            break;  
        }
    }

// ---------------------------------------------------------------------------
// From class MCCPObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::IncomingCall( MCCPCall* aCall, MCCPCall& aTempCall )
    {
    CCELOGSTRING("CCCEPlugin::IncomingCall():IN");
    CCELOGSTRING3("    originalcall: aTempCall=0x%x transfercall: aCall=0x%x", &aTempCall, aCall );
  
    if( !aCall || aCall == &aTempCall )
        {
        CCELOGSTRING("CCCEPlugin::IncomingCall(MCCPCall*,MCCPCall&) - Error: Invalid parameters!");
        return;
        }
    
    iTransferController.TransferCallIncoming( aCall, &aTempCall );
    CCELOGSTRING("CCCEPlugin::IncomingCall():OUT");
    }

// ---------------------------------------------------------------------------
// From class MCCPObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::IncomingCall( MCCPCall* aCall )
    {
    CCELOGSTRING( "CCCEPlugin::IncomingCall" );

    if( !aCall )
        {
        CCELOGSTRING("CCCEPlugin::IncomingCall(MCCPCall*) - Error: incoming call pointer was null!");
        return;
        }
 
    CCCECall* call = NULL;
    TInt nonConferenceCalls = IsCallAllowed();

    if( nonConferenceCalls >= 0 )
        {
        call = iCallContainer.GetNextFreeCall();
        }

    if( call )
        {
        TRAPD( error,
            call->SetConvergedCallL( aCall, iPluginId ) );

        // Order call to ringing state
        // Requires no active calls and conference call must be idle
        if( nonConferenceCalls == 0 && 
            iCallContainer.ConferenceCall().State() ==
            MCCEConferenceCallObserver::ECCEConferenceIdle )
            {
            CCELOGSTRING( "CCCEPlugin::IncomingCall:No active calls" );
            error = aCall->Ringing();
            }
        else
            {
            CCELOGSTRING( "CCCEPlugin::IncomingCall:One active call" );
            error = aCall->Queue();
            }
            
        // Error handling
        if( error )
            {
            CCELOGSTRING2( "CCCEPlugin::IncomingCall: Error = %d", error );
            aCall->Reject();
            call->Release();
            }
        else
            {
            CCELOGSTRING( "CCCEPlugin::IncomingCall:No errors occurred-> inform phoneengine" );
            iObserver.IncomingCall( *call );
            }
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::IncomingCall:Error, CCECall not found" );
        aCall->Reject();
        iCallContainer.ReleaseCall( *aCall, iPluginId );
        }
    }

// ---------------------------------------------------------------------------
// From class MCCPObserver
// ---------------------------------------------------------------------------
//
void CCCEPlugin::CallCreated( MCCPCall* aNewTransferCall,
                              MCCPCall* aOriginator,
                              TBool aAttented )
    {
    CCELOGSTRING2("CCCEPlugin::CallCreated():IN aAttended=%d",aAttented);
    CCELOGSTRING3("     TransferCall: aNewTransferCall=0x%x OriginalCall: aOriginator=0x%x",
            aNewTransferCall, aOriginator );
    
    if ( !aNewTransferCall || aNewTransferCall == aOriginator )
        {
        CCELOGSTRING("CCCETransferController::HandleTransferRequest() Error: Invalid parameters!");
        return;
        }

    if ( aAttented ) 
        {
        iTransferController.HandleRemoteTransferRequest( aNewTransferCall,
            aOriginator, aAttented );
        }
    else
        {
        TInt nonConferenceCalls = IsCallAllowed();
        
        if ( nonConferenceCalls == 0 || nonConferenceCalls == 1 )
            {
            iTransferController.HandleRemoteTransferRequest( aNewTransferCall,
                aOriginator, aAttented );
            }
        else
            {
            // Reject unattended transfer 
            CCELOGSTRING( "CCCEPlugin::CallCreated(): reject transfer" );
            CCCECall* originator = iCallContainer.GetCall( aOriginator );
            TRAP_IGNORE( aOriginator->TransferProviderL(*originator)->AcceptTransfer(EFalse) );
            iCallContainer.ReleaseCall( *aNewTransferCall );
            }       
        }
    
    CCELOGSTRING( "CCCEPlugin::CallCreated():OUT" );
    }
    

// ---------------------------------------------------------------------------
// CCCEPlugin::BarringEventOccurred
// ---------------------------------------------------------------------------
//
void CCCEPlugin::BarringEventOccurred( const MCCPSsObserver::TCCPSsBarringEvent aBarringEvent )
    {
    CCELOGSTRING2("CCCEPlugin::BarringEventOccurred():IN event=%d",aBarringEvent );

    if( iCallContainer.SsObserver() )
        {
        // one to one mapping at the moment can be casted directly, change if mapping gets out of sync
        iCallContainer.SsObserver()->BarringEventOccurred(
            (MCCESsObserver::TCCESsBarringEvent)aBarringEvent );
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::BarringEventOccurred: Event ignored no observer is set!" );
        }

    CCELOGSTRING( "CCCEPlugin::BarringEventOccurred:OUT" );
    }
                      

// ---------------------------------------------------------------------------
// CCCEPlugin::CLIEventOccurred
// ---------------------------------------------------------------------------
//
void CCCEPlugin::CLIEventOccurred( const MCCPSsObserver::TCCPSsCLIEvent aCallLineEvent )
    {
    CCELOGSTRING2("CCCEPlugin::CLIEventOccurred():IN event=%d",aCallLineEvent );

    if( iCallContainer.SsObserver() )
        {
        // one to one mapping at the moment can be casted directly, change if mapping gets out of sync
        iCallContainer.SsObserver()->CLIEventOccurred(
                    (MCCESsObserver::TCCESsCLIEvent)aCallLineEvent );
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::CLIEventOccurred: Event ignored no observer is set!" );
        }

    CCELOGSTRING( "CCCEPlugin::CLIEventOccurred:OUT" );
    }
                              

// ---------------------------------------------------------------------------
// CCCEPlugin::BarringEventOccurred
// ---------------------------------------------------------------------------
//
void CCCEPlugin::CallForwardEventOccurred( 
    const MCCPSsObserver::TCCPSsCallForwardEvent aCallForwardEvent,
    const TDesC& aRemoteAddress )
    {
    CCELOGSTRING2("CCCEPlugin::CallForwardEventOccurred():IN event=%d",aCallForwardEvent );

    if( iCallContainer.SsObserver() )
        {
        // one to one mapping at the moment can be casted directly, change if mapping gets out of sync
        iCallContainer.SsObserver()->CallForwardEventOccurred(
            (MCCESsObserver::TCCESsCallForwardEvent)aCallForwardEvent,
            aRemoteAddress );
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::CallForwardEventOccurred: Event ignored no observer is set!" );
        }

    CCELOGSTRING( "CCCEPlugin::CallForwardEventOccurred:OUT" );
    
    }


// ---------------------------------------------------------------------------
// CCCEPlugin::CallCugEventOccurred
// ---------------------------------------------------------------------------
//
void CCCEPlugin::CallCugEventOccurred( const MCCPSsObserver::TCCPSsCugEvent aCugEvent )
    {
    CCELOGSTRING2("CCCEPlugin::CallCugEventOccurred():IN event=%d",aCugEvent );

    if( iCallContainer.SsObserver() )
        {
        // one to one mapping at the moment can be casted directly, change if mapping gets out of sync
        iCallContainer.SsObserver()->CallCugEventOccurred(
            (MCCESsObserver::TCCESsCugEvent)aCugEvent );
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::CallCugEventOccurred: Event ignored no observer is set!" );
        }

    CCELOGSTRING( "CCCEPlugin::CallCugEventOccurred:OUT" );
    }
    
// ---------------------------------------------------------------------------
// CCCEPlugin::NotifyCurrentActiveALSLine
// ---------------------------------------------------------------------------
//    
void CCCEPlugin::NotifyCurrentActiveALSLine( TInt aLine )
    {
    CCELOGSTRING2("CCCEPlugin::NotifyCurrentActiveALSLine():IN line=%d",aLine );

    if( iCallContainer.SsObserver() )
        {
        iCallContainer.SsObserver()->NotifyCurrentActiveALSLine(aLine);
        }
    else
        {
        CCELOGSTRING( "CCCEPlugin::NotifyCurrentActiveALSLine: Event ignored no observer is set!" );
        }

    CCELOGSTRING( "CCCEPlugin::NotifyCurrentActiveALSLine():OUT" );
    }
 
// ---------------------------------------------------------------------------
// CCCEPlugin::IsCsPlugin
// ---------------------------------------------------------------------------
//
TBool CCCEPlugin::IsCsPlugin( MCCPCall& aCall )
    {
    TBool isCsPlugin( EFalse );
    if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
        {
        if ( aCall.Parameters().CallType() != CCPCall::ECallTypePS )
            {
            isCsPlugin = ETrue;
            }
        }
    else
        {
        if( iPluginId == KCSProviderUid )
            {
            isCsPlugin = ETrue;
            }
        }
    return isCsPlugin;
    }

// end of file
