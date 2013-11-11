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
* Description:  It is base class for all handlers.
*
*/


#ifndef CPHONEGSMHANDLERBASE_H
#define CPHONEGSMHANDLERBASE_H

// INCLUDES
#include    <e32base.h>

// FORWARD DECLARATIONS
class CPhoneGsmParserResult;

// CLASS DECLARATION

/**
* It is base class for all handlers.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmHandlerBase 
    : public CBase
    {
    public: // New functions
        
        /**
        * Process the results of parsing.
        *
        * @param aResult It is results to be processed.
        */
        virtual void ProcessL( 
            CPhoneGsmParserResult& aResult ) = 0;

    };

#endif      // CPHONEGSMHANDLERBASE_H
            
// End of File
