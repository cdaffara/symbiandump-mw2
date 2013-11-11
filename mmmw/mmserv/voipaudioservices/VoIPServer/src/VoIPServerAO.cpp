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
 * Description:  Implements active object used for ITC.
 *
 */

#include <e32base.h>
#include "VoIPServerAO.h"

// -----------------------------------------------------------------------------
// CVoIPServerAO::CVoIPServerAO
// Constructor
// -----------------------------------------------------------------------------
//
CVoIPServerAO::CVoIPServerAO(MThreadComObserver* aObserver, TThreadId aID) :
    CActive(CActive::EPriorityStandard),
    iObserver(aObserver),
    iID(aID)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CVoIPServerAO::DoCancel
// Cancels wait for completion of an outstanding request.
// -----------------------------------------------------------------------------
//
void CVoIPServerAO::DoCancel()
    {
    TRequestStatus* status = &iStatus;
    RThread t;
    TInt err = t.Open(iID);
    if (err == KErrNone)
        {
        t.RequestComplete(status, KErrCancel);
        t.Close();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPServerAO::Request
// Sests this A/O waiting for events.
// -----------------------------------------------------------------------------
//
void CVoIPServerAO::Request()
    {
    if (!IsActive())
        {
        iStatus = KRequestPending;
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPServerAO::RunL
// Handles A/O's request completion event.
// -----------------------------------------------------------------------------
//
void CVoIPServerAO::RunL()
    {
    iObserver->DoHandleCmd(EExcUserInterrupt, iStatus.Int());
    Request();
    }

// -----------------------------------------------------------------------------
// CVoIPServerAO::RunError
// Called by the active scheduler in case of RunL leave.
// -----------------------------------------------------------------------------
//
TInt CVoIPServerAO::RunError(TInt aError)
    {
    iObserver->DoHandleError(aError);
    return KErrNone;
    }

// End of file
