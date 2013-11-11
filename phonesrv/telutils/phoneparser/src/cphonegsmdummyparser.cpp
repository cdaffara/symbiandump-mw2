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
* Description:  Implementation of dummy parser.
*
*/


// INCLUDE FILES
#include    "cphonegsmdummyparser.h" 
#include    "cphonegsmparserresult.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmDummyParser::CPhoneGsmDummyParser
// -----------------------------------------------------------------------------
//
CPhoneGsmDummyParser::CPhoneGsmDummyParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmDummyParser::ParseL
// -----------------------------------------------------------------------------
//
CPhoneGsmDummyParser* CPhoneGsmDummyParser::NewL()
    {
    CPhoneGsmDummyParser* self = new (ELeave) CPhoneGsmDummyParser;
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmDummyParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmDummyParser::ParseL( 
        const TDesC& /*aString*/,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& /*aOptions*/ )
    {
    aResult.ClearL();
    return EFalse;
    }      

//  End of File  
