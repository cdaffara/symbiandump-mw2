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
* Description:  Header of SourceDopplerControl interface.
*
*/


#ifndef SOURCEDOPPLERCONTROL_H
#define SOURCEDOPPLERCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying SourceDoppler Control
    const TUid KSourceDopplerEffectControl = {0x10207BE4};
    
    class MSourceDopplerControl : public MAudioEffectControl
        {
		public:
			static const TUint KFactorChanged = KSpecificEffectBase + 1;
			static const TUint KCartesianVelocityChanged = KSpecificEffectBase + 2;
			static const TUint KSphericalVelocityChanged = KSpecificEffectBase + 3;
			static const TUint KMaxFactorChanged = KSpecificEffectBase + 4;

        public:
			/**
	        * Get the velocity's cartesian settings
	        * @since 3.0
	        * @param aX Velocity in X direction (mm/s)
	        * @param aY Velocity in Y direction (mm/s)
	        * @param aZ Velocity in Z direction (mm/s)
	        */
			virtual TInt CartesianVelocity( TInt& aX, TInt& aY, TInt& aZ ) = 0; 
	
			/**
	        * Get the current multiplier factor
	        * @since 3.0
	        * @return multiplier factor
	        */
			virtual TInt Factor(TUint& aFactor) = 0; 
	
			/**
	        * Get the maximum multiplier factor
	        * @since 3.0
	        * @return multiplier factor
	        */
			virtual TInt FactorMax(TUint& aFactorMax) = 0; 
	
			/**
	        * Sets the velocity in Cartesian coordinates of the sound source with respect to the listener.
	        * @since 3.0
	        * @param aX Velocity in X direction (mm/s)
	        * @param aY Velocity in Y direction (mm/s)
	        * @param aZ Velocity in Z direction (mm/s)
	        * @return -
	        */
			virtual TInt SetCartesianVelocity( TInt aX, TInt aY, TInt aZ ) = 0; 
	
			/**
	        * Sets the multiplier factor.
	        * @since 3.0
	        * @param aFactor Factor value in hundredths that ranges from 0 to FactorMax(),
	        *                where 100 corresponds to 1.00, 200 corresponds to 2.00, etc.
	        * @return -
	        */
			virtual TInt SetFactor( TUint aFactor ) = 0; 
	
			/**
	        * Sets the velocity in spherical coordinates of the sound source with respect to the listener.
	        * @since 3.0
	        * @param aAzimuth the Azimuth (thousandths of radians)
	        * @param aElevation the elevation (thousandths of radians)
	        * @param aRadius the radius (thousandths of radians)
	        * @return -
	        */
			virtual TInt SetSphericalVelocity( TInt aAzimuth, TInt aElevation, TInt aRadius ) = 0; 
	
			/**
	        * Gets the velocity's spherical coordinates settings.
	        * @since 3.0
	        * @param aAzimuth the Azimuth (thousandths of radians)
	        * @param aElevation the elevation (thousandths of radians)
	        * @param aRadius the radius (thousandths of radians)
	        * @return -
	        */
			virtual TInt SphericalVelocity( TInt& aAzimuth, TInt& aElevation, TInt& aRadius ) = 0; 
        };
    } // namespace multimedia

#endif // SOURCEDOPPLERCONTROL_H

// End of file
