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
* Description:  Parser for phone numbers.
*
*/


// INCLUDE FILES
#include    "cphonegsmphonenumberparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "cphonevoipnumberparser.h" 
#include    "cphoneparserfeatures.h" 
#include    "phoneparsercommon.h" 
// CONSTANTS
_LIT( KPhoneClirSuppress, "*31#" );
_LIT( KPhoneClirInvoke, "#31#" );

const TInt KPhoneNumberOne = '1';

const TInt KPhoneNumberMaxLength = 80;

const TInt KPhoneShortStringMinLength = 1;  // Must be positive.
const TInt KPhoneShortStringMaxLength = 2;  // Must be positive.
const TInt KPhoneHashStringMinLength = 2;
const TInt KPhoneTwoDigitStringLength = 2;

// If there is one of the characters in the string, then the string
// is considered as phone number.
_LIT( KPhoneMustPhoneNumberChars, "pw" );
_LIT( KPhoneHashChar, "#" );
_LIT( KPhoneNumberZero, "0" );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::CPhoneGsmPhoneNumberParser
// -----------------------------------------------------------------------------
//
CPhoneGsmPhoneNumberParser::CPhoneGsmPhoneNumberParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmPhoneNumberParser* CPhoneGsmPhoneNumberParser::NewLC()
    {
    CPhoneGsmPhoneNumberParser* self = 
        new ( ELeave ) CPhoneGsmPhoneNumberParser;
    
    CleanupStack::PushL( self );
        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::ParseL
// 
// Phone number can contain following parts: supplementary
// service prefix, main part and dtmf postfix.
// 
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::ParseL( 
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
// CPhoneGsmPhoneNumberParser::DoParseL
// 
// Phone number can contain following parts: supplementary
// service prefix, main part and dtmf postfix.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::DoParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainer& aOptions )
    {
    TBool result = EFalse;

    if ( !aOptions.FindOptionStatus( KPhoneOptionSend ))
        {
        return EFalse;
        }
    if ( !ContainsPauseOrWaitChars( aString )) // p or w --> always phone number
        {
        if ( IsHashString( aString ))
            {
            return EFalse;
            }
        if ( aString.Length() >= KPhoneShortStringMinLength &&
             aString.Length() <= KPhoneShortStringMaxLength )
            {
            if ( !IsZeroDialling( aString, aOptions ) && 
                !IsTwoDigitDialling( aString ) && !Is1xDialling( aString, aOptions )) 
                {
                return EFalse;
                }
            }
        }
    TLex input( aString );
    
    if ( !HandleServicePrefix( input, aResult ) )
        {
        aResult.SetAuxInformation( KPhoneDialDefaultClir );
        }

    // Take number part and add it to result.
    result = TakeNumberPartL( input, aResult );
        
    if ( result )
        {
        if ( !aOptions.FindOptionStatus( KPhoneOptionVoipCall ) )
            {
            aResult.SetUid( KPhoneUidPhoneNumber );
            }

        // Add rest of string( dtmf postfix ) to result.
        if ( !TakeDtmfPostfixL( input, aResult ) )
            {
            aResult.AddParameterL( KNullDesC );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::HandleServicePrefix
// 
// Checks if there is clir suppress/invoke prefix in the
// string. 
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::HandleServicePrefix( 
        TLex& aLex, 
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TPtrC remainder( aLex.Remainder() );

    if ( EqualsLeft( remainder, KPhoneClirSuppress ) )
        {
        aLex.Inc( KPhoneClirSuppress().Length() );
        aResult.SetAuxInformation( KPhoneDialSuppressClir );
        result = ETrue;
        }
    else if ( EqualsLeft( remainder, KPhoneClirInvoke ) )
        {
        aLex.Inc( KPhoneClirSuppress().Length() );
        aResult.SetAuxInformation( KPhoneDialInvokeClir );
        result = ETrue;
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::TakeNumberPartL
// 
// Takes a main part of the phone number. And add
// it to the aReturn parameter with or without optional international prefix.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::TakeNumberPartL(
        TLex& aLex,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    
    TLexMark start;
    aLex.Mark( start );

    TInt maxLength = KPhoneNumberMaxLength;

    // Checks is first char optional international prefix
    // and if its increments to the next char position.
    if ( aLex.Peek() == KPhonePlus )
        {
        aLex.Inc();
        maxLength++;
        }

    // Check possible 'p','w' or '+' marks.
    while ( ( aLex.Peek().IsDigit() ) || 
            ( aLex.Peek() == KPhoneNumberAsterisk ) ||
            ( aLex.Peek() == KPhoneNumberHash ) )
        {
        aLex.Inc();
        }

    // Add parsing string to result parameter
    TPtrC mainpart( aLex.MarkedToken( start ) );
    const TInt length = mainpart.Length();
    if ( length <= maxLength )
        {
        result = ETrue;
        aResult.AddParameterL( mainpart );
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::TakeDtmfPostfixL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::TakeDtmfPostfixL(
        TLex& aLex,
        CPhoneGsmParserResult& aResult )
    {
    // Add rest of string( after TLex16::iNext ) to result.
    aResult.AddParameterL( aLex.Remainder() );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::EqualsLeft
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::EqualsLeft( 
        const TDesC& aDesc, 
        const TDesC& aLeft )
    {
    return IsPrefixOf( aDesc, aLeft );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::ContainsPauseOrWaitChars
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::ContainsPauseOrWaitChars(
        const TDesC& aString ) const
    {

    TBool result = EFalse;
    TPtrC chs( KPhoneMustPhoneNumberChars );
    for ( TInt index = 0; index < chs.Length(); index++ )
        {
        if ( aString.Locate( chs[ index ] ) != KErrNotFound )
            {
            result = ETrue;
            break;
            }
        }

    return result; 
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::IsHashString
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::IsHashString( 
        const TDesC& aString ) const
    {
    return aString.Length() >= KPhoneHashStringMinLength &&
        // Rightmost character must be #
        aString.Right( 1 ) == KPhoneHashChar; 
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::IsZeroDialling
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::IsZeroDialling( 
        const TDesC& aString, CPhoneGsmOptionContainer& aOptions ) const
    {
    return !aOptions.FindOptionStatus( KPhoneOptionInCall ) &&
        !aString.Compare( KPhoneNumberZero ) && 
        CPhoneParserFeatures::IsZeroDialEnabled(); 
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::IsTwoDigitDialling
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::IsTwoDigitDialling( 
        const TDesC& aString ) const
    {
    return CPhoneParserFeatures::TwoDigitCallingEnabled() &&
        aString.Length()== KPhoneTwoDigitStringLength;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberParser::Is1xDialling
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPhoneNumberParser::Is1xDialling( 
    const TDesC& aString, CPhoneGsmOptionContainer& aOptions  ) const
    {
    return !aOptions.FindOptionStatus( KPhoneOptionInCall ) &&
        aString.Length()== KPhoneShortStringMaxLength &&
        aString[0] == KPhoneNumberOne &&
        TChar( aString[1] ).IsDigit();
    }

//  End of File  
