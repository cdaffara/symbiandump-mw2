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
* Description:  Playlist Plugin Handler
*
*  CMPXPlaylistPluginHandler inherits from CMPXPluginHandlerBase which owns a
*  CMPXPluginMonitor that monitors playlist plugins.
*
*  CMPXPlaylistPluginHandler also contains a list of loaded playlist plugins.
*  A playlist plugin is added to the list when it's selected for the first
*  time. When a new plugin is installed, CMPXPluginMonitor notifies this plugin
*  handler through CMPXPluginMonitorObserver interface. However, the new plugin
*  will not be loaded at this time until a client explictly selects this new
*  plugin.
*
*  When a plugin is uninstalled, CMPXPluginMonitor also notifies this plugin
*  handler through CMPXPluginMonitorObserver interface. If such plugin is
*  currently loaded, it will be unloaded and removed from the list. All plugins
*  are removed when CMPXPlaylistPluginHandler is destroyed.
*
*
*/

#include <bamdesca.h>
#include <badesca.h>
#include <uri16.h>
#include <apgcli.h>

#include <mpxlog.h>
#include <mpxuser.h>
#include <mpxpluginmonitor.h>
#include <mpxpluginhandlerobserver.h>
#include "mpxplaylistenginedefs.hrh"
#include "mpxplaylistpluginhandler.h"
#include "mpxplaylistplugin.h"




// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaylistPluginHandler::CMPXPlaylistPluginHandler(
    MMPXPlaylistPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver,
    RFs& aFs,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet)
:   CMPXPluginHandlerBase(TUid::Uid(KMPXPlaylistInterfaceUid),
                                    ESelectionType,
                                    EMPXPlaylistTypeM3U,
                         aObserver),
    iPluginObserver(aPluginObserver),
    iFs(aFs),
    iTopCharacterSet(aTopCharacterSet),
    iAvailableCharacterSet(aAvailableCharacterSet),
    iPluginUid(KNullUid),
    iPluginType(EMPXPlaylistTypeUnknown)
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistPluginHandler::ConstructL()
    {
    BaseConstructL();
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaylistPluginHandler* CMPXPlaylistPluginHandler::NewL(
    MMPXPlaylistPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver,
    RFs& aFs,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet)
    {
    CMPXPlaylistPluginHandler* self = new(ELeave)CMPXPlaylistPluginHandler(
            aPluginObserver, aObserver, aFs, aTopCharacterSet, aAvailableCharacterSet);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaylistPluginHandler::~CMPXPlaylistPluginHandler()
    {
    iLoadedPlugins.ResetAndDestroy();
    iLoadedPluginsUids.Close();
    delete iDisplayName;
    }

// ----------------------------------------------------------------------------
// selects a plug-in appropriate for a Uri and data type
//
// This is a wrapper method for the base class's SelectPluginL to grant client
// access and gives the method name some context.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaylistPluginHandler::SelectPlaylistPluginL(
    const TDesC& aUri,
    const TDesC8& aDataType)
    {
    MPX_DEBUG2("CMPXPlaylistPluginHandler::SelectPlaylistPluginL( URI: %S, DataType )", &aUri);

    ClearSelectionCriteria();
    SelectPluginL( aUri, aDataType );
    }

// ----------------------------------------------------------------------------
// selects a plug-in appropriate for a file
//
// This is a wrapper method for the base class's SelectPluginL to grant client
// access and gives the method name some context.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaylistPluginHandler::SelectPlaylistPluginL(const RFile& aFile)
    {
    MPX_DEBUG1("CMPXPlaylistPluginHandler::SelectPlaylistPluginL(RFile)");

    ClearSelectionCriteria();
    SelectPluginL( aFile );
    }

// ----------------------------------------------------------------------------
// selects a plug-in appropriate for a plugin type
//
// This is a wrapper method for the base class's SelectPluginL to grant client
// access and gives the method name some context.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaylistPluginHandler::SelectPlaylistPluginL(
    TUint32 aPluginType)
    {
    MPX_DEBUG2("CMPXPlaylistPluginHandler::SelectPlaylistPluginL( ParserType: %x )", aPluginType);

    SelectPluginL( aPluginType );
    }

// ----------------------------------------------------------------------------
// selects plug-in with the specified name
//
// This is a wrapper method for the base class's SelectPluginL to grant client
// access and gives the method name some context.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaylistPluginHandler::SelectPlaylistPluginL(
    const TDesC& aPluginName)
    {
    MPX_DEBUG2("CMPXPlaylistPluginHandler::SelectPlaylistPluginL( aPluginName: %S )", &aPluginName);

    SelectPluginL( aPluginName );
    }

// ----------------------------------------------------------------------------
// select a plugin with the specific id
//
// This is a wrapper method for the base class's SelectPluginL to grant client
// access and gives the method name some context.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaylistPluginHandler::SelectPlaylistPluginL(const TUid& aUid)
    {
    MPX_DEBUG2("CMPXPlaylistPluginHandler::SelectPlaylistPluginL(UID: %x)", aUid.iUid);

    SelectPluginL( aUid );
    }

// ----------------------------------------------------------------------------
// creates a plugin with the specific id. if already loaded, returns the reference
// without recreating it
// ----------------------------------------------------------------------------
//
CMPXPlaylistPlugin* CMPXPlaylistPluginHandler::CreatePlaylistPluginL(const TUid& aUid)
    {
    MPX_DEBUG2("CMPXPlaylistPluginHandler::CreatePlaylistPluginL(UID: %x)", aUid.iUid);

    CMPXPlaylistPlugin* p(NULL);
    TInt i=iLoadedPluginsUids.Find(aUid);
    if (i==KErrNotFound)
        {
        p=CMPXPlaylistPlugin::NewL(aUid, iPluginObserver, iFs, iTopCharacterSet, iAvailableCharacterSet);
        CleanupStack::PushL(p);
        iLoadedPlugins.AppendL(p);
        TInt err = iLoadedPluginsUids.Append(aUid);
        if (err != KErrNone) // if the second append failed, it should revert the first append
        	{
        	iLoadedPlugins.Remove(iLoadedPlugins.Count());
        	User::Leave(err);
        	}
        CleanupStack::Pop(p);
        }
     else
        {
        p=iLoadedPlugins[i];
        }
    return p;
    }

// ----------------------------------------------------------------------------
// Resolve a plugin to iPlugin, based on properties (iDataType, iExt and iScheme)
// and selection criteria.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistPluginHandler::ResolvePluginL()
    {
    MPX_DEBUG1("CMPXPlaylistPluginHandler::ResolvePluginL");

    //
    // Resolve plugin
    //
    TUid pluginUid( KNullUid );
    TInt index( KErrNotFound );
    TPtrC displayName;
    TInt pluginType( EMPXPlaylistTypeUnknown );

    DoResolvePluginL(pluginUid, index, displayName, pluginType );

    //
    // Load resolved plug-in
    //
    CMPXPlaylistPlugin* p = CreatePlaylistPluginL(pluginUid);
    UsePlugin(pluginUid);

    if (p != iPlugin)
        {
        iPlugin = p;

        iPluginUid = pluginUid;

        delete iDisplayName;
        iDisplayName = NULL;
        iDisplayName = displayName.AllocL();

        iPluginType = pluginType;
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaylistPluginHandler::IsPluginLoaded
// ----------------------------------------------------------------------------
//
TBool CMPXPlaylistPluginHandler::IsPluginLoaded(
    const TUid& aPluginUid)
    {
    return iLoadedPluginsUids.Find(aPluginUid) != KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CMPXPlaylistPluginHandler::LoadPluginL
// ----------------------------------------------------------------------------
//
void CMPXPlaylistPluginHandler::LoadPluginL(
    const TUid& aPluginUid)
    {
    (void)CreatePlaylistPluginL(aPluginUid);
    }

// ----------------------------------------------------------------------------
// Unloads a specified plugin.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistPluginHandler::UnloadPlugin(
    const TUid& aPluginUid)
    {
    MPX_FUNC("CMPXPlaylistPluginHandler::UnloadPlugin");
    TInt i = iLoadedPluginsUids.Find(aPluginUid);
    if (i != KErrNotFound)
        {
        UnloadPlugin(i);
        }
    }

// ----------------------------------------------------------------------------
// Unloads a plugin by index.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistPluginHandler::UnloadPlugin(
    TInt aIndex)
    {
    MPX_FUNC("CMPXPlaylistPluginHandler::UnloadPlugin");

    CMPXPlaylistPlugin* p = iLoadedPlugins[aIndex];
    if (p == iPlugin)
        {
        iPlugin = NULL;
        iPluginUid = KNullUid;
        iPluginType = EMPXPlaylistTypeUnknown;
        delete iDisplayName;
        iDisplayName = NULL;
        }

    iLoadedPlugins.Remove(aIndex);
    iLoadedPluginsUids.Remove(aIndex);
    delete p;
    }

// End of file
