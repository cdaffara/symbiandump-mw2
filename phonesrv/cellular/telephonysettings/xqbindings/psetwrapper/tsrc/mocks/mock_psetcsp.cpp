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
#include <psetcsp.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::NewL
// -----------------------------------------------------------------------------
//
CPsetCustomerServiceProfile * CPsetCustomerServiceProfile::NewL(  )
    {
    SMC_MOCK_NEWL0( CPsetCustomerServiceProfile * )

    CPsetCustomerServiceProfile* self = new( ELeave ) CPsetCustomerServiceProfile;
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::~CPsetCustomerServiceProfile
// -----------------------------------------------------------------------------
//
CPsetCustomerServiceProfile::~CPsetCustomerServiceProfile(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsCWSupported
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::IsCWSupported( 
        TBool & aSupport )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aSupport )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsCBSupported
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::IsCBSupported( 
        TBool & aSupport )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aSupport )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsCFSupported
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::IsCFSupported( 
        TBool & aSupport )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aSupport )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsALSSupported
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::IsALSSupported( 
        TBool & aSupport )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aSupport )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::OpenCSProfileL
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::OpenCSProfileL(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsNetworkSelectionSupported
// -----------------------------------------------------------------------------
//
TInt CPsetCustomerServiceProfile::IsNetworkSelectionSupported( 
        TBool & aSupport )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aSupport )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCWSupported
// -----------------------------------------------------------------------------
//
TBool CPsetCustomerServiceProfile::CheckIfCWSupported( 
        const RMobilePhone::TCspCallCompletion aContainer ) const
    {
    SMC_MOCK_METHOD1( TBool, const RMobilePhone::TCspCallCompletion, aContainer )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCBSupported
// -----------------------------------------------------------------------------
//
TBool CPsetCustomerServiceProfile::CheckIfCBSupported( 
        const RMobilePhone::TCspCallRestriction aContainer ) const
    {
    SMC_MOCK_METHOD1( TBool, const RMobilePhone::TCspCallRestriction, aContainer )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCFSupported
// -----------------------------------------------------------------------------
//
TBool CPsetCustomerServiceProfile::CheckIfCFSupported( 
        const RMobilePhone::TCspCallOffering aContainer ) const
    {
    SMC_MOCK_METHOD1( TBool, const RMobilePhone::TCspCallOffering, aContainer )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfAlsSupported
// -----------------------------------------------------------------------------
//
TBool CPsetCustomerServiceProfile::CheckIfAlsSupported( 
        const RMobilePhone::TCspCPHSTeleservices aContainer ) const
    {
    SMC_MOCK_METHOD1( TBool, const RMobilePhone::TCspCPHSTeleservices, aContainer )
    }


// -----------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported
// -----------------------------------------------------------------------------
//
TBool CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported( 
        const RMobilePhone::TCspValueAdded aContainer ) const
    {
    SMC_MOCK_METHOD1( TBool, const RMobilePhone::TCspValueAdded, aContainer )
    }


