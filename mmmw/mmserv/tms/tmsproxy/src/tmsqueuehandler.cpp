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

#include <tms.h>
#include <tmsbuffer.h>
#include "tmsutility.h"
#include "tmsshared.h"
#include "tmsqueuehandler.h"
#include "tmsmembuffer.h"
#include "tmsglobalcontext.h"
#include "tmscallproxy.h"

using namespace TMS;

// ----------------------------------------------------------------------------
// TMSQueueHandler::NewL
// Symbian constructor
// ----------------------------------------------------------------------------
//
TMSQueueHandler* TMSQueueHandler::NewL(RMsgQueue<TmsMsgBuf>* aMsgQueue,
        TMSGlobalContext* glblCtx)
    {
    TMSQueueHandler* self = new (ELeave) TMSQueueHandler(aMsgQueue, glblCtx);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::ConstructL
// Second phase constructor.
// ----------------------------------------------------------------------------
//
void TMSQueueHandler::ConstructL()
    {
    iObserver = NULL;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::~TMSQueueHandler
// Destructor.
// ----------------------------------------------------------------------------
//
TMSQueueHandler::~TMSQueueHandler()
    {
    Cancel();
    if (iMsgQueue->Handle() > 0)
        {
        iMsgQueue->Close();
        }
    iObserversList.Reset();
    iClientList.Reset();
    iChunk.Close();
    delete iBuffer;
    iObserver = NULL;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::TMSQueueHandler
// Constructor.
// ----------------------------------------------------------------------------
//
TMSQueueHandler::TMSQueueHandler(RMsgQueue<TmsMsgBuf>* aMsgQueue,
        TMSGlobalContext* glblCtx) :
    CActive(CActive::EPriorityStandard),
    iMsgQueue(aMsgQueue),
    iChunkDataPtr(0, 0, 0)
    {
    CActiveScheduler::Add(this);
    iTMSGlobalContext = glblCtx;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::Start
// Start listening for events on queue 0.
// ----------------------------------------------------------------------------
//
void TMSQueueHandler::Start()
    {
    if (!IsActive() && iMsgQueue)
        {
        iStatus = KRequestPending;
        iMsgQueue->NotifyDataAvailable(iStatus);
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::AddObserver
//
// ----------------------------------------------------------------------------
//
gint TMSQueueHandler::AddObserver(MQueueHandlerObserver& aObserver,
        gint /*aClientId*/)
    {
    gint status = TMS_RESULT_SUCCESS;

    if (iObserver == NULL)
        {
        iObserver = &aObserver;
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return status;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::RemoveObserver
// Marks observer as inactive in the list
// ----------------------------------------------------------------------------
//
gint TMSQueueHandler::RemoveObserver(MQueueHandlerObserver& /*aObserver*/)
    {
    iObserver = NULL;
    return TMS_RESULT_SUCCESS;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::DoCancel
// Cancel outstanding request
// ----------------------------------------------------------------------------
//
void TMSQueueHandler::DoCancel()
    {
    if (iMsgQueue)
        {
        iMsgQueue->CancelDataAvailable();
        }
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::RunL
// Process requests.
// ----------------------------------------------------------------------------
//
void TMSQueueHandler::RunL()
    {
    TmsMsgBuf msgBuf;
    gint err = TMS_RESULT_SUCCESS;

    if (iMsgQueue)
        {
        iMsgQueue->Receive(msgBuf);
        }
    else
        {
        err = TMS_RESULT_INVALID_STATE;
        }

    // Start monitoring for more events before calling the observer in case
    // client decides to destroy us before this RunL completes executing.
    Start();

    if (iObserver == NULL)
        {
        return;
        }

    if (err == TMS_RESULT_SUCCESS)
        {
        switch (msgBuf.iRequest)
            {
            case ECmdGlobalEffectChange:
                TMSVolumeEventChangeData event;
                event.level = msgBuf.iUint;
                event.output = static_cast<TMSAudioOutput>(msgBuf.iInt2);
                event.output_changed = msgBuf.iBool;
                iObserver->QueueEvent(msgBuf.iInt, msgBuf.iStatus, &event);
                break;
            case ECmdGlobalRoutingChange:
                iObserver->QueueEvent(msgBuf.iInt, msgBuf.iStatus,
                        &msgBuf.iUint);
                break;
            case ECmdRingToneOpenComplete:
                iObserver->QueueEvent(TMS_EVENT_RINGTONE_OPEN_COMPLETE,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdRingTonePlayComplete:
                iObserver->QueueEvent(TMS_EVENT_RINGTONE_PLAY_COMPLETE,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdRingToneDeinitComplete:
                iObserver->QueueEvent(TMS_EVENT_RINGTONE_DEINIT_COMPLETE,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdDTMFToneDnlPlayStarted:
            case ECmdDTMFToneUplPlayStarted:
                iObserver->QueueEvent(TMS_EVENT_DTMF_TONE_STARTED,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdDTMFTonePlayFinished:
                iObserver->QueueEvent(TMS_EVENT_DTMF_TONE_STOPPED,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdInbandToneStarted:
                iObserver->QueueEvent(TMS_EVENT_INBAND_TONE_STARTED,
                        msgBuf.iStatus, NULL);
                break;
            case ECmdInbandToneStopped:
                iObserver->QueueEvent(TMS_EVENT_INBAND_TONE_STOPPED,
                        msgBuf.iStatus, NULL);
                break;
            default:
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::RunError
// Process requests.
// ----------------------------------------------------------------------------
//
TInt TMSQueueHandler::RunError(TInt /*aError*/)
    {
    // Current implementation of RunL does not leave
    return TMS_RESULT_SUCCESS;
    }

// ----------------------------------------------------------------------------
// TMSQueueHandler::Status
// Return request status.
// ----------------------------------------------------------------------------
//
TRequestStatus* TMSQueueHandler::Status()
    {
    return &iStatus;
    }

// End of File
