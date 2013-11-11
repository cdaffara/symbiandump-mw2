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
* Description:  It is base handler for voip number.
*
*/


#ifndef CPHONEVOIPNUMBERHANDLER_H
#define CPHONEVOIPNUMBERHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is base handler for phone number.
*
* @since S60 5.0
* @lib phoneparser.lib
*/
class CPhoneVoipNumberHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions
    
        /**
        * Process dial to voip number
        * 
        * @param aNumber main number part.
        * @param aDtmfPostfix DTMF postfix of the number
        */
        virtual void ProcessDialToVoipNumberL( 
            const TDesC& aNumber,
            const TDesC& aDtmfPostfix
            ) = 0;

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, processes result.
        *
        * @param aResult It is to be processed.
        */
        IMPORT_C virtual void ProcessL( CPhoneGsmParserResult& aResult );
    };

#endif      // CPHONEVOIPNUMBERHANDLER_H
            
// End of File
