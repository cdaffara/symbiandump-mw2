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
* Description:  Header of ListenerOrientationControl interface.
*
*/


#ifndef LISTENERORIENTATIONCONTROL_H
#define LISTENERORIENTATIONCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying ListenerOrientation Control
    const TUid KListenerOrientationEffectControl = {0x10207BE7};
    
    class MListenerOrientationControl : public MAudioEffectControl
        {
		public:
			static const TUint KOrientationChanged = KSpecificEffectBase + 1;
			static const TUint KOrientationVectorsChanged = KSpecificEffectBase + 2;

        public:
			/**
	        * Get the orientation vectors for the position
	        * @since 5.0
	        * @param aHeading The heading (thousandths of radians)
	        * @param aPitch The Pitch (thousandths of radians)
	        * @param aRoll The Roll (thousandths of radians)
	        * @return -
	        */
			virtual TInt Orientation( TInt& aHeading, TInt& aPitch, TInt& aRoll ) = 0; 
	
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
			virtual TInt OrientationVectors( TInt& aFrontX, TInt& aFrontY, TInt& aFrontZ,
									   	      TInt& aAboveX, TInt& aAboveY, TInt& aAboveZ ) = 0; 
	
			/**
	        * Sets the Heading, Pitch, Roll values for the orientation of the source
	        * @since 5.0
	        * @param aHeading The heading (thousandths of radians)
	        * @param aPitch The Pitch (thousandths of radians)
	        * @param aRoll The Roll (thousandths of radians)
	        * @return -
	        */
			virtual TInt SetOrientation( TInt aHeading, TInt aPitch, TInt aRoll ) = 0; 
	
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
			virtual TInt SetOrientationVectors( TInt aFrontX, TInt aFrontY, TInt aFrontZ,
									   	      TInt aAboveX, TInt aAboveY, TInt aAboveZ ) = 0; 
        };
    } // namespace multimedia

#endif // LISTENERORIENTATIONCONTROL_H

// End of file
