/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Category management for a local landmark database.
*
*
*/


#ifndef CPOSLMLOCALCATEGORYMANAGER_H
#define CPOSLMLOCALCATEGORYMANAGER_H

#include <EPos_CPosLmCategoryManager.h>

class CPosLmLocalDatabase;
class CPosLmDiskUtilities;
class CPosLmOperation;
class PosLmCategoryHandler;

/**
*  Category management for a local landmark database.
*
*  This class contains functions for managing landmark categories in a local
*  database. This includes reading, listing, creating and updating landmark
*  categories.
*
*  @lib eposlmlocalaccess.lib
*/
class CPosLmLocalCategoryManager : public CPosLmCategoryManager
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aLmLocalDatabase The landmark database to manage categories
        *   in. Must be a CPosLmLocalDatabase.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmLocalCategoryManager* NewL( TAny* aLmLocalDatabase );

        /**
        * Destructor.
        */
        IMPORT_C ~CPosLmLocalCategoryManager();

    public: // From CPosLmCategoryManager

        /** @param aCategoryId The ID of the landmark category to read.
        *   @returns The requested landmark category. The category object is put
        *   on the cleanup stack. */
        CPosLandmarkCategory* ReadCategoryLC( TPosLmItemId  aCategoryId );

        /** @param aSortPref How to sort the categories. Default is no sort.
        *   @return The landmark iterator. */
        CPosLmItemIterator* CategoryIteratorL(
            TCategorySortPref  aSortPref = ECategorySortOrderNone );

        /** @param aSortPref How to sort the categories. Default is no sort.
        *   @return The landmark iterator. */
        CPosLmItemIterator* ReferencedCategoryIteratorL(
            TCategorySortPref  aSortPref = ECategorySortOrderNone );

        /** @param[in, out] aCategory The landmark category to add.
        *   @return The ID of the new category. */
        TPosLmItemId AddCategoryL( CPosLandmarkCategory&  aCategory );

        /** @param[in] aCategory The new landmark category data. */
        void UpdateCategoryL( const CPosLandmarkCategory&  aCategory );

        /** @param aCategoryId The ID of the landmark category to delete.
        *   @returns A handle to the operation. */
        CPosLmOperation* RemoveCategoryL( TPosLmItemId  aCategoryId );

        /** @param[in] aCategoryIdArray The IDs of the landmark categories to delete.
        *   @returns A handle to the operation. */
        CPosLmOperation* RemoveCategoriesL(
            const RArray<TPosLmItemId>&  aCategoryIdArray );

        /** @param[in] aCategoryId The category to add to the set of landmarks.
        *   @param[in] aLandmarkIdArray The landmarks to add the category to.
        *   @returns A handle to the operation. */
        CPosLmOperation* AddCategoryToLandmarksL(
            TPosLmItemId  aCategoryId,
            RArray<TPosLmItemId>&  aLandmarkIdArray );

        /*  @param aCategoryId The category to remove from the set of landmarks.
        *   @param[in] aLandmarkIdArray The landmarks to remove the category from.
        *   @returns A handle to the operation. */
        CPosLmOperation* RemoveCategoryFromLandmarksL(
            TPosLmItemId  aCategoryId,
            RArray<TPosLmItemId>&  aLandmarkIdArray );

        /** @param[in] aCategoryName The name of the category to get.
        *   @return @p KPosLmNullItemId if the category was not found, otherwise
        *   the id of the category item in the database. */
        TPosLmItemId GetCategoryL( const TDesC&  aCategoryName );

        /** @param aGlobalCategory The global category to look for.
        *   @return @p KPosLmNullItemId if the category was not found, otherwise
        *   the id of the category item in the database. */
        TPosLmItemId GetGlobalCategoryL( TPosLmGlobalCategory aGlobalCategory );

        /** @param aGlobalCategory The global category to get a name for.
        *   @return The name of the global category or @p NULL if the category
        *   is not recognized. */
        HBufC* GlobalCategoryNameL( TPosLmGlobalCategory aGlobalCategory );

        /** @returns A handle to the operation. */
        CPosLmOperation* ResetGlobalCategoriesL();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalCategoryManager();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aLandmarkDatbase The landmark database to manage categories
        *   in.
        */
        void ConstructL(
        /* IN */        TAny* aLmLocalDatabase
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalCategoryManager(
            const CPosLmLocalCategoryManager& );
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalCategoryManager& operator=(
            const CPosLmLocalCategoryManager& );

    private:    // Data

        // Local landmark database (no ownership)
        CPosLmLocalDatabase* iDb;

        // Disk Utilities
        CPosLmDiskUtilities* iDiskUtilities;

    };

#endif      // CPOSLMLOCALCATEGORYMANAGER_H

