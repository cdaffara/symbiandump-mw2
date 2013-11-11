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


#ifndef LISTENERLOCATIONEFFECTIMPL_H
#define LISTENERLOCATIONEFFECTIMPL_H

#include <e32base.h>
#include <ListenerLocationControl.h>
#include "EffectControlBase.h"
#include <ListenerLocationProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CListenerLocationEffect : public CBase,
                          public MListenerLocationControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CListenerLocationEffect();
        ~CListenerLocationEffect();
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
        
        // From MListenerLocationEffect begins
		/**
        * Gets the cartesian coordinates for the location of the position.
        * @since 3.0
        * @param aX The x-coordinate of the position (in millimeters)
        * @param aY The y-coordinate of the position (in millimeters)
        * @param aZ The z-coordinate of the position (in millimeters)
        * @return -
        */
		TInt LocationCartesian( TInt& aX, TInt& aY, TInt& aZ );

		/**
        * Gets the spherical coordinates for the location of the position.
        * @since 3.0
        * @param aAzimuth The Azimuth of the position (thousandths of radians)
        * @param aElevation The elevation of the position (thousandths of radians)
        * @param aRadius The radius of the position (thousandths of radians)
        * @return -
        */
		TInt LocationSpherical( TInt& aAzimuth, TInt& aElevation, TInt& aRadius );

		/**
        * Sets the cartesian coordinates for the location of the position.
        * @since 3.0
        * @param aX The x-coordinate of the position
        * @param aY The y-coordinate of the position
        * @param aZ The z-coordinate of the position
        * @return -
        */
		TInt SetLocationCartesian( TInt aX, TInt aY, TInt aZ );

		/**
        * Sets the spherical coordinates for the location of the position.
        * @since 3.0
        * @param aAzimuth The Azimuth of the position (thousandths of radians)
        * @param aElevation The elevation of the position (thousandths of radians)
        * @param aRadius The radius of the position (thousandths of radians)
        * @return -
        */
		TInt SetLocationSpherical( TInt aAzimuth, TInt aElevation, TInt aRadius );

       // From MListenerLocationEffect ends

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
        CListenerLocationProxy* iListenerLocationProxy;
        CListenerLocationProxy* iPrevListenerLocationProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // LISTENERLOCATIONEFFECTIMPL_H

// End of file
