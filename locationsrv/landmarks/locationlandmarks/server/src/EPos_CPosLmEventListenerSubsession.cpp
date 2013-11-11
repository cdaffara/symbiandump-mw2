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


#include "EPos_CPosLmEventListenerSubsession.h"
#include "EPos_CPosLmDbManEventHandler.h"
#include "EPos_CPosLmServer.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"

// -----------------------------------------------------------------------------
// CPosLmEventListenerSubsession::CPosLmEventListenerSubsession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmEventListenerSubsession::CPosLmEventListenerSubsession(
    CPosLmServer* aLmServer) :
    CPosLmSubsessionBase(EPosEventListenerSubsession, aLmServer)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmEventListenerSubsession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmEventListenerSubsession::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmEventListenerSubsession::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmEventListenerSubsession* CPosLmEventListenerSubsession::NewLC(
    CPosLmServer* aLmServer)
    {
    CPosLmEventListenerSubsession* self =
        new (ELeave) CPosLmEventListenerSubsession(aLmServer);
    CleanupClosePushL(*self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmEventListenerSubsession::~CPosLmEventListenerSubsession()
    {
    delete iDbManEventHandler;
    }

// -----------------------------------------------------------------------------
// CPosLmEventListenerSubsession::ServiceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventListenerSubsession::ServiceL(
    const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case EPosLmServerListenForDbEvent:
            if (!iDbManEventHandler)
                {
                iDbManEventHandler = CPosLmDbManEventHandler::NewL();
                }
            User::LeaveIfError(Server()->StartSettingsListenerIfNeeded());
            iDbManEventHandler->HandleRequestForEventL(aMessage);
            break;
        case EPosLmServerCancelListenForDbEvent:
            delete iDbManEventHandler;
            iDbManEventHandler = NULL;
            LmServerGlobal::Complete(aMessage, KErrNone);
            break;
        case EPosLmServerUriFromLastEvent:
            if (!iDbManEventHandler)
                {
                LmServerGlobal::Complete(aMessage, KErrNotFound);
                }
            else
                {
                iDbManEventHandler->ServerUriFromLastEvent(aMessage);
                }
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmEventListenerSubsession::HandleOperationEventL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventListenerSubsession::HandleOperationEventL(
    TPosLmDatabaseEvent aEvent,
    const TDesC& aUri)
    {
    if (iDbManEventHandler)
        {
        iDbManEventHandler->HandleIncomingEventL(aEvent, aUri);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmServerSession::CompleteOutstandingEventRequest
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmEventListenerSubsession::CompleteOutstandingEventRequest(
    TInt aError)
    {
    if (iDbManEventHandler)
        {
        iDbManEventHandler->CompleteAnyOutStandingRequest(aError);
        }
    }

// End of File
