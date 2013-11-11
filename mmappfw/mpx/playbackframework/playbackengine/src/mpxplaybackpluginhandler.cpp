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
* Description:  Implements the playback plugin handler.
*
*  There is just one plaback plugin loaded for each plugin handler / engine and
*  whenever a new plugin is selected for the engine the old one is automatically
*  unloaded. Different engines can load multiple instances of the same playback
*  plugin, however there is no connection between these instances as they
*  are tied to different songs.
*
*  There is no point in applying the reference counting mechanism for playback.
*
*
*/


#include <mpxplaybackplugin.h>
#include <mpxplaybackpluginobserver.h>
#include <mpxpluginmonitor.h>
#include <mpxlog.h>
#include "mpxplaybackplugininfo.h"
#include "mpxplaybackpluginhandler.h"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackPluginHandler* CMPXPlaybackPluginHandler::NewL(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver)
    {
    CMPXPlaybackPluginHandler* p=new(ELeave)CMPXPlaybackPluginHandler(
                                                                aPluginMonitor,
                                                                aPluginObserver,
                                                                aObserver);
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackPluginHandler::CMPXPlaybackPluginHandler(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver)
:   CMPXPluginHandlerBase(KMPXPlaybackInterfaceUid,
                          ESelectionType,
                          EPbLocal,
                          aObserver,
                          &aPluginMonitor),
    iPluginObserver(&aPluginObserver),
    iSelectedSubPlayerIndex(KErrNotFound)
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::ConstructL()
    {
    BaseConstructL();
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackPluginHandler::~CMPXPlaybackPluginHandler()
    {
    MPX_FUNC_EX("CMPXPlaybackPluginHandler::Destructor");
    //close plugins
    for (TInt i = iLoadedPlugins.Count(); --i >= 0;)
        {
        CMPXPlaybackPlugin* p=iLoadedPlugins[i];
        TRAP_IGNORE(p->CommandL(EPbCmdClose));
        }
    iLoadedPlugins.ResetAndDestroy();
    iLoadedPluginsUids.Close();
    delete iDisplayName;
    }

// ----------------------------------------------------------------------------
// Resolve a plugin to iPlugin, based on properties (iDataType, iExt and iScheme)
// and selection criteria. If selection is by type, then there is always a
// plug-in resolved (if there are any of that type). Room for optimisation.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::ResolvePluginL()
    {
    MPX_FUNC_EX("CMPXPlaybackPluginHandler::ResolvePluginL()");
    //
    // Resolve plugin
    //
    TUid pluginUid( KNullUid );
    TInt index( KErrNotFound );
    TPtrC displayName;
    TInt pluginType( EPbUnknown );

    DoResolvePluginL(pluginUid, index, displayName, pluginType );

    //
    // Load resolved plug-in
    //
    CMPXPlaybackPlugin* p=CreatePlayerPluginL(pluginUid);
    UsePlugin(pluginUid);

    MPX_DEBUG5("CMPXPlaybackPluginHandler::ResolvePluginL() p(%08x), iPlugin(%08x), pluginUid 0x%08x %S",
                p, iPlugin, pluginUid.iUid, &displayName);
    if (p!=iPlugin)
        {
        if (iPlugin)
            {
            UnloadPlugin(iPluginUid);
            }
        iPlugin=p;
        iPluginUid=pluginUid;

        delete iDisplayName;
        iDisplayName=NULL;
        iDisplayName=displayName.AllocL();
        iPluginType=static_cast<TMPXPlaybackPlayerType>(pluginType);
        }

    if (iSelectedSubPlayerIndex!=KErrNotFound)
        {
        iPlugin->SelectSubPlayerL(iSelectedSubPlayerIndex);
        }
    }

// ----------------------------------------------------------------------------
// Return a plugin with the specific id
// ----------------------------------------------------------------------------
//
CMPXPlaybackPlugin* CMPXPlaybackPluginHandler::CreatePlayerPluginL(
    const TUid& aUid)
    {
    MPX_DEBUG3("==>CMPXPlaybackPluginHandler::CreatePlayerPluginL 0x%08x (aUid 0x%08x)",
               this, aUid.iUid);
    CMPXPlaybackPlugin* p=NULL;
    TInt i=iLoadedPluginsUids.Find(aUid);
    if (i==KErrNotFound)
        {
        p=CMPXPlaybackPlugin::NewL(aUid,*iPluginObserver);
        CleanupStack::PushL(p);
        iLoadedPlugins.AppendL(p);
        TInt err = iLoadedPluginsUids.Append(aUid);
        if (err != KErrNone) // if the second append failed, it should revert the first append
             {
             iLoadedPlugins.Remove(iLoadedPlugins.Count()-1);
             User::Leave(err);
             }
        CleanupStack::Pop(p);
        }
     else
        {
        p=iLoadedPlugins[i];
        }
    MPX_DEBUG3("<==CMPXPlaybackPluginHandler::CreatePlayerPluginL 0x%08x (aUid 0x%08x)",
               this, aUid.iUid);
    return p;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackPluginHandler::IsPluginLoaded
// ----------------------------------------------------------------------------
//
TBool CMPXPlaybackPluginHandler::IsPluginLoaded(
    const TUid& aPluginUid)
    {
    MPX_FUNC_EX("CMPXPlaybackPluginHandler::IsPluginLoaded");
    return (iLoadedPluginsUids.Find(aPluginUid) != KErrNotFound);
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackPluginHandler::LoadPluginL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::LoadPluginL(
    const TUid& aPluginUid)
    {
    MPX_DEBUG2("===>CMPXPlaybackPluginHandler::LoadPluginL 0x%08x", this);

    if (iPlugin)
        {
        UnloadPlugin(iPluginUid);
        }

    iPlugin = CreatePlayerPluginL(aPluginUid);
    iPluginUid = aPluginUid;

    delete iDisplayName;
    iDisplayName = NULL;
    iDisplayName = PluginName(aPluginUid).AllocL();
    MPX_DEBUG3("<===CMPXPlaybackPluginHandler::LoadPluginL 0x%08x plugin name %S",
               this, iDisplayName);
    }

// ----------------------------------------------------------------------------
// Constructs the collection plugin info class
// ----------------------------------------------------------------------------
//
CMPXPluginInfo* 
CMPXPlaybackPluginHandler::ConstructPluginInfoLC(
    const CImplementationInformation& aData  )
    {
    return CMPXPlaybackPluginInfo::NewLC( aData );
    }

// ----------------------------------------------------------------------------
// Unload the plugin
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::UnloadPlugin(
    const TUid& aUid)
    {
    MPX_FUNC_EX("CMPXPlaybackPluginHandler::UnloadPlugin");

    TInt index(iLoadedPluginsUids.Find(aUid));
    if (index != KErrNotFound)
        {
        UnloadPlugin(index);
        }
    }

// ----------------------------------------------------------------------------
// Return player name
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMPXPlaybackPluginHandler::PlayerName(
    TMPXPlaybackPlayerType aType) const
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::PlayerName()");
    return PluginName(aType);
    }

// ----------------------------------------------------------------------------
// Get player list with the specific type
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::GetPlayerListL(
    RArray<TUid>& aPlayers,
    TMPXPlaybackPlayerType aType)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::GetPlayerListL()");
    GetPluginUidsL(aPlayers, aType);
    }

// ----------------------------------------------------------------------------
// Select players with the specific type
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::SelectPlayersL(
    TMPXPlaybackPlayerType aType)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayersL(TMPXPlaybackPlayerType aType)");
    iSelectedSubPlayerIndex=KErrNotFound;
    SelectPluginL(aType);
    }

// ----------------------------------------------------------------------------
// Returns plug-in appropriate for a Uri and data type
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::SelectPlayerL(
    const TDesC& aUri,
    const TDesC8& aDataType)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayerL(const TDesC& aUri)");
    MPX_DEBUG2("CMPXPlaybackPluginHandler::SelectPlayerL(aUri %S)", &aUri);
    SelectPluginL( aUri, aDataType );
    }

// ----------------------------------------------------------------------------
// Returns plug-in appropriate for a file; may already be loaded or not
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::SelectPlayerL(RFile& aFile)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayerL(RFile& aFile)");
    SelectPluginL( aFile );
    }

// ----------------------------------------------------------------------------
// Select players with the specific UID
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::SelectPlayerL(TUid aPlayerUid)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayerL(TUid aPlayerUid)");
    SelectSubPlayerL(aPlayerUid,KErrNotFound);
    iSelectedType = iPluginType;
    }

// ----------------------------------------------------------------------------
// Select subplayer
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::SelectSubPlayerL(
    TUid aPlayerUid,
    TInt aSubPlayerIndex)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayerL(TUid aPlayerUid, TInt aSubPlayerIndex)");
    MPX_DEBUG3("CMPXPlaybackPluginHandler::SelectPlayerL(0x%08x, %d)", aPlayerUid.iUid, aSubPlayerIndex);
    iSelectedSubPlayerIndex=aSubPlayerIndex;
    SelectPluginL(aPlayerUid);
    }

// ----------------------------------------------------------------------------
// Clear selection
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::ClearSelectPlayersL()
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::ClearSelectPlayersL()");
    SelectPlayersL(EPbLocal);
    }

// ----------------------------------------------------------------------------
// Get current selection
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackPluginHandler::GetSelection(
    TMPXPlaybackPlayerType& aType,
    TUid& aPlayer,
    TInt& aSubPlayerIndex,
    TPtrC& aSubPlayerName)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::GetSelectionL()");
    aType=iPluginType;
    aPlayer=iSelectedUid;
    aSubPlayerIndex=iSelectedSubPlayerIndex;
    if ( iPlugin )
        {
        aSubPlayerName.Set(iPlugin->SubPlayerName());
        }
    else
        {
        aSubPlayerName.Set(KNullDesC);
        }
    }

// ----------------------------------------------------------------------------
// Return a list of interfaces supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPlaybackPluginHandler::SupportedInterfacesL()
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        CMPXPlaybackPluginInfo* pi = 
            static_cast<CMPXPlaybackPluginInfo*>(iPluginInfoArray[i]);
        MPXUser::MergeArray(pi->SupportedInterfaces(), *descArray);
        }
    return descArray;
    }

// ----------------------------------------------------------------------------
// Return a list of interfaces supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPlaybackPluginHandler::SupportedInterfacesL(const TUid& aUid)
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    CleanupStack::PushL(descArray);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        if (aUid == iPluginInfoArray[i]->ImplementationUid())
            {
            CMPXPlaybackPluginInfo* pi = 
                static_cast<CMPXPlaybackPluginInfo*>(iPluginInfoArray[i]);            
            MPXUser::CopyArrayL(pi->SupportedInterfaces(), *descArray);
            break;
            }
        }
    CleanupStack::Pop(descArray);
    return descArray;
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackPluginHandler::UnloadPlugin
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::UnloadPlugin(
    TInt aIndex)
    {
    CMPXPlaybackPlugin* p = iLoadedPlugins[aIndex];
    TRAP_IGNORE(p->CommandL(EPbCmdClose));
    iLoadedPlugins.Remove(aIndex);
    iLoadedPluginsUids.Remove(aIndex);
    if (p == iPlugin)
        {
        iPlugin = NULL;
        }
    delete p;
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
// ----------------------------------------------------------------------------
// Returns plug-in appropriate for a file; may already be loaded or not
// ----------------------------------------------------------------------------
//
void CMPXPlaybackPluginHandler::SelectPlayer64L(RFile64& aFile)
    {
    MPX_FUNC("CMPXPlaybackPluginHandler::SelectPlayer64L(RFile64& aFile)");
    SelectPlugin64L( aFile );
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

// end of file
