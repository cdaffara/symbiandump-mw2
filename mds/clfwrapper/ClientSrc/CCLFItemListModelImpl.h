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



#ifndef CCLFITEMLISTMODELIMPL_H
#define CCLFITEMLISTMODELIMPL_H

//  INCLUDES
#include <MCLFItemListModel.h>
#include "MCLFItemProviderObserver.h"
#include "MCLFChangedItemProvider.h"
#include <badesca.h>

// FORWARD DECLARATIONS
class MCLFOperationObserver;
class CCLFDefaultOperation;
class TResourceReader;
class CCLFDbItemProvider;
class CCLFDbItemContainer;

// CLASS DECLARATION

/**
*  This class implements MCLFItemListModel and MCLFItemProviderObserver
*  interfaces
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFItemListModelImpl ) : public CBase,
                                             public MCLFItemListModel,
                                             public MCLFItemProviderObserver,
                                             public MCLFChangedItemProviderObserver

    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFItemListModelImpl* NewLC(
                            CCLFDbItemProvider* aItemProvider,
                            MCLFOperationObserver& aObserver,
                            MCLFChangedItemProvider& aChangedItemProvider );

        /**
        * Two-phased constructor.
        */
        static CCLFItemListModelImpl* NewLC(
                            CCLFDbItemProvider* aItemProvider,
                            MCLFOperationObserver& aObserver,
                            MCLFChangedItemProvider& aChangedItemProvider,
                            TResourceReader& aResource );

        /**
        * Destructor.
        */
        virtual ~CCLFItemListModelImpl();

    protected:  // New functions

        void ReadMimeTypeListL( TResourceReader& aResource );
        void ReadMediaTypeListL( TResourceReader& aResource );
        void CheckVersionL( TResourceReader& aResource,
                            TInt aVersion );
        void CopyArrayL( const TArray<MCLFItem*>& aSource,
                         RPointerArray<MCLFItem>& aDest );
        void ResetMimeTypeArrayL();
        void ResetMediaTypeArrayL();

    protected:  // Functions from base classes

        /**
        * From MCLFItemListModel
        */
        const MCLFItem& Item( TInt aIndex ) const;

        /**
        * From MCLFItemListModel
        */
        TInt ItemCount() const;

        /**
        * From MCLFItemListModel
        */
        void SetSortingStyle( MCLFSortingStyle* aSortingStyle );

        /**
        * From MCLFItemListModel
        */
        void AppendSecondarySortingStyleL(
                                MCLFSortingStyle& aSortingStyle );

        /**
        * From MCLFItemListModel
        */
        void SetCustomSorter( MCLFCustomSorter* aCustomSorter );

        /**
        * From MCLFItemListModel
        */
        void SetGroupingStyle( TCLFGrouping aGrouping );

        /**
        * From MCLFItemListModel
        */
        void SetCustomGrouper( MCLFCustomGrouper* aCustomGrouper );

        /**
        * From MCLFItemListModel
        */
        void SetPostFilter( MCLFPostFilter* aPostFilter );

        /**
        * From MCLFItemListModel
        */
        void SetWantedMimeTypesL( const MDesCArray& aMimeTypes );

        /**
        * From MCLFItemListModel
        */
        void SetWantedMimeTypesL( TResourceReader& aResource );

        /**
        * From MCLFItemListModel
        */
        void SetWantedMediaTypesL(
                            const TArray<TInt>& aMediaTypes );

        /**
        * From MCLFItemListModel
        */
        void SetWantedMediaTypesL( TResourceReader& aResource );

        /**
        * From MCLFItemListModel
        */
        void RefreshL( );

        /**
        * From MCLFItemListModel
        */
        void RefreshL( TInt32 aRefreshType );

        /**
        * From MCLFItemListModel
        */
        void CancelRefresh();

        /**
        * From MCLFItemProviderObserver
        */
        void OperationCompleteL( TInt aError );

        void NewChangedItemsL(); 

    private:

        /**
        * C++ default constructor.
        */
        CCLFItemListModelImpl(
                CCLFDbItemProvider* aItemProvider,
                MCLFOperationObserver& aObserver,
                MCLFChangedItemProvider& aChangedItemProvider );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void ConstructL( TResourceReader& aResource );

    private:    // Data
        // Own. items
        RPointerArray< MCLFItem > iItemArray;
        // Ref. items
        RPointerArray< MCLFItem > iResultArray;
        // Own mime type list
        CDesCArray* iMimeTypeArray;
        // Own media type list
        CArrayFix< TInt >* iMediaTypeArray;
        // Ref.
        MCLFCustomGrouper* iGrouper;
        // Ref.
        MCLFCustomSorter* iSorter;
        // Ref.
        MCLFPostFilter* iPostFilter;
        // Own
        CCLFDbItemProvider* iItemProvider;
        // Ref.
        MCLFOperationObserver& iOperationObserver;
        // Own.
        CCLFDefaultOperation* iDefaultOperation;
        // Own: this arrays ows sortings styles that are read from resource.
        RPointerArray< MCLFSortingStyle > iSortingStyleArray;
        // Ref:
        MCLFChangedItemProvider& iChangedItemProvider;
        // Own
        TBool iIsItemsFetched;
    };

#endif      // CCLFITEMLISTMODELIMPL_H

// End of File
