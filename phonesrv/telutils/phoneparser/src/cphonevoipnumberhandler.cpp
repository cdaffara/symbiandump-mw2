/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handler for voip numbers.
*
*/


// INCLUDE FILES
#include    "cphonevoipnumberhandler.h" 
#include    "cphonevoipnumberparser.h" 
#include    "cphonegsmparserresult.h" 

// CONSTANTS
const TInt KPhoneVoipNumberDialParam = 0;
const TInt KPhoneVoipNumberPostfixParam = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneVoipNumberHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneVoipNumberHandler::ProcessL( 
            CPhoneGsmParserResult& aResult )
    {
    if ( aResult.Uid() == KPhoneUidVoipNumber )
        {
        TPtrC number( aResult.ParameterAt( KPhoneVoipNumberDialParam ) );
        TPtrC postfix( aResult.ParameterAt( KPhoneVoipNumberPostfixParam ) );
        ProcessDialToVoipNumberL( number, postfix );
        }
    }   

//  End of File  
