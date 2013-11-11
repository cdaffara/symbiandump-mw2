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
* Description:  Principal parsing process is defined by the CPhoneGsmParser
*                class.
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>

#include    "cphonegsmparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmparserbase.h" 
#include    "cphonegsmdummyparser.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "cphoneparserfeatures.h" 
#include    "cphonegsmoptioncontainerbase.h" 

// CONSTANTS


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmParser::NewL
// -----------------------------------------------------------------------------
//
CPhoneGsmParser* CPhoneGsmParser::NewL( 
        CPhoneGsmParserBase* aSsProcedure,
        CPhoneGsmParserBase* aSimControlProcedure,
        CPhoneGsmParserBase* aPhoneNumber,
        CPhoneGsmParserBase* aManufacturerSpecific,
        CPhoneGsmParserBase* aEmergencyNumberParser,
        CPhoneGsmParserBase* aVoipNumber)
    {
    // Ownership of these instances is transferred, thus
    // creation of the instance must not leave. 

    CPhoneGsmParser* self = 
        new CPhoneGsmParser(
            aSsProcedure,
            aSimControlProcedure,
            aPhoneNumber,
            aManufacturerSpecific,
            aEmergencyNumberParser,
            aVoipNumber);
    
    if ( !self )
        {
        delete aSsProcedure;
        delete aSimControlProcedure;
        delete aPhoneNumber;
        delete aManufacturerSpecific;
        delete aEmergencyNumberParser;
        delete aVoipNumber;
        
        User::Leave( KErrNoMemory );
        }

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::~CPhoneGsmParser
// -----------------------------------------------------------------------------
//
CPhoneGsmParser::~CPhoneGsmParser()
    {
    delete iGsmSsProcedure;
    delete iGsmSimControlProcedure;
    delete iGsmPhoneNumber;
    delete iGsmManufacturerProcedure;
    delete iGsmEmergencyNumber;
    delete iVoipNumber;
    CPhoneParserFeatures::Free();
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::CPhoneGsmParser
// -----------------------------------------------------------------------------
//
CPhoneGsmParser::CPhoneGsmParser( 
        CPhoneGsmParserBase* aSsProcedure,
        CPhoneGsmParserBase* aSimControlProcedure,
        CPhoneGsmParserBase* aPhoneNumber,
        CPhoneGsmParserBase* aManufacturerSpecific,
        CPhoneGsmParserBase* aEmergencyNumberParser,
        CPhoneGsmParserBase* aVoipNumber)
    : iGsmSsProcedure( aSsProcedure ), 
      iGsmSimControlProcedure( aSimControlProcedure ),
      iGsmPhoneNumber( aPhoneNumber ),
      iGsmManufacturerProcedure( aManufacturerSpecific ),
      iGsmEmergencyNumber( aEmergencyNumberParser ),
      iVoipNumber( aVoipNumber )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParser::ConstructL()
    {
    // If we didn't receive a parser for something, then create dummy.

    if ( !iGsmSsProcedure )
        {
        iGsmSsProcedure = CPhoneGsmDummyParser::NewL();
        }
    if ( !iGsmSimControlProcedure )
        {
        iGsmSimControlProcedure = CPhoneGsmDummyParser::NewL();
        }
    if ( !iGsmPhoneNumber )
        {
        iGsmPhoneNumber = CPhoneGsmDummyParser::NewL();
        }
    if ( !iGsmManufacturerProcedure )
        {
        iGsmManufacturerProcedure = CPhoneGsmDummyParser::NewL();
        }
    if ( !iGsmEmergencyNumber )
        {
        iGsmEmergencyNumber = CPhoneGsmDummyParser::NewL();
        }
    if ( !iVoipNumber )
        {
        iVoipNumber = CPhoneGsmDummyParser::NewL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParser::ParseL( 
        const TDesC& aString, 
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TBool result = EFalse;
        
    result = DoParseL(
            aString,
            aResult, 
            static_cast<CPhoneGsmOptionContainer&>( aOptions ) );

    if ( !result )
        {
        aResult.ClearL();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::DoParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParser::DoParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainer& aOptions )
    {
    TBool result = EFalse;
    aResult.ClearL();

    // We do not handle empty strings.
    if ( !aString.Length() )
        {
        return EFalse;
        }

    TBool sendOperation = aOptions.FindOptionStatus( KPhoneOptionSend );
    
    // Check if emergency number.
    result = iGsmEmergencyNumber->ParseL( aString, aResult, aOptions );
    
    // First check if string is GSM ss procedure supported by MS.
    if ( !result )
        {
        result = iGsmSsProcedure->ParseL( aString, aResult, aOptions );
        }
        
    if ( !result )
        {
        // Check if it is sim control procedure.
        result = iGsmSimControlProcedure->ParseL( aString, aResult, aOptions );
        }

    if ( !result )
        {
        // Check if it is manufacturer specific procedure.
        result = 
            iGsmManufacturerProcedure->ParseL( aString, aResult, aOptions );
        }
    if ( !result )
        {
        // Check if it voip call.
        result = iVoipNumber->ParseL( aString, aResult, aOptions );
        }
    if ( !result )
        {
        result = iGsmPhoneNumber->ParseL( aString, aResult, aOptions );
        }
    if ( !result )
        {
        result = DefineAsUssdL( aString, aResult, aOptions );
        } 

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParser::DefineAsUssd
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParser::DefineAsUssdL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainer& aOptions )
    {
    aResult.ClearL();
    
    TBool result = aOptions.FindOptionStatus( KPhoneOptionSend );
    if ( result )
        {
        aResult.SetUid( KPhoneUidUnstructuredService );
        aResult.AddParameterL( aString );
        }
    return result;
    }

//  End of File  
