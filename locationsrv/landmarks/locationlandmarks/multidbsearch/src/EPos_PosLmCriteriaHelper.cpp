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
* Description: Helper class that validates a search criteria and creates a copy.
*
*
*/


// INCLUDE FILES
#include    <EPos_CPosLmSearchCriteria.h>
#include    <EPos_CPosLmAreaCriteria.h>
#include    <EPos_CPosLmTextCriteria.h>
#include    <EPos_CPosLmCompositeCriteria.h>
#include    <EPos_CPosLmCategoryCriteria.h>
#include    <EPos_CPosLmNearestCriteria.h>
#include    <EPos_CPosLmCatNameCriteria.h>
#include    "EPos_PosLmCriteriaHelper.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyL(
    const CPosLmSearchCriteria& aCriteria,
    TBool aIsLm,
    TBool& aUseDistanceSorting,
    TBool aIsTopCriteria)
    {
    CPosLmSearchCriteria* criteria = NULL;
    aUseDistanceSorting = EFalse;

    switch (aCriteria.CriteriaType())
        {
        case CPosLmSearchCriteria::ECriteriaArea:
            __ASSERT_ALWAYS(aIsLm, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmAreaCriteria&>(aCriteria));
            break;

        case CPosLmSearchCriteria::ECriteriaText:
            __ASSERT_ALWAYS(aIsLm, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmTextCriteria&>(aCriteria));
            break;

        case CPosLmSearchCriteria::ECriteriaComposite:
            __ASSERT_ALWAYS(aIsLm, User::Leave(KErrNotSupported));
            __ASSERT_ALWAYS(aIsTopCriteria, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmCompositeCriteria&>(aCriteria),
                aIsLm, aUseDistanceSorting);
            break;

        case CPosLmSearchCriteria::ECriteriaCategory:
            __ASSERT_ALWAYS(aIsLm, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmCategoryCriteria&>(aCriteria));
            break;

        case CPosLmSearchCriteria::ECriteriaFindNearest:
            __ASSERT_ALWAYS(aIsLm, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmNearestCriteria&>(aCriteria));
            aUseDistanceSorting = ETrue;
            break;

        case CPosLmSearchCriteria::ECriteriaCategoryByName:
            __ASSERT_ALWAYS(!aIsLm, User::Leave(KErrArgument));

            criteria = ValidateAndCopyLC(
                static_cast<const CPosLmCatNameCriteria&>(aCriteria));
            break;

        case CPosLmSearchCriteria::ECriteriaIdList:
            User::Leave(KErrArgument);

        default:
            User::Leave(KErrNotSupported);
        }

    CleanupStack::Pop(criteria);
    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmCompositeCriteria& aCriteria,
    TBool aIsLm,
    TBool& aUseDistanceSorting)
    {
    __ASSERT_ALWAYS(aCriteria.NumOfArguments() > 0, User::Leave(KErrArgument));

    CPosLmCompositeCriteria* criteria =
        CPosLmCompositeCriteria::NewLC(aCriteria.CompositionType());

    TBool componentUseDistanceSorting;
    TInt noOfcomponentUseDistanceSorting(0);

    for (TUint i = 0; i < aCriteria.NumOfArguments(); i++)
        {
        CPosLmSearchCriteria* component =
            ValidateAndCopyL(aCriteria.Argument(i), aIsLm,
                             componentUseDistanceSorting, EFalse);

        CleanupStack::PushL(component);
        User::LeaveIfError(criteria->AddArgument(component));
        CleanupStack::Pop(component);

        if (componentUseDistanceSorting)
            {
            noOfcomponentUseDistanceSorting++;
            }

        }

    // Composite with none or more than one distance sorting criteria means
    // no distance sorting.
    aUseDistanceSorting = (noOfcomponentUseDistanceSorting == 1);

    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmAreaCriteria& aCriteria)
    {
    TReal64 southLatitude;
    TReal64 northLatitude;
    TReal64 westLongitude;
    TReal64 eastLongitude;
    aCriteria.GetSearchArea(southLatitude, northLatitude,
        westLongitude, eastLongitude);

    CPosLmAreaCriteria* criteria = CPosLmAreaCriteria::NewLC(
        southLatitude, northLatitude,
        westLongitude, eastLongitude);

    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmTextCriteria& aCriteria)
    {
    __ASSERT_ALWAYS(aCriteria.Text().Length() > 0, User::Leave(KErrArgument));

    CPosLmTextCriteria* criteria = CPosLmTextCriteria::NewLC();

    criteria->SetTextL(aCriteria.Text());
    criteria->SetAttributesToSearch(aCriteria.AttributesToSearch());

    RArray<TUint> positionFields;
    CleanupClosePushL(positionFields);
    aCriteria.GetPositionFieldsToSearchL(positionFields);
    criteria->SetPositionFieldsToSearchL(positionFields);
    CleanupStack::PopAndDestroy(&positionFields);

    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmCategoryCriteria& aCriteria)
    {
    __ASSERT_ALWAYS(aCriteria.CategoryItemId() == KPosLmNullItemId,
        User::Leave(KErrArgument));

    CPosLmCategoryCriteria* criteria = CPosLmCategoryCriteria::NewLC();
    if (aCriteria.CategoryName().Length() > 0)
        {
        criteria->SetCategoryNameL(aCriteria.CategoryName());
        }
    else if (aCriteria.GlobalCategory() != KPosLmNullGlobalCategory)
        {
        criteria->SetGlobalCategory(aCriteria.GlobalCategory());
        }

    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmNearestCriteria& aCriteria)
    {
    TCoordinate coordinate;
    aCriteria.GetCoordinate(coordinate);

    CPosLmNearestCriteria* criteria = CPosLmNearestCriteria::NewLC(
        coordinate,
        aCriteria.UseCoverageRadius());
    criteria->SetMaxDistance(aCriteria.MaxDistance());

    return criteria;
    }

// -----------------------------------------------------------------------------
// PosLmCriteriaHelper::ValidateAndCopyLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSearchCriteria* PosLmCriteriaHelper::ValidateAndCopyLC(
    const CPosLmCatNameCriteria& aCriteria)
    {
    __ASSERT_ALWAYS(aCriteria.SearchPattern().Length() > 0,
        User::Leave(KErrArgument));

    CPosLmCatNameCriteria* criteria = CPosLmCatNameCriteria::NewLC();
    criteria->SetSearchPatternL(aCriteria.SearchPattern());

    return criteria;
    }

//  End of File
