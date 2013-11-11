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
* Description:  Notifier client side active object
*
*/


// INCLUDE FILES
#include "mdenotifierao.h"

#include "mdcresult.h"
#include "mdeenginesession.h"
#include "mdesessionimpl.h"
#include "mdenamespacedef.h"
#include "mdcserializationbuffer.h"
#include "mdccommon.pan"

// ========================= MEMBER FUNCTIONS ==================================

CMdENotifierAO* CMdENotifierAO::NewL(
    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    {
    CMdENotifierAO* self = CMdENotifierAO::NewLC( aSessionImpl, aSession );
    CleanupStack::Pop( self );
    return self;
    }


CMdENotifierAO* CMdENotifierAO::NewLC(
    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    {
    CMdENotifierAO* self =
        new ( ELeave ) CMdENotifierAO( aSessionImpl, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CMdENotifierAO::CMdENotifierAO(
    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    : CActive( CActive::EPriorityUserInput )
    , iSessionImpl( aSessionImpl )
    , iSession( aSession )
    {
    CActiveScheduler::Add( this );
    }

void CMdENotifierAO::ConstructL()
    {
    }


CMdENotifierAO::~CMdENotifierAO()
    {
    Cancel(); // Causes call to DoCancel()
    delete iDataBuffer;
    iDataBuffer = NULL;
    iIdArray.Close();
    iRelationItemArray.Close();
    }


void CMdENotifierAO::RegisterL( TUint32 aType, TAny* aObserver,
    							CMdECondition* aCondition, CMdENamespaceDef& aNamespaceDef )
    {
    iObserver = aObserver;
    iType = aType;
    iNamespaceDefId = aNamespaceDef.Id();

	CMdCSerializationBuffer* buffer = NULL;

	if( aCondition )
		{
   		buffer = CMdCSerializationBuffer::NewLC( aCondition->RequiredBufferSize() );

   		// only needed, because method needs it as parameter
   		TUint32 freespaceOffset = 0;

   		aCondition->SerializeL( *buffer, freespaceOffset );
		}
	else
		{
		// create empty serialized condition buffer
		buffer = CMdCSerializationBuffer::NewLC( 0 );
		}

   	iSession.DoRegisterL( Id(), iType, *buffer, iNamespaceDefId );
	CleanupStack::PopAndDestroy( buffer );

    // listen for first event
    iSession.DoListen( Id(), &iResultSize, iStatus );
    SetActive();
    }

TBool CMdENotifierAO::Match( TUint32 aType, TAny* aObserver, CMdENamespaceDef& aNamespaceDef )
    {
    if( aNamespaceDef.Id() != iNamespaceDefId )
    	{
    	return EFalse;
    	}

    if ( iObserver != aObserver )
    	{
    	return EFalse;
    	}
    
    return ( iType & aType );
    }

TInt CMdENotifierAO::Id()
    {
    return (TInt)this;
    }

void CMdENotifierAO::DoCancel()
    {
    TRAP_IGNORE( iSession.DoUnregisterL( Id() ) );
    }

void CMdENotifierAO::RunL()
    {
    const TInt status = iStatus.Int();

    if ( status >= KErrNone )
    	{
	    if ( status & ( EObjectNotifyAdd | EObjectNotifyModify | EObjectNotifyRemove
	    				| EObjectNotifyPresent | EObjectNotifyNotPresent
	    				| ERelationNotifyAdd | ERelationNotifyModify | ERelationNotifyRemove
	    				| ERelationNotifyPresent | ERelationNotifyNotPresent
	    				| EEventNotifyAdd | EEventNotifyRemove ) )
	    	{
	    	if( !iDataBuffer )
	    		{
	    		iDataBuffer = CMdCSerializationBuffer::NewL( iResultSize() );
	    		}
	    	else if( iDataBuffer->Buffer().MaxSize() < iResultSize() )
	    		{
	    		delete iDataBuffer;
	    		iDataBuffer = NULL;
	    		iDataBuffer = CMdCSerializationBuffer::NewL( iResultSize() );
	    		}
	    	
	    	if( iResultSize() )
	    		{
	        	iSession.DoGetDataL( *iDataBuffer, Id() ); // reads ids to the buffer
	        	DecodeIdBufferL(); // decodes ids from the data buffer and puts them to iIdArray
	        	
	        	delete iDataBuffer;
	        	iDataBuffer = NULL;
	        	
	            DoNotifyObserver(); // notifies the observer about the event with an array of ids
	    		}
	        iSession.DoListen( Id(), &iResultSize, iStatus );  // continue listening for events
	        SetActive();
	        }
	    else if ( status & ( ERelationItemNotifyRemove ) ) // a relation was removed
	    	{
	    	if( !iDataBuffer )
	    		{
	    		iDataBuffer = CMdCSerializationBuffer::NewL(iResultSize());
	    		}
	    	else if( iDataBuffer->Size() < (TUint32)iResultSize() )
	    		{
	    		delete iDataBuffer;
	    		iDataBuffer = NULL;
	    		iDataBuffer = CMdCSerializationBuffer::NewL(iResultSize());
	    		}
	    	
	    	if(iResultSize())
	    		{
	        	iSession.DoGetDataL( *iDataBuffer, Id() ); // reads ids to the buffer
	        	DecodeRelationItemBufferL(); // decodes ids from the data buffer and puts them to iIdArray
	        	
	        	delete iDataBuffer;
	        	iDataBuffer = NULL;
	        	
	            DoNotifyObserver(); // notifies the observer about the event with an array of ids
	    		}
	        iSession.DoListen( Id(), &iResultSize, iStatus ); // continue listening for events
	        SetActive();
	        }
	    else if ( status == ESchemaModify )  // schema has been modified
	    	{
	        DoNotifyObserver(); // notifies the observer about the event with an array of ids
	        iSession.DoListen( Id(), &iResultSize, iStatus ); // continue listening for events
	        SetActive();
	    	}
	    else if ( status & ( EObjectNotifyAddWithUri | EObjectNotifyModifyWithUri
	                           | EObjectNotifyRemoveWithUri ) )
	        {
	        if( !iDataBuffer )
	            {
	            iDataBuffer = CMdCSerializationBuffer::NewL( iResultSize() );
	            }
	        else if( iDataBuffer->Buffer().MaxSize() < iResultSize() )
	            {
	            delete iDataBuffer;
	            iDataBuffer = NULL;
	            iDataBuffer = CMdCSerializationBuffer::NewL( iResultSize() );
	            }
	        
	        if( iResultSize() )
	            {
	            iSession.DoGetDataL( *iDataBuffer, Id() ); // reads data to the buffer
	            DecodeDataBufferL(); // decodes ids and uri from the data buffer 
	            
	            delete iDataBuffer;
	            iDataBuffer = NULL;
	            
	            DoNotifyObserver(); // notifies the observer about the event
	            }
	        iSession.DoListen( Id(), &iResultSize, iStatus );  // continue listening for events
	        SetActive();
	        }
    	}
    else
        {
        // error in notifier mechanism
        if( status != KErrServerTerminated )
        	{        
        	iSession.DoUnregisterL( Id() ); // no response expected
        	}
        else
        	{
        	iSessionImpl.NotifyError( status );
        	}
        iSessionImpl.NotifierInError( this );
        }
    }

TInt CMdENotifierAO::RunError(TInt aError)
    {
    if( aError == KErrServerTerminated )
    	{
    	iSessionImpl.NotifyError( aError );
    	}
    
    // notifying observer failed - continue listening anyway.
    iSessionImpl.NotifierInError( this );
    return KErrNone;
    }

void CMdENotifierAO::DoNotifyObserver()
    {
    if( !iObserver )
        {
        return;
        }
    const TInt status = iStatus.Int();
    switch( iType & status )
        {
        case EObjectNotifyAdd:
        	{
        	MMdEObjectObserver* obs = static_cast<MMdEObjectObserver*>( iObserver );
        	obs->HandleObjectNotification( iSessionImpl, ENotifyAdd, iIdArray );     
        	break;
        	}
        case EObjectNotifyModify:
        	{
        	MMdEObjectObserver* obs = static_cast<MMdEObjectObserver*>( iObserver );
        	obs->HandleObjectNotification( iSessionImpl, ENotifyModify, iIdArray );         
        	break;
        	}
        case EObjectNotifyRemove:
        	{
        	MMdEObjectObserver* obs = static_cast<MMdEObjectObserver*>( iObserver );
        	obs->HandleObjectNotification( iSessionImpl, ENotifyRemove, iIdArray );        
        	break;
        	}

        case EObjectNotifyPresent:
        	{
	    	MMdEObjectPresentObserver* obs = static_cast<MMdEObjectPresentObserver*>( iObserver );
            obs->HandleObjectPresentNotification( iSessionImpl, ETrue, iIdArray );      
	    	break;
        	}
        case EObjectNotifyNotPresent:
        	{
	    	MMdEObjectPresentObserver* obs = static_cast<MMdEObjectPresentObserver*>( iObserver );
            obs->HandleObjectPresentNotification( iSessionImpl, EFalse, iIdArray );       
	    	break;
        	}
    	
        case ERelationNotifyAdd:
        	{
	        MMdERelationObserver* obs = static_cast<MMdERelationObserver*>( iObserver );
            obs->HandleRelationNotification( iSessionImpl, ENotifyAdd, iIdArray );           
	        break;
        	}
        case ERelationNotifyModify:
        	{
	        MMdERelationObserver* obs = static_cast<MMdERelationObserver*>( iObserver );
            obs->HandleRelationNotification( iSessionImpl, ENotifyModify, iIdArray );        
	        break;
        	}
        case ERelationNotifyRemove:
        	{
	        MMdERelationObserver* obs = static_cast<MMdERelationObserver*>( iObserver );
            obs->HandleRelationNotification( iSessionImpl, ENotifyRemove, iIdArray );        
	        break;
        	}

        case ERelationNotifyPresent:
        	{
	    	MMdERelationPresentObserver* obs = static_cast<MMdERelationPresentObserver*>( iObserver );
            obs->HandleRelationPresentNotification( iSessionImpl, ETrue, iIdArray );          
	    	break;
        	}
        case ERelationNotifyNotPresent:
        	{
	    	MMdERelationPresentObserver* obs = static_cast<MMdERelationPresentObserver*>( iObserver );
            obs->HandleRelationPresentNotification( iSessionImpl, EFalse, iIdArray );         
	    	break;
        	}
        	
        case ERelationItemNotifyRemove:
        	{
        	MMdERelationItemObserver* obs = static_cast<MMdERelationItemObserver*>( iObserver );
            obs->HandleRelationItemNotification( iSessionImpl, ENotifyRemove, iRelationItemArray );
            iRelationItemArray.Reset();
            iRelationItemArray.Compress();
        	break;
        	}

        case EEventNotifyAdd:
        	{
            MMdEEventObserver* obs = static_cast<MMdEEventObserver*>( iObserver );
            obs->HandleEventNotification( iSessionImpl, ENotifyAdd, iIdArray);      
            break;
        	}
        case EEventNotifyRemove:
        	{
            MMdEEventObserver* obs = static_cast<MMdEEventObserver*>( iObserver );
            obs->HandleEventNotification( iSessionImpl, ENotifyRemove, iIdArray);           
            break;
        	}
    	
        case ESchemaModify:
        	{
            MMdESchemaObserver* obs = static_cast<MMdESchemaObserver*>( iObserver );
            obs->HandleSchemaModified();
        	break;
        	}
        	
        case EObjectNotifyAddWithUri:
            {
            MMdEObjectObserverWithUri* obs = static_cast<MMdEObjectObserverWithUri*>( iObserver );
            obs->HandleUriObjectNotification( iSessionImpl, ENotifyAdd, iIdArray, iUriArray );
            iUriArray.ResetAndDestroy();
            iUriArray.Compress();
            break;
            }
        case EObjectNotifyModifyWithUri:
            {
            MMdEObjectObserverWithUri* obs = static_cast<MMdEObjectObserverWithUri*>( iObserver );
            obs->HandleUriObjectNotification( iSessionImpl, ENotifyModify, iIdArray, iUriArray );
            iUriArray.ResetAndDestroy();
            iUriArray.Compress();
            break;
            }
        case EObjectNotifyRemoveWithUri:
            {
            MMdEObjectObserverWithUri* obs = static_cast<MMdEObjectObserverWithUri*>( iObserver );
            obs->HandleUriObjectNotification( iSessionImpl, ENotifyRemove, iIdArray, iUriArray );
            iUriArray.ResetAndDestroy();
            iUriArray.Compress();
            break;
            }    	
        	
        default:
        	// no observer to call - this should be skipped on server side!
        	break;
        }
    iIdArray.Reset();
    iIdArray.Compress();
    }

void CMdENotifierAO::DecodeIdBufferL()
	{
	// IDs are always stored in object IDs, 
	// even if those are actually relation or event IDs
	
	iIdArray.Reset();
	iDataBuffer->PositionL( KNoOffset );
	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *iDataBuffer );
	__ASSERT_DEBUG( iNamespaceDefId == itemIds.iNamespaceDefId, User::Panic( _L("Incorrect namespaceDef from returned items!"), KErrCorrupt ) );

    iDataBuffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
    iIdArray.ReserveL( itemIds.iObjectIds.iPtr.iCount );
	for( TUint32 i = 0; i < itemIds.iObjectIds.iPtr.iCount; ++i )
		{
		TItemId id;
		iDataBuffer->ReceiveL( id );
		iIdArray.AppendL( id );
		}
	}

void CMdENotifierAO::DecodeDataBufferL()
    {    
    iIdArray.Reset();
    iUriArray.ResetAndDestroy();
    
    iDataBuffer->PositionL( KNoOffset );
    
    const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *iDataBuffer );
    __ASSERT_DEBUG( iNamespaceDefId == itemIds.iNamespaceDefId, User::Panic( _L("Incorrect namespaceDef from returned items!"), KErrCorrupt ) );
    
    // Get IDs
    iDataBuffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
	iIdArray.ReserveL( itemIds.iObjectIds.iPtr.iCount );
    for( TUint32 i = 0; i < itemIds.iObjectIds.iPtr.iCount; ++i )
        {
        TItemId id;
        iDataBuffer->ReceiveL( id );
        iIdArray.AppendL( id );
        }
    
    // Get uri count
    TUint32 uriCount ( 0 );
    iDataBuffer->ReceiveL( uriCount );
    
    HBufC* uri = NULL;
    
    for( TInt i( 0 ); i < uriCount; i++ )
        {        
        //Get uri
        uri = iDataBuffer->ReceiveDes16L();
        CleanupStack::PushL( uri );
        iUriArray.AppendL( uri );
        CleanupStack::Pop( uri );
        }
    }

void CMdENotifierAO::DecodeRelationItemBufferL()
	{
    iRelationItemArray.Reset();
	iDataBuffer->PositionL( KNoOffset );
	const TMdCItems& items = TMdCItems::GetFromBufferL( *iDataBuffer );
	__ASSERT_DEBUG( iNamespaceDefId == items.iNamespaceDefId, User::Panic( _L("Incorrect namespaceDef from returned items!"), KErrCorrupt ) );

    CMdENamespaceDef& namespaceDef = iSessionImpl.GetNamespaceDefL( iNamespaceDefId );
    iDataBuffer->PositionL( items.iRelations.iPtr.iOffset );
    TMdERelation relation;
    iRelationItemArray.ReserveL( items.iRelations.iPtr.iCount );
    for (TInt i = 0; i < items.iRelations.iPtr.iCount; ++i )
    	{
    	relation.DeSerializeL( *iDataBuffer, namespaceDef );
    	if ( relation.Id() )
    		{
    		iRelationItemArray.AppendL( relation );
    		}
    	}
	}

