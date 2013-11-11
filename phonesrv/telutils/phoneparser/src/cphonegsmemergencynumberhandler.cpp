/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handler for emergency numbers.
*
*/


// INCLUDE FILES
#include    "cphonegsmemergencynumberhandler.h" 
#include    "cphonegsmemergencynumberparser.h" 
#include    "cphonegsmparserresult.h" 

// CONSTANTS
const TInt KPhoneEmergencyNumberDialParam = 0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmEmergencyNumberHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmEmergencyNumberHandler::ProcessL( 
            CPhoneGsmParserResult& aResult )
    {
    if ( aResult.Uid() == KPhoneUidEmergencyNumber )
        {
        TPtrC number( aResult.ParameterAt( KPhoneEmergencyNumberDialParam ) );
        ProcessDialToEmergencyNumberL( number );
        }
    }   

//  End of File  
