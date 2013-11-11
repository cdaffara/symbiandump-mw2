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
* Description:  Project file for EnhancedMediaClient Utility
*
*/


#ifndef VOLUMEEFFECTIMPL_H
#define VOLUMEEFFECTIMPL_H

#include <e32base.h>
#include <VolumeControl.h>
#include "EffectControlBase.h"


namespace multimedia
    {
    class CVolumeEffect : public CBase,
                          public MVolumeControl,
                          public CEffectControlBase
        {
    public:
        CVolumeEffect();
        ~CVolumeEffect();
        TInt PostConstructor();

        // From MControl begins
        TInt AddObserver( MControlObserver& aObserver );
        TInt RemoveObserver( MControlObserver& aObserver );
        TUid Type();
        TControlType ControlType();  
        // From MControl ends

        // From MEffectControl begins
        TInt Apply();
        // From MEffectControl ends
        
        // From MVolumeEffect begins
        TInt GetMinVolume(TInt& aMinVolume);
        TInt GetMaxVolume(TInt& aMaxVolume);
        TInt GetDefaultVolume(TInt& aDefaultVolume);
        TInt GetVolume(TInt& aVolume);
        TInt SetVolume(TInt& aVolume);                        
        TInt SetVolumeRamp(TInt aInitialVol,TInt aFinalVol,TUint64& aDuration,TVolumeRampMode aMode); 

       // From MVolumeEffect ends
        
        // From CEffectControlBase begins
        void Event( TEffectControlEvent aEvent );
        // From CEffectControlBase ends
    
    private:
        TInt DoApply();        
    
    private:
        TInt iVolume;
        TInt64 iRampDuration;
        // Set when Client calls Apply
        TBool iEnabled;
        };
    } // namespace multimedia

#endif // VOLUMEEFFECT_H

// End of file
