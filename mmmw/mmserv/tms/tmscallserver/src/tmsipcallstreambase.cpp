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

#include <e32math.h>
#include <mmcccodecinformation.h>
#include "tmsutility.h"
#include "tmsshared.h"
#include "tmsipcallstream.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::TMSIPCallStreamBase
// Constructor
// -----------------------------------------------------------------------------
//
TMSIPCallStreamBase::TMSIPCallStreamBase(TMSIPDevSoundObserver& observer) :
    iObserver(observer)
    {
    }

// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::~TMSIPCallStreamBase
// Destructor
// -----------------------------------------------------------------------------
//
TMSIPCallStreamBase::~TMSIPCallStreamBase()
    {
    TRACE_PRN_FN_ENT;

    if (iChunk.Handle() > 0)
        {
        iChunk.Close();
        }

#ifndef __USE_GSTREAMER__
    delete iDevSound;
#endif //__USE_GSTREAMER__
    TRACE_PRN_FN_EXT;
    }

#ifndef __USE_GSTREAMER__
// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::InitDevSoundL
//
// -----------------------------------------------------------------------------
//
void TMSIPCallStreamBase::InitDevSoundL(const TMMFState aDevSoundState,
        const TMMFPrioritySettings priority)
    {
    TRACE_PRN_FN_ENT;

    if (!iDevSound)
        {
        // Create D/S recording/playing device
        iDevSound = CMMFDevSound::NewL();
        }

    if (iDevSound)
        {
        iDevSound->InitializeL(*this, TFourCC(iCodecID), aDevSoundState);
        iDevSound->SetPrioritySettings(priority);
        }
    }
#endif //__USE_GSTREAMER__

// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::SetMsgQueue
//
// -----------------------------------------------------------------------------
//
void TMSIPCallStreamBase::SetMsgQueue(const RMsgQueue<TmsMsgBuf> aMsgQueue)
    {
    iMsgQueue = aMsgQueue;
    }

// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::ConfigureMedia
//
// -----------------------------------------------------------------------------
//
gint TMSIPCallStreamBase::ConfigureMedia(const guint32 aCodecID)
    {
    gint framelen = 0;

    switch (aCodecID)
        {
        case KMccFourCCIdG711:
            iMediaType = "audio/x-alaw";
            // iMediaType = "audio/x-mulaw";
            framelen = KTMSG711FrameLen10ms; //TODO: may be H/W specific!
            //framelen = KTMSG711FrameLen20ms;
            break;
        case KMccFourCCIdG729:
            iMediaType = "audio/g729";
            framelen = KTMSG729FrameLen;
            break;
        case KMccFourCCIdILBC:
            iMediaType = "audio/ilbc";
            framelen = KTMSILBCFrameLen20ms;
            //framelen = KTMSILBCFrameLen30ms;
            break;
        case KMccFourCCIdAMRNB:
            iMediaType = "audio/amr";
            framelen = KTMSAMRNBFrameLen;
            //framelen = KTMSAMRNBFrameLenFM;
            break;
        case KMMFFourCCCodePCM16:
        default:
            iMediaType = "audio/x-raw-int";
            framelen = KTMSPCM16FrameLen;
#ifdef __WINSCW__
            framelen *= 4; //16kB
#endif
            break;
        }

    //TODO: DEVLON TEST
    if (iCodecID != KMMFFourCCCodePCM16)
        {
        framelen *= 10;
        }
    //TODO: DEVLON TEST

    return framelen;
    }

// -----------------------------------------------------------------------------
// TMSIPCallStreamBase::DoChunk
// Checks, adjusts and creates global RChunk.
// -----------------------------------------------------------------------------
//
gint TMSIPCallStreamBase::DoChunk(const gint aDataLen, TmsMsgBuf& aMsgBuffer)
    {
    gint status(KErrNone);

    // If chunk already created, try to reuse it
    if (iChunk.Handle())
        {
        // committed size is smaller but chunk is large enough to hold data
        if ((iChunk.Size() < aDataLen) && (aDataLen <= iChunk.MaxSize()))
            {
            status = iChunk.Adjust(aDataLen);
            TRACE_PRN_N1(_L("IP_CALL: DoChunk->Chunk Adjusted [%d]"), aDataLen);
            }
        else if (aDataLen > iChunk.MaxSize())
            {
            iChunk.Close();
            TRACE_PRN_N(_L("IP_CALL: DoChunk->Chunk Closed"));
            }
        }
    RET_ERR_IF_ERR(status);

    // Request kernel to create global RChunk if needed
    aMsgBuffer.iBool = FALSE;
    if (!iChunk.Handle())
        {
        // TODO consider using some max instead
        status = iChunk.CreateGlobal(KNullDesC, aDataLen, aDataLen);
        // Mark to re-open chunk handle
        aMsgBuffer.iBool = TRUE;
        aMsgBuffer.iUint32 = Math::Random();
        TRACE_PRN_N2(_L("IP_CALL: DoChunk->Chunk Created [%d]key[%d]"),
                aDataLen, aMsgBuffer.iUint32);
        }
    TRACE_PRN_IF_ERR(status);
    return status;
    }

// End of file
