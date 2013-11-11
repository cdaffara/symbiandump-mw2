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
* Description:  It is base handler for phone number.
*
*/


#ifndef CPHONEGSMPHONENUMBERHANDLER_H
#define CPHONEGSMPHONENUMBERHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is base handler for phone number.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmPhoneNumberHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions

        /**
        * Clir overriding options
        *
        * EClirDefault      - default clir
        * EClirInvoke       - explicit invoke
        * EClirSuppress     - explicit suppress
        */
        enum TClir
            {
            EClirDefault,
            EClirInvoke,
            EClirSuppress 
            };
        
        /**
        * Process dial to number
        *
        * @param aNumber main number part.
        * @param aDtmfPostfix dtmf postfix.
        * @param aClir clir part.
        */
        virtual void ProcessDialToNumberL( 
            const TDesC& aNumber,
            const TDesC& aDtmfPostfix,
            TClir aClir ) = 0;

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, processes result.
        *
        * @param aResult It is to be processed.
        */
        IMPORT_C virtual void ProcessL( CPhoneGsmParserResult& aResult );
    };

#endif      // CPHONEGSMPHONENUMBERHANDLER_H
            
// End of File
