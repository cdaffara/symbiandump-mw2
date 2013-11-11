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
* Description:  Parser for PCN procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmpcnprocedureparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 

// CONSTANTS

// GsmPcn sim lock code prefix
_LIT( KPhoneGsmPcnSimLockCodePrefix, "*pw+" );

// GsmPcn sim unlock code prefix
_LIT( KPhoneGsmPcnSimUnlockCodePrefix, "#pw+" );

// Plus character.
const TInt KPhoneGsmPcnPlus = '+';

// Hash string
_LIT( KPhoneGsmPcnHash, "#" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::CPhoneGsmPcnProcedureParser
// -----------------------------------------------------------------------------
//
CPhoneGsmPcnProcedureParser::CPhoneGsmPcnProcedureParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmPcnProcedureParser* CPhoneGsmPcnProcedureParser::NewLC()
    {
    CPhoneGsmPcnProcedureParser* self = 
        new ( ELeave ) CPhoneGsmPcnProcedureParser;
    
    CleanupStack::PushL( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPcnProcedureParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TBool result = EFalse;
    aResult.ClearL();
    
    CPhoneGsmOptionContainer& options =
                static_cast<CPhoneGsmOptionContainer&>( aOptions );
    
    if ( !options.FindOptionStatus( KPhoneOptionSend ) )
        {
        result = HandleSimLockCodeL( aString, aResult );

        if ( !result )
            {
            result = HandleSimUnlockCodeL( aString, aResult );
            }
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::HandleSimLockCodeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPcnProcedureParser::HandleSimLockCodeL(
        const TDesC& aString,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;

    // Is this SIM lock command
    if ( IsPrefixOf( aString, KPhoneGsmPcnSimLockCodePrefix ) )
        {
        TLex input( aString );
        input.Inc( KPhoneGsmPcnSimLockCodePrefix().Length() );
       
        if ( HandleLockCodeParametersL( input.Remainder(), aResult ) )
            {
            result = ETrue;
            aResult.SetUid( KPhoneUidSimLockCode );
            }
        else if ( IsPostfixOf( aString, KPhoneGsmPcnHash ) )
            {
            result = ETrue;
            aResult.ClearL();
            aResult.SetUid( KPhoneUidSimLockCode );
            aResult.AddParameterL( KNullDesC );
            aResult.AddParameterL( KNullDesC );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::HandleSimUnlockCodeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPcnProcedureParser::HandleSimUnlockCodeL(
        const TDesC& aString,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;

    // Is this SIM unlock command
    if ( IsPrefixOf( aString, KPhoneGsmPcnSimUnlockCodePrefix ) )
        {
        TLex input( aString );
        input.Inc( KPhoneGsmPcnSimLockCodePrefix().Length() );

        if ( HandleLockCodeParametersL( input.Remainder(), aResult ) )
            {
            result = ETrue;
            aResult.SetUid( KPhoneUidSimUnlockCode );
            }
        else if ( IsPostfixOf( aString, KPhoneGsmPcnHash ) )
            {
            result = ETrue;
            aResult.ClearL();
            aResult.SetUid( KPhoneUidSimUnlockCode );
            aResult.AddParameterL( KNullDesC );
            aResult.AddParameterL( KNullDesC );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureParser::HandleLockCodeParametersL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmPcnProcedureParser::HandleLockCodeParametersL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TLex input( aString );
    TInt len; // length of strings
   
    // Extract first parameter
    TPtrC password( ExtractNumber( aString ) );
    len = password.Length();

    if ( len )
        {
        // Parameter ok
        input.Inc( len );       

        // Plus is separator of two parameters
        if ( input.Peek() == KPhoneGsmPcnPlus )
            {
            input.Inc();

            // Extract second parameter
            TPtrC type( ExtractNumber( input.Remainder() ) );
            len = type.Length();

            if ( len )
                {
                // Parameter ok
                input.Inc( len );

                // Check if we are at the end of string
                if ( input.Remainder() == KPhoneGsmPcnHash )
                    {
                    aResult.AddParameterL( password );
                    aResult.AddParameterL( type );

                    result = ETrue;
                    }
                }
            }
        }
    
    return result;
    }

//  End of File  
