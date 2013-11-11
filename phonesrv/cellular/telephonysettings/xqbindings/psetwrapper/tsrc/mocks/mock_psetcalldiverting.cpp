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
#include <etelmm.h>           
#include <e32math.h>
#include <e32svr.h>
#include <badesca.h>
#include <centralrepository.h>
#include <SettingsInternalCRKeys.h>
#include <mpsetrequestobs.h>
#include <mpsetdivertobs.h>
#include "PsetCallDiverting.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPsetCallDiverting::CPsetCallDiverting
// -----------------------------------------------------------------------------
//
CPsetCallDiverting::CPsetCallDiverting( 
        RMobilePhone & aPhone ) : iPhone(aPhone)
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::ConstructL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::ConstructL( 
        MPsetDivertObserver & /*aObserver*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::NewL
// -----------------------------------------------------------------------------
//
CPsetCallDiverting * CPsetCallDiverting::NewL( 
        MPsetDivertObserver & aObserver,
        RMobilePhone & aPhone )
    {
    SMC_MOCK_NEWL2( CPsetCallDiverting *, MPsetDivertObserver &, aObserver, 
        RMobilePhone &, aPhone )

    CPsetCallDiverting* self = new( ELeave ) CPsetCallDiverting( aPhone );
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::~CPsetCallDiverting
// -----------------------------------------------------------------------------
//
CPsetCallDiverting::~CPsetCallDiverting(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetDivertingL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::SetDivertingL( 
        const TCallDivertSetting & aDivert,
        TBasicServiceGroups aBsc,
        TBool aVmbx )
    {
    SMC_MOCK_METHOD3( void, const TCallDivertSetting &, aDivert, 
        TBasicServiceGroups, aBsc, 
        TBool, aVmbx )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::GetDivertingStatusL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::GetDivertingStatusL( 
        const TServiceGroup aServiceGroup,
        const TCallDivertingCondition aCondition,
        TBasicServiceGroups aBsc )
    {
    SMC_MOCK_METHOD3( void, const TServiceGroup, aServiceGroup, 
        const TCallDivertingCondition, aCondition, 
        TBasicServiceGroups, aBsc )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::CancelCurrentRequest
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::CancelCurrentRequest(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::GetTimerValueL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::GetTimerValueL(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetTimerValueL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::SetTimerValueL( 
        const TInt & aIndex )
    {
    SMC_MOCK_METHOD1( TInt, const TInt &, aIndex )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::GetDefaultNumbersL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::GetDefaultNumbersL( 
        CDesC16ArrayFlat & aDefNumbers )
    {
    _LIT( KNumber1,"111111" );
    _LIT( KNumber2,"222222" );
    
    aDefNumbers.AppendL(KNumber1());
    aDefNumbers.AppendL(KNumber2());

    SMC_MOCK_METHOD1( TInt, CDesC16ArrayFlat &, aDefNumbers )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetNewDefaultNumberL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::SetNewDefaultNumberL( 
        TDes & aNumber )
    {
    SMC_MOCK_METHOD1( TInt, TDes &, aNumber )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SwapDefaultNumberL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::SwapDefaultNumberL( 
        const TInt & aLocation )
    {
    SMC_MOCK_METHOD1( void, const TInt &, aLocation )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetUsedDataNumberL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::SetUsedDataNumberL( 
        TTelNumber & aNumber )
    {
    SMC_MOCK_METHOD1( TInt, TTelNumber &, aNumber )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetUsedFaxNumberL
// -----------------------------------------------------------------------------
//
TInt CPsetCallDiverting::SetUsedFaxNumberL( 
        TTelNumber & aNumber )
    {
    SMC_MOCK_METHOD1( TInt, TTelNumber &, aNumber )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::GetUsedFaxNumberLC
// -----------------------------------------------------------------------------
//
HBufC * CPsetCallDiverting::GetUsedFaxNumberLC(  )
    {
    SMC_MOCK_METHOD0( HBufC * )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::GetUsedDataNumberLC
// -----------------------------------------------------------------------------
//
HBufC * CPsetCallDiverting::GetUsedDataNumberLC(  )
    {
    SMC_MOCK_METHOD0( HBufC * )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::SetRequestObserver
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::SetRequestObserver( 
        MPsetRequestObserver * aObs )
    {
    SMC_MOCK_METHOD1( void, MPsetRequestObserver *, aObs )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::VoiceMailQueryL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::VoiceMailQueryL( 
        TDes & aTelNumber )
    {
    SMC_MOCK_METHOD1( void, TDes &, aTelNumber )
    }


// -----------------------------------------------------------------------------
// CPsetCallDiverting::VideoMailQueryL
// -----------------------------------------------------------------------------
//
void CPsetCallDiverting::VideoMailQueryL( 
        TDes & aTelNumber )
    {
    SMC_MOCK_METHOD1( void, TDes &, aTelNumber )
    }


