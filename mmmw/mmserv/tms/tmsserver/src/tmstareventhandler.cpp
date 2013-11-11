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
#include "tmstareventhandler.h"
#include "tmsclientserver.h"
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSTarEventHandler::TMSTarEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSTarEventHandler::TMSTarEventHandler(TMSServer* aServer) :
    CActive(EPriorityStandard),
    iTMSSer(aServer)
    {
    }

// -----------------------------------------------------------------------------
// TMSTarEventHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSTarEventHandler::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    User::LeaveIfError(iProperty.Attach(KTMSPropertyCategory, ERoutingPs));
    iStatus = KRequestPending;
    iProperty.Subscribe(iStatus);
    SetActive();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSTarEventHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSTarEventHandler* TMSTarEventHandler::NewL(TMSServer* aServer)
    {
    TMSTarEventHandler* self = new (ELeave) TMSTarEventHandler(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
TMSTarEventHandler::~TMSTarEventHandler()
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
// TMSTarEventHandler::DoCancel
// -----------------------------------------------------------------------------
//
void TMSTarEventHandler::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iProperty.Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSTarEventHandler::RunL
// -----------------------------------------------------------------------------
//
void TMSTarEventHandler::RunL()
    {
    TRACE_PRN_FN_ENT;
    // Subscribe immediately before analyzing the notification to ensure that we
    // don't miss further updates.
    if (iStatus.Int() == TMS_RESULT_SUCCESS)
        {
        iStatus = KRequestPending;
        iProperty.Subscribe(iStatus);
        SetActive();
        TRoutingMsgBufPckg routingpckg;
        iProperty.Get(routingpckg);
        iTMSSer->NotifyTarClients(routingpckg);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSTarEventHandler::RunError
// -----------------------------------------------------------------------------
//
TInt TMSTarEventHandler::RunError(TInt aError)
    {
    return aError;
    }

