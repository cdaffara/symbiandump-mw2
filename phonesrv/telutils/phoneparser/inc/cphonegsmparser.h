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
* Description:  It is class that provides parsing functionality.
*
*/


#ifndef CPHONEGSMPARSER_H
#define CPHONEGSMPARSER_H

// INCLUDES
#include    <e32base.h>
#include    "cphonegsmparserbase.h" 

// FORWARD DECLARATIONS
class CPhoneGsmParserResult;
class CPhoneGsmOptionContainerBase;
class CPhoneGsmOptionContainer;

// CLASS DECLARATION

/**
* It is class that provides parsing functionality.
*
* @since 1.0
* @lib phoneparser.lib.
*/
NONSHARABLE_CLASS( CPhoneGsmParser )
    : public CPhoneGsmParserBase
    {           
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        *
        * Note that ownership is transferred!
        */
        static CPhoneGsmParser* NewL(
            CPhoneGsmParserBase* aSsProcedure,
            CPhoneGsmParserBase* aSimControlProcedure,
            CPhoneGsmParserBase* aPhoneNumber,
            CPhoneGsmParserBase* aManufacturerSpecific,
            CPhoneGsmParserBase* aEmergencyNumberParser,
            CPhoneGsmParserBase* iVoipNumber);
        
        /**
        * Destructor.
        */
        ~CPhoneGsmParser();

    public: // New functions
        
        /**
        * From CPhoneGsmParserBase.
        * Parses the string.
        *
        * @param aString It is string to be parsed.
        * @param aResult It will contain the results of parsing.
        * @param aOptions It is option container.
        */
        TBool ParseL( 
            const TDesC& aString, 
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );

    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmParser(
            CPhoneGsmParserBase* aSsProcedure,
            CPhoneGsmParserBase* aSimControlProcedure,
            CPhoneGsmParserBase* aPhoneNumber,
            CPhoneGsmParserBase* aManufacturerSpecific,
            CPhoneGsmParserBase* aEmergencyNumberParser,
            CPhoneGsmParserBase* iVoipNumber);

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Parses the string.
        */
        TBool DoParseL( 
            const TDesC& aString, 
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainer& aOptions );

        /**
        * Define string as unstructured supplementary service.
        */
        TBool DefineAsUssdL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainer& aOptions );
        
    private:    // Data

        // Owned supplementary service procedurwe parser.
        CPhoneGsmParserBase* iGsmSsProcedure;

        // Owned SIM control procedure parser.
        CPhoneGsmParserBase* iGsmSimControlProcedure;

        // Owned phone number parser.
        CPhoneGsmParserBase* iGsmPhoneNumber;

        // Owned manufacturer specific procedure parser.
        CPhoneGsmParserBase* iGsmManufacturerProcedure;
        
        // Owned emergency number parser.
        CPhoneGsmParserBase* iGsmEmergencyNumber;
        
        // Owned voip number parser.
        CPhoneGsmParserBase* iVoipNumber;
    };

#endif      // CPHONEGSMPARSER_H
            
// End of File
