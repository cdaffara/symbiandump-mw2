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
* Description: handles icon paths in the database.
*
*
*/


// INCLUDE FILES
#include    <d32dbms.h>
#include    "EPos_PosLmIconHandler.h"
#include    "EPos_CPosLmLocalDbAccess.h"
#include    "EPos_PosLmLongTextColHandler.h"
#include    "EPos_LandmarkDatabaseStructure.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// PosLmIconHandler::GetIconPathIdL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmIconHandler::GetIconPathIdL(
    CPosLmLocalDbAccess& aDbAccess,
    const TDesC& aIconPath,
    TUint32& aIconPathId)
    {
    // First search icon table for icon path
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen+KMaxFileName);
    sql->Des().Format(KPosLmSqlFindString, &KPosLmIconIdCol, &KPosLmIconTable,
        &KPosLmIconPathCol, &aIconPath);

    RDbView view;
    aDbAccess.PrepareViewLC(
        CPosLmLocalDbAccess::EUpdatablePreparation, view, *sql);

    if (!view.NextL())
        {
        // Icon path not found. Add icon path to database.
        AddIconPathL(aDbAccess, aIconPath, aIconPathId);
        }
    else
        {
        // Icon path found. Fetch number.
        view.GetL();
        aIconPathId = view.ColUint32(1);
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// PosLmIconHandler::GetIconPathL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmIconHandler::GetIconPathL(
    CPosLmLocalDbAccess& aDbAccess,
    TUint32 aIconPathId,
    TDes& aIconPath)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmIconPathCol, &KPosLmIconTable,
        &KPosLmIconIdCol, aIconPathId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    TInt iconPathFound = KErrNotFound;

    if (view.FirstL())
        {
        view.GetL();
        PosLmLongTextColHandler::ReadFromLongTextColumnL(aIconPath, view, 1);
        iconPathFound = KErrNone;
        }

    CleanupStack::PopAndDestroy(2, sql); //&view

    return iconPathFound;
    }

// -----------------------------------------------------------------------------
// PosLmIconHandler::RemoveIconIfNotUsedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmIconHandler::RemoveIconIfNotUsedL(
    CPosLmLocalDbAccess& aDbAccess,
    const TUint32& aIconPathId)
    {
    // First check landmark table
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlFindUint, &KPosLmIconIdCol, &KPosLmLandmarkTable,
        &KPosLmIconIdCol, aIconPathId);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation, view,
        *sql);

    if (view.IsEmptyL())
        {
        // Not found, check category table
        CleanupStack::PopAndDestroy(&view); // Delete old view

        sql->Des().Format(KPosLmSqlFindUint, &KPosLmIconIdCol,
            &KPosLmCategoryTable, &KPosLmIconIdCol, aIconPathId);

        aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EUpdatablePreparation,
            view, *sql);

        if (view.IsEmptyL())
            {
            // Not found, remove icon path because it is not longer in use.
            sql->Des().Format(KPosLmSqlDeleteUint, &KPosLmIconTable,
                &KPosLmIconIdCol, aIconPathId);

            aDbAccess.ExecuteL(*sql);
            }
        }

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// PosLmIconHandler::AddIconPathL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmIconHandler::AddIconPathL(
    CPosLmLocalDbAccess& aDbAccess,
    const TDesC& aIconPath,
    TUint32& aIconPathId)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmIconTable);

    RDbView view;
    aDbAccess.PrepareViewLC(CPosLmLocalDbAccess::EAddPreparation, view, *sql);

    view.InsertL();
    PosLmLongTextColHandler::WriteToLongTextColumnL(aIconPath, view,
        EPosLmIcIconPathCol);
    view.PutL();

    aIconPathId = view.ColUint32(EPosLmIcIconIdCol);

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

//  End of File
