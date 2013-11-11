/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Static Phone Server startup utility class.
*
*/


#ifndef PHSRVSTARTUP_H
#define PHSRVSTARTUP_H


// INCLUDES
#include <phclttypes.h> 


// FORWARD DECLARATIONS
//None.

// CLASS DECLARATION

/**
* Static server startup utility class. Encapsulates all the knowledge of how 
* to start and prepare the phone server, including build target (WINS/MARM) 
* specific functionality.
*
* @since 1.0
*/
class PhoneServerStartUpUtils
    {
    public: // New functions


        /**
        * Creates a new server and starts it.
        */
        static void CreateAndRunServerL();

        /**
        * Prepare the server thread, i.e. create a cleanup stack environment 
        * and start the server for real.
        *
        * @return Symbian error code of the operation.
        */
        static TInt PrepareServerEnvironment();
    };


#endif  // PHSRVSTARTUP_H


// End of File
