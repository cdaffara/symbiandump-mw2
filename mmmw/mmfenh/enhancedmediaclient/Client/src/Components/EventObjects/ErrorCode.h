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
* Description:  header of ErrorCode class.
*
*/


#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <e32base.h>
#include <Events.h>
#include "EventBase.h"

//using multimedia::MStreamControl;

namespace multimedia
    {
    class CErrorCode : public CEventBase,
                        public MErrorCode
        {
        public:
            CErrorCode( TInt aReason );
            ~CErrorCode();
            
            // From CEventBase
            TAny* GetInterface();
            
            // From MStateChangedEvent begins
            TInt GetErrorCode();
            // From MStateChangedEvent ends
            
        private:
            TInt iReason;
        };
    } // namespace multimedia

#endif // ERRORCODEOBJECT_H

// End of file
