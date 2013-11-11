/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Radio FM Tuner Utility interface implementation.
*
*/


#include <RadioFmTunerUtility.h>
#include "RadioFmTunerUtilityBody.h"
#include "trace.h"

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::NewL
// Destructor
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility* CRadioFmTunerUtility::NewL(
    RRadioSession& aRadioSession,
    MRadioFmTunerObserver& aObserver)
    {
    FUNC_LOG;
    CRadioFmTunerUtility* self = new(ELeave) CRadioFmTunerUtility;
    CleanupStack::PushL(self);
    self->iBody = CRadioFmTunerUtility::CBody::NewL(aRadioSession, aObserver);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::~CRadioFmTunerUtility
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioFmTunerUtility::~CRadioFmTunerUtility()
    {
    FUNC_LOG;
    delete iBody;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CRadioFmTunerUtility
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility::CRadioFmTunerUtility()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::RequestTunerControl
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::RequestTunerControl()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->RequestTunerControl();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::Close
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::Close()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->Close();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetCapabilities(
    TFmTunerCapabilities& aCaps)
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetCapabilities(aCaps);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::EnableTunerInOfflineMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::EnableTunerInOfflineMode(
    TBool aEnable )

    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->EnableTunerInOfflineMode(aEnable);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::SetFrequency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::SetFrequency(
    TInt aFrequency )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->SetFrequency(aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CancelSetFrequency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::CancelSetFrequency()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelSetFrequency();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetFrequency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetFrequency(
    TInt& aFrequency ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetFrequency(aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::SetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::SetFrequencyRange(
    TFmRadioFrequencyRange aRange )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->SetFrequencyRange(aRange);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CancelSetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::CancelSetFrequencyRange()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelSetFrequencyRange();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetFrequencyRange(
    TFmRadioFrequencyRange& aRange,
    TInt& aMinFreq,
    TInt& aMaxFreq ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetFrequencyRange(aRange, aMinFreq, aMaxFreq);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::StationSeek
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::StationSeek(
    TBool aUpwards )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->StationSeek(aUpwards);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CancelStationSeek
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioFmTunerUtility::CancelStationSeek()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelStationSeek();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetSignalStrength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetSignalStrength(
    TInt& aSignalStrength ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetSignalStrength(aSignalStrength);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetMaxSignalStrength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetMaxSignalStrength(
    TInt& aMaxSignalStrength ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetMaxSignalStrength(aMaxSignalStrength);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetStereoMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetStereoMode(
    TBool& aStereo ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetStereoMode(aStereo);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::ForceMonoReception
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::ForceMonoReception(
    TBool aForcedMono )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->ForceMonoReception(aForcedMono);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetForcedMonoReception
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetForcedMonoReception(
    TBool& aForcedMono ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetForcedMonoReception(aForcedMono);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::SetSquelch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::SetSquelch(
    TBool aSquelch )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetSquelch(aSquelch);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetSquelch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioFmTunerUtility::GetSquelch(
    TBool& aSquelch ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetSquelch(aSquelch);
    }

// End of File
