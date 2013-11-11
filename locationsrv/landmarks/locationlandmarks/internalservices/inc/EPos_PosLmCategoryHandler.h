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
* Description: Handles category operations on a local landmark database.
*
*
*/


#ifndef POSLMCATEGORYHANDLER_H
#define POSLMCATEGORYHANDLER_H

#include <d32dbms.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_Landmarks.h>

class CPosLmLocalDbAccess;
class CPosLandmarkCategory;

/**
*  Handles category operations on a local landmark database.
*/
class PosLmCategoryHandler
    {

    public: // Data types

        /**
        * Category check type.
        */
        enum TCheckCategoryType
            {
            EAddCheck = 0,
            EUpdateCheck,
            EServerUpdateCheck
            };

    public: // New functions

        /**
        * Reads a landmark category from the database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in, out] aCategory A category with a specified ID.
        */
        IMPORT_C static void ReadCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aCategory );

        /**
        * Adds a landmark category to the database.
        *
        * This function will leave with code @p KErrAlreadyExists if a category
        * with the same name already exists in the database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in, out] aCategory The landmark category to add. On out, the ID have
        *   been set.
        */
        IMPORT_C static void AddCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aCategory );

        /**
        * Updates a landmark category in the database.
        *
        * This function will leave with code @p KErrAlreadyExists if another
        * category with the same name exists in the database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory The new landmark category data. ID must be set.
        */
        IMPORT_C static void UpdateCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory );

        /**
        * Removes a landmark category from the database.
        *
        * This call will also remove the category from all landmarks which
        * contained it.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategoryId The ID of the landmark category to delete.
        */
        IMPORT_C static void RemoveCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId  aCategoryId );

        /**
        * Updates a global category name in the database if the global category
        * exists and if the name has not been modified. If another category has
        * the same name, that category will be deleted and all landmarks
        * including that category will be added to the global category instead.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        */
        IMPORT_C static void UpdateGlobalCategoryNameL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory );

        /**
        * Resets a global category.
        * @param[in] aDbAccess A database access.
        * @param[in, out] aGlCategory A global category.
        */
        IMPORT_C static void ResetGlobalCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aGlCategory );

        /**
        * Leaves with KErrAlreadyExists if the category name already exists in
        * the database. For EUpdateCheck mode, the existing category name is
        * allowed if it belongs to the category that is going to be updated.
        * For EServerUpdateCheck mode, the existing category name is allowed if
        * it belongs to the global category that is going to be updated.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        * @param[in] aMode Type of check (add/update).
        */
        IMPORT_C static void VerifyCategoryNameNotExistsL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory,
            const TCheckCategoryType& aMode );

        /**
        * Verifies whether a category name exists or not.
        *
        * Returns KErrAlreadyExists if the category name already exists in
        * the database and if so the id of that category is returned.
        *
        * For EUpdateCheck mode existing category name is allowed if it belongs
        * to the category that is going to be updated.
        * For EServerUpdateCheck mode existing category name is allowed if it
        * belongs to the global category that is going to be updated.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        * @param[in] aMode Type of check (add/update).
        * @param[out] aId Category id of the category having the specified name.
        * @return KErrAlreadyExists if the category name already exists,
        *   otherwise KErrNone.
        */
        IMPORT_C static TInt VerifyCategoryNameNotExistsL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory,
            const TCheckCategoryType& aMode,
            TPosLmItemId& aId );

        /**
        * Leaves with KErrAlreadyExists if the global category has been changed
        * for the specified category or KErrNotFound if the category is not
        * found.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        */
        IMPORT_C static void VerifyGlobalCategoryUnchangedL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory );

        /**
        * Leaves with KErrArgument if the category is not properly set.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        * @param[in] aMode Type of check (add/update).
        */
        IMPORT_C static void VerifyCategoryIsProperL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory,
            const TCheckCategoryType& aMode );

        /**
        * Returns whether a global category exists or not. If it exists the
        * category id will be returned together with a flag telling if the
        * category name has been modified.
        * @param[in] aDbAccess A database access.
        * @param[in] aGlCategoryId A global category id.
        * @param[out] aCategoryId Category id of the existing global category.
        * @param[out] aIsModified ETrue if the category name has been modified,
        *   EFalse otherwise.
        * @return ETrue if the global category exists, EFalse otherwise.
        */
        IMPORT_C static TBool IsGlobalCategoryExistingL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmGlobalCategory aGlCategoryId,
            TPosLmItemId& aCategoryId,
            TBool& aIsModified );

        /**
        * Searches for a category in the landmark database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategoryId The category to search for.
        * @return KErrNone if found, otherwise KErrNotFound.
        */
        IMPORT_C static TInt FindCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aCategoryId );

        /**
        * Get all category IDs from the landmark database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aSortPref A sort preference.
        * @param[out] aIdArray A category ID array, containing the results.
        */
        IMPORT_C static void GetAllCategoryIdsL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLmCategoryManager::TCategorySortPref aSortPref,
            RArray<TPosLmItemId>& aIdArray );

        /**
        * Get all referenced category IDs from the landmark database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aSortPref A sort preference.
        * @param[out] aIdArray A category ID array, containing the results.
        */
        IMPORT_C static void GetReferencedCategoryIdsL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLmCategoryManager::TCategorySortPref aSortPref,
            RArray<TPosLmItemId>& aIdArray );

        /*
        * Gets the category IDs of all global categories except for those that
        * are not wanted.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aNotWantedIds Global categories that should not be included in
        *   the result.
        * @param[out] aIdArray A category ID array containing the result.
        */
        IMPORT_C static void GetGlobalCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TInt>& aNotWantedIds,
            RArray<TPosLmItemId>& aIdArray );

        /**
        * Removes all global categories from a database.
        *
        * @param[in] aDbAccess A database access.
        */
        IMPORT_C static void RemoveAllGlobalCategoriesL(
            CPosLmLocalDbAccess& aDbAccess );

        /** Returns total number of category rows */
        IMPORT_C static TUint TotalCategoryCountL( CPosLmLocalDbAccess& aDbAccess );

        /**
         * Reads a landmark category from the database. The category id must be set.
         *
         * Leaves with @p KErrNotFound if not found.
         *
         * @param[in][in] aView View to categorytable, positioned
         *      to the record with category's data
         * @param[in][in] aDbAccess A database access.
         * @param[in][in,out] aCategory A landmark category with the ID set.
         */
        IMPORT_C static void ReadCategoryL(
            RDbRowSet& aView,
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aCategory );

    private:

        /**
        * Gets category IDs from a landmark database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aSortPref A sort preference.
        * @param[in] aFetchReferenced Only get referenced category IDs.
        * @param[in] aRefIdArray If fetching referenced categories, only categories
        *   matching these IDs will be returned.
        * @param[out] aIdArray A category ID array, containing the results.
        */
        static void GetCategoryIdsL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLmCategoryManager::TCategorySortPref aSortPref,
            TBool aFetchReferenced,
            const RArray<TPosLmItemId>& aRefIdArray,
            RArray<TPosLmItemId>& aIdArray );

        /**
        * Reads global id from the prepared view. RDbView::GetL must have
        * been called before calling this function.
        * @param[in, out] aView A prepared view.
        * @param[in, out] aCategory The global id will be stored into this category.
        */
        static void ReadGlobalIdL(
            RDbRowSet& aView,
            CPosLandmarkCategory& aCategory );

        /**
        * Reads name from the prepared view. RDbView::GetL must have
        * been called before calling this function.
        * @param[in, out] aView A prepared view.
        * @param[in, out] aCategory The name will be stored into this category.
        */
        static void ReadNameL(
            RDbRowSet& aView,
            CPosLandmarkCategory& aCategory );

        /**
        * Reads an icon from the prepared view and the database.
        * RDbView::GetL must have been called before calling this function.
        * @param[in] aDbAccess A database access.
        * @param[in, out] aView A prepared view.
        * @param[in, out] aCategory The icon will be stored into this category.
        */
        static void ReadIconL(
            CPosLmLocalDbAccess& aDbAccess,
            RDbRowSet& aView,
            CPosLandmarkCategory& aCategory );

        /**
        * Adds category info to the database.
        * @param[in] aDbAccess A database access.
        * @param[in, out] aCategory A category.
        * @param[in] aIconId Icon database id.
        */
        static void AddCategoryInfoL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aCategory,
            TUint32 aIconId );

        /**
        * Performs an update of a category.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        * @param[in] aIconId Icon database id.
        * @param[in] aReset ETrue if in resetting mode. EFalse if ordinary update.
        */
        static void PerformUpdateL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory,
            const TUint32& aIconId,
            TBool aReset );

        /**
        * Reads the icon id of the specified category id.
        * @param[in] aDbAccess A database access.
        * @param[out] aIconId An icon id in database.
        * @return ETrue if the id is not set. EFalse if set.
        */
        static TBool ReadIconIdL(
            CPosLmLocalDbAccess& aDbAccess,
            const TPosLmItemId& aId,
            TUint32& aIconId );

        /**
        * Performs an add of a category.
        * @param[in] aDbAccess A database access.
        * @param[in, out] aCategory A category.
        */
        static void PerformAddL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmarkCategory& aCategory );

        /**
        * Merges the category with the global category. This function will
        * change all landmarks containing the category to contain the
        * global category instead.
        * @param[in] aDbAccess A database access.
        * @param[in] aGlCategory Category id of the global category.
        * @param[in] aCategory Category id of the category.
        */
        static void MergeCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aGlCategory,
            TPosLmItemId aCategory );

        /**
        * Changes the name of a category. If the category does not have a
        * category name this function will do nothing.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategoryId A category id.
        * @param[in] aCategory A category.
        */
        static void ChangeCategoryNameL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aCategoryId,
            const CPosLandmarkCategory& aCategory );

        /**
        * Updates a category without using any begin/commit transaction.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategory A category.
        * @param[in] aReset ETrue if in resetting mode. EFalse if ordinary update.
        */
        static void UpdateCategoryWithoutTransactionL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmarkCategory& aCategory,
            TBool aReset );

        /** Opens view, positioned at the specified category. 
         * @param[in] aCategoryId ID of the category 
         * @param[in] aView A view 
         * @param[in] aDbAccess A database access.
         */
        static void OpenCategoryViewLC(
            TPosLmItemId aCategoryId,
            RDbView& aView,
            CPosLmLocalDbAccess& aDbAccess );
    };

#endif      // POSLMCATEGORYHANDLER_H

