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
* Description: Operation for importing landmarks to a database.
*
*
*/


#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <EPos_CPosLandmarkParser.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_cposlmglobalcategoryreader.h>
#include    <epos_cposlmdiskutilities.h>

#include    "epos_cposlmlocaldatabase.h"
#include    "epos_cposlmlocaldbaccess.h"
#include    "EPos_LocalLandmarks.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"
#include    "EPos_CPosLmLocalInternalOpActive.h"
#include    "EPos_CPosLmLocalImportOp.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalImportOp::CPosLmLocalImportOp(
    CPosLmLocalDatabase& aDb,
    CPosLandmarkParser& aLandmarkParser,
    CPosLandmarkDatabase::TTransferOptions aTransferOptions,
    CPosLmDiskUtilities& aDiskUtils)
:   CPosLmLocalModifyOp(aDb),
    iLandmarkParser(&aLandmarkParser),
    iTransferOptions(aTransferOptions),
    iImportState(EImportParseLm),
    iParserOpStatus(KPosLmOperationNotComplete),
    iDiskUtils(&aDiskUtils)
    {
    iUsesServerData = ETrue; // updates lm name index
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ConstructL()
    {
    CPosLandmarkDatabase::TTransferOptions invalidOptions =
        iTransferOptions &
        ~CPosLandmarkDatabase::EIncludeCategories &
        ~CPosLandmarkDatabase::EIncludeGlobalCategoryNames &
        ~CPosLandmarkDatabase::ESupressCategoryCreation;

    __ASSERT_ALWAYS(invalidOptions == CPosLandmarkDatabase::EDefaultOptions,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    BaseConstructL();

    ReadInfoFromResourceFileL(R_POS_LM_LOCAL_IMPORT_LMS_OP);

    if (iTransferOptions & CPosLandmarkDatabase::EIncludeCategories)
        {
        iReader = CPosLmGlobalCategoryReader::NewL();
        }

    CPosLmOperation* parserOp = iLandmarkParser->ParseContentL(EFalse);
    CleanupStack::PushL(parserOp);

    iInternalOpActive = CPosLmLocalInternalOpActive::NewL(*this, parserOp);
    CleanupStack::Pop(parserOp); // ownership transferred
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ConstructL(
    const RArray<TUint>& aLandmarksSelection)
    {
    if (aLandmarksSelection.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    for (TInt i = 0; i < aLandmarksSelection.Count(); i++)
        {
        User::LeaveIfError(iSelectedLmIds.Append(aLandmarksSelection[i]));
        }
    iSelectedLmIds.SortUnsigned();

    ConstructL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalImportOp* CPosLmLocalImportOp::NewL(
    CPosLmLocalDatabase& aDb,
    CPosLandmarkParser& aLandmarkParser,
    CPosLandmarkDatabase::TTransferOptions aTransferOptions,
    CPosLmDiskUtilities& aDiskUtils)
    {
    CPosLmLocalImportOp* self = new (ELeave)
        CPosLmLocalImportOp(aDb, aLandmarkParser, aTransferOptions, aDiskUtils);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalImportOp* CPosLmLocalImportOp::NewL(
    CPosLmLocalDatabase& aDb,
    CPosLandmarkParser& aLandmarkParser,
    const RArray<TUint>& aLandmarksSelection,
    CPosLandmarkDatabase::TTransferOptions aTransferOptions,
    CPosLmDiskUtilities& aDiskUtils)
    {
    CPosLmLocalImportOp* self = new (ELeave)
        CPosLmLocalImportOp(aDb, aLandmarkParser, aTransferOptions, aDiskUtils);
    CleanupStack::PushL(self);
    self->ConstructL(aLandmarksSelection);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalImportOp::~CPosLmLocalImportOp()
    {
    RollbackAndGenerateEventIfNeeded(Progress());

    delete iReader;

    delete iCurrentLm;
    iCurrentCategoryIds.Close();

    iSelectedLmIds.Close();
    iImportedLmIds.Close();

    delete iInternalOpActive;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ImportedLandmarkItemIdsL(
    RArray<TPosLmItemId>& aLmItemIds)
    {
    for (TInt i=0; i<iImportedLmIds.Count(); i++)
        {
        User::LeaveIfError(aLmItemIds.Append(iImportedLmIds[i]));
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::DoNextStepPreL(
    TReal32& aProgress)
    {
    if (iImportState == EImportParseLm)
        {
        ParseLandmarkL();

        // If it is a selected landmark we need to import it to db
        if (IsSelectedLandmark())
            {
            GetParsedLandmarkL();
            if (iTransferOptions & CPosLandmarkDatabase::EIncludeCategories &&
                iCurrentCategoryIds.Count() != 0)
                {
                iImportState = EImportAddLmCatToDb;
                }
            else
                {
                iImportState = EImportAddLmToDb;
                }
            }
        }
    else if (iImportState == EImportAddLmCatToDb)
        {
        ImportLmCategoriesL();
        // Ready importing categories ?
        if (iCurrentCategory == iCurrentCategoryIds.Count())
            {
            iImportState = EImportAddLmToDb;
            }
        }
    else
        {
        // iImportState == EImportAddLmToDb
        ImportLandmarkL();
        // Ready importing landmark
        iImportState = EImportParseLm;
        }

    // Update progress
    UpdateProgress(aProgress);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::DoNextStepPostL(
    TReal32& aProgress)
    {
    // Check if we are ready parsing or not
    if ((iParserOpStatus == KErrNone && iImportState == EImportParseLm) ||
        (IsSelectedLandmarksImported()))
        {
        __ASSERT_ALWAYS(!LandmarkSelection() ||
                        IsSelectedLandmarksImported() ||
                        (LandmarkSelection() &&
                         iSelectedLmIds.Count() > iSelectedLmIdPos &&
                         iCurrentLmIndex > iSelectedLmIds[iSelectedLmIdPos]),
                Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

        User::LeaveIfError(GenerateEventIfNeeded(aProgress));
        iStatusFlag = KErrNone;
        return EFalse;
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalImportOp::Step()
    {
    return iCurrentProgress - iLastReportedProgress;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::CheckResourceValue(
    TInt /*aResourceId*/,
    TInt aNoOfSubOperations)
    {
    __ASSERT_ALWAYS(aNoOfSubOperations > 0, Panic(KPosLandmarksClientPanic,
        EPosInvalidValueSpecifiedInResourceFile));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    RollbackAndGenerateEventIfNeeded(Progress());
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::InternalOpCompleted(
    TInt aStatus,
    TReal32 aProgress,
    TRequestStatus& aClientStatus,
    TReal32& aClientProgress)
    {
    iParserOpStatus = aStatus;
    iParserProgress = aProgress;

    if (aStatus == KErrNone || aStatus == KPosLmOperationNotComplete)
        {
        // Internal operation did not fail. Even if parser operation completed
        // with KErrNone, this operation is not done; the landmark must be
        // imported as well.

        TRAP(aStatus, DoNextStepPreL(aClientProgress));

        if (aStatus == KErrNone)
            {
            // Status must be changed to KPosLmOperationNotComplete. Otherwise,
            // the client thinks that the operation is done.
            aStatus = KPosLmOperationNotComplete;
            }
        else
            {
            RollbackAndGenerateEventIfNeeded(aProgress);
            }
        }
    else
        {
        RollbackAndGenerateEventIfNeeded(aProgress);
        }

    TRequestStatus* clientStatus = &aClientStatus;
    User::RequestComplete(clientStatus, aStatus);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::DoNextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    if (IsInParsingState())
        {
        // Performs next step on parser operation with active object.
        aStatus = KRequestPending;
        PrepareNextParsingStep();
        iInternalOpActive->DoNext(aStatus, aProgress);
        }
    else
        {
        // Import landmark or categories. May also be in compacting state.
        NextStep(aStatus, aProgress);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::DoExecuteL()
    {
    TRequestStatus status(KPosLmOperationNotComplete);
    TRequestStatus statusInternalOp;
    TReal32 progress;

    do
        {
        if (IsInParsingState())
            {
            // Performs next step on parser operation without active object.
            // Using an active object would hang the thread when calling
            // User::WaitForRequest.
            PrepareNextParsingStep();
            iInternalOpActive->InternalOpNextStep(statusInternalOp, progress);
            User::WaitForRequest(statusInternalOp);
            User::LeaveIfError(statusInternalOp.Int());

            // To finish a parsing next step, this function must be called.
            // The function would have been called by the active object if
            // running asynchronously (DoNextStep).
            InternalOpCompleted(statusInternalOp.Int(), progress, status,
                progress);
            }
        else
            {
            // Import landmark or categories. May also be in compacting state.
            NextStep(status, progress);
            }

        User::WaitForRequest(status);
        User::LeaveIfError(status.Int());
        } while (status.Int() == KPosLmOperationNotComplete);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ParseLandmarkL()
    {
    // A landmark has previously been parsed when this function is called.

    // Error during parsing of landmarks
    if (iParserOpStatus != KErrNone &&
        iParserOpStatus != KPosLmOperationNotComplete)
        {
        User::Leave(iParserOpStatus);
        }

    // Reset landmark and landmark category
    delete iCurrentLm;
    iCurrentLm = NULL;
    iCurrentCategoryIds.Reset();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::GetParsedLandmarkL()
    {
    // Get landmark and landmark category ids
    iCurrentLm = iLandmarkParser->LandmarkLC();
    CleanupStack::Pop();
    iCurrentLm->GetCategoriesL(iCurrentCategoryIds);

    // Reset category ids in landmark
    iCurrentLm->RemoveLandmarkAttributes(CPosLandmark::ECategoryInfo);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ImportLandmarkL()
    {
    iDiskUtils->DiskSpaceBelowCriticalLevelL(
        iDiskUtils->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::EAddLandmarkOp,
        *iCurrentLm ), iDb->DatabaseDrive() );

    // Add landmark to db
    PosLmLandmarkHandler::AddLandmarkL( *iDb->DatabaseAccess(), *iCurrentLm );
    iDb->NameIndex().AddL( *iCurrentLm );

    // Add lm item id to array of imported lm item ids
    User::LeaveIfError( iImportedLmIds.Append( iCurrentLm->LandmarkId() ) );

    // Imported a selected landmark, so point to next id in array
    iSelectedLmIdPos++;

    iCurrentCategory = 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::ImportLmCategoriesL()
    {
    for (TInt i = 0;
        i < iNoOfSubOperations &&
        iCurrentCategory < iCurrentCategoryIds.Count();
        i++)
        {
        // Get next gategory
        CPosLandmarkCategory* lmcat =
            iLandmarkParser->LandmarkCategoryLC(
            iCurrentCategoryIds[iCurrentCategory++]);
        TPosLmItemId dbItemId;
        TBool lmCatInDb = ETrue;

        // Is global category ?
        if (lmcat->GlobalCategory() != KPosLmNullGlobalCategory)
            {
            TBool dummyBool;
            // Is global category in db ?
            if (PosLmCategoryHandler::IsGlobalCategoryExistingL(
                    *iDb->DatabaseAccess(), lmcat->GlobalCategory(),
                    dbItemId, dummyBool))
                {
                // Set category id for global category
                PosLmImplExtension::SetCategoryIdL(*lmcat, dbItemId);

                // Import global category names ?
                if (iTransferOptions &
                    CPosLandmarkDatabase::EIncludeGlobalCategoryNames)
                    {
                    // Update global category name in db
                    TPtrC categoryName;
                    TInt err = lmcat->GetCategoryName(categoryName);
                    if (err == KErrNone)
                        {
                        PosLmCategoryHandler::UpdateGlobalCategoryNameL(
                            *iDb->DatabaseAccess(), *lmcat);
                        iLastChangedCategoryId = lmcat->CategoryId();
                        iNrOfCategoriesUpdated++;
                        }
                    }
                }
            else
                {
                // Is global category in resource ?
                HBufC* categoryNameFromResource =
                    GlobalCategoryNameInResourceL(lmcat);
                CleanupStack::PushL(categoryNameFromResource);
                if (categoryNameFromResource)
                    {
                    // Import global category names ?
                    if (iTransferOptions &
                        CPosLandmarkDatabase::EIncludeGlobalCategoryNames)
                        {
                        // Add global category to db with imported name
                        lmCatInDb = AddCategoryL(*lmcat);
                        }
                    else
                        {
                        // Add global category to db with name from resource
                        lmcat->SetCategoryNameL(
                            categoryNameFromResource->Des());
                        lmCatInDb = AddCategoryL(*lmcat);
                        }
                    }
                else
                    {
                    // Global category neither in db or resource file
                    lmCatInDb = AddLocalCategoryL(*lmcat, dbItemId);
                    }

                CleanupStack::PopAndDestroy(categoryNameFromResource);
                }
            }
        else
            {
            // Not a global category
            lmCatInDb = AddLocalCategoryL(*lmcat, dbItemId);
            }

        if (lmCatInDb)
            {
            // Add category id to landmark
            iCurrentLm->AddCategoryL(lmcat->CategoryId());
            }

        CleanupStack::PopAndDestroy(lmcat);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::AddLocalCategoryL(
    CPosLandmarkCategory& aLmCat,
    TPosLmItemId& aLmItemId)
    {
    // Set category id for category
    PosLmImplExtension::SetGlobalCategory(aLmCat, KPosLmNullGlobalCategory);

    // Category name doesn't exists in db ?
    if (PosLmCategoryHandler::VerifyCategoryNameNotExistsL(
            *iDb->DatabaseAccess(), aLmCat, PosLmCategoryHandler::EAddCheck,
            aLmItemId) == KErrNone)
        {
        // Add local category
        return AddCategoryL(aLmCat);
        }
    else
        {
        // Set category id for category
        PosLmImplExtension::SetCategoryIdL(aLmCat, aLmItemId);
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::UpdateProgress(TReal32& aProgress)
    {
    iLastReportedProgress = iCurrentProgress;

    // Set the progress for the operation
    TReal32 importStateProgress;
    // Three (3) step are taken per imported landmark for each NextStepL call
    TReal32 importStatePartProgress =
        (iParserProgress - iOldParserProgress) / 3;
    if (iImportState == EImportAddLmCatToDb)
        {
        TReal32 importLmCatPart = importStatePartProgress;
        // Add current lm cat index
        if (iCurrentCategoryIds.Count() > 0)
            {
            importStatePartProgress =
                (TReal32(1) / iCurrentCategoryIds.Count()) *
                importStatePartProgress;
            importLmCatPart = importStatePartProgress *
                (iCurrentCategory + 1);
            }
        aProgress = iOldParserProgress + importLmCatPart;
        }
    else if (iImportState == EImportAddLmToDb)
        {
        importStateProgress = importStatePartProgress;
        aProgress = iOldParserProgress + (2 * importStateProgress);
        }
    else
        {
        // iImportState == EImportParseLm
        aProgress = iParserProgress;
        }

    iCurrentProgress = aProgress;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::LandmarkSelection()
    {
    return (iSelectedLmIds.Count() > 0);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::IsSelectedLandmark()
    {
    if (!LandmarkSelection() ||
        (LandmarkSelection() &&
        iSelectedLmIds.Count() > iSelectedLmIdPos &&
        iCurrentLmIndex == iSelectedLmIds[iSelectedLmIdPos]))
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::IsSelectedLandmarksImported()
    {
    if (LandmarkSelection() &&
        iSelectedLmIds.Count() == iSelectedLmIdPos)
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalImportOp::GlobalCategoryNameInResourceL(
    CPosLandmarkCategory* aCategory)
    {
    HBufC* name = NULL;
    TPosLmGlobalCategory globalCat = aCategory->GlobalCategory();

    for (TInt i = 0; i < iReader->Count() && !name; i++)
        {
        CPosLandmarkCategory* readerCategory = iReader->GlobalCategoryLC(i);

        TPtrC catName;
        if (readerCategory->GlobalCategory() == globalCat &&
            readerCategory->GetCategoryName(catName) == KErrNone)
            {
            name = catName.AllocL();
            }

        CleanupStack::PopAndDestroy(readerCategory);
        }

    return name;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::RollbackAndGenerateEventIfNeeded(
    TReal32 aProgress)
    {
    RollbackIfNeeded();

    GenerateEventIfNeeded(aProgress);

    // Reset flag: rollback and event generation should not be done twice.
    iStatusFlag = KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalImportOp::GenerateEventIfNeeded(
    TReal32 /*aProgress*/)
    {
    TInt status = KErrNone;
    if (iStatusFlag != KErrNone && iImportedLmIds.Count() > 0)
        {
        if (iImportedLmIds.Count() == 1)
            {
            status = iDb->RegisterEvent(EPosLmEventLandmarkCreated,
                iImportedLmIds[0]);
            if (status != KErrNone)
                {
                return status;
                }
            }
        if (iNrOfCategoriesAdded == 1 && iNrOfCategoriesUpdated <= 1)
            {
            status = iDb->RegisterEvent(EPosLmEventCategoryCreated,
                iLastChangedCategoryId);
            if (status != KErrNone)
                {
                return status;
                }
            }
        if (iNrOfCategoriesUpdated == 1 && iNrOfCategoriesAdded <= 1)
            {
            status = iDb->RegisterEvent(EPosLmEventCategoryUpdated,
                iLastChangedCategoryId);
            if (status != KErrNone)
                {
                return status;
                }
            }

        if (iImportedLmIds.Count() > 1 &&
            (iNrOfCategoriesAdded > 1 || iNrOfCategoriesUpdated > 1))
            {
            return iDb->RegisterEvent(EPosLmEventUnknownChanges);
            }
        else if (iImportedLmIds.Count() > 1)
            {
            return iDb->RegisterEvent(EPosLmEventLandmarkUnknownChanges);
            }
        else if (iNrOfCategoriesAdded > 1 || iNrOfCategoriesUpdated > 1)
            {
            return iDb->RegisterEvent(EPosLmEventCategoryUnknownChanges);
            }
        }
    return status;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::AddCategoryL(
    CPosLandmarkCategory& aCategory)
    {
    TBool toreturn = ETrue;

    if (iTransferOptions &
        CPosLandmarkDatabase::ESupressCategoryCreation)
        {
        toreturn = EFalse;
        }
    else
        {
        iDiskUtils->DiskSpaceBelowCriticalLevelL(
            iDiskUtils->EstimatedDiskSizeOfLmOperation(
                CPosLmDiskUtilities::EAddLmCategoryOp, aCategory),
            iDb->DatabaseDrive());

        PosLmCategoryHandler::AddCategoryL(*iDb->DatabaseAccess(), aCategory);
        iLastChangedCategoryId = aCategory.CategoryId();
        iNrOfCategoriesAdded++;
        }

    return toreturn;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportOp::PrepareNextParsingStep()
    {
    iOldParserProgress = iParserProgress;
    iCurrentLmIndex = iLandmarkParser->NumOfParsedLandmarks();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalImportOp::IsInParsingState()
    {
    return (iImportState == EImportParseLm && !IsInCompactingState() &&
        iParserOpStatus == KPosLmOperationNotComplete);
    }

