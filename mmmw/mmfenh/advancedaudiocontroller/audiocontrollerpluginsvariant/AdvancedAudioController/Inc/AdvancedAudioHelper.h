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
* Description:  Definition of Advance Audio Decoder base class.
*
*/


#ifndef ADVANCEDAUDIOHELPER_H
#define ADVANCEDAUDIOHELPER_H

#include <e32def.h>

class MAdvancedAudioDecoderUtilityObserver
    {
    public:
        virtual void DecodeIntervalEvent() = 0;
        virtual void SeekPositionReached(TUint aTimeMs) = 0;
		virtual TInt GetOffsets(TInt& aOffsets) = 0;
		virtual void PlayWindowEndPositionReached() = 0;
    };

#endif