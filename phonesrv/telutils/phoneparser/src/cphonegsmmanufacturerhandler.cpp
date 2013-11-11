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
* Description:  Implementation of manufacturer handler.
*
*/


// INCLUDE FILES
#include    "cphonegsmmanufacturerhandler.h" 
#include    "cphonegsmmanufacturerparser.h" 
#include    "cphonegsmparserresult.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmManufacturerHandler::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {
    if ( aResult.Uid() == KPhoneUidManufacturerCode )
        {
        ProcessCommandL( aResult.AuxInformation() );
        }
#ifdef PHONEPARSER_PARSE_DEBUG_CODE
    else if ( aResult.Uid() == KPhoneUidManufacturerDebugCode )
        {
        ProcessDebugL( aResult.ParameterAt( 0 ) ); // The first parameter is code.
        }
#endif
    }
    
//  End of File  
