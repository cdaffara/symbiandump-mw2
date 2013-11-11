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
* Description: Class for specifying sort preference.
*
*
*/


// INCLUDE FILES
#include "EPos_TPosLmSortPref.h"
#include "EPos_LandmarksErrors.h"

// LOCAL FUNCTION PROTOTYPES
void VerfifyOnlySingleAttributeSet(
    CPosLandmark::TAttributes aLandmarkAttribute);


// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// VerfifyOnlySingleAttributeSet
// This function will panic if none or several landmark attributes are set.
// -----------------------------------------------------------------------------
//
void VerfifyOnlySingleAttributeSet(
    CPosLandmark::TAttributes aLandmarkAttribute)
    {
    TBool singleAttributeFound = EFalse;

    if ((aLandmarkAttribute == CPosLandmark::ELandmarkName) ||
        (aLandmarkAttribute == CPosLandmark::EPosition) ||
        (aLandmarkAttribute == CPosLandmark::ECoverageRadius) ||
        (aLandmarkAttribute == CPosLandmark::ECategoryInfo) ||
        (aLandmarkAttribute == CPosLandmark::EIcon) ||
        (aLandmarkAttribute == CPosLandmark::EDescription))
        {
        singleAttributeFound = ETrue;
        }

    if (!singleAttributeFound)
        {
        Panic(KPosLandmarksClientPanic, EPosNoneOrMultipleLandmarkAttributeSet);
        }
    }

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C TPosLmSortPref::TPosLmSortPref(
    CPosLandmark::TAttributes aLandmarkAttribute,
    TSortOrder aSortOrder)
    :   iSortOrder(aSortOrder),
        iLandmarkAttribute(aLandmarkAttribute)
    {
    __ASSERT_ALWAYS(aSortOrder == EAscending || aSortOrder == EDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    VerfifyOnlySingleAttributeSet(aLandmarkAttribute);

    for (TInt i = 0; i < 16; i++)
        {
        iUnusedData[i] = 0;
        }
    }

// ---------------------------------------------------------
// TPosLmSortPref::SortOrder
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmSortPref::TSortOrder TPosLmSortPref::SortOrder() const
    {
    return iSortOrder;
    }

// ---------------------------------------------------------
// TPosLmSortPref::LandmarkAttributeToSortBy
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPosLandmark::TAttributes
    TPosLmSortPref::LandmarkAttributeToSortBy() const
    {
    return iLandmarkAttribute;
    }

// ---------------------------------------------------------
// TPosLmSortPref::SetSortByLandmarkAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPosLmSortPref::SetSortByLandmarkAttribute(
    CPosLandmark::TAttributes aLandmarkAttribute,
    TSortOrder aSortOrder)
    {
    __ASSERT_ALWAYS(aSortOrder == EAscending || aSortOrder == EDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    VerfifyOnlySingleAttributeSet(aLandmarkAttribute);

    iLandmarkAttribute = aLandmarkAttribute;
    iSortOrder = aSortOrder;
    }

//  End of File
