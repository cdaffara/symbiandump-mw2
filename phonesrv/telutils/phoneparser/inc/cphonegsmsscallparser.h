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
* Description:  It is responsible of parsing GSM specified supplementary 
*                services that can be entered during calls.
*
*/


#ifndef CPHONEGSMSSCALLPARSER_H
#define CPHONEGSMSSCALLPARSER_H

// INCLUDES
#include    <e32base.h>
#include    "cphonegsmparserbase.h" 

// CONSTANTS

/*
* SUPPLEMENTARY SERVICE UIDS - DURING CALLS
*
* Final commands depend on the state of the phone and that's why they
* decided in handler.
*/

const TUint32 KPhoneUidCommand0 =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryServiceDuringCall, 0 );
const TUint32 KPhoneUidCommand1 =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryServiceDuringCall, 1 );
const TUint32 KPhoneUidCommand2 =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryServiceDuringCall, 2 );
const TUint32 KPhoneUidCommand3 =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryServiceDuringCall, 3 );
const TUint32 KPhoneUidCommand4 =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryServiceDuringCall, 10 );

// CLASS DECLARATION

/**
* It is responsible of parsing GSM specified supplementary services 
* that can be entered during calls.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmSsCallParser )
    : public CPhoneGsmParserBase
    {
    public: // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmSsCallParser* NewLC();
        
    public: // Functions from base classes
        
        /**
        * From CPhoneGsmParserBase. 
        * Parses ss strings.
        *
        * @param aString string to parse.
        * @param aResult It will contain result.
        * @param aOptions It contains options.
        * @return ETrue iff parsed successfully.
        */
        TBool ParseL( 
            const TDesC& aString, 
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );
                
    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmSsCallParser();
      
    };

#endif      // CPHONEGSMSSCALLPARSER_H
            
// End of File
