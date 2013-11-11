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
 * Description:  VOIP Audio Services  CMsgQueueHandler class implementation.
 *
 */

#include <voipaudiocommon.h>
#include <voipuplinkstream.h>
#include <voipdownlinkstream.h>
#include <voipdtmftoneplayer.h>
#include <voipringtoneplayer.h>
#include "debugtracemacros.h"
#include "VoIPSharedData.h"
#include "VoIPDataBufferImpl.h"
#include "VoIPQueueHandler.h"

// ----------------------------------------------------------------------------
// CQueueHandler::NewL
// Symbian constructor
// ----------------------------------------------------------------------------
//
CQueueHandler* CQueueHandler::NewL(MQueueHandlerObserver* aObserver,
        RMsgQueue<TVoIPMsgBuf>* aMsgQueue, TInt aBufferLen)
    {
    CQueueHandler* self = new (ELeave) CQueueHandler(aMsgQueue);
    CleanupStack::PushL(self);
    self->ConstructL(aObserver, aBufferLen);
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CQueueHandler::ConstructL
// Second phase constructor.
// ----------------------------------------------------------------------------
//
void CQueueHandler::ConstructL(MQueueHandlerObserver* aObserver,
        TInt aBufferLen)
    {
    iObserver = aObserver;

    // aBufferLen == 0 for MsgCom queue
    if (aBufferLen > 0)
        {
        iBuffer = CVoIPDataBufferImpl::NewL(aBufferLen);
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandler::~CQueueHandler
// Destructor.
// ----------------------------------------------------------------------------
//
CQueueHandler::~CQueueHandler()
    {
    Cancel();
    iChunk.Close();
    delete iBuffer;
    }

// ----------------------------------------------------------------------------
// CQueueHandler::CQueueHandler
// Constructor.
// ----------------------------------------------------------------------------
//
CQueueHandler::CQueueHandler(RMsgQueue<TVoIPMsgBuf>* aMsgQueue) :
    CActive(CActive::EPriorityStandard),
    iMsgQueue(aMsgQueue),
    iChunkDataPtr(0, 0, 0)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CQueueHandler::Start
// Start listening for events on queue 0.
// ----------------------------------------------------------------------------
//
void CQueueHandler::Start()
    {
    if (!IsActive())
        {
        iStatus = KRequestPending;
        iMsgQueue->NotifyDataAvailable(iStatus);
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandler::DoCancel
// Cancel outstanding request
// ----------------------------------------------------------------------------
//
void CQueueHandler::DoCancel()
    {
    iMsgQueue->CancelDataAvailable();
    }

// ----------------------------------------------------------------------------
// CQueueHandler::RunL
// Process requests.
// ----------------------------------------------------------------------------
//
void CQueueHandler::RunL()
    {
    TVoIPMsgBuf msgBuf;
    TInt err = iMsgQueue->Receive(msgBuf);

    // Start monitoring for more events before calling the observer as client
    // may decide to destroy us before this RunL completes executing.
    Start();

    if (err == KErrNone)
        {
        switch (msgBuf.iRequest)
            {
            case ECmdDownlinkInitComplete:
                {
                iObserver->Event(MVoIPDownlinkObserver::KOpenComplete,
                        msgBuf.iStatus);
                break;
                }
            case ECmdUplinkInitComplete:
                {
                iObserver->Event(MVoIPUplinkObserver::KOpenComplete,
                        msgBuf.iStatus);
                break;
                }
            case ECmdFillBuffer:
                {
                DoFillBuffer(msgBuf.iInt, msgBuf.iStatus);
                break;
                }
            case ECmdEmptyBuffer:
                {
                DoEmptyBuffer(msgBuf.iInt, msgBuf.iStatus);
                break;
                }
            case ECmdDownlinkThreadClosed:
                {
                iChunk.Close();
                iObserver->Event(MVoIPDownlinkObserver::KDownlinkClosed,
                        msgBuf.iStatus);
                break;
                }
            case ECmdUplinkThreadClosed:
                {
                iChunk.Close();
                iObserver->Event(MVoIPUplinkObserver::KUplinkClosed,
                        msgBuf.iStatus);
                break;
                }
            case ECmdRingToneOpenComplete:
                {
                iObserver->Event(MRingToneObserver::KOpenComplete,
                        msgBuf.iStatus);
                break;
                }
            case ECmdRingTonePlayComplete:
                {
                iObserver->Event(MRingToneObserver::KPlaybackComplete,
                        msgBuf.iStatus);
                break;
                }
            case ECmdDnLinkError:
            case ECmdDnLinkJBError:
                {
                iObserver->Event(MVoIPDownlinkObserver::KDownlinkError,
                        msgBuf.iStatus);
                break;
                }
            case ECmdUpLinkError:
                {
                iObserver->Event(MVoIPUplinkObserver::KUplinkError,
                        msgBuf.iStatus);
                break;
                }
            case ECmdDTMFOpenDnlinkComplete:
                {
                iObserver->Event(MDTMFToneObserver::KOpenCompleteDNL,
                        msgBuf.iStatus);
                break;
                }
            case ECmdDTMFOpenUplinkComplete:
                {
                iObserver->Event(MDTMFToneObserver::KOpenCompleteUPL,
                        msgBuf.iStatus);
                break;
                }
            case ECmdDTMFTonePlayFinished:
                {
                TRACE_PRN_N1(_L("VoIP->TonePlayFinished [%d]"), msgBuf.iStatus);
                break;
                }
            default:
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandler::DoFillBuffer
//
// ----------------------------------------------------------------------------
//
void CQueueHandler::DoFillBuffer(TInt aBufLen, TInt aStatus)
    {
    TInt err = KErrNone;

    if (iChunk.Handle() == 0)
        {
        // aStatus is either RChunk handle or an error
        err = iChunk.OpenGlobal(KChunkDNL, EFalse);
        }

    if (err == KErrNone)
        {
        iChunkDataPtr.Set(iChunk.Base(), 0, aBufLen);
        iBuffer->SetPayloadPtr(iChunkDataPtr);
        iObserver->FillBuffer(iBuffer);
        }
    else
        {
        // returned error
        iObserver->Event(MVoIPDownlinkObserver::KDownlinkError, aStatus);
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandler::DoEmptyBuffer
//
// ----------------------------------------------------------------------------
//
void CQueueHandler::DoEmptyBuffer(TInt aBufLen, TInt aStatus)
    {
    TInt err = KErrNone;

    if (iChunk.Handle() == 0)
        {
        // aStatus is either RChunk handle or an error
        err = iChunk.OpenGlobal(KChunkUPL, EFalse);
        }

    if (err == KErrNone)
        {
        iChunkDataPtr.Set(iChunk.Base(), aBufLen, aBufLen);
        iBuffer->SetPayloadPtr(iChunkDataPtr);
        iObserver->EmptyBuffer(iBuffer);
        }
    else
        {
        iObserver->Event(MVoIPUplinkObserver::KUplinkError, aStatus);
        }
    }

// ----------------------------------------------------------------------------
// CQueueHandler::RunError
// Process requests.
// ----------------------------------------------------------------------------
//
TInt CQueueHandler::RunError(TInt /*aError*/)
    {
    // Current implementation of RunL does not leave
    return 0;
    }

// ----------------------------------------------------------------------------
// CQueueHandler::Status
// Return request status.
// ----------------------------------------------------------------------------
//
TRequestStatus* CQueueHandler::Status()
    {
    return &iStatus;
    }

// End of File
