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
* Description:  Header of DataBuffer interface.
*
*/


#ifndef DATABUFFER_H
#define DATABUFFER_H

namespace multimedia
    {
    // CLASS DECLARATION
    class MDataBuffer
        {
        public:
            virtual TPtr8& GetBufferPtr() = 0;
            virtual TBool IsLastBuffer() = 0;
            virtual void SetLastBuffer(TBool aStatus) = 0;
        };
    }

#endif // DATABUFFER_H

//  End of File
