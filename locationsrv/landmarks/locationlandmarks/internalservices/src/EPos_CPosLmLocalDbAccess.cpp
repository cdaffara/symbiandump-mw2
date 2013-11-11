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
* Description: Class that provides access to landmark databases.
*
*
*/


#include <f32file.h>
#include <EPos_Landmarks.h>
#include <eposlmcompactdblevel.rsg>
#include <EPos_LandmarksErrors.h>
#include "EPos_CPosLmLocalDbAccess.h"
#include "EPos_CPosLmResourceReader.h"
#include "EPos_LandmarkDatabaseStructure.h"
#include "EPos_PosLmDatabaseUtility.h"

_LIT(KPosCompactDbLevelResFile, "\\resource\\eposlmcompactdblevel.rsc");

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LmDbRollback Rollbacks a transaction for the CPosLmLocalDbAccess. This
// function can be used as a TCleanupItem.
// -----------------------------------------------------------------------------
//
void LmDbRollback (TAny* aLmLocalDbAccess)
    {
    reinterpret_cast<CPosLmLocalDbAccess*>(aLmLocalDbAccess)->
        RollbackTransaction();
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalDbAccess::CPosLmLocalDbAccess(TCompactLevelType aCompactLevel) :
    iInit(ENoInit),
    iLevelType(aCompactLevel)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbAccess::ConstructL()
    {
    ReadCompactDbLevelL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalDbAccess* CPosLmLocalDbAccess::NewL(
    TCompactLevelType aCompactLevel)
    {
    CPosLmLocalDbAccess* self = new (ELeave) CPosLmLocalDbAccess(aCompactLevel);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalDbAccess::~CPosLmLocalDbAccess()
    {
    iDb.Close();
    iDbSession.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::BeginTransactionLC()
    {
    User::LeaveIfError(BeginTransaction());

    CleanupStack::PushL(TCleanupItem(*LmDbRollback, this));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::CommitTransactionAndPopL()
    {
    User::LeaveIfError(CommitTransaction());

    CleanupStack::Pop(); // LmDbRollback
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::GetDatabase(
    RDbNamedDatabase& aDatabase)
    {
    aDatabase = iDb;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDbAccess::OpenDatabaseL(
    const TDesC& aUri)
    {
    iDb.Close();
    iDbSession.Close();

    TPtrC dbPath(aUri);
    PosLmDatabaseUtility::RemoveProtocolFromUriL(dbPath);

    User::LeaveIfError(iDbSession.Connect());
    TInt err = iDb.Open(iDbSession, dbPath, KPosLmDbSecureFormat);

    if (err == KErrNone)
        {
        err = VerifyDatabaseNeedInitializationL();
        }
        
    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDbAccess::BeginTransaction()
    {
    return iDb.Begin();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDbAccess::CommitTransaction()
    {
    if ( InTransaction() )
        {
        return iDb.Commit();
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::RollbackTransaction()
    {
    if ( InTransaction() )
        {
        iDb.Rollback();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmLocalDbAccess::InTransaction() const
    {
    return iDb.InTransaction();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmLocalDbAccess::InitFlag() const
    {
    return iInit;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TUint* CPosLmLocalDbAccess::InitFlagPtr()
    {
    return &iInit;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::ResetInitFlag()
    {
    iInit = ENoInit;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmLocalDbAccess::IsDamaged()
    {
    return iDb.IsDamaged();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::ExecuteL(
    const TDesC& aQuery)
    {
    User::LeaveIfError(iDb.Execute(aQuery));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::PrepareViewLC(
    TPreparationType aType,
    RDbView& aView,
    const TDesC& aSql,
    TDbTextComparison aComparison)
    {
    CleanupClosePushL(aView);

    switch (aType)
        {
        case EAddPreparation:
            User::LeaveIfError(aView.Prepare(iDb, TDbQuery(aSql, aComparison),
                TDbWindow::EUnlimited, RDbView::EInsertOnly));
            break;
        default: // EUpdatablePreparation
            User::LeaveIfError(aView.Prepare(iDb, TDbQuery(aSql, aComparison),
                TDbWindow::EUnlimited, RDbView::EUpdatable));
            User::LeaveIfError(aView.EvaluateAll());
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabase::TSize CPosLmLocalDbAccess::SizeL()
    {
    CPosLandmarkDatabase::TSize size;

    User::LeaveIfError(iDb.UpdateStats());

    size.iFileSize = iDb.Size().iSize;
    size.iUsage = TReal32(iDb.Size().iUsage) / 100;

    return size;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDbAccess::IsInitializeNeededL( TBool& aIsNeeded )
    {
    aIsNeeded = EFalse;
    return PerformCheckL( ETrue, aIsNeeded );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::SetInitializeDoneL()
    {
    SetFlagL(EFalse, TLanguage());
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDbAccess::IsLanguageChangeNeededL(
    TBool& aIsNeeded)
    {
    return PerformCheckL(EFalse, aIsNeeded);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDbAccess::SetLanguageL(
    TLanguage aLanguage)
    {
    SetFlagL(ETrue, aLanguage);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmLocalDbAccess::IsCompactNeededL()
    {
    CPosLandmarkDatabase::TSize size = SizeL();

    // For local compact, compact is needed when usage is below the relative
    // level.
    // For server compact, compact is needed when the wasted database usage
    // exceeds the absolute level and usage is below the relative level.
    return (iLevelType == ELocalCompactLevel &&
        size.iUsage < iRelativeCompactLevel) ||
        (iLevelType == EServerCompactLevel &&
        ((1 - size.iUsage) * size.iFileSize > iAbsoluteCompactLevel) &&
        size.iUsage < iRelativeCompactLevel);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbAccess::VerifyDatabaseNeedInitializationL()
    {
    TInt err = KErrNone;

    if ( iInit != ECreated )
        {
        TBool isNeeded;
        err = IsInitializeNeededL( isNeeded );
        if ( err == KErrNone )
            {
            if ( isNeeded )
                {
                iInit = ECreated;
                }
            else
                {
                err = IsLanguageChangeNeededL( isNeeded );

                if ( err == KErrNone && isNeeded )
                    {
                    iInit = ELanguage;
                    }

                if ( IsIndexCreationNeededL() )
                    {
                    iInit |= EIndexes;
                    }
                }
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbAccess::ReadCompactDbLevelL()
    {
    CPosLmResourceReader* resources =
        CPosLmResourceReader::NewLC(KPosCompactDbLevelResFile);

    if (iLevelType == ELocalCompactLevel)
        {
        iRelativeCompactLevel = TReal32(resources->ReadInt32L(
            R_POS_LM_LOCAL_COMPACT_DB_USAGE_LEVEL)) / 100;
        }
    else
        {
        iRelativeCompactLevel = TReal32(resources->ReadInt32L(
            R_POS_LM_SERVER_COMPACT_RELATIVE_DB_USAGE_LEVEL)) / 100;
        iAbsoluteCompactLevel = resources->ReadInt32L(
            R_POS_LM_SERVER_COMPACT_ABSOLUTE_WASTED_DB_USAGE_LEVEL) * 1000;
        }

    CleanupStack::PopAndDestroy(resources);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbAccess::PerformCheckL( TBool aIsInitCheck, TBool& aIsNeeded )
    {
    HBufC* sql = HBufC::NewLC( KPosLmSqlStatementMaxLen );
    sql->Des().Format( KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmSettingsTable );

    RDbView view;
    CleanupClosePushL( view );

    TInt err = view.Prepare( iDb, TDbQuery( *sql ), TDbWindow::EUnlimited );

    if (err == KErrNotFound)
        {
        // Database is corrupt, because the settings table cannot be found.
        err = KErrCorrupt;
        }
    else
        {
        User::LeaveIfError( err );
        User::LeaveIfError( view.EvaluateAll() );

        view.FirstL();
        if ( view.AtEnd() )
            {
            // no records in settings - init is needed
            aIsNeeded = ETrue;
            }
        else
            {
            view.GetL();

            if ( aIsInitCheck )
                {
                // does this database
                aIsNeeded = ( view.ColInt32( EPosLmScInitFlagCol ) != ECreated );
                }
            else
                {
                aIsNeeded =
                    ( User::Language() != view.ColInt32( EPosLmScLanguageCol ) );
                }
            }
        }

    CleanupStack::PopAndDestroy( &view );
    CleanupStack::PopAndDestroy( sql );

    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbAccess::SetFlagL(
    TBool aSetLanguage,
    TLanguage aLanguage)
    {
    HBufC* sql = HBufC::NewLC(KPosLmSqlStatementMaxLen);
    sql->Des().Format(KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmSettingsTable);

    RDbView view;
    PrepareViewLC(EUpdatablePreparation, view, *sql);

    if (view.NextL())
        {
        view.UpdateL();
        }
    else
        {
        view.InsertL();
        }

    if (!aSetLanguage)
        {
        // Set initialized flag
        view.SetColL(EPosLmScInitFlagCol, (TInt)ECreated);
        }
    else
        {
        // Set language switched flag
        view.SetColL(EPosLmScLanguageCol, (TInt)aLanguage);
        }

    view.PutL();

    CleanupStack::PopAndDestroy(2, sql); //&view
    }

// -----------------------------------------------------------------------------
// Some databases may be missing several indexes, which were added in later versions
// of this component. Those a verified and if not found, initialization is requested.
// Added indexes: 
//  Index on landmark ID in landmark-field table
//  Index on landmark ID in landmark-category table
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmLocalDbAccess::IsIndexCreationNeededL() const
    {
    // category table
    if ( !IndexExistsL( KPosLmLandmarkFieldTable, KPosLmFieldsLmIdIndex ) )
        {
        return ETrue;
        }

    // landmark-category table
    if ( !IndexExistsL( KPosLmLandmarkCategoryTable, KPosLmCategoryLmIdIndex ) )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalDbAccess::IndexExistsL( const TDesC& aTable, const TDesC& aIndex ) const
    {
    return PosLmDatabaseUtility::IndexExistsL( iDb, aTable, aIndex );
    }
    
