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
* Description: Operation for resetting global categories.
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    <epos_cposlmglobalcategoryreader.h>
#include    <epos_poslmcategoryhandler.h>
#include    <eposlmasyncops.rsg>
#include    <epos_cposlmlocaldbaccess.h>
#include    "EPos_CPosLmLocalResetGlobalCatOp.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_CPosLmLocalCategoryManager.h"
#include    "EPos_LocalLandmarks.h"

// CONSTANTS
const TReal32 KReadGlCat =              0.2f;
const TReal32 KResetGlCat =             0.5f;
const TReal32 KCheckGlCatToBeRemoved =  0.1f;
const TReal32 KRemoveNotInResource =    0.2f;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::CPosLmLocalResetGlobalCatOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalResetGlobalCatOp::CPosLmLocalResetGlobalCatOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmLocalModifyOp(aDb),
    iState(EReadGlCat)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::ConstructL()
    {
    BaseConstructL();
    ReadInfoFromResourceFileL(R_POS_LM_LOCAL_RESET_GLOBAL_CATEGORIES_OP);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalResetGlobalCatOp* CPosLmLocalResetGlobalCatOp::NewL(
    CPosLmLocalDatabase& aDb)
    {
    CPosLmLocalResetGlobalCatOp* self = new(ELeave) CPosLmLocalResetGlobalCatOp(
        aDb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalResetGlobalCatOp::~CPosLmLocalResetGlobalCatOp()
    {
    RollbackAndRegisterIfNeeded();
    iModifiedGlCats.Close();
    iToBeRemoved.Close();
    delete iGlCatReader;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::DoNextStepPreL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::DoNextStepPreL(
    TReal32& aProgress)
    {
    switch (iState)
        {
        case EReadGlCat:
            iGlCatReader = CPosLmGlobalCategoryReader::NewL();
            aProgress = KReadGlCat;
            break;
        case EResetGlCat:
            ResetCategoryPreL(aProgress);
            break;
        case ECheckGlCatToBeRemoved:
            // Find global categories to be removed (not in resource file)
            PosLmCategoryHandler::GetGlobalCategoriesL(*iDb->DatabaseAccess(),
                iModifiedGlCats, iToBeRemoved);
            iCurrent = 0; // Reset counter, so it can be reused
            aProgress = KReadGlCat + KResetGlCat + KCheckGlCatToBeRemoved;
            break;
        default: //ERemoveNotInResource
            RemoveCategoryNotInResourceL(aProgress);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::DoNextStepPostL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalResetGlobalCatOp::DoNextStepPostL(
    TReal32& /*aProgress*/)
    {
    switch (iState)
        {
        case EReadGlCat:
            iState = EResetGlCat;
            return ETrue;
        case EResetGlCat:
            if (iGlCatReader->Count() == 0 || iCurrent == iGlCatReader->Count())
                {
                // Done with reset, so step into next step.
                iState = ECheckGlCatToBeRemoved;
                }
            return ETrue;
        case ECheckGlCatToBeRemoved:
            iState = ERemoveNotInResource;
            return ETrue;
        default: //ERemoveNotInResource
            if (iToBeRemoved.Count() == 0 || iCurrent == iToBeRemoved.Count())
                {
                // Operation done
                iStatusFlag = KErrNone;

                User::LeaveIfError(
                    iDb->RegisterEvent(EPosLmEventCategoryUnknownChanges));

                return EFalse;
                }
            return ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::Step
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalResetGlobalCatOp::Step()
    {
    TReal32 step;

    switch (iState)
        {
        case EReadGlCat:
            step = KReadGlCat;
            break;
        case EResetGlCat:
            if (iGlCatReader->Count() > 0)
                {
                step = KResetGlCat/iGlCatReader->Count();
                }
            else
                {
                step = KResetGlCat;
                }
            break;
        case ECheckGlCatToBeRemoved:
            step = KCheckGlCatToBeRemoved;
            break;
        default: //ERemoveNotInResource
            if (iToBeRemoved.Count() > 0)
                {
                step = KCheckGlCatToBeRemoved/iToBeRemoved.Count();
                }
            else
                {
                step = KCheckGlCatToBeRemoved;
                }
            break;
        }

    return step;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::CheckResourceValue
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::CheckResourceValue(
    TInt /*aResourceId*/,
    TInt aNoOfSubOperations)
    {
    __ASSERT_ALWAYS(aNoOfSubOperations > 0, Panic(KPosLandmarksClientPanic,
        EPosInvalidValueSpecifiedInResourceFile));
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    RollbackAndRegisterIfNeeded();
    iState = EError;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::ResetCategoryPreL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::ResetCategoryPreL(
    TReal32& aProgress)
    {
    for (TInt i = 0; i < iNoOfSubOperations &&
        iCurrent < iGlCatReader->Count(); i++)
        {
        ResetCategoryL(iCurrent++);
        }

    if (iGlCatReader->Count() != 0)
        {
        aProgress = KReadGlCat +
            (TReal32(iCurrent)/iGlCatReader->Count()) * KResetGlCat;
        }
    else
        {
        aProgress = KReadGlCat + KResetGlCat;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::RemoveCategoryNotInResourceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::RemoveCategoryNotInResourceL(
    TReal32& aProgress)
    {
    // Remove global categories not in the resource file
    if (iToBeRemoved.Count() > 0)
        {
        PosLmCategoryHandler::RemoveCategoryL(*iDb->DatabaseAccess(),
            iToBeRemoved[iCurrent++]);
        }

    if (iToBeRemoved.Count() != 0)
        {
        aProgress = KResetGlCat + KResetGlCat + KCheckGlCatToBeRemoved +
            (TReal32(iCurrent)/iToBeRemoved.Count()) * KRemoveNotInResource;
        }
    else
        {
        aProgress = KReadGlCat + KResetGlCat + KCheckGlCatToBeRemoved +
            KRemoveNotInResource;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::ResetCategoryL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::ResetCategoryL(
    TInt aIndex)
    {
    CPosLandmarkCategory* glCategory =
        iGlCatReader->GlobalCategoryLC(aIndex);

    PosLmCategoryHandler::ResetGlobalCategoryL(*iDb->DatabaseAccess(),
        *glCategory);

    User::LeaveIfError(iModifiedGlCats.Append(glCategory->GlobalCategory()));

    CleanupStack::PopAndDestroy(glCategory);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalResetGlobalCatOp::RollbackAndRegisterIfNeeded
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalResetGlobalCatOp::RollbackAndRegisterIfNeeded()
    {
    RollbackIfNeeded();

    // If progress is not made or an error has occured
    // previously, an event should not be registred
    if (Progress() > KReadGlCat && iState != EError)
        {
        iDb->RegisterEvent(EPosLmEventCategoryUnknownChanges);
        }

    // Reset flag: rollback and event generation should not be done twice.
    iStatusFlag = KErrNone;
    }

//  End of File
