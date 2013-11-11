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
* Description:  Header of RoomLevelControl interface.
*
*/


#ifndef ROOMLEVELCONTROL_H
#define ROOMLEVELCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {

    // Uid identifying RoomLevel Control
    const TUid KRoomLevelEffectControl = {0x10207BE3};

    class MReverbControl;

    class MRoomLevelControl : public MAudioEffectControl
        {
        public:
            static const TUint KRoomLevelChanged = KSpecificEffectBase + 1;

        public:
            /**
            * Gets the RoomLevel current level in mB
            * @since 5.0
            * @return RoomLevel current level
            */
            virtual TInt Level(TInt& aLevel) = 0;

            /**
            * Gets the RoomLevel current level maximum and minimum in mB
            * @since 5.0
            * @param aMin Minimum current level
            * @param aMax Maximum current level
            * @return -
            */
            virtual TInt LevelRange( TInt& aMin, TInt& aMax ) = 0;

            /**
            * Sets the RoomLevel level, it will leave if aRoomLevel is not within range of Min and Max
            * @since 5.0
            * @param aRoomLevelLevel The RoomLevel level in mB
            */
            virtual TInt SetRoomLevel( TInt aRoomLevel ) = 0;

            /**
            * Links Room Level Control to Reverb Control object.
            * Room Level control cannot be used independently, A reverb control
            * has to be attached to it always.
            * Reverb Control object should be added to MStreamControl before
            * adding Room Level Control to MStreamControl.
            *
            * Usage: CREATION SEQUENCE
            *       // 1. Create MReverbControl and MRoomLevelControl from factory
            *       // 2. Attach room level to reverb
            *       MRoomLevelControl::AttachReverb(MReverbControl);
            *       // 3. Add effects in the following order
            *       MStreamControl::AddEffect(MReverbControl);
            *       MStreamControl::AddEffect(MRoomLevelControl);
            *       
            *        DELETION SEQUENCE
            *       MRoomLevelControl->DetachReverb(*iEnvRev);
            *       MStreamControl->RemoveEffect( MRoomLevelControl );
            *       MStreamControl->RemoveEffect( MReverbControl );
            *
            * @since 5.0
            * @return
            */
            virtual TInt AttachReverb( MReverbControl& aReverb ) = 0;

            /**
            * Delinks Room Level Control to Reverb Control object.
            * @since 5.0
            * @return
            */
            virtual TInt DetachReverb( MReverbControl& aReverb ) = 0;
        };
    } // namespace multimedia

#endif // ROOMLEVELCONTROL_H

// End of file
