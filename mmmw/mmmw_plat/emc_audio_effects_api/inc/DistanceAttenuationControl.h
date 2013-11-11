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
* Description:  Header of DistanceAttenuationControl interface.
*
*/


#ifndef DISTANCEATTENUATIONCONTROL_H
#define DISTANCEATTENUATIONCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying DistanceAttenuation Control
    const TUid KDistanceAttenuationEffectControl = {0x10207BE2};
    
    class MDistanceAttenuationControl : public MAudioEffectControl
        {
		public:
			static const TUint KDistanceAttenuationChanged = KSpecificEffectBase + 1;

        public:
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
        virtual TInt DistanceAttenuation( TInt& aRMin, TInt& aRMax, TBool& aMuteAfterMax,
                                           TUint& aRollOffFactor, TUint& aRoomRollOffFactor ) = 0;

        /**
        * Get roll off factor maximum value.
        * @since 3.0
        * @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        virtual TInt RollOffFactorMax( TUint& aRollOfFactorMax ) = 0;

        /**
        * Get room roll off factor maximum value.
        * @since 3.0
        * @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
        * @return -
        */
        virtual TInt RoomRollOffFactorMax( TUint& aRoomRollOfFactorMax ) = 0;

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
        virtual TInt SetDistanceAttenuation( TInt aRMin, TInt aRMax, TBool aMuteAfterMax,
                                               TUint aRollOffFactor, TUint aRoomRollOffFactor ) = 0;
        };
    } // namespace multimedia

#endif // DISTANCEATTENUATIONCONTROL_H

// End of file
