/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  SIMPLE engine CenRep keys
*
*/



#ifndef SIMPLEENGINEDOMAINCRKEYS_H
#define SIMPLEENGINEDOMAINCRKEYS_H

#include <e32base.h>

const TUid KCRUIDSimpleOTA = {0x10281EEE};

// SIMPLEEngine OMA Presence Client Provisioning

// APPID
const TUint32 KSimpleAppID =                0x00000001;

// PROVIDER-ID
const TUint32 KSimpleProviderID =           0x00000002;

// ToConRef (SIP profile id)
const TUint32 KSimpleSipProfile =           0x00000003;

// NAME
const TUint32 KSimpleSetName =              0x00000004;

// Identifies a set in Presence Settings UI
const TUint32 KSimpleSetId =	            0x00000005;
			
// XDM set id
const TUint32 KSimpleXDMSet	=               0x00000006;
		
// Content Server address			
const TUint32 KSimpleContentSvrAdd =	    0x00000007;

// BitMasked status of each property
const TUint32 KSimpleStatusofProps =	0x00000008;	

// CLIENT-OBJ-DATA-LIMIT
const TUint32 KSimpleClientObjDataLimit =   0x0000000A;

// SOURCE-THROTTLE-PUBLISH
const TUint32 KSimpleSourceThrottlePublish = 0x0000000B;

// MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS
const TUint32 KSimpleMaxNbrPresenceSubs =   0x0000000C;

// MAX-NUMBER-OF-SUBSCRIPTIONS-IN-PRESENCE-LIST
const TUint32 KSimpleMaxNbrSubsInList =     0x0000000D;

// SERVICE-URI-TEMPLATE
const TUint32 KSimpleServiceUriTemplate =   0x0000000E;


#endif      // SIMPLEENGINEDOMAINCRKEYS_H

// End of File
