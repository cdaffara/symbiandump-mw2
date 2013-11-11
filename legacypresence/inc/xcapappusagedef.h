/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XcapAppUsage enumerations
*
*/




#ifndef __XCAPAPPUSAGEDEF__
#define __XCAPAPPUSAGEDEF__

// Supported data types    
enum TDataType
    {
    EDataTypeUndefined = 0,
    EDataTypeString,
    EDataTypeNonNegativeInteger,
    EDataTypeAnyURI,
    EDataTypeDateTime,
    EDataTypeBoolean,
    EDataTypeToken,
    EDataTypeEmpty
    };
    
// AUIDs
_LIT8( KXdmResourceListsUsageAUID,         "/resource-lists/" );
_LIT8( KXdmCapabilityUsageAUID,            "/xcap-caps/" );
_LIT8( KXdmDirectoryUsageAUID,             "/org.openmobilealliance.xcap-directory/" );
_LIT8( KXdmIetfCommonPolicyUsageAUID,      "/policy-capabilities/" );
_LIT8( KXdmOmaCommonPolicyUsageAUID,       "/org.openmobilealliance.policy-capabilities/" );
_LIT8( KXdmPocUserAccessUsageAUID,         "/org.openmobilealliance.poc-rules/" );
_LIT8( KXdmIetfPresRulesUsageAUID,         "/pres-rules/" );
_LIT8( KXdmRlsServicesUsageAUID,           "/rls-services/" );
_LIT8( KXdmSharedXDMUsageAUID,             "/resource-lists/" );
_LIT8( KXdmOmaPresRulesUsageAUID,          "/org.openmobilealliance.pres-rules/" );
_LIT8( KXdmPocGroupUsageAUID,              "/org.openmobilealliance.poc-groups/" );
// add new AUID definitions here
//_LIT8( KXdmTestAppUsageAUID,               "/org.foo.bar/" );

// ContentTypes
_LIT8( KXdmResourceListsUsageContType,     "application/resource-lists+xml" );
_LIT8( KXdmCapabilityUsageContType,        "application/xcap-caps+xml" );
_LIT8( KXdmDirectoryUsageContType,         "application/oma-directory+xml" );
_LIT8( KXdmIetfCommonPolicyUsageContType,  "application/policy-caps+xml" );
_LIT8( KXdmOmaCommonPolicyUsageContType,   "application/policy-caps+xml" );
_LIT8( KXdmSharedXDMUsageContType,         "application/resource-lists+xml" );
_LIT8( KXdmIetfPresRulesUsageContType,     "application/auth-policy+xml" );
_LIT8( KXdmPocUserAccessUsageContType,     "application/auth-policy+xml" );
_LIT8( KXdmOmaPresRulesUsageContType,      "application/auth-policy+xml" );
_LIT8( KXdmRlsServicesUsageContType,       "application/rls-services+xml" );
_LIT8( KXdmPocGroupUsageContType,          "application/vnd.oma.poc.groups+xml" );
// add new content types here
//_LIT8( KXdmTestAppUsageContType,           "application/test-type+xml" );


// Namespaces
_LIT8( KXdmResourceListsNamespace,         "urn:ietf:params:xml:ns:resource-lists" );
_LIT8( KXdmCapabilityNamespace,            "urn:ietf:params:xml:ns:xcap-caps" );
_LIT8( KXdmDirectoryNamespace,             "urn:oma:params:xml:ns:xcap-directory" );
_LIT8( KXdmIetfCommonPolicyNamespace,      "urn:ietf:params:xml:ns:common-policy" );
_LIT8( KXdmOmaCommonPolicyNamespace,       "urn:oma:xml:xdm:common-policy" );
_LIT8( KXdmOmaPresRulesNamespace,          "urn:oma:xml:prs:pres-rules" );
_LIT8( KXdmIetfPresRulesNamespace,         "urn:ietf:params:xml:ns:pres-rules" );
_LIT8( KXdmRlsServicesNamespace,           "urn:ietf:params:xml:ns:rls-services" );
_LIT8( KXdmSharedXDMUriUsageNamespace,     "urn:oma:params:xml:ns:resource-list:oma-uriusage" );
_LIT8( KXdmPocUserAccessNamespace,         "urn:oma:xml:poc:poc-rules" );
_LIT8( KXdmPocGroupNamespace,              "urn:oma:xml:poc:list-service" );
// add new namespace definitions here

// Namespace prefixes
_LIT8( KXdmSharedXDMUriUsageNsPrefix,       "ou" );
_LIT8( KXdmResourceListsNsPrefix,           "rl" );
_LIT8( KXdmIetfCommonPolicyNsPrefix,        "cr" );
_LIT8( KXdmOmaCommonPolicyNsPrefix,         "ocp" );
_LIT8( KXdmIetfPresRulesNsPrefix,           "pr" );
_LIT8( KXdmTestAppUsageDefaultNamespace,    "ta" );
_LIT8( KXdmPocUserAccessNsPrefix,           "poc" );
_LIT8( KXdmOmaPresRulesNsPrefix,            "opr" );
_LIT8( KXdmRlsServicesNsPrefix,             "rls" );
_LIT8( KXdmPocGroupNsPrefix,                "ls" );
// add new namespace prefix definitions here

// Common literals
_LIT( KXdmAuid,                             "auid" );
_LIT( KXdmUri,                              "uri" );
_LIT( KXdmEtag,                             "etag" );
_LIT( KXdmName,                             "name" );
_LIT( KXdmRef,                              "ref" );
_LIT( KXdmUse,                              "use" );
_LIT( KXdmRuleset,                          "ruleset" );
_LIT( KXdmEntry,                            "entry" );
_LIT( KXdmEntity,                           "entity" );
_LIT( KXdmId,                               "id" );
_LIT( KXdmAnyIdentity,                      "any-identity" );
_LIT( KXdmIdentity,                         "identity" );
_LIT( KXdmDomain,                           "domain" );
_LIT( KXdmAnc,                              "anc" );
_LIT( KXdmDisplayName,                      "display-name" );
_LIT( KXdmAnchor,                           "anchor" );
_LIT( KXdmExternal,                         "external" );
_LIT(  KXdmList,                            "list" );
_LIT(  KXdmServiceId,                       "service-id" );
_LIT8( KXdmZero,                            "0" );
_LIT8( KXdmOneNbr,                          "1" );
_LIT8( KXdmTrue,                            "true" );
_LIT8( KXdmFalse,                           "false");
_LIT8( KXdmFull,                            "full" );
_LIT8( KXdmAllow,                           "allow" );
_LIT8( KXdmBlock,                           "block" );

// Literals for capability
_LIT( KXdmExtensions,                       "extensions" );
_LIT( KXdmExtension,                        "extension" );
_LIT( KXdmAuids,                            "auids" );
_LIT( KXdmNamespaces,                       "namespaces" );
_LIT( KXdmNamespace,                        "namespace");

// Literals for directory
_LIT( KXdmXcapDirectory,                    "xcap-directory" );
_LIT( KXdmFolder,                           "folder" );
_LIT( KXdmErrorCode,                        "error-code" );
_LIT( KXdmLastModified,                     "last-modified" );
_LIT( KXdmSize,                             "size" );

// Literals for presence
_LIT( KXdmServiceUriScheme,                "service-uri-scheme" );
_LIT( KXdmClass,                           "class" );
_LIT( KXdmOccurenceId,                     "occurence-id" );
_LIT( KXdmServiceUri,                      "service-uri" );
_LIT( KXdmAllServices,                     "all-services" );
_LIT( KXdmProvideServices,                 "provide-services" );
// pres-rules-03
_LIT( KXdmDeviceId,                        "device-id" );
// pres-rules-05
_LIT( KXdmDeviceID,                        "deviceID" );
_LIT( KXdmAllDevices,                      "all-devices" );
_LIT( KXdmProvideDevices,                  "provide-devices" );
_LIT( KXdmAllPersons,                      "all-persons" );
_LIT( KXdmProvidePersons,                  "provide-persons" );
_LIT( KXdmProvideActivities,               "provide-activities" );
_LIT( KXdmProvideClass,                    "provide-class" );
_LIT( KXdmProvideDeviceId,                 "provide-device-id" );
_LIT( KXdmProvideMood,                     "provide-mood" );
_LIT( KXdmProvidePlaceIs,                  "provide-place-is" );
_LIT( KXdmProvidePlaceType,                "provide-place-type" );
_LIT( KXdmProvidePrivacy,                  "provide-privacy" );
_LIT( KXdmProvideRelationship,             "provide-relationship" );
_LIT( KXdmProvideStatusIcon,               "provide-status-icon" );
_LIT( KXdmProvideSphere,                   "provide-sphere" );
_LIT( KXdmProvideTimeOffset,               "provide-time-offset" );
_LIT( KXdmProvideUserInput,                "provide-user-input" );
_LIT( KXdmProvideNote,                     "provide-note" );
_LIT( KXdmProvideUnknownAttribute,         "provide-unknown-attribute" );
_LIT( KXdmProvideAllAttributes,            "provide-all-attributes" );
_LIT( KXdmSubHandling,                     "sub-handling" );
_LIT( KXdmProvideWillingness,              "provide-willingness" );
_LIT( KXdmProvideNetworkAvailability,      "provide-network-availability" );
_LIT( KXdmProvideSessionParticipation,     "provide-session-participation" );
_LIT( KXdmProvideGeopriv,                  "provide-geopriv" );
_LIT( KXdmProvideRegistrationState,        "provide-registration-state" );
_LIT( KXdmProvideBarringState,             "provide-barring-state" );
_LIT8( KXdmBare,                           "bare" );
_LIT8( KXdmThresholds,                     "thresholds" );
_LIT8( KXdmRandomize,                      "randomize" );
_LIT8( KXdmObfuscate,                      "obfuscate" );
_LIT8( KXdmConfirm,                        "confirm" );
_LIT8( KXdmPoliteBlock,                    "polite-block" );

// Literals for commonpolicy
_LIT( KXdmOtherIdentity,                    "other-identity" );
_LIT( KXdmExternalList,                     "external-list" );
_LIT( KXdmAnonymousRequest,                 "anonymous-request" );
_LIT( KXdmRule,                             "rule" );
_LIT( KXdmExceptDomain,                     "except-domain" );
_LIT( KXdmExcept,                           "except" );
_LIT( KXdmFrom,                             "from" );
_LIT( KXdmTo,                               "to" );
_LIT( KXdmValue,                            "value" );
_LIT( KXdmSphere,                           "sphere" );
_LIT( KXdmValidity,                         "validity" );
_LIT( KXdmConditions,                       "conditions" );
_LIT( KXdmActions,                          "actions" );
_LIT( KXdmTransformations,                  "transformations" );
_LIT( KXdmMany,                             "many" );
_LIT( KXdmUntil,                            "until" );
_LIT( KXdmOne,                              "one" );
_LIT( KXdmScheme,                           "scheme" );

// Literals for poc
_LIT(  KXdmGroup,                           "group");
_LIT(  KXdmListService,                     "list-service" );
_LIT(  KXdmInviteMembers,                   "invite-members" );
_LIT(  KXdmMaxParticipantCount,             "max-participant-count" );
_LIT(  KXdmIsListMember,                    "is-list-member" );
_LIT(  KXdmAllowConfState,                  "allow-conference-state" );
_LIT(  KXdmAllowInvUsersDyn,                "allow-invite-users-dynamically" );
_LIT(  KXdmJoinHandling,                    "join-handling" );
_LIT(  KXdmAllowInitiateConf,               "allow-initiate-conference" );
_LIT(  KXdmAllowAnonymity,                  "allow-anonymity");
_LIT(  KXdmIsKeyParticipant,                "list-service" );
_LIT(  KXdmAllowInvite,                     "allow-invite" );
_LIT8( KXdmReject,                          "reject" );
_LIT8( KXdmAccept,                          "accept" );
_LIT8( KXdmPass,                            "pass" );

// Literals for resource list
_LIT(  KXdmEntryRef,                        "entry-ref");
_LIT(  KXdmResourceLists,                   "resource-lists");

// Literals for shared
_LIT(  KXdmUriUsages,                       "uriusages");
_LIT(  KXdmUriUsage,                        "uriusage");

// Literals for rls
_LIT(  KXdmRlsServices,                     "rls-services" );
_LIT(  KXdmService,                         "service");
_LIT(  KXdmResourceList,                    "resource-list" );
_LIT(  KXdmPackages,                        "packages" );
_LIT(  KXdmPackage,                         "package" );
_LIT8( KXdmPresence,                        "presence" );

#endif      //__XCAPAPPUSAGEDEF__
            
// End of File
