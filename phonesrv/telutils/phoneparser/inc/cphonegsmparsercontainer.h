/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is handler that is also container for handlers. Parsing 
*                requests are simply forwarded to the contained handlers.
*
*/


#ifndef CPHONEGSMPARSERCONTAINER_H
#define CPHONEGSMPARSERCONTAINER_H

// INCLUDES
#include    <e32base.h>
#include    "cphonegsmparserbase.h" 

// CLASS DECLARATION

/**
* It is itself a parser and it is container for parsers. Parsing is
* done so that each of the contained parsers are called.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmParserContainer )
    : public CPhoneGsmParserBase
    {
    public: // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmParserContainer* NewLC();
        
        /**
        * Destructor.
        */
        ~CPhoneGsmParserContainer();

    public: // New functions
        
        /**
        * Add a parser to the container.
        *
        * Note that ownership is transferred. Panics if parser is already
        * added.
        */
        void AddL( CPhoneGsmParserBase& aParser );

    public: // Functios from base classes

        /**
        * From CPhoneGsmParserBase, parses strings.
        */
        TBool ParseL( 
            const TDesC& aString, 
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );
                
    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmParserContainer();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();
       
    private:    // Data

        // Type for parser array.
        typedef CArrayPtrFlat< CPhoneGsmParserBase > CParserArray;

        // Owned array of parsers.
        CParserArray* iArray;

    };

#endif      // CPHONEGSMPARSERCONTAINER_H
            
// End of File
