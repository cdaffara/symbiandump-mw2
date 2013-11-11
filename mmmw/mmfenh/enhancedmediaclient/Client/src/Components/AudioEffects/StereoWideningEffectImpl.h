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


#ifndef STEREOWIDENINGEFFECTIMPL_H
#define STEREOWIDENINGEFFECTIMPL_H

#include <e32base.h>
#include <StereoWideningControl.h>
#include "EffectControlBase.h"
#include <StereoWideningProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CStereoWideningEffect : public CBase,
                          public MStereoWideningControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CStereoWideningEffect();
        ~CStereoWideningEffect();
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
        
        // From MStereoWideningEffect begins
        /**
        * Check if continuous level is supported for stereo widening.
        * @since 5.0
        * @param  ETrue if continuous level is supported for stereo widening.  If this returns
        *         EFalse, then the adaptation is actually setting the level to the nearest supported discrete
        *         value, when SetStereoWidenlingLevelL is called.
        * @return Error Code
        */
        TInt IsContinuousLevelSupported(TBool& aSupported);

        /**
        * Set stereo widening level.
        * @since 5.0
        * @param aLevel Stereo widening level ranges from 0-100, where 0 is None and 100 is Maximum widening.
        */
        TInt SetStereoWideningLevel( TInt aLevel );

        /**
        * Get the stereo widening settings.
        * @since 5.0
        * @param aLevel Gets the Current Stereo widening level
        * @return Error Code
        */
        TInt StereoWideningLevel(TInt& aLevel);

       // From MStereoWideningEffect ends

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
        CStereoWideningProxy* iStereoWideningProxy;
        CStereoWideningProxy* iPrevStereoWideningProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // STEREOWIDENINGEFFECTIMPL_H

// End of file
