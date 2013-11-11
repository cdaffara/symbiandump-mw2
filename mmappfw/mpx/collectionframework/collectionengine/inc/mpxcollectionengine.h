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
* Description:  Controls collection via plug-ins
*
*/



#ifndef CMPXCOLLECTIONENGINE_H
#define CMPXCOLLECTIONENGINE_H

#include <e32std.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxcollectionpluginobserver.h>
#include <mpxpluginhandlerobserver.h>
#include <mpxlog.h>

#include "mpxcollectionclientcontext.h"

class CMPXCollectionPluginHandler;
class CMPXCollectionPlugin;
class CMPXCollectionPath;
class CMPXActiveTaskQueue;
class CMPXCollectionType;
class CMPXMedia;
class CMPXCollectionCache;
class TReleaseInfo;

/**
*  The collection 'engine' which delegates to plug-ins (or 'collections').
*  Unless stated otherwise, methods apply to currently loaded plug-in
*/
class CMPXCollectionEngine : public CBase,
                             public MMPXCollectionPluginObserver,
                             public MMPXPluginHandlerObserver
    {
public:
    /**
    *  Two-phased constructor.
    *
    *  @return Constructed object
    */
    IMPORT_C static CMPXCollectionEngine* NewL();

    /**
    *  Destructor
    */
    IMPORT_C ~CMPXCollectionEngine();

    /**
    *  Called by each new session, to set up the collection for use
    *
    *  @aModeId Collection mode id
    *  @aClientTid Client Thread ID for tracking
    *  @aMsgQueue message queue for the client
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    *  @return Reference to the client context object
    */
    IMPORT_C CMPXCollectionClientContext& SessionInitL(
                                        const TUid& aModeId,
                                        TThreadId aClientTid,
                                        CMPXMessageQueue* aMsgQueue);

    /**
    *  Broadcasts an event to all contexts
    *  @param aMsg message to send
    *  @param aData some data
    */
    IMPORT_C void NotifyL(TMPXCollectionBroadCastMsg aMsg, TInt aData);
    
    
    /**
    *  Broadcast a command to all plug-ins
    *  @param aCmd command to send
    *  @param aData some data
    */
    void Command(TMPXCollectionCommand aCmd, TInt aData);

    /**
    * Retrieves the set of supported file types in the collection server
    * @param aArray, array, which on return will be filled with the types
    */
    IMPORT_C void GetSupportedTypesL(RPointerArray<CMPXCollectionType>& aArray );

    /**
    * Lookup a collection Virtual ID to the actual implementation ID
    * NOTE: This function is currently hard coded for Inc8,
    * need to change plugin schema for INC9
    * @param aUid, virtual id to lookup
    * @return UID of the implementation
    */
    IMPORT_C TUid LookupCollectionPluginID(const TUid aUid);

    /**
    * Resolve a collection plugin uid
    * @param aAry, array of uids to resolve the plugin
    * @return Implementation uid of the plugin with the highest score
    *         KNullUidValue will be returned if no plugin found
    */
    IMPORT_C TUid ResolvePluginUid(const TArray<TUid>& aAry);

    /**
    *  Get a list of plugins which support uids in aSupportedUids
    *
    *  @param aEntries a list of plugins
    *  @param aSupportedUids a list of supported uids
    */
    void ListPluginsL(CMPXMedia& aEntries, const TArray<TUid>& aSupportedUids);

    /**
    *  Returns a pointer to the plug-in responsible for a certain collection path
    *
    *  @return collection plugin handler
    */
    CMPXCollectionPlugin* ResolvePluginL(const CMPXCollectionPath& aPath);

    /**
    *  Returns a pointer to the plug-in responsible for a collection uid
    *
    *  @return collection plugin handler
    */
    CMPXCollectionPlugin* ResolvePluginL(const TUid& aUid);

    /**
    *  Returns a pointer to the plug-in responsible for the URI
    *
    *  @return collection plugin handler
    */
    CMPXCollectionPlugin* ResolvePluginL(const TDesC& aUri);

    /**
    *  Indicates that a client started using the specified plugin.
    *
    *  @param aPluginUid plugin UID to be deselected
    */
    void UsePlugin(const TUid& aPluginUid);

    /**
    *  Indicates that a client no longer uses the specified plugin.
    *
    *  @param aPluginUid plugin UID to be deselected
    *  @return ETrue if the plugin was unloaded, EFalse otherwise
    */
    TBool ReleasePlugin(const TUid& aPluginUid);

    /**
    * Get the plugin cacheable attribute
    * @param aUid uid to get the cacheable attribute
    * @return ETrue / EFalse if plugin is cachable
    */
    TBool PluginCacheable( const TUid& aUid );

    /**
    * Get the list of non-cachable attributes for a plugin
    * @param aUid uid of the pluginto get the cacheable attribute
    * @return list of UIDs for attributes not cacheable
    */
    const TArray<TUid> PluginNonCacheableAttributesL( const TUid& aUid );

    /**
    * Remove a client context
    * @param aContext context to be removed.
    */
    void RemoveContext(const CMPXCollectionClientContext& aContext);

    /**
    * Notify collection change to other client contexts
    * @param aNotifier context of notifier
    * @param aMessage change event messages
    * @param aError error code
    */
    void NotifyChange(const CMPXCollectionClientContext& aNotifier,
                      CMPXMessage* aMessage,
                      TInt aError);
    /**
    * Check if refresh is ongoing
    */
    inline TBool IsRefreshing();

    /**
    *  Remove task for one client which is the observer of tasks
    *  @aCallback the observer of the tasks to be removed
    */
    void RemoveTask(TAny* aCallback);
    
    /*
     * Resets the content of the cache
     * @param aPath path defining the context to be removed. 
     */
    void ResetCacheL(CMPXCollectionPath& aPath);

    /**
    *  Pushes the specified plugin on the cleanup stack and the internal stack.
    *  @param aPlugin plugin to be pushed on the cleanup stack
    */
    void CleanupPluginPushL(CMPXCollectionPlugin* aPlugin);

    /**
    *  Pops a plugin from the cleanup stack and the internal stack.
    */
    void PluginPop();

    /**
    *  Returns the loaded plugin by implementation uid.
    *
    *  @param aUid implementation uid of the plugin
    *  @return plugin pointer if plugin already loaded or NULL if not loaded
    */
    CMPXCollectionPlugin* LoadedPlugin(const TUid& aUid);

public: // From base classes

    ///////////////////////////////////////////////////////////////////////////
    // From MMPXCollectionPluginObserver

    /**
    * Handle message from plug-in
    *
    * @param aMessage, message from the plug-in
    * @param aError error code
    */
    inline void HandleMessage(CMPXMessage* aMsg, TInt aError);

    /**
    * Handle opening group contents
    *
    * @param aMedia media, Ownership not transferred.
    * @param aErr error code
    */
    inline void HandleOpen(CMPXMedia* aMedia, TInt aErr);

    /**
     * Handle opening a group, e.g. an artist
     *
     * @param aMedia media object returned. NULL will be returned if error.
     *        Ownership not transferred.
     * @param aPath full path to to current level. Including top level node as well.
     *         NULL will be returned if error. Ownership not transferred
     * @param aErr error code
     */
    inline void HandleOpen(CMPXMedia* aMedia,
                           const CMPXCollectionPath* aPath,
                           TInt aErr);


    /**
    * Handle opening an item
    *
    * @param aPath selected item, Ownership not transferred.
    * @param aErr error code
    */
    inline void HandleOpen(CMPXCollectionPath* aPath, TInt aErr);

    /**
    *  Handle media
    *
    *  @param aMedia media, Ownership not transferred.
    *  @param aError error code
    */
    inline void HandleMedia(CMPXMedia* aMedia, TInt aError);

    /**
     *  Handle completion of a asynchronous command
     *
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code
     */
    inline void HandleCommandComplete(CMPXCommand* aCommandResult,
                                      TInt aError);
    /**
    * Handle Find results
    * @param aMedia media item(s)
    * @param aError, error code
    */
    inline void HandleFindAll(CMPXMedia* aMedia, TInt aError);

     /**
    * Handles remove results
    * @param aUriArray, NULL if the plugin does not use concept of URI
    *                   else, contains the file path to delete a file
    * @param aError, error code
    */
    inline void HandleRemove(const CDesCArray& aUriArray, TInt aError);

    /*
    * From MMPXPluginHandlerObserver
    * @see MMPXPluginHandlerObserver
    */
    void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
        TBool aLoaded, TInt aData);

private:  // New functions


    /**
    *  Creates a new context in the list
    *
    *  @param aModeId context mode id
    *  @return The new path object
    */
    CMPXCollectionClientContext* CreateNewContextL(const TUid& aModeId);

    /**
    *  Finds a default context for the given thread
    *
    *  @param aClientId The calling client's thread ID
    *  @return The found path, or NULL if not found
    */
    CMPXCollectionClientContext* FindDefaultContext(TThreadId aClientId);
    /**
    *  Releases a plugin. Called when PopAndDestroy is called on a plugin
    *  cleanup stack item.
    *  @param aEngine pointer to the current engine instance
    */
    static void CMPXCollectionEngine::ReleasePlugin(TAny* aEngine);

    /**
    *  Called when a plugin is unloaded.
    *  @param aUid identifies the plugin that was unloaded.
    */
    void HandlePluginUnloaded(const TUid& aUid);

    /**
    * Non leaving function to cleanup collection cache
    */
    void DoHandlePluginUnloadedL(const TUid& aUid);

    /**
    * Handle the collection change message
    * @param aMessage message to process
    */
    void DoNotifyChangeL( const CMPXMessage& aMessage );

    /**
    * Handle a single change message
    * @param aMessage message to process
    * @return ETrue if the cache has been invalidated for a particular plugin
    */
    TBool DoNotifySingleChangeL( const CMPXMessage& aMessage );

    /**
    * Sends a plugin handler message to all clients.
    * @param aMessageId the value of the KMPXMessageGeneralId attribute
    * @param aPluginUid plugin UID this message is for
    * @param aLoaded ETrue if the plugin is loaded
    * @param aVersion plugin version
    */
    void SendPluginHandlerMessageL(TInt aMessageId, const TUid& aPluginUid,
        TBool aLoaded, TInt aVersion = 0);

private:

    /**
    *  C++ constructor
    *
    */
    CMPXCollectionEngine();

    /**
    *  2nd phase contructor
    */
    void ConstructL();

private:

    CMPXCollectionPluginHandler* iPluginHandler;

    /**
    *   Playlist/song data
    */
    RPointerArray<CMPXCollectionClientContext> iContexts;

    TBool iRefreshing;

    /**
    *   Collection cache - singleton for all client contexts.
    */
    CMPXCollectionCache* iCache;

    /**
    *  Internal stack used for releasing plugins. This is used in addition to the
    *  Symbian cleanup stack.
    */
    RArray<TReleaseInfo> iCleanupStack;

    /*
    * If not KNullUid, indicates the plugin that is currently being upgraded.
    */
    TUid iUpdatedPluginUid;

    CMPXMedia* iMediaForHeapKeepAlive; // own
    };

#include "mpxcollectionengine.inl"

#endif // CMPXCOLLECTIONENGINE_H

