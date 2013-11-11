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
* Description: Handles landmark operations on a local landmark database.
*
*
*/


#ifndef POSLMLANDMARKHANDLER_H
#define POSLMLANDMARKHANDLER_H

#include <d32dbms.h>
#include <EPos_Landmarks.h>
#include <EPos_CPosLandmark.h>

class CPosLmLocalDbAccess;
class CPosLandmarkCategory;
class CPosLmPartialReadParameters;

/**
*  Handles landmark operations on a local landmark database.
*/
class PosLmLandmarkHandler
    {

    public: // New functions

        /**
        * Reads a landmark from the database. The landmark id must be set. The
        * id specifies which landmark to read.
        *
        * Leaves with @p KErrNotFound if not found.
        *
        * @param[in] aDbAccess A database access.
        * @param[in,out] aLandmark A landmark with the ID set.
        */
        IMPORT_C static void ReadLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark );

        /**
        * Adds a landmark to the database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aLandmark The landmark to add.
        */
        IMPORT_C static void AddLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark );

        /**
        * Updates a landmark in the database.
        *
        * Leaves with @p KErrNotFound if not found.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aLandmark The new landmark data.
        */
        IMPORT_C static void UpdateLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark );

        /**
        * Removes a landmark from the database.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aLandmarkId The ID of the landmark to delete.
        */
        IMPORT_C static void RemoveLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId  aLandmarkId );

        /**
        * Reads partial information about a landmark from the landmark
        * database. The landmark id must be set. The id specifies which
        * landmark to read.
        *
        * @param[in] aDbAccess A database access.
        * @param[in,out] aLandmark A landmark with id set.
        * @param[in] aPartialSettings Partial settings.
        */
        IMPORT_C static void ReadPartialLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark,
            CPosLmPartialReadParameters& aPartialSettings );

        /**
         * Reads partial information about a landmark from the landmark
         * database. The landmark id must be set. The id specifies which
         * landmark to read.
         *
         * @param[in] aView View to landmark table, positioned
         *      to landmark's record
         * @param[in] aDbAccess A database access.
         * @param[in,out] aLandmark A landmark with id set.
         * @param[in] aPartialSettings Partial settings.
         */
         IMPORT_C static void ReadPartialLandmarkL(
             RDbRowSet& aView,
             CPosLmLocalDbAccess& aDbAccess,
             CPosLandmark& aLandmark,
             CPosLmPartialReadParameters& aPartialSettings );

         /**
        * Adds the specified categories to the specified landmark.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategories An array of category IDs.
        * @param[in] aLandmarkId A landmark id.
        */
        IMPORT_C static void AddCategoriesToLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TPosLmItemId>& aCategories,
            const TPosLmItemId& aLandmarkId );

        /**
        * Fetches all landmark IDs from the database. Sorts the IDs by landmark
        * name if specified.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aUseSort Whether to use sort or not.
        * @param[in] aSortDescending If sort is used, ETrue for descending order,
        *   otherwise ascending order.
        * @param[out] aLandmarkIds The fetched landmark IDs.
        */
        IMPORT_C static void GetAllLandmarkIdsL(
            CPosLmLocalDbAccess& aDbAccess,
            TBool aUseSort,
            TBool aSortDescending,
            RArray<TPosLmItemId>& aLandmarkIds );

        /**
        * Checks whether a landmark exists or not.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aLandmarkId A landmark id.
        */
        IMPORT_C static TBool LandmarkExistsL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aLandmarkId );

        /**
        * Removes a specified category from a specified landmark.
        * @param[in] aDbAccess A database access.
        * @param[in] aCategoryId A category.
        * @param[in] aLandmarkId A landmark.
        */
        IMPORT_C static void RemoveCategoryFromLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aCategoryId,
            TPosLmItemId aLandmarkId );

        /**
        * Checks whether the specified landmark has the specified category.
        *
        * @param[in] aDbAccess A database access.
        * @param[in] aCategoryId A category id.
        * @param[in] aLandmarkId A landmark id.
        * @return ETrue if the specified landmark has the specified category,
        *   otherwise EFalse.
        */
        IMPORT_C static TBool HasLandmarkSpecifiedCategoryL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aCategoryId,
            TPosLmItemId aLandmarkId );

        /** Returns total number of landmark rows */
        IMPORT_C static TUint TotalLandmarkCountL( CPosLmLocalDbAccess& aDbAccess );
    
        /**
         * Reads a landmark from the database. The landmark id must be set. The
         * id specifies which landmark to read.
         *
         * Leaves with @p KErrNotFound if not found.
         *
         * @param[in] aView View to landmark table, positioned
         *      to landmark's record
         * @param[in] aDbAccess A database access.
         * @param[in,out] aLandmark A landmark with the ID set.
         */
        IMPORT_C static void ReadLandmarkL(
            RDbRowSet& aView,
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark );

    private:

	    static void ReadLandmarkL(
	        CPosLmLocalDbAccess& aDbAccess,
	        CPosLandmark& aLandmark,
	        CPosLandmark::TAttributes aAttributes,
	        TBool aHasRequestedFields,
	        const RArray<TUint>& aRequestedFields );
    
    	static void ReadLandmarkL(
            RDbRowSet& aView,
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark,
            CPosLandmark::TAttributes aAttributes,
            TBool aHasRequestedFields,
            const RArray<TUint>& aRequestedFields );

        static void AddBasicLandmarkInfoL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark );

        static void AddFieldsL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark );

        static void AddCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark );

        static void ReadPositionL(
        	RDbRowSet& aView,
            CPosLandmark& aLandmark );

        static void ReadCoverageRadiusL(
        	RDbRowSet& aView,
            CPosLandmark& aLandmark );

        /** Reads landmark name from current row in given rowset
         *  and fills it to given landmark */
        static void ReadNameL(
            RDbRowSet& aView,
            CPosLandmark& aLandmark );

        static void ReadDescriptionL(
        	RDbRowSet& aView,
            CPosLandmark& aLandmark );

        static void ReadIconL(
            CPosLmLocalDbAccess& aDbAccess,
            RDbRowSet& aView,
            CPosLandmark& aLandmark );

        static void ReadFieldsL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark,
            TBool aHasRequestedFields,
            const RArray<TUint>& aRequestedFields );

        static void ReadCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            CPosLandmark& aLandmark );

        static void UpdateCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark,
            const CPosLandmark& aOldLandmark );

        static void UpdateFieldsL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark,
            const CPosLandmark& aOldLandmark );

        static void UpdateIconAndBasicLandmarkInfoL(
            CPosLmLocalDbAccess& aDbAccess,
            const CPosLandmark& aLandmark,
            const CPosLandmark& aOldLandmark );

        static TBool CheckIfIconAndBasicInfoAreEqualL(
            const CPosLandmark& aLandmark,
            const CPosLandmark& aLandmarkInDb );

        static void RemoveNonExistingCategoriesL(
            CPosLmLocalDbAccess& aDbAccess,
            RArray<TPosLmItemId>& aCategoryIdArray );

        static void RemoveCategoriesFromLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TPosLmItemId>& aCategories,
            const TPosLmItemId& aLandmarkId );

        static void AddFieldsToLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TUint>& aFieldIds,
            const CPosLandmark& aLandmark );

        static void UpdateFieldsForLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TUint>& aFieldIds,
            const CPosLandmark& aLandmark );

        static void RemoveFieldsFromLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            const RArray<TUint>& aFieldIds,
            const CPosLandmark& aLandmark );

        static void InsertFieldL(
            RDbView& view,
            const TPositionFieldId& aFieldId,
            const CPosLandmark& aLandmark );

        static void SetPositionL(
            RDbView& aView,
            const CPosLandmark& aLandmark );

        static void SetCoverageRadiusL(
            RDbView& aView,
            const CPosLandmark& aLandmark );

        static void SetLandmarkNameL(
            RDbView& aView,
            const CPosLandmark& aLandmark );

        static void SetLandmarkDescriptionL(
            RDbView& aView,
            const CPosLandmark& aLandmark );

        static void SetIconL(
            RDbView& aView,
            const CPosLandmark& aLandmark,
            const TUint32 aIconPathId );

        static TBool ReadIconPathIdL(
            CPosLmLocalDbAccess& aDbAccess,
            const TPosLmItemId& aId,
            TUint32& aIconPathId );

        static void RemoveAllCategoriesFromLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aLandmarkId );

        static void RemoveAllFieldsFromLandmarkL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aLandmarkId );

        static TBool RemoveBasicLandmarkInfoL(
            CPosLmLocalDbAccess& aDbAccess,
            TPosLmItemId aLandmarkId,
            TUint32& aIconPathId );
        
        static void OpenLandmarkViewLC(
            TPosLmItemId aLandmarkId,
            RDbView& aView,
            CPosLmLocalDbAccess& aDbAccess );

    };

#endif      // POSLMLANDMARKHANDLER_H

// End of File

