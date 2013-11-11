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
* Description: Provides Category iterator.
*
*
*/


#ifndef CPOSLMLOCALCATEGORYITERATOR_H
#define CPOSLMLOCALCATEGORYITERATOR_H

//  INCLUDES
#include <EPos_CPosLmCategoryManager.h>
#include "EPos_CPosLmLocalIterator.h"

// FORDWARD DECLARATIONS
class CPosLmLocalDatabase;

// CLASS DECLARATION

/**
* Category iterator. Iterates through all categories in a landmarks database.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalCategoryIterator) : public CPosLmLocalIterator
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aDb A database.
        * @param aSortPref A sort preference object.
        * @param aReferencedCategories Gets referenced categories if set to
        *   ETrue. All categories otherwise.
        */
        static CPosLmLocalCategoryIterator* NewL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        CPosLmCategoryManager::TCategorySortPref aSortPref,
        /* IN */        TBool aReferencedCategories
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalCategoryIterator();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalCategoryIterator();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aDb A database.
        * @param aSortPref A sort preference object.
        * @param aReferencedCategories Gets referenced categories if set to
        *   ETrue. All categories otherwise.
        */
        void ConstructL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        CPosLmCategoryManager::TCategorySortPref aSortPref,
        /* IN */        TBool aReferencedCategories
        );

    };

#endif      // CPOSLMLOCALCATEGORYITERATOR_H

// End of File
