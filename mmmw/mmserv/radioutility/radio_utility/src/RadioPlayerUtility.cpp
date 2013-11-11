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
* Description:  Radio Player Utility interface implementation
*
*/


#include <RadioPlayerUtility.h>
#include "RadioPlayerUtilityBody.h"
#include "RadioSession.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioPlayerUtility* CRadioPlayerUtility::NewL(
     RRadioSession& aRadioSession,
     MRadioPlayerObserver& aObserver)
    {
    FUNC_LOG;
    CRadioPlayerUtility* self = new(ELeave) CRadioPlayerUtility;
    CleanupStack::PushL(self);
    self->iBody = CRadioPlayerUtility::CBody::NewL(aRadioSession, aObserver);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CRadioPlayerUtility
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioPlayerUtility::~CRadioPlayerUtility()
    {
    FUNC_LOG;
    delete iBody;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CRadioPlayerUtility
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioPlayerUtility::CRadioPlayerUtility()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::PlayerState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TPlayerState CRadioPlayerUtility::PlayerState() const
    {
    FUNC_LOG;
    TPlayerState playerState;
    ASSERT(iBody);
    iBody->PlayerState(playerState);

    return playerState;
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::Close
// Abstraction of Close
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioPlayerUtility::Close()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->Close();
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::Play
// Abstraction of Play
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioPlayerUtility::Play()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->Play();
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::Stop
// Abstraction of Stop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CRadioPlayerUtility::Stop()
    {
    FUNC_LOG;
    ASSERT(iBody);
    iBody->Stop();
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::Mute
// Abstraction of Mute
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::Mute(TBool aMute)
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->Mute(aMute);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::IsMute
// Abstraction of IsMute
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRadioPlayerUtility::IsMute()
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->IsMute();
    }


// -----------------------------------------------------------------------------
// CRadioPlayerUtility::SetVolume
// Abstraction of SetVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::SetVolume(TInt aVolume)
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetVolume(aVolume);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::GetVolume
// Abstraction of GetVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::GetVolume(TInt& aVolume) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetVolume(aVolume);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::SetVolumeRamp
// Abstraction of SetVolumeRamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::SetVolumeRamp(
    const TTimeIntervalMicroSeconds& aRampInterval)
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetVolumeRamp(aRampInterval);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::GetMaxVolume
// Abstraction of Utility MaxVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::GetMaxVolume(
    TInt& aMaxVolume ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetMaxVolume(aMaxVolume);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::SetBalance
// Abstraction of SetBalance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::SetBalance(
    TInt aLeftPercentage,
    TInt aRightPercentage )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->SetBalance(aLeftPercentage, aRightPercentage);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::GetBalance
// Abstraction of GetBalance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRadioPlayerUtility::GetBalance(
    TInt& aLeftPercentage,
    TInt& aRightPercentage ) const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->GetBalance(aLeftPercentage, aRightPercentage);
    }

// -----------------------------------------------------------------------------
// CRadioPlayerUtility::CustomInterface
// Abstraction of CustomInterface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* CRadioPlayerUtility::CustomInterface(
    TUid aInterfaceId )
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->CustomInterface(aInterfaceId);
    }



// End of File
