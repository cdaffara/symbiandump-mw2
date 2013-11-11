/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Category management for a local landmark database.
*
*
*/


#include    <EPos_LandmarksErrors.h>
#include    <EPos_CPosLandmarkCategory.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_cposlmglobalcategoryreader.h>
#include    <EPos_CPosLmOperation.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_cposlmdiskutilities.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    "EPos_CPosLmLocalCategoryManager.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_CPosLmLocalAddCatToLmOp.h"
#include    "EPos_CPosLmLocalCategoryIterator.h"
#include    "EPos_CPosLmLocalRemoveCategoriesOp.h"
#include    "EPos_CPosLmLocalResetGlobalCatOp.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCategoryManager::CPosLmLocalCategoryManager() :
    CPosLmCategoryManager()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategoryManager::ConstructL(
    TAny* aLmLocalDatabase)
    {
    iDb = reinterpret_cast<CPosLmLocalDatabase*>(aLmLocalDatabase);
    iDiskUtilities = CPosLmDiskUtilities::NewL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalCategoryManager* CPosLmLocalCategoryManager::NewL(
    TAny* aLmLocalDatabase)
    {
    CPosLmLocalCategoryManager* self = new( ELeave ) CPosLmLocalCategoryManager;
    CleanupStack::PushL(self);
    self->ConstructL(aLmLocalDatabase);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalCategoryManager::~CPosLmLocalCategoryManager()
    {
    delete iDiskUtilities;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmarkCategory* CPosLmLocalCategoryManager::ReadCategoryLC(
    TPosLmItemId aCategoryId)
    {
    iDb->LeaveIfInitializingIsNeededL();

    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    PosLmImplExtension::SetCategoryIdL(*category, aCategoryId);

    iDb->AquireLockLC(CPosLmLocalDatabase::EReadLock);

    PosLmCategoryHandler::ReadCategoryL(*iDb->DatabaseAccess(), *category);

    CleanupStack::PopAndDestroy(); // DbLock

    return category;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalCategoryManager::CategoryIteratorL(
    TCategorySortPref aSortPref)
    {
    __ASSERT_ALWAYS(
        aSortPref >= CPosLmCategoryManager::ECategorySortOrderNone &&
        aSortPref <= CPosLmCategoryManager::ECategorySortOrderNameDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    iDb->LeaveIfInitializingIsNeededL();

    return CPosLmLocalCategoryIterator::NewL(*iDb,
        aSortPref, EFalse);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalCategoryManager::ReferencedCategoryIteratorL(
    TCategorySortPref aSortPref)
    {
    __ASSERT_ALWAYS(
        aSortPref >= CPosLmCategoryManager::ECategorySortOrderNone &&
        aSortPref <= CPosLmCategoryManager::ECategorySortOrderNameDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    iDb->LeaveIfInitializingIsNeededL();

    return CPosLmLocalCategoryIterator::NewL(*iDb,
        aSortPref, ETrue);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalCategoryManager::AddCategoryL(
    CPosLandmarkCategory& aCategory)
    {
    iDb->LeaveIfInitializingIsNeededL();
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::EAddLmCategoryOp,
        aCategory), iDb->DatabaseDrive());

    iDb->AquireLockLC(CPosLmLocalDatabase::EWriteLock);
    iDb->DatabaseAccess()->BeginTransactionLC();

    PosLmCategoryHandler::VerifyCategoryIsProperL(*iDb->DatabaseAccess(),
        aCategory, PosLmCategoryHandler::EAddCheck);
    PosLmCategoryHandler::AddCategoryL(*iDb->DatabaseAccess(), aCategory);

    iDb->DatabaseAccess()->CommitTransactionAndPopL();
    CleanupStack::PopAndDestroy(); // DbLock

    User::LeaveIfError(iDb->RegisterEvent(EPosLmEventCategoryCreated,
        aCategory.CategoryId()));

    return aCategory.CategoryId();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategoryManager::UpdateCategoryL(
    const CPosLandmarkCategory& aCategory)
    {
    iDb->LeaveIfInitializingIsNeededL();

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::EUpdateLmCategoryOp,
        aCategory), iDb->DatabaseDrive());

    iDb->AquireLockLC(CPosLmLocalDatabase::EWriteLock);
    iDb->DatabaseAccess()->BeginTransactionLC();

    PosLmCategoryHandler::VerifyCategoryIsProperL(*iDb->DatabaseAccess(),
        aCategory, PosLmCategoryHandler::EUpdateCheck);
    PosLmCategoryHandler::UpdateCategoryL(*iDb->DatabaseAccess(), aCategory);

    iDb->DatabaseAccess()->CommitTransactionAndPopL();
    CleanupStack::PopAndDestroy(); // DbLock

    User::LeaveIfError(iDb->RegisterEvent(EPosLmEventCategoryUpdated,
        aCategory.CategoryId()));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalCategoryManager::RemoveCategoryL(
    TPosLmItemId aCategoryId)
    {
    iDb->LeaveIfInitializingIsNeededL();

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::ERemoveLmCategoryOp,
        aCategoryId), iDb->DatabaseDrive());

    return CPosLmLocalRemoveCategoriesOp::NewL(*iDb, aCategoryId);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalCategoryManager::RemoveCategoriesL(
    const RArray<TPosLmItemId>& aCategoryIdList)
    {
    iDb->LeaveIfInitializingIsNeededL();

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::ERemoveLmCategoriesOp,
        aCategoryIdList), iDb->DatabaseDrive());

    return CPosLmLocalRemoveCategoriesOp::NewL(*iDb, aCategoryIdList);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalCategoryManager::AddCategoryToLandmarksL(
    TPosLmItemId aCategoryId,
    RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    iDb->LeaveIfInitializingIsNeededL();

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmCatOperation(
        CPosLmDiskUtilities::EAddLmCategoryToLmOp,
        aCategoryId, aLandmarkIdArray), iDb->DatabaseDrive());

    return CPosLmLocalAddCatToLmOp::NewL(*iDb, aCategoryId, aLandmarkIdArray);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalCategoryManager::RemoveCategoryFromLandmarksL(
    TPosLmItemId aCategoryId,
    RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    iDb->LeaveIfInitializingIsNeededL();

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmCatOperation(
        CPosLmDiskUtilities::ERemoveLmCategoryFromLmOp,
        aCategoryId, aLandmarkIdArray), iDb->DatabaseDrive());

    return CPosLmLocalRemoveCategoriesOp::NewL(*iDb, aCategoryId,
        aLandmarkIdArray);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalCategoryManager::GetCategoryL(
    const TDesC& aCategoryName)
    {
    iDb->LeaveIfInitializingIsNeededL();

    // Wrap the category name into a category.
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(aCategoryName);

    iDb->AquireLockLC(CPosLmLocalDatabase::EReadLock);

    TPosLmItemId categoryId;
    TInt err = PosLmCategoryHandler::VerifyCategoryNameNotExistsL(
        *iDb->DatabaseAccess(), *category, PosLmCategoryHandler::EAddCheck,
        categoryId);

    if (err == KErrNone)
        {
        categoryId = KPosLmNullItemId;
        }
    else if (err != KErrAlreadyExists)
        {
        User::Leave(err);
        }

    CleanupStack::PopAndDestroy(); // DbLock

    CleanupStack::PopAndDestroy(category);

    return categoryId;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalCategoryManager::GetGlobalCategoryL(
    TPosLmGlobalCategory aGlobalCategory)
    {
    iDb->LeaveIfInitializingIsNeededL();

    TPosLmItemId categoryId;
    TBool isModified;

    iDb->AquireLockLC(CPosLmLocalDatabase::EReadLock);

    if (!PosLmCategoryHandler::IsGlobalCategoryExistingL(*iDb->DatabaseAccess(),
        aGlobalCategory, categoryId, isModified))
        {
        categoryId = KPosLmNullItemId;
        }

    CleanupStack::PopAndDestroy(); // DbLock

    return categoryId;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalCategoryManager::GlobalCategoryNameL(
    TPosLmGlobalCategory aGlobalCategory)
    {
    iDb->LeaveIfInitializingIsNeededL();

    CPosLmGlobalCategoryReader* reader = CPosLmGlobalCategoryReader::NewL();
    CleanupStack::PushL(reader);

    HBufC* name = NULL;
    TPtrC catName;

    for (TInt i = 0; i < reader->Count() && !name; i++)
        {
        CPosLandmarkCategory* category = reader->GlobalCategoryLC(i);

        if (category->GlobalCategory() == aGlobalCategory &&
            category->GetCategoryName(catName) == KErrNone)
            {
            name = catName.AllocL();
            }
        CleanupStack::PopAndDestroy(category);
        }

    CleanupStack::PopAndDestroy(reader);

    return name;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalCategoryManager::ResetGlobalCategoriesL()
    {
    iDb->LeaveIfInitializingIsNeededL();

    return CPosLmLocalResetGlobalCatOp::NewL(*iDb);
    }

