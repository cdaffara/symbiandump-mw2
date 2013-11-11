/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SATEngine Subsystem PubSub uid and keys declaration.
*
*/


#ifndef SATDOMAINPSKEYS_H
#define SATDOMAINPSKEYS_H

// CONSTANTS

// SAT Configuration API
// Uid :
const TUid  KPSUidSatServer = { 0x101F79FC };

/**
 * Time between language selection notification and reboot.
 *
 * Default value: N/A
 */
const TUint32 KSatLanguageSelectionTimeBeforeReboot( 0x00000008 );

#endif      // SATDOMAINPSKEYS_H
