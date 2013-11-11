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
* Description:  Handler for call concerned SS procedures.
*
*/


// INCLUDE FILES
#include    "cphonegsmsscallhandler.h" 
#include    "cphonegsmsscallparser.h" 
#include    "cphonegsmparserresult.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmSsCallHandler::ProcessL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmSsCallHandler::ProcessL( 
            CPhoneGsmParserResult& aResult )
    {
    switch ( aResult.Uid() )
        {
        case KPhoneUidCommand0:
            ProcessEndOrRejectL();
            break;

        case KPhoneUidCommand1:
            {
            TInt value = aResult.AuxInformation();

            if ( !value )
                {
                ProcessEndAndAcceptL();
                }
            else
                {
                value--;
                ProcessEndCallL( value );
                }
            }
            break;

        case KPhoneUidCommand2:
            {
            TInt value = aResult.AuxInformation();

            if ( !value )
                {
                ProcessSwapL();
                }
            else
                {
                value--;
                ProcessHoldAllCallsExceptL( value );
                }
            }
            break;

        case KPhoneUidCommand3:
            ProcessJoinL();
            break;

        case KPhoneUidCommand4:
            ProcessExplicitCallTransferL();
            break;

        default:
            break;
        }
    }   

//  End of File  
