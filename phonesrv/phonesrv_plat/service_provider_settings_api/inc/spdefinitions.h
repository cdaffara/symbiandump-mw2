/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  service provider settings definitions
*
*/


#ifndef SPDEFINITIONS_H
#define SPDEFINITIONS_H

/**  Maximum length of descriptor data */
const TInt KSPMaxDesLength = 512;

/** Service attribute mask */
enum TPropertyServiceAttributes
    {
    ESupportsCSVoiceCall            = 0x00000001, // Supports CS voice call
    ESupportsCSVideoCall            = 0x00000002, // Supports CS video call
    ESupportsCSData                 = 0x00000004, // Supports CS data transfer
    ESupportsFax                    = 0x00000008, // Service supports fax
    ESupportsInternetCall           = 0x00000010, // Service supports internet call
    ESupportsEmergencyCall          = 0x00000020, // service supports emergency call
    ESupportsMSISDNAddressing       = 0x00000040, // Service supports MSISDN addressing
    ESupportsAlphanumericAddressing = 0x00000080, // Service supports alphanumeric addressing
    EIsVisibleInCallMenu            = 0x00000100, // Service is visible in call menu
    EBootstrapCallProvider          = 0x00000200, // Boostrap call provider
    ESupportVoIPSS                  = 0x00000400, // Supports VoIP SS
    ESimplifiedConferenceCallBubble = 0x00000800  // Simplified conference call bubble
    };
    
/** Supported feature check */  
enum TSPServiceFeature
    {
    ESupportInternetCallFeature,    //ESupportsInternetCall + ESupportsAlphanumericAddressing + EIsVisibleInCallMenu 
    ESupportCallOutFeature,         //ESupportsInternetCall + ESupportsMSISDNAddressing + EIsVisibleInCallMenu
    ESupportVoIPSSFeature,          //ESupportVoIPSS
    ESupportVoIPFeature             //FeatureManager::FeatureSupported()+dynamic voip flag check+with subservice voip
    };

/**  Service provider ID not defined */
const TInt KSPNoId = 0;
/**  Types of service provider settings items in settings table
     - for internal use */
enum TSPItemType
    {
    EItemTypeNotDefined = 1,
    EItemTypeDeleted,
    EItemTypeEntry,
    EItemTypeProperty,

    // Specifies certain subservice group
    EItemTypeVoIPSubProperty,
    EItemTypePresenceSubProperty,
    EItemTypeIMSubProperty,
    EItemTypeVMBXSubProperty,
    };

/**  List of predefined properties */
enum TServicePropertyName 
    {
    EPropertyUnknown                        = 0x00000001, // Unknown property
    
    EServiceName                            = 0x00002000, // Service name. Set by RCSE / VoIP adapter
    
    // TInt
    // properties
    EPropertyServiceAttributeMask           = 0x00003000, // Service attributes, see TPropertyServiceAttributes. Set by RCSE
    EPropertyCTIPluginId                    = 0x00004000, // Phone Engine CTI plugin UID
    EPropertyCLIPluginId                    = 0x00005000, // Phone application CLI plugin UID. Set by RCSE
    EPropertyCallLoggingPluginId            = 0x00006000, // Phone application call logging plugin UID. Set by RCSE
    EPropertyCallStateIndicatorPluginId     = 0x00007000, // Phone application call state indicator plugin UID. Set by RCSE
    EPropertyCallMenuHandlerPluginId        = 0x00008000, // Phone application menu handler plugin UID. Set by RCSE
    EPropertyCallProviderPluginId           = 0x00009000, // CCE call provider plugin UID. Set by RCSE
    EPropertyPCSPluginId                    = 0x0000A000, // Presence FW PSC plugin UID. Set by RCSE
    EPropertyVoIPSubServicePluginId         = 0x0000B000, // CCH VoIP subservice connectivity plugin UID. Set by RCSE
    EPropertyPresenceSubServicePluginId     = 0x0000C000, // CCH Presence subservice connectivity plugin UID. Set by VoIP adapter
    EPropertyIMSubServicePluginId           = 0x0000D000, // CCH Instant Messaging subservice connectivity plugin UID
    EPropertyVMBXSubServicePluginId         = 0x0000E000, // CCH VMBX subservice connectivity plugin UID. Set by RCSE
    EPropertyServiceSetupPluginId           = 0x0000F000, // Service setup plugin UID. Set by RCSE
    EPropertyContactViewId                  = 0x00010000, // Phonebook2 contact view ID. Set by Phonebook extension
    EPropertyBrandVersion                   = 0x00011000, // Brand version
    EPropertyBrandLanguage                  = 0x00012000, // Brand language
    EPropertyLogCntLinkMaxLength            = 0x00013000, // Maximum length of a serialised contact link. Set by RCSE
    EPropertyContactViewPluginId            = 0x00014000, // Contact view plugin UID. Set by RCSE
    EPropertySendFwImPluginId               = 0x00015000, // SendUi service provider UID
    EPropertyRingtoneTimeout                = 0x00016000, // Ringtone timeout

    // Subproperties
    // VoIP subproperties
    ESubPropertyVoIPSettingsId              = 0x00017000, // VoIP settings ID. Set by RCSE
    ESubPropertyVoIPPreferredSNAPId         = 0x00018000, // Preferred SNAP ID. Set by VoIP adapter
    ESubPropertyVoIPPreferredIAPId          = 0x00019000, // Preferred IAP ID. Set by VoIP adapter
    ESubPropertyVoIPTemporaryIAPId          = 0x0001A000, // Temporary IAP ID. Set by Phonebook extension

    // Presence subproperties
    ESubPropertyPresenceSettingsId          = 0x0001B000, // Presence settings ID. Set by VoIP adapter
    ESubPropertyPresencePreferredSNAPId     = 0x0001C000, // Preferred SNAP ID
    ESubPropertyPresencePreferredIAPId      = 0x0001D000, // Preferred IAP ID
    ESubPropertyPresencePresentityIDFieldType = 0x0001E000, // Presentity ID field type. Set by RCSE
    ESubPropertyPresenceLaunchMethod        = 0x0001F000, // Presence launching method
    ESubPropertyPresenceLaunchUid           = 0x00020000, // Presence plugin or application UID

    // IM subproperties
    ESubPropertyIMSettingsId                = 0x00021000, // IM settings ID
    ESubPropertyIMPreferredSNAPId           = 0x00022000, // Preferred SNAP ID
    ESubPropertyIMPreferredIAPId            = 0x00023000, // Preferred IAP ID
    ESubPropertyIMLaunchMethod              = 0x00024000, // IM launching method
    ESubPropertyIMLaunchUid                 = 0x00025000, // IM plugin or application UID

    // VMBX subproperties
    ESubPropertyVMBXSettingsId              = 0x00026000, // VMBX settings ID. Set by VoIP adapter
    ESubPropertyVMBXPreferredSNAPId         = 0x00027000, // Preferred SNAP ID
    ESubPropertyVMBXPreferredIAPId          = 0x00028000, // Preferred IAP ID. Set by VoIP adapter
    ESubPropertyVMBXMWISubscribeInterval    = 0x00029000, // Message waiting indicator, Re-subscribe interval. Set by VoIP adapter
    ESubPropertyVMBXLaunchMethod            = 0x0002A000, // VMBX launching method
    ESubPropertyVMBXLaunchUid               = 0x0002B000, // VMBX plugin or application UID


    // DES
    //--------------------------------------------------------------------
    // Properties
    EPropertyBrandId                        = 0x0002C000, // Brand ID. Links the service to brand information
                                                          // stored in the branding server. Set by RCSE
    EPropertyContactStoreId                 = 0x0002D000, // Virtual phonebook contact store ID. Set by RCSE
    EPropertyServiceBookmarkUri             = 0x0002E000, // Service provider bookmark URI. Set by VoIP adapter
    EPropertyIncomingEmailserver            = 0x0002F000, // Service incoming e-mail server
                

    // Subproperties
    // VoIP subproperties
    ESubPropertyVoIPRelNumber               = 0x00030000, // VoIP release number
    ESubPropertyVoIPBrandDataUri            = 0x00031000, // VoIP branding data URI. Set by VoIP adapter
    ESubPropertyVoIPBrandIconUri            = 0x00032000, // VoIP branding icon URI
    ESubPropertyVoIPAddrScheme              = 0x00033000, // VoIP addressing scheme
    
    // Presence subproperties
    ESubPropertyPresenceAddrScheme          = 0x00034000, // Presence addressing scheme
    
    // IM subproperties
    ESubPropertyIMAddrScheme                = 0x00035000, // IM addressing scheme
    
    // VMBX subproperties
    ESubPropertyVMBXListenAddress           = 0x00036000, // VMBX listen address. Set by VoIP adapter
    ESubPropertyVMBXMWIAddress              = 0x00037000, // VMBX message waiting indicator address. Set by VoIP adapter
    ESubPropertyVMBXBrandIconUri            = 0x00038000, // Brand icon URI
    ESubPropertyVMBXAddrScheme              = 0x00039000, // VMBX addressing scheme


    // OnOff
    //--------------------------------------------------------------------
    // VoIP subproperties
    ESubPropertyVoIPEnabled                 = 0x0003A000, // Enable/disable VoIP. Set by VoIP adapter
    
    // Presence subproperties
    ESubPropertyPresenceEnabled             = 0x0003B000, // Enable/disable presence. Set by VoIP adapter
    ESubPropertyPresenceRequestPreference   = 0x0003C000, // Auto accept buddy request. Set by VoIP adapter
    
    // IM subproperties
    ESubPropertyIMEnabled                   = 0x0003D000, // Enable/disable IM
    
    // VMBX subproperties
    ESubPropertyVMBXListenRegister          = 0x0003E000, // Listen without or with registration
    ESubPropertyVMBXEnabled                 = 0x0003F000,  // Enable/disable VMBX. Set by VoIP adapter
    
    //VCC subproperties
    ESubPropertyVccName                         = 0x00040000,
    ESubPropertyVccVDI                          = 0x00041000,
    ESubPropertyVccVDN                          = 0x00042000,
    ESubPropertyVccPreferredDomain              = 0x00043000,                     
    ESubPropertyVccImmediateDomainTransfer      = 0x00044000, 
    ESubPropertyVccDtCstoPsAllowed              = 0x00045000,
    ESubPropertyVccDtPstoCsAllowed              = 0x00046000,
    ESubPropertyVccDtHeldWaitingCallsAllowed    = 0x00047000,
    ESubPropertyVccDtCSHoTriggerLevel           = 0x00048000, 
    ESubPropertyVccDtCSHoHysteresis             = 0x00049000,
    ESubPropertyVccDtCSHoHysteresisTimerLow     = 0x0004A000,
    ESubPropertyVccDtCSHoHysteresisTimerHigh    = 0x0004B000,
    ESubPropertyVccDtWLANHoTriggerLevel         = 0x0004C000,
    ESubPropertyVccDtWLANHoHysteresis           = 0x0004D000,
    ESubPropertyVccDtWLANHoHysteresisTimerLow   = 0x0004E000,
    ESubPropertyVccDtWLANHoHysteresisTimerHigh  = 0x0004F000,
    ESubPropertyVccHoNotificationToneMode       = 0x00050000,
    ESubPropertyVccDtVoipServiceId              = 0x00051000,
    ESubPropertyVccDtAllowedWhenCsOriginated    = 0x00052000,
    ESubPropertyVccAppId                        = 0x00053000,

    };


/**  On/off data type */
enum TOnOff
    {
    EOONotSet = -1,
    EOff = 0,
    EOn = 1
    };

/**  List of supported datatypes of properties */
enum TPropertyDataType
    {
    EDataTypeNotSet,  // NotSet
    EDataTypeUnknown, // Unknown
    EDataTypeInt,     // TInt
    EDataTypeDes,     // TDesC
    EDataTypeOnOff,   // TOnOff
    };
    
/** List of subservice launching method */
enum TSPSubServiceLaunchMethod
    {
    ESPFrameWorkPlugin,
    ESPLauncherPlugin,
    ESPApplication
    };


/**  Service ID */
typedef TUint TServiceId;

/**  Array of CSPProperty objects */
class CSPProperty;
typedef RPointerArray<CSPProperty> RPropertyArray;

/**  Array of service provider IDs */
typedef RArray<TServiceId> RIdArray;

/**  Array of service provider IDs */
typedef RArray<TServicePropertyName> RPropertyNameArray;

/**  central repsitory keys array */
typedef RArray<TUint32> RKeyArray;


#endif      // SPDEFINITIONS_H


