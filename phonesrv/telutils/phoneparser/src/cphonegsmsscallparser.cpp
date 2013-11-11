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
* Description:  Parser for call concerned SS procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmsscallparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 

// CONSTANTS

const TInt KPhoneCommandLength = 1;

// The length of the following are expected to be KPhoneCommandLength.
_LIT( KPhoneCommand0, "0" );
_LIT( KPhoneCommand1, "1" );
_LIT( KPhoneCommand2, "2" );
_LIT( KPhoneCommand3, "3" );
_LIT( KPhoneCommand4, "4" );

// 2 character commands.
const TInt KPhoneCommandXLength = 2;
_LIT( KPhoneCommand2X, "2" );
_LIT( KPhoneCommand1X, "1" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSsCallParser::CPhoneGsmSsCallParser
// -----------------------------------------------------------------------------
//
CPhoneGsmSsCallParser::CPhoneGsmSsCallParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsCallParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmSsCallParser* CPhoneGsmSsCallParser::NewLC()
    {
    CPhoneGsmSsCallParser* self = 
        new ( ELeave ) CPhoneGsmSsCallParser;
    
    CleanupStack::PushL( self );
        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsCallParser::ParseL
//
// Parse call concerned ss strings.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsCallParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    aResult.ClearL();

    if ( !aOptions.FindOptionStatus( KPhoneOptionInCall ) ||
         !aOptions.FindOptionStatus( KPhoneOptionSend ) )
        {
        return EFalse;
        }
    
    TInt length = aString.Length();

    if ( length == KPhoneCommandXLength )
        {
        // Check one character from the beginning.
        TPtrC prefix( aString.Left( 1 ) );

        if ( prefix == KPhoneCommand1X )
            {
            TLex lex( aString.Right(1) );
            TUint32 value;

            if ( lex.Val( value, EDecimal ) == KErrNone )
                {
                aResult.SetUid( KPhoneUidCommand1 );
                aResult.SetAuxInformation( value + 1 );
                }           
            }
        else if ( prefix == KPhoneCommand2X )
            {
            TLex lex( aString.Right(1) );
            TUint32 value;

            if ( lex.Val( value, EDecimal ) == KErrNone )
                {
                aResult.SetUid( KPhoneUidCommand2 );
                aResult.SetAuxInformation( value + 1 );
                }
            }
        }
    else
        {
        if ( length == KPhoneCommandLength )
            {
            if ( aString == KPhoneCommand0 )
                {
                aResult.SetUid( KPhoneUidCommand0 );
                }
            else if ( aString == KPhoneCommand1 )
                {
                aResult.SetUid( KPhoneUidCommand1 );
                }
            else if ( aString == KPhoneCommand2 )
                {
                aResult.SetUid( KPhoneUidCommand2 );
                }
            else if ( aString == KPhoneCommand3 )
                {
                aResult.SetUid( KPhoneUidCommand3 );
                }
            else if ( aString == KPhoneCommand4 )
                {
                aResult.SetUid( KPhoneUidCommand4 );
                }
            }
        }
   
    return ( aResult.Uid() != KPhoneGsmUidInvalid );
    }

//  End of File  
