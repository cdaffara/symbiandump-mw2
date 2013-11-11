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
* Description:  It is parser class for manufacturer specific codes.
*
*/


#ifndef CPHONEGSMMANUFACTURERPARSER_H
#define CPHONEGSMMANUFACTURERPARSER_H

// INCLUDES
#include    "cphonegsmparserbase.h" 

// FORWARD DECLARATIONS
class CPhoneGsmOptionContainer;

// CONSTANTS

/*
* MANUFACTURER CODE UIDS
*/

const TUint32 KPhoneUidManufacturerCode = 
    PHONE_MAKE_UID( KPhoneGsmUidManufacturer, 0 );

const TUint32 KPhoneUidManufacturerDebugCode =
    PHONE_MAKE_UID( KPhoneGsmUidManufacturer, 1 );

// MACROS

// This definition enables parsing of debug code. 
//#define PHONEPARSER_PARSE_DEBUG_CODE

// CLASS DECLARATION

/**
* It is parser for manufacturer specific codes.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmManufacturerParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmManufacturerParser* NewLC();
        
        /**
        * Destructor.
        */
        ~CPhoneGsmManufacturerParser();

    public: // New functions

        /**
        * Enumerates all flags.
        *
        * EFlagCode       - string is a code, checked only when # is pressed.
        * EFlagSend       - SEND key must be pressed.
        * EFlagPrefix     - string is just a prefix.
        * EFlagNotInIdle   - not in from idle state.
        * EFlagNotInInCall - not in during incall.
        */
        enum TFlags
            {
            EFlagCode = 0x01,        
            EFlagSend = 0x02,        
            EFlagPrefix = 0x04,
            EFlagNotInIdle = 0x08,
            EFlagNotInInCall = 0x10
            };

        /**
        * Add string to the set of manufacturer specific codes.
        *
        * Note that string must exist as long as the instance.
        *
        * @param aString string
        * @param aFlags flags, see TFlags
        * @param aCommand command id
        * @param aFeatureId feature manager id.
        */
        void AddStringL( 
            const TDesC& aString, 
            TInt aFlags, 
            TUint aCommand,
            TInt aFeatureId = 0 );

    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase.
        * Parse string.
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

        class TCodeInfo;

        /**
        * C++ default constructor.
        */
        CPhoneGsmManufacturerParser();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Checks code.
        *
        * @param aString string to be checked.
        * @param aInfo code information.
        * @param aOptions options.
        */
        TBool CheckCode( 
            const TDesC& aString, 
            const TCodeInfo& aInfo,
            CPhoneGsmOptionContainerBase& aOptions );

        /**
        * Is situation ok for string.
        */
        TBool CheckSituation( 
            const TDesC& aString,
            TInt aFlags,
            CPhoneGsmOptionContainerBase& aOptions );
            
    private:    // Data

        /**
        * It defines structure that contains information of single code.
        */
        class TCodeInfo
            {
            public:
                TPtrC iString;
                TInt iFlags;
                TUint iCommand;
                TInt iFeatureId;
            };
        
        // Type definition for code array.
        typedef CArrayFixFlat< TCodeInfo > CCodeArray;

        // Owned array of codes.
        CCodeArray* iArray;

    };

#endif      // CPHONEGSMMANUFACTURERPARSER_H
            
// End of File
