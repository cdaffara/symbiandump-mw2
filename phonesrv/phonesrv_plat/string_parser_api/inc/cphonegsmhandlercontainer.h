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
* Description:  It is container for handlers. It can also be used as a handler.
*
*/


#ifndef CPHONEGSMHANDLERCONTAINER_H
#define CPHONEGSMHANDLERCONTAINER_H

// INCLUDES
#include    <e32base.h>
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is container for handlers. It can also be used as a handler.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmHandlerContainer 
    : public CPhoneGsmHandlerBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneGsmHandlerContainer* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C ~CPhoneGsmHandlerContainer();

    public: // New functions
        
        /**
        * Add a new handler to the container.
        *
        * Note that ownership is transferred. The method panics if
        * you try to add same instance twice.
        */
        IMPORT_C void AddHandlerL( CPhoneGsmHandlerBase& aHandler );

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, process the results of parsing.
        *
        * Note that it will forward the request to each of the contained
        * handler.
        *
        * @param aResult It is results to be processed.
        */
        void ProcessL( 
            CPhoneGsmParserResult& aResult );

    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmHandlerContainer();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPhoneGsmHandlerContainer( const CPhoneGsmHandlerContainer& );
        // Prohibit assigment operator
        CPhoneGsmHandlerContainer& operator= ( const CPhoneGsmHandlerContainer& );

    private:    // Data
        
        typedef CArrayPtrFlat< CPhoneGsmHandlerBase > CHandlerArray;

        // Owned array of handlers.
        CHandlerArray* iHandlers;

    };

#endif      // CPHONEGSMHANDLERCONTAINER_H
            
// End of File
