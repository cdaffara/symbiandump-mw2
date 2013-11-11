/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Provides static functions which handles server startup.
*
*
*/


#ifndef POSLMSERVERSTARTUP_H
#define POSLMSERVERSTARTUP_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  This class contains static functions which handles server startup.
*/
class PosLmServerStartup
    {

    public: // New functions

        /**
        * Server thread entry point
        *
        * @return error code
        */
        static TInt StartServer();

    private: // New functions

        static void StartServerL();

    };

#endif      // POSLMSERVERSTARTUP_H

// End of File

