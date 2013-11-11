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
* Description: Operation for exporting landmarks from a database.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmresourcereader.h>
#include    <eposlmasyncops.rsg>
#include    <EPos_CPosLandmarkEncoder.h>
#include    <EPos_LandmarksErrors.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_cposlmglobalcategoryreader.h>
#include    "EPos_CPosLmLocalExportOp.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "epos_cposlmlocaldbaccess.h"
#include    "EPos_LocalLandmarks.h"

//CONSTANTS
const TReal32 KOperationDone = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalExportOp::CPosLmLocalExportOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalExportOp::CPosLmLocalExportOp(
    CPosLmLocalDatabase& aDb,
    CPosLandmarkEncoder& aLandmarkEncoder,
    CPosLandmarkDatabase::TTransferOptions aTransferOptions)
:   CPosLmLocalOperation(aDb),
    iLandmarkEncoder(&aLandmarkEncoder),
    iTransferOptions(aTransferOptions),
    iStatusFlag(KPosLmOperationNotComplete),
    iCurrentIsLm(ETrue),
    iNrOfSubOperations(1)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalExportOp::ConstructL(
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    CPosLandmarkDatabase::TTransferOptions invalidOptions =
        iTransferOptions &
        ~CPosLandmarkDatabase::EIncludeCategories &
        ~CPosLandmarkDatabase::EIncludeGlobalCategoryNames;

    __ASSERT_ALWAYS(invalidOptions == CPosLandmarkDatabase::EDefaultOptions,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    BaseConstructL();

    if (aLandmarkIdArray.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    if ((iTransferOptions & CPosLandmarkDatabase::EIncludeCategories) &&
        (iTransferOptions & CPosLandmarkDatabase::EIncludeGlobalCategoryNames))
        {
        iReader = CPosLmGlobalCategoryReader::NewL();
        }

    for (TInt i = 0; i < aLandmarkIdArray.Count(); i++)
        {
        User::LeaveIfError(iLandmarkIdArray.Append(aLandmarkIdArray[i]));
        }

    ReadInfoFromResourceFileL();

    AquireLockL(CPosLmLocalDatabase::EReadLock);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalExportOp* CPosLmLocalExportOp::NewL(
    CPosLmLocalDatabase& aDb,
    CPosLandmarkEncoder& aLandmarkEncoder,
    const RArray<TPosLmItemId>& aLandmarkIdArray,
    CPosLandmarkDatabase::TTransferOptions aTransferOptions)
    {
    CPosLmLocalExportOp* self = new (ELeave)
        CPosLmLocalExportOp(aDb, aLandmarkEncoder, aTransferOptions);
    CleanupStack::PushL(self);
    self->ConstructL(aLandmarkIdArray);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalExportOp::~CPosLmLocalExportOp()
    {
    delete iReader;
    iLandmarkIdArray.Close();
    iCategoryIdArray.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportOp::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalExportOp::NextStepL(
    TReal32& aProgress)
    {
    if (iCurrentIsLm)
        {
        CPosLandmark* lm = ReadLmFromDbLC(iLandmarkIdArray[iCurrentLm++]);
        iLandmarkEncoder->AddLandmarkL(*lm);

        if (iTransferOptions & CPosLandmarkDatabase::EIncludeCategories)
            {
            iCategoryIdArray.Close();
            iCurrentCategory = 0;

            lm->GetCategoriesL(iCategoryIdArray);
            if (iCategoryIdArray.Count() != 0)
                {
                iCurrentIsLm = EFalse;
                }
            }

        CleanupStack::PopAndDestroy(lm);
        }
    else
        {
        for (TInt i = 0;
            i < iNrOfSubOperations &&
                iCurrentCategory < iCategoryIdArray.Count(); i++)
            {
            	//coverity[alloc_fn : FALSE]
            	//coverity[assign :FALSE]
            CPosLandmarkCategory* category =
                ReadCategoryFromDbLC(iCategoryIdArray[iCurrentCategory++]);

            if (iTransferOptions &
                CPosLandmarkDatabase::EIncludeGlobalCategoryNames)
                {
                	//coverity[leave_without_push : FALSE]
                SetPredefinedGlobalNameL(category);
                }

            iLandmarkEncoder->AddCategoryForLatestLandmarkL(*category);

            CleanupStack::PopAndDestroy(category);

            if (iCurrentCategory == iCategoryIdArray.Count())
                {
                iCurrentIsLm = ETrue;
                }
            }
        }

    if (iCurrentLm == iLandmarkIdArray.Count() &&
        iCurrentCategory == iCategoryIdArray.Count())
        {
        iStatusFlag = KErrNone;
        aProgress = KOperationDone;
        ReleaseLock();
        }
    else if (iCurrentIsLm)
        {
        aProgress = TReal32(iCurrentLm) / iLandmarkIdArray.Count();
        }
    else
        {
        aProgress = TReal32(iCurrentLm - 1) / iLandmarkIdArray.Count() +
                    TReal32(iCurrentCategory + 1) /
                    (iCategoryIdArray.Count() + 1) /
                    iLandmarkIdArray.Count();
        }

    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalExportOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    ReleaseLock();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportLandmarksOp::ReadLmFromDbLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmLocalExportOp::ReadLmFromDbLC(TPosLmItemId aId)
    {
    CPosLandmark* landmark = CPosLandmark::NewLC();
    PosLmImplExtension::SetLandmarkIdL(*landmark, aId);

    PosLmLandmarkHandler::ReadLandmarkL(*iDb->DatabaseAccess(), *landmark);
    return landmark;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportLandmarksOp::ReadCategoryFromDbLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmarkCategory* CPosLmLocalExportOp::ReadCategoryFromDbLC(
    TPosLmItemId aId)
    {
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    PosLmImplExtension::SetCategoryIdL(*category, aId);

    PosLmCategoryHandler::ReadCategoryL(*iDb->DatabaseAccess(), *category);
    return category;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportLandmarksOp::SetPredefinedGlobalNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalExportOp::SetPredefinedGlobalNameL(
    CPosLandmarkCategory* aCategory)
    {
    TPosLmGlobalCategory globalCat = aCategory->GlobalCategory();
    if (globalCat != KPosLmNullGlobalCategory)
        {
        TBool catFound = EFalse;
        for (TInt i = 0; i < iReader->Count() && !catFound; i++)
            {
            CPosLandmarkCategory* readerCategory = iReader->GlobalCategoryLC(i);

            TPtrC catName;
            if (readerCategory->GlobalCategory() == globalCat &&
                readerCategory->GetCategoryName(catName) == KErrNone)
                {
                catFound = ETrue;
                aCategory->SetCategoryNameL(catName);
                }

            CleanupStack::PopAndDestroy(readerCategory);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalExportLandmarksOp::ReadInfoFromResourceFileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalExportOp::ReadInfoFromResourceFileL()
    {
    CPosLmResourceReader* resources =
        CPosLmResourceReader::NewLC(KPosLocalSuboperationResourceFile);

    iNrOfSubOperations = resources->ReadInt32L(R_POS_LM_LOCAL_EXPORT_LMS_OP);

    if (iNrOfSubOperations <= 0)
        {
        Panic(KPosLandmarksClientPanic,
            EPosInvalidValueSpecifiedInResourceFile);
        }

    CleanupStack::PopAndDestroy(resources);
    }

//  End of File
