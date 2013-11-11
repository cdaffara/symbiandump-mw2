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
* Description:  Implementation of interface for playbackutility
*
*/



#ifndef CMPXPLAYBACKUTILITY_H
#define CMPXPLAYBACKUTILITY_H

#include <e32base.h>
#include <f32file.h>
#include <mpxplaybackobserver.h>
#include <mpxplaybackmessage.h>
#include <mpxsession.h>
#include <mpxmessageobserver.h>
#include "mpxplaybackutility.h"

class CMPXMessageMonitor;
class CMPXTaskQueue;

NONSHARABLE_STRUCT( TMPXPlaybackUtilityStruct )
    {
    // Default mode playback utility
    MMPXPlaybackUtility* iDefaultPlaybackUtility;
    };

/**
*  Implementation of playback utility interface
*  this interface:
*/
NONSHARABLE_CLASS(CMPXPlaybackUtility) : public CActive,
                            public MMPXPlaybackUtility,
                            public MMPXMessageObserver,
                            public MMPXPlayer,
                            public MMPXSource,
                            public MMPXPlayerManager
    {
public:
    /**
    * Two-phased constructor
    *
    * @param aModeId playback mode id
    * @param aObs observer
	* @param aCategory category of client / engine
    * @return object of constructed
    */
    static CMPXPlaybackUtility* NewL(const TUid& aModeId,
                                     MMPXPlaybackObserver* aObs,
                                     const TMPXCategory aCategory);

    /**
    * Gets the interface for the client, or creates a new one.
    * This interface could be a singleton
    */
    static MMPXPlaybackUtility* UtilityL( const TUid& aModeId,
                                        const TMPXCategory aCategory);										

private:
    /**
    * Destructor
    */
    ~CMPXPlaybackUtility();

    /**
    * C++ constructor
    */
    CMPXPlaybackUtility();

    /**
    * Second phase constructor
    *
    * @param aModeId playback mode id
    * @param aObs observer
	* @param aCategory category of client / engine
    */
    void ConstructL(const TUid& aModeId, MMPXPlaybackObserver* aObs, const TMPXCategory aCategory);
    /**
    * Second phase constructor
    *
    * @param aObs observer
    */
    void ConstructL(MMPXPlaybackObserver* aObs);

    /**
    *  From MMPXPlaybackUtility
    *  Add a Observer. Clients only receive events from the engine corresponding
    *  to the specific mode the interface was created in
    *
    *  @param aObs, observer to be added
    */
    void AddObserverL(MMPXPlaybackObserver& aObs);

    /**
    *  From MMPXPlaybackUtility
    *  Remove a observer.
    *
    *  @param aObs, observer to be removed
    */
    void RemoveObserverL(MMPXPlaybackObserver& aObs);

    /**
    *  From MMPXPlaybackUtility
    *  Returns PIds of clients that are using the engine in the mode this
    *  interface was created in. For example, if a client was interested in all
    *  the clients' pids with an active engine, they would:
    *
    *       MMPXPlaybackUtility* m=MMPXPlaybackUtility::NewL(EActivePlayer);
    *       RArray<TProcessId> pids;
    *       m->GetClients(pids);
    *       .........
    *       pids->Close();
    *       m->Close();
    *
    *  @param aClients Array of Pids returned.
    */
    void GetClientsL(RArray<TProcessId>& aClients);

    /**
    *  From MMPXPlaybackUtility
    *  Initializes a track given by aCollectionPath
    *
    *  @param aCollectionPath collection path
    *  @param aPlay Whether to initialize for playback, or
    *               else just to setup the engine first
    */
    virtual void InitL(const CMPXCollectionPlaylist& aPlaylist,
                       TBool aPlay=ETrue);

    /**
    *  From MMPXPlaybackUtility
    *  Inititialises with a single item, may not be part of any collection
    *
    *  @param aUri URI of the item
    *  @param aType the mime type of the item
    */
    void InitL(const TDesC& aUri,const TDesC8* aType=NULL);

    /**
    *  From MMPXPlaybackUtility
    *  Inititialises with a single item, may not be part of any collection
    *  Before this function can be called, the file server session which owns
    *  this file handle must first be marked as shareable by calling
    *  RFs::ShareProtected().
    *
    *  @param aFile file handle
    */
    void InitL(RFile& aShareableFile);

    /**
    *  From MMPXPlaybackUtility
    *  Frees up client side resources only; a player is freed when there are no
    *  clients using it, and all resources are freed when the last client closed
    */
    void Close();

    /**
    *  From MMPXPlaybackUtility
    *  Stops any async operations that are currently under way
    */
    void CancelRequest();

    /**
    *  From MMPXPlaybackUtility
    *  Issue player commands, with optional data. ECommandReceived and
    *  ECommandComplete events should follow
    *
    *  @param aCmd the command
    *  @param aData optional data
    */
    void CommandL(TMPXPlaybackCommand aCmd,TInt aData=0);

    /**
     *  From MMPXPlaybackUtility
     *  Send a command to the playback server
     *
     *  @param aCmd, reference to command object
     *  @param aCallback, call back when command exection completed.
     *                    Asynchronously command only
     */
    void CommandL(CMPXCommand& aCmd, MMPXPlaybackCallback* aCallback=NULL);

    /**
    *  From MMPXPlaybackUtility
    *  Current state of player
    *  @return current state
    */
    TMPXPlaybackState StateL() const;

    /**
    *  From MMPXPlaybackUtility
    *  Current source being played, NULL if none was supplied
    *
    *  @return source object
    */
    MMPXSource* Source();

    /**
    *  From MMPXPlaybackUtility
    *  Player manager
    *
    *  @return reference to player manager
    */
    MMPXPlayerManager& PlayerManager();

    /**
    *  From MMPXPlaybackUtility
    *  Set playback property, EPropertyChanged event when complete
    *
    *  @param aProperty the property to be set
    *  @param aValue the the value of the property
    */
    void SetL(TMPXPlaybackProperty aProperty,TInt aValue);

    /**
    *  From MMPXPlaybackUtility
    *  Get value of a playback property, Asyc
    *
    *  @param aCallback playback callback
    *  @param aProperty the property
    */
    void ValueL(MMPXPlaybackCallback& aCallback,TMPXPlaybackProperty aProperty);

    /**
     *  Get value of a playback property, Asyc
     *  Result will be called back in HandleProperty
     *
     *  @param aCallback playback callback
     *  @param aProperty the property
     */
    void PropertyL(MMPXPlaybackCallback& aCallback,
                   TMPXPlaybackProperty aProperty);

    /**
    *  From MMPXPlaybackUtility
    *  Return a list of mime types supported by playback framework
    *
    *  @return an array of mime types
    */
    virtual CDesCArray* SupportedMimeTypes();

    /**
    *  From MMPXPlaybackUtility
    *  Return a list of file extensions supported by playback framework
    *
    *  @return an array of extensions
    */
    virtual CDesCArray* SupportedExtensions();

    /**
    *  From MMPXPlaybackUtility
    *  Return a list of schemas supported by playback framework
    *
    *  @return an array of schemas
    */
    virtual CDesCArray* SupportedSchemas();

    /**
    *  Sets the priority of the playback utility
    *
    *  @param aPriority Priority to set
    */
    virtual void SetPriority( TInt aPriority );

    /**
    * Adds a message subscription for this client.
    * @param aSubscription subscription to be added
    */
    void AddSubscriptionL(const CMPXSubscription& aSubscription);

    /**
    * Removes a message subscription for this client.
    * @param aSubscription subscription to be removed.
    */
    void RemoveSubscriptionL(const CMPXSubscription& aSubscription);

    /**
    * Removes all message subscriptions for this client.
    */
    void ClearSubscriptionsL();

    /**
    *  From MMPXPlayerManager
    *  Typically, a client will obtain the list of all the (non-duplicated)
    *  player 'types' in the system from GetPlayerTypesL and present localized
    *  type names (e.g. "Local","Home network", if possible, else if
    *  non-standard type,should use PlayerTypeDisplayNameL to get text from
    *  player).
    *
    *  User should be given opportunity to choose IF there is more than one
    *  type available, or if there is one type but more than one sub players,
    *  the user could be presented with the sub player list.
    *
    *  @param aTypes the list of player types
    */
    void GetPlayerTypesL(RArray<TMPXPlaybackPlayerType>& aTypes);

    /**
    *  From MMPXPlayerManager
    *  Returns display name for custom types; standard types should be
    *  recognized by client and localized - calling this method would then not
    *  be required
    *
    *  @param aType playback type
    */
    HBufC* PlayerTypeDisplayNameL(TMPXPlaybackPlayerType aType);

    /**
    *  From MMPXPlayerManager
    *  Get the list of UIDs of all players
    *
    *  @param aPlayers All the players' uids in the system
    */
    void GetPlayerListL(RArray<TUid>& aPlayers);

    /**
    *  From MMPXPlayerManager
    *  Get the list of UIDs of players with the specific type
    *
    *  @param aPlayers All the players' uids with the same type
    *  @param aType the type of players
    */
    void GetPlayerListL(RArray<TUid>& aPlayers,TMPXPlaybackPlayerType aType);

    /**
    *  From MMPXPlayerManager
    *  Get the list of sub player names, Async
    *
    *  @param aCallback sub players returned in call back interface
    *  @param aPlayer UID of the player
    */
    void SubPlayerNamesL(MMPXPlaybackCallback& aCallback,TUid aPlayer);

    /**
    *  From MMPXPlayerManager
    *  Client selects all players with the type aType, the specific player used
    *  then being resolved on the basis of the content.
    *
    *  @param aType the type of player
    */
    void SelectPlayersL(TMPXPlaybackPlayerType aType);

    /**
    *  From MMPXPlayerManager
    *  Client selects specific player aPlayer and sub player aSubPlayerIndex
    *  from array returned by SubPlayerNamesL
    *
    *  @param aPlayer the UID of player
    *  @param aSubPlayerIndex the index of the sub player
    */
    void SelectSubPlayerL(TUid aPlayer,TInt aSubPlayerIndex);

    /**
    *  From MMPXPlayerManager
    *  Client selects a specific player
    *
    *  @param aPlayer the UID of the player
    */
    void SelectPlayerL(TUid aPlayer);

    /**
    *  From MMPXPlayerManager
    *  Clears all selection criteria. Essentially the same as
    *  SelectPlayersL(EPbLocal).
    */
    void ClearSelectPlayersL();

    /**
    *  From MMPXPlayerManager
    *  Retreives the current selection: KNullUid and KErrNotFound are possible
    *  return values for aPlayer and aSubPlayerIndex respectively if none are
    *  explicitly selected
    *
    *  @param aType player type
    *  @param aPlayer the UID of player
    *  @param aSubPlayerIndex index of the subplayer
    *  @param aSubPlayerName friendly name of the subplayer
    *         Notes: Client owns the returned aSubPlayerName object.
    */
    void GetSelectionL(TMPXPlaybackPlayerType& aType,
                       TUid& aPlayer,
                       TInt& aSubPlayerIndex,
                       HBufC*& aSubPlayerName);

    /**
    *  From MMPXPlayerManager
    *  The current player, may change with each item, NULL if none found
    *
    *  @return object of current player
    */
    MMPXPlayer* CurrentPlayer();

    /**
    *  From MMPXPlayer
    *  The 'type' of the player. Built-in types are EPbLocal and EPbRemote, but
    *  the plug-ins could extend this if they desire to be a different type
    *  which can be selected by the user
    *
    *  @return player type
    */
    TMPXPlaybackPlayerType TypeL();

    /**
    *  From MMPXPlayer
    *  If TypeL returns a value that is not known to the UI (and therefore
    *  can't localize it),then TypeNameL can be used for unlocalized display
    *  purposes.
    *
    *  @return player name.
    */
    HBufC* TypeNameL();

    /**
    *  From MMPXPlayer
    *  If the player supports other 'players', else array count == 0.
    *
    *  @param aCallback sub players returned in the callback
    */
    void SubPlayerNamesL(MMPXPlaybackCallback& aCallback);

    /**
    *  From MMPXPlayer
    *  Selected sub player index, corresponding to sub player names array.
    *
    *  @return sub player index. KErrNotFound if none
    */
    TInt SubPlayerL() const;

    /**
    *  From MMPXPlayer
    *  The UID identifying this player
    *
    *  @return UID of the player
    */
    TUid UidL() const;

    /**
    *  From MMPXSource
    *  Path to the collection, or NULL if not in any collection
    *  Ownership transferred
    *
    *  @return collection path
    */
    CMPXCollectionPlaylist* PlaylistL();

    /**
    *  From MMPXSource
    *  File passed in, or NULL if none passed in
    *
    *  @return pointer to file handle
    */
    RFile* FileL();

    /**
    *  From MMPXSource
    *  URI of current item
    *
    *  @return URI of the item
    */
    HBufC* UriL();

    /**
    *  From MMPXSource
    *
    *  DEPRECATED
    *
    *  Media request
    *  HandleMediaL of the callback
    *
    *  @param aAttrs attributes requested
    *  @param aCallback playback callback
    */
    void MediaL(const TArray<TMPXAttribute>& aAttrs,
                MMPXPlaybackCallback& aCallback);

    /**
    *  From MMPXSource
    *  Media request
    *  HandleMediaL of the callback
    *
    *  @param aAttrs attributes requested
    *  @param aCallback playback callback
    *  @param aSpecs, specifications for attributes
    */
    void MediaL(const TArray<TMPXAttribute>& aAttrs,
                MMPXPlaybackCallback& aCallback,
                CMPXAttributeSpecs* aSpecs);

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  From MMPXSource
    *  File passed in, or NULL if none passed in
    *
    *  @return pointer to file handle
    */
    RFile64* File64L();
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

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

    /**
     * From MMPXMessageObserver
     * Message received
     * @param aMsgData message data
     * @param aError error code
     */
    void MessageReceived(TInt aMsgData, TInt aError);
    
    /**
    *  Inititialises for streaming with URI and Access Point
    *
    *  @since S60 9.2
    *  @param aUri URI of the item
    *  @param aType the mime type of the item
    *  @param aAccessPoint the access point of the item
    */
    virtual void InitStreamingL(const TDesC& aUri, const TDesC8* aType, const TInt aAccessPoint);

    /**
    *  Inititialises for streaming with URI and Access Point, the file may not be part of any collection
    *  Before this function can be called, the file server session which owns
    *  this file handle must first be marked as shareable by calling
    *  RFs::ShareProtected().
    *
    *  @since S60 9.2
    *  @param aShareableFile shareable file handle
    *  @param aAccessPoint the access point of the item
    */
    virtual void InitStreamingL(RFile& aShareableFile, const TInt aAccessPoint); 

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /** 
    *  Inititialises with a single item, may not be part of any collection 
    *  Before this function can be called, the file server session which owns 
    *  this file handle must first be marked as shareable by calling 
    *  RFs::ShareProtected(). 
    * 
    *  @since S60 9.2 
    *  @param aShareableFile shareable RFile64 file handle 
    */ 
    virtual void Init64L(RFile64& aShareableFile);
     
    /** 
     *  Inititialises for streaming with URI and Access Point, the file may not be part of any collection 
     *  Before this function can be called, the file server session which owns 
     *  this file handle must first be marked as shareable by calling 
     *  RFs::ShareProtected(). 
     * 
     *  @since S60 9.2 
     *  @param aShareableFile shareable RFile64 file handle 
     *  @param aAccessPoint the access point of the item 
     */ 
    virtual void InitStreaming64L(RFile64& aShareableFile, const TInt aAccessPoint);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
    /** 
     *  Sets the primary client 
     * 
     *  @since S60 9.2
     *  @return system error 
     */     
     TInt SetPrimaryClientL();
private:


    /**
    * Helper to return a descriptor buffer
    *
    * @param aSize size of the buffer
    */
    HBufC* DesL(TInt aSize);

    /**
    *  Add a request
    *
    *  @param aFunction function code
    *  @param aCallback callback
    *  @param aParamData data
    *  @param aBuf buffer data, ownership transferred
    *  @param aPtr, pointer to a object
    *  @param aAlivePtr1, object to keep alive for a task
    *                     ownership transferred
    *  @leave leave with system error code when failed
    */
    void AddRequestL(TMPXPlaybackServerOp aFunction,
                     MMPXPlaybackCallback* aCallback=NULL,
                     TInt aParamData=0,
                     CBufBase* aBuf=NULL,
                     TAny* aPtr=NULL,
                     CBase* aAlivePtr=NULL);

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
    RArray<MMPXPlaybackObserver*> iObservers;
    CMPXMessageMonitor* iMsgMonitor;
    CMPXTaskQueue* iTaskQueue;
    RMPXSession iPbs;
    CBufBase* iBuffer;
    TPckgBuf<TInt> iPropertyValuePckg;
    TPckgBuf<TBool> iCompletePckg;
    TPckgBuf<TInt> iResultSizePckg;
    RFile iFile;
    TBool iCallbackOngoing; // whether client is processing callback or not
#ifdef _ENABLE_GUARD_TIMER
    CPeriodic* iGuardTimer; // guard timer for async request
#endif
    CMPXMedia* iMediaOnError; // media object while handling error
    TInt iRefCount;
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    RFile64 iFile64;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    };

#endif // CMPXPLAYBACKUTILITY_H
