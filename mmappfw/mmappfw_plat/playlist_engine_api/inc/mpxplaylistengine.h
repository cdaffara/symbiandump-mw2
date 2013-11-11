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
* Description:  Playlist engine
*
*/



#ifndef MPXPLAYLISTENGINE_H
#define MPXPLAYLISTENGINE_H

#include <barsc.h>
#include <mpxtaskqueueobserver.h>
#include <mpxpluginhandlerobserver.h>

#include "mpxplaylistenginedefs.h"
#include "mpxplaylistengineobserver.h"
#include "mpxplaylistpluginobserver.h"
#include "mpxplaylistpluginhandler.h"

// FORWARD DECLARATIONS
class CMPXActiveTaskQueue;

/**
* This is the main class in playlist engine and the entry point for a client.
* Each instance of a playlist engine is capable of handling one client.
*
* Once instantiated, CMPXPlaylistPluginHandler allows the client to select an
* appropriate playlist plugin to handle requests if
* necessary (e.g. ExternalizePlaylistL). A playlist plugin is loaded when selected
* by a client or in some cases, it's loaded when a request is issued for the plugin
* (e.g. InternalizePlaylistL). It's unloaded when the plugin is uninstalled from
* the system or when the client destroys its playlist engine.
*
* A client can access playlist plugin via Playlist Plugin Handler. However, it's
* recommended that a client direct any request for the plugin through the playlist
* engine and the client is notified of the completion through
* MMPXPlaylistEngineObserver for async operations.
*
* @lib mpxplaylistengine.lib
*/
class CMPXPlaylistEngine : public CActive,
                           public MMPXPlaylistPluginObserver,
                           public MMPXTaskQueueObserver,
                           public MMPXPluginHandlerObserver
    {
public:

    /**
    *  Two-phased constructor.
    *
    *  @since S60 3.2.3
    *  @param aObserver A playlist engine observer
    *  @return Constructed playlist engine object
    */
    IMPORT_C static CMPXPlaylistEngine* NewL(MMPXPlaylistEngineObserver& aObserver);

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C ~CMPXPlaylistEngine();

    /**
    * returns a handle to playlist plugin handler which a client can use
    * to select an appropriate plugin.
    *
    * Lifetime for this playlist plugin handler is the same as the playlist
    * engine.
    *
    * @see CMPXPlaylistPluginHandler
    *
    * @since S60 3.2.3
    * @return reference to playlist plugin handler
    */
    IMPORT_C CMPXPlaylistPluginHandler& PlaylistPluginHandler();

    /**
    * Determines whether the given media is a playlist from available
    * playlist plugins currently in the system. The existence of the
    * file is not part of this validation process.
    *
    * An unsupported playlist file is tested as EFalse. But after the
    * appropriate playlist plugin has been installed, the client can
    * retest it and ETrue will be returned.
    *
    * @since S60 3.2.3
    * @param aUri URI of the media to be tested
    * @return ETrue if it's a playlist; otherwise EFalse.
    */
    IMPORT_C TBool IsPlaylistL( const TDesC& aUri );

    /**
    * Internalize a playlist (async). Client is not required to select an
    * appropriate plugin as one that is capable of handling the given
    * media from its URI is automatically selected. Client can issue
    * another request without having to wait for the completion of a request.
    * When a request is complete, client is notified through
    * MMPXPlaylistEngineObserver's HandlePlaylistL method. When an
    * appropriate playlist plugin cannot be found to handle this request,
    * KErrNotSupported is returned (through MMPXPlaylistEngineObserver's
    * HandlePlaylistL).
    *
    * @see MMPXPlaylistEngineObserver
    *
    * @since S60 3.2.3
    * @param aPlaylistUri Uri of the playlist to be internalized. Client may
    *        free this parameter as soon as this request is successfully
    *        submitted. From the URI provided, an appropriate plugin is
    *        automatically selected for the client to internalize the playlist.
    * @leave KErrNotFound File does not exist
    */
    IMPORT_C void InternalizePlaylistL(const TDesC& aPlaylistUri);

    /**
    * Internalize a playlist (async). Client is not required to select an
    * appropriate plugin as one which is capable of handling the given
    * file is selected automatically. Client can issue another request without
    * having to wait for the completion of a request. When a request is complete,
    * the client is notified through MMPXPlaylistEngineObserver's
    * HandlePlaylistL method. When an appropriate playlist plugin cannot be
    * found to handle this request, KErrNotSupported error is returned.
    *
    * @see MMPXPlaylistEngineObserver
    *
    * @since S60 3.2.3
    * @param aPlaylistFileHandle a file handle to the playlist to be
    *        internalized. Client may close this file handle as soon as
    *        this request is successfully submitted.
    *
    * From the fullname of the given file handle, an appropriate plugin is
    * automatically selected for the client to internalize the playlist.
    * @leave KErrArgument File handle does not exist
    */
    IMPORT_C void InternalizePlaylistL(const RFile& aPlaylistFileHandle);

    /**
    * Externalize a playlist (async). Before issuing this request, client must
    * use CMPXPlaylistPluginHandler to query available plugins and select an
    * appropriate plugin through CMPXPlaylistPluginHandler. A client
    * can issue another request without having to wait for the completion of
    * a request. When a request is complete, client is notified through
    * MMPXPlaylistEngineObserver's HandlePlaylistL method.
    *
    * NOTE:
    * 1) If the client does not select a plugin prior to issuing this request,
    *    this request may or may not fail depending on whether the client
    *    has loaded a plugin through previous operation, e.g.
    *    InternalizePlaylistL. When a plugin hasn't been selected, processing
    *    of the request will result in KErrNotFound error.
    * 2) If the client has successfully selected a playlist plugin prior to
    *    issuing this request but the selected playlist plugin has since been
    *    uninstalled when this request is ready for processing, KErrNotSupported
    *    error will be returned through HandlePlaylistL.
    *
    * @see MMPXPlaylistEngineObserver
    *
    * @since S60 3.2.3
    * @param aPlaylist a playlist to be externalized. This media must contain
    *        the following attributes:
    *
    *            KMPXMediaGeneralTitle
    *            KMPXMediaGeneralType:
    *               The value of this attribute must be EMPXItem
    *            KMPXMediaGeneralCategory:
    *               The value of this attribute must be EMPXPlaylist
    *            KMPXMediaArrayContents
    *            KMPXMediaArrayCount
    *
    *        KErrArgument is returned if client fails to comply with any of
    *        the above.
    *
    *        Each media item in the playlist must to contain the following
    *        attributes:
    *
    *            KMPXMediaGeneralType:
    *               This contains the value of EMPXItem
    *            KMPXMediaGeneralCategory:
    *               This contains the value of EMPXSong
    *
    *            Client is recommended to call CMPXPlaylistPlugin::RequiredAttributes
    *            before making a request to export the playlist and provide the rest
    *            of attributes required for each media item in the playlist. If client
    *            is unable to provide all of those attributes, client MUST at least
    *            provide the URI of the media item.
    *
    *        NOTE: It's recommended that client deletes this playlist as soon
    *              as this method returns to save memory consumption as
    *              playlistengine will have made a copy of the playlist and
    *              stored it in the task queue.
    *
    * @param aFilePath file path for the playlist to be created, e.g.
    *        e:\playlists\. This must be a valid path. Leaves with
    *        KErrPathNotFound if the given path is invalid.
    * @leave KErrArgument Playlist does not contain expected attribute
    * @leave KErrArgument Media is not a Playlist
    * @leave KErrPathNotFound File path does not exist
    * @leave KErrNotFound Plugin has not been selected
    */
    IMPORT_C void ExternalizePlaylistL(
                        const CMPXMedia& aPlaylist,
                        const TDesC& aFilePath);

    /**
    * Cancel current request, if any, and all other outstanding requests. Client
    * will be notified through MMPXPlaylistEngineObserver's HandlePlaylistL if
    * there is a current request being cancelled.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void CancelRequests();

private:

    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    *  @param aObserver observer
    */
    CMPXPlaylistEngine(MMPXPlaylistEngineObserver& aObserver);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

    /**
    *  From MMPXPlaylistPluginObserver.
    *  Callback from a plugin for an InternalizePlaylistL request, in the
    *  task queue, which is currently being processed.
    *
    *  @since S60 3.2.3
    *  @param aPlaylist a list of media items parsed from the playlist file
    *  @param aError error code
    *  @param aCompleted a flag that indicates if there will be
    *         subsequent callback for the same playlist. EFalse if not
    *         all the results have been sent back; there will be
    *         subsequent callbacks.
    */
    void HandlePlaylistL(CMPXMedia* aPlaylist,
                         const TInt aError,
                         const TBool aCompleted);

    /**
    *  From MMPXPlaylistPluginObserver.
    *  Callback from a plugin for an ExternalizePlaylistL request, in the
    *  task queue, which is currently being processed.
    *
    *  @since S60 3.2.3
    *  @param aPlaylistUri URI for the playlist which has been externalized
    *  @param aError error code
    */
    void HandlePlaylistL(const TDesC& aPlaylistUri,
                         const TInt aError);

    /**
    *  From CActive.
    *  Handles an active object's request completion event.
    *
    *  @since S60 3.2.3
    */
    void RunL();

    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2.3
    */
    void DoCancel();

    /**
    * From MMPXTaskQueueObserver.
    * Execute a task.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aParamData parameter
    * @param aPtrData any object
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
    * Handle a task error.
    *
    * @see MMPXTaskQueueObserver
    *
    * @since S60 3.2.3
    */
    void HandleTaskError(TInt aTask,
                         TAny* aPtrData,
                         TAny* aCallback,
                         TInt aError);

    /*
    * Handle event from plugin.
    * @see MMPXPluginHandlerObserver
    *
    * @since S60 3.2.3
    */
    void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
        TBool aLoaded, TInt aData);

    /**
    * Execute a task.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aBuf externalized parameters for the task
    * @panic KErrInUse, if not active
    * @leave KErrNotSupported Plugin is not found
    */
    void ExecuteTaskL(TInt aTask, const CBufBase& aBuf);

    /**
    * Handles a leave occurring in the request completion event handler
    * ExecuteTaskL.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aError error code
    */
    void HandleExecuteTaskError(TInt aTask, TInt aError);

    /**
    * Handles a leave occurring in the request completion event handler
    * ExecuteTaskL.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aError error code
    */
    void HandleExecuteTaskErrorL(TInt aTask, TInt aError);

    /**
    * Cleanup after processing the current task.
    *
    * @since S60 3.2.3
    */
    void Cleanup();

    /**
    * Generate to character sets based on locale.
    *
    * @since S60 3.2.3
    */
    void GenerateTopCharacterSetsL();

    /**
    * Select character set(s) for the specified language.
    *
    * @since S60 3.2.3
    * @param aLanguage language to select character sets for
    */
    void SelectCharacterSetsForLanguageL(TInt aLanguage);

    /**
    * reads the character set for the specified resource.
    *
    * @since S60 3.2.3
    * @param aResourceId id of the resource to read from the resource file
    */
    void ReadCharacterSetResourceL(TInt aResourceId);

    /**
    * Determine whether the given character set is specified
    * as top character set.
    *
    * @since S60 3.2.3
    * @param aCharacterSetId id of the character set to be tested
    */
    TBool IsInTopCharacterSet(TUint aCharacterSetId);

private:

    // playlist commands
    enum TMPXPlaylistCmd
        {
        EInternalizePlaylist,
        EExternalizePlaylist
        };

private:

    RFs                                     iFs;

    MMPXPlaylistEngineObserver&             iObserver;

    CMPXPlaylistPluginHandler*              iPluginHandler;

    CMPXActiveTaskQueue*                    iTaskQueue;

    // Character set for unicode conversion
    CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iCharacterSet;
    // Most likely character set for unicode conversion
    CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet;

    RResourceFile                           iRscFile;

    //
    // parameters for the task currently being processed to ensure
    // they are available for the duration of the task processing.
    // they are freed after each task completion.
    //
    HBufC*                                  iPlaylistUri;
    HBufC*                                  iFilePath;
    CMPXMedia*                              iPlaylist;

    };

#endif // MPXPLAYLISTENGINE_H
