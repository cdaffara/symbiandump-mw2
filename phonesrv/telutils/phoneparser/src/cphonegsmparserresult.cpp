/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulates results of parsing.
*
*/


// INCLUDE FILES
#include    <featmgr.h>

#include    "cphonegsmparserresult.h" 
#include    "cphonegsmparserbase.h" 
#include    "phonegsmparser.h" 

// CONSTANTS
const TInt KPhoneParserResultParameterGranularity = 10;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::CPhoneGsmParserResult
// -----------------------------------------------------------------------------
//
CPhoneGsmParserResult::CPhoneGsmParserResult()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::ConstructL()
    {
    iParameters = 
        new ( ELeave ) 
            CArrayFixFlat< TPtrC >( KPhoneParserResultParameterGranularity );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::NewL
// -----------------------------------------------------------------------------
//
CPhoneGsmParserResult* CPhoneGsmParserResult::NewL()
    {
    CPhoneGsmParserResult* self = new (ELeave) CPhoneGsmParserResult;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::~CPhoneGsmParserResult
// -----------------------------------------------------------------------------
//
CPhoneGsmParserResult::~CPhoneGsmParserResult()
    {
    delete iParameters;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::ReserveParametersL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::ReserveParametersL( 
        TUint aAmount )
    {
    iParameters->SetReserveL( aAmount );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::ClearL
//
// Zero all information.
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::ClearL()
    {
    iUid = KPhoneGsmUidInvalid;
    iAuxInformation = 0;

    if ( iParameters->Count() )
        {
        iParameters->ResizeL( 0 ); // Doesn't leave
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::ParameterAt
// -----------------------------------------------------------------------------
//
const TDesC& CPhoneGsmParserResult::ParameterAt( 
        TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex >= 0 &&
                     aIndex < iParameters->Count(),
                     PhoneGsmParser::Panic( PhoneGsmParser::EIncorrectUse ) );
    
    return iParameters->At( aIndex );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::SetUid
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::SetUid( 
        TUint aUid )
    {
    iUid = aUid;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::SetAuxInformation
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::SetAuxInformation( 
        TUint aAuxInformation )
    {
    iAuxInformation = aAuxInformation;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserResult::AddParameterL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserResult::AddParameterL( 
        const TDesC& aParameter )
    {
    iParameters->AppendL( aParameter );
    }           

//  End of File  
