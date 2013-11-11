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
* Description:  Header of StereoWideningControl interface.
*
*/


#ifndef STEREOWIDENINGCONTROL_H
#define STEREOWIDENINGCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying StereoWidening Control
    const TUid KStereoWideningEffectControl = {0x10207BE0};
    
    class MStereoWideningControl : public MAudioEffectControl
        {
		public:
			static const TUint KStereoWideningLevelChanged = KSpecificEffectBase + 1;

        public:
	        /**
	        * Check if continuous level is supported for stereo widening.
	        * @since 5.0
	        * @param  ETrue if continuous level is supported for stereo widening.  If this returns
	        *         EFalse, then the adaptation is actually setting the level to the nearest supported discrete
	        *         value, when SetStereoWidenlingLevelL is called.
	        * @return Error Code
	        */
	        virtual TInt IsContinuousLevelSupported(TBool& aSupported) = 0; 
	
	        /**
	        * Set stereo widening level.
	        * @since 5.0
	        * @param aLevel Stereo widening level ranges from 0-100, where 0 is None and 100 is Maximum widening.
	        */
	        virtual TInt SetStereoWideningLevel( TInt aLevel ) = 0; 
	
	        /**
	        * Get the stereo widening settings.
	        * @since 5.0
	        * @param aLevel Gets the Current Stereo widening level
	        * @return Error Code
	        */
	        virtual TInt StereoWideningLevel(TInt& aLevel) = 0; 
        };
    } // namespace multimedia

#endif // STEREOWIDENINGCONTROL_H

// End of file
