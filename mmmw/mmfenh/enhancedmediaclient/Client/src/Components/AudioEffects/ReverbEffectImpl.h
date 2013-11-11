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


#ifndef REVERBEFFECTIMPL_H
#define REVERBEFFECTIMPL_H

#include <e32base.h>
#include <ReverbControl.h>
#include "EffectControlBase.h"
#include <EnvironmentalReverbProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CReverbEffect : public CBase,
                          public MReverbControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CReverbEffect();
        ~CReverbEffect();
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

		// From MAudioEffectControl
		/**
        * Disable the effect
        * @since 5.0
        */
		TInt Disable();

		/**
        * Enable the effect
        * @since 5.0
        */
		TInt Enable();

		/**
        * Enforce the effect.
        * @since 5.0
        * @param aEnforced Indicate the effect is to be enforced or not. ETrue = Enforced.
        */
		TInt Enforce( TBool &aEnforced );

		/**
        * Check if this effect object currently has update rights.
        * A client can lose update rights in some hardware platforms where there are a limited
        * number of instances of an effect that can exist at the same time. When an effect instance
        * has lost update rights the user can still change settings, but any calls to Apply the
        * settings will be deferred until update rights are regained.
        * @since 5.0
        * @return ETrue if this object currently has rights to update the settings of this effect,
        *         EFalse otherwise.
        */
		TInt HaveUpdateRights(TBool &aHaveUpdateRights);

		/**
        * Check if the effect is enabled
        * @since 5.0
        * @return ETrue if the effect is enabled, EFalse if the effect is disabled.
        */
		TInt IsEnabled(TBool &aEnabled);

		/**
        * Check if the effect is enforced.
        * @since 5.0
        * @return ETrue if the effect is enforced, EFalse if the effect isn ot enforced.
        */
		TInt IsEnforced(TBool &aEnforced);

		/*
        * Get the unique identifier of the audio effect
        * @since 5.0
        * @return Unique identifier of the audio effect object.
        */
		TInt Uid(TUid &aUid);
		// From MAudioEffectControl Ends

		// MReverbControl Begins
		
        /**
        * Gets the decay HF Ratio in hundredths
        * @since 5.0
        * @return decay HF Ratio
        */
        TInt DecayHFRatio(TUint &aRatio);

        /**
        * Gets the reverb decay HF Ratio minimum and maximum in hundredths.
        * @since 5.0
        * @param aMin Minimum decay HF Ratio
        * @param aMax Maximum decay HF Ratio
        */
        TInt DecayHFRatioRange(TUint& aMin, TUint& aMax);

        /**
        * Gets the decay time in milliseconds
        * @since 5.0
        * @return decay time
        */
        TInt DecayTime(TUint &aDecayTime);

        /**
        * Gets the allowable reverb decay time range in milliseconds.
        * @since 5.0
        * @param aMin Minimum decay time in milliseconds
        * @param aMax Maximum decay time in milliseconds
        */
        TInt DecayTimeRange(TUint& aMin, TUint& aMax);

        /**
        * Gets the density current value as a percentage in hundredths
        * @since 5.0
        * @return density value
        */
        TInt Density(TUint &aDensity);

        /**
        * Gets the diffusion current value as a percentage in hundredths.
        * @since 5.0
        * @return diffusion value
        */
        TInt Diffusion(TUint &aDiffusion);

        /**
        * Gets the reverb reflections delay in ms.
        * @since 5.0
        * @return reverb reflections delay
        */
        TInt ReflectionsDelay(TUint &aDelay);

        /**
        * Gets the reverb reflections delay maximum in milliseconds.
        * @since 5.0
        * @return reverb reflections delay maximum
        */
        TInt ReflectionsDelayMax(TUint &aDelayMax);

        /**
        * Gets the reverb reflections level in mB
        * @since 5.0
        * @return Reverb reflections level
        */
        TInt ReflectionsLevel(TInt &aLevel);

        /**
        * Gets the reverb reflections level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum reflections level
        * @param aMax Maximum reflections level
        */
        TInt ReflectionLevelRange( TInt& aMin, TInt& aMax );

        /**
        * Gets the reverb delay in milliseconds
        * @since 5.0
        * @return reverb delay
        */
        TInt ReverbDelay(TUint &aDelay);

        /**
        * Gets the reverb delay maximum in milliseconds
        * @since 5.0
        * @return reverb delay maximum
        */
        TInt ReverbDelayMax(TUint &aDelayMax);

        /**
        * Gets the reverb current level in mB
        * @since 5.0
        * @return reverb current level
        */
        TInt ReverbLevel(TInt &aLevel);

        /**
        * Gets the reverb current level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum current level
        * @param aMax Maximum current level
        * @return -
        */
        TInt ReverbLevelRange( TInt& aMin, TInt& aMax );

        /**
        * Gets the room HF level current ratio
        * @since 5.0
        * @return room HF level ratio
        */
        TInt RoomHFLevel(TInt &aLevel);

        /**
        * Gets the room HF level maximum and minimum ratios
        * @since 5.0
        * @param aMin Minimum current room HF level
        * @param aMax Maximum current room HF level
        * @return -
        */
        TInt RoomHFLevelRange( TInt& aMin, TInt& aMax );

        /**
        * Gets the room level current value in mB
        * @since 5.0
        * @return room level value
        */
        TInt RoomLevel(TInt &aLevel);

        /**
        * Gets the room level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum current room level
        * @param aMax Maximum current room level
        * @return -
        */
        TInt RoomLevelRange( TInt& aMin, TInt& aMax );

        /**
        * Sets the decay HF Ratio in hundredths
        * @since 5.0
        * @param aDecayHFRatio The decay high frequence ratio in hundredths
        * @return -
        */
        TInt SetDecayHFRatio( TUint aDecayHFRatio );

        /**
        * Sets the decay time in millisecond
        * @since 5.0
        * @param aDecayTime Decay time in ms
        */
        TInt SetDecayTime( TUint aDecayTime );

        /**
        * Sets the density value as percentage in hundredths
        * @since 5.0
        * @param aDensity The density.
        */
        TInt SetDensity( TUint aDensity );

        /**
        * Sets the diffusion value as a percentage in hundredths
        * @since 5.0
        * @param aDiffusion The diffusion.
        */
        TInt SetDiffusion( TUint aDiffusion );

        /**
        * Sets the reverb reflections delay
        * @since 5.0
        * @param aRefectionsDelay The reflection delay in ms.
        */
        TInt SetReflectionsDelay( TUint aReflectionsDelay );

        /**
        * Sets the reverb reflections level in milli-dB
        * @since 5.0
        * @param aRefectionsLevel The reflection level in mB
        */
        TInt SetReflectionsLevel( TInt aReflectionsLevel );

        /**
        * Sets the reverb delay
        * @since 5.0
        * @param aReverbDelay The reverb delay in ms
        */
        TInt SetReverbDelay( TUint aReverbDelay );

        /**
        * Sets the reverb level
        * @since 5.0
        * @param aReverbLevel The reverb level in mB
        */
        TInt SetReverbLevel( TInt aReverbLevel );

        /**
        * Sets the room HF level ratio
        * @since 5.0
        * @param aRoomHFLevel The room high frequency ratio
        */
        TInt SetRoomHFLevel( TInt aRoomHFLevel );

        /**
        * Sets the room level value in milli-dB
        * @since 5.0
        * @param aRoomLevel The room level
        */
        TInt SetRoomLevel( TInt aRoomLevel );

        /**
        * Gets the total delay maximum in milliseconds
        * @since 5.0
        * @return reverb delay maximum
        */
        TInt DelayMax(TUint &aDelayMax);
		
		// MReverbControl Ends
		
        // MAudioEffectObserver begins
        void EffectChanged( const CAudioEffect* aObservedEffect, TUint8 aEvent );
        // MAudioEffectObserver ends
        
        // From CEffectControlBase begins
        void Event( TEffectControlEvent aEvent );
        // From CEffectControlBase ends
        
        TInt GetCEnvironmentalReverb( CEnvironmentalReverb*& aOutEnvRev );
    
    private:
        TInt DoApply(); 
        TInt CreateEffectProxy();
        TInt DeleteEffectProxy();       
        TInt SavePreviousSettings();   
    
    private:
        CEnvironmentalReverbProxy* iReverbProxy;
        CEnvironmentalReverbProxy* iPrevReverbProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;    
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // REVERBEFFECTIMPL_H

// End of file
