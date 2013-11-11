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
* Description:  Header of AudioEffectControl interface.
*
*/


#ifndef AUDIOEFFECTCONTROL_H
#define AUDIOEFFECTCONTROL_H

#include <EffectControl.h>

namespace multimedia
    {
    class MAudioEffectControl : public MEffectControl
        {
		public:
			static const TUint KEnabled = 1;				// The effect is enabled.
			static const TUint KDisabled = 2;				// The effect is disabled
			static const TUint KEnforced = 3;				// The effect is enforced
			static const TUint KNotEnforced = 4;			// The effect is not enforced
			static const TUint KLostUpdateRights = 5;		// The effect has lost update rights
			static const TUint KGainedUpdateRights = 6;		// The effect has gained update rights
			static const TUint KSpecificEffectBase = 20;	// Offset to specific effect events
		

        public:
			/**
	        * Disable the effect
	        * @since 5.0
	        */
			virtual TInt Disable() = 0;
	
			/**
	        * Enable the effect
	        * @since 5.0
	        */
			virtual TInt Enable() = 0;
	
			/**
	        * Enforce the effect.
	        * @since 5.0
	        * @param aEnforced Indicate the effect is to be enforced or not. ETrue = Enforced.
	        */
			virtual TInt Enforce( TBool &aEnforced ) = 0;
	
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
			virtual TInt HaveUpdateRights(TBool &aHaveUpdateRights) = 0;
	
			/**
	        * Check if the effect is enabled
	        * @since 5.0
	        * @return ETrue if the effect is enabled, EFalse if the effect is disabled.
	        */
			virtual TInt IsEnabled(TBool &aEnabled) = 0;
	
			/**
	        * Check if the effect is enforced.
	        * @since 5.0
	        * @return ETrue if the effect is enforced, EFalse if the effect isn ot enforced.
	        */
			virtual TInt IsEnforced(TBool &aEnforced) = 0;
	
			/*
	        * Get the unique identifier of the audio effect
	        * @since 5.0
	        * @return Unique identifier of the audio effect object.
	        */
			virtual TInt Uid(TUid &aUid) = 0 ;
        };
    } // namespace multimedia

#endif // AUDIOEFFECTCONTROL_H

// End of file
