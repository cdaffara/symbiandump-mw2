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
* Description:  Header of StreamControlObserver interface.
*
*/


#ifndef STREAMCONTROLOBSERVER_H
#define STREAMCONTROLOBSERVER_H

#include <ControlObserver.h>

namespace multimedia
    {
    class MStreamControlObserver : public MControlObserver
        {
        public:    
    		static const TUint KStateChangedEvent                   = KSpecificEventBase + 51;
    		static const TUint KDurationChangedEvent                = KSpecificEventBase + 52;
    		static const TUint KSeekingSupportChangedEvent          = KSpecificEventBase + 53;
    		static const TUint KRandomSeekingSupportChangedEvent    = KSpecificEventBase + 54;
        };
    } // namespace multimedia

#endif // STREAMCONTROLOBSERVER_H

// End of file
