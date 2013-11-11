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
* Description: Operation for removing categories from a database.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmresourcereader.h>
#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    "EPos_CPosLmLocalRemoveCategoriesOp.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_LocalLandmarks.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::CPosLmLocalRemoveCategoriesOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveCategoriesOp::CPosLmLocalRemoveCategoriesOp(
    CPosLmLocalDatabase& aDb,
    TRemoveType aRemoveType) :
    CPosLmLocalModifyOp(aDb),
    iRemoveType(aRemoveType)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::ConstructL(
    TPosLmItemId aCategoryId)
    {
    BaseConstructL();

    // Set to 1 for asynchronous operation to loop correct.
    iNoOfSubOperations = 1;

    User::LeaveIfError(iIdArray.Append(aCategoryId));
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::ConstructL(
    const RArray<TPosLmItemId>& aCategoryIdArray)
    {
    BaseConstructL();

    ReadInfoFromResourceFileL(R_POS_LM_LOCAL_REMOVE_CATS_OP);

    if (aCategoryIdArray.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    for (TInt i = 0; i < aCategoryIdArray.Count(); i++)
        {
        User::LeaveIfError(iIdArray.Append(aCategoryIdArray[i]));
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::ConstructL(
    TPosLmItemId aCategoryId,
    RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    BaseConstructL();

    ReadInfoFromResourceFileL(R_POS_LM_LOCAL_REMOVE_CAT_FROM_LMS_OP);

    if (aLandmarkIdArray.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    // Make sure that landmark category exists
    User::LeaveIfError(
        PosLmCategoryHandler::FindCategoryL(
            *iDb->DatabaseAccess(), aCategoryId));
    iCategoryId = aCategoryId;

    for (TInt i = 0; i < aLandmarkIdArray.Count(); i++)
        {
        User::LeaveIfError(iIdArray.Append(aLandmarkIdArray[i]));
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveCategoriesOp* CPosLmLocalRemoveCategoriesOp::NewL(
    CPosLmLocalDatabase& aDb,
    TPosLmItemId aCategoryId)
    {
    CPosLmLocalRemoveCategoriesOp* self = new (ELeave)
        CPosLmLocalRemoveCategoriesOp(aDb, ERemoveSingleCategory);
    CleanupStack::PushL(self);
    self->ConstructL(aCategoryId);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveCategoriesOp* CPosLmLocalRemoveCategoriesOp::NewL(
    CPosLmLocalDatabase& aDb,
    const RArray<TPosLmItemId>& aCategoryIdArray)
    {
    CPosLmLocalRemoveCategoriesOp* self = new (ELeave)
        CPosLmLocalRemoveCategoriesOp(aDb,
        ERemoveMultipleCategories);
    CleanupStack::PushL(self);
    self->ConstructL(aCategoryIdArray);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalRemoveCategoriesOp* CPosLmLocalRemoveCategoriesOp::NewL(
    CPosLmLocalDatabase& aDb,
    TPosLmItemId aCategoryId,
    RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    CPosLmLocalRemoveCategoriesOp* self = new (ELeave)
        CPosLmLocalRemoveCategoriesOp(aDb,
        ERemoveCategoryFromLandmarks);
    CleanupStack::PushL(self);
    self->ConstructL(aCategoryId, aLandmarkIdArray);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalRemoveCategoriesOp::~CPosLmLocalRemoveCategoriesOp()
    {
    RollbackAndGenerateEventIfNeeded(Progress());

    iIdArray.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::DoNextStepPreL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::DoNextStepPreL(
    TReal32& aProgress)
    {
    for (TInt i = 0; i < iNoOfSubOperations &&
        iCurrent < iIdArray.Count(); i++)
        {
        if (iRemoveType == ERemoveSingleCategory ||
            iRemoveType == ERemoveMultipleCategories)
            {
            PosLmCategoryHandler::RemoveCategoryL(*iDb->DatabaseAccess(),
                iIdArray[iCurrent]);
            }
        else
            {
            PosLmLandmarkHandler::RemoveCategoryFromLandmarkL(
                *iDb->DatabaseAccess(), iCategoryId,  iIdArray[iCurrent]);
            }

        iCurrent++;
        }

    // Set progress
    aProgress = TReal32(iCurrent)/iIdArray.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::DoNextStepPostL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalRemoveCategoriesOp::DoNextStepPostL(
    TReal32& aProgress)
    {
    if (iCurrent < iIdArray.Count())
        {
        return ETrue;
        }

    // Operation completed
    User::LeaveIfError(GenerateEventIfNeeded(aProgress));
    iStatusFlag = KErrNone;
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::Step
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalRemoveCategoriesOp::Step()
    {
    return 1.0f/iIdArray.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::CheckResourceValue
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::CheckResourceValue(
    TInt aResourceId,
    TInt aNoOfSubOperations)
    {
    switch (aResourceId)
        {
        case R_POS_LM_LOCAL_REMOVE_CATS_OP:
        case R_POS_LM_LOCAL_REMOVE_CAT_FROM_LMS_OP:
            __ASSERT_ALWAYS(aNoOfSubOperations > 0, Panic(
                KPosLandmarksClientPanic,
                EPosInvalidValueSpecifiedInResourceFile));
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveCategoriesOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    RollbackAndGenerateEventIfNeeded(Progress());
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveLandmarksOp::RollbackAndGenerateEventIfNeeded
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalRemoveCategoriesOp::RollbackAndGenerateEventIfNeeded(
    TReal32 aProgress)
    {
    RollbackIfNeeded();

    GenerateEventIfNeeded(aProgress);

    // Reset flag: rollback and event generation should not be done twice.
    iStatusFlag = KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalRemoveLandmarksOp::GenerateEventIfNeeded
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalRemoveCategoriesOp::GenerateEventIfNeeded(
    TReal32 aProgress)
    {
    if (iStatusFlag != KErrNone && aProgress > 0)
        {
        if (iRemoveType == ERemoveSingleCategory ||
            iRemoveType == ERemoveMultipleCategories)
            {
            // Only one category has been removed, so a more specific event
            // can be reported.
            if (iCurrent == 1)
                {
                return iDb->RegisterEvent(
                    EPosLmEventCategoryDeleted, iIdArray[0]);
                }

            // More than one category has been removed. A general event is
            // reported.
            if (iCurrent > 0)
                {
                return iDb->RegisterEvent(EPosLmEventCategoryUnknownChanges);
                }

            // Nothing has happened. Do not report event.
            return KErrNone;
            }

        // Removing categories from landmark.
        return iDb->RegisterEvent(EPosLmEventLandmarkUnknownChanges);
        }
    // Nothing has happened. Do not report event.
    return KErrNone;
    }

//  End of File
