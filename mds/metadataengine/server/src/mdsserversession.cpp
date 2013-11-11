/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is Metadata engine server session file
*
*/


// INCLUDE FILES
#include "mdsserversession.h"

#include "mdcresult.h"
#include "mdslogger.h"
#include "mdcserializationbuffer.h"
#include "mdsserver.h"
#include "mdsmanipulationengine.h"
#include "mdsmaintenanceengine.h"
#include "mdsfindengine.h"
#include "mdsobjectlocklist.h"
#include "mdsnotifier.h"
#include "mdsschema.h"
#include "mdcresult.h"
#include "mdcitem.h"

#include "mdsutils.h"

__USES_LOGGER


// ========================= MEMBER FUNCTIONS ==================================


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CMdSServerSession* CMdSServerSession::NewL( CMdSServer& aServer )
    {
    CMdSServerSession* self = CMdSServerSession::NewLC( aServer );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CMdSServerSession* CMdSServerSession::NewLC( CMdSServer& aServer )
    {
    CMdSServerSession* self = new ( ELeave ) CMdSServerSession( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CMdSServerSession::CMdSServerSession( CMdSServer& aServer )
	: iServer( aServer ) 
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMdSServerSession::~CMdSServerSession()
    {
    iFindEngines.ResetAndDestroy();
    iFindEngines.Close();

    if( !iServer.ShutdownInProgress() )
        {
        iServer.LockList().UnlockBySession( *this );
        iServer.Notifier().RemoveEntriesBySession( *this );
        }

    // purge any pending notifications
	iNotificationCache.ResetAndDestroy();
    iNotificationCache.Close();
    }

// ---------------------------------------------------------------------------
// Service the server message 
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ServiceL( const RMessage2& aMessage )
    {
    __LOG2( ELogServer, "ServiceL message: %d uid: %d", 
    		aMessage.Function(),
    		aMessage.Identity().iUid);
    
	if( iServer.BackupOrRestoreRunning() )
		{
        // Open client applications are closed during backup/restore,
        // thus registered observers that would be removed during
        // shutdown on clients, must be allowed to be removed even
        // if backup/restore is running
	    if( aMessage.Function() == EUnregister )
	        {
            TRAPD( err, ServiceFunctionL( aMessage ) );
            if( err != KErrNone )
                {
                aMessage.Complete( err );
                }
            return;
	        }
		aMessage.Complete( KErrServerBusy );
		return;
		}

    if( iServer.ShutdownInProgress() )
        {
        aMessage.Complete( KErrServerTerminated );
        return;
        }
	
    TRAPD( err, ServiceFunctionL( aMessage ) );
    if( err != KErrNone )
        {
        aMessage.Complete( err );
        }
    }

// ---------------------------------------------------------------------------
// ServiceFunctionL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ServiceFunctionL( const RMessage2& aMessage )
    {
    TInt complete(KErrNone);

    switch ( aMessage.Function() )
        {
        // Add items 
        case EAdd:
            {
            AddL( aMessage );
            break;
            }

        // Add relation def
        case EAddRelationDef:
        	{
        	AddRelationDefL(aMessage);
        	iServer.Notifier().NotifySchemaAddedL();
        	break;
        	}

        // Add event def
        case EAddEventDef:
        	{
        	AddEventDefL(aMessage);
        	iServer.Notifier().NotifySchemaAddedL();
        	break;
        	}        	

        // Remove items
        case ERemove:
            {
            RemoveL( aMessage );
            break;
            }

        // Find items
        case EFind:
            {
            TRAPD( err, FindL( aMessage ) );
            if ( err != KErrNone )
                {
                if ( err != KErrNotReady )
                    {
                    FindCancel( aMessage, err );
                    return;
                    }
	            complete = err;
                }
            break;
            }

        case EAsyncFind:
            {
            TRAPD( err, FindAsyncL( aMessage ) );
            if ( err != KErrNone)
            	{
            	if(err != KErrNotReady)
	                {
	                FindCancel( aMessage, err );
	                return;
	                }
	    		complete = err;
            	}
            else 
            	{
            	// if no error don't complete message
            	return;
            	}
            break;
            }

        case EContinueAsyncFind:
            {
            TRAPD( err, FindContinueAsyncL( aMessage ) );
            if ( err != KErrNone )
                {
                FindCancel( aMessage, err );
                }
            
            // if no error don't complete message
            return;
            }

        case ECancelFind:
            {
            FindCancel( aMessage, KErrNone );
            return;
            }

        case ECheckObject:
            {
            CheckObjectL( aMessage );
            break;
            }
            
        case EGetData:
            {
            GetDataL( aMessage );
            break;
            }

        case ECancelObject:
            {
            CancelObjectL( aMessage );
            break;
            }

        case EUpdate:
            {
            UpdateL( aMessage );
            break;
            }

        case ERegister:
            {
            RegisterL( aMessage );
            break;
            }

        case EListen:
            {
            ListenL( aMessage );
            
            // if no error don't complete message
            return;
            }

        case EUnregister:
            {
            UnregisterL( aMessage );
            break;
            }
            
        case EShutdown:
            {
            ShutdownL( aMessage );
            break;
            }
            
        case EImportMetadata:
        case EAsyncImportMetadata:
            {
            ImportMetadataL( aMessage );
            break;
            }

        case EExportMetadata:
        case EAsyncExportMetadata:
            {
            ExportMetadataL( aMessage );
            break;
            }

        case EImportSchema:
            {
            TRAPD( err, ImportSchemaL( aMessage ) );
            if(err != KErrNone && err != KErrAccessDenied)
            	{
            	// Map all other errors to KErrCorrupt
            	err = KErrCorrupt;
            	}

            if(!err)
            	{
            	iServer.Notifier().NotifySchemaAddedL();
            	}

			complete = err;
            break;
            }

        case EAddMemoryCard:
        	{
        	AddMemoryCardL( aMessage );
        	break;
        	}

    	case EGetMemoryCard:
        	{
        	GetMemoryCardL( aMessage );
        	break;
        	}

    	case ECheckMemoryCard:
        	{
        	CheckMemoryCardL( aMessage );
        	break;
        	}

    	case ESetMedia:
    		{
    		SetMediaL( aMessage );
    		break;
    		}
        
    	case EGetMedia:
    		{
    		GetMediaL( aMessage );
    		break;
    		}
        	
    	case EGetPresentMedias:
    		{
    		GetPresentMediasL( aMessage );
    		break;
    		}
        	
    	case ESetFileToPresent:
        	{
        	complete = KErrNotSupported;
        	break;
        	}

    	case ESetFilesToPresent:
        	{
        	SetFilesToPresentL(aMessage);
        	break;
        	}        	
        	
    	case ESetFilesToNotPresent:
        	{
        	SetFilesToNotPresentL(aMessage);
        	break;
        	}

    	case ERemoveFilesNotPresent:
        	{
        	RemoveFilesNotPresentL(aMessage);
        	break;
        	}

    	case EGetSchemaVersion:
        	{
        	GetSchemaVersionL(aMessage);
        	break;
        	}

    	case ESetObjectToPresentByGuid:
    		{
    		SetObjectToPresentByGuidL(aMessage);
    		break;
    		}
        	
        case EResetDB:
        	{
			#ifdef _DEBUG
        	iServer.ResetDBL();
			#else
        	User::Leave( KErrNotSupported );
			#endif
        	break;
        	}
        	
        case ESetHarvestingPrioritizationChunk:
        	{
        	iServer.SetHarvestingPrioritizationChunkL( aMessage, 0 );
        	break;
        	}

        case EAddHarvestingPrioritizationObserver:
        	{
            User::LeaveIfError( iServer.AddHarvestingPrioritizationObserver( aMessage ) );
            // if no error don't complete message
        	return;
        	}

        case ECancelHarvestingPrioritizationObserver:
        	{
        	User::LeaveIfError( iServer.CancelHarvestingPrioritizationObserver() );
        	break;
        	}

        case EChangePath:
        	{
        	ChangePathL( aMessage );
        	break;
        	}
        	
        case EChangeMediaId:
        	{
        	ChangeMediaIdL( aMessage );
        	break;
        	}	

        case EChangeMassStorageMediaId:
            {
            CheckMassStorageMediaIdL( aMessage );
            break;
            }   
        	
        case ESetPending:
        	{
        	SetPendingL( aMessage );
        	}
        	break;

        case EResetPending:
        	{
        	ResetPendingL( aMessage );
        	}
        	break;

        case EGetPendingCount:
        	{
        	complete = GetPendingCountL( aMessage );
        	}
        	break;

        case EGetPending:
        	{
        	complete = GetPendingL( aMessage );
        	}
        	break;
        	
        default:
            iServer.PanicClient( aMessage, EBadRequest );
        }

    aMessage.Complete(complete);
    }

// ---------------------------------------------------------------------------
// QueriesCompleteL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::QueriesCompleteL()
	{
	// check if all queries are complete
    for( TInt i = iFindEngines.Count() - 1; i >= 0; i--)
    	{
    	CMdSFindEngine* fe = iFindEngines[i];

    	if( !fe->IsComplete() )
        	{
    	    User::Leave( KErrNotReady );
			}
    	}		
	}

// ---------------------------------------------------------------------------
// AddL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::AddL( const RMessage2& aMsg )
    {
	if ( iServer.DiskFull() )
		{
		User::Leave( KErrDiskFull );
		}

	TInt resultLength = aMsg.GetDesLengthL( 1 );
	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aMsg, 0 );	
	CMdCSerializationBuffer* resultBuf = CMdCSerializationBuffer::NewLC( resultLength );

    iServer.Manipulate().AddL( *buffer, *resultBuf, this );

	aMsg.WriteL(1, resultBuf->Buffer() );
	CleanupStack::PopAndDestroy( resultBuf );
	CleanupStack::PopAndDestroy( buffer );
    }

// ---------------------------------------------------------------------------
// AddRelationDefL
// ---------------------------------------------------------------------------
//	
void CMdSServerSession::AddRelationDefL(const RMessage2 &aMsg)	
	{
	TDefId namespaceId = aMsg.Int0();

	TInt nameLength = aMsg.GetDesLength( EAddDefArgDefName );
	if ( nameLength < 0 )
		{
		User::Leave( KErrBadDescriptor );
		}
	HBufC* namebuf = HBufC::NewLC( nameLength );
	TPtr bufdes = namebuf->Des();
	aMsg.ReadL( EAddDefArgDefName, bufdes );

	iServer.Manipulate().AddRelationDefL( namespaceId, *namebuf );

	iServer.Schema().SerializeToSharedMemoryL();
	
	CleanupStack::PopAndDestroy( namebuf );
	}
	
void CMdSServerSession::AddEventDefL(const RMessage2 &aMsg)	
	{
	TDefId namespaceId = aMsg.Int0();

	TInt nameLength = aMsg.GetDesLength( EAddDefArgDefName );
	if ( nameLength < 0 )
		{
		User::Leave( KErrBadDescriptor );
		}
	HBufC* namebuf = HBufC::NewLC( nameLength );
	TPtr bufdes = namebuf->Des();
	aMsg.ReadL( EAddDefArgDefName, bufdes );

	iServer.Manipulate().AddEventDefL( namespaceId, *namebuf );
	
	iServer.Schema().SerializeToSharedMemoryL();
	
	CleanupStack::PopAndDestroy( namebuf );
	}	

// RemoveL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::RemoveL( const RMessage2& aMsg )
    {
	TInt successfulLength = aMsg.GetDesLengthL( 1 );

    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aMsg, 0 );
    CMdCSerializationBuffer* successfullBuffer = CMdCSerializationBuffer::NewLC( successfulLength );

    iServer.Manipulate().RemoveL( *buffer, *successfullBuffer );
    aMsg.WriteL( 1, successfullBuffer->Buffer() );
    CleanupStack::PopAndDestroy( 2, buffer ); // successfullBuffer, buffer
    }

// ---------------------------------------------------------------------------
// FindL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::FindL( const RMessage2& aMsg )
    {
    __LOGLB( ELogServer, "-> Find Sync" );

	// check that all queries are complete
	QueriesCompleteL();

    const TInt serializedCriteriaLength = aMsg.GetDesLength( EFindArgConditions );
	CMdCSerializationBuffer* serializedCriteriaBuffer = CMdCSerializationBuffer::NewLC( serializedCriteriaLength );
	TPtr8 serializedCriteriaBufferPtr(serializedCriteriaBuffer->Buffer());
	aMsg.ReadL( EFindArgConditions, serializedCriteriaBufferPtr );

    const TUint32 queryId = (TUint32)aMsg.Int0();

    CMdSFindEngine* find = CMdSFindEngine::NewLC( queryId, *this,
        iServer.LockList(), iServer.Schema() );
    find->SetFindCriteria( serializedCriteriaBuffer );
    find->CreateResultSet( aMsg );
    find->SetFindParams( (TUint32)aMsg.Int3() );

    CleanupStack::Pop( find );
    CleanupStack::Pop( serializedCriteriaBuffer );
    CleanupStack::PushL( find );

    // check clients user level (whether access to confidential data or not)
    TUserLevel userLevel = EUserLevelNone;
    userLevel = ( aMsg.HasCapability( ECapabilityReadDeviceData ) ?
                  EUserLevelDeviceAccess : EUserLevelNormal );

    find->FindL( userLevel );

    CleanupStack::Pop( find );
    
    iFindEngines.AppendL( find );
    }

// ---------------------------------------------------------------------------
// FindAsyncL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::FindAsyncL( const RMessage2& aMsg )
    {
    __LOGLB( ELogServer, "-> Find Async" );

    // check that all queries are complete
    QueriesCompleteL();

    const TInt serializedCriteriaLength = aMsg.GetDesLength( EFindArgConditions );
	CMdCSerializationBuffer* serializedCriteriaBuffer = CMdCSerializationBuffer::NewLC( serializedCriteriaLength );
	TPtr8 serializedCriteriaBufferPtr(serializedCriteriaBuffer->Buffer());
	aMsg.ReadL( EFindArgConditions, serializedCriteriaBufferPtr );

    const TUint32 queryId = (TUint32)aMsg.Int0();

    CMdSFindEngine* find = CMdSFindEngine::NewLC( queryId, *this,
        iServer.LockList(), iServer.Schema() );
    find->SetFindCriteria( serializedCriteriaBuffer );
    find->CreateResultSet( aMsg );
    find->SetFindParams( (TUint32)aMsg.Int3() );

    CleanupStack::Pop( find );
    CleanupStack::Pop( serializedCriteriaBuffer );    
    CleanupStack::PushL( find );

    // check clients user level (whether access to confidential data or not)
    TUserLevel userLevel = EUserLevelNone;
    userLevel = ( aMsg.HasCapability( ECapabilityReadDeviceData ) ?
                  EUserLevelDeviceAccess : EUserLevelNormal );

    find->FindAsyncL( userLevel );

    CleanupStack::Pop( find );
    
    iFindEngines.AppendL( find );
    }

// ---------------------------------------------------------------------------
// FindContinueAsyncL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::FindContinueAsyncL( const RMessage2& aMsg )
    {
    __LOGLB( ELogServer, "-> Find Continue" );

    CMdSFindEngine* findEngine = NULL;

    const TUint32 queryId = (TUint32)aMsg.Int0();
    
    TInt feIndex = 0;

    for( TInt i = iFindEngines.Count() - 1; i >=0; i-- )
    	{
    	if( queryId == iFindEngines[i]->QueryId() )
    		{
        	findEngine = iFindEngines[i];

    		feIndex = i;
    		break;
    		}
    	}

    // no correct find engine found
    if( !findEngine )
    	{
    	User::Leave( KErrNotFound );
    	}
    // find engine is already complete
    else if( findEngine->IsComplete() )
    	{
    	delete findEngine;
    	findEngine = NULL;

    	iFindEngines.Remove( feIndex );
    	
    	User::Leave( KErrCompletion );
    	}

    findEngine->CreateResultSet( aMsg );

    findEngine->ContinueAsync();
    }

// ---------------------------------------------------------------------------
// FindCancel
// ---------------------------------------------------------------------------
//
void CMdSServerSession::FindCancel( const RMessage2& aMsg, TInt aError )
    {
    __LOGLB( ELogServer, "-> Find Cancel" );
    
    const TUint32 queryId = (TUint32)aMsg.Int0();

    for( TInt i = iFindEngines.Count() - 1; i >=0; i-- )
    	{
    	CMdSFindEngine* findEngine = iFindEngines[i];

    	if( queryId == findEngine->QueryId() )
    		{
    		findEngine->Cancel( aError );

    		delete findEngine;
    		findEngine = NULL;

    		iFindEngines.Remove( i );

    		aMsg.Complete( aError );
    		return;
    		}
    	}

    // complete message even if correct find engine wasn't found
    aMsg.Complete( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CheckObjectL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::CheckObjectL( const RMessage2& aMsg )
	{	
	TPckgBuf<TDefId> namespaceDefIdPckg;
	TInt nsDefIdLength = aMsg.GetDesLengthL( ECheckObjectArgNamespaceDefId );
	if( nsDefIdLength != sizeof( TDefId ) )
		{
		User::Leave( KErrArgument );
		}	
	aMsg.Read( ECheckObjectArgNamespaceDefId, namespaceDefIdPckg );
	const TDefId namespaceDefId = namespaceDefIdPckg();

	const TInt resultBufferLength = aMsg.GetDesLengthL( ECheckObjectArgObject );
	if( resultBufferLength <= 0 )
		{
		User::Leave( KErrBadDescriptor );
		}

	CMdCSerializationBuffer* buffer = NULL;

	const TInt type = aMsg.Int0();
	switch( type )
		{
		case ECheckObjectByUri:
			{
			TInt uriLength = aMsg.GetDesLengthL( ECheckObjectArgTypeValue );
			if( uriLength <= 0 )
				{
				User::Leave( KErrArgument );
				}
		    RBuf uri;
		    uri.CreateL( uriLength );
		    CleanupClosePushL( uri );
		    aMsg.ReadL( ECheckObjectArgTypeValue, uri );
		    buffer = iServer.Manipulate().CheckObjectL( resultBufferLength, uri, namespaceDefId );
			CleanupStack::PopAndDestroy( &uri );
			CleanupStack::PushL( buffer );
			}
			break;

		case ECheckObjectById:
			{
			TPckgBuf<TItemId> idPckg;
			TInt idLength = aMsg.GetDesLengthL( ECheckObjectArgTypeValue );
			if( idLength != sizeof( TItemId ) )
				{
				User::Leave( KErrArgument );
				}
			aMsg.Read( ECheckObjectArgTypeValue, idPckg );
			const TItemId id = idPckg();
			buffer = iServer.Manipulate().CheckObjectL( resultBufferLength, id, namespaceDefId );
			CleanupStack::PushL( buffer );
			}
			break;

		case ECheckObjectByIds:
			{
			TInt idLength = aMsg.GetDesLengthL( ECheckObjectArgTypeValue );
			if( idLength < CMdCSerializationBuffer::KRequiredSizeForTUint32 )
				{
				User::Leave( KErrArgument );
				}

			CMdCSerializationBuffer* ids = CMdCSerializationBuffer::NewLC( idLength );
			TPtr8 idsPtr( ids->Buffer() );
			aMsg.ReadL( ECheckObjectArgTypeValue, idsPtr );
			
			buffer = iServer.Manipulate().CheckObjectL( resultBufferLength, *ids, namespaceDefId );
			
			CleanupStack::PopAndDestroy( ids );
			
			CleanupStack::PushL( buffer );
			}
			break;

		default:
			{
			User::Leave( KErrNotSupported );
			}
		}

	aMsg.WriteL( ECheckObjectArgObject, buffer->Buffer() );
	CleanupStack::PopAndDestroy( buffer );
	}

// ---------------------------------------------------------------------------
// GetDataL writes to the client the data from the required operation
// ---------------------------------------------------------------------------
//
void CMdSServerSession::GetDataL( const RMessage2& aMsg )
    {
    TMdEServRqst serverRequest = (TMdEServRqst)aMsg.Int2();

    switch( serverRequest )
        {
        case ELoadSchema:
            {
            __LOGLB( ELogServer, "-> Get data (Schema)" );
		    TPckgBuf<TInt> handleBuf( iServer.Schema().SharedMemoryHandleL() );
    		aMsg.WriteL( 1, handleBuf );
            }
            break;
        case EFind:
        case EAsyncFindSetReady:
        case EAsyncFindComplete:
            {
            __LOGLB( ELogServer, "-> Get data (Find)" );

            const TUint32 queryId = (TUint32)aMsg.Int0();
            
            CMdSFindEngine* findEngine = NULL;

            const TInt count = iFindEngines.Count();

            TInt findEngineIndex;
            for( findEngineIndex = 0; findEngineIndex < count; findEngineIndex++ )
            	{
            	if( queryId == iFindEngines[findEngineIndex]->QueryId() )
            		{
            		findEngine = iFindEngines[findEngineIndex];
            		break;
            		}
            	}

            if( !findEngine )
            	{
            	User::Leave( KErrNotFound );
            	}

            TMdCQueryLockType extraData = (TMdCQueryLockType)aMsg.Int3();
            if (extraData == ELock && aMsg.HasCapability( ECapabilityWriteDeviceData ) )
            	{
            	findEngine->LockFindResultObjectsL( iServer.LockList() );
            	}

            // write result to client's result buffer
            aMsg.WriteL( 1, findEngine->ResultsL().Buffer() );

            if ( serverRequest != EAsyncFindSetReady )
                {
                delete findEngine;
                findEngine = NULL;
                
                iFindEngines.Remove( findEngineIndex );
                iFindEngines.Compress();
                }
            }
            break;
        case EListen:
            {
            __LOGLB( ELogServer, "-> Get data (Listen)" );
            CMdSNotifier::TEntry& entry = iServer.Notifier().FindEntryL( aMsg.Int3() );
            CMdCSerializationBuffer* buffer = entry.GetDataBuffer();
        	CleanupStack::PushL( buffer );
        	aMsg.WriteL( 1, buffer->Buffer() );
        	CleanupStack::PopAndDestroy( buffer );
            }
            break;
        default:
            User::Leave( KErrNotSupported );
        }
    }

// ---------------------------------------------------------------------------
// CancelObjectL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::CancelObjectL( const RMessage2& aMsg )
    {
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aMsg, 0 );

    const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *buffer );
    if ( itemIds.iObjectIds.iPtr.iCount != 1 )
    	{
    	User::Leave( KErrArgument );
    	}
	TItemId idValue;
	buffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
	buffer->ReceiveL( idValue );

	CMdsSchema& schema = iServer.Schema();
	const CMdsNamespaceDef* namespaceDef = schema.GetNamespaceByIdL( itemIds.iNamespaceDefId );

	if (iServer.LockList().IsLocked( *namespaceDef, idValue ) )
		{
		iServer.LockList().UnlockById( *namespaceDef, idValue );
		}
	else
		{
		idValue = KNoId;
		}

	buffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
	buffer->InsertL( idValue );

    aMsg.WriteL( 0, buffer->Buffer() );
    CleanupStack::PopAndDestroy( buffer ); // buffer
    }

// ---------------------------------------------------------------------------
// UpdateL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::UpdateL( const RMessage2& aMsg )
    {
    const TInt successfulLength = aMsg.GetDesLengthL( 1 );
    
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aMsg, 0 );
    CMdCSerializationBuffer* successfullBuffer = CMdCSerializationBuffer::NewLC( successfulLength );

    iServer.Manipulate().UpdateL( *buffer, *successfullBuffer );
    aMsg.WriteL( 1, successfullBuffer->Buffer() );
    CleanupStack::PopAndDestroy( 2, buffer ); // successfullBuffer, buffer
    }
    
// ---------------------------------------------------------------------------
// RegisterL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::RegisterL( const RMessage2& aMsg )
    {
    __LOG3( ELogServer, "-> Register %u for NS: %u Type: %d",
        (TUint32)aMsg.Int0(), (TDefId)aMsg.Int3(), aMsg.Int1() );

    const TInt length = aMsg.GetDesLength( 2 );
	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( length );
	TPtr8 bufferPtr( buffer->Buffer() );
	aMsg.ReadL( 2, bufferPtr );
    
    iServer.Notifier().CreateEntryL( aMsg.Int0(),
        (TConditionType)aMsg.Int1(), buffer, (TDefId)aMsg.Int3(), *this, 
        aMsg.HasCapability(ECapabilityReadDeviceData) );
    
    CleanupStack::Pop( buffer );                
    }

// ---------------------------------------------------------------------------
// ListenL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ListenL( const RMessage2& aMsg )
    {
    const TInt notifierId = aMsg.Int0();
    __LOG1( ELogServer, "-> Listen %d", notifierId );

    CMdSNotifier::TEntry& entry = iServer.Notifier().FindEntryL( notifierId );
    entry.SetupForCallback( aMsg, 1 );

    const TInt entryId = entry.Id();    

    TInt low( 0 );
    TInt high( iNotificationCache.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( entryId - iNotificationCache[mid]->iId );
        if( compare == 0 )
            {
            // The cache holds a new notification for this notifier, trigger it
            CNotificationCacheItem* item = iNotificationCache[mid];
            iNotificationCache.Remove(mid);
            iNotificationCache.Compress();

            CleanupStack::PushL( item );

            entry.TriggerCachedL( item->iCode, item->iData );
        
            // take ownership of iData from item and delete it
            item->iData = NULL;
            CleanupStack::PopAndDestroy( item );
        
            return;
            }
        else if( compare > 0 )
            {
            low = mid + 1;
            }
        else
            {
            high = mid;
            }
        }
    }

// ---------------------------------------------------------------------------
// UnregisterL 
// ---------------------------------------------------------------------------
//
void CMdSServerSession::UnregisterL( const RMessage2& aMsg )
    {
    __LOG1( ELogServer, "-> Unregister %d", aMsg.Int0() );
    const TInt id = aMsg.Int0();
    iServer.Notifier().RemoveEntryL(id);
	TInt count = iNotificationCache.Count();
	for(TInt i(count - 1); i >= 0; --i)
		{
		if(iNotificationCache[i]->iId == id)
			{
			delete iNotificationCache[i]->iData;
			iNotificationCache[i]->iData = NULL;
			iNotificationCache.Remove(i);
			iNotificationCache.Compress();
			}
		}
    }

// ---------------------------------------------------------------------------
// CacheNotificationL caches a notifier event
// ---------------------------------------------------------------------------
//

CMdCSerializationBuffer* CMdSServerSession::CombineBuffersL(CMdCSerializationBuffer& aLeftBuffer,
		CMdCSerializationBuffer& aRightBuffer )
	{
	// IDs are always stored in object IDs, 
	// even if those are actually relation or event IDs	
	
	aLeftBuffer.PositionL( KNoOffset );
	aRightBuffer.PositionL( KNoOffset );

	const TMdCItemIds& leftItemIds = TMdCItemIds::GetFromBufferL( aLeftBuffer );
	const TMdCItemIds& rightItemIds = TMdCItemIds::GetFromBufferL( aRightBuffer );

	// check that namespaces match
	if ( leftItemIds.iNamespaceDefId != rightItemIds.iNamespaceDefId )
		{
		return NULL;
		}

	// create new buffer, which will contain combined results
	const TInt leftBufferSize = aLeftBuffer.Size();
	const TInt rightBufferSize = aRightBuffer.Size();
	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( 
			leftBufferSize + rightBufferSize );

	TMdCItemIds combinedItemIds;
	
	// use left buffer's data as base line
	Mem::Copy( &combinedItemIds, &leftItemIds, sizeof( TMdCItemIds ) );
	
	// and add right buffer's relation count
	combinedItemIds.iObjectIds.iPtr.iCount += rightItemIds.iObjectIds.iPtr.iCount;

	combinedItemIds.SerializeL( *buffer );

	// move left and right buffer to begin of relations
	aLeftBuffer.PositionL( leftItemIds.iObjectIds.iPtr.iOffset );
	aRightBuffer.PositionL( rightItemIds.iObjectIds.iPtr.iOffset );

	// copy IDs from left and right buffers to combined buffer
	for (TInt i = 0; i < leftItemIds.iObjectIds.iPtr.iCount; ++i)
		{
		TItemId id;
		aLeftBuffer.ReceiveL( id );
		buffer->InsertL( id );		
		}

	for (TInt i = 0; i < rightItemIds.iObjectIds.iPtr.iCount; ++i)
		{
		TItemId id;
		aRightBuffer.ReceiveL( id );
		buffer->InsertL( id );		
		}
	
	CleanupStack::Pop( buffer );
	return buffer;
	}

// ---------------------------------------------------------------------------
// CacheNotificationL caches a notifier event
// ---------------------------------------------------------------------------
//

CMdCSerializationBuffer* CMdSServerSession::CombineUriNotificationsBuffersL(CMdCSerializationBuffer& aLeftBuffer,
        CMdCSerializationBuffer& aRightBuffer )
    {
    // IDs are always stored in object IDs, 
    // even if those are actually relation or event IDs 
    
    aLeftBuffer.PositionL( KNoOffset );
    aRightBuffer.PositionL( KNoOffset );

    const TMdCItemIds& leftItemIds = TMdCItemIds::GetFromBufferL( aLeftBuffer );
    const TMdCItemIds& rightItemIds = TMdCItemIds::GetFromBufferL( aRightBuffer );

    // check that namespaces match
    if ( leftItemIds.iNamespaceDefId != rightItemIds.iNamespaceDefId )
        {
        return NULL;
        }

    // create new buffer, which will contain combined results
    const TInt leftBufferSize = aLeftBuffer.Size();
    const TInt rightBufferSize = aRightBuffer.Size();
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( 
            leftBufferSize + rightBufferSize );

    TMdCItemIds combinedItemIds;
    
    // use left buffer's data as base line
    Mem::Copy( &combinedItemIds, &leftItemIds, sizeof( TMdCItemIds ) );
    
    // and add right buffer's count
    combinedItemIds.iObjectIds.iPtr.iCount += rightItemIds.iObjectIds.iPtr.iCount;

    combinedItemIds.SerializeL( *buffer );

    // move left and right buffer to begin of items
    aLeftBuffer.PositionL( leftItemIds.iObjectIds.iPtr.iOffset );
    aRightBuffer.PositionL( rightItemIds.iObjectIds.iPtr.iOffset );

    // copy IDs from left and right buffers to combined buffer
    for (TInt i = 0; i < leftItemIds.iObjectIds.iPtr.iCount; ++i)
        {
        TItemId id;
        aLeftBuffer.ReceiveL( id );
        buffer->InsertL( id );      
        }

    for (TInt i = 0; i < rightItemIds.iObjectIds.iPtr.iCount; ++i)
        {
        TItemId id;
        aRightBuffer.ReceiveL( id );
        buffer->InsertL( id );      
        }
    
    //Add combined URI count
    TUint32 leftUriCount ( 0 );
    aLeftBuffer.ReceiveL( leftUriCount );
    TUint32 rightUriCount ( 0 );
    aRightBuffer.ReceiveL( rightUriCount );
    buffer->InsertL( TUint32( leftUriCount + rightUriCount) );    
   
    //Add uris
    HBufC* uri = NULL;
    for( TInt i( 0 ); i < leftUriCount; i++ )
        {        
        //Get uri
        uri = aLeftBuffer.ReceiveDes16L();
        CleanupStack::PushL( uri );
        buffer->InsertL( *uri );
        CleanupStack::Pop( uri );
        }
    
    for( TInt i( 0 ); i < rightUriCount; i++ )
        {        
        //Get uri
        uri = aRightBuffer.ReceiveDes16L();
        CleanupStack::PushL( uri );
        buffer->InsertL( *uri );
        CleanupStack::Pop( uri );
        }
    
    CleanupStack::Pop( buffer );
    return buffer;
    }

CMdCSerializationBuffer* CMdSServerSession::CombineItemBuffersL( CMdCSerializationBuffer& aLeftBuffer,
		CMdCSerializationBuffer& aRightBuffer )
	{
	// Current implementation supports only combining relations 
	// (used in remove relation item observer case)

	aLeftBuffer.PositionL( KNoOffset );
	aRightBuffer.PositionL( KNoOffset );

	const TMdCItems& leftItems = TMdCItems::GetFromBufferL( aLeftBuffer );
	const TMdCItems& rightItems = TMdCItems::GetFromBufferL( aRightBuffer );

	// check that namespaces match
	if ( leftItems.iNamespaceDefId != rightItems.iNamespaceDefId )
		{
		return NULL;
		}

	// create new buffer, which will contain combined results
	const TInt leftBufferSize = aLeftBuffer.Size();
	const TInt rightBufferSize = aRightBuffer.Size();
	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC(
			leftBufferSize + rightBufferSize );

	TMdCItems combinedItems;

	// use left buffer's data as base line
	Mem::Copy( &combinedItems, &leftItems, sizeof( TMdCItems ) );

	// and add right buffer's relation count
	combinedItems.iRelations.iPtr.iCount += rightItems.iRelations.iPtr.iCount;

	combinedItems.SerializeL( *buffer );

	// move left and right buffer to begin of relations
	aLeftBuffer.PositionL( leftItems.iRelations.iPtr.iOffset );
	aRightBuffer.PositionL( rightItems.iRelations.iPtr.iOffset );

	// copy relations from left and right buffers to combined buffer
	for ( TUint32 i = 0; i < leftItems.iRelations.iPtr.iCount; ++i )
		{
		TMdCRelation& leftRelation = CONST_CAST( TMdCRelation&, 
				TMdCRelation::GetFromBufferL( aLeftBuffer ) );
		
		leftRelation.SerializeL( *buffer );
		}

	for ( TUint32 i = 0; i < rightItems.iRelations.iPtr.iCount; ++i )
		{
		TMdCRelation& rightRelation = CONST_CAST( TMdCRelation&, 
				TMdCRelation::GetFromBufferL( aRightBuffer ) );
		
		rightRelation.SerializeL( *buffer );
		}

	CleanupStack::Pop( buffer );
	return buffer;
	}


// ---------------------------------------------------------------------------
// CacheNotificationL caches a notifier event
// ---------------------------------------------------------------------------
//
void CMdSServerSession::CacheNotificationL(TInt aId, TUint32 aCompleteCode, CMdCSerializationBuffer* aData)
	{
	CleanupStack::PushL( aData );

	const TInt notificationCount = iNotificationCache.Count();

	const TInt KMaxCachedItems = 256;

	if(notificationCount > KMaxCachedItems)
		{
		User::Leave( KErrOverflow );
		}

	if( aData )
		{
		// search for matching notification and combine new results to it
		for (TInt i = 0; i < notificationCount; ++i)
			{
			CNotificationCacheItem& notificationItem = *iNotificationCache[i];
			if ( notificationItem.iId == aId && notificationItem.iCode == aCompleteCode )
				{
				CMdCSerializationBuffer* data = NULL;
				// combine buffers
				if( notificationItem.iCode == EObjectNotifyAddWithUri ||
				    notificationItem.iCode == EObjectNotifyModifyWithUri || 
				    notificationItem.iCode == EObjectNotifyRemoveWithUri )
				    {
				    data = CombineUriNotificationsBuffersL( *notificationItem.iData, *aData );
				    }
				else if ( notificationItem.iCode != ERelationItemNotifyRemove )
					{
					data = CombineBuffersL( *notificationItem.iData, *aData );
					}
				else
					{
					data = CombineItemBuffersL( *notificationItem.iData, *aData );
					}
				// delete unecessary ones and change iData to new data
				if (data)
					{
					CleanupStack::PopAndDestroy( aData );
					
					delete notificationItem.iData;
					notificationItem.iData = data;
					return;
					}
				}
			}
		}

	// change ownership of aData to item
	CNotificationCacheItem* item = 
		new (ELeave) CNotificationCacheItem( aId, aCompleteCode, aData );
	CleanupStack::Pop( aData );
	CleanupStack::PushL( item );

	iNotificationCache.InsertInOrderAllowRepeats( item, 
	                              TLinearOrder<CNotificationCacheItem>(CMdSServerSession::ComparePropertiesCacheItem)); 

	CleanupStack::Pop( item );
	}
	
CMdSServer& CMdSServerSession::GetServer() const
	{
	return iServer;
	}

// ---------------------------------------------------------------------------
// ShutdownL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ShutdownL( const RMessage2& /*aMsg*/ )
    {
    __LOGLB( ELogServer, "-> Shutdown session" );

    for( TInt i = iFindEngines.Count() - 1; i >=0; i-- )
        {
        CMdSFindEngine* fe = iFindEngines[i];
        
        fe->Cancel( KErrNone );
        }
    iFindEngines.ResetAndDestroy();
    iFindEngines.Close();
    }

// ---------------------------------------------------------------------------
// SizeToRemoteL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::SizeToRemoteL( const RMessage2& aMsg,
    TInt aMessageSlot, TInt aSize)
    {
    __LOGLB( ELogServer, "-> Size to remote" );
    TPckgBuf<TInt> sizeBuf( aSize );
    aMsg.WriteL( aMessageSlot, sizeBuf );
    }

// ---------------------------------------------------------------------------
// ImportSchemaL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ImportSchemaL( const RMessage2& aMsg )
    {
    __LOGLB( ELogServer, "-> Import schema" );

    const TInt fileNameLength = aMsg.GetDesLength( 0 );
    if ( fileNameLength <= 0 )
    	{
    	User::Leave( KErrBadDescriptor );
    	}

    RBuf fileName;
    fileName.CreateL( fileNameLength );
    CleanupClosePushL( fileName );
    aMsg.ReadL( 0, fileName );

    TVendorId id = aMsg.VendorId();
    iServer.Maintenance().ImportSchemaL( iServer.Schema(), fileName, id.iId );
    CleanupStack::PopAndDestroy( &fileName );
    }

// ---------------------------------------------------------------------------
// ImportMetadataL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ImportMetadataL( const RMessage2& aMsg )
    {
    __LOGLB( ELogServer, "-> Import (async?) metadata" );
    
    const TInt fileNameLength = aMsg.GetDesLength( 0 );
    if ( fileNameLength <= 0 )
    	{
    	User::Leave( KErrBadDescriptor );
    	}
    
    RBuf fileName;
    fileName.CreateL( fileNameLength );
    CleanupClosePushL( fileName );
    aMsg.ReadL( 0, fileName );

    // if result is negative then result is error code
    // else result is item import fail count
    TInt result = iServer.Maintenance().ImportMetadataL( 
    		iServer.Manipulate(), iServer.Schema(), fileName );

    CleanupStack::PopAndDestroy( &fileName );

    TPckgBuf<TInt> failBuf( result );
    aMsg.WriteL( 1, failBuf );
    }
    
// ---------------------------------------------------------------------------
// ExportMetadataL
// ---------------------------------------------------------------------------
//
void CMdSServerSession::ExportMetadataL( const RMessage2& aMsg )
    {
    const TInt fileNameLength = aMsg.GetDesLength( 0 );
    if ( fileNameLength <= 0 )
    	{
    	User::Leave( KErrBadDescriptor );
    	}

    RBuf fileName;
    fileName.CreateL( fileNameLength );
    CleanupClosePushL( fileName );
    aMsg.ReadL( 0, fileName );

    CMdCSerializationBuffer* items = CMdCSerializationBuffer::NewLC( aMsg, 1 );

    iServer.Maintenance().ExportMetadataL( iServer.Schema(), fileName, *items );

    CleanupStack::PopAndDestroy( 2, &fileName ); // items, fileName
    }

void CMdSServerSession::AddMemoryCardL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
	aMessage.ReadL( 0, mediaIdPckg );

	iServer.Manipulate().AddMemoryCardL( mediaId );
	}

void CMdSServerSession::GetMemoryCardL(const RMessage2& aMessage)
	{
	TUint32 mediaId;

	iServer.Manipulate().GetMemoryCardL( mediaId );

    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.WriteL( 0, mediaIdPckg );
	}

void CMdSServerSession::CheckMemoryCardL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
	aMessage.ReadL( 0, mediaIdPckg );

	TBool result = iServer.Manipulate().CheckMemoryCardL( mediaId );

    TPckg<TBool> resultPckg( result );
    aMessage.WriteL( 1, resultPckg );
	}

void CMdSServerSession::SetMediaL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 0, mediaIdPckg );
    TChar drive;
    TPckg<TChar> drivePckg( drive );
    aMessage.ReadL( 1, drivePckg );
    TBool presentState;
    TPckg<TBool> presentStatePckg( presentState );
    aMessage.ReadL( 2, presentStatePckg );

	iServer.Manipulate().SetMediaL( mediaId, drive, presentState );
	}

void CMdSServerSession::GetMediaL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 0, mediaIdPckg );

    TChar drive;
    TBool presentState;

    const TBool exists = iServer.Manipulate().GetMediaL( mediaId, drive, 
    		presentState );

    if( exists )
    	{
    	TPckg<TChar> drivePckg( drive );
    	aMessage.WriteL( 1, drivePckg );
    	TPckg<TBool> presentStatePckg( presentState );
    	aMessage.WriteL( 2, presentStatePckg );
    	}

    TPckg<TBool> existsPckg( exists );
    aMessage.WriteL( 3, existsPckg );
    }

void CMdSServerSession::GetPresentMediasL(const RMessage2& aMessage)
	{
	// buffer size for media info for every possible drive
	const TInt32 KMediaInfoSize = sizeof( TMdEMediaInfo ) * KMaxDrives;

	RBuf8 mediaInfoBuffer;
	mediaInfoBuffer.CreateL( KMediaInfoSize );
	CleanupClosePushL( mediaInfoBuffer );

	const TInt32 mediaCount = iServer.Manipulate().GetPresentMediasL( 
			mediaInfoBuffer );

	TPckgC<TInt32> mediaCountPckg( mediaCount );
	aMessage.WriteL( 0, mediaCountPckg );

	if( mediaCount > 0)
		{
		const TInt32 mediaInfosLength = aMessage.GetDesMaxLength( 1 );
		if ( mediaInfosLength < KMediaInfoSize )
			{
			User::Leave( KErrBadDescriptor );
			}
		
		aMessage.WriteL( 1, mediaInfoBuffer );
		}

	CleanupStack::PopAndDestroy(); //mediaInfoBuffer
	}

void CMdSServerSession::SetFilesToPresentL(const RMessage2& aMessage)
	{
	TMdSMediaIdAndCount mediaIdAndCount;
	TPckg<TMdSMediaIdAndCount> mediaIdAndCountPckg( mediaIdAndCount );
	aMessage.Read( 0, mediaIdAndCountPckg );
	
	CMdCSerializationBuffer* uris = CMdCSerializationBuffer::NewLC( aMessage, 1 );
	CMdCSerializationBuffer* fileInfos = CMdCSerializationBuffer::NewLC( aMessage, 2 );

	TInt resultsSize = aMessage.GetDesMaxLengthL( 3 );
	if( resultsSize < ( mediaIdAndCount.iCount * CMdCSerializationBuffer::KRequiredSizeForTUint8 ) )
		{
		User::Leave( KErrBadDescriptor );
		}
	CMdCSerializationBuffer* results = CMdCSerializationBuffer::NewLC( resultsSize );

	iServer.Manipulate().SetFilesToPresentL( 
			mediaIdAndCount.iMediaId, mediaIdAndCount.iCount, *uris, *fileInfos, 
			*results );

	aMessage.WriteL( 3, results->Buffer() );

	CleanupStack::PopAndDestroy( results );
	CleanupStack::PopAndDestroy( fileInfos );
	CleanupStack::PopAndDestroy( uris );
	}


void CMdSServerSession::SetFilesToNotPresentL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
	aMessage.Read( 0, mediaIdPckg );
	
	TBool startUp;
    TPckg<TBool> startUpPckg( startUp );
	aMessage.Read( 1, startUpPckg );
	
	// only accept correct media IDs, other are silently ignored
	if( mediaId != 0 )
		{
		iServer.Manipulate().SetFilesToNotPresentL( mediaId, startUp );		
		}
	}

void CMdSServerSession::RemoveFilesNotPresentL(const RMessage2& aMessage)
	{
	TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
	aMessage.Read( 0, mediaIdPckg );
	
	TBool startUp;
    TPckg<TBool> startUpPckg( startUp );
	aMessage.Read( 1, startUpPckg );

	iServer.Manipulate().RemoveFilesNotPresentL( mediaId, startUp );
	}

void CMdSServerSession::GetSchemaVersionL(const RMessage2& aMessage)
	{
	TInt majorVersion, minorVersion;

	iServer.Manipulate().GetSchemaVersionL( majorVersion, minorVersion );

	TPckg<TInt> pckgMajorVersion( majorVersion );
	TPckg<TInt> pckgMinorVersion( minorVersion );
	aMessage.WriteL( 0, pckgMajorVersion );
	aMessage.WriteL( 1, pckgMinorVersion );
	}

void CMdSServerSession::SetObjectToPresentByGuidL(const RMessage2& aMessage)
	{
	TInt64 guidHigh;
    TPckg<TInt64> guidHighPckg( guidHigh );
	aMessage.ReadL( ESetObjectToPresentByGuidArgGuidHigh, guidHighPckg );
	
	TInt64 guidLow;
    TPckg<TInt64> guidLowPckg( guidLow );
	aMessage.ReadL( ESetObjectToPresentByGuidArgGuidLow, guidLowPckg );
	
	iServer.Manipulate().SetObjectToPresentByGuidL( guidHigh, guidLow );
	}

void CMdSServerSession::ChangePathL(const RMessage2& aMessage)
	{
	const TInt oldPathLength = aMessage.GetDesLength( 0 );
	if ( oldPathLength <= 0 )
		{
		User::Leave( KErrBadDescriptor );
		}

	const TInt newPathLength = aMessage.GetDesLength( 1 );
	if ( newPathLength <= 0 )
		{
		User::Leave( KErrBadDescriptor );
		}

	RBuf oldPath;
	oldPath.CreateL( oldPathLength );
    CleanupClosePushL( oldPath );
    aMessage.ReadL( 0, oldPath );

    RBuf newPath;
    newPath.CreateL( newPathLength );
    CleanupClosePushL( newPath );
    aMessage.ReadL( 1, newPath );
    
    iServer.Manipulate().ChangePathL( oldPath, newPath );
    
    CleanupStack::PopAndDestroy(&newPath);
    CleanupStack::PopAndDestroy(&oldPath);
	}

void CMdSServerSession::ChangeMediaIdL( const RMessage2& /*aMessage*/ )
	{
	iServer.Manipulate().ChangeMediaIdL();
	}

void CMdSServerSession::CheckMassStorageMediaIdL( const RMessage2& aMessage )
    {
    TUint32 mediaId;
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 0, mediaIdPckg );

    iServer.Manipulate().CheckMassStorageMediaIdL( mediaId );
    }

void CMdSServerSession::SetPendingL(const RMessage2& aMessage)
	{
	const TInt serializedObjectIdsLength = aMessage.GetDesLength( 0 );
	
	if ( serializedObjectIdsLength < 0 || serializedObjectIdsLength % sizeof( TItemId ) )
		{
		User::Leave( KErrBadDescriptor );
		}

	if( serializedObjectIdsLength > 0 )
		{
		RArray<TItemId> objectIds;
		CleanupClosePushL( objectIds );
	
		HBufC8* serializedObjectIds = HBufC8::NewLC( serializedObjectIdsLength );
	
		TPtr8 ptr = serializedObjectIds->Des();
		aMessage.ReadL( 0, ptr );
	
		DeserializeArrayL( *serializedObjectIds, objectIds );
	
		iServer.Manipulate().SetPendingL( objectIds );
		
		CleanupStack::PopAndDestroy( serializedObjectIds );
	
		CleanupStack::PopAndDestroy( &objectIds );
		}
	}

void CMdSServerSession::ResetPendingL(const RMessage2& aMessage)
	{
	const TInt serializedObjectIdsLength = aMessage.GetDesLength( 0 );
	
	if ( serializedObjectIdsLength < 0 || serializedObjectIdsLength % sizeof( TItemId ) )
		{
		User::Leave( KErrBadDescriptor );
		}
	
	if( serializedObjectIdsLength > 0 )
		{
		RArray<TItemId> objectIds;
		CleanupClosePushL( objectIds );
		
		HBufC8* serializedObjectIds = HBufC8::NewLC( serializedObjectIdsLength );
		
		TPtr8 ptr = serializedObjectIds->Des();
		aMessage.ReadL( 0, ptr );

		DeserializeArrayL( *serializedObjectIds, objectIds );
		
		iServer.Manipulate().ResetPendingL( objectIds );
		
		CleanupStack::PopAndDestroy( serializedObjectIds );
		
		CleanupStack::PopAndDestroy( &objectIds );
		}
	}

TInt CMdSServerSession::GetPendingCountL(const RMessage2& aMessage)
	{
	TDefId objectDefId;
	TPckg<TDefId> objectDefIdPckg( objectDefId );
	aMessage.ReadL( 0, objectDefIdPckg );
	
	TInt result = iServer.Manipulate().GetPendingCountL( objectDefId );

	TPckg<TInt> objectIdCountPckg( result );
	aMessage.WriteL( 1, objectIdCountPckg );
	
	return KErrNone;
	}

TInt CMdSServerSession::GetPendingL(const RMessage2& aMessage)
	{
	TDefId objectDefId;
	TPckg<TDefId> objectDefIdPckg( objectDefId );
	aMessage.ReadL( 0, objectDefIdPckg );

	const TInt serializedObjectIdsLength = aMessage.GetDesLength( 2 );

	if ( serializedObjectIdsLength < sizeof(TItemId) || 
		 serializedObjectIdsLength % sizeof(TItemId) )
		{
		User::Leave( KErrBadDescriptor );
		}

	RArray<TItemId> objectIds;
	CleanupClosePushL( objectIds );

	TInt bufferSize = serializedObjectIdsLength / sizeof(TItemId);

	TInt result = iServer.Manipulate().GetPendingL( objectDefId, bufferSize, objectIds );

	const TInt objectIdCount = objectIds.Count();

	TPckg<TInt> resultPckg( result );
	aMessage.WriteL( 3, resultPckg );
	
	TPckg<TInt> objectIdCountPckg( objectIdCount );
	aMessage.WriteL( 1, objectIdCountPckg );

	if( objectIdCount > 0 )
		{
		HBufC8* serializedObjectIds = SerializeArrayL( objectIds );
		CleanupStack::PushL( serializedObjectIds );

		aMessage.WriteL( 2, serializedObjectIds->Des() );

		CleanupStack::PopAndDestroy( serializedObjectIds );
		}

	CleanupStack::PopAndDestroy( &objectIds );

	return KErrNone;
	}

TInt CMdSServerSession::ComparePropertiesCacheItem(const CNotificationCacheItem& aFirst, const CNotificationCacheItem& aSecond)
    {
    return aFirst.iId - aSecond.iId;
    }

