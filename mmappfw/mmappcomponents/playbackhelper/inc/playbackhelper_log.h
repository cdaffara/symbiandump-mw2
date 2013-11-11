/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Debug print macros
*
*/

// Version : %version: 7 %


#ifndef PLAYBACKHELPER_LOG_H
#define PLAYBACKHELPER_LOG_H

// INCLUDES
#include <e32svr.h>
#include <e32def.h>


class PlaybackHelperDebug
{
    public:
        inline static void NullLog( TRefByValue<const TDesC16> /*aFmt*/, ... )
        {
        }
};


#ifdef _DEBUG
    #define PLAYBACKHELPER_DEBUG RDebug::Print
#else
    #define PLAYBACKHELPER_DEBUG PlaybackHelperDebug::NullLog
#endif


#endif  // PLAYBACKHELPER_LOG_H

// End of File
