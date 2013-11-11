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
* Description: Handles landmark settings events.
*
*
*/


// INCLUDE FILES
#include <EPos_Landmarks.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include <epos_cposlmsettingshandler.h>
#include "EPos_CPosLmServerSettingsHandler.h"
#include "EPos_CPosLmServer.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::CPosLmServerSettingsHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmServerSettingsHandler::CPosLmServerSettingsHandler(
    CPosLmServer& aServer)
    : CActive(EPriorityLow), iLmServer(&aServer)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CPosLmServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmServerSettingsHandler::ConstructL()
    {
    iSettingsHandler = CPosLmSettingsHandler::NewL();
    }

// Two-phased constructor
CPosLmServerSettingsHandler* CPosLmServerSettingsHandler::NewL(
    CPosLmServer& aServer)
    {
    CPosLmServerSettingsHandler* self =
        new (ELeave) CPosLmServerSettingsHandler(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmServerSettingsHandler::~CPosLmServerSettingsHandler()
    {
    Cancel();
    delete iSettingsHandler;
    }

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::StartListening
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmServerSettingsHandler::StartListening()
    {
    TInt err = iSettingsHandler->NotifySettingsChanged(iStatus);
    if (err == KErrNone)
        {
        SetActive();
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerSettingsHandler::RunL()
    {
    User::LeaveIfError(StartListening());
    HBufC* uri = iSettingsHandler->GetDefaultDatabaseUriL();
    CleanupStack::PushL(uri);
    HandleLandmarkSettingsChangedL(*uri);
    CleanupStack::PopAndDestroy(uri);
    }

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerSettingsHandler::DoCancel()
    {
    iSettingsHandler->CancelNotifySettingsChanged();
    }

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::RunError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmServerSettingsHandler::RunError(TInt aError)
    {
    iLmServer->CompleteOutstandingEventRequests(aError);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmServerSettingsHandler::HandleLandmarkSettingsChangedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerSettingsHandler::HandleLandmarkSettingsChangedL(
    const TDesC& aUri)
    {
    // To remove a warning, the event must be default constructed.
    TPosLmDatabaseEvent event =
            {
            EPosLmDbNewDefaultDbLocation,
            {0,0,0,0,0,0,0,0}
            };

    iLmServer->ForwardEventToAllSessionsL(event, aUri);

    // To remove a warning, the event must be default constructed.
    TPosLmEvent lmEvent =
            {
            EPosLmEventNewDefaultDatabaseLocation,
            KPosLmNullItemId,
            {0,0,0,0,0,0,0,0}
            };

    iLmServer->ForwardEventToAllSessionsL(lmEvent, aUri, EFalse);
    }

// End of File
