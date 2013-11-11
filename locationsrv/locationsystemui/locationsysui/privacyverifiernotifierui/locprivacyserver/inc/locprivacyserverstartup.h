/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class definition for server startup .
*
*/

#ifndef LOCPRIVACYSERVERSTARTUP_H
#define LOCPRIVACYSERVERSTARTUP_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  This class contains static functions which handles server startup.
*/
class LocPrivacyServerStartup
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

#endif      // LOCPRIVACYSERVERSTARTUP_H

// End of File

