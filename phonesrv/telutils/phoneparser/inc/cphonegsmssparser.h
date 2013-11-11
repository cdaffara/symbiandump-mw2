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
* Description:  It is responsible of parsing of GSM specified supplementary 
*                service requests.
*
*/


#ifndef CPHONEGSMSSPARSER_H
#define CPHONEGSMSSPARSER_H

// INCLUDES
#include    <e32base.h>
#include    "cphonegsmparserbase.h" 

// CONSTANTS

/*
* SUPPLEMENTARY SERVICE UIDS
*/

/*
* Now follows uids for all supported supplementary services.
*/
const TUint32 KPhoneUidForwardingAll = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 0 );
const TUint32 KPhoneUidForwardingAllConditional = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 1 );
const TUint32 KPhoneUidForwardingUnconditional = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 2 );
const TUint32 KPhoneUidForwardingNoReply = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 3 );
const TUint32 KPhoneUidForwardingNotReachable = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 4 );
const TUint32 KPhoneUidForwardingBusy = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 5 );
const TUint32 KPhoneUidBarringAllOutgoingCalls = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 6 );
const TUint32 KPhoneUidBarringAllIncomingCalls =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 7 );
const TUint32 KPhoneUidBarringAllServices =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 8 );
const TUint32 KPhoneUidBarringAllOutgoingInternationalCalls =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 9 );
const TUint32 KPhoneUidBarringAllOutgoingInternationalCallsExceptHome =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 10 );
const TUint32 KPhoneUidBarringAllOutgoingServices = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 11 );
const TUint32 KPhoneUidBarringAllIncomingCallsWhenRoaming =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 12 );
const TUint32 KPhoneUidBarringAllIncomingServices = 
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 13 );
const TUint32 KPhoneUidWaiting =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 14 );
const TUint32 KPhoneUidCallingLineIdentificationPresentation =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 15 );
const TUint32 KPhoneUidCallingLineIdentificationRestriction =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 16 );
const TUint32 KPhoneUidConnectedLinePresentation =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 17 );
const TUint32 KPhoneUidConnectedLinePresentationRestriction =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 18 );
const TUint32 KPhoneUidPassword =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 19 );
const TUint32 KPhoneUidCallingNamePresentation =
    PHONE_MAKE_UID( KPhoneGsmUidSupplementaryService, 20 );

/**
* It defines the maximum lenght of service code.
*/
const TInt KPhoneGsmSsServiceCodeMaxLength = 5;

// CLASS DECLARATION

/**
* It is responsible of parsing GSM specified supplementary services that
* are supported.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmSsParser )
    : public CPhoneGsmParserBase
    {
    public: // Types

        /**
        * Enumeration for different service types.
        */
        enum TServiceType
            {
            EServiceActivation,
            EServiceDeactivation,
            EServiceInterrogation,
            EServiceRegistration,
            EServiceErasure
            };

        /** 
        * Enumeration for different syntax.
        *
        * ESyntaxNotAllowed
        *   Not allowed.
        *
        * ESyntaxNoParameters
        *   No parameters. 
        *
        *     SS_code#
        *
        * ESyntaxEmptyOrBasicCode
        *   No parameters or empty and basic code.
        *
        *     SS_code# or SS_code**BB#             
        *
        * ESyntaxNumberOptBasicCodeOptTimer
        *   Number parameter or number parameter and basic code or
        *   number parameter and basic code and timer or number parameter
        *   and timer.
        *
        *     SS_code*NN# or SS_code*NN*BB# or SS_code*NN*BB*TT# or
        *     SS_code*NN**TT#
        *
        * ESyntaxNumberOptBasicCode
        *   Number parameter or number parameter and basic code.
        *
        *     SS_code*NN# or SS_code*NN*BB#
        *
        * ESyntaxPasswordOptBasicCode
        *   Password or password and basic code. 
        *
        *     SS_code*PPPP# or SS_code*PPPP*BB#
        *
        * ESyntaxOptGroupCodePasswordNewPasswordNewPassword
        *
        *   Register group code and password and new password and 
        *   new password. Or same with empty register group.
        *
        *     SS_code*xxx*PPPP*WWWW*WWWW#         
        *     SS_code**PPPP*WWWW*WWWW#
        *
        * ESyntaxEmptyOrBasicCode2    
        *   Empty or Basic code 
        *
        *     SS_code# or SS_code*BB#
        */
        enum TServiceSyntax
            {
            ESyntaxNotAllowed,
            ESyntaxNoParameters, 
            ESyntaxEmptyOrBasicCode, 
            ESyntaxNumberOptBasicCodeOptTimer,
            ESyntaxNumberOptBasicCode,
            ESyntaxPasswordOptBasicCode,
            ESyntaxOptGroupCodePasswordNewPasswordNewPassword,
            ESyntaxEmptyOrBasicCode2
            };

    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmSsParser* NewLC();
        
        /**
        * Destructor.
        */
        ~CPhoneGsmSsParser();

    public: // New functions
        

        /**
        * Pack data to integer.
        *
        * @param aResult It will contain packed data.
        * @param aType type of service.
        * @param aSyntax syntax of service.
        */
        static void PackData(
            TUint& aResult,
            TServiceType aType,
            TServiceSyntax aSyntax );       

        /**
        * Unpack data from integer.
        *
        * @param aData packed data.
        * @param aType type of service.
        * @param aSyntax syntax of service.
        */
        static void UnpackData(
            TUint aData,
            TServiceType& aType,
            TServiceSyntax& aSyntax );
        
// from base class CPhoneGsmParserBase    
        
        /**
         * From CPhoneGsmParserBase.
         * Parses ss strings.
         *
         * @param aString string to be parser.
         * @param aResult It will contain result.
         * @param aOptions It contains options.
         * @return ETrue iff string parsed successfully.
         */
         TBool ParseL(
                 const TDesC& aString, 
                 CPhoneGsmParserResult& aResult,
                 CPhoneGsmOptionContainerBase& aOptions );
        
    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmSsParser();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Handle parameters.
        */
        TBool HandleParametersL( 
            TLex& aInput, 
            CPhoneGsmParserResult& aResult,
            TServiceSyntax aSyntax );

        /**
        * Handle "not allowed" syntax.
        */
        TBool HandleNotAllowedL( 
            TLex& aInput, 
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "no more parameters" syntax.
        */ 
        TBool HandleNoParametersL( 
            TLex& aInput, 
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "empty or basic code" syntax.
        */
        TBool HandleEmptyOrBasicCodeL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "number optionally basic code and timer" syntax.
        */
        TBool HandleNumberOptBasicCodeOptTimerL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "number optionally basic code" syntax.
        */
        TBool HandleNumberOptBasicCodeL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "password optionally basic code" syntax.
        */
        TBool HandlePasswordOptBasicCodeL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "optionally group code, then old password and new
        * password twice" syntax.
        */
        TBool HandleOptGroupCodePasswordNewPasswordNewPasswordL(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "empty or basic code" syntax.
        */
        TBool HandleEmptyOrBasicCode2L(
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "empty parameter" syntax.
        */
        TBool HandleEmptyParameterL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "basic code" syntax.
        */
        TBool HandleBasicCodeL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "directory number" syntax.
        */
        TBool HandleNumberL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "timer" syntax.
        */
        TBool HandleTimerL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "password" syntax.
        */
        TBool HandlePasswordL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Handle "group code" syntax.
        */
        TBool HandleGroupCodeL( 
            TLex& aInput,
            CPhoneGsmParserResult& aResult );

        /**
        * Check basic service code.
        */
        TBool CheckBasicCode( const TDesC& aCode );

        /**
        * Check 'no reply timer'
        */
        TBool CheckTimer( const TDesC& aCode );

        /**
        * Check password group code.
        */ 
        TBool CheckGroupCode( const TDesC& aCode );

        /**
        * Find string (starting from current position) that 
        * contains characters from given string. 
        *
        * (note that empty string may be returned).
        */
        static TPtrC FindString( 
            TLex& aInput,
            const TDesC& aAllowedCharacters );

        /**
        * Add a new service.
        *
        * SHOULD ONLY BE USED DURING CONSTRUCTION.
        *
        * Panics if invalid service description is passed.
        *
        * @param aService It is service description.
        * @param aUid It is uid for the service.
        */
        void AddL( const TDesC& aService, TUint aUid );

        /**
        * Add a new service.
        *
        * SHOULD ONLY BE USED DURING CONSTRUCTION.
        */
        void AddServiceL( 
            const TDesC& aServiceCode,
            TServiceSyntax aActivation,
            TServiceSyntax aDeactivation,
            TServiceSyntax aInterrogation,
            TServiceSyntax aRegistration,
            TServiceSyntax aErasure,
            TUint aUid );

        /**
        * Get next number as descriptor.
        *
        * ONLY TO BE USED DURING SERVICE DESCRIPTION PARSING.
        *
        * Panics if next character (not a space) does not start 
        * a new number or the number is invalid.
        *
        * @param aLex It is lex.
        * @return Returns number.
        */
        static TPtrC NextNumber( TLex& aLex );

        /**
        * Get next number as value.
        *
        * ONLY TO BE USED DURING SERVICE DESCRIPTION PARSING.
        *
        * Panics if next character (not a space) does not start 
        * a new number or the number is invalid.
        *
        * @param aLex It is lex.
        * @return Returns number as value.
        */
        static TUint NextNumberAsValue( TLex& aLex );       

        /**
        * Converts to service syntax.
        *
        * @param aService It is numerical value of service syntax.
        * @return Returns service syntax corresponding to value.
        */
        static TServiceSyntax ConvertToServiceSyntax( TUint aService );

    private:    // Data

        /**
        * It is type for service code.
        */
        typedef TBuf< KPhoneGsmSsServiceCodeMaxLength > TServiceCode;
    
        /**
        * It defines structure containing information about a service.
        */
        class TServiceInfo
            {
            public:
                TServiceCode iCode;
                TServiceSyntax iActivation;
                TServiceSyntax iDeactivation;
                TServiceSyntax iInterrogation;
                TServiceSyntax iRegistration;
                TServiceSyntax iErasure;
                TUint iUid;
            };

        // Type for service code array.
        typedef CArrayFixFlat< TServiceInfo > CServiceArray;

        // Owned array of service codes.
        CServiceArray* iArray;

    };

#endif      // CPHONEGSMSSPARSER_H
            
// End of File
