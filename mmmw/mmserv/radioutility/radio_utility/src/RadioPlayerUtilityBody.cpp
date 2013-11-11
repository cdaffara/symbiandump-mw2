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
* Description:  Radio Player Utility body implementation
*
*/


#include <CustomInterfaceUtility.h>
#include "RadioServerData.h"
#include "RadioPlayerUtilityBody.h"
#include "RadioSession.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CRadioPlayerUtility::CBody* CRadioPlayerUtility::CBody::NewL(
    RRadioSession& aRadioSession,
    MRadioPlayerObserver& aObserver )
    {
    FUNC_LOG;
    CRadioPlayerUtility::CBody* s = new(ELeave) CRadioPlayerUtility::CBody( aRadioSession, aObserver );
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop();
    return s;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CRadioPlayerUtility::CBody::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::CBody
// -----------------------------------------------------------------------------
//
CRadioPlayerUtility::CBody::CBody(
        RRadioSession& aRadioSession,
        MRadioPlayerObserver& aObserver )
    :iRadioSession(aRadioSession),
    iRadioPlayerUtilityClient(aObserver)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CRadioPlayerUtility::CBody::~CBody()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::PlayerState
// Get the player's state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
//
void CRadioPlayerUtility::CBody::PlayerState(
    TPlayerState& aPlayerState ) const
    {
    FUNC_LOG;
    TRsPlayerState playerState;

    if ( !iRadioSession.PlayerState( playerState ) )
        {
        aPlayerState = (TPlayerState)playerState;
        }
    else
        {
        aPlayerState = ERadioPlayerIdle;
        }
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::Close
// Stops radio playback only if no other primary clients are connected to the
// radio server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioPlayerUtility::CBody::Close()
    {
    FUNC_LOG;
    iRadioSession.Stop( ETrue );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::Play
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioPlayerUtility::CBody::Play()
    {
    FUNC_LOG;
    iRadioSession.Play();
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::Stop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRadioPlayerUtility::CBody::Stop()
    {
    FUNC_LOG;
    iRadioSession.Stop();
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::Mute
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::Mute(
    TBool aMute )
    {
    FUNC_LOG;
    return iRadioSession.Mute( aMute );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::IsMute
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRadioPlayerUtility::CBody::IsMute()
    {
    FUNC_LOG;
    TBool Mute = EFalse;
    iRadioSession.GetMuteStatus( Mute );
    return Mute;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::SetVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::SetVolume(
    TInt aVolume )
    {
    FUNC_LOG;
    return iRadioSession.SetVolume( aVolume );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::GetVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::GetVolume(
    TInt& aVolume ) const
    {
    FUNC_LOG;
    return iRadioSession.GetVolume( aVolume );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::SetVolumeRamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::SetVolumeRamp(
    const TTimeIntervalMicroSeconds& aRampInterval )
    {
    FUNC_LOG;
    return iRadioSession.SetVolumeRamp( aRampInterval );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::GetMaxVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::GetMaxVolume(
    TInt& aMaxVolume ) const
    {
    FUNC_LOG;
    return iRadioSession.GetMaxVolume( aMaxVolume );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::SetBalance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::SetBalance(
    TInt aLeftPercentage,
    TInt aRightPercentage )
    {
    FUNC_LOG;
    INFO_2("aLeftPercentage = %d, aRightPercentage = %d", aLeftPercentage, aRightPercentage);
    return iRadioSession.SetBalance( aLeftPercentage, aRightPercentage );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::GetBalance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRadioPlayerUtility::CBody::GetBalance(
    TInt& aLeftPercentage,
    TInt& aRightPercentage ) const
    {
    FUNC_LOG;
    return iRadioSession.GetBalance( aLeftPercentage, aRightPercentage );
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CBody::CustomInterface
// Get a custom interface for the specified interface Id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TAny* CRadioPlayerUtility::CBody::CustomInterface(
    TUid aInterfaceId )
    {
    FUNC_LOG;
    INFO_1("aInterfaceId = %d", aInterfaceId);
    TAny* ci = NULL;
    CCustomInterfaceUtility* customInterfaceUtil = NULL;

    TRAPD( error, customInterfaceUtil = CCustomInterfaceUtility::NewL( iRadioSession ) );

    if ( !error )
        {
        ci = customInterfaceUtil->CustomInterface( aInterfaceId );
        if ( !ci )
            {
            // The custom interface utility is owned by the custom interface, implicitly.
            // The custom interface utility instance is destroyed when the
            // custom interface is finally destroyed!
            // But if there's no valid ci, we need to take care of deleting the
            // custom interface utility instance.
            delete customInterfaceUtil;
            }
        }
    return ci;
    }

//End of File
