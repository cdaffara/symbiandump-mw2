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
* Description:  Maintains sessions to paths
*
*/

#ifndef CMPXCOLLECTIONCLIENTCONTEXT_H
#define CMPXCOLLECTIONCLIENTCONTEXT_H

#include <mpxcollectionpluginobserver.h>
#include <mpxtaskqueueobserver.h>
#include <mpxcmn.h>
#include <mpxcommand.h>
#include <mpxfilter.h>
#include <mpxattributespecs.h>
#include <mpxmessagequeue.h>
#include "mpxcollectioncache.h"

class CMPXClientList;
class CMPXCollectionPath;
class CMPXCollectionEngine;
class CMPXCollectionPlugin;
class MMPXCollectionEngineObserver;
class CMPXActiveTaskQueue;
class CMPXMedia;
class CMPXCollectionCache;

class CMPXCollectionClientContext : public CBase,
                                    public MMPXCollectionPluginObserver,
                                    public MMPXTaskQueueObserver
    {
public: // DATA type
    enum TContextType
        {
        EContextBrowse, // context for browing
        EContextMedia, // context for media properties
        EContextRemove, // context for removing paths
        EContextCount
        };
public:
    /**
    *  Two-phase constructor
    *
    *  @param aEngine reference to collection engine
    *  @param aModeId mode id
    *  @return object created
    */
    static CMPXCollectionClientContext* NewL(CMPXCollectionEngine& aEngine,
        CMPXCollectionCache& aCache, const TUid& aModeId);

    /**
    *  Destructor
    *
    */
    ~CMPXCollectionClientContext();

    /**
    *  Open collection by path
    *
    *  @param aPath collection path. Ownership transferred if not leave
    *  @param aMode, open mode
    *  @param aCallback callback
    *  @leave KErrArgument if aPath is not valid, or other system error
    */
    IMPORT_C void OpenL(CMPXCollectionPath* aPath,
                        TInt aMode,
                        MMPXCollectionEngineObserver* aCallback);

    /**
    *  Open collection by index
    *
    *  @param aIndex the index to current browse path
    *  @param aMode, open mode
    *  @param aAttrs, media attributes requested
    *  @param aCallback callback
    *  @leave system error
    */
    IMPORT_C void OpenL(TInt aIndex,
                        TInt aMode,
                        const TArray<TMPXAttribute>& aAttrs,
                        MMPXCollectionEngineObserver* aCallback);

    /**
    *  Open collection by uids. Collections which match the uids will be returned.
    *
    *  @param aUids a list of uids to be matched
    *  @param aMode, open mode
    *  @param aCallback callback
    *  @leave KErrNotReady if current browse path is invalid,
    *         KErrArgument if aIndex is out of range
    *         or other system error
    */
    IMPORT_C void OpenL(const TArray<TUid>& aUids,
                        TInt aMode,
                        MMPXCollectionEngineObserver* aCallback);

    /**
     *  Request current content of browse context
     *
     *  @param aMode, open mode
     *  @param aCallback callback
     *  @leave system error
     */
    IMPORT_C void OpenL(TInt aMode, MMPXCollectionEngineObserver* aCallback);

    /**
     *  Back to previous level
     *  @param aCallback callback
     *  @leave system error
     */
    IMPORT_C void BackL(MMPXCollectionEngineObserver* aCallback);

    /**
     *  Media request command
     *
     *  @param aCmd  command of media request
     *  @param aCallback callback
     *  @leave KErrArgument if aCmd is not valid, or other system error
     */
    IMPORT_C void MediaL(const CMPXCommand& aCmd,
                         MMPXCollectionEngineObserver* aCallback);

    /**
     *  Add a media
     *
     *  @param aMedia media object
     *  @leave system error
     */
    IMPORT_C void AddL(const CMPXMedia& aMedia);

    /**
     *  Remove a media
     *
     *  @param aMedia media object
     *  @leave system error
     */
    IMPORT_C void RemoveL(const CMPXMedia& aMedia);

    /**
     *  Update a media
     *
     *  @param aMedia media object
     *  @leave system error
     */
    IMPORT_C void SetL(const CMPXMedia& aMedia);

    /**
     *  Remove media by path
     *
     *  @param aPath collection path. Ownership transferred if not leave
     *  @param aCallback callback
     *  @leave KErrArgument if aPath is not valid, or other system error
     */
    IMPORT_C void RemoveL(CMPXCollectionPath* aPath,
                          MMPXCollectionEngineObserver* aCallback);

    /**
    *  Return plugin uid
    *
    *  @return plugin uid
    */
    IMPORT_C TUid PluginId() const;

    /**
    *  Return current collection path
    *
    *  @return collection path
    */
    IMPORT_C const CMPXCollectionPath& Path() const;

    /**
    *  Cancel all requests from aCallback
    *
    *  @param aCallback callback
    */
    IMPORT_C void CancelRequest(MMPXCollectionEngineObserver* aCallback);

    /**
    *  Attach another client to this object. aName is the unique name of the
    *  client, used for identifying the client and passing messages; aId is the
    *  thread id of the client of the interface (not the server session)
    *
    *  @param aId thread id of the client
    *  @param aMsgQueue message queue for the client
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    */
    IMPORT_C void AddClientL(TThreadId aId,
                             CMPXMessageQueue* aMsgQueue);

    /**
    *  Remove the client from the context. aName is the unique name of the
    *  client, used for identifying the client and passing messages.
    *
    *  @param aMsgQueue message queue for the client
    */
    IMPORT_C void RemoveClient(const CMPXMessageQueue& aMsgQueue);

    /**
    * Get the supported attributes of the current plugin using by the context
    * @return TCollectionCapability bit mask flag
    */
    IMPORT_C TCollectionCapability GetCapabilities();

    /**
     *  Find all
     *
     *  @param aMedia media object of matching criteria, Ownership transferred
     *  @param aBuf, buffer data, array of attributes for results,
     *               Ownership transferred if find asynchronously
     *  @param aCallback callback
     *  @leave system error
     */
    IMPORT_C void FindAllL(const CMPXMedia& aMedia,
                           CBufBase* aBuf,
                           MMPXCollectionEngineObserver* aCallback);

   /** DEPRECATE, Use CommandL(CMPXCommand)
    *  Find all synchronously
    *
    *  @param aMedia media object of matching criteria
    *  @param aBuf, buffer data, array of attributes for results
    *  @leave system error
    *  @return results of the search
    */
    IMPORT_C CMPXMedia* FindAllSyncL(const CMPXMedia& aMedia,
                                     const CBufBase& aBuf);

    /**
     * Set the current filter applied to browsing
     */
    IMPORT_C void SetFilterL(const CMPXFilter* aFilter);

    /**
    * Get the current filter applied to browsing
    * @return filter
    */
    IMPORT_C const CMPXFilter* Filter() const;

    /** DEPRECATED
     * Handle a collection command
     * @param aCmd, command to handle
     * @param aData, optional data
     */
    IMPORT_C void CommandL(TMPXCollectionCommand aCmd, TInt aData);

    /**
     * Handle a collection command
     * @param aCmd, command to handle
     * @param aCallback callback
     * @param aMsgQueue Message Queue for the client
     * @leave KErrNotReady if not collection opened, or system error
     */
    IMPORT_C void CommandL(const CMPXCommand& aCmd,
                           MMPXCollectionEngineObserver* aCallback,
                           const CMPXMessageQueue& aMsgQueue);

    /**
    *  Does this thread have a shareable client?  If yes, a default mode
    *  connection can bind to it.
    *
    *  @param aId thread id of the client
    *  @return ETrue, if shareableclient is in the client list
    */
    TBool HasShareableClient(TThreadId aId);

    /**
    *  Notify the client of a collection event
    *  @param aMsg Message type
    *  @param aData some data
    */
    void NotifyL( TMPXCollectionBroadCastMsg aMsg, TInt aData );

    /**
    * Get the mode id of the context
    * @return collection mode id
    */
    const TUid& ModeId() const;

    /**
    * Handle message from plug-in
    *
    * @param aMsg, message from the plug-in
    * @param aError, error code
    * @param aNotifyOthers, indicates whether to notify other client contexts
    *        that the collection has changed
    */
    void DoHandleMessage(CMPXMessage* aMsg,
                         TInt aError,
                         TBool aNotifyOthers);

private: // From base classes

    ///////////////////////////////////////////////////////////////////////////
    // From MMPXCollectionPluginObserver

    /**
    * Handle message from plug-in
    *
    * @param aMessage, message from the plug-in, Ownership not transferred.
    * @param aError error code
    */
    void HandleMessage(CMPXMessage* aMsg, TInt aError);

    /**
    * Handle opening group contents
    *
    * @param aMedia media, Ownership not transferred.
    * @param aErr error code
    */
    void HandleOpen(CMPXMedia* aMedia, TInt aErr);

    /**
     * Handle opening a group, e.g. an artist
     *
     * @param aMedia media object returned. NULL will be returned if error.
     *        Ownership not transferred.
     * @param aPath full path to to current level. Including top level node as well.
     *         NULL will be returned if error. Ownership not transferred
     * @param aErr error code
     */
    void HandleOpen(CMPXMedia* aMedia,
                    const CMPXCollectionPath* aPath,
                    TInt aErr);

    /**
    * Handle opening an item
    *
    * @param aPath selected item, Ownership not transferred.
    * @param aErr error code
    */
    void HandleOpen(CMPXCollectionPath* aPath, TInt aErr);

    /**
    *  Handle media
    *
    *  @param aMedia media Ownership not transferred.
    *  @param aError error code
    */
    void HandleMedia(CMPXMedia* aMedia, TInt aError);

    /**
     *  Handle completion of a asynchronous command
     *
     *  @param aCommandResult result of the command, NULL if error
     * Ownership not transferred.
     *  @param aError error code
     */
     void HandleCommandComplete(CMPXCommand* aCommandResult,
                                TInt aError);

    /**
    * Handle Find results
    * @param aMedia media item(s), Ownership not transferred.
    * @param aError, error code
    */
    void HandleFindAll(CMPXMedia* aMedia, TInt aError);

     /**
    * Handles remove results
    * @param aUriArray, NULL if the plugin does not use concept of URI
    *                   else, contains the file path to delete a file
    * @param aError, error code
    */
    void HandleRemove(const CDesCArray& aUriArray, TInt aError);

    ///////////////////////////////////////////////////////////////////////////
    // From MMPXTaskQueueObserver

    /**
    * Execute a task
    * @param aTask task number
    * @param aParamData, parameter
    * @param aPtrData, any object
    * @param aBuf, buffer containing externalized parameters for the task.
    * @param aCallback call back function pointer
    * @param aCObject1 object 1 owned by task queue
    * @param aCObject2 object 2 owned by task queue
    */
    void ExecuteTask(TInt aTask,
                     TInt aParamData,
                     TAny* aPtrData,
                     const CBufBase& aBuf,
                     TAny* aCallback,
                     CBase* aCObject1,
                     CBase* aCObject2);

    /**
    * @see MMPXTaskQueueObserver
    */
    void HandleTaskError(TInt aTask,
                         TAny* aPtrData,
                         TAny* aCallback,
                         TInt aError);

private:
    /**
    *  Open current level of collection
    *
    */
    void OpenL();


    /**
    *  Back to upper level
    *
    */
    void BackL();

    /**
    *  Reopen current level
    *
    */
    void ReOpenL();

    /**
    * Find Media Request
    * @param aMedia, searching criteria
    * @param aBuf, buffered data
    * @param aPlugin, plug to handle the find all
    * @param aResult, sync finding result
    * @param aSync, flag to indicate sync or async find
    */
    void FindAllL(const CMPXMedia& aMedia,
                  const CBufBase& aBuf,
                  CMPXCollectionPlugin& aPlugin,
                  CMPXMedia** aResult = NULL,
                  TBool aSync = EFalse);

    /**
    * Initialize
    * @param aCallback callback when initialization completes
    */
    void InitL(MMPXCollectionEngineObserver* aCallback);

    /**
    * Execute a task
    * @param aTask task number
    * @param aParamData, parameter
    * @param aPtrData, any object
    * @param aCallback call back function pointer
    * @param aBuf buffer data
    * @param aCObject1 object 1 owned by task queue
    * @param aCObject2 object 2 owned by task queue
    * @param aPlugin out parameter, plugin of the task.
    */
    void ExecuteTaskL(TInt aTask,
                      TInt aParamData,
                      TAny* aPtrData,
                      TAny* aCallback,
                      const CBufBase& aBuf,
                      CBase* aCObject1,
                      CBase* aCObject2,
                      CMPXCollectionPlugin*& aPlugin);

    /**
    * Handle error of execution
    * @param aPlugin plugin object
    * @param aErr error number
    * @param aUnuserPlugin ETrue decrement reference count of plugin
    */
    void HandleError(CMPXCollectionPlugin& aPlugin,
                     TInt aErr,
                     TBool aUnusePlugin=EFalse);

    /**
    * Resolve plugin
    * @param aPath, Collection path
    * @param aUid implementation uid of the plugin associated with the path
    */
    void ResolvePluginL(const CMPXCollectionPath& aPath, TUid& aUid);

    /**
    * Resolve plugin for FindAllL
    *
    * @param aMedia media object contains matching criteria
    * @param aPlugin, plugin reutrned
    */
    void ResolvePluginL(const CMPXMedia& aMedia,
                        CMPXCollectionPlugin*& aPlugin);

    /**
    * Add a media into the collection
    * Unpacks a media item group
    * @param aOp, operation code
    * @param aMedia, media object representing a list or a file
    */
    void DoUpdateMediaL(TInt aOp, const CMPXMedia& aMedia);

    /**
    * Add/Remove/Set an "item" into the collection
    * @param aOp, operation code
    * @param aMedia, single media "item"
    */
    void DoHandleItemL(TInt aOp, const CMPXMedia& aMedia);

    /**
    * Handle a synchronous collection command
    * @param aCmd, command to handle
    * @param aPlugin, plugin to prococess the command
    */
    void DoHandleSyncCommandL(const CMPXCommand& aCmd,
                              const CMPXMessageQueue& aMsgQueue,
                              CMPXCollectionPlugin* aPlugin=NULL);

    /**
    * Handle a select collection command
    * @param aCmd, command to handle
    */
    void DoHandleSelectCommandL(const CMPXCommand& aCmd);

    /**
    * Handle opening group contents
    *
    * @param aMedia media
    * @param aPath full path to current level
    * @param aCallback callback of the open request
    * @param aErr error code
    * @param aAddtoCache Should this be added to the cache or not
    * @param aOpenTask is this a OpenL task
    */
    void DoHandleOpenL(CMPXMedia* aMedia,
                       const CMPXCollectionPath* aPath,
                       MMPXCollectionEngineObserver* aCallback,
                       TInt aErr,
                       TBool aAddToCache,
                       TBool aOpenTask );

    /**
    * Handle opening an item
    *
    * @param aPath selected item
    * @param aErr error code
    */
    void DoHandleOpenL(CMPXCollectionPath* aPath,
                       TInt aErr);

    /**
    * Handle message from plug-in
    *
    * @param aMsg, message from the plug-in
    * @param aError, error code
    */
    void DoHandleMessageL(const CMPXMessage& aMsg, TInt aError);

    /**
    * Handles an Open request. First checks the cache and if the results
    * are not found there it calls the plugin.
    */
    void DoPluginOpenL();

    /**
    * Handle a media request.  First checks the cache and if the results
    * are not found there it calls  the plugin.
    */
    void DoPluginMediaL( CMPXCollectionPath& aPath,
                         const TCapabilitySet& aCaps,
                         CMPXAttributeSpecs& aSpecs,
                         CMPXFilter& aFilter);

    /**
    * Sets the plugin of the specified type to the value provided. This method
    * ensures that if valid, the previous plugin's reference count is decremented.
    * @param aType identifies the context type
    * @param aPlugin new plugin
    */
    void SetPlugin(TContextType aType, CMPXCollectionPlugin* aPlugin);

    /**
     * Checks whether or not the attributes are cacheable
     *
     * @return ETrue if attributes are cacheable, else EFalse
     */
    TBool AttributesCacheableL(
        const TArray<TMPXAttribute>& aAttrs,
        const CMPXCollectionPath& aPath );

    /**
    *  Adds a result set to the cache.
    *  @param aPath path defining the context of the results.
    *  @param aAttrs attributes requested by the client application and included in
    *         the result set.
    *  @param aResults result set corresponding to the path.  Not owned.
    *  @param aNotCacheableAttrs attributes that are not cacheable
    *  @param aMediaFromOpenL whether or not this media was from an OpenL
    *  @param aPriority result set priority
    *  @return CMPXMedia object in the cache or the original object if add failed
    */
    CMPXMedia* AddToCache(
        const CMPXCollectionPath& aPath,
        const TArray<TMPXAttribute>& aAttrs,
        CMPXMedia& aResults,
        TBool aMediaFromOpenL = EFalse,
        CMPXCollectionCache::TCachePriority aPriority = CMPXCollectionCache::EPriorityNormal );

    /**
    * Return loaded plugin of a context
    *
    * @param aType identifies the context type. plugin must be loaded for this context
    *              in other words, iPluginUids[aType]!=KNullUid
    * @return loaded plugin pointer for the context.
    */
    CMPXCollectionPlugin* LoadedPlugin(TContextType aType);

private:

    /**
    * C++ constructor
    * @param aEngine reference to engine
    * @param aModeId context mode id
    */
    CMPXCollectionClientContext(CMPXCollectionEngine& aEngine,
        CMPXCollectionCache& aCache, const TUid& aModeId);

    /**
    * 2nd phase contructor
    */
    void ConstructL();

private:
    TUid iModeId; // context mode id

    TMPXItemId iFocusItemId; // Item id for the currently focused item
    TInt iIndex; // Focused item in path

    CMPXCollectionEngine& iEngine;
    CMPXMedia* iMedia; // browsing entries
    CMPXFilter* iFilter; // filter to apply to browsing
    TInt iMediaType; // media type for browsing entries

    CMPXClientList* iClientList;
    CMPXCollectionPath* iBrowsePath; // path for browsing
    CMPXCollectionPath* iMediaPath;
    RArray<TUid> iPluginUids; // uids of loaded plugins for context
    TBool iPathUpdated; // changed due to collection change
    RArray<TUid> iUids; // Implementation sorted uids of collection plugins
    CMPXCollectionCache& iCache;
    TBool iCacheMedia; // Whether or not to cache the media returned
    };

#endif  // CMPXCOLLECTIONCLIENTCONTEXT_H
