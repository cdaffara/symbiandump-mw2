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
* Description:  Implementation of the CVmbxEmergencyCall class
*
*/

// INCLUDE FILES
#include <cphcltemergencycall.h>
#include <phcltutils.h>
#include "vmbxlogger.h"
#include "vmbxemergencycall.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxEngineBase::CVmbxEngineBase
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxEmergencyCall::CVmbxEmergencyCall() 
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::CVmbxEmergencyCall =>" );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::CVmbxEmergencyCall <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxEmergencyCall::~CVmbxEngineBase
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxEmergencyCall::~CVmbxEmergencyCall()
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::~CVmbxEmergencyCall =>" );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::~CVmbxEmergencyCall <=" );
    }
    
// ---------------------------------------------------------------------------
// CVmbxEmergencyCall::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxEmergencyCall* CVmbxEmergencyCall::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::NewL =>" );
    CVmbxEmergencyCall* vmbx = new( ELeave )CVmbxEmergencyCall();
    CleanupStack::PushL( vmbx );
    vmbx->ConstructL();
    CleanupStack::Pop( vmbx );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::NewL <=" );
    return vmbx;
    }

// ---------------------------------------------------------------------------
// CVmbxEmergencyCall::ConstructL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CVmbxEmergencyCall::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::ConstructL =>" );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::ConstructL <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxEmergencyCall::HandleDialL
//
// ----------------------------------------------------------------------------
//
void CVmbxEmergencyCall::HandleDialL( const TInt /*aStatus*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::HandleDialL =>" );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::HandleDialL <=" );
    }

// ----------------------------------------------------------------------------
//  CVmbxEmergencyCall::HandleEmergencyDialL()
// ----------------------------------------------------------------------------
//
void CVmbxEmergencyCall::HandleEmergencyDialL( const TInt /*aStatus*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::HandleEmergencyDialL =>" );
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::HandleEmergencyDialL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxEmergencyCall::IsEmergencyNumber
// Verifies if the given number is an emergency number
// ---------------------------------------------------------------------------
//
TBool CVmbxEmergencyCall::IsEmergencyNumber( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: CVmbxEmergencyCall::IsEmergencyNumber: =>" );
    TBool isEmergency( EFalse );
    // Emergency Call API does not strip invalid characters like Call UI does,
    // so it is done manually here.

    TPhCltTelephoneNumber number( aNumber.Left( KPhCltTelephoneNumberLength ) );
    PhCltUtils::RemoveInvalidChars( number );
    // Return value is KErrArgument, when number is more than 6 digits and
    // then number cannot be emergency number.
    // So no need to check the return value here.
    CPhCltEmergencyCall* emCall( NULL );
    TRAPD( err, emCall = CPhCltEmergencyCall::NewL( this ) );
    if ( KErrNone == err )
        {
        TInt result = emCall->IsEmergencyPhoneNumber( aNumber, isEmergency );
        if ( result != KErrNone ) 
            {
            isEmergency = EFalse; 
            }
        }
    delete emCall;

    VMBLOGSTRING2( "VMBX: CVmbxEmergencyCall::IsEmergencyNumber%I <=",
        isEmergency);

    return isEmergency;
    }

// End of file
