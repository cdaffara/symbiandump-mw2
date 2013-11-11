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
* Description: Operation for adding a category to a set of landmarks.
*
*
*/


// INCLUDE FILES
#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    "EPos_CPosLmLocalAddCatToLmOp.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_LocalLandmarks.h"

// CONSTANTS
const TInt KCategoryPosition = 0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::CPosLmLocalAddCatToLmOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalAddCatToLmOp::CPosLmLocalAddCatToLmOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmLocalModifyOp(aDb)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalAddCatToLmOp::ConstructL(
    RArray<TPosLmItemId>& aLandmarkIdArray,
    TPosLmItemId aCategoryId)
    {
    BaseConstructL();

    if (aLandmarkIdArray.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    ReadInfoFromResourceFileL(R_POS_LM_LOCAL_ADD_CAT_TO_LMS_OP);

    for (TInt i = 0; i < aLandmarkIdArray.Count(); i++)
        {
        User::LeaveIfError(iLandmarkIdArray.Append(aLandmarkIdArray[i]));
        }

    User::LeaveIfError(iCategory.Append(aCategoryId));

    User::LeaveIfError(PosLmCategoryHandler::FindCategoryL(
        *iDb->DatabaseAccess(), iCategory[KCategoryPosition]));
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalAddCatToLmOp* CPosLmLocalAddCatToLmOp::NewL(
    CPosLmLocalDatabase& aDb,
    TPosLmItemId aCategoryId,
    RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    CPosLmLocalAddCatToLmOp* self = new(ELeave) CPosLmLocalAddCatToLmOp(aDb);
    CleanupStack::PushL(self);
    self->ConstructL(aLandmarkIdArray, aCategoryId);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalAddCatToLmOp::~CPosLmLocalAddCatToLmOp()
    {
    RollbackAndGenerateEventIfNeeded(Progress());

    iLandmarkIdArray.Close();
    iCategory.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::DoNextStepPreL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalAddCatToLmOp::DoNextStepPreL(
    TReal32& aProgress)
    {
    for (TInt i = 0; i < iNoOfSubOperations &&
        iCurrent < iLandmarkIdArray.Count(); i++)
        {
        if (PosLmLandmarkHandler::LandmarkExistsL(
             *iDb->DatabaseAccess(), iLandmarkIdArray[iCurrent])  &&
            !PosLmLandmarkHandler::HasLandmarkSpecifiedCategoryL(
             *iDb->DatabaseAccess(), iCategory[KCategoryPosition],
             iLandmarkIdArray[iCurrent]))
            {
            PosLmLandmarkHandler::AddCategoriesToLandmarkL(
                *iDb->DatabaseAccess(), iCategory,
                iLandmarkIdArray[iCurrent]);
            }

        iCurrent++;
        }

    // Set progress
    aProgress = TReal32(iCurrent)/iLandmarkIdArray.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::DoNextStepPostL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalAddCatToLmOp::DoNextStepPostL(
    TReal32& aProgress)
    {
    if (iCurrent < iLandmarkIdArray.Count())
        {
        return ETrue;
        }

    User::LeaveIfError(GenerateEventIfNeeded(aProgress));
    iStatusFlag = KErrNone;
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::Step
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalAddCatToLmOp::Step()
    {
    return 1.0f/iLandmarkIdArray.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::CheckResourceValue
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalAddCatToLmOp::CheckResourceValue(
    TInt /*aResourceId*/,
    TInt aNoOfSubOperations)
    {
    __ASSERT_ALWAYS(aNoOfSubOperations > 0, Panic(KPosLandmarksClientPanic,
        EPosInvalidValueSpecifiedInResourceFile));
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalAddCatToLmOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    RollbackAndGenerateEventIfNeeded(Progress());
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::RollbackAndGenerateEventIfNeeded
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalAddCatToLmOp::RollbackAndGenerateEventIfNeeded(
    TReal32 aProgress)
    {
    RollbackIfNeeded();

    GenerateEventIfNeeded(aProgress);

    // Reset flag: rollback and event generation should not be done twice.
    iStatusFlag = KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalAddCatToLmOp::GenerateEventIfNeeded
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalAddCatToLmOp::GenerateEventIfNeeded(
    TReal32 aProgress)
    {
    if (iStatusFlag != KErrNone && aProgress > 0)
        {
        return iDb->RegisterEvent(EPosLmEventLandmarkUnknownChanges);
        }
    return KErrNone;
    }

//  End of File
