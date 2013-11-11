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
* Description:  Principal handler for supplementary service procedures.
*
*/


// INCLUDE FILES
#include    <bldvariant.hrh>

#include    "cphonegsmsshandler.h" 
#include    "cphonegsmssparser.h" 
#include    "cphonegsmparserresult.h" 

// LOCAL FUNCTION PROTOTYPES

/**
* Converts from service type to action.
*/
CPhoneGsmSsHandler::TAction ConvertFromServiceType( 
    CPhoneGsmSsParser::TServiceType aType );

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ConvertFromServiceType
// -----------------------------------------------------------------------------
//
CPhoneGsmSsHandler::TAction ConvertFromServiceType( 
        CPhoneGsmSsParser::TServiceType aType )
    {
    CPhoneGsmSsHandler::TAction result = 
        CPhoneGsmSsHandler::EActivate;

    switch ( aType )
        {
        case CPhoneGsmSsParser::EServiceActivation:
            result = CPhoneGsmSsHandler::EActivate;
            break;
        case CPhoneGsmSsParser::EServiceDeactivation:
            result = CPhoneGsmSsHandler::EDeactivate;
            break;
        case CPhoneGsmSsParser::EServiceInterrogation:
            result = CPhoneGsmSsHandler::EInterrogate;
            break;
        case CPhoneGsmSsParser::EServiceRegistration:
            result = CPhoneGsmSsHandler::ERegister;
            break;
        case CPhoneGsmSsParser::EServiceErasure:
            result = CPhoneGsmSsHandler::EErase;
            break;

        default:
            break;
        }

    return result;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSsHandler::ProcessL
//
// Process result and call appropriate function.
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmSsHandler::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {            
    switch( aResult.Uid() )
        {
        case KPhoneUidForwardingAll:
            DoProcessForwardingL( 
                EForwardingAll,
                aResult );
            break;

        case KPhoneUidForwardingAllConditional:
            DoProcessForwardingL(
                EForwardingAllConditional,
                aResult );
            break;

        case KPhoneUidForwardingUnconditional:
            DoProcessForwardingL(
                EForwardingUnconditional,
                aResult );
            break;

        case KPhoneUidForwardingNoReply:
            DoProcessForwardingL(
                EForwardingNoReply,
                aResult );
            break;

        case KPhoneUidForwardingNotReachable:
            DoProcessForwardingL(
                EForwardingNotReachable,
                aResult );
            break;

        case KPhoneUidForwardingBusy:
            DoProcessForwardingL( 
                EForwardingBusy,
                aResult );
            break;

        case KPhoneUidBarringAllOutgoingCalls:
            DoProcessBarringL( 
                EBarringAllOutgoingCalls,
                aResult );
            break;

        case KPhoneUidBarringAllIncomingCalls:
            DoProcessBarringL( 
                EBarringAllIncomingCalls,
                aResult );
            break;

        case KPhoneUidBarringAllServices:
            DoProcessBarringL( 
                EBarringAllServices,
                aResult );
            break;

        case KPhoneUidBarringAllOutgoingInternationalCalls:
            DoProcessBarringL( 
                EBarringAllOutgoingInternationalCalls,
                aResult );

            break;

        case KPhoneUidBarringAllOutgoingInternationalCallsExceptHome:
            DoProcessBarringL( 
                EBarringAllOutgoingInternationalCallsExceptHome,
                aResult );
            break;

        case KPhoneUidBarringAllOutgoingServices:
            DoProcessBarringL( 
                EBarringAllOutgoingServices,
                aResult );
            break;

        case KPhoneUidBarringAllIncomingCallsWhenRoaming:
            DoProcessBarringL( 
                EBarringAllIncomingCallsWhenRoaming,
                aResult );
            break;

        case KPhoneUidBarringAllIncomingServices:
            DoProcessBarringL( 
                EBarringAllIncomingServices,
                aResult );
            break;

        case KPhoneUidWaiting:
            {
            TPtrC ptr( KNullDesC );

            if ( aResult.CountParameters() )
                {
                ptr.Set( aResult.ParameterAt( 0 ) );
                }

            TUint data = aResult.AuxInformation();
            CPhoneGsmSsParser::TServiceType type;
            CPhoneGsmSsParser::TServiceSyntax syntax;

            CPhoneGsmSsParser::UnpackData( data, type, syntax );

            TAction action = ConvertFromServiceType( type ); 
            ProcessWaitingL( action, ptr );
            }
            break;

        case KPhoneUidCallingLineIdentificationPresentation:
            ProcessClipStatusL();
            break;

        case KPhoneUidCallingLineIdentificationRestriction:
            ProcessClirStatusL();
            break;

        case KPhoneUidConnectedLinePresentation:
            ProcessColpStatusL();
            break;

        case KPhoneUidConnectedLinePresentationRestriction:
            ProcessColrStatusL();
            break;

        case KPhoneUidPassword:
            {
            TPasswordProcedure procedure = EPasswordNetwork;
            if ( aResult.ParameterAt( 0 ).Length() )
                {
                procedure = EPasswordBarring;
                }

            ProcessPasswordChangeL( 
                procedure,
                aResult.ParameterAt(1),
                aResult.ParameterAt(2),
                aResult.ParameterAt(3) );
            }
            break;

        case KPhoneUidCallingNamePresentation:
            ProcessCnapStatusL();
            break;

        default:
            break;
        }

    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsHandler::DoProcessBarringL
//
// Process barring procedures.
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsHandler::DoProcessBarringL(
        TBarringProcedure aBarring,
        CPhoneGsmParserResult& aResult )
    {
    CPhoneGsmSsParser::TServiceType type;
    CPhoneGsmSsParser::TServiceSyntax syntax;
    TPtrC password( KNullDesC );
    TPtrC basicCode( KNullDesC );

    CPhoneGsmSsParser::UnpackData( 
        aResult.AuxInformation(),
        type,
        syntax );

    TAction action = ConvertFromServiceType( type ); 

    switch ( type )
        {
        case CPhoneGsmSsParser::EServiceDeactivation:
        case CPhoneGsmSsParser::EServiceActivation:
            
            // First parameter is password.
            if ( aResult.CountParameters() >= 1 )
                {
                password.Set( aResult.ParameterAt( 0 ) );
                }

            // Second parameter is basic service code.
            if ( aResult.CountParameters() >= 2 )
                {
                basicCode.Set( aResult.ParameterAt( 1 ) );
                }

            ProcessBarringL( 
                action,
                aBarring,
                password,
                basicCode );

            break;
    
        case CPhoneGsmSsParser::EServiceInterrogation:

            // First parameter is basic service code.
            if ( aResult.CountParameters() >= 1 )
                {
                basicCode.Set( aResult.ParameterAt( 0 ) );
                }

            ProcessBarringL(
                action,
                aBarring,
                KNullDesC,
                basicCode );

            break;

        case CPhoneGsmSsParser::EServiceRegistration:
        case CPhoneGsmSsParser::EServiceErasure:
            // These can never happen.
            break;

        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSsHandler::DoProcessForwardingL
//
// Process result and call appropriate function.
// -----------------------------------------------------------------------------
//
void CPhoneGsmSsHandler::DoProcessForwardingL(
        TForwardingProcedure aForwarding,
        CPhoneGsmParserResult& aResult )
    {
    CPhoneGsmSsParser::TServiceType type;
    CPhoneGsmSsParser::TServiceSyntax syntax;

    CPhoneGsmSsParser::UnpackData( 
        aResult.AuxInformation(),
        type,
        syntax );

    TAction action = ConvertFromServiceType( type ); 

    TPtrC number( KNullDesC );
    TPtrC basicCode( KNullDesC );
    TPtrC timer( KNullDesC );

    switch ( type )
        {
        case CPhoneGsmSsParser::EServiceActivation:
        case CPhoneGsmSsParser::EServiceDeactivation:
        case CPhoneGsmSsParser::EServiceInterrogation:
        case CPhoneGsmSsParser::EServiceErasure:
            basicCode.Set( aResult.ParameterAt( 1 ) );
            break;

        case CPhoneGsmSsParser::EServiceRegistration:
            {
            number.Set( aResult.ParameterAt( 0 ) );
            
            if ( aResult.CountParameters() > 1 )
                {
                basicCode.Set( aResult.ParameterAt( 1 ) );
                }

            if ( aResult.CountParameters() > 2 )
                {
                timer.Set( aResult.ParameterAt( 2 ) );
                }
            }
            break;

        default:
            break;
        }

    ProcessForwardingL(
        action,
        aForwarding,
        number,
        basicCode,
        timer );
            
    }
    
//  End of File  
