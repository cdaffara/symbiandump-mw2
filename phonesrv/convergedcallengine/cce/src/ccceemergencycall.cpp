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
* Description:  Base class of emergency call object
*
*/


//  INCLUDE FILES
#include <mccpcall.h>
#include "cconvergedcallprovider.h"
#include "mccecallobserver.h"
#include "ccceemergencycall.h"
#include "cccecallcontainer.h"
#include "ccce.h"
#include "cccelogger.h"
#include "ccceutility.h"
#include "mccpcscall.h"
#include "cccedurationtimer.h"
#include "tccecallindex.h"


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//

CCCEEmergencyCall::CCCEEmergencyCall(CCCECallContainer& aCallContainer, 
                                     CCCEPluginManager& aPluginManager ):
                                     CActive(EPriorityNormal), iCallContainer(aCallContainer), 
                                     iPluginManager(aPluginManager),
                                     iCallIndex( KInvalidCallIndex )
    {
    }
    
// ---------------------------------------------------------------------------
// Construct object
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::ConstructL()
    {
    CActiveScheduler::Add(this);
    iIsInProgress = EFalse;
    
    iExitCode = KErrNone;
    iCallParameters = CCCECallParameters::NewL();
    iCallParameters->SetCallType( CCPCall::ECallTypeCSVoice );
    iCallParameters->SetServiceId(0);
    
    iCCEDurationTimer = CCCEDurationTimer::NewL();
    iObserver = CCCECallObserverAsynchroniser::NewL();
    
    // TODO iStartTime = set starting time ...
    }


// ---------------------------------------------------------------------------
// Default Symbian constructor
// ---------------------------------------------------------------------------
//
CCCEEmergencyCall* CCCEEmergencyCall::NewL(CCCECallContainer& aCallContainer,
                                           CCCEPluginManager& aPluginManager )
    {
    CCCEEmergencyCall* self = CCCEEmergencyCall::NewLC(aCallContainer, aPluginManager);
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Default Symbian constructor (object is left into cleanupstack)
// ---------------------------------------------------------------------------
//
CCCEEmergencyCall* CCCEEmergencyCall::NewLC(CCCECallContainer& aCallContainer, 
                                            CCCEPluginManager& aPluginManager )
    {
    CCCEEmergencyCall* self = new( ELeave ) CCCEEmergencyCall(aCallContainer, aPluginManager);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCEEmergencyCall::~CCCEEmergencyCall()
    {
    Cancel();
    delete iCCEDurationTimer;
    delete iObserver;
    delete iCallParameters;
    }
    
    
// ---------------------------------------------------------------------------
// Return ETrue if emergency call is in progress
// ---------------------------------------------------------------------------
//
TBool CCCEEmergencyCall::IsInProgress()
    {
    return iIsInProgress;
    }

// ---------------------------------------------------------------------------
// SetObserver( MCCECallObserver& aObserver )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::SetObserver( const MCCECallObserver& aObserver )
    {
    CCELOGSTRING("CCCEEmergencyCall::SetObserver()");
    iObserver->SetMCCECallObserver( const_cast<MCCECallObserver*>( &aObserver ));
    
    if ( iCCEDurationTimer )
        {
        iCCEDurationTimer->SetObserver( iObserver );
        }
    }


// ---------------------------------------------------------------------------
// Notify observers about call status change
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::NotifyCallStatusChange( 
    CCPCall::TCallState aCallState, 
    TBool aInband, 
    CCECallEnums::TNotifyType aNotify )
    {
    CCELOGSTRING2("CCCEEmergencyCall::NotifyCallStatusChange: Status = %d", aCallState );
  
    iCallState = aCallState;
    
    if ( CCPCall::EStateConnected == aCallState )
        {
        // Start airtime duration timer
        iCallContainer.AirTimeDurationStart( NULL );
        }
    else if ( CCPCall::EStateIdle == aCallState )
        {
        // Stop airtime duration timer
        iCallContainer.AirTimeDurationStop( NULL );
        iIsInProgress = EFalse;
        iCallIndex = KInvalidCallIndex;
        }
    
    if ( aNotify != CCECallEnums::ENoNotify )
        {
        if ( CCPCall::EStateDialling == aCallState )
            {
            TCCECallIndex callIndex( iCallContainer );
            iCallIndex = callIndex.CurrentCallIndex();
            }
         // Notify observer immediately
        if ( aInband )
            {
            iObserver->CallStateChangedWithInband( aCallState );
            }
        else
            {
            iObserver->CallStateChanged( aCallState );
            }
        }
    iCallContainer.HandleEmergencyCallStatusChange( aNotify );
    }
    

// ---------------------------------------------------------------------------
// CActive RunL
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::RunL()
    {
    if( iState == ETerminated )
        {
        CCELOGSTRING("CCCEEmergencyCall::RunL -> Emergency Terminated" );
        ReleasePlugin();
        return;
        }
        
    // is success?    
    if ( iStatus == KErrNone )
        {
        CCELOGSTRING("CCCEEmergencyCall::RunL -> Emergency Connected" );
        return;
        }
    else
        {
        // is retry needed or is plugin done?
        if ( iStatus != ECCPEmergencyFailed ) // error 2, retry needed:
            {
            CCELOGSTRING("CCCEEmergencyCall::RunL ->Error !ECCPEmergencyFailed -> Retry plugin!" );
            switch( iState )
                {
                case EOther:                
                    CCELOGSTRING("CCCEEmergencyCall::RunL Other Recoverable failure,trying again");
                    // release call from plugin
                    iPluginManager.GetPluginL( iEmergencyProvider )->
                        ReleaseEmergencyCall( *iEmergencyCallInUse );

                    iEmergencyCallInUse = NULL;
                    TRAP_IGNORE( iEmergencyCallInUse = iPluginManager.GetPluginL( iEmergencyProvider )->
                        NewEmergencyCallL( 0, iEmergencyNumber, *this ) );
                    
                    if( !iEmergencyCallInUse )
                        {
                        CCELOGSTRING("CCCEEmergencyCall::RunL Recovering failled, try next plugin");
                        NextPluginL();
                        }

                    break;
                case EPrimary:
                    CCELOGSTRING("CCCEEmergencyCall::RunL Primary Recoverable failure, retrying" );
                    // in case of primary recoverable error we just ry again.     
                    break;
                }
                     
            }
        else    // error 1, plugin done
            {
            CCELOGSTRING("CCCEEmergencyCall::RunL-> ECCPEmergencyFailed -> Try load new plugin" );
            switch( iState )
                {
                case EOther:
                    // Fallthru is intentional
                case EPrimary: 
                    iState = EOther;
                    // Find "other" plugin to be used
                    NextPluginL();
                    iCallParameters->SetCallType( CCPCall::ECallTypePS );
                    break;
                }
            }
          
        if ( iEmergencyCallInUse )    
            {
            SetActive();
            iStatus = KRequestPending;
            iInitializing = &iStatus;
            CCELOGSTRING("CCCEEmergencyCall::RunL Dialing.." );
            
            if ( !iPrimaryEmergencyCall )
                {
                // not notifed earlier
                NotifyCallStatusChange( 
                    CCPCall::EStateDialling, 
                    EFalse, CCECallEnums::ENotifyAsynchronously );
                }
            
            iEmergencyCallInUse->Dial( iEmergencyNumber );  
            }
        else
            {
            CCELOGSTRING("CCCEEmergencyCall::RunL Failed..." );
            // Emergency call failed. report error
            CCELOGSTRING("CCCEEmergencyCall::Inform UI..." );
            iExitCode = KErrCouldNotConnect;
            
            iObserver->ErrorOccurred( ECCPEmergencyFailed );
            NotifyCallStatusChange( CCPCall::EStateIdle, 
                EFalse, CCECallEnums::ENotifyAsynchronously );
            }
        }    
    }
    
// ---------------------------------------------------------------------------
// CActive RunError
// ---------------------------------------------------------------------------
//

TInt CCCEEmergencyCall::RunError(TInt /*aError*/)
    {
    // Emergency call failed. report error
    CCELOGSTRING("CCCEEmergencyCall::RunError" );

    iObserver->ErrorOccurred( ECCPEmergencyFailed );
    return KErrNone;
    }
    
 // ---------------------------------------------------------------------------
// CActive DoCancel
// ---------------------------------------------------------------------------
//   
void CCCEEmergencyCall::DoCancel()
    {
    if( iInitializing )
        {
        User::RequestComplete( iInitializing, KErrCancel );
        }
    }
    
// ---------------------------------------------------------------------------
// Load next available plugin providing emergency support
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::NextPluginL()
    { 
    TInt err = KErrNone;
    
    CCELOGSTRING("CCCEEmergencyCall::NextPluginL, checking plugin availability" );
    iAltEmergencyCallPlugins = iPluginManager.AlternativeEmergencyPlugins();
    // Primary plugin = -1
    TInt index = iAltEmergencyCallPlugins.Find( iEmergencyProvider );
    
     // Try next other plugin, or if out of plugins primary plugin again
    if ( iAltEmergencyCallPlugins.Count() > index+1 )
        {
        CCELOGSTRING("CCCEEmergencyCall::NextPluginL,  More plugins available!" );
        // loop plugins to find one really returning emergency object
        do
            {
            ReleasePlugin();

            index++;
            iEmergencyProvider = iAltEmergencyCallPlugins[index];
            
            iEmergencyCallInUse = NULL;
            TRAP( err, iEmergencyCallInUse = 
                iPluginManager.GetPluginL( iEmergencyProvider )->
                    NewEmergencyCallL( 0, iEmergencyNumber, *this ) );
                
            CCELOGSTRING2(
                "CCCEEmergencyCall:: Emergency call loaded err = %i", err );
            

            }
        while( !iEmergencyCallInUse && 
            iAltEmergencyCallPlugins.Count() > index+1 );
    
        }
    else
        {
        CCELOGSTRING("CCCEEmergencyCall::NextPluginL,  No more plugins available" );
        
        ReleasePlugin();
        }
    }
    
// ---------------------------------------------------------------------------
// Removes old plugin if was other than primary
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::ReleasePlugin()
    {
    if( iEmergencyCallInUse && 
        ( iAltEmergencyCallPlugins.Find( iEmergencyProvider ) != KErrNotFound ) )
        {
        CCELOGSTRING("CCCEEmergencyCall:: Deleting previous emergency call" );
        TRAP_IGNORE( iPluginManager.GetPluginL( iEmergencyProvider )->
            ReleaseEmergencyCall( *iEmergencyCallInUse ); );
        iEmergencyCallInUse = NULL;
        }
    else if( iEmergencyCallInUse && 
        ( iEmergencyProvider == iPluginManager.PrimaryEmergencyCallUid() ) )
        {
        CCELOGSTRING("CCCEEmergencyCall:: Null emergency call" );
        iEmergencyCallInUse = NULL;
        }
    
    if ( iAltEmergencyCallPlugins.Find( iEmergencyProvider ) != KErrNotFound )
        {
        CCELOGSTRING("CCCEEmergencyCall:: Deleting previous emergency plugin" );
        iPluginManager.RemovePlugin( iEmergencyProvider );
        }
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// ?implementation_description
// ---------------------------------------------------------------------------
//
TInt CCCEEmergencyCall::HangUp()
    {
    CCELOGSTRING("CCCEEmergencyCall::HangUp()" );
    iAllowDisconnect = ETrue;
    TInt ret = KErrNone;
    
    // Inform of cancel if active request
    if( iActiveRequest != ECCENone )
        {
        iObserver->ErrorOccurred( ECCPErrorCancel );
        iActiveRequest = ECCENone;
        }
    
    // Cancel active object
    Cancel();
    
    switch( iCallState )
        {
        case  CCPCall::EStateDialling: 
            if ( iEmergencyCallInUse )
                {
                iEmergencyCallInUse->Cancel();
                }
           
            HandleCallStateChanged( MCCPCallObserver::ECCPStateIdle, EFalse );
            break;            
        case CCPCall::EStateConnecting:
        case CCPCall::EStateConnected:
            NotifyCallStatusChange( 
                    CCPCall::EStateDisconnecting, EFalse, CCECallEnums::ENoNotify );
            iActiveRequest = ECCEReleaseActive;
            iEmergencyCallInUse->HangUp();
            
            break;    
        case CCPCall::EStateDisconnecting:
            ret = KErrAlreadyExists;
            break;
        case CCPCall::EStateIdle:
            ret = KErrNotFound;
            break;
        default:
            break;
        }
    
    return ret;
    }


// ---------------------------------------------------------------------------
// From class MCCECall.
// ?implementation_description
// ---------------------------------------------------------------------------
//
TInt CCCEEmergencyCall::Answer()
    {
    CCELOGSTRING("CCCEEmergencyCall::Answer()");
    return KErrNotSupported;
    }


// ---------------------------------------------------------------------------
// Return if call is an emergency call or not.
// ---------------------------------------------------------------------------
//
TBool CCCEEmergencyCall::IsEmergency() const
    {
    return ETrue;
    }


// ---------------------------------------------------------------------------
// DialEmergencyCall( const TDesC16& /*aNumber*/ )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::DialEmergencyCall( const TDesC16& aNumber )
    {
    CCELOGSTRING("CCCEEmergencyCall::DialEmergencyCall()");
    if ( !iIsInProgress && !IsActive() )
        {
        iIsInProgress = ETrue;
        iEmergencyNumber.Zero();
        iEmergencyNumber.Copy(aNumber);
        iActiveRequest = ECCEDial;
        iAllowDisconnect = EFalse;
        iExitCode = KErrNone;
        iCallParameters->SetCallType( CCPCall::ECallTypeCSVoice );
        iCallParameters->SetServiceId(0);
        iPrimaryEmergencyCall = iPluginManager.PrimaryEmergencyCall();
        iEmergencyCallInUse = NULL;
        iEmergencyProvider = iPluginManager.PrimaryEmergencyCallUid();
        
        // disconnect all VoIP calls
        iCallContainer.TerminateAllVoIPCalls();
                
        iEmergencyCallInUse = iPrimaryEmergencyCall;
        iState = EPrimary;
        
        SetActive();
        iInitializing = &iStatus;
        iStatus = KRequestPending; 
               
        if ( iEmergencyCallInUse )
            {
            CCELOGSTRING("CCCEEmergencyCall::DialEmergencyCall() Using primary call");
            NotifyCallStatusChange( 
                    CCPCall::EStateDialling, 
                EFalse, CCECallEnums::ENotifyAsynchronously );
            iEmergencyCallInUse->Dial( iEmergencyNumber );
            }
        else // no can do
            {
            CCELOGSTRING("CCCEEmergencyCall::DialEmergencyCall() No primary call available...");
    
            User::RequestComplete( iInitializing, ECCPEmergencyFailed );
            }
        }
    else
        {
        CCELOGSTRING("CCCEEmergencyCall::DialEmergencyCall() Already in call!!");
        return;
        }
    }


// ---------------------------------------------------------------------------
// From class MCCECall.
// Close call object
// ---------------------------------------------------------------------------
//
TInt CCCEEmergencyCall::Release()
    {
    CCELOGSTRING("CCCEEmergencyCall::Release" );

    if( iCCEDurationTimer )
        {
        iCCEDurationTimer->Reset();     
        }
    
    iObserver->SetMCCECallObserver( NULL );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// ErrorOccured( TCCPCallError aError )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::ErrorOccurred( TCCPError aError, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING2("CCCEEmergencyCall::ErrorOccurred: Error = %d", aError );
    if ( iInitializing && IsActive() )
        {
        User::RequestComplete( iInitializing, aError );
        }
    else
        {
        iObserver->ErrorOccurred( aError );
        }
    }

// ---------------------------------------------------------------------------
// CallStateChanged( TCCPCallState aState )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::CallStateChanged( TCCPCallState aState, 
                                          MCCPCall* /*aCall*/ )
    {
    CCELOGSTRING("CCCEEmergencyCall::CallStateChanged()");
    HandleCallStateChanged( aState, EFalse );
    }
    
// ---------------------------------------------------------------------------
// CallStateChangedWithInband( TCCPCallState aState )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::CallStateChangedWithInband( TCCPCallState aState, 
                                                    MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING("CCCEEmergencyCall::CallStateChangedWithInband()");
    HandleCallStateChanged( aState, ETrue );
    }
    
// ---------------------------------------------------------------------------
// HandleCallStateChanged( TCCPCallEvent aEvent )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::HandleCallStateChanged( TCCPCallState aState, TBool aInband )
    {
    CCELOGSTRING2("CCCEEmergencyCall::HandleCallStateChanged() State = %d", aState);
    TInt notify = EFalse;
    CCPCall::TCallState state( CCPCall::EStateIdle );
    
    if( !iIsInProgress )
        {
        CCELOGSTRING("CCCEEmergencyCall:: Not in progress; return");
        return;
        }
    
    switch ( aState )
        {
        
        case ECCPStateDialling:
            CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Dialling");
            state = CCPCall::EStateDialling;
            notify = EFalse;
                
            break;   
            
        case ECCPStateAnswering:
            CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Dialling");
            state = CCPCall::EStateAnswering;
            notify = ETrue;
                
            break;    
   
        case ECCPStateConnecting:
            CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Connecting"); 
            if( iActiveRequest == ECCEDial )
                {
                CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Ending Dial phase");
                iObserver->ErrorOccurred( ECCPErrorNone );
                iActiveRequest = ECCENone;
                }
              
            if ( iInitializing && IsActive() )
                {
                User::RequestComplete( iInitializing, KErrNone );
                }
            
            notify = ETrue;
            state = CCPCall::EStateConnecting;
                
            iAllowDisconnect = ETrue;
            break;
            
        case ECCPStateConnected:
            CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Connected");
            notify = ETrue;
            state = CCPCall::EStateConnected;
            
            // Start duration timer
            iCCEDurationTimer->Start();
            
            if( iActiveRequest == ECCEDial )
                {
                CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Ending Dial phase");
                iObserver->ErrorOccurred( ECCPErrorNone );
                iActiveRequest = ECCENone;
                
                if ( iInitializing && IsActive() )
                    {
                    User::RequestComplete( iInitializing, KErrNone );
                    } 
                } 
            iAllowDisconnect = ETrue;
            break;
            
        case ECCPStateDisconnecting:
            if ( iAllowDisconnect )
                { 
                CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: ECCPStatusDisconnecting");
                // Stop duration timer
                iCCEDurationTimer->Stop();
                
                state = CCPCall::EStateDisconnecting;
                notify = ETrue;       
                }
            
            break;
            
        case ECCPStateIdle:
           
            if ( iActiveRequest )
                {
                iObserver->ErrorOccurred( ECCPErrorNone );
                iActiveRequest = ECCENone;
                }
                
            if ( iAllowDisconnect )
                {
                 // Stop duration timer
                iCCEDurationTimer->Stop();   
                
                if ( EOther == iState )
                    {
                    iState = ETerminated;
                    
                    if ( !IsActive() )
                        {
                        SetActive();
                        }
                    
                    iInitializing = &iStatus;
                    iStatus = KRequestPending; 
                    User::RequestComplete( iInitializing, KErrNone );
                    }
                
                CCELOGSTRING("CCCEEmergencyCall::CallStateChanged: Disconnected/Idle");
                notify = ETrue;
                state = CCPCall::EStateIdle;         
                }
            break;

        default:
            CCELOGSTRING2("CCCEEmergencyCall::CallStateChanged: Unhandled state = %d",
                aState );
            break;
        }
        
    if ( notify )
        {
        NotifyCallStatusChange( state, aInband, 
            CCECallEnums::ENotifyAsynchronously );
        }
    }
    
// ---------------------------------------------------------------------------
// CallEventOccurred( TCCPCallEvent aEvent )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::CallEventOccurred( TCCPCallEvent aEvent, 
                                           MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING2("CCCEEmergencyCall::CallEventOccurred(%d)", aEvent);

    switch ( aEvent )
        {
        case ECCPRemoteTerminated:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteTerminated);
            break;

        case ECCPQueued:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteWaiting );
            break;

        case ECCPLocalHold:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalHold );
            break;

        case ECCPLocalResume:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalResume );
            break;

        case ECCPRemoteHold:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteHold );
            break;

        case ECCPRemoteResume:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteResume );
            break;

        case ECCCSPEarlyMediaStarted:
            iObserver->CallEventOccurred( MCCECallObserver::EVoIPEventEarlyMediaStarted );
            break;
            
        default:
            CCELOGSTRING2("CCCEEmergencyCallll::SessionEventOccurred: Unhandled event = %d",
                aEvent );
            break;
        }
    }

// ---------------------------------------------------------------------------
// CallCapsChanged( TUint32 aCapsFlags )
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::CallCapsChanged( TUint32 aCapsFlags, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING("CCCEEmergencyCall::CallCapsChanged()");
  
    iObserver->CallCapsChanged(
        ( MCCECallObserver::TCCECallControlCaps ) aCapsFlags );
    }
  
// ---------------------------------------------------------------------------
// GetMobileCallInfo
// ---------------------------------------------------------------------------
//
void CCCEEmergencyCall::GetMobileCallInfo( TDes8& aCallInfo ) const
    {
    RMobileCall::TMobileCallInfoV3Pckg& pckg =
        reinterpret_cast<RMobileCall::TMobileCallInfoV3Pckg&>( aCallInfo );
    pckg().iEmergency = ETrue;
    pckg().iExitCode = iExitCode;    
    }
    
// ---------------------------------------------------------------------------
// State()
// ---------------------------------------------------------------------------
//
CCPCall::TCallState CCCEEmergencyCall::State() const
    {
    CCELOGSTRING2("CCCEEmergencyCall::State() = %d", iCallState );
    return iCallState;
    }

// ---------------------------------------------------------------------------
// CallDuration()
// ---------------------------------------------------------------------------
//
TTimeIntervalSeconds CCCEEmergencyCall::CallDuration() const
    {
    return iCCEDurationTimer->NumberOfBeats();
    }

// ---------------------------------------------------------------------------
// StartTime()
// ---------------------------------------------------------------------------
//
TDateTime CCCEEmergencyCall::StartTime() const
    {
    return iStartTime;
    }

// ---------------------------------------------------------------------------
// ImplementationUid()
// ---------------------------------------------------------------------------
//
const TUid CCCEEmergencyCall::ImplementationUid() const
    {
    return iEmergencyProvider;
    }

// ---------------------------------------------------------------------------
// Tone()
// ---------------------------------------------------------------------------
//
TCCPTone CCCEEmergencyCall::Tone() const
    {
    TCCPTone tone = ECCPNoSoundSequence;
    
    if( iActiveRequest != ECCEReleaseActive )
        {
        tone = ECCPToneUserBusy;
        }

    return tone;
    }


// ---------------------------------------------------------------------------
// From base class MCCECallInfo
// Returns direction of the call
// ---------------------------------------------------------------------------
//
CCPCall::TCallDirection CCCEEmergencyCall::Direction() const
    {
    return CCPCall::EMobileOriginated;
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfo
// Returns the type of the call
// ---------------------------------------------------------------------------
//
CCPCall::TCallType CCCEEmergencyCall::CallType() const
    {
    if ( iCallParameters )
        {
        return iCallParameters->CallType();
        }
    return CCPCall::ECallTypeCSVoice;
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfo, MCCECall
// Returns the index of the call
// ---------------------------------------------------------------------------
//
TInt CCCEEmergencyCall::CallIndex() const
    {
    return iCallIndex;
    }
    
// Stub part
TBool CCCEEmergencyCall::SecureSpecified() const
    {
    return ETrue;
    }
   
TBool CCCEEmergencyCall::IsSecured() const
    {
    return EFalse;
    }
    
void CCCEEmergencyCall::SetParameters( const CCCECallParameters& aNewParams )
    {
    iCallParameters->SetServiceId(aNewParams.ServiceId());
    iCallParameters->SetCallType(aNewParams.CallType());
    }

const CCCECallParameters& CCCEEmergencyCall::Parameters() const
    {
    return *iCallParameters;
    }
 
TInt CCCEEmergencyCall::Hold()
    {
    return KErrNotSupported;
    }
    
TInt CCCEEmergencyCall::Resume()
    {
    return KErrNotSupported;
    }

TInt CCCEEmergencyCall::Swap()
    {
    return KErrNotSupported;
    }

TInt CCCEEmergencyCall::Dial( const TDesC8& /*aCallParams*/ )
    {
    return KErrNotSupported;
    }

void CCCEEmergencyCall::NoFDNCheck()
    {
    // void functionality
    }

void CCCEEmergencyCall::HoldNoNotifyL()
    {   
    }

void CCCEEmergencyCall::ResumeNoNotifyL()
    {
    }

void CCCEEmergencyCall::ForwardToAddressL( TInt /*aIndex*/ )
    {
    }
    
TUint32 CCCEEmergencyCall::ServiceId() const
    {
    return iCallParameters->ServiceId();
    }

MCCECallObserver::TCCECallControlCaps CCCEEmergencyCall::Caps() const
    {
    return (MCCECallObserver::TCCECallControlCaps)0;
    }

TInt CCCEEmergencyCall::GetMobileDataCallCaps( TDes8& /*aCaps*/ ) const
    {
    return KErrNotSupported;
    }

TBool CCCEEmergencyCall::LogDialedNumber() const
    {
    return ETrue;
    }

TInt CCCEEmergencyCall::AttendedTransfer( const TDesC& /*aTransferTarget*/ )
    {
    return KErrNotSupported;
    }

TInt CCCEEmergencyCall::UnattendedTransfer( const TDesC& /*aTarget*/ )
    {
    return KErrNotSupported;
    }

TInt CCCEEmergencyCall::AcceptTransfer( TBool /*aAccept*/ )
    {
    return KErrNotSupported;
    }

const TDesC& CCCEEmergencyCall::TransferTarget() const
    {
    return KNullDesC();
    }

const CDesC8Array& CCCEEmergencyCall::GetForwardAddressChoicesL()
    {   
    User::Leave( KErrNotSupported );
    
    // No return value can be defined
    // never executed. just to remove warning
    CDesC8Array* dummy;
    dummy = NULL;
    return *dummy;   
    }

TInt CCCEEmergencyCall::SwitchAlternatingCall()
    {
    return KErrNotSupported;
    }

const TDesC& CCCEEmergencyCall::RemoteParty() const
    {
    return KNullDesC;
    }

const TDesC& CCCEEmergencyCall::RemotePartyName() const
    {
    return KNullDesC;
    }

const TDesC& CCCEEmergencyCall::DialledParty() const
    {
    return KNullDesC;
    }

TInt CCCEEmergencyCall::ReplaceActive()
    {
    return KErrNotSupported;
    }

// End of file
