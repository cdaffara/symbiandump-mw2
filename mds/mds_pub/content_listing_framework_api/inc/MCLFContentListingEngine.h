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


#ifndef MCLFCONTENTLISTINGENGINE_H
#define MCLFCONTENTLISTINGENGINE_H

//  INCLUDES
#include <CLFContentListing.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MCLFContentListingEngineExt;
class MCLFChangedItemObserver;
class MCLFItemListModel;
class MCLFOperationObserver;
class TResourceReader;
class MCLFProcessObserver;

// CLASS DECLARATION

/**
*  Content Listing Engine.
*  With Content Listing Engine, client can create item list models,
*  add an observer to get notifications of changed items and
*  send update notifications to server.
*  Client can create an instance this class with ContentListingFactory.<br><br>
*  Usage:
*
*  @code
*
*  // Create a new instance with Content Listing Factory
*  MCLFContentListingEngine* engine =
*                    ContentListingFactory::NewContentListingEngineLC();
*
*  // Add changed item observer
*  engine->AddChangedItemObserverL( *myObserver );
*
*  // Send update notification when item is changed.
*  engine->UpdateItemsL();
*
*  // Remember to remove the observer, when it is deleted
*  engine->RemoveChangedItemObserver( *myObserver );
*
*  @endcode
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFContentListingEngine
    {
    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MCLFContentListingEngine() {}

    public: // New functions

        /**
        * Create a new instance of the Item List Model.
        * @since S60 3.1
        * @param aObserver Reference to an Operation Observer
        * @return New instance of the Item List Model.
        *         Ownership is transfered to the client application.
        */
        virtual MCLFItemListModel* CreateListModelLC(
                        MCLFOperationObserver& aObserver ) = 0;

        /**
        * Create a new instance of Item List Model from the application
        * resource.
        * @since S60 3.1
        * @param aObserver Reference to an Operation Observer.
        * @param aResource Resource reader to list model resource.
        *                  Use resource struct CLF_LIST_MODEL. See
        *                  CLFContentListing.rh
        * @return New instance of Item List Model.
        *         Ownership is transfered to the client application.
        */
        virtual MCLFItemListModel* CreateListModelLC(
                        MCLFOperationObserver& aObserver,
                        TResourceReader& aResource ) = 0;

        /**
        * Add a changed item observer to the Engine.
        * Changed item observer will notify you when items are changed, added
        * or removed. Remember to remove the observer before you delete
        * observer object.
        * @since S60 3.1
        * @param aObserver Changed item observer to be added
        */
        virtual void AddChangedItemObserverL(
                        MCLFChangedItemObserver& aObserver ) = 0;

        /**
        * Remove a changed item observer from the Engine.
        * @since S60 3.1
        * @param aObserver Changed item observer to be removed
        */
        virtual void RemoveChangedItemObserver(
                        MCLFChangedItemObserver& aObserver ) = 0;

        /**
        * Add CLF process observer.
        * Remember to remove the observer before delete the observer object.
        * @since S60 3.1
        * @param aObserver Process observer to be added
        */
        virtual void AddCLFProcessObserverL(
                        MCLFProcessObserver& aObserver ) = 0;

        /**
        * Remove CLF process observer.
        * @since S60 3.1
        * @param aObserver Process observer to be removed
        */
        virtual void RemoveCLFProcessObserver(
                        MCLFProcessObserver& aObserver ) = 0;

        /**
        * Send item update notification.
        * Client must use this method when media item(s) is changed or deleted.
        * This method call will request the CLF server to update its list
        * of items. Unneccesary update notifications should be avoided because
        * they generate load to the system!
        * @since S60 3.1
        */
        virtual void UpdateItemsL() = 0;

        /**
        * Send item update notification.
        * Client must use this method when media item(s) is changed or deleted.
        * This method call will request the CLF server to update items that are
        * defined in the TArray.
        * @since S60 3.1
        * @param aItemIDArray List of Item IDs of modified items
        */
        virtual void UpdateItemsL(
                        const TArray<TCLFItemId>& aItemIDArray ) = 0;

        /**
        * Send item update notification.
        * Client must use this method when media item(s) is changed or deleted.
        * @param aSemanticId Sematic ID is transfered to server plugins
        * @param aOpaqueData Opaque data is transfered to server plugins
        * @since S60 3.1
        */
        virtual void UpdateItemsL(
                        TInt aSemanticId,
                        const TDesC8& aOpaqueData ) = 0;

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFContentListingEngineExt* Extension() { return NULL; }

    };

#endif      // MCLFCONTENTLISTINGENGINE_H

// End of File
