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
* Description: class handles event notification to subscribers.
*
*
*/


#ifndef CPOSLMDBMANEVENTHANDLER_H
#define CPOSLMDBMANEVENTHANDLER_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <EPos_TPosLmDatabaseEvent.h>

// CLASS DECLARATION

/**
* This class handles event notification to subscribers.
*/
class CPosLmDbManEventHandler : public CBase
    {

    public:  // Constructor & Destructor

        /**
        * Two-phased constructor.
        */
        static CPosLmDbManEventHandler* NewL();

        /**
        * Destructor.
        */
        ~CPosLmDbManEventHandler();

    public:     // New functions

        /**
        * Returns the database URI from last event to client.
        *
        * @param aMessage A message.
        */
        void ServerUriFromLastEvent(
        /* IN */        const RMessagePtr2& aMessage
        );

        /**
        * If there is at least one event waiting in the event queue, this
        * message will be completed directly. Otherwise the message will be
        * put on hold until an event arrives.
        *
        * @param aMessage A message.
        */
        void HandleRequestForEventL(
        /* IN */        const RMessagePtr2& aMessage
        );

        /**
        * Called when a landmark operation has generated an event.
        * If there is a message waiting for an event it will be completed
        * directly. Otherwise this event will be put in the event queue.
        *
        * @param aEvent An event.
        * @param aUri A database URI.
        */
        void HandleIncomingEventL(
        /* IN */        TPosLmDatabaseEvent aEvent,
        /* IN */        const TDesC& aUri
        );

        /**
        * Completes an outstanding event if it exists.
        *
        * @param aError The error code to complete with.
        */
        void CompleteAnyOutStandingRequest(
        /* IN */        TInt aError
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmDbManEventHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        */
        void ConstructL();

        void WriteEventAndComplete(
        /* IN */        const RMessagePtr2& aMessage
        );

        void AddToEventQueueL(
        /* IN */        TPosLmDatabaseEvent aEvent,
        /* IN */        const TDesC& aUri
        );

        // By default, prohibit copy constructor
        CPosLmDbManEventHandler( const CPosLmDbManEventHandler& );
        // Prohibit assigment operator
        CPosLmDbManEventHandler& operator= ( const CPosLmDbManEventHandler& );

    private:    // Data

        RArray<TPosLmDatabaseEvent> iEventQueue;
        RMessagePtr2 iHoldingMessage;
        CDesCArrayFlat* iUriArray;
        HBufC* iLastUri;

    };

#endif  // CPOSLMEVENTHANDLER_H

// End of File
