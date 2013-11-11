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
* Description: Class for specifying a sort preference that can be used both for landmark
*  sorting and landmark category sorting.
*
*
*/


// INCLUDE FILES
#include <EPos_TPosLmSortPref.h>
#include "EPos_TPosLmLocalSortPref.h"

// LOCAL FUNCTION PROTOTYPES

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::TPosLmLocalSortPref
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TPosLmLocalSortPref::TPosLmLocalSortPref()
:   iSortType(ENoSorting),
    iSortOrder(ESortAscending),
    iLmSortAttribute(CPosLandmark::ENoAttribute)
    {
    }

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::TPosLmLocalSortPref
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TPosLmLocalSortPref::TPosLmLocalSortPref(
    const TPosLmSortPref& aSortPref)
:   iSortType(ELandmarkSorting),
    iSortOrder(ESortAscending),
    iLmSortAttribute(aSortPref.LandmarkAttributeToSortBy())
    {
    if (aSortPref.SortOrder() == TPosLmSortPref::EDescending)
        {
        iSortOrder = ESortDescending;
        }
    }

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::TPosLmLocalSortPref
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TPosLmLocalSortPref::TPosLmLocalSortPref(
    CPosLmCategoryManager::TCategorySortPref aSortPref)
:   iSortType(ECategorySorting),
    iSortOrder(ESortAscending),
    iLmSortAttribute(CPosLandmark::ENoAttribute)
    {
    if (aSortPref == CPosLmCategoryManager::ECategorySortOrderNone)
        {
        iSortType = ENoSorting;
        }
    else if (aSortPref ==
        CPosLmCategoryManager::ECategorySortOrderNameAscending)
        {
        iSortOrder = ESortAscending;
        }
    else
        {
        iSortOrder = ESortDescending;
        }
    }

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::SortType
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPosLmLocalSortPref::TSortType TPosLmLocalSortPref::SortType() const
    {
    return iSortType;
    }

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::SortOrder
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPosLmLocalSortPref::TSortOrder TPosLmLocalSortPref::SortOrder() const
    {
    return iSortOrder;
    }

// -----------------------------------------------------------------------------
// TPosLmLocalSortPref::LandmarkAttributeToSortBy
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmark::TAttributes TPosLmLocalSortPref::LandmarkAttributeToSortBy() const
    {
    return iLmSortAttribute;
    }

//  End of File
