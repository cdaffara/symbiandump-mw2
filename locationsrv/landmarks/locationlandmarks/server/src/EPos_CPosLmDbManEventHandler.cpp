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
#include "EPos_CPosLmDbManEventHandler.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"

// CONSTANTS
const TInt KEventQueueMaxSize = 5;
const TInt KArrayGranularity = 6;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDbManEventHandler::CPosLmDbManEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDbManEventHandler::CPosLmDbManEventHandler()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDbManEventHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::ConstructL()
    {
    iUriArray = new (ELeave) CDesCArrayFlat(KArrayGranularity);
    }

// Two-phased constructor.
CPosLmDbManEventHandler* CPosLmDbManEventHandler::NewL()
    {
    CPosLmDbManEventHandler* self = new (ELeave) CPosLmDbManEventHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmDbManEventHandler::~CPosLmDbManEventHandler()
    {
    CompleteAnyOutStandingRequest(KErrCancel);
    iEventQueue.Close();
    delete iUriArray;
    delete iLastUri;
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::ServerUriFromLastEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::ServerUriFromLastEvent(
    const RMessagePtr2& aMessage)
    {
    if (!iLastUri || iLastUri->Length() == 0)
        {
        LmServerGlobal::Complete(aMessage, KErrNotFound);
        }
    else
        {
        LmServerGlobal::Write(aMessage, KPosLmServerUriArg, *iLastUri);
        LmServerGlobal::Complete(aMessage, KErrNone);
        }
    delete iLastUri;
    iLastUri = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmDbManEventHandler::HandleRequestForEventL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::HandleRequestForEventL(
    const RMessagePtr2& aMessage)
    {
    // check that descriptor parameters are OK so that server
    // won't panic when trying to write.
    TPckgBuf<TPosLmDatabaseEvent> event;
    TPckgBuf<TInt> length;
    TInt eventMaxLength = aMessage.GetDesMaxLength(KPosLmServerDbManEventArg);
    TInt lengthMaxLength = aMessage.GetDesMaxLength(KPosLmServerUriLengthArg);
    if (eventMaxLength  != event.Length() ||
        lengthMaxLength != length.Length())
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    if (iEventQueue.Count() > 0)
        {
        // Send the first event in the queue to the client.
        WriteEventAndComplete(aMessage);
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
// CPosLmDbManEventHandler::HandleIncomingEventL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::HandleIncomingEventL(
    TPosLmDatabaseEvent aEvent,
    const TDesC& aUri)
    {
    if (iEventQueue.Count() == KEventQueueMaxSize)
        {
        iEventQueue.Reset();
        iUriArray->Reset();
        aEvent.iEventType = EPosLmDbUnknownEvent;
        AddToEventQueueL(aEvent, KNullDesC);
        }
    else
        {
        AddToEventQueueL(aEvent, aUri);
        }

    if (!iHoldingMessage.IsNull())
        {
        // There is a message on hold. Send response immediately.
        WriteEventAndComplete(iHoldingMessage);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmDbManEventHandler::CompleteAnyOutStandingRequest
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::CompleteAnyOutStandingRequest(
    TInt aError)
    {
    LmServerGlobal::Complete(iHoldingMessage, aError);
    }

// -----------------------------------------------------------------------------
// CPosLmDbManEventHandler::WriteEventAndComplete
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::WriteEventAndComplete(
    const RMessagePtr2& aMessage)
    {
    HBufC* tmpBuf = iUriArray->MdcaPoint(0).Alloc();
    if (!tmpBuf)
        {
        LmServerGlobal::Complete(aMessage, KErrNoMemory);
        }
    else
        {
        iUriArray->Delete(0);
        iUriArray->Compress();
        delete iLastUri;
        iLastUri = tmpBuf;

        TPckgBuf<TPosLmDatabaseEvent> event(iEventQueue[0]);
        TPckgBuf<TInt> length(iLastUri->Length());
        if (LmServerGlobal::Write(aMessage, KPosLmServerEventArg, event)
            == KErrNone &&
            LmServerGlobal::Write(aMessage, KPosLmServerUriLengthArg, length)
            == KErrNone)
            {
            LmServerGlobal::Complete(aMessage, KErrNone);
            }
        iEventQueue.Remove(0);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventHandler::AddToEventQueueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbManEventHandler::AddToEventQueueL(
    TPosLmDatabaseEvent aEvent,
    const TDesC& aUri)
    {
    TInt index = KErrNotFound;
    if (aEvent.iEventType == EPosLmDbNewDefaultDbLocation)
        {
        index = iEventQueue.Find(aEvent);
        }

    iUriArray->AppendL(aUri);
    TInt err = iEventQueue.Append(aEvent);

    if (err != KErrNone)
        {
        iUriArray->Delete(iUriArray->Count() - 1);
        User::Leave(err);
        }
    else if (index != KErrNotFound)
        {
        iEventQueue.Remove(index);
        iUriArray->Delete(index);
        }
    }

// End of File
