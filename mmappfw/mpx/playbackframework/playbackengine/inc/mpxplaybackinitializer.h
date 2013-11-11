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
* Description:  Initialize a file before playback
*
*/


#ifndef CMPXPLAYBACKINITIALIZER_H
#define CMPXPLAYBACKINITIALIZER_H

#include <mpxplaybackframeworkdefs.h>
#include <mpxplaybackpluginobserver.h>
#include <mpxpluginhandlerobserver.h>
#include <mpxcollectionobserver.h>

class CMPXPlaybackPluginHandler;
class CMPXCollectionPlaylist;
class CMPXPlaybackEngine;
class CMPXPlaybackPluginHandler;
class CMPXPlaybackPlugin;
class CMPXPlaybackMediaHelper;

/**
*  Initializes playlist to be played. This class is a friend
*  of CMPXPlaybackEngine
*
*  The use of this class is intended to be as follows by the
*  playback engine:
*  1) The Engine creates initializer class and initializes it with a
*     CMPXCollectionPlaylist.
*  2) This initializer will then request the media.
*  3) The Engine then calls SetActive(), which means the Engine is now
*     active, so that means the resultant media request will be forwarded
*     directly to the Engine.
*  4) The Engine can then call Next() anytime afterwards.  This will
*     make the initializer call request media on the next item in the playlist.
*  5) Once the media returns to the initializer, it will create a playback
*     plugin and initialize the file
*  6) Once initialized, the plugin stays in the initialized state.  If the
*     Engine then calls SetActive() again, the plugin and all data will
*     be swapped, and then the Engine can start playing immediately.
*
*  At anytime during the process (e.g. during request media, during
*  initialization, etc), the Engine can interrupt the process and call
*  SetActive(), which will swap any plugins or data appropriately, depending
*  on the current state.
*
*/
class CMPXPlaybackInitializer : public CBase,
                                public MMPXPlaybackPluginObserver,
                                public MMPXPluginHandlerObserver
    {
public:
    /**
     *  Two-phased constructor.
     *
     *  @param aEngine reference to engine
     *  @param aMediaHelper Media helper object
     *  @return Constructed object
     */
    static CMPXPlaybackInitializer* NewL(
        CMPXPlaybackEngine& aEngine,
        CMPXPlaybackMediaHelper& aMediaHelper );

    /**
     *  Destructor
     */
    ~CMPXPlaybackInitializer();

    /**
    * Start intializing first media in the playlist.
    * Clears any current playlist being processed
    */
    void Init(CMPXCollectionPlaylist& aPlaylist);

    /**
    * Initializes next track in playlist. Discards current
    * track being processed
    */
    TBool Next();

    /**
    * Set the playlist active in the engine. Subsequently,
    * the next track or a new playlist can be initialized
    */
    void SetActiveL();

    /**
    * Close the initializer
    */
    void Close();

    /**
    * Return the TMPXItemId of the current file in the initializer
    */
    TMPXItemId CurrentInitItemId();

    /**
     * Implementation uid of the plugin of pre-initialzer
     */
    TUid PluginUid();
    
private:
    /**
    *  C++ constructor
    *
    *  @param aEngine reference to engine
    *  @param aMediaHelper Media helper object
    */
    CMPXPlaybackInitializer(
        CMPXPlaybackEngine& aEngine,
        CMPXPlaybackMediaHelper& aMediaHelper );

    /**
    *  2nd phase contructor
    */
    void ConstructL();

    /**
    * Resets such that next media can be initialised
    */
    void Reset();

    /**
    * Get the Media from the collection - the callback triggers
    * the initialization
    */
    void RequestMediaL();

    /**
    * Updates the forwarding variable
    * to send the media to the engine if a media has been requested.
    */
    void UpdateMediaForwarding();

    /**
    * Swaps the plugin being handled in the initializer to the
    * engine, which takes over in whatever state it currently is in.
    */
    void SwapPluginL();

    /**
    * Swaps the item uri in the engine
    * Only in the EPbStateInitialising and EPbStateStopped states
    */
    void SwapUriL();

    /**
    * Swaps the item db flag in the engine
    * Only in the EPbStateInitialising and EPbStateStopped states
    */
    void SwapFlag();

    /**
    * Is this playlist the one currently being processed?
    * @param aPlaylist Playlist to check if it is already initialized
    */
    TBool IsInitializing(const CMPXCollectionPlaylist& aPlaylist);

    /**
    * Initializes a track
    * @param aMedia CMPXMedia object to initialize
    * @param aError Error returned from media request
    */
    void InitL(const CMPXMedia& aMedia,TInt aError);

    /**
    * Cancels the pre-init timer
    */
    void CancelPreInitTimer();

    /**
    * Handle pre-init timer callback
    */
    void HandlePreInitTimerCallback();

    /**
    * Callback for pre-init timer.
    */
    static TInt PreInitTimerCallback(TAny* aPtr);

    /**
     * Initialize streaming
     */
    void InitL(const TDesC& aUri, const TDesC8& aType ); 
    
private:
    /**
     *  From MMPXPlaybackPluginObserver
     *  Handle plugin event
     *
     *  @param aEvent the event type
     *  @param aData optional data
     *  @param aError error code of the event
     */
    void HandlePluginEvent(TEvent aEvent,TInt aData,TInt aError);

    /**
     *  Leaving version of CMPXPlaybackInitializer::HandlePluginEvent
     */
    void HandlePluginEventL(TEvent aEvent,TInt aData,TInt aError);

    /**
     * From MMPXPlaybackPluginObserver
     * Handle message from plug-in
     *
     * @param aMsg, message from the plug-in. Ownership not transferred.
     * @param aErr system error code. if aErr is not KErrNone, aMsg will not be
     *        used/dereferenced at playback engine.
     */
    void HandlePlaybackMessage(CMPXMessage* aMsg, TInt aErr);

    /**
     *  From MMPXPlaybackPluginObserver
     *  Async response for Value request
     *
     *  @param aProperty property key
     *  @param aValue property value
     *  @param aError error code
     */
    void HandleProperty(TMPXPlaybackProperty aProperty,
                         TInt aValue,TInt aError);

    /**
     *  From MMPXPlaybackPluginObserver
     *  Async response for subplayer request
     *
     *  @param aPlayer UID of the subplayer
     *  @param aSubPlayers array of subplayers
     *  @param aComplete ETrue, completed, EFalse, not completed
     *  @param aError error code
     */
    void HandleSubPlayerNames(TUid aPlayer,const MDesCArray* aSubPlayers,
                               TBool aComplete,TInt aError);

    /**
     *  From MMPXPlaybackPluginObserver
     *  Async response for ExtendedMediaProperties request
     *
     *  @param aMedia property object
     *  @param aError error code
     */
    void HandleMedia(const CMPXMedia& aProperties,TInt aError);

    ///////////////////////////////////////////////////////////////////////////
    // From MMPXPluginHandlerObserver

    /*
    * @see MMPXPluginHandlerObserver
    */
    void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent,
        const TUid& aPluginUid, TBool aLoaded, TInt aData);

private:
    TMPXPlaybackState iState; // State of playlist
    CMPXPlaybackPluginHandler* iHandler;
    CMPXPlaybackEngine& iEngine;
    CMPXPlaybackMediaHelper& iMediaHelper;  // not owned

    TInt iPathIndex;
    TInt iInitPlaylistCount;
    TBool iMediaToEngine;
    TBool iMediaRequested;

    TInt iDuration;
    TInt iError;
    CMPXMedia* iMedia;
    CMPXMessage* iMediaMessage; // media update message from plugin
    TInt iErrorOfMediaMessage; // error along with media update message
    CPeriodic* iTimer;
    };

#endif // CMPXPLAYBACKINITIALIZER_H
