/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata main server class*
*/

// INCLUDE FILES
#include <f32file.h>
#include "mdsserver.h"
#include "mdscommoninternal.h"
#include "mdsserversession.h"
#include "mdsdbconnectionpool.h"
#include "mdssqldbmaintenance.h"
#include "mdssqliteconnection.h"
#include "mdsnotifier.h"
#include "mdsobjectlocklist.h"
#include "mdslogger.h"
#include "mdsmaintenanceengine.h"
#include "mdsmanipulationengine.h"
#include "mdsclausebuffer.h"
#include "mdebackuprestorewatcher.h"
#include "mdsschema.h"
#include "mdcserializationbuffer.h"

__DEFINE_LOGGER

const TInt64 KDiskSpaceGarbageCollectorThreshold = 1024*1024*2; // 2 MB 

_LIT( KSchema, "schema.mde" );
_LIT( KDefaultImportProfile, "defaultimportfile.mde" );
_LIT( KBackupRegistration, "backup_registration.xml" );
_LIT( KSchemaPath, "z:\\private\\200009F3\\schema.mde" );
_LIT( KDefaultImportProfilePath, "z:\\private\\200009F3\\defaultimportfile.mde" );
_LIT( KBackupRegistrationPath, "z:\\private\\200009F3\\backup_registration.xml" );

// ========================= MdS POLICY ==================================

const TUint KMdsRangeCount = EUnknownMdEServRqst + 1;

const TInt KMdsRanges[KMdsRangeCount] = 
    {
    EShutdown,          	// shut down the session
    EAdd,					// add items
    ERemove,				// remove items
    EFind,              	// finds in sync
    EAsyncFind,         	// finds asynchronously
    EContinueAsyncFind, 	// continues existing asynchronous find, fetches next set
    ECancelFind,        	// cancels asynchronous find
    ECancelObject,      	// cancels opened object
    ECheckObject,           // get object's "base" values
    EUpdate,		      	// update items
    ERegister,          	// registers an object to be notified
    EListen,            	// listens for events for a registered profile
    EUnregister,        	// unregisters for no more notifications
    EGetData,           	// (transfers data server->client)
    EImportMetadata,    	// starts metadata importing
    EAsyncImportMetadata,	// starts asynchronous metadata importing
    EExportMetadata,		// starts metadata exporting
    EAsyncExportMetadata,	// starts asynchronous metadata exporting
    EImportSchema,      	// starts schema importing
    EAddRelationDef,		// Add relation def
    EAddEventDef,		    // Add event def
    EAddMemoryCard,
    EGetMemoryCard,
    ECheckMemoryCard,
    ESetMedia,
    EGetMedia,
    EGetPresentMedias,
    ESetFileToPresent,
    ESetFilesToPresent,
    ESetFilesToNotPresent,
    ERemoveFilesNotPresent,
    EGetSchemaVersion,
    ESetObjectToPresentByGuid, 
    ESetHarvestingPrioritizationChunk,
    EAddHarvestingPrioritizationObserver,
    ECancelHarvestingPrioritizationObserver,
    EChangePath, 
    ESetPending,
    EResetPending,
    EGetPendingCount,
    EGetPending,
    EResetDB,
    EChangeMediaId,
    EChangeMassStorageMediaId,
    EUnknownMdEServRqst     // handle for unknown requests
    };

const TUint8 KMdsElementsIndex[KMdsRangeCount] = 
    {
    CPolicyServer::ECustomCheck,    // EShutdown
    CPolicyServer::ECustomCheck,    // EAdd
    CPolicyServer::ECustomCheck,    // ERemove
    CPolicyServer::ECustomCheck,    // EFind
    CPolicyServer::ECustomCheck,    // EAsyncFind
    CPolicyServer::ECustomCheck,    // EContinueAsyncFind
    CPolicyServer::ECustomCheck,    // ECancelFind
    CPolicyServer::ECustomCheck,    // ECancelObject
    CPolicyServer::ECustomCheck,    // ECheckObject
    CPolicyServer::ECustomCheck,    // EUpdate
    CPolicyServer::ECustomCheck,    // ERegister
    CPolicyServer::ECustomCheck,    // EListen
    CPolicyServer::ECustomCheck,    // EUnregister
    CPolicyServer::ECustomCheck,    // EGetData
    CPolicyServer::ECustomCheck,    // EImportMetadata
    CPolicyServer::ECustomCheck,    // EAsyncImportMetadata
    CPolicyServer::ECustomCheck,    // EExportMetadata
    CPolicyServer::ECustomCheck,    // EAsyncExportMetadata
    CPolicyServer::ECustomCheck,    // EImportSchema
    CPolicyServer::ECustomCheck,	// EAddRelationDef
    CPolicyServer::ECustomCheck,	// EAddEventDef
    CPolicyServer::ECustomCheck, 	// EAddMemoryCard
    CPolicyServer::ECustomCheck, 	// EGetMemoryCard
    CPolicyServer::ECustomCheck, 	// ECheckMemoryCard
    CPolicyServer::ECustomCheck, 	// ESetMedia
    CPolicyServer::ECustomCheck, 	// EGetMedia
    CPolicyServer::ECustomCheck, 	// EGetPresentMedias
    CPolicyServer::ECustomCheck, 	// ESetFileToPresent
    CPolicyServer::ECustomCheck, 	// ESetFilesToPresent
    CPolicyServer::ECustomCheck, 	// ESetFilesToNotPresent
    CPolicyServer::ECustomCheck, 	// ERemoveFilesNotPresent
    CPolicyServer::ECustomCheck, 	// EGetSchemaVersion
    CPolicyServer::ECustomCheck, 	// ESetObjectToPresentByGuid
    CPolicyServer::ECustomCheck, 	// ESetHarvestingPrioritizationChunk
    CPolicyServer::ECustomCheck, 	// EAddHarvestingPrioritizationObserver
    CPolicyServer::ECustomCheck, 	// ECancelHarvestingPrioritizationObserver
    CPolicyServer::ECustomCheck, 	// EChangePath
    CPolicyServer::ECustomCheck, 	// ESetPending
    CPolicyServer::ECustomCheck, 	// EResetPending
    CPolicyServer::ECustomCheck, 	// EGetPendingCount
    CPolicyServer::ECustomCheck, 	// EGetPending
    CPolicyServer::ECustomCheck, 	// EResetDB
    CPolicyServer::ECustomCheck,    // EChangeMediaId
    CPolicyServer::ECustomCheck,    // EChangeMassStorageMediaId
    CPolicyServer::ENotSupported	// EUnknownMdEServRqst
    };


const CPolicyServer::TPolicy KMdsPolicy =
    {
    CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
    KMdsRangeCount,                   
    KMdsRanges,
    KMdsElementsIndex,
    NULL //mdsElements
    };

// ======================= LOCAL FUNCTIONS ================================

void CheckAndInitializeFileL( RFs& aFs, TBuf<KMaxFileName>& aFile, const TDesC16& aFilePath,
                                            RFileReadStream& aTempFile, CFileMan* aFileMan )
    {
    __LOG1( ELogAlways, "CheckAndInitializeFileL() - handling file %S", &aFilePath );
    const TInt err = aTempFile.Open( aFs, aFile, EFileRead | EFileShareAny );
    __LOG1( ELogAlways, "CheckAndInitializeFileL() - open file error %d", err );
    aTempFile.Close();
    if ( err != KErrNone )
        {
        if ( err == KErrNotFound )
            {
            // Path found but not schema.mde, copy schema.m
            const TInt error = aFileMan->Copy( aFilePath, aFile, CFileMan::EOverWrite );
            __LOG1( ELogAlways, "CheckAndInitializeFileL() - copy file error %d", error );
            }
        else if ( err == KErrPathNotFound)
            {
            // Create private dir
            User::LeaveIfError( aFs.CreatePrivatePath( EDriveC ) );
            
            // Copy schema.mde
            const TInt error = aFileMan->Copy( aFilePath, aFile, CFileMan::EOverWrite );
            __LOG1( ELogAlways, "CheckAndInitializeFileL() - copy file error %d", error );
            }    
        }    
    }

// ======================= MEMBER FUNCTIONS ===============================

CPolicyServer::TCustomResult CMdSServer::CustomSecurityCheckL(
        const RMessage2& aMsg, TInt& /*aAction*/, TSecurityInfo& /*aMissing*/ )
    {
    CPolicyServer::TCustomResult securityCheckResult = EFail;
    switch ( aMsg.Function() )
        {
        case EAdd:
        case ERemove:
        case ECancelObject:
        case EUpdate:
        case EImportMetadata:
        case EAsyncImportMetadata:
        case EAddRelationDef:
        case EAddEventDef:
        case EImportSchema:
        case EChangeMassStorageMediaId:
        case ESetObjectToPresentByGuid:
            {
            if( aMsg.HasCapability( ECapabilityWriteDeviceData ) )
                {
                securityCheckResult = EPass;
                }
            }
            break;

        case ERegister:
        case EListen:
        case EUnregister:
        case EFind:
        case EAsyncFind:
        case EContinueAsyncFind:
        case ECancelFind:    
        case EGetData:
        case ECheckObject:
            {
            securityCheckResult = EPass;
            }
            break;
            
        case EExportMetadata:
        case EAsyncExportMetadata:
        case EGetSchemaVersion:
            {
            if( aMsg.HasCapability( ECapabilityReadUserData ) || 
            	aMsg.HasCapability( ECapabilityReadDeviceData ) )
                {
                securityCheckResult = EPass;
                }
            }
        	break;
        	
        // Only for internal MdS usage
        case EAddMemoryCard:
    	case EGetMemoryCard:
        case ECheckMemoryCard:
        case ESetMedia:
        case EGetMedia:
        case EGetPresentMedias:
    	case ESetFileToPresent:
    	case ESetFilesToPresent:
    	case ESetFilesToNotPresent:
    	case ERemoveFilesNotPresent:
    	case ESetHarvestingPrioritizationChunk:
    	case EAddHarvestingPrioritizationObserver:
    	case ECancelHarvestingPrioritizationObserver:
    	case EChangePath:
    	case EChangeMediaId:
    	case ESetPending:
    	case EResetPending:
    	case EGetPendingCount:
    	case EGetPending:
    	case EResetDB:
    		{
            if( aMsg.HasCapability( ECapabilityDiskAdmin ) )
                {
                securityCheckResult = EPass;
                }
    		}
        	break;
        	
        default:
            {
            securityCheckResult = EFail;
            }
        }

    return securityCheckResult;
    }

CPolicyServer::TCustomResult CMdSServer::CustomFailureActionL(
        const RMessage2& /*aMsg*/, TInt /*aAction*/, const TSecurityInfo& /*aMissing*/)
    {
    // Not used
    return EFail;
    }


// ========================= MEMBER FUNCTIONS ==================================
void CMdSServer::HandleDiskSpaceNotificationL( TDiskSpaceDirection aDiskSpaceDirection )
	{
	// skip db cleaning if drive's free space is over limit or
	// backup or restore is running
	if( MMdSDiskSpaceNotifierObserver::EMore == aDiskSpaceDirection ||
		iBackupOrRestoreRunning )
		{
		return;
		}

	// delete only objects which are in "not present" state and
	// object was not located in last (count = KMemoryCardLimit) 
	// inserted memory cards and object doesn't have any relations
	_LIT( KMemoryCardCleanup, "DELETE FROM Object%u WHERE ObjectId IN(SELECT ObjectId FROM Object%u WHERE((Flags&?) AND (MediaId NOT IN(SELECT DISTINCT Value FROM MdE_Preferences WHERE Key=? ORDER BY ExtraValue DESC LIMIT ?)) AND (ObjectId NOT IN(SELECT DISTINCT LeftObjectId FROM Relations%u)) AND (ObjectId NOT IN(SELECT DISTINCT RightObjectId FROM Relations%u))) ORDER BY LastModifiedDate ASC LIMIT ?);" );

	const TInt KMaxLenghtForNamespaceIdsAsText = 4 * KMaxUintValueLength;
	CMdsClauseBuffer* clause = CMdsClauseBuffer::NewLC( 
			KMemoryCardCleanup.iTypeLength + KMaxLenghtForNamespaceIdsAsText );
	
	clause->BufferL().Format( KMemoryCardCleanup, 
		KDefaultNamespaceDefId, 
		KDefaultNamespaceDefId, 
		KDefaultNamespaceDefId, 
		KDefaultNamespaceDefId );
	
	RRowData variables;
    CleanupClosePushL( variables );

	_LIT( KMemoryCardKey, "MC" );
	const TInt KMemoryCardLimit = 1;
	const TInt KObjectCleanupLimit = 1024;
	
	variables.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
	variables.AppendL( TColumn( KMemoryCardKey ) );
	variables.AppendL( TColumn( KMemoryCardLimit ) );
	variables.AppendL( TColumn( KObjectCleanupLimit ) );
	
	const TInt rowCount = iDefaultDBConnection->ExecuteL( clause->ConstBufferL(), variables );

	CleanupStack::PopAndDestroy( 2, clause ); // variables, clause
	}


void CMdSServer::HandleDiskSpaceError(TInt /*aError*/)
	{
	}


CMdSServer* CMdSServer::NewL()
    {
    CMdSServer* MdSServer = CMdSServer::NewLC();
    CleanupStack::Pop( MdSServer );
    return MdSServer;
    }


CMdSServer* CMdSServer::NewLC()
    {
    CMdSServer* MdSServer = new ( ELeave ) CMdSServer( CActive::EPriorityStandard );
    CleanupStack::PushL( MdSServer );
    MdSServer->ConstructL();
    return MdSServer;
    }


void CMdSServer::ConstructL()
    {
    __INIT_LOGGER;
    StartL( KMdSServerName );
    __LOGLB( ELogAlways, "Server start" );

    RProcess process;
    process.SetPriority( EPriorityBackground );
    process.Close();
    
    CheckInitSriptL();
    
    iLockList = CMdSObjectLockList::NewL();
    CMdSSqLiteConnection* conn = CMdSSqLiteConnection::NewLC();
    iDefaultDBConnection = conn;
    MMdSDbConnectionPool::SetDefaultDB( conn );
    CleanupStack::Pop( conn );

    CMdSMaintenanceEngine::InitConnectionL();    
    
    iNotifier = CMdSNotifier::NewL();

    InitializeL();

    iDiskSpaceGarbageCollectorNotifier = 
    	CMdSDiskSpaceNotifierAO::NewL( *this, 
    								   KDiskSpaceGarbageCollectorThreshold,
    								   KMdsSqlDbDefaultName );
    
    iDiskFullNotifier = 
    	CMdSDiskSpaceNotifierAO::NewL( *this, 
    									KDiskFullThreshold,
    									KMdsSqlDbDefaultName );

    // Create a backup & restore watcher and add this server as its observer.								   
    iBURWatcher = CMdEBackupRestoreWatcherAO::NewL(*this);
    
    // create shutdown observer
    iShutdownObserver = CMDSShutdownObserver::NewL( *this );
    iShutdown = EFalse;   
    }

void CMdSServer::InitializeL()
    {
    iMaintenance = CMdSMaintenanceEngine::NewL();

    iSchema = CMdsSchema::NewL();

    iManipulate = CMdSManipulationEngine::NewL( *iSchema, *iNotifier, 
        *iLockList );

    // TRAP InstallL - first time for if there has been schema update, and 
    // the DB version is too old. Delete the DB and try to recreate it
    TRAPD( error, iMaintenance->InstallL( *iManipulate, *iSchema ) );
    
    if( error == KErrCorrupt )
        {
        delete iSchema;
        iSchema = NULL; // for CS
        iSchema = CMdsSchema::NewL();
        
        delete iManipulate;
        iManipulate = NULL; // for CS
        iManipulate = CMdSManipulationEngine::NewL( *iSchema, *iNotifier, 
            *iLockList );
    
        CMdSMaintenanceEngine::InitConnectionL();    
        // TRAP InstallL - second time for if the schema file in private not updated
        // during update, and the first attempt to recreate the DB fails. 
        // Then schema file in rom is used for final attempt to recreate the DB
        TRAP( error, iMaintenance->InstallL( *iManipulate, *iSchema ) );
        
        if( error == KErrCorrupt )
            {
            delete iSchema;
            iSchema = NULL; // for CS
            iSchema = CMdsSchema::NewL();
            
            delete iManipulate;
            iManipulate = NULL; // for CS
            iManipulate = CMdSManipulationEngine::NewL( *iSchema, *iNotifier, 
                *iLockList );
        
            CMdSMaintenanceEngine::InitConnectionL();    
            // If the DB cannot be created from ANY available schema file, nothing can be 
            // can be done at this point, unfortunately
            iMaintenance->InstallL( *iManipulate, *iSchema );
            }
        }
    else if( error != KErrNone )
        {
        User::Leave( error );
        }
    }

void CMdSServer::DeInitializeL()
    {
    if ( iMaintenance )
    	{
    	delete iMaintenance;
    	iMaintenance = NULL;
    	}
    if ( iManipulate )
    	{
    	delete iManipulate;
    	iManipulate = NULL;
    	}
    if ( iSchema )
    	{
    	delete iSchema;
    	iSchema = NULL;
    	}
    }

TInt CMdSServer::ResetDBL()
	{
	DeInitializeL();

	CMdSMaintenanceEngine::DeleteDatabase();
	CMdSMaintenanceEngine::InitConnectionL();
	
    InitializeL();
	return KErrNone;
	}

CMdSServer::CMdSServer( TInt aPriority ) : 
	CPolicyServer(aPriority, KMdsPolicy), 
	iBackupOrRestoreRunning( EFalse ),
	iHarvestingPrioritizationSerializationBuffer( NULL )
    {
    iServerErrorStatus = KErrNone;
    }


CMdSServer::~CMdSServer()
    {
    iShutdown = ETrue;
    iClientThread.Close();
    
	delete iBURWatcher;
	iBURWatcher = NULL;
	delete iDiskSpaceGarbageCollectorNotifier;
	iDiskSpaceGarbageCollectorNotifier = NULL;
	delete iDiskFullNotifier;
	iDiskFullNotifier = NULL;
    delete iManipulate;
    iManipulate = NULL;
    delete iSchema;
    iSchema = NULL;
    delete iNotifier;
    iNotifier = NULL;
    delete iLockList;
    iLockList = NULL;
    delete iMaintenance;
    iMaintenance = NULL;
    delete iDefaultDBConnection;
    iDefaultDBConnection = NULL;

    delete iHarvestingPrioritizationSerializationBuffer;
    iHarvestingPrioritizationSerializationBuffer = NULL;
	iHarvestingPrioritizationChunk.Close();
	
	delete iShutdownObserver;
	iShutdownObserver = NULL;

    __LOGLB( ELogAlways, "Server stop" );
    __DESTROY_LOGGER;
    }


CSession2* CMdSServer::NewSessionL( const TVersion& aVersion, const RMessage2& /*aMessage*/ ) const
    {
    // Check we are the right version
    if ( !User::QueryVersionSupported( TVersion( KMdSServMajorVersionNumber,
                                                 KMdSServMinorVersionNumber,
                                                 KMdSServBuildVersionNumber ),
                                       aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

	if ( iShutdownObserver->UpdateInProgress() )
        {
        __LOGLB( ELogAlways, "CMdSServer::NewSessionL: iad update in progress: KErrLocked");
        User::Leave(KErrLocked);
        }

    if ( iServerErrorStatus != KErrNone )
        {
        // leave with the same error code that the server has
        User::Leave ( iServerErrorStatus );
        }

    __LOGLB( ELogAlways, "New Session" );
    // Make new session
    return CMdSServerSession::NewL( *const_cast<CMdSServer*> ( this ) );
    }

CMdsSchema& CMdSServer::Schema()
    {
    return *iSchema;
    }

CMdSObjectLockList& CMdSServer::LockList()
    {
    return *iLockList;
    }

CMdSNotifier& CMdSServer::Notifier()
    {
    return *iNotifier;
    }

CMdSMaintenanceEngine& CMdSServer::Maintenance()
    {
    return *iMaintenance;
    }

CMdSManipulationEngine& CMdSServer::Manipulate()
    {
    return *iManipulate;
    }

TBool CMdSServer::DiskFull() const
	{
	return iDiskFullNotifier->DiskFull();
	}
   
TBool CMdSServer::BackupOrRestoreRunning() const
	{
	return iBackupOrRestoreRunning;
	}
  
TBool CMdSServer::ShutdownInProgress() const
    {
    return iShutdown;
    }

// -----------------------------------------------------------------------------
// CMdSServer::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CMdSServer::ShutdownNotification()
    {
    if (!iShutdown)
        {    
        User::RenameThread(KMdSServerNameIAD);
        CActiveScheduler::Stop();
        iShutdown = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CMdSServer::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CMdSServer::RestartNotification()
    {
    }


TInt CMdSServer::SetHarvestingPrioritizationChunkL( const RMessagePtr2 aMessage, TInt aParam )
	{
	// if there happens to be old chunk, close it
	iHarvestingPrioritizationChunk.Close();

	const TInt error = iHarvestingPrioritizationChunk.Open( aMessage, aParam, EFalse );

	if( error != KErrNone )
		{
		return error;
		}

	if( iHarvestingPrioritizationSerializationBuffer )
		{
		delete iHarvestingPrioritizationSerializationBuffer;
		iHarvestingPrioritizationSerializationBuffer = NULL;
		}

	iHarvestingPrioritizationSerializationBuffer = 
		CMdCSerializationBuffer::NewL( 
			iHarvestingPrioritizationChunk.Base(), 
			iHarvestingPrioritizationChunk.Size() );
	
	iHarvestingPrioritizationLocked = EFalse;

	return KErrNone;
	}

TInt CMdSServer::AddHarvestingPrioritizationObserver( RMessagePtr2 aMessage )
	{
	if( iHarvestingPrioritizationObserver.IsNull() )
		{
		iHarvestingPrioritizationLocked = EFalse;

		iClientThread.Close();
		iHarvestingPrioritizationObserver = aMessage;
		iHarvestingPrioritizationObserver.Client( iClientThread );

		// reserve space for harvesting prioritization URI count
		iHarvestingPrioritizationLimit = CMdCSerializationBuffer::KRequiredSizeForTInt32;
		iHarvestingPrioritizationUriCount = 0;
		
		return KErrNone;
		}
	else
	    {
        TExitType exitType = iClientThread.ExitType();
        if( EExitPending != exitType )
            {
            iHarvestingPrioritizationLocked = EFalse;

            iClientThread.Close();
            iHarvestingPrioritizationObserver = aMessage;
            iHarvestingPrioritizationObserver.Client( iClientThread );

            // reserve space for harvesting prioritization URI count
            iHarvestingPrioritizationLimit = CMdCSerializationBuffer::KRequiredSizeForTInt32;
            iHarvestingPrioritizationUriCount = 0;
        
            return KErrNone;
            }
	    }

	return KErrAlreadyExists;
	}

TInt CMdSServer::CancelHarvestingPrioritizationObserver()
	{	
	iHarvestingPrioritizationLocked = EFalse;

	if( iHarvestingPrioritizationObserver.IsNull() )
		{
		return KErrNotFound;
		}
	
	NotifyHarvestingPrioritizationObserver( KErrCancel );
	return KErrNone;
	}

TBool CMdSServer::ReserveSpaceHarvestingPrioritizationUri( const TDesC16& aUri )
	{
	if( !iHarvestingPrioritizationLocked && 
		iHarvestingPrioritizationObserver.IsNull() == EFalse && 
		iHarvestingPrioritizationSerializationBuffer )
		{
		// reserve space for URI offset and URI
		const TInt reserveSpace = 
			CMdCSerializationBuffer::KRequiredSizeForTUint32 + 
			CMdCSerializationBuffer::RequiredSize( aUri );

		const TInt maxBufferSize = 
			iHarvestingPrioritizationSerializationBuffer->Size();

		// check if buffer has enough space for the new URI
		if( maxBufferSize >= ( iHarvestingPrioritizationLimit + reserveSpace ) )
			{			
			iHarvestingPrioritizationLimit += reserveSpace;
			iHarvestingPrioritizationUriCount++;
			return ETrue;
			}
		// if buffer doesn't have enough space for the new uri, 
		// discard current URI and all following URIs
		else
			{
			iHarvestingPrioritizationLimit = maxBufferSize;
			return EFalse;
			}
		}
	return EFalse;
	}

TBool CMdSServer::StartAddingHarvestingPrioritizationUrisL()
	{
	if( !iHarvestingPrioritizationLocked && 
		iHarvestingPrioritizationObserver.IsNull() == EFalse && 
		iHarvestingPrioritizationSerializationBuffer )
		{
		iHarvestingPrioritizationLocked = ETrue;

		// add URI count to harvesting prioritization buffer
		iHarvestingPrioritizationSerializationBuffer->PositionL( 0 );
		iHarvestingPrioritizationSerializationBuffer->InsertL( 
			iHarvestingPrioritizationUriCount );

		iHarvestingPrioritizationBufferUriCount = 0;

		// set URI offset to position of the first URI
		iHarvestingPrioritizationBufferUriOffset = 
			CMdCSerializationBuffer::KRequiredSizeForTInt32 + 
			CMdCSerializationBuffer::KRequiredSizeForTUint32 * 
			iHarvestingPrioritizationUriCount;

		return ETrue;
		}

	return EFalse;
	}

TBool CMdSServer::AddHarvestingPrioritizationUriL( TDesC16& aUri )
	{
	if( iHarvestingPrioritizationObserver.IsNull() == EFalse && 
		iHarvestingPrioritizationSerializationBuffer )
		{
		const TUint32 offset = 
			CMdCSerializationBuffer::KRequiredSizeForTInt32 + 
			CMdCSerializationBuffer::KRequiredSizeForTUint32 * 
			iHarvestingPrioritizationBufferUriCount;

		iHarvestingPrioritizationSerializationBuffer->PositionL( offset );
		iHarvestingPrioritizationSerializationBuffer->InsertL( 
			iHarvestingPrioritizationBufferUriOffset );

		// move to URI offset, insert URI and get new URI offset
		iHarvestingPrioritizationSerializationBuffer->PositionL( 
			iHarvestingPrioritizationBufferUriOffset );
		iHarvestingPrioritizationBufferUriOffset = 
			iHarvestingPrioritizationSerializationBuffer->InsertL( aUri );

		iHarvestingPrioritizationBufferUriCount++;
		
		return ETrue;
		}
	return EFalse;
	}

void CMdSServer::NotifyHarvestingPrioritizationObserver( TInt aStatus ) const
	{
	if( !iHarvestingPrioritizationObserver.IsNull() )
		{
	    TExitType exitType = iClientThread.ExitType();
	    if( EExitPending == exitType )
	        {
	        iHarvestingPrioritizationObserver.Complete( aStatus );
	        }
		}
	}

TInt CMdSServer::RunError( TInt aError )
    {
    __LOG1( ELogAlways, "Server::RunError %d", aError );

    if ( aError == KErrBadDescriptor )
        {
        // A bad descriptor error implies a badly programmed client,
        // so panic it; otherwise report the error to the client
        PanicClient( Message(), KErrBadDescriptor );
        }
    else
        {
        Message().Complete( aError );
        }

    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();

    return KErrNone;    // Handled the error fully
    }



void CMdSServer::PanicClient( const RMessage2& aMessage, TInt aPanic )
    {
    __LOG1( ELogAlways, "Server::PanicClient %d", aPanic );
    aMessage.Panic( KMetadataEngine, aPanic );
    }


void CMdSServer::PanicServer( TMetadataPanic aPanic )
    {
    __LOG1( ELogAlways, "Server::PanicServer %d", aPanic );
    MMdCCommon::Panic( aPanic );
    }

void CMdSServer::ThreadFunctionL()
    {
    User::LeaveIfError(User::RenameThread(KMdSServerName));
    // Construct active scheduler
    CActiveScheduler* activeScheduler = new ( ELeave ) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );

    // Install active scheduler
    // We don't need to check whether an active scheduler is already installed
    // as this is a new thread, so there won't be one
    CActiveScheduler::Install( activeScheduler );

    // Construct our server
    CMdSServer::NewLC();    // Anonymous

    RProcess::Rendezvous(KErrNone);

    // Start handling requests
    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy( 2, activeScheduler ); 
    }


TInt CMdSServer::ThreadFunction( TAny* /*aNone*/ )
    {
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if ( !( cleanupStack ) )
        {
        PanicServer( ECreateTrapCleanup );
        }

    TRAPD( err, ThreadFunctionL() );
    if ( err != KErrNone )
        {
        PanicServer( ESrvCreateServer );
        }

    delete cleanupStack;
    cleanupStack = NULL;

    return KErrNone;
    }
    

// Backup & restore methods
void CMdSServer::HandleBackup()
	{
	iBackupOrRestoreRunning = ETrue;
	
	// Free resources for back up.
	iManipulate->GarbageCollector().Pause();
	
	// disconnect all DB connections
	CMdSMaintenanceEngine::CloseDatabase();
	}

void CMdSServer::HandleRestore()
	{
	iBackupOrRestoreRunning = ETrue;

	// Free resources for restore.
	iManipulate->GarbageCollector().Pause();

	// disconnect all DB connections
	CMdSMaintenanceEngine::CloseDatabase();
	
	// Currently trust that device will reboot after restore.
	}

void CMdSServer::ResumeOperation()
	{
	iBackupOrRestoreRunning = EFalse;

	// Resume normal operation after back up.

	// restore all DB connections
	TRAPD( error, CMdSMaintenanceEngine::InitConnectionL() );

	// if setting back DB connections fails 
	// something very bad has happened during backup or restore
	if( KErrNone != error )
		{
		MMdCCommon::Panic( EServerBackupOrRestore );
		}

	// Restore resources
	iManipulate->GarbageCollector().Resume();
	}

void CMdSServer::CheckInitSriptL()
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    TBuf<KMaxFileName> privatePath;
    TBuf<KMaxFileName> schema;
    TBuf<KMaxFileName> defaultImportProfile;
    TBuf<KMaxFileName> backupRegistration;
    RFileReadStream tmpFile;
    
    fs.PrivatePath( privatePath );
    
    schema.Copy( privatePath );
    schema.Append( KSchema );
    
    defaultImportProfile.Copy( privatePath );
    defaultImportProfile.Append( KDefaultImportProfile );
    
    backupRegistration.Copy( privatePath );
    backupRegistration.Append( KBackupRegistration );
    
    CFileMan* fileMan = CFileMan::NewL( fs );
    CleanupStack::PushL( fileMan );
    
    CheckAndInitializeFileL( fs, schema, KSchemaPath(), tmpFile, fileMan );
    
    CheckAndInitializeFileL( fs, defaultImportProfile, KDefaultImportProfilePath(), tmpFile, fileMan );    
    
    CheckAndInitializeFileL( fs, backupRegistration, KBackupRegistrationPath(), tmpFile, fileMan );

    CleanupStack::PopAndDestroy( 2 ); //fileman, fs
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

TInt E32Main()
    {
    __UHEAP_MARK;
 	CTrapCleanup* cleanup=CTrapCleanup::New();
 	TInt result = KErrNoMemory;
 	if (cleanup)
 		{
 		TRAP(result, CMdSServer::ThreadFunctionL());
		delete cleanup;
		cleanup = NULL;
 		}
    __UHEAP_MARKEND;
 	return result;
    }
    
// End of File

