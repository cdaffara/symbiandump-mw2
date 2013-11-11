/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Resource Manager.
*
*/

#ifndef CPHSRVRESOURCEMANAGER_H
#define CPHSRVRESOURCEMANAGER_H


// INCLUDES
#include <barsc.h>
#include <f32file.h>


// CLASS DECLARATION

/**
*  The resource manager.
*
*  @since 1.0
*/
class CPhSrvResourceManager : 
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        *
        * @param aFsSession The file session.
        */
        CPhSrvResourceManager( RFs& aFsSession );

        /**
        * C++ destructor.
        */
        ~CPhSrvResourceManager();

        /**
        * Symbian OS second phase constructor
        */
        void ConstructL();


    public: // New functions

        /**
        * Read some text from the resource file.
        *
        * @param aResourceId The resource identifier to read
        * @return The resource text on the cleanup stack
        */
        HBufC* ReadResourceLC( TUint aResourceId );


    private:  // Data

        // The file session.
        RFs&            iFsSession;

        // The resource file.
        RResourceFile   iResourceFile;
    };

#endif // CPHSRVRESOURCEMANAGER_H


// End of File
