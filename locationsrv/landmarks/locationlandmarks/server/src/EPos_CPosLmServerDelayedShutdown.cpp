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
* Description: Class to handle server delayed shutdown
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLmServerDelayedShutdown.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmServer::CPosLmServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmServerDelayedShutdown::CPosLmServerDelayedShutdown()
    : CActive(EPriorityLow)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CPosLmServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmServerDelayedShutdown::ConstructL()
    {
    User::LeaveIfError(iTimer.CreateLocal());
    }

// Two-phased constructor
CPosLmServerDelayedShutdown* CPosLmServerDelayedShutdown::NewL()
    {
    CPosLmServerDelayedShutdown* self =
        new (ELeave) CPosLmServerDelayedShutdown;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmServerDelayedShutdown::~CPosLmServerDelayedShutdown()
    {
    Cancel();
    iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmServerDelayedShutdown::Start
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerDelayedShutdown::Start(
    TTimeIntervalMicroSeconds32 aDelay)
    {
    iTimer.After(iStatus, aDelay);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPosLmServerDelayedShutdown::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerDelayedShutdown::RunL()
    {
    // Shutdown the server by shutting down the active scheduler.
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CPosLmServerDelayedShutdown::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmServerDelayedShutdown::DoCancel()
    {
    iTimer.Cancel();
    }

// End of File
