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
* Description:  Collection plug-in handler
*
*/

#include <mpxlog.h>
#include <mpxcollectionplugin.h>
#include <mpxcollectionplugin.hrh>
#include "mpxcollectionpluginhandler.h"
#include "mpxcollectionplugininfo.h"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionPluginHandler* CMPXCollectionPluginHandler::NewL(
    MMPXCollectionPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver)
    {
    CMPXCollectionPluginHandler* p=new(ELeave)CMPXCollectionPluginHandler(
        aPluginObserver, aObserver);
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionPluginHandler::CMPXCollectionPluginHandler(
    MMPXCollectionPluginObserver& aPluginObserver,
    MMPXPluginHandlerObserver& aObserver)
:   CMPXPluginHandlerBase(KMPXCollectionInterfaceUid, ESelectionType, EMcLocal, aObserver),
    iPluginObserver(&aPluginObserver)
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::ConstructL()
    {
    BaseConstructL();
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionPluginHandler::~CMPXCollectionPluginHandler()
    {
    //close plugins
    for (TInt i = iLoadedPlugins.Count(); --i >= 0;)
        {
        CMPXCollectionPlugin* p=iLoadedPlugins[i];
        TRAP_IGNORE( p->CommandL(EMcCmdClose) );
        }
    iLoadedPlugins.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// Return the list of loaded plugins
// ----------------------------------------------------------------------------
//
TArray<CMPXCollectionPlugin*> CMPXCollectionPluginHandler::LoadedPlugins()
    {
    return iLoadedPlugins.Array();
    }

// ----------------------------------------------------------------------------
// Remove tasks of a client which is the observer of tasks
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::RemoveTask(TAny* aCallback)
    {
    for (TInt i=iLoadedPlugins.Count(); --i>=0; )
        {
        CMPXCollectionPlugin* plugin(iLoadedPlugins[i]);
        if (plugin->Callback()==aCallback)
            {
            MPX_DEBUG1("CMPXCollectoinPluginHandler::RemoveTask removes active task");
            plugin->CancelRequest();
            plugin->CompleteTask();
            plugin->SetCallback(NULL);
            plugin->SetObserver(*iPluginObserver);
            }
        else
            {
            iLoadedPlugins[i]->RemoveTask(aCallback);
            }
        }
    }

// ----------------------------------------------------------------------------
// Get the list of noncacheable attributes
// ----------------------------------------------------------------------------
//
const TArray<TUid> CMPXCollectionPluginHandler::NonCacheableAttributesL(
                                                        const TUid& aPluginId )
    {
    TInt i(KErrNotFound);
    TInt count( iPluginInfoArray.Count() );
    TBool found( EFalse );
    for( i = 0; !found && i < count; ++i )
        {
        if ( iPluginInfoArray[i]->ImplementationUid() == aPluginId )
            {
            found = ETrue;
            break;
            }
        }
    if ( !found )
        {
        User::Leave( KErrNotFound );
        }

    return ((CMPXCollectionPluginInfo*)iPluginInfoArray[i])->NonCacheableAttrs();
    }

// ----------------------------------------------------------------------------
// Get whether the plugin is cacheable or not
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionPluginHandler::PluginCachable( const TUid& aPluginId )
    {
    TUint flags(0);

    TInt count( iPluginInfoArray.Count() );
    for( TInt i = 0; i < count; ++i )
        {
        if ( iPluginInfoArray[i]->ImplementationUid() == aPluginId )
            {
            flags = iPluginInfoArray[i]->Flags();
            break;
            }
        }
    return ((flags&EMPXCollectionPluginFlagCacheable) != 0);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::LoadedPlugin
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionPluginHandler::LoadedPlugin(const TUid& aUid)
    {
    CMPXCollectionPlugin* plugin(NULL);
    TInt index(LoadedPluginIndex(aUid));
    if (KErrNotFound != index)
        {
        plugin = iLoadedPlugins[index];
        }
    return plugin;
    }

// ----------------------------------------------------------------------------
// Resolve a plugin to iPlugin, based on properties (iDataType, iExt and iScheme)
// and selection criteria. If selection is by type, then there is always a
// plug-in resolved (if there are any of that type). Room for optimisation!
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::ResolvePluginL()
    {
    //
    // Resolve plugin
    //
    TUid pluginUid( KNullUid );
    TInt index( KErrNotFound );
    TPtrC displayName;
    TInt pluginType( EMcUnknown );

    DoResolvePluginL(pluginUid, index, displayName, pluginType );

    iPlugin = CreateCollectionPluginL(pluginUid);
    if(iPlugin != NULL)
    UsePlugin(pluginUid);
    }
// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::IsPluginLoaded
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionPluginHandler::IsPluginLoaded(
    const TUid& aPluginUid)
    {
    return LoadedPluginIndex(aPluginUid) != KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::LoadPluginL
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::LoadPluginL(
    const TUid& aPluginUid)
    {
    (void)CreateCollectionPluginL(aPluginUid);
    }

// ----------------------------------------------------------------------------
// Constructs the collection plugin info class
// ----------------------------------------------------------------------------
//
CMPXPluginInfo* CMPXCollectionPluginHandler::ConstructPluginInfoLC(
                                     const CImplementationInformation& aData  )
    {
    return CMPXCollectionPluginInfo::NewLC( aData );
    }

// ----------------------------------------------------------------------------
// Unloads a specified plugin.
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::UnloadPlugin(
    const TUid& aUid)
    {
    MPX_DEBUG3("===>CMPXCollectionPluginHandler::UnloadPlugin 0x%08x uid 0x%08x",
               this, aUid.iUid);
    TInt index(LoadedPluginIndex(aUid));
    if (index != KErrNotFound)
        {
        UnloadPlugin(index);
        }
    MPX_DEBUG3("<===CMPXCollectionPluginHandler::UnloadPlugin 0x%08x uid 0x%08x",
               this, aUid.iUid);
    }

// ----------------------------------------------------------------------------
// Return a plugin controller with the specific id
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin*
CMPXCollectionPluginHandler::CreateCollectionPluginL(const TUid& aUid)
    {
    MPX_DEBUG3("===>CMPXCollectionPluginHandler::CreateCollectionPluginL 0x%08x uid 0x%08x",
               this, aUid.iUid);

    CMPXCollectionPlugin* p=NULL;

    TInt index(LoadedPluginIndex(aUid));
    if (index == KErrNotFound)
        {
        MPX_DEBUG3("CMPXCollectionPluginHandler::CreateCollectionPluginL create a new plugin 0x%08x uid 0x%08x",
                   this, aUid.iUid);
        TRAPD(err, p = CMPXCollectionPlugin::NewL(aUid, *iPluginObserver));
        if(err == KErrNone)
            {
        CleanupStack::PushL(p);
        iLoadedPlugins.AppendL(p);
        CleanupStack::Pop(p);
            }
        else
            {
            MPX_DEBUG3("CMPXCollectionPluginHandler::CreateCollectionPluginL error creating plugin %d uid 0x%08x",
                               err, aUid.iUid);
            User::Leave(err);
            }
        }
     else
        {
        p = iLoadedPlugins[index];
        }

    MPX_DEBUG3("<===CMPXCollectionPluginHandler::CreateCollectionPluginL 0x%08x uid 0x%08x",
               this, aUid.iUid);

    return p;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::LoadedPluginIndex
// ----------------------------------------------------------------------------
//
TInt CMPXCollectionPluginHandler::LoadedPluginIndex(const TUid& aUid)
    {
    TInt index(KErrNotFound);

    for (TInt i = iLoadedPlugins.Count(); --i >= 0; )
        {
        if (iLoadedPlugins[i]->Uid() == aUid)
            {
            index = i;
            break;
            }
        }

    return index;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::UnloadPlugin
// ----------------------------------------------------------------------------
//
void CMPXCollectionPluginHandler::UnloadPlugin(TInt aIndex)
    {
    MPX_DEBUG3("===>CMPXCollectionPluginHandler::UnloadPlugin 0x%08x index 0x%08x",
               this, aIndex);
    CMPXCollectionPlugin* p = iLoadedPlugins[aIndex];
    TRAP_IGNORE(p->CommandL(EMcCmdClose));
    iLoadedPlugins.Remove(aIndex);
    delete p;
    MPX_DEBUG3("<===CMPXCollectionPluginHandler::UnloadPlugin 0x%08x index 0x%08x",
               this, aIndex);
    }

// End of file
