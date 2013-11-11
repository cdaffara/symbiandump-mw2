/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Parser for emergency numbers.
*
*/


// INCLUDE FILES
#include    "cphonegsmemergencynumberparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "cphoneparserfeatures.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmEmergencyNumberParser::CPhoneGsmEmergencyNumberParser
// -----------------------------------------------------------------------------
//
CPhoneGsmEmergencyNumberParser::CPhoneGsmEmergencyNumberParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmEmergencyNumberParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmEmergencyNumberParser* CPhoneGsmEmergencyNumberParser::NewLC()
    {
    CPhoneGsmEmergencyNumberParser* self = 
        new ( ELeave ) CPhoneGsmEmergencyNumberParser;
    
    CleanupStack::PushL( self );
        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmEmergencyNumberParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmEmergencyNumberParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    aResult.ClearL();

    TBool result = DoParseL( 
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
// CPhoneGsmEmergencyNumberParser::DoParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmEmergencyNumberParser::DoParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainer& aOptions )
    {
    TBool result( EFalse );

    if ( aOptions.FindOptionStatus( KPhoneOptionSend ) )
        {
        iEmergencyPhoneNumber.Zero();

        if ( CPhoneParserFeatures::IsEmergencyNumber( 
            aString, iEmergencyPhoneNumber ))
            {
            aResult.AddParameterL( iEmergencyPhoneNumber );
            aResult.SetUid( KPhoneUidEmergencyNumber );
            result = ETrue;
            }
        }

    return result;
    }

    
//  End of File  
