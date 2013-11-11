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
* Description:  This file contains the base implementation of Source.
*
*/


#ifndef SOURCEBASE_H
#define SOURCEBASE_H

// INCLUDES
#include <e32base.h>

class MCustomCommand;
class TMMFMessageDestination;

namespace multimedia
    {
    // CLASS DECLARATION
    class CSourceBase : public CBase
        {
        public: // Constructors and destructor
            virtual ~CSourceBase();
            virtual void ServerSourceCreated(
                MCustomCommand& aCustomCommand,
                TMMFMessageDestination& aSourceHandle );
            
            virtual void ServerSourceDeleted();
            virtual TBool IsEncrypted() = 0;
            virtual TUid GetSourceUid() = 0;
            virtual TInt GetHeaderData(TPtr& aPtr) = 0;
        };
    } // namespace multimedia

#endif // SOURCEBASE_H

//  End of File
