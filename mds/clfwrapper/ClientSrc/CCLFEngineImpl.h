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


#ifndef CCLFENGINEIMPL_H
#define CCLFENGINEIMPL_H

//  INCLUDES
#include "MCLFContentListingEngine.h"
#include "MCLFEventHandlerObserver.h"
#include "MCLFProcessObserver.h"
#include "CCLFServerProxy.h"
#include "MCLFItemProviderObserver.h"
#include "MCLFChangedItemProvider.h"
#include <MCLFItem.h>
#include <badesca.h>
#include <mdesession.h>

// FORWARD DECLARATIONS
class CCLFEventHandler;
class CCLFDbItemContainer;
class CCLFDbItemProvider;
class CCLFQueryAdapter;
class CCLFServerProxy;

// CLASS DECLARATION

/**
*  Implements MCLFContentListingEngine, MCLFEventHandlerObserver and MCLFItemProviderObserver
*  interfaces
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFEngineImpl ) : public CBase,
                                      public MCLFContentListingEngine,
                                      public MCLFEventHandlerObserver,
                                      public MCLFItemProviderObserver,
                                      public MMdESessionObserver
    {
    private:

        NONSHARABLE_CLASS( CCLFTempModel ) : public CBase,
                                             public MCLFChangedItemProvider
            {
            public: // Constructors and destructor

                /**
                * Two-phased constructor.
                */
                CCLFTempModel();

                 /**
                * Destructor.
                */               
                virtual ~CCLFTempModel();

            public: // New functions

                void AddObserverL( MCLFChangedItemProviderObserver* aObserver );

                void ResetModelArray();

                void NotifyObserverL();

            protected: // Functions from base classes

                /**
                * From MCLFChangedItemProvider
                */
                TBool IsItemsForModel(
                                const MDesCArray& aMimeTypes,
                                const TArray<TInt>& aMediaTypes,
                                const TArray<TCLFItemId>& aModelItemIds );

                /**
                * From MCLFChangedItemProvider
                */
                void RemoveChangedItemProviderObserver(
                            MCLFChangedItemProviderObserver* aObserver );

            protected: // New functions

                void PrepareModelL();


            public: // Data
            
                // Own: temp model for changed items
                RPointerArray<MCLFItem> iItemArray;
                RArray<TCLFItemId> iChangedItemIdArray;

            private: // Data

                // Ref. observers
                RPointerArray<MCLFChangedItemProviderObserver>
                                            iChangedItemProviderObserverArray;
                CDesCArraySeg iMimeTypeArray;
                RArray<TInt> iMediaTypeArray;

            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFEngineImpl* NewLC();

        /**
        * Destructor.
        */
        virtual ~CCLFEngineImpl();

    protected:  // Functions from base classes

        /**
        * From MCLFContentListingEngine
        */
        MCLFItemListModel* CreateListModelLC(
                                    MCLFOperationObserver& aObserver );

        /**
        * From MCLFContentListingEngine
        */
        MCLFItemListModel* CreateListModelLC(
                                    MCLFOperationObserver& aObserver,
                                    TResourceReader& aReader );

        /**
        * From MCLFContentListingEngine
        */
        void UpdateItemsL();

        /**
        * From MCLFContentListingEngine
        */
        void UpdateItemsL( const TArray<TCLFItemId>& aItemIDArray );

        /**
        * From MCLFContentListingEngine
        */
        void AddChangedItemObserverL(
                                    MCLFChangedItemObserver& aObserver );

        /**
        * From MCLFContentListingEngine
        */
        void RemoveChangedItemObserver(
                                    MCLFChangedItemObserver& aObserver );

        /**
        * From MCLFContentListingEngine
        */
        void UpdateItemsL( TInt aSemanticId, const TDesC8& aOpaqueData );

        /**
        * From MCLFContentListingEngine
        */
        void AddCLFProcessObserverL(
                        MCLFProcessObserver& aObserver );

        /**
        * From MCLFContentListingEngine
        */
        void RemoveCLFProcessObserver(
                        MCLFProcessObserver& aObserver );

        /**
        * From MCLFEventHandlerObserver
        */
        void HandleUpdateEndEventL( TInt aError );

        /**
        * From MCLFEventHandlerObserver
        */
        void HandleUpdateStartEventL( TInt aError );

        /**
        * From MCLFItemProviderObserver
        */
        void OperationCompleteL( TInt aError );

        /**
         * From MMdESessionObserver
         */
        void HandleSessionOpened( CMdESession& aSession, TInt aError );

        /**
         * From MMdESessionObserver
         */
        void HandleSessionError( CMdESession& aSession, TInt aError );
        
        
    private:  // New functions

        void StopEventHandling();

        void NotifyProcessObserversL( TCLFProcessEvent aEvent );

        static void CleanupTempModel( TAny* aObj );

    private:

        /**
        * C++ default constructor.
        */
        CCLFEngineImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        // Own
        CCLFServerProxy* iCLFServerProxy;
        // Own
        CMdESession* iMdESession;
        // Own
        TInt iError;
        // Own
        CActiveSchedulerWait iActiveSchedulerWait;
        // Own
        CCLFEventHandler* iUpdateStartEventHandler;
        // Own
        CCLFEventHandler* iUpdateEndEventHandler;
        // Ref. observers
        RPointerArray< MCLFChangedItemObserver > iChangedItemObserverArray;
        // Ref. observers
        RPointerArray< MCLFProcessObserver > iProcessObserverArray;
        // Own:
        CCLFDbItemContainer* iDbItemContainer;
        // Own. to get changed items
        CCLFDbItemProvider* iItemProvider;
        // Own:
        CCLFTempModel iChangedItemModel;
        // Own
        CCLFQueryAdapter* iQueryAdapter;        
    };

#endif      // CCLFENGINEIMPL_H

// End of File
