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
* Description: class to handle event notification to subscribers.
*
*
*/


#ifndef CPOSLMEVENTHANDLER_H
#define CPOSLMEVENTHANDLER_H

// INCLUDES
#include <e32base.h>
#include <EPos_Landmarks.h>

// CLASS DECLARATION

/**
* This class handles event notification to subscribers.
*/
class CPosLmEventHandler : public CBase
    {

    /**
    * Specifies which events that should be removed from the event queue in
    * @p RemoveEvents.
    */
    enum TRemoveEventType
        {
        ESpecificLandmarks = 0,
        ESpecificCategories,
        ENewDefaultDbs
        };

    public:  // Constructor & Destructor

        /**
        * Two-phased constructor.
        */
        static CPosLmEventHandler* NewL();

        /**
        * Destructor.
        */
        ~CPosLmEventHandler();

    public:     // New functions

        /**
        * If there is at least one event waiting in the event queue, this
        * message will be completed directly. Otherwise the message will be
        * put on hold until an event arrives.
        *
        * @param aMessagePtr A message.
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
        */
        void HandleIncomingEventL(
        /* IN */        TPosLmEvent aEvent
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
        CPosLmEventHandler();

        void WriteEventAndComplete(
        /* IN */        const RMessagePtr2& aMessage,
        /* IN */        TPosLmEvent aEvent
        );

        void AddToEventQueueL(
        /* IN */        TPosLmEvent aEvent
        );

        TBool MergeEventsIfPossibleL(
        /* IN */        TPosLmEvent aEvent
        );

        void HandleUnknownEvent(
        /* IN */        TPosLmEventType aEventInQueue,
        /* IN */        TBool aExpression,
        /* IN/OUT */    TBool& aAddEvent,
        /* IN/OUT */    TInt& aIndex
        );

        void HandleSpecificEvent(
        /* IN */        TPosLmEventType aEventInQueue,
        /* IN */        TPosLmEventType aCompareEventType,
        /* IN/OUT */    TBool& aAddEvent,
        /* IN/OUT */    TInt& aNoOfSpecificCats,
        /* IN/OUT */    TInt& aNoOfSpecificLms
        );

        TBool IsSpecificLandmark(
        /* IN */        TPosLmEventType aEventType
        );

        TBool IsSpecificCategory(
        /* IN */        TPosLmEventType aEventType
        );

        /**
        * Removes events of a specified type from the queue.
        *
        * @param aRemoveType A type of events to remove from the queue.
        * @param aQueuePosition Holds the queue position where the first
        *   occurrence of the event type was found. Set to KErrNotFound if
        *   no events was found. Note: the event at that position was removed,
        *   but another event may have been moved to that position.
        */
        void RemoveEvents(
        /* IN */        TRemoveEventType aRemoveType,
        /* OUT */       TInt& aQueuePosition
        );

        // By default, prohibit copy constructor
        CPosLmEventHandler( const CPosLmEventHandler& );
        // Prohibit assigment operator
        CPosLmEventHandler& operator= ( const CPosLmEventHandler& );

    private:    // Data

        RArray<TPosLmEvent> iEventQueue;
        RMessagePtr2 iHoldingMessage;

    };

#endif  // CPOSLMEVENTHANDLER_H

// End of File
