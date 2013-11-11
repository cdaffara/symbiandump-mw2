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
* Description:  Radio Utility body implementation
*
*/


#include <RadioFmTunerUtility.h>
#include <RadioPlayerUtility.h>
#include <RadioRdsUtility.h>
#include "RadioUtilityBody.h"
#include "RadioServerData.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioUtility::CBody* CRadioUtility::CBody::NewL(
    TBool aPrimaryClient )
    {
    FUNC_LOG;
    CRadioUtility::CBody* self = new (ELeave) CRadioUtility::CBody();
    CleanupStack::PushL(self);
    self->ConstructL(aPrimaryClient);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioUtility::CBody::ConstructL(
    TBool aPrimaryClient )
    {
    FUNC_LOG;
    INFO_1("Size of RRadioSession: %i", sizeof(RRadioSession) );
    iRadioSession = RRadioSession::NewL();
    User::LeaveIfError(iRadioSession->Connect(*this, aPrimaryClient));
    }

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::CBody
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioUtility::CBody::CBody()
    :   iRadioSession(NULL),
        iFmTunerUtility(NULL),
        iPlayerUtility(NULL),
        iRdsUtility(NULL)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CRadioUtility::CBody::~CBody()
    {
    FUNC_LOG;
    delete iFmTunerUtility;
    delete iPlayerUtility;
    delete iRdsUtility;
    if ( iRadioSession )
        {
        iRadioSession->Close();
        delete iRadioSession;
        }
    }

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::RadioFmTunerUtilityL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioFmTunerUtility& CRadioUtility::CBody::RadioFmTunerUtilityL(
    MRadioFmTunerObserver& aObserver)
    {
    FUNC_LOG;

    if ( !iFmTunerUtility )
        {
        iFmTunerUtility = CRadioFmTunerUtility::NewL( *iRadioSession, aObserver );
        iTunerObserver = &aObserver;
        }
    else
        {
        iTunerObserver = &aObserver;
        }

    return *iFmTunerUtility;
    }

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::RadioPlayerUtilityL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioPlayerUtility& CRadioUtility::CBody::RadioPlayerUtilityL(
    MRadioPlayerObserver& aObserver)
    {
    FUNC_LOG;

    if ( !iPlayerUtility )
        {
        iPlayerUtility = CRadioPlayerUtility::NewL( *iRadioSession, aObserver );
        iPlayerObserver = &aObserver;
        }
    else
        {
        iPlayerObserver = &aObserver;
        }

    return *iPlayerUtility;
    }

// -----------------------------------------------------------------------------
// CRadioUtility::CBody::RadioRdsUtilityL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioRdsUtility& CRadioUtility::CBody::RadioRdsUtilityL(
    MRadioRdsObserver& aObserver)
    {
    FUNC_LOG;

    if ( !iRdsUtility )
        {
        iRdsUtility = CRadioRdsUtility::NewL( *iRadioSession, aObserver );
        iRdsObserver = &aObserver;
        }
    else
        {
        iRdsObserver = &aObserver;
        }

    return *iRdsUtility;
    }


// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of Tuner Control Request.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RequestTunerControlComplete(
    TRadioServerError aError )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoRequestTunerControlComplete( aError );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of Set Frequency Range Request.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::SetFrequencyRangeComplete(
    TRadioServerError aError )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoSetFrequencyRangeComplete( aError );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of Set Frequency Request.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::SetFrequencyComplete(
    TRadioServerError aError )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoSetFrequencyComplete( aError );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of Station Seek Request.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::StationSeekComplete(
    TRadioServerError aError,
    TInt aFrequency )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoStationSeekComplete( aError, aFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles transmitter status change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventTransmitterStatusChange(
    TBool aActive )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoFmTransmitterStatusChange( aActive );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles antenna status change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventAntennaStatusChange(
    TBool aAttached )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoAntennaStatusChange( aAttached );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles offline mode change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventOfflineModeChange(
    TBool aOfflineMode )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoOfflineModeStatusChange( aOfflineMode );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles frequency range change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventFrequencyRangeChanged(
    TRsFrequencyRange aNewRange )
    {
    FUNC_LOG;

    if ( iTunerObserver )
        {
        TFmRadioFrequencyRange band = EFmRangeEuroAmerica;

        switch (aNewRange)
            {
            case ERsRangeFmEuroAmerica:
                band = EFmRangeEuroAmerica;
                break;
            case ERsRangeFmJapan:
                band = EFmRangeJapan;
                break;
            default:
                break;
            }
        iTunerObserver->MrftoFrequencyRangeChange( band );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles frequency change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventFrequencyChange(
    TInt aNewFrequency )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoFrequencyChange( aNewFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles forced mono change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventForcedMonoChanged(
    TBool aForceMono )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoForcedMonoChange( aForceMono );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles squelch change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventSquelchChanged(
    TBool aSquelch )
    {
    FUNC_LOG;
    if ( iTunerObserver )
        {
        iTunerObserver->MrftoSquelchChange( aSquelch );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles radio state change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventStateChange(
    TBool aRadioOn,
    TRadioServerError aError )
    {
    FUNC_LOG;
    if ( iPlayerObserver )
        {
        TPlayerState state = ERadioPlayerIdle;

        if ( aRadioOn )
            {
            state = ERadioPlayerPlaying;
            }
        iPlayerObserver->MrpoStateChange( state, (TInt)aError );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles volume change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventVolumeChange(
    TInt aVolume )
    {
    FUNC_LOG;
    if ( iPlayerObserver )
        {
        iPlayerObserver->MrpoVolumeChange( aVolume );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles mute change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventMuteChange(
    TBool aMute )
    {
    FUNC_LOG;
    if ( iPlayerObserver )
        {
        iPlayerObserver->MrpoMuteChange( aMute );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles balance change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventBalanceChange(
    TInt aLeftPercentage,
    TInt aRightPercentage )
    {
    FUNC_LOG;
    if ( iPlayerObserver )
        {
        iPlayerObserver->MrpoBalanceChange( aLeftPercentage, aRightPercentage );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of station seek by PTY.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::StationSeekByPTYComplete(
    TInt aError,
    TInt aFrequency )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroStationSeekByPTYComplete(aError, aFrequency );
        }
    }


// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of station seek by TA.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::StationSeekByTAComplete(
    TInt aError,
    TInt aFrequency )
    {
    FUNC_LOG;

    if ( iRdsObserver )
        {
        iRdsObserver->MrroStationSeekByTAComplete( aError, aFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of station seek by TP.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::StationSeekByTPComplete(
    TInt aError,
    TInt aFrequency )
    {
    FUNC_LOG;

    if ( iRdsObserver )
        {
        iRdsObserver->MrroStationSeekByTPComplete( aError, aFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of get frequency by PTY.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::GetFreqByPTYComplete(
    TInt aError,
    RArray<TInt>& aFreqList )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroGetFreqByPTYComplete( aError, aFreqList );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of get frequency by TA.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::GetFreqByTAComplete(
    TInt aError,
    RArray<TInt>& aFreqList )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroGetFreqByTAComplete( aError, aFreqList );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of get PS by PTY.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::GetPSByPTYComplete(
    TInt aError,
    RArray<TRsRdsPSName>& aPsList )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        RArray<TRdsPSName> psList;
        if ( aError == KErrNone )
            {
            for( TInt i = 0; i < aPsList.Count(); i++ )
                {
                TRsRdsPSName ps = aPsList[i];
                if ( ps.Length() )
                    {
                    psList[i].Copy(ps);
                    }
                }
            }
        iRdsObserver->MrroGetPSByPTYComplete( aError, psList );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles completion of get PS by TA.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::GetPSByTAComplete(
    TInt aError,
    RArray<TRsRdsPSName>& aPsList )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        RArray<TRdsPSName> psList;
        if ( aError == KErrNone )
            {
            for( TInt i = 0; i < aPsList.Count(); i++ )
                {
                TRsRdsPSName ps = aPsList[i];
                if ( ps.Length() )
                    {
                    psList[i].Copy(ps);
                    }
                }
            }
        iRdsObserver->MrroGetPSByTAComplete( aError, psList );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles program identification RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataPI(
    TInt aPi )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsDataPI( aPi );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles program type RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataPTY(
    TRsRdsProgrammeType aPty )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsDataPTY( aPty );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles program service name RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataPS(
    TRsRdsPSName& aPs )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        TRdsPSName ps;

        if ( aPs.Length() > 0 )
            {
            ps.Copy(aPs);
            }
        iRdsObserver->MrroRdsDataPS( ps );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles radio text RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataRT(
    TRsRdsRadioText& aRt )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        TRdsRadioText rt;

        if ( aRt.Length() > 0 )
            {
            rt.Copy( aRt );
            }

        iRdsObserver->MrroRdsDataRT( rt );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles radio text plus RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataRTplus(
    TRsRdsRTplusClass aRtPlusClass,
    TRsRdsRadioText& aRtPlusData )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        TRdsRTplusClass rtPlusClass;
        TRdsRadioText rtPlusData;

        rtPlusClass = (TRdsRTplusClass)aRtPlusClass;

        if ( aRtPlusData.Length() > 0 )
            {
            rtPlusData.Copy( aRtPlusData );
            }
        iRdsObserver->MrroRdsDataRTplus( rtPlusClass, rtPlusData );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles clock time RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataCT(
    TDateTime& aCt )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsDataCT( aCt );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles traffic annoucement RDS data event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsDataTA(
    TBool aTaOn )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsDataTA( aTaOn );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles alternate frequency search begin event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsSearchBeginAF()
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsSearchBeginAF();
        }
    }


// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles alternate frequency search end event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsSearchEndAF(
    TInt aError,
    TInt aFrequency )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsSearchEndAF( aError, aFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles station change to traffic annoucement event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsStationChangeTA(
    TInt aFrequency )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsStationChangeTA( aFrequency );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles automatic switching setting change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsAutomaticSwitchingChange(
    TBool aAuto )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsEventAutomaticSwitchingChange( aAuto );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles automatic traffic annoucement setting change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsAutomaticTrafficAnnouncement(
    TBool aAuto )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsEventAutomaticTrafficAnnouncement( aAuto );
        }
    }

// ---------------------------------------------------------------------------
// From class MRadioObserver.
// Handles RDS signal status change event.
// ---------------------------------------------------------------------------
//
void CRadioUtility::CBody::RadioEventRdsSignalChange(
    TBool aSignal )
    {
    FUNC_LOG;
    if ( iRdsObserver )
        {
        iRdsObserver->MrroRdsEventSignalChange( aSignal );
        }
    }


//End of File

