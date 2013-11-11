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
* Description: Subsession class for database management event listening.
*
*
*/


#ifndef CPOSLMEVENTLISTENERSUBSESSION_H
#define CPOSLMEVENTLISTENERSUBSESSION_H

// INCLUDE FILES
#include <EPos_TPosLmDatabaseEvent.h>
#include "EPos_CPosLmSubsessionBase.h"

// FORWARD DECLARATIONS
class CPosLmDbManEventHandler;

/**
* Subsession class for database management event listening.
*/
class CPosLmEventListenerSubsession : public CPosLmSubsessionBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aLmServer Reference to the landmarks server.
        */
        static CPosLmEventListenerSubsession* NewLC(
        /* IN  */ CPosLmServer* aLmServer);

        /**
        * Destructor.
        */
        ~CPosLmEventListenerSubsession();

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
        * Handles an incoming operation event from the server.
        *
        * @param aEvent The event.
        * @param aUri A database URI.
        */
        void HandleOperationEventL(
        /* IN */        TPosLmDatabaseEvent aEvent,
        /* IN */        const TDesC& aUri
        );

        /**
        * Completes outstanding events in the event handlers.
        *
        * @param aError An error code to complete the request with.
        */
        void CompleteOutstandingEventRequest(
        /* IN */        TInt aError
        );

    private:

        /**
        * C++ default constructor.
        *
        * @param aLmServer Reference to the landmarks server.
        */
        CPosLmEventListenerSubsession(
        /* IN  */ CPosLmServer* aLmServer);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        CPosLmDbManEventHandler* iDbManEventHandler;



    };

#endif //CPOSLMEVENTLISTENERSUBSESSION_H

// End of File
