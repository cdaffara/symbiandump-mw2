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
* Description:  It is parser for PCN specific procedures.
*
*/


#ifndef CPHONEGSMPCNPROCEDUREPARSER_H
#define CPHONEGSMPCNPROCEDUREPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 

// CONSTANTS

/*
* Perfom SIM locking.
*/
const TUint32 KPhoneUidSimLockCode =
    PHONE_MAKE_UID( KPhoneGsmUidPcnProcedures, 0 );

/*
* Perform SIM unlocking. 
*/
const TUint32 KPhoneUidSimUnlockCode =
    PHONE_MAKE_UID( KPhoneGsmUidPcnProcedures, 1 );

// CLASS DECLARATION

/**
* It is parser for GsmPcn specific procedures.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmPcnProcedureParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmPcnProcedureParser* NewLC();
        
    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase, parses string.
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
        TBool ParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );

    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmPcnProcedureParser();

        /**
        * Handles SIM lock closing.
        */
        TBool HandleSimLockCodeL(
            const TDesC& aString,
            CPhoneGsmParserResult& aResult );

        /**
        * Handles SIM lock opening.
        */
        TBool HandleSimUnlockCodeL(
            const TDesC& aString,
            CPhoneGsmParserResult& aResult );       

        /**
        * Handle lock commands.
        *
        * @param aString string.
        * @param aResult result.
        */
        TBool HandleLockCodeParametersL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult );

    };

#endif      // CPHONEGSMPCNPROCEDUREPARSER_H
            
// End of File
