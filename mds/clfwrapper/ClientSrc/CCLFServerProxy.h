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


#ifndef CCLFSERVERPROXY_H
#define CCLFSERVERPROXY_H

//  INCLUDES
#include <e32std.h>
#include <badesca.h>
#include <f32file.h>
#include <mdesession.h>
#include <mdeobjectquery.h>
#include <harvesterclient.h>
#include <CLFContentListing.h>
#include "CLFConsts.h"

// FORWARD DECLARATIONS
class CActiveSchedulerWait;

// CLASS DECLARATION

/**
*  Abstract base class for handling different types of UpdateItems requests.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CUpdateItemsHandler ) : public CBase
    {
    public:
        virtual void StartHandlingL() = 0;
        
        virtual void HandleObjectNotification( TItemId aId, TObserverNotificationType aType ) = 0;

        virtual TBool WasNotificationHandled() const;

        virtual TBool AllDone() const = 0;

        virtual void StartScheduler();

        virtual void AsyncStopScheduler();
        
        virtual void SetRemoveObserverFlag( TBool aRemove );

    public:

        /**
         * Destructor.
         */
        virtual ~CUpdateItemsHandler() {}

    protected:

        /**
         * C++ default constructor.
         */
        inline CUpdateItemsHandler();

    protected:
        // Ref.
        CActiveSchedulerWait iScheduler;
        // Own.
        TBool iWasNotificationHandled;
        // Ref.
        MMdEObjectObserver* iParentObserver;
        // Ref.
        MMdEObjectPresentObserver* iParentPresentObserver;
        // Own
        TBool iRemoveObserver;
    };

/**
*  ID-based UpdateItems request handler.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CUpdateIDsHandler ) : public CUpdateItemsHandler
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CUpdateIDsHandler* NewL( const TArray< TCLFItemId >& aItemIDArray );

        /**
         * Destructor.
         */
        virtual ~CUpdateIDsHandler();

    protected: // Functions from base classes

        /**
         * From CUpdateItemsHandler
         */
        virtual void StartHandlingL();
        
        /**
         * From CUpdateItemsHandler
         */
        void HandleObjectNotification( TItemId aId, TObserverNotificationType aType );

        /**
         * From CUpdateItemsHandler
         */
        TBool AllDone() const;

    private:

        /**
         * C++ default constructor.
         */
        inline CUpdateIDsHandler( const TArray< TCLFItemId >& aItemIDArray );

    private:
        // Own.
        RArray< TCLFItemId > iIdsPendingUpdate;
    };

/**
*  UpdateItems request handler for requests with KCLFItemsDeletedSemanticId.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CItemsDeletedHandler ) : public CUpdateItemsHandler
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CItemsDeletedHandler* NewL( CMdESession& aMdESession, 
                                                                const CDesCArray& aUriArray, 
                                                                MMdEObjectObserver* aParentObserver,
                                                                MMdEObjectPresentObserver* aParentPresentObserver );

        /**
         * Destructor.
         */
        virtual ~CItemsDeletedHandler();

    protected: // Functions from base classes

        /**
         * From CUpdateItemsHandler
         */
        virtual void StartHandlingL();

        /**
         * From CUpdateItemsHandler
         */
        void HandleObjectNotification( TItemId aId, TObserverNotificationType aType );

        /**
         * From CUpdateItemsHandler
         */
        TBool AllDone() const;

    private:

        /**
         * C++ default constructor.
         */
        inline CItemsDeletedHandler( CMdESession& aMdESession );

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        inline void ConstructL( const CDesCArray& aUriArray, 
                                             MMdEObjectObserver* aParentObserver,
                                             MMdEObjectPresentObserver* aParentPresentObserver );

    private:
        // Own.
        RPointerArray< CMdEObject > iObjectsPendingRemoval;
        // Own.
        RArray< TCLFItemId > iIdsPendingRemoval;
        // Ref.
        CMdESession& iMdESession;
    };

/**
*  UpdateItems request handler for requests with KCLFUpdateFoldersSemanticId.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CUpdateFoldersHandler ) : public CUpdateItemsHandler,
                                             public MMdEQueryObserver,
                                             public MHarvestObserver
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUpdateFoldersHandler* NewL( CMdESession& aMdESession, 
                                                                 const CDesCArray& aUriArray,
                                                                 MMdEObjectObserver* aParentObserver,
                                                                 MMdEObjectPresentObserver* aParentPresentObserver,
                                                                 RFs& aFs,
                                                                 RHarvesterClient& aHarvester,
                                                                 CDesCArray* aSupportedTypes );

        /**
        * Destructor.
        */
        virtual ~CUpdateFoldersHandler();

    protected: // Functions from base classes

        /**
         * From CUpdateItemsHandler
         */
        virtual void StartHandlingL();

        /**
         * From CUpdateItemsHandler
         */
        void HandleObjectNotification( TItemId aId, TObserverNotificationType aType );

        /**
         * From CUpdateItemsHandler
         */
        TBool AllDone() const;

        /**
         * From MMdEQueryObserver
         */
        void HandleQueryNewResults( CMdEQuery& aQuery, TInt aFirstNewItemIndex, TInt aNewItemCount );

        /**
         * From MMdEQueryObserver
         */
        void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        inline CUpdateFoldersHandler( CMdESession& aMdESession, 
                                                         RFs& aFs, 
                                                         RHarvesterClient& aHarvester,
                                                         CDesCArray* aSupportedTypes );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        inline void ConstructL( const CDesCArray& aUriArray, 
                                             MMdEObjectObserver* aParentObserver, 
                                             MMdEObjectPresentObserver* aParentPresentObserver );

        void ScanNodeForUrisL( const TDesC16& aNodeName, CDesCArray& aUriArray, CDesCArray& aFolderArray );

        void DetermineIdsToRemoveL ( const CMdEObjectQuery& aQuery );

        void DetermineIdsToUpdateL ( const CDesCArray& aUriArray );

        void DoQueryL( CMdEObjectQuery*& aQuery, const CDesCArray& aFolderArray );

        void DoHarvestL( const TDesC& aUri );

        void HarvestingComplete( TDesC& aURI, TInt aError );
        
        TBool IsSupportedType( const TDesC& aExtension );

    private:
        // Ref.
        RFs iFs;
        // Own.
        CDesCArraySeg iUrisFound;
        // Own.
        CDesCArraySeg iFoldersFound;
        // Own.
        RArray< TCLFItemId > iIdsPendingRemoval;
        // Own
        RArray< TCLFItemId >  iIdsPendingUpdate;
        // Own
        RArray< TCLFItemId > iIdsHandled;
        // Ref.
        CMdESession& iMdESession;
        // Ref.
        RHarvesterClient iHarvester;
        // Own.
        TFileName iHarvestingFile;
        // Own.
        TInt iHarvestError;
        // Own
        TBool iStartingHandling;
        // Not owned.
        CDesCArray* iExtensionArray;
        
        TBool iHarvestingOngoing;
    };

/**
*  Client-side proxy class for simulating the CLF server.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CCLFServerProxy ) : public CBase,
                                       public MMdEObjectObserver,
                                       public MHarvesterEventObserver,//clf_wrapper_improve add
                                       public MMdEObjectPresentObserver
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFServerProxy* NewL( CMdESession& aMdESession );

        /**
        * Destructor.
        */
        virtual ~CCLFServerProxy();

    public:

        void GetUpdateEndEvent( TRequestStatus& aRequestStatus );

        TInt CancelGetEvent();

        void GetUpdateStartEvent( TRequestStatus& aRequestStatus );

        TInt CancelGetUpdateStartEvent();

        TInt FetchItemListData( RArray< TCLFItemId >& aItemIDArray );

        TInt UpdateItemsL( const TArray< TCLFItemId >& aItemIDArray );

        TInt UpdateItemsL( TInt aSemanticId, const TDesC8& aOpaqueData );

        TInt UpdateAllItemsL();

        /**
         * From MMdEObjectObserver
         */
         void HandleObjectNotification( CMdESession& aSession,
                                        TObserverNotificationType aType,
                                        const RArray<TItemId>& aObjectIdArray );
         
         /**
          * From MMdEObjectPresentObserver
          */
          void HandleObjectPresentNotification( CMdESession& aSession, 
                                                                         TBool aPresent, 
                                                                         const RArray<TItemId>& aObjectIdArray );
        // clf_wrapper_improve add
      	void HarvestingUpdated( 
      				HarvesterEventObserverType aHEObserverType, 
      				HarvesterEventState aHarvesterEventState,
      				TInt aItemsLeft );
      	// clf_wrapper_improve end
    private:
        /**
        * C++ default constructor.
        */
        inline CCLFServerProxy( CMdESession& aMdESession );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        inline void ConstructL();

        void StartHandlingL( CUpdateItemsHandler* aHandler );

         void NotifyUpdateEvent( const TCLFServerOpCodes aOpCode );

         void DeSerializeL( const TDesC8& aData, CDesCArray& aDataArray );

         void ProcessObjectId( const CMdEObject* aObject );

         void GetEvent( TRequestStatus& aRequestStatus, TCLFServerOpCodes aOpcode );

         void CancelGetEvent( TCLFServerOpCodes aOpcode );
         
         void PopulateSupportedExtensionL();

    private: // Data
        // Own.
        RPointerArray< TRequestStatus > iStatusArray;
        // Own.
        RArray< TCLFServerOpCodes > iOpCodeArray;
        // Own.
        RArray< TCLFItemId > iUpdatedIds;
        // Own.
        RPointerArray< CUpdateItemsHandler > iUpdateItemsHandlerArray;
        // Ref.
        CMdESession& iMdESession;
        // Own.
        CDesCArray* iUriArray;
        // Own.
        CDesCArray* iExtensionArray;
        // Own.
        RFs iFs;

        RHarvesterClient iHC;
        
        TBool iHEStateFinished;
        
        TBool iNeedNotifyCLFGetUpdateEvent;
        
        TInt iDelayNotifyCLFGetUpdateEventCount;
    };

#endif // CCLFSERVERPROXY_H

// End of File
