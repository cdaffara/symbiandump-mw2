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

#include <smcmockclassincludes.h>
#include <mphcltemergencycallobserver.h>
#include "mock_cphcltemergencycall.h"

TBool CPhCltEmergencyCallMock::iSimulateEmergencyNumberMatch = EFalse;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltEmergencyCall::CPhCltEmergencyCall
// C++ constructor.
// -----------------------------------------------------------------------------
//
CPhCltEmergencyCall::CPhCltEmergencyCall( 
        MPhCltEmergencyCallObserver* aObserver )
    : CActive( EPriorityStandard ), iObserver( aObserver )
    {
    
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCall::CPhCltEmergencyCall
// -----------------------------------------------------------------------------
//
CPhCltEmergencyCall* CPhCltEmergencyCall::NewL( 
        MPhCltEmergencyCallObserver* aObserver )
    {
    return CPhCltEmergencyCallMock::NewL( aObserver );
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::CPhCltEmergencyCallMock
// C++ constructor.
// -----------------------------------------------------------------------------
//
CPhCltEmergencyCallMock::CPhCltEmergencyCallMock( 
        MPhCltEmergencyCallObserver * aObserver )
    : CPhCltEmergencyCall( aObserver )
    {
    
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::NewL
// -----------------------------------------------------------------------------
//
CPhCltEmergencyCallMock * CPhCltEmergencyCallMock::NewL( 
        MPhCltEmergencyCallObserver * aObserver )
    {
    SMC_MOCK_NEWL1( CPhCltEmergencyCallMock *, MPhCltEmergencyCallObserver *, aObserver )

    CPhCltEmergencyCallMock* self = new( ELeave ) CPhCltEmergencyCallMock( aObserver );
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::IsEmergencyPhoneNumber
// -----------------------------------------------------------------------------
//
TInt CPhCltEmergencyCallMock::IsEmergencyPhoneNumber( 
        const TDesC & aNumber,
        TBool & aIsEmergencyNumber )
    {
    SMC_MOCK_METHOD2( TInt, const TDesC &, aNumber, 
        TBool &, aIsEmergencyNumber )
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::FindEmergencyPhoneNumber
// -----------------------------------------------------------------------------
//
TInt CPhCltEmergencyCallMock::FindEmergencyPhoneNumber( 
        TDes & aNumber,
        TBool & aIsEmergencyNumber )
    {
    SMC_MOCK_METHOD2( TInt, TDes &, aNumber, 
        TBool &, aIsEmergencyNumber )
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::DialEmergencyCallL
// -----------------------------------------------------------------------------
//
void CPhCltEmergencyCallMock::DialEmergencyCallL( 
        const TDesC & aNumber )
    {
    SMC_MOCK_METHOD1( void, const TDesC &, aNumber )
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::IsEmergencyPhoneNumber
// -----------------------------------------------------------------------------
//
TInt CPhCltEmergencyCallMock::IsEmergencyPhoneNumber( 
        const TDesC & aNumber,
        TDes & aMatchedEmergencyNumber,
        TBool & aIsEmergencyNumber )
    {
    if (iSimulateEmergencyNumberMatch) {
        aIsEmergencyNumber = ETrue;
    }
    
    SMC_MOCK_METHOD3( TInt, const TDesC &, aNumber, 
        TDes &, aMatchedEmergencyNumber, 
        TBool &, aIsEmergencyNumber )
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::RunL
// Declaration needed due to vftable references.
// -----------------------------------------------------------------------------
//
void CPhCltEmergencyCallMock::RunL()
    {
    
    }


// -----------------------------------------------------------------------------
// CPhCltEmergencyCallMock::DoCancel
// Declaration needed due to vftable references.
// -----------------------------------------------------------------------------
//
void CPhCltEmergencyCallMock::DoCancel()
    {
    
    }
