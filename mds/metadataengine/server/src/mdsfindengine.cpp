/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manages object search from database*
*/

#include "mdsfindengine.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdsdbconnectionpool.h"
#include "mdsobjectlocklist.h"
#include "mdsserversession.h"
#include "mdsfindsequence.h"
#include "mdslogger.h"
#include "mdcserializationbuffer.h"
#include "mdsschema.h"
#include "mdsserver.h"

__USES_LOGGER



// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CMdSFindEngine* CMdSFindEngine::NewL(
	TUint32 aQueryId, 
    CMdSServerSession& aParent,
    CMdSObjectLockList& aLockList,
    CMdsSchema& aSchemaNew )
    {
    CMdSFindEngine* self = CMdSFindEngine::NewLC(
        aQueryId, aParent, aLockList, aSchemaNew );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CMdSFindEngine* CMdSFindEngine::NewLC(
	TUint32 aQueryId, 
    CMdSServerSession& aParent,
    CMdSObjectLockList& aLockList,
    CMdsSchema& aSchemaNew )
    {
    CMdSFindEngine* self = new ( ELeave ) CMdSFindEngine(
        aQueryId, aParent, aLockList );
    CleanupStack::PushL( self );
    self->ConstructL( aSchemaNew );
    return self;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CMdSFindEngine::CMdSFindEngine(
	TUint32 aQueryId, 
    CMdSServerSession& aParent,
    CMdSObjectLockList& aLockList )
    : iQueryId( aQueryId )
    , iParent( aParent )
    , iLockList( aLockList )
    , iSequence( NULL )
    , iSerializedCriteria( NULL )
    , iAsyncPending( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::ConstructL( CMdsSchema& aSchema )
    {
    iSequence = CMdSFindSequence::NewL( iParent.GetServer(), aSchema, *this );
    }

// ---------------------------------------------------------------------------
// QueryId
// ---------------------------------------------------------------------------
//
TUint32 CMdSFindEngine::QueryId()
	{
	return iQueryId;
	}

// ---------------------------------------------------------------------------
// SetFindParams
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::SetFindParams( TUint32 aNotifyCount )
    {
    iSequence->SetFindParams( aNotifyCount );
    }

// ---------------------------------------------------------------------------
// SetFindCriteria
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::SetFindCriteria( CMdCSerializationBuffer* aSerializedCriteria )
    {
    iSerializedCriteria = aSerializedCriteria;
    }

// ---------------------------------------------------------------------------
// CreateResultSetL
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::CreateResultSet(RMessage2 aMessage)
    {
    iMessage = aMessage;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMdSFindEngine::~CMdSFindEngine()
    {
   	delete iSerializedCriteria;
   	iSerializedCriteria = NULL;
   	delete iSequence;
   	iSequence = NULL;
    }

// ---------------------------------------------------------------------------
// FindL
// ---------------------------------------------------------------------------
//
TInt CMdSFindEngine::FindL( TUserLevel aUserLevel )
    {
    const TInt result = iSequence->FindL( *iSerializedCriteria, aUserLevel );

    if( result == KErrNone && iMessage.IsNull() == EFalse )
    	{
    	TPckgBuf<TInt> sizeBuf( iSequence->ResultsL().Size() );
    	iMessage.WriteL( 2, sizeBuf );
    	}
    
    return result;
    }

// ---------------------------------------------------------------------------
// ContinueL
// ---------------------------------------------------------------------------
//
TInt CMdSFindEngine::ContinueL()
    {
    const TInt result = iSequence->ContinueL();
    
    if( result == KErrNone && iMessage.IsNull() == EFalse )
    	{
    	TPckgBuf<TInt> sizeBuf( iSequence->ResultsL().Size() );
    	iMessage.WriteL( 2, sizeBuf );
    	}
        
    return result;
    }

// ---------------------------------------------------------------------------
// FindAsyncL
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::FindAsyncL( TUserLevel aUserLevel )
    {
    iAsyncPending = ETrue;
    iSequence->FindAsync( *iSerializedCriteria, aUserLevel );
    }

// ---------------------------------------------------------------------------
// ContinueAsync
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::ContinueAsync()
    {
    iAsyncPending = ETrue;
    iSequence->ContinueAsync();
    }

// ---------------------------------------------------------------------------
// Cancel
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::Cancel( TInt aError )
    {
    TBool toComplete = iAsyncPending;
    iAsyncPending = EFalse;
    iSequence->Cancel();

    if( iMessage.IsNull() == EFalse )
    	{
    	if ( toComplete ) 
    		{
    		iMessage.Complete( KErrCancel );
    		}
    	else 
    		{
    		iMessage.Complete( aError );
    		}
    	}
    }

// ---------------------------------------------------------------------------
// SetComplete
// ---------------------------------------------------------------------------
//
void CMdSFindEngine::SetComplete( TInt aError, TBool aSet )
    {
    TBool toComplete = iAsyncPending;
    
#ifdef _DEBUG
    if( aSet )
        {
        __LOG1( ELogServer, "<- Find Set Complete (%d)", aError );
        }
    else
        {
        __LOG1( ELogServer, "<- Find Complete (%d)", aError );    
        }
#endif

    if( aError == KErrNone )
        {
        TUint32 resultSize = 0;

        TInt err = KErrNone;

        TRAP( err, resultSize = iSequence->ResultsL().Size() );

       	if( err )
       		{
       		if ( toComplete && iMessage.IsNull() == EFalse ) 
       			{
       			iMessage.Complete( err );
       			}
       		return;
       		}

        iAsyncPending = EFalse;

       	if ( iMessage.IsNull() == EFalse )
       		{
       		TPckgBuf<TInt> sizeBuf( resultSize );

       		err = iMessage.Write( 2, sizeBuf );
       		}

       	if( err )
       		{
       		if ( toComplete && iMessage.IsNull() == EFalse ) 
       			{
       			iMessage.Complete( err );
       			}
       		return;
       		}
       	else
       		{        	
       		if ( toComplete && iMessage.IsNull() == EFalse ) 
       			{
       		    if( aSet )
       		        {
                    iMessage.Complete( EAsyncFindSetReady );
       		        }
       		    else
       		        {
       		        iMessage.Complete( EAsyncFindComplete );
       		        }
       			}
       		}
        }
    else
        {
       	if ( toComplete && iMessage.IsNull() == EFalse ) 
       		{
       		iMessage.Complete( aError );
       		}
       	return;
        }
    }

const CMdCSerializationBuffer& CMdSFindEngine::ResultsL() const
	{
	return iSequence->ResultsL();
	}

void CMdSFindEngine::SetResultMode( TBool aResultModeItems )
	{
	iLastResultModeItems = aResultModeItems;
	}

TBool CMdSFindEngine::IsComplete() const
	{
	if( iSequence )
		{		
		return iSequence->IsComplete();
		}
	else
		{
		return ETrue;
		}
	}
	
TBool CMdSFindEngine::IsQueryComplete()
	{
	if( iSequence )
		{		
		return iSequence->IsQueryComplete();
		}
	else
		{
		return ETrue;
		}
	}	

CMdSServer& CMdSFindEngine::GetServer() const
	{
	return iParent.GetServer();
	}

void CMdSFindEngine::LockFindResultObjectsL( CMdSObjectLockList& aLockList )
	{
	if (!iLastResultModeItems)
		{
		return;
		}

	CMdsSchema& schema = iParent.GetServer().Schema();
	CMdCSerializationBuffer& buffer = iSequence->ResultsL();
	buffer.PositionL( KNoOffset );
	TMdCItems items;
	items.DeserializeL( buffer );

	const CMdsNamespaceDef* namespaceDef = schema.GetNamespaceByIdL( items.iNamespaceDefId );

	for(TInt32 i = 0; i < items.iObjects.iPtr.iCount; ++i)
		{
		buffer.PositionL( items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject) );
		TMdCObject object;
		object.DeserializeL( buffer );

		if ( aLockList.IsLocked( *namespaceDef, object.iId ) )
			{
			object.iFlags &= ~EMdEObjectFlagModOpen;
			}
		else
			{
			aLockList.LockObjectL( iParent, *namespaceDef, object.iId );
			object.iFlags |= EMdEObjectFlagModOpen;
			}

		buffer.PositionL( items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject) );
		object.SerializeL( buffer );
		}
	}

