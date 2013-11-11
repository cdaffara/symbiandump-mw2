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
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmMultiDbSortPref.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::CPosLmMultiDbSortPref
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CPosLmMultiDbSortPref::CPosLmMultiDbSortPref(TSortType aSortType)
    : iSortType(aSortType)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSortPref::ConstructL(
    const TPosLmSortPref& aSortPref)
    {
    CPosLandmark::TAttributes attribute = aSortPref.LandmarkAttributeToSortBy();

    __ASSERT_ALWAYS(attribute == CPosLandmark::ELandmarkName,
        User::Leave(KErrNotSupported));

    iLmSortPref = new (ELeave) TPosLmSortPref(attribute, aSortPref.SortOrder());
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSortPref::ConstructL(
    CPosLmCategoryManager::TCategorySortPref aSortPref)
    {
    iCatSortPref = new (ELeave) CPosLmCategoryManager::TCategorySortPref(
        aSortPref);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSortPref* CPosLmMultiDbSortPref::NewL()
    {
    CPosLmMultiDbSortPref* self = new (ELeave)
        CPosLmMultiDbSortPref(ELandmarkNoSorting);

    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSortPref* CPosLmMultiDbSortPref::NewL(
    const TPosLmSortPref& aSortPref)
    {
    CPosLmMultiDbSortPref* self = new (ELeave)
        CPosLmMultiDbSortPref(ELandmarkSorting);

    CleanupStack::PushL(self);
    self->ConstructL(aSortPref);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSortPref* CPosLmMultiDbSortPref::NewL(
    CPosLmCategoryManager::TCategorySortPref aSortPref)
    {
    CPosLmMultiDbSortPref* self = new (ELeave)
        CPosLmMultiDbSortPref(ECategorySorting);

    CleanupStack::PushL(self);
    self->ConstructL(aSortPref);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmMultiDbSortPref::~CPosLmMultiDbSortPref()
    {
    delete iLmSortPref;
    delete iCatSortPref;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::SetDistanceSorting
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSortPref::SetDistanceSorting()
    {
    iSortType = ELandmarkDistanceSorting;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::SortType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSortPref::TSortType CPosLmMultiDbSortPref::SortType() const
    {
    return iSortType;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::LandmarkSortPref
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TPosLmSortPref& CPosLmMultiDbSortPref::LandmarkSortPref() const
    {
    return *iLmSortPref;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSortPref::CategorySortPref
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmCategoryManager::TCategorySortPref
CPosLmMultiDbSortPref::CategorySortPref() const
    {
    return *iCatSortPref;
    }

//  End of File
