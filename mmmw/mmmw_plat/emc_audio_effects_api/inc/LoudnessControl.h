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
* Description:  Header of LoudnessControl interface.
*
*/


#ifndef LOUDNESSCONTROL_H
#define LOUDNESSCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying Loudness Control
    const TUid KLoudnessEffectControl = {0x10207BE1};
    
    class MLoudnessControl : public MAudioEffectControl
        {
        	
        };
    } // namespace multimedia

#endif // LOUDNESSCONTROL_H

// End of file
