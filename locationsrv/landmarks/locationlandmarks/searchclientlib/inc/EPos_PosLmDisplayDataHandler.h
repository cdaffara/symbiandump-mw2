/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Provides static exported functions for modifying a display data
*
*/


#ifndef POSLMDISPLAYDATAHANDLER_H
#define POSLMDISPLAYDATAHANDLER_H

#include <e32std.h>
#include <EPos_TPosLmSortPref.h>
#include <EPos_CPosLmCategoryManager.h>

class CPosLmDisplayData;
class CPosLmDisplayItem;

/*
* Class containing static exported functions for modifying a display data
* needed by the implementation of components in the Landmarks subsystem.
*/
class PosLmDisplayDataHandler
    {

    public:  // New functions

        /*
        * Inserts a displayable item into a display data.
        * The item will be appended to the end of the array.
        *
        * @param[in,out] aDisplayData A display data.
        * @param[in] aDisplayItem A display item.
        */
        IMPORT_C static void InsertItemL(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem );

        /*
        * Inserts a displayable item into a display data in order
        * according to the sort preference.
        *
        * @param[in,out] aDisplayData A display data.
        * @param[in] aDisplayItem A display item.
        * @param[in] aSortPref A sort preference.
        */
        IMPORT_C static void InsertItemInOrderL(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem,
            TPosLmSortPref aSortPref );

        /*
        * Inserts a displayable item into a display data in order
        * according to the sort preference.
        *
        * InsertItemL() will be called if the sort preference is set to
        * CPosLmCategoryManager::ECategorySortOrderNone.
        *
        * @param[in,out] aDisplayData A display data.
        * @param[in] aDisplayItem A display item.
        * @param[in] aSortPref A sort preference.
        */
        IMPORT_C static void InsertItemInOrderL(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem,
            CPosLmCategoryManager::TCategorySortPref aSortPref );

        /*
        * Inserts a displayable item into a display data in order
        * according to its distance.
        *
        * @param[in,out] aDisplayData A display data.
        * @param[in] aDisplayItem A display item.
        */
        IMPORT_C static void InsertInDistanceOrderL(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem );

        /*
        * Removes a displayable item from a display data.
        *
        * The ownership of the displayable item is transferred to the caller.
        *
        * @param[in,out] aDisplayData A display data.
        * @param[in] aItemIndex A displayable item index.
        */
        IMPORT_C static CPosLmDisplayItem* RemoveItem(
            CPosLmDisplayData& aDisplayData,
            TInt aItemIndex );

        /*
        * Copies a displayable item from a display data.
        *
        * The ownership of the displayable item is transferred to the caller.
        *
        * @param[in] aDisplayData A display data.
        * @param[in] aItemIndex A displayable item index.
        */
        IMPORT_C static CPosLmDisplayItem* CopyItemLC(
            CPosLmDisplayData& aDisplayData,
            TInt aItemIndex );

        /*
        * Function that compares two landmark display item objects
        * by name in ascending order.
        *
        * @param[in] aFirst The first item.
        * @param[in] aSecond The second item.
        * @return Zero if the objects are equal, a negative value if aFirst is
        *   less than aSecond and a positive value if aFirst is greater than
        *   aSecond.
        */
        static TInt CompareLandmarkNameAsc(
            const CPosLmDisplayItem& aFirst,
            const CPosLmDisplayItem& aSecond );

        /*
        * Function that compares two landmark display item objects
        * by name in descending order.
        *
        * @param[in] aFirst The first item.
        * @param[in] aSecond The second item.
        * @return Zero if the objects are equal, a negative value if aFirst is
        *   less than aSecond and a positive value if aFirst is greater than
        *   aSecond.
        */
        static TInt CompareLandmarkNameDesc(
            const CPosLmDisplayItem& aFirst,
            const CPosLmDisplayItem& aSecond );

        /*
        * Function that compares two landmark display item objects
        * by their distance.
        *
        * @param[in] aFirst The first item.
        * @param[in] aSecond The second item.
        * @return Zero if the objects are equal, a negative value if aFirst is
        *   less than aSecond and a positive value if aFirst is greater than
        *   aSecond.
        */
        static TInt CompareLandmarkDistance(
            const CPosLmDisplayItem& aFirst,
            const CPosLmDisplayItem& aSecond );

        /*
        * Function that compares two category display item objects
        * by name in ascending order.
        *
        * @param[in] aFirst The first item.
        * @param[in] aSecond The second item.
        * @return Zero if the objects are equal, a negative value if aFirst is
        *   less than aSecond and a positive value if aFirst is greater than
        *   aSecond.
        */
        static TInt CompareCategoryNameAsc(
            const CPosLmDisplayItem& aFirst,
            const CPosLmDisplayItem& aSecond );

        /*
        * Function that compares two category display item objects
        * by name in descending order.
        *
        * @param[in] aFirst The first item.
        * @param[in] aSecond The second item.
        * @return Zero if the objects are equal, a negative value if aFirst is
        *   less than aSecond and a positive value if aFirst is greater than
        *   aSecond.
        */
        static TInt CompareCategoryNameDesc(
            const CPosLmDisplayItem& aFirst,
            const CPosLmDisplayItem& aSecond );

        /*
        * Finds the item index of a displayable item in a display data.
        *
        * @param[in] aDisplayData A display data.
        * @param[in] aDisplayItem A display item.
        *
        * @return A displayable item index.
        */
        IMPORT_C static TInt FindItemIndex(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem );

        /*
         * Inserts a displayable item into a display data.
         * The item will be inserted at the specified position.
         *
         * @param[in,out] aDisplayData A display data.
         * @param[in] aDisplayItem A display item.
         * @param[in] aPosition The position in the display data to insert to.
         */
         IMPORT_C static void InsertItemL(
             CPosLmDisplayData& aDisplayData,
             CPosLmDisplayItem* aDisplayItem,
             TUint aPosition );

    private:

        static void InsertItemInOrderL(
            CPosLmDisplayData& aDisplayData,
            CPosLmDisplayItem* aDisplayItem,
            TLinearOrder<CPosLmDisplayItem>& aOrder
        );
    };

#endif      // POSLMDISPLAYDATAHANDLER_H

// End of File
