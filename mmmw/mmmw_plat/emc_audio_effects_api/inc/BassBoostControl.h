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
* Description:  Header of BassBoostControl interface.
*
*/


#ifndef BASSBOOSTCONTROL_H
#define BASSBOOSTCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying BassBoost Control
    const TUid KBassBoostEffectControl = {0x10207BDF};
    
    class MBassBoostControl : public MAudioEffectControl
        {
		public:
			static const TUint KBassBoostChanged = KSpecificEffectBase + 1;	
        };
    } // namespace multimedia

#endif // BASSBOOSTCONTROL_H

// End of file
