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
* Description:  It is class containing static methods to create 
*                instances of parser and parser results.
*
*/


#ifndef PHONEGSMPARSER_H
#define PHONEGSMPARSER_H

// INCLUDES
#include    <e32base.h>

// FORWARD DECLARATIONS
class CPhoneGsmParserBase;
class CPhoneGsmParserResult;
class CPhoneGsmOptionContainerBase;
class CPhoneParserFeatures;

// CLASS DECLARATION

/**
* It is class containing static methods to create 
* instances of parser and parser results.
*
* @since 1.0
* @lib phoneparser.lib
*/
class PhoneGsmParser 
    {
    public: // New functions
           
        /**
        * Creates an instance of CPhoneGsmParser, GSM specific parser.
        *
        * @return Returns new instance of CPhoneGsmParser.
        */
        IMPORT_C static CPhoneGsmParserBase* 
            CreateParserL();

        /**
        * Creates an instance of CPhoneGsmParserResult.
        *
        * @return Returns new instance of CPhoneGsmParserResult.
        */
        IMPORT_C static CPhoneGsmParserResult* 
            CreateResultL();

        /**
        * Creates option container.
        *
        * Note that options are defined and set to initial status.
        *
        * @return Returns new instance of CPhoneGsmOptionContainerBase.
        */
        IMPORT_C static CPhoneGsmOptionContainerBase*
            CreateOptionContainerL();
        
        /**
        * Enumerates all common types.
        */
        enum TContentType
            {
            EContentInvalid,
            EContentUnknown, // but still valid
            EContentPhoneNumber,
            EContentSupplementaryService,
            EContentUnstructuredService
            };

        /**
        * Determines the type of content.
        */
        IMPORT_C static TContentType DetermineContentType( 
            const CPhoneGsmParserResult& aResult );

        /**
        * Checks if it is allowed for arriving call.
        *
        * Note that this is ui issue. This could be done otherwise,
        * but this is clearly the simplest option.
        *
        * @return ETrue if allowed.
        */
        IMPORT_C static TBool IsAllowedForArriving(
            const CPhoneGsmParserResult& aResult );

        /**
        * Checks if it is allowed for bad sim state.
        *
        * Note that this is ui issue. This could be done otherwise,
        * but this is clearly the simplest option.
        *
        * @return ETrue if allowed.
        */
        IMPORT_C static TBool IsAllowedForBadSim(
            const CPhoneGsmParserResult& aResult );

#ifdef __WINS__
        /**
        * For testing purposes.
        *
        * @param aStub feature stub.
        */
        IMPORT_C static void UseFeatureStub( CPhoneParserFeatures* aStub );
#endif // __WINS__

        /**
        * It enumerates the reasons for panics in parser.
        */
        enum TPanicReason
            {
            EIncorrectServiceDescription,
            EIncorrectUse,
            EOptionNotDefined,
            EOptionAlreadyDefined,
            EShouldNeverHappen
            };

        /**
        * Panic!
        *
        * @param aReason It is the reason for panic.
        */
        static void Panic( TPanicReason aReason );

    private:

        /** 
        * Define all options.
        *
        * @param aOptions It is the option container.
        */
        static void DefineOptionsL( 
            CPhoneGsmOptionContainerBase& aOptions );
        
        /**
        * Initialise options to default values.
        */
        static void InitialiseOptionsToDefaultValues(
                CPhoneGsmOptionContainerBase& aOptions );

        /**
        * Creates parser for manufacturer specific procedures.
        */
        static CPhoneGsmParserBase* CreateManufacturerProcedureParserLC();

    };

#endif      // PHONEGSMPARSER_H
            
// End of File
