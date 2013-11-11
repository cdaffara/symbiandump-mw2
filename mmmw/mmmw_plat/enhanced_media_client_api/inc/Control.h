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
* Description:  Header of Control interface.
*
*/


#ifndef CONTROL_H
#define CONTROL_H

#include <e32base.h>

namespace multimedia
    {
    // Forward declaration
    class MControlObserver;
    
    enum TControlType
        {
            EStreamControl,
            ESourceControl,
            ESinkControl,
            EEffectControl
        };
    
    class MControl
        {
        public:
            virtual TInt AddObserver( MControlObserver& aObserver ) = 0;
            virtual TInt RemoveObserver( MControlObserver& aObserver ) = 0;
            virtual TUid Type() = 0;
            virtual TControlType ControlType() = 0;
        };
    } // namespace multimedia

#endif // CONTROL_H

// End of file
