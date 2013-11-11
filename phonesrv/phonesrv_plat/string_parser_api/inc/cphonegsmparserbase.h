/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is base class for parsers.
*
*/


#ifndef CPHONEGSMPARSERBASE_H
#define CPHONEGSMPARSERBASE_H

// INCLUDES
#include    <e32base.h>

// MACROS

/**
* It defines macro that can be used to create a single uid.
* 
* Parameters main and sub should be positive integers, range of
* main is 0..255 and sub is 0..2^16-1. 
*
* Macro is equivalent to (main mod 256)*2**16 + sub.
*/
#define PHONE_MAKE_UID( main, sub ) (TUint32)((((main) & 0xFF) << 16) + ((sub) & 0xFFFF))

/**
* Extracts main part from uid.
*
* Macro is equivalent to (main / 2**16) mod 256.
*/
#define PHONE_EXTRACT_MAIN( uid ) (((uid) >> 16) & 0xFF)

// CONSTANTS

/**
* ALL MAIN UIDS ARE LISTED HERE.
*
* Please, do not change unless you know what you are doing!
*/

/**
* Invalid string.
*/
const TUint32 KPhoneGsmUidInvalid = 0;

/**
* Supplementary services.
*/
const TUint32 KPhoneGsmUidSupplementaryService = 1;

/**
* Sim control procedure.
*/
const TUint32 KPhoneGsmUidSimControlProcedure = 2;

/**
* Phone number
*/
const TUint32 KPhoneGsmUidDialPhoneNumber = 3;

/**
* Misc GSM required strings, most notably USSD.
*/
const TUint32 KPhoneGsmUidMisc = 4;

/**
* Supplementary service, during calls.
*/
const TUint32 KPhoneGsmUidSupplementaryServiceDuringCall = 5;

/**
* Manufacturer specific codes.
*/
const TUint32 KPhoneGsmUidManufacturer = 6;

/**
* PCN specific codes.
*/
const TUint32 KPhoneGsmUidPcnProcedures = 7;

/**
* Emergency number
*/
const TUint32 KPhoneGsmUidDialEmergencyNumber = 8;

/**
* Voip number
*/
const TUint32 KPhoneUidDialVoipyNumber = 9;

/**
* ALL MISC UIDS:
*/
const TUint32 KPhoneUidUnstructuredService = 
    PHONE_MAKE_UID( KPhoneGsmUidMisc, 0 );

// FORWARD DECLARATIONS
class CPhoneGsmParserResult;
class CPhoneGsmOptionContainerBase;

// CLASS DECLARATION

/**
* It is base class for parsers.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmParserBase 
    : public CBase
    {
    public: // New functions
        
        /**
        * Parses string.
        *
        * String must not contain other characters than from set
        * { 0..9, *, #, +, p, w }. If method leaves, then result
        * may contain something that is not valid.
        *
        * @param aString string to be parsed.
        * @param aResult It will contain result.
        * @param aOptions It contains all options related to parsing.
        * @return It returns boolean value indicating success of parsing.
        */
        virtual TBool ParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions ) = 0;

        // SOME COMMON STATIC FUNCTIONS

        /**
        * Checks if string is prefix of another.
        *
        * @param aString original string.
        * @param aPrefix possibly prefix of aString.
        * @return ETrue iff aPrefix is prefix of aString.
        */
        static TBool IsPrefixOf( 
            const TDesC& aString, 
            const TDesC& aPrefix );
        
        /**
        * Checks if string is postfix of another.
        *
        * @param aString original string.
        * @param aPrefix possibly postfix of aString.
        * @return ETrue iff aPostfix is postfix of aString.
        */
        static TBool IsPostfixOf(
            const TDesC& aString,
            const TDesC& aPostfix );

        /**
        * Extracts sequence of digits.
        *
        * @param aString input.
        * @return sequence of digits, prefix of aString.
        */
        static TPtrC ExtractNumber( 
            const TDesC& aString );

    };

#endif      // CPHONEGSMPARSERBASE_H
            
// End of File
