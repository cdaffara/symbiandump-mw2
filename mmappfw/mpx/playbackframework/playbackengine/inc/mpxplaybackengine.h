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
* Description:  Controls playback via plug-ins
*
*/



#ifndef CMPXPLAYBACKENGINE_H
#define CMPXPLAYBACKENGINE_H


#include <bamdesca.h>
#include <badesca.h>
//
#include <mpxcenrepobserver.h>
#include <mpxplaybackframeworkdefs.h>
#include <mpxplaybackmessage.h>
#include <mpxplaybackplugin.h>
#include <mpxplaybackpluginobserver.h>
#include <mpxpluginhandlerobserver.h>
#include <mpxclientlist.h>
#include <mpxtaskqueue.h>
#include <mpxtaskqueueobserver.h>
#include <mpxcollectionobserver.h>
#include <mpxcollectionplaylistobserver.h>
#include <mpxcommand.h>
#include <mpxcollectionplaylist.h>
#include <mpxmessagequeue.h>
#include <mpxsubscription.h>

#include "mpxplaybackpluginhandler.h"
#include "mpxplaybackcommandbufferobserver.h"
#include "mpxaccessoryobserver.h"
#include "mpxplaybacksettings.h"

class CMPXPluginMonitor;
class CMPXPlaybackPluginHandler;
class CMPXAutoResumeHandler;
class CMPXPlaybackCmdBuffer;
class CMPXCollectionPlaylist;
class MMPXPlaybackActiveEngineObserver;
class CMPXActiveTaskQueue;
class CMPXPlaybackInitializer;
class CMPXPlaybackMediaHelper;
class CMPXPlaybackDummyMediaObserver;

// Constants
_LIT(KMPXPlaybackPluginVersion2, "MPXPlaybackPluginVersion2");
/**
*  The playback 'engine' which delegates to plug-ins (or 'players'). Unless
*  stated otherwise, methods apply to currently loaded plug-in (player) and
*  currently initialised song.
*/
class CMPXPlaybackEngine : public CBase,
                           public MMPXPlaybackPluginObserver,
                           public MMPXPlaybackCmdBufferObserver,
                           public MBeating,
                           public MMPXAccessoryEventObserver,
                           public MMPXTaskQueueObserver,
                           public MMPXCollectionMediaObserver,
                           public MMPXCollectionPlaylistObserver,
                           public MMPXPluginHandlerObserver
    {
public:

    /**
    *  Two-phased constructor.
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aObserver engine observer
    *  @param aClientListObserver client list observer
    *  @param aModeId denotes whether this player is designated for the
    *                        stand-alone application
    *  @return Constructed object
    */
    IMPORT_C static CMPXPlaybackEngine* NewL(
                           CMPXPluginMonitor& aPluginMonitor,
                           MMPXPlaybackActiveEngineObserver& aObserver,
                           MMPXClientlistObserver* aClientListObserver,
                           const TUid& aModeId);


   /**
    *  Two-phased constructor.
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aObserver engine observer
    *  @param aClientListObserver client list observer
    *  @param aModeId denotes whether this player is designated for the
    *                        stand-alone application
	*  @param aCategory category of engine (Music, Video)
    *  @return Constructed object
    */
    IMPORT_C static CMPXPlaybackEngine* NewL(
                           CMPXPluginMonitor& aPluginMonitor,
                           MMPXPlaybackActiveEngineObserver& aObserver,
                           MMPXClientlistObserver* aClientListObserver,
                           const TUid& aModeId,
                           const TInt aCategory);

						   
    /**
    *  Destructor
    */
    IMPORT_C ~CMPXPlaybackEngine();

public:

    /**
    *  Returns reference to client list
    *
    *  @return client list object
    */
    inline CMPXClientList* ClientList();

    /**
    *  Returns object encapsulating list of clients and its management
    *
    *  @return playback plugin handler
    */
    inline CMPXPlaybackPluginHandler* PluginHandler();

    /**
    *  Does this player (i.e. the currently loaded plug-in) have access to
    *  audio resources
    *
    *  @return ETrue audio resource is used by the player
    */
    inline TBool IsPlayerActive() const;

    /**
    *  Return mode id of this engine
    *
    *  @return mode id
    */
    inline const TUid& ModeId() const;
	

    /**
    *  Return category of this engine
    *
    *  @return category
    */

    inline const TInt Category() const;


    /**
    *  Current state
    *
    *  @return playback state
    */
    inline TMPXPlaybackState State() const;

    /**
    *  Current collection path, or NULL if none (i.e. not from collection)
    */
    inline const CMPXCollectionPlaylist* Playlist() const;

    /**
    *  Current song index, or KErrNotFound if none (i.e. not from collection)
    */
    inline TInt SongIndex() const;

    /**
    *  Current uri, or KNullDesC if none (i.e. uri not supplied)
    */
    inline const TDesC& Uri() const;

    /**
    *  Current file handle
    */
    inline const RFile& File() const;

    /**
     *  Retursn last active process id
     *
     *  @return process id of last active process
     */
    inline TProcessId LastActiveProcessId() const;

    /**
    *  Initializes the player with a song in the collection
    *
    *  @param aCollectionPath collection path
    *  @param aPlay Whether to initialize for playback, or
    *               else just to setup the engine first
    */
    IMPORT_C void InitL( const CMPXCollectionPlaylist& aPlaylist,
                         TBool aPlay );

    /**
    *  Initializes the player with a uri - doesn't use collection
    *
    *  @param aUri URI of the song
    *  @param aType the type of song
    */
    IMPORT_C void InitL(const TDesC& aUri,const TDesC8& aType);

    /**
    *  Initializes the player with a file - doesn't use collection
    *
    *  @aFile file handle
    */
    IMPORT_C void InitL(const RFile& aFile);
    
    /**
    *  Initializes the player with a uri and Access Point
    *
    *  @since S60 9.2
    *  @param aUri URI of the item
    *  @param aType the mime type of the item
    *  @param aAccessPoint the access point of the item
    */
    IMPORT_C void InitStreamingL(const TDesC& aUri, const TDesC8& aType, const TInt aAccessPoint);

    /**
    *  Initializes the player with a file and access point
    *
    *  @since S60 9.2
    *  @param aFile file handle
    *  @param aAccessPoint the access point of the item
    */
    IMPORT_C void InitStreamingL(const RFile& aFile, const TInt aAccessPoint);

    /**
    *  Cancel all outstanding, asynchronous requests
    */
    IMPORT_C void CancelRequests();

    /**
    *  Issue a playback command
    *
    *  @param aCmd playback command
    *  @param aData optional data
    */
    IMPORT_C void CommandL(TMPXPlaybackCommand aCmd,TInt aData=0);

    /**
     *  Issue a playback command
     *
     *  @param aCmd playback command
     *  @param aMsgQueue Message Queue for the client
     */
    IMPORT_C void CommandL(CMPXCommand& aCmd, const CMPXMessageQueue& aMsgQueue);

    /**
    *  Sets the property value. Value not set until a property changed event is
    *  returned to the client
    */
    IMPORT_C void SetL(TMPXPlaybackProperty aProperty,TInt aValue);

    /**
    *  Asynchronously retrieves the value of a property. Result will be called
    *  back by using MMPXPlaybackEngineObserver
    */
    IMPORT_C void PropertyL(TMPXPlaybackProperty aProperty,
                            MMPXPlaybackEngineObserver& aCallback);

    /**
    *  DEPRECATED
    *
    *  Asynchronously return media properties
    *  using MMPXPlaybackEngineObserver
    *
    *  @param aCallback playback callback
    *  @param aBuf parameter buffer, take over the ownership
    */
    IMPORT_C void MediaL(MMPXPlaybackEngineObserver& aCallback, CBufBase* aBuf);

    /**
    *  Asynchronously return media properties
    *  using MMPXPlaybackEngineObserver
    *
    *  @param aCallback playback callback
    *  @param aCmd Command object containing attribute spec and attribute array
    */
    IMPORT_C void MediaL(MMPXPlaybackEngineObserver& aCallback, const CMPXCommand& aCmd);

    /**
    *  Asynchronously retrieves the sub player names. See MMPXPlayerManager
    *
    *  @param aCallback playback callback
    */
    IMPORT_C void SubPlayerNamesL(TUid aPlayerUid,
                                  MMPXPlaybackEngineObserver& aCallback);

    /**
    * Gets media from the plugin
    *
    * @param aCallback Callback method to call after retrieving media
    * @param aCmd Command containing the media attributes and attribute specs.
    *             Ownership transfered
    */
    void MediaFromPluginL( MMPXPlaybackEngineObserver* aCallback,
                           CMPXCommand* aCmd );
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  Current file handle
    */
    inline const RFile64& File64() const;

    /**
    *  Initializes the player with a file - doesn't use collection
    *
    *  @aFile file handle
    */
    IMPORT_C void Init64L(const RFile64& aFile);
    
    /**
    *  Initializes the player with a file and access point
    *
    *  @since S60 9.2
    *  @param aFile file handle
    *  @param aAccessPoint the access point of the item
    */
    IMPORT_C void InitStreaming64L(const RFile64& aFile, const TInt aAccessPoint);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
public:

    /**
    *  Handle command
    *
    *  @param aCmd command
    */
    void HandleCommandL(TMPXPlaybackCommand aCmd, TInt aData=0);

private:
    enum TPlaySource
        {
        EPlayNone,
        EPlayFromCollection,
        EPlayFromUri,
        EPlayFromFile,
        EPlayFromFile64
        };

    enum TAsyncRequest
        {
        EProperty,
        EMedia,
        ESubPlayerNames,
        EHandleNext
        };

    /**
    *  Helper class to reflect playback call back to engine itself.
    */
    class TCallbackReflector : public MMPXPlaybackEngineObserver
        {
        public:

            /**
            *  Contructor
            *
            *  @param aEngine playback engine
            */
            inline TCallbackReflector(CMPXPlaybackEngine& aEngine);

            /**
            *  From MMPXPlaybackEngineObserver
            *  Handle playback property
            *
            *  @param aProperty the property
            *  @param aValue the value of the property
            *  @param aError error code
            */
            inline void HandleProperty(TMPXPlaybackProperty aProperty,
                                       TInt aValue,
                                       TInt aError);

            /**
            *  From MMPXPlaybackEngineObserver
            *  Method is called continously until aComplete=ETrue, signifying that
            *  it is done and there will be no more callbacks
            *  Only new items are passed each time
            *
            *  @param aPlayer UID of the subplayer
            *  @param aSubPlayers a list of sub players
            *  @param aComplete ETrue no more sub players. EFalse more subplayer
            *                   expected
            *  @param aError error code
            */
            inline void HandleSubPlayerNames(TUid aPlayer,
                                             const MDesCArray* aSubPlayers,
                                             TBool aComplete,
                                             TInt aError);

            /**
            *  From MMPXPlaybackEngineObserver
            *  Handle media object
            *
            *  @param aMedia media object
            *  @param aError error code
            */
            inline void HandleMedia(CMPXMedia* aMedia, TInt aError);

        private:
            CMPXPlaybackEngine& iEngine;
        };

private:

    /**
    *  C++ constructor
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aObserver engine observer
    *  @param aModeId player engine mode
    */
    CMPXPlaybackEngine(CMPXPluginMonitor& aPluginMonitor,
                       MMPXPlaybackActiveEngineObserver& aObserver,
                       const TUid& aModeId);
					   

    /**
    *  C++ constructor
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aObserver engine observer
    *  @param aModeId player engine mode
    *  @param aCategory player engine category
    */

    CMPXPlaybackEngine(CMPXPluginMonitor& aPluginMonitor,
                       MMPXPlaybackActiveEngineObserver& aObserver,
                       const TUid& aModeId,
                       const TInt aCategory);


    /**
    *  2nd phase contructor
    *
    *  @param aClientListObserver client list observer
    */
    void ConstructL(MMPXClientlistObserver* aClientListObserver);


private: // Functions from base class

    /**
    *   From MBeating
    *   Handles a regular heartbeat timer event
    */
    void Beat();

    /**
    *   From MBeating
    *   Synchronises the heartbeat timer with system clock
    */
    void Synchronize();

    /**
    *   From MMPXPlayerAccessoryEventObserver
    *   Handle accessory event
    *
    *   @param aMode playback accessory mode
    */
    void HandleAccesoryEventL(TMPXPlaybackAccessoryMode aMode);

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
                         TInt aValue, TInt aError);

    /**
    *  From MMPXPlaybackPluginObserver
    *  Async response for MediaL request
    *
    *  @param aMedia media object
    *  @param aError error code
    */
    void HandleMedia(CMPXMedia* aMedia, TInt aError);

    /**
    *  From MMPXPlaybackPluginObserver
    *  Async response for subplayer request
    *
    *  @param aPlayer UID of the subplayer
    *  @param aSubPlayers array of subplayers
    *  @param aComplete ETrue, completed, EFalse, not completed
    *  @param aError error code
    */
    void HandleSubPlayerNames(TUid aPlayer, const MDesCArray* aSubPlayers,
                              TBool aComplete, TInt aError);

    /**
    *  From MMPXPlaybackCmdBufferObserver
    *  Handle command from buffering
    *
    *  @param aEvent comand info
    */
    void HandleCmdBufferEventL(const TMPXPlaybackCmdInfo& aEvent);

    /**
    *  From MMPXPlaybackCmdBufferObserver
    *  Handle command skip event from key buffering
    *
    * @param aSkipEvent Skip event
    */
    void HandleCmdBufferSkipEventL(const TMPXPlaybackSkipEvent aSkipEvent);

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
    *  From MMPXCollectionMediaObserver
    *  Handle media properties from collection
    *
    *  @param aMedia media
    *  @param aError error code
    */
    void HandleCollectionMediaL(const CMPXMedia& aProperties, TInt aError);

    /**
    *  From MMPXCollectionPlaylistObserver
    *  Handle collection playlist change
    *
    *  @param aError KErrNotFound - Playlist is updated, current item removed
    *                KErrNone - Playlist is updated, current item is valid
    *                KErrEof - Playlist is updated, current item removed and
    *                          reached to the end of playlist
    */
    void HandleCollectionPlaylistChange(TInt aError);

    ///////////////////////////////////////////////////////////////////////////
    // From MMPXPluginHandlerObserver

    /*
    * @see MMPXPluginHandlerObserver
    */
    void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
        TBool aLoaded, TInt aData);

private:  // New functions


    /**
    *  Return current plugin
    *
    *  @return plugin object
    */
    inline CMPXPlaybackPlugin* PluginL() const;

    /**
    *  Suspend playback
    */
    void Suspend();

    /**
    *  Seek timer tick
    */
    void SeekTimerTick();

    /**
    *  Seek timer callback
    *
    *  @param aPtr pointer the this
    */
    static TInt SeekTimerCallback(TAny* aPtr);

    /**
    *  Init plugin
    *
    *  @param aSong, song path
    *  @param aType, song type
    *  @param aFile, file handle
    *  @param aAccessPoint aAccessPoint
    */
    void InitL(const TDesC* aSong,const TDesC8* aType,RFile* aFile, 
            TInt aAccessPoint=0);

    /**
    *  Handle play command
    */
    void HandlePlayL();

    /**
    *  Handle play command with fade in
    */
    void HandlePlayWithFadeInL();

    /**
    *  Handle pause command
    */
    void HandlePauseL();

    /**
    *  Handle stop command
    */
    void HandleStopL();

    /**
    *  Stop play back
    *  @param aSavePlaybackInfo flag to determine if the PlaybackInfo should be saved
    */
    void DoStopL(TBool aSavePlaybackInfo = ETrue);

    /**
    *  Handle next command
    *  @param aOffset offset to current index
    *  @param aIgnoreRepeat ETrue to ignore repeat setting
    */
    void HandleNextL( TInt aOffset = 1, TBool aIgnoreRepeat=EFalse );

    /**
    *  Handle previous command
    *  @param aOffset offset to current index
    *  @param aIgnoreRepeat ETrue to ignore repeat setting
    */
    void HandlePreviousL(TInt aOffset = -1, TBool aIgnoreRepeat=EFalse );

    /**
    *  Handle replay command
    */
    void HandleReplayL();

    /**
    *  Handle sart seekingcommand
    */
    void HandleStartSeekL(TBool aForward);

    /**
    *  Handle stop seeking command
    */
    void HandleStopSeekingL();

    /**
    *  Handle increase volume command
    */
    void HandleIncreaseVolumeL();

    /**
    *  Handle decrease volume command
    */
    void HandleDecreaseVolumeL();

    /**
    *  Handle set volume command
    */
    void HandleSetVolumeL(TInt aVolume);

    /**
    *  Handle mute command
    */
    void HandleMuteL(TBool aMute);

    /**
    *  Handle close command
    *  @param aData data to be forwarded to the player
    */
    void HandleCloseL( TInt aData = KErrNone );

    /**
    * Handle a custom command
    */
    void HandleEffectL(TInt aData);

    /**
    * Handle a custom command
    */
    void HandleDisableEffectL();

    /**
    * Handle a request to close the file handle for an item
    * @param aItemId item that is requested to be freed
    */
    void HandleCloseItemL( TInt aItemId );

    /**
    *  Sets the current properties for the plug-in
    */
    void SetPropertiesL();

    /**
    *  Stop seeking timer
    */
    void EndSeek();

    /**
    * Set a playback state
    * @param aState new state
    */
    void SetStateL(TMPXPlaybackState aState);

    /**
    * Set a playback state, takes in a TMPXPlaybackMessage with the
    * state already set as the iType parameter.  This Allows caller to
    * set iData information as well.
    * @param aMessage Message containing state information
    */
    void SetStateL( TMPXPlaybackMessage& aMsg );

    /**
    * Set player activated
    * @param aActive ETrue actived, EFalse inactived.
    */
    void SetPlayerActivated(TBool aActive);

    /**
    * Execute a task
    * @param aTask task number
    * @param aParamData, parameter
    * @param aPtrData, any object
    * @param aCallback call back function pointer
    * @param aBuf buffer data
    * @param aCObject1 object 1 owned by task queue
    * @param aCObject2 object 2 owned by task queue
    */
    void ExecuteTaskL(TInt aTask,
                      TInt aParamData,
                      TAny* aPtrData,
                      TAny* aCallback,
                      const CBufBase& aBuf,
                      CBase* aCObject1,
                      CBase* aCObject2);

    /**
    * @see MMPXTaskQueueObserver
    */
    void HandleTaskError(TInt aTask,
                         TAny* aPtrData,
                         TAny* aCallback,
                         TInt aError);

    /**
    * Handle error of execution
    * @param aTask task number
    * @param aErr error number
    * @param aParamData parameter
    */
    void HandleError(TInt aTask, TInt aErr, TInt aParamData = 0);

    /**
    * Get command from state
    */
    TMPXPlaybackCommand CommandFromState( TMPXPlaybackState aState );

    /**
    * Cleanup when the engine closed
    */
    void DoClose();

    /**
    * Media request from collection
    */
    void MediaFromCollectionL();

    /**
    *  Handle plugin event
    *
    *  @param aEvent the event type
    *  @param aData optional data
    *  @param aError error code of the event
    */
    void DoHandlePluginEventL(TEvent aEvent,TInt aData,TInt aError);

    /**
    *  Update playbackengine state machine
	*  Handles plugin events that could cause state change in playback-engine
    *
    *  @param aEvent the event type
    *  @param aData optional data
    *  @param aError error code of the event
    */
    void UpdateStateMachineL(TEvent aEvent,TInt aData,TInt aError);    
    
   /*
    * Handle plugin event error handling
    * This is internal function called from DoHandlePluginEvent()
    * @param aEvent the event type
    * @param aError error code of the event
    */    
    void HandlePluginEventErrorHandling(TEvent aEvent, TInt aError);

    /*
     * Handle Plugin Event: Initialised
     * Handles the event called Initialised
     * @param aState the mpx playbackstate
     * @param aData data passed from playback plugin in call to HandlePluginEvent
     */
    void HandlePluginEventInitialisedL(TMPXPlaybackState& aState, TInt aData);    
    
    /**
    * Save playback Information
    */
    void SavePlaybackInfoL();

    /**
    * Increments play count
    */
    void SavePlaybackCompleteInfoL();

    /**
    * Restore playback position if it was saved previously
    */
    void RestorePlaybackPositionL( const CMPXMedia& aMedia );

    /**
    * Restore playback state if it was saved previously
    */
    void RestorePlaybackStateL();
  
    /**
    * Sets the volume increment depending on accessory state
    */
    void SetVolumeIncrement( TMPXPlaybackAccessoryMode aMode );

    /**
    * Marks the current item as corrupted
    * @param aCorrupted Flag if corrupted or not
    */
    void MarkItemCorrupted( const TBool aCorrupted );

    /**
    * Marks the current item as invalid
    * @param aInvalid Flag if invalid or not
    */
    void MarkItemInvalid( const TBool aInvalid );

    /**
    * Marks the current item as DRM invalid (e.g. no rights)
    * @param aDrmInvalid Flag if invalid or not
    */
    void MarkItemDrmInvalid( const TBool aDrmInvalid );

    /**
    * Sets flag bits
    * @aSet ETrue to set, EFalse to unset
    * @aFlag Flag bits to set/unset
    */
    void SetFlagBitsL( const TBool aSet, const TUint aFlag );

    /**
    * Creates a CMPXMedia object to be used to Set attributes
    * @param aPath Collection Path
    * @return CMPXMedia object
    */
    CMPXMedia* CreateMediaToSetLC( const CMPXCollectionPath& aPath );

    /**
    * Sends a request to iMediaHelper to request media
    */
    void RequestMediaL();

    /**
    * Sends a plugin handler message to all clients.
    * @param aMessageId the value of the KMPXMessageGeneralId attribute
    * @param aPluginUid plugin UID this message is for
    * @param aLoaded ETrue if the plugin is loaded
    * @param aVersion plugin version
    */
    void SendPluginHandlerMessageL(TInt aMessageId, const TUid& aPluginUid,
        TBool aLoaded, TInt aVersion = 0);
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  Init plugin
    *
    *  @param aSong, song path
    *  @param aType, song type
    *  @param aFile, file handle
    *  @param aAccessPoint aAccessPoint
    */
    void Init64L(RFile64* aFile, TInt aAccessPoint=0);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  Callback for sync message timer
    *
    *  @since 9.2    
    *  @param this
    *  @return systme error
    */         
    static TInt SyncMsgTimerCallback(TAny* aPtr);
    /**
    *  Cancel the sync message timer
    *
    *  @since 9.2    
    */    
    void CancelSyncMsgTimer();    
    /**
    *  Stop the wait loop
    *
    *  @since 9.2    
    */    
    void StopWaitLoop();
    /**
     * Handle a synchronous message
     * 
     * @since 9.2
     * @param aMsg, message
     * @return system error
     */
    TInt HandlePlaybackSyncMessage (const CMPXMessage& aMsg);
    
private:
    friend class TCallbackReflector;
    friend class CMPXPlaybackInitializer;

    TCallbackReflector iReflector;
    CMPXPluginMonitor& iPluginMonitor;
    CMPXClientList* iClientList;
    CMPXPlaybackInitializer* iInitializer;
    CMPXPlaybackPluginHandler* iPluginHandler;
    TBool iInitNext;
    /**
    *   Utilities
    */
    CHeartbeat* iProgressTimer;
    CPeriodic* iSeekTimer;
    CMPXAccessoryObserver* iAccessoryObs;
    CMPXPlaybackCmdBuffer* iCmdBuffer;
    CMPXPlaybackSettings* iPlaybackSettings;
    CMPXAutoResumeHandler* iAutoResumeHandler;

    /**
    *   Playlist/song data
    */
    CMPXCollectionPlaylist* iPlaylist;
    HBufC* iUri;
    TMPXItemId iItemId;
    RFile iFile;
    TPlaySource iPlaySource;
    TUint iDbFlag;
    TInt iMediaDuration;    // Duration value inside CMPXMedia
    
    TInt iAccessPoint;
    TBool iAccessPointSet;  // Access Point is set
    
    /**
    *   This player data
    */
    TMPXPlaybackState iState; // current state
    TMPXPlaybackState iNextState; // next possible state when handling commands
    TMPXPlaybackState iPluginState; // plugin state
    TBool iEmbeddedMode;
    TUid iModeId;
    TInt iCategory;
    TBool iPlayerActive;
    //
    TFixedArray<TInt,EPbPropertyNum> iProperties;
    TInt iSeekStep; // millisecond
    TInt iMaxSeekStep; // millisecond
    MMPXPlaybackActiveEngineObserver& iObserver;
    CMPXActiveTaskQueue* iTaskQueue;
    MMPXPlaybackEngineObserver* iCallback; // Not owned.

    TMPXPlaybackState iPreservedState;
    TInt iPreservedPosition;

    TInt iVolumeIncrement;
    RArray<TMPXAttribute> iMediaAttrs;
    TBool iSkipping;
    CMPXPlaybackMediaHelper* iMediaHelper;

    CMPXPlaybackDummyMediaObserver* iDummyMediaObserver;
    TUid iPluginUid; // plugin uid for current item of current playlist
    TProcessId iLastActiveProcess; // Last active process for this engine
    TProcessId iLastInactiveProcess; // Last inactive process for this engine
    TBool iInitVolume;  // Initialize volume on first creation
    // the index which be firstly saved when request media
    TInt iFirstRequestMediaIndex; 
    CPeriodic* iSyncMsgTimer;  //timer for sync message
    CActiveSchedulerWait* iSyncMsgWait;  // wait loop use to sync message    
#if defined(__HIGH_RESOLUTION_VOLUME)
    // flag to indicate whether the volume setting has been rounded up last
    // used to adjust volume up button setting 
    TBool iVolRoundedUp;
#endif
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    RFile64 iFile64;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
    TInt iPositionFromMedia; 
    
    };

#include "mpxplaybackengine.inl"

#endif // CMPXPLAYBACKENGINE_H

