/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface to collections
*
*/


#ifndef MMPXCOLLECTIONUTILITY_H
#define MMPXCOLLECTIONUTILITY_H

#include <mpxcollectionframeworkdefs.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxcollectionobserver.h>
#include <mpxattribute.h>
#include <mpxfilter.h>
#include <mpxcommand.h>
#include <mpxsearchcriteria.h>
#include <mpxattributespecs.h>

#include <badesca.h>

class CMPXCollectionPath;
class CMPXCollectionType;
class MMPXCollection;
class MMPXCollectionFindObserver;
class CMPXSubscription;

/********************************************************************************
*
*  Note:
*
*  ** remove void CommandL(TMPXCollectionCommand aCmd, TInt aData = 0)
*
*  ** remove TUid CollectionIDL(TUid& aCollection)
*
*  ** all methods which do not (necessarily) operate on current collection
*     should be moved from MMPXCollection to MMPXCollectionUtility
*
*
*********************************************************************************/

/**
 *  Main client interface for collections. Some terms used in the documentation of
 *  this interface:
 *
 *    Context - represents the 'state' of the collection, i.e. the current
 *              browse path (encapsulated in CMPXCollectionPath) and filter applied
 *             (encapsulated in CMPXFilter)
 *
 *    Playlist - an abstract list of songs. A collection path allows the client
 *             to cross reference the playlist with a collection, for example to
 *             iterate through the playlist, extract user-supplied metadata etc.
 *
 *  @lib mpxcollectionutility.lib
 */
class MMPXCollectionUtility
    {
public:
    /**
     *  Creates the interface for the client. This interface is bound to (i.e.
     *  controls and receives events from) either an existing context or a new
     *  one, according to:
     *
     *    aModeId=KMcModeDefault - bound to the same context as any other client
     *                             running in the same thread, or KMcModeIsolated
     *                             context created if the first client in the thread
     *    aModeId=KMcModeIsolated - new context created specifically for client,
     *                              may be shared later
     *    aModeId=<UID> - bound to the same context as other clients which have
     *                    supplied the same UID. Most common case is application UID
     *                    so that clients in external processes can easily bind to
     *                    a specific application context
     *
     *  @since S60 3.2.3
     *  @param aObs collection observer
     *  @param aModeId collection mode id
     *  @return collection utility object
     */
    IMPORT_C static MMPXCollectionUtility* NewL(
                                        MMPXCollectionObserver* aObs = NULL,
                                        const TUid& aModeId = KMcModeDefault);

    /**
     *  The collection.
     *
     *  @since S60 3.2.3
     *  @return the collection
     */
    virtual MMPXCollection& Collection() = 0;

    /**
     * Retrieves the ID of the collection plugin which matched aUids best.
     * If multiple plugins match the aUids, the only one with high priority will
     * be returned.
     *
  	 * @since S60 3.2.3
     * @param aUids selection criteria
     * @return TUid containing the implementation UID of the collection that
     * would be selected
     */
    virtual TUid CollectionIDL(const TArray<TUid>& aUids) = 0;

    /**
     *  Frees up client side resources only. A collection is freed when there are no
     *  clients using it, and all resources are freed when the last client closes.
     *
     *  @since S60 3.2.3
     */
    virtual void Close() = 0;
    };

/**
 *  MPXCollection
 *
 *  @lib mpxcollectionutility.lib
 */
class MMPXCollection
    {
public:
    /**
     *  Opens the collection in its current state.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer, i.e.contains other media objects.
     *
     *  @since S60 3.2.3
     *  @param aMode open mode
     */
    virtual void OpenL(TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection at a specific index.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer, i.e.contains other media objects.
     *
     *  @since S60 3.2.3
     *  @param aIndex the index into the currently opened item list
     *  @param aMode type of open mode
     */
    virtual void OpenL(TInt aIndex,TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection at a specific level.
     *
     *  @since S60 3.2.3
     *  @param aPath the path of the collection
     *  @param aMode type of open mode required
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL()
     */
    virtual void OpenL(const CMPXCollectionPath& aPath,
                       TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection at a specific index.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer, i.e.contains other media objects which will
     *  contain the attribute values specified by aAttrs.   
     *
     *  @since S60 3.2.3
     *  @param aIndex the index into the currently opened item list
     *  @param aAttrs attributes requested
     *  @param aMode type of open mode
     */
    virtual void OpenL(TInt aIndex,
                       const TArray<TMPXAttribute>& aAttrs,
                       TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection at a specific level.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer, i.e.contains other media objects which will
     *  contain the attribute values specified by aAttrs.
     *
     *  @since S60 3.2.3
     *  @param aPath the path of the collection
     *  @param aAttrs attributes requested
     *  @param aMode type of open mode required
     */
    virtual void OpenL(const CMPXCollectionPath& aPath,
                       const TArray<TMPXAttribute>& aAttrs,
                       TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection with a list of implementation uids to be matched.
     *  If current context matches the criteria, it will return existing browsing
     *  media; otherwise it will be back to root level and only plug-ins which
     *  match all provided uids will be returned, in priority order.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer. Returned plugins will be sorted by plugin
     *  priority if mulitple plugins support aUids.
     *
     *  @since S60 3.2.3
     *  @param aUids array of UIDs used to select collection plugins based on
     *  							supported uids in plugin resource file.
     *  @param aMode open mode
     */
    virtual void OpenL(const TArray<TUid>& aUids,
                       TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Opens the collection with an uid to be matched.
     *  If current context matches the criteria, it will return existing browsing
     *  media; otherwise it will be back to root level and only plug-ins which
     *  match the uid will be returned, in priority order.
     *
     *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
     *  with ID KMPXMediaIdContainer. Returned plugins will be sorted by plugin
     *  priority if mulitple plugins support the uid.
     *
     *  @since S60 3.2.3
     *  @param aUid single UID used to select a collection
     *  @param aMode open mode
     */
    virtual void OpenL(const TUid& aUid,
                       TMPXOpenMode aMode=EMPXOpenDefault) = 0;

    /**
     *  Apply a filter to collection browsing. All subsequent calls
     *  to OpenL() will have this filter applied.
     *
     *  @since S60 3.2.3
     *  @param aFilter the filter to apply, or NULL to remove current
     *  filter
     */
    virtual void SetFilterL(CMPXFilter* aFilter) = 0;

    /**
     *  Current filter that is applied to collection browsing. i.e. results
     *  returned through MMPXCollectionObserver::HandleOpenL() callback from
     *  OpenL().
     *
     *  @since S60 3.2.3
     *  @return filter applied to browsing, or NULL if no filter applied
     */
    virtual CMPXFilter* FilterL() = 0;

    /**
     *  Returns current collection plugin implementation UID.
     *
     *  @since S60 3.2.3
     *  @return UID of the collection, or KNullUid if no collection opened
     */
    virtual TUid UidL() const=0;

    /**
     *  The path of the collection in its current state.
     *  Ownsership transferred.
     *
     *  @since S60 3.2.3
     *  @return the path of the collection
     */
    virtual CMPXCollectionPath* PathL() = 0;

    /**
     *  Navigates back to the container of the current items.
     *  Calls back MMPXCollectionObserver::HandleOpenL().
     *
     *  @since S60 3.2.3
     */
    virtual void BackL() = 0;

    /**
     *  Is this path on a 'remote' collection.
     *
     *  @since S60 3.2.3
     *  @aParam aPath the collection path
     *  @return ETrue if is remote collection, otherwise EFalse
     */
    virtual TBool IsRemote(const CMPXCollectionPath& aPath) = 0;

    /**
     *  Stops any async operations that are currently under way.
     *
     *  @since S60 3.2.3
     */
    virtual void CancelRequest() = 0;

    /**
     *  Adds item(s) to the collection.
     *
     *  @since S60 3.2.3
     *  @param aNewMedia new item(s) to add
     */
    virtual void AddL(const CMPXMedia& aNewMedia) = 0;

    /**
     *  Remove an item (or items under a group) from the collection
     *
     *  @param aPath, item(s) to remove
     *  @param aObs, observer to this operation, Optional callback.
     *  @deprecated please use CommandL (CMPXCommand)
     */
    virtual void RemoveL(const CMPXCollectionPath& aPath,
                         MMPXCollectionRemoveObserver* aObs = NULL ) = 0;

    /**
     *  Removes item(s) from the collection.
     *
     *  @since S60 3.2.3
     *  @param aMedia item(s) to remove
     */
    virtual void RemoveL(const CMPXMedia& aMedia) = 0;

    /**
     *  Sets/updates the media for the item.
     *  Callback via EMediaChanged message if currently opened.
     *
     *  @since S60 3.2.3
     *  @param aMedia updated item
     *
     */
    virtual void SetSyncL(const CMPXMedia& aMedia) = 0;

    /** 
     *
     *  Sets/updates the item specified in the path, Aynchronous
     *  Callback for completion with HandleCollectionMessageL()
     *           event:  TMPXCollectionMessage::EAsyncOpComplete
     *           type:   EMcsSetMediaAsync
     *           data:   error code
     *
     *  @param aMedia, updated item
     *  @deprecated, please use CommandL(CMPXCommand& aCmd)
     */
    virtual void SetL(const CMPXMedia& aMedia) = 0;

    /**
     * Find a list of media properties based on a search criteria.
     * The types of supported "queries" are collection dependent.
     * Callback via MMPXCollectionFindObserver::HandleFindL();
     *
     * @since S60 3.2.3
     * @param aCriteria criteria to search on
     * @param aAttrs attributes to return
     * @param aObs observer for find callback
     *    
     */
    virtual void FindAllL(const CMPXSearchCriteria& aCriteria,
                          const TArray<TMPXAttribute>& aAttrs,
                          MMPXCollectionFindObserver& aObs) = 0;

    /**
     * Find a list of media properties based on a search criteria.
     * The types of supported "queries" are collection dependent.
     * This is a SYNCHRONOUS METHOD, use only if necessary.
     *
     * @since S60 3.2.3
     * @param aCriteria criteria to search on
     * @param aAttrs attributes to return
     * @return search results maching the given criteria
     */
    virtual CMPXMedia* FindAllL(const CMPXSearchCriteria& aCriteria,
                                const TArray<TMPXAttribute>& aAttrs) = 0;

    /**
     *  Retrieve media for the item specified in the path.
     *  Note: if selection is set in aPath, HandleMedia will return an array of
     *        media of current selected items.
     *        if no selection is set in aPath, HandleMedia will return media
     *        properities of current item.
     *  Calls back MMPXCollectionObserver::HandleMediaL()
     *
     *  @since S60 3.2.3
     *  @param aPath the path of the collection
     *  @param aAttrs attributes requested.
     *  @param aSpecs specifications for attributes, Ownership not transferred
     *  @param aFilter filter to apply, Ownership not transferred
     *    
     */
    virtual void MediaL(const CMPXCollectionPath& aPath,
                        const TArray<TMPXAttribute>& aAttrs,
                        CMPXAttributeSpecs* aSpecs=NULL,
                        CMPXFilter* aFilter=NULL) = 0;

    /**
     *  Broadcasts an event to all clients or plugins.
     *
     * @since S60 3.2.3
     * @param aMsg event to broadcast
     * @param aData data to accompany the event
     */
    virtual void NotifyL(TMPXCollectionBroadCastMsg aMsg, TInt aData) = 0;

    /**
     *  Send a command to the collection server
     *
     *  @param aCmd, command to handle
     *  @param aData, some data about event (event specific, optional)
     *  @deprecated use CommandL(CMPXCommand& aCmd)
     */
    virtual void CommandL(TMPXCollectionCommand aCmd, TInt aData = 0) = 0;

    /**
     *  Send a command to the collection server.
     *
     *  @since S60 3.2.3
     *  @param aCmd command to handle
     */
    virtual void CommandL(CMPXCommand& aCmd) = 0;

    /**
     * Retrieves the list of supported types.
     *
     * @since S60 3.2.3
     * @param aArray On return, array will be filled with collection type items
     */
    virtual void GetSupportedTypesL(RPointerArray<CMPXCollectionType>& aArray) = 0;

    /**
     * Fetches the capabilities supported by the current collection context.
     *
     * @since S60 3.2.3
     * @param aCapability bitmask of the capabilities supported.    
     */
    virtual void GetSupportedCapabilitiesL(TCollectionCapability& aCapability) = 0;

    /**
     * 
     * Retrieves the real collection UID based on a predefined UID
     * @param aCollection, UID for lookup, defined in "mpxcollectionframeworkdefs.h"
     * @return TUid containing the real implementation UID
     * @deprecated use MMPXCollectionUtility::CollectionIDL
     */
    virtual TUid CollectionIDL(TUid& aCollection) = 0;

    /**
     * Adds a message subscription for this client.
     *
     * @since S60 3.2.3
     * @param aSubscription subscription to be added
     */
    virtual void AddSubscriptionL(const CMPXSubscription& aSubscription) = 0;

    /**
     * Removes a message subscription for this client.
     *
     * @since S60 3.2.3
     * @param aSubscription subscription to be removed.
     */
    virtual void RemoveSubscriptionL(const CMPXSubscription& aSubscription) = 0;

    /**
     * Removes all message subscriptions for this client.
     *
     * @since S60 3.2.3
     */
    virtual void ClearSubscriptionsL() = 0;
    };

#endif      // MMPXCOLLECTIONUTILITY_H

