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
* Description:  Header of ReverbControl interface.
*
*/


#ifndef REVERBCONTROL_H
#define REVERBCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying Reverb Control
    const TUid KReverbEffectControl = {0x10207BDE};
    
    class MReverbControl : public MAudioEffectControl
        {
		public:
			static const TUint KDecayTimeChanged = KSpecificEffectBase + 1;
			static const TUint KDecayHFRatioChanged = KSpecificEffectBase + 2;
			static const TUint KDensityChanged = KSpecificEffectBase + 3;
			static const TUint KDiffusionChanged = KSpecificEffectBase + 4;
			static const TUint KReflectionsDelayChanged = KSpecificEffectBase + 5;
			static const TUint KReflectionsLevelChanged = KSpecificEffectBase + 6;
			static const TUint KReverbDelayChanged = KSpecificEffectBase + 7;
			static const TUint KReverbLevelChanged = KSpecificEffectBase + 8;
			static const TUint KRoomLevelChanged = KSpecificEffectBase + 9;
			static const TUint KRoomHFLevelChanged = KSpecificEffectBase + 10;		

        public:
        /**
        * Gets the decay HF Ratio in hundredths
        * @since 5.0
        * @return decay HF Ratio
        */
        virtual TInt DecayHFRatio(TUint &aRatio) = 0;

        /**
        * Gets the reverb decay HF Ratio minimum and maximum in hundredths.
        * @since 5.0
        * @param aMin Minimum decay HF Ratio
        * @param aMax Maximum decay HF Ratio
        */
        virtual TInt DecayHFRatioRange(TUint& aMin, TUint& aMax) = 0;

        /**
        * Gets the decay time in milliseconds
        * @since 5.0
        * @return decay time
        */
        virtual TInt DecayTime(TUint &aDecayTime) = 0;

        /**
        * Gets the allowable reverb decay time range in milliseconds.
        * @since 5.0
        * @param aMin Minimum decay time in milliseconds
        * @param aMax Maximum decay time in milliseconds
        */
        virtual TInt DecayTimeRange(TUint& aMin, TUint& aMax) = 0;

        /**
        * Gets the density current value as a percentage in hundredths
        * @since 5.0
        * @return density value
        */
        virtual TInt Density(TUint &aDensity) = 0;

        /**
        * Gets the diffusion current value as a percentage in hundredths.
        * @since 5.0
        * @return diffusion value
        */
        virtual TInt Diffusion(TUint &aDiffusion) = 0;

        /**
        * Gets the reverb reflections delay in ms.
        * @since 5.0
        * @return reverb reflections delay
        */
        virtual TInt ReflectionsDelay(TUint &aDelay) = 0;

        /**
        * Gets the reverb reflections delay maximum in milliseconds.
        * @since 5.0
        * @return reverb reflections delay maximum
        */
        virtual TInt ReflectionsDelayMax(TUint &aDelayMax) = 0;

        /**
        * Gets the reverb reflections level in mB
        * @since 5.0
        * @return Reverb reflections level
        */
        virtual TInt ReflectionsLevel(TInt &aLevel) = 0;

        /**
        * Gets the reverb reflections level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum reflections level
        * @param aMax Maximum reflections level
        */
        virtual TInt ReflectionLevelRange( TInt& aMin, TInt& aMax ) = 0;

        /**
        * Gets the reverb delay in milliseconds
        * @since 5.0
        * @return reverb delay
        */
        virtual TInt ReverbDelay(TUint &aDelay) = 0;

        /**
        * Gets the reverb delay maximum in milliseconds
        * @since 5.0
        * @return reverb delay maximum
        */
        virtual TInt ReverbDelayMax(TUint &aDelayMax) = 0;

        /**
        * Gets the reverb current level in mB
        * @since 5.0
        * @return reverb current level
        */
        virtual TInt ReverbLevel(TInt &aLevel) = 0;

        /**
        * Gets the reverb current level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum current level
        * @param aMax Maximum current level
        * @return -
        */
        virtual TInt ReverbLevelRange( TInt& aMin, TInt& aMax ) = 0;

        /**
        * Gets the room HF level current ratio
        * @since 5.0
        * @return room HF level ratio
        */
        virtual TInt RoomHFLevel(TInt &aLevel) = 0;

        /**
        * Gets the room HF level maximum and minimum ratios
        * @since 5.0
        * @param aMin Minimum current room HF level
        * @param aMax Maximum current room HF level
        * @return -
        */
        virtual TInt RoomHFLevelRange( TInt& aMin, TInt& aMax ) = 0;

        /**
        * Gets the room level current value in mB
        * @since 5.0
        * @return room level value
        */
        virtual TInt RoomLevel(TInt &aLevel) = 0;

        /**
        * Gets the room level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum current room level
        * @param aMax Maximum current room level
        * @return -
        */
        virtual TInt RoomLevelRange( TInt& aMin, TInt& aMax ) = 0;

        /**
        * Sets the decay HF Ratio in hundredths
        * @since 5.0
        * @param aDecayHFRatio The decay high frequence ratio in hundredths
        * @return -
        */
        virtual TInt SetDecayHFRatio( TUint aDecayHFRatio ) = 0;

        /**
        * Sets the decay time in millisecond
        * @since 5.0
        * @param aDecayTime Decay time in ms
        */
        virtual TInt SetDecayTime( TUint aDecayTime ) = 0;

        /**
        * Sets the density value as percentage in hundredths
        * @since 5.0
        * @param aDensity The density.
        */
        virtual TInt SetDensity( TUint aDensity ) = 0;

        /**
        * Sets the diffusion value as a percentage in hundredths
        * @since 5.0
        * @param aDiffusion The diffusion.
        */
        virtual TInt SetDiffusion( TUint aDiffusion ) = 0;

        /**
        * Sets the reverb reflections delay
        * @since 5.0
        * @param aRefectionsDelay The reflection delay in ms.
        */
        virtual TInt SetReflectionsDelay( TUint aReflectionsDelay ) = 0;

        /**
        * Sets the reverb reflections level in milli-dB
        * @since 5.0
        * @param aRefectionsLevel The reflection level in mB
        */
        virtual TInt SetReflectionsLevel( TInt aReflectionsLevel ) = 0;

        /**
        * Sets the reverb delay
        * @since 5.0
        * @param aReverbDelay The reverb delay in ms
        */
        virtual TInt SetReverbDelay( TUint aReverbDelay ) = 0;

        /**
        * Sets the reverb level
        * @since 5.0
        * @param aReverbLevel The reverb level in mB
        */
        virtual TInt SetReverbLevel( TInt aReverbLevel ) = 0;

        /**
        * Sets the room HF level ratio
        * @since 5.0
        * @param aRoomHFLevel The room high frequency ratio
        */
        virtual TInt SetRoomHFLevel( TInt aRoomHFLevel ) = 0;

        /**
        * Sets the room level value in milli-dB
        * @since 5.0
        * @param aRoomLevel The room level
        */
        virtual TInt SetRoomLevel( TInt aRoomLevel ) = 0;

        /**
        * Gets the total delay maximum in milliseconds
        * @since 5.0
        * @return reverb delay maximum
        */
        virtual TInt DelayMax(TUint &aDelayMax) = 0;

        };
    } // namespace multimedia

#endif // REVERBCONTROL_H

// End of file
