/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of server core class.
*
*/

#ifndef POSINDICATORSERVERCONSTS_H
#define POSINDICATORSERVERCONSTS_H

/**
 * Server core name.
 */
_LIT( KPosIndHelperSrvName,"posindicatorhelperserver" );


/**
 * Current server version number
 *
 */
const TInt8 KMajorVersionNumber = 1;
const TInt8 KMinorVersionNumber = 0;
const TInt16 KBuildVersionNumber = 0;


/**
 * Service Ids for client service requests. These
 * service numbers should be used by the client library 
 * in all IPC messages to indicate the type of service
 * required
 */
enum TServiceIds
    {
    /* Subsession Open */
    ESubSessionOpen,
    /* Subsession Close */
    ESubSessionClose,
    /* Get current position */
    EGetCurrentPosition,
    /* Get current address info size */
    EGetCurrentAddressInfoSize,
    /* Get current address info */
    EGetCurrentAddressInfo,
    /* Cancel outstanding request*/
    ECancelCurrentOperation
   };


// This position in the IPC message should hold the sub-session handle
const TInt KParamSubsessionHandle = 3;  

// This position in the IPC message should hold last known position.
const TInt KParamLastKnownPosition = 0;  

// This position in the IPC message should hold current position.
const TInt KParamCurrentPosition = 0;  

// This position in the IPC message should hold current address size.
const TInt KParamCurrentAddressSize = 0;  

// This position in the IPC message should hold current address size.
const TInt KParamCurrentAddress = 0;  

#endif // POSINDICATORSERVERCONSTS_H
