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
* Description:  Radio RDS Utility API implementation
*
*/


#include "RadioRdsUtilityBody.h"
#include "trace.h"

// -----------------------------------------------------------------------------
// CRadioRdsUtility::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioRdsUtility* CRadioRdsUtility::NewL(
    RRadioSession& aRadioSession,
    MRadioRdsObserver& aObserver)
    {
    FUNC_LOG;
    CRadioRdsUtility* self = new(ELeave) CRadioRdsUtility;
    CleanupStack::PushL(self);
    self->iBody = CRadioRdsUtility::CBody::NewL(aRadioSession, aObserver);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::~CRadioRdsUtility
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioRdsUtility::~CRadioRdsUtility()
    {
    FUNC_LOG;
    delete iBody;
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CRadioRdsUtility
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioRdsUtility::CRadioRdsUtility()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::Close
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::Close()
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->Close();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetCapabilities(
    TRdsCapabilities& aCaps ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetCapabilities(aCaps);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetRdsSignalStatus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetRdsSignalStatus(
    TBool& aRdsSignal ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetRdsSignalStatus( aRdsSignal );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::NotifyRdsDataChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::NotifyRdsDataChange(
    TRdsData aRdsData )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->NotifyRdsDataChange( aRdsData );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelNotifyRdsDataChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelNotifyRdsDataChange()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelNotifyRdsDataChange();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::SetAutomaticSwitching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::SetAutomaticSwitching(
    TBool aAuto )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetAutomaticSwitching(aAuto);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetAutomaticSwitching
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetAutomaticSwitching(
    TBool& aAuto )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetAutomaticSwitching(aAuto);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelAFSearch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelAFSearch()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelAFSearch();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::SetAutomaticTrafficAnnouncement
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::SetAutomaticTrafficAnnouncement(
    TBool aAuto )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetAutomaticTrafficAnnouncement(aAuto);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetAutomaticTrafficAnnouncement
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetAutomaticTrafficAnnouncement(
    TBool& aAuto )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetAutomaticTrafficAnnouncement(aAuto);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::StationSeekByPTY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::StationSeekByPTY(
    TRdsProgrammeType aPty,
    TBool aUpwards )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->StationSeekByPTY(aPty, aUpwards);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::StationSeekByTA
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::StationSeekByTA(
    TBool aSeekUp )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->StationSeekByTA(aSeekUp);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::StationSeekByTP
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::StationSeekByTP(
    TBool aSeekUp )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->StationSeekByTP(aSeekUp);
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelRdsStationSeek
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelRdsStationSeek()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelRdsStationSeek();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetFreqByPTY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::GetFreqByPTY(
    TRdsProgrammeType aPty )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->GetFreqByPTY( aPty );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelGetFreqByPTY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelGetFreqByPTY()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelGetFreqByPTY();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetFreqByTA
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::GetFreqByTA()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->GetFreqByTA();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelGetFreqByTA
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelGetFreqByTA()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelGetFreqByTA();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetPSByPTY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::GetPSByPTY(
    TRdsProgrammeType aPty )
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->GetPSByPTY( aPty );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelGetPSByPTY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelGetPSByPTY()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelGetPSByPTY();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetPSByTA
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::GetPSByTA()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->GetPSByTA();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::CancelGetPSByTA
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C void CRadioRdsUtility::CancelGetPSByTA()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->CancelGetPSByTA();
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetProgrammeIdentification
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetProgrammeIdentification(
    TInt& aPi )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetProgrammeIdentification( aPi );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetProgrammeType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetProgrammeType(
    TRdsProgrammeType& aPty )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetProgrammeType( aPty );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetProgrammeService
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetProgrammeService(
    TRdsPSName& aPs )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetProgrammeService( aPs );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetRadioText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetRadioText(
    TRdsRadioText& aRt )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetRadioText( aRt );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetRadioTextPlus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetRadioTextPlus(
    TRdsRTplusClass aRtPlusClass,
    TRdsRadioText& aRtPlusData )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetRadioTextPlus( aRtPlusClass,aRtPlusData );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetClockTime
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetClockTime(
    TDateTime& aCt )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetClockTime( aCt );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetTrafficAnnouncementStatus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetTrafficAnnouncementStatus(
    TBool& aTaStatus )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetTrafficAnnouncementStatus( aTaStatus );
    }

// -----------------------------------------------------------------------------
// CRadioRdsUtility::GetTrafficProgrammeStatus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TInt CRadioRdsUtility::GetTrafficProgrammeStatus(
    TBool& aTpStatus )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetTrafficProgrammeStatus( aTpStatus );
    }


// End of File
