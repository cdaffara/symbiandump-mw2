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
* Description: Helper class to provide sort preference
*
*/


#ifndef CPOSLMMULTIDBSORTPREF_H
#define CPOSLMMULTIDBSORTPREF_H

//  INCLUDES
#include <EPos_CPosLmCategoryManager.h>

// FORWARD DECLARATION
class CPosLmSearchCriteria;
class TPosLmSortPref;

// CLASS DECLARATION

/**
* Helper class that can hold either a TPosLmSortPref or a
* CPosLmCategoryManager::TCategorySortPref.
*/
NONSHARABLE_CLASS(CPosLmMultiDbSortPref) : public CBase
    {
    public:  // Data types

        /**
        *  Specifies the sort preference type.
        *
        */
        enum TSortType
            {
            ELandmarkNoSorting = 0,
            ELandmarkDistanceSorting,
            ELandmarkSorting,
            ECategorySorting
            };

    public:  // Constructors and destructor

        /*
        * Two-phased constructor.
        * @return A new instance of this class.
        */
        static CPosLmMultiDbSortPref* NewL(
        );

        /*
        * Two-phased constructor.
        * @param aSortPref A sort preference object.
        * @return A new instance of this class.
        */
        static CPosLmMultiDbSortPref* NewL(
        /* IN  */       const TPosLmSortPref& aSortPref
        );

        /*
        * Two-phased constructor.
        * @param aSortPref A sort preference object.
        * @return A new instance of this class.
        */
        static CPosLmMultiDbSortPref* NewL(
        /* IN  */       CPosLmCategoryManager::TCategorySortPref aSortPref
        );

        /*
        * Destructor.
        */
        virtual ~CPosLmMultiDbSortPref();

    public: // New functions

        /*
        * Sets the sort preference to use distance sorting for landmarks.
        */
        void SetDistanceSorting();

        /*
        * Get the preferred sort type.
        *
        * @return The preferred sort type.
        */
        TSortType SortType() const;

        /*
        * Get the landmark sort preference object.
        *
        * @return The landmark sort preference object.
        */
        const TPosLmSortPref& LandmarkSortPref() const;

        /*
        * Get the category sort preference object.
        *
        * @return The category sort preference object.
        */
        CPosLmCategoryManager::TCategorySortPref CategorySortPref() const;

    private:

        /**
        * C++ default constructor.
        */
        CPosLmMultiDbSortPref(
        /* IN  */       TSortType aSortType
        );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL(
        /* IN  */       const TPosLmSortPref& aSortPref
        );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL(
        /* IN  */       CPosLmCategoryManager::TCategorySortPref aSortPref
        );

    private:    // Data

        TSortType                                   iSortType;

        TPosLmSortPref*                             iLmSortPref;
        CPosLmCategoryManager::TCategorySortPref*   iCatSortPref;

    };

#endif      // CPOSLMMULTIDBSORTPREF_H

// End of File
