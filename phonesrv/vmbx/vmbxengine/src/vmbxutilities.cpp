/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the VmbxUtilities class
*
*/


// INCLUDE FILES
#include <featmgr.h>
#include <msssettingsobserver.h> // ALS changes

#include "vmbxlogger.h"
#include "vmbxcenrephandler.h"
#include "vmbxemergencycall.h"
#include "vmbxutilities.h"
#include "voicemailboxdefsinternal.h"

// CONSTANTS
const TInt KVmbxPhoneNumMinLength = 2;
const TInt KVmbxPhoneNumMaxLength = 40;

_LIT( KAllowedTelNumChars, "0123456789" );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// VmbxUtilities::IsValidPhoneNumber
// Validates phone number
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::IsValidPhoneNumber( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::IsValidPhoneNumber: =>" );
    TBool result( EFalse );
    if ( KVmbxPhoneNumMinLength < aNumber.Length() 
        && KVmbxPhoneCharMaxLength >= aNumber.Length() )
        {
        VMBLOGSTRING2( "VMBX: VmbxUtilities::IsValidPhoneNumber: aNumber %S",
        &aNumber );
        TLex lexer( aNumber );
        lexer.SkipSpace();
        TChar current = lexer.Peek();
        // If no SS code then consider this is valid and return true.    
        if ( '*' == current )
            {
            // check whether accord ss regulation,
            lexer.Inc(); // Skip first *
            }
    result = ValidateTelNum( lexer );
    // If zero length then consider as valid and return value of 
    // result, If not the end of the string, check if it's DTMF numbers
    if ( !lexer.Eos() && result )
        {
        result = ValdateDtmfPart( lexer );
        }
    }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::IsValidPhoneNumber: result = %d<=",
        result );
    return result;
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::AlsLine
// 
// ---------------------------------------------------------------------------
//
TVmbxAlsLineType VmbxUtilities::AlsLine()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::AlsLine: =>" );
    TInt ssLine( ESSSettingsAlsNotSupported );
    TVmbxAlsLineType alsLine( EVmbxAlsLineDefault );
    RSSSettings ssSettings;
    TInt res = ssSettings.Open();
    if( KErrNone == res )
        {
        res = ssSettings.Get( ESSSettingsAls, ssLine );
        if( KErrNone == res )
            {
            if ( ESSSettingsAlsAlternate == ssLine )
                 {
                 alsLine = EVmbxAlsLine2;
                 }
            else if ( ESSSettingsAlsPrimary == ssLine )
                {
                 alsLine = EVmbxAlsLine1;
                }
            }
        }
    ssSettings.Close();
    VMBLOGSTRING2( "VMBX: VmbxUtilities::AlsLine: alsLine=%I <=",
       alsLine );
    return alsLine;
    }

// ---------------------------------------------------------------------------
// CVmbxUiUtilities::VideoSupported
// 
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::VideoSupported()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::VideoSupported: =>" );
    TBool result( EFalse );
    
    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        CVmbxCenRepHandler* cenRepHandler( NULL );
        // TRAP_IGNORE for no leave function
        TRAP_IGNORE( cenRepHandler = CVmbxCenRepHandler::NewL() );
        if ( cenRepHandler )
            {
            result = cenRepHandler->VideoSupported();
            }
        delete cenRepHandler;
        cenRepHandler = NULL;
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::VideoSupported: %d <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::IsEmergencyNumber
// Verifies if the given number is an emergency number
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::IsEmergencyNumber( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::IsEmergencyNumber: =>" );
    TBool result( EFalse );
    CVmbxEmergencyCall* emergencyCall( NULL );
    // TRAP_IGNORE for no leave function
    TRAP_IGNORE( emergencyCall = CVmbxEmergencyCall::NewL() );
    if ( emergencyCall )
        {
        result = emergencyCall->IsEmergencyNumber( aNumber );
        }
    delete emergencyCall;
    emergencyCall = NULL;
    VMBLOGSTRING2( "VMBX: VmbxUtilities::IsEmergencyNumber: result%d <=", 
            result);
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxUiUtilities::VoIPSupported
// 
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::VoIPSupported()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::VoIPSupported: <=>" );
    return FeatureManager::FeatureSupported( KFeatureIdCommonVoip );
    }

// -----------------------------------------------------------------------------
// VmbxUtilities::ValidateTelNum
// Parses string until end or invalid tel number character is found.
// Check number length.
// -----------------------------------------------------------------------------
//  
TBool VmbxUtilities::ValidateTelNum( TLex& aLexer ) 
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::ValidateTelNum: =>" );
    TBool result( ETrue );

    // Skip country code prefix '+'
    if ( '+' == aLexer.Peek() )
        {
        aLexer.Inc(); // Skip country code separator
        }

    TInt telNumDigits( 0 );
    TBool invalidCharFound( EFalse );
    // Parse until invalid telnumber char found
    while ( !aLexer.Eos() && !invalidCharFound )
        {
        const TChar nextChar = aLexer.Peek();
        // Check validSS chars
        if ( KErrNotFound == KAllowedTelNumChars().Locate( nextChar ) )
            {
            // Invalid char found so string before it is the tel number part
            invalidCharFound = ETrue;
            }
        else
            {
            aLexer.Inc();
            telNumDigits++;
            }
        }

    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateTelNum:\
        telNumDigits %d", telNumDigits );
    // digitlength <3 or digit length>40, invalid telnumber
    if ( KVmbxPhoneNumMinLength >= telNumDigits 
        || KVmbxPhoneNumMaxLength < telNumDigits 
        )
        {
        result = EFalse;
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateTelNum:\
         result %d<=", result );
    return result;
    }

// -----------------------------------------------------------------------------
// VmbxUtilities::ValdateDtmfPart
// Parse string until end and validate for allowed characters.
// -----------------------------------------------------------------------------
//       
TBool VmbxUtilities::ValdateDtmfPart( TLex& aLexer )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::ValdateDtmfPart: =>" );
    // DTMF string consists of following three parts:
    // the first part of the string is a phone number,
    // the second part of the string a DTMF special character (p, w or +),
    // the last third part is an actual DTMF tone string, which is sent to the
    // remote end.
    TBool result( EFalse );
    // check the second part of the string
    const TChar nextChar = aLexer.Peek();
    // Check DTMF number discover or not
    if ( 'p' == nextChar || 'P' == nextChar 
        || 'w'== nextChar || 'W' == nextChar || '+' == nextChar )
        {
        // After DTMF char, is tone string, there is no specfic rule about tone string,
        // so just do a simple check.
        result = ETrue;
        VMBLOGSTRING( "VMBX: VmbxUtilities::ValdateDtmfPart DtmfNumberDiscover" );
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValdateDtmfPart: result = %d<=",
        result );
    return result;
    }

// End of file
