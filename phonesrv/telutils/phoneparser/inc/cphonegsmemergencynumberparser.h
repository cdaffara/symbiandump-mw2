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
* Description:  It is parser for emergency numbers.
*
*/


#ifndef CPHONEGSMEMERGENCYNUMBERPARSER_H
#define CPHONEGSMEMERGENCYNUMBERPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 
#include    <phclttypes.h> 

// FORWARD DECLARATIONS
class CPhoneGsmOptionContainer;

// CONSTANTS

/*
* EMERGENCY NUMBER UIDS
*/
const TUint32 KPhoneUidEmergencyNumber = 
    PHONE_MAKE_UID( KPhoneGsmUidDialEmergencyNumber, 0 );

// CLASS DECLARATION

/**
* It is parser for emergency numbers.
*
* @since S60 5.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmEmergencyNumberParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmEmergencyNumberParser* NewLC();

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
        CPhoneGsmEmergencyNumberParser();

        /**
        * Parse.
        */
        TBool DoParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainer& aOptions );

    private:    // Data

        // Emergency number.
        TPhCltTelephoneNumber iEmergencyPhoneNumber;

    };

#endif      // CPHONEGSMEMERGENCYNUMBERPARSER_H
            
// End of File
