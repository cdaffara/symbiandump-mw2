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

// -----------------------------------------------------------------------------
// TMSQueueHandler::NewL
// Symbian constructor
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// TMSQueueHandler::ConstructL
// Second phase constructor.
// -----------------------------------------------------------------------------
//
void TMSQueueHandler::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::~TMSQueueHandler
// Destructor.
// -----------------------------------------------------------------------------
//
TMSQueueHandler::~TMSQueueHandler()
    {
    Cancel();
    iObserversList.Reset();
    iClientList.Reset();
    iChunk.Close();
    delete iBuffer;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::TMSQueueHandler
// Constructor.
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// TMSQueueHandler::Start
// Start listening for events on queue 0.
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// TMSQueueHandler::AddObserver
//
// -----------------------------------------------------------------------------
//
gint TMSQueueHandler::AddObserver(MQueueHandlerObserver& aObserver,
        gint aClientId)
    {
    // Add to list if observer is not already added
    gint status = iObserversList.Find(&aObserver);
    if (status == KErrNotFound)
        {
        status = iObserversList.Append(&aObserver);
        status = iClientList.Append(aClientId);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::RemoveObserver
// Marks observer as inactive in the list
// -----------------------------------------------------------------------------
//
gint TMSQueueHandler::RemoveObserver(MQueueHandlerObserver& aObserver)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iObserversList.Find(&aObserver);
    // If found status has index to observer in the array
    // else it would contain KErrNotFound
    if (index >= 0)
        {
        iObserversList.Remove(index);
        iClientList.Remove(index);
        status = TMS_RESULT_SUCCESS;
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::DoCancel
// Cancel outstanding request
// -----------------------------------------------------------------------------
//
void TMSQueueHandler::DoCancel()
    {
    if (iMsgQueue)
        {
        iMsgQueue->CancelDataAvailable();
        }
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::RunL
// Process requests.
// -----------------------------------------------------------------------------
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

    if (err == TMS_RESULT_SUCCESS)
        {
        switch (msgBuf.iRequest)
            {
            case ECmdDownlinkInitComplete:
            case ECmdUplinkInitComplete:
            case ECmdDownlinkDeInitComplete:
            case ECmdUplinkDeInitComplete:
            case ECmdDownlinkStarted:
            case ECmdUplinkStarted:
            case ECmdDownlinkPaused:
            case ECmdUplinkPaused:
                {
                gint index = FindStreamInList();
                if (index != KErrNotFound)
                    {
                    TMSStreamState streamstate = ConvertToStreamState(
                            msgBuf.iRequest);
                    iObserversList[index]->QueueEvent(streamstate,
                            msgBuf.iStatus, NULL);
                    }
                else
                    {
                    // This should never happen
                    err = TMS_RESULT_DOES_NOT_EXIST;
                    }
                break;
                }
            case ECmdFillBuffer:
                {
                DoFillBuffer(msgBuf.iInt, msgBuf.iStatus, msgBuf.iBool,
                        msgBuf.iUint32);
                break;
                }
            case ECmdEmptyBuffer:
                {
                DoEmptyBuffer(msgBuf.iInt, msgBuf.iStatus, msgBuf.iBool,
                        msgBuf.iUint32);
                break;
                }
            case ECmdDownlinkClosed:
                {
                iChunk.Close();
                break;
                }
            case ECmdUplinkClosed:
                {
                iChunk.Close();
                break;
                }
            case ECmdDnLinkError:
                {
                break;
                }
            case ECmdUpLinkError:
                {
                break;
                }
            case ECmdSetGain:
                {
                gint index = FindGainEffectInList();
                if (index != KErrNotFound)
                    {
                    iObserversList[index]->QueueEvent(
                            TMS_EVENT_EFFECT_GAIN_CHANGED, msgBuf.iStatus,
                            NULL);
                    }
                }
                break;
            case ECmdSetVolume:
                {
                gint index = FindVolEffectInList();
                if (index != KErrNotFound)
                    {
                    iObserversList[index]->QueueEvent(
                            TMS_EVENT_EFFECT_VOL_CHANGED, msgBuf.iStatus,
                            NULL);
                    }
                }
                break;
            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::DoFillBuffer
//
// -----------------------------------------------------------------------------
//
void TMSQueueHandler::DoFillBuffer(gint aBufLen, gint aStatus,
        gboolean aOpenChunk, guint32 key)
    {
    gint err = TMS_RESULT_SUCCESS;

    // See if chunk needs to be opened
    if (aOpenChunk)
        {
        if (iChunk.Handle())
            {
            iChunk.Close();
            }
        delete iBuffer;
        iBuffer = NULL;

        // Get handle to chunk from proxy
        gint hndl(0);
        err = TMS_RESULT_NULL_ARGUMENT;
        if (iTMSGlobalContext->CallProxy)
            {
            hndl = (iTMSGlobalContext->CallProxy)->GetDataXferChunkHandle(
                    iTMSGlobalContext->CallType,
                    iTMSGlobalContext->StreamType,
                    iTMSGlobalContext->StreamId, key);
            err = iChunk.SetReturnedHandle(hndl);
            }
        }

    if (err == TMS_RESULT_SUCCESS)
        {
        iChunkDataPtr.Set(iChunk.Base(), 0, aBufLen);
        if (!iBuffer)
            {
            TMSMemBuffer::Create((guint) aBufLen,
                    const_cast<guint8*>(iChunkDataPtr.Ptr()), iBuffer);
            }
        iBuffer->SetDataSize(aBufLen);

        gint index = iClientList.Find(TMS_SOURCE_CLIENT);
        if (index != KErrNotFound)
            {
            iObserversList[index]->QueueEvent(TMS_EVENT_SOURCE_FILL_BUFFER,
                    aStatus, iBuffer);
            }
        else
            {
            err = TMS_RESULT_DOES_NOT_EXIST;
            }
        }

    // TODO: handle error here
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::DoEmptyBuffer
//
// -----------------------------------------------------------------------------
//
void TMSQueueHandler::DoEmptyBuffer(gint aBufLen, gint aStatus,
        gboolean aOpenChunk, guint32 key)
    {
    gint err(TMS_RESULT_SUCCESS);

    // See if chunk needs to be opened
    if (aOpenChunk)
        {
        if (iChunk.Handle())
            {
            iChunk.Close();
            }

        // Get handle to chunk from proxy
        gint hndl(0);
        err = TMS_RESULT_NULL_ARGUMENT;
        if (iTMSGlobalContext->CallProxy)
            {
            hndl = (iTMSGlobalContext->CallProxy)->GetDataXferChunkHandle(
                    iTMSGlobalContext->CallType,
                    iTMSGlobalContext->StreamType,
                    iTMSGlobalContext->StreamId, key);
            err = iChunk.SetReturnedHandle(hndl);
            }
        // TODO handle error here
        delete iBuffer;
        iBuffer = NULL;
        }

    if (err == TMS_RESULT_SUCCESS)
        {
        iChunkDataPtr.Set(iChunk.Base(), aBufLen, aBufLen);
        if (!iBuffer)
            {
            TMSMemBuffer::Create((guint) aBufLen,
                    const_cast<guint8*>(iChunkDataPtr.Ptr()), iBuffer);
            }
        iBuffer->SetDataSize(aBufLen);
        gint index = iClientList.Find(TMS_SINK_CLIENT);
        if (index != KErrNotFound)
            {
            iObserversList[index]->QueueEvent(TMS_EVENT_SINK_PROCESS_BUFFER,
                    aStatus, iBuffer);
            }
        else
            {
            err = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
	//TODO: Handle error here
    //iObserver->Event(MVoIPUplinkObserver::KUplinkError, aStatus);
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::ConvertToStreamState
//
// -----------------------------------------------------------------------------
//
TMSStreamState TMSQueueHandler::ConvertToStreamState(gint request)
    {
    TMSStreamState state = TMS_STREAM_UNINITIALIZED;
    switch (request)
        {
        case ECmdDownlinkInitComplete:
        case ECmdUplinkInitComplete:
            state = TMS_STREAM_INITIALIZED;
            break;
        case ECmdDownlinkStarted:
        case ECmdUplinkStarted:
            state = TMS_STREAM_STARTED;
            break;
        case ECmdDownlinkPaused:
        case ECmdUplinkPaused:
            state = TMS_STREAM_PAUSED;
            break;
        case ECmdDownlinkDeInitComplete:
        case ECmdUplinkDeInitComplete:
            state = TMS_STREAM_UNINITIALIZED;
            break;
        default:
            break;
        }
    return state;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::RunError
// Process requests.
// -----------------------------------------------------------------------------
//
TInt TMSQueueHandler::RunError(TInt /*aError*/)
    {
    // Current implementation of RunL does not leave
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::Status
// Return request status.
// -----------------------------------------------------------------------------
//
TRequestStatus* TMSQueueHandler::Status()
    {
    return &iStatus;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::FindStreamInList
// Return stream index.
// -----------------------------------------------------------------------------
//
gint TMSQueueHandler::FindStreamInList()
    {
    gint index(-1);
    index = iClientList.Find(TMS_STREAM_UPLINK);
    if (index == KErrNotFound)
        {
        index = iClientList.Find(TMS_STREAM_DOWNLINK);
        }
    return index;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::FindGainEffectInList
// Return effect index.
// -----------------------------------------------------------------------------
//
gint TMSQueueHandler::FindGainEffectInList()
    {
    gint index(-1);
    index = iClientList.Find(TMS_EFFECT_GAIN);
    return index;
    }

// -----------------------------------------------------------------------------
// TMSQueueHandler::FindVolEffectInList
// Return effect index.
// -----------------------------------------------------------------------------
//
gint TMSQueueHandler::FindVolEffectInList()
    {
    gint index(-1);
    index = iClientList.Find(TMS_EFFECT_VOLUME);
    return index;
    }

// End of File
