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
* Description: Implementation of CPosLmDisplayItem class
*
*
*/


// INCLUDE FILES
#include    <EPos_CPosLandmark.h>
#include    <EPos_CPosLandmarkCategory.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmDisplayItem.h"

// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::CPosLmDisplayItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDisplayItem::CPosLmDisplayItem(
    CPosLandmark* aLandmark,
    TUint aDatabaseIndex)
:   CBase(),
    iDisplayItemType(ELandmarkItem),
    iLandmark(aLandmark),
    iDatabaseIndex(aDatabaseIndex)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::CPosLmDisplayItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDisplayItem::CPosLmDisplayItem(
    CPosLandmarkCategory* aCategory,
    TUint aDatabaseIndex)
:   CBase(),
    iDisplayItemType(ECategoryItem),
    iCategory(aCategory),
    iDatabaseIndex(aDatabaseIndex)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem* CPosLmDisplayItem::NewL(
    CPosLandmark* aLandmark,
    TUint aDatabaseIndex)
    {
    CPosLmDisplayItem* self = new (ELeave) CPosLmDisplayItem(
        aLandmark, aDatabaseIndex);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmLandmarkDisplayItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem* CPosLmDisplayItem::NewL(
    CPosLandmarkCategory* aCategory,
    TUint aDatabaseIndex)
    {
    CPosLmDisplayItem* self = new (ELeave) CPosLmDisplayItem(
        aCategory, aDatabaseIndex);
    return self;
    }

// Destructor
CPosLmDisplayItem::~CPosLmDisplayItem()
    {
    delete iLandmark;
    delete iCategory;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::DisplayItemType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDisplayItem::TDisplayItemType
CPosLmDisplayItem::DisplayItemType() const
    {
    return iDisplayItemType;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::DatabaseIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmDisplayItem::DatabaseIndex() const
    {
    return iDatabaseIndex;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::Landmark
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CPosLandmark& CPosLmDisplayItem::Landmark() const
    {
    __ASSERT_ALWAYS(iDisplayItemType == CPosLmDisplayItem::ELandmarkItem,
        Panic(KPosLandmarksClientPanic, EPosInvalidItemType));

    return *iLandmark;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::Category
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CPosLandmarkCategory& CPosLmDisplayItem::Category() const
    {
    __ASSERT_ALWAYS(iDisplayItemType == CPosLmDisplayItem::ECategoryItem,
        Panic(KPosLandmarksClientPanic, EPosInvalidItemType));

    return *iCategory;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::GetDistance
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDisplayItem::GetDistance(
    TReal32& aDistance)  const
    {
    if (iHasDistance)
        {
        aDistance = iDistance;
        return KErrNone;
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::SetDatabaseIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDisplayItem::SetDatabaseIndex(
    TUint aDatabaseIndex)
    {
    iDatabaseIndex = aDatabaseIndex;
    }

// -----------------------------------------------------------------------------
// CPosLmDisplayItem::SetDistance
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDisplayItem::SetDistance(
    TReal32 aDistance)
    {
    iHasDistance = ETrue;
    iDistance = aDistance;
    }

//  End of File
