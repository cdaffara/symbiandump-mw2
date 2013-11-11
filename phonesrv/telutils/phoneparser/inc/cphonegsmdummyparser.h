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
* Description:  It is a parser that parses nothing.
*
*/


#ifndef CPHONEGSMDUMMYPARSER_H
#define CPHONEGSMDUMMYPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 

// CLASS DECLARATION

/**
* It is a parser that parses nothing.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmDummyParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmDummyParser* NewL();

    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase, parse string.
        *
        * @param aString It is string to be parsed.
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
        * C++ constructor.
        */
        CPhoneGsmDummyParser();

    };

#endif      // CPHONEGSMDUMMYPARSER_H
            
// End of File
