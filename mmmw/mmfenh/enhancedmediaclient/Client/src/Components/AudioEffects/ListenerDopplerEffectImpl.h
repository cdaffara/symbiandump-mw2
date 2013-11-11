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


#ifndef LISTENERDOPPLEREFFECTIMPL_H
#define LISTENERDOPPLEREFFECTIMPL_H

#include <e32base.h>
#include <ListenerDopplerControl.h>
#include "EffectControlBase.h"
#include <ListenerDopplerProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CListenerDopplerEffect : public CBase,
                          public MListenerDopplerControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CListenerDopplerEffect();
        ~CListenerDopplerEffect();
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
        
        // From MListenerDopplerEffect begins
		/**
        * Get the velocity's cartesian settings
        * @since 3.0
        * @param aX Velocity in X direction (mm/s)
        * @param aY Velocity in Y direction (mm/s)
        * @param aZ Velocity in Z direction (mm/s)
        */
		TInt CartesianVelocity( TInt& aX, TInt& aY, TInt& aZ );

		/**
        * Get the current multiplier factor
        * @since 3.0
        * @return multiplier factor
        */
		TInt Factor(TUint& aFactor);

		/**
        * Get the maximum multiplier factor
        * @since 3.0
        * @return multiplier factor
        */
		TInt FactorMax(TUint& aFactorMax);

		/**
        * Sets the velocity in Cartesian coordinates of the sound source with respect to the listener.
        * @since 3.0
        * @param aX Velocity in X direction (mm/s)
        * @param aY Velocity in Y direction (mm/s)
        * @param aZ Velocity in Z direction (mm/s)
        * @return -
        */
		TInt SetCartesianVelocity( TInt aX, TInt aY, TInt aZ );

		/**
        * Sets the multiplier factor.
        * @since 3.0
        * @param aFactor Factor value in hundredths that ranges from 0 to FactorMax(),
        *                where 100 corresponds to 1.00, 200 corresponds to 2.00, etc.
        * @return -
        */
		TInt SetFactor( TUint aFactor );

		/**
        * Sets the velocity in spherical coordinates of the sound source with respect to the listener.
        * @since 3.0
        * @param aAzimuth the Azimuth (thousandths of radians)
        * @param aElevation the elevation (thousandths of radians)
        * @param aRadius the radius (thousandths of radians)
        * @return -
        */
		TInt SetSphericalVelocity( TInt aAzimuth, TInt aElevation, TInt aRadius );

		/**
        * Gets the velocity's spherical coordinates settings.
        * @since 3.0
        * @param aAzimuth the Azimuth (thousandths of radians)
        * @param aElevation the elevation (thousandths of radians)
        * @param aRadius the radius (thousandths of radians)
        * @return -
        */
		TInt SphericalVelocity( TInt& aAzimuth, TInt& aElevation, TInt& aRadius );

       // From MListenerDopplerEffect ends

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
        CListenerDopplerProxy* iListenerDopplerProxy;
        CListenerDopplerProxy* iPrevListenerDopplerProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // LISTENERDOPPLEREFFECTIMPL_H

// End of file
