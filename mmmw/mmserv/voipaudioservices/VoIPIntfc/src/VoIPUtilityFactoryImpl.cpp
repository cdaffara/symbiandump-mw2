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
 *
 */

#include <e32base.h>
#include <AudioPreference.h>
#include <mmffourcc.h>
#include <voipuplinkstream.h>
#include <voipdownlinkstream.h>
#include "debugtracemacros.h"
#include "VoIPDataBufferImpl.h"
#include "VoIPJBDataBufferImpl.h"
#include "VoIPQueueHandler.h"
#include "VoIPSharedData.h"
#include "VoIPAudioSession.h"
#include "VoIPDtmfTonePlayerImpl.h"
#include "VoIPDownlinkStreamImpl.h"
#include "VoIPUplinkStreamImpl.h"
#include "VoIPUtilityFactoryImpl.h"

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPUtilityFactoryImpl* CVoIPUtilityFactoryImpl::NewL()
    {
    CVoIPUtilityFactoryImpl* self = new (ELeave) CVoIPUtilityFactoryImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::~CVoIPUtilityFactoryImpl
// ---------------------------------------------------------------------------
//
CVoIPUtilityFactoryImpl::~CVoIPUtilityFactoryImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CVoIPUtilityFactoryImpl
// ---------------------------------------------------------------------------
//
CVoIPUtilityFactoryImpl::CVoIPUtilityFactoryImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::ConstructL()
    {
    // Clients must have MultimediaDD capability to use this priority/pref.
    // Also, VAS will monitor for emergency call and if detected it  will
    // deny access to audio resources.
    iPriorityDn.iPref = KAudioPrefVoipAudioDownlink;
    iPriorityDn.iPriority = KAudioPriorityVoipAudioDownlink;
    iPriorityUp.iPref = KAudioPrefVoipAudioUplinkNonSignal; //for DTMF mixing
    iPriorityUp.iPriority = KAudioPriorityVoipAudioUplink;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CreateDownlinkStreamL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::CreateDownlinkStreamL(
        const TVersion /*aVersion*/,
        const CVoIPUtilityFactory::TVoIPCallType /*aCallType*/,
        CVoIPAudioDownlinkStream*& aDnLink)
    {
    iDownLink = CVoIPAudioDownlinkStreamImpl::NewL(iPriorityDn);
    aDnLink = iDownLink;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CreateDownlinkStreamL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::CreateDownlinkStreamL(
        const TVersion /*aVersion*/, CVoIPJitterBufferIntfc*& aJBIntfc,
        CVoIPAudioDownlinkStream*& aDnLink)
    {
    iDownLink = CVoIPAudioDownlinkStreamImpl::NewL(iPriorityDn);
    aDnLink = iDownLink;

    if (iDownLink)
        {
        iDownLink->CreateJitterBufferIntfcL(aJBIntfc);
        }
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CreateUplinkStreamL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::CreateUplinkStreamL(
        const TVersion /*aVersion*/,
        const CVoIPUtilityFactory::TVoIPCallType /*aCallType*/,
        CVoIPAudioUplinkStream*& aUpLink)
    {
    iUpLink = CVoIPAudioUplinkStreamImpl::NewL(iPriorityUp);
    aUpLink = iUpLink;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CreateDTMFTonePlayerL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::CreateDTMFTonePlayerL(
        CDTMFTonePlayer*& aDtmfPlayer)
    {
    aDtmfPlayer = CDTMFTonePlayerImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::CreateBufferL
// ---------------------------------------------------------------------------
//
void CVoIPUtilityFactoryImpl::CreateBufferL(CVoIPDataBuffer*& aBuffer,
        const CVoIPDataBuffer::TVoIPBufferType aType,
        const TVoIPCodecFormat aCodecFormat)
    {
    TRACE_PRN_FN_ENT;

    TInt bufLen = 0;

    switch (aCodecFormat)
        {
        case EG729:
            bufLen = KVoIPG729FrameLen;
            break;
        case EILBC:
            bufLen = KVoIPILBCFrameLen;
            break;
        case EAMR_NB:
            bufLen = KVoIPAMRNBFrameLen;
            break;
        case EPCM16:
            bufLen = KVoIPPCM16FrameLen;
            break;
        case EG711_10MS:
            bufLen = KVoIPG711FrameLen10ms;
            break;
        case EG711:
        default:
            bufLen = KVoIPG711FrameLen20ms;
        }

    // Create buffer type- and codec-dependent
    if (aType == CVoIPDataBuffer::EJitterBuffer && aCodecFormat == EPCM16)
        {
        User::Leave(KErrNotSupported);
        }
    else if (aType == CVoIPDataBuffer::EJitterBuffer)
        {
        aBuffer = CVoIPJBDataBufferImpl::NewL(bufLen);
        }
    else
        {
        aBuffer = CVoIPDataBufferImpl::NewL(bufLen);
        }

    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::GetSupportedDownlinkFormats
// ---------------------------------------------------------------------------
//
TInt CVoIPUtilityFactoryImpl::GetSupportedDownlinkFormats(
        RArray<TVoIPCodecFormat>& aFormats)
    {
    TInt err = KErrNotFound;
    if (iDownLink)
        {
        TRAP(err, iDownLink->GetSupportedFormatsL(iPriorityDn, aFormats));
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPUtilityFactoryImpl::GetSupportedUplinkFormats
// ---------------------------------------------------------------------------
//
TInt CVoIPUtilityFactoryImpl::GetSupportedUplinkFormats(
        RArray<TVoIPCodecFormat>& aFormats)
    {
    TInt err = KErrNotFound;
    if (iUpLink)
        {
        TRAP(err, iUpLink->GetSupportedFormatsL(iPriorityUp, aFormats));
        }
    return err;
    }

// End of file
