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
 * Description: VoIP Audio Services
 *
 */

// INCLUDE FILES
#include <ctsydomainpskeys.h>
#include "debugtracemacros.h"
#include "VoIPECallEventHandler.h"

// -----------------------------------------------------------------------------
// CVoIPECallEventHandler::CVoIPECallEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVoIPECallEventHandler::CVoIPECallEventHandler(CVoIPAudioServer* aServer) :
    CActive(EPriorityStandard),
    iVasServer(aServer)
    {
    }

// -----------------------------------------------------------------------------
// CVoIPECallEventHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVoIPECallEventHandler::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    User::LeaveIfError(iProperty.Attach(KPSUidCtsyEmergencyCallInfo,
            KCTSYEmergencyCallInfo));
    iStatus = KRequestPending;
    iProperty.Subscribe(iStatus);
    SetActive();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPECallEventHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVoIPECallEventHandler* CVoIPECallEventHandler::NewL(
        CVoIPAudioServer* aServer)
    {
    CVoIPECallEventHandler* self = new (ELeave) CVoIPECallEventHandler(
            aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CVoIPECallEventHandler::~CVoIPECallEventHandler()
    {
    TRACE_PRN_FN_ENT;
    if (IsActive())
        {
        Cancel();
        }
    iProperty.Close();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPECallEventHandler::DoCancel
// -----------------------------------------------------------------------------
//
void CVoIPECallEventHandler::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iProperty.Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPECallEventHandler::RunL
// -----------------------------------------------------------------------------
//
void CVoIPECallEventHandler::RunL()
    {
    TRACE_PRN_FN_ENT;
    // Subscribe immediately before analyzing the notification to ensure that we
    // don't miss further updates.
    if (iStatus.Int() == KErrNone)
        {
        iStatus = KRequestPending;
        iProperty.Subscribe(iStatus);
        SetActive();

        TInt eCall = 0;
        iProperty.Get(eCall);
        if (iVasServer)
            {
            iVasServer->NotifyECallEvent(eCall);
            }
        }
    TRACE_PRN_FN_EXT;
    }

//  End of File

