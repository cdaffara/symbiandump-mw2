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
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include "secuimanualsecuritysettings.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CManualSecuritySettings::CManualSecuritySettings
// -----------------------------------------------------------------------------
//
CManualSecuritySettings::CManualSecuritySettings(  )
    //:
    //CBase( /**/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::ConstructL
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::NewL
// -----------------------------------------------------------------------------
//
CManualSecuritySettings * CManualSecuritySettings::NewL(  )
    {
    SMC_MOCK_NEWL0( CManualSecuritySettings * )

    CManualSecuritySettings* self = new( ELeave ) CManualSecuritySettings;
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::~CManualSecuritySettings
// -----------------------------------------------------------------------------
//
CManualSecuritySettings::~CManualSecuritySettings(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::ChangePinL
// -----------------------------------------------------------------------------
//
TBool CManualSecuritySettings::ChangePinL( 
        TPin aPin,
        const TDesC & aOld,
        const TDesC & aNew,
        const TDesC & aVerifyNew )
    {
    SMC_MOCK_METHOD4( TBool, TPin, aPin, 
        const TDesC &, aOld, 
        const TDesC &, aNew, 
        const TDesC &, aVerifyNew )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::CancelChangePin
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::CancelChangePin(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::UnblockPinL
// -----------------------------------------------------------------------------
//
TBool CManualSecuritySettings::UnblockPinL( 
        TPin aPin,
        const TDesC & aPuk,
        const TDesC & aNew,
        const TDesC & aVerifyNew )
    {
    SMC_MOCK_METHOD4( TBool, TPin, aPin, 
        const TDesC &, aPuk, 
        const TDesC &, aNew, 
        const TDesC &, aVerifyNew )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::CancelUnblockPin
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::CancelUnblockPin(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::LockSimL
// -----------------------------------------------------------------------------
//
TBool CManualSecuritySettings::LockSimL( 
        const TDesC & aLockCode,
        const TDesC & aType )
    {
    SMC_MOCK_METHOD2( TBool, const TDesC &, aLockCode, 
        const TDesC &, aType )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::CancelLockSim
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::CancelLockSim(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::UnlockSimL
// -----------------------------------------------------------------------------
//
TBool CManualSecuritySettings::UnlockSimL( 
        const TDesC & aUnlockCode,
        const TDesC & aType )
    {
    SMC_MOCK_METHOD2( TBool, const TDesC &, aUnlockCode, 
        const TDesC &, aType )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::CancelUnlockSim
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::CancelUnlockSim(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CManualSecuritySettings::ShowResultNoteL
// -----------------------------------------------------------------------------
//
void CManualSecuritySettings::ShowResultNoteL( 
        CAknNoteDialog::TTone aTone,
        TInt aResourceID )
    {
    SMC_MOCK_METHOD2( void, CAknNoteDialog::TTone, aTone, 
        TInt, aResourceID )
    }


