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
* Description:  This is Metadata engine client session file
*
*/

#include <e32std.h>
#include <f32file.h>

// INCLUDE FILES
#include "mdeenginesession.h"
#include "mdcserializationbuffer.h"
#include "mdequerycriteriaserialization.h"
#include "mdesessionimpl.h"
#include "mdsutils.h"

RMdEEngineSession::RMdEEngineSession(CMdESessionImpl& aSession) 
	: RSessionBase(), iSession( aSession )
    {
   	iIsSessionOpen = EFalse;
   	iIsSessionProcessOpen = EFalse;
    }

void RMdEEngineSession::OpenL()
    {
    TFindServer findMdEServer( KMdSServerName );
    TFullName name;
    
    TInt result = findMdEServer.Next( name );
    // Server already running
    if( result == KErrNone )
        {
        TFindProcess findProcess( KMdSServerProcessString );
        result = findProcess.Next( name );
        if ( result == KErrNone )
            {
            result = iServerProcess.Open( name );
            if( result != KErrNone )
                {
                User::Leave( KErrNotReady );
                }
            iIsSessionProcessOpen = ETrue;
            return;
            }
        else
            {
            User::Leave( KErrNotReady );
            }
        }

    User::LeaveIfError( iServerProcess.Create( KMdSServerName, 
    		KNullDesC ) );  
     
    TRequestStatus stat( 0 );
    iServerProcess.Rendezvous( stat );
	
    if( stat != KRequestPending )
    	{
        iServerProcess.Kill( 0 );   // abort startup
    	}
    else
    	{
        iServerProcess.Resume();    // logon OK - start the server   
    	}	

    User::WaitForRequest( stat ); 
    
    iIsSessionProcessOpen = ETrue;
    }

void RMdEEngineSession::OpenCancel()
	{
	if( iIsSessionProcessOpen )
		{		
		iServerProcess.Close();
	
		iIsSessionProcessOpen = EFalse;
		}
	}

void RMdEEngineSession::ConnectL()
    {
    if( iIsSessionProcessOpen )
    	{
	    const TBool error = (iServerProcess.ExitType() != EExitPending);

    	iServerProcess.Close();

		iIsSessionProcessOpen = EFalse;
	
		if( error )
			{
			User::LeaveIfError( KErrCommsBreak );
			}

	    const TInt result = CreateSession( 
	    	KMdSServerName,
			Version(),
	        KMetadataMessageSlots,
	        EIpcSession_Unsharable );

		User::LeaveIfError( result );
		
		iIsSessionOpen = ETrue;
    	}
    else
    	{
    	User::LeaveIfError( KErrBadHandle );
    	}
    }

void RMdEEngineSession::Shutdown()
    {
    if( iIsSessionOpen )
    	{
	    SendReceive( EShutdown );
    	}

	if( iIsSessionProcessOpen )
		{		
		iServerProcess.Close();
		}
    }

TVersion RMdEEngineSession::Version() const
    {
    return( TVersion( KMdSServMajorVersionNumber,
                      KMdSServMinorVersionNumber,
                      KMdSServBuildVersionNumber ) );
    }

void RMdEEngineSession::DoAddEventDefL(const TDefId aNameSpaceId, 
		const TDesC &aName )
	{
	TIpcArgs ipcArgs;
	ipcArgs.Set( EAddDefArgNamespaceId, aNameSpaceId );
	ipcArgs.Set( EAddDefArgDefName,     &aName );
    const TInt ret = SendReceive( EAddEventDef, ipcArgs );
    
    NotifySessionError( ret );
    
    User::LeaveIfError( ret );
	}

void RMdEEngineSession::DoAddRelationDefL(const TDefId aNameSpaceId, 
		const TDesC &aName )
	{
	TIpcArgs ipcArgs;
	ipcArgs.Set( EAddDefArgNamespaceId, aNameSpaceId );
	ipcArgs.Set( EAddDefArgDefName,     &aName );
    const TInt ret = SendReceive( EAddRelationDef, ipcArgs );
    
    NotifySessionError( ret );
    
    User::LeaveIfError( ret );
	}

void RMdEEngineSession::DoAddItemsL( 
		const CMdCSerializationBuffer& aSerializedBuffer, 
		CMdCSerializationBuffer& aResultBuffer )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aSerializedBuffer.BufferConstPtr() );
    ipcArgs.Set( 1, aResultBuffer.BufferPtr() );
    const TInt ret = SendReceive( EAdd, ipcArgs );
    
    NotifySessionError( ret );
    
    User::LeaveIfError( ret );
    }

void RMdEEngineSession::DoAddItemsAsync( 
		const CMdCSerializationBuffer& aSerializedBuffer, 
		CMdCSerializationBuffer& aResultBuffer, TRequestStatus& aStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aSerializedBuffer.BufferConstPtr() );
    ipcArgs.Set( 1, aResultBuffer.BufferPtr() );
	SendReceive( EAdd, ipcArgs, aStatus );
    }

void RMdEEngineSession::DoUpdateItemsL( 
		const CMdCSerializationBuffer& aSerializedBuffer, 
		CMdCSerializationBuffer& aResultBuffer )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aSerializedBuffer.BufferConstPtr() );
    ipcArgs.Set( 1, aResultBuffer.BufferPtr() );
    const TInt ret = SendReceive( EUpdate, ipcArgs );
    
    NotifySessionError( ret );
    
    User::LeaveIfError( ret );
    }

void RMdEEngineSession::DoUpdateItemsAsync( 
		const CMdCSerializationBuffer& aSerializedBuffer, 
		CMdCSerializationBuffer& aResultBuffer, TRequestStatus& aStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aSerializedBuffer.BufferConstPtr() );
    ipcArgs.Set( 1, aResultBuffer.BufferPtr() );
	SendReceive( EUpdate, ipcArgs, aStatus );
    }
    
CMdCSerializationBuffer* RMdEEngineSession::DoFindSyncLC( 
	const CMdEQuery* aQuery, 
    const CMdEQueryCriteriaSerialization& aSerializationBuffer, 
    TMdCQueryLockType aLocktype, TUint32 aNotifyCount)
	{
    TIpcArgs ipcArgs;
    TPckgBuf<TInt> size(0);

    ipcArgs.Set( EFindArgQueryId,         (TUint32)aQuery->GetQueryId() );
    ipcArgs.Set( EFindArgConditions,      aSerializationBuffer.BufferPtr() );
    ipcArgs.Set( EFindArgResulBufferSize, &size );
    ipcArgs.Set( EFindArgNotifyCount,     aNotifyCount );	

    const TInt err = SendReceive( EFind, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );

    CMdCSerializationBuffer* resultBuf = 
    	CMdCSerializationBuffer::NewLC( size() );

    DoGetDataL( aQuery, resultBuf, EFind, aLocktype );

    return resultBuf;	
	}

void RMdEEngineSession::DoFindAsync( TUint32 aQueryId, 
		const CMdEQueryCriteriaSerialization& aSerializationBuffer,
		TPckgBuf<TInt>* aSizeBuf, TRequestStatus& aAsyncStatus, 
		TUint32 aNotifyCount )
    {
    TIpcArgs ipcArgs;

    ipcArgs.Set( EFindArgQueryId,         (TUint32)aQueryId );
    ipcArgs.Set( EFindArgConditions,      aSerializationBuffer.BufferPtr() );
    ipcArgs.Set( EFindArgResulBufferSize, aSizeBuf );
    ipcArgs.Set( EFindArgNotifyCount,     aNotifyCount );

    SendReceive( EAsyncFind, ipcArgs, aAsyncStatus );
    }

void RMdEEngineSession::DoFindContinue( 
	const CMdEQuery* aQuery, 
    TPckgBuf<TInt>* aSizeBuf,
    TRequestStatus& aAsyncStatus )
    {
    TIpcArgs ipcArgs;

    ipcArgs.Set( EFindArgQueryId,         (TUint32)aQuery->GetQueryId() );
    ipcArgs.Set( EFindArgResulBufferSize, aSizeBuf );

    SendReceive( EContinueAsyncFind, ipcArgs, aAsyncStatus );
    }

void RMdEEngineSession::DoFindCancelL( const CMdEQuery* aQuery )
    {
    // silently skip if session is not open
    if( iIsSessionOpen )
    	{
    	TIpcArgs ipcArgs;
    	ipcArgs.Set( EFindArgQueryId, (TUint32)aQuery->GetQueryId() );

	    const TInt err = SendReceive( ECancelFind, ipcArgs );
	
	    NotifySessionError( err );
	    
	    User::LeaveIfError( err );
    	}
    }
    
void RMdEEngineSession::DoCheckObjectL( 
		CMdCSerializationBuffer& aObjectBuffer, const TDesC& aUri, 
		TDefId aNamespaceDefId )
	{
	TIpcArgs ipcArgs;

	TPckgBuf<TDefId> namespaceDefId( aNamespaceDefId );

	ipcArgs.Set( ECheckObjectArgType,           ECheckObjectByUri );
	ipcArgs.Set( ECheckObjectArgTypeValue,      &aUri );
	ipcArgs.Set( ECheckObjectArgNamespaceDefId, &namespaceDefId );
	ipcArgs.Set( ECheckObjectArgObject,         aObjectBuffer.BufferPtr() );

	TInt err = SendReceive( ECheckObject, ipcArgs );
	
	NotifySessionError( err );
	
	User::LeaveIfError( err );
	}

void RMdEEngineSession::DoCheckObjectL( 
		CMdCSerializationBuffer& aObjectBuffer, TItemId aId, 
		TDefId aNamespaceDefId )
	{
	TIpcArgs ipcArgs;
	TPckgBuf<TDefId> namespaceDefId( aNamespaceDefId );
	TPckgBuf<TItemId> objectId( aId );

	ipcArgs.Set( ECheckObjectArgType,           ECheckObjectById );
	ipcArgs.Set( ECheckObjectArgTypeValue,      &objectId );
	ipcArgs.Set( ECheckObjectArgNamespaceDefId, &namespaceDefId );
	ipcArgs.Set( ECheckObjectArgObject,         aObjectBuffer.BufferPtr() );

	TInt err = SendReceive( ECheckObject, ipcArgs );
	
	NotifySessionError( err );
	
	User::LeaveIfError( err );
	}

void RMdEEngineSession::DoCheckObjectL( 
		CMdCSerializationBuffer& aObjectsBuffer, 
		const CMdCSerializationBuffer& aObjectIdsBuffer, 
		TDefId aNamespaceDefId )
	{
	TIpcArgs ipcArgs;
	TPckgBuf<TDefId> namespaceDefId( aNamespaceDefId );

	ipcArgs.Set( ECheckObjectArgType,           ECheckObjectByIds );
	ipcArgs.Set( ECheckObjectArgTypeValue,      aObjectIdsBuffer.BufferPtr() );
	ipcArgs.Set( ECheckObjectArgNamespaceDefId, &namespaceDefId );
	ipcArgs.Set( ECheckObjectArgObject,         aObjectsBuffer.BufferPtr() );

	TInt err = SendReceive( ECheckObject, ipcArgs );
	
	NotifySessionError( err );
	
	User::LeaveIfError( err );
	}

void RMdEEngineSession::DoCommitObjectsL( const CMdCSerializationBuffer& aCodedObjects )
	{
	TIpcArgs ipcArgs;
	ipcArgs.Set( 0, aCodedObjects.BufferConstPtr() );

	TInt err = SendReceive( EUpdate, ipcArgs );
	
	NotifySessionError( err );
	
	User::LeaveIfError( err );
	}

void RMdEEngineSession::DoCancelObjectL( CMdCSerializationBuffer& aRemoveId )
    {
    // silently skip if session is not open
    if( iIsSessionOpen )
    	{
	    TIpcArgs ipcArgs;
	    ipcArgs.Set( 0, aRemoveId.BufferPtr() );
	
	   	TInt err = SendReceive( ECancelObject, ipcArgs );
	   	
	   	NotifySessionError( err );
	   	
	   	User::LeaveIfError( err );
    	}
    }

void RMdEEngineSession::DoRegisterL( TInt aId,
    TUint32 aType,
    CMdCSerializationBuffer& aBuffer,
    TDefId aNamespaceDefId )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aId );
    ipcArgs.Set( 1, aType );
    ipcArgs.Set( 2, aBuffer.BufferPtr() );
    ipcArgs.Set( 3, aNamespaceDefId );

    const TInt err = SendReceive( ERegister, ipcArgs );

    NotifySessionError( err );

    User::LeaveIfError( err );
    }

void RMdEEngineSession::DoListen( TInt aId,
    TPckgBuf<TInt>* aSizeBuf,
    TRequestStatus& aAsyncStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aId );
    ipcArgs.Set( 1, aSizeBuf );

    SendReceive( EListen, ipcArgs, aAsyncStatus );
    }

void RMdEEngineSession::DoUnregisterL( TInt aId )
    {
    // silently skip if session is not open
    if( iIsSessionOpen )
    	{
	    TIpcArgs ipcArgs;
	    ipcArgs.Set( 0, aId );
	
	    const TInt err = SendReceive( EUnregister, ipcArgs );
	
	    NotifySessionError( err );
	
	    User::LeaveIfError( err );
    	}
    }

void RMdEEngineSession::DoRemoveItemsL( const CMdCSerializationBuffer& aRemoveId, CMdCSerializationBuffer& aSuccessfulId )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aRemoveId.BufferConstPtr() );
    ipcArgs.Set( 1, aSuccessfulId.BufferPtr() );

   	TInt err = SendReceive( ERemove, ipcArgs );
   	
   	NotifySessionError( err );
   	
   	User::LeaveIfError( err );
    }

void RMdEEngineSession::DoRemoveItemsAsync( const CMdCSerializationBuffer& aRemoveId, CMdCSerializationBuffer& aSuccessfulId, TRequestStatus& aStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aRemoveId.BufferPtr() );
    ipcArgs.Set( 1, aSuccessfulId.BufferPtr() );

   	SendReceive( ERemove, ipcArgs, aStatus );
    }

void RMdEEngineSession::DoLoadSchemaL( TInt& aHandle )
    {
    TPckgBuf<TInt> handleBuf;

    TIpcArgs ipcArgs;
    ipcArgs.Set( 1, &handleBuf );
    ipcArgs.Set( 2, ELoadSchema );
    
    const TInt err = SendReceive( EGetData, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );

    aHandle = handleBuf();
    }

void RMdEEngineSession::DoGetDataL( const CMdEQuery* aQuery, 
	CMdCSerializationBuffer* aResultBuffer, 
	TUint32 aRelatedCommand, TUint32 aExtraData)
	{
	TIpcArgs ipcArgs;

    ipcArgs.Set( 0, (TUint32)aQuery->GetQueryId() );
	ipcArgs.Set( 1, aResultBuffer->BufferPtr() );
    ipcArgs.Set( 2, aRelatedCommand );
    ipcArgs.Set( 3, aExtraData );

    const TInt err = SendReceive( EGetData, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );
	}

void RMdEEngineSession::DoGetDataL( CMdCSerializationBuffer& aBuffer, TUint32 aExtraData )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, 0 );
    ipcArgs.Set( 1, aBuffer.BufferPtr() );
    ipcArgs.Set( 2, EListen );
    ipcArgs.Set( 3, aExtraData );
    const TInt err = SendReceive( EGetData, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );
    }

void RMdEEngineSession::DoImportSchemaL( const TDesC& aFileName )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &aFileName );

    const TInt err = SendReceive( EImportSchema, ipcArgs );

    NotifySessionError( err );
    
    User::LeaveIfError( err );
    }
    
TInt RMdEEngineSession::DoImportMetadataL( const TDesC& aFileName )
    {
    TIpcArgs ipcArgs;
    TPckgBuf<TInt> failed;
    ipcArgs.Set( 0, &aFileName );
    ipcArgs.Set( 1, &failed );

    const TInt err = SendReceive( EImportMetadata, ipcArgs );

    NotifySessionError( err );
    
    User::LeaveIfError( err );    
    return failed();
    }

void RMdEEngineSession::DoImportMetadata( const TDesC& aFileName, 
		TPckgBuf<TInt>& aResult, TRequestStatus& aStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &aFileName );
    ipcArgs.Set( 1, &aResult );

    SendReceive( EAsyncImportMetadata, ipcArgs, aStatus );
    }

void RMdEEngineSession::DoExportMetadataL( const TDesC& aFileName, 
		const CMdCSerializationBuffer& aItems )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &aFileName );
    ipcArgs.Set( 1, aItems.BufferConstPtr() );

    const TInt err = SendReceive( EExportMetadata, ipcArgs );

    NotifySessionError( err );
    
    User::LeaveIfError( err );    
    }

void RMdEEngineSession::DoExportMetadataL( const TDesC& aFileName, 
		const CMdCSerializationBuffer& aItems, TRequestStatus& aStatus )
    {
    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &aFileName );
    ipcArgs.Set( 1, aItems.BufferConstPtr() );

    SendReceive( EAsyncExportMetadata, ipcArgs, aStatus );
    }

void RMdEEngineSession::DoAddMemoryCard(TUint32 aMediaId)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );

    const TInt err = SendReceive( EAddMemoryCard, ipcArgs );

    NotifySessionError( err );
	}

TBool RMdEEngineSession::DoGetMemoryCard(TUint32& aMediaId)
	{
    TPckg<TUint32> mediaIdPckg( aMediaId );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );

    const TInt error = SendReceive( EGetMemoryCard, ipcArgs );

    NotifySessionError( error );

    if( error != KErrNone )
    	{
    	return EFalse;
    	}
    else
    	{
	    aMediaId = mediaIdPckg();
    	return ETrue;
    	}
    }

TBool RMdEEngineSession::DoCheckMemoryCard(TUint32 aMediaId, TBool& aExist)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckg<TBool> existPckg( aExist );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &existPckg );

    const TInt error = SendReceive( ECheckMemoryCard, ipcArgs );

    NotifySessionError( error );
    
    if( error != KErrNone )
    	{
    	return EFalse;
    	}
    else
    	{
	    aExist = existPckg();
    	return ETrue;
    	}
    }

void RMdEEngineSession::DoSetMediaL(TUint32 aMediaId, TChar aDrive, 
		TBool aPresentState)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckgC<TChar> drivePckg( aDrive );
    TPckgC<TBool> presentStatePckg( aPresentState );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &drivePckg );
    ipcArgs.Set( 2, &presentStatePckg );

    const TInt err = SendReceive( ESetMedia, ipcArgs );

    NotifySessionError( err );
    
    User::LeaveIfError( err );
	}

TBool RMdEEngineSession::DoGetMediaL(TUint32 aMediaId, TChar& aDrive, 
		TBool& aPresentState)
	{
	TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckg<TChar> drivePckg( aDrive );
    TPckg<TBool> presentStatePckg( aPresentState );

    TBool exists;
    TPckg<TBool> existsPckg( exists );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &drivePckg );
    ipcArgs.Set( 2, &presentStatePckg );
    ipcArgs.Set( 3, &existsPckg );

    const TInt error = SendReceive( EGetMedia, ipcArgs );

    NotifySessionError( error );

	User::LeaveIfError( error );

	aDrive = drivePckg();
	aPresentState = presentStatePckg();
	
	exists = existsPckg();
	
	return exists;
    }

void RMdEEngineSession::DoGetPresentMediasL(RArray<TMdEMediaInfo>& aMedias)
	{
	// buffer size for media info for every possible drive
	const TInt KMediaInfoSize = sizeof( TMdEMediaInfo ) * KMaxDrives;

	TInt32 mediaCount;
	TPckg<TInt32> mediaCountPckg( mediaCount );
	
	HBufC8* mediaInfoBuffer = HBufC8::NewLC( KMediaInfoSize );
	
	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaCountPckg );
    TPtr8 ptr( CONST_CAST( TUint8*, mediaInfoBuffer->Ptr() ), KMediaInfoSize );
    ipcArgs.Set( 1, &ptr );

	TInt error = SendReceive( EGetPresentMedias, ipcArgs );

    NotifySessionError( error );

	User::LeaveIfError( error );

	mediaCount = mediaCountPckg();

	aMedias.ReserveL( mediaCount );

	mediaCount *= sizeof( TMdEMediaInfo );
	
	const TUint8* mediaInfoPtr = mediaInfoBuffer->Ptr();
	
	// get media infos from buffer
	for( TInt32 i = 0; i < mediaCount; i += sizeof( TMdEMediaInfo ) )
		{
		const TMdEMediaInfo& mediaInfo = *STATIC_CAST( const TMdEMediaInfo*, 
				(const TMdEMediaInfo*)( mediaInfoPtr + i ) );
		
		aMedias.AppendL( mediaInfo );
		}
	
	CleanupStack::PopAndDestroy( mediaInfoBuffer );
	}

TBool RMdEEngineSession::DoSetFileToPresent(TUint32 aMediaId, 
		const TDesC& aUri, TMdSFileInfo& aFileInfo)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckgC<TMdSFileInfo> fileInfoPckg( aFileInfo );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &aUri );
    ipcArgs.Set( 2, &fileInfoPckg );

   	TInt error = SendReceive( ESetFileToPresent, ipcArgs );

   	NotifySessionError( error );

   	if( error != KErrNone )
    	{
    	return EFalse;
    	}
    else
    	{
    	return ETrue;
    	}
	}

TInt RMdEEngineSession::DoSetFilesToPresent(TMdSMediaIdAndCount aMediaIdAndCount, 
		const TDes8& aUris, const TDes8& aFileInfos, TDes8& aResults)
	{
    TPckgC<TMdSMediaIdAndCount> mediaIdAndCountPckg( aMediaIdAndCount );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdAndCountPckg );
    ipcArgs.Set( 1, &aUris );
    ipcArgs.Set( 2, &aFileInfos );
    ipcArgs.Set( 3, &aResults );

   	TInt error = SendReceive( ESetFilesToPresent, ipcArgs );

   	return error;
	}

void RMdEEngineSession::DoSetFilesToNotPresent(TUint32 aMediaId, TBool aStartUp)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckgC<TBool> startUpPckg( aStartUp );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &startUpPckg );

    const TInt err = SendReceive( ESetFilesToNotPresent, ipcArgs );

    NotifySessionError( err );
	}
	
void RMdEEngineSession::DoRemoveFilesNotPresent(TUint32 aMediaId, TBool aStartUp)
	{
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckgC<TBool> startUpPckg( aStartUp );
	
	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &startUpPckg );

    const TInt err = SendReceive( ERemoveFilesNotPresent, ipcArgs );
    
    NotifySessionError( err );
	}

void RMdEEngineSession::DoGetSchemaVersionL(
		TInt& aMajorVersion, TInt& aMinorVersion)
	{
    TPckg<TInt> majorVersion( aMajorVersion );
    TPckg<TInt> minorVersion( aMinorVersion );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &majorVersion );
    ipcArgs.Set( 1, &minorVersion );

    const TInt err = SendReceive( EGetSchemaVersion, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );
    }

void RMdEEngineSession::DoSetObjectToPresentByGuidL( 
		const TInt64& aGuidHigh, const TInt64& aGuidLow )
	{
    TPckgC<TInt64> guidHigh( aGuidHigh );
    TPckgC<TInt64> guidLow( aGuidLow );

	TIpcArgs ipcArgs;
    ipcArgs.Set( ESetObjectToPresentByGuidArgGuidHigh, &guidHigh );
    ipcArgs.Set( ESetObjectToPresentByGuidArgGuidLow,  &guidLow );

    const TInt err = SendReceive( ESetObjectToPresentByGuid, ipcArgs );

    NotifySessionError( err );

    User::LeaveIfError( err );
	}

void RMdEEngineSession::DoChangePath( 
		const TDesC& aOldPath, const TDesC& aNewPath, TRequestStatus& aStatus )
	{
	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &aOldPath );
    ipcArgs.Set( 1, &aNewPath );

	SendReceive( EChangePath, ipcArgs, aStatus );
	}

void RMdEEngineSession::DoChangeCDriveMediaId()
	{
	Send( EChangeMediaId );
	}

void RMdEEngineSession::DoCheckMassStorageMediaId( const TUint32 aMediaId  )
    {
    TPckgC<TUint32> mediaIdPckg( aMediaId );

    TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );

    SendReceive( EChangeMassStorageMediaId, ipcArgs );
    }

void RMdEEngineSession::DoSetPendingL( const RArray<TItemId>& aObjectIds )
	{
	HBufC8* idBuffer = SerializeArrayL( aObjectIds );
	CleanupStack::PushL( idBuffer );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, idBuffer );

    const TInt err = SendReceive( ESetPending, ipcArgs );

    NotifySessionError( err );

	CleanupStack::PopAndDestroy( idBuffer );
	}

void RMdEEngineSession::DoResetPendingL( const RArray<TItemId>& aObjectIds )
	{
	HBufC8* idBuffer = SerializeArrayL( aObjectIds );
	CleanupStack::PushL( idBuffer );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, idBuffer );

    const TInt err = SendReceive( EResetPending, ipcArgs );

    NotifySessionError( err );

	CleanupStack::PopAndDestroy( idBuffer );
	}
	
TInt RMdEEngineSession::DoGetPendingCount( const CMdEObjectDef* aObjectDef )
	{
	TDefId objectDefId = KNoDefId;
	
	if( aObjectDef )
		{
		objectDefId = aObjectDef->Id();
		}

	TPckg<TDefId> objectDefIdPckg( objectDefId );
	TPckg<TInt> idCountPckg( 0 );
	
	TIpcArgs ipcArgs;
	ipcArgs.Set( 0, &objectDefIdPckg );
	ipcArgs.Set( 1, &idCountPckg );

    SendReceive( EGetPendingCount, ipcArgs );

    return idCountPckg();
	}

TInt RMdEEngineSession::DoGetPending( 
		const CMdEObjectDef* aObjectDef, TInt& /* aObjectIdCount */, 
		CMdCSerializationBuffer& aObjectIdBuffer )
	{
	TDefId objectDefId = KNoDefId;
	
	if( aObjectDef )
		{
		objectDefId = aObjectDef->Id();
		}

	TPckg<TDefId> objectDefIdPckg( objectDefId );
	TPckg<TInt> idCountPckg( 0 );
	TPckg<TInt> resultPckg( 0 );
	TInt result( 0 );
	
	TIpcArgs ipcArgs;
	ipcArgs.Set( 0, &objectDefIdPckg );
	ipcArgs.Set( 1, &idCountPckg );
	ipcArgs.Set( 2, aObjectIdBuffer.BufferPtr() );
	ipcArgs.Set( 3, &resultPckg );
	
    SendReceive( EGetPending, ipcArgs );

    result = resultPckg();
    
    return result;
	}

void RMdEEngineSession::DoResetDBL()
	{
	TInt err = SendReceive( EResetDB );

    NotifySessionError( err );

	User::LeaveIfError( err );
	}

void RMdEEngineSession::DoSetHarvestingPrioritizationChunkL( 
		RHandleBase aHandle )
	{
	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, aHandle );

    const TInt err = SendReceive( ESetHarvestingPrioritizationChunk, ipcArgs );
    
    NotifySessionError( err );
    
    User::LeaveIfError( err );
	}

void RMdEEngineSession::DoAddHarvestingPrioritizationObserver( 
		TRequestStatus& aStatus )
	{
    SendReceive( EAddHarvestingPrioritizationObserver, aStatus );
	}

TInt RMdEEngineSession::DoCancelHarvestingPrioritizationObserver()
	{
    const TInt err = SendReceive( ECancelHarvestingPrioritizationObserver );
    
    NotifySessionError( err );
    
    return err;
	}

void RMdEEngineSession::NotifySessionError( TInt aError )
	{
	if( aError != KErrServerTerminated )
		{
		return;
		}
	
	iIsSessionOpen = EFalse;
	iSession.NotifyError( aError );
	}
