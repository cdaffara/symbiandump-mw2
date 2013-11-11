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
* Description:    RLS and Presence XDM, These are exported constants.
*
*/



#ifndef __RLSPRESXDMCONSTS_H__
#define __RLSPRESXDMCONSTS_H__

const TInt KNodeNameMaxLength = 25;
const TInt KNodeContentMaxLength = 100;

// for getting and settings complex transforms
// doesnt need for setting 'provide-all'
struct TTransDataCompo
    {
    TBuf<KNodeNameMaxLength> nodeName;
    TBuf<KNodeContentMaxLength> nodeContent;    
    };

// for getting and setting condition validity    
struct TTimePeriod
    {
    TDateTime from;
    TDateTime until;
    };
    
// The following actions are available
_LIT(   KPresBlock,              "block"); 
_LIT(   KPresConfirm,            "confirm");
_LIT(   KPresPoliteBlock,        "polite-block");
_LIT(   KPresAllow,              "allow");

// These values are only for 'provide-user-input' transform
_LIT(   KPresFalse,             "false"); 
_LIT(   KPresBare,              "bare");
_LIT(   KPresThresholds,        "thresholds");
_LIT(   KPresFull,              "full");

// Supported Presence complex transform types
_LIT(   KPresProvideServices,                           "provide-services"); 
_LIT(   KPresProvideDevices,                            "provide-devices");
_LIT(   KPresProvidePersons,                            "provide-persons");

//Supported Presence complex transform node names
_LIT(   KPresServiceId,                                 "service-id");
_LIT(   KPresClass,                                     "class");
_LIT(   KPresOccurenceID,                               "occurrence-id");
_LIT(   KPresDeviceID,                                  "device-id");
_LIT(   KPresServiceUri,                                "service-uri");
_LIT(   KPresServiceUriScheme,                          "service-uri-scheme");

// Following node names are only returned in getters, doesnt need in setters
_LIT(   KPresAllDevices,                                "all-devices");
_LIT(   KPresAllPersons,                                "all-persons");
_LIT(   KPresAllServices,                               "all-services");


//Supported Presence boolean tranform types (IETF)
_LIT(   KPresProvideActivities,                         "provide-activities");
_LIT(   KPresProvideClass,                              "provide-class");
_LIT(   KPresProvideDeviceID,                           "provide-deviceID");
_LIT(   KPresProvideMood,                               "provide-mood");
_LIT(   KPresProvidePlaceIs,                            "provide-place-is");
_LIT(   KPresProvidePlaceType,                          "provide-place-type");
_LIT(   KPresProvidePrivacy,                            "provide-privacy");
_LIT(   KPresProvideRelationship,                       "provide-relationship");
_LIT(   KPresProvideSphere,                             "provide-sphere");
_LIT(   KPresProvideStatusIcon,                         "provide-status-icon");
_LIT(   KPresProvideTimeOffset,                         "provide-time-offset");
_LIT(   KPresProvideNote,                               "provide-note");
_LIT(   KPresProvideUnknownAttribute,                   "provide-unknown-attribute");

//Supported Presence boolean tranform types (OMA Extensions)
_LIT(   KPresProvideWillingness,                        "provide-willingness");
_LIT(   KPresProvideNetworkAvailability,                "provide-network-availability");
_LIT(   KPresProvideSessionParticipation,               "provide-session-participation");
_LIT(   KPresProvideGeoPriv,                            "provide-geopriv");
_LIT(   KPresProvideRegistrationState,                  "provide-registration-state");
_LIT(   KPresProvideBarringState,                       "provide-barring-state");

//Supported Presence transform other types (IETF)
_LIT(   KPresProvideUserInput,                          "provide-user-input");
_LIT(   KPresProvideAllAttributes,                      "provide-all-attributes");

//RLS XDM Elements
_LIT(   KPresList,                                      "list");
_LIT(   KPresEntry,                                     "entry");
_LIT(   KPresExternal,                                  "external");
_LIT(   KPresEntryRef,                                  "entry-ref");
_LIT(   KPresResourceList,                              "resource-list");

#endif // __RLSPRESXDMCONSTS_H__