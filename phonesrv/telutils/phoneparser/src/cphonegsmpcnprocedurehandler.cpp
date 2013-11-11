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
* Description:  Handler for PCN procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmpcnprocedurehandler.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmpcnprocedureparser.h" 

// CONSTANTS

// Position of password in SIM codes.
const TInt KPhoneGsmPcnSimPasswordParamAt = 0;

// Position of type in SIM codes.
const TInt KPhoneGsmPcnSimTypeParamAt = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmPcnProcedureHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmPcnProcedureHandler::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {
    switch ( aResult.Uid() )
        {
        case KPhoneUidSimLockCode:
            ProcessSimLockL( 
                aResult.ParameterAt( KPhoneGsmPcnSimPasswordParamAt ),
                aResult.ParameterAt( KPhoneGsmPcnSimTypeParamAt ) );
            break;

        case KPhoneUidSimUnlockCode:
            ProcessSimUnlockL(
                aResult.ParameterAt( KPhoneGsmPcnSimPasswordParamAt ),
                aResult.ParameterAt( KPhoneGsmPcnSimTypeParamAt ) );
            break;

        default:
            break;
        }
    }

//  End of File  
