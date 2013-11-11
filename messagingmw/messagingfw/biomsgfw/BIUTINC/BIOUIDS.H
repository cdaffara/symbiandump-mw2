// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description: Contains the TUid defines for the Bio Messaging
// BIOUUIDS.H
// 
//

/**
 * @file
 * @publishedAll
 * @released
 */

#if !defined(__BIOUTILS_H__)
#define __BIOUTILS_H__

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include <biomessageuids.h>
#endif

//forward declarations
/**
* @publishedAll
* @released
*/
class TUid;

//  BIO Mtm components

/**
* BIO message type UID. 
* @publishedAll
* @released
*/
const TUid KUidBIOMessageTypeMtm		= {0x10001262};

/** 
* Stream ID used for storing parsed BIO data. 
* @publishedAll
* @released
*/
const TUid KUidMsvBIODataStream			= {0x10005268}; // stream used for storing parsed data

#endif *__BIOUTILS_H__
