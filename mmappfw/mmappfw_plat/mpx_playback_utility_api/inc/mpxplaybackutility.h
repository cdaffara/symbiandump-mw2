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
* Description:  Interface to playback
*
*/


#ifndef MMPXPLAYBACKUTILITY_H
#define MMPXPLAYBACKUTILITY_H

#include <mpxplaybackframeworkdefs.h>
#include <mpxcommonframeworkdefs.h>
#include <badesca.h>
#include <mpxattribute.h>
#include <mpxcommand.h>
#include <mpxattributespecs.h>

class RFile;
class CMPXMedia;
class MMPXPlayer;
class MMPXSource;
class MMPXPlayerManager;
class MMPXPlaybackObserver;
class MMPXPlaybackCallback;
class CMPXCollectionPlaylist;
class CMPXSubscription;
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
class RFile64;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

/**
*  Main client interface for playback. Some terms used in the documentation of
*  this interface:
*
*    Engine - it is the 'engine' which manages the playback and maintains the
*             state. An abstract concept for where the playback functionality
*             is somehow implemented, but is completely hidden from the client.
*
*    Player - the engine may access of a number of 'players' for playback,
*             using only one at a time. A player is an abstract concept
*             representing the knowledge to play either a 'specific kind of item',
*             or 'play in a specific way', or both. The player may therefore
*             automatically change between items if required
*             ('specific kind of item'), or the client may choose the players
*             that are used ('play in a specific way').
*
*    Sub player - a player may support 0 or more 'sub' players, a logical
*             concept whereby the client,having chosen a player, can select
*             options within this player identified by name (text)
*
*    Playlist - an abstract list of items. A collection path allows the client
*             to cross reference the playlist with a collection, for example to
*             iterate through the playlist, extract user-supplied metadata etc.
*
*    @lib mpxplaybackutility.lib
*
*/
class MMPXPlaybackUtility
    {
public:
    /**
    *  Creates the interface for the client. This interface is bound to (i.e.
    *  controls and recieves events from) either an existing engine or a new
    *  one, according to:
    *
    *    aModeId=KPbModeDefault - bound to the same engine as any other player
    *                           running in same thread, or KPbModeNewPlayer if none.
    *    aModeId=KPbModeActivePlayer - bound to whichever engine has audio access,
    *                           else whichever is running, provided the category 
	*							of client and engine match. Else KPbModeNewPlayer if none.
    *    aModeId=KPbModeNewPlayer - new engine created specifically for client,
    *                            may be shared later.
    *    aModeId=Application UID - bound to the same engine as the stand alone
    *                            Application.
    *	When no category is supplied (as with this API), EMPXCategoryMusic is assumed.
	*	For category description, refer to next API. 
	*
    *  @since S60 3.2.3
    *  @param aModeId playback mode
    *  @param aObs plabyback observer
    *  @return playback utility object
    */
    IMPORT_C static MMPXPlaybackUtility* NewL(
                                        const TUid& aModeId = KPbModeDefault,
                                        MMPXPlaybackObserver* aObs=NULL);


    /**
    *  Creates the interface for the client. This interface is bound to (i.e.
    *  controls and recieves events from) either an existing engine or a new
    *  one, according to:
    *
    *    aModeId=KPbModeDefault - bound to the same engine as any other player
    *                           running in same thread, or KPbModeNewPlayer if none.
    *    aModeId=KPbModeActivePlayer - bound to whichever engine has audio access,
    *                           else whichever is running, provided the category 
	*							of client and engine match. Else KPbModeNewPlayer if none.
    *    aModeId=KPbModeNewPlayer - new engine created specifically for client,
    *                            may be shared later.
    *    aModeId=Application UID - bound to the same engine as the stand alone
    *                            Application.
    *		
	*	 aCategory - a client can associate itself with a category (TMPXCategory).
	*	 Category is used primarily with the KPbModeActivePlayer mode. The client gets 
	*	 EActivePlayerChanged message only when an engine with the same category becomes  
	*	 active. If a new engine (player) is created, the engine is tagged 
	*	 with the category. When no category is supplied, EMPXCategoryMusic is assumed.
	*	 
    *  @since S60 3.2.3
    *  @param aCategory engine category (TMPXCategory)
    *  @param aModeId playback mode
    *  @param aObs plabyback observer
    *  @return playback utility object
    */
    IMPORT_C static MMPXPlaybackUtility* NewL(
                                            const TMPXCategory aCategory,
                                            const TUid& aModeId = KPbModeDefault,
                                            MMPXPlaybackObserver* aObs=NULL);

    /*
    * Gets the interface for the client, or creates a new one.
    * This interface could be a singleton.
    *
    * @since S60 3.2.3
    */
    IMPORT_C static MMPXPlaybackUtility* UtilityL(
                                        const TUid& aModeId = KPbModeDefault );

    /*
    * Gets the interface for the client, or creates a new one.
    * This interface could be a singleton.
    * Variant of API with supplied category
	*
    * @since S60 3.2.3
    */
    IMPORT_C static MMPXPlaybackUtility* UtilityL(
                                        const TMPXCategory aCategory,
                                        const TUid& aModeId = KPbModeDefault );


    /**
    *  Add a Observer. Clients only receive events from the engine corresponding
    *  to the specific mode the interface was created in.
    *
    *  @since S60 3.2.3
    *  @param aObs observer to be added
    */
    virtual void AddObserverL(MMPXPlaybackObserver& aObs)=0;

    /**
    *  Remove a observer.
    *
    *  @since S60 3.2.3
    *  @param aObs observer to be removed
    */
    virtual void RemoveObserverL(MMPXPlaybackObserver& aObs)=0;

    /**
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
    *  From the Array of clients returned, the first one will be the
    *  last active one.
    *
    *  @since S60 3.2.3
    *  @param aClients Array of Pids returned.
    */
    virtual void GetClientsL(RArray<TProcessId>& aClients)=0;

    /**
    *  Initializes a track given by aCollectionPath.
    *
    *  @since S60 3.2.3
    *  @param aCollectionPath collection path
    *  @param aPlay Whether to initialize for playback, or
    *               else just to setup the engine first
    */
    virtual void InitL(const CMPXCollectionPlaylist& aPlaylist,
                       TBool aPlay=ETrue)=0;

    /**
    *  Inititialises with a single item, may not be part of any collection.
    *
    *  @since S60 3.2.3
    *  @param aUri URI of the item
    *  @param aType the mime type of the item
    */
    virtual void InitL(const TDesC& aUri, const TDesC8* aType=NULL)=0;

    /**
    *  Inititialises with a single item, may not be part of any collection
    *  Before this function can be called, the file server session which owns
    *  this file handle must first be marked as shareable by calling
    *  RFs::ShareProtected().
    *
    *  @since S60 3.2.3
    *  @param aShareableFile shareable file handle
    */
    virtual void InitL(RFile& aShareableFile)=0;

    /**
    *  Frees up client side resources only; a player is freed when there are no
    *  clients using it, and all resources are freed when the last client closed.
    *
    *  @since S60 3.2.3
    */
    virtual void Close()=0;

    /**
    *  Stops any async operations that are currently under way.
    *
    *  @since S60 3.2.3
    */
    virtual void CancelRequest()=0;

    /** 
    *  Issue player commands, with optional data. ECommandReceived and
    *  ECommandComplete events should follow.
    *
    *  @since S60 3.2.3
    *  @param aCmd the command
    *  @param aData optional data
    *  @deprecated Use method MMPXPlaybackUtility::CommandL(CMPXCommand& aCmd,
    *             MMPXPlaybackCallback* aCallback=NULL) instead.
    */
    virtual void CommandL(TMPXPlaybackCommand aCmd, TInt aData=0)=0;

    /**
     *  Send a command to the playback server.
     *
     *  @since S60 3.2.3
     *  @param aCmd reference to command object
     *  @param aCallback call back when command exection completed.
     *                    Asynchronously command only
     */
    virtual void CommandL(CMPXCommand& aCmd,
                          MMPXPlaybackCallback* aCallback=NULL)=0;

    /**
    *  Current state of player.
    *
    *  @since S60 3.2.3
    *  @return current state
    */
    virtual TMPXPlaybackState StateL() const=0;

    /**
    *  Current source of playlist being played, NULL if none was supplied.
    *
    *  @since S60 3.2.3
    *  @return source object
    */
    virtual MMPXSource* Source()=0;

    /**
    *  Player manager.
    *
    *  @since S60 3.2.3
    *  @return reference to player manager
    */
    virtual MMPXPlayerManager& PlayerManager()=0;

    /**
    *  Set playback property, EPropertyChanged event when complete.
    *
    *  @since S60 3.2.3
    *  @param aProperty the property to be set
    *  @param aValue the the value of the property
    */
    virtual void SetL(TMPXPlaybackProperty aProperty, TInt aValue)=0;

    /** 
    *  Get value of a playback property, Asyc.
    *
    *  @since S60 3.2.3
    *  @param aCallback playback callback
    *  @param aProperty the property
    *  @deprecated Use method MMPXPlaybackUtility::PropertyL(
    *       MMPXPlaybackCallback& aCallback, 
    *       TMPXPlaybackProperty aProperty) instead.
    */
    virtual void ValueL(MMPXPlaybackCallback& aCallback,
                        TMPXPlaybackProperty aProperty)=0;

    /**
     *  Get value of a playback property, Asyc.
     *  Result will be called back in HandleProperty.
     *
     *  @since S60 3.2.3
     *  @param aCallback playback callback
     *  @param aProperty the property
     */
    virtual void PropertyL(MMPXPlaybackCallback& aCallback,
                           TMPXPlaybackProperty aProperty)=0;


    /**
    *  Return a list of mime types supported by playback framework.
    *
    *  @since S60 3.2.3
    *  @return an array of mime types
    */
    virtual CDesCArray* SupportedMimeTypes()=0;

    /**
    *  Return a list of file extensions supported by playback framework.
    *
    *  @since S60 3.2.3
    *  @return an array of extensions
    */
    virtual CDesCArray* SupportedExtensions()=0;

    /**
    *  Return a list of schemas supported by playback framework.
    *
    *  @since S60 3.2.3
    *  @return an array of schemas
    */
    virtual CDesCArray* SupportedSchemas()=0;

    /**
    *  Sets the priority of the playback utility.
    *
    *  @since S60 3.2.3
    *  @param aPriority Priority to set
    */
    virtual void SetPriority( TInt aPriority )=0;

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
    
    /**
    *  Inititialises for streaming with URI and Access Point
    *
    *  @since S60 9.2
    *  @param aUri URI of the item
    *  @param aType the mime type of the item
    *  @param aAccessPoint the access point of the item
    */
    virtual void InitStreamingL(const TDesC& aUri, const TDesC8* aType, const TInt aAccessPoint)=0;

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
    virtual void InitStreamingL(RFile& aShareableFile, const TInt aAccessPoint)=0;
    
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
    virtual void Init64L(RFile64& /*aShareableFile*/) {};
     
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
    virtual void InitStreaming64L(RFile64& /*aShareableFile*/, const TInt /*aAccessPoint*/) {};
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
    /** 
     *  Sets the primary client 
     * 
     *  @since S60 9.2
     *  @return system error 
     */     
    virtual TInt SetPrimaryClientL() {return KErrNotSupported; };

    
    };

/**
*  Manages the players. Golden rules: by default, all players of type EPbLocal
*  are tried and the first one found which can handle the content is used; when
*  a specific player is selected by uid, (i.e. SelectPlayerL(TUid aPlayer))
*  then only that player will be considered in the search; when players are
*  selected by type (i.e. SelectPlayersL(TMPXPlaybackPlayerType aType)), then only
*  players with that type are considered; When a sub player is explicitly
*  selected (i.e. SelectSubPlayerL()), then only that player and its sub player
*  are considered.
*
*  @lib mpxplaybackutility.lib
*/
class MMPXPlayerManager
    {
public:
    /**
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
    *  @since S60 3.2.3
    *  @param aTypes the list of player types
    */
    virtual void GetPlayerTypesL(RArray<TMPXPlaybackPlayerType>& aTypes)=0;

    /**
    *  Returns display name for custom types; standard types should be
    *  recognized by client and localized - calling this method would then not
    *  be required.
    *
    *  @since S60 3.2.3
    *  @param aType playback type
    */
    virtual HBufC* PlayerTypeDisplayNameL(TMPXPlaybackPlayerType aType)=0;

    /**
    *  Get the list of UIDs of all players.
    *
    *  @since S60 3.2.3
    *  @param aPlayers All the players' uids in the system
    */
    virtual void GetPlayerListL(RArray<TUid>& aPlayers)=0;

    /**
    *  Get the list of UIDs of players with the specific type.
    *
    *  @since S60 3.2.3
    *  @param aPlayers All the players' uids with the same type
    *  @param aType the type of players
    */
    virtual void GetPlayerListL(RArray<TUid>& aPlayers,
                               TMPXPlaybackPlayerType aType)=0;

    /**
    *  Get the list of sub player names, Async.
    *
    *  @since S60 3.2.3
    *  @param aCallback sub players returned in call back interface
    *  @param aPlayer UID of the player
    */
    virtual void SubPlayerNamesL(MMPXPlaybackCallback& aCallback,
                                 TUid aPlayer)=0;

    /**
    *  Client selects all players with the type aType, the specific player used
    *  then being resolved on the basis of the content.
    *
    *  @since S60 3.2.3
    *  @param aType the type of player
    */
    virtual void SelectPlayersL(TMPXPlaybackPlayerType aType)=0;

    /**
    *  Client selects specific player aPlayer and sub player aSubPlayerIndex
    *  from array returned by SubPlayerNamesL.
    *  NOTE: Once a player is manually selected by UID, all subsequent
    *        InitL() will continue to use that player, it will not
    *        try to resolve a plugin.  If the client later wants
    *        the engine to auto resolve the proper plugin, the client must
    *        call ClearSelectPlayersL() to reset the engine.
    *
    *  @since S60 3.2.3
    *  @param aPlayer the UID of player
    *  @param aSubPlayerIndex the index of the sub player
    */
    virtual void SelectSubPlayerL(TUid aPlayer, TInt aSubPlayerIndex)=0;

    /**
    *  Client selects a specific player.
    *  NOTE: Once a player is manually selected by UID, all subsequent
    *        InitL() will continue to use that player, it will not
    *        try to resolve a plugin.  If the client later wants
    *        the engine to auto resolve the proper plugin, the client must
    *        call ClearSelectPlayersL() to reset the engine.
    *
    *  @since S60 3.2.3
    *  @param aPlayer the UID of the player
    */
    virtual void SelectPlayerL(TUid aPlayer)=0;

    /**
    *  Clears all selection criteria. Essentially the same as
    *  SelectPlayersL(EPbLocal).
    *
    *  @since S60 3.2.3
    */
    virtual void ClearSelectPlayersL()=0;

    /**
    *  Retreives the current selection: KNullUid and KErrNotFound are possible
    *  return values for aPlayer and aSubPlayerIndex respectively if none are
    *  explicitly selected.
    *
    *  @since S60 3.2.3
    *  @param aType player type
    *  @param aPlayer the UID of player
    *  @param aSubPlayerIndex index of the subplayer
    *  @param aSubPlayerName friendly name of the subplayer.
    *         Notes: Client owns the returned aSubPlayerName object.
    */
    virtual void GetSelectionL(TMPXPlaybackPlayerType& aType,
                               TUid& aPlayer,
                               TInt& aSubPlayerIndex,
                               HBufC*& aSubPlayerName)=0;

    /**
    *  The current player, may change with each item, NULL if none found.
    *
    *  @since S60 3.2.3
    *  @return object of current player
    */
    virtual MMPXPlayer* CurrentPlayer()=0;
    };


/**
*  Player functionality specific to selected player.
*
*  @lib mpxplaybackutility.lib
*/
class MMPXPlayer
    {
public:
    /**
    *  The 'type' of the player. Built-in types are EPbLocal and EPbRemote, but
    *  the plug-ins could extend this if they desire to be a different type
    *  which can be selected by the user.
    *
    *  @since S60 3.2.3
    *  @return player type
    */
    virtual TMPXPlaybackPlayerType TypeL()=0;

    /**
    *  If TypeL returns a value that is not known to the UI (and therefore
    *  can't localize it),then TypeNameL can be used for unlocalized display
    *  purposes.
    *
    *  @since S60 3.2.3
    *  @return player name.
    */
    virtual HBufC* TypeNameL()=0;

    /**
    *  If the player supports other 'players', else array count == 0.
    *
    *  @since S60 3.2.3
    *  @param aCallback sub players returned in the callback
    */
    virtual void SubPlayerNamesL(MMPXPlaybackCallback& aCallback)=0;

    /**
    *  Selected sub player index, corresponding to sub player names array.
    *
    *  @since S60 3.2.3
    *  @return sub player index. KErrNotFound if none
    */
    virtual TInt SubPlayerL() const=0;

    /**
    *  The UID identifying this player.
    *
    *  @since S60 3.2.3
    *  @return UID of the player
    */
    virtual TUid UidL() const=0;
    };

/**
*  Media functionality. Each media is identified by its index into a container,
*  which is the same as supplied in InitL(const CMPXCollectionPlaylist& aPlaylist).
*
*  @lib mpxplaybackutility.lib
*/
class MMPXSource
    {
public:
    /**
    *  Path to the collection, or NULL if not in any collection.
    *  Ownership transferred.
    *
    *  @since S60 3.2.3
    *  @return collection path
    */
    virtual CMPXCollectionPlaylist* PlaylistL()=0;

    /**
    *  File passed in, or NULL if none passed in.
    *
    *  @since S60 3.2.3
    *  @return pointer to file handle
    */
    virtual RFile* FileL()=0;

    /**
    *  URI of current item.
    *
    *  @since S60 3.2.3
    *  @return URI of the item
    */
    virtual HBufC* UriL()=0;

    /**
    *  
    *  Request for media properties of current playing item
    *  HandleMediaL of the callback.
    *
    *  @since S60 3.2.3
    *  @param aAttrs attributes requested
    *  @param aCallback playback callback
    *  @deprecated Use method MMPXPlaybackUtility::MediaL(
    *                   const TArray<TMPXAttribute>& aAttrs,
    *                   MMPXPlaybackCallback& aCallback,
    *                   CMPXAttributeSpecs* aSpecs) instead.
    */
    virtual void MediaL(const TArray<TMPXAttribute>& aAttrs,
                        MMPXPlaybackCallback& aCallback)=0;

    /**
    *  Request for media properties of current playing item
    *  HandleMediaL of the callback.
    *
    *  @since S60 3.2.3
    *  @param aAttrs attributes requested
    *  @param aCallback playback callback
    *  @param aSpecs specifications for attributes
    */
    virtual void MediaL(const TArray<TMPXAttribute>& aAttrs,
                        MMPXPlaybackCallback& aCallback,
                        CMPXAttributeSpecs* aSpecs)=0;

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  File passed in, or NULL if none passed in.
    *
    *  @since S60 9.2
    *  @return pointer to file handle
    */
    virtual RFile64* File64L() {return NULL;};
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    };

#endif      // MMPXPLAYBACKUTILITY_H

// End of File
