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
* Description:  Music Player private CenRep keys.
*
*/



#ifndef MPXPRIVATECRKEYS_H
#define MPXPRIVATECRKEYS_H

#include <e32cmn.h>



// Music Settings API

const TUid KCRUidMPXSettings = {0x101FFC03};

const TUint32 KMPXPlaybackVolume = 0x00000001; // Percentage of max volume
const TUint32 KMPXPlaybackRandomMode = 0x00000002;
const TUint32 KMPXPlaybackRepeatMode = 0x00000003;

// view utility settings
const TUint32 KMPXVUCurrentDefaultView = 0x00000004;

// Pre-Init enabled
const TUint32 KMPXPlaybackPreInitEnabled = 0x00000005;

// Maximum size of MPX global heap in MB.
const TUint32 KMPXMaxGlobalHeapSize = 0x00000006;

const TUint32 KMPXPlaybackMute = 0x00000007;

#endif      // MPXPRIVATECRKEYS_H

// End of File
