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
* Description:  This file defines the interface of the Network Handling
*
*/



#ifndef NWHandlingEngine_H
#define NWHandlingEngine_H

//  INCLUDES
#include <e32base.h>
#include <rmmcustomapi.h> 
#include <etelpckt.h> // for RPacketService::TDynamicCapsFlags


// CONSTANTS
// Max length of network country code.
const TInt KNWCountryCodeLength = 4; // This value is defined in multimode etel definition document.
// Max length of network identity code.
const TInt KNWIdentityLength = 8; // This value is defined in multimode etel definition document.
// Max length of network display alpha tag
const TInt KNWDisplayTagLength = 30;   // New EtelMM API defition describe this value to be 32, but etelmm.h contains value 30. 
// Max length of network short name.
const TInt KNWShortNameLength = 10; // New EtelMM API defition describe this value to be 8, but etelmm.h contains value 10. 
// Max length of network long name.
const TInt KNWLongNameLength = 20; // New EtelMM API defition describe this value to be 16, but etelmm.h contains value 20. 
// Max length of service provider name.
const TInt KNWServiceProviderNameLength = 16;
// Max length of network provider name.
const TInt KNWProviderNameLength = 25;
// Max length of MCN name.
const TInt KNWMCNNameLength = 20;
// Max length of VIAG text tag
const TInt KNWViagTextTagLength = 13;
// Max length of PLMN field
const TInt KNWPLMNFieldLength = 251;


// DATA TYPES
// MCN name
typedef TBuf<KNWMCNNameLength> TNWMCNName;
// Viag text tag
typedef TBuf<KNWViagTextTagLength> TNWViagTextTag;
// MCC in GSM and TDMA
typedef TBuf<KNWCountryCodeLength> TNWCountryCode;
// MNC in GSM and SID in TDMA
typedef TBuf<KNWIdentityLength> TNWIdentity;
// Network Alpha-tag
typedef TBuf<KNWDisplayTagLength> TNWDisplayTag;
// The short name of the network operator
typedef TBuf<KNWShortNameLength> TNWShortName;
// The long name of the network operator
typedef TBuf<KNWLongNameLength> TNWLongName;
// Service provider name
typedef TBuf<KNWServiceProviderNameLength> TNWServiceProviderName;
// The PLMN Field
typedef TBuf<KNWPLMNFieldLength> TNWPLMNField;
// Network provider name
typedef TBuf<KNWProviderNameLength> TNWProviderName;
// The Programmable Operator Name type.
typedef RMmCustomAPI::TOperatorNameInfo TNWOperatorNameInfo;
// Display of the registered PLMN. 
typedef TUint32 TNWDisplayRequirementsFlags;


enum TNWRegistrationStatus
    {
    ENWRegistrationUnknown,
    ENWNotRegisteredNoService,
    ENWNotRegisteredEmergencyOnly,
    ENWNotRegisteredSearching,
    ENWRegisteredBusy,
    ENWRegisteredOnHomeNetwork,
    ENWRegistrationDenied,
    ENWRegisteredRoaming
    };

enum TNWMode
    {
    ENWModeUnknown,
    ENWModeUnregistered,
    ENWModeGsm,
    ENWModeAmps,
    ENWModeCdma95,
    ENWModeCdma2000,
    ENWModeWcdma,
    ENWModeTdcdma
    };

enum TNWStatus
    {
    ENWStatusUnknown,
    ENWStatusAvailable,
    ENWStatusCurrent,
    ENWStatusForbidden
    };

enum TNWDisplayRequirements
    {
    ENWDisplayPLMNNotRequired = 0x00000001,
    ENWDisplayPLMNRequired = 0x00000002,
    ENWDisplaySPNRequired = 0x00000004,
    ENWDisplaySPNNotRequired = 0x00000008
    };

enum TNWViagIndicatorType //enum for viag indicator type
    {
    ENWViagIndicatorTypeNone,
    ENWViagIndicatorTypeHomeZone,
    ENWViagIndicatorTypeCityZone
    };

enum TNWMCNIndicatorType //enum for mcn indicator type
    {
    ENWMCNIndicatorTypeNone,
    ENWMCNIndicatorTypeActive
    };

// CDMA enumerations for TNWInfo
enum TNWNoServiceMode
    {
    ENWNotRegisteredOutOfRange,
    ENWNotRegisteredPowerSaveMode
    };

// CDMA enumerations for TNWInfo
enum TNWVoicePrivacyStatus
    {
    ENWVoicePrivacyStatusOff,
    ENWVoicePrivacyStatusOn,
    ENWVoicePrivacyStatusUnknown
    };

enum TNWRead
    {
    ESPNEFRead,
    ENPNEFRead,
    EProgEFRead,
    EViagEFRead,
    EFullFileChange
    };

#ifdef RD_PHONE_NG
enum TNWNetworkSelectionSetting
    {
    ENWNetworkSelectionUnknown,
    ENWNetworkSelectionAutomatic,
    ENWNetworkSelectionManual
    };    
#endif // RD_PHONE_RG
// CLASS DECLARATION

//Network info definition
struct TNWInfo
    {
    TNWRegistrationStatus iRegistrationStatus;              // Network registration status
    TNWMode iNetworkMode;                                   // Mode of the network
    TNWStatus iStatus;                                      // Status of the Network
    TNWCountryCode iCountryCode;                            // MCC in GSM and TDMA 
    TNWIdentity iNetworkId;                                 // MNC in GSM and SID in TDMA
    TNWDisplayTag iDisplayTag;                              // Network Alpha-tag
    TNWShortName iShortName;                                // The short name of the operator
    TNWLongName iLongName;                                  // The long name of the operator
    TNWViagIndicatorType iViagIndicatorType;                // Zone indicator type
    TNWViagTextTag iViagTextTag;                            // Viag text tag
    TNWMCNIndicatorType iMCNIndicatorType;                  // MCN indicator type
    TNWMCNName iMCNName;                                    // MCN name
    TNWServiceProviderName iSPName;                         // Service provider name
    TNWDisplayRequirementsFlags iServiceProviderNameDisplayReq; // Service provider name display required
    TNWProviderName iNPName;                                // Network provider name
    TNWOperatorNameInfo iOperatorNameInfo;                  // Operator name
    TNWNoServiceMode iNoServerMode;                         // 
    TNWVoicePrivacyStatus iVoicePrivacyStatus;              // Voice Privacy Status
    TNWPLMNField iPLMNField;                                // Contains a number of PLMN network names coded as TLV objects
#ifdef RD_PHONE_NG
    TNWNetworkSelectionSetting iSelectionSetting;           // Contains a network selection setting manual/automatic etc.
#endif // RD_PHONE_NG
    RPacketService::TDynamicCapsFlags iDynamicCapsFlags;    // Dynamic packet data capabilities 
                                                            // (used to hide alpha tag when CS registration is unsuccessful)
    };

// CLASS DECLARATION

/**
*  Offers message interface from network handling to client
*  
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class MNWMessageObserver
    {
    public:

        enum TNWMessages
            {
            // COMMON NETWORK MESSAGES                      0 - 99

            // Message will be sent when current network information changed
            ENWMessageNetworkInfoChange,
            // Message will be sent when network mode changed (e.g.Gsm to Wcdma )
            ENWMessageNetworkModeChange,
            // Message will be sent when network registration status changed
            ENWMessageNetworkRegistrationStatusChange,
            // Message will be sent when programmable operator name changed
            ENWMessageProgrammableOperatorInfoChange,
            // Message will be sent when network provider changed
            ENWMessageNetworkProviderNameChange,
            // Message will be sent when service provider changed
            ENWMessageServiceProviderNameChange,
            // Message will be sent when NW starts update Operator Name Info
            ENWMessageProgrammableOperatorInfoUpdating,
            // Message will be sent when NW starts update Network Service Provider Name
            ENWMessageNetworkProviderNameUpdating,
            // Message will be sent when NW starts update Service Provider Name
            ENWMessageServiceProviderNameUpdating,
            // Message will be sent when allow refresh is called.
            ENWMessageAllowRefresh,
            // Message will be sent when networkSelection setting changed
#ifdef RD_PHONE_NG
            ENWMessageNetworkSelectionSettingChange,
#endif // RD_PHONE_NG

            // GSM SPECIFIC MESSAGES                        100 - 199

            // Message will be sent when cell re-selection occured
            ENWMessageNetworkCellReselection              = 100,
            // Message will be sent when network connection failured
            ENWMessageNetworkConnectionFailure,
            // Message will be sent when MCN CBS message was reveived
            ENWMessageCurrentCellInfoMessage,
            // Message will be sent when Viag CBS message was reveived
            // and completely handled.
            ENWMessageCurrentHomeZoneMessage,
            // Message will be sent when cell info display changed
            ENWMessageNetworkIndicatorChange, 
            // Message will be sent when dynamic packet data capabilities change
            // (Used to hide alpha tag when CS registration is unsuccessful)
            ENWMessageDynamicCapsChange,

            // CDMA SPECIFIC MESSAGES                       200 - 299

            ENWMessageNetworkEmergencyMode                = 200,
            ENWMessageVoicePrivacyStatusChange,
            ENWMessageStartSystemSearchRequestCompleteOk,
            ENWMessageStartSystemSearchRequestCompleteFail,
            ENWMessageStopProtocolStackRequestCompleteOk,
            ENWMessageStopProtocolStackRequestCompleteFail

           };

        enum TNWOperation
            {
            ENWNone,
            // Fetching Service Provider Name
            ENWGetServiceProviderName,
            // Fetching Network Provider Name
            ENWGetNetworkProviderName,
            // Fetching Programmable Operator Name
            ENWGetProgrammableOperatorName,
            // Notifying network registration status change
            ENWNotifyNetworkRegistrationStatusChange
            };

        /**
        * Offers message interface to the client
        * @param aMessage 
        * This methods execute time must be short,since code 
        * starting to run from RunL.
        */
        virtual void HandleNetworkMessage( const TNWMessages aMessage ) = 0;

        /**
        * Offers error message interface to the client
        * @param aOperation operation which failed
        * @param aErrorCode returned Symbian OS error code
        * 
        */
        virtual void HandleNetworkError( const TNWOperation aOperation, TInt aErrorCode ) = 0;
    };


#endif      // NWHandlingEngine_H   
            
// End of File
