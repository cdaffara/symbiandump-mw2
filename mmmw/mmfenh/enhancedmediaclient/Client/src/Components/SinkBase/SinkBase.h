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
* Description:  This file contains the base implementation of Sink.
*
*/


#ifndef SINKBASE_H
#define SINKBASE_H

// INCLUDES
#include <e32base.h>

namespace multimedia
    {
    // CLASS DECLARATION
    class CSinkBase : public CBase
        {
        public: // Constructors and destructor
            virtual ~CSinkBase();
            virtual TUid GetSinkUid();
        };
    } // namespace multimedia

#endif // SINKBASE_H

//  End of File
