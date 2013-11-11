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
* Description: 
*
*/


#include "mediacollectionmanagerimpl.h"
#include "CLFConsts.h"
#include "CLFPanics.h"

const TInt KCLFMCMObserverArrayGranularity( 2 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::CMediaCollectionManagerImpl
// -----------------------------------------------------------------------------
//
CMediaCollectionManagerImpl::CMediaCollectionManagerImpl()
    : iObserverArray( KCLFMCMObserverArrayGranularity )
    {
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::NewL
// -----------------------------------------------------------------------------
//
CMediaCollectionManagerImpl* CMediaCollectionManagerImpl::NewL()
    {
    CMediaCollectionManagerImpl* self =
                        new (ELeave) CMediaCollectionManagerImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::~CMediaCollectionManagerImpl
// -----------------------------------------------------------------------------
//
CMediaCollectionManagerImpl::~CMediaCollectionManagerImpl()
    {
    iObserverArray.Close();
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::GetCollectionInfosL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::GetCollectionInfosL(
			RPointerArray<MMediaCollectionInfo>& /* aCollectionInfoArray */ ) const
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::CollectionInfoByIdLC
// -----------------------------------------------------------------------------
//
MMediaCollectionInfo* CMediaCollectionManagerImpl::CollectionInfoByIdLC(
                TInt /* aId */ ) const
    {
    User::Leave( KErrNotSupported );

    // to stop compile warnings
    MMediaCollectionInfo* info = NULL;
    return info;
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::CreateCollectionL
// -----------------------------------------------------------------------------
//
TInt CMediaCollectionManagerImpl::CreateCollectionL(
                const TDesC& /* aName */,
                const TArray<TInt>& /* aCollectionItemTypes */,
                TInt32 /* aCollectionProperties */ )
    {
    User::Leave( KErrNotSupported );

    // to stop compile warnings
    TInt collectionID( 0 );
    return collectionID;
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::DeleteCollectionL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::DeleteCollectionL(
                TInt /* aId */ )
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::RenameCollectionL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::RenameCollectionL(
                TInt /* aId */,
                const TDesC& /* aNewName */ )
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::AddToCollectionL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::AddToCollectionL(
                TInt /* aId */,
                const MDesCArray& /* aItemArray */,
                TBool /* aRemoveFromOtherCollections */ )
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::RemoveFromCollectionL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::RemoveFromCollectionL(
                TInt /* aId */,
                const MDesCArray& /* aItemArray */ )
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::RemoveFromAllCollectionsL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::RemoveFromAllCollectionsL(
                    const MDesCArray& /* aItemArray */ )
    {
#if ( 0 )
    User::Leave( KErrNotSupported );
#endif
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::GetCollectionItemArrayL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::GetCollectionItemArrayL(
                TInt /* aId */,
                CDesCArray& /* aItemArray */ ) const
    {
    User::Leave( KErrNotSupported );

    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::GetCollectionIdByItemL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::GetCollectionIdByItemL(
                const TDesC& /* aItem */,
                RArray<TInt>& /* aIdArray */ ) const
    {
#if (0)
    User::Leave( KErrNotSupported );
#endif
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::AddCollectionObserverL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::AddCollectionObserverL(
                MMediaCollectionObserver& aObserver )
    {
    iObserverArray.AppendL( &aObserver );
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::RemoveCollectionObserverL
// -----------------------------------------------------------------------------
//
void CMediaCollectionManagerImpl::RemoveCollectionObserverL(
                MMediaCollectionObserver& aObserver )
    {
    const TInt index( iObserverArray.Find( &aObserver ) );
    if( index >= 0 )
        {
        iObserverArray.Remove( index );
        }
    }

// -----------------------------------------------------------------------------
// CMediaCollectionManagerImpl::GetDriveById
// -----------------------------------------------------------------------------
//
TInt32 CMediaCollectionManagerImpl::GetDriveByIdL( TInt /* aId */ )
    {
    User::Leave( KErrNotSupported );
    
    // to stop compile warnings
    const TInt drive( 0 );
    return drive;
    }

// End of file
