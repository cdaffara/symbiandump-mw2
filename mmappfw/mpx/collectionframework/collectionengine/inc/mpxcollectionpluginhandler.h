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


#ifndef CMPXCOLLECTIONPLUGINHANDLER_H
#define CMPXCOLLECTIONPLUGINHANDLER_H

#include <mpxpluginhandlerbase.h>
#include <mpxcollectionframeworkdefs.h>

class CMPXCollectionPlugin;
class MMPXCollectionPluginObserver;

/**
*   Processes plug-ins
*/
NONSHARABLE_CLASS(CMPXCollectionPluginHandler) : public CMPXPluginHandlerBase
    {
public:
    /**
    *  Two-phased constructor.
    *
    *  @param aPluginObserver plugin observer
    *  @param aObserver plugin handler observer
    *  @return Constructed object
    */
    static CMPXCollectionPluginHandler* NewL(MMPXCollectionPluginObserver& aPluginObserver,
        MMPXPluginHandlerObserver& aObserver);

    /**
    *  Destructor
    */
    ~CMPXCollectionPluginHandler();

public:
    /**
    *  Returns currently loaded plug-in, or NULL if none
    *
    *  @return plugin object interface
    */
    inline CMPXCollectionPlugin* Plugin();

    /**
    * Get all currently loaded plugins
    * @return array of loaded plugins
    */
    TArray<CMPXCollectionPlugin*> LoadedPlugins();

   /**
    *  Remove task for one client which is the observer of tasks
    *  @aCallback the observer of the tasks to be removed
    */
    void RemoveTask(TAny* aCallback);

    /**
    * Retrieve the list of non cachable attributes for a plugin
    * @param aPlugin UID of the plugin
    */
    const TArray<TUid> NonCacheableAttributesL( const TUid& aPluginId );

    /**
    * Check if a plugin is cachable in the collection cache
    * @param aPlugin
    */
    TBool PluginCachable( const TUid& aPluginId );

    /**
    *  Returns the loaded plugin by implementation uid.
    *
    *  @param aUid implementation uid of the plugin
    *  @return plugin pointer if plugin already loaded or NULL if not loaded
    */
    CMPXCollectionPlugin* LoadedPlugin(const TUid& aUid);

private:

    /**
    * From Base CMPXPluginHandlerBase
    *  Resolves a plugin
    */
    void ResolvePluginL();

    /**
    * From Base CMPXPluginHandlerBase
    * Constructs the collection plugin info
    */
    CMPXPluginInfo* ConstructPluginInfoLC( const CImplementationInformation& aData  );

    /**
    * @see CMPXPluginHandlerBase
    */
    TBool IsPluginLoaded(const TUid& aPluginUid);

    /**
    * @see CMPXPluginHandlerBase
    */
    void LoadPluginL(const TUid& aPluginUid);


    /**
    * From Base CMPXPluginHandlerBase
    *  Unloads a plugin
    *  @param aUid identifies the plugin
    */
    void UnloadPlugin(const TUid& aUid);


    /**
    *  Create the plugin object given UID
    *
    *  @param UID of the plugin
    *  @return plugin object
    */
    CMPXCollectionPlugin* CreateCollectionPluginL(const TUid& aUid);

    /**
    *  Returns the loaded plugin index corresponding to the UID.
    *
    *  @param aUid implementation uid of the plugin
    *  @return plugin index in iLoadedPlugins or KErrNotFound
    */
    TInt LoadedPluginIndex(const TUid& aUid);

    /**
    *  Unloads the plugin at a specified index.
    *  @param aIndex index of the plugin in iLoadedPlugins
    */
    void UnloadPlugin(TInt aIndex);

private:

    /**
    *  2nd phase contructor
    */
    void ConstructL();

    /**
    *  C++ constructor
    *
    *  @param aPluginObserver plugin observer
    *  @param aObserver plugin handler observer
    */
    CMPXCollectionPluginHandler(MMPXCollectionPluginObserver& aPluginObserver,
        MMPXPluginHandlerObserver& aObserver);

private:
    MMPXCollectionPluginObserver* iPluginObserver; //Not Owned
    RPointerArray<CMPXCollectionPlugin> iLoadedPlugins;

    // Most recently Selected plugin
    //
    CMPXCollectionPlugin* iPlugin;
    };

#include "mpxcollectionpluginhandler.inl"

#endif // CMPXCOLLECTIONPLUGINHANDLER_H
