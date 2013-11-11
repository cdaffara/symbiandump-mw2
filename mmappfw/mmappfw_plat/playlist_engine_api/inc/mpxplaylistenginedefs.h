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
* Description:  Common definitions for the playlist engine
*
*/


#ifndef MPXPLAYLISTENGINEDEFS_H
#define MPXPLAYLISTENGINEDEFS_H

// playlist types
enum TMPXPlaylistType
    {
    EMPXPlaylistTypeUnknown = 0x00,
    EMPXPlaylistTypeM3U = 0x01,
    EMPXPlaylistTypePLA = 0x02,
    EMPXPlaylistTypeMPV = 0x04,
    EMPXPlaylistTypeXSPF = 0x08,
    EMPXPlaylistTypeXML = 0x10
    };

#endif // MPXPLAYLISTENGINEDEFS_H

