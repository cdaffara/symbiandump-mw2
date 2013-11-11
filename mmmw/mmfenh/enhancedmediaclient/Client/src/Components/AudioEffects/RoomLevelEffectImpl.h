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


#ifndef ROOMLEVELEFFECTIMPL_H
#define ROOMLEVELEFFECTIMPL_H

#include <e32base.h>
#include <RoomLevelControl.h>
#include "EffectControlBase.h"
#include <RoomLevelProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
	class CReverbEffect;

	class CRoomLevelEffect : public CBase,
                          public MRoomLevelControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CRoomLevelEffect();
        ~CRoomLevelEffect();
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
        
        // From MRoomLevelEffect begins
        /**
        * Gets the RoomLevel current level in mB
        * @since 5.0
        * @return RoomLevel current level
        */
        TInt Level(TInt& aLevel);

        /**
        * Gets the RoomLevel current level maximum and minimum in mB
        * @since 5.0
        * @param aMin Minimum current level
        * @param aMax Maximum current level
        * @return -
        */
        TInt LevelRange( TInt& aMin, TInt& aMax );

        /**
        * Sets the RoomLevel level, it will leave if aRoomLevel is not within range of Min and Max
        * @since 5.0
        * @param aRoomLevelLevel The RoomLevel level in mB
        */
        TInt SetRoomLevel( TInt aRoomLevel );

       // From MRoomLevelEffect ends

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

        // From MRoomLevelEffect begins
        TInt AttachReverb( MReverbControl& aReverb );
        TInt DetachReverb( MReverbControl& aReverb );
        // From MRoomLevelEffect ends
    
    private:
        TInt DoApply();        
        TInt CreateEffectProxy();
        TInt DeleteEffectProxy();    
        TInt SavePreviousSettings();   
    
    private:
        CRoomLevelProxy* iRoomLevelProxy;
        CRoomLevelProxy* iPrevRoomLevelProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;
        CReverbEffect* iCReverbEffect;
        };
    } // namespace multimedia

#endif // ROOMLEVELEFFECTIMPL_H

// End of file
