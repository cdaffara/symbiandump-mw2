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
* Description:  Parser for SIM control procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmsimcontrolparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 

// CONSTANTS
_LIT( KPhoneSimControlPinChangeCode, "04" );
_LIT( KPhoneSimControlPin2ChangeCode, "042" );
_LIT( KPhoneSimControlPinUnblockCode, "05" );
_LIT( KPhoneSimControlPin2UnblockCode, "052" );

const TInt KPhoneSimControlPinMinLength = 4;
const TInt KPhoneSimControlPinMaxLength = 8;
const TInt KPhoneSimControlPukLength = 8;

const TInt KPhoneSimControlAsterisk = '*';
const TInt KPhoneSimControlHash = '#';

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::CPhoneGsmSimControlParser
// -----------------------------------------------------------------------------
//
CPhoneGsmSimControlParser::CPhoneGsmSimControlParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmSimControlParser* CPhoneGsmSimControlParser::NewLC()
    {
    CPhoneGsmSimControlParser* self = 
        new ( ELeave ) CPhoneGsmSimControlParser;
    
    CleanupStack::PushL( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::ParseL
// 
// Parse sim control procedures.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    aResult.ClearL();    
    
    TBool sendOperation = aOptions.FindOptionStatus( KPhoneOptionSend );
    TBool incall = aOptions.FindOptionStatus( KPhoneOptionInCall );

    TBool result = DoParseL( aString, aResult, sendOperation, incall );

    if ( !result )
        {
        aResult.ClearL();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::DoParseL
// 
// Parse sim control procedures.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::DoParseL(
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        TBool aSendOperation,
        TBool /*aInCall*/ )
    {
    TBool result = EFalse;
    aResult.ClearL();

    TLex input( aString );

    if ( !aSendOperation )
        {
        result = HandlePinOperationsL( input, aResult );
        }

    return result;
    }  

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePinOperationsL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePinOperationsL( 
        TLex& aInput, 
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TLexMark start;
    aInput.Mark( start );

    if ( HandlePinPrefix( aInput, aResult ) )
        {
        TPtrC number( NextNumber( aInput ) );

        if ( number == KPhoneSimControlPinChangeCode )
            {
            aResult.SetUid( KPhoneUidPinChange );
            result = HandlePinChangeL( aInput, aResult );
            }
        else if ( number == KPhoneSimControlPin2ChangeCode )
            {
            aResult.SetUid( KPhoneUidPin2Change );
            result = HandlePinChangeL( aInput, aResult );
            }
        else if ( number == KPhoneSimControlPinUnblockCode )
            {
            aResult.SetUid( KPhoneUidPinUnblock );
            result = HandlePinUnblockL( aInput, aResult );
            }
        else if ( number == KPhoneSimControlPin2UnblockCode )
            {
            aResult.SetUid( KPhoneUidPin2Unblock );
            result = HandlePinUnblockL( aInput, aResult );
            }
        }

    if ( !result )
        {
        aResult.ClearL();
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePinPrefix
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePinPrefix( 
        TLex& aInput,
        CPhoneGsmParserResult& /*aResult*/ )
    {
    TBool result = ETrue;

    TLexMark start;
    aInput.Mark( start );

    if ( aInput.Peek() != KPhoneSimControlAsterisk )
        {
        result = EFalse;
        }
    else
        {
        aInput.Inc();

        if ( aInput.Peek() != KPhoneSimControlAsterisk )
            {
            result = EFalse;
            }
        else
            {
            aInput.Inc();
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePinChangeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePinChangeL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TLexMark start;
    aInput.Mark( start );

    result = HandlePinL( aInput, aResult );
    result = result && HandlePinL( aInput, aResult );
    result = result && HandlePinL( aInput, aResult );
    result = result && HandleNoParameters( aInput, aResult );

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePinUnblockL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePinUnblockL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TLexMark start;
    aInput.Mark( start );

    result = HandlePukL( aInput, aResult );
    result = result && HandlePinL( aInput, aResult );
    result = result && HandlePinL( aInput, aResult );
    result = result && HandleNoParameters( aInput, aResult );

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandleNoParameters
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandleNoParameters(
        TLex& aInput,
        CPhoneGsmParserResult& /*aResult*/ )
    {
    TBool result = EFalse;

    if ( aInput.Remainder().Length() == 1 &&    // single character
         aInput.Peek() == KPhoneSimControlHash )
        {
        result = ETrue;
        aInput.Inc();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePinL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePinL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    return HandleParameterL(
        aInput,
        aResult,
        CPhoneGsmSimControlParser::ValidatePin );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandlePukL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandlePukL(
        TLex& aInput, 
        CPhoneGsmParserResult& aResult )
    {
    return HandleParameterL( 
        aInput, 
        aResult,
        CPhoneGsmSimControlParser::ValidatePuk );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::HandleParameterL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::HandleParameterL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult,
        TParameterValidationFunction aFunc )
    {
    TBool result = EFalse;

    TLexMark start;
    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSimControlAsterisk )
        {
        aInput.Inc();

        TPtrC password( NextNumber( aInput ) );
        
        if ( aFunc( password ) )
            {
            aResult.AddParameterL( password );
            result = ETrue;
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }


// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::NextNumber
// -----------------------------------------------------------------------------
//
TPtrC CPhoneGsmSimControlParser::NextNumber( 
        TLex& aInput )
    {
    TLexMark start;
    aInput.Mark( start );
    
    while ( aInput.Peek().IsDigit() )
        {
        aInput.Inc();
        }

    return aInput.MarkedToken( start );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::ValidatePin
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::ValidatePin( const TDesC& aParam )
    {
    return ( aParam.Length() >= KPhoneSimControlPinMinLength ) &&
           ( aParam.Length() <= KPhoneSimControlPinMaxLength );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlParser::ValidatePuk
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSimControlParser::ValidatePuk( const TDesC& aParam )
    {
    return ( aParam.Length() == KPhoneSimControlPukLength );
    }

//  End of File  
