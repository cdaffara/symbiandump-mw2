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
* Description:  Handler for phone numbers.
*
*/


// INCLUDE FILES
#include    "cphonegsmphonenumberhandler.h" 
#include    "cphonegsmphonenumberparser.h" 
#include    "cphonegsmparserresult.h" 

// CONSTANTS
const TInt KPhonePhoneNumberDialParam = 0;
const TInt KPhonePhoneNumberPostfixParam = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmPhoneNumberHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmPhoneNumberHandler::ProcessL( 
            CPhoneGsmParserResult& aResult )
    {
    if ( aResult.Uid() == KPhoneUidPhoneNumber )
        {
        TPtrC number( aResult.ParameterAt( KPhonePhoneNumberDialParam ) );
        TPtrC postfix( aResult.ParameterAt( KPhonePhoneNumberPostfixParam ) );
        TClir clirover = EClirDefault;
        
        if ( aResult.AuxInformation() == KPhoneDialSuppressClir )
            {
            clirover = EClirSuppress;
            }
        else if ( aResult.AuxInformation() == KPhoneDialInvokeClir )
            {
            clirover = EClirInvoke;
            } 

            ProcessDialToNumberL( number, postfix, clirover );
        }
        
    }   

//  End of File  
