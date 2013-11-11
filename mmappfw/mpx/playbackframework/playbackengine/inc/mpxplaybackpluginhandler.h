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
* Description:  Plugin handler
*
*/



#ifndef CMPXPLAYBACKPLUGINHANDLER_H
#define CMPXPLAYBACKPLUGINHANDLER_H

#include <mpxplaybackframeworkdefs.h>
#include <mpxpluginhandlerbase.h>

class CMPXPluginMonitor;
class CMPXPlaybackPlugin;
class MMPXPlaybackPluginObserver;


/**
*   Processes plug-ins
*/
class CMPXPlaybackPluginHandler : public CMPXPluginHandlerBase
    {
public:
    /**
    *  Two-phased constructor.
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aPluginObserver Observer
    *  @param aObserver plugin handler observer
    *  @return Constructed object
    */
    static CMPXPlaybackPluginHandler* NewL(CMPXPluginMonitor& aPluginMonitor,
                                           MMPXPlaybackPluginObserver& aPluginObserver,
                                           MMPXPluginHandlerObserver& aObserver);

    /**
    *  Destructor
    */
    ~CMPXPlaybackPluginHandler();

public:
    /**
    * Set a new observer
    *
    * @param aObs observer
    */
    inline void SetObserver(MMPXPlaybackPluginObserver& aObs);

    /**
    *  Returns currently loaded plug-in, or NULL if none
    *
    *  @return plugin object interface
    */
    inline CMPXPlaybackPlugin* Plugin();

    /**
    *  Returns currently loaded plug-in UID, or KNullUId if none loaded
    *
    *  @return UID of the plugin
    */
    inline TUid PluginUid() const;

    /**
    *  Returns currently loaded plug-in type
    *
    *  @return plugin type
    */
    inline TMPXPlaybackPlayerType PluginType() const;

    /**
    *  Returns display name for custom type for current plug-in; standard types
    *  should be recognized by client and localized - calling this method would
    *  then not be required. See MMPXPlayerManager
    *
    *  @return player name
    */
    inline const TDesC& PlayerName() const;

    /**
    *  Is a plug-in currently loaded
    *
    *  @return ETrue if a play-in is loaded
    */
    inline TBool PlayerFound() const;

    /**
    *  Currently selected sub player. or KErrNotFound if none
    *
    *  @return index of subplayer
    */
    inline TInt SubPlayer() const;

    /**
    *  Returns plug-in with UID aUid, or leaves with KErrNotFound if not there.
    *  Plug-in loaded until Unload called
    *
    *  @param UID of the plugin
    *  @return plugin object interface
    */
    CMPXPlaybackPlugin* CreatePlayerPluginL(const TUid& aUid);

    /**
    * @see CMPXPluginHandlerBase
    */
    TBool IsPluginLoaded(const TUid& aPluginUid);

    /**
    * @see CMPXPluginHandlerBase
    */
    void LoadPluginL(const TUid& aPluginUid);

    /**
    * @see CMPXPluginHandlerBase
    */
    void UnloadPlugin(const TUid& aPluginUid);

    /**
    *  Returns display name for custom types; standard types should be
    *  recognized by client and localized - calling this method would then not
    *  be required. See MMPXPlayerManager
    *
    *  @param aType player type
    *  @return player name
    */
    IMPORT_C const TDesC& PlayerName(TMPXPlaybackPlayerType aType) const;


    /**
    *  All the UIDs of the plug-ins in the system of a specific type.
    *  See MMPXPlayerManager
    *
    *  @param aPlayers array of UIDs of all players of type aType
    *  @param aType player type
    */
    IMPORT_C void GetPlayerListL(RArray<TUid>& aPlayers,
                                TMPXPlaybackPlayerType aType);

    /**
    *  Only plug-ins of a specific type will be considered for selection.
    *  See MMPXPlayerManager
    *
    *  @param aType player type
    */
    IMPORT_C void SelectPlayersL(TMPXPlaybackPlayerType aType);

    /**
    *  selects plug-in appropriate for a Uri; may already be loaded or not
    *  @param aUri URI of the song
    *  @param aDataType data type
    */
    void SelectPlayerL(const TDesC& aUri,const TDesC8& aDataType);

    /**
    *  selects plug-in appropriate for a file; may already be loaded or not
    *
    *  @param aFile file handle
    */
    void SelectPlayerL(RFile& aFile);

    /**
    *  Only this plug-in will be used, if possible. See MMPXPlayerManager
    *
    *  @param aPlayerUid UID of players
    */
    IMPORT_C void SelectPlayerL(TUid aPlayerUid);

    /**
    *  Only this plug-in will be used, and only this sub player, if possible.
    *  See MMPXPlayerManager
    *
    *  @param aPlayerUid UID of players
    *  @param aSubPlayerIndex index to the subplayer
    */
    IMPORT_C void SelectSubPlayerL(TUid aPlayerUid,TInt aSubPlayerIndex);

    /**
    *  Back to default slection criteria, i.e. type=EPbLocal, no sub player
    *  selected
    */
    IMPORT_C void ClearSelectPlayersL();

    /**
    *  Retreives the current selection: KNullUid and KErrNotFound are possible
    *  return values for aPlayer and aSubPlayerIndex respectively if none are
    *  explicitly selected
    *
    *  @param aType player type
    *  @param aPlayer UID
    *  @param aSubPlayerIndex index of sub player
    *  @param aSubPlayerName Friendly name of the sub player
    */
    IMPORT_C void GetSelection(TMPXPlaybackPlayerType& aType,
                               TUid& aPlayer,
                               TInt& aSubPlayerIndex,
                               TPtrC& aSubPlayerName);

    /**
    *  Return a list of interfaces supported by plugins.
    *
    *  @return an array of interfaces. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedInterfacesL();
    
    /**
    *  Return a list of interfaces supported by a plugin.
    *
    *  @param aUid plugin id
    *  @return an array of interfaces. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedInterfacesL(const TUid& aUid);    
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    *  selects plug-in appropriate for a file; may already be loaded or not
    *
    *  @param aFile file handle
    */
    void SelectPlayer64L(RFile64& aFile);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
protected:
    
    /**
    * From Base CMPXPluginHandlerBase
    * Constructs the collection plugin info
    */
    CMPXPluginInfo* ConstructPluginInfoLC( const CImplementationInformation& aData  );
    
private:

    /**
    *  2nd phase contructor
    */
    void ConstructL();

    /**
    *  C++ constructor
    *
    *  @param aPluginMonitor plugin monitor
    *  @param aPluginObserver plugin observer
    *  @param aObserver handler observer
    */
    CMPXPlaybackPluginHandler(CMPXPluginMonitor& aPluginMonitor,
                              MMPXPlaybackPluginObserver& aPluginObserver,
                              MMPXPluginHandlerObserver& aObserver);

private:

    /**
    * From Base CMPXPluginHandlerBase
    *  Resolves a plugin
    */
    void ResolvePluginL();

    /**
    *  Unloads the plugin at a specified index.
    *  @param aIndex index of the plugin in iLoadedPlugins
    */
    void UnloadPlugin(TInt aIndex);

private:
    MMPXPlaybackPluginObserver* iPluginObserver;  // Not owned.
    //
    // Loaded. The index must correspond in these arrays
    //
    RPointerArray<CMPXPlaybackPlugin> iLoadedPlugins;
    RArray<TUid> iLoadedPluginsUids;

    // Chosen
    //
    CMPXPlaybackPlugin* iPlugin;
    TUid iPluginUid;
    TMPXPlaybackPlayerType iPluginType;
    HBufC* iDisplayName;
    //
    // Desired
    //
    TInt iSelectedSubPlayerIndex;
    };

#include "mpxplaybackpluginhandler.inl"

#endif // CMPXPLAYBACKPLUGINHANDLER_H
