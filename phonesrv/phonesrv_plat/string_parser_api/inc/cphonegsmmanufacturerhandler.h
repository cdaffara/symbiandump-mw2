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
* Description:  It is base handler for manufacturer specific codes.
*
*/


#ifndef CPHONEGSMMANUFACTURERHANDLER_H
#define CPHONEGSMMANUFACTURERHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is base handler for manufacturer specific codes.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmManufacturerHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions
        
        /**
        * It enumerates all commands.
        *
        * EShowVersion - SW version display
        * EBadPinChange - bad PIN change request, e.g. **04*#
        * EBadPin2Change - bad PIN2 change request
        * EBadPinUnblock - bad PIN unblock request, e.g. **05*#
        * EBadPin2Unblock - bad PIN2 unblock request
        * EActivateRfsNormal - RFS activation
        * EActivateRfsDeep - RFS activation, deep version
        * EShowBtAddress - BT address display
        * EResetWallet - Wallet reset
        * EBTLoopback - BT RF loopback activation/deactivation
        * EBTDebugMode - BT debug mode activation
        */
        enum
            {
            EBadPinChange,
            EBadPin2Change,
            EBadPinUnblock,
            EBadPin2Unblock,
            EActivateRfsNormal,
            EActivateRfsDeep,
            EShowBtAddress,
            EBTLoopback,
            EShowWlanMac,
            EBTDebugMode
            };

        /**
        * Process manufacturer specific command.
        *
        * @param aCommand command to be performed.
        */
        virtual void ProcessCommandL( TUint aCommand ) = 0;

        /**
        * Process debug code.
        *
        * @param aCode code to be processed.
        */
        virtual void ProcessDebugL( const TDesC& aCode ) = 0;

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, process the results of parsing.
        *
        * @param aResult It is results to be processed.
        */
        IMPORT_C virtual void ProcessL( 
            CPhoneGsmParserResult& aResult );
    
    };

#endif      // CPHONEGSMMANUFACTURERHANDLER_H
            
// End of File
