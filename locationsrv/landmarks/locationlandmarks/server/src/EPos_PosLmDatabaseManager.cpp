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
* Description: Provides static functions which handles database creation
*								and deletion.
*
*
*/


#include <d32dbms.h>
#include <EPos_Landmarks.h>
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmdatabaseutility.h>
#include "EPos_PosLmDatabaseManager.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::DeleteDatabaseL(
    const TDesC& aUri)
    {
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    TPtrC dbPath(aUri);
    PosLmDatabaseUtility::RemoveProtocolFromUriL(dbPath);

    User::LeaveIfError(dbSession.DeleteDatabase(dbPath, KPosLmDbSecureUid));

    CleanupStack::PopAndDestroy(&dbSession);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateDatabaseL(
    const TDesC& aUri,
    TBool aDefaultDatabase,
    TBool aReplaceDatabase)
    {
    RDbs dbSession;
    CleanupClosePushL(dbSession);

    User::LeaveIfError(dbSession.Connect());

    TPtrC dbPath(aUri);
    PosLmDatabaseUtility::RemoveProtocolFromUriL(dbPath);

    RDbNamedDatabase db;
    CleanupClosePushL(db);

    if (aDefaultDatabase && aReplaceDatabase)
        {
        TInt delerr = dbSession.DeleteDatabase(dbPath, KPosLmDbSecureUid);
        if (delerr != KErrNone && delerr != KErrNotFound)
            {
            User::Leave(delerr);
            }
        }

    TInt createerr = db.Create(dbSession, dbPath, KPosLmDbSecureFormat);
    if (!aDefaultDatabase ||
        (aDefaultDatabase && createerr != KErrAlreadyExists))
        {
        User::LeaveIfError(createerr);
        }


    if (createerr == KErrNone)
        {
        CreateTablesL(db);
        CreateIndexesL(db);
        SetCounterToOneL(db, KPosLmLandmarkTable);
        SetCounterToOneL(db, KPosLmCategoryTable);

        // The settings table is created last so that a database can be
        // validated when opening it (there will be a check for language there
        // which is a part of the settings table).
        CreateSettingsTableL(db);
        }

    CleanupStack::PopAndDestroy(2, &dbSession); // &db
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CopyDatabaseL(
    const TDesC& aSourceUri,
    const TDesC& aTargetUri)
    {
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    TPtrC sourceDbPath(aSourceUri);
    TPtrC targetDbPath(aTargetUri);
    PosLmDatabaseUtility::RemoveProtocolFromUriL(sourceDbPath);
    PosLmDatabaseUtility::RemoveProtocolFromUriL(targetDbPath);

    User::LeaveIfError(dbSession.CopyDatabase(sourceDbPath,
                                              targetDbPath,
                                              KPosLmDbSecureUid));
    CleanupStack::PopAndDestroy(&dbSession);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateTablesL(
    RDbNamedDatabase& aDb)
    {
    CreateLandmarkTableL(aDb);
    CreateLandmarkFieldTableL(aDb);
    CreateCategoryTableL(aDb);
    CreateLmCategoryTableL(aDb);
    CreateIconTableL(aDb);

    // Do not create settings table here. This is created in the end so that the
    // database can be validated when opening it.
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateLandmarkTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmLandmarkIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    columns->AddL(col);

    columns->AddL(TDbCol(KPosLmLatitudeCol, EDbColReal64));
    columns->AddL(TDbCol(KPosLmLongitudeCol, EDbColReal64));
    columns->AddL(TDbCol(KPosLmAltitudeCol, EDbColReal32));
    columns->AddL(TDbCol(KPosLmHorizAccCol, EDbColReal32));
    columns->AddL(TDbCol(KPosLmVertAccCol, EDbColReal32));
    columns->AddL(TDbCol(KPosLmCoverageRadiusCol, EDbColReal32));
    columns->AddL(TDbCol(KPosLmIconIdCol, EDbColUint32));
    columns->AddL(TDbCol(KPosLmIconPicNrCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmMaskPicNrCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmNameCol, EDbColLongText,
        KPosLmMaxTextFieldLength));
    columns->AddL(TDbCol(KPosLmNDescCol, EDbColLongText,
        KPosLmMaxDescriptionLength));

    User::LeaveIfError(aDb.CreateTable(KPosLmLandmarkTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateLandmarkFieldTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmLandmarkIdCol, EDbColUint32);
    col.iAttributes = col.ENotNull;
    columns->AddL(TDbCol(col));

    TDbCol col2(KPosLmFieldTypeCol, EDbColUint16);
    col2.iAttributes = col2.ENotNull;
    columns->AddL(col2);

    columns->AddL(TDbCol(KPosLmFieldStringCol, EDbColLongText,
        KPosLmMaxTextFieldLength));

    User::LeaveIfError(aDb.CreateTable(KPosLmLandmarkFieldTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateCategoryTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmCategoryIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    columns->AddL(col);

    columns->AddL(TDbCol(KPosLmGlobalCategoryIdCol, EDbColUint16));
    columns->AddL(TDbCol(KPosLmIconIdCol, EDbColUint32));
    columns->AddL(TDbCol(KPosLmIconPicNrCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmMaskPicNrCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmCategoryNameCol, EDbColText,
        KPosLmMaxCategoryNameLength));
    columns->AddL(TDbCol(KPosLmCategoryNameModifiedCol, EDbColBit));

    User::LeaveIfError(aDb.CreateTable(KPosLmCategoryTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateLmCategoryTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmLandmarkIdCol, EDbColUint32);
    col.iAttributes = col.ENotNull;
    columns->AddL(col);

    TDbCol col2(KPosLmCategoryIdCol, EDbColUint32);
    col2.iAttributes = col2.ENotNull;
    columns->AddL(col2);

    User::LeaveIfError(aDb.CreateTable(KPosLmLandmarkCategoryTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateIconTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmIconIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    columns->AddL(col);

    columns->AddL(TDbCol(KPosLmIconPathCol, EDbColLongText, KMaxFileName));

    User::LeaveIfError(aDb.CreateTable(KPosLmIconTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateSettingsTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    columns->AddL(TDbCol(KPosLmLanguageCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmInitFlagCol, EDbColInt32));

    User::LeaveIfError(aDb.CreateTable(KPosLmSettingsTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateIndexTableL( RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    columns->AddL( TDbCol( KPosLmLanguageCol, EDbColInt32 ) );
    columns->AddL( TDbCol( KPosLmTimestampCol, EDbColDateTime ) );
    columns->AddL( TDbCol( KPosLmIndexDataCol, EDbColLongBinary ) );

    User::LeaveIfError( aDb.CreateTable( KPosLmIndexTable, *columns ) );
    CleanupStack::PopAndDestroy( columns );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateIndexesL( RDbNamedDatabase& aDb )
    {
    CreateIndexL(aDb, KPosLmLandmarkIdIndex, KPosLmLandmarkTable,
        KPosLmLandmarkIdCol, ETrue);
    CreateIndexL(aDb, KPosLmCategoryIdIndex, KPosLmCategoryTable,
        KPosLmCategoryIdCol, ETrue);
    CreateIndexL(aDb, KPosLmGlobalCategoryIdIndex, KPosLmCategoryTable,
        KPosLmGlobalCategoryIdCol, EFalse);
    CreateIndexL(aDb, KPosLmCategoryNameIndex, KPosLmCategoryTable,
        KPosLmCategoryNameCol, ETrue);
    CreateIndexL(aDb, KPosLmIconIdIndex, KPosLmIconTable,
        KPosLmIconIdCol, ETrue);
    CreateIndexL(aDb, KPosLmFieldsLmIdIndex, KPosLmLandmarkFieldTable,
        KPosLmLandmarkIdCol, EFalse);
    CreateIndexL(aDb, KPosLmCategoryLmIdIndex, KPosLmLandmarkCategoryTable,
        KPosLmLandmarkIdCol, EFalse);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::CreateIndexL(
    RDbNamedDatabase& aDb,
    const TDesC& aIndex,
    const TDesC& aTable,
    const TDesC& aColumn,
    TBool aUnique)
    {
    CDbKey* keys = CDbKey::NewLC();
    keys->AddL(TDbKeyCol(aColumn));

    if (aUnique)
        {
        keys->MakeUnique();
        }

    User::LeaveIfError(aDb.CreateIndex(aIndex, aTable, *keys));
    CleanupStack::PopAndDestroy(keys);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmDatabaseManager::SetCounterToOneL(
    RDbNamedDatabase& aDb,
    const TDesC& aTable)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll, &aTable);

    RDbView view;
    CleanupClosePushL(view);
    User::LeaveIfError(view.Prepare(aDb, TDbQuery(*sql),
        TDbWindow::EUnlimited));

    view.InsertL();
    view.PutL();

    if (!view.FirstL())
        {
        User::Leave(KErrGeneral);
        }

    view.DeleteL();

    CleanupStack::PopAndDestroy(2, sql); //&view
    }
