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
* Description:  It is base class for handlers of ussd & show imei.
*
*/


#ifndef CPHONEGSMMISCHANDLER_H
#define CPHONEGSMMISCHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is base class for handlers of ussd & show imei.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmMiscHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions
        
        /**
        * Process USSD request.
        *
        * @param aString It is string to be sent.
        */
        virtual void ProcessUssdL( const TDesC& aString ) = 0;
    
    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, process the results of parsing.
        *
        * @param aResult It is results to be processed.
        */
        IMPORT_C virtual void ProcessL( 
            CPhoneGsmParserResult& aResult );
            
    };

#endif      // CPHONEGSMMISCHANDLER_H
            
// End of File
