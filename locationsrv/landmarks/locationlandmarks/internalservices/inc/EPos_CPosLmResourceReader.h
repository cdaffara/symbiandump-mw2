/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Simplified interface to resource files
*
*
*/


#ifndef CPOSLMRESOURCEREADER_H
#define CPOSLMRESOURCEREADER_H

//  INCLUDES
#include <e32base.h>
#include <barsc.h>
#include <barsread.h>

// CONSTANTS

// CLASS DECLARATION

/**
*  Simplified interface to resource files
*/
class CPosLmResourceReader : public CBase
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aPath Path to the resource file (excluding drive).
        */
        IMPORT_C static CPosLmResourceReader* NewLC(
        /* IN  */       const TDesC& aPath
        );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmResourceReader();

    public:     // New functions

        /**
        * Reads a string from the currently loaded resource.
        *
        * @param aResourceId Resource id to read from. The function leaves if
        *   there is no matching resource or the offset does not match.
        * @return A heap-allocated string.
        */
        IMPORT_C HBufC* ReadHBufCL(
        /* IN  */      TInt aResourceId
        );

        /**
        * Reads an TInt/integer from the currently loaded resource.
        *
        * @param aResourceId Resource id to read from. The function leaves if
        *   there is no matching resource or the offset does not match.
        * @return An integer.
        */
        IMPORT_C TInt ReadInt16L(
        /* IN  */      TInt aResourceId
        );

        /**
        * Reads an TInt/integer from the currently loaded resource.
        *
        * @param aResourceId Resource id to read from. The function leaves if
        *   there is no matching resource or the offset does not match.
        * @return An integer.
        */
        IMPORT_C TInt ReadInt32L(
        /* IN  */      TInt aResourceId
        );

    protected: // New functions

        /**
        * Loads a resource from the resource file into the buffer.
        *
        * @param aResourceId Id of the resource to be loaded.
        */
        IMPORT_C void LoadResourceL(
        /* IN  */       TInt aResourceId
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmResourceReader();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aPath Path to the resource file (excluding drive).
        */
        void ConstructL(
        /* IN  */   const TDesC& aPath
        );

        // By default, prohibit copy constructor
        CPosLmResourceReader( const CPosLmResourceReader& );
        // Prohibit assigment operator
        CPosLmResourceReader& operator= ( const CPosLmResourceReader& );

    private:    // Data

        RFs             iFileSession;
        RResourceFile   iResourceFile;
        HBufC8*         iResourceBuffer;
        TResourceReader iResourceReader;
    };

#endif      // CPOSLMRESOURCEREADER_H

// End of File
