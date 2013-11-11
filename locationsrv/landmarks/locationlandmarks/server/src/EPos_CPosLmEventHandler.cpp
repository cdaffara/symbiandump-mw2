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


// INCLUDE FILES
#include "EPos_CPosLmEventHandler.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"

// CONSTANTS
const TInt KEventQueueMaxSize = 5;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmEventHandler::CPosLmEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmEventHandler::CPosLmEventHandler()
    {
    }

// Two-phased constructor.
CPosLmEventHandler* CPosLmEventHandler::NewL()
    {
    CPosLmEventHandler* self = new (ELeave) CPosLmEventHandler();
    return self;
    }

// Destructor
CPosLmEventHandler::~CPosLmEventHandler()
    {
    LmServerGlobal::Complete(iHoldingMessage, KErrCancel);
    iEventQueue.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::HandleRequestForEventL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::HandleRequestForEventL(
    const RMessagePtr2& aMessage)
    {
    TPckgBuf<TPosLmEvent> event;
    TInt desMaxLength = aMessage.GetDesMaxLength(KPosLmServerEventArg);
    if (desMaxLength != event.Length())
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    if (iEventQueue.Count() > 0)
        {
        // Send the first event in the queue to the client.
        WriteEventAndComplete(aMessage, iEventQueue[0]);
        iEventQueue.Remove(0);
        }
    else
        {
        // No events in queue. Put the message on hold.
        if (!iHoldingMessage.IsNull())
            {
            PanicClient(aMessage,
                EPosEventNotifierAlreadyHasOutstandingRequest);
            }
        else
            {
            iHoldingMessage = aMessage;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::HandleIncomingEventL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::HandleIncomingEventL(
    TPosLmEvent aEvent)
    {
    if (!iHoldingMessage.IsNull())
        {
        // There is a message on hold. Send response immediately.
        WriteEventAndComplete(iHoldingMessage, aEvent);
        }
    else
        {
        // Subscriber is not waiting. Add event to queue.
        AddToEventQueueL(aEvent);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::CompleteAnyOutStandingRequest
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::CompleteAnyOutStandingRequest(
    TInt aError)
    {
    LmServerGlobal::Complete(iHoldingMessage, aError);
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::WriteEventAndComplete
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::WriteEventAndComplete(
    const RMessagePtr2& aMessage,
    TPosLmEvent aEvent)
    {
    TPckgBuf<TPosLmEvent> event(aEvent);
    if (LmServerGlobal::Write(
        aMessage, KPosLmServerEventArg, event) == KErrNone)
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        }
    // Client will be panicked if write fails.
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::AddToEventQueueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::AddToEventQueueL(
    TPosLmEvent aEvent)
    {
    if (aEvent.iEventType == EPosLmEventMediaRemoved)
        {
        // Media is removed. All other events are removed. They are unnecessary.
        iEventQueue.Reset();
        }
    else if (aEvent.iEventType == EPosLmEventNewDefaultDatabaseLocation)
        {
        // Remove all other equal events, if found.
        TInt queuePosition;
        RemoveEvents(ENewDefaultDbs, queuePosition);
        }
    else if (!MergeEventsIfPossibleL(aEvent))
        {
        // Incoming event should not be added to queue.
        return;
        }

    User::LeaveIfError(iEventQueue.Append(aEvent));
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::MergeEventsIfPossibleL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmEventHandler::MergeEventsIfPossibleL(
    TPosLmEvent aEvent)
    {
    TPosLmEventType incoming = aEvent.iEventType;
    TBool addEvent = ETrue;
    TInt noOfSpecificLms = 0;
    TInt noOfSpecificCats = 0;

    for (TInt i = 0; i < iEventQueue.Count(); i++)
        {
        TPosLmEventType eventInQueue = iEventQueue[i].iEventType;

        // EPosLmEventMediaRemoved and EPosLmEventNewDefaultDatabaseLocation
        // should not be removed from the queue, no matter what.
        // See also AddToEventQueue.
        if (eventInQueue != EPosLmEventMediaRemoved &&
            eventInQueue != EPosLmEventNewDefaultDatabaseLocation)
            {
            if (incoming == EPosLmEventUnknownChanges)
                {
                // Unknown changes. All old events are removed.
                iEventQueue.Remove(i--);
                }
            else if (incoming == EPosLmEventCategoryUnknownChanges)
                {
                // Unknown category changes.
                HandleUnknownEvent(eventInQueue,
                    IsSpecificCategory(eventInQueue) ||
                    eventInQueue == EPosLmEventCategoryUnknownChanges,
                    addEvent, i);
                }
            else if (incoming == EPosLmEventLandmarkUnknownChanges)
                {
                // Unknown landmark changes.
                HandleUnknownEvent(eventInQueue,
                    IsSpecificLandmark(eventInQueue) ||
                    eventInQueue == EPosLmEventLandmarkUnknownChanges,
                    addEvent, i);
                }
            else if (IsSpecificCategory(incoming))
                {
                // Specific category
                HandleSpecificEvent(eventInQueue,
                    EPosLmEventCategoryUnknownChanges,
                    addEvent, noOfSpecificCats, noOfSpecificLms);
                }
            else
                {
                // Specific landmark
                HandleSpecificEvent(eventInQueue,
                    EPosLmEventLandmarkUnknownChanges,
                    addEvent, noOfSpecificCats, noOfSpecificLms);
                }
            }
        }

    // If adding a specific landmark or category and queue is full,
    // either specific landmarks or categories are merged into a
    // corresponding EPosLmEventLandmarkUnknownChanges or
    // EPosLmEventCategoryUnknownChanges.
    if ((noOfSpecificLms > 0 || noOfSpecificCats > 0) &&
        iEventQueue.Count() == KEventQueueMaxSize)
        {
        TPosLmEvent event;
        event.iLandmarkItemId = KPosLmNullItemId;
        TInt queuePositionOfFirstSpecific;

        if (noOfSpecificCats > noOfSpecificLms)
            {
            RemoveEvents(ESpecificCategories, queuePositionOfFirstSpecific);
            event.iEventType = EPosLmEventCategoryUnknownChanges;
            addEvent = IsSpecificLandmark(incoming);
            }
        else
            {
            RemoveEvents(ESpecificLandmarks, queuePositionOfFirstSpecific);
            event.iEventType = EPosLmEventLandmarkUnknownChanges;
            addEvent = IsSpecificCategory(incoming);
            }

        // This should not happen, because when calling RemoveEvents above,
        // there will always be either categories or landmarks in the queue
        // (see if-statement above).
        __ASSERT_DEBUG(queuePositionOfFirstSpecific != KErrNotFound,
            PanicServer(KErrGeneral));

        User::LeaveIfError(iEventQueue.Insert(event,
            queuePositionOfFirstSpecific));
        }

    return addEvent;
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::HandleUnknownEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::HandleUnknownEvent(
    TPosLmEventType aEventInQueue,
    TBool aExpression,
    TBool& aAddEvent,
    TInt& aIndex)
    {
    if (aEventInQueue == EPosLmEventUnknownChanges)
        {
        // Unknown changes found in queue. Incoming event should not
        // be added to queue.
        aAddEvent = EFalse;
        }
    else if (aExpression)
        {
        // Remove any specific events or
        // EPosLmEventCategoryUnknownChanges/EPosLmEventLandmarkUnknownChanges.
        iEventQueue.Remove(aIndex--);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::HandleSpecificEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::HandleSpecificEvent(
    TPosLmEventType aEventInQueue,
    TPosLmEventType aCompareEventType,
    TBool& aAddEvent,
    TInt& aNoOfSpecificCats,
    TInt& aNoOfSpecificLms)
    {
    if (aEventInQueue == EPosLmEventUnknownChanges ||
        aEventInQueue == aCompareEventType)
        {
        // Unknown (category) changes found in queue. Incoming event
        // should not be added to queue.
        aAddEvent = EFalse;
        }
    else if (IsSpecificCategory(aEventInQueue))
        {
        aNoOfSpecificCats++;
        }
    else if (IsSpecificLandmark(aEventInQueue))
        {
        aNoOfSpecificLms++;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::IsSpecificLandmark
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmEventHandler::IsSpecificLandmark(
    TPosLmEventType aEventType)
    {
    return aEventType > EPosLmEventLandmarkUnknownChanges &&
        aEventType < EPosLmEventCategoryUnknownChanges;
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::IsSpecificCategory
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmEventHandler::IsSpecificCategory(
    TPosLmEventType aEventType)
    {
    return aEventType > EPosLmEventCategoryUnknownChanges;
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::RemoveEvents
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventHandler::RemoveEvents(
    TRemoveEventType aRemoveType,
    TInt& aQueuePosition)
    {
    aQueuePosition = KErrNotFound;

    for (TInt i = 0; i < iEventQueue.Count(); i++)
        {
        TPosLmEventType eventInQueue = iEventQueue[i].iEventType;

        if (aRemoveType == ESpecificLandmarks &&
                IsSpecificLandmark(eventInQueue) ||
            aRemoveType == ESpecificCategories &&
                IsSpecificCategory(eventInQueue) ||
            aRemoveType == ENewDefaultDbs &&
                eventInQueue == EPosLmEventNewDefaultDatabaseLocation)
            {
            if (aQueuePosition == KErrNotFound)
                {
                aQueuePosition = i;
                }
            iEventQueue.Remove(i--);
            }
        }
    }

// End of File
