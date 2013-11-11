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
#include "PsetCli.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPsetCli::CPsetCli
// -----------------------------------------------------------------------------
//
CPsetCli::CPsetCli( 
        RMobilePhone & aPhone )
    :
        CActive( EPriorityStandard ),
        iPhone( aPhone )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCli::NewL
// -----------------------------------------------------------------------------
//
CPsetCli * CPsetCli::NewL( 
        RMobilePhone & aPhone,
        MPsetCliObserver & aObserver )
    {
    SMC_MOCK_NEWL2( CPsetCli *, RMobilePhone &, aPhone, 
        MPsetCliObserver &, aObserver )

    CPsetCli* self = new( ELeave ) CPsetCli(aPhone);
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPsetCli::~CPsetCli
// -----------------------------------------------------------------------------
//
CPsetCli::~CPsetCli(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCli::GetColpModeL
// -----------------------------------------------------------------------------
//
void CPsetCli::GetColpModeL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::GetClipModeL
// -----------------------------------------------------------------------------
//
void CPsetCli::GetClipModeL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::GetClirModeL
// -----------------------------------------------------------------------------
//
void CPsetCli::GetClirModeL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::GetColrModeL
// -----------------------------------------------------------------------------
//
void CPsetCli::GetColrModeL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::CancelAll
// -----------------------------------------------------------------------------
//
void CPsetCli::CancelAll(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::GetCnapL
// -----------------------------------------------------------------------------
//
void CPsetCli::GetCnapL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CPsetCli::SetRequestObserver
// -----------------------------------------------------------------------------
//
void CPsetCli::SetRequestObserver( 
        MPsetRequestObserver * aObs )
    {
    SMC_MOCK_METHOD1( void, MPsetRequestObserver *, aObs )
    }

// -----------------------------------------------------------------------------
// CPsetCli::RunL
// -----------------------------------------------------------------------------
//
void CPsetCli::RunL()
    {
        
    }

// -----------------------------------------------------------------------------
// CPsetCli::DoCancel
// -----------------------------------------------------------------------------
//
void CPsetCli::DoCancel()
    {
        
    }
