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


#ifndef DistanceAttenuationEFFECTIMPL_H
#define DistanceAttenuationEFFECTIMPL_H

#include <e32base.h>
#include <DistanceAttenuationControl.h>
#include "EffectControlBase.h"
#include <DistanceAttenuationProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CDistanceAttenuationEffect : public CBase,
                          public MDistanceAttenuationControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CDistanceAttenuationEffect();
        ~CDistanceAttenuationEffect();
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
        /**
        * Gets the Distance Attenuation of the listener.
        * @since 3.0
        * @param aRMin The source-to-listener distance below which the sound level is constant
        * @param aRMax The source-to-listener distance above which the sound level is constant or zero
        * @param aMuteAfterMax Indicate if the sound is muted beyond RMax
        * @param aRollOffFactor Multiplier factor to source-to-listener distance
        * @param aRoomRollOffFactor Multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        TInt DistanceAttenuation( TInt& aRMin, TInt& aRMax, TBool& aMuteAfterMax,
                                           TUint& aRollOffFactor, TUint& aRoomRollOffFactor );

        /**
        * Get roll off factor maximum value.
        * @since 3.0
        * @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        TInt RollOffFactorMax( TUint& aRollOfFactorMax );

        /**
        * Get room roll off factor maximum value.
        * @since 3.0
        * @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        TInt RoomRollOffFactorMax( TUint& aRoomRollOfFactorMax );

        /**
        * Sets the Distance Attenuation
        * @since 3.0
        * @param aRMin The source-to-listener distance below which the sound level is constant
        * @param aRMax The source-to-listener distance above which the sound level is constant or zero
        * @param aMuteAfterMax Indicate if the sound is muted beyond RMax
        * @param aRollOffFactor Multiplier factor to source-to-listener distance
        * @param aRoomRollOffFactor Multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        TInt SetDistanceAttenuation( TInt aRMin, TInt aRMax, TBool aMuteAfterMax,
                                               TUint aRollOffFactor, TUint aRoomRollOffFactor );
        
        // From MDistanceAttenuationEffect begins

       // From MDistanceAttenuationEffect ends

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
        CDistanceAttenuationProxy* iDistanceAttenuationProxy;
        CDistanceAttenuationProxy* iPrevDistanceAttenuationProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // DistanceAttenuationEFFECTIMPL_H

// End of file
