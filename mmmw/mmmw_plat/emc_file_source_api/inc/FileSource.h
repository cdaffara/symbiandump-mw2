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
* Description:  Header of File Source interface.
*
*/



#ifndef FILESOURCE_H
#define FILESOURCE_H

#include <e32base.h>
#include <SourceControl.h>

namespace multimedia
    {
    // Uid identifying Control
    const TUid KFileSourceControl = {0x10207B8D};
    
    // Class declaration
    class MFileSource : public MSourceControl
        {
        public:
            // Config msgs
            virtual TInt Open( TDesC& aFileName, TDesC8& aMimeType  ) = 0;
            virtual TInt GetBitRate(TUint& aRate) = 0;
            virtual TInt GetFileName(TPtr& aFileName) = 0;
        };
    } // namespace multimedia

#endif // FILESOURCE_H

// End of file
