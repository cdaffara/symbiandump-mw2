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
* Description: Reads global categories from a resource file.
*
*
*/


#ifndef CPOSLMGLOBALCATEGORYREADER_H
#define CPOSLMGLOBALCATEGORYREADER_H

//  INCLUDES
#include <e32base.h>
#include <barsc.h>
#include <barsread.h>

// FORWARD DECLARATION
class CPosLmGlobalCategoryData;
class CPosLandmarkCategory;

// CLASS DECLARATION

/**
* Reads global categories from a resource file.
*/
class CPosLmGlobalCategoryReader : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPosLmGlobalCategoryReader* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmGlobalCategoryReader();

    public: // New functions

        /**
        * Returns the number of global categories.
        *
        * @return The number of global categories.
        */
        IMPORT_C TInt Count();

        /**
        * Returns the global category specified by an index, which must be
        * between 0 and the number of global categories minus one.
        * Caller takes ownership of the returned object.
        * Leaves with KErrArgument if index is out of boundary.
        *
        * @param aIndex An index.
        * @return A global category.
        */
        IMPORT_C CPosLandmarkCategory* GlobalCategoryLC(
        /* IN */        TInt aIndex
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmGlobalCategoryReader();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Reads all data from the resource file.
        */
        void ReadDataL();

        /**
        * Reads the icon paths in the resource file.
        */
        void ReadIconPathsL();

        /**
        * Reads the global categories data from the resource file.
        */
        void ReadGlobalCategoriesL();

    private:    // Data

        RFs             iFileSession;
        RResourceFile   iResourceFile;
        HBufC8*         iResourceBuffer;
        TResourceReader iResourceReader;

        CArrayPtrFlat<CPosLmGlobalCategoryData>* iGlCatArray;
        CDesCArrayFlat* iPathArray;
    };

#endif      // CPOSLMGLOBALCATEGORYREADER_H

// End of File

