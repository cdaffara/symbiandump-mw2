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
* Description:  It is handler for PCN specific procedures.
*
*/


#ifndef CPHONEGSMPCNPROCEDUREHANDLER_H
#define CPHONEGSMPCNPROCEDUREHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is handler for GsmPcn procedures.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmPcnProcedureHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions

        /**
        * Process SIM locking.
        *
        * Parameters are empty strings if SIM lock
        * code was just garbage.
        * 
        * @param aPassword sequence of digits.
        * @param aType sequence of digits.
        */
        virtual void ProcessSimLockL( 
            const TDesC& aPassword, 
            const TDesC& aType ) = 0;

        /**
        * Process SIM unlocking.
        *
        * Parameters are empty strings if SIM unlock
        * code was just garbage.
        * 
        * @param aPassword sequence of digits.
        * @param aType sequence of digits.
        */
        virtual void ProcessSimUnlockL(
            const TDesC& aPassword,
            const TDesC& aType ) = 0;

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, processes result.
        *
        * @param aResult It is to be processed.
        */
        IMPORT_C virtual void ProcessL( CPhoneGsmParserResult& aResult );
        
    };

#endif      // CPHONEGSMPCNPROCEDUREHANDLER_H
            
// End of File
