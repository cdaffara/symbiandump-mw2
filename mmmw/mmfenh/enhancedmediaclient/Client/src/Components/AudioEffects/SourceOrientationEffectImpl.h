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


#ifndef SOURCEORIENTATIONEFFECTIMPL_H
#define SOURCEORIENTATIONEFFECTIMPL_H

#include <e32base.h>
#include <SourceOrientationControl.h>
#include "EffectControlBase.h"
#include <SourceOrientationProxy.h>
#include <MAudioEffectObserver.h>

namespace multimedia
    {
	class MControlObserver;
    class CSourceOrientationEffect : public CBase,
                          public MSourceOrientationControl,
                          public MAudioEffectObserver,
                          public CEffectControlBase
        {
    public:
        CSourceOrientationEffect();
        ~CSourceOrientationEffect();
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
        
		// From MSourceOrientationEffect begins
		/**
	    * Get the orientation vectors for the position
	    * @since 5.0
	    * @param aHeading The heading (thousandths of radians)
	    * @param aPitch The Pitch (thousandths of radians)
	    * @param aRoll The Roll (thousandths of radians)
	    * @return -
	    */
		TInt Orientation( TInt& aHeading, TInt& aPitch, TInt& aRoll );
	
		/**
	    * Gets the orientation of the position.
	    * @since 5.0
	    * @param aFrontX X value of Front vector
	    * @param aFrontY Y value of Front vector
	    * @param aFrontZ Z value of Front vector
	    * @param aAboveX X value of Above vector
	    * @param aAboveY Y value of Above vector
	    * @param aAboveZ Z value of Above vector
	    * @return -
	    */
		TInt OrientationVectors( TInt& aFrontX, TInt& aFrontY, TInt& aFrontZ,
									TInt& aAboveX, TInt& aAboveY, TInt& aAboveZ );
	
		/**
	    * Sets the Heading, Pitch, Roll values for the orientation of the source
	    * @since 5.0
	    * @param aHeading The heading (thousandths of radians)
	    * @param aPitch The Pitch (thousandths of radians)
	    * @param aRoll The Roll (thousandths of radians)
	    * @return -
	    */
		TInt SetOrientation( TInt aHeading, TInt aPitch, TInt aRoll );
	
		/**
	    * Sets the Front and Above vectors for the orientation of the position.
	    * @since 5.0
	    * @param aFrontX X value of Front vector
	    * @param aFrontY Y value of Front vector
	    * @param aFrontZ Z value of Front vector
	    * @param aAboveX X value of Above vector
	    * @param aAboveY Y value of Above vector
	    * @param aAboveZ Z value of Above vector
	    * @return -
	    */
		TInt SetOrientationVectors( TInt aFrontX, TInt aFrontY, TInt aFrontZ,
										TInt aAboveX, TInt aAboveY, TInt aAboveZ );
       // From MSourceOrientationEffect ends

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
        CSourceOrientationProxy* iSourceOrientationProxy;
        CSourceOrientationProxy* iPrevSourceOrientationProxy;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MCustomCommand* iCustomCommand;        
        RPointerArray<MControlObserver> iObservers;    
        };
    } // namespace multimedia

#endif // SOURCEORIENTATIONEFFECTIMPL_H

// End of file
