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
#include <mpsetrequestobs.h>
#include "PsetCallWaiting.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPsetCallWaiting::CPsetCallWaiting
// -----------------------------------------------------------------------------
//
CPsetCallWaiting::CPsetCallWaiting( 
        RMobilePhone & aPhone )
    :
        CActive( EPriorityStandard ),
        iPhone(aPhone)
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::ConstructL
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::ConstructL( 
        MPsetCallWaitingObserver & /*aObserver*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::NewL
// -----------------------------------------------------------------------------
//
CPsetCallWaiting * CPsetCallWaiting::NewL( 
        RMobilePhone & aPhone,
        MPsetCallWaitingObserver & aObserver )
    {
    SMC_MOCK_NEWL2( CPsetCallWaiting *, RMobilePhone &, aPhone, 
        MPsetCallWaitingObserver &, aObserver )

    CPsetCallWaiting* self = new( ELeave ) CPsetCallWaiting(aPhone);
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::~CPsetCallWaiting
// -----------------------------------------------------------------------------
//
CPsetCallWaiting::~CPsetCallWaiting(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::SetCallWaitingL
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::SetCallWaitingL( 
        TSetCallWaiting aSetting,
        TBasicServiceGroups aBsc )
    {
    SMC_MOCK_METHOD2( void, TSetCallWaiting, aSetting, 
        TBasicServiceGroups, aBsc )
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::GetCallWaitingStatusL
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::GetCallWaitingStatusL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::CancelProcess
// -----------------------------------------------------------------------------
//
TInt CPsetCallWaiting::CancelProcess(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::SetRequestObserver
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::SetRequestObserver( 
        MPsetRequestObserver * aObs )
    {
    SMC_MOCK_METHOD1( void, MPsetRequestObserver *, aObs )
    }


// -----------------------------------------------------------------------------
// CPsetCallWaiting::PhoneSettingChanged
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::PhoneSettingChanged( 
        TSSSettingsSetting aSetting,
        TInt aNewValue )
    {
    SMC_MOCK_METHOD2( void, TSSSettingsSetting, aSetting, 
        TInt, aNewValue )
    }

// -----------------------------------------------------------------------------
// CPsetCallWaiting::RunL
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::RunL()
{
    
}

// -----------------------------------------------------------------------------
// CPsetCallWaiting::DoCancel
// -----------------------------------------------------------------------------
//
void CPsetCallWaiting::DoCancel()
{
    
}
