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
#include "tmscsuplink.h"
#include "tmscsdevsoundobserver.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
TMSCSUplink::TMSCSUplink(TMSCSDevSoundObserver& observer) :
    TMSCSDevSound(observer)
    {
    }

// -----------------------------------------------------------------------------
// Second phase constructor
// -----------------------------------------------------------------------------
//
void TMSCSUplink::ConstructL(const gint retrytime)
    {
    TMSCSDevSound::ConstructL(TMS_STREAM_UPLINK, retrytime);
    }

// -----------------------------------------------------------------------------
// Static constructor
// -----------------------------------------------------------------------------
//
TMSCSUplink* TMSCSUplink::NewL(TMSCSDevSoundObserver& observer,
        const gint retrytime)
    {
    TMSCSUplink* self = new (ELeave) TMSCSUplink(observer);
    CleanupStack::PushL(self);
    self->ConstructL(retrytime);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
TMSCSUplink::~TMSCSUplink()
    {
    }

// -----------------------------------------------------------------------------
// Sets mic gain
// -----------------------------------------------------------------------------
//
void TMSCSUplink::SetGain(gint gain)
    {
    if (iDevSound)
        {
        iDevSound->SetGain(gain);
        }
    }

// -----------------------------------------------------------------------------
// Returns mic gain
// -----------------------------------------------------------------------------
//
gint TMSCSUplink::Gain()
    {
    gint gain = 0;
    if (iDevSound)
        {
        gain = iDevSound->Gain();
        }
    return gain;
    }

// -----------------------------------------------------------------------------
// Returns max mic gain
// -----------------------------------------------------------------------------
//
gint TMSCSUplink::MaxGain()
    {
    gint gain = 0;
    if (iDevSound)
        {
        gain = iDevSound->MaxGain();
        }
    return gain;
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// Uplink stream has been activated successfully.
// -----------------------------------------------------------------------------
//
void TMSCSUplink::BufferToBeEmptied(CMMFBuffer* /*aBuffer*/)
    {
    TRACE_PRN_N(_L("TMSCSUplink::BufferToBeEmptied"));

    // We dont react to devsound messages unless we are activating.
    if (iActivationOngoing)
        {
        iActive = ETrue;
        iActivationOngoing = EFalse;
        iObserver.UplinkActivationCompleted(KErrNone);
        }
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// Uplink stream activation failed
// -----------------------------------------------------------------------------
//
void TMSCSUplink::RecordError(TInt aError)
    {
    TRACE_PRN_N1(_L("TMSCSUplink::RecordError[%d]"), aError);

    // We dont react to devsound messages unless we are activating.
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
            iObserver.UplinkActivationCompleted(aError);
            }
#else  //__WINSCW__
        iObserver.UplinkActivationCompleted(TMS_RESULT_SUCCESS);
#endif //__WINSCW__
        }
    }

// -----------------------------------------------------------------------------
// From class TMSCSDevSound
// Activates Uplink stream.
// -----------------------------------------------------------------------------
//
void TMSCSUplink::DoActivateL()
    {
    if (iDevSound)
        {
        iDevSound->RecordInitL();
        }
    }

//  End of File
