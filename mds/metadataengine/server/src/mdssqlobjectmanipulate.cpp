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
* Description:  Add, update and remove items in DB.
*                 Basic DB manipulation engine.
*
*/

#include "mdssqlobjectmanipulate.h"

#include "mdcitem.h"
#include "mdsdbconnectionpool.h"
#include "mdssqliteconnection.h"
#include "mdsfindsqlclausedef.h"
#include "mdsschema.h"
#include "mdsnamespacedef.h"
#include "mdsobjectdef.h"
#include "mdspropertydef.h"
#include "mdsclausebuffer.h"
#include "mdsobjectlocklist.h"
#include "mdsindexer.h"
#include "mdspreferences.h"
#include "mdslogger.h"
#include "mdcserializationbuffer.h"
#include "mdssqldbmaintenance.h"
#include "mdcresult.h"
#include "mdeinternalerror.h"

#include "mdsgetimeiao.h"

#include <e32math.h>
#include <mdeobject.h>

/** logging instance */
__USES_LOGGER

const TInt KMaxBuffers = 5;

_LIT( KMemoryCard, "MC" );


//------------------------------------------------------------------------------------
// CLASS      CMdSSqlObjectManipulate
//------------------------------------------------------------------------------------

CMdSIdentifierGenerator* CMdSIdentifierGenerator::NewL( )
    {
    CMdSIdentifierGenerator* self = CMdSIdentifierGenerator::NewLC( );
    CleanupStack::Pop( self );
    return self;
    }

CMdSIdentifierGenerator* CMdSIdentifierGenerator::NewLC( )
    {
    CMdSIdentifierGenerator* self = new ( ELeave ) CMdSIdentifierGenerator( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

CMdSIdentifierGenerator::~CMdSIdentifierGenerator()
    {
    delete iDigest;
    iDigest = NULL;
    }

void CMdSIdentifierGenerator::ConstructL( )
    {
    CMdsGetImeiAO * imeigetter = CMdsGetImeiAO::NewLC();
    iImei = imeigetter->GetIMEI();
    
    TTime uTime;
    uTime.UniversalTime();
    TInt64 intTime = uTime.Int64();
    TUint32 rand = Math::Rand( intTime );

    iImei ^= (TInt64)(rand & 0xFF000000) << ( sizeof( TUint32 ) * 8 );
    
    CleanupStack::PopAndDestroy(imeigetter);
    iLastTime = 0;
    
    iDigest = CMessageDigestFactory::NewDigestL( CMessageDigest::EMD5 );
    }

void CMdSIdentifierGenerator::GenerateGuid( TInt64& aGuidHigh, TInt64& aGuidLow )
	{
	if (aGuidHigh == 0 && aGuidLow == 0)
		{
		TTime currTime;
		currTime.UniversalTime();
		const TInt64 tickCount = currTime.Int64();
		if (iLastTime < tickCount)
			{
			iLastTime = tickCount;
			}
		else
			{
			++iLastTime;
			}

		//MD5 hash GUID
		const TInt64 KXorImei = I64LIT( 0x2C3E773D1A25916E );
		const TInt64 xorImei = iImei ^ KXorImei;

		const TInt KMaxQuidLength = 16;
		TBuf8<KMaxQuidLength> message;
		message.SetLength( KMaxQuidLength );
		Mem::Copy( (TAny*)( message.Ptr() ), &xorImei, sizeof( TInt64 ) );
		Mem::Copy( (TAny*)( message.Ptr() + sizeof( TInt64 ) ), &iLastTime, sizeof( TInt64 ) );

		TPtrC8 hash = iDigest->Final( message );

		__ASSERT_DEBUG( hash.Size() >= KMaxQuidLength, User::Panic( _L("CMdSIdentifierGenerator::GenerateGuid"), KErrGeneral ) );

		Mem::Copy( &aGuidHigh, hash.Ptr(), sizeof( TInt64 ) );
		Mem::Copy( &aGuidLow, hash.Ptr() + sizeof( TInt64 ), sizeof( TInt64 ) );
		//MD5 hash GUID end
		}
	}

HBufC* CMdSIdentifierGenerator::GenerateUriL( const CMdsObjectDef& aObjectDef, 
		const TInt64& aGuidHigh, const TInt64& aGuidLow ) const
	{
	_LIT( KDot, "." );
	HBufC* uri = HBufC::NewLC( aObjectDef.GetName().Length() + 2 /*two dots*/+ 
			2*KMaxUint64ValueLength );

	TPtr uriPtr( uri->Des() );
	uriPtr.Append( aObjectDef.GetName() );
	uriPtr.Append( KDot );
	uriPtr.AppendNum( aGuidHigh );
	uriPtr.Append( KDot );
	uriPtr.AppendNum( aGuidLow );
	CleanupStack::Pop( uri );
	return uri;
	}


//-----------------------------------------------------------------------------
// CLASS      CMdSSqlObjectManipulate
//-----------------------------------------------------------------------------

CMdSSqlObjectManipulate* CMdSSqlObjectManipulate::NewL( 
		const CMdsSchema& aSchema, CMdSObjectLockList& aLockList )
    {
    CMdSSqlObjectManipulate* self = CMdSSqlObjectManipulate::NewLC( aSchema, 
    		aLockList );
    CleanupStack::Pop( self );
    return self;
    }

CMdSSqlObjectManipulate* CMdSSqlObjectManipulate::NewLC( 
		const CMdsSchema& aSchema, CMdSObjectLockList& aLockList )
    {
    CMdSSqlObjectManipulate* self = new ( ELeave ) CMdSSqlObjectManipulate( 
    		aSchema, aLockList );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

CMdSSqlObjectManipulate::~CMdSSqlObjectManipulate()
    {
    // doesn't own namespace definition
	iNamespaceDef = NULL;

	const TInt count = iBuffers.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		delete iBuffers[i].iBuffer;
		iBuffers[i].iBuffer = NULL;
		}
	iBuffers.Close();

	delete iGenerator;
	iGenerator = NULL;
	delete iUri;
	iUri = NULL;
    }

CMdSSqlObjectManipulate::CMdSSqlObjectManipulate( const CMdsSchema& aSchema, 
		CMdSObjectLockList& aLockList )
	: iSchema( aSchema ), iGenerator(NULL), iLockList( aLockList ) 
    {
    }

void CMdSSqlObjectManipulate::ConstructL( )
    {
	iGenerator = CMdSIdentifierGenerator::NewL();

	iNamespaceDef = NULL;
	
	for (TInt i = 0; i < KMaxBuffers; ++i)
		{
	    TLockBuffer lockBuffer;
	    lockBuffer.iLock = EFalse;
	    lockBuffer.iBuffer = CMdsClauseBuffer::NewLC( 1024 );
		iBuffers.AppendL( lockBuffer );
		CleanupStack::Pop(); // buffer
		}
    }

TBool CMdSSqlObjectManipulate::GarbageCollectionL()
	{
	return DoGarbageCollectionL();
	}

void CMdSSqlObjectManipulate::SetNamespace( 
		const CMdsNamespaceDef* aNamespaceDef )
	{
	iNamespaceDef = aNamespaceDef;
	}

void CMdSSqlObjectManipulate::AddMemoryCardL(TUint32 aMediaId)
	{
	TTime currentTime;
	currentTime.UniversalTime();

	TInt modifiedRowCount = MMdsPreferences::UpdateL( KMemoryCard, 
			MMdsPreferences::EPreferenceExtraSet, aMediaId, 
			currentTime.Int64() );

	if( modifiedRowCount <= 0 )
		{
		MMdsPreferences::InsertL( KMemoryCard, 
								  MMdsPreferences::EPreferenceBothSet, 
								  aMediaId, currentTime.Int64() );
		}
	}

void CMdSSqlObjectManipulate::GetMemoryCardL(TUint32& aMediaId)
	{
	TInt rowCount = MMdsPreferences::GetL( KMemoryCard, 
										   MMdsPreferences::EPreferenceValueGet | MMdsPreferences::EPreferenceExtraSortDesc, 
								  		   aMediaId );

	// if no rows found leave
	if( rowCount <= 0 )
		{
		User::Leave( KErrNotFound );
		}
	}

TBool CMdSSqlObjectManipulate::CheckMemoryCardL(TUint32 aMediaId)
	{
	TInt rowCount = MMdsPreferences::GetL( KMemoryCard, 
										   MMdsPreferences::EPreferenceValueGet | MMdsPreferences::EPreferenceValueSet, 
								  		   aMediaId );

	if( rowCount > 0 )
		{
		return ETrue;
		}
	return EFalse;
	}


void CMdSSqlObjectManipulate::SetMediaL(TUint32 aMediaId, TChar aDrive,
		TBool aPresentState)
	{
	_LIT( KInsertMedia, "INSERT OR REPLACE INTO MdS_Medias VALUES(?,?,?,?);" );

	RClauseBuffer commonClause( *this, KInsertMedia.iTypeLength );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& clauseBuffer = commonClause.BufferL();
	clauseBuffer.BufferL() = KInsertMedia;

	TTime currentTime;
	currentTime.UniversalTime();
	const TInt64 currentTime64 = currentTime.Int64();
	
    RRowData var;
    CleanupClosePushL( var );
    var.ReserveL( 4 ); // reserve space for media ID, drive, present state and time
    var.AppendL( TColumn( aMediaId ) );
    var.AppendL( TColumn( (TInt32)aDrive ) );
    var.AppendL( TColumn( aPresentState ) );
    var.AppendL( TColumn( currentTime64 ) );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	connection.ExecuteL( clauseBuffer.ConstBufferL(), var );

	CleanupStack::PopAndDestroy( 2, &commonClause ); // var, commonClauseOne
	}

TBool CMdSSqlObjectManipulate::GetMediaL(TUint32 aMediaId, TChar& aDrive, 
		TBool& aPresentState)
	{
	_LIT( KGetMedia, "SELECT Drive, PresentState FROM MdS_Medias WHERE MediaId=?;" );

	RClauseBuffer commonClause( *this, KGetMedia.iTypeLength );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& clauseBuffer = commonClause.BufferL();
	clauseBuffer.BufferL() = KGetMedia;

    RRowData var;
    CleanupClosePushL( var );
    var.AppendL( TColumn( aMediaId ) );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	RMdsStatement statement;
	CleanupClosePushL( statement );
	connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

	var.Free();
	var.Reset();

	TInt32 drive = 0;
	
    var.ReserveL( 2 ); // reserve space for drive and present state
	var.AppendL( TColumn( drive ) );
	var.AppendL( TColumn( aPresentState ) );

	TBool found = EFalse;

	if( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( drive );
		var.Column( 1 ).Get( aPresentState );
		
		aDrive = drive;
		
		found = ETrue;
		}

	CleanupStack::PopAndDestroy( 3, &commonClause ); // statement, var, commonClauseOne

	return found;
	}

TInt32 CMdSSqlObjectManipulate::GetPresentMediasL(TDes8& aMediaInfoBuffer)
	{
	_LIT( KGetPresentMedias, "SELECT MediaId, Drive FROM MdS_Medias WHERE PresentState!=?;" );

	RClauseBuffer commonClause( *this, KGetPresentMedias.iTypeLength );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& clauseBuffer = commonClause.BufferL();
	clauseBuffer.BufferL() = KGetPresentMedias;

	const TBool notPresentState = EFalse;

    RRowData var;
    CleanupClosePushL( var );
    var.AppendL( TColumn( notPresentState ) );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	RMdsStatement statement;
	CleanupClosePushL( statement );
	connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

	var.Free();
	var.Reset();

	TUint32 mediaId = 0;
	TInt32 drive = 0;

    var.ReserveL( 2 ); // reserve space for drive and present state
	var.AppendL( TColumn( mediaId ) );
	var.AppendL( TColumn( drive ) );

	TInt32 count = 0;

	TInt bufferPosition = 0;
	
	const TInt lastBufferPosition = aMediaInfoBuffer.MaxSize() - sizeof( TMdEMediaInfo );

	const TUint8* aMediaInfoBufferPtr = aMediaInfoBuffer.Ptr();
	
	while( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( mediaId );
		var.Column( 1 ).Get( drive );
		
		TMdEMediaInfo mediaInfo;
		mediaInfo.iMediaId = mediaId;
		mediaInfo.iDrive = drive;

		if( bufferPosition > lastBufferPosition )
			{
			User::Leave( KErrBadDescriptor );
			}
	
		TUint8* ptr = (TUint8*)( aMediaInfoBufferPtr + bufferPosition );
		
		Mem::Copy( ptr, &mediaInfo, sizeof( TMdEMediaInfo ) );
		
		bufferPosition += sizeof( TMdEMediaInfo );
		
		++count;
		}
	
	aMediaInfoBuffer.SetLength( bufferPosition );

	CleanupStack::PopAndDestroy( 3, &commonClause ); // statement, var, commonClauseOne

	return count;
	}

TItemId CMdSSqlObjectManipulate::SearchNotPresentFileL(TUint32 aMediaId, 
		TDesC16& aUri, TMdSFileInfo& aFileInfo,
		TFilePresentStates& aPlaceHolder, TBool& aNotPresentState)
	{
	_LIT( KSearchNotPresentFile, "SELECT ObjectId, Flags, LastModifiedDate, Size FROM Object%u WHERE NOT Flags&? AND (Flags&? OR Flags&? OR Flags&? ) AND MediaId=? AND URI=?;" );

	RClauseBuffer commonClauseOne(*this, KSearchNotPresentFile.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	clauseBuffer.BufferL().Format( KSearchNotPresentFile, KDefaultNamespaceDefId );

	TItemId objectId = KNoId;
	TUint32 flags = 0;

    RRowData var;
    CleanupClosePushL( var );
    var.ReserveL( 6 );
    var.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    var.AppendL( TColumn( EMdEObjectFlagNotPresent ) ); // not present flag
    var.AppendL( TColumn( EMdEObjectFlagStartUpNotPresent ) ); // start up not present flag
    var.AppendL( TColumn( EMdeObjectFlagPending ) ); // pending for composing - needed to complete composing
    var.AppendL( TColumn( aMediaId ) );
    var.AppendL( TColumn( aUri ) );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	RMdsStatement statement;
	CleanupClosePushL( statement );
    connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

    TInt64 modifiedTime(0);
    TUint32 size(0);

    RRowData result;
    CleanupClosePushL( result );
    result.ReserveL( 4 );
    result.AppendL( TColumn( objectId ) );
    result.AppendL( TColumn( flags ) );
    result.AppendL( TColumn( modifiedTime ) );
    result.AppendL( TColumn( size ) );

    // default values (used if object doesn't exist)
	aPlaceHolder = EMdsNotFound;
	aNotPresentState = EFalse;

	if( connection.NextRowL( statement, result ) )
		{
		result.Column( 0 ).Get( objectId );
		result.Column( 1 ).Get( flags );

		if ( flags & EMdEObjectFlagPlaceholder )
			{
			aPlaceHolder = EMdsPlaceholder;
			}
		else
			{
			result.Column( 2 ).Get( modifiedTime );
			result.Column( 3 ).Get( size );

			// check if file is modified
    		if ( ( aFileInfo.iModifiedTime == modifiedTime ) && ( aFileInfo.iSize == size ) )
    			{
    			aPlaceHolder = EMdsNormal;
    			
    	        // If images were pending for composing, composer needs to be notified for the composing to complete 
    	        if( (flags & EMdeObjectFlagPending) && (flags & EMdEObjectFlagStartUpNotPresent) )
    	            {
    	            aNotPresentState = ETrue;
    	            }
    			}
    		else
    			{
    			aPlaceHolder = EMdsModified;
    			}
			}

		if( flags & EMdEObjectFlagNotPresent )
			{
			aNotPresentState = ETrue;
			}
		}

	CleanupStack::PopAndDestroy( &result );
	CleanupStack::PopAndDestroy( &statement );
	CleanupStack::PopAndDestroy( &var );
	CleanupStack::PopAndDestroy( &commonClauseOne );
	return objectId;
	}

void CMdSSqlObjectManipulate::SetFilesToPresentL(const RArray<TItemId>& aObjectIds)
	{
	const TInt objectCount = aObjectIds.Count();
    if ( objectCount > 0 )
    	{
		_LIT( KSetFileToPresent, "UPDATE Object%u SET Flags=Flags&? WHERE (Flags&?)<>0 AND ObjectId IN (?" );
		_LIT( KCommaQuestionmark, ",?");
		_LIT( KBracketSemicolon, ");");
		
		RClauseBuffer commonClauseOne(*this, KSetFileToPresent.iTypeLength + KMaxUintValueLength +
				( KCommaQuestionmark.iTypeLength * ( objectCount-1 ) ) +
				KBracketSemicolon.iTypeLength );
		CleanupClosePushL( commonClauseOne );
		CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
		clauseBuffer.BufferL().Format( KSetFileToPresent, KDefaultNamespaceDefId );

	    RRowData var;
	    var.ReserveL( 3 );
	    CleanupClosePushL( var );
	    // reset not present and start up not present flags
	    var.AppendL( TColumn( ~(EMdEObjectFlagNotPresent | EMdEObjectFlagStartUpNotPresent) ) );
	    var.AppendL( TColumn( (EMdEObjectFlagNotPresent | EMdEObjectFlagStartUpNotPresent) ) );
	    var.AppendL( TColumn( aObjectIds[0] ) );

	    for ( TInt i = 1; i < objectCount; i++ )
	    	{
	    	clauseBuffer.AppendL( KCommaQuestionmark );
		    var.AppendL( TColumn( aObjectIds[i] ) );
	    	}
	    
	    clauseBuffer.AppendL( KBracketSemicolon );
	    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	    connection.ExecuteL( clauseBuffer.ConstBufferL(), var );
	
		CleanupStack::PopAndDestroy( &var );
		CleanupStack::PopAndDestroy( &commonClauseOne ); 
    	}
	}

void CMdSSqlObjectManipulate::SetRelationsToPresentL(TItemId aObjectId, 
		RArray<TItemId>& aIdArray)
	{
	_LIT( KSearchNotPresentRelations, "SELECT RelationId FROM Relations%u WHERE NOT Flags&? AND Flags&? AND LeftObjectId=? UNION SELECT RelationId FROM Relations%u WHERE NOT Flags&? AND Flags&? AND RightObjectId=?;" );
	_LIT( KSetRelationsToPresent1, "UPDATE Relations%u SET Flags=Flags&? WHERE Flags&? AND LeftObjectId=?;" );
	_LIT( KSetRelationsToPresent2, "UPDATE Relations%u SET Flags=Flags&? WHERE Flags&? AND RightObjectId=?;" );

	RClauseBuffer commonClauseOne(*this, KSearchNotPresentRelations.iTypeLength + 2*KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	clauseBuffer.BufferL().Format( KSearchNotPresentRelations, KDefaultNamespaceDefId, KDefaultNamespaceDefId );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RRowData var;
    CleanupClosePushL( var );
    
    var.ReserveL( 6 ); // reserve space for flags and object IDs
    var.AppendL( TColumn( EMdERelationFlagDeleted ) );
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( aObjectId ) );
    var.AppendL( TColumn( EMdERelationFlagDeleted ) );
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( aObjectId ) );

	RMdsStatement statement;
	CleanupClosePushL( statement );
	connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

	var.Free();	
	var.Reset();
    TItemId relationId(0);
    var.AppendL( TColumn( relationId ) );
	while( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( relationId );
		aIdArray.AppendL( relationId );
		}

	clauseBuffer.ReserveSpaceL( KSetRelationsToPresent1.iTypeLength + KMaxUintValueLength );
	clauseBuffer.BufferL().Format( KSetRelationsToPresent1, KDefaultNamespaceDefId );

    var.Free(); 
    var.Reset();

    var.ReserveL( 3 );
    var.AppendL( TColumn( ~EMdERelationFlagNotPresent ) ); // reset not present flag
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( aObjectId ) );

    connection.ExecuteL( clauseBuffer.ConstBufferL(), var );

    clauseBuffer.ReserveSpaceL( KSetRelationsToPresent2.iTypeLength + KMaxUintValueLength );
    clauseBuffer.BufferL().Format( KSetRelationsToPresent2, KDefaultNamespaceDefId );

    var.Free(); 
    var.Reset();

    var.AppendL( TColumn( ~EMdERelationFlagNotPresent ) ); // reset not present flag
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( aObjectId ) );

    connection.ExecuteL( clauseBuffer.ConstBufferL(), var );
    
	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // statement, var, clauseBuffer
	}

void CMdSSqlObjectManipulate::SetFilesToNotPresentL(TUint32 aMediaId, TBool aStartUp,
		RArray<TItemId>& aObjectIds)
	{	
	_LIT( KSearchPresentFilesStartUpL, "SELECT ObjectId FROM Object%u WHERE NOT Flags&? AND MediaId=?;" );
	_LIT( KSearchPresentFilesL, "SELECT ObjectId FROM Object%u WHERE NOT Flags&? AND NOT Flags&? AND MediaId=?;" );
	
	RClauseBuffer commonClauseOne(*this, ( aStartUp ? KSearchPresentFilesStartUpL.iTypeLength : KSearchPresentFilesL.iTypeLength )+ 
			KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	
	if( aStartUp )
		{
		clauseBuffer.BufferL().Format( KSearchPresentFilesStartUpL, KDefaultNamespaceDefId );
		}
	else
		{
		clauseBuffer.BufferL().Format( KSearchPresentFilesL, KDefaultNamespaceDefId );
		}

	TItemId objectId = 0;

    RRowData var;
    CleanupClosePushL( var );
    
    var.ReserveL( 3 ); // reserve space for flags and media ID
    var.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    if ( !aStartUp )
    	{
    	var.AppendL( TColumn( EMdEObjectFlagNotPresent ) ); // present flag
    	}
    var.AppendL( TColumn( aMediaId ) );

	RMdsStatement statement;
	CleanupClosePushL( statement );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

    var.Free(); 
    var.Reset();

    var.AppendL( TColumn( objectId ) );
	while( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( objectId );
		aObjectIds.AppendL( objectId );
		}

    var.Free(); 
    var.Reset();
	
	if( aStartUp )
	    {
        _LIT( KSetFilesToNotPresentBoot, "UPDATE Object%u SET Flags=Flags|? WHERE MediaId=?;" );
        clauseBuffer.ReserveSpaceL( 
                KSetFilesToNotPresentBoot.iTypeLength + 
                KMaxUintValueLength ); // TUint32 max value's lenght is 10 numbers so %u + 8
        clauseBuffer.BufferL().Format( KSetFilesToNotPresentBoot, KDefaultNamespaceDefId );

        var.AppendL( TColumn( EMdEObjectFlagStartUpNotPresent ) ); // set not present flag
	    }
	else
	    {
        _LIT( KSetFilesToNotPresent, "UPDATE Object%u SET Flags=Flags|? WHERE NOT (Flags&?)<>0 AND MediaId=?;" );
        clauseBuffer.ReserveSpaceL( 
                KSetFilesToNotPresent.iTypeLength + 
                KMaxUintValueLength ); // TUint32 max value's lenght is 10 numbers so %u + 8
        clauseBuffer.BufferL().Format( KSetFilesToNotPresent, KDefaultNamespaceDefId );

        var.AppendL( TColumn( EMdEObjectFlagNotPresent ) ); // set not present flag	
        var.AppendL( TColumn( EMdEObjectFlagNotPresent ) ); // check not present flag 
	    }

	var.AppendL( TColumn( aMediaId ) );

    connection.ExecuteL( clauseBuffer.ConstBufferL(), var );

    // statement, var, commonClauseOne
	CleanupStack::PopAndDestroy( 3, &commonClauseOne );
	}

void CMdSSqlObjectManipulate::SetRelationsToNotPresentL(
		TUint32 aMediaId, RArray<TItemId>& aRelationIds)
	{
	_LIT( KSearchPresentRelations, "SELECT A.RelationId FROM Relations%u AS A, Object%u AS B WHERE A.LeftObjectId = B.ObjectId AND B.MediaId=%u AND NOT A.Flags&%u AND NOT A.Flags&%u UNION SELECT A.RelationId FROM Relations%u AS A, Object%u AS B WHERE A.RightObjectId = B.ObjectId AND B.MediaId=%u AND NOT A.Flags&%u AND NOT A.Flags&%u" );
	_LIT( KSetRelationsToPresent, "UPDATE Relations%u SET Flags=Flags|? WHERE NOT Flags&? AND RelationId IN (%S);" );

	// RelationIDs query sql statement
	RClauseBuffer commonClauseOne(*this, 
			KSearchPresentRelations.iTypeLength + 
			10*KMaxUintValueLength  );
    CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBufferOne = commonClauseOne.BufferL();
	clauseBufferOne.BufferL().Format( KSearchPresentRelations, 
                KDefaultNamespaceDefId,
                KDefaultNamespaceDefId,
                aMediaId,
                EMdERelationFlagDeleted,
                EMdERelationFlagNotPresent,
                KDefaultNamespaceDefId,
                KDefaultNamespaceDefId,
                aMediaId,
                EMdERelationFlagDeleted,
                EMdERelationFlagNotPresent
                );
	
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    	
    RRowData var;
    CleanupClosePushL( var );
    
	RMdsStatement statement;
	CleanupClosePushL( statement );

	connection.ExecuteQueryL( clauseBufferOne.ConstBufferL(), statement, var );

    // Get RelationIDs From query result
    TItemId relationId(0);
    var.AppendL( TColumn( relationId ) );
	while( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( relationId );
		aRelationIds.AppendL( relationId );
		}
	
    // Set objects' relations not present by MediaID
	RClauseBuffer commonClauseTwo(*this, 
			KSetRelationsToPresent.iTypeLength + 			 
            KMaxUintValueLength +
			clauseBufferOne.ConstBufferL().Length() );
	
    CleanupClosePushL( commonClauseTwo );
	CMdsClauseBuffer& clauseBufferTwo = commonClauseTwo.BufferL();
	clauseBufferTwo.BufferL().Format( KSetRelationsToPresent, 
                                   KDefaultNamespaceDefId,
                                   &clauseBufferOne.ConstBufferL() );

	var.Free();
	var.Reset();
    var.ReserveL( 2 ); 
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );

    connection.ExecuteL( clauseBufferTwo.ConstBufferL(), var );

    // commonClauseTwo, statement, var, commonClauseOne
	CleanupStack::PopAndDestroy( 4, &commonClauseOne );
	}

void CMdSSqlObjectManipulate::RemoveFilesNotPresentL(TUint32 aMediaId, RArray<TItemId>* aObjectIds)
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	
	// collect object IDs from start up not present objects
	if( aObjectIds )
		{
		_LIT( KSelectFilesStartUpNotPresent, "SELECT ObjectId FROM Object%u WHERE NOT Flags&? AND NOT Flags&? AND Flags&? AND MediaId=?;" );

		RClauseBuffer commonClauseOne(*this, 
				KSelectFilesStartUpNotPresent.iTypeLength + KMaxUintValueLength );
		CleanupClosePushL( commonClauseOne );
		CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();

		clauseBuffer.BufferL().Format( KSelectFilesStartUpNotPresent, KDefaultNamespaceDefId );

		RRowData var;
		CleanupClosePushL( var );
		var.ReserveL( 4 );
		var.AppendL( TColumn( EMdEObjectFlagRemoved ) );
		var.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
		var.AppendL( TColumn( EMdEObjectFlagStartUpNotPresent ) );
		var.AppendL( TColumn( aMediaId ) );

		RMdsStatement statement;
		CleanupClosePushL( statement );
	    connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

		RRowData result;
	    CleanupClosePushL( result );
	    TItemId objectId( KNoId );
	    result.AppendL( TColumn( objectId ) );

		while( connection.NextRowL( statement, result ) )
			{
			result.Column( 0 ).Get( objectId );
			aObjectIds->AppendL( objectId );
			}

		CleanupStack::PopAndDestroy( &result );
		CleanupStack::PopAndDestroy( &statement );
		CleanupStack::PopAndDestroy( &var );
		CleanupStack::PopAndDestroy( &commonClauseOne );
		}

	_LIT( KRemoveFilesNotPresent, "UPDATE Object%u SET Flags=Flags|? WHERE NOT Flags&? AND (Flags&? OR Flags&?) AND MediaId=?;" );

	RClauseBuffer commonClauseOne(*this, 
			KRemoveFilesNotPresent.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	clauseBuffer.BufferL().Format( KRemoveFilesNotPresent, KDefaultNamespaceDefId );

    RRowData var;
    CleanupClosePushL( var );
    var.ReserveL( 5 ); 
    var.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    var.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    var.AppendL( TColumn( EMdEObjectFlagNotPresent ) ); // not present flag
    var.AppendL( TColumn( EMdEObjectFlagStartUpNotPresent ) ); // start up not present flag
    var.AppendL( TColumn( aMediaId ) );

	__LOGQUERY_16( _L("RemoveFilesNotPresentL:"), clauseBuffer.ConstBufferL(), var);
    connection.ExecuteL( clauseBuffer.ConstBufferL(), var );

	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // var, commonClauseOne
	}

void CMdSSqlObjectManipulate::GetSchemaVersionL(
		TInt& aMajorVersion, TInt& aMinorVersion)
	{
	TInt64 minorVersion;
	TInt rowCount = MMdsPreferences::GetL( KMdsSchemaVersionName, 
			MMdsPreferences::EPreferenceBothGet, 
			aMajorVersion, &minorVersion );

	aMinorVersion = minorVersion;

	// if no rows found leave
	if( rowCount <= 0 )
		{
		User::Leave( KErrNotFound );
		}
	}

void CMdSSqlObjectManipulate::SetObjectToPresentByGuidL( 
		const TInt64& aGuidHigh, const TInt64& aGuidLow )
	{
	// get object ID for later queries
	_LIT( KGetObjectIdByGuid, "SELECT ObjectId FROM Object%u WHERE Flags&? AND GuidHigh=? AND GuidLow=?;" );

	RClauseBuffer commonClauseOne(*this, 
			KGetObjectIdByGuid.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	clauseBuffer.BufferL().Format( KGetObjectIdByGuid, KDefaultNamespaceDefId );

    RRowData var;
    CleanupClosePushL( var );
    var.ReserveL( 3 ); // for flags and GUID high/low in WHERE

    var.AppendL( TColumn( (TUint32)EMdEObjectFlagNotPresent ) );
    var.AppendL( TColumn( aGuidHigh ) );
    var.AppendL( TColumn( aGuidLow ) );

	RMdsStatement statement;
	CleanupClosePushL( statement );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	__LOGQUERY_16( _L("SetObjectToPresentByGuidL q1:"), clauseBuffer.ConstBufferL(), var);

    connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

	var.Free();
	var.Reset();
	TItemId objectId = KNoId;
	var.AppendL( TColumn( objectId ) );

	if( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( objectId );

		// set object, which object ID is matching, to present state
		_LIT( KSetObjectToPresentByGuid, "UPDATE Object%u SET Flags=Flags&? WHERE ObjectId=? AND (Flags&?)<>0;" );

		RClauseBuffer commonClauseTwo(*this, 
				KSetObjectToPresentByGuid.iTypeLength + KMaxUintValueLength);
		CleanupClosePushL( commonClauseTwo );
		CMdsClauseBuffer& clauseBuffer2 = commonClauseTwo.BufferL();
		clauseBuffer2.BufferL().Format( KSetObjectToPresentByGuid, 
				KDefaultNamespaceDefId );

		var.Free();
		var.Reset();
		var.AppendL( TColumn( (TUint32)~EMdEObjectFlagNotPresent ) );
		var.AppendL( TColumn( objectId ) );
		
		var.AppendL( TColumn( (TUint32)EMdEObjectFlagNotPresent ) );

		__LOGQUERY_16( _L("SetObjectToPresentByGuidL q2:"), 
				clauseBuffer2.ConstBufferL(), var);
		
		connection.ExecuteL( clauseBuffer2.ConstBufferL(), var );
		
		CleanupStack::PopAndDestroy( &commonClauseTwo );

		// set relations, which left or right object ID is matching, 
		// to present state
		_LIT( KSetRelationToPresentByGuid, "UPDATE Relations%u SET Flags=Flags&? WHERE LeftObjectId=? OR RightObjectId=?;" );
		
		RClauseBuffer commonClauseThree(*this, 
				KSetRelationToPresentByGuid.iTypeLength + KMaxUintValueLength);
		CleanupClosePushL( commonClauseThree );
		CMdsClauseBuffer& clauseBuffer3 = commonClauseThree.BufferL();
		clauseBuffer3.BufferL().Format( KSetRelationToPresentByGuid, 
				KDefaultNamespaceDefId );

		var.Free();
		var.Reset();
		var.AppendL( TColumn( (TUint32)~EMdERelationFlagNotPresent ) );
		var.AppendL( TColumn( objectId ) );
		var.AppendL( TColumn( objectId ) );

		__LOGQUERY_16( _L("SetObjectToPresentByGuidL q3:"), 
				clauseBuffer3.ConstBufferL(), var);

		connection.ExecuteL( clauseBuffer3.ConstBufferL(), var );
		
		CleanupStack::PopAndDestroy( &commonClauseThree );
		}
	// no any matching object
	else
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // statement, var, commonClauseOne
	}

void CMdSSqlObjectManipulate::ChangePathL(
		const TDesC& aOldPath, const TDesC& aNewPath, 
		RArray<TItemId>& aObjectIds)
	{
	// collect object IDs from object which match to the old path
	_LIT( KGetObjectIdByBeginOfUri, "SELECT ObjectId FROM Object%u WHERE NOT Flags&? AND substr(URI, 1, ?) = ?;" );

	RClauseBuffer commonClauseOne(*this, 
			KGetObjectIdByBeginOfUri.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBuffer = commonClauseOne.BufferL();
	clauseBuffer.BufferL().Format( KGetObjectIdByBeginOfUri, KDefaultNamespaceDefId );

	const TUint32 flags = EMdEObjectFlagNotPresent | EMdEObjectFlagRemoved | EMdEObjectFlagGarbage;

	const TInt oldPathLength = aOldPath.Length();

	RRowData var;
    CleanupClosePushL( var );
	var.ReserveL( 3 );
    var.AppendL( TColumn( flags ) );
    var.AppendL( TColumn( oldPathLength ) );
    var.AppendL( TColumn( aOldPath ) );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RMdsStatement statement;
	CleanupClosePushL( statement );

	__LOGQUERY_16( _L("ChangePathL q1:"), clauseBuffer.ConstBufferL(), var);

    connection.ExecuteQueryL( clauseBuffer.ConstBufferL(), statement, var );

    var.Free();
    var.Reset();
    TItemId objectId( 0 );
    var.AppendL( TColumn( objectId ) );

	while( connection.NextRowL( statement, var ) )
		{
		var.Column( 0 ).Get( objectId );
		aObjectIds.AppendL( objectId );
		}

	// update the new path to objects which match with the old path
	_LIT( KChangeOldPathToNewPath, "UPDATE Object%u SET URI=? || substr(URI, ?, length(URI)) WHERE NOT Flags&? AND substr(URI, 1, ?) = ?;" );

	RClauseBuffer commonClauseTwo(*this, 
			KChangeOldPathToNewPath.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseTwo );
	CMdsClauseBuffer& clauseBuffer2 = commonClauseTwo.BufferL();
	clauseBuffer2.BufferL().Format( KChangeOldPathToNewPath, KDefaultNamespaceDefId );

	var.Free();
	var.Reset();

	var.ReserveL( 5 );
    var.AppendL( TColumn( aNewPath ) );
    var.AppendL( TColumn( oldPathLength + 1 ) );
    var.AppendL( TColumn( flags ) );
    var.AppendL( TColumn( oldPathLength ) );
    var.AppendL( TColumn( aOldPath ) );

	__LOGQUERY_16( _L("ChangePathL q2:"), clauseBuffer2.ConstBufferL(), var);

	connection.ExecuteL( clauseBuffer2.ConstBufferL(), var );

	CleanupStack::PopAndDestroy( 4, &commonClauseOne ); // commonClauseTwo, statement, var, commonClauseOne
	}

void CMdSSqlObjectManipulate::ChangeMediaIdL()
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	
	TVolumeInfo volumeInfo;
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	fs.Volume( volumeInfo, EDriveC );
	CleanupStack::PopAndDestroy( &fs );

	// update the new media id to objects which match with the old id
	_LIT( KChangeOldIdToNewId, "UPDATE Object%u SET MediaId=? WHERE NOT Flags&? AND MediaId = (SELECT Value FROM MdE_Preferences WHERE Key=?);" );

	RClauseBuffer clauseOne( *this, KChangeOldIdToNewId.iTypeLength + KMaxUintValueLength );
	CleanupClosePushL( clauseOne );
	CMdsClauseBuffer& clauseBuffer = clauseOne.BufferL();
	clauseBuffer.BufferL().Format( KChangeOldIdToNewId, KDefaultNamespaceDefId );

	const TUint32 flags = EMdEObjectFlagNotPresent | EMdEObjectFlagRemoved | EMdEObjectFlagGarbage;

	RRowData var;
	CleanupClosePushL( var );

	var.ReserveL( 3 );
	var.AppendL( TColumn( (TUint32) volumeInfo.iUniqueID ) );
	var.AppendL( TColumn( flags ) );
	var.AppendL( TColumn( KCMediaIdKey ) );

	__LOGQUERY_16( _L("ChangeMediaIdL q2:"), clauseBuffer.ConstBufferL(), var);

	connection.ExecuteL( clauseBuffer.ConstBufferL(), var );
	
	// update the new media id to objects which match with the old id
	_LIT( KUpdateMediaId, "UPDATE MdE_Preferences SET Value=? WHERE Key = ?;" );
	
	var.Free();
	var.Reset();
	
	TTime currentTime;
	currentTime.UniversalTime();
	const TInt64 currentTime64 = currentTime.Int64();

	var.ReserveL( 2 );
	var.AppendL( TColumn( (TUint32) volumeInfo.iUniqueID ) );
	var.AppendL( TColumn( KCMediaIdKey ) );

	connection.ExecuteL( KUpdateMediaId, var );

	CleanupStack::PopAndDestroy( &var );
	CleanupStack::PopAndDestroy( &clauseOne );
	}

void CMdSSqlObjectManipulate::CheckMassStorageMediaIdL( const TUint32 aMediaId )
    {
    TUint32 oldMediaId( 0 );
    const TInt rowCount = MMdsPreferences::GetL( KMassStorageMediaIdKey, 
                                                                 MMdsPreferences::EPreferenceValueGet, oldMediaId );
    if( rowCount == 0 )
        {
        MMdsPreferences::InsertL( KMassStorageMediaIdKey, MMdsPreferences::EPreferenceValueSet, (TUint32) aMediaId );
        }
    else if( aMediaId != oldMediaId )
        {
        RArray<TItemId> objectIds;
        CleanupClosePushL( objectIds );
        MMdsPreferences::UpdateL( KMassStorageMediaIdKey, MMdsPreferences::EPreferenceValueSet, (TUint32) aMediaId );
        SetFilesToNotPresentL( oldMediaId, EFalse, objectIds );
        CleanupStack::PopAndDestroy( &objectIds );
        RemoveFilesNotPresentL( oldMediaId, NULL );
        }
    }

const CMdsPropertyDef& CMdSSqlObjectManipulate::ReadPropertyL( 
		CMdCSerializationBuffer& aBuffer, const CMdsObjectDef& aObjectDef,
		CMdsClauseBuffer& aBaseObjectClause, CMdsClauseBuffer& aObjectClause,
		RRowData& aBaseObjectDataRow, RRowData& aObjectDataRow, TUint8& aFlags)
	{
	const TMdCProperty& property = TMdCProperty::GetFromBufferL( aBuffer );
	const CMdsPropertyDef* propertyDef = aObjectDef.GetPropertyByIdL( 
			property.iPropertyDefId );
	if ( !propertyDef )
		{
#ifdef _DEBUG
		WRITELOG( "Incorrect property" );
#endif
		User::Leave( KErrMdEUnknownPropertyDef );
		}
	aFlags = property.iModFlags;

	if (property.iModFlags == EMdEPropertyModNone)
		{
		return *propertyDef;
		}

	const TBool baseObjectProperty = 
		iBaseObjectDef->GetPropertyByIdL(property.iPropertyDefId) != NULL;

	// if property is not removed add actual value
	if( !( property.iModFlags & EMdEPropertyModRemove ) )
		{
		switch(propertyDef->GetType())
			{
			case EPropertyBool:
				{
				const TBool boolValue = property.iValue.iInt32;
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( boolValue ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( boolValue ) );
					}
				break;
				}
			case EPropertyInt8:
				{
				const TInt8 int8Value = property.iValue.iInt32;
				if ( !propertyDef->CheckMinMaxValue( (TInt32)int8Value ) )
					{
#ifdef _DEBUG
					TInt32 debugValue = int8Value;
					WRITELOG2( "Incorrect property[%S] value: %d", 
							&propertyDef->GetName(), debugValue );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( int8Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( int8Value ) );
					}
				break;
				}
			case EPropertyUint8:
				{
				const TUint8 uInt8Value = property.iValue.iUint32;
				if ( !propertyDef->CheckMinMaxValue( (TInt32)uInt8Value ) )
					{
#ifdef _DEBUG
					TInt32 debugValue = uInt8Value;
					WRITELOG2( "Incorrect property[%S] value: %d", 
							&propertyDef->GetName(), debugValue );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( uInt8Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( uInt8Value ) );
					}
				break;
				}
			case EPropertyInt16:
				{
				const TInt16 int16Value = property.iValue.iInt32;
				if ( !propertyDef->CheckMinMaxValue( (TInt32)int16Value ) )
					{
#ifdef _DEBUG
					TInt32 debugValue = int16Value;
					WRITELOG2( "Incorrect property[%S] value: %d", 
							&propertyDef->GetName(), debugValue );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( int16Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( int16Value ) );
					}
				break;
				}
			case EPropertyUint16:
				{
				const TUint16 uInt16Value = property.iValue.iUint32;
				if ( !propertyDef->CheckMinMaxValue( (TInt32)uInt16Value ) )
					{
#ifdef _DEBUG
					TInt32 debugValue = uInt16Value;
					WRITELOG2( "Incorrect property[%S] value: %d", 
							&propertyDef->GetName(), debugValue );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( uInt16Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( uInt16Value ) );
					}
				break;
				}
			case EPropertyInt32:
				{
				const TInt32 int32Value = property.iValue.iInt32;
				if ( !propertyDef->CheckMinMaxValue( int32Value ) )
					{
#ifdef _DEBUG
					WRITELOG2( "Incorrect property[%S] value: %d", 
							&propertyDef->GetName(), int32Value );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( int32Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( int32Value ) );
					}
				break;
				}
			case EPropertyUint32:
				{
				const TUint32 uInt32Value = property.iValue.iUint32;
				if ( !propertyDef->CheckMinMaxValue( uInt32Value ) )
					{
#ifdef _DEBUG
					WRITELOG2( "Incorrect property[%S] value: %u", 
							&propertyDef->GetName(), uInt32Value );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( uInt32Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( uInt32Value ) );
					}
				break;
				}
			case EPropertyInt64:
				{
				const TInt64 int64Value = property.iValue.iInt64;
				if ( !propertyDef->CheckMinMaxValue( int64Value ) )
					{
#ifdef _DEBUG
					WRITELOG2( "Incorrect property[%S] value: %Ld", 
							&propertyDef->GetName(), int64Value );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( int64Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( int64Value ) );
					}
				break;
				}
			case EPropertyTime:
				{
				const TInt64 int64Value = property.iValue.iInt64;
				if ( !propertyDef->CheckMinMaxValue( int64Value ) )
					{
#ifdef _DEBUG					
					WRITELOG2( "Incorrect property[%S] value: %Ld", 
							&propertyDef->GetName(), int64Value );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( int64Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( int64Value ) );
					}
				break;
				}
			case EPropertyReal32:
				{
				const TReal32 real32Value = property.iValue.iReal;
				if ( !propertyDef->CheckMinMaxValue( real32Value ) )
					{
#ifdef _DEBUG
					TReal64 debugValue = real32Value;
					WRITELOG2( "Incorrect property[%S] value: %.2f", 
							&propertyDef->GetName(), debugValue );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( real32Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( real32Value ) );
					}
				break;
				}
			case EPropertyReal64:
				{
				const TReal64 real64Value = property.iValue.iReal;
				if ( !propertyDef->CheckMinMaxValue( real64Value ) )
					{
#ifdef _DEBUG					
					WRITELOG2( "Incorrect property[%S] value: %.2f", 
							&propertyDef->GetName(), real64Value );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( real64Value ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( real64Value ) );
					}
				break;
				}
			case EPropertyText:
				{
				aBuffer.PositionL( property.iValue.iPtr.iOffset );
				TPtrC16 textValue( aBuffer.ReceivePtr16L() );
				if ( !propertyDef->CheckMinMaxValue( (TInt32)textValue.Length() ) )
					{
#ifdef _DEBUG
					WRITELOG3( "Incorrect property[%S] value: \"%S\", length: %d", 
							&propertyDef->GetName(), &textValue, 
							textValue.Length() );
#endif
					User::Leave( KErrMdEPropertyValueNotBetweenAllowedMinMax );
					}
				if ( baseObjectProperty )
					{
					aBaseObjectDataRow.AppendL( TColumn( textValue ) );
					}
				else
					{
					aObjectDataRow.AppendL( TColumn( textValue ) );
					}
				break;
				}
			default:
				User::Leave( KErrMdEUnknownPropertyType );
			}
		}
	// if property is removed add null value
	else
		{
		TPtrC16 val = TPtr16((TUint16*)0, 0);
		if( baseObjectProperty )
			{
			aBaseObjectDataRow.AppendL( TColumn( val ) );
			}
		else
			{
			aObjectDataRow.AppendL( TColumn( val ) );
			}
		}

	if( baseObjectProperty )
		{
		aBaseObjectClause.AppendL( KComma );
		aBaseObjectClause.AppendL( propertyDef->GetName() );
		}
	else
		{
		aObjectClause.AppendL( KComma );
		aObjectClause.AppendL( propertyDef->GetName() );
		}
	
	return *propertyDef;
	}

TItemId CMdSSqlObjectManipulate::AddObjectL( CMdSSqLiteConnection& aConnection, 
       CMdCSerializationBuffer& aBuffer,
       RMdsStatement& aMdsBaseObjStatement, RMdsStatement& aMdsObjStatement,
       const CMdSServerSession* aServerSession )
	{
	_LIT( KMdsObjectAddBaseObjectBegin, "INSERT INTO Object%u(ObjectId,ObjectDefId,Flags,MediaId,GuidHigh,GuidLow,URI" );
	_LIT( KMdsObjectAddBaseObjectMiddle, ") VALUES(?,?,?,?,?,?,?" );
	_LIT( KMdsObjectAddObjectBegin, "INSERT INTO %S%u(ObjectId" );
	_LIT( KMdsObjectAddObjectMiddle, ") VALUES(?" );
	_LIT( KMdsObjectAddObjectEnd,   ");" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RRowData baseObjectRow;
	CleanupClosePushL( baseObjectRow );
	RRowData objectRow;
	CleanupClosePushL( objectRow );

	const TMdCObject& object = TMdCObject::GetFromBufferL( aBuffer );
	// there must be at least one property (btw. base object have 7 properties)
	if (object.iProperties.iPtr.iCount < 1)
		{
		User::Leave( KErrMdEMandatoryPropertyMissing );
		}

	// objectid
	if (object.iId != KNoId)
		{
		User::Leave( KErrAlreadyExists );
		}

	RSQLIndex sqlIndex;
	CleanupClosePushL( sqlIndex );
	const TItemId objectId = sqlIndex.GetId();

	baseObjectRow.AppendL( TColumn( objectId ) );
	objectRow.AppendL( TColumn( objectId ) );

	// objectdefid
	const CMdsObjectDef* objectDef = iNamespaceDef->GetObjectByIdL( object.iDefId );
	if ( !objectDef )
		{
		// objectDef doesn't exist
		User::Leave( KErrMdEUnknownObjectDef );
		}
	baseObjectRow.AppendL( TColumn( object.iDefId ) );

	// get BaseObjectDef
	iBaseObjectDef = iNamespaceDef->GetObjectByIdL( KBaseObjectDefId );

	RClauseBuffer commonClauseOne(*this);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseBaseObject = commonClauseOne.BufferL(); 
	RClauseBuffer commonClauseTwo(*this);
	CleanupClosePushL( commonClauseTwo );
	CMdsClauseBuffer& clauseObject = commonClauseTwo.BufferL(); 

	const TDesC& objName = objectDef->GetName();
	if(objName != iLastAddedObjName)
	    {
        iLastAddedObjName = objName;
	    aMdsObjStatement.Close();
        aMdsObjStatement = RMdsStatement();
        }
 
    clauseObject.BufferL().Format( KMdsObjectAddObjectBegin, &objName, iNamespaceDef->GetId() );
	clauseBaseObject.BufferL().Format( KMdsObjectAddBaseObjectBegin, iNamespaceDef->GetId() );

	TUint32 objectFlags = 0;
	if ( !( object.iFlags & ( EMdEObjectFlagModOpen | EMdEObjectFlagAutoLock ) ) )
		{
		User::Leave( KErrMdENotLocked );
		}

	if (object.iFlags & EMdEObjectFlagConfidential) 
		{
		objectFlags |= EMdEObjectFlagConfidential;
		}
	if (object.iFlags & EMdEObjectFlagModFreeText)
		{
		objectFlags |= EMdEObjectFlagFreetexts;
		}
	if (object.iFlags & EMdEObjectFlagPlaceholder)
		{
		objectFlags |= EMdEObjectFlagPlaceholder;
		}
	if (objectDef->GetFlags() == CMdsObjectDef::EObjectDefFlagsContext)
		{
		objectFlags |= EMdEObjectFlagContext;
		}
	baseObjectRow.AppendL( TColumn( objectFlags ) );

	// mediaId
	baseObjectRow.AppendL( TColumn( object.iMediaId ) );

	TInt64 guidHigh = object.iGuidHigh;
	TInt64 guidLow = object.iGuidLow;

	if (guidLow == 0 && guidHigh == 0)
		{
		iGenerator->GenerateGuid( guidHigh, guidLow );
		}

	baseObjectRow.AppendL( TColumn( guidHigh ) );
	baseObjectRow.AppendL( TColumn( guidLow ) );

	// uri
	HBufC* uriBuf = NULL;
	if (object.iUri.iPtr.iCount == 0)
		{
		uriBuf = iGenerator->GenerateUriL( *objectDef, guidHigh, guidLow );
		}
	else
		{
		aBuffer.PositionL( object.iUri.iPtr.iOffset );
		uriBuf = aBuffer.ReceiveDes16L();
		}
	CleanupStack::PushL( uriBuf );
	TPtr16 uri( uriBuf->Des() );
	uri.LowerCase();
	baseObjectRow.AppendL( TColumn( uri ) );

	const TInt baseObjectRowSizeWithoutProperties = baseObjectRow.Size();

	TInt mandatoryPropertyCount = objectDef->GetMandatoryPropertyCount();
	
	// try to add property
	for ( TUint32 i = 0; i < object.iProperties.iPtr.iCount; ++i )
		{
		aBuffer.PositionL( object.iProperties.iPtr.iOffset + i * sizeof(TMdCProperty) );
		TUint8 modFlags;
		const CMdsPropertyDef& propertyDef = ReadPropertyL( aBuffer, *objectDef, clauseBaseObject, clauseObject, baseObjectRow, objectRow, modFlags );
		if ( modFlags == EMdEPropertyModNone )
			{
			continue;
			}

		// check if mandatory property is removed
		if( propertyDef.GetMandatory() )
			{
			if( modFlags & EMdEPropertyModRemove )
				{
				User::Leave( KErrMdEMandatoryPropertyMissing );
				}
			else
				{
				--mandatoryPropertyCount;
				}
			}
		}

	if( mandatoryPropertyCount != 0 )
		{
		User::Leave( KErrMdEMandatoryPropertyMissing );
		}

	clauseBaseObject.AppendL( KMdsObjectAddBaseObjectMiddle );

	const TInt baseObjectPropertyCount = baseObjectRow.Size() - 
		baseObjectRowSizeWithoutProperties;

	for ( TInt i = 0; i < baseObjectPropertyCount; ++i )
		{
		clauseBaseObject.AppendL( KComma );
		clauseBaseObject.AppendL( KVariable );
		}

	clauseObject.AppendL( KMdsObjectAddObjectMiddle );

	// object row property count (object row size without object ID)
	const TInt objectPropertyCount = objectRow.Size() - 1;

	for ( TInt i = 0; i < objectPropertyCount; ++i )
		{
		clauseObject.AppendL( KComma );
		clauseObject.AppendL( KVariable );
		}
	
	clauseBaseObject.AppendL( KMdsObjectAddObjectEnd );
	clauseObject.AppendL( KMdsObjectAddObjectEnd );

	// EVERYTHING IS OK - add object to DB

	TInt queryResult = 0, err;
	// add base object properties
	__LOGQUERY_16( _L("Add BaseObject to DB:"), clauseBaseObject.ConstBufferL(), baseObjectRow);
	TRAP( err, aConnection.ExecuteL( clauseBaseObject.ConstBufferL(), baseObjectRow, &aMdsBaseObjStatement ) );
	if (err == KSqlErrConstraint)
		{
		__LOG1( ELogQuery, "Adding baseObject constraint error - err:%d", err );
		TRAP_IGNORE( RemoveObjectForceL( uri, KNoId ) );
		__LOGQUERY_16( _L("Add AGAIN BaseObject to DB:"), clauseBaseObject.ConstBufferL(), baseObjectRow);
		TRAP( err, aConnection.ExecuteL( clauseBaseObject.ConstBufferL(), baseObjectRow, &aMdsBaseObjStatement ) );
		}

	if (err != KErrNone)
		{
		__LOG1( ELogQuery, "Adding baseObject failed - err:%d", err );
		User::Leave( KErrGeneral );
		}

	if( !( // skip if object definition is "base object"
		   // and object is placeholder
			(object.iDefId == KBaseObjectDefId) &&
			(object.iFlags & EMdEObjectFlagPlaceholder)
		 ) 
	  )
		{	
		__LOGQUERY_16( _L("Add Object to DB:"), clauseObject.ConstBufferL(), 
				objectRow);
		TRAP( err, queryResult = aConnection.ExecuteL( 
		        clauseObject.ConstBufferL(), objectRow, &aMdsObjStatement ) );
		if (err != KErrNone || queryResult != 1)
			{
			__LOG2( ELogQuery, "Adding object failed - err:%d, queryResult:%d", err, queryResult );
			TRAP_IGNORE( RemoveObjectForceL( objectId ) );
			User::Leave( err );
			}
		}

	CleanupStack::PopAndDestroy( uriBuf );
	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // commonClauseTwo, commonClauseOne

	// add freetext to DB
	if ( object.iFreeTexts.iPtr.iCount > 0 )
		{
		aBuffer.PositionL( object.iFreeTexts.iPtr.iOffset );
		TRAP( err, AddFreeTextL( aBuffer, object.iFreeTexts.iPtr.iCount, objectId ) );
		if (err != KErrNone)
			{
			__LOG1( ELogQuery, "Adding object freetext failed - err:%d", err );
			TRAP_IGNORE( RemoveObjectForceL( objectId ) );
			User::Leave( err );
			}
		}

	sqlIndex.Commit();
	CleanupStack::PopAndDestroy( &sqlIndex );
	CleanupStack::PopAndDestroy( 2, &baseObjectRow ); // objectRow, baseObjectRow

	if( ( object.iFlags & EMdEObjectFlagAutoLock ) && aServerSession )
		{
		iLockList.LockObjectL( *aServerSession, *iNamespaceDef, objectId );
		}
	
	return objectId;
	}

TUint32 CMdSSqlObjectManipulate::AddFreeTextToDBL( TPtrC16& aWord, TItemId aObjectId,
    TInt32 aPosition, TBool aSearch )
	{
	_LIT( KMdsFreeTextAdd, "INSERT INTO TextSearch%d(WordId,ObjectId,Position) VALUES(?,?,?)" );
	_LIT( KMdsFreeTextDictAdd, "INSERT INTO TextSearchDictionary%d(Word) VALUES(?)" );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RClauseBuffer commonClauseOne(*this);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& freeTextAdd     = commonClauseOne.BufferL(); 
	RClauseBuffer commonClauseTwo(*this);
	CleanupClosePushL( commonClauseTwo );
	CMdsClauseBuffer& freeTextDictAdd = commonClauseTwo.BufferL();

	freeTextAdd.BufferL().Format( KMdsFreeTextAdd, iNamespaceDef->GetId() );
	freeTextDictAdd.BufferL().Format( KMdsFreeTextDictAdd, iNamespaceDef->GetId() );

	TItemId freeTextId = KNoId;
	RRowData freeTextRow;
	CleanupClosePushL( freeTextRow );
	RRowData freeTextRowDict;
	CleanupClosePushL( freeTextRowDict );

	freeTextRow.ReserveL( 3 );

	freeTextRow.AppendL( TColumn(KNoId) );
	freeTextRow.AppendL( TColumn(aObjectId) );
	freeTextRow.AppendL( TColumn(aPosition) );

    freeTextId = FindFreeTextInDbL( aWord );
	if ( aSearch && freeTextId )
		{
		freeTextRow.Column(0).Set( freeTextId );
		__LOGQUERY_16( _L("Add FreeText index to DB:"), 
				freeTextAdd.ConstBufferL(), freeTextRow);
		connection.ExecuteL( freeTextAdd.ConstBufferL(), freeTextRow );
		}
	else
		{
		freeTextRowDict.AppendL( TColumn( aWord ) );
		__LOGQUERY_16( _L("Add FreeText to DB:"), 
				freeTextDictAdd.ConstBufferL(), freeTextRowDict);
		TRAPD( err, freeTextId = MMdSIndexer::ExecuteAndGetIndexL( 
				freeTextDictAdd.ConstBufferL(), freeTextRowDict ) );
		if (err != KErrNone || freeTextId == KNoId)
			{
			__LOG2( ELogQuery, "Adding freetextDict failed - err:%d, freeTextId:%d", 
					err, freeTextId );
			User::Leave( KErrGeneral );
			}
		
		freeTextRow.Column(0).Set( freeTextId );
		connection.ExecuteL( freeTextAdd.ConstBufferL(), freeTextRow );
		}

	CleanupStack::PopAndDestroy( 4, &commonClauseOne ); // freeTextRowDict, freeTextRow, commonClauseTwo, commonClauseOne
	return freeTextId;
	}

TInt CMdSSqlObjectManipulate::AddFreeTextL( CMdCSerializationBuffer& aBuffer, TInt aFreeTextCount, TItemId aObjectId )
	{
	TInt freeTextAdded = 0;

	for ( TUint32 i = 0; i < aFreeTextCount; ++i )
		{
		TPtrC16 freeText = aBuffer.ReceivePtr16L();
		AddFreeTextToDBL( freeText, aObjectId, i );
		++freeTextAdded;
		}
	return freeTextAdded;
	}

TItemId CMdSSqlObjectManipulate::FindFreeTextInDbL( TPtrC16 aFreeText )
	{
	_LIT( KMdsFreeTextSearch, "SELECT WordId FROM TextSearchDictionary%d WHERE Word=? LIMIT 1;" );
	
	RClauseBuffer commonClauseOne(*this, KMdsFreeTextSearch.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& freeTextSearchClause = commonClauseOne.BufferL();
	freeTextSearchClause.BufferL().Format( KMdsFreeTextSearch, iNamespaceDef->GetId() );

	TItemId freeTextId = 0;
	RRowData wordRow;
	CleanupClosePushL( wordRow );
	RRowData idRow;
	CleanupClosePushL( idRow );
	RMdsStatement query;
	CleanupClosePushL( query );
	
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	wordRow.AppendL( TColumn( aFreeText ) );
	idRow.AppendL( TColumn( freeTextId ) );
	connection.ExecuteQueryL( freeTextSearchClause.ConstBufferL(), query, wordRow );
	if( connection.NextRowL( query, idRow ) )
		{
		idRow.Column( 0 ).Get( freeTextId );
		}

	CleanupStack::PopAndDestroy( 4, &commonClauseOne ); // query, idRow, wordRow, commonClauseOne
	return freeTextId;
	}

void CMdSSqlObjectManipulate::RemoveObjectForceL( const TDesC16& aUri, TItemId aObjectId )
	{
	_LIT( KMdsRemoveObjectByUri, "DELETE FROM Object%u WHERE Flags&? AND ObjectId!=? AND URI=?;" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RClauseBuffer commonClauseOne(*this, KMdsRemoveObjectByUri.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& removeObjectClause = commonClauseOne.BufferL();

	removeObjectClause.BufferL().Format( KMdsRemoveObjectByUri, 
			iNamespaceDef->GetId() );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    // do remove
    RRowData varRemove;
    CleanupClosePushL( varRemove );
    varRemove.ReserveL( 3 );
    varRemove.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    varRemove.AppendL( TColumn( aObjectId ) );
    varRemove.AppendL( TColumn( aUri ) );

#ifdef _DEBUG    
#ifdef LOG_MASTER_FLAG
	TInt queryResult = 0; // For debug
	__LOGQUERY_16( _L("Remove FORCE object using URI:"), 
			removeObjectClause.ConstBufferL(), varRemove);
	TRAPD( err, queryResult = connection.ExecuteL( 
			removeObjectClause.ConstBufferL(), varRemove ) );
	if( err != KErrNone )
		{
		__LOG2( ELogQuery, "Remove FORCE object err:%d, queryResult:%d", err, queryResult );
		}
#else
    TRAP_IGNORE( connection.ExecuteL( removeObjectClause.ConstBufferL(), varRemove ) );
#endif // LOG_MASTER_FLAG
#else
    TRAP_IGNORE( connection.ExecuteL( removeObjectClause.ConstBufferL(), varRemove ) );    
#endif // _DEBUG
	
	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // varRemove, commonClauseOne
	}

// USE WITH CAUTION !!!
// THIS FUNCTION REMOVES OBJECT WITHOUT ANY CHECKING
// ONLY FOR INTERNAL USE !!!
void CMdSSqlObjectManipulate::RemoveObjectForceL( TItemId aObjectId )
	{
	_LIT( KMdsRemoveObjectById, "DELETE FROM Object%u WHERE ObjectId=?;" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RClauseBuffer commonClauseOne(*this,  KMdsRemoveObjectById.iTypeLength + KMaxUintValueLength );
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& removeObjectClause = commonClauseOne.BufferL();

	removeObjectClause.BufferL().Format( KMdsRemoveObjectById, iNamespaceDef->GetId() );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    // do remove
    RRowData varRemove;
    CleanupClosePushL( varRemove );
    varRemove.AppendL( TColumn( aObjectId ) );

#ifdef _DEBUG    
#ifdef LOG_MASTER_FLAG
	TInt queryResult = 0; // For debug
	__LOGQUERY_16( _L("Remove object using ID:"), 
			removeObjectClause.ConstBufferL(), varRemove);
	TRAPD( err, queryResult = connection.ExecuteL( 
			removeObjectClause.ConstBufferL(), varRemove ) );
	if ( err != KErrNone )
		{
		__LOG2( ELogQuery, "Remove FORCE Object err:%d, queryResult:%d", err, queryResult );
		}
#else
    TRAP_IGNORE( connection.ExecuteL( removeObjectClause.ConstBufferL(), varRemove ) );
#endif // LOG_MASTER_FLAG
#else
    TRAP_IGNORE( connection.ExecuteL( removeObjectClause.ConstBufferL(), varRemove ) );  
#endif // _DEBUG
    
	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // varRemove, commonClauseOne
	}

CMdCSerializationBuffer* CMdSSqlObjectManipulate::CheckObjectL( TInt aResultBufferSize, 
		const TDesC& aUri, TDefId aNamespaceDefId )
	{
	_LIT( KMdsCheckObjectByUri, "SELECT ObjectId, ObjectDefId, Flags FROM Object%u WHERE URI=?;" );

	RClauseBuffer commonClauseOne(*this, KMdsCheckObjectByUri.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& checkObjectClause = commonClauseOne.BufferL();

	checkObjectClause.BufferL().Format( KMdsCheckObjectByUri, aNamespaceDefId );

    RRowData rowData;
    CleanupClosePushL( rowData );
    rowData.AppendL( TColumn( aUri ) );
    
    RMdsStatement query;
	CleanupClosePushL( query );

	CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();

	db.ExecuteQueryL( checkObjectClause.ConstBufferL(), query, rowData );

	rowData.Free();	
	rowData.Reset();
	rowData.ReserveL( 3 ); // space for SELECTs

	TMdCObject object;
    rowData.AppendL( TColumn( object.iId ) );
    rowData.AppendL( TColumn( object.iDefId ) );
    rowData.AppendL( TColumn( object.iFlags ) );

    CMdCSerializationBuffer* buffer = NULL;
	if ( db.NextRowL( query, rowData ) )
		{
		rowData.Column(0).Get( object.iId );
		rowData.Column(1).Get( object.iDefId );
		rowData.Column(2).Get( object.iFlags );

		buffer = CMdCSerializationBuffer::NewLC( aResultBufferSize );
		object.SerializeL( *buffer );
		CleanupStack::Pop( buffer );
		}
	else
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // query, rowData, commonClauseOne

	return buffer;
	}

CMdCSerializationBuffer* CMdSSqlObjectManipulate::CheckObjectL( TInt aResultBufferSize, 
		TItemId aId, TDefId aNamespaceDefId )
	{
	_LIT( KMdsCheckObjectById, "SELECT ObjectDefId, Flags FROM Object%u WHERE ObjectId=?;" );

	RClauseBuffer commonClauseOne(*this, KMdsCheckObjectById.iTypeLength + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& checkObjectClause = commonClauseOne.BufferL();

	checkObjectClause.BufferL().Format( KMdsCheckObjectById, aNamespaceDefId );
	
    RRowData rowData;
    CleanupClosePushL( rowData );
    rowData.AppendL( TColumn( aId ) );

    RMdsStatement query;
	CleanupClosePushL( query );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	connection.ExecuteQueryL( checkObjectClause.ConstBufferL(), query, rowData );

	rowData.Free();	rowData.Reset();
	rowData.ReserveL( 2 ); // space for SELECTs

	TMdCObject object;
	object.iId = aId;
    rowData.AppendL( TColumn( object.iDefId ) );
    rowData.AppendL( TColumn( object.iFlags ) );

    CMdCSerializationBuffer* buffer = NULL;
	if ( connection.NextRowL( query, rowData ) )
		{
		rowData.Column(0).Get( object.iDefId );
		rowData.Column(1).Get( object.iFlags );

		buffer = CMdCSerializationBuffer::NewLC( aResultBufferSize );
		object.SerializeL( *buffer );
		CleanupStack::Pop( buffer );
		}
	else
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // query, rowData, commonClauseOne

	return buffer;
	}

CMdCSerializationBuffer* CMdSSqlObjectManipulate::CheckObjectL( TInt aResultBufferSize, 
		CMdCSerializationBuffer& aIds, TDefId aNamespaceDefId )
	{

	aIds.PositionL( KNoOffset );
	TUint32 idCount = 0;
	aIds.ReceiveL( idCount );

	// if no IDs, just return 0 objects
	if( idCount == 0 )
		{
		CMdCSerializationBuffer* buffer = 
			CMdCSerializationBuffer::NewLC( CMdCSerializationBuffer::KRequiredSizeForTUint32 );
		buffer->InsertL( idCount );
		return buffer;
		}

	_LIT( KMdsCheckObjectByIds, "SELECT ObjectId, ObjectDefId, Flags FROM Object%u WHERE ObjectId IN(?" );
	_LIT( KMdsCheckObjectByIdsAppend, ",?" );
	_LIT( KMdsCheckObjectByIdsEnd, ");" );

	RClauseBuffer commonClauseOne(*this, KMdsCheckObjectByIds.iTypeLength + KMaxUintValueLength + 
										 KMdsCheckObjectByIdsAppend.iTypeLength * (idCount - 1) + 
										 KMdsCheckObjectByIdsEnd.iTypeLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& checkObjectClause = commonClauseOne.BufferL();
	
	RRowData rowData;
	CleanupClosePushL( rowData );

	checkObjectClause.BufferL().Format( KMdsCheckObjectByIds, aNamespaceDefId );
	TItemId id;
	aIds.ReceiveL( id );
	rowData.AppendL( TColumn( id ) );
	
	for( TUint32 i = 1; i < idCount; i++ )
		{
		checkObjectClause.BufferL().Append( KMdsCheckObjectByIdsAppend );
		aIds.ReceiveL( id );
		rowData.AppendL( TColumn( id ) );
		}

	checkObjectClause.BufferL().Append( KMdsCheckObjectByIdsEnd );

	RMdsStatement query;
	CleanupClosePushL( query );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	connection.ExecuteQueryL( checkObjectClause.ConstBufferL(), query, rowData );

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aResultBufferSize );
	
	// initialize with 0 objects (will be updated later)
	TUint32 resultObjectCount = 0;
	buffer->InsertL( resultObjectCount );

	rowData.Free();
	rowData.Reset();

	rowData.ReserveL( 3 ); // space for SELECT's object
	
	TMdCObject object;
	rowData.AppendL( TColumn( object.iId ) );
	rowData.AppendL( TColumn( object.iDefId ) );
	rowData.AppendL( TColumn( object.iFlags ) );

	while( connection.NextRowL( query, rowData ) )
		{
		rowData.Column(0).Get( object.iId );
		rowData.Column(1).Get( object.iDefId );
		rowData.Column(2).Get( object.iFlags );

		object.SerializeL( *buffer );
		++resultObjectCount;
		}

	// update object count
	buffer->PositionL( KNoOffset );
	buffer->InsertL( resultObjectCount );

	CleanupStack::Pop( buffer );
	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // query, rowData, commonClauseOne
	
	return buffer;
	}

static TInt CompareTItemIds( const TItemId& aLeft, const TItemId& aRight )
	{
	return aLeft - aRight;
	}

void CMdSSqlObjectManipulate::CollectRemovedItemsL( RArray<TItemId>& aRemoveIds, RArray<TItemId>& aObjectIds,
                                                    RArray<TItemId>& aRelationIds, RArray<TItemId>& /*aEventIds*/ )
	{
    _LIT( KCollectGetDeleteId1, "SELECT O.ObjectId, R.RelationId FROM Object%u AS O LEFT JOIN Relations%u AS R ON O.ObjectId=R.LeftObjectId WHERE NOT O.Flags&? AND ObjectId IN(?");
    _LIT( KCollectGetDeleteId2, " UNION SELECT O.ObjectId, R.RelationId FROM Object%u AS O LEFT JOIN Relations%u AS R ON O.ObjectId=R.RightObjectId WHERE NOT O.Flags&? AND ObjectId IN(?");

	_LIT( KCollectUpdateObjectBegin,    "UPDATE Object%u SET Flags=Flags|? WHERE ObjectId IN(?" );
	_LIT( KCollectUpdateRelationsBegin, "UPDATE Relations%u SET Flags=Flags|? WHERE RelationId IN(?" );
	_LIT( KCollectMiddle, ",?" );
	_LIT( KCollectEnd1,    ")" );
	_LIT( KCollectEnd2,    ");" );

	const TInt removeIdsCount = aRemoveIds.Count();
	if (removeIdsCount < 1)
		{
		return;
		}

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	
	const TInt clauseSize( KCollectGetDeleteId1().Length() + 2*KMaxUintValueLength +
                                      (removeIdsCount-1) * KCollectMiddle().Length() +
                                      KCollectEnd1().Length() +
                                      KCollectGetDeleteId2().Length() + KMaxUintValueLength +
                                      (removeIdsCount-1) * KCollectMiddle().Length() +
                                      KCollectEnd2().Length() );
	
	HBufC* clause = HBufC::NewLC( clauseSize );
	
	TPtr clauseBuffer( clause->Des() );
	clauseBuffer.Append( KCollectGetDeleteId1 );	
	for( TInt i = removeIdsCount - 2; i >=0; i-- )
        {
	    clauseBuffer.Append( KCollectMiddle );
        }
	clauseBuffer.Append( KCollectEnd1 );	
    
	clauseBuffer.Append( KCollectGetDeleteId2 );
    for( TInt i = removeIdsCount - 2; i >=0; i-- )
        {
        clauseBuffer.Append( KCollectMiddle );
        }
    clauseBuffer.Append( KCollectEnd2 );  
	
    RClauseBuffer commonClauseOne(*this,  clauseSize );
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buffer = commonClauseOne.BufferL();

	// getting removed object id and relation id
	buffer.BufferL().Format( clauseBuffer, 
	                                    iNamespaceDef->GetId(), 
	                                    iNamespaceDef->GetId(),
	                                    iNamespaceDef->GetId(), 
	                                    iNamespaceDef->GetId() );

	RRowData dataRow;
	CleanupClosePushL( dataRow );
	dataRow.ReserveL( removeIdsCount*2 + 2 );
	dataRow.AppendL( TColumn( EMdEObjectFlagRemoved ) );
	
    for( TInt i = removeIdsCount - 1; i >=0; i-- )
        {
        dataRow.AppendL( TColumn( aRemoveIds[i] ) );
        }

    dataRow.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    
    for( TInt i = removeIdsCount - 1; i >=0; i-- )
        {
        dataRow.AppendL( TColumn( aRemoveIds[i] ) );
        }

	RMdsStatement objectQuery;
	CleanupClosePushL( objectQuery );

	__LOGQUERY_16( _L("Find objects to delete:"), buffer.ConstBufferL(), dataRow);
	connection.ExecuteQueryL( buffer.ConstBufferL(), objectQuery, dataRow );

	dataRow.Free();	
	dataRow.Reset();
	TItemId objectId = KNoId;
    TItemId prevId = objectId;
    TItemId relationId = KNoId;
	dataRow.AppendL( TColumn( objectId ) );
	dataRow.AppendL( TColumn( relationId ) );
	while ( connection.NextRowL( objectQuery, dataRow ) )
		{
		dataRow.Column(0).Get( objectId );
		if (objectId != prevId)
			{
			aObjectIds.AppendL( objectId );
			prevId = objectId;
			}
		if (!dataRow.Column(1).IsNull())
			{
			dataRow.Column(1).Get( relationId );
			const TInt error( aRelationIds.InsertInOrder( relationId, TLinearOrder<TItemId>( CompareTItemIds ) ) );
			if( error == KErrNoMemory )
			    {
			    User::Leave( error );
			    }
			}
		else
			{
			dataRow.Column(1).Set( relationId );
			}
		}

	// mark object's as removed
	// remove only item that are OK to remove (not removed earlier)
	const TInt removeObjectCount = aObjectIds.Count();
	if ( removeObjectCount > 0 )
		{
		buffer.ReserveSpaceL( KCollectUpdateObjectBegin().Length() + KMaxUintValueLength +
							   (removeObjectCount-1) * KCollectMiddle().Length() +
							   KCollectEnd2().Length() );

		buffer.BufferL().Format( KCollectUpdateObjectBegin, iNamespaceDef->GetId() );

		dataRow.Free();
		dataRow.Reset();
		dataRow.AppendL( TColumn( EMdEObjectFlagRemoved ) );

		for (TInt i = 0; i < removeObjectCount; ++i)
			{
			if(i>0)
				{
				buffer.AppendL( KCollectMiddle );
				}
			dataRow.AppendL( TColumn( aObjectIds[i] ) );
			}
		buffer.AppendL( KCollectEnd2 );

		__LOGQUERY_16( _L("Remove objects:"), buffer.ConstBufferL(), dataRow);
		connection.ExecuteL( buffer.ConstBufferL(), dataRow );
		}

	// mark relations as removed
	// remove only item that are OK to remove (not removed earlier)
	const TInt removeRelationCount = aRelationIds.Count();
	if ( removeRelationCount > 0 )
		{
		buffer.ReserveSpaceL( KCollectUpdateRelationsBegin().Length() + KMaxUintValueLength +
							   (removeRelationCount-1) * KCollectMiddle().Length() +
							   KCollectEnd2().Length() );

		buffer.BufferL().Format( KCollectUpdateRelationsBegin, iNamespaceDef->GetId() );

		dataRow.Free();	dataRow.Reset();
		dataRow.ReserveL( 1 + removeRelationCount );
		dataRow.AppendL( TColumn( EMdERelationFlagDeleted ) );

		for ( TInt i = 0; i < removeRelationCount; ++i )
			{
			if( i > 0 )
				{
				buffer.AppendL( KCollectMiddle );
				}
			dataRow.AppendL( TColumn( aRelationIds[i] ) );
			}
		buffer.AppendL( KCollectEnd2 );

		__LOGQUERY_16( _L("Remove relations:"), buffer.ConstBufferL(), dataRow);
		connection.ExecuteL( buffer.ConstBufferL(), dataRow );
		}


	CleanupStack::PopAndDestroy( 4, clause ); // objectQuery, dataRow, commonClauseOne, clause
	}

void CMdSSqlObjectManipulate::RemoveObjectsByIdL( 
		CMdCSerializationBuffer& aBuffer, TInt aCount, RArray<TItemId>& aIdArray, 
		RArray<TItemId>& aRelationIds, RArray<TItemId>& aEventIds,
		TBool aUrisRequired, RPointerArray<HBufC>& aRemovedItemUriArray )
	{
	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RArray<TItemId> objectIds;
	CleanupClosePushL( objectIds );

	TItemId objectId = KNoId;

	objectIds.ReserveL( aCount );
	for (TUint32 i = 0; i < aCount; ++i)
		{
		aBuffer.ReceiveL( objectId );
		if ( objectId != KNoId && 
		     objectId != KSystemFavouritesAlbumId && 
		     objectId != KSystemCapturedAlbumId )
			{
			if ( iLockList.IsLocked( *iNamespaceDef, objectId ) )
				{
				iLockList.UnlockById( *iNamespaceDef, objectId );
				}

			objectIds.AppendL( objectId );
			}
		}
    
    CollectRemovedItemsL( objectIds, aIdArray, aRelationIds, aEventIds );

    if( aUrisRequired )
         {
         const TInt count( aIdArray.Count() );
         for( TInt i( 0 ); i < count; i++ )
             {
             TUint32 flags;
             SearchObjectUriByIdL( aIdArray[i], flags );
             aRemovedItemUriArray.AppendL( iUri );
             iUri = NULL;
             }
         }
    
    CleanupStack::PopAndDestroy( &objectIds );
	}

void CMdSSqlObjectManipulate::RemoveObjectsByUriL( 
		CMdCSerializationBuffer& aBuffer, TInt aCount, RArray<TItemId>& aIdArray,
        RArray<TItemId>& aRelationIds, RArray<TItemId>& aEventIds, 
        TBool aUrisRequired, RPointerArray<HBufC>& aRemovedItemUriArray )
	{
	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RArray<TItemId> objectIds;
	CleanupClosePushL( objectIds );

	TUint32 flags;
	TItemId objectId = KNoId;
	objectIds.ReserveL( aCount );
	for (TUint32 i = 0; i < aCount; ++i)
		{
		TPtrC16 uri = aBuffer.ReceivePtr16L();
        objectId = SearchObjectByUriL( uri, flags );
        if ( objectId != KNoId && 
             uri != KSystemFavouritesAlbumUri && 
             uri != KSystemCapturedAlbumUri )
			{
			// unlock object, so update is no possible anymore
			if ( iLockList.IsLocked( *iNamespaceDef, objectId ) )
				{
				iLockList.UnlockById( *iNamespaceDef, objectId );
				}
			
			objectIds.AppendL( objectId );
			
			if( aUrisRequired )
			    {
			    // Only objects have uris, so if the are removed by uri, it can be assumed
			    // that only objects are removed.
			    aRemovedItemUriArray.AppendL( uri.AllocL() );
			    }
			}
		}

	CollectRemovedItemsL( objectIds, aIdArray, aRelationIds, aEventIds );

    CleanupStack::PopAndDestroy( &objectIds );
	}

TItemId CMdSSqlObjectManipulate::SearchObjectByUriL( const TDesC16& aUri, 
		TUint32& aFlags )
	{
	_LIT( KMdsSearchObjectbyUri, "SELECT ObjectId,Flags FROM Object%u WHERE NOT Flags&? AND NOT Flags&? AND URI=? LIMIT 1;" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	RClauseBuffer commonClauseOne(*this, KMdsSearchObjectbyUri.iTypeLength + KMaxUintValueLength );
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& searchUriClause = commonClauseOne.BufferL();

	searchUriClause.BufferL().Format( KMdsSearchObjectbyUri, iNamespaceDef->GetId() );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	TItemId objectId = KNoId;
	aFlags = 0;

    RRowData varSearch;
    CleanupClosePushL( varSearch );
    varSearch.ReserveL( 3 );
    varSearch.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
    varSearch.AppendL( TColumn( EMdEObjectFlagRemoved ) );
    varSearch.AppendL( TColumn( aUri ) );

	RMdsStatement getQuery;
    CleanupClosePushL( getQuery );

	__LOGQUERY_16( _L("Search object by URI:"), 
			searchUriClause.ConstBufferL(), varSearch);
	TRAPD( err, connection.ExecuteQueryL( 
			searchUriClause.ConstBufferL(), getQuery, varSearch ) );

	varSearch.Free();
	varSearch.Reset();
	varSearch.AppendL( TColumn( objectId ) );
	varSearch.AppendL( TColumn( aFlags ) );
	if ( err == KErrNone && connection.NextRowL( getQuery, varSearch ) )
		{
		varSearch.Column(0).Get( objectId );
		varSearch.Column(1).Get( aFlags );
		}

	CleanupStack::PopAndDestroy( 2, &varSearch ); // getQuery, varSearch
	CleanupStack::PopAndDestroy( &commonClauseOne );
	return objectId;
	}

HBufC*& CMdSSqlObjectManipulate::SearchObjectUriByIdL( const TItemId aId, 
        TUint32& aFlags )
    {
    _LIT( KMdsSearchObjectUribyId, "SELECT URI,Flags FROM Object%u WHERE NOT Flags&? AND ObjectId=? LIMIT 1;" );

    if ( !iNamespaceDef )
        {
        User::Leave( KErrMdEUnknownNamespaceDef );
        }

    RClauseBuffer commonClauseOne(*this, KMdsSearchObjectUribyId.iTypeLength + KMaxUintValueLength );
    CleanupClosePushL( commonClauseOne );
    CMdsClauseBuffer& searchUriClause = commonClauseOne.BufferL();

    searchUriClause.BufferL().Format( KMdsSearchObjectUribyId, iNamespaceDef->GetId() );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    TPtrC16 uri;
    aFlags = 0;

    RRowData varSearch;
    CleanupClosePushL( varSearch );
    varSearch.ReserveL( 2 );
    varSearch.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
    varSearch.AppendL( TColumn( aId ) );

    RMdsStatement getQuery;
    CleanupClosePushL( getQuery );

    __LOGQUERY_16( _L("Search object URI by ID:"), 
            searchUriClause.ConstBufferL(), varSearch);
    TRAPD( err, connection.ExecuteQueryL( 
            searchUriClause.ConstBufferL(), getQuery, varSearch ) );

    varSearch.Free();
    varSearch.Reset();
    varSearch.AppendL( TColumn( uri ) );
    varSearch.AppendL( TColumn( aFlags ) );
    if ( err == KErrNone && connection.NextRowL( getQuery, varSearch ) )
        {
        varSearch.Column(0).Get( uri );
        varSearch.Column(1).Get( aFlags );
        iUri = uri.AllocL();
        }

    CleanupStack::PopAndDestroy( 2, &varSearch ); // getQuery, varSearch
    CleanupStack::PopAndDestroy( &commonClauseOne );
    
    return iUri;
    }

TItemId CMdSSqlObjectManipulate::UpdateObjectL( 
		CMdSSqLiteConnection& aConnection, CMdCSerializationBuffer& aBuffer )
	{
	_LIT( KMdsObjectUpdateBaseObjectBegin,     "UPDATE Object%u SET " );
	_LIT( KMdsObjectUpdateBaseObjectFlags,     "Flags=? " );
	_LIT( KMdsObjectUpdateBaseObjectEnd,       ",MediaId=?,GuidHigh=?,GuidLow=?,URI=? ");
	_LIT( KMdsObjectUpdateObjectBegin,         "UPDATE %S%u SET " );
	_LIT( KMdsObjectUpdateEnd,                 " WHERE ObjectId=?;" );
	_LIT( KUpdateEqual, "=?" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	const TMdCObject& object = TMdCObject::GetFromBufferL( aBuffer );

	// objectid
	if (object.iId == KNoId)
		{
		User::Leave( KErrArgument );
		}

	// object must be locked
	if (!iLockList.IsLocked(*iNamespaceDef, object.iId))
		{
		User::Leave( KErrMdENotLocked );
		}

	// objectdefid
	// objectDef exists ??
	const CMdsObjectDef* objectDef = iNamespaceDef->GetObjectByIdL( 
			object.iDefId );
	if ( !objectDef )
		{
		User::Leave( KErrMdEUnknownObjectDef );
		}

	// get BaseObjectDef
	iBaseObjectDef = iNamespaceDef->GetObjectByIdL( KBaseObjectDefId );

	TUint32 objectFlags = 0;
	if (object.iFlags & EMdEObjectFlagConfidential)
		{
		objectFlags |= EMdEObjectFlagConfidential;
		}
	if (object.iFlags & EMdEObjectFlagModFreeText)
		{
		objectFlags |= EMdEObjectFlagFreetexts;
		}
	if (object.iFlags & EMdEObjectFlagPlaceholder)
		{
		objectFlags |= EMdEObjectFlagPlaceholder;
		}
	if (objectDef->GetFlags() == CMdsObjectDef::EObjectDefFlagsContext)
		{
		objectFlags |= EMdEObjectFlagContext;
		}

	if (!(object.iFlags & EMdEObjectFlagModOpen))
		{
		User::Leave( KErrMdENotLocked );
		}

	const TBool KUpdateModObject   = 
		object.iFlags & EMdEObjectFlagModObject   ? ETrue : EFalse;
	const TBool KUpdateModFreeText = 
		object.iFlags & EMdEObjectFlagModFreeText ? ETrue : EFalse;
	const TBool KUpdateModProperty = 
		object.iFlags & EMdEObjectFlagModProperty ? ETrue : EFalse;

	if ( !( KUpdateModObject || KUpdateModFreeText || KUpdateModProperty ) )
		{
		// nothing to change, so unlock object and return
		iLockList.UnlockById( *iNamespaceDef, object.iId );
		return object.iId;
		}

	RRowData baseObjectRow;
	CleanupClosePushL( baseObjectRow );
	RRowData objectRow;
	CleanupClosePushL( objectRow );

	RClauseBuffer commonClauseOne(*this);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& clauseObject = commonClauseOne.BufferL();
	
	if (KUpdateModProperty)
		{
		clauseObject.BufferL().Format( KMdsObjectUpdateObjectBegin, 
				&objectDef->GetName(), iNamespaceDef->GetId() );
		}

	RClauseBuffer commonClauseTwo(*this);
	CleanupClosePushL( commonClauseTwo );
	CMdsClauseBuffer& clauseBaseObject = commonClauseTwo.BufferL();
	if (KUpdateModObject || KUpdateModProperty)
		{
		clauseBaseObject.BufferL().Format( KMdsObjectUpdateBaseObjectBegin, 
				iNamespaceDef->GetId() );
		}

	aBuffer.PositionL( object.iUri.iPtr.iOffset );
	HBufC* uriBuf = aBuffer.ReceiveDes16L();
	CleanupStack::PushL( uriBuf );
	TPtr16 uri( uriBuf->Des() );
	uri.LowerCase();

	// if auto locking, skip extra checks, because only flags can be changed
	if( !(object.iFlags & EMdEObjectFlagAutoLock) )
		{
		// leave if GUID high and low are 0
		if( object.iGuidHigh == 0 && object.iGuidLow == 0 )
			{
			User::Leave( KErrCorrupt );
			}

		// leave if URI is "empty"
		if( uri.Length() == 0 )
			{
			User::Leave( KErrCorrupt );
			}
		}

	TBool KBaseObjectPropertyMod = EFalse;
	TBool KObjectPropertyMod     = EFalse;

	if (KUpdateModObject)
		{
		clauseBaseObject.AppendL( KMdsObjectUpdateBaseObjectFlags );
		baseObjectRow.AppendL( TColumn( objectFlags ) );

		if( !(object.iFlags & EMdEObjectFlagAutoLock) )
			{
			clauseBaseObject.AppendL( KMdsObjectUpdateBaseObjectEnd );
			baseObjectRow.AppendL( TColumn( object.iMediaId ) );
			baseObjectRow.AppendL( TColumn( object.iGuidHigh ) );
			baseObjectRow.AppendL( TColumn( object.iGuidLow ) );
			baseObjectRow.AppendL( TColumn( uri ) );
			}
		KBaseObjectPropertyMod = ETrue;
		}

	// read properties array
	if ( KUpdateModProperty )
		{
		// try to add property and increase property count
		for ( TUint32 i = 0; i < object.iProperties.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( object.iProperties.iPtr.iOffset + 
					i * sizeof(TMdCProperty) );
			TUint8 modFlags;
			const CMdsPropertyDef& propertyDef = ReadPropertyL( aBuffer, 
					*objectDef, clauseBaseObject, clauseObject, baseObjectRow, 
					objectRow, modFlags );
			// check if property is already in array
			if ( modFlags == EMdEPropertyModNone )
				{
				continue;
				}

			if( ( modFlags & EMdEPropertyModRemove ) && propertyDef.GetMandatory() )
				{
				User::Leave( KErrMdEMandatoryPropertyMissing );
				}

			const TDefId propertyId = propertyDef.GetId();

			if ( iBaseObjectDef->GetPropertyByIdL( propertyId ) )
				{
				if (!KBaseObjectPropertyMod)
					{
					// remove comma before first property
					clauseBaseObject.BufferL().Delete( 
							clauseBaseObject.ConstBufferL().LocateReverse( ',' ), 1 );
					KBaseObjectPropertyMod = ETrue;
					}
				clauseBaseObject.AppendL( KUpdateEqual );
				}
			else
				{
				if (!KObjectPropertyMod)
					{
					// remove comma before first property
					clauseObject.BufferL().Delete( 
							clauseObject.ConstBufferL().LocateReverse( ',' ), 1 );
					KObjectPropertyMod = ETrue;
					}
				clauseObject.AppendL( KUpdateEqual );
				}
			}
		}

	if (KUpdateModObject || KBaseObjectPropertyMod)
		{
		clauseBaseObject.AppendL( KMdsObjectUpdateEnd );
		// append confidential and deleted flags
		baseObjectRow.AppendL( TColumn( object.iId ) );
		}

	if (KObjectPropertyMod)
		{
		clauseObject.AppendL( KMdsObjectUpdateEnd );
		// append confidential and deleted flags
		objectRow.AppendL( TColumn( object.iId ) );
		}

	TInt queryResult( 0 );
	TInt err( KErrNone );

	if ( KUpdateModObject || KBaseObjectPropertyMod )
		{
		__LOGQUERY_16( _L("Update BaseObject:"), 
				clauseBaseObject.ConstBufferL(), baseObjectRow);
		TRAP( err, queryResult = aConnection.ExecuteL( 
				clauseBaseObject.ConstBufferL(), baseObjectRow ) );

		// Try to remove the object which caused the constraint error and try add the object again.
		if ( err == KSqlErrConstraint )
			{
			__LOG2( ELogQuery, "Update baseObject constraint error - err:%d, queryResult:%d", err, queryResult );
			// The reason of the constraint error is not checked due to performance hit.
			TRAP_IGNORE( RemoveObjectForceL( uri, object.iId ) );
			__LOGQUERY_16( _L("Update AGAIN BaseObject:"), 
					clauseBaseObject.ConstBufferL(), baseObjectRow);
			// Fails if the object was not marked as removed.
			TRAP( err, queryResult = aConnection.ExecuteL( 
					clauseBaseObject.ConstBufferL(), baseObjectRow ) );
			}

		if ( err != KErrNone || queryResult != 1 )
			{
			__LOG2( ELogQuery, "Update baseObject failed - err:%d, queryResult:%d", err, queryResult );
			User::Leave( KErrGeneral );
			}
		}

	if ( KObjectPropertyMod )
		{
		__LOGQUERY_16( _L("Update Object:"), clauseObject.ConstBufferL(), 
				objectRow);
		TRAP( err, queryResult = aConnection.ExecuteL( 
				clauseObject.ConstBufferL(), objectRow ) );
		if (err != KErrNone || queryResult != 1)
			{
			__LOG2( ELogQuery, "Update Object failed - err:%d, queryResult:%d", err, queryResult );
			User::Leave( KErrGeneral );
			}
		}

	if ( KUpdateModFreeText )
		{
		if ( object.iFreeTexts.iPtr.iCount > 0 )
			{
			aBuffer.PositionL( object.iFreeTexts.iPtr.iOffset );
			UpdateFreeTextL( aBuffer, object.iFreeTexts.iPtr.iCount, object.iId );
			}
		}

    CleanupStack::PopAndDestroy( uriBuf );
    CleanupStack::PopAndDestroy( &commonClauseTwo );
	CleanupStack::PopAndDestroy( &commonClauseOne );

    // objectRow, baseObjectRow
	CleanupStack::PopAndDestroy( 2, &baseObjectRow );

	iLockList.UnlockById( *iNamespaceDef, object.iId );
	return object.iId;
	}


void CMdSSqlObjectManipulate::UpdateFreeTextL( CMdCSerializationBuffer& aBuffer,
		TInt aCount, TItemId aObjectId )
	{
	_LIT( KMdSUpdateFreeTextDelete,     "DELETE FROM TextSearch%u WHERE ObjectId=?;" );
	_LIT( KMdSUpdateFreeTextDictDelete, "DELETE FROM TextSearchDictionary%u WHERE WordId NOT IN(SELECT WordId FROM TextSearch%u);" );
	_LIT( KMdSUpdateFreeTextObjectFlagSet,   "UPDATE Object%u SET Flags=Flags|? WHERE ObjectId=?;" );
	_LIT( KMdSUpdateFreeTextObjectFlagReset, "UPDATE Object%u SET Flags=Flags&? WHERE ObjectId=?;" );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RClauseBuffer commonClauseOne(*this, KMdSUpdateFreeTextDictDelete.iTypeLength + 2*KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buffer = commonClauseOne.BufferL();
	RRowData delRow;
	CleanupClosePushL( delRow );

	delRow.AppendL( TColumn( aObjectId ) );
	buffer.BufferL().Format( KMdSUpdateFreeTextDelete, iNamespaceDef->GetId() );
	connection.ExecuteL( buffer.ConstBufferL(), delRow );

	delRow.Free(); delRow.Reset();
	buffer.BufferL().Format( KMdSUpdateFreeTextDictDelete, iNamespaceDef->GetId(), 
			iNamespaceDef->GetId() );
	connection.ExecuteL( buffer.ConstBufferL(), delRow );

	// update object flags
	delRow.Free(); delRow.Reset();
	delRow.AppendL( TColumn( EMdEObjectFlagFreetexts ) );
	delRow.AppendL( TColumn( aObjectId ) );
	if ( AddFreeTextL( aBuffer, aCount, aObjectId ) > 0 )
		{
		buffer.BufferL().Format( KMdSUpdateFreeTextObjectFlagSet, iNamespaceDef->GetId() );
		}
	else
		{
		buffer.BufferL().Format( KMdSUpdateFreeTextObjectFlagReset, iNamespaceDef->GetId() );
		delRow.Column(0).Set( ~EMdEObjectFlagFreetexts );
		}
	connection.ExecuteL( buffer.ConstBufferL(), delRow );

	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // delRow, commonClauseOne
	}

TItemId CMdSSqlObjectManipulate::AddEventL( CMdSSqLiteConnection& aConnection, 
		CMdCSerializationBuffer& aBuffer )
	{
	_LIT( KAddEvent, "INSERT INTO Event%u(EventId,ObjectId,EventDefId,Timestamp,Source,Participant) VALUES(?,?,?,?,?,?);" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	
	RClauseBuffer commonClauseOne(*this, KAddEvent().Length() + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& sqlBuf = commonClauseOne.BufferL();
	sqlBuf.BufferL().Format( KAddEvent, iNamespaceDef->GetId() );

   	RRowData var;
    CleanupClosePushL( var );

    const TMdCEvent& event = TMdCEvent::GetFromBufferL( aBuffer );

    TItemId eventId = event.iId;
	if ( eventId != KNoId )
		{
		User::Leave( KErrArgument );
		}
	
	RSQLIndex sqlIndex;
	CleanupClosePushL( sqlIndex );
	eventId = sqlIndex.GetId();

	if ( !iNamespaceDef->GetEventByIdL( event.iDefId ) )
		{
		User::Leave( KErrMdEUnknownEventDef );
		}

    if ( event.iObjectId == KNoId )
    	{
    	User::Leave( KErrCorrupt );
    	}

    var.AppendL( TColumn( eventId ) );
    var.AppendL( TColumn( event.iObjectId ) );
    var.AppendL( TColumn( event.iDefId ) );
    var.AppendL( TColumn( event.iTime.Int64() ) );

    TPtrC16 source;
    if ( event.iSourceText.iPtr.iCount > 0 )
    	{
    	aBuffer.PositionL( event.iSourceText.iPtr.iOffset );
        source.Set( aBuffer.ReceivePtr16L() );
    	}
    else
    	{
    	source.Set( TPtr16((TUint16*)0, 0) );
    	}
    var.AppendL( TColumn( source ) );

	TPtrC16 participant;
    if ( event.iParticipantText.iPtr.iCount > 0 )
    	{
    	aBuffer.PositionL( event.iParticipantText.iPtr.iOffset );
    	participant.Set( aBuffer.ReceivePtr16L() );
    	}
    else
    	{
    	participant.Set( TPtr16((TUint16*)0, 0) );
    	}
    var.AppendL( TColumn( participant ) );

    aConnection.ExecuteL( sqlBuf.ConstBufferL(), var );

	sqlIndex.Commit();
	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // sqlIndex, var, commonClauseOne
	return eventId;
	}
        
TItemId CMdSSqlObjectManipulate::AddRelationL( 
		CMdSSqLiteConnection& aConnection, CMdCSerializationBuffer& aBuffer )
	{
	_LIT( KAddRelation, "INSERT INTO Relations%u(RelationId,Flags,RelationDefId,LeftObjectId,RightObjectId,Parameter,GuidHigh,GuidLow,LastModifiedDate) VALUES(?,?,?,?,?,?,?,?,?);" );

	if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	
	RClauseBuffer commonClauseOne(*this, KAddRelation().Length() + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buffer = commonClauseOne.BufferL();
	buffer.BufferL().Format( KAddRelation, iNamespaceDef->GetId() );

    RRowData var;
	CleanupClosePushL( var );

	const TMdCRelation& relation = TMdCRelation::GetFromBufferL( aBuffer );
	TItemId relationId = relation.iId;

	if ( relationId != KNoId )
		{
		User::Leave( KErrArgument );
		}
	
	RSQLIndex sqlIndex;
	CleanupClosePushL( sqlIndex );
	relationId = sqlIndex.GetId();

	if ( !iNamespaceDef->GetRelationByIdL( relation.iDefId ) )
		{
		User::Leave( KErrMdEUnknownRelationDef );
		}

    if ( relation.iLeftObjectId == KNoId || relation.iRightObjectId == KNoId )
    	{
    	User::Leave( KErrCorrupt );
    	}

	TInt64 guidHigh = relation.iGuidHigh;
	TInt64 guidLow = relation.iGuidLow;
	if ( guidHigh == 0 && guidLow == 0 )
		{
		iGenerator->GenerateGuid( guidHigh, guidLow );
		}

	// if last modified date is 0, set it to current universal time
	TInt64 lastModifiedDate = relation.iLastModifiedDate.Int64();
	if( lastModifiedDate == 0 )
		{
		TTime currentTime;
		currentTime.UniversalTime();
		lastModifiedDate = currentTime.Int64();
		}

    var.AppendL( TColumn( relationId ) );
    var.AppendL( TColumn( TUint32(0) ) ); // no flags
    var.AppendL( TColumn( relation.iDefId) );
    var.AppendL( TColumn( relation.iLeftObjectId ) );
    var.AppendL( TColumn( relation.iRightObjectId ) );
    var.AppendL( TColumn( relation.iParameter ) );
    var.AppendL( TColumn( guidHigh ) );
    var.AppendL( TColumn( guidLow ) );
    var.AppendL( TColumn( lastModifiedDate ) );

    aConnection.ExecuteL( buffer.ConstBufferL(), var );

	sqlIndex.Commit();
	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // sqlIndex, var, commonClauseOne
	return relationId;
	}

void CMdSSqlObjectManipulate::RemoveRelationsL( CMdCSerializationBuffer& aBuffer,
		TInt aCount, RArray<TItemId>& aIdArray )
    {	
	_LIT( KRemoveRelation, "UPDATE Relations%u SET Flags=Flags|? WHERE RelationId=? AND NOT Flags&?;" );
    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();
    
	RClauseBuffer commonClauseOne(*this, KRemoveRelation().Length() + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buf = commonClauseOne.BufferL();
    buf.BufferL().Format( KRemoveRelation, iNamespaceDef->GetId() );

	TInt queryResult = 0;
	TItemId relationId = KNoId;

    RRowData varRemove;
    CleanupClosePushL( varRemove );
    varRemove.ReserveL( 3 );
    varRemove.AppendL( TColumn( 
    		EMdERelationFlagDeleted | EMdERelationFlagGarbageDeleted ) );
    varRemove.AppendL( TColumn( relationId ) );
    varRemove.AppendL( TColumn( 
    		EMdERelationFlagDeleted | EMdERelationFlagNotPresent ) );

	for ( TUint32 i = 0; i < aCount; ++i )
		{
		aBuffer.ReceiveL( relationId );
		varRemove.Column( 1 ).Set( relationId );
		TRAPD( err, queryResult = db.ExecuteL( buf.ConstBufferL(), varRemove ) );
		if ( err == KErrNone && queryResult == 1 )
			{
			aIdArray.AppendL( relationId );
			}
		else
			{
			aIdArray.AppendL( KNoId );
			}
		}
	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // varRemove, commonClauseOne
    }

void CMdSSqlObjectManipulate::RemoveEventsL( CMdCSerializationBuffer& aBuffer, 
		TInt aCount, RArray<TItemId>& aIdArray )
    {
	_LIT( KRemoveEvent, "DELETE FROM Event%u WHERE EventId=?;" );

    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();

	RClauseBuffer commonClauseOne(*this, KRemoveEvent().Length() + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buf = commonClauseOne.BufferL();
    buf.BufferL().Format( KRemoveEvent, iNamespaceDef->GetId() );

	TInt queryResult = 0;
	TItemId eventId = KNoId;

    RRowData varRemove;
    CleanupClosePushL( varRemove );
    varRemove.AppendL( TColumn( eventId ) );

	for ( TUint32 i = 0; i < aCount; ++i )
		{
		aBuffer.ReceiveL( eventId );
		varRemove.Column( 0 ).Set( eventId );
		TRAPD( err, queryResult = db.ExecuteL( buf.ConstBufferL(), varRemove ) );
		if ( err == KErrNone && queryResult == 1 )
			{
			aIdArray.AppendL( eventId );
			}
		else
			{
			aIdArray.AppendL( KNoId );
			}
		}
	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // varRemove, commonClauseOne
    }

TItemId CMdSSqlObjectManipulate::UpdateRelationsL(
		CMdSSqLiteConnection& aConnection, CMdCSerializationBuffer& aBuffer )
    {	
	_LIT( KUpdateRelation, "UPDATE Relations%u SET RelationDefId=?,LeftObjectId=?,RightObjectId=?,Parameter=?,GuidHigh=?,GuidLow=?,LastModifiedDate=? WHERE NOT Flags&? AND NOT Flags&? AND RelationId=?;" );

if ( !iNamespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	
	RClauseBuffer commonClauseOne(*this, KUpdateRelation().Length() + KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& sqlBuf = commonClauseOne.BufferL();
    sqlBuf.BufferL().Format( KUpdateRelation, iNamespaceDef->GetId() );

    RRowData var;
    CleanupClosePushL( var );

    const TMdCRelation& relation = TMdCRelation::GetFromBufferL( aBuffer );

    if ( relation.iId == KNoId )
    	{
    	User::Leave( KErrArgument );
    	}

    if ( !iNamespaceDef->GetRelationByIdL( relation.iDefId ) )
		{
		User::Leave( KErrMdEUnknownRelationDef );
		}

	// leave if GUID high and low are 0
	if( relation.iGuidHigh == 0 && relation.iGuidLow == 0 )
		{
		User::Leave( KErrCorrupt );
		}

	// if last modified date is 0, set it to current universal time
	TInt64 lastModifiedDate = relation.iLastModifiedDate.Int64();
	if( lastModifiedDate == 0 )
		{
		TTime currentTime;
		currentTime.UniversalTime();
		lastModifiedDate = currentTime.Int64();
		}

    var.AppendL( TColumn( relation.iDefId ) );
    var.AppendL( TColumn( relation.iLeftObjectId ) );
    var.AppendL( TColumn( relation.iRightObjectId ) );
    var.AppendL( TColumn( relation.iParameter ) );
    var.AppendL( TColumn( relation.iGuidHigh ) );
    var.AppendL( TColumn( relation.iGuidLow ) );
    var.AppendL( TColumn( lastModifiedDate ) );
    var.AppendL( TColumn( EMdERelationFlagNotPresent ) );
    var.AppendL( TColumn( EMdERelationFlagDeleted ) );
	var.AppendL( TColumn( relation.iId ) );

	aConnection.ExecuteL( sqlBuf.ConstBufferL(), var );

	CleanupStack::PopAndDestroy( 2, &commonClauseOne ); // var, commonClauseOne

	return relation.iId;
    }

void CMdSSqlObjectManipulate::GetRemovedRelationItemsL( CMdCSerializationBuffer& aBuffer,
		const RArray<TItemId>& aRemovedRelations,
		const RArray<TItemId>& aAdditionalRemovedRelations )
	{
	_LIT( KGetRelationItemsStart, "SELECT RelationId,RelationDefId,LeftObjectId,RightObjectId FROM Relations%u WHERE Flags&? AND RelationId IN(?" );
	_LIT( KGetRelationItemsMiddle, ",?" );
	_LIT( KGetRelationItemsEnd, ");" );

	const TInt relationsCount = aRemovedRelations.Count() + aAdditionalRemovedRelations.Count();
	
	TMdCItems relationItems;
	
	relationItems.iNamespaceDefId = iNamespaceDef->GetId();
	relationItems.iRelations.iPtr.iCount = 0; // will be updated later
	relationItems.iRelations.iPtr.iOffset = sizeof( TMdCItems );

	if ( relationsCount == 0 )
		{
		relationItems.SerializeL( aBuffer );

		return;
		}
	
	RClauseBuffer commonClauseOne(*this, KGetRelationItemsStart().Length() + KMaxUintValueLength
			+ KGetRelationItemsMiddle().Length() * relationsCount 
			+ KGetRelationItemsEnd().Length() );
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& sqlBuffer = commonClauseOne.BufferL();
	sqlBuffer.BufferL().Format( KGetRelationItemsStart, iNamespaceDef->GetId() );

    RRowData var;
    CleanupClosePushL( var );
    var.ReserveL( 1 + relationsCount );
    var.AppendL( EMdERelationFlagDeleted );

    const TInt removedRelationsCount = aRemovedRelations.Count();
    for (TInt i = 0; i < removedRelationsCount-1; ++i)
    	{
    	sqlBuffer.AppendL( KGetRelationItemsMiddle );
    	var.AppendL( aRemovedRelations[i] );
    	}
    if ( removedRelationsCount )
    	{
	    var.AppendL( aRemovedRelations[removedRelationsCount-1] );
    	}

    const TInt additionalRemovedRelationsCount = aAdditionalRemovedRelations.Count();
    for (TInt i = 0; i < additionalRemovedRelationsCount-1; ++i)
    	{
    	sqlBuffer.AppendL( KGetRelationItemsMiddle );
    	var.AppendL( aAdditionalRemovedRelations[i] );
    	}
    if ( removedRelationsCount == 0 /*&& additionalRemovedRelationsCount*/ )
    	{
    	// cannot be empty
    	var.AppendL( aAdditionalRemovedRelations[additionalRemovedRelationsCount-1] );
    	}
    
    sqlBuffer.AppendL( KGetRelationItemsEnd );
    
	RMdsStatement getQuery;
    CleanupClosePushL( getQuery );

	__LOGQUERY_16( _L("CMdSSqlObjectManipulate::GetRemovedRelationItemsL:"), 
			sqlBuffer.ConstBufferL(), var);
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	
    connection.ExecuteQueryL( sqlBuffer.ConstBufferL(), getQuery, var );

	var.Free(); 
	var.Reset();

    var.ReserveL( 4 );
    
    TMdCRelation relation;
	var.AppendL( relation.iId );
	var.AppendL( relation.iDefId );
	var.AppendL( relation.iLeftObjectId );
	var.AppendL( relation.iRightObjectId );
	
    TUint32 resultCount = 0;
    
    // move position after items
	aBuffer.PositionL( relationItems.iRelations.iPtr.iOffset );

	while( connection.NextRowL( getQuery, var ) )
		{
		++resultCount;

		var.Column( 0 ).Get( relation.iId );
		var.Column( 1 ).Get( relation.iDefId );
		var.Column( 2 ).Get( relation.iLeftObjectId );
		var.Column( 3 ).Get( relation.iRightObjectId );

		relation.SerializeL( aBuffer );
		}

	// update relation count
	relationItems.iRelations.iPtr.iCount = resultCount;

	// move position to begin of buffer and serialize items
	aBuffer.PositionL( KNoOffset );
	relationItems.SerializeL( aBuffer );

	CleanupStack::PopAndDestroy( 3, &commonClauseOne ); // getQuery, var, commonClauseOne
	}

void CMdSSqlObjectManipulate::SetPendingL(const RArray<TItemId>& aObjectIds, 
		TBool aReset)
	{
	_LIT( KUpdatePending,   "UPDATE Object%u SET Flags=Flags%c? WHERE ObjectId IN(?" );
	_LIT( KUpdatePendingMiddle, ",?" );
	_LIT( KUpdatePendingEnd, ");" );

    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();

    const TInt objectIdsCount = aObjectIds.Count();

	RClauseBuffer commonClause(*this, KUpdatePending().Length() + KMaxUintValueLength + 
			KUpdatePendingMiddle().Length() * (objectIdsCount - 1) + 
			KUpdatePendingEnd().Length() );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& buf = commonClause.BufferL();

	const TChar KSetUpdateFlag = '|';
	const TChar KResetUpdateFlag = '&';
	
	TChar flagUpdate = KSetUpdateFlag; // set flag
	TUint32 objectFlag = EMdeObjectFlagPending;

	if( aReset )
		{
		flagUpdate = KResetUpdateFlag; // reset flag
		objectFlag = ~objectFlag; // not pending flag
		}
	
    buf.BufferL().Format( KUpdatePending, KDefaultNamespaceDefId, (TUint)flagUpdate );

    RRowData var;
    CleanupClosePushL( var );

    var.ReserveL( 1 + objectIdsCount );
    
    var.AppendL( TColumn( objectFlag ) );

	var.AppendL( aObjectIds[0] );
    for (TInt i = 1; i < objectIdsCount; ++i)
    	{
    	buf.AppendL( KUpdatePendingMiddle );
    	var.AppendL( aObjectIds[i] );
    	}
    buf.AppendL( KUpdatePendingEnd );

    db.ExecuteL( buf.ConstBufferL(), var );

	CleanupStack::PopAndDestroy( 2, &commonClause ); // var, commonClause
	}

TInt CMdSSqlObjectManipulate::GetPendingCountL(TDefId aObjectDefId)
	{
	_LIT( KGetPendingCount,   "SELECT count(*) FROM Object%u WHERE Flags&? AND NOT Flags&?" );
	_LIT( KGetPendingCountObjectDefId, " AND ObjectDefId=?" );
	_LIT( KGetPendingCountEnd, ";" );

    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();

    TInt bufferSize = KGetPendingCount().Length() + KMaxUintValueLength + 
    		KGetPendingCountEnd().Length();
    
	if( aObjectDefId != KNoDefId )	
		{
		bufferSize += KGetPendingCountObjectDefId().Length();
		}
    
	RClauseBuffer commonClause( *this, bufferSize );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& buf = commonClause.BufferL();
    buf.BufferL().Format( KGetPendingCount, KDefaultNamespaceDefId );
    
    TInt varReservation = 2;
    
	if( aObjectDefId != KNoDefId )	
		{
		buf.AppendL( KGetPendingCountObjectDefId );
		++varReservation;
		}

	buf.AppendL( KGetPendingCountEnd );
	
    RRowData var;
    CleanupClosePushL( var );

    var.ReserveL( varReservation );

    var.AppendL( TColumn( EMdeObjectFlagPending ) );
    var.AppendL( TColumn( EMdEObjectFlagNotPresent | EMdEObjectFlagRemoved | 
    		EMdEObjectFlagPlaceholder | EMdEObjectFlagStartUpNotPresent ) );

    if( aObjectDefId != KNoDefId )
    	{
    	var.AppendL( TColumn( aObjectDefId ) );
    	}
    
	RMdsStatement getQuery;
    CleanupClosePushL( getQuery );

	db.ExecuteQueryL( buf.ConstBufferL(), getQuery, var );

	var.Free();
	var.Reset();

	TUint32 count = 0;
	
	var.AppendL( TColumn( count ) );

	if ( db.NextRowL( getQuery, var ) )
		{
		var.Column(0).Get( count );
		}

	CleanupStack::PopAndDestroy( 2, &var ); // getQuery, varSearch
	CleanupStack::PopAndDestroy( &commonClause );
	
	return count;
	}

TInt CMdSSqlObjectManipulate::GetPendingL(TDefId aObjectDefId, 
		TInt aBufferSize, RArray<TItemId>& aObjectIds)
	{
	_LIT( KGetPending,   "SELECT ObjectId FROM Object%u WHERE Flags&? AND NOT Flags&?" );
	_LIT( KGetPendingObjectDefId, " AND ObjectDefId=?" );
	_LIT( KGetPendingEnd, " LIMIT ?;" );

    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();

    TInt bufferSize = KGetPending().Length() + KMaxUintValueLength + 
    		KGetPendingEnd().Length();

	if( aObjectDefId != KNoDefId )	
		{
		bufferSize += KGetPendingObjectDefId().Length();
		}

	RClauseBuffer commonClause( *this, bufferSize );
	CleanupClosePushL( commonClause );
	CMdsClauseBuffer& buf = commonClause.BufferL();
    buf.BufferL().Format( KGetPending, KDefaultNamespaceDefId );

    TInt varReservation = 3;

	if( aObjectDefId != KNoDefId )	
		{
		buf.AppendL( KGetPendingObjectDefId );
		++varReservation;
		}

	buf.AppendL( KGetPendingEnd );

    RRowData var;
    CleanupClosePushL( var );

    var.ReserveL( varReservation );

    var.AppendL( TColumn( EMdeObjectFlagPending ) );
    var.AppendL( TColumn( EMdEObjectFlagNotPresent | EMdEObjectFlagRemoved | 
    		EMdEObjectFlagPlaceholder | EMdEObjectFlagStartUpNotPresent ) );

    if( aObjectDefId != KNoDefId )
    	{
    	var.AppendL( TColumn( aObjectDefId ) );
    	}

    // get as much as possible to buffer and check if there is more than that
    var.AppendL( TColumn( aBufferSize + 1 ) );

	RMdsStatement getQuery;
    CleanupClosePushL( getQuery );

	db.ExecuteQueryL( buf.ConstBufferL(), getQuery, var );

	var.Free();
	var.Reset();

	TItemId objectId = 0;

	var.AppendL( TColumn( objectId ) );

	TInt extraRows = 0;

	while( db.NextRowL( getQuery, var ) )
		{
		var.Column(0).Get( objectId );
		
		if( aObjectIds.Count() < aBufferSize )
			{
			aObjectIds.Append( objectId );
			}
		else
			{
			extraRows = 1;
			break;
			}
		}

	CleanupStack::PopAndDestroy( 2, &var ); // getQuery, varSearch
	CleanupStack::PopAndDestroy( &commonClause );

	return extraRows;
	}

TBool CMdSSqlObjectManipulate::DoGarbageCollectionL()
	{
    _LIT( KDeleteObject,                 "DELETE FROM Object%u WHERE ObjectId IN (SELECT ObjectId FROM Object%u WHERE Flags&? LIMIT 100);" );
	_LIT( KUpdateDeleteObject,           "UPDATE Object%u SET Flags=Flags|? WHERE Flags&?;" );
	_LIT( KDeleteRelations,              "DELETE FROM Relations%u WHERE Flags&?;" );
	_LIT( KUpdateDeleteRelations,        "UPDATE Relations%u SET Flags=Flags|? WHERE Flags&?;" );
	_LIT( KUpdateDeleteContextObjects,   "UPDATE Object%u SET Flags=Flags|? WHERE ObjectId IN ( SELECT ObjectId FROM Object%u AS O WHERE Flags&? AND UsageCount=0 AND ( SELECT count(*) FROM Relations%u WHERE NOT Flags&? AND ( LeftObjectId = O.ObjectId OR RightObjectId = O.ObjectId ) )= 0 );" );
    _LIT( KDeleteWordFromTextSearchDict, "DELETE FROM TextSearchDictionary%u WHERE NOT EXISTS(SELECT WordId FROM TextSearch%u WHERE WordId = TextSearchDictionary%u.WordId);");

	RClauseBuffer commonClauseOne(*this, KUpdateDeleteContextObjects().Length() + 3 * KMaxUintValueLength);
	CleanupClosePushL( commonClauseOne );
	CMdsClauseBuffer& buffer = commonClauseOne.BufferL();

    RRowData rowDataDel;
    CleanupClosePushL( rowDataDel );
    rowDataDel.AppendL( TColumn( EMdEObjectFlagGarbage ) );

   	const RPointerArray<CMdsNamespaceDef>& namespaceDefs = 
   		iSchema.NamespaceDefs();

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	TInt deleteObjectResult = 0;
	TInt updateResult = 0;
	
	const TInt count = namespaceDefs.Count();
	
   	for( TInt i = 0; i < count; ++i )
   	    {
   	    const TDefId nmspId = namespaceDefs[i]->GetId();

		// deleting objects
		buffer.BufferL().Format( KDeleteObject, nmspId, nmspId );
   	    User::LeaveIfError( deleteObjectResult = connection.ExecuteL( 
   	    		buffer.ConstBufferL(), rowDataDel ) );
   	    
   	    if( deleteObjectResult > 0 )
   	        {
   	        // If objects were deleted, continue garbage collection 
   	        iDictionaryToBeCleaned = ETrue;
   	        CleanupStack::PopAndDestroy( 2, &commonClauseOne );
   	        return ETrue;
   	        }

   	    RRowData rowDataUpd;
   	    CleanupClosePushL( rowDataUpd );
   	    rowDataUpd.AppendL( TColumn( EMdEObjectFlagGarbage ) );
   	    rowDataUpd.AppendL( TColumn( EMdEObjectFlagRemoved ) );
   	    
        buffer.BufferL().Format( KUpdateDeleteObject, nmspId );
   	    User::LeaveIfError( updateResult += connection.ExecuteL(
   	    		buffer.ConstBufferL(), rowDataUpd ) );

        if( updateResult > 0 )
            {
            // If objects were modified, continue garbage collection 
            CleanupStack::PopAndDestroy( 3, &commonClauseOne );
            return ETrue;
            }

        RRowData rowDataDelRel;
        CleanupClosePushL( rowDataDelRel );
        rowDataDelRel.AppendL( TColumn( EMdERelationFlagGarbageDeleted ) );
        
        RRowData rowDataUpdRel;
        CleanupClosePushL( rowDataUpdRel );
        rowDataUpdRel.AppendL( TColumn( EMdERelationFlagGarbageDeleted ) );
        rowDataUpdRel.AppendL( TColumn( EMdERelationFlagDeleted ) );
        
		// deleting relations
		buffer.BufferL().Format( KDeleteRelations, nmspId );
   	    User::LeaveIfError( connection.ExecuteL( 
   	    		buffer.ConstBufferL(), rowDataDelRel ) );

		buffer.BufferL().Format( KUpdateDeleteRelations, nmspId );
   	    User::LeaveIfError( updateResult += connection.ExecuteL( 
   	    		buffer.ConstBufferL(), rowDataUpdRel ) );
   	    
        if( updateResult > 0 )
            {
            // If objects were modified, continue garbage collection 
            CleanupStack::PopAndDestroy( 5, &commonClauseOne );
            return ETrue;
            }

        RRowData rowDataDelContext;
        CleanupClosePushL( rowDataDelContext );
        rowDataDelContext.AppendL( TColumn( EMdEObjectFlagRemoved ) );
        rowDataDelContext.AppendL( TColumn( EMdEObjectFlagContext ) );
        rowDataDelContext.AppendL( TColumn( EMdERelationFlagDeleted ) );
        
		// deleting context objects
		buffer.BufferL().Format( KUpdateDeleteContextObjects, nmspId, nmspId, nmspId );
   	    User::LeaveIfError( updateResult += connection.ExecuteL( 
   	    		buffer.ConstBufferL(), rowDataDelContext ) );
   	    
        if( updateResult > 0 )
            {
            // If objects were modified, continue garbage collection 
            CleanupStack::PopAndDestroy( 6, &commonClauseOne );
            return ETrue;
            }

        RRowData emptyRow;
        CleanupClosePushL( emptyRow );
        
		// deleting words from text search dictionary
		if ( iDictionaryToBeCleaned )
			{
			buffer.BufferL().Format( KDeleteWordFromTextSearchDict, nmspId, nmspId, 
					nmspId );
	   	    User::LeaveIfError( connection.ExecuteL( 
	   	    		buffer.ConstBufferL(), emptyRow ) );
	   	    iDictionaryToBeCleaned = EFalse;
			}
		
	    // empryRow, rowDataDelContext, rowDataUpdRel, rowDataDelRel, rowDataUpd, 
	    CleanupStack::PopAndDestroy( 5, &rowDataUpd );		
   	    }

   	// rowDataDel, commonClauseOne
	CleanupStack::PopAndDestroy( 2, &commonClauseOne );

#ifdef MDS_PLAYLIST_HARVESTING_ENABLED	
	if( updateResult == 0 )
	    {
	    updateResult = CleanPlaylistsL();
	    }
#endif

	return EFalse;
	}

void CMdSSqlObjectManipulate::AnalyzeL()
    {
    CMdSSqLiteConnection& db = MMdSDbConnectionPool::GetDefaultDBL();
    db.DoAnalyzeL();
    }

#ifdef MDS_PLAYLIST_HARVESTING_ENABLED
TInt CMdSSqlObjectManipulate::CleanPlaylistsL()
    {
    _LIT( KDeleteWholePlaylists, "DELETE FROM Object%u WHERE ObjectId IN (SELECT ObjectId FROM AudioPlaylistItem%u WHERE PlaylistID NOT IN (SELECT ObjectId FROM Object%u));" );
    
    RClauseBuffer commonClauseOne(*this, KDeleteWholePlaylists().Length());
    CleanupClosePushL( commonClauseOne );
    CMdsClauseBuffer& buffer = commonClauseOne.BufferL();

    RRowData rowDataDel;
    CleanupClosePushL( rowDataDel );

    const RPointerArray<CMdsNamespaceDef>& namespaceDefs = 
        iSchema.NamespaceDefs();

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    const TInt updateResult = 0; // once all files to be cleaned are handled, no need to continue
    
    const TInt count = namespaceDefs.Count();
    
    for( TInt i = 0; i < count; ++i )
        {
        const TDefId nmspId = namespaceDefs[i]->GetId();

        buffer.BufferL().Format( KDeleteWholePlaylists, nmspId, nmspId, nmspId );
        User::LeaveIfError( connection.ExecuteL( 
                buffer.ConstBufferL(), rowDataDel ) );    
        }

    // rowDataDel, commonClauseOne
    CleanupStack::PopAndDestroy( 2, &commonClauseOne );
    
    return updateResult;
    }
#endif

CMdSSqlObjectManipulate::RClauseBuffer::RClauseBuffer( CMdSSqlObjectManipulate& aSOM, TInt aSize )
	: iBuffers( aSOM.iBuffers ), iBuffer( NULL ), iNr( -1 ), iSize( aSize )
	{
	// search for available buffer
	// or create new one
	for (TInt i = 0; i < iBuffers.Count(); ++i)
		{
		if (!iBuffers[i].iLock)
			{
		    iBuffers[i].iLock = ETrue;
            CMdsClauseBuffer* oldBuffer( iBuffer );
            iBuffer = iBuffers[i].iBuffer;
            TRAPD( error, iBuffer->ReserveSpaceL(aSize) );
            if( error != KErrNone )
                {
                iBuffer = oldBuffer;
                iBuffers[i].iLock = EFalse;
                continue;
                }		
            TRAP_IGNORE( iBuffer->BufferL().Zero() );			
			iNr = i;
			return;
			}
		}
	}

CMdsClauseBuffer& CMdSSqlObjectManipulate::RClauseBuffer::BufferL()
	{
	if ( iNr < 0 && !iBuffer )
		{
		iBuffer = CMdsClauseBuffer::NewL( iSize );
		}
	return *iBuffer;
	}

void CMdSSqlObjectManipulate::RClauseBuffer::Close()
	{
	if ( iNr < 0 )
		{
		__ASSERT_DEBUG( iBuffer, _L("RClauseBuffer::Close()") );
		if ( iBuffer )
			{
			delete iBuffer;
			iBuffer = NULL;
			}
		}
	else
		{
		iBuffers[iNr].iLock = EFalse;
		iBuffer = NULL;
		}
	}
