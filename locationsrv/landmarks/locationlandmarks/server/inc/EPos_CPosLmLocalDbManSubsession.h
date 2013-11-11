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
* Description: Subsession class for local database management operations
*
*
*/


#ifndef CPOSLMLOCALDBMANSUBSESSION_H
#define CPOSLMLOCALDBMANSUBSESSION_H

// INCLUDE FILES
#include "EPos_CPosLmSubsessionBase.h"

// FORWARD DECLARATIONS

/**
* Subsession class for local database management operations, i.e.
* create, copy and delete operations for local databases.
*/
class CPosLmLocalDbManSubsession : public CPosLmSubsessionBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aLmServer Reference to the landmarks server.
        */
        static CPosLmLocalDbManSubsession* NewLC(
        /* IN  */ CPosLmServer* aLmServer);

        /**
        * Destructor.
        */
        ~CPosLmLocalDbManSubsession();

    public: // Functions from base classes

        /**
        * Handles the servicing of client requests to the server.
        *
        * @param aMessage The message containing the client request.
        */
        void ServiceL(
        /* IN */        const RMessage2& aMessage
        );

    public: // New functions

         /**
        * Handles database create and delete operations.
        *
        * @param aMessage The message containing the client request.
        */
        void HandleDbManagementOperationL(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Handles the database copy operation.
        *
        * @param aMessage The message containing the client request.
        */
        void HandleDbCopyOperationL(
        /* IN */        const RMessage2& aMessage
        );

    private:

        /**
        * C++ default constructor.
        *
        * @param aLmServer Reference to the landmarks server.
        */
        CPosLmLocalDbManSubsession(
        /* IN  */ CPosLmServer* aLmServer);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void CheckDiskSizeL(
        /* IN */        const TDesC& aUri
        );

        void CheckDiskSizeForCopyL(
        /* IN */        const TDesC& aSourceUri,
        /* IN */        const TDesC& aTargetUri
        );




    };

#endif //CPOSLMLOCALDBMANSUBSESSION_H

// End of File
