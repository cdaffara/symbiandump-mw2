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
* Description:  Domain Publish & Subscribe keys for network handling
 *
 *
*/


#ifndef NETWORKHANDLINGDOMAINPSKEYS_H
#define NETWORKHANDLINGDOMAINPSKEYS_H

const TUid KPSUidNetworkInfo = { 0x10202999 };
/**
* Used to indicate status of homezone
*
* Possible values:
* 0: None
* 1: City zone
* 2: Home zone
 */
const TUint KNWHomeZoneStatus = 0x00000001;

enum KNWZoneStatus
    {
    ENWNone,
    ENWCityZone, 
    ENWHomeZone 
    };

/**
* Used to indicate mode of network
*
* Possible values:
* 0: Unknown mode, Default value
* 1: Gsm mode
* 2: Cdma mode
* 3: Wcdma mode
*/
const TUint KNWTelephonyNetworkMode = 0x00000002;

enum KNWNetworkMode
    {
    ENWNetworkModeUnknown,
    ENWNetworkModeGsm,
    ENWNetworkModeCdma,
    ENWNetworkModeWcdma
    };
/**
* Used to indicate network registration status
*
* Possible values:
* 0: Registration status unknown
* 1: Not registered, no service
* 2: Not registered, service available for emergency calls only
* 3: Not registered, searching for network
* 4: Registered, network busy
* 5: Registered on home network
* 6: Registration denied 
* 7: Registered on roaming network
*/
const TUint KNWRegistrationStatus = 0x00000003;

enum TNWNetworkRegistrationStatus
    {
    ENWStatusRegistrationUnknown,
    ENWStatusNotRegisteredNoService,
    ENWStatusNotRegisteredEmergencyOnly,
    ENWStatusNotRegisteredSearching,
    ENWStatusRegisteredBusy,
    ENWStatusRegisteredOnHomeNetwork,
    ENWStatusRegistrationDenied,
    ENWStatusRegisteredRoaming
    };    

#endif //NETWORKHANDLINGDOMAINPSKEYS_H