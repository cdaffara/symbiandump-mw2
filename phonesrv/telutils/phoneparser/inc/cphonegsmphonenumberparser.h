/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is parser for phone numbers.
*
*/


#ifndef CPHONEGSMPHONENUMBERPARSER_H
#define CPHONEGSMPHONENUMBERPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 

// FORWARD DECLARATIONS
class CPhoneGsmOptionContainer;
// CONSTANTS

/*
* PHONE NUMBER UIDS
*/

const TUint32 KPhoneUidPhoneNumber = 
    PHONE_MAKE_UID( KPhoneGsmUidDialPhoneNumber, 0 );

/*
* Dial phone number related constants, store to aux information field.
*/

/*
* There was no prefix, use default clir.
*/
const TUint KPhoneDialDefaultClir = 0;

/*
* There was clir suppress prefix.
*/
const TUint KPhoneDialSuppressClir = 1;

/*
* There was clir invoke prefix.
*/
const TUint KPhoneDialInvokeClir = 2;

// CLASS DECLARATION

/**
* It is parser for phone numbers.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmPhoneNumberParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmPhoneNumberParser* NewLC();

    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase.
        * Parse string.
        *
        * @param aString It is string to be parsed.
        * @param aResult It will contain result.
        * @param aOptionUid It is the uid of the option.
        * @return It returns boolean value indicating success of parsing.
        */
        TBool ParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );
            
    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmPhoneNumberParser();

        /**
        * Parse.
        * 
        * @param aString It is string to be parsed.
        * @param aResult It will contain result.
        * @param aOptionUid It is the uid of the option.
        * @return It returns boolean value indicating success of parsing.
        */
        TBool DoParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainer& aOptions );


        /**
        * Handle supplementary service prefixes.
        * Checks is there clir suppress/invoke prefix in the string.
        * @param aLex Contains string to be handled.
        * @param aResult It will contain result.
        */
        TBool HandleServicePrefix( 
            TLex& aLex, 
            CPhoneGsmParserResult& aResult );

        /**
        * Takes a main part of the phone number( before 'p','w' or 
        * second '+' marks ). 
        * And add it to parameter of the aResult with or without optional 
        * international prefix.
        * @param aLex Contains string to be handled.
        * @param aResult It will contain result.
        */
        TBool TakeNumberPartL(
            TLex& aLex,
            CPhoneGsmParserResult& aResult );

        /**
        * Takes Dtmf postfix and add it to parameter of the aResult.
        * @param aLex Contains string to be handled.
        * @param aResult It will contain result.
        */
        TBool TakeDtmfPostfixL(
            TLex& aLex,
            CPhoneGsmParserResult& aResult );

        /**
        * Checks if param aLeft is prefix of param aDest.
        */
        static TBool EqualsLeft( const TDesC& aDest, const TDesC& aLeft );

        /**
        * Checks if string contains p or w characters.
        *
        * @param aString It is string to be checked.
        * @return ETrue if string contains p or w characters.
        */
        TBool ContainsPauseOrWaitChars( const TDesC& aString ) const;

        /**
        * Checks if rightmost character in string is #.
        *
        * @param aString It is string to be checked.
        * @return ETrue if string is hash string.
        */
        TBool IsHashString( const TDesC& aString ) const;

        /**
        * Checks if string is zero and zero dialling is on and
        * it is not in call situation.
        *
        * @param aString It is string to be checked.
        * @param aOptions Is call options
        * @return ETrue if zero dialling.
        */
        TBool IsZeroDialling( const TDesC& aString, 
            CPhoneGsmOptionContainer& aOptions ) const;

        /**
        * Checks if two digit dialling is on and string 
        * is two digit string.
        *
        * @param aString It is string to be checked.
        * @return ETrue if two digit dialling.
        */
        TBool IsTwoDigitDialling( const TDesC& aString ) const;

        /**
        * Checks if Input string is a 2 digit starting with a "1"
        * and it is not in call situation.
        *
        * @param aString It is string to be checked.
        * @param aOptions Is call options
        * @return ETrue if 1x dialling situation.
        */
        TBool Is1xDialling( const TDesC& aString,
            CPhoneGsmOptionContainer& aOptions ) const;
        
    };

#endif      // CPHONEGSMPHONENUMBERPARSER_H
            
// End of File
