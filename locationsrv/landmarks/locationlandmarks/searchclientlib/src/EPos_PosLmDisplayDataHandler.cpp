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
*
*/


#include    <EPos_LandmarksErrors.h>
#include    <EPos_CPosLmDisplayData.h>
#include    <EPos_CPosLmDisplayItem.h>
#include    "EPos_PosLmDisplayDataHandler.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDisplayDataHandler::InsertItemL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem )
    {
    InsertItemL( aDisplayData, aDisplayItem, aDisplayData.Count() ); // append to the end
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDisplayDataHandler::InsertItemInOrderL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem,
    TPosLmSortPref aSortPref)
    {
    __ASSERT_DEBUG(
        aDisplayItem->DisplayItemType() == CPosLmDisplayItem::ELandmarkItem,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    __ASSERT_DEBUG(
        aSortPref.LandmarkAttributeToSortBy() == CPosLandmark::ELandmarkName,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    if (aSortPref.SortOrder() == TPosLmSortPref::EAscending)
        {
        TLinearOrder<CPosLmDisplayItem>
            order(PosLmDisplayDataHandler::CompareLandmarkNameAsc);
        InsertItemInOrderL(aDisplayData, aDisplayItem, order);
        }
    else
        {
        TLinearOrder<CPosLmDisplayItem>
            order(PosLmDisplayDataHandler::CompareLandmarkNameDesc);
        InsertItemInOrderL(aDisplayData, aDisplayItem, order);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDisplayDataHandler::InsertItemInOrderL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem,
    CPosLmCategoryManager::TCategorySortPref aSortPref)
    {
    __ASSERT_DEBUG(
        aDisplayItem->DisplayItemType() == CPosLmDisplayItem::ECategoryItem,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    if (aSortPref == CPosLmCategoryManager::ECategorySortOrderNone)
        {
        InsertItemL(aDisplayData, aDisplayItem);
        }
    else if (aSortPref ==
        CPosLmCategoryManager::ECategorySortOrderNameAscending)
        {
        TLinearOrder<CPosLmDisplayItem>
            order(PosLmDisplayDataHandler::CompareCategoryNameAsc);
        InsertItemInOrderL(aDisplayData, aDisplayItem, order);
        }
    else
        {
        TLinearOrder<CPosLmDisplayItem>
            order(PosLmDisplayDataHandler::CompareCategoryNameDesc);
        InsertItemInOrderL(aDisplayData, aDisplayItem, order);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDisplayDataHandler::InsertInDistanceOrderL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem)
    {
    __ASSERT_DEBUG(
        aDisplayItem->DisplayItemType() == CPosLmDisplayItem::ELandmarkItem,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    TLinearOrder<CPosLmDisplayItem>
        order(PosLmDisplayDataHandler::CompareLandmarkDistance);
    InsertItemInOrderL(aDisplayData, aDisplayItem, order);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem* PosLmDisplayDataHandler::RemoveItem(
    CPosLmDisplayData& /*aDisplayData*/,
    TInt /*aItemIndex*/)
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem* PosLmDisplayDataHandler::CopyItemLC(
    CPosLmDisplayData& aDisplayData,
    TInt aItemIndex)
    {
    RPointerArray<CPosLmDisplayItem>& displayItems =
        aDisplayData.DisplayItems();

    __ASSERT_DEBUG(aItemIndex >= 0 && aItemIndex < displayItems.Count(),
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    CPosLmDisplayItem* displayItem = displayItems[aItemIndex];
    TUint dbIndex = displayItem->DatabaseIndex();

    CPosLmDisplayItem* copyItem;
    if (displayItem->DisplayItemType() == CPosLmDisplayItem::ELandmarkItem)
        {
        CPosLandmark* lm = CPosLandmark::NewLC(displayItem->Landmark());
        copyItem = CPosLmDisplayItem::NewL(lm, dbIndex);
        CleanupStack::Pop(lm);

        TReal32 distance;
        if (displayItem->GetDistance(distance) == KErrNone)
            {
            copyItem->SetDistance(distance);
            }
        }
    else
        {
        CPosLandmarkCategory* category =
            CPosLandmarkCategory::NewLC(displayItem->Category());
        copyItem = CPosLmDisplayItem::NewL(category, dbIndex);
        CleanupStack::Pop(category);
        }

    CleanupStack::PushL(copyItem);
    return copyItem;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDisplayDataHandler::CompareLandmarkNameAsc(
    const CPosLmDisplayItem& aFirst,
    const CPosLmDisplayItem& aSecond)
    {
    TPtrC firstLandmarkName;
    TPtrC secondLandmarkName;

    TInt status = aFirst.Landmark().GetLandmarkName(firstLandmarkName);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    status = aSecond.Landmark().GetLandmarkName(secondLandmarkName);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    return firstLandmarkName.CompareC(secondLandmarkName);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDisplayDataHandler::CompareLandmarkNameDesc(
    const CPosLmDisplayItem& aFirst,
    const CPosLmDisplayItem& aSecond)
    {
    // Switch sign for ascending comparison.
    return -1 * CompareLandmarkNameAsc(aFirst, aSecond);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDisplayDataHandler::CompareLandmarkDistance(
    const CPosLmDisplayItem& aFirst,
    const CPosLmDisplayItem& aSecond)
    {
    TReal32 firstDistance;
    TReal32 secondDistance;

    TInt status = aFirst.GetDistance(firstDistance);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    status = aSecond.GetDistance(secondDistance);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    if (firstDistance < secondDistance)
        {
        return -1;
        }
    else if (firstDistance > secondDistance)
        {
        return 1;
        }
    else
        {
        return 0;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDisplayDataHandler::CompareCategoryNameAsc(
    const CPosLmDisplayItem& aFirst,
    const CPosLmDisplayItem& aSecond)
    {
    TPtrC firstCategoryName;
    TPtrC secondCategoryName;

    TInt status = aFirst.Category().GetCategoryName(firstCategoryName);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    status = aSecond.Category().GetCategoryName(secondCategoryName);
    __ASSERT_DEBUG(status == KErrNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    return firstCategoryName.CompareC(secondCategoryName);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDisplayDataHandler::CompareCategoryNameDesc(
    const CPosLmDisplayItem& aFirst,
    const CPosLmDisplayItem& aSecond)
    {
    // Switch sign for ascending comparison.
    return -1 * CompareCategoryNameAsc(aFirst, aSecond);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt PosLmDisplayDataHandler::FindItemIndex(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem)
    {
    return aDisplayData.DisplayItems().Find(aDisplayItem);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDisplayDataHandler::InsertItemL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem,
    TUint aIndex )
    {
    RPointerArray<CPosLmDisplayItem>& displayItems =
        aDisplayData.DisplayItems();
    RPointerArray<CPosLmDisplayItem>& newDisplayItems =
        aDisplayData.NewDisplayItems();

    if ( aIndex > displayItems.Count() )
        aIndex = displayItems.Count();
    
    displayItems.InsertL( aDisplayItem, aIndex );

    if ( aIndex > newDisplayItems.Count() )
        aIndex = newDisplayItems.Count();

    TInt appendStatus = newDisplayItems.Insert( aDisplayItem, aIndex );
    if ( appendStatus != KErrNone )
        {
        displayItems.Remove( aIndex );
        User::Leave( appendStatus );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDisplayDataHandler::InsertItemInOrderL(
    CPosLmDisplayData& aDisplayData,
    CPosLmDisplayItem* aDisplayItem,
    TLinearOrder<CPosLmDisplayItem>& aOrder)
    {
    RPointerArray<CPosLmDisplayItem>& displayItems =
        aDisplayData.DisplayItems();
    RPointerArray<CPosLmDisplayItem>& newDisplayItems =
        aDisplayData.NewDisplayItems();

    displayItems.InsertInOrderAllowRepeatsL(aDisplayItem, aOrder);

    TInt appendStatus =
        newDisplayItems.InsertInOrderAllowRepeats(aDisplayItem, aOrder);
    if (appendStatus != KErrNone)
        {
        displayItems.Remove(displayItems.Find(aDisplayItem));
        User::Leave(appendStatus);
        }
    }

