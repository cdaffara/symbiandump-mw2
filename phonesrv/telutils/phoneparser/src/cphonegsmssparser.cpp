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
* Description:  Principal parser for supplementary service procedures.
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>
#include    <featmgr.h>
#include    <nwdefs.h>

#include    "cphonegsmssparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "phonegsmparser.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "cphoneparserfeatures.h" 

// CONSTANTS

// Granularity of service array
const TInt KPhoneServiceArrayGranularity = 20;

// Activation starts with *. See also registration.
_LIT( KPhoneSsActivation, "*" );

// Deactivation starts with #.
_LIT( KPhoneSsDeactivation, "#" );

// Interrogation starts with *#.
_LIT( KPhoneSsInterrogation, "*#" );

// Registration starts with ** OR *. Case * is handled in the code.
_LIT( KPhoneSsRegistration, "**" );

// Erasure starts with ##.
_LIT( KPhoneSsErasure, "##" );

// It defines characters that can be used in these prefixes (see above).
_LIT( KPhoneSsActionCharacters, "*#" );

// It defines string that ends all ss requests.
_LIT( KPhoneSsEnd, "#" );

// It defines characters that are considered to be digits.
_LIT( KPhoneSsDigits, "1234567890" );

// Asterisk.
const TInt KPhoneSsAsterisk = '*';

// Hash.
const TInt KPhoneSsHash = '#';

// Plus.
const TInt KPhoneSsPlus = '+';

// Remainder: single character
const TInt KPhoneSsRemainderSingle = 1;

// Remainder: still a parameter, but ok ss
const TInt KPhoneSsRemainderMinLength = 2; // something + #

const TInt KPhoneSsPasswordLength = 4; 

// Remainder: passwordk, ok ss
const TInt KPhoneSsRemainderPasswordMinLength = 5; // four digits + #

// Maximum length of directory number.
const TInt KPhoneSsDirectoryNumberMaxLength = 38;
// Maximum length of basic service code.
const TInt KPhoneSsBasicCodeMaxLength = 2;
// Maximum length of group code.
const TInt KPhoneSsGroupCodeMaxLength = 4;
// Maximum length of timer.
const TInt KPhoneSsTimerMaxLength = 2;

// The only acceptable group code (change of barring password)
const TUint KPhoneSsOnlyAllowedGroupCode = 330;

// Pack: shift
const TInt KPhoneGsmPackShift = 8;

// Pack: mask
const TInt KPhoneGsmPackMask = 255;

// -----------------------------------------------------------------------------
// Syntax:
//    
//      SC      Service code
//      A       Activation
//      D       Deactivation
//      I       Interrogation
//      R       Registration
//      E       Erasure
//
//     Text string must contain each of these in the following order
//     "SC A D I R E" (separated by space). Following values correspond
//      
//
//     0 - ENotAllowed       
//     1 - ENoParameters
//     2 - EEmptyOrBasicCode
//     3 - ENumberOptBasicCodeOptTimer
//     4 - ENumberOptBasicCode     
//     5 - EPasswordOptBasicCode
//     6 - EOptGroupCodePasswordNewPasswordNewPassword    
//     7 - EEmptyOrBasicCode2         
// -----------------------------------------------------------------------------
_LIT( KPhoneUidForwardingAllCode,                                   "002 2 2 0 3 2" );
_LIT( KPhoneUidForwardingAllConditionalCode,                        "004 2 2 0 3 2" );
_LIT( KPhoneUidForwardingUnconditionalCode,                         "21  2 2 2 4 2" );
_LIT( KPhoneUidForwardingNoReplyCode,                               "61  2 2 2 3 2" );
_LIT( KPhoneUidForwardingNotReachableCode,                          "62  2 2 2 4 2" );
_LIT( KPhoneUidForwardingBusyCode,                                  "67  2 2 2 4 2" );
_LIT( KPhoneUidBarringAllOutgoingCallsCode,                         "33  5 5 1 0 0" );
_LIT( KPhoneUidBarringAllIncomingCallsCode,                         "35  5 5 1 0 0" );
_LIT( KPhoneUidBarringAllServicesCode,                              "330 0 5 0 0 0" );
_LIT( KPhoneUidBarringAllOutgoingInternationalCallsCode,            "331 5 5 1 0 0" );
_LIT( KPhoneUidBarringAllOutgoingInternationalCallsExceptHomeCode,  "332 5 5 1 0 0" );
_LIT( KPhoneUidBarringAllOutgoingServicesCode,                      "333 0 5 0 0 0" );
_LIT( KPhoneUidBarringAllIncomingCallsWhenRoamingCode,              "351 5 5 1 0 0" );
_LIT( KPhoneUidBarringAllIncomingServicesCode,                      "353 0 5 0 0 0" );
_LIT( KPhoneUidWaitingCode,                                         "43  7 7 7 0 0" );
_LIT( KPhoneUidCallingLineIdentificationPresentationCode,           "30  0 0 1 0 0" );
_LIT( KPhoneUidCallingLineIdentificationRestrictionCode,            "31  0 0 1 0 0" );
_LIT( KPhoneUidConnectedLinePresentationCode,                       "76  0 0 1 0 0" );
_LIT( KPhoneUidConnectedLinePresentationRestrictionCode,            "77  0 0 1 0 0" ); 
_LIT( KPhoneUidPasswordCode,                                        "03  6 0 0 6 0" );
_LIT( KPhoneUidCallingNamePresentationCode,                         "300 0 0 1 0 0" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::CPhoneGsmSsParser
// -----------------------------------------------------------------------------
//
CPhoneGsmSsParser::CPhoneGsmSsParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsParser::ConstructL()
    {
    iArray = 
        new ( ELeave ) CServiceArray( 
            KPhoneServiceArrayGranularity );

    // Check AddL method for details what these really mean.
    
    // Forwarding
    AddL( KPhoneUidForwardingAllCode, 
        KPhoneUidForwardingAll );
    AddL( KPhoneUidForwardingAllConditionalCode, 
        KPhoneUidForwardingAllConditional );
    AddL( KPhoneUidForwardingUnconditionalCode, 
        KPhoneUidForwardingUnconditional );
    AddL( KPhoneUidForwardingNoReplyCode, 
        KPhoneUidForwardingNoReply );
    AddL( KPhoneUidForwardingNotReachableCode, 
        KPhoneUidForwardingNotReachable );
    AddL( KPhoneUidForwardingBusyCode, 
        KPhoneUidForwardingBusy );

    // Barring
    AddL( KPhoneUidBarringAllOutgoingCallsCode, 
        KPhoneUidBarringAllOutgoingCalls );
    AddL( KPhoneUidBarringAllIncomingCallsCode, 
        KPhoneUidBarringAllIncomingCalls );
    AddL( KPhoneUidBarringAllServicesCode, 
        KPhoneUidBarringAllServices );
    AddL( KPhoneUidBarringAllOutgoingInternationalCallsCode, 
        KPhoneUidBarringAllOutgoingInternationalCalls );
    AddL( KPhoneUidBarringAllOutgoingInternationalCallsExceptHomeCode, 
        KPhoneUidBarringAllOutgoingInternationalCallsExceptHome );
    AddL( KPhoneUidBarringAllOutgoingServicesCode, 
        KPhoneUidBarringAllOutgoingServices );
    AddL( KPhoneUidBarringAllIncomingCallsWhenRoamingCode, 
        KPhoneUidBarringAllIncomingCallsWhenRoaming );
    AddL( KPhoneUidBarringAllIncomingServicesCode, 
        KPhoneUidBarringAllIncomingServices );
    
    // Waiting
    AddL( KPhoneUidWaitingCode, 
        KPhoneUidWaiting );

    // Calling line identification presentation
    AddL( KPhoneUidCallingLineIdentificationPresentationCode, 
        KPhoneUidCallingLineIdentificationPresentation );

    // Calling line identification restriction
    AddL( KPhoneUidCallingLineIdentificationRestrictionCode, 
        KPhoneUidCallingLineIdentificationRestriction );

    // Connected line presentation
    AddL( KPhoneUidConnectedLinePresentationCode, 
        KPhoneUidConnectedLinePresentation );

    // Connected line presentation restriction.
    AddL( KPhoneUidConnectedLinePresentationRestrictionCode, 
        KPhoneUidConnectedLinePresentationRestriction );

    // CNAP.  
    if ( CPhoneParserFeatures::FeatureSupported( KFeatureIdPhoneCnap ) )
        {
        AddL( KPhoneUidCallingNamePresentationCode, 
            KPhoneUidCallingNamePresentation );
        }

    // Password
    AddL( KPhoneUidPasswordCode, 
        KPhoneUidPassword );

    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmSsParser* CPhoneGsmSsParser::NewLC()
    {
    CPhoneGsmSsParser* self = new ( ELeave ) CPhoneGsmSsParser;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::~CPhoneGsmSsParser
// -----------------------------------------------------------------------------
//
CPhoneGsmSsParser::~CPhoneGsmSsParser()
    {
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::ParseL( 
        const TDesC& aString, 
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TBool result = EFalse;
    aResult.ClearL();      
// All operations require SEND.
// If VoIP call no action needed here.
    if ( !aOptions.FindOptionStatus( KPhoneOptionSend ) ||
         aOptions.FindOptionStatus( KPhoneOptionVoipCall ) )
        {
        return EFalse;
        }

    TLex input( aString );
    TPtrC action( 
        FindString( 
            input, 
            KPhoneSsActionCharacters() ) );

    // Check if action is proper for service requests.
    TServiceType type = EServiceActivation;
    
    // This variable will indicate if action was ok.
    TBool ok = ETrue;

    // Fill type.
    if ( action == KPhoneSsActivation )
        {
        type = EServiceActivation;
        }
    else if ( action == KPhoneSsDeactivation )
        {
        type = EServiceDeactivation;
        }
    else if ( action == KPhoneSsInterrogation )
        {
        type = EServiceInterrogation;
        }
    else if ( action == KPhoneSsRegistration )
        {
        type = EServiceRegistration;
        }
    else if ( action == KPhoneSsErasure )
        {
        type = EServiceErasure;
        }
    else 
        {
        // Not ok.
        ok = EFalse;
        }

    // Prefix must be one of the service types.
    if ( ok )
        {
        TPtrC serviceCode( FindString( input, KPhoneSsDigits() ) );
        
        // Check that service code is not too long.
        if ( serviceCode.Length() > KPhoneGsmSsServiceCodeMaxLength )
            {
            return EFalse;
            }

        TKeyArrayFix key( _FOFF( TServiceInfo, iCode ), ECmpNormal );
        TServiceInfo info;
        TInt position;       
        info.iCode = serviceCode;

        // Check if the string can be a service request.
        if ( !iArray->FindIsq( info, key, position ) )
            {
            info = iArray->At( position );
            }
        else
            {
            // Not found.
            return EFalse;
            }

        TServiceSyntax syntax = ESyntaxNotAllowed;

        // Take syntax from info record.
        switch ( type )
            {
            case EServiceActivation:
                syntax = info.iActivation;
                break;

            case EServiceDeactivation:
                syntax = info.iDeactivation;
                break;

            case EServiceInterrogation:
                syntax = info.iInterrogation;
                break;

            case EServiceRegistration:
                syntax = info.iRegistration;
                break;

            case EServiceErasure:
                syntax = info.iErasure;
                break;

            default:
                PhoneGsmParser::Panic( PhoneGsmParser::EShouldNeverHappen );
                break;
            }

        TUint data;
        PackData( data, type, syntax );

        aResult.SetUid( info.iUid );
        aResult.SetAuxInformation( data );
        
        // We may have to backtrack in case HandleParametersL fails.
        TLexMark mark;
        input.Mark( mark );

        // First attempt
        result = HandleParametersL( input, aResult, syntax );

        // If action was activation and result was failure, then try registration.
        if ( !result && type == EServiceActivation )
            {
            // Backtrack.
            input.UnGetToMark( mark );

            // Clear previous results & fill information again.

            type = EServiceRegistration;
            syntax = info.iRegistration;
            PackData( data, type, syntax );

            aResult.ClearL();
            aResult.SetUid( info.iUid );
            aResult.SetAuxInformation( data );

            // Try again. If this fails, then it is really failure.
            result = HandleParametersL( input, aResult, syntax );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::PackData
//
// Combine data to integer.
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsParser::PackData(
        TUint& aResult,
        TServiceType aType,
        TServiceSyntax aSyntax )
    {
    aResult = 
        ( ( ( TUint )aType ) << KPhoneGsmPackShift ) + ( ( TUint )aSyntax );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::UnpackData
// 
// Take data from integer.
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsParser::UnpackData(
        TUint aData,
        TServiceType& aResultType,
        TServiceSyntax& aResultSyntax )
    {
    aResultSyntax = 
        CPhoneGsmSsParser::TServiceSyntax( aData & KPhoneGsmPackMask );
    aResultType = 
        CPhoneGsmSsParser::TServiceType( 
            ( aData >> KPhoneGsmPackShift ) & 
            KPhoneGsmPackMask );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleParametersL
//
// Selects correct function by syntax information.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleParametersL( 
        TLex& aInput, 
        CPhoneGsmParserResult& aResult,
        TServiceSyntax aSyntax )
    {
    TBool result = EFalse;

    switch ( aSyntax )
        {
        case ESyntaxNotAllowed:
            result = 
                HandleNotAllowedL( aInput, aResult );
            break;

        case ESyntaxNoParameters:
            result = 
                HandleNoParametersL( aInput, aResult );
            break;

        case ESyntaxEmptyOrBasicCode:
            result = 
                HandleEmptyOrBasicCodeL( aInput, aResult );
            break;
            
        case ESyntaxNumberOptBasicCodeOptTimer:
            result = 
                HandleNumberOptBasicCodeOptTimerL( 
                    aInput, 
                    aResult );
            break;

        case ESyntaxNumberOptBasicCode:
            result = 
                HandleNumberOptBasicCodeL( 
                    aInput, 
                    aResult );
            break;

        case ESyntaxPasswordOptBasicCode:
            result = 
                HandlePasswordOptBasicCodeL( 
                    aInput, 
                    aResult );
            break;
            
        case ESyntaxOptGroupCodePasswordNewPasswordNewPassword:
            result = 
                HandleOptGroupCodePasswordNewPasswordNewPasswordL( 
                    aInput, 
                    aResult );
            break;

        case ESyntaxEmptyOrBasicCode2:
            result = 
                HandleEmptyOrBasicCode2L( 
                    aInput, 
                    aResult );
            break;

        default:
            break;
        }          

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleNotAllowedL
//
// Just clear the result and return that parsing failed.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleNotAllowedL( 
        TLex& /*aInput*/, 
        CPhoneGsmParserResult& aResult )
    {
    aResult.ClearL();
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleNoParametersL
//
// Handle no more parameters, only hash 
// as the last character.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleNoParametersL( 
        TLex& aInput, 
        CPhoneGsmParserResult& /*aResult*/ )
    {
    // Next character is last character and it is hash character.
    TBool result = EFalse;
    TPtrC ptr = aInput.Remainder();

    if ( ptr.Length() == KPhoneSsRemainderSingle )
        {
        result = ( ptr[0] == KPhoneSsHash );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleEmptyOrBasicCodeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleEmptyOrBasicCodeL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;

    if ( HandleNoParametersL( aInput, aResult ) )
        {
        aResult.AddParameterL( KNullDesC );
        aResult.AddParameterL( KNullDesC );
        result = ETrue;
        }
    else
        {
        result = HandleEmptyParameterL( aInput, aResult );

        if ( result )
            {
            result = HandleBasicCodeL( aInput, aResult );
            result = result && HandleNoParametersL( aInput, aResult );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleNumberOptBasicCodeOptTimerL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleNumberOptBasicCodeOptTimerL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    
    if ( HandleNumberL( aInput, aResult ) )
        {
        if ( HandleNoParametersL( aInput, aResult ) )
            {
            // Add basic code and timer.
            aResult.AddParameterL( KNullDesC ); 
            aResult.AddParameterL( KNullDesC ); 
            result = ETrue;
            }
        else 
            {
            if ( HandleBasicCodeL( aInput, aResult ) )
                {
                if ( HandleTimerL( aInput, aResult ) )
                    {
                    result = HandleNoParametersL( aInput, aResult );
                    }
                else
                    {
                    result = HandleNoParametersL( aInput, aResult );
                    aResult.AddParameterL( KNullDesC );
                    }
                }
            else if ( HandleEmptyParameterL( aInput, aResult ) )
                {
                result = HandleTimerL( aInput, aResult );
                result = result && 
                         HandleNoParametersL( aInput, aResult );
                }
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleNumberOptBasicCodeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleNumberOptBasicCodeL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    
    if ( HandleNumberL( aInput, aResult ) )
        {
        if ( HandleNoParametersL( aInput, aResult ) )
            {
            aResult.AddParameterL( KNullDesC );
            result = ETrue;
            }
        else
            {
            result = HandleBasicCodeL( aInput, aResult );
            result = result && HandleNoParametersL( aInput, aResult );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandlePasswordOptBasicCodeL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandlePasswordOptBasicCodeL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;

    if ( HandlePasswordL( aInput, aResult ) )
        {
        if ( HandleNoParametersL( aInput, aResult ) )
            {
            aResult.AddParameterL( KNullDesC );
            result = ETrue;
            }
        else
            {
            result = HandleBasicCodeL( aInput, aResult );
            result = result && HandleNoParametersL( aInput, aResult );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::
// HandleOptGroupCodePasswordNewPasswordNewPasswordL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::
    HandleOptGroupCodePasswordNewPasswordNewPasswordL(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = HandleGroupCodeL( aInput, aResult );

    if ( !result )
        {
        result = HandleEmptyParameterL( aInput, aResult );
        }

    result = result && HandlePasswordL( aInput, aResult );
    result = result && HandlePasswordL( aInput, aResult );
    result = result && HandlePasswordL( aInput, aResult );
    result = result && HandleNoParametersL( aInput, aResult );

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleEmptyOrBasicCode2L
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleEmptyOrBasicCode2L(
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = HandleNoParametersL( aInput, aResult );

    if ( result )
        {
        aResult.AddParameterL( KNullDesC );
        }
    else
        {
        result = HandleBasicCodeL( aInput, aResult );
        result = result && HandleNoParametersL( aInput, aResult );
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleEmptyParameterL
//
// Handle empty parameter, two consecutive asterisks.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleEmptyParameterL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    aInput.Mark();

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();

        result = ( aInput.Peek() == KPhoneSsAsterisk );
        
        if ( result )
            {
            aResult.AddParameterL( KNullDesC );
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleBasicCodeL
//
// Basically, basic service code is a two digit string.
// In this method, only explicit
// values are handled (parameter might be empty).
//
// The method checks that the first character is * and then
// it is followed by two digits. And there must not be more
// than two digits.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleBasicCodeL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {   
    TBool result = EFalse;
    TLexMark start;

    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();
    
        TLexMark mark;
        aInput.Mark( mark );
        
        if ( aInput.Peek().IsDigit() )
            {
            aInput.Inc();

            if ( aInput.Peek().IsDigit() )
                {
                aInput.Inc();

                if ( !aInput.Peek().IsDigit() )
                    {
                    TPtrC code = aInput.MarkedToken( mark );

                    result = CheckBasicCode( code );

                    if ( result )
                        {
                        aResult.AddParameterL( code );
                        }
                    }
                }
            }

        // Check empty parameters.
        if ( !result )
            {
            aInput.UnGetToMark( mark );
            
            if ( aInput.Remainder() == KPhoneSsEnd )
                {
                aResult.AddParameterL( KNullDesC );
                result = ETrue;
                }
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleNumberL
//
// Directory number as a parameter must be present. It can
// contain at most 38 digits. Digits and '+' characters
// are allowed, but '+' character is allowed to be only in the 
// beginning of the string.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleNumberL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    if ( aInput.Remainder().Length() < KPhoneSsRemainderMinLength )
        {
        return EFalse;
        }

    TInt maxLength = KPhoneSsDirectoryNumberMaxLength;
    TBool result = EFalse;
    TLexMark start;

    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();
       
        TLexMark mark;
        aInput.Mark( mark );

        if ( aInput.Peek() == KPhoneSsPlus )
            {
            aInput.Inc();
            maxLength++;
            }

        while ( aInput.Peek().IsDigit() )
            {
            aInput.Inc();
            }

        TPtrC number = aInput.MarkedToken( mark );
        if ( number.Length() <= maxLength )
            {  
            aResult.AddParameterL( number );
            result = ETrue;
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleTimerL
//
// The method checks if the next part is 'no reply timer'.
// It is at most two digits. Value is expected to be present.
// Value is checked in CheckTimer method.
//
// (if value is not present, then it is handled in calling
//  method)
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleTimerL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    TBool result = EFalse;
    TLexMark start;
    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();

        TLexMark mark;
        aInput.Mark( mark );
        
        while ( aInput.Peek().IsDigit() )
            {
            aInput.Inc();
            }
        
        TPtrC timer( aInput.MarkedToken( mark ) );
        
        if ( CheckTimer( timer ) )
            {
            aResult.AddParameterL( timer );
            result = ETrue;
            }

        // Check empty parameters.
        if ( !result )
            {
            aInput.UnGetToMark( mark );
            
            if ( aInput.Remainder() == KPhoneSsEnd )
                {
                aResult.AddParameterL( KNullDesC );
                result = ETrue;
                }
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandlePasswordL
//
// Password is four digit string.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandlePasswordL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    if ( aInput.Remainder().Length() <
         KPhoneSsRemainderPasswordMinLength )
        {
        return EFalse;
        }

    TBool result = EFalse;
    TLexMark start;
    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();

        TLexMark mark;
        aInput.Mark( mark );

        while( aInput.Peek().IsDigit() )
            {
            aInput.Inc();
            }

        TPtrC password( aInput.MarkedToken( mark ) );
        
        if ( password.Length() == KPhoneSsPasswordLength )
            {
            aResult.AddParameterL( password );
            result = ETrue;
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::HandleGroupCodeL
//
// The method handles password group codes. However, code 
// must be one to four digits. 
//
// (if group code is null, then it is handled in calling
//  method)
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::HandleGroupCodeL( 
        TLex& aInput,
        CPhoneGsmParserResult& aResult )
    {
    if ( aInput.Remainder().Length() <= KPhoneSsRemainderMinLength )
        {
        return EFalse;
        }

    TBool result = EFalse; 
    TLexMark start;

    aInput.Mark( start );

    if ( aInput.Peek() == KPhoneSsAsterisk )
        {
        aInput.Inc();
        
        TLexMark mark;
        aInput.Mark( mark );

        while ( aInput.Peek().IsDigit() )
            {
            aInput.Inc();
            }

        TPtrC groupCode( aInput.MarkedToken( mark ) );

        if ( CheckGroupCode( groupCode ) )
            {
            aResult.AddParameterL( groupCode );
            result = ETrue;
            }
        }

    if ( !result )
        {
        aInput.UnGetToMark( start );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::CheckBasicCode
//
// Check that basic code is appropriate
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::CheckBasicCode( const TDesC& aCode )
    {
    TLex lex( aCode );
    TUint value;
    
    if ( aCode.Length() <= KPhoneSsBasicCodeMaxLength )
        {
        if ( lex.Val( value, EDecimal ) != KErrNone )
            {
            return EFalse;
            }
        }
    else
        {
        return EFalse;
        }

    TBool result = EFalse;
  
    switch ( value )
        {
        // TELESERVICES
        case EAllTele:          // GSM: All tele and bearer services
        case ETelephony:        // GSM: Teleservices
        case EAllDataTele:      // GSM: All data teleservices
        case EFax:              // GSM: Facsimile services
        case ESms:              // GSM: Short message services
        case EAllTeleExcSms:    // GSM: All teleservices except SMS
        case EAltTele:          // PCN: Alternate line service

        // BEARER SERVICES
        case EAllBearer:        // GSM: All bearer services
        case EAllAsync:         // GSM: All async services
        case EAllSync:          // GSM: All sync services
        case ESyncData:         // GSM: All data circuit sync
        case EAsyncData:        // GSM: All data circuit async
        case EPacketData:       // GSM: All dedicated packet access
        case EPadAccess:        // GSM: All dedicated PAD access

            result = ETrue;
            break;

        // PLMN specific teleservices and bearer services
        default:
            
            // GSM: PLMN specific teleservices
            if ( value >= EAllPlmnTele && value <= EPlmnTeleF )
                {
                result = ETrue;
                }
            // GSM: PLMN specific bearer services
            else if ( value >= EAllPlmnBearer && value <= EPlmnBearerServF )
                {
                result = ETrue;
                }

            // Otherwise, result == EFalse and it's OK.
            break;
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::CheckTimer
//
// Check if timer value is ok.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::CheckTimer( const TDesC& aCode )
    {
    TLex lex( aCode );
    TUint value;
    
    if ( aCode.Length() <= KPhoneSsTimerMaxLength )
        {
        if ( lex.Val( value, EDecimal ) != KErrNone )
            {
            return EFalse;
            }
        }
    else 
        {
        return EFalse;
        }

    TBool result = EFalse;

    switch ( value )
        {
        case 5:
        case 10:
        case 15:
        case 20:
        case 25:
        case 30:
            result = ETrue;
            break;

        default:
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::CheckGroupCode
//
// Check if group code is ok.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmSsParser::CheckGroupCode( const TDesC& aCode )
    {
    TLex lex( aCode );
    TUint value;
    TBool result = EFalse;

    if ( aCode.Length() <= KPhoneSsGroupCodeMaxLength )
        {
        if ( lex.Val( value, EDecimal ) == KErrNone )
            {
            result = ( value == KPhoneSsOnlyAllowedGroupCode );
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::FindString
// -----------------------------------------------------------------------------
//
TPtrC CPhoneGsmSsParser::FindString( 
        TLex& aInput,
        const TDesC& aAllowedCharacters )
    {
    TLexMark mark;
    aInput.Mark( mark );

    // Return string containing characters that appear in 
    // aAllowedCharacters.
    while ( aAllowedCharacters.Locate( aInput.Peek() ) != 
            KErrNotFound )
        {
        aInput.Inc();
        }
    
    return aInput.MarkedToken( mark );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::AddL
//
// Parse service description and add it.
//
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsParser::AddL( 
        const TDesC& aService, 
        TUint aUid )
    {
    // There must be six positive integers in aService.   
    TLex input( aService );

    // First number is service code.
    TPtrC serviceCode( NextNumber( input ) );

    // And then next five are syntax descriptions.
    TServiceSyntax activation = 
        ConvertToServiceSyntax( NextNumberAsValue( input ) );
    TServiceSyntax deactivation = 
        ConvertToServiceSyntax( NextNumberAsValue( input ) );
    TServiceSyntax interrogation = 
        ConvertToServiceSyntax( NextNumberAsValue( input ) );
    TServiceSyntax registration = 
        ConvertToServiceSyntax( NextNumberAsValue( input ) );
    TServiceSyntax erasure = 
        ConvertToServiceSyntax( NextNumberAsValue( input ) );
   
    // Add to array.
    AddServiceL( 
        serviceCode,
        activation,
        deactivation,
        interrogation,
        registration,
        erasure,
        aUid );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::AddServiceL
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsParser::AddServiceL( 
        const TDesC& aServiceCode,
        TServiceSyntax aActivation,
        TServiceSyntax aDeactivation,
        TServiceSyntax aInterrogation,
        TServiceSyntax aRegistration,
        TServiceSyntax aErasure,
        TUint aUid )
    {   
    TServiceInfo info;

    // Fill information to info structure.
    info.iCode = aServiceCode;
    info.iActivation = aActivation;
    info.iDeactivation = aDeactivation;
    info.iInterrogation = aInterrogation;
    info.iRegistration = aRegistration;
    info.iErasure = aErasure;
    info.iUid = aUid;

    TKeyArrayFix key( _FOFF( TServiceInfo, iCode ), ECmpNormal );

    // Add it to the array.
    TRAPD( err, iArray->InsertIsqL( info, key ) ); 

    if ( err != KErrNone && err != KErrNoMemory )
        {
        PhoneGsmParser::Panic( 
            PhoneGsmParser::EIncorrectServiceDescription );
        }

    // Just leave if no memory.
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::NextNumber
//
// Skip spaces and then get next number.
// -----------------------------------------------------------------------------
//
TPtrC CPhoneGsmSsParser::NextNumber( 
        TLex& aLex )
    {
    // Skip whites.
    aLex.SkipSpace();

    // Go over digits.
    aLex.Mark();
    while ( aLex.Peek().IsDigit() )
        {
        aLex.Inc();
        }

    // Check if we got proper number.
    if ( !aLex.TokenLength() )
        {
        PhoneGsmParser::Panic( 
            PhoneGsmParser::EIncorrectServiceDescription );
        }
    
    return aLex.MarkedToken();       
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::NextNumberAsValue
//
// Get number and convert it to value.
// -----------------------------------------------------------------------------
//
TUint CPhoneGsmSsParser::NextNumberAsValue( 
        TLex& aLex )
    {
    // Get number.
    TPtrC number( NextNumber( aLex ) );
    TLex input( number );
    TUint value;

    // Parse number.
    if ( input.Val( value, EDecimal ) != KErrNone )
        {
        PhoneGsmParser::Panic( 
            PhoneGsmParser::EIncorrectServiceDescription );
        }

    return value;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsParser::ConvertToServiceSyntax
//
// Convert value to a service syntax enumeration.
// -----------------------------------------------------------------------------
//
CPhoneGsmSsParser::TServiceSyntax 
    CPhoneGsmSsParser::ConvertToServiceSyntax( 
        TUint aService )
    {
    TServiceSyntax service = ESyntaxNotAllowed;

    switch ( aService )
        {
        case 0:
            service = ESyntaxNotAllowed;        
            break;
        case 1:
            service = ESyntaxNoParameters;
            break;
        case 2:
            service = ESyntaxEmptyOrBasicCode;
            break;
        case 3:
            service = ESyntaxNumberOptBasicCodeOptTimer;
            break;
        case 4:
            service = ESyntaxNumberOptBasicCode;
            break;
        case 5:
            service = ESyntaxPasswordOptBasicCode;
            break;
        case 6:
            service = ESyntaxOptGroupCodePasswordNewPasswordNewPassword;
            break;
        case 7:
            service = ESyntaxEmptyOrBasicCode2;
            break;
        default:
            PhoneGsmParser::Panic( 
                PhoneGsmParser::EIncorrectServiceDescription );       
        }

    return service;
    }

//  End of File  
