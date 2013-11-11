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
* Description:  Header of SourceControl interface.
*
*/


#ifndef SOURCECONTROL_H
#define SOURCECONTROL_H

#include <Control.h>
#include <e32base.h>

namespace multimedia
    {
    
    class MSourceControl : public MControl
        {
        public:
            virtual TInt GetSize( TUint& aSize ) = 0;
            virtual TInt GetMimeType( TDes8& aMimeType ) = 0;
            virtual TInt Close() = 0;
        };
    } // namespace multimedia

#endif // SOURCECONTROL_H

// End of file
