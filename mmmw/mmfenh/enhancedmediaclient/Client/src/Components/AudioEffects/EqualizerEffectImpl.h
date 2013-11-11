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


#ifndef EQUALIZEREFFECTIMPL_H
#define EQUALIZEREFFECTIMPL_H

#include <e32base.h>
#include <EqualizerControl.h>
#include "EffectControlBase.h"
#include <AudioEqualizerProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CEqualizerEffect : public CBase,
                          public MEqualizerControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CEqualizerEffect();
        ~CEqualizerEffect();
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
        
        // From MEqualizerEffect begins
		/**
        * Get the band level in mB for the specified band
        * @since 3.0
        * @param aBand Frequency Band
        * @return Returns the band level in mB for the specified band
        */
		TInt BandLevel( TInt& aBand, TInt& aBandLevel );

		/**
        * Get the band width in Hz for the specified band.
        * @since 3.0
        * @param aBand Frequency Band
        * @return The band width in Hz for the specified band
        */
		TInt BandWidth( TInt& aBand, TInt& aBandWidth );

		/**
        * Get the center frequency in Hz for a given band
        * @since 3.0
        * @param aBand Frequency Band
        * @return The center frequency in Hz for a given band.
        */
		TInt CenterFrequency( TInt& aBand, TInt& aCenterFrequency );

		/**
        * Get the cross-over frequency between the given frequency band (aBand) and the next band
        * @since 3.0
        * @param aBand Frequency Band
        * @return Crossover frequency.
        */
		TInt CrossoverFrequency( TInt& aBand, TInt& aCrossoverFreq );

		/**
        * Get the dB range in mB for the equalizer
        * @since 3.0
        * @param aMin Minimum level in dB
        * @param aMin Maximum level in dB
        */
		TInt DbLevelLimits( TInt& aMin, TInt& aMax );

		/**
        * Get number of equalizer bands.
        * @since 3.0
        * @param -
        * @return The number of equalizer bands.
        */
		TInt NumberOfBands(TInt& aNumberofBands);

		/**
        * Sets the equalizer band level value in mB, ranging from Min to Max
        * @since 3.0
        * @param aBand Frequency Band
        * @param aLevel band level in dB, ranges from DbLevelLimits()
        * @return -
        */
		TInt SetBandLevel( TInt& aBand, TInt& aLevel );

       // From MEqualizerEffect ends

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
        // MAudioEffectObserver begins
        void EffectChanged( const CAudioEffect* aObservedEffect, TUint8 aEvent );
        // MAudioEffectObserver ends
        
        // From CEffectControlBase begins
        void Event( TEffectControlEvent aEvent );
        // From CEffectControlBase ends
    
    private:
        TInt DoApply();        
        TInt CreateEffectProxy();
        TInt DeleteEffectProxy();    
        TInt SavePreviousSettings();   
    
    private:
        CAudioEqualizerProxy* iEqualizerProxy;
        CAudioEqualizerProxy* iPrevEqualizerProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // EQUALIZEREFFECTIMPL_H

// End of file
