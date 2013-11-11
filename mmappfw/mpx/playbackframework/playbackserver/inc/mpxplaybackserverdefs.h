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
* Description:  Playback Server common definitions
*
*/


#ifndef MPXPLAYBACKSERVERDEFS_H
#define MPXPLAYBACKSERVERDEFS_H


#include<e32base.h>
#include<apmstd.h>
#include<mpxplaybackframeworkdefs.h>

// Server name
_LIT(KMPXPlaybackServerName,"MPXPlaybackServer");
_LIT(KMPXPlaybackServerImg,"mpxplaybackserver");        // DLL/EXE name
// UID
const TUid KMPXPlaybackServerUid3={0x1020381D};  
// Serverversion number
const TUint KMPXPlaybackServerMajorVersionNumber=0;
const TUint KMPXPlaybackServerMinorVersionNumber=1;
const TUint KMPXPlaybackServerBuildVersionNumber=1;

#endif  // MPXPLAYBACKSERVERDEFS_H

