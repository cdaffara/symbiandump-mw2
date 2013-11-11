/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  An interface to Location Object
*
*/

#include "locationdatatype.h"
#include "rlocationobjectmanipulator.h"
#include "locationmanagerdefs.h"
#include "locationmanagerdebug.h"
#include "mdsutils.h"
#include <locationeventdef.h>
#include "mdcserializationbuffer.h"

// --------------------------------------------------------------------------
// RLocationObjectManipulator::CreateLocationObject
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationObjectManipulator::CreateLocationObject( TLocationData& aLocationData,
									TItemId aObjectId )
	{
	LOG( "RLocationObject::CreateLocationObject begin" );
    TInt ret( KErrDisconnected );
    TPckg<TLocationData> locationData( aLocationData );
    TPckg<TItemId> objectId( aObjectId );    
    if ( iHandle )
        {
        ret = SendReceive( ELocManCreateLocationObject, TIpcArgs( &locationData,  &objectId) );
        }
    LOG( "RLocationObject::CreateLocationObject end" );
    return ret;
	}

// --------------------------------------------------------------------------
// RLocationObjectManipulator::LocationSnapshot
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationObjectManipulator::LocationSnapshot(	TItemId aObjectId )
	{
	LOG( "RLocationObject::LocationSnapshot begin" );
    TInt ret( KErrDisconnected );
    TPckg<TItemId> objectId( aObjectId );    
    if ( iHandle )
        {
        ret = SendReceive( ELocManLocationSnapshot, TIpcArgs( &objectId ) );
        }
    LOG( "RLocationObject::LocationSnapshot end" );
    return ret;
	}


// --------------------------------------------------------------------------
// RLocationObjectManipulator::RemoveLocationObject
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationObjectManipulator::RemoveLocationObject( TItemId aObjId )
	{
	LOG( "RLocationObject::RemoveLocationObject begin" );
	TInt ret( KErrDisconnected );
	TPckg<TItemId> objId( aObjId );
	if ( iHandle )
        {
        ret = SendReceive( ELocManRemoveLocationObject, TIpcArgs( &objId ));
        }
	LOG( "RLocationObject::RemoveLocationObject end" );
	return ret;
	}
// --------------------------------------------------------------------------
// RLocationObjectManipulator::CopyLocationData
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationObjectManipulator::CopyLocationData (TItemId aSourceId, 
												RArray<TItemId>& aTargetIds,
												TRequestStatus& aStatus)
	{
	LOG( "RLocationObject::CopyLocationData begin" );
	
	TPckg<TItemId> sourceId( aSourceId );
	// serializing RArray, must deserialize in server side

	HBufC8* paramBuf = NULL;   
    TRAPD( err, paramBuf = SerializeArrayL( aTargetIds ) );
    // was serializing failed
    if ( err != KErrNone )
    	{
        LOG1 ( "RLocationObject::CopyLocationData - cannot create serialized objectIds for sending, error: %d", err );
        delete paramBuf;
        paramBuf = NULL;
        CompleteRequest( aStatus, err );

        return;
    	}
    
    if ( iHandle )
        {
       	TIpcArgs args( &sourceId, paramBuf );
        const TInt result = SendReceive( ELocManCopyLocationDataById, args );
        CompleteRequest( aStatus, result );
        }
    else
        {
        CompleteRequest( aStatus, KErrDisconnected );
        }
    
    delete paramBuf;
    paramBuf = NULL;
	LOG( "RLocationObject::CopyLocationData end" );
	}
// --------------------------------------------------------------------------
// RLocationObjectManipulator::CopyLocationData
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationObjectManipulator::CopyLocationData (TDesC& aSourceURI, 
								RPointerArray<TDesC>& aTargetURIs,
								TRequestStatus& aStatus)
	{
	LOG( "RLocationObject::CopyLocationData begin" );

    TInt32 uriCount = aTargetURIs.Count();
	
	// Required size for serialized URI buffer, count
    TInt urisRequiredSize = CMdCSerializationBuffer::KRequiredSizeForTInt32;

    // and URIs
    for( TInt i = uriCount - 1; i >=0; i-- )
    	{
    	urisRequiredSize += CMdCSerializationBuffer::RequiredSize( *aTargetURIs[i] );
    	}

   	// serializing URIs
    CMdCSerializationBuffer* uriBuffer( NULL );
    TRAPD( error, uriBuffer = CMdCSerializationBuffer::NewL( urisRequiredSize ); 
                          uriBuffer->InsertL( uriCount ) );
    
    if( error != KErrNone )
        {
        LOG( "RLocationObject::CopyLocationData CMdCSerializationBuffer creation failed" );
        delete uriBuffer;
        uriBuffer = NULL;
        CompleteRequest( aStatus, error );
        return;
        }

    for( TInt i = uriCount - 1; i >=0; i-- )
		{
		TRAPD( err, uriBuffer->InsertL( *aTargetURIs[i] ) );
		
	    // was serializing failed
	    if ( err != KErrNone )
	    	{
	        LOG1 ( "RLocationObject::CopyLocationData - cannot create serialized objectIds for sending, error: %d", err );
	        delete uriBuffer;
	        uriBuffer = NULL;
	        CompleteRequest( aStatus, err );
	        return;
	    	}
		}
    
    if ( iHandle )
        {
        TIpcArgs args( &aSourceURI, uriBuffer->BufferConstPtr() );
        const TInt result = SendReceive( ELocManCopyLocationDataByUri, args );
        CompleteRequest( aStatus, result );
        }
    else
        {
        CompleteRequest( aStatus, KErrDisconnected );
        }
    
    delete uriBuffer;
    uriBuffer = NULL;
    LOG( "RLocationObject::CopyLocationData end" );
	}

