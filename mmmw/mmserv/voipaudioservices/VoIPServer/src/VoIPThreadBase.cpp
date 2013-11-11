/*
 * Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VoIP Audio Services
 *               Implements base class for uplink and downlink thread objects.
 *
 */

#include "debugtracemacros.h"
#include "VoIPServerThread.h"

// -----------------------------------------------------------------------------
// CVoIPThreadBase::~CVoIPThreadBase
// Destructor
// -----------------------------------------------------------------------------
//
CVoIPThreadBase::~CVoIPThreadBase()
    {
    TRACE_PRN_FN_ENT;

    iChunk.Close();
    delete iMsgQComHandler;
    iMsgQueue.Close();
    iMsgComQueue.Close();
    delete iDevSound;
    delete iActiveScheduler;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPThreadBase::InitThreadL
// Thread initialization
// -----------------------------------------------------------------------------
//
void CVoIPThreadBase::InitThreadL()
    {
    TRACE_PRN_FN_ENT;

    // Create and install active scheduler for this thread
    iActiveScheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPThreadBase::InitMsgQueuesL
//
// -----------------------------------------------------------------------------
//
void CVoIPThreadBase::InitMsgQueuesL(const TDesC& aBufferQ, const TDesC& aComQ)
    {
    TRACE_PRN_FN_ENT;

    // Make sure client thread finished creating message queues
    while (iMsgQueue.OpenGlobal(aBufferQ))
        {
        User::After(KQueueCreateWaitTime);
        }

    // Create message queue for ITC with this thread
    while (iMsgComQueue.OpenGlobal(aComQ))
        {
        User::After(KQueueCreateWaitTime);
        }

    iMsgQComHandler = CQueueHandlerSrv::NewL(this, &iMsgComQueue);

    if (iMsgQComHandler)
        {
        iMsgQComHandler->Start();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPThreadBase::InitDevSoundL
//
// -----------------------------------------------------------------------------
//
void CVoIPThreadBase::InitDevSoundL(const TMMFState aDevSoundState,
        const TInt aPriority, const TInt aPreference)
    {
    TRACE_PRN_FN_ENT;

    iDevSound = CMMFDevSound::NewL();

    if (iDevSound)
        {
        iDevSound->InitializeL(*this, TFourCC(iCodecID), aDevSoundState);
        TMMFPrioritySettings prio;
        prio.iPref = aPreference;
        prio.iPriority = aPriority;
        iDevSound->SetPrioritySettings(prio);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPThreadBase::DetermineMaxBufferLen
//
// -----------------------------------------------------------------------------
//
TInt CVoIPThreadBase::DetermineMaxBufferLen(
        TVoIPCodecSettings::TG711FrameRate aG711FrameRate)
    {
    TRACE_PRN_FN_ENT;

    TInt len = 0;

    switch (iCodecID)
        {
        case KMccFourCCIdG711:
            {
            len = KVoIPG711FrameLen20ms;
            if (aG711FrameRate == TVoIPCodecSettings::E10MS)
                {
                len = KVoIPG711FrameLen10ms;
                }
            break;
            }
        case KMccFourCCIdG729:
            len = KVoIPG729FrameLen;
            break;
        case KMccFourCCIdILBC:
            len = KVoIPILBCFrameLen;
            break;
        case KMccFourCCIdAMRNB:
            len = KVoIPAMRNBFrameLen;
            break;
        case KMMFFourCCCodePCM16:
            len = KVoIPPCM16FrameLen;
            break;
        default:
            break;
        }

    return len;
    }

// -----------------------------------------------------------------------------
// CVoIPThreadBase::DoChunk
// Checks, adjusts and creates global RChunk.
// -----------------------------------------------------------------------------
//
TInt CVoIPThreadBase::DoChunk(const TDesC& aChunkName, const TInt aChunkLen,
        const TInt aChunkMaxLen)
    {
    TInt err = KErrNone;

    if (iChunk.Handle() == 0)
        {
        // NOTE: The fact we closed chunk handle doesn't mean the chunk is
        //       physically removed from the kernel. Most likely it still
        //       exist as long as the owning process is up and running.
        //       Trying to create new global chunk with duplicate name fails
        //       with -11. That's why we need to search first.

        TFindChunk chunk(aChunkName);
        TFullName name;
        if (chunk.Next(name) == KErrNone)
            {
            // Chunk found - open and use it
            iChunk.SetHandle(chunk.Handle());
            err = iChunk.Open(chunk);
            if (err == KErrNone)
                {
                err = iChunk.Adjust(aChunkLen);
                }
            TRACE_PRN_N1(_L("VoIP->THRD: DoChunk->Chunk Opened [%d]"), err);
            }
        else
            {
            err = iChunk.CreateGlobal(aChunkName, aChunkLen, aChunkMaxLen);
            TRACE_PRN_N1(_L("VoIP->THRD: DoChunk->Chunk Created [%d]"), err);
            }
        }
    else
        {
        // Make sure global chunk's current committed size is not
        // smaller than the buffer size requested by the D/S
        if (aChunkLen > iChunk.Size())
            {
            // Re-adjust length of the existing RChunk
            err = iChunk.Adjust(aChunkLen);
            TRACE_PRN_N1(_L("VoIP->THRD: DoChunk->Chunk Adjusted [%d]"), err);
            }
        }

    return err;
    }

// End of file
