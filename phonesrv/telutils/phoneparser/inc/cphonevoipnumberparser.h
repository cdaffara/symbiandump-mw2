/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is parser for voip numbers.
*
*/


#ifndef CPHONEVOIPNUMBERPARSER_H
#define CPHONEVOIPNUMBERPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 
#include    <phclttypes.h> 

// FORWARD DECLARATIONS
class CPhoneGsmOptionContainer;

// CONSTANTS

/*
* EMERGENCY NUMBER UIDS
*/
const TUint32 KPhoneUidVoipNumber = 
    PHONE_MAKE_UID( KPhoneUidDialVoipyNumber, 0 );

// CLASS DECLARATION

/**
* It is parser for voip numbers.
*
* @since S60 5.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneVoipNumberParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneVoipNumberParser* NewLC();

    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase. 
        * Parse string.
        *
        * @param aString It is string to be parsed.
        * @param aResult It will contain result.
        * @param aOptions It indicates operation options.
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
        CPhoneVoipNumberParser();

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
        * Takes a main part of the phone number( before 'p','w' or 
        * second '+' marks ). 
        * And add it to parameter of the aResult with or without optional 
        * international prefix.
        * @param aLex Contains string to be handled.
        * @param aResult It will contain result.
        */
        void TakeNumberPartL(
            TLex& aLex,
            CPhoneGsmParserResult& aResult );

        /**
        * Takes Dtmf postfix and add it to parameter of the aResult.
        * @param aLex Contains string to be handled.
        * @param aResult It will contain result.
        */
        void TakeDtmfPostfixL(
            TLex& aLex,
            CPhoneGsmParserResult& aResult );
        
        /**
        * Checks that string contains only valid dtmf characters 
        * @param aString string to be checked
        * @return ETrue if valid dtmf string 
        */
        TBool IsValidDtmfString( TPtrC& aString );

    };

#endif      // CPHONEVOIPNUMBERPARSER_H
            
// End of File
