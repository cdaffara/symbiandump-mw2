/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Sending capabilities for sending services. Used by SendUi to
*                filter sending services from "Send" pop-up list query.
*
*/




#ifndef TSENDINGCAPABILITIES_H
#define TSENDINGCAPABILITIES_H

//  INCLUDES
#include <e32base.h>

// MACROS
#define KCapabilitiesForAllServices TSendingCapabilities::CapabilitiesForAllServices()

// CLASS DECLARATION

/**
*  Sending capabilities for sending services.
*  Used by SendUi to filter sending services from "Send" pop-up list query.
*
*  @lib CSendUi.lib
*  @since Series 60 3.0
*/
class TSendingCapabilities
    {
    public:

    enum TSendingFlags
        {
        EAllServices = 0,
        ESupportsAttachments = 1,
        ESupportsBodyText = 2,
        ESupportsBioSending = 4,
        ESupportsEditor = 8
        };

    public:  // Constructors and destructor

        /**
        * Default C++ constructor
        */
        IMPORT_C TSendingCapabilities();
        
        /**
        * C++ constructor
        * @param aBodySize Maximum body size.
        * @param aMessageSize Maximum whole message size (inc. attachments).
        * @param aFlags: TSendingFlags.
        */
        IMPORT_C TSendingCapabilities( 
            TInt aBodySize,
            TInt aMessageSize,
            TInt aFlags );
        
    public:  // New functions

        static inline TSendingCapabilities CapabilitiesForAllServices();

        /**
        * Overloaded equal operator.
        * @param aSendingCapabilities Sending capabilities to be compared
        * @return ETrue if equal, otherwise, EFalse
        */
        IMPORT_C TBool operator==(
            const TSendingCapabilities& aSendingCapabilities ) const;

        /**
        * Overloaded not equal operator.
        * @param aSendingCapabilities Sending capabilities to be compared
        * @return ETrue if not equal, otherwise, EFalse
        */
        IMPORT_C TBool operator!=(
            const TSendingCapabilities& aSendingCapabilities ) const;

    public:
        
        TInt iBodySize;
        TInt iMessageSize;
        TInt iFlags;
    };

#include "TSendingCapabilities.inl"

#endif      // TSENDINGCAPABILITIES_H  
            
// End of File

