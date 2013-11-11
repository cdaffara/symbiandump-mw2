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
#include <rsssettings.h> 
#include <PsetCallBarring.h>
#include <mpsetrequestobs.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPsetCallBarring::CPsetCallBarring
// -----------------------------------------------------------------------------
//
CPsetCallBarring::CPsetCallBarring( 
        RMobilePhone & aPhone )
    :
	CActive(EPriorityStandard),
	iPhone(aPhone)
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::ConstructL
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::ConstructL( 
        MPsetBarringObserver & /*aObserver*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::NewL
// -----------------------------------------------------------------------------
//
CPsetCallBarring * CPsetCallBarring::NewL( 
        MPsetBarringObserver & aObserver,
        RMobilePhone & aPhone )
    {
    SMC_MOCK_NEWL2( CPsetCallBarring *, MPsetBarringObserver &, aObserver, 
        RMobilePhone &, aPhone )

    CPsetCallBarring* self = new( ELeave ) CPsetCallBarring(aPhone);
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::~CPsetCallBarring
// -----------------------------------------------------------------------------
//
CPsetCallBarring::~CPsetCallBarring(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::SetBarringL
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::SetBarringL( 
        const TCallBarringSetting & aBarring,
        TBasicServiceGroups aBsc )
    {
    SMC_MOCK_METHOD2( void, const TCallBarringSetting &, aBarring, 
        TBasicServiceGroups, aBsc )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::GetBarringStatusL
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::GetBarringStatusL( 
        const TServiceGroup aGroup,
        const TBarringProgram aMode )
    {
    SMC_MOCK_METHOD2( void, const TServiceGroup, aGroup, 
        const TBarringProgram, aMode )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::CancelCurrentRequest
// -----------------------------------------------------------------------------
//
TInt CPsetCallBarring::CancelCurrentRequest(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::PhoneSettingChanged
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::PhoneSettingChanged( 
        TSSSettingsSetting aSetting,
        TInt aNewValue )
    {
    SMC_MOCK_METHOD2( void, TSSSettingsSetting, aSetting, 
        TInt, aNewValue )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::ChangePasswordL
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::ChangePasswordL( 
        RMobilePhone::TMobilePhonePasswordChangeV2 & aPwds,
        TBool aIsBarringPassword )
    {
    SMC_MOCK_METHOD2( void, RMobilePhone::TMobilePhonePasswordChangeV2 &, aPwds, 
        TBool, aIsBarringPassword )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::ChangePasswordL
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::ChangePasswordL( 
        RMobilePhone::TMobilePhonePasswordChangeV2 & aPwds )
    {
    SMC_MOCK_METHOD1( void, RMobilePhone::TMobilePhonePasswordChangeV2 &, aPwds )
    }


// -----------------------------------------------------------------------------
// CPsetCallBarring::SetRequestObserver
// -----------------------------------------------------------------------------
//
void CPsetCallBarring::SetRequestObserver( 
        MPsetRequestObserver * aObs )
    {
    SMC_MOCK_METHOD1( void, MPsetRequestObserver *, aObs )
    }


// ---------------------------------------------------------------------------
// CPsetCallBarring::DoCancel
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::DoCancel()
    {    

    }
// ---------------------------------------------------------------------------
// CPsetCallBarring::RunL

// ---------------------------------------------------------------------------
//
void CPsetCallBarring::RunL()
    {
    }
