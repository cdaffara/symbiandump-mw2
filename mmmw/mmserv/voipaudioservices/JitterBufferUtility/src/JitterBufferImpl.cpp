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
 * Description:  Implementation of Mcc Jitterbuffer
 *
 */

// INCLUDE FILES
#include <e32base.h>
#include <mmcccodecinformation.h> //codec FourCC declarations
#include "debugtracemacros.h"
#include "CngGenerator.h"
#include "InternalDef.h"
#include "JitterBufferObserver.h"
#include "JitterBufferImpl.h"

// -----------------------------------------------------------------------------
// TJitterBufferElement::CompareSeqNum
// Compare SequenceNumber
// -----------------------------------------------------------------------------
//
TInt TJitterBufferElement::CompareSeqNum(const TJitterBufferElement& aElem1,
        const TJitterBufferElement& aElem2)
    {
    if (aElem1.iSequenceNumber > aElem2.iSequenceNumber)
        {
        return (-1);
        }
    else if (aElem1.iSequenceNumber < aElem2.iSequenceNumber)
        {
        return (1);
        }
    else
        {
        return (0);
        }
    }

// -----------------------------------------------------------------------------
// TJitterBufferElement::CompareStamp
// Compare Stamp
// -----------------------------------------------------------------------------
//
TInt TJitterBufferElement::CompareStamp(const TJitterBufferElement& aElem1,
        const TJitterBufferElement& aElem2)
    {
    if (aElem1.iTimeStamp > aElem2.iTimeStamp)
        {
        return (-1);
        }
    else if (aElem1.iTimeStamp < aElem2.iTimeStamp)
        {
        return (1);
        }
    else
        {
        return (0);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::CVoIPJitterBufferImpl
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CVoIPJitterBufferImpl::CVoIPJitterBufferImpl(MJitterBufferObserver* aObserver) :
    iBufStampSorter(TLinearOrder<TJitterBufferElement> (
            TJitterBufferElement::CompareStamp)),
    iBufSequenceSorter(TLinearOrder<TJitterBufferElement> (
            TJitterBufferElement::CompareSeqNum))
    {
    iObserver = aObserver;
    iTonePlayTime.UniversalTime();
    iSampleRate = KDefaultSampleRateInkHz;
    iPlay = EFalse;
    iLastPlayedSeqNum = -1;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CVoIPJitterBufferImpl* CVoIPJitterBufferImpl::NewL(
        MJitterBufferObserver* aObserver)
    {
    CVoIPJitterBufferImpl* self = new (ELeave) CVoIPJitterBufferImpl(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::~CVoIPJitterBufferImpl
// Destructor deallocate memory.
// -----------------------------------------------------------------------------
//
CVoIPJitterBufferImpl::~CVoIPJitterBufferImpl()
    {
    TRACE_PRN_FN_ENT;

    delete iCNGenerator;

    // Deallocate payload memory of jitter buffer elements.
    TInt count = iBuffer.Count();

    for (TInt i = 0; i < count; i++)
        {
        if (iBuffer[i].iDataFrame)
            {
            delete iBuffer[i].iDataFrame;
            }
        }

    iBuffer.Close();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::SetupL
// Setup Jitter buffer
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::SetupL(const TFourCC aCodec,
        const TVoIPJBConfig& aJBConfig)
    {
    TRACE_PRN_FN_ENT;

    __ASSERT_ALWAYS(aJBConfig.iJitterLatency, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aJBConfig.iJBBufferLength, User::Leave(KErrArgument));
    __ASSERT_ALWAYS(aJBConfig.iSampleInterval, User::Leave(KErrArgument));

    // Save the original HW frame time because we may need it in case of
    // dynamic G.711 adjustment.
    const TUint8 origHwtime = iJBConfig.iSampleInterval;
    iJBConfig = aJBConfig;

    if (iJBConfig.iJBInactivityTimeOut)
        {
        if ((iJBConfig.iJBPlayToneFrequency > 0) &&
                (iJBConfig.iJBPlayToneDuration > 0))
            {
            iPlayToneInterval = iJBConfig.iJBPlayToneTimeout;
            iPlay = ETrue;
            }
        }

    TInt bufLenMultiplier = 1;

    if (aCodec == KMMFFourCCCodeAMR)
        {
        iFrameSize = KAMRNBFrameSize;
        iSampleInterval = KAMRNBFrameTime;
        }
    else if (aCodec == KMccFourCCIdG711)
        {
        // G.711 is configured dynamically. Take voip headerlength also into
        // account. G.711 hwframetime is in milliseconds, so need to multiply.
        iFrameSize = (iJBConfig.iSampleInterval * KDefaultSampleRateInkHz) +
                KVoIPHeaderLength;
        iSampleInterval = 0;

        // In case of G.711 codec dynamic configuration, we may need to double
        // the jitter buffer length if HW frame time is changed from 20ms to
        // 10ms.
        if (origHwtime)
            {
            bufLenMultiplier = origHwtime / iJBConfig.iSampleInterval;

            if (!bufLenMultiplier)
                {
                bufLenMultiplier = 1;
                }
            }
        }
    else if (aCodec == KMccFourCCIdILBC)
        {
        iFrameSize = KILBCFrameSize;
        iSampleInterval = 0;
        }
    else if (aCodec == KMccFourCCIdG729)
        {
        iFrameSize = KG729FrameSize;
        iSampleInterval = 0;
        // Multiply G.729 also by two...
        bufLenMultiplier = 2;
        }
    else
        {
        TRACE_PRN_N(_L("JB-> Codec Not Supported"));
        User::Leave(KErrNotSupported);
        }

    // Delete and reset old buffer
    const TInt elems = iBuffer.Count();

    for (TInt i = 0; i < elems; i++)
        {
        if ((iBuffer[i].iDataFrame))
            {
            delete iBuffer[i].iDataFrame;
            iBuffer[i].iDataFrame = NULL;
            }
        }

    iBuffer.Reset();

    // Calculate needed elements
    iBufferLength = iJBConfig.iJBBufferLength * bufLenMultiplier;

    if (iJBConfig.iJBThreshold >= iBufferLength)
        {
        // adjust threshold size (no need to leave here)
        iJBConfig.iJBThreshold = iBufferLength / 2;
        }

    // If the difference between buffer length and threshold is less than 10
    // increase buffer length, so the differences is 10. This helps handle
    // buffer overflow more easily.
    CheckThresholdBufferLength(iBufferLength, iJBConfig.iJBThreshold);
    iCurrentPlayThreshold = iJBConfig.iJBThreshold;
    iOriginalPlayThreshold = iJBConfig.iJBThreshold;

    if (iCNGenerator)
        {
        delete iCNGenerator;
        iCNGenerator = NULL;
        }

    iCNGenerator = CVoIPCNGenerator::NewL(iObserver, aCodec);

    // Create the elements in the Buffer
    for (TInt k = 0; k < iBufferLength; k++)
        {
        CMMFDataBuffer* buf = CMMFDataBuffer::NewL(iFrameSize);
        CleanupStack::PushL(buf);
        TJitterBufferElement newElement;
        newElement.iDataFrame = buf;
        newElement.iSequenceNumber = -1;
        newElement.iTimeStamp = -1;
        iBuffer.AppendL(newElement);
        CleanupStack::Pop(buf);
        }

    // Reset statistics
    iFramesLost = 0;
    iFramesReceived = 0;
    iNumOfLateFrames = 0;
    iFramesRemoved = 0;
    iFramesPlayed = 0;
    iPacketsInBuffer = 0;

    // Calculate the sequence number increment
    iSeqNumIncrement = iSampleRate * iJBConfig.iSampleInterval;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::ResetBufferL
// Reset Jitter Buffer
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::ResetBuffer(TBool aPlayTone)
    {
    TRACE_PRN_FN_ENT;

    for (TInt i = 0; i < iBufferLength; i++)
        {
        iBuffer[i].iSequenceNumber = -1;
        iBuffer[i].iTimeStamp = -1;
        }

    iLastPlayedSeqNum = -1;
    iCurrentPlayThreshold = iOriginalPlayThreshold;
    iPacketsInBuffer = 0;
    iPlay = aPlayTone;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::DelayUpL
// Adds a buffer element into the jitter buffer
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::DelayUpL()
    {
    TRACE_PRN_FN_ENT;

    CMMFDataBuffer* buf = CMMFDataBuffer::NewL(iFrameSize);
    CleanupStack::PushL(buf);
    TJitterBufferElement newElement;
    newElement.iDataFrame = buf;
    newElement.iSequenceNumber = -1;
    newElement.iTimeStamp = -1;
    iBuffer.AppendL(newElement);
    CleanupStack::Pop(buf);

    iBufferLength++;

    // Insert one NO_DATA frame into the audio stream, so the jitterbuffer has
    // the possibility to grow at least one frame from the current size. If
    // the current playout threshold is zero, then there is ongoing playback or
    // a DTX period. This means that we should buffer at least one frame before
    // starting playback.
    if (iCurrentPlayThreshold == 0)
        {
        TRACE_PRN_N(_L("JB-> Adjusting Playback Threshold"));
        iCurrentPlayThreshold = iPacketsInBuffer + 1;
        }

    TRACE_PRN_N1(_L("JB-DelayUpL-> BUF_LEN [%d]"), iBufferLength);
    TRACE_PRN_N1(_L("JB-DelayUpL-> CUR_FRAMES [%d]"), iPacketsInBuffer);
    TRACE_PRN_N1(_L("JB-DelayUpL-> PLAY_TH [%d]"), iCurrentPlayThreshold);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::DelayDownL
// Removes an buffer element from the jitter buffer
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::DelayDownL()
    {
    TRACE_PRN_FN_ENT;

    // We need to remove one frame from the jitterbuffer. If currently we are
    // in a DTX period, we're in luck as we do not affect voice quality at all.

    // DURING DTX:
    // During DTX periods the jitterbuffer can be empty, so we cannot remove
    // anything from nothing. Then we basically can remove one element from the
    // buffer so it is shorter.

    // DURING SPEECH:
    // We'll need to remove one audio frame and mark the one preceding the
    // removed frame as bad. Then we'll continue as usual.
    TRACE_PRN_N1(_L("JB-DelayDownL-> BUF_LEN [%d]"), iBufferLength);
    TRACE_PRN_N1(_L("JB-DelayDownL-> CUR_FRAMES [%d]"), iPacketsInBuffer);

    if (IsEmpty())
        {
        // Cannot do anything for empty buffer
        if (KLowBufferLimit < iBufferLength)
            {
            TRACE_PRN_N(_L("JB-DelayDownL-> Empty buffer"));

            delete iBuffer[iBufferLength - 1].iDataFrame;
            iBuffer.Remove(iBufferLength - 1);
            iBufferLength--;
            }
        else if (iCurrentPlayThreshold > 0)
            {
            TRACE_PRN_N(_L("JB-DelayDownL-> Play threshold modification"));

            iCurrentPlayThreshold--;
            }
        else
            {
            // Cannot do anything currently
            TRACE_PRN_N(_L("JB-DelayDownL-> Buffer empty - Do nothing"));
            return;
            }
        }
    else if (IsFull())
        {
        // If there is sufficiently data in the buffer, then remove one
        // frame and mark the one preceding it bad
        if (KLowBufferLimit < iPacketsInBuffer)
            {
            TRACE_PRN_N(_L("JB-DelayDownL-> Buffer full - Removing"));

            // Remove the 2nd oldest frame and mark the oldest as bad
            delete iBuffer[1].iDataFrame;
            iBuffer.Remove(1);

            // MARK THE FIRST AS BAD FRAME!!!

            iBufferLength--;
            iPacketsInBuffer--;
            iFramesRemoved++;
            }
        else
            {
            // Cant do removing, see if playthreshold can be adjusted
            if (iCurrentPlayThreshold > 0)
                {
                TRACE_PRN_N(_L("JB-DelayDownL-> Buffer full - Do nothing"));

                iCurrentPlayThreshold--;
                }
            }
        }
    else
        {
        if (KLowBufferLimit < iBufferLength)
            {
            TRACE_PRN_N(_L("JB-DelayDownL-> Adjusting frame length"));

            // Adjust the length of the buffer
            delete iBuffer[iBufferLength - 1].iDataFrame;
            iBuffer.Remove(iBufferLength - 1);
            iBufferLength--;
            }
        else
            {
            TRACE_PRN_N(_L("JB-DelayDownL-> Buffer too small"));
            }
        }

    TRACE_PRN_FN_EXT;
    }

#ifdef __FEATURE_NOT_SUPPORTED__
// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::CalculateDelay
// Calculates the current jitter buffer playback
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CVoIPJitterBufferImpl::CalculateDelay() const
    {
    TTimeIntervalMicroSeconds32 delay = iJBConfig.iJitterLatency *
            iPacketsInBuffer;

    //TRACE_PRN_N1(_L("JB-CalculateDelay-> Delay [%d]"), delay);

    return delay;
    }
#endif //__FEATURE_NOT_SUPPORTED__

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::AddDataFrame
// Adds audio frame into the jitter buffer
// -----------------------------------------------------------------------------
//
TInt CVoIPJitterBufferImpl::AddDataFrame(CMMFBuffer* aDataBuffer)
    {
    if (aDataBuffer == NULL)
        {
        return KErrNotReady;
        }

    iCurrentSeqNum = aDataBuffer->FrameNumber();
    iFramesReceived++;

    // TODO: && TBD: Playout threshold update & re-buffering when starting
    // a new talk-burst.
    //
    // 1. If sender does not use inbaund DTX signalling.
    //  1.1 We may start re-buffering and sender goes into DTX mode again
    //      => We may not reach the playout threshold if the talkspurt is short
    //      => Also some random babble noise may activate the sender temporarily
    //      => The babble noise frame never gets played or some of these frames
    //         never get played.
    //
    // 2. If sender does use inband DTX signalling
    //  2.1 We can see the start of the DTX period which is cheked in
    //      GetDataFrameL()
    //  2.2 During DTX period we may detect the end of the DTX period &
    //      updates during the DTX period.
    //
    // 3. Packet loss may affect in the DTX detection.
    //  3.1 DTX start packet may be lost
    //  3.2 DTX UPDATE packet may be lost (if used)
    //

    TUint index = FindLargestSeqNum();

    if (iLastPlayedSeqNum < KMaxSeqNumber - iSeqNumIncrement)
        {
        iIsWrappedAround = IsSeqNumWrappedAround(iLargestSeqNum,
                iCurrentSeqNum);
        }
    else
        {
        // If the last played frame was larger than the current largest
        // in JB we are most likely running in the wrap-around mode.
        iIsWrappedAround = IsSeqNumWrappedAround(iLastPlayedSeqNum,
                iLargestSeqNum);
        }

    if (iIsWrappedAround)
        {
        iLastPlayedSeqNum = -1;
        }

    TRACE_PRN_N1(_L("JB-> [ADD] LARGEST IN  [%d]"), TInt32(iLargestSeqNum));
    TRACE_PRN_N1(_L("JB-> [ADD] THIS FRAME  [%d]"), TInt32(iCurrentSeqNum));
    TRACE_PRN_N1(_L("JB-> [ADD] LAST PLAYED [%d]"), TInt32(iLastPlayedSeqNum));
    TRACE_PRN_N1(_L("JB-> [ADD] PACKETS [%d]"), iPacketsInBuffer);

    iDataBuffer = static_cast<CMMFDataBuffer*> (aDataBuffer);

    if (iCurrentSeqNum > iLastPlayedSeqNum)
        {
        if (IsFull())
            {
            TRACE_PRN_N(_L("JB-> [ADD] BUFFER OVERFLOW"));

            // Remove the oldest frame from the buffer
            iLastPlayedSeqNum = iBuffer[iPacketsInBuffer - 1].iSequenceNumber;
            iBuffer[iPacketsInBuffer - 1].iSequenceNumber = -1;
            iBuffer[iPacketsInBuffer - 1].iTimeStamp = -1;
            iPacketsInBuffer--;

            InsertBufferElement(iDataBuffer->Data(), index);
            iDataBuffer = NULL;
            iFramesRemoved++;
            iOverflowCounter++;

            TRACE_PRN_N1(_L("JB-> [ADD] OverflowCount[%d]"), iOverflowCounter);

            // Report overflow only after KOverflowAlarmLimit
            if (iObserver && iOverflowCounter > KOverflowAlarmLimit)
                {
                iObserver->EventJB(MJitterBufferObserver::EBufferOverflow);
                ResetBuffer(ETrue);
                iOverflowCounter = 0;
                }
            }
        else if (IsEmpty())
            {
            TTimeIntervalSeconds interval;
            TTime currentTime;
            currentTime.UniversalTime();
            currentTime.SecondsFrom(iTonePlayTime, interval);

            if (iPlay && interval.Int() >= iPlayToneInterval)
                {
                TRACE_PRN_N(_L("JB-> [ADD] BUFFER EMPTY - Tone to be played"));
                iTonePlayTime.UniversalTime();
                iPlay = EFalse;
                }
            else
                {
                TRACE_PRN_N(_L("JB-> [ADD] BUFFER EMPTY - Tone NOT played"));
                TRACE_PRN_N1(_L("JB-> [ADD] Interval Tm [%d]"), interval.Int());
                }

            // No point to run at 0 threshold; restore the original
            // settings and rebuffer.
            iCurrentPlayThreshold = iOriginalPlayThreshold;

            const TInt64 nextGetSeqNum(iLastPlayedSeqNum + iSeqNumIncrement);
            const TInt64 frameSeq(iDataBuffer->FrameNumber());

            if (frameSeq > nextGetSeqNum)
                {
                // We are empty and there are frames missing between last
                // received frame and frame sequence number. This might be
                // because of DTX period, so we need to adjust accordingly.
                iLastPlayedSeqNum = frameSeq - iSeqNumIncrement;
                InsertBufferElement(iDataBuffer->Data(), index);

                TRACE_PRN_N(_L("JB-> [ADD] MISSING & EMPTY"));
                }
            else
                {
                InsertBufferElement(iDataBuffer->Data(), index);
                }
            }
        else
            {
            TRACE_PRN_N(_L("JB-> [ADD] NORMAL"));
            iOverflowCounter = 0;
            InsertBufferElement(iDataBuffer->Data(), index);
            }
        }
    else
        {
        iNumOfLateFrames++;
        aDataBuffer->SetStatus(EAvailable);
        TRACE_PRN_N(_L("JB-> [ADD] TOO LATE"));
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::GetDataFrame
// Get DataFrame
// -----------------------------------------------------------------------------
//
TInt CVoIPJitterBufferImpl::GetDataFrame(CMMFBuffer* aBuffer)
    {
    if (aBuffer == NULL)
        {
        return KErrNotReady;
        }

    iDataBuffer = static_cast<CMMFDataBuffer*> (aBuffer);
    TDes8& playBuffer(iDataBuffer->Data());
    iFramesPlayed++;

    if (IsEmpty())
        {
        // Buffer is empty, so we need to generate CN frames. During DTX period
        // this is where we'll end up. If we are empty because of DTX period,
        // the CN generator will do the error concealment.
        TRACE_PRN_N(_L("JB-> [GET] BUFFER EMPTY"));

        iCNGenerator->GenerateSidPacket(playBuffer,
                iDataBuffer->RequestSize());

        // Used for talkburst; when talk-burst is over, reset buffer
        // and sequence number
        if (iJBConfig.iJBInactivityTimeOut > 0 && iInactivityTime >= 0)
            {
            // Currently it is only used for AMR codec.
            iInactivityTime += iSampleInterval;

            if (iInactivityTime >= iJBConfig.iJBInactivityTimeOut)
                {
                TRACE_PRN_N1(_L("JB-> [GET] Inactivity Time Detected [%d]"),
                        iInactivityTime);
                iInactivityTime = -1;
                ResetBuffer();
                }
            }
        }
    else
        {
        // Check if threshold is met. If not, continue buffering.
        if (iPacketsInBuffer < iCurrentPlayThreshold)
            {
            // Give comfort noise when we are buffering before playback starts
            TRACE_PRN_N(_L("JB-> [GET] THRESHOLD NOT REACHED"));
            iCNGenerator->GenerateSidPacket(playBuffer,
                    iDataBuffer->RequestSize());
            }
        else
            {
            // Reset threshold, so we can  play current talk-spurt till the
            // end without affecting speech quality. This means that we will
            // play the buffer until it is empty. After that, it is either
            // DTX period or packet loss.
            iCurrentPlayThreshold = 0;

            // The actual sequence number which is in the buffer
            //const TInt64 frameToPlay(
            //iBuffer[iPacketsInBuffer - 1].iSequenceNumber);

            TRACE_PRN_N1(_L("JB-> [GET] FRAME TO PLAY [%d]"),
                    TInt32(iBuffer[iPacketsInBuffer - 1].iSequenceNumber));
            TRACE_PRN_N1(_L("JB-> [GET] LAST PLAYED   [%d]"),
                    TInt32(iLastPlayedSeqNum));
            TRACE_PRN_N1(_L("JB-> [GET] PACKETS [%d]"), iPacketsInBuffer);

            // Get next in order frame to play
            TJitterBufferElement& jbElement = iBuffer[iPacketsInBuffer - 1];

            if (playBuffer.MaxLength() >=
                    jbElement.iDataFrame->Data().Length())
                {
                playBuffer.Copy(jbElement.iDataFrame->Data());
                }
            else
                {
                TRACE_PRN_N1(_L("JB-> [GET] ERROR: BUFFER TOO SMALL [%d]"),
                        playBuffer.MaxLength());

                iCNGenerator->GenerateSidPacket(playBuffer,
                        iDataBuffer->RequestSize());
                }

            iLastPlayedSeqNum = jbElement.iSequenceNumber;
            jbElement.iSequenceNumber = -1;
            jbElement.iTimeStamp = -1;
            iPacketsInBuffer--;

            // Reset inactivity timeout timer
            iInactivityTime = 0;

#ifdef __FEATURE_NOT_SUPPORTED__
            // Try to lead new buffer to DTX period
            iCNGenerator->DoDtxDecision(playBuffer);
#endif //__FEATURE_NOT_SUPPORTED__
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::FindLargestSeqNum
// Find Largest SequenceNumber
// -----------------------------------------------------------------------------
//
TUint CVoIPJitterBufferImpl::FindLargestSeqNum()
    {
    TUint pos = 0;
    const TInt len = BufferLength();
    iLargestSeqNum = 0;

    for (TUint i = 0; i < len; i++)
        {
        // 99% of the time element 0 will hold the largest sequence num,
        // so, it won't have to search through the entire array.
        if (iBuffer[i].iSequenceNumber > iLargestSeqNum)
            {
            iLargestSeqNum = iBuffer[i].iSequenceNumber;
            pos = i;
            }
        }

    return pos;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::InsertBufferElement
// Insert Buffer Element
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::InsertBufferElement(const TDesC8& aBuffer,
        TInt64 aLargestSeqNum)
    {
    /* if (aBuffer.Length() == 0 || aBuffer.Length() > iFrameSize)
        {
        TRACE_PRN_N(_L("JB->INVALID DATA, IGNORING"));
        return;
        }*/

    const TInt len(BufferLength());

    if (iIsWrappedAround)
        {
        iBuffer[len - 1].iDataFrame->Data().Copy(aBuffer);
        iBuffer[len - 1].iSequenceNumber = iCurrentSeqNum;

        for (TInt i = 0; i < len; i++)
            {
            if (iBuffer[i].iTimeStamp > aLargestSeqNum)
                {
                aLargestSeqNum = iBuffer[i].iTimeStamp;
                }
            }

        iBuffer[len - 1].iTimeStamp = aLargestSeqNum + 1;
        iBuffer.Sort(iBufStampSorter);
        iLastPlayedSeqNum = -1;
        }
    else
        {
        iBuffer[len - 1].iDataFrame->Data().Copy(aBuffer);
        iBuffer[len - 1].iSequenceNumber = iCurrentSeqNum;
        iBuffer[len - 1].iTimeStamp = iCurrentSeqNum - BufferLength();
        iBuffer.Sort(iBufSequenceSorter);
        }

    iPacketsInBuffer++;
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::BufferLength()
// Return buffer length.
// -----------------------------------------------------------------------------
//
TInt CVoIPJitterBufferImpl::BufferLength() const
    {
    return iBufferLength;
    }

#ifdef __FEATURE_NOT_SUPPORTED__
// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::PacketCount()
// Return number of packets that are currently in the buffer.
// -----------------------------------------------------------------------------
//
TInt CVoIPJitterBufferImpl::PacketCount() const
    {
    return iPacketsInBuffer;
    }
#endif //__FEATURE_NOT_SUPPORTED__

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::IsFull()
// Return:    True if full
//            False if not full
// -----------------------------------------------------------------------------
//
TBool CVoIPJitterBufferImpl::IsFull() const
    {
    return (iPacketsInBuffer == iBufferLength);
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::IsEmpty()
// Return:    True if empty
//            False if not empty
// -----------------------------------------------------------------------------
//
TBool CVoIPJitterBufferImpl::IsEmpty() const
    {
    return (iPacketsInBuffer == 0);
    }

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::IsSeqNumWrappedAround
// Checks if sequence number is wrapped around between aSeqNum1 and aSeqNum2.
// The sequence number wrap-around condition cannot be reliably detected by
// checking for exact numbers (e.g. aSeqNum1==KMaxSeqNumber and aSeqNum2==0).
// This is because of network packet loss, where there might be gaps between
// frame sequence numbers.
// -----------------------------------------------------------------------------
//
TBool CVoIPJitterBufferImpl::IsSeqNumWrappedAround(TInt64 aSeqNum1,
        TInt64 aSeqNum2) const
    {
    TBool status = EFalse;

    if (aSeqNum1 >= (KMaxSeqNumber - iSeqNumIncrement) &&
            aSeqNum2 <= (iBufferLength * iSeqNumIncrement))
        {
        TRACE_PRN_N(_L("JB-> SEQUENCE WRAPPED-AROUND"));
        status = ETrue;
        }

    return status;
    }

#ifdef __FEATURE_NOT_SUPPORTED__
// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::GenerateStatistics
// Generates the statistics from the jitterbuffer
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::GenerateStatistics(/*TJBStats& aStats*/) const
    {
    aStats.iFramesReceived = iFramesReceived;
    aStats.iBufferLength = iBufferLength;
    aStats.iFramesInBuffer = iPacketsInBuffer;
    aStats.iFrameLoss = iFramesLost;
    aStats.iLateFrames = iNumOfLateFrames;
    aStats.iFramesRemoved = iFramesRemoved;
    aStats.iFramesPlayed = iFramesPlayed;
    }
#endif //__FEATURE_NOT_SUPPORTED__

// -----------------------------------------------------------------------------
// CVoIPJitterBufferImpl::CheckThresholdBufferLength
// -----------------------------------------------------------------------------
//
void CVoIPJitterBufferImpl::CheckThresholdBufferLength(TInt& aBufferLength,
        TInt aTreshhold) const
    {
    const TInt numTen = 10;

    if ((aBufferLength - aTreshhold) < numTen)
        {
        aBufferLength = aTreshhold + numTen;
        }
    }

//  End of File

