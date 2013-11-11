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
* Description:  Definition of the DataBufferProcessedEvent class
*
*/


#ifndef DATABUFFERPROCESSEDEVENT_H
#define DATABUFFERPROCESSEDEVENT_H

#include <e32base.h>
#include <DataBufferSource.h>
#include "EventBase.h"

namespace multimedia
    {
    class MDataBuffer;
        
    class CDataBufferProcessedEvent : public CEventBase,
                                        public MBufferProcessedEvent
        {
        public:
            CDataBufferProcessedEvent( MDataBuffer* aBuffer, TInt aReason );
            ~CDataBufferProcessedEvent();
            
            // From CEventBase
            TAny* GetInterface();
            
            // From MStateChangedEvent begins
            virtual MDataBuffer* GetDataBuffer();
            virtual TInt GetErrorCode();
            // From MStateChangedEvent ends
            
        private:
            MDataBuffer* iDataBuffer;
            TInt iReason;
        };
    } // namespace multimedia

#endif // DATABUFFERPROCESSEDEVENT_H

// End of file
