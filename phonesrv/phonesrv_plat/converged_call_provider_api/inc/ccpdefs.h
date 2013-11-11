/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common error etc defenitions for CCP plug-ins and CCE, PE and other phone components.
*
*/


#ifndef CCPDEFS_H
#define CCPDEFS_H

// INCLUDES
#include <e32base.h>

/*
Some error defines derived exterror.h. 
Abbreviations below:
CC = Call Control
MM = Mobility Management
RR = Radio Resource
SS - Supplementary Services
General TE-TA Errors - 0707
SMS - SMS
**/

const TUint KCCPPhoneSerialNumberSize = 50;        // 
const TUint KCCPSysUtilVersionTextLength = 64;     // from sysutil.h
const TUint KCCPPhoneModelIdSize = 50;             // from RMobilePhone::KPhoneModelIdSize
const TUint KCCPPhoneManufacturerIdSize = 50;      // fromRMobilePhone::KPhoneManufacturerIdSize
const TUint KCCPUUSIdSize                = 129;    // etelmm.h, as KMaxUUISize
const TUint KCCPAlphaIdMaxSize = 254;              // etelmm.h, as KAlphaIdMaxSize


/**
* CS specific information. Fetched from CS-plugin via CCE.
* No other plug-ins use this information.
* Used by CConvergedCallProvider::GetCSInfo.
*/
typedef class 
    {
    public:
    // Serial number
    TBuf<KCCPPhoneSerialNumberSize> iSerialNumber;
    
    // sw version
    TBuf<KCCPSysUtilVersionTextLength> iSWVersion;

    // PhoneModel
    TBuf<KCCPPhoneModelIdSize> iPhoneModel;

    // Manufacturer name
    TBuf<KCCPPhoneManufacturerIdSize> iManufacturerIdSize;
    } CSInfo;



/** 
* Same as in RMmCustomAPI.h. No default ctor though. 
*/
class TCCPLifeTimeData
    {
    public: // definitions

        enum TCCPLifeTimeDataCaps
            {
            ECCPLifeTimeDataCapsUnknown = 0x00000000,
            ECCPLifeTimeDataCapsLifeTime = 0x00000001,
            ECCPLifeTimeDataCapsManufacturerDate = 0x00000002
            };

    public: // internal attributes

        TUint32 iCaps;
        TUint32 iHours; // range 0-999999
        TUint8 iMinutes; // range 0-59
        TDateTime iManufacturingDate;
    };

typedef  TPckg<TCCPLifeTimeData> TCCPLifeTimeDataPckg;

namespace CCPCall
    {    
    
    /** Call states */
    enum TCallState
        {
        /** Indicates initial call state after call creation. Basic state. */
        EStateIdle,
        /** Indicates that the MT call is ringing but not answered yet by the local user. Basic state.*/
        EStateRinging,
        /** MO Call: the network notifies to the MS that the remote party is now ringing. Basic state.*/
        EStateConnecting,
        /** Indicates that call is connected and active. Basic state.*/
        EStateConnected,
        /** Indicates that call is disconnecting. Basic state.*/
        EStateDisconnecting,
        /** Indicates that the call is connected but on hold. Basic state.*/
        EStateHold,

        /** Call is transfering. Optional state.*/
        EStateTransferring,
        /** MO call is being forwarded at receiver end. Optional state. */
        EStateForwarding,
        /** Call is queued locally. Optional state. */
        EStateQueued,

        /** Indicates that the local user has answered the MT call but the network has not
            acknowledged the call connection yet.  Must be sent after MCCPCall::Answer method has been completed. */
        EStateAnswering,
       
        /** Indicates that the call is dialing. Must be sent after MCCPCall::Dial method has been completed. */
        EStateDialling,            
        };    
    
    enum TCallType
        {
        /** CS voice call */
        ECallTypeCSVoice = 0, 
        /** Video call.*/
        ECallTypeVideo,        
        /** VoIP (or any other than CS) call.*/
        ECallTypePS       
        };    
    
    enum TCallDirection
        {
        /** The direction of the call is unknown. */
        EDirectionUnknown,
        /** The call was originated by this phone, i.e. it is an outgoing call. */
        EMobileOriginated,
        /** The call was terminated by this phone, i.e. it is an incoming call. */
        EMobileTerminated
        };
    }    


/**
* CCP error and general value defenitions used for plug-ins, CCE and phone.
*
*  @since S60 3.2
*/

    /** Conference call errors. */
    enum TCCPConferenceCallError
       {
       /** Adding call to conference failed. */
       ECCPConferenceErrorAddCall = -1,
       /** Removing call from conference failed. */
       ECCPConferenceErrorRemoveCall = -2,
       /** Swap failed for conference. */
       ECCPConferenceErrorSwap = -3,
       /** Hold failed for conference. */
       ECCPConferenceErrorHold = -4,
       /** Resume failed for conference. */
       ECCPConferenceErrorResume = -5,
       /** One to one for specific call failed. */
       ECCPConferenceErrorGoOneToOne = -6,
       /** Adding current calls to conference failed. */
       ECCPConferenceErrorCurrentCallsToConference = -7,
       /** HangUp failed. */
       ECCPConferenceErrorHangUp  = -8
       };


    /** Inband tone mappings */
    enum TCCPTone
        {
        /**
        KErrGsmCCUserBusy - 1 TCCPErrorBusy
        inBandTone = EUserBusy;
        */
        ECCPToneUserBusy,

        /**
        KErrGsmCCNumberChanged - 1 TCCPErrorMovedPermanently
        KErrGsmCCResponseToStatusEnquiry - 0
        KErrGsmCCNormalUnspecified - 1 TCCPErrorNoAnswerForVideo
        inBandTone = ERadioPathNotAvailable;
        */
        ECCPToneRadioPathNotAvailable,
        
        /**
        KErrGsmCCNoChannelAvailable - 1 TCCPErrorNetworkBusy
        KErrGsmCCTemporaryFailure - 1 TCCPErrorNetworkBusy
        KErrGsmCCSwitchingEquipmentCongestion - 1 TCCPErrorNetworkBusy
        KErrGsmCCRequestedChannelNotAvailable - 1 TCCPErrorNetworkBusy
        KErrGsmCCQualityOfServiceNotAvailable -1 TCCPErrorQualityOfServiceNotAvailable
        KErrGsmCCBearerCapabilityNotCurrentlyAvailable - 0 
        inBandTone = ECongestion;
        */
        ECCPToneCongestion,                
   
        /**     
        KErrGsmCCUnassignedNumber -1 TCCPErrorNotReached
        KErrGsmCCNoRouteToDestination -1 TCCPErrorConnectionErrorRedial
        KErrGsmCCChannelUnacceptable -1 TCCPErrorUnacceptableChannel
        KErrGsmCCOperatorDeterminedBarring -1 TCCPErrorNumberBarred
        KErrGsmCCUserNotResponding - 1 TCCPErrorNotResponding 
        KErrGsmCCUserAlertingNoAnswer -1 TCCPErrorNotResponding
        KErrGsmCCCallRejected - 1 TCCPErrorNotAllowed
        KErrGsmCCNonSelectedUserClearing - 0
        KErrGsmCCDestinationOutOfOrder -1 TCCPErrorConnectionErrorRedial
        KErrGsmCCInvalidNumberFormat -1 TCCPErrorInvalidURI
        KErrGsmCCFacilityRejected -1 TCCPErrorRejected
        KErrGsmCCNetworkOutOfOrder -1 TCCPErrorNetworkOutOfOrder
        KErrGsmCCAccessInformationDiscarded -1 TCCPErrorAccessInformationDiscarded
        KErrGsmCCResourceNotAvailable -1 TCCPErrorConnectionErrorRedial
        KErrGsmCCRequestedFacilityNotSubscribed - 0 
        KErrGsmCCIncomingCallsBarredInCug -1 TCCPErrorNumberBarred
        KErrGsmCCBearerCapabilityNotAuthorised - 0 
        KErrGsmCCServiceNotAvailable - 0 
        KErrGsmCCBearerServiceNotImplemented - 0 
        KErrGsmCCChannelTypeNotImplemented - 0 
        KErrGsmCCAcmGreaterThanAcmMax - 0 
        KErrGsmCCRequestedFacilityNotImplemented - 1 TCCPErrorServiceSettingError
        KErrGsmCCOnlyRestrictedDigitalInformationBCAvailable - 0
        KErrGsmCCServiceNotImplemented - 0
        KErrGsmCCInvalidCallReferenceValue - 0 TCCPErrorInvalidCallReferenceValue
        KErrGsmCCChannelDoesNotExist - 0
        KErrGsmCCSuspendedCallExistsButCallIdentityDoesNotWork -0 
        KErrGsmCCCallIdentityInUse - 0 
        KErrGsmCCNoCallSuspended - 0
        KErrGsmCCRequestedCallIdentityAlreadyCleared -0 
        KErrGsmCCUserNotInCug -0 
        KErrGsmCCIncompatibleDestination - 2  TCCPErrorConnectionError TCCPErrorBadRequest
        KErrGsmCCInvalidTransitNetworkSelection - 1 TCCPErrorConnectionErrorRedial
        KErrGsmCCIncompatibleSegmentedMessage - 0
        KErrGsmCCSemanticallyIncorrectMessage - 1 TCCPErrorConnectionErrorRedial
        KErrGsmCCInvalidMandatoryInformation - 0
        KErrGsmCCNonExistentMessageType - 0 
        KErrGsmCCIncompatibleMessageInProtocolState - 1 TCCPErrorConnectionErrorRedial
        KErrGsmCCNonExistentInformationElement - 0
        KErrGsmCCConditionalIEError -1 TCCPErrorConnectionError
        KErrGsmCCIncompatibleMessageInCallState - 0
        KErrGsmCCRecoveryOnTimerExpiry - 0
        KErrGsmCCUnspecifiedProtocolError - 1 TCCPErrorConnectionError
        KErrGsmCCUnspecifiedInterworkingError -0
        inBandTone = ESpecialInformation;
        */        
        ECCPToneSpecialInformation,    

        /**
        */        
        ECCPReorder,
        
        /**
        */        
        ECCPRemoteAlerting,
        
        /**
        */        
        ECCPDataCallTone,
        
        /**
        */        
        ECCPNoSoundSequence,
        
        /**
        */        
        ECCPBeepSequence,
        
        /**
        */        
        ECCPCallWaiting
        };


    /** 
    Generally defined common errors for plug-ins to notify errors to phone engine.
    */
    enum TCCPError
        {
        /**
        General: KErrNone
        */
        ECCPErrorNone = 0,

        /**
        General: KErrNotFound             
        */
        ECCPErrorNotFound = -1,

        /**
        General: KErrGeneral 
        */
        ECCPErrorGeneral = -2,

        /**
        General: KErrCancel
        */
        ECCPErrorCancel = -3,

        /** 
        General: KErrNoMemory 
        */
        ECCPErrorNoMemory = -4,

        /** 
        General: KErrNotSupported
        */
        ECCPErrorNotSupported = -5,

        /**
        General: : KErrArgument
        */
        ECCPErrorArgument = -6,

        /**
        General: KErrBadHandle
        */
        ECCPErrorBadHandle = -8,

        /**
        General: KErrOverflow
        */
        ECCPErrorOverflow = -9,

        /**
        General: KErrAlreadyExists
        */
        ECCPErrorAlreadyExists = -11,

        /**
        General: KErrDied
        */
        ECCPErrorDied = -13,

        /**
        General: KErrInUse        
        */
        ECCPErrorAlreadyInUse = -14,

        /**
        General: KErrServerTerminated
        */
        ECCPErrorServerTerminated = -15,

        /**
        General: KErrServerBusy
        */
        ECCPErrorServerBusy = -16,

        /**
        General: KErrNotReady
        */
        ECCPErrorNotReady = -18,

        /**
        General: KErrUnknown
        */
        ECCPErrorUnknown = -19,

        /**
        General: KErrLocked
        */
        ECCPErrorLocked = -22,

        /**
        General: KErrAbort
        */
        ECCPErrorAbort = -39,

        /**
        General: KErrTooBig
        */
        ECCPErrorTooBig = -40,

        /**
        General: KErrPermissionDenied
        */
        ECCPErrorPermissionDenied = -46,

        /** 
        KErrGsmCallServerFail        
        KErrGsm0707NotFound        
        */
        ECCPErrorRejected = -101,

        /** 
        KErrGsmCCUnassignedNumber 
        SIP: 305 (Use Proxy)
        SIP: 380 (Alternative Service)
        SIP: 3xx

        */
        ECCPErrorNotReached = -102,

        /** KErrGsmCCNumberChanged    
        SIP: 301 (Moved Permanently)  
        SIP: 410 (Gone)
        SIP: 604 (Does Not Exist Anywhere)
        */
        ECCPErrorMovedPermanently  = -103,

        /**     
        KErrGsmCCInvalidNumberFormat    
        SIP: 414 (Request-URI Too Large)
        SIP: 416 (Unsupported URI Scheme)
        SIP: 484 (Address Incomplete)
        SIP: 485 (Ambiguous)
        */
        ECCPErrorInvalidURI = -104,

        /** 
        KErrGsmCCNetworkOutOfOrder 
        SIP: 500 (Internal Server Error) 
        SIP: 501 (Not Implemented) 
        SIP: 502 (Bad Gateway) 
        SIP: 503 (Service Unavailable) 
        SIP: 504 (Server Time-out) 
        SIP: 505 (Version not supported) 
        SIP: 513 (Message Too Large) 
        SIP: 580 (Precondition Failure) 
        SIP: 482 (Loop Detected) 
        SIP: 483 (Too Many Hops) 
        SIP: 5xx 
        */
        ECCPErrorNetworkOutOfOrder = -105,

        /** 
        KErrGsmCCUserBusy    
        SIP: 486 (Busy Here)
        SIP: 600 (Busy Everywhere)
        SIP: 603 (Decline)
        */
        ECCPErrorBusy = -106,

        /** 
        SIP: 404 (Not Found)
        SIP: 480 (Temporarily Unavailable)
        SIP: 487 (Request Terminated) 
        */
        ECCPErrorNotResponding = -107,

        /** 
        KErrGsmCCTemporaryFailure    
        KErrGsmCCSwitchingEquipmentCongestion    
        KErrGsmCCRequestedChannelNotAvailable        
        */
        ECCPErrorNetworkBusy = -108,

        /** 
        SIP: 408 (Request Timeout)
        */
        ECCPErrorConnectionErrorRedial = -109,

        /**
        KErrArgument    
        */    
        ECCPErrorInvalidPhoneNumber = -110,

        /**
        KErrGsmNoService        
        SIP: 401 (Unauthorized)
        SIP: 402 (Payment Required)
        SIP: 403 (Forbidden)
        SIP: 407 (Proxy Authentication Required)  
        SIP: 423 (Interval Too Brief)
        SIP: 489 (Bad Event)
        SIP: 494 (Security Agreement Required)

        */
        ECCPErrorNoService = -111,

        /**
        KErrGsmNoNumber        
        */
        ECCPErrorInvalidPhoneNumberCancelRedial = -112,

        /**
        KErrGsmReleaseByUser        
        */
        ECCPErrorReleaseByUserForCancelRedial = -113,

        /**
        KErrGsmCCRequestedFacilityNotImplemented        
        */
        ECCPErrorServiceSettingError = -114,

        /**
        KErrGsm0707SimNotInserted
        KErrGsm0707SIMPuk1Required
        KErrGsm0707SimPin1Required
        KErrGsm0707SimPin2Required
        KErrGsm0707SimFailure
        KErrGsm0707PhoneToSimLockRequired
        KErrGsm0707SimWrong        
        */
        ECCPErrorAuthenticationFailed = -115,

        /** 
        KErrGsmOfflineOpNotAllowed
        */
        ECCPErrorNotAllowedInOfflineMode = -116,

        /**
        KErrMMEtelCallForbidden
        KErrGsmCallInProgress
        KErrGsmNumberBarred
        KErrGsmNotAllowed
        KErrEtelCallNotActive
        KErrEtelCallAlreadyActive
        */
        ECCPErrorNotAllowed = -117,

        /**
        KErrTimedOut        
        */
        ECCPErrorTimedOut = -118,

        /**
        KErrGsmInvalidFdn        
        */
        ECCPErrorInvalidFDN = -119,

        /**
        KErrGsmCCChannelUnacceptable        
        */
        ECCPErrorUnacceptableChannel = -120,

        /**
        KErrGsmCCAccessInformationDiscarded        
        */
        ECCPErrorAccessInformationDiscarded = -121,

        /**
        KErrGsmCCQualityOfServiceNotAvailable        
        */
        ECCPErrorQualityOfServiceNotAvailable = -122,

        /**
        KErrGsmCCInvalidCallReferenceValue
        */
        ECCPErrorInvalidCallReferenceValue = -123,

        /**
        KErrGsmCCSemanticallyIncorrectMessage
        KErrGsmCCConditionalIEError        
        KErrGsmCCUnspecifiedProtocolError    
        */
        ECCPErrorConnectionError = -124,

        /**
        KErrGsmCCIncompatibleDestination    
        */
        ECCPErrorBadRequest = -125,

        /**
        KErrGsmCCOperatorDeterminedBarring
        */
        ECCPErrorNumberBarred = -126,

        /**
        KErrGsmCCNormalUnspecified    
        */
        ECCPErrorNoAnswerForVideo = -127,
        
        /**
        KErrPhoneEngineNoWcdmaNetwork
        */
        ECCPErrorVideoCallNotSupportedByNetwork = -128,

        /**
        KErrPhoneEngineVideoCallNotAllowedDuringRestore
        */
        ECCPErrorVideoCallNotAllowedDuringRestore = -129,

        /**
        KErrPhoneEngineVideoCallSetupFailed
        */
        ECCPErrorVideoCallSetupFailed = -130,
        
        /**
        KErrDiagnosticInfoBarredWithCUG
        */
        ECCPErrorCUGOutgoingCallsBarred = -131,

        /**
        KErrDiagnosticInfoBarredNoCUG
        */
        ECCPErrorCUGNotSelected = -132,

        /**
        KErrDiagnosticInfoBarredUnknownCUG
        */
        ECCPErrorCUGIndexUnknown = -133,

        /**
        KErrDiagnosticInfoBarredIncompatibleCUG
        */
        ECCPErrorCUGIndexIncompatible = -134,

        /**
        KErrDiagnosticInfoBarredFailureCUG
        */
        ECCPErrorCUGCallsFailure = -135,

        /**
        KErrDiagnosticInfoBarredClirNotSubscribed
        */
        ECCPErrorCLIRNotSubscribed = -136,

        /**
        KErrDiagnosticInfoBarredCCBSPossible
        */
        ECCPErrorCCBSPossible = -137,

        /**    
        KErrDiagnosticInfoBarredCCBSNotPossible
        */
        ECCPErrorCCBSNotPossible = -138,

        /**    
        Secure call failed.
        */
        ECCPSecureCallFailed = -139,  

        /**    
        Emergency call failed.
        */
        ECCPEmergencyFailed = -140,

        /**    
        Transfer failed.
        */
        ECCPTransferFailed = -141,

        /**    
        Local hold failed.
        */
        ECCPLocalHoldFail = -142,

        /**    
        Local resume failed.
        */
        ECCPLocalResumeFail = -143,

        /** 
        TODO: old mapping, When none of other errors map and received a 3XX response. 
        */
        ECCPRedirection = -144,

        /** 
        TODO: old mapping,When none of other errors map and received a 4XX response. 
        */
        ECCPRequestFailure = -145, 

        /** 
        TODO: old mapping,When none of other errors map and received a 5XX response. 

        */
        ECCPServerFailure = -146, 

        /** 
        TODO: old mapping,When none of other errors map and received a 6XX response. 
        */
        ECCPGlobalFailure = -147,
        
        /** Rtp detection media error. */
        ECCPErrorMedia = -148,

        /** User not in Gug. Used also in place of ECCPErrorCCUserNotInCug */
        ECCPErrorUserNotInCug= -149,

        /**
        SIP: 400 (Bad Request) Unable to call due to recipient’s restrictions 
        SIP: 405 (Method Not Allowed) Unable to call due to recipient’s restrictions 
        SIP: 406 (Not Acceptable) Unable to call due to recipient’s restrictions 
        SIP: 415 (Unsupported Media Type) In Unable to call due to recipient’s restrictions 
        SIP: 413 (Request Entity Too Large) Unable to call due to recipient’s restrictions 
        SIP: 420 (Bad Extension) Unable to call due to recipient’s restrictions 
        SIP: 421 (Extension Required) In Unable to call due to recipient’s restrictions 
        SIP: 481 (Call/Transaction Does Not Exist) Unable to call due to recipient’s restrictions 

        SIP: 488 (Not Acceptable Here) Unable to call due to recipient’s restrictions 

        SIP: 4xx Unable to call due to recipient’s restrictions 
        
        */
        ECCPErrorNotAcceptable = -150,

        /** Access denied error */
        ECCPErrorAccessDenied = -151,


        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCNoRouteToDestination = -152,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCDestinationOutOfOrder  = -153,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCResourceNotAvailable = -154,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCInvalidTransitNetworkSelection = -155,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCIncompatibleDestination = -156,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCIncompatibleMessageInCallState = -157,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCIncompatibleMessageInProtocolState = -158,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCNormalCallClearing = -159,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCUserAlertingNoAnswer = -160,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCUserNotResponding = -161,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCCallRejected = -162,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCPreemption = -163,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCFacilityRejected = -164,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCResponseToStatusEnquiry = -165,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCInvalidMandatoryInformation = -166,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCNonExistentMessageType = -167,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCNonExistentInformationElement = -168,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCNoChannelAvailable = -169,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCRequestedFacilityNotSubscribed = -170,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCIncomingCallsBarredInCug = -171,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCBearerCapabilityNotAuthorised = -172,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCBearerCapabilityNotCurrentlyAvailable = -173,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCServiceNotAvailable = -174,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCBearerServiceNotImplemented = -175,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCOnlyRestrictedDigitalInformationBCAvailable = -176,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCServiceNotImplemented = -177,

        /** 
        Change ECCPError to KErrGsm, for original source error. (3GPP GSM 04.08) 
        */
        ECCPErrorCCUnspecifiedInterworkingError = -178,
        
        /** 
        */
        ECCPErrorCCRecoveryOnTimerExpiry = -179,
                
        /** 
         The SAT call control error code can be received following Call Control.  
         It is returned when the (U)SIM (or MO SM Control by (U)SIM) either bars 
         or modifies the original action. (etelsat.h)
         */
        ECCPErrorSatControl = -180
        };

#endif // CCPDEFS_H

// End of File


