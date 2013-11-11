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
* Description:   XDM Settings CenRep keys
*
*/





#ifndef __XDMENGINEDOMAINCRKEYS__
#define __XDMENGINEDOMAINCRKEYS__

//Number of possible setting properties
const TInt KXdmPropertyCount                                = 10;

//UID of the XDM Settings Central Repository
const TUid KCRUidXdmEngine                                  = { 0x10207453 };

//UID of the XCAP Protocol variation repository
const TUid KCRUIDXDMEngineXcapLocal                         = { 0x10282384 };

/*
* First column: Identifies a settings collection
*/
const TUint32 KXdmSettingsId =                              0x00000000;

/*
* Second column: OMA XDM 1 - Uniquely identifies the application
*/
const TUint32 KXDMAppId =                                   0x00000001;

/*
* Third column: OMA XDM 2 - User displayable name for the XDM service
*/
const TUint32 KXDMAppName =                                 0x00000002;

/*
* Fourth column: OMA XDM 3 - Identity of the XDM service provider
*/
const TUint32 KXDMProviderId =                              0x00000003;

/*
* Fifth column: OMA XDM 4 - Access point used for the XCAP traffic
*/
const TUint32 KXDMNAPID =                                   0x00000004;

/*
* Sixth column: OMA XDM 5 - Reference to the SIP/IP core
*/
const TUint32 KXDMSipReference =	                        0x00000005;

/*
* Seventh column: OMA XDM 6 - The root of all XDM resources
* (points to the aggregation proxy address)
*/
const TUint32 KXDMXcapRootURI =                             0x00000006;

/*
* Eight column: OMA XDM 7 - HTTP digest username
*/
const TUint32 KXDMUserName =                                0x00000007;

/*
* Ninth column: OMA XDM 8 - HTTP digest password
*/
const TUint32 KXDMPassword =                                0x00000008;

/*
* Tenth column: OMA XDM 9 - Authentication method for XDMS over XCAP
*/
const TUint32 KXDMAuthType =                                0x00000009;

//************************ Other Settings ******************************

/*
* Maximum amount of cache data (in KB)
*/
const TUint32 KXDMXcapCacheSize =                           0x00000000;

#endif      // __XDMENGINEDOMAINCRKEYS__

