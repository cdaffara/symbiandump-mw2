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
* Description:  It is handler for SIM control procedures.
*
*/


#ifndef CPHONEGSMSIMCONTROLHANDLER_H
#define CPHONEGSMSIMCONTROLHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is handler for sim control procedures.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmSimControlHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions
        
        /**
        * It defines different types of codes.
        */
        enum TPinCode 
            {
            EPinCode,
            EPin2Code
            };

        /**
        * Change pin code
        *
        * @param aType It is type of pin.
        * @param aOldPin It is old pin.
        * @param aNewPin It is new pin.
        * @param aNewPin2 It is new pin.
        */
        virtual void ProcessChangePinL( 
            TPinCode aType, 
            const TDesC& aOldPin,
            const TDesC& aNewPin,
            const TDesC& aNewPin2 ) = 0;

        /**
        * Unblock pin code.
        *
        * @param aType It is type of pin to unblock.
        * @param aPuk It is unblocking code.
        * @param aNewPin It is new pin.
        * @param aNewPin2 It is new pin.
        */
        virtual void ProcessUnblockPinL(
            TPinCode aType,
            const TDesC& aPuk,
            const TDesC& aNewPin,
            const TDesC& aNewPin2 ) = 0;
    
    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, process the results of parsing.
        *
        * @param aResult It is results to be processed.
        */
        IMPORT_C virtual void ProcessL( 
            CPhoneGsmParserResult& aResult );

    private:

        /**
        * Process pin change.
        */
        void ProcessPinChangeL(
            TPinCode aType,
            CPhoneGsmParserResult& aResult );

        /**
        * Process pin unblock.
        */
        void ProcessPinUnblockL(
            TPinCode aType,
            CPhoneGsmParserResult& aResult );
    };

#endif      // CPHONEGSMSIMCONTROLHANDLER_H
            
// End of File
