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
* Description:  MPX view plugin handler implementation
*
*/



#ifndef CMPXVIEWPLUGINHANDLER_H
#define CMPXVIEWPLUGINHANDLER_H


// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>
#include <mpxpluginhandlerbase.h>
#include <mpxviewplugin.h>
#include "mpxviewpluginmanager.h"


// FORWARD DECLARATIONS
class MMPXCustomCommandObserver;


// CLASS DECLARATION

/**
 *  MPX view plugin handler.
 *
 *  @lib mpxviewutility.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CMPXViewPluginHandler ) : public CMPXPluginHandlerBase,
                                             public MMPXViewPluginManager
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since 3.1
     *
     * @param aCommandObserver Observer to handle commands from the plugins
     * @return Pointer to newly created object.
     */
    static CMPXViewPluginHandler* NewL(
        MMPXCustomCommandObserver* aCommandObserver,
        MMPXPluginHandlerObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CMPXViewPluginHandler();

    /**
     * Wraper function to resolve view plugin
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since 3.0
     * @param aUid Specific view plugin uid or plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @param aResolvedPluginImplementationUid On return, stores the implementation
     *                                         Uid of the resolved plugin
     * @return Pointer to a view plugin object. Leave if not found.
     */
    CMPXViewPlugin* ResolvePluginL(
        const TUid& aUid, TUid& aResolvedPluginType,
        TUid& aResolvedPluginImplementationUid );

    /**
     * Wraper function to resolve view plugin
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since 3.0
     * @param aUids List of Uids, including specific view plugin uid and/or
     *              plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @param aResolvedPluginImplementationUid On return, stores the implementation
     *                                         Uid of the resolved plugin
     * @return Pointer to a view plugin object. Leave if not found.
     */
    CMPXViewPlugin* ResolvePluginL(
        const RArray<TUid>& aUids, TUid& aResolvedPluginType,
        TUid& aResolvedPluginImplementationUid );

    /**
     * Wraper function to get a list of plugins that need to be pre-loaded.
     *
     * @since 3.0
     * @param aUids On return, a list of plugins' uids that match the criteria.
	 * @param aUid of specific application
     */
    void GetPreLoadPluginsL(
        RArray<TUid>& aUids, const TUid& aUid );

// from base class MMPXViewPluginManager

    /**
     * From MMPXViewPluginManager
     * Return view plugin with specific view plugin uid.
     *
     * @since 3.0
     * @param aUid Specific view plugin uid.
     * @return Pointer to a view plugin object. Leave if not found.
     */
    CMPXViewPlugin* PluginL(
        const TUid& aUid );

    /**
     * From MMPXViewPluginManager
     * Wraper function to resolve view plugin
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since 3.0
     * @param aUid Specific view plugin uid or plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @return Pointer to a view plugin object. Leave if not found.
     */
    CMPXViewPlugin* ResolvePluginL(
        const TUid& aUid, TUid& aResolvedPluginType );

    /**
     * From MMPXViewPluginManager
     * Wraper function to resolve view plugin
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since 3.0
     * @param aUids List of Uids, including specific view plugin uid and/or
     *              plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @return Pointer to a view plugin object. Leave if not found.
     */
    CMPXViewPlugin* ResolvePluginL(
        const RArray<TUid>& aUids, TUid& aResolvedPluginType );

private:

    /**
     * C++ default constructor.
     *
     * @param aCommandObserver Observer to handle commands from the plugins
     * @param aObserver handler observer
     */
    CMPXViewPluginHandler( MMPXCustomCommandObserver* aCommandObserver,
        MMPXPluginHandlerObserver& aObserver );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * From Base CMPXPluginHandlerBase
     *
     * Not used, use ResolvePluginL( const RArray/Uid, TUid ) instead
     */
    void ResolvePluginL();

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

private:    // data

    /**
     * Loaded plugins. The index must correspond in these arrays.
     */
    RPointerArray<CMPXViewPlugin> iLoadedPlugins;
    RArray<TUid> iLoadPluginUids;

    MMPXCustomCommandObserver* iCommandObserver; // not owned
    };

#endif  // CMPXVIEWPLUGINHANDLER_H

// End of File
