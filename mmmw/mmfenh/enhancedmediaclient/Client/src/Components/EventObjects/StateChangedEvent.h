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
* Description:  header of StateChangedEvent class.
*
*/


#ifndef StateChangedEvent_H
#define StateChangedEvent_H

#include <e32base.h>
#include <Events.h>
#include "EventBase.h"
#include <StreamControl.h>

using multimedia::MStreamControl;

namespace multimedia
    {
    class CStateChangedEvent : public CEventBase,
                        public MStateChangedEvent
        {
        public:
            CStateChangedEvent( MStreamControl::TStreamState aState, TInt aReason );
            ~CStateChangedEvent();
            
            // From CEventBase
            TAny* GetInterface();
            
            // From MStateChangedEvent begins
            MStreamControl::TStreamState GetState();
            TInt GetErrorCode();
            // From MStateChangedEvent ends
            

            
        private:
            MStreamControl::TStreamState iState;
            TInt iReason;
        };
    } // namespace multimedia

#endif // StateChangedEventOBJECT_H

// End of file
