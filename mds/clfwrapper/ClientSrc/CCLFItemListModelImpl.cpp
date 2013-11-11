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
#include    "CCLFItemListModelImpl.h"
#include    "CCLFDbItemProvider.h"
#include    "CCLFDefaultOperation.h"
#include    <MCLFOperationObserver.h>
#include    <MCLFItem.h>
#include    <MCLFSortingStyle.h>
#include    "MGTracePrint.h"
#include    <ContentListingFactory.h>
#include    <barsread.h>
#include    <collate.h>
#include    "MGDebugPrint.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

// CONSTANTS
const TInt KCLFResourceVersionNumber( 1 );
const TInt KCLFEmptyArrayGranularity( 1 );
const TInt KCLFSortingStyleArrayGranularity( 3 );
const TInt KCLFResourceVersion1( 1 );
const TInt KCLFResourceVersion2( 2 );

_LIT( KCLFAllMimeTypeMatchString, "*" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::CCLFItemListModelImpl
// -----------------------------------------------------------------------------
//
CCLFItemListModelImpl::CCLFItemListModelImpl(
                                        CCLFDbItemProvider* aItemProvider,
                                        MCLFOperationObserver& aObserver,
                                        MCLFChangedItemProvider& aChangedItemProvider )
    : iMimeTypeArray( NULL ),
      iMediaTypeArray( NULL ),
      iGrouper( NULL ),
      iSorter( NULL ),
      iPostFilter( NULL ),
      iItemProvider( aItemProvider ),
      iOperationObserver( aObserver ),
      iSortingStyleArray( KCLFSortingStyleArrayGranularity ),
      iChangedItemProvider( aChangedItemProvider ),
      iIsItemsFetched( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::ConstructL()
    {
    iMediaTypeArray = new ( ELeave ) CArrayFixFlat< TInt >( KCLFEmptyArrayGranularity );
    iMimeTypeArray = new ( ELeave ) CDesCArrayFlat( KCLFEmptyArrayGranularity );
    iDefaultOperation = CCLFDefaultOperation::NewL();
    iSorter = iDefaultOperation;
    iGrouper = iDefaultOperation;
    iPostFilter = iDefaultOperation;
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::ConstructL( TResourceReader& aResource )
   {
    ConstructL();
    const TInt version( aResource.ReadInt16() );

    // resource versions are defined in Content Listing framework .rh file
    if( version != KCLFResourceVersion1 &&
        version != KCLFResourceVersion2 )
        {
        User::Leave( KErrNotSupported );
        }
    SetGroupingStyle( TCLFGrouping( aResource.ReadInt32() ) );
    SetWantedMimeTypesL( aResource );
    SetWantedMediaTypesL( aResource );


    const TInt count(
        version == KCLFResourceVersion2 ? aResource.ReadInt16() - 1 : 0 );

    MCLFSortingStyle* sortingStyle =
                        ContentListingFactory::NewSortingStyleLC( aResource );
    iSortingStyleArray.AppendL( sortingStyle ); // takes ownership
    CleanupStack::Pop(); // sortingStyle
    iDefaultOperation->SetSortingStyle( sortingStyle);

    for( TInt i = 0 ; i < count ; ++i )
        {
        MCLFSortingStyle* sortingStyle =
                        ContentListingFactory::NewSortingStyleLC( aResource );
        iSortingStyleArray.AppendL( sortingStyle ); // takes ownership
        CleanupStack::Pop(); // sortingStyle
        iDefaultOperation->AppendSortingStyleL( *sortingStyle );
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::NewLC
// -----------------------------------------------------------------------------
//
CCLFItemListModelImpl* CCLFItemListModelImpl::NewLC(
                                            CCLFDbItemProvider* aItemProvider,
                                            MCLFOperationObserver& aObserver,
                                            MCLFChangedItemProvider& aChangedItemProvider )
    {
    CCLFItemListModelImpl* self = new( ELeave ) CCLFItemListModelImpl( aItemProvider, aObserver, aChangedItemProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::NewLC
// -----------------------------------------------------------------------------
//
CCLFItemListModelImpl* CCLFItemListModelImpl::NewLC(
                                            CCLFDbItemProvider* aItemProvider,
                                            MCLFOperationObserver& aObserver,
                                            MCLFChangedItemProvider& aChangedItemProvider,
                                            TResourceReader& aResource)
    {
    CCLFItemListModelImpl* self = new( ELeave ) CCLFItemListModelImpl( aItemProvider, aObserver, aChangedItemProvider );
    CleanupStack::PushL( self );
    self->ConstructL( aResource );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::~CCLFItemListModelImpl
// Destructor
// -----------------------------------------------------------------------------
//
CCLFItemListModelImpl::~CCLFItemListModelImpl()
    {
    iChangedItemProvider.RemoveChangedItemProviderObserver( this );
    delete iMimeTypeArray;
    iMimeTypeArray = NULL;
    delete iMediaTypeArray;
    iMediaTypeArray = NULL;
    iItemArray.ResetAndDestroy();
    delete iItemProvider;
    iResultArray.Close();
    delete iDefaultOperation;
    iSortingStyleArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::Item
// -----------------------------------------------------------------------------
//
const MCLFItem& CCLFItemListModelImpl::Item( TInt aIndex ) const
    {
    return *( iResultArray[aIndex] );
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ItemCount
// -----------------------------------------------------------------------------
//
TInt CCLFItemListModelImpl::ItemCount() const
    {
    return iResultArray.Count();
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetSortingStyle
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetSortingStyle( MCLFSortingStyle* aSortingStyle )
    {
    iDefaultOperation->SetSortingStyle( aSortingStyle );
    iSortingStyleArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::AppendSecondarySortingStyleL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::AppendSecondarySortingStyleL(
                                            MCLFSortingStyle& aSortingStyle )
    {
    iDefaultOperation->AppendSortingStyleL( aSortingStyle );
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetCustomSorter
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetCustomSorter( MCLFCustomSorter* aCustomSorter )
    {
    if( aCustomSorter )
        {
        iSorter = aCustomSorter;
        }
    else
        {
        iSorter = iDefaultOperation;
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetGroupingStyle
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetGroupingStyle( TCLFGrouping aGrouping )
    {
    iDefaultOperation->SetGrouping( aGrouping );
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetCustomGrouper
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetCustomGrouper(
                                            MCLFCustomGrouper* aCustomGrouper )
    {
    if( aCustomGrouper )
        {
        iGrouper = aCustomGrouper;
        }
    else
        {
        iGrouper = iDefaultOperation;
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetPostFilter
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetPostFilter( MCLFPostFilter* aPostFilter )
    {
    if( aPostFilter )
        {
        iPostFilter = aPostFilter;
        }
    else
        {
        iPostFilter = iDefaultOperation;
        }
    }


// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetWantedMimeTypesL
// Method makes new array, because if we can't make new then old array is used
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetWantedMimeTypesL( const MDesCArray& aMimeTypes )
    {
    const TInt mimeTypeCount( aMimeTypes.MdcaCount() );
    
    if( mimeTypeCount > 0 )
        {
        if( mimeTypeCount == 1 )
            {
            TCollationMethod m = *Mem::CollationMethodByIndex( 0 );
            m.iFlags = ( TCollationMethod::EIgnoreNone | TCollationMethod::EFoldCase );   
            
            const TDesC& mimeType = aMimeTypes.MdcaPoint( 0 );
            if ( mimeType.CompareC( KCLFAllMimeTypeMatchString, 3, &m ) == 0 )
                {
                ResetMimeTypeArrayL();
                return;
                }
            }
        CDesCArray* array = new (ELeave) CDesCArrayFlat( mimeTypeCount );
        CleanupStack::PushL( array );
        for( TInt i = mimeTypeCount - 1; i >=0; i--)
            {
            array->AppendL( aMimeTypes.MdcaPoint( i ) );
            }
        CleanupStack::Pop( array );
        delete iMimeTypeArray;
        iMimeTypeArray = array;
        }
    else
        {
        ResetMimeTypeArrayL();
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetWantedMimeTypesL
// Method makes new array, because if we can't make new then old array is used
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetWantedMimeTypesL( TResourceReader& aResource )
    {
    CheckVersionL( aResource, KCLFResourceVersionNumber );
    const TInt numMimeTypes( aResource.ReadInt16() );
    if ( numMimeTypes > 0 )
        {
        // TResourceReader re-reads the length, have to rewind
        aResource.Rewind( sizeof( TInt16 ) );
        CDesCArray* array = aResource.ReadDesCArrayL();
        delete iMimeTypeArray;
        iMimeTypeArray = array;
        }
    else
        {
        ResetMimeTypeArrayL();
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetWantedMediaTypesL
// Method makes new array, because if we can't make new then old array is used
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetWantedMediaTypesL(
                            const TArray<TInt>& aMediaTypes )
    {
    const TInt count( aMediaTypes.Count() );
    if( count > 0 )
        {
        CArrayFix<TInt>* array =
                        new ( ELeave ) CArrayFixFlat<TInt>( count );
        CleanupStack::PushL( array );
        for(TInt i = count - 1; i >=0; i--)
            {
            array->AppendL( aMediaTypes[i] );
            }
        CleanupStack::Pop( array );
        delete iMediaTypeArray;
        iMediaTypeArray = array;
        }
    else
        {
        ResetMediaTypeArrayL();
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::SetWantedMediaTypesL
// Method makes new array, because if we can't make new then old array is used
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::SetWantedMediaTypesL( TResourceReader& aResource )
    {
    CheckVersionL( aResource, KCLFResourceVersionNumber );
    const TInt numMediaTypes( aResource.ReadInt16() );
    if ( numMediaTypes > 0 )
        {
        CArrayFix<TInt>* array =
                new ( ELeave ) CArrayFixFlat<TInt>( numMediaTypes );
        CleanupStack::PushL( array );
        for( TInt i = numMediaTypes - 1; i >=0; i--)
            {
            array->AppendL( aResource.ReadInt32() );
            }
        CleanupStack::Pop( array );
        delete iMediaTypeArray;
        iMediaTypeArray = array;
        }
    else
        {
        ResetMediaTypeArrayL();
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::RefreshL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::RefreshL()
    {
    MG_TRACE1( KCLFTrace, "CLF list model refresh start" )

    iIsItemsFetched = EFalse;
    iItemArray.ResetAndDestroy();
    iItemProvider->PrepareItemsL( *iMimeTypeArray,
                             iMediaTypeArray->Array(),
                             *this );

    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::RefreshL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::RefreshL( TInt32 aRefreshType )
    {
    RPointerArray<MCLFItem> tmpArray;
    CleanupClosePushL( tmpArray );

    MG_TRACE1( KCLFTrace, "CLF list model refresh post filter start" )

    if( aRefreshType & ECLFRefreshPostFilter )
        {
        iPostFilter->FilterItemsL( iItemArray.Array(), tmpArray );
        }
    else
        {
        CopyArrayL( iItemArray.Array(), tmpArray );
        }
    iResultArray.Reset();

    MG_TRACE1( KCLFTrace1, "CLF list model refresh grouping start" )

    if( aRefreshType & ECLFRefreshGrouping )
        {
        iGrouper->GroupItemsL( tmpArray.Array(), iResultArray );
        }
    else
        {
        CopyArrayL( tmpArray.Array(), iResultArray );
        }
    CleanupStack::PopAndDestroy( &tmpArray );

    MG_TRACE1( KCLFTrace2, "CLF list model refresh sorting start" )

    if( aRefreshType & ECLFRefreshSorting )
        {
        iSorter->SortItemsL( iResultArray );
        }

    MG_TRACE1( KCLFTrace3, "CLF list model refresh sorting done" )
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::CopyArrayL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::CopyArrayL( const TArray<MCLFItem*>& aSource,
                                        RPointerArray<MCLFItem>& aDest )
    {
    CleanupResetAndDestroyPushL( aDest );
    for( TInt i = aSource.Count() - 1; i >=0; i--)
        {
        aDest.AppendL( aSource[i] );
        }
    CleanupStack::Pop( &aDest );
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::CancelRefresh
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::CancelRefresh()
    {
    iItemProvider->Cancel();
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::OperationCompleteL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::OperationCompleteL( TInt aError )
    {
    MG_TRACE1( KCLFTrace, "CLF list model items ready" )

    TInt error( aError );
    if ( error == KErrNone )
        {
        iIsItemsFetched = ETrue;
        iResultArray.Reset(); // reset result array before items are deleted
        iItemArray.ResetAndDestroy();
        error = iItemProvider->GetItems( iItemArray );
        if ( error == KErrNone )
            {
            RefreshL( ECLFRefreshAll );
            }
        }
    iOperationObserver.HandleOperationEventL( ECLFRefreshComplete, error );
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::CheckVersionL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::CheckVersionL( TResourceReader& aResource,
                                           TInt aVersion )
    {
    const TInt version( aResource.ReadInt16() );
    if( version != aVersion )
        {
        User::Leave( KErrNotSupported );
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ResetMimeTypeArrayL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::ResetMimeTypeArrayL()
    {
    CDesCArray* array =
                new (ELeave) CDesCArrayFlat( KCLFEmptyArrayGranularity );
    delete iMimeTypeArray;
    iMimeTypeArray = array;
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ResetMediaTypeArrayL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::ResetMediaTypeArrayL()
    {
    CArrayFix<TInt>* array =
                        new ( ELeave ) CArrayFixFlat<TInt>(
                                            KCLFEmptyArrayGranularity );
    delete iMediaTypeArray;
    iMediaTypeArray = array;
    }

// -----------------------------------------------------------------------------
// CCLFItemListModelImpl::ChangedItemsL
// -----------------------------------------------------------------------------
//
void CCLFItemListModelImpl::NewChangedItemsL()
    {
    if( iIsItemsFetched )
        {
        const TInt count( iItemArray.Count() );
        RArray<TCLFItemId> idArray( count == 0 ? 1 : count );
        CleanupClosePushL( idArray );
        for( TInt i = count - 1; i >=0; i--)
            {
            idArray.AppendL( iItemArray[i]->ItemId() );
            }
        
        if( iChangedItemProvider.IsItemsForModel( *iMimeTypeArray,
                                                  iMediaTypeArray->Array(),
                                                  idArray.Array() ) )
            {
            iOperationObserver.HandleOperationEventL( ECLFModelOutdated, KErrNone );
            }
        CleanupStack::PopAndDestroy( &idArray );
        }
    }

//  End of File
