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
* Description:  playlist plugin handler
*
*/


#ifndef MPXPLAYLISTPLUGINHANDLER_H
#define MPXPLAYLISTPLUGINHANDLER_H

#include <ecom/ecom.h>
#include <mpxpluginhandlerbase.h>
#include "mpxplaylistenginedefs.h"
#include "mpxplaylistplugin.h"

/**
*   CMPXPlaylistPluginHandler is instantiated and owned by CMPXPlaylistEngine.
*   It provides playlist plugin selection APIs for the external client and
*   the internal client (i.e. CMPXPlaylistEngine) and manages loading and
*   unloading of playlist plugins. A plugin is loaded when selected and
*   unloaded when the plugin has been uninstalled or when CMPXPlaylistEngine
*   is destroyed (CMPXPlaylistEngine destroys CMPXPlaylistPluginHandler when
*   itself is destroyed).
*
*   @lib mpxplaylistengine.lib
*/
class CMPXPlaylistPluginHandler : public CMPXPluginHandlerBase
    {
public:

    /**
    *  Two-phased constructor.
    *
    *  @since S60 3.2.3
    *  @param aPluginObserver playlist plugin observer
    *  @param aObserver plugin handler observer
    *  @param aFs file session handle
    *  @param aTopCharacterSet top character set for the locale
    *  @param aAvailableCharacterSet available character set in the system
    *  @return Constructed object
    */
    IMPORT_C static CMPXPlaylistPluginHandler* NewL(
        MMPXPlaylistPluginObserver& aPluginObserver,
        MMPXPluginHandlerObserver& aObserver,
        RFs& aFs,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet);

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    ~CMPXPlaylistPluginHandler();

    /**
    *  Returns currently loaded plug-in UID, or KNullUid if none loaded.
    *
    *  @since S60 3.2.3
    *  @return UID of the plugin
    */
    inline TUid PluginUid() const;

    /**
    *  Returns currently loaded plug-in type or EMPXPlaylistTypeUnknown if
    *  no plugin is loaded.
    *
    *  @since S60 3.2.3
    *  @return plugin type Refer to TMPXPlaylistType for the representation
    *                      of each bit of the return value
    */
    inline TUint32 PluginType() const;

    /**
    *  Returns display name for current plug-in or KNullDesC if no plugin
    *  is loaded.
    *
    *  @since S60 3.2.3
    *  @return plugin name.
    */
    inline const TDesC& PluginName() const;

    /**
    *  Is a plug-in currently loaded.
    *
    *  @since S60 3.2.3
    *  @return ETrue if a plugin is loaded; EFalse otherwise.
    */
    inline TBool PluginFound() const;

    /**
    *  Returns currently loaded plug-in, or NULL if none. CMPXPlaylistPluginHandler
    *  is responsible for the memory management of the return playlist plugin
    *  object.
    *
    *  @since S60 3.2.3
    *  @return plugin object interface
    */
    inline CMPXPlaylistPlugin* Plugin() const;

    /**
    *  Selects a plug-in appropriate for a Uri and data type; may already be loaded.
    *  If the selected plug-in has not been loaded before, one is instantiated.
    *
    *  If successful, information related to the currently loaded plugin will reflect
    *  this selected plug-in. Leaves with KErrNotSupported if unable to find a
    *  suitable plugin.
    *
    *  @since S60 3.2.3
    *  @param aUri URI of the song
    *  @param aDataType data type. KNullDesC8 if this parameter is not to be used
    *                   for selection.
    */
    IMPORT_C void SelectPlaylistPluginL(
                    const TDesC& aUri,
                    const TDesC8& aDataType);

    /**
    *  Selects a plug-in appropriate for a file; may already be loaded. If the
    *  selected plug-in has not been loaded before, one is instantiated.
    *
    *  If successful, information related to currently loaded plugin will reflect
    *  this selected plug-in. Leaves with KErrNotSupported if unable to find a
    *  suitable plugin.
    *
    *  @since S60 3.2.3
    *  @param aFile file handle
    */
    IMPORT_C void SelectPlaylistPluginL(const RFile& aFile);

    /**
    *  Selects a plugin appropriate for the plugin type; may already be loaded.
    *  If the selected plugin has not been loaded before, one is instantiated.
    *
    *  If successful, information related to currently loaded plugin will reflect
    *  this selected plug-in. Leaves with KErrNotSupported if unable to find a
    *  suitable plugin.
    *
    *  This is the recommended method for the client to select a plugin prior
    *  to issuing an ExternalizePlaylist request.
    *
    *  @since S60 3.2.3
    *  @param aPluginType A plugin type, possible values are defined in
    *                    TMPXPlaylistType.
    */
    IMPORT_C void SelectPlaylistPluginL(TUint32 aPluginType);

    /**
    *  Selects a plugin with the specified display name; may already be loaded.
    *  If the selected plugin has not been loaded before, one is instantiated.
    *
    *  If successful, information related to currently loaded plugin will reflect
    *  this selected plug-in. Leaves with KErrNotSupported if unable to find
    *  a suitable plugin.
    *
    *  This is not the recommended method for the client to select a plugin
    *  prior to issuing an ExternalizePlaylist request. The recommended
    *  method is through a plugin type.
    *
    *  @since S60 3.2.3
    *  @param aPluginName The name of the plugin to be selected.
    */
    IMPORT_C void SelectPlaylistPluginL(const TDesC& aPluginName);

    /**
    *  Selects a plug-in with UID aUid, or leaves with KErrNotSupported if not
    *  found. If the selected plugin has not been loaded before, one is
    *  instantiated.
    *
    *  If successful, information related to currently loaded plugin will reflect
    *  this selected plug-in. Leaves with KErrNotSupported if unable to find
    *  the specified plugin.
    *
    *  @since S60 3.2.3
    *  @param aUid UID of the plugin
    */
    IMPORT_C void SelectPlaylistPluginL(const TUid& aUid);

private:

    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    *  @param aPluginObserver playlist plugin observer
    *  @param aObserver plugin handler observer
    *  @param aFs file session handle
    *  @param aTopCharacterSet top character set for the locale
    *  @param aAvailableCharacterSet available character set in the system
    */
    CMPXPlaylistPluginHandler(
        MMPXPlaylistPluginObserver& aPluginObserver,
        MMPXPluginHandlerObserver& aObserver,
        RFs& aFs,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

    /**
    *  Creates a plugin. If already loaded, returns a reference without creating
    *  another instance of the plugin.
    *
    *  @since S60 3.2.3
    *  @return plugin object interface
    *  @leave The function leaves with one of the system error codes,
    */
    CMPXPlaylistPlugin* CreatePlaylistPluginL(const TUid& aUid);

    /**
    *  From CMPXPluginHandlerBase.
    *  Resolves plugin.
    *
    *  @since S60 3.2.3
    */
    void ResolvePluginL();

    /**
    * Verify the specified plugin is loaded.
    *
    * @see CMPXPluginHandlerBase
    *
    * @since S60 3.2.3
    */
    TBool IsPluginLoaded(const TUid& aPluginUid);

    /**
    * Load the specified plugin.
    * @see CMPXPluginHandlerBase
    *
    * @since S60 3.2.3
    */
    void LoadPluginL(const TUid& aPluginUid);

    /**
    * Unload the specified plugin.
    * @see CMPXPluginHandlerBase
    *
    * @since S60 3.2.3
    */
    void UnloadPlugin(const TUid& aPluginUid);

    /**
    * Unloads a plugin by index.
    * @since S60 3.2.3
    * @param aIndex index of the plugin to unload
    *
    */
    void UnloadPlugin(TInt aIndex);

private:

    MMPXPlaylistPluginObserver&         iPluginObserver;
    RFs&                                iFs;
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& iTopCharacterSet;
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& iAvailableCharacterSet;

    //
    // Loaded. The index must correspond in these arrays
    //
    RPointerArray<CMPXPlaylistPlugin>   iLoadedPlugins;
    RArray<TUid>                        iLoadedPluginsUids;

    //
    // Chosen
    //
    CMPXPlaylistPlugin*                 iPlugin;     // ownership belongs to iLoadedPlugins
    TUid                                iPluginUid;
    TUint32                             iPluginType;
    HBufC*                              iDisplayName;

    };

#include "mpxplaylistpluginhandler.inl"

#endif // MPXPLAYLISTPLUGINHANDLER_H
