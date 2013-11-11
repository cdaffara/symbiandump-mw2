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
* Description:  Base plugin handler
*
*/


#ifndef CMPXPLUGINHANDLERBASE_H
#define CMPXPLUGINHANDLERBASE_H

#include <ecom/ecom.h>
#include <badesca.h>
#include "mpxpluginmonitorobserver.h"

class CMPXPluginMonitor;
class CMPXPluginInfo;
class MMPXPluginHandlerObserver;

/**
*  CMPXPluginHandlerBase instantiates and owns a CMPXPluginMonitor to monitor
*  plugins for the designated plugin interface. It also provides plugin
*  selection APIs and a common method for resolving a plugin from a set of
*  selection criteria.
*
*  @lib mpxcommon.lib
*/
class CMPXPluginHandlerBase : public CBase,
                              public MMPXPluginMonitorObserver
    {
public:

    enum TPluginSelection
        {
        ESelectionType, // select a plugin by type
        ESelectionUid  // select a plugin by uid
        };

 public:

    /**
    * Constructor.
    *
    * @since S60 3.2.3
    * @param aInterfaceUid The UID of the ECOM plugin interface this plugin
    *                      handler is designated for
    * @param aSelection plugin selection choices as defined in TPluginSelection
    * @param aSelectionType specifies the type of plugin for selection
    * @param aObserver plugin handler observer instance
    * @param aPluginMonitor plugin monitor. It is for the clients to share the
    *                       same plugin monitor object.
    */
    IMPORT_C CMPXPluginHandlerBase(
                    TUid aInterfaceUid,
                    TPluginSelection aSelection,
                    TInt aSelectionType,
                    MMPXPluginHandlerObserver& aObserver,
                    CMPXPluginMonitor* aPluginMonitor=NULL);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C ~CMPXPluginHandlerBase();

protected:
    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void BaseConstructL();

public:

    /**
    * Use this particular plugin.
    *
    * @since S60 3.2.3
    * @param aPluginUid UID of the plugin to be used
    */
    IMPORT_C  void SelectPluginL(const TUid& aPluginUid);

    /**
    * Use the plugin with the specified display name.
    *
    * @since S60 3.2.3
    * @param aPluginName name of the plugin
    */
    IMPORT_C  void SelectPluginL(const TDesC& aPluginName);

    /**
    * Select a plugin based on the URI and data type of the media given.
    *
    * @since S60 3.2.3
    * @param aUri URI of the media
    * @param aDataType Mime type of the media. KNullDesC8 if not to be
    *        used for selection.
    * @leave KErrArgument Invalid URI
    */
    IMPORT_C virtual void SelectPluginL(
                    const TDesC& aUri,
                    const TDesC8& aDataType);

    /**
    * Select a plugin based on file handle for the given media.
    *
    * @since S60 3.2.3
    * @param aFile a file handle to the media
    */
    IMPORT_C virtual void SelectPluginL(const RFile& aFile);

    /**
    * Select a plugin based on the plugin type.
    *
    * @since S60 3.2.3
    * @param aPluginType plugin type for the appropriate plugin to be used.
    *        The representation for each bit is to be defined by each plugin
    *        interface.
    */
    IMPORT_C virtual void SelectPluginL(TInt aPluginType);

    /**
    * Indicates that this plugin is used by a client. Increments the reference
    * count for the plugin.
    *
    * @since S60 3.2.3
    * @param aPluginUid plugin UID
    */
    IMPORT_C void UsePlugin(const TUid& aPluginUid);

    /**
    * Indicates that this plugin is no longer used by a client. Plugins are
    * reference counted and this call may result in the plugin being unloaded.
    *
    * @since S60 3.2.3
    * @param aPluginUid plugin UID
    * @return ETrue if the plugin is unloaded / EFalse if it is not.
    */
    IMPORT_C TBool ReleasePlugin(const TUid& aPluginUid);

    /**
    *  Retrieves the current selection criteria.
    *
    *  @since S60 3.2.3
    *  @param aPluginSelection OUT parameter. the selection choice as defined
    *         by TPluginSelection
    *  @param aPluginType OUT parameter. plugin type to be used for selection
    *  @param aPluginUid OUT parameter. UID of the plugin to be used for selection.
    *         KNullUid is a possible return value if none is explicitly selected.
    *  @param aDataType OUT parameter. The data type to be used for selection.
    *         KNullDesC8 if none is specified. CMPXPluginHandlerBase is
    *         responsible for the memory management of this parameter.
    *  @param aScheme OUT parameter. The scheme to be used for selection.
    *         KNullDesC8 if none is specified. CMPXPluginHandlerBase is
    *         responsible for the memory management of this parameter.
    *  @param aExtension OUT parameter. The extension to be used for selection.
    *         KNullDesC8 if none is specified. CMPXPluginHandlerBase is
    *         responsible for the memory management of this parameter.
    */
    IMPORT_C virtual void GetSelectionCriteria(
                    TPluginSelection& aPluginSelection,
                    TInt& aPluginType,
                    TUid& aPluginUid,
                    TPtrC8& aDataType,
                    TPtrC8& aScheme,
                    TPtrC8& aExtension);

    /**
    * Clear selection criteria parameters.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual void ClearSelectionCriteria();
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    * Select a plugin based on file handle for the given media.
    *
    * @since S60 3.2.3
    * @param aFile a 64 bit file handle to the media
    */
    IMPORT_C virtual void SelectPlugin64L(const RFile64& aFile);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    
protected:

    /**
    *  Resolves a plugin based on properties (iDataType, iExt and iScheme) and
    *  selection criteria. When a plugin is found, information about the plugin
    *  is returned through parameters.
    *
    *  @since S60 3.2.3
    *  @param aPluginUid This is an OUT parameter. The UID of the matching plugin
    *  @param aIndex This is an OUT parameter. The index for the plugin info, plugin
    *         UID, and plugin types array for the matching plugin
    *  @param aDisplayName This is an OUT parameter. The display name for the matching
    *         plugin. CMPXPluginHandlerBase is responsible for the memory management
    *         of this parameter.
    *  @param aPluginType This is an OUT parameter. The plugin type for the matching
    *         plugin
    *  @leave KErrNotSupported Index is not found
    */
    IMPORT_C virtual void DoResolvePluginL(
                    TUid& aPluginUid,
                    TInt& aIndex,
                    TPtrC& aDisplayName,
                    TInt& aPluginType);

    /**
    * From MMPXPluginMonitorObserver. This is invoked by CMPXPluginMonitor when
    * there is a change in the plugins for the designated plugin interface in
    * the system.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual void PluginsChangedL();

    /**
    * From MMPXPluginMonitorObserver.
    *
    * @since S60 3.2.3
    * @see MMPXPluginMonitorObserver
    */
    IMPORT_C virtual void HandlePluginUnload(const TUid& aPluginUid);

    /**
    * Resolves a plugin. To be implemented by the child class. This is invoked
    * whenever a client selects a plugin and whenever CMPXPluginMonitor notifies
    * that there is a change in the plugins for the designated plugin interface
    * in the system.
    *
    * A typical implementation for this method is to invoke DoResolvePluginL
    * and instantiate a plugin using the plugin UID returned from DoResolvePluginL.
    *
    * NOTE: this method is not pure virtual because currently the playlist
    * recognizer instantiates this class as opposed to the derived handler.
    * This should be fixed and both ResolvePluginL and UnloadPlugin should be
    * made pure virtuals.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual void ResolvePluginL();

    /**
    * Checks if the specified plugin is loaded.
    *
    * @since S60 3.2.3
    * @param aPluginUid identifies the plugin
    * @return ETrue if the plugin is loaded, EFalse otherwise
    */
    IMPORT_C virtual TBool IsPluginLoaded(const TUid& aPluginUid);

    /**
    * Request to the derived class to load a specified plugin.
    *
    * @since S60 3.2.3
    * @param aPluginUid identifies the plugin
    */
    IMPORT_C virtual void LoadPluginL(const TUid& aPluginUid);

    /**
    * Request to the derived class to unload a specified plugin.
    *
    * @since S60 3.2.3
    * @param aPluginUid identifies the plugin
    */
    IMPORT_C virtual void UnloadPlugin(const TUid& aPluginUid);

    /**
    * Constructs the plugins info class.
    * Can be overwritten by subclasses to construct specialized instances of the plugin info.
    * Default implementation constructs a CMPXPluginInfo class.
    *
    * @since S60 3.2.3
    * @param aData implementation data
    */
    IMPORT_C virtual CMPXPluginInfo* ConstructPluginInfoLC( const CImplementationInformation& aData  );

#ifdef _DEBUG

    /**
    * Prints all plugin info entries to the debug log.
    *
    * @since S60 3.2.3
    */
    void PrintPluginInfo();

#endif

public:

    /**
    *  Returns display name for plugin with the given type.
    *
    *  @since S60 3.2.3
    *  @param aType plugin type.
    *  @return plugin name. KNullDesC returned if not found.
    */
    IMPORT_C const TDesC& PluginName(TInt aPluginType) const;

    /**
    *  Returns display name for plugin with the given uid.
    *
    *  @since S60 3.2.3
    *  @param aUid plugin id
    *  @return plugin name. KNullDesC returned if not found.
    */
    IMPORT_C const TDesC& PluginName(const TUid& aUid) const;

    /**
    *  Returns a list of display names for the plugins.
    *
    *  @since S60 3.2.3
    *  @return an array of display names. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* PluginNamesL();

    /**
    *  Return a list of mime types supported by plugins.
    *
    *  @since S60 3.2.3
    *  @return an array of mime types. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedMimeTypesL();

    /**
    *  Return a list of file extensions supported by plugins.
    *
    *  @since S60 3.2.3
    *  @return an array of extensions. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedExtensionsL();

    /**
    *  Return a list of schemas supported by plugins.
    *
    *  @since S60 3.2.3
    *  @return an array of schemas. Client is responsible for
    *          the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedSchemasL();

    /**
    *  Return a list of mime types supported by a plugin.
    *
    *  @since S60 3.2.3
    *  @param aUid plugin id
    *  @return an array of mime types. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedMimeTypesL(const TUid& aUid);

    /**
    *  Return a list of file extensions supported by a plugin.
    *
    *  @since S60 3.2.3
    *  @param aUid plugin id
    *  @return an array of extensions. Client is responsible
    *          for the memory management of this array and its
    *          items.
    */
    IMPORT_C CDesCArray* SupportedExtensionsL(const TUid& aUid);

    /**
    *  Get plugin type for a given implementation UID.
    *
    *  @since S60 3.2.3
    *  @param aUid plugin's implementation Uid
    *  @return plugin's type Uid, or KNullUid if not found
    */
    IMPORT_C TUid PluginType( const TUid& aUid ) const;

    /**
    *  Get supported app uid for a given implementation UID
    *
    *  @param aUid plugin's implementation Uid
    *  @return plugin's supported app Uid, or KNullUid if not found
    */
    IMPORT_C TUid SupportedAppUid( const TUid& aUid ) const;
    
    /**
    *  Get plugin flag for a given implementation UID.
    *
    *  @since S60 3.2.3
    *  @param aUid plugin's implementation Uid
    *  @return Plugin's flag, or leave with KErrNotFound if not found
    *  @leave KErrNotFound Uid is not found
    */
    IMPORT_C TUint PluginFlagsL( const TUid& aUid ) const;

    /**
    * Get currently available plug-in types.
    *
    * @since S60 3.2.3
    * @param aType contains plugins types
    */
    IMPORT_C void GetPluginTypes(RArray<TInt>& aTypes) const;

    /**
    *  Get currently available plug-in UIDs.
    *
    *  @since S60 3.2.3
    *  @param aUids contains plugin uids
    */
    IMPORT_C void GetPluginUids(RArray<TUid>& aUids) const;

    /**
    *  All the UIDs of the plug-ins in the system of a specific type.
    *
    *  @since S60 3.2.3
    *  @param aPlugins OUT parameter. Array of UIDs of all plugins of type
    *                  aPluginType. Client is responsible for the memory
    *                  management of this RArray.
    *  @param aPluginType IN parameter. Plugin type
    */
    IMPORT_C void GetPluginUidsL(RArray<TUid>& aPlugins,
                                 TInt aPluginType) const;

    /**
    * Returns the UID of the plugin with the specified display name.
    *
    * @since S60 3.2.3
    * @param aPluginName a plugin's display name
    * @return UID of the plugin. KNullUid is returned if there is no
    *         plugin by that name.
    */
    IMPORT_C TUid PluginUid(const TDesC& aPluginName) const;

    /**
    * Returns the index of the specified plugin.
    *
    * @param aPluginUid UID of the plugin to search for
    * @return index of the specified plugin in CMPXPluginMonitor's 3
    *         arrays, iPluginsInfo, iPluginUids, and iPluginTypes.
    *         KErrNotFound, if no matching object can be found.
    */
    IMPORT_C TInt IndexOf(const TUid& aPluginUid) const;

    /**
    *  Create a list of plugins.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void CreatePluginListL();

    /**
    *  Check if the plugin supports the array of uids.
    *
    *  @since S60 3.2.3
    *  @param aPluginId plugin id
    *  @param aUids contains plugin uids
    *  @return ETrue if plugin supports all uids
    */
    IMPORT_C TBool SupportUids(const TUid& aPluginId,
                               const TArray<TUid>& aUids) const;

    /**
    *  Find a plugin with matches the array of uids the best.
    *
    *  @since S60 3.2.3
    *  @param aUids contains plugin uids
    *  @return Implementation id of collection plugin,
    *          KNullUidValue will be returned if no plugin found
    */
    IMPORT_C TUid FindPlugin(const TArray<TUid>& aUids) const;

protected:
    MMPXPluginHandlerObserver&          iObserver;          // observer
    TUid                                iInterfaceUid;      // plugin interface Uid
    CMPXPluginMonitor*                  iPluginMonitor;
    RPointerArray<CMPXPluginInfo>       iPluginInfoArray;   // Ordered by priority
    TBool                               iOwnedPluginMonitor;

    //
    // Selection parameters
    //
    TPluginSelection                    iSelection;
    TInt                                iSelectedType;
    TUid                                iSelectedUid;

    //
    // Resolution parameters
    //
    HBufC8*                             iDataType;
    HBufC8*                             iScheme;
    HBufC8*                             iExt;
    };

#endif // CMPXPLUGINHANDLERBASE_H
