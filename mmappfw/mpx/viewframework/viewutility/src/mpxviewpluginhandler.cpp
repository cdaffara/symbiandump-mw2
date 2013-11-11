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
* Description:  Implementation of view plugin handler
*
*/



// INCLUDE FILES
#include <mpxcustomcommandobserver.h>
#include <mpxviewplugin.hrh>
#include "mpxpluginhandlerobserver.h"
#include "mpxviewpluginhandler.h"
#include "mpxlog.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMPXViewPluginHandler::CMPXViewPluginHandler(
    MMPXCustomCommandObserver* aCommandObserver,
    MMPXPluginHandlerObserver& aObserver ) :
    CMPXPluginHandlerBase(
        TUid::Uid( KMPXViewPluginInterfaceUid ), ESelectionType, 0, aObserver ),
    iCommandObserver( aCommandObserver )
    {
    }

// ---------------------------------------------------------------------------
// By default Symbian 2nd phase constructor is private.
// ---------------------------------------------------------------------------
//
void CMPXViewPluginHandler::ConstructL()
    {
    MPX_FUNC( "CMPXViewPluginHandler::ConstructL" );
    BaseConstructL();
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewPluginHandler* CMPXViewPluginHandler::NewL(
    MMPXCustomCommandObserver* aCommandObserver,
    MMPXPluginHandlerObserver& aObserver )
    {
    CMPXViewPluginHandler* self =
        new ( ELeave ) CMPXViewPluginHandler( aCommandObserver, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXViewPluginHandler::~CMPXViewPluginHandler()
    {
    for ( TInt i = iLoadedPlugins.Count(); --i >= 0; )
        {
        iLoadedPlugins[i]->DestroyView();
        }
    iLoadedPlugins.ResetAndDestroy();
    iLoadPluginUids.Close();

    iCommandObserver = NULL;
    }

// ---------------------------------------------------------------------------
// Resolve view plugin based on the criteria passed in parameter.
// ---------------------------------------------------------------------------
//
CMPXViewPlugin* CMPXViewPluginHandler::ResolvePluginL(
    const TUid& aUid, TUid& aResolvedPluginType,
    TUid& aResolvedPluginImplementationUid )
    {
    MPX_FUNC( "CMPXViewPluginHandler::ResolvePluginL single uid" );
    RArray<TUid> array;
    CleanupClosePushL( array );
    array.AppendL( aUid );
    CMPXViewPlugin* plugin( NULL );
    plugin = ResolvePluginL(
        array, aResolvedPluginType, aResolvedPluginImplementationUid );
    CleanupStack::PopAndDestroy( &array );

    return plugin;
    }

// ---------------------------------------------------------------------------
// Resolve view plugin based on the criteria passed in parameter.
// ---------------------------------------------------------------------------
//
CMPXViewPlugin* CMPXViewPluginHandler::ResolvePluginL(
    const RArray<TUid>& aUids, TUid& aResolvedPluginType,
    TUid& aResolvedPluginImplementationUid )
    {
    MPX_FUNC( "CMPXViewPluginHandler::ResolvePluginL uid array" );
#ifdef _DEBUG
    for ( TInt i = 0; i < aUids.Count(); i++ )
        {
        MPX_DEBUG2( "CMPXViewPluginHandler::ResolvePluginL input Uid = 0x%x", aUids[i].iUid );
        }
#endif // _DEBUG
    TUid impUid( FindPlugin( aUids.Array() ) );
    MPX_DEBUG2( "CMPXViewPluginHandler::ResolvePluginL resolved implementation Uid = 0x%x", impUid.iUid );
    if ( impUid == KNullUid )
        {
        User::Leave( KErrNotFound );
        }
    else
        {
        aResolvedPluginImplementationUid = impUid;
        aResolvedPluginType = PluginType( impUid );
        }
    return PluginL( impUid );
    }

// ---------------------------------------------------------------------------
// Get a list of plugins that need to be pre-loaded.
// ---------------------------------------------------------------------------
//
void CMPXViewPluginHandler::GetPreLoadPluginsL( RArray<TUid>& aUids, const TUid& aUid )
    {
    MPX_DEBUG2( "CMPXViewPluginHandler::GetPreLoadPluginsL aUID: 0x%x", aUid );
    RArray<TUid> array;
    CleanupClosePushL( array );
    GetPluginUids( array );

    for ( TInt i = array.Count(); --i >= 0; )
        {
        if ( PluginFlagsL( array[ i ] ) & EMPXPluginFlagPreLoad )
            {
            if ( aUid == KNullUid )
            	{
            MPX_DEBUG2( "CMPXViewPluginHandler::GetPreLoadPluginsL adding UID: 0x%x", array[i] );
            aUids.Append( array[ i ] );
				}
			else
				{
				TUid impUid = SupportedAppUid( array[ i ] );
				MPX_DEBUG2( "APP UID: 0x%x", impUid );
				if ( aUid == impUid )
					{
            		MPX_DEBUG2( "CMPXViewPluginHandler::GetPreLoadPluginsL adding UID: 0x%x", array[i] );
            		aUids.Append( array[ i ] );
					}
				}
            }
        }
    CleanupStack::PopAndDestroy( &array );
    }

// ---------------------------------------------------------------------------
// From MMPXViewPluginManager
// Return view plugin with specific view plugin uid.
// ---------------------------------------------------------------------------
//
CMPXViewPlugin* CMPXViewPluginHandler::PluginL(
    const TUid& aUid )
    {
    MPX_FUNC( "CMPXViewPluginHandler::PluginL" );
    CMPXViewPlugin* plugin( NULL );
    TInt index = iLoadPluginUids.Find( aUid );

    if ( KErrNotFound == index )
        {
        // Check if uid is a valid view plugin uid.
        if ( IndexOf( aUid ) != KErrNotFound )
            {
            plugin = CMPXViewPlugin::NewL( aUid, iCommandObserver );
            CleanupStack::PushL( plugin );
            iLoadPluginUids.AppendL( aUid );
            iLoadedPlugins.AppendL( plugin );
            CleanupStack::Pop( plugin );
            }
        else
            {
            // Leave if not found
            User::Leave( KErrNotFound );
            }
        }
    else
        {
        plugin = iLoadedPlugins[index];
        }
    return plugin;
    }

// ---------------------------------------------------------------------------
// From MMPXViewPluginManager
// Resolve view plugin based on the criteria passed in parameter.
// ---------------------------------------------------------------------------
//
CMPXViewPlugin* CMPXViewPluginHandler::ResolvePluginL(
    const TUid& aUid, TUid& aResolvedPluginType )
    {
    MPX_FUNC( "CMPXViewPluginHandler::ResolvePluginL single uid wrapper" );
    TUid impUid( KNullUid );
    return ResolvePluginL( aUid, aResolvedPluginType, impUid );
    }

// ---------------------------------------------------------------------------
// From MMPXViewPluginManager
// Resolve view plugin based on the criteria passed in parameter.
// ---------------------------------------------------------------------------
//
CMPXViewPlugin* CMPXViewPluginHandler::ResolvePluginL(
    const RArray<TUid>& aUids, TUid& aResolvedPluginType )
    {
    MPX_FUNC( "CMPXViewPluginHandler::ResolvePluginL uid array wrapper" );
    TUid impUid( KNullUid );
    return ResolvePluginL( aUids, aResolvedPluginType, impUid );
    }

// ---------------------------------------------------------------------------
// From CMPXPluginHandlerBase
// Resolve view plugin based on the criteria passed in parameter.
// ---------------------------------------------------------------------------
//
void CMPXViewPluginHandler::ResolvePluginL()
    {
    MPX_FUNC( "CMPXViewPluginHandler::ResolvePluginL" );
    // not used
    }

// ----------------------------------------------------------------------------
// CMPXViewPluginHandler::IsPluginLoaded
// ----------------------------------------------------------------------------
//
TBool CMPXViewPluginHandler::IsPluginLoaded(
    const TUid& aPluginUid)
    {
    MPX_FUNC("CMPXViewPluginHandler::IsPluginLoaded");
    return (iLoadPluginUids.Find(aPluginUid) != KErrNotFound);
    }

// ----------------------------------------------------------------------------
// CMPXViewPluginHandler::LoadPluginL
// ----------------------------------------------------------------------------
//
void CMPXViewPluginHandler::LoadPluginL(
    const TUid& aPluginUid)
    {
    MPX_FUNC("CMPXViewPluginHandler::LoadPluginL");
    (void)PluginL(aPluginUid);
    }

// ----------------------------------------------------------------------------
// Unload the plugin
// ----------------------------------------------------------------------------
//
void CMPXViewPluginHandler::UnloadPlugin(
    const TUid& aUid)
    {
    MPX_FUNC("CMPXViewPluginHandler::UnloadPlugin");

    TInt index = iLoadPluginUids.Find(aUid);
    if (index != KErrNotFound)
        {
        CMPXViewPlugin* plugin = iLoadedPlugins[index];
        plugin->DestroyView();
        iLoadedPlugins.Remove(index);
        iLoadPluginUids.Remove(index);
        delete plugin;
        }
    }


//  End of File
