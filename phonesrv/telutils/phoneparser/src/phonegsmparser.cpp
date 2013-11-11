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
* Description:  Utility functions for clients.
*
*/


// INCLUDE FILES
#include    "phonegsmparser.h" 
#include    "cphonegsmparser.h" 
#include    "cphonegsmparserbase.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 

#include    "cphonegsmparsercontainer.h" 
#include    "cphonegsmssparser.h" 
#include    "cphonegsmsimcontrolparser.h" 
#include    "cphonegsmphonenumberparser.h" 
#include    "cphonegsmsscallparser.h" 
#include    "cphonegsmmanufacturerparser.h" 
#include    "cphonegsmpcnprocedureparser.h" 
#include    "cphonegsmmanufacturerhandler.h" 
#include    "cphonegsmemergencynumberparser.h" 
#include    "cphoneparserfeatures.h" 
#include    "cphonevoipnumberparser.h" 

// CONSTANTS
const TInt KPhoneParserResultParameterReserver = 10;
_LIT( KPhoneParserPanic, "PhoneParser" );

// Software version display code
_LIT( KPhoneCodeBadPinChange, "**04*" );
_LIT( KPhoneCodeBadPin2Change, "**042*" );
_LIT( KPhoneCodeBadPinUnblock, "**05*" );
_LIT( KPhoneCodeBadPin2Unblock, "**052*" );
_LIT( KPhoneCodeActivateRfsNormal, "*#7780#" );
_LIT( KPhoneCodeActivateRfsDeep, "*#7370#" );
_LIT( KPhoneCodeBtAddress, "*#2820#" );
_LIT( KPhoneCodeRFLoopback, "*#9990#" );
_LIT( KPhoneCodeWLANMAC, "*#62209526#" );
_LIT( KPhoneCodeBtDebugMode, "*#2873#" );
                  
// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PhoneGsmParser::CreateParserL
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneGsmParserBase* PhoneGsmParser::CreateParserL()
    {
    // 1. SS CONTAINER TO STACK
    CPhoneGsmParserContainer* ssContainer =
        CPhoneGsmParserContainer::NewLC();

    CPhoneGsmParserBase* ssProcedure = 
        CPhoneGsmSsParser::NewLC();
    ssContainer->AddL( *ssProcedure );
    CleanupStack::Pop( ssProcedure );

    CPhoneGsmParserBase* ssCallProcedure =
        CPhoneGsmSsCallParser::NewLC();
    ssContainer->AddL( *ssCallProcedure );
    CleanupStack::Pop( ssCallProcedure );
    
    // 2. SIM CONTROL PROCEDURE TO STACK
    CPhoneGsmParserBase* simControlProcedure = 
        CPhoneGsmSimControlParser::NewLC();
   
    // 3. PHONE NUMBER TO STACK
    CPhoneGsmParserBase* phoneNumber =
        CPhoneGsmPhoneNumberParser::NewLC();

    // 4. MANUFACTURER SPECIFIC TO STACK
    CPhoneGsmParserBase* manufacturerSpecific = 
        CreateManufacturerProcedureParserLC();
        
    // 5. EMERGENCY NUMBER PARSER TO STACK
    CPhoneGsmParserBase* emergencyNumberParser =
        CPhoneGsmEmergencyNumberParser::NewLC();
        
    // 6. VOIP NUMBER PARSER TO STACK
    CPhoneGsmParserBase* voipNumberParser =
        CPhoneVoipNumberParser::NewLC();
    
    CPhoneGsmParser* parser = 
        CPhoneGsmParser::NewL(
            ssContainer, 
            simControlProcedure,
            phoneNumber,
            manufacturerSpecific,
            emergencyNumberParser,
            voipNumberParser);

    CleanupStack::Pop( 6, ssContainer );

    return parser;
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::UseFeatureStub
// -----------------------------------------------------------------------------
//
#ifdef __WINS__
EXPORT_C void PhoneGsmParser::UseFeatureStub( CPhoneParserFeatures* aStub )
    {
    CPhoneParserFeatures::SetInstance( aStub );
    }
#endif // __WINS__

// -----------------------------------------------------------------------------
// PhoneGsmParser::CreateResultsL
//
// Create parser so that AddParameterL shouldn't leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneGsmParserResult* PhoneGsmParser::CreateResultL()
    {
    CPhoneGsmParserResult* result = CPhoneGsmParserResult::NewL();
    
    CleanupStack::PushL( result );
    result->ReserveParametersL( KPhoneParserResultParameterReserver );
    CleanupStack::Pop( result );

    result->ClearL();
    return result;
    }


// -----------------------------------------------------------------------------
// PhoneGsmParser::CreateOptionContainerL
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneGsmOptionContainerBase* PhoneGsmParser::CreateOptionContainerL()
    {
    CPhoneGsmOptionContainer* options =
        CPhoneGsmOptionContainer::NewL();
    
    CleanupStack::PushL( options );

    DefineOptionsL( *options );
    InitialiseOptionsToDefaultValues( *options );

    CleanupStack::Pop( options );
    return options;
    }


// -----------------------------------------------------------------------------
// PhoneGsmParser::InitialiseOptionsToDefaultValues
// -----------------------------------------------------------------------------
//
void PhoneGsmParser::InitialiseOptionsToDefaultValues( 
        CPhoneGsmOptionContainerBase& aOptions )
    {
    aOptions.SetOptionStatus( KPhoneOptionSend, EFalse );
    aOptions.SetOptionStatus( KPhoneOptionInCall, EFalse );
    aOptions.SetOptionStatus( KPhoneOptionVoipCall, EFalse );
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::DetermineContentType
//
// It is helper method to categorize processed strings.
// -----------------------------------------------------------------------------
//
EXPORT_C PhoneGsmParser::TContentType 
    PhoneGsmParser::DetermineContentType( 
        const CPhoneGsmParserResult& aResult )
    {
    TContentType type = EContentUnknown;
    TUint uid = aResult.Uid();
    TUint main = PHONE_EXTRACT_MAIN( aResult.Uid() );

    switch ( main )
        {
        case KPhoneGsmUidInvalid:
            type = EContentInvalid;
            break;

        case KPhoneGsmUidMisc:
            if ( uid == KPhoneUidUnstructuredService )
                {
                type = EContentUnstructuredService;
                }
            break;

        case KPhoneGsmUidSupplementaryService:
        case KPhoneGsmUidSupplementaryServiceDuringCall:
            type = EContentSupplementaryService;
            break;

        case KPhoneGsmUidDialPhoneNumber:
            type = EContentPhoneNumber;
            break;

        default:
            break;
        }

    return type;
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::IsAllowedForArriving
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PhoneGsmParser::IsAllowedForArriving(
        const CPhoneGsmParserResult& aResult )
    {  
    TUint32 uid = aResult.Uid();
    
    // Allowed:
    //      0 SEND
    //      All manufacturer codes
    //      All sim control procedures
    //      All pcn procedures
    
    return ( uid == KPhoneUidCommand0 ||
             uid == KPhoneUidManufacturerDebugCode ||
             uid == KPhoneUidManufacturerCode || 
             PHONE_EXTRACT_MAIN( uid ) == KPhoneGsmUidSimControlProcedure ||
             PHONE_EXTRACT_MAIN( uid ) == KPhoneGsmUidPcnProcedures );
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::IsAllowedForBadSim
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PhoneGsmParser::IsAllowedForBadSim(
        const CPhoneGsmParserResult& aResult )
    {
    TUint32 uid = aResult.Uid();
    TUint aux = aResult.AuxInformation();

    // Allowed:
    //      DialEmergency
    //      Some manufacturer codes
    //      All sim control procedures
    //      All pcn procedures
    
    return ( uid == KPhoneUidEmergencyNumber ||
             uid == KPhoneUidManufacturerDebugCode ||
             ( uid == KPhoneUidManufacturerCode && 
               ( aux == CPhoneGsmManufacturerHandler::EBadPinChange ||
                 aux == CPhoneGsmManufacturerHandler::EBadPin2Change ||
                 aux == CPhoneGsmManufacturerHandler::EBadPinUnblock ||
                 aux == CPhoneGsmManufacturerHandler::EBadPin2Unblock ) ) ||

             PHONE_EXTRACT_MAIN( uid ) == KPhoneGsmUidSimControlProcedure ||
             PHONE_EXTRACT_MAIN( uid ) == KPhoneGsmUidPcnProcedures );
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::Panic
//
// Panic for phone parser.
// -----------------------------------------------------------------------------
//
void PhoneGsmParser::Panic( 
        TPanicReason aReason )
    {
    User::Panic( KPhoneParserPanic, aReason );
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::DefineOptionsL
// -----------------------------------------------------------------------------
//
void PhoneGsmParser::DefineOptionsL( 
        CPhoneGsmOptionContainerBase& aOptions )
    {
    CPhoneGsmOptionContainer& options =
            static_cast<CPhoneGsmOptionContainer&>( aOptions );
    
    options.DefineOptionL( KPhoneOptionSend, EFalse );
    options.DefineOptionL( KPhoneOptionInCall, EFalse );
    options.DefineOptionL( KPhoneOptionVoipCall, EFalse );
    }

// -----------------------------------------------------------------------------
// PhoneGsmParser::CreateManufacturerProcedureParserLC
// -----------------------------------------------------------------------------
//
CPhoneGsmParserBase* 
    PhoneGsmParser::CreateManufacturerProcedureParserLC()
    {
    // CONTAINER TO STACK.
    CPhoneGsmParserContainer* manufacturer =
        CPhoneGsmParserContainer::NewLC();

    CPhoneGsmPcnProcedureParser* gsmPcnCodes =
        CPhoneGsmPcnProcedureParser::NewLC();
    manufacturer->AddL( *gsmPcnCodes );
    CleanupStack::Pop( gsmPcnCodes );

    CPhoneGsmManufacturerParser* manuCodes =
        CPhoneGsmManufacturerParser::NewLC();
    manufacturer->AddL( *manuCodes );
    CleanupStack::Pop( manuCodes );

    manuCodes->AddStringL( 
        KPhoneCodeBadPinChange,
        CPhoneGsmManufacturerParser::EFlagCode + 
        CPhoneGsmManufacturerParser::EFlagPrefix,
        CPhoneGsmManufacturerHandler::EBadPinChange );

    manuCodes->AddStringL( 
        KPhoneCodeBadPin2Change,
        CPhoneGsmManufacturerParser::EFlagCode + 
        CPhoneGsmManufacturerParser::EFlagPrefix,
        CPhoneGsmManufacturerHandler::EBadPin2Change );

    manuCodes->AddStringL( 
        KPhoneCodeBadPinUnblock,
        CPhoneGsmManufacturerParser::EFlagCode + 
        CPhoneGsmManufacturerParser::EFlagPrefix,
        CPhoneGsmManufacturerHandler::EBadPinUnblock );

    manuCodes->AddStringL( 
        KPhoneCodeBadPin2Unblock,
        CPhoneGsmManufacturerParser::EFlagCode + 
        CPhoneGsmManufacturerParser::EFlagPrefix,
        CPhoneGsmManufacturerHandler::EBadPin2Unblock );

    manuCodes->AddStringL(
        KPhoneCodeActivateRfsNormal,
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EActivateRfsNormal );

    manuCodes->AddStringL(
        KPhoneCodeActivateRfsDeep,
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EActivateRfsDeep );

    manuCodes->AddStringL(
        KPhoneCodeBtAddress,
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EShowBtAddress,
        KFeatureIdBt );

    manuCodes->AddStringL(
        KPhoneCodeWLANMAC, 
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EShowWlanMac,
        KFeatureIdProtocolWlan );       

    manuCodes->AddStringL( 
        KPhoneCodeRFLoopback,
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EBTLoopback,
        KFeatureIdBtTestMode );

    manuCodes->AddStringL( 
        KPhoneCodeBtDebugMode,
        CPhoneGsmManufacturerParser::EFlagCode,
        CPhoneGsmManufacturerHandler::EBTDebugMode,
        KFeatureIdBt );

    return manufacturer;
    }

//  End of File  
