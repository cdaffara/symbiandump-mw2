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
* Description: Class holding icon path data from a resource file.
*
*
*/


#ifndef CPOSLMGLOBALCATEGORYDATA_H
#define CPOSLMGLOBALCATEGORYDATA_H

//  INCLUDES
#include <e32base.h>
#include <barsread.h>

// CLASS DECLARATION

/**
* Class holding icon path data from a resource file.
*/
NONSHARABLE_CLASS(CPosLmGlobalCategoryData) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aReader A resource reader.
        */
        static CPosLmGlobalCategoryData* NewLC(
        /* IN/OUT */    TResourceReader& aReader
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmGlobalCategoryData();

    public: // New functions

        /**
        * Returns the global category id.
        *
        * @return The global category id.
        */
        TUint Id() const;

        /**
        * Returns the global category name.
        *
        * @return The global category name.
        */
        TPtrC Name() const;

        /**
        * Returns the icon index.
        *
        * @return The icon index.
        */
        TInt IconIndex() const;

        /**
        * Returns the icon mask index.
        *
        * @return The icon mask index.
        */
        TInt IconMaskIndex() const;

        /**
        * Returns the icon path number within the resource array.
        *
        * @return The icon path number.
        */
        TInt IconPathNumber() const;

    private:

        /**
        * C++ default constructor.
        */
        CPosLmGlobalCategoryData();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aReader A resource reader.
        */
        void ConstructL(
        /* IN/OUT */    TResourceReader& aReader
        );

    private: // Data

        /** Category id. */
        TUint iId;

        /** Category name. */
        HBufC* iName;

        /** Icon index. */
        TInt iIconIndex;

        /** Icon mask index. */
        TInt iIconMaskIndex;

        /** Icon path number within the path array. */
        TInt iIconPathNr;
    };

#endif      // CPOSLMGLOBALCATEGORYDATA_H

// End of File

