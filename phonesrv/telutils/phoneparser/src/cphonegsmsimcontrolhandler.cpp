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
* Description:  Handler for SIM control procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmsimcontrolhandler.h" 
#include    "cphonegsmsimcontrolparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "phonegsmparser.h" 

// CONSTANTS
const TInt KPhoneSimControlOldPinPukParam = 0;
const TInt KPhoneSimControlNewPinParam = 1;
const TInt KPhoneSimControlNewPinVerifyParam = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmSimControlHandler::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {
    switch( aResult.Uid() )
        {
        case KPhoneUidPinChange:
            ProcessPinChangeL( EPinCode, aResult );
            break;

        case KPhoneUidPin2Change:
            ProcessPinChangeL( EPin2Code, aResult );
            break;

        case KPhoneUidPinUnblock:
            ProcessPinUnblockL( EPinCode, aResult );
            break;

        case KPhoneUidPin2Unblock:
            ProcessPinUnblockL( EPin2Code, aResult );
            break;

        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlHandler::ProcessPinChangeL
// -----------------------------------------------------------------------------
//
void CPhoneGsmSimControlHandler::ProcessPinChangeL( 
        TPinCode aType,
        CPhoneGsmParserResult& aResult )
    {
    ProcessChangePinL( 
        aType,
        aResult.ParameterAt( KPhoneSimControlOldPinPukParam ),
        aResult.ParameterAt( KPhoneSimControlNewPinParam ),
        aResult.ParameterAt( KPhoneSimControlNewPinVerifyParam ) );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmSimControlHandler::ProcessPinUnblockL
// -----------------------------------------------------------------------------
//
void CPhoneGsmSimControlHandler::ProcessPinUnblockL(
        TPinCode aType,
        CPhoneGsmParserResult& aResult )
    {
    ProcessUnblockPinL( 
        aType,
        aResult.ParameterAt( KPhoneSimControlOldPinPukParam ),
        aResult.ParameterAt( KPhoneSimControlNewPinParam ),
        aResult.ParameterAt( KPhoneSimControlNewPinVerifyParam ) );
    }

//  End of File  
