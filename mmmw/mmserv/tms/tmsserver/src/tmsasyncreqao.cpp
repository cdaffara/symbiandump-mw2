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

//  INCLUDE FILES
#include "tmsasyncreqao.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::TMSAsyncReqAO
// -----------------------------------------------------------------------------
//
TMSAsyncReqAO::TMSAsyncReqAO(TMSAsyncReqObsrv* aObserver) :
    CActive(CActive::EPriorityStandard),
    iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::NewL
// -----------------------------------------------------------------------------
//
TMSAsyncReqAO* TMSAsyncReqAO::NewL(TMSAsyncReqObsrv* aObserver)
    {
    return new (ELeave) TMSAsyncReqAO(aObserver);
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
TMSAsyncReqAO::~TMSAsyncReqAO()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::Start
// -----------------------------------------------------------------------------
//
void TMSAsyncReqAO::Start()
    {
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    }

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::DoCancel
// -----------------------------------------------------------------------------
//
void TMSAsyncReqAO::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::RunError
// -----------------------------------------------------------------------------
//
TInt TMSAsyncReqAO::RunError(TInt aError)
    {
    iObserver->HandleError(aError);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMSAsyncReqAO::RunL
// -----------------------------------------------------------------------------
//
void TMSAsyncReqAO::RunL()
    {
    iObserver->HandleAsyncReq();
    }

//  End of File
