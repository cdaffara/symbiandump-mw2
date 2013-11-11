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
* Description: Handles landmark operations on a local landmark database.
*
*
*/


#include    <e32math.h>
#include    <lbsposition.h>
#include    <EPos_LandmarksErrors.h>
#include    <EPos_PosLmImplExtension.h>
#include    "EPos_PosLmLandmarkHandler.h"
#include    "EPos_CPosLmLocalDbAccess.h"
#include    "EPos_PosLmLongTextColHandler.h"
#include    "EPos_PosLmIconHandler.h"
#include    "EPos_PosLmCategoryHandler.h"
#include    "EPos_PosLmTrealCompare.h"
#include    "EPos_LandmarkDatabaseStructure.h"
#include    "EPos_PosLmDatabaseUtility.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::ReadLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark)
    {
    RDbView view;
    OpenLandmarkViewLC( aLandmark.LandmarkId(), view, aDbAccess );
    view.GetL();
    ReadLandmarkL( view, aDbAccess, aLandmark );
    CleanupStack::PopAndDestroy( &view );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::ReadLandmarkL(
    RDbRowSet& aView,
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark)
    {
    RArray<TUint> dummy; // Not used here. All fields should be read.
    ReadLandmarkL( 
        aView, aDbAccess, aLandmark, CPosLandmark::EAllAttributes, EFalse, dummy);
    dummy.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::AddLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark)
    {
    AddBasicLandmarkInfoL(aDbAccess, aLandmark);
    AddFieldsL(aDbAccess, aLandmark);
    AddCategoriesL(aDbAccess, aLandmark);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::UpdateLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark)
    {
    CPosLandmark* oldLandmark = CPosLandmark::NewLC();
    PosLmImplExtension::SetLandmarkIdL(*oldLandmark, aLandmark.LandmarkId());
    ReadLandmarkL(aDbAccess, *oldLandmark);

    UpdateCategoriesL(aDbAccess, aLandmark, *oldLandmark);
    UpdateFieldsL(aDbAccess, aLandmark, *oldLandmark);
    UpdateIconAndBasicLandmarkInfoL(aDbAccess, aLandmark, *oldLandmark);

    CleanupStack::PopAndDestroy(oldLandmark);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::RemoveLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aLandmarkId)
    {
    RemoveAllCategoriesFromLandmarkL(aDbAccess, aLandmarkId);
    RemoveAllFieldsFromLandmarkL(aDbAccess, aLandmarkId);

    TUint32 iconPathId;

    if (RemoveBasicLandmarkInfoL(aDbAccess, aLandmarkId, iconPathId))
        {
        PosLmIconHandler::RemoveIconIfNotUsedL(aDbAccess, iconPathId);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::ReadPartialLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark,
    CPosLmPartialReadParameters& aPartialSettings )
    {
    RDbView view;
    OpenLandmarkViewLC( aLandmark.LandmarkId(), view, aDbAccess );

    view.GetL();
    ReadPartialLandmarkL( view, aDbAccess, aLandmark, aPartialSettings );
    
    CleanupStack::PopAndDestroy( &view );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::ReadPartialLandmarkL(
    RDbRowSet& aLandmarkView,
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark,
    CPosLmPartialReadParameters& aPartialSettings )
    {
    PosLmImplExtension::SetPartialL( aLandmark, ETrue );

    RArray<TUint> requestedFields;
    CleanupClosePushL( requestedFields );

    User::LeaveIfError(
        aPartialSettings.GetRequestedPositionFields( requestedFields ) );

    ReadLandmarkL( 
        aLandmarkView, aDbAccess, aLandmark, 
        aPartialSettings.RequestedAttributes(), ETrue, requestedFields );

    CleanupStack::PopAndDestroy( &requestedFields );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::AddCategoriesToLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TPosLmItemId>& aCategories,
    const TPosLmItemId& aLandmarkId)
    {
    if (aCategories.Count() > 0)
        {
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll,
            &KPosLmLandmarkCategoryTable);

        RDbView view;
        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EAddPreparation, view,
            *sql);

        for (TInt i = 0; i < aCategories.Count(); i++)
            {
            view.InsertL();
            view.SetColL(EPosLmLccLandmarkIdCol, aLandmarkId);
            view.SetColL(EPosLmLccCategoryIdCol, aCategories[i]);
            view.PutL();
            }

        CleanupStack::PopAndDestroy(2, sql); //&view
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::GetAllLandmarkIdsL(
    CPosLmLocalDbAccess& aDbAccess,
    TBool aUseSort,
    TBool aSortDescending,
    RArray<TPosLmItemId>& aLandmarkIds)
    {
    aLandmarkIds.Reset();

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

    if (aUseSort)
        {
        sql->Des().Format(KPosLmSqlSelectOrderByString, &KPosLmLandmarkIdCol,
            &KPosLmLandmarkTable, &KPosLmNameCol);

        if (aSortDescending)
            {
            sql->Des().Append(KPosLmSqlDescOrder);
            }
        }
    else
        {
        sql->Des().Format(KPosLmSqlSelect, &KPosLmLandmarkIdCol,
            &KPosLmLandmarkTable);
        }

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql, EDbCompareCollated);

    aLandmarkIds.ReserveL( view.CountL() );
    while ( view.NextL() )
        {
        view.GetL();
        aLandmarkIds.AppendL( view.ColUint32(1) );
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmLandmarkHandler::LandmarkExistsL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aLandmarkId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmLandmarkIdCol,
        &KPosLmLandmarkTable, &KPosLmLandmarkIdCol, aLandmarkId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TBool exists = !view.IsEmptyL();

    CleanupStack::PopAndDestroy(2, sql); //&view

    return exists;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLandmarkHandler::RemoveCategoryFromLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aCategoryId,
    TPosLmItemId aLandmarkId)
    {
    // Remove category from all landmark.
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteBy2Uint, &KPosLmLandmarkCategoryTable,
        &KPosLmCategoryIdCol, aCategoryId, &KPosLmLandmarkIdCol, aLandmarkId);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmLandmarkHandler::HasLandmarkSpecifiedCategoryL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aCategoryId,
    TPosLmItemId aLandmarkId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(
        KPosLmSqlFind2Uint, &KPosLmSqlAll, &KPosLmLandmarkCategoryTable,
        &KPosLmLandmarkIdCol, aLandmarkId, &KPosLmCategoryIdCol, aCategoryId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TBool hasCategory = !view.IsEmptyL();

    CleanupStack::PopAndDestroy(2, sql); //&view

    return hasCategory;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark,
    CPosLandmark::TAttributes aAttributes,
    TBool aHasRequestedFields,
    const RArray<TUint>& aRequestedFields )
    {
    RDbView view;
    OpenLandmarkViewLC( aLandmark.LandmarkId(), view, aDbAccess );

    view.GetL();
    ReadLandmarkL( 
        view, aDbAccess, aLandmark, aAttributes, aHasRequestedFields, aRequestedFields );

    CleanupStack::PopAndDestroy( &view );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadLandmarkL(
    RDbRowSet& aView,
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark,
    CPosLandmark::TAttributes aAttributes,
    TBool aHasRequestedFields,
    const RArray<TUint>& aRequestedFields )
    {
    if ( aAttributes & CPosLandmark::ELandmarkName )
        {
        ReadNameL( aView, aLandmark );
        }

    if ( aAttributes & CPosLandmark::EPosition )
        {
        ReadPositionL( aView, aLandmark );
        }

    if ( aAttributes & CPosLandmark::ECoverageRadius )
        {
        ReadCoverageRadiusL( aView, aLandmark );
        }

    if ( aAttributes & CPosLandmark::EDescription )
        {
        ReadDescriptionL( aView, aLandmark );
        }
    
    if ( aAttributes & CPosLandmark::EIcon )
        {
        ReadIconL( aDbAccess, aView, aLandmark );
        }

    if ( aAttributes & CPosLandmark::ECategoryInfo )
        {
        ReadCategoriesL( aDbAccess, aLandmark );
        }

    ReadFieldsL( aDbAccess, aLandmark, aHasRequestedFields, aRequestedFields );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadPositionL(
    RDbRowSet& aView,
    CPosLandmark& aLandmark)
    {
    if (!aView.IsColNull(EPosLmLcLatitudeCol) &&
        !aView.IsColNull(EPosLmLcLongitudeCol))
        {
        TLocality position;

        if (!aView.IsColNull(EPosLmLcAltitudeCol))
            {
            position.SetCoordinate(aView.ColReal64(EPosLmLcLatitudeCol),
                aView.ColReal64(EPosLmLcLongitudeCol),
                aView.ColReal32(EPosLmLcAltitudeCol));
            }
        else
            {
            position.SetCoordinate(aView.ColReal64(EPosLmLcLatitudeCol),
                aView.ColReal64(EPosLmLcLongitudeCol));
            }

        if (!aView.IsColNull(EPosLmLcHorizAccCol))
            {
            position.SetHorizontalAccuracy(
                aView.ColReal32(EPosLmLcHorizAccCol));
            }

        if (!aView.IsColNull(EPosLmLcVertAccCol))
            {
            position.SetVerticalAccuracy(aView.ColReal32(EPosLmLcVertAccCol));
            }

        aLandmark.SetPositionL(position);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadCoverageRadiusL(
    RDbRowSet& aView,
    CPosLandmark& aLandmark)
    {
    if (!aView.IsColNull(EPosLmLcCoverageRadiusCol))
        {
        aLandmark.SetCoverageRadius(aView.ColReal32(EPosLmLcCoverageRadiusCol));
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadNameL(
    RDbRowSet& aView,
    CPosLandmark& aLandmark)
    {
    if (!aView.IsColNull(EPosLmLcNameCol))
        {
        HBufC* nameBuffer = HBufC::NewLC(KPosLmMaxTextFieldLength);
        TPtr buffer = nameBuffer->Des();
        PosLmLongTextColHandler::ReadFromLongTextColumnL(buffer, aView,
            EPosLmLcNameCol);

        aLandmark.SetLandmarkNameL(*nameBuffer);

        CleanupStack::PopAndDestroy(nameBuffer);
        }
    else
        {
        aLandmark.SetLandmarkNameL(KNullDesC);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadDescriptionL(
	RDbRowSet& aView,
    CPosLandmark& aLandmark)
    {
    if (!aView.IsColNull(EPosLmLcDescriptionCol))
        {
        HBufC* description = HBufC::NewLC(KPosLmMaxDescriptionLength);
        TPtr descriptionBuffer = description->Des();
        PosLmLongTextColHandler::ReadFromLongTextColumnL(descriptionBuffer,
            aView, EPosLmLcDescriptionCol);

        aLandmark.SetLandmarkDescriptionL(*description);

        CleanupStack::PopAndDestroy(description);
        }
    else
        {
        aLandmark.SetLandmarkDescriptionL(KNullDesC);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadIconL(
    CPosLmLocalDbAccess& aDbAccess,
    RDbRowSet& aView,
    CPosLandmark& aLandmark)
    {
    if (!aView.IsColNull(EPosLmLcIconIdCol) &&
        !aView.IsColNull(EPosLmLcIconPicNrCol) &&
        !aView.IsColNull(EPosLmLcMaskPicNrCol))
        {
        // Get icon path
        HBufC* iconPath = HBufC::NewLC(KMaxFileName);
        TPtr path = iconPath->Des();

        if (PosLmIconHandler::GetIconPathL(aDbAccess,
            aView.ColUint32(EPosLmLcIconIdCol), path) == KErrNone)
            {
            aLandmark.SetIconL(*iconPath,
                aView.ColInt32(EPosLmLcIconPicNrCol),
                aView.ColInt32(EPosLmLcMaskPicNrCol));
            }

        CleanupStack::PopAndDestroy(iconPath);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadFieldsL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark,
    TBool aHasRequestedFields,
    const RArray<TUint>& aRequestedFields)
    {
    if (!aHasRequestedFields || aRequestedFields.Count() > 0)
        {
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll,
            &KPosLmLandmarkFieldTable, &KPosLmLandmarkIdCol,
            aLandmark.LandmarkId());

        RDbView view;
        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation,
            view, *sql);

        HBufC* textFieldBuffer = HBufC::NewLC(KPosLmMaxTextFieldLength);

        while (view.NextL())
            {
            view.GetL();

            TUint16 fieldType = view.ColUint16(EPosLmLfcFieldTypeCol);

            if (!aHasRequestedFields ||
                aRequestedFields.Find(fieldType) != KErrNotFound)
                {
                TPtr buffer = textFieldBuffer->Des();
                PosLmLongTextColHandler::ReadFromLongTextColumnL(
                    buffer, view, EPosLmLfcFieldStringCol);

                aLandmark.SetPositionFieldL(fieldType, *textFieldBuffer);
                }
            }

        CleanupStack::PopAndDestroy(3, sql); //&view, textFieldBuffer
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::ReadCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll,
        &KPosLmLandmarkCategoryTable, &KPosLmLandmarkIdCol,
        aLandmark.LandmarkId());

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    while (view.NextL())
        {
        view.GetL();
        aLandmark.AddCategoryL(view.ColUint32(EPosLmLccCategoryIdCol));
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::AddBasicLandmarkInfoL(
    CPosLmLocalDbAccess& aDbAccess,
    CPosLandmark& aLandmark)
    {
    TPtrC iconPath;
    TInt iconIndex;
    TInt maskIndex;
    TUint32 iconPathId = 0;

    // Fetch icon path id if the landmark has an icon.
    if (aLandmark.GetIcon(iconPath, iconIndex, maskIndex) == KErrNone)
        {
        PosLmIconHandler::GetIconPathIdL(aDbAccess, iconPath, iconPathId);
        }

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmLandmarkTable);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EAddPreparation, view, *sql);

    view.InsertL();

    SetPositionL(view, aLandmark);
    SetCoverageRadiusL(view, aLandmark);
    SetIconL(view, aLandmark, iconPathId);
    SetLandmarkNameL(view, aLandmark);
    SetLandmarkDescriptionL(view, aLandmark);

    view.PutL();

    // Fetch and set the landmark id
    PosLmImplExtension::SetLandmarkIdL(aLandmark,
        view.ColUint32(EPosLmLcLandmarkIdCol));

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::AddFieldsL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark)
    {
    TPositionFieldId fieldId = aLandmark.FirstPositionFieldId();

    RArray<TUint> fieldIds;
    CleanupClosePushL(fieldIds);

    while (fieldId != EPositionFieldNone)
        {
        User::LeaveIfError(fieldIds.Append(fieldId));
        fieldId = aLandmark.NextPositionFieldId(fieldId);
        }

    AddFieldsToLandmarkL(aDbAccess, fieldIds, aLandmark);

    CleanupStack::PopAndDestroy(&fieldIds);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::AddCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark)
    {
    RArray<TPosLmItemId> categoryIdArray;
    CleanupClosePushL(categoryIdArray);

    aLandmark.GetCategoriesL(categoryIdArray);

    RemoveNonExistingCategoriesL(aDbAccess, categoryIdArray);
    AddCategoriesToLandmarkL(aDbAccess, categoryIdArray,
        aLandmark.LandmarkId());

    CleanupStack::PopAndDestroy(&categoryIdArray);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::UpdateCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark,
    const CPosLandmark& aOldLandmark)
    {
    RArray<TPosLmItemId> categoryArray;
    CleanupClosePushL(categoryArray);
    aLandmark.GetCategoriesL(categoryArray);

    RArray<TPosLmItemId> oldCategoryArray;
    CleanupClosePushL(oldCategoryArray);
    aOldLandmark.GetCategoriesL(oldCategoryArray);

    RemoveNonExistingCategoriesL(aDbAccess, categoryArray);

    RArray<TPosLmItemId> toBeAdded;
    CleanupClosePushL(toBeAdded);

    TInt i;
    for (i = 0; i < categoryArray.Count(); i++)
        {
        if (oldCategoryArray.Find(categoryArray[i]) == KErrNotFound)
            {
            User::LeaveIfError(toBeAdded.Append(categoryArray[i]));
            }
        }

    AddCategoriesToLandmarkL(aDbAccess, toBeAdded, aLandmark.LandmarkId());
    CleanupStack::PopAndDestroy(&toBeAdded);

    RArray<TPosLmItemId> toBeRemoved;
    CleanupClosePushL(toBeRemoved);

    for (i = 0; i < oldCategoryArray.Count(); i++)
        {
        if (categoryArray.Find(oldCategoryArray[i]) == KErrNotFound)
            {
            User::LeaveIfError(toBeRemoved.Append(oldCategoryArray[i]));
            }
        }

    RemoveCategoriesFromLandmarkL(aDbAccess, toBeRemoved,
        aLandmark.LandmarkId());

    CleanupStack::PopAndDestroy(3, &categoryArray); //&oldCategoryArray
    // &toBeRemoved
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::UpdateFieldsL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark,
    const CPosLandmark& aOldLandmark)
    {
    RArray<TUint> toBeChanged;
    CleanupClosePushL(toBeChanged);

    RArray<TUint> toBeAdded;
    CleanupClosePushL(toBeAdded);

    TPositionFieldId positionFieldId = aLandmark.FirstPositionFieldId();

    while (positionFieldId != EPositionFieldNone)
        {
        if (aOldLandmark.IsPositionFieldAvailable(positionFieldId))
            {
            TPtrC fieldValue;
            TPtrC oldFieldValue;

            User::LeaveIfError(aLandmark.GetPositionField(positionFieldId,
                fieldValue));
            User::LeaveIfError(aOldLandmark.GetPositionField(positionFieldId,
                oldFieldValue));

            if (fieldValue.Compare(oldFieldValue) != 0)
                {
                User::LeaveIfError(toBeChanged.Append(positionFieldId));
                }
            }
        else
            {
            User::LeaveIfError(toBeAdded.Append(positionFieldId));
            }

        positionFieldId = aLandmark.NextPositionFieldId(positionFieldId);
        }

    AddFieldsToLandmarkL(aDbAccess, toBeAdded, aLandmark);
    CleanupStack::PopAndDestroy(&toBeAdded);

    UpdateFieldsForLandmarkL(aDbAccess, toBeChanged, aLandmark);
    CleanupStack::PopAndDestroy(&toBeChanged);

    RArray<TUint> toBeRemoved;
    CleanupClosePushL(toBeRemoved);

    positionFieldId = aOldLandmark.FirstPositionFieldId();

    while (positionFieldId != EPositionFieldNone)
        {
        if (!aLandmark.IsPositionFieldAvailable(positionFieldId))
            {
            User::LeaveIfError(toBeRemoved.Append(positionFieldId));
            }

        positionFieldId = aOldLandmark.NextPositionFieldId(positionFieldId);
        }

    RemoveFieldsFromLandmarkL(aDbAccess, toBeRemoved, aLandmark);
    CleanupStack::PopAndDestroy(&toBeRemoved);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::UpdateIconAndBasicLandmarkInfoL(
    CPosLmLocalDbAccess& aDbAccess,
    const CPosLandmark& aLandmark,
    const CPosLandmark& aOldLandmark)
    {
    if (!CheckIfIconAndBasicInfoAreEqualL(aLandmark, aOldLandmark))
        {
        TUint32 oldIconId;
        TBool oldIconExists = ReadIconPathIdL(aDbAccess, aLandmark.LandmarkId(),
            oldIconId);

        TPtrC path;
        TInt iconIndex;
        TInt maskIndex;
        TUint32 iconId = 0;

        if (aLandmark.GetIcon(path, iconIndex, maskIndex) == KErrNone)
            {
            PosLmIconHandler::GetIconPathIdL(aDbAccess, path, iconId);
            }

        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlFindUint, &KPosLmSqlAll,
            &KPosLmLandmarkTable, &KPosLmLandmarkIdCol, aLandmark.LandmarkId());

        RDbView view;
        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation,
            view, *sql);

        if (view.NextL())
            {
            view.UpdateL();

            SetIconL(view, aLandmark, iconId);
            SetPositionL(view, aLandmark);
            SetCoverageRadiusL(view, aLandmark);
            SetLandmarkNameL(view, aLandmark);
            SetLandmarkDescriptionL(view, aLandmark);

            view.PutL();
            }
        else
            {
            User::Leave(KErrNotFound);
            }

        CleanupStack::PopAndDestroy(2, sql); //&view

        if (oldIconExists)
            {
            PosLmIconHandler::RemoveIconIfNotUsedL(aDbAccess, oldIconId);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmLandmarkHandler::CheckIfIconAndBasicInfoAreEqualL(
    const CPosLandmark& aLandmark,
    const CPosLandmark& aLandmarkInDb)
    {
    TBool equal = ETrue;

    TPtrC str1;
    TPtrC str2;
    TInt icon1;
    TInt icon2;
    TInt mask1;
    TInt mask2;
    TLocality pos1;
    TLocality pos2;
    TReal32 cov1;
    TReal32 cov2;

    if (aLandmark.GetIcon(str1, icon1, mask1) !=
        aLandmarkInDb.GetIcon(str2, icon2, mask2) ||
        (aLandmark.GetIcon(str1, icon1, mask1) == KErrNone &&
        (icon1 != icon2 || mask1 != mask2 || str1.Compare(str2) != 0)))
        { // Check icon
        equal = EFalse;
        }
    else if (aLandmark.GetPosition(pos1) != aLandmarkInDb.GetPosition(pos2) ||
        (aLandmark.GetPosition(pos1) == KErrNone && (
        !PosLmTrealCompare::IsEqual(pos1.Latitude(), pos2.Latitude()) ||
        !PosLmTrealCompare::IsEqual(pos1.Longitude(), pos2.Longitude()) ||
        !PosLmTrealCompare::IsEqual(pos1.Altitude(), pos2.Altitude()) ||
        !PosLmTrealCompare::IsEqual(pos1.HorizontalAccuracy(),
            pos2.HorizontalAccuracy()) ||
        !PosLmTrealCompare::IsEqual(pos1.VerticalAccuracy(),
            pos2.VerticalAccuracy())
        )))
        { // Check position
        equal = EFalse;
        }
    else if (aLandmark.GetCoverageRadius(cov1) !=
        aLandmarkInDb.GetCoverageRadius(cov2) ||
        (aLandmark.GetCoverageRadius(cov1) == KErrNone &&
        !PosLmTrealCompare::IsEqual(cov1, cov2)))
        { // Check coverage radius
        equal = EFalse;
        }
    else if (aLandmarkInDb.GetLandmarkName(str2) != KErrNone ||
        (aLandmark.GetLandmarkName(str1) != KErrNone &&
            str2.Compare(KNullDesC) != 0) ||
        (aLandmark.GetLandmarkName(str1) == KErrNone &&
            str2.Compare(str1) != 0))
        { // Check name
        equal = EFalse;
        }
    else if (aLandmarkInDb.GetLandmarkDescription(str2) != KErrNone ||
        (aLandmark.GetLandmarkDescription(str1) != KErrNone &&
            str2.Compare(KNullDesC) != 0) ||
        (aLandmark.GetLandmarkDescription(str1) == KErrNone &&
            str2.Compare(str1) != 0))
        { // Check description
        equal = EFalse;
        }

    return equal;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::RemoveNonExistingCategoriesL(
    CPosLmLocalDbAccess& aDbAccess,
    RArray<TPosLmItemId>& aCategoryIdArray)
    {
    RArray<TPosLmItemId> categoriesInDatabase;
    CleanupClosePushL(categoriesInDatabase);

    PosLmCategoryHandler::GetAllCategoryIdsL(aDbAccess,
        CPosLmCategoryManager::ECategorySortOrderNone, categoriesInDatabase);

    if (categoriesInDatabase.Count() == 0)
        {
        aCategoryIdArray.Reset();
        }

    for (TInt i = 0; i < aCategoryIdArray.Count();)
        {
        if (categoriesInDatabase.Find(aCategoryIdArray[i]) == KErrNotFound)
            {
            aCategoryIdArray.Remove(i);
            }
        else
            {
            i++;
            }
        }

    CleanupStack::PopAndDestroy(&categoriesInDatabase);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::RemoveCategoriesFromLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TPosLmItemId>& aCategories,
    const TPosLmItemId& aLandmarkId)
    {
    for (TInt i = 0; i < aCategories.Count(); i++)
        {
        RemoveCategoryFromLandmarkL(aDbAccess, aCategories[i], aLandmarkId);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::AddFieldsToLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TUint>& aFieldIds,
    const CPosLandmark& aLandmark)
    {
    if (aFieldIds.Count() > 0)
        {
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll,
            &KPosLmLandmarkFieldTable);

        RDbView view;
        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EAddPreparation, view,
            *sql);

        for (TInt i = 0; i < aFieldIds.Count(); i++)
            {
            InsertFieldL(view, (TPositionFieldId)aFieldIds[i], aLandmark);
            }

        CleanupStack::PopAndDestroy(2, sql); //&view
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::UpdateFieldsForLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TUint>& aFieldIds,
    const CPosLandmark& aLandmark)
    {
    for (TInt i = 0; i < aFieldIds.Count(); i++)
        {
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlFind2Uint, &KPosLmSqlAll,
            &KPosLmLandmarkFieldTable, &KPosLmLandmarkIdCol,
            aLandmark.LandmarkId(), &KPosLmFieldTypeCol, aFieldIds[i]);

        RDbView view;
        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation,
            view, *sql);

        TPtrC fieldValue;
        User::LeaveIfError(aLandmark.GetPositionField(
            (TPositionFieldId)aFieldIds[i], fieldValue));

        if (view.NextL())
            {
            view.UpdateL();
            PosLmLongTextColHandler::WriteToLongTextColumnL(fieldValue, view,
                EPosLmLfcFieldStringCol);
            view.PutL();
            }

        CleanupStack::PopAndDestroy(2, sql); //&view
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::RemoveFieldsFromLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    const RArray<TUint>& aFieldIds,
    const CPosLandmark& aLandmark)
    {
    for (TInt i = 0; i < aFieldIds.Count(); i++)
        {
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
        sql->Des().Format(KPosLmSqlDeleteBy2Uint, &KPosLmLandmarkFieldTable,
            &KPosLmLandmarkIdCol, aLandmark.LandmarkId(), &KPosLmFieldTypeCol,
            aFieldIds[i]);

        aDbAccess.ExecuteL(*sql);
        CleanupStack::PopAndDestroy(sql);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::InsertFieldL(
    RDbView& view,
    const TPositionFieldId& aFieldId,
    const CPosLandmark& aLandmark)
    {
    TPtrC fieldValue;

    User::LeaveIfError(aLandmark.GetPositionField(aFieldId, fieldValue));

    view.InsertL();
    view.SetColL(EPosLmLfcLandmarkIdCol, aLandmark.LandmarkId());
    view.SetColL(EPosLmLfcFieldTypeCol, (TUint)aFieldId);
    PosLmLongTextColHandler::WriteToLongTextColumnL(fieldValue, view,
        EPosLmLfcFieldStringCol);
    view.PutL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::SetPositionL(
    RDbView& aView,
    const CPosLandmark& aLandmark)
    {
    TLocality position;

    if (aLandmark.GetPosition(position) == KErrNone)
        {
        aView.SetColL(EPosLmLcLatitudeCol, position.Latitude());
        aView.SetColL(EPosLmLcLongitudeCol, position.Longitude());

        if (!Math::IsNaN(position.Altitude()))
            {
            aView.SetColL(EPosLmLcAltitudeCol, position.Altitude());
            }
        else
            {
            aView.SetColNullL(EPosLmLcAltitudeCol);
            }

        if (!Math::IsNaN(position.HorizontalAccuracy()))
            {
            aView.SetColL(EPosLmLcHorizAccCol, position.HorizontalAccuracy());
            }
        else
            {
            aView.SetColNullL(EPosLmLcHorizAccCol);
            }

        if (!Math::IsNaN(position.VerticalAccuracy()))
            {
            aView.SetColL(EPosLmLcVertAccCol, position.VerticalAccuracy());
            }
        else
            {
            aView.SetColNullL(EPosLmLcVertAccCol);
            }
        }
    else
        {
        aView.SetColNullL(EPosLmLcLatitudeCol);
        aView.SetColNullL(EPosLmLcLongitudeCol);
        aView.SetColNullL(EPosLmLcAltitudeCol);
        aView.SetColNullL(EPosLmLcHorizAccCol);
        aView.SetColNullL(EPosLmLcVertAccCol);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::SetCoverageRadiusL(
    RDbView& aView,
    const CPosLandmark& aLandmark)
    {
    TReal32 covRadius;

    if (aLandmark.GetCoverageRadius(covRadius) == KErrNone)
        {
        aView.SetColL(EPosLmLcCoverageRadiusCol, covRadius);
        }
    else
        {
        aView.SetColNullL(EPosLmLcCoverageRadiusCol);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::SetLandmarkNameL(
    RDbView& aView,
    const CPosLandmark& aLandmark)
    {
    TPtrC name;

    if (aLandmark.GetLandmarkName(name) == KErrNone)
        {
        PosLmLongTextColHandler::WriteToLongTextColumnL(name, aView,
            EPosLmLcNameCol);
        }
    else
        {
        aView.SetColNullL(EPosLmLcNameCol);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::SetLandmarkDescriptionL(
    RDbView& aView,
    const CPosLandmark& aLandmark)
    {
    TPtrC description;

    if (aLandmark.GetLandmarkDescription(description) == KErrNone)
        {
        PosLmLongTextColHandler::WriteToLongTextColumnL(description, aView,
            EPosLmLcDescriptionCol);
        }
    else
        {
        aView.SetColNullL(EPosLmLcDescriptionCol);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::SetIconL(
    RDbView& aView,
    const CPosLandmark& aLandmark,
    const TUint32 aIconPathId)
    {
    TPtrC path;
    TInt iconIndex;
    TInt maskIndex;

    if (aLandmark.GetIcon(path, iconIndex, maskIndex) == KErrNone)
        {
        aView.SetColL(EPosLmLcIconIdCol, aIconPathId);
        aView.SetColL(EPosLmLcIconPicNrCol, iconIndex);
        aView.SetColL(EPosLmLcMaskPicNrCol, maskIndex);
        }
    else
        {
        aView.SetColNullL(EPosLmLcIconIdCol);
        aView.SetColNullL(EPosLmLcIconPicNrCol);
        aView.SetColNullL(EPosLmLcMaskPicNrCol);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmLandmarkHandler::ReadIconPathIdL(
    CPosLmLocalDbAccess& aDbAccess,
    const TPosLmItemId& aLandmarkId,
    TUint32& aIconPathId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmIconIdCol,
        &KPosLmLandmarkTable, &KPosLmLandmarkIdCol, aLandmarkId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TBool iconIsSet = EFalse;

    if (view.NextL())
        {
        view.GetL();

        if (!view.IsColNull(1))
            {
            aIconPathId = view.ColUint32(1);
            iconIsSet = ETrue;
            }
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    return iconIsSet;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::RemoveAllCategoriesFromLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aLandmarkId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmLandmarkCategoryTable,
        &KPosLmLandmarkIdCol, aLandmarkId);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::RemoveAllFieldsFromLandmarkL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aLandmarkId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmLandmarkFieldTable,
        &KPosLmLandmarkIdCol, aLandmarkId);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmLandmarkHandler::RemoveBasicLandmarkInfoL(
    CPosLmLocalDbAccess& aDbAccess,
    TPosLmItemId aLandmarkId,
    TUint32& aIconPathId)
    {
    // Get icon if it exists.
    TBool hasIcon = ReadIconPathIdL(aDbAccess, aLandmarkId, aIconPathId);

    // Remove landmark
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmLandmarkTable,
        &KPosLmLandmarkIdCol, aLandmarkId);

    aDbAccess.ExecuteL(*sql);

    CleanupStack::PopAndDestroy(sql);

    return hasIcon;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TUint PosLmLandmarkHandler::TotalLandmarkCountL( CPosLmLocalDbAccess& aDbAccess )
    {
    return PosLmDatabaseUtility::GetTableRowCountL( aDbAccess, KPosLmLandmarkTable );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmLandmarkHandler::OpenLandmarkViewLC(
    TPosLmItemId aLandmarkId,
    RDbView& aView,
    CPosLmLocalDbAccess& aDbAccess )
    {
    HBufC* sql = HBufC::NewLC( KPosLmSqlStatementMaxLen );
    sql->Des().Format( KPosLmSqlFindUint, &KPosLmSqlAll, &KPosLmLandmarkTable,
        &KPosLmLandmarkIdCol, aLandmarkId );

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
