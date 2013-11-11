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
* Description: 
*   ?description_line
*
*/


// INCLUDE FILES
#include <f32file.h>
#include "FT_DatabaseUtility.h"

#include <D32DBMS.H>

#include <EPos_Landmarks.h>
// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// Code below taken from EPos_PosLmDatabaseManager.cpp
// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateDatabaseL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateDatabaseL(const TDesC& aDbPath, TBool aReplaceDatabase, TBool aWrongId, TBool aCreateInvalidIndex)
    {
    RDbs dbSession;
    CleanupClosePushL(dbSession);

    User::LeaveIfError(dbSession.Connect());

    TPtrC dbPath(aDbPath);
    

    RDbNamedDatabase db;
    CleanupClosePushL(db);

    
    TUid KPosSecureUidWrongId = { 0x10204058 };
    _LIT(KWrongIdString, "secure[10204058]");
    
    if (aReplaceDatabase)
        {
        TInt err =KErrNone;
        
        if (!aWrongId) err = dbSession.DeleteDatabase(dbPath, KPosLmDbSecureUid);
        else err = dbSession.DeleteDatabase(dbPath, KPosSecureUidWrongId);
        
        if (err == KErrNone || err == KErrNotFound)
            {
            if (!aWrongId) User::LeaveIfError(db.Create(dbSession, dbPath, KPosLmDbSecureFormat));
            else User::LeaveIfError(db.Create(dbSession, dbPath, KWrongIdString));
            }
        else
            {
            User::Leave(err);
            }
        }
    else
        {
        if (!aWrongId) 
        {
		User::LeaveIfError(db.Create(dbSession, dbPath, KPosLmDbSecureFormat));
		
        }
        else User::LeaveIfError(db.Create(dbSession, dbPath, KWrongIdString));
        }
        
    CreateTablesL(db, aCreateInvalidIndex);
    
    CreateIndexesL(db);
    
    SetCounterToOneL(db, KPosLmLandmarkTable);
    SetCounterToOneL(db, KPosLmCategoryTable);

    // The settings table is created last so that a database can be validated 
    // when opening it (there will be a check for language there which is a 
    // part of the settings table).
    CreateSettingsTableL(db);

    CleanupStack::PopAndDestroy(2, &dbSession); // &db
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateTablesL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateTablesL(
    RDbNamedDatabase& aDb, TBool aCreateInvalidIndex)
    {
    CreateLandmarkTableL(aDb, aCreateInvalidIndex);
    CreateLandmarkFieldTableL(aDb);
    CreateCategoryTableL(aDb, aCreateInvalidIndex);
    CreateLmCategoryTableL(aDb);
    CreateIconTableL(aDb);

    // Do not create settings table here. This is created in the end so that the
    // database can be validated when opening it.
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateLandmarkTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateLandmarkTableL(
    RDbNamedDatabase& aDb, TBool aCreateInvalidIndex)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    TDbCol col(KPosLmLandmarkIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    columns->AddL(col);

    columns->AddL(TDbCol(KPosLmLatitudeCol, EDbColReal64));
    columns->AddL(TDbCol(KPosLmLongitudeCol, EDbColReal64));
    columns->AddL(TDbCol(KPosLmAltitudeCol, EDbColReal32));
        	
    // Dont create the following tables when creating invalid table
    if (!aCreateInvalidIndex)
    	{
    	columns->AddL(TDbCol(KPosLmHorizAccCol, EDbColReal32));
        columns->AddL(TDbCol(KPosLmVertAccCol, EDbColReal32));
        columns->AddL(TDbCol(KPosLmCoverageRadiusCol, EDbColReal32));
        columns->AddL(TDbCol(KPosLmIconIdCol, EDbColUint32));
		columns->AddL(TDbCol(KPosLmIconPicNrCol, EDbColInt32));
        columns->AddL(TDbCol(KPosLmMaskPicNrCol, EDbColInt32));
	    columns->AddL(TDbCol(KPosLmNameCol, EDbColLongText, 
        	KPosLmMaxTextFieldLength));
    	}
	
    columns->AddL(TDbCol(KPosLmNDescCol, EDbColLongText, 
        	KPosLmMaxDescriptionLength));

    User::LeaveIfError(aDb.CreateTable(KPosLmLandmarkTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateLandmarkFieldTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateLandmarkFieldTableL(
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
// FTDatabaseUtility::CreateCategoryTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateCategoryTableL(
    RDbNamedDatabase& aDb, TBool aCreateInvalidIndex)
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

    // Dont create the following tables when creating invalid table
    if (!aCreateInvalidIndex)
    	{
        columns->AddL(TDbCol(KPosLmCategoryNameModifiedCol, EDbColBit));
        }

    User::LeaveIfError(aDb.CreateTable(KPosLmCategoryTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateLmCategoryTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateLmCategoryTableL(
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
// FTDatabaseUtility::CreateIconTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateIconTableL(
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
// FTDatabaseUtility::CreateSettingsTableL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateSettingsTableL(
    RDbNamedDatabase& aDb)
    {
    CDbColSet* columns = CDbColSet::NewLC();

    columns->AddL(TDbCol(KPosLmLanguageCol, EDbColInt32));
    columns->AddL(TDbCol(KPosLmInitFlagCol, EDbColInt32));

    User::LeaveIfError(aDb.CreateTable(KPosLmSettingsTable, *columns));
    CleanupStack::PopAndDestroy(columns);
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateIndexesL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateIndexesL(
    RDbNamedDatabase& aDb)
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
    }

// -----------------------------------------------------------------------------
// FTDatabaseUtility::CreateIndexL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::CreateIndexL(
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
// FTDatabaseUtility::SetCounterToOneL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void FTDatabaseUtility::SetCounterToOneL(
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


//  End of File  
