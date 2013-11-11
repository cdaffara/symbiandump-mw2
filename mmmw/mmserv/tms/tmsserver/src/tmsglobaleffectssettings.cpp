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

// INCLUDE FILES
#include "tmsglobaleffectssettings.h"
#include "tmsclientserver.h"
#include "tmsutility.h"

using namespace TMS;

// CONSTANTS
const TInt KDefaultVolume = 4;
const TInt KDefaultMaxVolume = 10;
const TInt KDefaultGain = 1;

#ifndef __WINSCW__
const TInt KDefaultMaxGain = 1;
#else
const TInt KDefaultMaxGain = 64;
#endif

TUid KCRUidTmseffects = {0x10207C80};
const TUint32 KTmsEarPieceVolume = 0x00000001;
const TUint32 KTmsLoudSpkrVolume = 0x00000002;

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::TMSGlobalEffectsSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSGlobalEffectsSettings::TMSGlobalEffectsSettings() :
    iGain(KDefaultGain),
    iMaxVolume(KDefaultMaxVolume),
    iMaxGain(KDefaultMaxGain),
    iEarVolume(KDefaultVolume),
    iLoudSpkrVolume(KDefaultVolume),
    iRepository(NULL)
    {
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    // Create repository instance
    iRepository = CRepository::NewL(KCRUidTmseffects);
    if (iRepository)
        {
        iRepository->Get(KTmsLoudSpkrVolume, iLoudSpkrVolume);
        iRepository->Get(KTmsEarPieceVolume, iEarVolume);
        TRACE_PRN_N1(_L("loud spkr vol %d"),iLoudSpkrVolume);
        TRACE_PRN_N1(_L("ear piece vol %d"),iEarVolume);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSGlobalEffectsSettings* TMSGlobalEffectsSettings::NewL()
    {
    TMSGlobalEffectsSettings* self = new (ELeave) TMSGlobalEffectsSettings();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
TMSGlobalEffectsSettings::~TMSGlobalEffectsSettings()
    {
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::SetLoudSpkrVolume
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::SetLoudSpkrVolume(TInt aVolume)
    {
    TRACE_PRN_FN_ENT;
    TInt status(KErrNone);
    if (iRepository)
        {
        if (aVolume > 0)
            {
            status = iRepository->Set(KTmsLoudSpkrVolume, aVolume);
            TRACE_PRN_N2(_L("status %d loud spkr vol %d"),status,aVolume);
            }
        else
            {
            status = iRepository->Set(KTmsLoudSpkrVolume, KDefaultVolume);
            TRACE_PRN_N(_L("loud spkr level 0 store default"));
            }
        if (status == KErrNone)
            {
            iLoudSpkrVolume = aVolume;
            TRACE_PRN_N1(_L("cached loud spkr vol %d"),iLoudSpkrVolume);
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::SetEarPieceVolume
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::SetEarPieceVolume(TInt aVolume)
    {
    TRACE_PRN_FN_ENT;
    TInt status(KErrNone);
    if (iRepository)
        {
        if (aVolume > 0)
            {
            status = iRepository->Set(KTmsEarPieceVolume, aVolume);
            TRACE_PRN_N2(_L("status %d ear vol %d"),status,aVolume);
            }
        else
            {
            status = iRepository->Set(KTmsEarPieceVolume, KDefaultVolume);
            TRACE_PRN_N(_L("ear piece vol level 0 store default"));
            }
        if (status == KErrNone)
            {
            iEarVolume = aVolume;
            TRACE_PRN_N1(_L("cached ear piece vol %d"),iEarVolume);
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::GetLoudSpkrVolume
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::GetLoudSpkrVolume(TInt& aVolume)
    {
    aVolume = iLoudSpkrVolume;
    TRACE_PRN_N1(_L("TMSGlobalEffectsSettings GetLoudSpkrVolume %d"),aVolume);
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::GetEarPieceVolume
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::GetEarPieceVolume(TInt& aVolume)
    {
    aVolume = iEarVolume;
    TRACE_PRN_N1(_L("TMSGlobalEffectsSettings GetEarPieceVolume  %d"),aVolume);
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::MaxVolume
// -----------------------------------------------------------------------------
//
TInt TMSGlobalEffectsSettings::MaxVolume() const
    {
    return iMaxVolume;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::SetGain
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::SetGain(TInt aGain)
    {
    iGain = aGain;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::Gain
// -----------------------------------------------------------------------------
//
TInt TMSGlobalEffectsSettings::Gain() const
    {
    return iGain;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::MaxGain
// -----------------------------------------------------------------------------
//
TInt TMSGlobalEffectsSettings::MaxGain() const
    {
    return iMaxGain;
    }

// -----------------------------------------------------------------------------
// TMSGlobalEffectsSettings::ResetDefaultVolume
// -----------------------------------------------------------------------------
//
void TMSGlobalEffectsSettings::ResetDefaultVolume()
    {
    if (iLoudSpkrVolume == 0)
        {
        iRepository->Get(KTmsLoudSpkrVolume, iLoudSpkrVolume);
        TRACE_PRN_N1(_L("Needed to reset default loudspkr vol %d"),iLoudSpkrVolume);
        }
    if (iEarVolume == 0)
        {
        iRepository->Get(KTmsEarPieceVolume, iEarVolume);
        TRACE_PRN_N1(_L("Needed to reset default ear vol %d"),iEarVolume);
        }
    }

