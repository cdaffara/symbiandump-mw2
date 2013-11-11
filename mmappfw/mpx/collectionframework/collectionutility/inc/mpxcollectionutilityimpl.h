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
* Description:  Implementation of interface for Collection Utility
*
*/



#ifndef CMPXCOLLECTIONUTILITY_H
#define CMPXCOLLECTIONUTILITY_H


#include <mpxcollectionobserver.h>
#include <mpxcollectionmessage.h>
#include <mpxsession.h>
#include <mpxmessageobserver.h>

#include "mpxcollectionserverdefs.h"
#include "mpxcollectionutility.h"

class CMPXMessageMonitor;
class CMPXTaskQueue;
class CMPXMsgBuf;
class CMPXMedia;

/**
*  Implementation of collection utility interface
*/
NONSHARABLE_CLASS(CMPXCollectionUtility) : public CActive,
                                           public MMPXCollectionUtility,
                                           public MMPXMessageObserver,
                                           public MMPXCollection

    {
public:
    /**
    * Two-phased constructor
    *
    * @param aObs observer
    * @param aModeId collection mode id
    * @return object of constructed
    */
    static CMPXCollectionUtility* NewL(const TUid& aModeId,
                                       MMPXCollectionObserver* aObs);

    /**
    * Destructor
    */
    ~CMPXCollectionUtility();

private:
    /**
    * C++ constructor
    */
    CMPXCollectionUtility(MMPXCollectionObserver* aObs);

    /**
    * Second phase constructor
    *
    * @param aModeId collection mode id
    */
    void ConstructL(const TUid& aModeId);

public:

    /**
    *  From MMPXCollectionUtility
    *  The collection.
    *
    *  @return the collection
    */
    MMPXCollection& Collection();

    /**
    * From MMPXCollectionUtility
    * Retrieves the ID of the collection plugin which matched aUids best.
    * If multiple plugins match the aUids, the only one with high priority will
    * be returned
    *
    * @param aUids, selection criteria
    * @return TUid containing the implementation UID of the collection that
    * would be selected
    */
    TUid CollectionIDL(const TArray<TUid>& aUids);

    /**
    *  From MMPXCollectionUtility
    *  Frees up client side resources only; a collection is freed when there are no
    *  clients using it, and all resources are freed when the last client closes
    */
    void Close();

    /**
    *  From MMPXCollection
    *  Opens the collection in its current state
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer, i.e.contains other media objects
    *
    *  @param aMode, open mode
    */

    void OpenL(TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection at a specific index
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer, i.e.contains other media objects
    *
    *  @param aIndex, the index into the currently opened item list
    *  @param aMode, type of open mode
    */
    void OpenL(TInt aIndex,TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection at a specific level
    *
    *  @param aPath, the path of the collection
    *  @param aMode, open mode
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL()
    */
    void OpenL(const CMPXCollectionPath& aPath,
                TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection at a specific index
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer, i.e.contains other media objects which will
    *  contain the attribute values specified by aAttrs
    *
    *  @param aIndex, the index into the currently opened item list
    *  @param aAttrs, attributes requested.
    *  @param aMode, type of open mode
    */
    void OpenL(TInt aIndex,const TArray<TMPXAttribute>& aAttrs,
                TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection at a specific level
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer, i.e.contains other media objects which will
    *  contain the attribute values specified by aAttrs
    *
    *  @param aPath, the path of the collection
    *  @param aAttrs, attributes requested.
    *  @param aMode, type of open mode required
    */
    void OpenL(const CMPXCollectionPath& aPath,
    		   const TArray<TMPXAttribute>& aAttrs,
               TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection with a list of implementation uids to be matched.
    *  if current context matches the criteria, it will return existing browsing
    *  media; otherwise it will be back to root level and only plug-ins which
    *  match all provided uids will be returned, in priority order.
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer. Returned plugins will be sorted by plugin
    *  priority if mulitple plugins support aUids.
    *
    *  @param aUids, array of UIDs used to select collection plugins based on
    *  supported uids in plugin resource file.
    *  @param aMode, open mode
    */
    void OpenL(const TArray<TUid>& aUids,
    		   TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Opens the collection with an uid to be matched.
    *  if current context matches the criteria, it will return existing browsing
    *  media; otherwise it will be back to root level and only plug-ins which
    *  match the uid will be returned, in priority order.
    *
    *  Calls back MMPXCollectionObserver::HandleOpenL() with CMPXMedia object
    *  with ID KMPXMediaIdContainer. Returned plugins will be sorted by plugin
    *  priority if mulitple plugins support the uid.
    *
    *  @param aUid, single UID used to select a collection
    *  @param aMode, open mode
    */
    void OpenL(const TUid& aUid,
    		   TMPXOpenMode aMode=EMPXOpenDefault);

    /**
    *  From MMPXCollection
    *  Apply a filter to collection browsing; all subsequent calls
    *  to OpenL() will have this filter applied.
    *
    *  @param aFilter, the filter to apply, or NULL to remove current
    *  filter
    */
    void SetFilterL(CMPXFilter* aFilter);

    /**
    *  From MMPXCollection
    *  Current filter that is applied to collection browsing, i.e. results
    *  returned through MMPXCollectionObserver::HandleOpenL() callback from
    *  OpenL()
    *
    *  @return filter applied to browsing, or NULL if no filter applied
    */
    CMPXFilter* FilterL();

    /**
    *  From MMPXCollection
    *  Returns current collection plugin implementation UID
    *
    *  @return UID of the collection
    */
    TUid UidL() const;

    /**
    *  From MMPXCollection
    *  The path of the collection in its current state
    *
    *  @return the path of the collection
    */
    CMPXCollectionPath* PathL();

    /**
    *  From MMPXCollection
    *  Navigates back to the container of the current items
    *  Calls back MMPXCollectionObserver::HandleOpenL()
    */
    void BackL();

    /**
    *  From MMPXCollection
    *  Is this path on a 'remote' collection
    *
    *  @aParam aPath, the collection path
    *  @return ETrue if is remote collection, otherwise EFalse
    */
    TBool IsRemote(const CMPXCollectionPath& aPath);

    /**
    *  From MMPXCollection
    *  Stops any async operations that are currently under way
    */
    void CancelRequest();

    /**
    *  From MMPXCollection
    *  Adds item(s) to the collection
    *
    *  @param aNewProperties, Properties of the item
    */
    void AddL(const CMPXMedia& aNewProperties);

    /**
    *  From MMPXCollection
    *  Remove an item (or items under a group) from the collection
    *
    *  @param aPath, item(s) to remove
    *  @param aObs, observer callback for items deleted (Optional)
    */
    void RemoveL(const CMPXCollectionPath& aPath,
                 MMPXCollectionRemoveObserver* aObs = NULL );

        /**
    *  From MMPXCollection
    *  Removes item(s) to the collection
    *
    *  @param aProperties, Properties of the item
    */
    void RemoveL(const CMPXMedia& aProperties);

    /**
    *  From MMPXCollection
    *
    *  Sets/updates the media for the item
    *  Callback via EMediaChanged message if currently opened
    *
    *  @param aMedia, updated item
    *
    */
    void SetSyncL(const CMPXMedia& aMedia);

    /**
    *  From MMPXCollection
    *
    *  Sets/updates the media for the item, Asynchronous
    *  Callback via HandleCollectionMessage, AsyncOp complete
    *
    *  @param aMedia, new value
    *
    */
    void SetL(const CMPXMedia& aMedia);

    /**
    * Find a list of media properties based on a search criteria
    * The types of supported "queries" are plugin dependent
    *
    * @param aMedia, criteria to search on
    * @param aAttrs, attributes to return
    * @param aObs, observer for find callback
    * Callback via MMPXCollectionFindObserver::HandleFindL();
    */
    void FindAllL(const CMPXMedia& aMedia,
                  const TArray<TMPXAttribute>& aAttrs,
                  MMPXCollectionFindObserver& aObs);

    /**
    * Find a list of media properties based on a search criteria
    * The types of supported "queries" are plugin dependent
    * This is a SYNCHRONOUS METHOD, use only if necessary.
    * @param aMedia, criteria to search on
    * @param aAttrs, attributes to return
    */
    CMPXMedia* FindAllL(const CMPXMedia& aMedia,
                        const TArray<TMPXAttribute>& aAttrs);

    /**
    *  From MMPXCollection
    *  Retrieve media for the item specified in the path
    *  Note: if selection is set in aPath, HandleMedia will return an array of
    *        media of current selected items.
    *        if no selection is set in aPath, HandleMedia will return media
    *        properities of current item.
    *
    *  @param aPath, the path of the collection
    *  @param aAttrs, attributes requested.
    *  @aParam aSpecs, specifications for attributes
    *  @param aFilter, filter to apply
    *
    *  Calls back MMPXCollectionObserver::HandleMediaL()
    */
    void MediaL(const CMPXCollectionPath& aPath,
                const TArray<TMPXAttribute>& aAttrs,
                CMPXAttributeSpecs* aSpecs=NULL,
                CMPXFilter* aFilter=NULL);

    /**
    * From MMPXCollection
    * Broadcasts an event to all observers
    * @param aMsg event to broadcast
    * @param aData data to accompany the event
    */
    void NotifyL( TMPXCollectionBroadCastMsg aMsg, TInt aData );

    /**
    * From MMPXCollection
    * Send a Command to the collection server
    * aCmd, command to handle
    * aData, some data about event (event specific, optional)
    */
    void CommandL(TMPXCollectionCommand aCmd, TInt aData = 0);

    /**
    * From MMPXCollection
    *  Send a command to the collection server
    *
    *  @param aCmd, command to handle
    */
    void CommandL(CMPXCommand& aCmd);

    /**
    * From MMPXCollection
    * Retrieves the set of supported file types
    * @param aArray Array to fill with the data
    */
    void GetSupportedTypesL( RPointerArray<CMPXCollectionType>& aArray );

    /**
    * From MMPXCollection
    * Retrieves the capabilities for the current collection context
    * @arg aCapability, bitmask of the capabilities supported.
    */
    void GetSupportedCapabilitiesL(TCollectionCapability& aCapability);

    /**
    * From MMPXCollection
    * Retrieves the real collection UID based on a predefined UID
    * @param aCollection, UID for lookup, defined in "mpxcollectionframeworkdefs.h"
    * @return TUid containing the real implementation UID
    */
    TUid CollectionIDL(TUid& aCollection);

    /**
    * From MMPXCollection
    * Adds a message subscription for this client.
    * @param aSubscription subscription to be added
    */
    void AddSubscriptionL(const CMPXSubscription& aSubscription);

    /**
    * From MMPXCollection
    * Removes a message subscription for this client.
    * @param aSubscription subscription to be removed.
    */
    void RemoveSubscriptionL(const CMPXSubscription& aSubscription);

    /**
    * From MMPXCollection
    * Removes all message subscriptions for this client.
    */
    void ClearSubscriptionsL();

    /**
    * From MMPXMessageObserver
    * Message received
    * @param aMsgData message data
    * @param aError error code
    */
    void MessageReceived(TInt aMsgData, TInt aError);

    /**
    * From CActive
    * Handles an active object's request completion event
    */
    void RunL();

    /**
    * From CActive
    * Implements cancellation of an outstanding request.
    */
   void DoCancel();

    /**
    * From CActive
    * Handles a leave occurring in the request completion event handler RunL()
    *
    * @param aError error code
    */
    TInt RunError(TInt aError);

private:

    /**
    *  Add a request to the task queue and run the next queue if no outstanding
    *  task
    *
    *  @param aFunction function code
    *  @param aObs, observer to callback asynchronously
    *  @param aParamData data
    *  @param aBuf, extra data in buffer, ownership transferred.
    *  @param aPtr, pointer to a object
    *  @param aAlivePtr1, object to keep alive for a task
                         ownership transferred
    *  @param aAlivePtr2, same as aAlivePtr1
    */
    void AddRequestL(TMPXCollectionServerOp aFunction,
                     TAny* aObs,
                     TInt aParamData=0,
                     CBufBase* aBuf=NULL,
                     TAny* aPtr=NULL,
                     CBase* aAlivePtr1=NULL,
                     CBase* aAlivePtr2=NULL);

    /**
    *  Send next request to server
    */
    void ExecuteNextRequest();

    /**
    *  Handle message from message queue
    *  @param aMsgData message data
    *  @param aError error code
    */
    void HandleMessageL(TInt aMsgData, TInt aError);

    /**
     * Handles error in RunL
     *
     * @param aError error code
     */
    void HandleRunErrorL(TInt aError);

#ifdef _ENABLE_GUARD_TIMER
    /**
     *  Gaurd timer callback
     *
     *  @param aPtr pointer the this
     */
    static TInt GuardTimerCallback(TAny* aPtr);
#endif

private:
    MMPXCollectionObserver* iObserver;
    CMPXMessageMonitor* iMsgMonitor;
    CMPXTaskQueue* iTaskQueue;
    RMPXSession iMcs;
    CMPXMedia* iCurrentEntries;

    CBufBase* iBuffer;
    CArrayFixBase* iArray;
    TPckgBuf<TInt> iSizePckg;
    TPckgBuf<TInt> iSelectionPckg;
    TPckgBuf<TInt> iCompletePckg;
    TPckgBuf<TInt> iPropertiesSizePckg;
    TInt iSelection;
    TBool iCallbackOngoing; // is client processing callback?
    // Keep alive pointers on global heap
    CMPXMedia* iMedia;
    CMPXMedia* iMediaOnError; // media object while handling error
#ifdef _ENABLE_GUARD_TIMER
    CPeriodic* iGuardTimer; //guard timer for async request
#endif
    };

#endif // CMPXCOLLECTIONUTILITY_H
