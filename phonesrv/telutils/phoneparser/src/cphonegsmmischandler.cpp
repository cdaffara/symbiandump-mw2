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
* Description:  Implementation of handler for USSD.
*
*/


// INCLUDE FILES
#include    "cphonegsmmischandler.h" 
#include    "cphonegsmparserbase.h" 
#include    "cphonegsmparserresult.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmMiscHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmMiscHandler::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {
    TUint uid = aResult.Uid();

    switch ( uid )
        {
        case KPhoneUidUnstructuredService:
            if ( aResult.CountParameters() )
                {
                // First parameter is ussd string.
                ProcessUssdL( aResult.ParameterAt( 0 ) );
                }
            break;

        default:
            break;
        }
    }   

//  End of File  
