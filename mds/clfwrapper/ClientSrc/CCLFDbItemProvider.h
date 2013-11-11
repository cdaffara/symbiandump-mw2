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


#ifndef CCLFDBITEMPROVIDER_H
#define CCLFDBITEMPROVIDER_H

//  INCLUDES
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>
#include <mdesession.h>
#include <bamdesca.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MCLFItem;
class MCLFItemProviderObserver;
class CCLFDbItemContainer;
class CCLFContainerItem;
class CCLFQueryAdapter;

// CLASS DECLARATION

/**
*  Helper class to get items from CLF server.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFDbItemProvider ) : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFDbItemProvider* NewL( CCLFDbItemContainer& aDbItemContainer,
                                         CCLFQueryAdapter& aQueryAdapter, CMdESession* iMdESession );

        /**
        * Destructor.
        */
        virtual ~CCLFDbItemProvider();

    public: // New functions

        /**
        * Async function for fething items from server.
        * @since Series 60 3.0
        * @param aMimeTypeArray
        * @param aMediaTypeArray
        * @param aObserver item provider observer
        */
        void PrepareItemsL( const MDesCArray& aMimeTypeArray,
                            const TArray<TInt>& aMediaTypeArray,
                            MCLFItemProviderObserver& aObserver );

        void PrepareItemsL( const TArray<TCLFItemId>& aItemIDArray,
                            MCLFItemProviderObserver& aObserver );

        /**
        * Get items.
        * @since Series 60 3.0
        * @param aItemArray
        * @return system-wide error code
        */
        TInt GetItems( RPointerArray<MCLFItem>& aItemArray );

    protected:  // New functions

        /**
        * From CActive
        */
        void DoRunL();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );

        void DoFetchMdSIdDataCachedL();
        void DoFetchMdSDataL();
        void DoFetchMdSIdDataL( RArray< TCLFItemId >& aItemIDArray );
        void CreateCLFItemL( CCLFContainerItem& aCItem );
    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        void RunL();

    private:

        /**
        * C++ default constructor.
        */
        CCLFDbItemProvider( CCLFDbItemContainer& aDbItemContainer,
                                   CCLFQueryAdapter& aQueryAdapter, CMdESession* aMdESession );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        enum TCLFItemProviderState
            {
            EPrepareItemIds = 0x1,
            EPrepareItems,
            EItemsReady
            };

        // Own
        TCLFItemProviderState iProviderState;
        // Ref
        MCLFItemProviderObserver* iObserver;
        // Own
        RPointerArray< MCLFItem > iItemArray;
        // Ref
        CCLFDbItemContainer& iDbItemContainer;
        // Ref
        CCLFQueryAdapter& iQueryAdapter;        
        // Own
        CMdEObjectQuery* iObjectQuery;
        // Ref
        const TDesC* iObjDefStr;
        // Ref
        CMdESession* iMdESession;
    };

#endif      // CCLFDBITEMPROVIDER_H

// End of File
