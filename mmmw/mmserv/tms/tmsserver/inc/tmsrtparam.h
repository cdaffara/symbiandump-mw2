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
 * Description:
 *
 */

#ifndef TMSRTPARAM_H
#define TMSRTPARAM_H

//  INCLUDES
#include <e32base.h>
#include <Profile.hrh>

namespace TMS {

// Defines different types of RingTone player
enum TRtPlayerType
    {
    EPlayerFirst,
    EPlayerAudio = EPlayerFirst,
    EPlayerBeepOnce,
    EPlayerSilent,
    EPlayerUnsecureVoIP,
    EPlayerDefault,
    EPlayerSequence,
    EPlayerBackup,
    EPlayerTts,
    EPlayerLast = EPlayerTts,
    EPlayerCount
    };

/**
 *  Structure holding ring tone configuration parameters.
 */
struct TMSRtParam
    {
public:
    TPtrC iTextToSay;
    TRtPlayerType iType;
    TUint iPriority;
    TUint iPreference;
    TInt iVolume;
    TProfileRingingType iRingingType;
    TBool iCallerText;
    TBool iCallerImage;
    };

} //namespace TMS

#endif // TMSRTPARAM_H

// End of File
