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
* Description: 
*
*/


// INCLUDE FILES
#include    <MCLFOperationObserver.h>
#include    <MCLFSortingStyle.h>
#include    <MCLFItem.h>
#include    <MCLFChangedItemObserver.h>
#include    "CCLFEngineImpl.h"
#include    "CCLFDbItemProvider.h"
#include    "CCLFItemImpl.h"
#include    "CCLFDefaultOperation.h"
#include    "CCLFItemListModelImpl.h"
#include    "CCLFEventHandler.h"
#include    "CCLFDbItemContainer.h"
#include    "MGDebugPrint.h"
#include    "CCLFQueryAdapter.h"

// CONSTANTS
const TInt KCLFObserverArrayGranularity( 2 );
const TInt KCLFChangedItemIdArrayGranularity( 10 );

// -----------------------------------------------------------------------------
// CleanupTempModel
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CleanupTempModel( TAny* aObj )
    {
    static_cast< CCLFEngineImpl::CCLFTempModel* >( aObj )->ResetModelArray();
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFEngineImpl
// -----------------------------------------------------------------------------
//
CCLFEngineImpl::CCLFEngineImpl()
    : iCLFServerProxy( NULL ),
      iMdESession( NULL ),
      iError( KErrNone ),
      iUpdateStartEventHandler( NULL ),
      iUpdateEndEventHandler( NULL ),
      iChangedItemObserverArray( KCLFObserverArrayGranularity ),
      iProcessObserverArray( KCLFObserverArrayGranularity ),
      iDbItemContainer( NULL ),
      iItemProvider( NULL ),
      iQueryAdapter( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::ConstructL()
    {
    iMdESession = CMdESession::NewL( *this );
    if( !iActiveSchedulerWait.IsStarted() )
        {
        iActiveSchedulerWait.Start();
        }
    User::LeaveIfError( iError );
    iCLFServerProxy = CCLFServerProxy::NewL( *iMdESession );
    iQueryAdapter = CCLFQueryAdapter::NewL( *iMdESession );
    iUpdateStartEventHandler = CCLFEventHandler::NewL( *iCLFServerProxy, *this );
    iUpdateEndEventHandler = CCLFEventHandler::NewL( *iCLFServerProxy, *this );
    iDbItemContainer = CCLFDbItemContainer::NewL();
    iItemProvider = CCLFDbItemProvider::NewL( *iDbItemContainer, *iQueryAdapter, iMdESession );
    iUpdateEndEventHandler->ObserverUpdateEndEvent();
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::NewLC
// -----------------------------------------------------------------------------
//
CCLFEngineImpl* CCLFEngineImpl::NewLC()
    {
    CCLFEngineImpl* self = new( ELeave ) CCLFEngineImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::~CCLFEngineImpl
// Destructor
// -----------------------------------------------------------------------------
//
CCLFEngineImpl::~CCLFEngineImpl()
    {
    delete iItemProvider;
    iItemProvider = NULL;
    delete iUpdateStartEventHandler;
    iUpdateStartEventHandler = NULL;
    delete iUpdateEndEventHandler;
    iUpdateEndEventHandler = NULL;
    delete iDbItemContainer;
    iDbItemContainer = NULL;
    delete iCLFServerProxy;
    iCLFServerProxy = NULL;
    iChangedItemObserverArray.Close();
    iProcessObserverArray.Close();
    delete iQueryAdapter;
    iQueryAdapter = NULL;
    delete iMdESession;
    iMdESession = NULL;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::AddChangedItemObserverL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::AddChangedItemObserverL( MCLFChangedItemObserver& aObserver )
    {
    iChangedItemObserverArray.AppendL( &aObserver );
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::RemoveChangedItemObserver
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::RemoveChangedItemObserver( MCLFChangedItemObserver& aObserver )
    {
    const TInt index( iChangedItemObserverArray.Find( &aObserver ) );
    if ( index >= 0 )
        {
        iChangedItemObserverArray.Remove( index );
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::AddCLFProcessObserverL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::AddCLFProcessObserverL( MCLFProcessObserver& aObserver )
    {
    iProcessObserverArray.AppendL( &aObserver );
    if ( !iUpdateStartEventHandler->IsActive() )
        {
        iUpdateStartEventHandler->ObserverUpdateStartEvent();
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::RemoveCLFProcessObserver
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::RemoveCLFProcessObserver( MCLFProcessObserver& aObserver )
    {
    const TInt index( iProcessObserverArray.Find( &aObserver ) );
    if ( index >= 0 )
        {
        iProcessObserverArray.Remove( index );
        }

    StopEventHandling();
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::UpdateItemsL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::UpdateItemsL()
    {
    User::LeaveIfError( iCLFServerProxy->UpdateAllItemsL() );
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::UpdateItemsL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::UpdateItemsL( const TArray<TCLFItemId>& aItemIDArray )
    {
    User::LeaveIfError( iCLFServerProxy->UpdateItemsL( aItemIDArray ) );
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::UpdateItemsL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::UpdateItemsL( TInt aSemanticId, const TDesC8& aOpaqueData )
    {
    User::LeaveIfError( iCLFServerProxy->UpdateItemsL( aSemanticId, aOpaqueData ) );
    }

// ---------------------------------------------------------------------------
// CCLFEngineImpl::HandleSessionOpened
// ---------------------------------------------------------------------------
//
void CCLFEngineImpl::HandleSessionOpened( CMdESession& /* aSession */, const TInt aError )
    {
    MG_DEBUG2( HSO, "[CLF]\t CCLFEngineImpl::HandleSessionOpened %d", aError );
    iActiveSchedulerWait.AsyncStop();

    iError = aError;
    }

// ---------------------------------------------------------------------------
// CCLFEngineImpl::HandleSessionError
// ---------------------------------------------------------------------------
//
void CCLFEngineImpl::HandleSessionError( CMdESession& /*aSession*/, const TInt aError )
    {
    MG_DEBUG2( HSE, "[CLF]\t CCLFEngineImpl::HandleSessionError %d", aError );

    iError = aError;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CreateListModelLC
// -----------------------------------------------------------------------------
//
MCLFItemListModel* CCLFEngineImpl::CreateListModelLC(
                                            MCLFOperationObserver& aObserver )
    {

    CCLFDbItemProvider* itemProvider = CCLFDbItemProvider::NewL( *iDbItemContainer, *iQueryAdapter, iMdESession );
    // self takes ownership of itemProvider.
    CCLFItemListModelImpl* self = CCLFItemListModelImpl::NewLC( itemProvider, aObserver, iChangedItemModel );
    iChangedItemModel.AddObserverL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CreateListModelLC
// -----------------------------------------------------------------------------
//
MCLFItemListModel* CCLFEngineImpl::CreateListModelLC(
                                            MCLFOperationObserver& aObserver,
                                            TResourceReader& aReader )
    {
    CCLFDbItemProvider* itemProvider = CCLFDbItemProvider::NewL( *iDbItemContainer, *iQueryAdapter, iMdESession );
    // self takes ownership of itemProvider.
    CCLFItemListModelImpl* self = CCLFItemListModelImpl::NewLC( itemProvider, aObserver, iChangedItemModel, aReader );
    iChangedItemModel.AddObserverL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::HandleUpdateEndEventL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::HandleUpdateEndEventL( const TInt aError )
    {
    if ( aError == KErrNone )
        {
        // get changed items and add new ones to array
        RArray< TCLFItemId > changedItemIdArray;
        CleanupClosePushL( changedItemIdArray );
        iUpdateEndEventHandler->GetChangedItemsL( changedItemIdArray );
        const TInt count( changedItemIdArray.Count() );
        for ( TInt i = 0; i < count; ++i )
            {
            const TCLFItemId id( changedItemIdArray[ i ] );
            const TInt index(
                    iChangedItemModel.iChangedItemIdArray.Find( id ) );
            if ( index == KErrNotFound )
                {
                iChangedItemModel.iChangedItemIdArray.AppendL( id );
                }
            }
        CleanupStack::PopAndDestroy( &changedItemIdArray );

        if ( iChangedItemModel.iChangedItemIdArray.Count() )
            {
            iDbItemContainer->SetDeprecatedItems(
                                    iChangedItemModel.iChangedItemIdArray );

            iItemProvider->PrepareItemsL(
                                iChangedItemModel.iChangedItemIdArray.Array(),
                                *this );
            }
        else
            {
            // no changed items -> just notify process observers
            NotifyProcessObserversL( ECLFUpdateStop );
            }
        }
    else
        {
        const TInt count( iChangedItemObserverArray.Count() );
        for ( TInt i = 0 ; i < count ; ++i )
            {
            iChangedItemObserverArray[i]->HandleError( aError );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::OperationCompleteL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::OperationCompleteL( const TInt aError )
    {
    // make sure that changed item model is reset
    // array must be reset when changed items are handled (also in leave case)
    CleanupStack::PushL( TCleanupItem( CleanupTempModel,
                                       &iChangedItemModel) );

    if ( aError )
        {
        for( TInt i = iChangedItemObserverArray.Count() - 1; i >=0; i--)
            {
            iChangedItemObserverArray[i]->HandleError( aError );
            }
        }
    else
        {
        // put items to container
        User::LeaveIfError( iItemProvider->GetItems(
                                    iChangedItemModel.iItemArray ) );

        // notify process observers
        NotifyProcessObserversL( ECLFUpdateStop );

        // notify changed item observers
        if ( iChangedItemModel.iChangedItemIdArray.Count() > 0 )
            {
            for( TInt i = iChangedItemObserverArray.Count() - 1; i >=0; i--)
                {
                iChangedItemObserverArray[i]->HandleItemChangeL(
                            iChangedItemModel.iChangedItemIdArray.Array() );
                }

            // notify models
            iChangedItemModel.NotifyObserverL();
            }
        }
    CleanupStack::PopAndDestroy(); // TCleanupItem
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::HandleUpdateStartEventL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::HandleUpdateStartEventL( const TInt aError )
    {
    if ( aError == KErrNone )
        {
        NotifyProcessObserversL( ECLFUpdateStart );
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::NotifyProcessObserversL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::NotifyProcessObserversL( TCLFProcessEvent aEvent )
    {
    const TInt count( iProcessObserverArray.Count() );
    for ( TInt i = 0 ; i < count ; ++i )
        {
        iProcessObserverArray[i]->HandleCLFProcessEventL( aEvent );
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::StopEventHandling
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::StopEventHandling()
    {
    if ( iProcessObserverArray.Count() == 0 )
        {
        // stop "start observing"
        iUpdateStartEventHandler->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::IsItemsForModel
// -----------------------------------------------------------------------------
//
TBool CCLFEngineImpl::CCLFTempModel::IsItemsForModel(
                                        const MDesCArray& aMimeTypes,
                                        const TArray<TInt>& aMediaTypes,
                                        const TArray<TUint>& aModelItemIds )
    {
    // first from item IDs
    TInt count( aModelItemIds.Count() );
    for ( TInt i = 0 ; i < count ; ++i )
        {
        const TInt index( iChangedItemIdArray.FindInOrder( aModelItemIds[ i ] ) );
        if( index >= 0 )
            {
            return ETrue;
            }
        }

    // find second from media types (faster)
    count = aMediaTypes.Count();
    for ( TInt i = 0 ; i < count ; ++i )
        {
        const TInt index( iMediaTypeArray.FindInOrder( aMediaTypes[ i ] ) );
        if ( index >= 0 )
            {
            return ETrue;
            }
        }

    // and mimetypes, final from mimetypes, 
    // because descriptor comparation is slow
    count = aMimeTypes.MdcaCount();
    const TInt mCount( iMimeTypeArray.MdcaCount() );
    for ( TInt i = 0 ; i < count ; ++i )
        {
        // match model mimetypes to changed item(s) mimetypes
        const TDesC& modelMimeType = aMimeTypes.MdcaPoint( i );
        for ( TInt j = 0 ; j < mCount ; ++j )
            {
            const TDesC& changedItemMimeType = iMimeTypeArray.MdcaPoint( j );
            const TInt match( changedItemMimeType.Match( modelMimeType ) );
            if ( match != KErrNotFound )
                {
                return ETrue;
                }
            }
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::CCLFTempModel
// -----------------------------------------------------------------------------
//
CCLFEngineImpl::CCLFTempModel::CCLFTempModel()
    : iItemArray( KCLFChangedItemIdArrayGranularity ),
      iChangedItemIdArray( KCLFChangedItemIdArrayGranularity ),
      iChangedItemProviderObserverArray( KCLFChangedItemIdArrayGranularity ),
      iMimeTypeArray( KCLFChangedItemIdArrayGranularity ),
      iMediaTypeArray( KCLFChangedItemIdArrayGranularity )
    {
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::~CCLFTempModel
// -----------------------------------------------------------------------------
//
CCLFEngineImpl::CCLFTempModel::~CCLFTempModel()
    {
    ResetModelArray();
    iChangedItemProviderObserverArray.Close();
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::AddObserverL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CCLFTempModel::AddObserverL(
                                MCLFChangedItemProviderObserver* aObserver )
    {
    iChangedItemProviderObserverArray.AppendL( aObserver );
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::ResetModelArray
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CCLFTempModel::ResetModelArray()
    {
    iItemArray.ResetAndDestroy();
    iMimeTypeArray.Reset();
    iMediaTypeArray.Reset();
    iChangedItemIdArray.Reset();
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CCLFTempModel::NotifyObserverL()
    {
    PrepareModelL();

    const TInt providerObserverCount(
                                    iChangedItemProviderObserverArray.Count() );
    for ( TInt i = 0 ; i < providerObserverCount ; ++i )
        {
        iChangedItemProviderObserverArray[i]->NewChangedItemsL();
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::PrepareModelL
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CCLFTempModel::PrepareModelL()
    {
    // sort array for faster find
    iChangedItemIdArray.Sort();

    // get mimetypes and media types from items
    const TInt modelCount( iItemArray.Count() );
    for ( TInt i = 0 ; i < modelCount ; ++i )
        {
        MCLFItem* item = iItemArray[ i ];

        TInt32 mediaType( 0 );
        TInt error( item->GetField( ECLFFieldIdMediaType, mediaType ) );
        if ( error == KErrNone &&
            iMediaTypeArray.FindInOrder( mediaType ) == KErrNotFound )
            {
            User::LeaveIfError( iMediaTypeArray.InsertInOrder( mediaType ) );
            }
        TPtrC mimeType;
        error = item->GetField( ECLFFieldIdMimeType, mimeType );
        TInt pos( 0 );
        if ( error == KErrNone &&
            iMimeTypeArray.FindIsq( mimeType, pos ) != 0 ) // not found
            {
            iMimeTypeArray.InsertIsqL( mimeType );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFEngineImpl::CCLFTempModel::ChangedItemProviderRemoveObserver
// -----------------------------------------------------------------------------
//
void CCLFEngineImpl::CCLFTempModel::RemoveChangedItemProviderObserver(
                                MCLFChangedItemProviderObserver* aObserver )
    {
    const TInt index( iChangedItemProviderObserverArray.Find( aObserver ) );
    if ( index >= 0 )
        {
        iChangedItemProviderObserverArray.Remove( index );
        }
    }

//  End of File
