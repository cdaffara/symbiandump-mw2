/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Private Publish and Subscribe keys.
*
*/


#ifndef SSSETTINGSPRIVATEPSKEYS_H
#define SSSETTINGSPRIVATEPSKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

/**
* Configuration settings in (U)SIM and product profile
*/
const TUid KPSUidSSConfig = {0x101F87E4};

/**
* Cached Customer Service Profile(CSP) values.
*
* This setting is only for SSSettings'
* internal use, don't use it directly!
*
* Bits 24-31 Call Completion Services
* Bits 16-23 Call Offering Services
* Bits 8-15 Call Restriction Services
* Bits 0-7 Cphs Teleservices
*/
const TUint32 KSettingsCspCache1 = 0x00000001;

/**
* Cached Customer Service Profile(CSP) values.
*
* This setting is only for SSSettings'
* internal use, don't use it directly!
*
* Bits 24-31 Other Supp Services (03)
* Bits 16-23  CPSH Teleservices (06)
* Bits 8-15 CPHS Features (07)
* Bits 0-7 Cphs Value Added Services
*/
const TUint32 KSettingsCspCache2 = 0x00000002;

/**
* Cached Customer Service Profile(CSP) values.
*
* This setting is only for SSSettings'
* internal use, don't use it directly!
*
* Bits 24-31 <not used>
* Bits 16-23 Information Numbers (D5) 
* Bits 8-15 Phase 2+ Services (09)
* Bits 0-7 Number Identifying (08)
*/
const TUint32 KSettingsCspCache3 = 0x00000003;

/**
* Support for ALS in product profile, integer value.
*
* Possible values:
* 0 (not supported)
* 1 (supported)
*/
const TUint32 KSettingsPPSupportsALS = 0x00000004;

/**
* Support for ALS in SIM, integer value.
*
* Possible values:
* 0 (not supported)
* 1 (supported)
*/
const TUint32 KSettingsSIMSupportsALS = 0x00000005;

#endif      // SSSETTINGSPRIVATEPSKEYS_H

// End of file