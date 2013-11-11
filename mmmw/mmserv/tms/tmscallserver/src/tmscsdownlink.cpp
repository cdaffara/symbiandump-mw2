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

#include <AudioPreference.h>
#include "tmsutility.h"
#include "tmscsdownlink.h"
#include "tmscsdevsoundobserver.h"

using namespace TMS;

const gint KDefaultVolume = 4;
const gint KDefaultMaxVolume = 10;

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
TMSCSDownlink::TMSCSDownlink(TMSCSDevSoundObserver& observer) :
    TMSCSDevSound(observer)
    {
    }

// -----------------------------------------------------------------------------
// Second phase constructor
// -----------------------------------------------------------------------------
//
void TMSCSDownlink::ConstructL(const gint retrytime)
    {
    TMSCSDevSound::ConstructL(TMS_STREAM_DOWNLINK, retrytime);

    if (iDevSound)
        {
        iDevSound->SetVolume(KDefaultVolume);
        }
    }

// -----------------------------------------------------------------------------
// Static constructor
// -----------------------------------------------------------------------------
//
TMSCSDownlink* TMSCSDownlink::NewL(TMSCSDevSoundObserver& observer,
        const gint retrytime)
    {
    TMSCSDownlink* self = new (ELeave) TMSCSDownlink(observer);
    CleanupStack::PushL(self);
    self->ConstructL(retrytime);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
TMSCSDownlink::~TMSCSDownlink()
    {
    }

// -----------------------------------------------------------------------------
// Sets volume
// -----------------------------------------------------------------------------
//
void TMSCSDownlink::SetVolume(gint volume)
    {
    if (iDevSound)
        {
        gint maxVolume(iDevSound->MaxVolume());
        maxVolume = (maxVolume > 0) ? maxVolume : KDefaultMaxVolume;
        gint scaledVolume = (volume * maxVolume) / KDefaultMaxVolume;
        iDevSound->SetVolume(scaledVolume);
        }
    }

// -----------------------------------------------------------------------------
// Returns volume
// -----------------------------------------------------------------------------
//
gint TMSCSDownlink::Volume()
    {
    gint vol = 0;
    if (iDevSound)
        {
        vol = iDevSound->Volume();
        }
    return vol;
    }

// -----------------------------------------------------------------------------
// Returns device's max volume
// -----------------------------------------------------------------------------
//
gint TMSCSDownlink::MaxVolume()
    {
    gint vol = 0;
    if (iDevSound)
        {
        vol = iDevSound->MaxVolume();
        }
    return vol;
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// Downlink stream has been activated successfully.
// -----------------------------------------------------------------------------
//
void TMSCSDownlink::BufferToBeFilled(CMMFBuffer* /*aBuffer*/)
    {
    TRACE_PRN_N(_L("TMSCSDownlink::BufferToBeFilled"));

    // We dont react to devsound messages unless we are activating.
    if (iActivationOngoing)
        {
        iActive = ETrue;
        iActivationOngoing = EFalse;
        iObserver.DownlinkActivationCompleted(KErrNone);
        }
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// Downlink stream activation failed
// -----------------------------------------------------------------------------
//
void TMSCSDownlink::PlayError(TInt aError)
    {
    TRACE_PRN_N1(_L("TMSCSDownlink::PlayError[%d]"), aError);

    // We don't react to devsound errors unless we are activating.
    if (iActivationOngoing && (aError == KErrAccessDenied ||
            aError == KErrInUse))
        {
#ifndef __WINSCW__
        if (iStartRetryTime != 0)
            {
            StartTimer();
            }
        else
            {
            CancelTimer();
            iActivationOngoing = EFalse;
            iObserver.DownlinkActivationCompleted(aError);
            }
#else  //__WINSCW__
        iObserver.DownlinkActivationCompleted(TMS_RESULT_SUCCESS);
#endif //__WINSCW__
        }
    }

// -----------------------------------------------------------------------------
// From class TMSCSDevSound
// Activates Downlink stream.
// -----------------------------------------------------------------------------
//
void TMSCSDownlink::DoActivateL()
    {
    if (iDevSound)
        {
        iDevSound->PlayInitL();
        }
    }

//  End of File
