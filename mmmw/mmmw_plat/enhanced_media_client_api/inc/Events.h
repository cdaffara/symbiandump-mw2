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
* Description:  Header of Events interface.
*
*/


#ifndef EVENTS_H
#define EVENTS_H

#include <StreamControl.h>

namespace multimedia
    {
    class MStreamControl;
    class MErrorCode
        {
        public:
            virtual TInt GetErrorCode() = 0;
        };
    
    class MStateChangedEvent : public MErrorCode
        {
        public:
            virtual MStreamControl::TStreamState GetState() = 0;
        };        
    } // namespace multimedia

#endif // EVENTS_H

// End of file
