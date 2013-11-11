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


#ifndef MMEDIACOLLECTIONMANAGERIMPL_H
#define MMEDIACOLLECTIONMANAGERIMPL_H

#include <mediacollectionmanager.h>

/**
 *  This class implements Media Collection Manager functionality
 *
 *  @lib MediaCollectionManager.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CMediaCollectionManagerImpl ) :
                            public CBase,
                            public MMediaCollectionManager
    {
    public:
    
        static CMediaCollectionManagerImpl* NewL();

        virtual ~CMediaCollectionManagerImpl();

    protected:

		/// From MMediaCollectionManager
        void GetCollectionInfosL(
                    RPointerArray<MMediaCollectionInfo>& aCollectionInfoArray ) const;

		/// From MMediaCollectionManager
        MMediaCollectionInfo* CollectionInfoByIdLC(
                    TInt aId ) const;

		/// From MMediaCollectionManager
        TInt CreateCollectionL(
                    const TDesC& aName,
                    const TArray<TInt>& aCollectionItemTypes,
                    TInt32 aCollectionProperties );

		/// From MMediaCollectionManager
        void DeleteCollectionL(
                    TInt aId );

		/// From MMediaCollectionManager
        void RenameCollectionL(
                    TInt aId,
                    const TDesC& aNewName );

		/// From MMediaCollectionManager
        void AddToCollectionL(
                    TInt aId,
                    const MDesCArray& aItemArray,
                    TBool aRemoveFromOtherCollections = ETrue );

		/// From MMediaCollectionManager
        void RemoveFromCollectionL(
                    TInt aId,
                    const MDesCArray& aItemArray );

		/// From MMediaCollectionManager
        void RemoveFromAllCollectionsL(
                    const MDesCArray& aItemArray );

		/// From MMediaCollectionManager
        void GetCollectionItemArrayL(
                    TInt aId,
                    CDesCArray& aItemArray ) const;

		/// From MMediaCollectionManager
        void GetCollectionIdByItemL(
                    const TDesC& aItem,
                    RArray<TInt>& aIdArray ) const;

		/// From MMediaCollectionManager
        void AddCollectionObserverL(
                    MMediaCollectionObserver& aObserver );

		/// From MMediaCollectionManager
        void RemoveCollectionObserverL(
                    MMediaCollectionObserver& aObserver );
        
        /// From MMediaCollectionManager            
        TInt32 GetDriveByIdL( TInt aId );
    private:
    
        CMediaCollectionManagerImpl();
        void ConstructL();

    private:
        /// Ref list to observers
        RPointerArray<MMediaCollectionObserver> iObserverArray;
    };


#endif // MMEDIACOLLECTIONMANAGER_H
