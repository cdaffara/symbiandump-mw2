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
#include    "cphonevoipnumberparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "phoneparsercommon.h" 

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::CPhoneVoipNumberParser
// -----------------------------------------------------------------------------
//
CPhoneVoipNumberParser::CPhoneVoipNumberParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneVoipNumberParser* CPhoneVoipNumberParser::NewLC()
    {
    CPhoneVoipNumberParser* self = 
        new ( ELeave ) CPhoneVoipNumberParser;
    
    CleanupStack::PushL( self );
        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneVoipNumberParser::ParseL( 
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
// CPhoneVoipNumberParser::DoParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneVoipNumberParser::DoParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainer& aOptions )
    {
    TBool result( EFalse );

    if ( aOptions.FindOptionStatus( KPhoneOptionSend ) )
        {
        if ( aOptions.FindOptionStatus( KPhoneOptionVoipCall ) &&
             aString.Length() )
            {
            TLex input( aString );
            
            // Take number part and put it to first parameter of the aResult.
            TakeNumberPartL( input, aResult );       
            // Take rest of string( postfix part ) to next parameter of the result.
            TakeDtmfPostfixL( input, aResult );

            aResult.SetUid( KPhoneUidVoipNumber );
            result = ETrue;
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::TakeNumberPartL
// 
// Take the main part of the phone number and add it to result.
// -----------------------------------------------------------------------------
//
void CPhoneVoipNumberParser::TakeNumberPartL(
        TLex& aLex,
        CPhoneGsmParserResult& aResult )
    {
    TLexMark start;
    aLex.Mark( start );
    TBool allDigits = ETrue;
    TBool dtmfFound = EFalse;
    
    while ( !aLex.Eos() &&
            !dtmfFound )
        {
        TChar nextChar = aLex.Peek().GetLowerCase();
        if ( allDigits &&
            ( KPhoneDtmfPause == nextChar ||
              KPhoneDtmfWait == nextChar ||
              KPhonePlus == nextChar ) )
            {
            // Dtmf string is always postfixed if present
            if ( aLex.Offset() > 0 )
                {
                // Check if rest of the string is valid dtmf string
                TPtrC remainder( aLex.Remainder() );
                if ( IsValidDtmfString( remainder ) )
                    {
                    dtmfFound = ETrue;     
                    }
                }
            }
        
        if ( !dtmfFound )
            {
            if ( !nextChar.IsDigit() )
                {
                allDigits = EFalse;
                }
            aLex.Inc();
            }
        }
    
    // Add main part of the number to the parsing result
    TPtrC mainpart( aLex.MarkedToken( start ) );
    aResult.AddParameterL( mainpart );   
    }

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::TakeDtmfPostfixL
// -----------------------------------------------------------------------------
//
void CPhoneVoipNumberParser::TakeDtmfPostfixL(
        TLex& aLex,
        CPhoneGsmParserResult& aResult )
    {
    // Add rest of string( after TLex16::iNext ) to result.
    aResult.AddParameterL( aLex.Remainder() );
    }

// -----------------------------------------------------------------------------
// CPhoneVoipNumberParser::IsValidDtmfString
// -----------------------------------------------------------------------------
//
TBool CPhoneVoipNumberParser::IsValidDtmfString( TPtrC& aString )
    { 
    TBool validDtmf = ETrue;
    // Check that string contains only valid dtmf characters
    for( TInt i = 0; i < aString.Length(); i++ )
        {
        if ( KErrNotFound == KValidDtmfChars().Locate( aString[i] ) )
            {
            validDtmf = EFalse;
            break;
            }      
        }
    return validDtmf;
    }
//  End of File  
