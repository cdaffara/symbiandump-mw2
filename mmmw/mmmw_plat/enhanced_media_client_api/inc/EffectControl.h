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
* Description:  Header of EffectControl interface.
*
*/


#ifndef EFFECTCONTROL_H
#define EFFECTCONTROL_H

#include <Control.h>

namespace multimedia
    {
    class MEffectControl : public MControl
        {
        public:
            virtual TInt Apply() = 0;
        };
    } // namespace multimedia

#endif // EFFECTCONTROL_H

// End of file
