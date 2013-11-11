/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  JitterBuffer component capable to audioframe buffering.
 *
 */

// INCLUDE FILES
#include <e32base.h>
#include <mmcccodecinformation.h> //FourCC codes
#include "debugtracemacros.h"
#include "InternalDef.h"
#include "JitterBufferObserver.h"
#include "JitterBufferImpl.h"
#include "JitterBuffer.h"

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::CVoIPJitterBuffer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVoIPJitterBuffer::CVoIPJitterBuffer(MJitterBufferObserver* aObserver) :
    CActive(EPriorityNormal)
    {
    iObserver = aObserver;
    iState = EJBufStopped;
    iTmCurrentEmptyBuffer = TTime(0);
    iTmPreviousEmptyBuffer = TTime(0);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CVoIPJitterBuffer* CVoIPJitterBuffer::NewL(
        MJitterBufferObserver* aObserver)
    {
    CVoIPJitterBuffer* self = new (ELeave) CVoIPJitterBuffer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::ConstructL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
void CVoIPJitterBuffer::ConstructL()
    {
    CActiveScheduler::Add(this);
    iTimer.CreateLocal();
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::~CVoIPJitterBuffer
// Destructor deallocate memory.
// -----------------------------------------------------------------------------
//
CVoIPJitterBuffer::~CVoIPJitterBuffer()
    {
    TRACE_PRN_FN_ENT;

    if (IsActive())
        {
        Cancel();
        }

    delete iJitterBufferImpl;
    iTimer.Close();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::SetupL
// Setup Jitterbuffer
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::SetupL(const TFourCC aCodec,
        const TVoIPJBConfig& aJBConfig)
    {
    TRACE_PRN_FN_ENT;

    iCodec = aCodec;

    if (!iJitterBufferImpl)
        {
        iJitterBufferImpl = CVoIPJitterBufferImpl::NewL(iObserver);
        }

    if (iJitterBufferImpl)
        {
        iJitterBufferImpl->SetupL(aCodec, aJBConfig);
        iSampleInterval = aJBConfig.iSampleInterval *
                KJBMillisecondsToMicroseconds;

        // Cannot use with G711 as it asks buffers in uncontrolled manner
        // TODO: change the logic so that it inspects what happens in long
        // run and if it fails, send error and pause itself.
        if (iCodec != KMccFourCCIdG711)
            {
            // Not too strict with delaying, threshold cannot be bigger than
            // frame time
            iEmptyBufferDelayThreshold = iSampleInterval / KJBEmptyBufferDelay;
            }

        TRACE_PRN_N1(_L("JB-> DelayThreshold [%d]"),iEmptyBufferDelayThreshold);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::ResetBuffer
// Reset Jitterbuffer
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::ResetBuffer(TBool aPlayTone)
    {
    TRACE_PRN_FN_ENT;

    if (iJitterBufferImpl)
        {
        iJitterBufferImpl->ResetBuffer(aPlayTone);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::DelayDownL
// Delay Down
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::DelayDown()
    {
    if (iJitterBufferImpl)
        {
        TRAPD(err, iJitterBufferImpl->DelayDownL());
        if (err != KErrNone)
            {
            TRACE_PRN_IF_ERR(err);
            iObserver->EventJB(MJitterBufferObserver::EGeneralError, err);
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::DelayUpL
// Delay Up
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::DelayUp()
    {
    if (iJitterBufferImpl)
        {
        TRAPD(err, iJitterBufferImpl->DelayUpL());
        if (err != KErrNone)
            {
            TRACE_PRN_IF_ERR(err);
            iObserver->EventJB(MJitterBufferObserver::EGeneralError, err);
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::Play
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::Play()
    {
    TRACE_PRN_FN_ENT;

    iState = EJBufPlaying;
    iRequestSize = 0;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::Pause
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::Pause()
    {
    iState = EJBufPaused;
    ResetBuffer();
    Cancel();
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::Stop
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CVoIPJitterBuffer::Stop()
    {
    iState = EJBufStopped;
    ResetBuffer();
    Cancel();
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::FillBufferL
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBuffer::FillBuffer(CMMFBuffer* aBuffer)
    {
    TInt err = KErrNone;

    if (iJitterBufferImpl && iJitterBufferImpl->BufferLength())
        {
        TRACE_PRN_N(_L("CVoIPJitterBuffer::FillBufferL NORMAL"));

        TUint32 emptyBufferDelay = 0;

        if (iCodec != KMccFourCCIdG729)
            {
            // This can cause JB overflow on G729 (it does in the loopback)
            DetermineEmptyBufferDelay();
            }

        iPlayBuffer = aBuffer;
        iJitterBufferImpl->GetDataFrame(iPlayBuffer);
        TransitionState(EEmptyData, emptyBufferDelay);
        }
    else
        {
        err = KErrNotReady;
        TRACE_PRN_IF_ERR(err);
        iObserver->EventJB(MJitterBufferObserver::EGeneralError, err);
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::EmptyBuffer
// Buffer with data comes in and is passed to JB queue.
//
// It was callback based; now DNL-THR->BufferFilled will call it directly
// upon data receiving. The supplier is now the DNL thread.
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBuffer::EmptyBuffer(CMMFBuffer* aBuffer)
    {
    TInt err = KErrNotReady;

    if (iJitterBufferImpl && (iState == EJBufPlaying))
        {
        if (iJitterBufferImpl->BufferLength() == 0)
            {
            return err;
            }

        //TRACE_PRN_N1(_L("JB-> BUF Size: [%d]"), aBuffer->BufferSize());
        //TRACE_PRN_N1(_L("JB-> REQ Size: [%d]"), aBuffer->RequestSize());

        // Adaptation control will be done based on played frames
        err = iJitterBufferImpl->AddDataFrame(aBuffer);
        }

    iObserver->EventJB(MJitterBufferObserver::EBufferConsumed);

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::PlayBuffer
//
// -----------------------------------------------------------------------------
//
void CVoIPJitterBuffer::PlayBuffer()
    {
    // iDataSink has valid data - send for playback
    iObserver->EventJB(MJitterBufferObserver::EBufferReadyToPlay);

    // Go to wait state
    //TransitionState(EWait);
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CVoIPJitterBuffer::DoCancel()
    {
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CVoIPJitterBuffer::RunL()
    {
    TRACE_PRN_FN_ENT;

    switch (iState)
        {
        case EJBufStopped:
            break;
        case EJBufPaused:
            break;
        case EJBufPlaying:
            {
            switch (iTransitionState)
                {
                case EWait:
                    TRACE_PRN_N(_L("CVoIPJitterBuffer::RunL EWait"));
                    break;
                case EEmptyData:
                    TRACE_PRN_N(_L("CVoIPJitterBuffer::RunL EEmptyData"));
                    PlayBuffer();
                    break;
                default:
                    User::Leave(KErrArgument);
                    break;
                }
            break;
            }
        default:
            User::Leave(KErrArgument);
            break;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::RunError
//
// -----------------------------------------------------------------------------
//
TInt CVoIPJitterBuffer::RunError(TInt aError)
    {
    iObserver->EventJB(MJitterBufferObserver::EGeneralError, aError);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::TransitionState
//
// -----------------------------------------------------------------------------
//
void CVoIPJitterBuffer::TransitionState(TJBTransitionState aTransitionState,
        TUint32 aStateChangeDelay)
    {
    TRequestStatus* stat = &iStatus;

    iTransitionState = aTransitionState;
    Cancel();

    if (aStateChangeDelay)
        {
        iStatus = KRequestPending;
        iTimer.After(iStatus, TTimeIntervalMicroSeconds32(aStateChangeDelay));
        }
    else
        {
        User::RequestComplete(stat, KErrNone);
        }

    SetActive();
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBuffer::DetermineEmptyBufferDelay
// -----------------------------------------------------------------------------
//
TUint32 CVoIPJitterBuffer::DetermineEmptyBufferDelay()
    {
    TUint32 emptyBufferDelay(0);
    iTmCurrentEmptyBuffer.HomeTime();

    if (iTmPreviousEmptyBuffer.Int64())
        {
        TTimeIntervalMicroSeconds difference =
                iTmCurrentEmptyBuffer.MicroSecondsFrom(iTmPreviousEmptyBuffer);

        if (difference.Int64() < iEmptyBufferDelayThreshold)
            {
            emptyBufferDelay = iSampleInterval - difference.Int64();
            }
        }

    iTmPreviousEmptyBuffer = iTmCurrentEmptyBuffer;
    return emptyBufferDelay;
    }

//  End of File
