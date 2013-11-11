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
* Description:  Header of VolumeControl interface.
*
*/


#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <EffectControl.h>

namespace multimedia
    {
    
    // Uid identifying Volume Control
    const TUid KVolumeEffectControl = {0x10207B75};
    
    class MVolumeControl : public MEffectControl
        {
        public:
            enum TVolumeRampMode
                {
                EIMMEDIATE
                };
                            
        public:
            virtual TInt GetMinVolume(TInt& aMinVolume) = 0;
            virtual TInt GetMaxVolume(TInt& aMaxVolume) = 0;
            virtual TInt GetDefaultVolume(TInt& aDefaultVolume) = 0;
            virtual TInt GetVolume(TInt& aVolume) = 0;
            virtual TInt SetVolume(TInt& aVolume) = 0;                        
            virtual TInt SetVolumeRamp(TInt aInitialVol,TInt aFinalVol,TUint64& aDuration,TVolumeRampMode aMode) = 0;                                    
        };
    } // namespace multimedia

#endif // VOLUMECONTROL_H

// End of file
