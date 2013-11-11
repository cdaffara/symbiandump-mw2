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
* Description:  header of EventBase class.
*
*/


#ifndef EVENTBASE_H
#define EVENTBASE_H

#include <e32base.h>

namespace multimedia
    {
    
    // Event Base class. All event objects need to inherit from this class.
    class CEventBase : public CBase
        {
        public:
            virtual ~CEventBase();
            
            // Returns a pointer to the interface implemented by the Event Object.
            // For e.g. if a Event Objec implements interface MTestIF
            // it should have the following code snipped in the implementation.
            // MTestIF* itfc = this;
            // return itfc;
            virtual TAny* GetInterface() = 0;
        };
    } // namespace multimedia

#endif // EVENTBASE_H

// End of file
