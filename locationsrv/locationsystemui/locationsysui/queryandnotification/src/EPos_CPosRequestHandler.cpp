/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for privacy notifier request handling helper.
*
*/



#include <EPos_CPosPrivacyNotifier.h>
#include "EPos_CPosRequestHandler.h"
#include "EPos_CPosPrivacyNotifierExtension.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosRequestHandler::CPosRequestHandler() :
    CActive(CActive::EPriorityHigh)
    {
    }

// EPOC default constructor can leave.
void CPosRequestHandler::ConstructL(
    CPosPrivacyNotifier* aNotifier,
    CPosPrivacyNotifierExtension* aExtension)
    {
    iPrivacyNotifier = aNotifier;
    iExtension = aExtension;
    CActiveScheduler::Add(this);
    }

// Two-phased constructor.
CPosRequestHandler* CPosRequestHandler::NewL(
    CPosPrivacyNotifier* aNotifier,
    CPosPrivacyNotifierExtension* aExtension)
    {
    CPosRequestHandler* self =
        new(ELeave) CPosRequestHandler;
    CleanupStack::PushL(self);
    self->ConstructL(aNotifier, aExtension);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosRequestHandler::~CPosRequestHandler()
    {
    Cancel();
    }

// ---------------------------------------------------------
// CPosRequestHandler::ScheduleRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosRequestHandler::ScheduleRequest(const TPosQNInputData& aInputData)
    {
    iInputData = aInputData;

    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    }

// ---------------------------------------------------------
// CPosRequestHandler::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosRequestHandler::RunL()
    {
    iPrivacyNotifier->HandleNewRequestL(iInputData.iId);
    }

// ---------------------------------------------------------
// CPosRequestHandler::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosRequestHandler::DoCancel()
    {
    }

// ---------------------------------------------------------
// CPosRequestHandler::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosRequestHandler::RunError(TInt aError)
    {
    iExtension->CompleteRequest(iInputData.iId, aError);
    return KErrNone;
    }

//  End of File
