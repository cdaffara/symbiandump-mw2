/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLmCategoryManager class
*
*/


#ifndef CPOSLMCATEGORYMANAGER_H
#define CPOSLMCATEGORYMANAGER_H

#include <e32base.h>
#include "EPos_CPosLandmarkDatabase.h"
#include "EPos_CPosLandmarkCategory.h"
#include "EPos_CPosLmItemIterator.h"
#include "EPos_CPosLmOperation.h"

/**
*  Category management for a landmark database.
*
*  A landmark database can contain a number of categories which can be
*  assigned to the landmarks in the database. A landmark can be associated
*  with multiple categories, e.g. a landmark can be a "Restaurant" and a "Pub".
*  Categories also enable filtered searches, e.g. a client could search for
*  nearby restaurants.
*
*  This class contains functions for managing landmark categories. This includes
*  reading, listing, creating and updating landmark categories.
*
*  @p NetworkServices capability is required for remote databases.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLmCategoryManager : public CBase
    {
    public:

        /**
        *  Specifies the sort preference for landmark categories.
        */
        enum TCategorySortPref
            {
            ECategorySortOrderNone = 0        /**<
                Categories not sorted */,
            ECategorySortOrderNameAscending   /**<
                Sorted ascending by category name. */,
            ECategorySortOrderNameDescending  /**<
                Sorted descending by category name. */
            };

    public:

        /**
        * Two-phased constructor.
        *
        * The client takes ownership of the category manager.
        *
        * @param[in] aLandmarkDatabase The landmark database to manage categories in.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmCategoryManager* NewL(
            CPosLandmarkDatabase& aLandmarkDatabase
        );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmCategoryManager();

    public:

        /**
        * Reads a landmark category from the database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * The client takes ownership of the returned category object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param aCategoryId The ID of the landmark category to read.
        * @returns The requested landmark category. The category object is put
        *   on the cleanup stack.
        *
        * @leave KErrNotFound The landmark category does not exist in the database.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLandmarkCategory* ReadCategoryLC( TPosLmItemId aCategoryId ) = 0;

        /**
        * Returns an object for iterating the landmark categories in the
        * database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * The iterator object is reset, so that the first
        * @ref CPosLmItemIterator::NextL call will return the first landmark
        * category.
        *
        * The client takes ownership of the returned iterator object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aSortPref How to sort the categories. Default is no sorting.
        * @return The landmark iterator.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        *
        * @panic "Landmarks Client"-EPosInvalidEnumValue
        *   Client specified invalid sort preference.
        */
        virtual CPosLmItemIterator* CategoryIteratorL(
            TCategorySortPref  aSortPref = ECategorySortOrderNone
        ) = 0;

        /**
        * Returns an object for iterating referenced landmark categories in
        * the database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * A category is referenced if there are landmarks in the database which
        * contains this category.
        *
        * The iterator object is reset, so that the first
        * @ref CPosLmItemIterator::NextL call will return the first landmark
        * category.
        *
        * The client takes ownership of the returned iterator object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aSortPref How to sort the categories. Default is no sorting.
        * @return The landmark iterator.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        *
        * @panic "Landmarks Client"-EPosInvalidEnumValue
        *   Client specified invalid sort preference.
        */
        virtual CPosLmItemIterator* ReferencedCategoryIteratorL(
            TCategorySortPref  aSortPref = ECategorySortOrderNone
        ) = 0;

        /**
        * Adds a landmark category to the database and returns its ID.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * Note: Clients are not allowed to create global categories.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @post Category is added to the database and category object
        *   has database item set (CPosLandmarkCategory::CategoryId()).
        *
        * @param[in,out] aCategory The landmark category to add.
        * @return The ID of the new category.
        *
        * @leave KErrArgument 1) Input category does not have a name set or
        *   2) if a global category is set in the category object.
        * @leave KErrAlreadyExists A category with the same name
        *   already exists in the database.
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual TPosLmItemId AddCategoryL(
            CPosLandmarkCategory& aCategory
        ) = 0;

        /**
        * Updates a landmark category in the database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * Note: Clients are not allowed to change the global category identifier in
        * the category object.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aCategory The new landmark category data.
        *
        * @leave KErrArgument 1) Input category does not have a name set or
        *   2) if a global category identifier is changed in the category object.
        * @leave KErrAlreadyExists A category with the same name
        *   already exists in the database.
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual void UpdateCategoryL(
            const CPosLandmarkCategory& aCategory
        ) = 0;

        /**
        * Removes a landmark category from the database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * If the landmark category does not exist in the database, nothing
        * happens.
        *
        * This call will also remove the category from all landmarks which
        * contained it.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that the category has not been removed,
        * but some landmarks may no longer contain this category
        *
        * The client takes ownership of the returned operation object.
        *
        * While removing the category, this operation will acquire a
        * write-lock on the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param aCategoryId The ID of the landmark category to delete.
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* RemoveCategoryL( TPosLmItemId aCategoryId ) = 0;

        /**
        * Remove a set of landmark categories from the database.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * If any of the landmark categories does not exist in the database, it
        * is ignored.
        *
        * This call will also remove the categories from all landmarks which
        * contained them.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that only a subset of the landmark
        * categories have been deleted.
        *
        * The client takes ownership of the returned operation object.
        *
        * If the database is read only, the returned operation will fail with error
        * code @p KErrAccessDenied.
        *
        * This call will also remove the categories from all landmarks which
        * contained them.
        *
        * While removing the category, this operation will acquire a
        * write-lock on the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param aCategoryIdArray The IDs of the landmark categories to delete.
        * @returns A handle to the operation.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* RemoveCategoriesL(
            const RArray<TPosLmItemId>& aCategoryIdArray
        ) = 0;

        /**
        * Adds a category to a set of landmarks.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * If any of the specified landmarks does not exist, the category will
        * be added to the other landmarks. No error will be reported though.
        *
        * If the category is already contained in one of the landmarks, nothing
        * will be further added to that landmark.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that the category has only been added
        * to a subset of the landmarks.
        *
        * The client takes ownership of the returned operation object.
        *
        * Note: There is no need to call
        * @p CPosLandmarkDatabase::UpdateLandmark for this change to take
        * place.
        *
        * If the database is read only, the returned operation will fail with error
        * code @p KErrAccessDenied.
        *
        * While adding the category to the landmarks, this operation will
        * acquire a write-lock on the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aCategoryId The category to add to the set of landmarks.
        * @param[in] aLandmarkIdArray The landmarks to add the category to.
        * @returns A handle to the operation.
        *
        * @leave KErrNotFound The specified category does not exist in database.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* AddCategoryToLandmarksL(
            TPosLmItemId aCategoryId,
            RArray<TPosLmItemId>& aLandmarkIdArray
        ) = 0;

        /**
        * Removes a category from a set of landmarks.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * If any of the specified landmarks does not exist, the category will
        * be removed from the other landmarks. No error will be reported though.
        *
        * If the category is not found in one of the landmarks, nothing will
        * happen for that landmark.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that the category has only been removed
        * from a subset of the landmarks.
        *
        * The client takes ownership of the returned operation object.
        *
        * If the database is read only, the returned operation will fail with error
        * code @p KErrAccessDenied.
        *
        * While removing the category from the landmarks, this operation will
        * acquire a write-lock on the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aCategoryId The category to remove from the set of landmarks.
        * @param[in] aLandmarkIdArray The landmarks to remove the category from.
        * @returns A handle to the operation.
        *
        * @leave KErrNotFound The specified category does not exist in database.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* RemoveCategoryFromLandmarksL(
            TPosLmItemId aCategoryId,
            RArray<TPosLmItemId>& aLandmarkIdArray
        ) = 0;

        /**
        * Gets a category by name.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * The category name must be unique in the database, so there cannot be
        * multiple matches.
        *
        * This function only looks for an exact match.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aCategoryName The name of the category to get.
        * @return @p KPosLmNullItemId if the category was not found, otherwise
        *   the ID of the category item in the database.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual TPosLmItemId GetCategoryL(
            const TDesC& aCategoryName
        ) = 0;

        /**
        * Gets the ID of a global category.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aGlobalCategory The global category to look for.
        * @return @p KPosLmNullItemId if the category was not found, otherwise
        *   the ID of the category item in the database.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual TPosLmItemId GetGlobalCategoryL(
            TPosLmGlobalCategory aGlobalCategory
        ) = 0;

        /**
        * Gets the predefined name of a global category.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * @param[in] aGlobalCategory The global category to get a name for.
        * @return The name of the global category or @p NULL if the category
        *   is not recognized.
        *
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual HBufC* GlobalCategoryNameL(
            TPosLmGlobalCategory aGlobalCategory
        ) = 0;

        /**
        * Resets the information for all global categories.
        *
        * @pre Database is initialized (see @ref CPosLandmarkDatabase::IsInitializingNeeded).
        *
        * Global categories usually has a default name and icon. The client
        * can change the name and icon. This function resets the name and
        * icon to the default ones.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, it is possible that that only a subset of the global
        * categories have been resetted.
        *
        * The client takes ownership of the returned operation object.
        *
        * While resetting, this operation will acquire a write-lock on the
        * database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @returns A handle to the operation.
        *
        * @leave KErrAccessDenied The database is read-only.
        * @leave KErrPosLmNotInitialized Database is not yet initialized.
        */
        virtual CPosLmOperation* ResetGlobalCategoriesL() = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLmCategoryManager();

    private:

        // Prohibit copy constructor
        CPosLmCategoryManager( const CPosLmCategoryManager& );
        // Prohibit assigment operator
        CPosLmCategoryManager& operator= ( const CPosLmCategoryManager& );

    private:

        // Implementation UID
        TUid iDtorIdKey;

    };

#endif      // CPOSLMCATEGORYMANAGER_H


