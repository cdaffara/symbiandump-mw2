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
* Description:  Radio FM Tuner Utility implementation body.
*
*/


#include "RadioFmTunerUtilityBody.h"
#include "RadioUtilityBody.h"
#include "trace.h"

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility::CBody* CRadioFmTunerUtility::CBody::NewL(
    RRadioSession& aRadioSession,
    MRadioFmTunerObserver& aObserver )
    {
    FUNC_LOG;
    CRadioFmTunerUtility::CBody* s = new(ELeave) CRadioFmTunerUtility::CBody( aRadioSession, aObserver );
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop();
    return s;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::CBody
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility::CBody::CBody( 
        RRadioSession& aRadioSession,
        MRadioFmTunerObserver& aObserver )
    : iRadioSession(aRadioSession),
    iRadioFmTunerUtilityClient(aObserver)
   
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::~CBody
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility::CBody::~CBody()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::RequestTunerControl
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::RequestTunerControl()
    {
    FUNC_LOG;
    iRadioSession.RequestTunerControl( ERsTunerFm );
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::Close
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::Close()
    {
    FUNC_LOG;
    // Consider releasing tuner control here.
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetCapabilities(
    TFmTunerCapabilities& aCaps ) const
    {
    FUNC_LOG;
    TRsTunerCapabilities caps;
    TInt error = iRadioSession.GetTunerCapabilities( caps );

    if ( !error )
        {
        aCaps.iTunerBands = caps.iFrequencyRange;
        aCaps.iTunerFunctions = caps.iCapabilities;
        aCaps.iAdditionalFunctions1 = caps.iAdditionalFunctions1;
        aCaps.iAdditionalFunctions2 = caps.iAdditionalFunctions2;

        }
    return error;
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::EnableTunerInOfflineMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::EnableTunerInOfflineMode(
    TBool aEnable )
    {
    FUNC_LOG;
    return iRadioSession.EnableTunerInOfflineMode( aEnable );
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetFrequencyRange(
    TFmRadioFrequencyRange& aRange,
    TInt& aMinFreq,
    TInt& aMaxFreq ) const
    {
    FUNC_LOG;

    TRsFrequencyRange range;
    TInt error = iRadioSession.GetFrequencyRange( range, aMinFreq, aMaxFreq );
    if ( !error )
        {
    switch ( range )
        {
        case ERsRangeFmEuroAmerica:
            aRange = EFmRangeEuroAmerica;
            break;
        case ERsRangeFmJapan:
            aRange = EFmRangeJapan;
            break;
        default:
            break;
            }
        }
    INFO_3("aRange = %d, aMinFreq = %d, aMaxFreq = %d", aRange, aMinFreq, aMaxFreq);
    return error;
    }


// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::SetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::SetFrequencyRange(
    TFmRadioFrequencyRange aRange )
    {
    FUNC_LOG;
    INFO_1("aRange = %d", aRange);
    TRsFrequencyRange range = ERsRangeFmEuroAmerica;    //default
    switch (aRange)
        {
        case EFmRangeEuroAmerica:
            range = ERsRangeFmEuroAmerica;
            break;
        case EFmRangeJapan:
            range = ERsRangeFmJapan;
            break;
        default:
            break;
        }
    iRadioSession.SetFrequencyRange( range );
    }


// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::CancelSetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::CancelSetFrequencyRange()
    {
    FUNC_LOG;
    iRadioSession.CancelSetFrequencyRange();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::SetFrequency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::SetFrequency(
    TInt aFrequency )
    {
    FUNC_LOG;
    INFO_1("aFrequency = %d", aFrequency);
    iRadioSession.SetFrequency(aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::CancelSetFrequencyRange
// Body of CancelSetFrequencyRange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::CancelSetFrequency()
    {
    FUNC_LOG;
    iRadioSession.CancelSetFrequency();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetFrequency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetFrequency(
    TInt& aFrequency ) const
    {
    FUNC_LOG;
    return iRadioSession.GetFrequency(aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::StationSeek
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::StationSeek(
    TBool aUpwards )
    {
    FUNC_LOG;
    INFO_1("aUpwards = %d", aUpwards);
    iRadioSession.StationSeek(aUpwards);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::CancelStationSeek
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioFmTunerUtility::CBody::CancelStationSeek()
    {
    FUNC_LOG;
    iRadioSession.CancelStationSeek();
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetSignalStrength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetSignalStrength(
    TInt& aSignalStrength ) const
    {
    FUNC_LOG;
    return iRadioSession.GetSignalStrength(aSignalStrength);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::GetMaxSignalStrength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetMaxSignalStrength(
    TInt& aMaxSignalStrength ) const
    {
    FUNC_LOG;
    return iRadioSession.GetMaxSignalStrength(aMaxSignalStrength);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetStereoMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetStereoMode(
    TBool& aStereo ) const
    {
    FUNC_LOG;
    return iRadioSession.GetStereoMode(aStereo);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::ForceMonoReception
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::ForceMonoReception(
    TBool aForcedMono)
    {
    FUNC_LOG;
    return iRadioSession.ForceMonoReception(aForcedMono);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetForcedMonoReception
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetForcedMonoReception(
    TBool& aForcedMono ) const
    {
    FUNC_LOG;
    return iRadioSession.GetForceMonoReception(aForcedMono);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::SetSquelch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::SetSquelch(
    TBool aSquelch )
    {
    FUNC_LOG;
    return iRadioSession.SetSquelch(aSquelch);
    }

// -----------------------------------------------------------------------------
// CRadioFmTunerUtility::CBody::GetSquelch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioFmTunerUtility::CBody::GetSquelch(
    TBool& aSquelch ) const
    {
    FUNC_LOG;
    return iRadioSession.GetSquelch(aSquelch);
    }

// End of File





