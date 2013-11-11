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
* Description: Handles category operations on a local landmark database.
*
*
*/


#include    <EPos_CPosLandmarkCategory.h>
#include    <EPos_PosLmImplExtension.h>
#include    "EPos_PosLmCategoryHandler.h"
#include    "EPos_CPosLmLocalDbAccess.h"
#include    "EPos_PosLmLongTextColHandler.h"
#include    "EPos_PosLmIconHandler.h"
#include    "EPos_LandmarkDatabaseStructure.h"
#include    "EPos_PosLmDatabaseUtility.h"

_LIT(KPosLmSqlMergeCategories, "UPDATE %S SET %S=%u WHERE %S=%u");
_LIT(KPosLmSqlChangeName, "UPDATE %S SET %S='%S' WHERE %S=%u");
_LIT(KPosLmSqlFindAllGlobalCategories, "SELECT %S, %S FROM %S WHERE %S IS NOT NULL");
_LIT(KPosLmSqlDeleteAllGlobalCategories, "DELETE FROM %S WHERE %S IS NOT NULL");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::ReadCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aCategory )
    {
    RDbView view;
    OpenCategoryViewLC( aCategory.CategoryId(), view, aDbAccess );

    view.GetL();
    ReadCategoryL( view, aDbAccess, aCategory );

    CleanupStack::PopAndDestroy( &view );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::AddCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aCategory)
    {
    VerifyCategoryNameNotExistsL(aDbAccess, aCategory, EAddCheck);
    PerformAddL(aDbAccess, aCategory);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::UpdateCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory)
    {
    VerifyCategoryNameNotExistsL(aDbAccess, aCategory, EUpdateCheck);
    UpdateCategoryWithoutTransactionL(aDbAccess, aCategory, EFalse);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::RemoveCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aCategoryId)
    {
    // Remove category from all landmarks.
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmLandmarkCategoryTable,
        &KPosLmCategoryIdCol, aCategoryId);

    aDbAccess.ExecuteL(*sql);

    // Remove category info.
    sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmCategoryTable,
        &KPosLmCategoryIdCol, aCategoryId);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::UpdateGlobalCategoryNameL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory)
    {
    TPosLmItemId glCategory;
    TBool glCategoryNameModified;

    if (IsGlobalCategoryExistingL(aDbAccess, aCategory.GlobalCategory(),
        glCategory, glCategoryNameModified))
        {
        if (!glCategoryNameModified)
            {
            TPosLmItemId existingCategory;

            if (VerifyCategoryNameNotExistsL(aDbAccess, aCategory,
                EServerUpdateCheck, existingCategory) == KErrAlreadyExists)
                {
                MergeCategoriesL(aDbAccess, glCategory, existingCategory);
                }

            ChangeCategoryNameL(aDbAccess, glCategory, aCategory);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::ResetGlobalCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aGlCategory)
    {
    TPosLmItemId glCatId;
    TBool isModified;

    TBool catExists = IsGlobalCategoryExistingL(aDbAccess,
        aGlCategory.GlobalCategory(), glCatId, isModified);

    TPosLmItemId nameExistId;
    TInt nameResult = VerifyCategoryNameNotExistsL(aDbAccess, aGlCategory,
        EServerUpdateCheck, nameExistId);

    if (catExists)
        {
        PosLmImplExtension::SetCategoryIdL(aGlCategory, glCatId);

        if (nameResult == KErrAlreadyExists)
            {
            MergeCategoriesL(aDbAccess, glCatId, nameExistId);
            }

        UpdateCategoryWithoutTransactionL(aDbAccess, aGlCategory, ETrue);
        }
    else
        {
        if (nameResult != KErrAlreadyExists)
            {
            PerformAddL(aDbAccess, aGlCategory);
            }
        else
            {
            PosLmImplExtension::SetCategoryIdL(aGlCategory, nameExistId);
            UpdateCategoryWithoutTransactionL(aDbAccess, aGlCategory, ETrue);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::VerifyCategoryNameNotExistsL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory,
    const TCheckCategoryType& aMode)
    {
    TPosLmItemId dummy = 0;
    User::LeaveIfError(VerifyCategoryNameNotExistsL(aDbAccess, aCategory, aMode,
        dummy));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt PosLmCategoryHandler::VerifyCategoryNameNotExistsL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory,
    const TCheckCategoryType& aMode,
    TPosLmItemId& aId)
    {
    TPtrC name;
    TInt error = aCategory.GetCategoryName(name);
    if (error != KErrNone)
        {
        return error;
        }

    HBufC* embeddedText =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(name);
    TPtr textPtr = embeddedText->Des();

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindString, &KPosLmSqlAll, &KPosLmCategoryTable,
        &KPosLmCategoryNameCol, &textPtr);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    if (view.NextL())
        {
        view.GetL();

        if (aMode == EAddCheck ||
            (aMode == EUpdateCheck && view.ColUint32(EPosLmCcCategoryIdCol) !=
            aCategory.CategoryId()) ||
            (aMode == EServerUpdateCheck && view.ColUint16(
            EPosLmCcGlobalCategoryIdCol) != aCategory.GlobalCategory()))
            {
            error = KErrAlreadyExists;
            aId = view.ColUint32(EPosLmCcCategoryIdCol);
            }
        }

    CleanupStack::PopAndDestroy(3, embeddedText); //sql, &view

    return error;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::VerifyGlobalCategoryUnchangedL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmGlobalCategoryIdCol,
        &KPosLmCategoryTable, &KPosLmCategoryIdCol, aCategory.CategoryId());

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    if (view.NextL())
        {
        view.GetL();

        const TInt KColumnInQuery = 1;

        if (view.ColUint16(KColumnInQuery) != aCategory.GlobalCategory())
            {
            User::Leave(KErrAlreadyExists);
            }
        }
    else
        {
        User::Leave(KErrNotFound);
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::VerifyCategoryIsProperL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory,
    const PosLmCategoryHandler::TCheckCategoryType& aMode)
    {
    TPtrC name;

    if (aMode == EUpdateCheck)
        {
        if (aCategory.GetCategoryName(name) != KErrNone)
            {
            // Leave if category name is not set.
            User::Leave(KErrArgument);
            }
        else if (aCategory.GlobalCategory() != KPosLmNullGlobalCategory)
            {
            VerifyGlobalCategoryUnchangedL(aDbAccess, aCategory);
            }
        }
    else
        {
        if (aCategory.GlobalCategory() != KPosLmNullGlobalCategory ||
            aCategory.GetCategoryName(name) != KErrNone)
            {
            // Leave if global category is set or category name is not set.
            User::Leave(KErrArgument);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmCategoryHandler::IsGlobalCategoryExistingL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmGlobalCategory aGlCategoryId,
    TPosLmItemId& aCategoryId,
    TBool& aIsModified)
    {
    if (aGlCategoryId == KPosLmNullGlobalCategory)
        {
        return EFalse;
        }

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll, &KPosLmCategoryTable,
        &KPosLmGlobalCategoryIdCol, aGlCategoryId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TBool exists = EFalse;

    if (view.NextL())
        {
        view.GetL();
        aCategoryId = view.ColUint32(EPosLmCcCategoryIdCol);
        aIsModified = (view.ColInt(EPosLmCcCategoryNameModifiedCol) == 1);
        exists = ETrue;
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    return exists;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt PosLmCategoryHandler::FindCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aCategoryId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmCategoryIdCol,
        &KPosLmCategoryTable, &KPosLmCategoryIdCol, aCategoryId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TInt result = KErrNone;

    if (view.IsEmptyL())
        {
        result = KErrNotFound;
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    return result;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::GetAllCategoryIdsL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    RArray<TPosLmItemId>& aIdArray)
    {
    aIdArray.Reset();

    GetCategoryIdsL(aDbAccess, aSortPref, EFalse, aIdArray, aIdArray);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::GetReferencedCategoryIdsL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    RArray<TPosLmItemId>& aIdArray)
    {
    aIdArray.Reset();

    RArray<TPosLmItemId> refCategoryIds;
    CleanupClosePushL(refCategoryIds);

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

    // Read categories associated with a landmark.
    sql->Des().Format(KPosLmSqlSelect, &KPosLmCategoryIdCol,
        &KPosLmLandmarkCategoryTable);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    while (view.NextL())
        {
        view.GetL();
        TPosLmItemId id = view.ColUint32(1);

        if (refCategoryIds.Find(id) == KErrNotFound)
            {
            User::LeaveIfError(refCategoryIds.Append(id));
            }
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    if (refCategoryIds.Count() > 0)
        {
        GetCategoryIdsL(aDbAccess, aSortPref, ETrue, refCategoryIds, aIdArray);
        }

    CleanupStack::PopAndDestroy(&refCategoryIds);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::GetGlobalCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TInt>& aNotWantedIds,
    RArray<TPosLmItemId>& aIdArray)
    {
    aIdArray.Reset();

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().AppendFormat(KPosLmSqlFindAllGlobalCategories, &KPosLmCategoryIdCol,
        &KPosLmGlobalCategoryIdCol, &KPosLmCategoryTable,
        &KPosLmGlobalCategoryIdCol);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    // Check whether any global categories should be removed or not
    while (view.NextL())
        {
        view.GetL();

        if (aNotWantedIds.Find(view.ColUint16(2)) == KErrNotFound)
            {
            aIdArray.AppendL( view.ColUint32(1) );
            }
        }

    CleanupStack::PopAndDestroy(2, sql); // &view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::RemoveAllGlobalCategoriesL(
    CPosLmLocalDbAccess& aDbAccess)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteAllGlobalCategories, &KPosLmCategoryTable,
        &KPosLmGlobalCategoryIdCol);

    aDbAccess.ExecuteL(*sql);
    CleanupStack::PopAndDestroy( sql );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::GetCategoryIdsL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    TBool aFetchReferenced,
    const RArray<TPosLmItemId>& aRefIdArray,
    RArray<TPosLmItemId>& aIdArray)
    {
    aIdArray.Reset();

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

    if (aSortPref != CPosLmCategoryManager::ECategorySortOrderNone)
        {
        sql->Des().Format(KPosLmSqlSelectOrderByString, &KPosLmCategoryIdCol,
            &KPosLmCategoryTable, &KPosLmCategoryNameCol);

        if (aSortPref ==
            CPosLmCategoryManager::ECategorySortOrderNameDescending)
            {
            sql->Des().Append(KPosLmSqlDescOrder);
            }
        }
    else
        {
        sql->Des().Format(KPosLmSqlSelect, &KPosLmCategoryIdCol,
            &KPosLmCategoryTable);
        }

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql, EDbCompareCollated);

    while (view.NextL())
        {
        view.GetL();

        const TInt KColumnInQuery = 1;

        TPosLmItemId id = view.ColUint32(KColumnInQuery);

        if (!aFetchReferenced || aRefIdArray.Find(id) != KErrNotFound)
            {
            User::LeaveIfError(aIdArray.Append(id));
            }
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::ReadGlobalIdL(
    RDbRowSet& aView,
    CPosLandmarkCategory& aCategory)
    {
    if (!aView.IsColNull(EPosLmCcGlobalCategoryIdCol))
        {
        PosLmImplExtension::SetGlobalCategory(aCategory,
            aView.ColUint16(EPosLmCcGlobalCategoryIdCol));
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::ReadNameL(
    RDbRowSet& aView,
    CPosLandmarkCategory& aCategory)
    {
    if (!aView.IsColNull(EPosLmCcCategoryNameCol))
        {
        aCategory.SetCategoryNameL(aView.ColDes(EPosLmCcCategoryNameCol));
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::ReadIconL(
    CPosLmLocalDbAccess& aDbAccess,
    RDbRowSet& aView,
    CPosLandmarkCategory& aCategory)
    {
    if (!aView.IsColNull(EPosLmCcIconIdCol) &&
        !aView.IsColNull(EPosLmCcIconPicNrCol) &&
        !aView.IsColNull(EPosLmCcMaskPicNrCol))
        {
        TInt iconIndex = aView.ColInt32(EPosLmCcIconPicNrCol);
        TInt maskIndex = aView.ColInt32(EPosLmCcMaskPicNrCol);

        // Get icon path
        HBufC* iconPath = HBufC::NewLC(KMaxFileName);
        TPtr path = iconPath->Des();

        if (PosLmIconHandler::GetIconPathL(aDbAccess,
            aView.ColUint32(EPosLmCcIconIdCol), path) == KErrNone)
            {
            aCategory.SetIconL(*iconPath, iconIndex, maskIndex);
            }

        CleanupStack::PopAndDestroy(iconPath);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::AddCategoryInfoL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aCategory,
    TUint32 aIconId)
    {
    TPtrC name;
    User::LeaveIfError(aCategory.GetCategoryName(name));

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmCategoryTable);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EAddPreparation, view, *sql);

    view.InsertL();
    view.SetColL(EPosLmCcCategoryNameCol, name);

    if (aCategory.GlobalCategory() != KPosLmNullGlobalCategory)
        {
        view.SetColL(EPosLmCcGlobalCategoryIdCol,
            (TUint)aCategory.GlobalCategory());
        }

    TInt iconIndex;
    TInt maskIndex;
    TPtrC str;

    if (aCategory.GetIcon(str, iconIndex, maskIndex) == KErrNone)
        {
        view.SetColL(EPosLmCcIconIdCol, aIconId);
        view.SetColL(EPosLmCcIconPicNrCol, iconIndex);
        view.SetColL(EPosLmCcMaskPicNrCol, maskIndex);
        }

    view.PutL();

    PosLmImplExtension::SetCategoryIdL(aCategory,
        view.ColUint32(EPosLmCcCategoryIdCol));

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::PerformUpdateL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory,
    const TUint32& aIconId,
    TBool aReset)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll, &KPosLmCategoryTable,
        &KPosLmCategoryIdCol, aCategory.CategoryId());

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    if (view.NextL())
        {
        view.GetL();
        view.UpdateL();

        TPtrC str;
        TInt iconIndex;
        TInt maskIndex;

        // Set icon
        if (aCategory.GetIcon(str, iconIndex, maskIndex) == KErrNone)
            {
            view.SetColL(EPosLmCcIconIdCol, aIconId);
            view.SetColL(EPosLmCcIconPicNrCol, iconIndex);
            view.SetColL(EPosLmCcMaskPicNrCol, maskIndex);
            }
        else
            {
            view.SetColNullL(EPosLmCcIconIdCol);
            view.SetColNullL(EPosLmCcIconPicNrCol);
            view.SetColNullL(EPosLmCcMaskPicNrCol);
            }

        // Set category name.
        if (aCategory.GetCategoryName(str) == KErrNone)
            {
            // Set category name modified column if string changed.
            if (aReset)
                {
                view.SetColL(EPosLmCcCategoryNameModifiedCol, 0);
                }
            else if (
                view.ColDes(EPosLmCcCategoryNameCol).Compare(str) != 0)
                {
                view.SetColL(EPosLmCcCategoryNameModifiedCol, 1);
                }
            view.SetColL(EPosLmCcCategoryNameCol, str);
            }
        else
            {
            User::Leave(KErrArgument);
            }

        // Set global category.
        if (aCategory.GlobalCategory() != KPosLmNullGlobalCategory)
            {
            view.SetColL(EPosLmCcGlobalCategoryIdCol,
                (TUint)aCategory.GlobalCategory());
            }
        else
            {
            view.SetColNullL(EPosLmCcGlobalCategoryIdCol);
            }

        view.PutL();
        }
    else
        {
        User::Leave(KErrNotFound);
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmCategoryHandler::ReadIconIdL(
    CPosLmLocalDbAccess& aDbAccess,
    const TPosLmItemId& aId,
    TUint32& aIconId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmIconIdCol,
        &KPosLmCategoryTable, &KPosLmCategoryIdCol, aId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TBool iconIsSet = ETrue;

    if (view.NextL())
        {
        view.GetL();

        if (view.IsColNull(1))
            {
            iconIsSet = EFalse;
            }
        else
            {
            aIconId = view.ColUint32(1);
            }
        }
    else
        {
        User::Leave(KErrNotFound);
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    return iconIsSet;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::PerformAddL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aCategory)
    {
    TPtrC iconFileName;
    TInt iconIndex;
    TInt maskIndex;
    TUint32 iconId = 0;

    if (aCategory.GetIcon(iconFileName, iconIndex, maskIndex) == KErrNone)
        {
        PosLmIconHandler::GetIconPathIdL(aDbAccess, iconFileName, iconId);
        }

    AddCategoryInfoL(aDbAccess, aCategory, iconId);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::MergeCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aGlCategory,
    TPosLmItemId aCategory)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlMergeCategories, &KPosLmLandmarkCategoryTable,
        &KPosLmCategoryIdCol, aGlCategory, &KPosLmCategoryIdCol, aCategory);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);

    // Remove old category
    RemoveCategoryL(aDbAccess, aCategory);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::ChangeCategoryNameL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aCategoryId,
    const CPosLandmarkCategory& aCategory)
    {
    TPtrC categoryName;

    if (aCategory.GetCategoryName(categoryName) == KErrNone)
        {
        HBufC* embeddedText =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(categoryName);
    	TPtr textPtr = embeddedText->Des();
        
        HBufC* sql = HBufC::NewLC(
            KPosLmSqlStatementMaxLen + categoryName.Length());
        sql->Des().AppendFormat(KPosLmSqlChangeName, &KPosLmCategoryTable,
            &KPosLmCategoryNameCol, &textPtr, &KPosLmCategoryIdCol,
            aCategoryId);

        aDbAccess.ExecuteL(*sql);

        CleanupStack::PopAndDestroy(sql);
        CleanupStack::PopAndDestroy(embeddedText);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::UpdateCategoryWithoutTransactionL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmarkCategory& aCategory,
    TBool aReset)
    {
    TPtrC str;
    TInt iconIndex;
    TInt maskIndex;
    TUint32 iconId = 0;

    if (aCategory.GetIcon(str, iconIndex, maskIndex) == KErrNone)
        {
        PosLmIconHandler::GetIconPathIdL(aDbAccess, str, iconId);
        }

    TUint32 oldIconId;
    TBool oldIconExists =
        ReadIconIdL(aDbAccess, aCategory.CategoryId(), oldIconId);

    PerformUpdateL(aDbAccess, aCategory, iconId, aReset);

    if (oldIconExists)
        {
        PosLmIconHandler::RemoveIconIfNotUsedL(aDbAccess, oldIconId);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TUint PosLmCategoryHandler::TotalCategoryCountL( CPosLmLocalDbAccess& aDbAccess )
    {
    return PosLmDatabaseUtility::GetTableRowCountL( aDbAccess, KPosLmCategoryTable );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmCategoryHandler::ReadCategoryL(
    RDbRowSet& aView,
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmarkCategory& aCategory )
    {
    ReadGlobalIdL( aView, aCategory );
    ReadNameL( aView, aCategory );
    ReadIconL( aDbAccess, aView, aCategory );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmCategoryHandler::OpenCategoryViewLC(
    TPosLmItemId aCategoryId,
    RDbView& aView,
    CPosLmLocalDbAccess& aDbAccess )
    {
    HBufC* sql = HBufC::NewLC( KPosLmSqlStatementMaxLen );

    sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll, &KPosLmCategoryTable,
        &KPosLmCategoryIdCol, aCategoryId );
    
    aDbAccess.PrepareViewLC( 
        CPosLmLocalDbAccess::EUpdatablePreparation, aView, *sql );

    if ( !aView.NextL() )
        {
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::Pop( &aView );
    CleanupStack::PopAndDestroy( sql );
    CleanupClosePushL( aView );
    }
