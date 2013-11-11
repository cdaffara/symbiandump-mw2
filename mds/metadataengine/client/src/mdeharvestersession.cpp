/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata engine Harverster Server client session
*
*/

#include "mdeharvestersession.h"
#include "mdesessionimpl.h"
#include "mdcserializationbuffer.h"
#include "harvesterlog.h"

EXPORT_C CMdEHarvesterSession* CMdEHarvesterSession::NewL(CMdESession& aSession)
	{
	CMdEHarvesterSession* self = CMdEHarvesterSession::NewLC( aSession );
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CMdEHarvesterSession* CMdEHarvesterSession::NewLC(CMdESession& aSession)
	{
	CMdEHarvesterSession* self = new (ELeave) CMdEHarvesterSession( aSession );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMdEHarvesterSession::CMdEHarvesterSession(CMdESession& aSession) : 
	iSession( STATIC_CAST( CMdESessionImpl*, &aSession ) ),
	iHarvestingPrioritizationSerializationBuffer( NULL )
	{
	}

CMdEHarvesterSession::~CMdEHarvesterSession()
	{
	iHarvestingPrioritizationChunk.Close();
	delete iHarvestingPrioritizationSerializationBuffer;
	iHarvestingPrioritizationSerializationBuffer = NULL;
	}

void CMdEHarvesterSession::ConstructL()
	{
	}

EXPORT_C void CMdEHarvesterSession::SetSession(CMdESession& aSession)
{
	iSession = STATIC_CAST( CMdESessionImpl*, &aSession );
}

EXPORT_C CMdESession& CMdEHarvesterSession::SessionRef()
	{
	return *iSession;
	}

EXPORT_C CMdESession* CMdEHarvesterSession::Session()
	{
	return iSession;
	}

EXPORT_C void CMdEHarvesterSession::AddMemoryCard(TUint32 aMediaId)
	{
	iSession->EngineSession().DoAddMemoryCard( aMediaId );
	}

EXPORT_C TBool CMdEHarvesterSession::GetMemoryCard(TUint32& aMediaId)
	{
	return iSession->EngineSession().DoGetMemoryCard( aMediaId );
	}

EXPORT_C TBool CMdEHarvesterSession::CheckMemoryCard(TUint32 aMediaId, 
		TBool& aExist)
	{
	return iSession->EngineSession().DoCheckMemoryCard( aMediaId, aExist );
	}

EXPORT_C void CMdEHarvesterSession::SetMediaL(TUint32 aMediaId, TChar aDrive, 
		TBool aPresentState)
	{
	iSession->EngineSession().DoSetMediaL( aMediaId, aDrive, aPresentState );
	}

EXPORT_C TBool CMdEHarvesterSession::GetMediaL(TUint32 aMediaId, TChar& aDrive, 
		TBool& aPresentState)
	{
	return iSession->EngineSession().DoGetMediaL( aMediaId, aDrive, 
			aPresentState );
	}

EXPORT_C void CMdEHarvesterSession::GetPresentMediasL(
		RArray<TMdEMediaInfo>& aMedias)
	{
	iSession->EngineSession().DoGetPresentMediasL( aMedias );
	}

EXPORT_C TBool CMdEHarvesterSession::SetFileToPresent(TUint32 aMediaId, 
		const TDesC& aUri, TMdSFileInfo& aFileInfo)
	{
	HBufC* uri = NULL;
	uri = HBufC::New( aUri.Length() );
	if( !uri )
		{
		return EFalse;
		}

	// Note: CopyLC doesn't push anything to cleanup stack
	TRAPD(err, uri->Des().CopyLC( aUri ));

	if(err != KErrNone)
	    {
		delete uri;
		uri = NULL;
		return EFalse;
	    }
		
	const TBool ret = iSession->EngineSession().DoSetFileToPresent( 
	aMediaId, *uri, aFileInfo );
	
	delete uri;
	uri = NULL;

	return ret;
	}

EXPORT_C void CMdEHarvesterSession::SetFilesToPresentL(TUint32 aMediaId, 
		const RArray<TPtrC>& aUris, const RArray<TMdSFileInfo>& aFileInfos, 
		RArray<TFilePresentStates>& aResults)
	{
	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- Start" );
	if( aUris.Count() != aFileInfos.Count() )
		{
#ifdef _DEBUG
		WRITELOG2( "CMdEHarvesterSession::SetFilesToPresentL -- Leave (%d, %d)", 
				aUris.Count(), aFileInfos.Count() );
#endif
		User::Leave( KErrArgument );
		}

	const TInt fileCount = aUris.Count();
	
	TInt urisSize = CMdCSerializationBuffer::KRequiredSizeForTUint32;

	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- Loop urisSize" );
	for( TInt i = fileCount - 1; i >=0; i-- )
		{
		urisSize += CMdCSerializationBuffer::RequiredSize( aUris[i] );
		}

	const TInt fileInfosSize = ( CMdCSerializationBuffer::KRequiredSizeForTUint32 +  
					CMdCSerializationBuffer::KRequiredSizeForTInt64 ) * fileCount;
	const TInt resultsSize = CMdCSerializationBuffer::KRequiredSizeForTUint8 * fileCount;

	CMdCSerializationBuffer* uriBuffer = CMdCSerializationBuffer::NewLC( urisSize );
	CMdCSerializationBuffer* fileInfosBuffer = CMdCSerializationBuffer::NewLC( fileInfosSize );
	CMdCSerializationBuffer* resultsBuffer = CMdCSerializationBuffer::NewLC( resultsSize );

	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- Loop fileinfos" );
	for( TInt i = 0; i < fileCount; i++ )
		{
		const TDesC& uri = aUris[i];
		uriBuffer->InsertL( CONST_CAST(TDesC&, uri) );

		fileInfosBuffer->InsertL( aFileInfos[i].iModifiedTime );
		fileInfosBuffer->InsertL( aFileInfos[i].iSize );
		}
	
	TMdSMediaIdAndCount mediaIdAndCount = {aMediaId, (TUint32)fileCount};
	
	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- DoSetFilesToPresent start" );
	TInt error = iSession->EngineSession().DoSetFilesToPresent( mediaIdAndCount, 
			*uriBuffer->BufferConstPtr(), *fileInfosBuffer->BufferConstPtr(), 
			*resultsBuffer->BufferPtr() );
	WRITELOG1( "CMdEHarvesterSession::SetFilesToPresentL -- DoSetFilesToPresent end(%d)", error );
	
	User::LeaveIfError( error );

	resultsBuffer->PositionL( KNoOffset );

	aResults.ReserveL( fileCount );

	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- loop results" );
	for( TInt i = 0; i < fileCount; i++ )
		{
		TUint8 result;
		resultsBuffer->ReceiveL( result );
		aResults.Append( (TFilePresentStates)result );
		}

	WRITELOG( "CMdEHarvesterSession::SetFilesToPresentL -- End" );
	CleanupStack::PopAndDestroy( resultsBuffer );
	CleanupStack::PopAndDestroy( fileInfosBuffer );
	CleanupStack::PopAndDestroy( uriBuffer );
	}

EXPORT_C void CMdEHarvesterSession::SetFilesToNotPresent(TUint32 aMediaId)
	{
	// only accept correct media IDs, other are silently ignored
	if( aMediaId != 0 )
		{
		iSession->EngineSession().DoSetFilesToNotPresent( aMediaId, EFalse );
		}
	}

EXPORT_C void CMdEHarvesterSession::RemoveFilesNotPresent(TUint32 aMediaId)
	{
	iSession->EngineSession().DoRemoveFilesNotPresent( aMediaId, EFalse );
	}

EXPORT_C void CMdEHarvesterSession::SetFilesToNotPresent(TUint32 aMediaId, 
		TBool aStartUp)
	{
	iSession->EngineSession().DoSetFilesToNotPresent( aMediaId, aStartUp );
	}

EXPORT_C void CMdEHarvesterSession::RemoveFilesNotPresent(TUint32 aMediaId, 
		TBool aStartUp)
	{
	iSession->EngineSession().DoRemoveFilesNotPresent( aMediaId, aStartUp );
	}

EXPORT_C void CMdEHarvesterSession::ResetDBL()
	{
	#ifdef _DEBUG
	iSession->EngineSession().DoResetDBL();
	iSession->LoadSchemaL();
	#else
	User::Leave( KErrNotSupported );
	#endif
	}

EXPORT_C void CMdEHarvesterSession::SetHarvestingPrioritizationChunkL( 
		TInt aSize )
	{
	const TInt error = 
		iHarvestingPrioritizationChunk.CreateGlobal( KNullDesC, aSize, aSize );

	User::LeaveIfError( error );

	iSession->EngineSession().DoSetHarvestingPrioritizationChunkL( 
		iHarvestingPrioritizationChunk );

	iHarvestingPrioritizationSerializationBuffer = 
		CMdCSerializationBuffer::NewL( 
			iHarvestingPrioritizationChunk.Base(), 
			iHarvestingPrioritizationChunk.Size() );
	
	iHarvestingPrioritizationSerializationBuffer->PositionL( 0 );
	const TInt32 initialUriCount = 0;
	iHarvestingPrioritizationSerializationBuffer->InsertL( initialUriCount );
	}

EXPORT_C void CMdEHarvesterSession::AddHarvestingPrioritizationObserver( 
		TRequestStatus& aStatus )
	{
	iSession->EngineSession().DoAddHarvestingPrioritizationObserver( aStatus );
	}

EXPORT_C TInt CMdEHarvesterSession::CancelHarvestingPrioritizationObserver()
	{
	return iSession->EngineSession().DoCancelHarvestingPrioritizationObserver();
	}

EXPORT_C TInt CMdEHarvesterSession::HarvestingPrioritizationUriCountL()
	{
	if( !iHarvestingPrioritizationSerializationBuffer )
		{
		User::Leave( KErrNotFound );
		}

	iHarvestingPrioritizationSerializationBuffer->PositionL( 0 );

	TInt32 count = 0;
	iHarvestingPrioritizationSerializationBuffer->ReceiveL( count );

	return count;
	}

EXPORT_C HBufC* CMdEHarvesterSession::HarvestingPrioritizationUriL( 
		TInt aIndex )
	{
	if( !iHarvestingPrioritizationSerializationBuffer )
		{
		User::Leave( KErrNotFound );
		}

	const TUint32 offsetPosition = 
		CMdCSerializationBuffer::KRequiredSizeForTInt32 + 
		CMdCSerializationBuffer::KRequiredSizeForTUint32 * aIndex;

	iHarvestingPrioritizationSerializationBuffer->PositionL( offsetPosition );

	TUint32 uriOffset = 0;
	iHarvestingPrioritizationSerializationBuffer->ReceiveL( uriOffset );

	iHarvestingPrioritizationSerializationBuffer->PositionL( uriOffset );

	return iHarvestingPrioritizationSerializationBuffer->ReceiveDes16L();
	}

EXPORT_C void CMdEHarvesterSession::AutoLockL( 
		RPointerArray<CMdEObject>& aObjects )
	{
	for( TInt i = aObjects.Count() - 1; i >=0; i-- )
		{
		aObjects[i]->AutoLockL();
		}
	}

EXPORT_C void CMdEHarvesterSession::ChangePath( 
		const TDesC& aOldPath, const TDesC& aNewPath, TRequestStatus& aStatus )
	{
	iSession->EngineSession().DoChangePath( aOldPath, aNewPath, aStatus );
	}

EXPORT_C void CMdEHarvesterSession::ChangeCDriveMediaId()
	{
	iSession->EngineSession().DoChangeCDriveMediaId();
	}

EXPORT_C void CMdEHarvesterSession::SetPendingL( 
		const RArray<TItemId>& aObjectIds )
	{
	iSession->EngineSession().DoSetPendingL( aObjectIds );
	}

EXPORT_C void CMdEHarvesterSession::ResetPendingL( 
		const RArray<TItemId>& aObjectIds )
	{
	iSession->EngineSession().DoResetPendingL( aObjectIds );
	}

EXPORT_C TInt CMdEHarvesterSession::GetPendingCount( const CMdEObjectDef* aObjectDef )
	{
	return iSession->EngineSession().DoGetPendingCount( aObjectDef );
	}
	
EXPORT_C TInt CMdEHarvesterSession::GetPending( 
		const CMdEObjectDef* aObjectDef, TInt& aObjectIdCount, 
		CMdCSerializationBuffer& aObjectIdBuffer )
	{
	return iSession->EngineSession().DoGetPending( aObjectDef, aObjectIdCount, 
			aObjectIdBuffer );
	}

EXPORT_C void CMdEHarvesterSession::CheckMassStorageMediaId( const TUint32 aMediaId  )
    {
    iSession->EngineSession().DoCheckMassStorageMediaId( aMediaId );
    }

