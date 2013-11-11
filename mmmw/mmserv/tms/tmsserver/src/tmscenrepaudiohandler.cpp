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
 * Description:  Implements the class TMSCenRepAudioHandler
 *
 */

#include <e32base.h>
#include <e32property.h>
#include "tmstelephonycenrep.h"

#ifdef _USE_TELEPHONY_CENREP_
#include <telmicmutestatuspskeys.h>
#include <telincallvolcntrlcrkeys.h>
#else
const TUid KCRUidInCallVolume = {0x102828B1};
const TUint32 KTelIncallEarVolume = 0x00000001;
const TUint32 KTelIncallLoudspeakerVolume = 0x00000002;
#endif

#include "tmscenrepaudiohandler.h"
#include "tmscenreplistener.h"
#include "tmspubsublistener.h"
#include "tmsutility.h"

#ifdef __WINSCW__
const TInt KDefaultMaxGain = 64;
#else
const TInt KDefaultMaxGain = 1;
#endif

using namespace TMS;

// ---------------------------------------------------------------------------
// TMSCenRepAudioHandler::NewL.
// ---------------------------------------------------------------------------
//
TMSCenRepAudioHandler* TMSCenRepAudioHandler::NewL(TMSServer* aServer)
    {
    TRACE_PRN_FN_ENT;
    TMSCenRepAudioHandler* self = new (ELeave) TMSCenRepAudioHandler(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// ---------------------------------------------------------------------------
// Destructs the object by canceling first ongoing monitoring.
// ---------------------------------------------------------------------------
//
TMSCenRepAudioHandler::~TMSCenRepAudioHandler()
    {
    TRACE_PRN_FN_ENT;
    delete iMuteListener;
    delete iIncallLoudspeakerVolumeListener;
    delete iIncallEarVolumeListener;
    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// TMSCenRepAudioHandler::HandleNotifyPSL
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::HandleNotifyPSL(const TUid /*aUid*/,
        const TInt& /*aKey*/, const TRequestStatus& /*aStatus*/)
    {
    TInt muteVal;
    TInt err = KErrNotFound;
    if (iPublish)
        {
        if (iMuteListener)
            {
            err = iMuteListener->Get(muteVal);
            }
        if (err == KErrNone && muteVal == EPSTelMicMuteOn)
            {
#if !defined(__WINSCW__)
            if (iTMSSer)
                {
                iTMSSer->SetGain(NULL, 0);
                }
#endif //__WINSCW__
            }
        else if (err == KErrNone)
            {
#if !defined(__WINSCW__)
            // Change when gain is really changed
            if (iTMSSer)
                {
                iTMSSer->SetGain(NULL, KDefaultMaxGain);
                }
#endif //__WINSCW__
            }
        }
    iPublish = TRUE;
    }

// ---------------------------------------------------------------------------
// TMSCenRepAudioHandler::SetMuteState
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::SetMuteState(TInt level)
    {
    if (iMuteListener)
        {
        if (level == 0)
            {
            iMuteListener->Set(EPSTelMicMuteOn);
            }
        else
            {
            iMuteListener->Set(EPSTelMicMuteOff);
            }
        }
    iPublish = FALSE;
    }

// ---------------------------------------------------------------------------
// TMSCenRepAudioHandler::SetLoudSpeakerVol
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::SetLoudSpeakerVol(TInt vol)
    {
    if (iIncallLoudspeakerVolumeListener)
        {
        iIncallLoudspeakerVolumeListener->Set(vol);
        }
    }

// ---------------------------------------------------------------------------
// TMSCenRepAudioHandler::SetEarPieceVol
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::SetEarPieceVol(TInt vol)
    {
    if (iIncallEarVolumeListener)
        {
        iIncallEarVolumeListener->Set(vol);
        }
    }

// ---------------------------------------------------------------------------
// From TMSCenRepObserver
// TMSCenRepAudioHandler::HandleNotifyCenRepL
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::HandleNotifyCenRepL(const TUid /*aUid*/,
        const TUint32 aKey, TInt aVal)
    {
    TRACE_PRN_FN_ENT;
    if (iTMSSer && aKey == KTelIncallLoudspeakerVolume)
        {
        iTMSSer->SetLevel(NULL, FALSE, aVal);
        }
    else if (iTMSSer && aKey == KTelIncallEarVolume)
        {
        iTMSSer->SetLevel(NULL, FALSE, aVal);
        }
    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// Constructs the monitor.
// ---------------------------------------------------------------------------
//
TMSCenRepAudioHandler::TMSCenRepAudioHandler(TMSServer* aServer) :
    iTMSSer(aServer)
    {
    TRACE_PRN_FN_ENT;
    iCallCount = 0; // Active calls count
    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// Second phase construction.
// ---------------------------------------------------------------------------
//
void TMSCenRepAudioHandler::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    RProperty::TType type(RProperty::EInt);
    TSecurityPolicy readPolicy(ECapability_None);
    TSecurityPolicy writePolicy(ECapabilityWriteDeviceData);

    RProperty::Define(KPSUidTelMicrophoneMuteStatus, KTelMicrophoneMuteState,
            type, readPolicy, writePolicy);

    iMuteListener = TMSPubSubListener::NewL(KPSUidTelMicrophoneMuteStatus,
            KTelMicrophoneMuteState, this);

    iIncallLoudspeakerVolumeListener = TMSCenRepListener::NewL(
            KCRUidInCallVolume, KTelIncallLoudspeakerVolume, this);

    iIncallEarVolumeListener = TMSCenRepListener::NewL(KCRUidInCallVolume,
            KTelIncallEarVolume, this);

    // Initialize audio volumes
    TInt volEar;
    TInt volLoud;

    if (iIncallEarVolumeListener)
        {
        /*TInt volGetRes =*/ iIncallEarVolumeListener->Get(volEar);
        }
    if (iIncallLoudspeakerVolumeListener)
        {
        /*volGetRes =*/ iIncallLoudspeakerVolumeListener->Get(volLoud);
        }

    iPublish = TRUE;
    TRACE_PRN_FN_EXT;
    }

// End of file
