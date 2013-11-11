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
* Description:  Header of ListenerLocationControl interface.
*
*/


#ifndef LISTENERLOCATIONCONTROL_H
#define LISTENERLOCATIONCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying ListenerLocation Control
    const TUid KListenerLocationEffectControl = {0x10207BE9};
    
    class MListenerLocationControl : public MAudioEffectControl
        {
		public:
			static const TUint KLocationCartesianChanged = KSpecificEffectBase + 1;
			static const TUint KLocationSphericalChanged = KSpecificEffectBase + 2;

        public:
			/**
	        * Gets the cartesian coordinates for the location of the position.
	        * @since 3.0
	        * @param aX The x-coordinate of the position (in millimeters)
	        * @param aY The y-coordinate of the position (in millimeters)
	        * @param aZ The z-coordinate of the position (in millimeters)
	        * @return -
	        */
			virtual TInt LocationCartesian( TInt& aX, TInt& aY, TInt& aZ ) = 0;
	
			/**
	        * Gets the spherical coordinates for the location of the position.
	        * @since 3.0
	        * @param aAzimuth The Azimuth of the position (thousandths of radians)
	        * @param aElevation The elevation of the position (thousandths of radians)
	        * @param aRadius The radius of the position (thousandths of radians)
	        * @return -
	        */
			virtual TInt LocationSpherical( TInt& aAzimuth, TInt& aElevation, TInt& aRadius ) = 0;
	
			/**
	        * Sets the cartesian coordinates for the location of the position.
	        * @since 3.0
	        * @param aX The x-coordinate of the position
	        * @param aY The y-coordinate of the position
	        * @param aZ The z-coordinate of the position
	        * @return -
	        */
			virtual TInt SetLocationCartesian( TInt aX, TInt aY, TInt aZ ) = 0;
	
			/**
	        * Sets the spherical coordinates for the location of the position.
	        * @since 3.0
	        * @param aAzimuth The Azimuth of the position (thousandths of radians)
	        * @param aElevation The elevation of the position (thousandths of radians)
	        * @param aRadius The radius of the position (thousandths of radians)
	        * @return -
	        */
			virtual TInt SetLocationSpherical( TInt aAzimuth, TInt aElevation, TInt aRadius ) = 0;
        };
    } // namespace multimedia

#endif // LISTENERLOCATIONCONTROL_H

// End of file
