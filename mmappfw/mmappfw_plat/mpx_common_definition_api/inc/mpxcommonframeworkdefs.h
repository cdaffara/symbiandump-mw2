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
* Description:  Framework common definitions
*
*/


#ifndef MPXCOMMONFRAMEWORKDEFS_H
#define MPXCOMMONFRAMEWORKDEFS_H

#include <e32base.h>
#include <mpxitemid.h>

enum TMPXOpenMode
    {
    EMPXOpenDefault,
    EMPXOpenGroupOrPlaylist,
    EMPXOpenAllItems,
    EMPXOpenNoPlaylist,
    EMPXOpenPlaylistOnly
    };

/**
 *  Enable timer guard for async request. Debug purpose only
 */
//#define _ENABLE_GUARD_TIMER
#ifdef _ENABLE_GUARD_TIMER
//Guard timer 15 seconds for async request
const TInt32 KMPXGuardTimer=15000000;
#endif

#endif // MPXCOMMONFRAMEWORKDEFS_H

