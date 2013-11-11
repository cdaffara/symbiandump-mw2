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
* Description:  Base class of call object
*
*/


//  INCLUDE FILES
#include "cccecall.h"
#include <exterror.h>                // GSM error codes
#include <gsmerror.h>
#include <etelmm.h>
#include <ccpdefs.h>

#include <featmgr.h>

#include "mccptransferprovider.h"
#include "mccpcall.h"
#include "mccecallobserver.h"
#include "ccce.h"
#include "cccelogger.h"
#include "ccceutility.h"
#include "cccedurationtimer.h"
#include "mccpcscall.h"
#include "cccecallcontainer.h"
#include "cccetransfercontroller.h"
#include "ccceconferencecall.h"
#include "mccpforwardprovider.h"
#include "cccecallobserverasynchroniser.h"
#include "tccecallindex.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCECall::CCCECall( CCCECallContainer& aCallContainer, TInt aCallId ) :
    iCallId( aCallId ),
    iCallContainer( aCallContainer ),
    iCallIndex( KInvalidCallIndex )
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// Construct object
// ---------------------------------------------------------------------------
//
void CCCECall::ConstructL()
    {
    FeatureManager::InitializeLibL();
    iCCEDurationTimer = CCCEDurationTimer::NewL();
    iObserver = CCCECallObserverAsynchroniser::NewL();
    }
    
// ---------------------------------------------------------------------------
// Default Symbian constructor (object is left into cleanupstack)
// ---------------------------------------------------------------------------
//
CCCECall* CCCECall::NewLC( CCCECallContainer& aCallContainer, TInt aCallId )
    {
    CCCECall* self = new( ELeave ) CCCECall( aCallContainer, aCallId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCECall::~CCCECall()
    {
    Reset();
    delete iCCEDurationTimer;
    delete iObserver;
    delete iCallParameters;
    FeatureManager::UnInitializeLib();
    }

// ---------------------------------------------------------------------------
// SetObserver( MCCECallObserver& aObserver )
// ---------------------------------------------------------------------------
//
void CCCECall::SetObserver( const MCCECallObserver& aObserver )
    {
    CCELOGSTRING2("id:%d CCCECall::SetObserver()", iCallId);
    
    iObserver->SetMCCECallObserver( const_cast<MCCECallObserver*>( &aObserver ));
    
    if (iCCEDurationTimer)
        {
        iCCEDurationTimer->SetObserver( iObserver );
        }
    }

// ---------------------------------------------------------------------------
// SetConvergedCall( MCCPCall* aCall )
// ---------------------------------------------------------------------------
//
void CCCECall::SetConvergedCallL( MCCPCall* aCall, 
                                 TUid aImplementationUid,
                                 TBool aTransferred )
    {
    CCELOGSTRING2("id:%d CCCECall::SetConvergedCall():IN", iCallId);
       
    iCall = aCall;
    iImplementationUid = aImplementationUid;
    
    delete iCallParameters;
    iCallParameters = NULL;
    iCallParameters = aCall->Parameters().CloneL();
    
    
    // This object is now in use

    CCELOGSTRING3("id:%d CCCECall::SetConvergedCall: CallId = %d", iCallId, iCallId);
    // Reserve call id for this call
    // If this is transferred call then call id has been already reserved
    if( !aTransferred )
        {
        iCallContainer.ReserveCallInfoCallId( iCallId );    
        }
    
    CCELOGSTRING2("id:%d CCCECall::SetConvergedCall: CallId reserved", iCallId);
    
    // Set call direction
    if( aCall->IsMobileOriginated() ) 
        {
        CCELOGSTRING2("id:%d CCCECall::SetConvergedCall:Mobile originated call", iCallId);
        iCallDirection = CCPCall::EMobileOriginated;
        }
    else
        {
        CCELOGSTRING2("id:%d CCCECall::SetConvergedCall():Mobile terminated call", iCallId);
        
        if ( !aTransferred )
            {
            iCallState  = CCPCall::EStateRinging;
            }
        iCallDirection = CCPCall::EMobileTerminated;
        aCall->AddObserverL(*this);
        }

    TRAP_IGNORE( iTransferProvider = iCall->TransferProviderL(*this) );
    
    TRAP_IGNORE( iForwardProvider = iCall->ForwardProviderL(*this) );

    CCELOGSTRING2("id:%d CCCECall::SetConvergedCall():OUT", iCallId);
    }

// ---------------------------------------------------------------------------
// Returns implementationuid
// ---------------------------------------------------------------------------
//
const TUid CCCECall::ImplementationUid() const
    {
    return iImplementationUid;
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfo
// Returns direction of the call
// ---------------------------------------------------------------------------
//
CCPCall::TCallDirection CCCECall::Direction() const
    {
    return iCallDirection;
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfo
// Returns the type of the call
// ---------------------------------------------------------------------------
//
CCPCall::TCallType CCCECall::CallType() const
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
TInt CCCECall::CallIndex() const
    {
    return iCallIndex;
    }

// ---------------------------------------------------------------------------
// ServiceId
// ---------------------------------------------------------------------------
//
TUint32 CCCECall::ServiceId() const 
    {
    if( iCall )
        {
        return iCall->Parameters().ServiceId(); 
        }
    return 0;
    }

// ---------------------------------------------------------------------------
// Return ETrue if this call object is in use
// ---------------------------------------------------------------------------
//
TBool CCCECall::IsInUse() const
    {
    CCELOGSTRING3("id:%d CCCECall::IsInUse() %d", iCallId, ( iCall != NULL ) );
    return ( iCall != NULL );
    }

// ---------------------------------------------------------------------------
// Return ETrue if this calls secure status can be specified
// ---------------------------------------------------------------------------
//
TBool CCCECall::SecureSpecified() const
    {
    return iCall->SecureSpecified();
    }

// ---------------------------------------------------------------------------
// Return ETrue if this call object is secured
// ---------------------------------------------------------------------------
//
TBool CCCECall::IsSecured() const
    {
    return iCall->IsSecured();
    }

// ---------------------------------------------------------------------------
// Return if call is an emergency call or not.
// ---------------------------------------------------------------------------
//
TBool CCCECall::IsEmergency() const
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CCCECall::RemoteParty
// ---------------------------------------------------------------------------
//
const TDesC& CCCECall::RemoteParty() const
    {
    if( iCall )
        {
        return iCall->RemoteParty();
        }

    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// CCCECall::RemotePartyName
// ---------------------------------------------------------------------------
//
const TDesC& CCCECall::RemotePartyName() const
    {
    if( iCall )
        {
        return iCall->RemotePartyName();
        }

    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// CCCECall::DialledParty
// ---------------------------------------------------------------------------
//
const TDesC& CCCECall::DialledParty() const
    {
    if( iCall )
        {
        return  iCall->DialledParty();
        }

    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// Return current active request
// ---------------------------------------------------------------------------
//
CCCECall::TCallRequest CCCECall::Request() const
    {
    return iActiveRequest;
    }
    
// ---------------------------------------------------------------------------
// Returns reference to converged call provider call object
// ---------------------------------------------------------------------------
//
MCCPCall& CCCECall::GetCCPCall() const
    {
    return *iCall;
    }

// ---------------------------------------------------------------------------
// CallId()
// Returns ID of the call
// ---------------------------------------------------------------------------
//
TInt CCCECall::CallId() const
    {
    return iCallId;
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// Hold()
// ---------------------------------------------------------------------------
//
TInt CCCECall::Hold()
    {
    CCELOGSTRING2( "id:%d CCCECall::Hold() ", iCallId);
    TInt error = KErrNone;
    
    if( iCallState != CCPCall::EStateConnected )
        {
        if( iCallState == CCPCall::EStateHold )
            {
            error = KErrAlreadyExists;
            }
        else
            {
            error = KErrNotReady;
            }
        }
    else
        {
        iActiveRequest = ECCEHold;
        error = iCall->Hold();
        }

    return error;
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// Resume()
// ---------------------------------------------------------------------------
//
TInt CCCECall::Resume()
    {
    CCELOGSTRING2( "id:%d CCCECall::Resume()", iCallId );
    TInt error = KErrNone;
    
    if( iCallState != CCPCall::EStateHold )
        {
        if( iCallState == CCPCall::EStateConnected )
            {
            error = KErrAlreadyExists;
            }
        else
            {
            error = KErrNotReady;
            }
        }
    else
        {
        iActiveRequest = ECCEResume;
        error = iCall->Resume();
        }

    return error;
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// HangUp()
// ---------------------------------------------------------------------------
//
TInt CCCECall::HangUp()
    {
    CCELOGSTRING3("id:%d CCCECall::HangUp() callstate = %d", iCallId, iCallState);

    TInt error = KErrNone;

    // Inform that currently ongoing request is canceled
    if( iActiveRequest != ECCENone &&
        iActiveRequest != ECCEReleaseActive &&
        iActiveRequest != ECCEReleaseHeld &&
        iActiveRequest != ECCEReleaseOutgoing &&
        iActiveRequest != ECCETerminateDisconnected )
        {
        RequestComplete( ECCPErrorCancel );
        }
    
    switch(iCallState)
        {
        case CCPCall::EStateDialling:
            CCELOGSTRING2("id:%d CCCECall::HangUp(): DialCancel: Cancel", iCallId);
            iActiveRequest = ECCEReleaseOutgoing;

            error = iCall->Cancel();

            RequestComplete( ECCPErrorNone );
            CallStateChanged( MCCPCallObserver::ECCPStateIdle, EFalse );
            break;
            
        case CCPCall::EStateConnecting:
            CCELOGSTRING2("id:%d CCCECall::HangUp(): DialCancel: HangUp", iCallId);
            iActiveRequest = ECCEReleaseOutgoing;
            NotifyCallStatusChange( 
                    CCPCall::EStateDisconnecting, ETrue, CCECallEnums::ENoNotify );
            error = iCall->HangUp();
            break;

        case CCPCall::EStateAnswering:
        case CCPCall::EStateRinging:
        case CCPCall::EStateQueued:
            CCELOGSTRING2("id:%d CCCECall::HangUpL: Reject", iCallId);
            iActiveRequest = ECCERejectIncoming;
            // Set exitcode to rejected to avoid 
            // missed call notification
            iExitCode = ECCPErrorRejected;
            NotifyCallStatusChange( 
                    CCPCall::EStateDisconnecting, ETrue, CCECallEnums::ENoNotify );
            error = iCall->Reject();
            break;
            
        case CCPCall::EStateDisconnecting: 
            if( ECCETerminateDisconnected != iActiveRequest &&
                ECCEReleaseOutgoing != iActiveRequest )
                {
                RequestComplete( ECCPErrorNone );
                iActiveRequest = ECCETerminateDisconnected;
                error = iCall->HangUp();
                }
            else 
                {
                error = KErrGeneral;  
                }   
            break;
            
        case CCPCall::EStateIdle:
            error = KErrAlreadyExists;
            break;
            
        case CCPCall::EStateHold:
            iActiveRequest = ECCEReleaseHeld;
            iCCEDurationTimer->Stop();
            NotifyCallStatusChange( 
                    CCPCall::EStateDisconnecting, ETrue, CCECallEnums::ENoNotify );
            error = iCall->HangUp();
            break;
            
        case CCPCall::EStateConnected:
            iActiveRequest = ECCEReleaseActive;
            iCCEDurationTimer->Stop();
            NotifyCallStatusChange( 
                    CCPCall::EStateDisconnecting, ETrue, CCECallEnums::ENoNotify );
            error = iCall->HangUp();
            break;
            
        default:
            CCCEUtility::Panic( ECCEPanicInvalidState ); 
            break;
        }
        


    // If hangup was completed successfully, change
    // call status to disconnecting. This must be done
    // because if hangup was caused by replace -command
    // and in that case AnswerIncomingCall is called right
    // after hangup. If call is not changed to disconnecting
    // state, ResumeAutomaticallyHeldCall would try to
    // resume this call and it causes error.
    return error;
    }

// ---------------------------------------------------------------------------
// Swap()
// ---------------------------------------------------------------------------
//
TInt CCCECall::Swap()
    {
    CCELOGSTRING2( "id:%d CCCECall::Swap()", iCallId );
    
    TInt error = KErrNone;

    // if there is other CSProvider call use swap instead of Hold
    if ( ( iImplementationUid == KCSProviderUid || 
           iImplementationUid == KVCCProviderUid ) && 
         ( IsCsPlugin() ) && 
           1 < iCallContainer.CountCsCalls( iImplementationUid ) )
        {
        error = iCall->Swap();
        }
    else if( iCallState == CCPCall::EStateConnected )
        {
        // Swapping starts - 1.phase
        error = HoldNoNotify();
        }
    else
        {
        error = KErrNotReady;
        
        iActiveRequest = ECCENone;
        }

    if( !error )
        {
        // Everything went ok
        iActiveRequest = ECCESwap;
        }

    return error;
    }

// ---------------------------------------------------------------------------
// NotifyRequestComplete( TInt aRequest, TCCPError aError )
// ---------------------------------------------------------------------------
//
void CCCECall::NotifyRequestComplete( TInt aRequest, TCCPError aError )
    {
    CCELOGSTRING4("id:%d CCCECall::NotifyRequestComplete: aRequest = %d, aError = %d", 
        iCallId, aRequest, aError );

    // Notify observer, if NoNotify is NOT set
    if ( aRequest == iDoNotReportRequest )
        {
        iDoNotReportRequest = KErrNotFound;
        }
    else
        {
        if( iObserver )
            {
            iObserver->ErrorOccurred( aError );
            }
        else
            {
            CCELOGSTRING( "CCCECall::NotifyRequestComplete: observer was NOT set!" );
            }
        }
    }

// ---------------------------------------------------------------------------
// GetMobileCallInfo( TDes8& aCallInfo ) const
// TODO: We should use our own packet instead of TMobileCallInfoV3Pckg
// ---------------------------------------------------------------------------
//
void CCCECall::GetMobileCallInfo( TDes8& aCallInfo ) const
    {
    CCELOGSTRING3("id:%d CCCECall::GetMobileCallInfo() Call = %d", iCallId, iCallId);
    
    RMobileCall::TMobileCallInfoV3Pckg& pckg =
        reinterpret_cast<RMobileCall::TMobileCallInfoV3Pckg&>( aCallInfo );

    // Check for call object needed, due call info can be fetched
    // also if dial fails.
    if ( iCall )
        {
        RMobileCall::TMobileCallStatus state = RMobileCall::EStatusUnknown;

        switch(iCallState)
            {
            case CCPCall::EStateIdle:
                state = RMobileCall::EStatusIdle;
                break;
            case CCPCall::EStateRinging:
                state = RMobileCall::EStatusRinging;
                break;
            case CCPCall::EStateConnecting:
                state = RMobileCall::EStatusConnecting;
                break;
            case CCPCall::EStateConnected:
                state = RMobileCall::EStatusConnected;
                break;
            case CCPCall::EStateDisconnecting:
                state = RMobileCall::EStatusDisconnecting;
                break;
            case CCPCall::EStateHold:
                state = RMobileCall::EStatusHold;
                break;
            case CCPCall::EStateTransferring:
                state = RMobileCall::EStatusTransferring;
                break;
            case CCPCall::EStateForwarding:
                state = RMobileCall::EStatusConnecting;
                break;
            case CCPCall::EStateQueued:
                state = RMobileCall::EStatusRinging;
                break;
            case CCPCall::EStateAnswering:
                state = RMobileCall::EStatusAnswering;
                break;
            case CCPCall::EStateDialling:
                state = RMobileCall::EStatusDialling;
                break;
            default:
                state = RMobileCall::EStatusUnknown;
                break;
            }
        
        if( IsCsPlugin() )
            {
            TInt error = 
                static_cast<MCCPCSCall*>(iCall)->GetMobileCallInfo(aCallInfo);
            
            pckg().iCallId = iCallId+1;
            pckg().iStatus = state;
            pckg().iDuration = iCCEDurationTimer->NumberOfBeats();
            return;
            }

        TInt err = KErrNone;
        switch( iExitCode )
            {
            case ECCPErrorNone:
                break;
            /**
            General: KErrNotFound
            */
            case ECCPErrorNotFound:
                err = KErrNotFound;
                break;
            /**
            General: KErrGeneral 
            */
            case ECCPErrorGeneral:
                err = KErrGeneral;
                break;

            /**
            General: KErrCancel
            */
            case ECCPErrorCancel:
                err = KErrCancel;
                break;

            /** 
            General: KErrNoMemory */
            case ECCPErrorNoMemory:
                err = KErrNoMemory;
                break;

            /** 
            General: KErrNotSupported */
            case ECCPErrorNotSupported:
                err = KErrNotSupported;
                break;
                
            /**
            General: KErrInUse
            */
            case ECCPErrorAlreadyInUse:
                err = KErrInUse;
                break;

            /**
            No corresponding cs/sip error identified yet.
            */
            case ECCPErrorNotReady:
                err = KErrNotReady;
                break;

            /** 
            KErrGsmCCFacilityRejected
            KErrGsmCallServerFail
            KErrGsm0707NotFound
            */
            case ECCPErrorRejected:
            case ECCPErrorCCFacilityRejected:
                err = KErrGsmCCFacilityRejected;
                break;

            /** 
            KErrGsmCCUnassignedNumber 
            SIP: 404 Not found
            */
            case ECCPErrorNotReached:
                err = KErrGsmCCUnassignedNumber;
                break;

            /** KErrGsmCCNumberChanged
            SIP: 301 Moved permanently/410 Gone
            */
            case ECCPErrorMovedPermanently:
                err = KErrGsmCCNumberChanged;
                break;
            /**
            KErrGsmCCInvalidNumberFormat
            SIP: BadUri
            */
            case ECCPErrorInvalidURI:
                err = KErrGsmCCInvalidNumberFormat;
                break;

            /** 
            KErrGsmCCNetworkOutOfOrder
            SIP: 502 Bad gateway
            (redial logic needs)
            */
            case ECCPErrorNetworkOutOfOrder:
                err = KErrGsmCCNetworkOutOfOrder;
                break;

            /** 
            KErrGsmCCUserBusy
            SIP: 486 Busy here
            */
            case ECCPErrorBusy:
                err = KErrGsmCCUserBusy;
                break;

            /** 
            KErrGsmCCUserNotResponding
            SIP: 480 Temporarily unavailable
            */
            case ECCPErrorNotResponding:
            case ECCPErrorCCUserNotResponding:
                err = KErrGsmCCUserNotResponding;
                break;

            /** 
            KErrGsmCCTemporaryFailure
            SIP: 500 Internal server error
            KErrGsmCCSwitchingEquipmentCongestion
            SIP: 503 Service unavailable
            KErrGsmCCRequestedChannelNotAvailable
            */
            case ECCPErrorNetworkBusy:
                err = KErrGsmCCTemporaryFailure;
                break;

            /** 
            KErrGsmCCNoRouteToDestination
            SIP: 404 Not Found
            SIP: 404 Not found / Timeout
            SIP: 503 Service unavailable
            */
            case ECCPErrorConnectionErrorRedial:
            case ECCPErrorCCNoRouteToDestination:
                err = KErrGsmCCNoRouteToDestination;
                break;

            /**
            KErrArgument
            */
            case ECCPErrorInvalidPhoneNumber:
                err = KErrArgument;
                break;

            /**
            KErrGsmNoService
            */
            case ECCPErrorNoService:
                err = KErrGsmNoService;
                break;

            /**
            KErrGsmNoNumber
            */
            case ECCPErrorInvalidPhoneNumberCancelRedial:
                err = KErrGsmNoNumber;
                break;

            /**
            KErrGsmReleaseByUser
            */
            case ECCPErrorReleaseByUserForCancelRedial:
                err = KErrGsmReleaseByUser;
                break;

            /**
            KErrGsmCCRequestedFacilityNotImplemented
            */
            case ECCPErrorServiceSettingError:
                err = KErrGsmCCRequestedFacilityNotImplemented;
                break;

            /**
            KErrGsm0707SimNotInserted
            KErrGsm0707SIMPuk1Required
            KErrGsm0707SimPin1Required
            KErrGsm0707SimPin2Required
            KErrGsm0707SimFailure
            KErrGsm0707PhoneToSimLockRequired
            KErrGsm0707SimWrong
            */
            case ECCPErrorAuthenticationFailed:
                err = KErrGsm0707SimNotInserted;
                break;

            /**
            KErrGsmOfflineOpNotAllowed
            */
            case ECCPErrorNotAllowedInOfflineMode:
                err = KErrGsmOfflineOpNotAllowed;
                break;

            /**
            KErrMMEtelCallForbidden
            KErrGsmCallInProgress
            KErrGsmNumberBarred
            KErrGsmNotAllowed
            KErrEtelCallNotActive
            KErrEtelCallAlreadyActive
            SIP: 403 Forbidden
            */
            case ECCPErrorNotAllowed:
                err = KErrGsmNotAllowed;
                break;

            /**
            KErrTimedOut
            */
            case ECCPErrorTimedOut:
                err = KErrTimedOut;
                break;

            /**
            KErrGsmInvalidFdn
            */
            case ECCPErrorInvalidFDN:
                err = KErrGsmInvalidFdn;
                break;

            /**
            KErrGsmCCChannelUnacceptable
            */
            case ECCPErrorUnacceptableChannel:
                err = KErrGsmCCChannelUnacceptable;
                break;

            /**
            KErrGsmCCAccessInformationDiscarded
            */
            case ECCPErrorAccessInformationDiscarded:
                err = KErrGsmCCAccessInformationDiscarded;
                break;

            /**
            KErrGsmCCQualityOfServiceNotAvailable
            */
            case ECCPErrorQualityOfServiceNotAvailable:
                err = KErrGsmCCQualityOfServiceNotAvailable;
                break;

            /**
            KErrGsmCCInvalidCallReferenceValue
            */
            case ECCPErrorInvalidCallReferenceValue:
                err = KErrGsmCCInvalidCallReferenceValue;
                break;

            /**
            KErrGsmCCInvalidTransitNetworkSelection
            KErrGsmCCSemanticallyIncorrectMessage
            KErrGsmCCConditionalIEError
            SIP: 400 Bad request
            KErrGsmCCUnspecifiedProtocolError
            SIP: 400 Bad request
            SIP: 400 Bad request
            */
            case ECCPErrorConnectionError:
            case ECCPErrorCCInvalidTransitNetworkSelection:
                err = KErrGsmCCInvalidTransitNetworkSelection;
                break;

            /**
            KErrGsmCCIncompatibleDestination
            SIP: 400 Bad request
            */
            case ECCPErrorBadRequest:
            case ECCPErrorCCIncompatibleDestination:
                err = KErrGsmCCIncompatibleDestination;
                break;

            /**
            KErrGsmCCOperatorDeterminedBarring
            SIP: 403 Forbidden
            */
            case ECCPErrorNumberBarred:
                err = KErrGsmCCOperatorDeterminedBarring;
                break;

            /**
            KErrGsmCCNormalUnspecified
            400 Bad request
            */
            case ECCPErrorNoAnswerForVideo:
                err = KErrGsmCCNormalUnspecified;
                break;

            /**
            KErrPhoneEngineNoWcdmaNetwork
            */
            case ECCPErrorVideoCallNotSupportedByNetwork:
            //  err = KErrPhoneEngineNoWcdmaNetwork;
                break;

            /**
            KErrPhoneEngineVideoCallNotAllowedDuringRestore
            */
            case ECCPErrorVideoCallNotAllowedDuringRestore:
            //  err = KErrPhoneEngineVideoCallNotAllowedDuringRestore;
                break;
                
            /**
            KErrPhoneEngineVideoCallSetupFailed
            */
            case ECCPErrorVideoCallSetupFailed:
            //  err = KErrPhoneEngineVideoCallSetupFailed;
                break;

            /**
            KErrDiagnosticInfoBarredWithCUG
            */
            case ECCPErrorCUGOutgoingCallsBarred:
    //            err = KErrDiagnosticInfoBarredWithCUG;
                break;

            /**
            KErrDiagnosticInfoBarredNoCUG
            */
            case ECCPErrorCUGNotSelected:
     //           err = KErrDiagnosticInfoBarredNoCUG;
                break;

            /**
            KErrDiagnosticInfoBarredUnknownCUG
            */
            case ECCPErrorCUGIndexUnknown:
     //           err = KErrDiagnosticInfoBarredUnknownCUG;
                break;

            /**
            KErrDiagnosticInfoBarredIncompatibleCUG
            */
            case ECCPErrorCUGIndexIncompatible:
    //            err = KErrDiagnosticInfoBarredIncompatibleCUG;
                break;

            /**
            KErrDiagnosticInfoBarredFailureCUG
            */
            case ECCPErrorCUGCallsFailure:
     //           err = KErrDiagnosticInfoBarredFailureCUG;
                break;

            /**
            KErrDiagnosticInfoBarredClirNotSubscribed
            */
            case ECCPErrorCLIRNotSubscribed:
     //           err = KErrDiagnosticInfoBarredClirNotSubscribed;
                break;

            /**
            KErrDiagnosticInfoBarredCCBSPossible
            */
            case ECCPErrorCCBSPossible:
      //          err = KErrDiagnosticInfoBarredCCBSPossible;
                break;

            /**
            KErrDiagnosticInfoBarredCCBSNotPossible
            */
            case ECCPErrorCCBSNotPossible:
      //          err = KErrDiagnosticInfoBarredCCBSNotPossible;
                break;

            /**
            Secure call failed.
            */
            case ECCPSecureCallFailed:
                err = KErrGsmCCChannelUnacceptable;
                break;

            /**
            Emergency call failed.
            */
            case ECCPEmergencyFailed:
                err = KErrGsmCCAcmGreaterThanAcmMax;
                break;

            /**
            Transfer failed.
            */
            case ECCPTransferFailed:
            /**
            Local hold failed.
            */
            case ECCPLocalHoldFail:
            /**
            Local resume failed.
            */
            case ECCPLocalResumeFail:
                err = KErrGsmCCNormalUnspecified;
                break;

            /**
            When none of other errors map and received a 3XX response. 
            */
            case ECCPRedirection:
                err = KErrGsmCCUnassignedNumber;
                break;

            /**
            When none of other errors map and received a 4XX response. 
            */
            case ECCPRequestFailure:
                err = KErrGsmCCUnassignedNumber;
                break;

            /**
            When none of other errors map and received a 5XX response. 

            */
            case ECCPServerFailure:
                err = KErrGsmCallServerFail;
                break;

            /** 
            When none of other errors map and received a 6XX response. 
            */
            case ECCPGlobalFailure:
                err = KErrGsmCCUnspecifiedProtocolError;
                break;

            /** Rtp detection media error. */
            case ECCPErrorMedia:
                err = KErrGsmCCUnspecifiedProtocolError; // TODO
                break;

            /**
            KErrGsmCCServiceNotImplemented
            SIP 488 Not Acceptable Here 
            SIP 606 Not Acceptable 
            */
            case ECCPErrorNotAcceptable:
            case ECCPErrorCCServiceNotImplemented:
                err = KErrGsmCCServiceNotImplemented;
                break;
            
            /* Access denied error */
            case ECCPErrorAccessDenied:
                err = KErrAccessDenied;
                break;


            /**
            KErrGsmCCDestinationOutOfOrder
            */
            case ECCPErrorCCDestinationOutOfOrder:                
                err = KErrGsmCCDestinationOutOfOrder;
                break;

            /**
            KErrGsmCCResourceNotAvailable
            */
            case ECCPErrorCCResourceNotAvailable:
                err = KErrGsmCCResourceNotAvailable;
                break;

            /** 
            KErrGsmCCIncompatibleMessageInCallState;
            */
            case ECCPErrorCCIncompatibleMessageInCallState:
                err = KErrGsmCCIncompatibleMessageInCallState;
                break;

            /** 
            KErrGsmCCIncompatibleMessageInProtocolState;
            */
            case ECCPErrorCCIncompatibleMessageInProtocolState:
                err = KErrGsmCCIncompatibleMessageInProtocolState;
                break;


            /** 
            KErrGsmCCNormalCallClearing;
            */
            case ECCPErrorCCNormalCallClearing:
                err = KErrGsmCCNormalCallClearing;
                break;


            /** 
            KErrGsmCCUserAlertingNoAnswer;
            */
            case ECCPErrorCCUserAlertingNoAnswer:
                err = KErrGsmCCUserAlertingNoAnswer;
                break;

            /** 
            KErrGsmCCCallRejected;
            */
            case ECCPErrorCCCallRejected:
                err = KErrGsmCCCallRejected;
                break;


            /** 
            KErrGsmCCPreemption;
            */
            case ECCPErrorCCPreemption:
                err = KErrGsmCCPreemption;
                break;

            /** 
            KErrGsmCCResponseToStatusEnquiry;
            */
            case ECCPErrorCCResponseToStatusEnquiry:
                err = KErrGsmCCResponseToStatusEnquiry;
                break;


            /** 
            KErrGsmCCInvalidMandatoryInformation;
            */
            case ECCPErrorCCInvalidMandatoryInformation:
                err = KErrGsmCCInvalidMandatoryInformation;
                break;


            /** 
            KErrGsmCCNonExistentMessageType;
            */
            case ECCPErrorCCNonExistentMessageType:
                err = KErrGsmCCNonExistentMessageType;
                break;


            /** 
            KErrGsmCCNonExistentInformationElement;
            */
            case ECCPErrorCCNonExistentInformationElement:
                err = KErrGsmCCNonExistentInformationElement;
                break;


            /** 
            KErrGsmCCNoChannelAvailable;
            */
            case ECCPErrorCCNoChannelAvailable:
                err = KErrGsmCCNoChannelAvailable;
                break;


            /** 
            KErrGsmCCRequestedFacilityNotSubscribed;
            */
            case ECCPErrorCCRequestedFacilityNotSubscribed:
                err = KErrGsmCCRequestedFacilityNotSubscribed;
                break;


            /** 
            KErrGsmCCIncomingCallsBarredInCug;
            */
            case ECCPErrorCCIncomingCallsBarredInCug:
                err = KErrGsmCCIncomingCallsBarredInCug;
                break;


            /** 
            KErrGsmCCBearerCapabilityNotAuthorised;
            */
            case ECCPErrorCCBearerCapabilityNotAuthorised:
                err = KErrGsmCCBearerCapabilityNotAuthorised;
                break;


            /** 
            KErrGsmCCBearerCapabilityNotCurrentlyAvailable;
            */
            case ECCPErrorCCBearerCapabilityNotCurrentlyAvailable:
                err = KErrGsmCCBearerCapabilityNotCurrentlyAvailable;
                break;


            /** 
            KErrGsmCCServiceNotAvailable;
            */
            case ECCPErrorCCServiceNotAvailable:
                err = KErrGsmCCServiceNotAvailable;
                break;


            /** 
            KErrGsmCCBearerServiceNotImplemented;
            */
            case ECCPErrorCCBearerServiceNotImplemented:
                err = KErrGsmCCBearerServiceNotImplemented;
                break;


            /** 
            KErrGsmCCOnlyRestrictedDigitalInformationBCAvailable;
            */
            case ECCPErrorCCOnlyRestrictedDigitalInformationBCAvailable:
                err = KErrGsmCCOnlyRestrictedDigitalInformationBCAvailable;
                break;

            /** 
            KErrGsmCCUnspecifiedInterworkingError;
            */
            case ECCPErrorCCUnspecifiedInterworkingError:
                err = KErrGsmCCUnspecifiedInterworkingError;
                break;


            /** User not in Gug */
            case ECCPErrorUserNotInCug: // TODO
            default:
                CCELOGSTRING3("id:%d CCCECall::GetMobileCallInfo() Unhandled iExitCode %d",
                    iCallId, iExitCode);
                break;
            }

        pckg().iValid =   RMobileCall::KCallDuration
                        | RMobileCall::KCallId
                        | RMobileCall::KCallRemoteParty
                        | RMobileCall::KCallDialledParty
                        | RMobileCall::KCallExitCode
                        | RMobileCall::KCallEmergency
                        | RMobileCall::KCallSecurity;

        pckg().iService = RMobilePhone::EVoiceService;
        pckg().iStatus = state;
        pckg().iCallId = iCallId+1;
        pckg().iExitCode = err;
        pckg().iEmergency = EFalse;
        pckg().iForwarded = iCall->IsCallForwarded();
        pckg().iDuration = iCCEDurationTimer->NumberOfBeats();
        pckg().iStartTime = iCCEDurationTimer->StartTime();
        
        pckg().iRemoteParty.iDirection = 
            (RMobileCall::TMobileCallDirection)iCallDirection;
        pckg().iRemoteParty.iRemoteNumber.iTelNumber = 
                iCall->RemoteParty().Left( RMobilePhone::KMaxMobileTelNumberSize );
                
        const TDesC& remoteParty = pckg().iRemoteParty.iRemoteNumber.iTelNumber;  

        if ( CCPCall::EMobileTerminated == iCallDirection )
            {
            if ( remoteParty == KNullDesC )
                {
                pckg().iRemoteParty.iRemoteIdStatus = 
                    RMobileCall::ERemoteIdentitySuppressed;
                }
            else
                {
                pckg().iRemoteParty.iRemoteIdStatus = 
                    RMobileCall::ERemoteIdentityAvailable;
                }

            pckg().iForwarded = EFalse;
            
            pckg().iValid |= RMobileCall::KCallForwarded;
            }
        else
            {
            pckg().iRemoteParty.iRemoteIdStatus = 
                RMobileCall::ERemoteIdentityUnknown;
            }

        pckg().iDialledParty.iTelNumber = remoteParty;
        pckg().iCallName = KNullDesC;      

        // InfoV3
        if ( iCallState == CCPCall::EStateConnected )
            { 
            if ( iCall->IsSecured())
                {
                // Because ETel does not support VoIP, GSM ciphering is reported
                pckg().iSecurity = RMobilePhone::ECipheringGSM;
                }
            }
       else
            {
            pckg().iSecurity = RMobilePhone::ECipheringOff;
            }
        }
    else
        {
        CCELOGSTRING2("id:%d CCCECall::GetMobileCallInfo: Not found", iCallId);
        }
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// 
// ---------------------------------------------------------------------------
//
CCPCall::TCallState CCCECall::State() const
    {
    CCELOGSTRING3( "id:%d CCCECall::State() = %d", iCallId, iCallState );
    return iCallState;
    }
    
// ---------------------------------------------------------------------------
// TCCPTone Tone() const
//
// ---------------------------------------------------------------------------
//
TCCPTone CCCECall::Tone() const
    {
    TCCPTone tone = iCall->Tone();  
    CCELOGSTRING3( "id:%d CCCECall::Tone() = %d", iCallId, (TInt)tone );
    return tone;
    
    }
// ---------------------------------------------------------------------------
// CallDuration() const
// ---------------------------------------------------------------------------
//
TTimeIntervalSeconds CCCECall::CallDuration() const
    {
    return iCCEDurationTimer->NumberOfBeats();
    }


// ---------------------------------------------------------------------------
// StartTime() const
// ---------------------------------------------------------------------------
//
TDateTime CCCECall::StartTime() const
    {
    return iCCEDurationTimer->StartTime();
    }

// ---------------------------------------------------------------------------
// Caps() const
// ---------------------------------------------------------------------------
//
MCCECallObserver::TCCECallControlCaps CCCECall::Caps() const
    {
    CCELOGSTRING3( "id:%d CCCECall::Caps(): caps = %d", iCallId, iCall->Caps() );
    
    return (MCCECallObserver::TCCECallControlCaps)iCall->Caps();
    }

// ---------------------------------------------------------------------------
// SetParameters()
// ---------------------------------------------------------------------------
//
void CCCECall::SetParameters( const CCCECallParameters& aNewParams )
    {
    delete iCallParameters;
    iCallParameters = NULL;
    TRAP_IGNORE(iCallParameters = aNewParams.CloneL());
    }

// ---------------------------------------------------------------------------
// Parameters() const
// ---------------------------------------------------------------------------
//
const CCCPCallParameters& CCCECall::Parameters() const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
        {
        return iCall->Parameters();
        }
    return *iCallParameters;
    }

// ---------------------------------------------------------------------------
// Get transfer target
// ---------------------------------------------------------------------------
//
const TDesC& CCCECall::TransferTarget() const
    {
    // nothing here yet
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// AttendedTransfer()
// ---------------------------------------------------------------------------
//
TInt CCCECall::AttendedTransfer( const TDesC& /*aTransferTarget*/ )
    {
    CCELOGSTRING2( "id:%d CCCECall::AttendedTransfer()", iCallId );
    TInt error = KErrNone;
      
    if ( iTransferProvider )
        {
        CCCECall* toCall = iCallContainer.GetCall( 
                CCPCall::EStateConnected, this );
        
        // On cs world it is possible to transfer call to connecting
        // call
        if( !toCall )
            {
            toCall = iCallContainer.GetCall( 
                    CCPCall::EStateConnecting, this );
            
            TBool condition( EFalse );
            
            if ( FeatureManager::FeatureSupported( 
                    KFeatureIdFfVoiceCallContinuity ) )
                {
                if ( toCall && 
                     toCall->Parameters().CallType() != 
                         CCPCall::ECallTypeCSVoice )
                    {
                    condition = ETrue;
                    }
                }
            else 
                {
                if ( toCall && toCall->ImplementationUid() != KCSProviderUid )
                    {
                    condition = ETrue;
                    }
                }            
            if ( condition )    
            
                {
                toCall = NULL;
                }
            }
            
        if( toCall )
            {
            error = iTransferProvider->AttendedTransfer( toCall->GetCCPCall() );      
            
            if ( error == KErrNone )
                {
                iActiveRequest = ECCETransfer;
                }
            }    
        else
            {
            error = KErrNotReady;
            }
        }
    else
        {
        error = KErrNotSupported;
        }

    return error;
    }
    
// ---------------------------------------------------------------------------
// UnattendedTransfer( const TDesC& /*aTarget*/ )
// ---------------------------------------------------------------------------
//
TInt CCCECall::UnattendedTransfer( const TDesC& aTarget )
    {
    CCELOGSTRING2("id:%d CCCECall::UnattendedTransfer():IN", iCallId);
    TInt error = KErrNone;
   
    if( iTransferProvider )
        {
        error = iTransferProvider->UnattendedTransfer( aTarget );
        
        if ( error == KErrNone )
            {
            iActiveRequest = ECCEUnattendedTransfer;
            }
        }
    else
        {
        error = KErrNotSupported;
        }

    CCELOGSTRING2("id:%d CCCECall::UnattendedTransfer():OUT", iCallId);
    return error;
    }
    
// ---------------------------------------------------------------------------
// AcceptTransfer( TBool /*aAccept*/ )
// ---------------------------------------------------------------------------
//
TInt CCCECall::AcceptTransfer( const TBool aAccept )
    {
    CCELOGSTRING3("id:%d CCCECall::AcceptTransfer():IN aAccept=%d", iCallId,aAccept);
    TInt error = KErrNone;
   
    if( iTransferController &&
        iTransferProvider )
        {
        if( CCPCall::EStateConnected == iCallState ||
            CCPCall::EStateHold == iCallState )
            {
            iTransferProvider->AcceptTransfer( aAccept );
            if( aAccept )
                {
                iTransferController->SetTransferAccepted( aAccept );    
                }
            else
                {
                iTransferController->ReleaseTransferCall();    
                }
            }
        else
            {
            iTransferController->ReleaseTransferCall();    
            }   
        }
     else
        {
        error = KErrNotSupported;
        }
    CCELOGSTRING2("id:%d CCCECall::AcceptTransfer():OUT", iCallId);
    return error;
    }
    
// ---------------------------------------------------------------------------
// ForwardCallToAddressChoices()
// ---------------------------------------------------------------------------
//
const CDesC8Array& CCCECall::GetForwardAddressChoicesL()
    {
    CCELOGSTRING2("id:%d CCCECall::GetForwardAddressChoicesL()", iCallId);
    const CDesC8Array* array = NULL;
    
    if( iForwardProvider )
        {
        array = &iForwardProvider->GetForwardAddressChoicesL();
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
        
    return *array;
    }
    
// ---------------------------------------------------------------------------
// ForwardCallToAddress( TInt /*aIndex*/ )
// ---------------------------------------------------------------------------
//    
void CCCECall::ForwardToAddressL( TInt aIndex )
    {
    CCELOGSTRING2("id:%d CCCECall::ForwardCallToAddress()", iCallId);
    if( iForwardProvider )
        {
        iForwardProvider->ForwardToAddressL( aIndex );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// Check if other call needs to placed on hold
// If no, continue call
// ---------------------------------------------------------------------------
//
TInt CCCECall::Dial( const TDesC8& aCallParams )
    {
    CCELOGSTRING2( "id:%d CCCECall::Dial()", iCallId );
    TInt error = KErrNone;

    if( iCallState != CCPCall::EStateIdle )
        {
        return KErrNotReady;
        }
    
    iCallParams = &aCallParams;
    iActiveRequest = ECCEDial;
    
    // If this is unattended transfer dial
    if( iTransferController &&
        iCall == iTransferController->TransferCall() )      
        {
        CCELOGSTRING2( "id:%d CCCECall::Dial(): TransferDial", iCallId);
        // Must wait that original call is disconnected before transfer
        // dial can be done
        if( MCCPCallObserver::ECCPStateDisconnecting == 
            iTransferController->OriginatorCall()->State() )
            {
            error = DoPendingRequest();
            }
        else
            {
            iTransferController->SetTransferDialPending( ETrue );
            }
        }
    else
        {
        
        CCCECall* call = iCallContainer.GetCall( 
                CCPCall::EStateConnected, this);
            
        if(call)
            {
            if( 0 == iCallContainer.ConferenceCall().EnumerateCalls() )
                {
                if( (iCallContainer.GetCall( 
                        CCPCall::EStateHold, this))  && (ECCENone != iActiveRequest))
                    {
                    CCELOGSTRING2( "id:%d CCCECall::Dial() failed. Too many calls. ", iCallId);
                    error = KErrInUse;
                    }
                else
                    {
                    error = call->HoldNoNotify();
                    }                                     
                }
            else
                {
                error = iCallContainer.ConferenceCall().Swap();
                }
            }
        else
            {
            // Continue dial if no other connected calls exist
            error = DoPendingRequest();
            }
        }
    return error;
    }
    
// ---------------------------------------------------------------------------
// From class MCCECSCall.
// NoFDNCheck()
// ---------------------------------------------------------------------------
//
void CCCECall::NoFDNCheck()
    {
    if ( IsCsPlugin() )    
        {
        static_cast<MCCPCSCall*>(iCall)->NoFDNCheck();
        }
    }
    
// ---------------------------------------------------------------------------
// From class MCCECall.
// Answer()
// ---------------------------------------------------------------------------
//
TInt CCCECall::Answer()
    {
    CCELOGSTRING2( "id:%d CCCECall::Answer()", iCallId );
    TInt error = KErrNone;
    iActiveRequest = ECCEAnswer;

    CCCECall* call = iCallContainer.GetCall( 
            CCPCall::EStateConnected, this);
        
    if(call)
        {
        if( 0 == iCallContainer.ConferenceCall().EnumerateCalls() )
            {
            if( (iCallContainer.GetCall(  
                    CCPCall::EStateHold, this))  && (ECCENone != iActiveRequest))
                {
                CCELOGSTRING2( "id:%d CCCECall::Answer() failed. Too many calls. "
                    , iCallId);
                error = KErrInUse;
                iActiveRequest = ECCENone; 
                }
            else
                {
                error = call->HoldNoNotify();
                }                                     
            }
        else
            {
            error = iCallContainer.ConferenceCall().Swap();
            }
        }
    else
        {
        // Continue answering if no other connected calls exist
        error = DoPendingRequest();
        }

    return error;
    }

// ---------------------------------------------------------------------------
// DoPendingRequest()
// This does the pending request. It can be called from cccecallcontainer
// ---------------------------------------------------------------------------
//
TInt CCCECall::DoPendingRequest()
    {
    CCELOGSTRING2( "id:%d CCCECall::DoPendingRequest()", iCallId );
    TInt error = KErrNone;
    
    if( iActiveRequest == ECCEDial )
        {
        CCELOGSTRING2("id:%d CCCECall::DoDial", iCallId);

        if ( IsCsPlugin() )
            {
            error = static_cast<MCCPCSCall*>(iCall)->Dial(*iCallParams);
            }
        else if( iCall )
            {
            error = iCall->Dial();
            }
        }
    else if( iActiveRequest == ECCEAnswer ||
             iActiveRequest == ECCEReplaceActive )
        {
        CCELOGSTRING2("id:%d CCCECall::DoAnswer", iCallId);
        error = iCall->Answer();
        }
    else
        {
        error = KErrNotFound;
        }
        
    return error;
    }

// ---------------------------------------------------------------------------
// DialEmergencyCall( const TDesC16& /*aNumber*/ )
// ---------------------------------------------------------------------------
//
void CCCECall::DialEmergencyCall( const TDesC16& /*aNumber*/ )
    {
    CCELOGSTRING2("id:%d CCCECall::DialEmergencyCall()", iCallId);
    }

// ---------------------------------------------------------------------------
// From class MCCECall.
// Close call object
// ---------------------------------------------------------------------------
//
TInt CCCECall::Release()
    {
    CCELOGSTRING2( "id:%d CCCECall::Release()", iCallId );
    
    if( CCPCall::EStateIdle != iCallState )
        {
        iCallContainer.HandleCallStatusChange( 
                CCPCall::EStateIdle, 
            this, CCECallEnums::ENotifySynchronously );    
        }
     
    // Stop duration timer if it is not yet stopped.
    iCCEDurationTimer->Stop();
    // Reset timer observer
    iCCEDurationTimer->SetObserver( NULL );
     
    // Delete converged call
    // Release call id
    iCallContainer.ReleaseCallInfoCallId( iCallId );

    if( iCall )
        {
        iCallContainer.ReleaseCall(*iCall);    
        }
    
    Reset();

    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// Request is to an ringing/waiting state call.
// Active call if found is hang-up
// When active call goes to idle incoming call is answered
// ---------------------------------------------------------------------------
//
TInt CCCECall::ReplaceActive()
    {
    CCELOGSTRING2( "id:%d CCCECall::ReplaceActive()", iCallId );
    TInt error = KErrNone;
    iActiveRequest = ECCEReplaceActive;
    CCCECall* callConnected = iCallContainer.GetCall( 
            CCPCall::EStateConnected, this);
        
    if ( callConnected )
        { 
        if ( !iCallContainer.ConferenceCall().IsPartOfConference( *callConnected ) )
            {    
            error = callConnected->GetCCPCall().HangUp();                                             
            }
        else if ( iCallContainer.ConferenceCall().State() == 
            MCCPConferenceCallObserver::ECCPConferenceActive )
            {
            error = iCallContainer.ConferenceCall().HangUp();
            }
            
        if ( KErrNone != error )
            {
            iActiveRequest = ECCENone;
            }
        }
    else
        {
        // Continue answering if no other connected calls exist
        error = DoPendingRequest();
        }

    return error;
    }
    
// ---------------------------------------------------------------------------
// Hold call but do not notify observer when completed
// ---------------------------------------------------------------------------
//
TInt CCCECall::HoldNoNotify()
    {
    CCELOGSTRING2( "id:%d CCCECall::HoldNoNotifyL()", iCallId );
    
    return iCall->Hold();
    }


// ---------------------------------------------------------------------------
// Resume call but do not notify observer when completed
// ---------------------------------------------------------------------------
//
TInt CCCECall::ResumeNoNotify()
    {
    CCELOGSTRING2( "id:%d CCCECall::ResumeNoNotifyL()", iCallId );
    
    return iCall->Resume();
    }

// ---------------------------------------------------------------------------
// HandleTransfer()
// Notifies phoneengine about received transfer request from remote end
// ---------------------------------------------------------------------------
//
void CCCECall::HandleTransfer(const TBool aAttented, 
    CCCETransferController& aTransferController )
    {
    iTransferController = &aTransferController;
    if( iObserver )
        {
        iObserver->HandleTransfer( aAttented,
            iTransferProvider->TransferTarget() );
        }
    else
        {
        CCELOGSTRING( "CCCECall::HandleTransfer: observer was NOT set!" );
        }
    }

// ---------------------------------------------------------------------------
// SetTransferController()
// 
// ---------------------------------------------------------------------------
//
void CCCECall::SetTransferController( 
    CCCETransferController* aTransferController )
    {
    iTransferController = aTransferController;
    }

// ---------------------------------------------------------------------------
// Sets call index 
// ---------------------------------------------------------------------------
//
void CCCECall::SetNewCallIndex()
    {
    if ( iCallIndex == KInvalidCallIndex )
        {
        TCCECallIndex callIndex( iCallContainer );
        iCallIndex = callIndex.CurrentCallIndex();
        }
    }
    
// ---------------------------------------------------------------------------
// Reset call object to initial state 
// ---------------------------------------------------------------------------
//
void CCCECall::Reset()
    {
    CCELOGSTRING2( "id:%d  CCCECall::Reset()", iCallId );
    
    iActiveRequest = ECCENone;
    
    // only one observer at the moment. Clear it.
    if (iObserver)
        {
        iObserver->SetMCCECallObserver(NULL);
        }
     
   
    // This object is no longer in use.
    iExitCode = ECCPErrorNone;
    
    iCallDirection = CCPCall::EDirectionUnknown;
    iCallState    = CCPCall::EStateIdle;

    iImplementationUid.iUid = 0;
    
    iCall = NULL;
    delete iCallParameters;
    iCallParameters = NULL;
    iCallParams = NULL;
       
    iDoNotReportRequest = KErrNotFound;

    iTransferProvider = NULL;
    iTransferController = NULL;
    
    if( iCCEDurationTimer )
        {
        iCCEDurationTimer->Reset();     
        }
    }

// ---------------------------------------------------------------------------
// GetMobileDataCallCaps( TDes8& /*aCaps*/ ) const
// ---------------------------------------------------------------------------
//
TInt CCCECall::GetMobileDataCallCaps( TDes8& aCaps ) const
    {
    CCELOGSTRING2("id:%d CCCECall::GetMobileDataCallCaps()", iCallId);
    
    if ( IsCsPlugin() )
        {
        return static_cast<MCCPCSCall*>(iCall)->GetMobileDataCallCaps(aCaps);
        }
    else
        return KErrNotSupported;
    }


// ---------------------------------------------------------------------------
// LogDialedNumber()
// ---------------------------------------------------------------------------
//    
TBool CCCECall::LogDialedNumber() const
    {
    // CS related 
    CCELOGSTRING2("id:%d CCCECall::LogDialedNumber()", iCallId);
    
    if ( IsCsPlugin() )
        {
        return static_cast<MCCPCSCall*>(iCall)->LogDialedNumber();
        }
    // for other than CS always return true
    return ETrue;
    }

// ---------------------------------------------------------------------------
// SwitchAlternatingCall()
// ---------------------------------------------------------------------------
//    
TInt CCCECall::SwitchAlternatingCall()
    {
    CCELOGSTRING2("id:%d CCCECall::SwitchAlternatingCall()", iCallId);
    if ( IsCsPlugin() )
        {
        static_cast<MCCPCSCall*>(iCall)->SwitchAlternatingCall();
        return KErrNone;
        }
    else
        return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// ErrorOccured( TCCPCallError aError )
// ---------------------------------------------------------------------------
//
void CCCECall::ErrorOccurred( TCCPError aError, MCCPCall* /*aCall*/ ) 
    {
    if( !iCall )
        {
        CCELOGSTRING2("id:%d CCCECall::ErrorOccurred()::Call not active", iCallId);
        return;
        }
        
    CCELOGSTRING3("id:%d CCCECall::ErrorOccurred: Error = %d", 
        iCallId, aError );

    iExitCode = aError;
    iCallContainer.ErrorOccurred( this, aError );
    
     // Complete request to observer
    if ( ECCENone != iActiveRequest )
        {
        RequestComplete( aError );
        }
    else
        {
        if( iObserver )
            {
            iObserver->ErrorOccurred(aError);
            }
        else
            {
            CCELOGSTRING( "CCCECall::ErrorOccurred: observer was NOT set!" );
            }
        }
    
    }

// ---------------------------------------------------------------------------
// RequestComplete( TInt aError )
// ---------------------------------------------------------------------------
//
void CCCECall::RequestComplete( TCCPError aError )
    {
    CCELOGSTRING4("id:%d CCCECall::RequestComplete: Request = %d, Error = %d",iCallId
        ,iActiveRequest, aError );

    // Complete request to observer
    if ( ECCENone != iActiveRequest )
        {
        NotifyRequestComplete( iActiveRequest, aError );
        }
    else
        {
        CCELOGSTRING2("id:%d CCCECall::RequestComplete: No active request.", iCallId);
        }

    // Reset active request
    iActiveRequest = ECCENone;
    }

// ---------------------------------------------------------------------------
// CallEventOccurred( TCCPCallEvent aEvent )
// ---------------------------------------------------------------------------
//
void CCCECall::CallEventOccurred( TCCPCallEvent aEvent, MCCPCall* /*aCall*/ ) 
    {
    if( !iCall )
        {
        CCELOGSTRING2("id:%d CCCECall::CallEventOccurred()::Call not active", iCallId);
        return;
        }


    if( !iObserver->GetMCCECallObserver() ) 
        {
        CCELOGSTRING( "CCCECall::CallEventOccurred: observer was NOT set!" );
        return;
        }


    CCELOGSTRING4("id:%d CCCECall::CallEventOccurred() Event = %d CallId = %d", 
        iCallId, aEvent, iCallId );

    switch ( aEvent )
        {
        /** Call has been placed on hold as a result of a local action. */
        case ECCPLocalHold:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalHold );
            break;
        /** Call has been placed on hold by the remote connected party. */
        case ECCPRemoteHold:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteHold );
            break;
        /** Call has been resumed as a result of a local action. */
        case ECCPLocalResume:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalResume );
            break;
        /** Call has been resumed by the remote connected party. */
        case ECCPRemoteResume:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteResume );
            break;
        /** Outgoing call has been barred by the local party. */
        case ECCPLocalBarred:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalBarred );
            break;
        /** Outgoing call has been barred by the remote party. */
        case ECCPRemoteBarred:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteBarred );
            break;
        /** Call is waiting at the remote end. */
        case ECCPRemoteWaiting:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteWaiting );
            break;
        /** Call has been terminated by the remote party. */
        case ECCPRemoteTerminated:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteTerminated);
            break;
        /** Call is in queue */
        case ECCPQueued:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteWaiting );
            break;
        /** VoIP specific */
        case ECCCSPEarlyMediaStarted:
            iObserver->CallEventOccurred( MCCECallObserver::EVoIPEventEarlyMediaStarted );
            break;
        /** Call is secure */
        case ECCPSecureCall:
            iObserver->CallEventOccurred( MCCECallObserver::ESecureSession );
            break;
        /** Call is not secure */
        case ECCPNotSecureCall:
            iObserver->CallEventOccurred( MCCECallObserver::ENotSecureSession );
            break;
        /** Secure call signaling is not possible */
        case ECCPNotSecureSessionWithSips:
            iObserver->CallEventOccurred( MCCECallObserver::EVoIPEventNotSecureSessionWithSips );
            break;
        /** Call has been added to remote conference call at remote end */
        case ECCPRemoteConferenceCreate:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteConferenceCreate );
            break;
        /** CS call specific event */
        case ECCPCSDataPortLoaned:
            iObserver->CallEventOccurred( MCCECallObserver::ECCECSDataPortLoaned );
            break;
        /** CS call specific event */
        case ECCPCSDataPortRecovered:
            iObserver->CallEventOccurred( MCCECallObserver::ECCECSDataPortRecovered );
            break;
        /** Remote party info target meaning has changed */
        case ECCPNotifyRemotePartyInfoChange:
            {
            //Ho occurred, call type has changed
            if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
                {
                CCELOGSTRING( "CCCECall::CallEventOccurred: ECCPNotifyRemotePartyInfoChange!" );
                delete iCallParameters;
                iCallParameters = NULL;
                
                TRAP_IGNORE( iCallParameters = iCall->Parameters().CloneL() );
                }           
            iCallContainer.AirTimeDurationSwap(
                                     iCall->Parameters().CallType() );
            iObserver->CallEventOccurred( MCCECallObserver::ECCENotifyRemotePartyInfoChange );
            }
            break;
        /** Call secure status cannot be determined */
        case ECCPSecureNotSpecified:
            iObserver->CallEventOccurred( MCCECallObserver::ECCESecureNotSpecified );
            break;
        default:
            CCELOGSTRING3("id:%d CCCECall::SessionEventOccurred: Unhandled event = %d", iCallId,
                aEvent );
            break;
        }
    }

// ---------------------------------------------------------------------------
// CallCapsChanged( TUint32 aCapsFlags )
// ---------------------------------------------------------------------------
//
void CCCECall::CallCapsChanged( TUint32 aCapsFlags, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING2("id:%d CCCECall::CallCapsChanged()", iCallId);
    if( !iCall )
        {
        CCELOGSTRING2("id:%d CCCECall::CallCapsChanged()::Call not active", iCallId);
        return;
        }
        
    if( iObserver )
        {
        iObserver->CallCapsChanged( (MCCECallObserver::TCCECallControlCaps)aCapsFlags );
        }
    else
        {
        CCELOGSTRING( "CCCECall::CallCapsChanged: observer was NOT set!" );
        }
    }

// ---------------------------------------------------------------------------
// TransferEventOccurred()
// ---------------------------------------------------------------------------
//
void CCCECall::TransferEventOccurred( TCCPTransferEvent aEvent )
    {
    CCELOGSTRING2("id:%d CCCECall::TransferEventOccurred()", iCallId);
    
    if( !iObserver->GetMCCECallObserver() )
        {
        CCELOGSTRING( "CCCECall::TransferEventOccurred: observer was NOT set!" );
        return;
        }
    
    switch ( aEvent )
        {
       /** Call has been transferred to another remote party as a result 
        of a local action. This is the result of transfer being succesfully 
        completed */
        case ECCPLocalTransfer:
            iObserver->CallEventOccurred( MCCECallObserver::ELocalTransfer );
            break;
       /** Outgoing call is transfering. The transfer request has now been 
        accepted by the current call recipient. */
        case ECCPRemoteTransferring:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteTransferring );
            break;
       /** Outgoing transfer call is alerting. Current call recipinet has sent
        connection request to transfer target*/
        case ECCPRemoteTransferAlerting:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteTransferAlerting );
            break;

        default:
            CCELOGSTRING3("id:%d CCCECall::TransferEventOccurred: Unhandled event = %d", iCallId,
                aEvent );
            break;
        }
    }

// ---------------------------------------------------------------------------
// ForwardEventOccurred()
// ---------------------------------------------------------------------------
//
void CCCECall::ForwardEventOccurred( 
    const MCCPForwardObserver::TCCPForwardEvent aEvent )
    {
    CCELOGSTRING2("id:%d CCCECall::ForwardEventOccurred()", iCallId);

    if( !iObserver->GetMCCECallObserver() )
        {
        CCELOGSTRING( "CCCECall::ForwardEventOccurred: observer was NOT set!" );
        return;
        }
    
    switch ( aEvent )
        {
        /** Call is being forwarded by the remote party. */
        /** MovedTemporarily is handled as forwarding */
        case ECCPRemoteForwarding:
        case ECCPMovedTemporarily:
            iObserver->CallEventOccurred( MCCECallObserver::ERemoteForwarding );
            break;
        /** Multiple call forward choices are available */
        case ECCPMultipleChoices:
            iObserver->CallEventOccurred( MCCECallObserver::EVoIPEventMultipleChoices );
            break;
        /** Call moved permanently */
        case ECCPMovedPermanentlyEvent:
            iObserver->CallEventOccurred( MCCECallObserver::EVoIPEventMovedPermanently );
            break;
        /** Call is forwarded */
        case ECCPForwarded:
        default:
            CCELOGSTRING3("id:%d CCCECall::ForwardEventOccurred: Unhandled event = %d", iCallId,
                aEvent );
            break;
        }
    }

// ---------------------------------------------------------------------------
// CallStateChanged( TCCPCallState aState )
// ---------------------------------------------------------------------------
//
void CCCECall::CallStateChanged( TCCPCallState aState, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING3("id:%d CCCECall::CallStateChanged() State = %d", 
        iCallId, aState );
    
    CallStateChanged( aState, EFalse );
    }

// ---------------------------------------------------------------------------
// CallStateChangedWithInband( TCCPCallState aState )
// ---------------------------------------------------------------------------
//
void CCCECall::CallStateChangedWithInband( TCCPCallState aState, 
                                           MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING3("id:%d CCCECall::CallStateChangedWithInband() State = %d", 
       iCallId , aState );
        
    CallStateChanged( aState, ETrue );
    }

// ---------------------------------------------------------------------------
// CallStateChanged( TCCPCSCallState aState )
// ---------------------------------------------------------------------------
//
void CCCECall::CallStateChanged( TCCPCallState aState, TBool aInband )
    {
    if( !iCall )
        {
        CCELOGSTRING2("id:%d CCCECall::CallStateChanged()::Call not active", iCallId);
        return;
        }
        
    TInt notify = EFalse;
    CCPCall::TCallState state( CCPCall::EStateIdle);
        
    switch ( aState )
        {
        case ECCPStateQueued:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Queued", iCallId);
            // Call index becomes available when call gets to dialling or ringing/queued state.
            // Idle call does not have call index.
            SetNewCallIndex();
            if ( iCallDirection == CCPCall::EMobileTerminated )
                {
                state = CCPCall::EStateQueued;
                notify = ETrue;
                }
            break;
        
        case ECCPStateDialling:
            // Call index becomes available when call gets to dialling or ringing/queued state.
            // Idle call does not have call index.
            SetNewCallIndex();

            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Dialling", iCallId);
            state = CCPCall::EStateDialling;
            notify = ETrue;
                
            break;   
            
        case ECCPStateAnswering:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Answering", iCallId);
            state = CCPCall::EStateAnswering;
            notify = ETrue;
                
            break;    
   
        case ECCPStateConnecting:
        case ECCPStateRinging:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Connecting/Ringing", iCallId);
            if ( iCallDirection == CCPCall::EMobileTerminated )
                {
                state = CCPCall::EStateRinging;
                // Call index becomes available when call gets to dialling or incoming state.
                // Idle call does not have call index.
                SetNewCallIndex();
                }
            else
                {
                if ( ECCEDial == iActiveRequest )
                    {
                    RequestComplete( ECCPErrorNone );
                    }
                    
                state = CCPCall::EStateConnecting;
                }
            notify = ETrue;
            break;

        case ECCPStateConnected:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Connected", iCallId);
            // Start call duration timer
            iCCEDurationTimer->Start();

            // We are either dialling or answering
            if ( ECCEAnswer == iActiveRequest ||
                 ECCEResume == iActiveRequest ||
                 ECCEDial == iActiveRequest ||
                 ECCEReplaceActive == iActiveRequest ) 
                {
                RequestComplete( ECCPErrorNone );
                }
                
            state = CCPCall::EStateConnected;
            notify = ETrue;
            break;

        case ECCPStateDisconnecting:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Disconnecting", iCallId);
            // Stop duration timer
            iCCEDurationTimer->Stop();

            if ( ECCETransfer == iActiveRequest )
                {
                RequestComplete( ECCPErrorNone );
                }

            state = CCPCall::EStateDisconnecting;
            notify = ETrue;
            break;

        case ECCPStateIdle:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Disconnected/Idle", iCallId);
            // Stop duration timer
            iCCEDurationTimer->Stop();

            // Remove call from conference before it is notified elsewhere
            if( iCallContainer.ConferenceCall().IsPartOfConference(*this) )
                {
                MCCPConferenceCallObserver& confObserver = iCallContainer.ConferenceCall(); 
                confObserver.ConferenceCallEventOccurred(
                     MCCPConferenceCallObserver::ECCPConferenceCallRemoved, iCall );
                }

            if ( ECCENone != iActiveRequest )
                {
                RequestComplete( ECCPErrorNone );
                }

            state = CCPCall::EStateIdle;
            iCallIndex = KInvalidCallIndex; // Call index not valid anymore in idle state
            notify = ETrue;
           
            if( !iObserver->GetMCCECallObserver() )
                {
                notify = EFalse;
                Release();
                
                }
            break;

        case ECCPStateHold:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Hold", iCallId);
            state = CCPCall::EStateHold;
            if ( ECCEHold == iActiveRequest )
                {
                RequestComplete( ECCPErrorNone );
                }
                
            notify = ETrue;
            break;

        case ECCPStateForwarding:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Forwarding", iCallId);
            // Notify observer
            // Forwarding state is notified to upstairs but is not set
            // to calls internal state
            if( iObserver )
                {
                if (aInband)
                    {
                    iObserver->CallStateChangedWithInband(CCPCall::EStateForwarding);  
                    }
                else
                    {
                    iObserver->CallStateChanged(CCPCall::EStateForwarding);   
                    }
                
                }
            break;

        case ECCPStateTransferring:
            CCELOGSTRING2("id:%d CCCECall::CallStateChanged: Transferring", iCallId);
            // Notify observer
            // Transferring state is notified to upstairs but is not set
            // to calls internal state
           if( iObserver )
                {
                if (aInband)
                    {
                    iObserver->CallStateChangedWithInband(CCPCall::EStateTransferring)  ;
                    }
                else
                    {
                    iObserver->CallStateChanged(CCPCall::EStateTransferring); 
                    }
                
                }
                
            break;

        default:
            CCELOGSTRING3("id:%d CCCECall::CallStateChanged: Unhandled state = %d", iCallId,
                aState );
            break;
        }
        
    if( notify )
        {
        NotifyCallStatusChange( state, aInband, CCECallEnums::ENotifyAsynchronously );
        }
    }
    
// ---------------------------------------------------------------------------
// Notify observers about call status change
// ---------------------------------------------------------------------------
//
void CCCECall::NotifyCallStatusChange( 
    CCPCall::TCallState aCallState, 
    TBool aInband, 
    CCECallEnums::TNotifyType aNotify )
    {
    CCELOGSTRING3("id:%d CCCECall::NotifyCallStatusChange: Status = %d",iCallId, aCallState );
    
    if( (iCallState == aCallState) && ( aCallState == MCCPCallObserver::ECCPStateIdle ))
        {
        CCELOGSTRING2("id:%d CCCECall::NotifyCallStatusChange: Same status, still Idle", iCallId );
        return;
        }
    
    iCallState = aCallState;

    if( iTransferController )
        {
        iTransferController->HandleCallStateChanged( this, aCallState );
        }
   
    // Notify observer
    if( iObserver && (aNotify != CCECallEnums::ENoNotify))
        {
       if( aInband )
            {
            iObserver->CallStateChangedWithInband( aCallState );
            }
        else
            {
            iObserver->CallStateChanged( aCallState );
            }
            
        }
    else
        {
        CCELOGSTRING2("id:%d CCCECall::NotifyCallStatusChange:No observer", iCallId );
        }

    // Notify CallContainer
    iCallContainer.HandleCallStatusChange( aCallState, this, aNotify );
    }

// ---------------------------------------------------------------------------
// Tells if CS plugin is used
// ---------------------------------------------------------------------------
//
TBool CCCECall::IsCsPlugin() const
    {
    TBool isCsPlugin( EFalse );
    if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
        {
        if ( iCall->Parameters().CallType() != 
            CCPCall::ECallTypePS )
            {
            isCsPlugin = ETrue;
            }
        }
    else
        {
        if( iImplementationUid == KCSProviderUid )
            {
            isCsPlugin = ETrue;
            }
        }
    return isCsPlugin;
    }

// End of file
