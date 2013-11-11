/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Implements VoIP delayed server shutdown routines.
 *
 */

// INCLUDE FILES
#include "VoIPServerShutdown.h"

// CONSTANTS
_LIT(KDelayServerShutdownPanic, "CDelayVoIPServerShutDown");

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDelayVoIPServerShutDown* CDelayVoIPServerShutDown::NewL()
    {
    CDelayVoIPServerShutDown* self = new (ELeave) CDelayVoIPServerShutDown;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::CDelayVoIPServerShutDown()
// C++ constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CDelayVoIPServerShutDown::CDelayVoIPServerShutDown() :
    CActive(0)
    {
    }

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDelayVoIPServerShutDown::ConstructL()
    {
    User::LeaveIfError(iShutDownTimer.CreateLocal());
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::~CDelayVoIPServerShutDown
// Destructor
// -----------------------------------------------------------------------------
//
CDelayVoIPServerShutDown::~CDelayVoIPServerShutDown()
    {
    Cancel();
    iShutDownTimer.Close();
    }

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::SetDelay
// Request a timeout after aDelay
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CDelayVoIPServerShutDown::SetDelay(TTimeIntervalMicroSeconds32 aDelay)
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
// CDelayVoIPServerShutDown::RunL
// Called by Active object framework when timer times out.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CDelayVoIPServerShutDown::RunL()
    {
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CDelayVoIPServerShutDown::DoCancel
// Called by the Active object framework when user cancels active object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CDelayVoIPServerShutDown::DoCancel()
    {
    iShutDownTimer.Cancel();
    }

//  End of File
