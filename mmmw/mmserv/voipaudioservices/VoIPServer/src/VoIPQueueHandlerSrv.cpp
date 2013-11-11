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
 * Description:  Implements message queue handler for handling messages
 *                passed between VoIPAudioServices threads.
 *
 */

#include <voipaudiocommon.h>
#include "VoIPSharedData.h"
#include "VoIPQueueHandlerSrv.h"

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::NewL
// Symbian constructor
// ----------------------------------------------------------------------------
//
CQueueHandlerSrv* CQueueHandlerSrv::NewL(MQueueHandlerObserverSrv* aObserver,
        RMsgQueue<TVoIPMsgBuf>* aMsgQueue)
    {
    CQueueHandlerSrv* self = new (ELeave) CQueueHandlerSrv(aMsgQueue);
    CleanupStack::PushL(self);
    self->ConstructL(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::ConstructL
// Second phase constructor.
// ----------------------------------------------------------------------------
//
void CQueueHandlerSrv::ConstructL(MQueueHandlerObserverSrv* aObserver)
    {
    iObserver = aObserver;
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::~CQueueHandlerSrv
// Destructor.
// ----------------------------------------------------------------------------
//
CQueueHandlerSrv::~CQueueHandlerSrv()
    {
    Cancel();
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::~CQueueHandlerSrv
// Destructor.
// ----------------------------------------------------------------------------
//
CQueueHandlerSrv::CQueueHandlerSrv(RMsgQueue<TVoIPMsgBuf>* aMsgQueue) :
    CActive(CActive::EPriorityStandard),
    iMsgQueue(aMsgQueue)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::Start
// Start listening for events on queue 0.
// ----------------------------------------------------------------------------
//
void CQueueHandlerSrv::Start()
    {
    if (!IsActive())
        {
        iStatus = KRequestPending;
        iMsgQueue->NotifyDataAvailable(iStatus);
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::DoCancel
// Cancel outstanding request
// ----------------------------------------------------------------------------
//
void CQueueHandlerSrv::DoCancel()
    {
    iMsgQueue->CancelDataAvailable();
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::RunL
// Process requests.
// ----------------------------------------------------------------------------
//
void CQueueHandlerSrv::RunL()
    {
    TVoIPMsgBuf msgBuf;
    TInt err = iMsgQueue->Receive(msgBuf);

    if (err == KErrNone)
        {
        iObserver->Event(msgBuf.iRequest, msgBuf.iStatus);
        }

    Start();
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::RunError
// Process requests.
// ----------------------------------------------------------------------------
//
TInt CQueueHandlerSrv::RunError(TInt aError)
    {
    // RunL can actually leave if any iObserver->Event leave is not trapped
    // iObserver->Event(ECmdGeneralError, aError);
    return aError;
    }

// ----------------------------------------------------------------------------
// CQueueHandlerSrv::Status
// Return request status.
// ----------------------------------------------------------------------------
//
TRequestStatus* CQueueHandlerSrv::Status()
    {
    return &iStatus;
    }

// End of File
