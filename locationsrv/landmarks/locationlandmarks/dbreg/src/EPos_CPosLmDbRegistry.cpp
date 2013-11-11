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
* Description: Registry for storing landmark database information.
*
*
*/


// INCLUDE FILES
#include <d32dbms.h>
#include <EPos_Landmarks.h>
#include <epos_poslmdatabaseutility.h>
#include <EPos_TPosLmDatabaseSettings.h>
#include "EPos_CPosLmDbRegistry.h"
#include "EPos_PosDbRegistryConstants.h"
#include "EPos_CPosLmDatabaseIterator.h"

//CONSTANTS
const TInt KPosLmSqlStatementMaxLen = 256;

//If a URI including the sql statement exceed KPosLmSqlStatementMaxLen
//a panic will be raised. The sql statement used in the function
//FindLongEntryL excluding the URI is approx. 55 characters. Creating a
//sql statement with a URI of max 180 characters will therefor not exceed
//KPosLmSqlStatementMaxLen.
const TInt KPosLmSqlStatementUriMaxLen = 180;

struct TDeleteFile
    {
    RFs* iFs;
    TPtrC iFilename;
    };

// ==================== LOCAL FUNCTIONS ====================

// ---------------------------------------------------------
// Deletes the database if anything goes
// wrong during database construction.
// ---------------------------------------------------------
//
void DeleteFileCleanupItem(TAny* aParam)
    {
    TDeleteFile* obj = reinterpret_cast<TDeleteFile*>(aParam);
    obj->iFs->Delete(obj->iFilename);
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::CPosLmDbRegistry
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDbRegistry::CPosLmDbRegistry()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmDbRegistry::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDbRegistry* CPosLmDbRegistry::NewL()
    {
    CPosLmDbRegistry* self = new (ELeave) CPosLmDbRegistry;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CPosLmDbRegistry::~CPosLmDbRegistry()
    {
    iDb.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::CreateL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::CreateL(
    RFs& aFileSession,
    const TDesC& aFileName)
    {
    User::LeaveIfError(iDb.Create(aFileSession, aFileName));

    TDeleteFile* del = new (ELeave) TDeleteFile;
    del->iFs = &aFileSession;
    del->iFilename.Set(aFileName);

    CleanupStack::PushL(TCleanupItem(DeleteFileCleanupItem, del));

    //Create the database table
    CDbColSet* columns = CDbColSet::NewLC();

    columns->AddL(TDbCol(KPosLmProtocolCol, EDbColText, KProtocolMaxLength));
    columns->AddL(TDbCol(KPosLmFileNameCol, EDbColLongText));
    columns->AddL(TDbCol(KPosLmSettingsCol, EDbColLongText));
    User::LeaveIfError(iDb.CreateTable(KPosLmDbRegistryTable, *columns));

    CleanupStack::PopAndDestroy(columns);

    //Create index on the protocol field
    _LIT(KPosSqlCreateIndex, "CREATE INDEX %S ON %S (%S)");

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosSqlCreateIndex, &KPosLmDbRegIndex,
                  &KPosLmDbRegistryTable, &KPosLmProtocolCol);
    User::LeaveIfError(iDb.Execute(*sql));
    CleanupStack::PopAndDestroy(sql);

    CleanupStack::PopAndDestroy(&del); //DeleteFileCleanupItem
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::Open
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDbRegistry::Open(
    RFs& aFileSession,
    const TDesC& aFileName)
    {
    return iDb.Open(aFileSession, aFileName);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::RegisterDatabaseL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::RegisterDatabaseL(
    const TDesC& aDatabaseUri,
    const TPosLmDatabaseSettings& aSettings)
    {
    RecoverIfNeededL();

    HBufC* embeddedUri =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(aDatabaseUri);

    RDbView view;
    CleanupClosePushL(view);

    if (FindEntryL(view, *embeddedUri))
        {
        User::Leave(KErrAlreadyExists);
        }

    CleanupStack::PopAndDestroy(2, embeddedUri); //&view

    _LIT(KPosLmSqlRegisterDb, "SELECT %S, %S, %S FROM %S");
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlRegisterDb, &KPosLmProtocolCol,
        &KPosLmFileNameCol, &KPosLmSettingsCol, &KPosLmDbRegistryTable);

    CleanupClosePushL(view);
    User::LeaveIfError(view.Prepare(iDb, TDbQuery(*sql),
        TDbWindow::EUnlimited, RDbView::EInsertOnly));

    CDbColSet* colset = view.ColSetL();
    CleanupStack::PushL(colset);

    view.InsertL();
    view.SetColL(
        colset->ColNo(KPosLmProtocolCol), ExtractProtocol(aDatabaseUri));

    RDbColWriteStream out;
    out.OpenLC(view, colset->ColNo(KPosLmFileNameCol));
    out.WriteL(ExtractFileName(aDatabaseUri));
    CleanupStack::Pop(&out);
    out.Close(); // This line must exist in order for
                 // the item to be written to db.

    TPckgC<TPosLmDatabaseSettings> settingsPckg(aSettings);
    out.OpenLC(view, colset->ColNo(KPosLmSettingsCol));
    out.WriteL(settingsPckg);
    CleanupStack::Pop(&out);
    out.Close(); // This line must exist in order for
                 // the item to be written to db.

    view.PutL();
    // CleanupStack::PopAndDestroy(3, sql); //view, colset

    // CleanupStack::PopAndDestroy(colset);
    CleanupStack::Pop(colset);
    delete colset;
    CleanupStack::PopAndDestroy(&view);
    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::UnregisterDatabaseL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::UnregisterDatabaseL(
    const TDesC& aDatabaseUri)
    {
    RecoverIfNeededL();

    HBufC* embeddedUri =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(aDatabaseUri);

    _LIT(KPosLmSqlUnregisterDb,
        "Delete FROM %S WHERE %S = '%S' AND %S = '%S'");

    //If the URI length is less than KPosLmSqlStatementUriMaxLen
    //proceed as usual, i.e. create a sql delete statement to unregister
    //the URI. However, URIs that exceed KPosLmSqlStatementUriMaxLen has
    //to be handled differently, see the else block.
    if (embeddedUri->Length() < KPosLmSqlStatementUriMaxLen)
        {
        TPtrC protPtr = ExtractProtocol(*embeddedUri);
        TPtrC namePtr = ExtractFileName(*embeddedUri);
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

        sql->Des().Format(KPosLmSqlUnregisterDb, &KPosLmDbRegistryTable,
            &KPosLmProtocolCol, &protPtr, &KPosLmFileNameCol, &namePtr);

        User::LeaveIfError(iDb.Execute(*sql));
        CleanupStack::PopAndDestroy(sql);
        }
    else
        {
        //Handle long URIs.
        RDbView view;
        CleanupClosePushL(view);

        if (FindLongEntryL(view, *embeddedUri))
            {
            view.DeleteL();
            }

        CleanupStack::PopAndDestroy(&view);
        }
    CleanupStack::PopAndDestroy(embeddedUri);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::UnregisterAllL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::UnregisterAllL(
    const TDesC& aProtocol)
    {
    RecoverIfNeededL();

    _LIT(KPosLmSqlUnregisterAll, "Delete FROM %S WHERE %S = '%S'");

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlUnregisterAll, &KPosLmDbRegistryTable,
        &KPosLmProtocolCol, &aProtocol);

    User::LeaveIfError(iDb.Execute(*sql));
    CleanupStack::PopAndDestroy(sql);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ReadDatabaseSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::ReadDatabaseSettingsL(
    const TDesC& aDatabaseUri,
    TPosLmDatabaseSettings& aSettings)
    {
    RecoverIfNeededL();

    HBufC* embeddedUri =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(aDatabaseUri);

    RDbView view;
    CleanupClosePushL(view);
    if (!FindEntryL(view, *embeddedUri))
        {
        User::Leave(KErrNotFound);
        }

    CDbColSet* colset = view.ColSetL();
    CleanupStack::PushL(colset);

    view.GetL();
    TPckg<TPosLmDatabaseSettings> dbPckg(aSettings);

    RDbColReadStream in;
    in.OpenLC(view, colset->ColNo(KPosLmSettingsCol));
    in.ReadL(dbPckg);
    in.Close();

    CleanupStack::PopAndDestroy(4, embeddedUri); //&view, colset, &in
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ModifyDatabaseSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDbRegistry::ModifyDatabaseSettingsL(
    const TDesC& aDatabaseUri,
    const TPosLmDatabaseSettings& aSettings)
    {
    RecoverIfNeededL();

    HBufC* embeddedUri =
        PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(aDatabaseUri);

    RDbView view;
    CleanupClosePushL(view);
    if (!FindEntryL(view, *embeddedUri))
        {
        User::Leave(KErrNotFound);
        }

    view.UpdateL();

    CDbColSet* colset = view.ColSetL();
    CleanupStack::PushL(colset);

    TPckg<TPosLmDatabaseSettings> settingsPckg(aSettings);
    RDbColWriteStream out;
    out.OpenLC(view, colset->ColNo(KPosLmSettingsCol));
    out.WriteL(settingsPckg);
    CleanupStack::Pop(&out);
    out.Close();

    view.PutL();

    CleanupStack::PopAndDestroy(3, embeddedUri); //&view, colset
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ListDatabasesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDatabaseIterator* CPosLmDbRegistry::ListDatabasesL(
    const TDesC& aProtocol)
    {
    RecoverIfNeededL();

    _LIT(KPosLmSqlListDb, "SELECT * FROM %S WHERE %S = '%S'");

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlListDb,
        &KPosLmDbRegistryTable, &KPosLmProtocolCol, &aProtocol);

    RDbView view;
    CleanupClosePushL(view);
    User::LeaveIfError(view.Prepare(
        iDb, TDbQuery(*sql), TDbWindow::EUnlimited));
    User::LeaveIfError(view.EvaluateAll());

    CPosLmDatabaseIterator* iter = CPosLmDatabaseIterator::NewL(view);
    CleanupStack::Pop(&view);
    CleanupStack::PopAndDestroy(sql);

    return iter;
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::Compact
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDbRegistry::Compact()
    {
    return iDb.Compact();
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ExtractProtocol
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CPosLmDbRegistry::ExtractProtocol(
    const TDesC& aDatabaseUri)
    {
    TInt pos = aDatabaseUri.Find(KProtocol);
    if (pos == KErrNotFound)
        {
        return TPtrC();
        }
    return aDatabaseUri.Left(pos);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::ExtractFileName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CPosLmDbRegistry::ExtractFileName(
    const TDesC& aDatabaseUri)
    {
    TInt pos = aDatabaseUri.Find(KProtocol);
    if (pos == KErrNotFound)
        {
        return TPtrC();
        }
    return aDatabaseUri.Right(aDatabaseUri.Length() -
        pos - KProtocol().Length());
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::FindEntryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmDbRegistry::FindEntryL(
    RDbView& aView,
    const TDesC& aDatabaseUri)
    {
    if (aDatabaseUri.Length() < KPosLmSqlStatementUriMaxLen)
        {
        _LIT(KPosLmSqlSelectSettings,
            "SELECT %S FROM %S WHERE %S = '%S' AND %S = '%S'");

        TPtrC protPtr = ExtractProtocol(aDatabaseUri);
        TPtrC namePtr = ExtractFileName(aDatabaseUri);
        HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

        sql->Des().Format(KPosLmSqlSelectSettings, &KPosLmSettingsCol,
            &KPosLmDbRegistryTable, &KPosLmProtocolCol, &protPtr,
            &KPosLmFileNameCol, &namePtr);

        User::LeaveIfError(aView.Prepare(iDb, TDbQuery(*sql),
            TDbWindow::EUnlimited));
        User::LeaveIfError(aView.EvaluateAll());

        CleanupStack::PopAndDestroy(sql);
        return aView.NextL();
        }

    return FindLongEntryL(aView, aDatabaseUri);
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::FindLongEntryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmDbRegistry::FindLongEntryL(
    RDbView& aView,
    const TDesC& aDatabaseUri)
    {
    _LIT(KPosLmSqlSelectSettingsLike,
        "SELECT %S, %S FROM %S WHERE %S = '%S' AND %S LIKE '%S*'");

    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);

    TPtrC protPtr = ExtractProtocol(aDatabaseUri);
    TPtrC namePtr = ExtractFileName(aDatabaseUri);
    TPtrC leftPart(namePtr.Left(KPosLmSqlStatementUriMaxLen));

    sql->Des().Format(KPosLmSqlSelectSettingsLike,
        &KPosLmSettingsCol, &KPosLmFileNameCol,
        &KPosLmDbRegistryTable, &KPosLmProtocolCol, &protPtr,
        &KPosLmFileNameCol, &leftPart);

    //Find all entries that match the supplied URI,
    User::LeaveIfError(
        aView.Prepare(iDb, TDbQuery(*sql), TDbWindow::EUnlimited));
    User::LeaveIfError(aView.EvaluateAll());

    //Check for exact matches.
    TBool found(EFalse);
    while (!found && aView.NextL())
        {
        aView.GetL();

        CDbColSet* colset = aView.ColSetL();
        CleanupStack::PushL(colset);

        TDbColNo colNo = colset->ColNo(KPosLmFileNameCol);
        TInt nameLength = aView.ColLength(colNo);

        CleanupStack::PopAndDestroy(colset);

        HBufC* name = HBufC::NewLC(nameLength);
        TPtr ptr = name->Des();

        RDbColReadStream in;
        in.OpenLC(aView, colNo);
        in.ReadL(ptr, nameLength);
        CleanupStack::Pop(&in);
        in.Close();

        if (ptr == namePtr)
            {
            found = ETrue;
            }
        CleanupStack::PopAndDestroy(name);
        }

    CleanupStack::PopAndDestroy(sql);
    return found;
    }

// -----------------------------------------------------------------------------
// CPosLmDbRegistry::RecoverIfNeededL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmDbRegistry::RecoverIfNeededL()
    {
    if (iDb.IsDamaged())
        {
        User::LeaveIfError(iDb.Recover());
        }
    }

//  End of File

