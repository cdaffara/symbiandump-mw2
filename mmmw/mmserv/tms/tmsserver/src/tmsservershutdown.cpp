/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

// INCLUDE FILES
#include "tmsservershutdown.h"

using namespace TMS;

// CONSTANTS
_LIT(KDelayServerShutdownPanic, "TMSServerShutDown");

// -----------------------------------------------------------------------------
// TMSServerShutDown::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSServerShutDown* TMSServerShutDown::NewL()
    {
    TMSServerShutDown* self = new (ELeave) TMSServerShutDown;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::TMSServerShutDown()
// C++ constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TMSServerShutDown::TMSServerShutDown() :
    CActive(0)
    {
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSServerShutDown::ConstructL()
    {
    User::LeaveIfError(iShutDownTimer.CreateLocal());
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::~TMSServerShutDown
// Destructor
// -----------------------------------------------------------------------------
//
TMSServerShutDown::~TMSServerShutDown()
    {
    Cancel();
    iShutDownTimer.Close();
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::SetDelay
// Request a timeout after aDelay
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSServerShutDown::SetDelay(TTimeIntervalMicroSeconds32 aDelay)
    {
    __ASSERT_ALWAYS(!IsActive(), User::Panic(KDelayServerShutdownPanic, 1));

    if (!IsActive())
        {
        iStatus = KRequestPending;
        iShutDownTimer.After(iStatus, aDelay);
        }

    SetActive();
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::RunL
// Called by Active object framework when timer times out.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSServerShutDown::RunL()
    {
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// TMSServerShutDown::DoCancel
// Called by the Active object framework when user cancels active object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSServerShutDown::DoCancel()
    {
    iShutDownTimer.Cancel();
    }

//  End of File
