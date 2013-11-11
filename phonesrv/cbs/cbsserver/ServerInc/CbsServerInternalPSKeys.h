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
* Description:  Internal Publish & Subscribe keys for CBS Server.
 *
 *
*/


#ifndef CBSSERVER_INTERNAL_PS_KEYS_H
#define CBSSERVER_INTERNAL_PS_KEYS_H

const TUid KPSUidCellBroadcast = { 0x10202994 };

/**
 * This key is used to indicate the status of CBS Server to other 
 * applications. The value is set to '1' when server is started,
 * otherwise it is '0'.
 *
 * Possible values:
 * 0: Default value, CBS Server is not running
 * 1: Indicates that CBS Server is running
 */
const TUint32 KCbsServerStarted = 0x00000000;

#endif

// End of file
