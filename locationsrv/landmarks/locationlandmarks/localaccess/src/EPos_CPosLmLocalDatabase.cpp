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
* Description: Handle to a local landmark database.
*
*
*/


#include    <EPos_CPosLandmark.h>
#include    <EPos_PosLmImplExtension.h>
#include    <EPos_CPosLmOperation.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_poslmcategoryhandler.h>
#include    <epos_poslmdatabaseutility.h>
#include    <epos_cposlmdiskutilities.h>
#include    <epos_cposlmsettingshandler.h>
#include    <epos_rposlandmarkserver.h>
#include    <EPos_LandmarksUids.hrh>

#include    "EPos_CPosLmLocalLandmarkIterator.h"
#include    "EPos_CPosLmLocalRemoveLandmarksOp.h"
#include    "EPos_CPosLmPartialReadParameters.h"
#include    "EPos_CPosLmLocalReadPartialLmOp.h"
#include    "EPos_CPosLmLocalInitializeOp.h"
#include    "EPos_CPosLmLocalCompactingOp.h"
#include    "EPos_CPosLmLocalExportOp.h"
#include    "EPos_CPosLmLocalImportOp.h"
#include    "EPos_CPosLmLocalImportedLmIterator.h"
#include    "EPos_CPosLmLocalInternalProxyOp.h"
#include    "epos_rposlmlocalnameindex.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"
#include    "EPos_CPosLmLocalCategoryManager.h"
#include    "epos_cposlmlocaldbfastcounters.h"
#include    "epos_cposlmlocaldatabase.h"

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LmDbReleaseReadLock Releases a db lock for the CPosLmLocalDatabase. This
// function can be used as a TCleanupItem.
// -----------------------------------------------------------------------------
//
void LmDbReleaseReadLock( TAny* aLocalDatabase )
    {
    reinterpret_cast<CPosLmLocalDatabase*>( aLocalDatabase )->
        ReleaseLock( CPosLmLocalDatabase::EReadLock );
    }

// -----------------------------------------------------------------------------
// LmDbReleaseWriteLock Releases a db lock for the CPosLmLocalDatabase. This
// function can be used as a TCleanupItem.
// -----------------------------------------------------------------------------
//
void LmDbReleaseWriteLock( TAny* aLocalDatabase )
    {
    reinterpret_cast<CPosLmLocalDatabase*>( aLocalDatabase )->
        ReleaseLock( CPosLmLocalDatabase::EWriteLock );
    }

// -----------------------------------------------------------------------------
// LmDbRollbackServerTransaction Rollbacks a transaction for the server data. 
// This function can be used as a TCleanupItem.
// -----------------------------------------------------------------------------
//
void LmDbRollbackServerTransaction( TAny* aLmLocalDatabase )
    {
    reinterpret_cast<CPosLmLocalDatabase*>( aLmLocalDatabase )->
        RollbackServerTransaction();
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalDatabase::CPosLmLocalDatabase()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::ConstructL(
    TAny* aDatabaseUri)
    {
    iPartialSettings = CPosLmPartialReadParameters::NewLC();
    CleanupStack::Pop(iPartialSettings);
    iDiskUtilities = CPosLmDiskUtilities::NewL();

    iDbAccess = CPosLmLocalDbAccess::NewL();

    iServer = new (ELeave) RPosLandmarkServer;
    User::LeaveIfError(iServer->Connect());

    iSubSession = new (ELeave) RPosLmLocalAccessSubsession;
    User::LeaveIfError(iSubSession->Open(*iServer));

    if (aDatabaseUri)
        {
        iDbUri = PosLmDatabaseUtility::CreateDatabaseUriL(
            *static_cast<HBufC*> (aDatabaseUri));

        OpenDatabaseL(EFalse);
        }
    else
        {
        CPosLmSettingsHandler* handler = CPosLmSettingsHandler::NewL();
        CleanupStack::PushL(handler);
        iDbUri = handler->GetDefaultDatabaseUriL();
        CleanupStack::PopAndDestroy(handler);

        OpenDatabaseL(ETrue);
        }

    iNameIndex = new (ELeave) RPosLmLocalNameIndex( *iSubSession );
    User::LeaveIfError( iNameIndex->Open() );
    
    iCategoryManager = CPosLmLocalCategoryManager::NewL( this );
    iFastCounters = CPosLmLocalDbFastCounters::NewL( *iDbAccess );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalDatabase* CPosLmLocalDatabase::OpenL(
    TAny* aDatabaseUri)
    {
    CPosLmLocalDatabase* self = new( ELeave ) CPosLmLocalDatabase;
    CleanupStack::PushL(self);
    self->ConstructL(aDatabaseUri);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalDatabase::~CPosLmLocalDatabase()
    {
    delete iFastCounters;
    delete iCategoryManager;
    if ( iNameIndex )
        {
        iNameIndex->Close();
        delete iNameIndex;
        }
    delete iDbAccess;
    if (iSubSession)
        {
        iSubSession->Close();
        delete iSubSession;
        }
    if (iServer)
        {
        iServer->Close();
        delete iServer;
        }
    delete iDiskUtilities;
    delete iDbUri;
    delete iPartialSettings;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::CopyPartialSettingsL(
    const CPosLmPartialReadParameters& aFromSetting,
    CPosLmPartialReadParameters& aToSetting)
    {
    RArray<TUint> requestedFields;
    CleanupClosePushL(requestedFields);

    aToSetting.SetRequestedAttributes(aFromSetting.RequestedAttributes());
    User::LeaveIfError(
        aFromSetting.GetRequestedPositionFields(requestedFields));
    User::LeaveIfError(aToSetting.SetRequestedPositionFields(requestedFields));

    CleanupStack::PopAndDestroy(&requestedFields);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmLocalDbAccess* CPosLmLocalDatabase::DatabaseAccess()
    {
    return iDbAccess;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::LeaveIfInitializingIsNeededL()
    {
    if (IsInitializingNeeded())
        {
        if (iDbAccess->IsDamaged())
            {
            User::Leave(KErrCorrupt);
            }
        else 
            {
            User::Leave(KErrPosLmNotInitialized);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TChar CPosLmLocalDatabase::DatabaseDrive()
    {
    return iDbDrive;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RPosLmLocalAccessSubsession& CPosLmLocalDatabase::Session()
    {
    return *iSubSession;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C RPosLmLocalNameIndex& CPosLmLocalDatabase::NameIndex()
    {
    return *iNameIndex;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C const RPosLmLocalNameIndex& CPosLmLocalDatabase::NameIndex() const
    {
    return *iNameIndex;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDatabase::RegisterEvent(
    TPosLmEventType aEventType,
    TPosLmItemId aId)
    {
    return iSubSession->ReportEvent(aEventType, aId);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::AquireLockLC( TLockType aLockType )
    {
    AquireLockL( aLockType );

    if (aLockType == EReadLock)
        {
        CleanupStack::PushL(TCleanupItem(*LmDbReleaseReadLock, this));
        }
    else
        {
        CleanupStack::PushL(TCleanupItem(*LmDbReleaseWriteLock, this));
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::AquireLockL( TLockType aLockType )
    {
    User::LeaveIfError( iSubSession->SetDatabaseLock( aLockType ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmLocalDatabase::ReleaseLock( TLockType aLockType )
    {
    return iSubSession->ReleaseDatabaseLock( aLockType );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::BeginServerTransactionL()
    {
    User::LeaveIfError( NameIndex().BeginTransaction() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::BeginServerTransactionLC()
    {
    BeginServerTransactionL();
    CleanupStack::PushL (TCleanupItem( *LmDbRollbackServerTransaction, this ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::CommitServerTransactionL()
    {
    User::LeaveIfError( NameIndex().CommitTransaction() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::CommitServerTransactionLX()
    {
    //coverity[naming_error : FALSE]
    CommitServerTransactionL();
    CleanupStack::Pop(); // rollback
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmLocalDatabase::RollbackServerTransaction()
    {
    NameIndex().RollbackTransaction();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalDatabase::IsInitializingNeeded() const
    {
    if ( iDbAccess->InitFlag() != ENoInit || 
         iDbAccess->IsDamaged() || 
         NameIndex().Status() != KErrNone )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::InitializeL()
    {
    return CPosLmLocalInitializeOp::NewL(*this);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmLocalDatabase::ReadLandmarkLC(
    TPosLmItemId aLandmarkId)
    {
    return ReadLmLC(aLandmarkId, EFalse);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalDatabase::LandmarkIteratorL()
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalLandmarkIterator::NewL(*this);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalDatabase::LandmarkIteratorL(
    const TPosLmSortPref& aSortPref)
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalLandmarkIterator::NewL(*this, aSortPref);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalDatabase::AddLandmarkL( CPosLandmark& aLandmark )
    {
    LeaveIfInitializingIsNeededL();
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::EAddLandmarkOp,
        aLandmark ), iDbDrive );

    AquireLockLC( EWriteLock );
    BeginServerTransactionLC();
    iDbAccess->BeginTransactionLC();

    PosLmLandmarkHandler::AddLandmarkL( *iDbAccess, aLandmark );
    NameIndex().AddL( aLandmark );

    iDbAccess->CommitTransactionAndPopL();
    CommitServerTransactionLX();
    CleanupStack::PopAndDestroy(); // DbLock
    
    User::LeaveIfError(
        RegisterEvent( EPosLmEventLandmarkCreated, aLandmark.LandmarkId() ) );

    return aLandmark.LandmarkId();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::UpdateLandmarkL( const CPosLandmark& aLandmark )
    {
    LeaveIfInitializingIsNeededL();

    if ( aLandmark.IsPartial() )
        {
        User::Leave( KErrArgument );
        }

    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::EUpdateLandmarkOp,
        aLandmark ), iDbDrive );

    AquireLockLC( EWriteLock );
    BeginServerTransactionLC();
    iDbAccess->BeginTransactionLC();

    PosLmLandmarkHandler::UpdateLandmarkL( *iDbAccess, aLandmark );
    NameIndex().UpdateL( aLandmark );

    iDbAccess->CommitTransactionAndPopL();
    CommitServerTransactionLX();
    CleanupStack::PopAndDestroy(); // DbLock
    
    User::LeaveIfError(
        RegisterEvent( EPosLmEventLandmarkUpdated, aLandmark.LandmarkId() ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::RemoveLandmarkL( TPosLmItemId aLandmarkId )
    {
    LeaveIfInitializingIsNeededL();
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::ERemoveLandmarkOp,
        aLandmarkId ), iDbDrive );

    AquireLockLC( EWriteLock );
    BeginServerTransactionLC();
    iDbAccess->BeginTransactionLC();

    PosLmLandmarkHandler::RemoveLandmarkL( *iDbAccess, aLandmarkId );
    NameIndex().RemoveL( aLandmarkId );

    iDbAccess->CommitTransactionAndPopL();
    CommitServerTransactionLX();
    CleanupStack::PopAndDestroy(); // DbLock

    User::LeaveIfError(
        RegisterEvent( EPosLmEventLandmarkDeleted, aLandmarkId ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::RemoveLandmarksL(
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    LeaveIfInitializingIsNeededL();
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::ERemoveLandmarksOp,
        aLandmarkIdArray), iDbDrive);

    return CPosLmLocalRemoveLandmarksOp::NewL(*this, aLandmarkIdArray);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::RemoveAllLandmarksL()
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalRemoveLandmarksOp::NewL(*this);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmPartialReadParameters* CPosLmLocalDatabase::PartialReadParametersLC()
    {
    CPosLmPartialReadParameters* partialSettings =
        CPosLmPartialReadParameters::NewLC();

    CopyPartialSettingsL(*iPartialSettings, *partialSettings);

    return partialSettings;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::SetPartialReadParametersL(
    const CPosLmPartialReadParameters& aPartialSettings)
    {
    CPosLmPartialReadParameters* newSetting =
        CPosLmPartialReadParameters::NewLC();

    CopyPartialSettingsL(aPartialSettings, *newSetting);

    CleanupStack::Pop(newSetting);
    delete iPartialSettings;
    iPartialSettings = newSetting;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmLocalDatabase::ReadPartialLandmarkLC(
    TPosLmItemId aLandmarkId)
    {
    return ReadLmLC(aLandmarkId, ETrue);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::PreparePartialLandmarksL(
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalReadPartialLmOp::NewL(
        *this, aLandmarkIdArray, iPartialSettings);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CArrayPtr<CPosLandmark>* CPosLmLocalDatabase::TakePreparedPartialLandmarksL(
    CPosLmOperation* aPreparePartialLandmarkOperation)
    {
    return static_cast<CPosLmLocalReadPartialLmOp*>
        (aPreparePartialLandmarkOperation)->GetResultL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalDatabase::DatabaseUriLC()
    {
    HBufC* dbUri = iDbUri->AllocL();
    CleanupStack::PushL(dbUri);

    return dbUri;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmarkDatabase::TSize CPosLmLocalDatabase::SizeL()
    {
    return iDbAccess->SizeL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::CompactL()
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalCompactingOp::NewL(*this);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::NotifyDatabaseEvent(
    TPosLmEvent& aEvent,
    TRequestStatus& aStatus)
    {
    iSubSession->NotifyDatabaseEvent(aEvent, aStatus);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDatabase::CancelNotifyDatabaseEvent()
    {
    return iSubSession->CancelNotifyDatabaseEvent();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::ExportLandmarksL(
    CPosLandmarkEncoder& aLandmarkEncoder,
    const RArray<TPosLmItemId>& aLandmarkIdArray,
    TTransferOptions aTransferOptions)
    {
    LeaveIfInitializingIsNeededL();

    return CPosLmLocalExportOp::NewL(*this, aLandmarkEncoder, aLandmarkIdArray,
        aTransferOptions);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::ImportLandmarksL(
    CPosLandmarkParser& aLandmarkParser,
    TTransferOptions aTransferOptions)
    {
    LeaveIfInitializingIsNeededL();

    return CreateImportProxyL(CPosLmLocalImportOp::NewL(*this, aLandmarkParser,
        aTransferOptions, *iDiskUtilities));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::ImportLandmarksL(
    CPosLandmarkParser& aLandmarkParser,
    const RArray<TUint>& aLandmarkSelection,
    TTransferOptions aTransferOptions)
    {
    LeaveIfInitializingIsNeededL();

    return CreateImportProxyL(CPosLmLocalImportOp::NewL(*this, aLandmarkParser,
        aLandmarkSelection, aTransferOptions, *iDiskUtilities));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalDatabase::ImportedLandmarksIteratorL(
    CPosLmOperation* aImportOperation)
    {
    return CPosLmLocalImportedLmIterator::NewL(aImportOperation);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmLocalDatabase::ReadLmLC(
    TPosLmItemId aLandmarkId,
    TBool aIsPartial)
    {
    LeaveIfInitializingIsNeededL();

    CPosLandmark* landmark = CPosLandmark::NewLC();
    PosLmImplExtension::SetLandmarkIdL(*landmark, aLandmarkId);

    AquireLockLC(EReadLock);

    if (!aIsPartial)
        {
        PosLmLandmarkHandler::ReadLandmarkL(*iDbAccess, *landmark);
        }
    else
        {
        PosLmLandmarkHandler::ReadPartialLandmarkL(*iDbAccess, *landmark,
            *iPartialSettings);
        }

    CleanupStack::PopAndDestroy(); // DbLock

    return landmark;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalDatabase::CreateImportProxyL(
    CPosLmLocalImportOp* aImportOp)
    {
    CleanupStack::PushL(aImportOp);
    CPosLmLocalInternalProxyOp* proxyOp = CPosLmLocalInternalProxyOp::NewL(
        *aImportOp, aImportOp);
    CleanupStack::Pop(aImportOp); // ownership transferred
    return proxyOp;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabase::OpenDatabaseL( TBool aIsDefaultDb )
    {
    iDbDrive = (*iDbUri)[KPosLocalDbFileProtocol().Length()];

    iSubSession->RegisterSessionL(*iDbUri);
    
    AquireLockLC(EReadLock);

    if (!aIsDefaultDb)
        {
        User::LeaveIfError(iDbAccess->OpenDatabaseL(*iDbUri));
        }
    else
        {
        TInt err = iDbAccess->OpenDatabaseL(*iDbUri);
        if (err != KErrNone)
            {
            // As this is a default landmarks db, all errors other than OutOfMemory 
            // should be handled by replacing a corrupt db or creating a new db if one is missing. 
            if (err == KErrNoMemory)
                {
                User::Leave(err);
                }
            else
                {
                if ((err == KErrNotFound) || (err == KErrPathNotFound)
                        || (err == KErrArgument) || (err == KErrAlreadyExists))
                    {
                    // Create a new database [default landmarks db]     
                    iSubSession->CreateDefaultDbL(*iDbUri, EFalse);
                    }
                else
                    {
                    // Try deleting the corrupted database and then create a new one                
                    iSubSession->CreateDefaultDbL(*iDbUri, ETrue);
                    }
                *DatabaseAccess()->InitFlagPtr() = ECreated;
                User::LeaveIfError(iDbAccess->OpenDatabaseL(*iDbUri));
                }
            }
        }
    CleanupStack::PopAndDestroy(); // DbLock
    }

// -----------------------------------------------------------------------------
// From CPosLandmarkDatabaseExtended
// -----------------------------------------------------------------------------
//
CPosLmCategoryManager& CPosLmLocalDatabase::CategoryManager()
    {
    return *iCategoryManager;
    }

// -----------------------------------------------------------------------------
// From CPosLandmarkDatabaseExtended
// -----------------------------------------------------------------------------
//
void* CPosLmLocalDatabase::GetExtension( TUid aExtensionUid )
    {
    switch ( aExtensionUid.iUid )
        {
        case KPosLmDatabaseFastCountersIfUid:
            return static_cast<MPosLmDatabaseFastCounters*>( iFastCounters );
        default:
            return NULL;
        }
    }

