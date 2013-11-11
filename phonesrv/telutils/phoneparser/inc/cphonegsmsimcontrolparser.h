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
* Description:  It is parser for SIM control procedures.
*
*/


#ifndef CPHONEGSMSIMCONTROLPARSER_H
#define CPHONEGSMSIMCONTROLPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 

// CONSTANTS

/**
* SIM CONTROL UIDS
*/

const TUint32 KPhoneUidPinChange = 
    PHONE_MAKE_UID( KPhoneGsmUidSimControlProcedure, 1 );
const TUint32 KPhoneUidPin2Change = 
    PHONE_MAKE_UID( KPhoneGsmUidSimControlProcedure, 2 );
const TUint32 KPhoneUidPinUnblock = 
    PHONE_MAKE_UID( KPhoneGsmUidSimControlProcedure, 3 );
const TUint32 KPhoneUidPin2Unblock = 
    PHONE_MAKE_UID( KPhoneGsmUidSimControlProcedure, 4 );

// CLASS DECLARATION

/**
* It parses sim control procedures.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmSimControlParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmSimControlParser* NewLC();

    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase, parses sim control procedures.
        *
        * @param aString It is string to be parsed.
        * @param aResult It will contain result.
        * @param aSendOperation It indicates if it would be SEND operation.
        * @return It returns boolean value indicating success of parsing.
        */    
        TBool ParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );
        
    private:

        /**
        * It is type of parameter checking procedures.
        *
        * @param aDesc It is the parameter.
        * @return Returns if the parameter is acceptable.
        */ 
        typedef TBool (*TParameterValidationFunction)( const TDesC& aParam );

        /**
        * C++ default constructor.
        */
        CPhoneGsmSimControlParser();

        /**
        * Parse sim control procedures.
        */
        TBool DoParseL(
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            TBool aSendOperation,
            TBool aInCall );

        /**
        * Handle pin operations (pin change, unblock)
        */
        TBool HandlePinOperationsL( 
            TLex& aInput, 
            CPhoneGsmParserResult& aResult );

        /**
        * Check that there is '**' in the beginning of input.
        */
        TBool HandlePinPrefix( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle pin change code.
        */
        TBool HandlePinChangeL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle pin unblock code.
        */
        TBool HandlePinUnblockL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle no more parameters (only hash).
        */
        TBool HandleNoParameters(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle pin password.
        */
        TBool HandlePinL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle pin unblocking key.
        */
        TBool HandlePukL(
            TLex& aInput, 
            CPhoneGsmParserResult& aResult );

        /**
        * Handle parameters.
        */
        TBool HandleParameterL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult,
            TParameterValidationFunction aFunc );

        /**
        * Returns number.
        */
        TPtrC NextNumber( TLex& aInput );

        /**
        * Validate syntax of pin.
        */
        static TBool ValidatePin( const TDesC& aParam );

        /**
        * Validate syntax of puk.
        */
        static TBool ValidatePuk( const TDesC& aParam );

    };

#endif      // CPHONEGSMSIMCONTROLPARSER_H
            
// End of File
