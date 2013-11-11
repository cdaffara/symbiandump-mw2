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
* Description:  Utility functions common for all parsers.
*
*/


// INCLUDE FILES
#include    "cphonegsmparserbase.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmParserBase::IsPrefixOf
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParserBase::IsPrefixOf( 
        const TDesC& aString, 
        const TDesC& aPrefix )
    {
    TBool result = EFalse;

    if ( aPrefix.Length() <= aString.Length() )
        {
        TPtrC part( aString.Left( aPrefix.Length() ) );

        result = ( part == aPrefix );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserBase::IsPostfixOf
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParserBase::IsPostfixOf(
        const TDesC& aString,
        const TDesC& aPostfix )
    {
    TBool result = EFalse;

    if ( aPostfix.Length() <= aString.Length() )
        {
        TPtrC part( aString.Right( aPostfix.Length() ) );

        result = ( part == aPostfix );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserBase::ExtractNumber
// -----------------------------------------------------------------------------
//
TPtrC CPhoneGsmParserBase::ExtractNumber( 
        const TDesC& aString )
    {
    TPtrC result( KNullDesC );

    if ( aString.Length() )
        {
        TLex input( aString );
        TLexMark mark;
        input.Mark( mark );

        while ( input.Peek().IsDigit() )
            {
            input.Inc();
            }

        result.Set( input.MarkedToken( mark ) );
        }
    
    return result;
    }

//  End of File  
