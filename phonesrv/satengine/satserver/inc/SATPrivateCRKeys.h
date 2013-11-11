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
* Description:  SATEngine Subsystem CenRep uid and keys declaration.
*
*/


#ifndef SATPRIVATECRKEYS_H
#define SATPRIVATECRKEYS_H

#include <SATInternalPSKeys.h>

// CONSTANTS

// SAT Icon Data Support API
// Uid :
const TUid KCRUidSatServer = { 0x102078E1 };

// Defines the limit for KSatMaxIconDataSize
// Shared Data Value was KMaxIconDataSize
/**
* Limit for icon size. Icon support is disabled if value is 0.
* Possible values are 0 to 9999999999999999.
*
* Default value: N/A
*/
const TUint32 KSatMaxIconDataSize( 0x00000004 );

// Defines if Cover UI is supported
/**
*  Support is disabled if value is 0
*
* Default value: N/A
*/
const TUint32 KSatSecondaryDisplaySupport( 0x00000008 );

// Defines if Launch Browser GCF Support is available
/**
* Support is disabled if value is 0
*
* Default value: N/A
*/
const TUint32 KSatLaunchBrowserGCF( 0x00000016 );

// Defines whether Home Zone Icon is visible or not
/**
* Support is disabled if value is 0
*
* Default value: N/A
*/
const TUint32 KSatHomeZoneIconVisible( 0x00000032 );

// Defines whether USSD sending result is shown if no alpha id
/**
* Support is disabled if value is 0
* 
* Default value: N/A
*/
const TUint32 KSatDisplayUssdResult( 0x00000064 );

#endif      // SATPRIVATECRKEYS_H

// End of File
