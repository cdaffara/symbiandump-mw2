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
* Description:  Implementation of view utility
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>
#include <centralrepository.h>
#include <mpxprivatecrkeys.h>

#include "mpxviewutilitycommanddef.h"
#include "mpxviewhistory.h"
#include "mpxviewutilityhelper.h"
#include "mpxviewutilityimp.h"
#include "mpxviewpluginhandler.h"
#include "mpxlog.h"

// CONSTANTS
_LIT( KMPXDelimiter, ";" );
const TInt KMPXMaxHistoryLength( 255 );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMPXViewUtilityImp::CMPXViewUtilityImp()
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// By default Symbian 2nd phase constructor is private.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::ConstructL()
    {
    iPluginManager = CMPXViewPluginHandler::NewL( this, *this );
    iCurrentView = CMPXViewHistory::NewL();
    ConstructDefaultViewHistoryL();
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewUtilityImp* CMPXViewUtilityImp::NewL()
    {
    CMPXViewUtilityImp* self = new ( ELeave ) CMPXViewUtilityImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXViewUtilityImp::~CMPXViewUtilityImp()
    {
    delete iPreviousView;
    delete iCurrentView;
    delete iDefaultView;
    delete iPluginManager;
    iDefaultHistoryPath.ResetAndDestroy();
    iCurrentHistoryPath.ResetAndDestroy();

    iObservers.Reset();
    }

// ---------------------------------------------------------------------------
// Retrieves view utility from TLS. Creates if it's not there.
// ---------------------------------------------------------------------------
//
MMPXViewUtility* CMPXViewUtilityImp::UtilityL()
    {
    MPX_FUNC( "CMPXViewUtilityImp::UtilityL" );
    MMPXViewUtility* utility = reinterpret_cast<MMPXViewUtility*>( Dll::Tls() );
    if ( !utility )
        {
        utility = CMPXViewUtilityImp::NewL();
        StoreViewUtility( utility );
        }

    static_cast<CMPXViewUtilityImp*>( utility )->iRefCount++;
    return utility;
    }

// ---------------------------------------------------------------------------
// Store view utility to TLS.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::StoreViewUtility( MMPXViewUtility* aUtility )
    {
    Dll::SetTls( aUtility );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// This method must be called when the client no longer needs this instance.
// This object will be destroyed when no one is using it.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::Close()
    {
    ASSERT( iRefCount > 0 );
    MPX_FUNC_EX( "CMPXViewUtilityImp::Close" );
    MPX_DEBUG2( "CMPXViewUtilityImp::Close ref count = %d", iRefCount );
    if ( --iRefCount == 0 )
        {
        MPX_DEBUG1( "CMPXViewUtilityImp::Close deleting this" );
        // last client released
        StoreViewUtility( NULL );
        delete this;
        }
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Activate view by resolving passed uid.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::ActivateViewL(
    const TUid& aUid,
    const TDesC* aParam )
    {
    RArray<TUid> array;
    CleanupClosePushL( array );
    array.AppendL( aUid );
    ActivateViewL( array, EFalse, EFalse, aParam );
    CleanupStack::PopAndDestroy( &array );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Activate view by resolving passed uids.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::ActivateViewL(
    const RArray<TUid>& aUids,
    const TDesC* aParam )
    {
    ActivateViewL( aUids, EFalse, EFalse, aParam );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Activate view as default view, does not bring to foreground.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::SetAsDefaultViewL( const TUid& aUid )
    {
    RArray<TUid> array;
    CleanupClosePushL( array );
    array.AppendL( aUid );
    SetAsDefaultViewL( array );
    CleanupStack::PopAndDestroy( &array );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Activate view by resolving passed uids.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::SetAsDefaultViewL( const RArray<TUid>& aUids )
    {
    TInt count = iCurrentHistoryPath.Count();
    if ( count > 0 )
        {
        User::Leave( KErrAlreadyExists );
        }
    else
        {
        TUid pluginType( KNullUid );
        TUid pluginImpUid( KNullUid );
        CMPXViewPlugin* plugin(
            iPluginManager->ResolvePluginL( aUids, pluginType, pluginImpUid ) );

        MPX_DEBUG4( "CMPXViewUtilityImp::SetAsDefaultViewL adding type: 0x%x, impUid: 0x%x, address: 0x%x to current view history",
            pluginType.iUid, pluginImpUid.iUid, plugin );
        CMPXViewHistory* viewHistory = CMPXViewHistory::NewL( aUids, pluginType, pluginImpUid, plugin );
        iCurrentHistoryPath.Append( viewHistory ); // iCurrentHistoryPath takes ownership
        delete iCurrentView;
        iCurrentView = NULL;
        iCurrentView = CMPXViewHistory::NewL( viewHistory );
        delete iDefaultView;
        iDefaultView = NULL;
        iDefaultView = CMPXViewHistory::NewL( viewHistory );

        NotifyObservers( ActiveViewType(), PreviousViewType() );

        MPX_DEBUG2( "CMPXViewUtilityImp::SetAsDefaultViewL activating 0x%x as default view", pluginType.iUid );
        TRAPD( err, plugin->SetAsDefaultViewL() );
        if ( err != KErrNone )
            {
            MPX_DEBUG2( "CMPXViewUtilityImp::SetAsDefaultViewL error duing SetAsDefaultViewL, leaving with code %d", err );
            // error during activation, clean up view history
            MarkViewAsClosedL( plugin );
            User::Leave( err );
            }
        else if ( ActiveViewType() != pluginType )
            {
            // notify observers again if the view type has changed
            // possibily because ActivateViewL of the plugin is sync
            // and the plugin exited already
            MPX_DEBUG1( "CMPXViewUtilityImp::SetAsDefaultViewL notifying observers again" );
            NotifyObservers( ActiveViewType(), PreviousViewType() );
            }
        else
            {
            // nothing to do
            }
        }
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Activate previous view.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::ActivatePreviousViewL()
    {
    // if can't find default history in cenrep, don't use default history
    if ( iCurrentHistoryPath.Count() > 1 )
        {
        TBool jump( EFalse );
        // previousViewType contains a list of view types that needs to be checked
        TUid previousViewType = KNullUid;
        if ( iDefaultHistoryPath.Count() > 0 )
            {
            // default path exist, check that
            TUid currentViewType(
                iCurrentHistoryPath[ iCurrentHistoryPath.Count() - 1 ]->iPluginType );
            TInt count( iDefaultHistoryPath.Count() );

            // loop starts at one, since there's no previous view in index 0
            for ( TInt i = 1; i < count && previousViewType == KNullUid; i++ )
                {
                if ( iDefaultHistoryPath[ i ]->iPluginType == currentViewType )
                    {
                    previousViewType = iDefaultHistoryPath[ i - 1 ]->iPluginType;
                    }
                }

            if ( previousViewType != KNullUid )
                {
                jump = ETrue;
                // if previousViewType exist in iCurrentHistoryPath, it's not a jump
                count = iCurrentHistoryPath.Count();
                for ( TInt i = 0; ( ( i < count ) && ( jump ) ); i++ )
                    {
                    if ( iCurrentHistoryPath[ i ]->iPluginType == previousViewType )
                        {
                        jump = EFalse;
                        }
                    }
                MPX_DEBUG2( "CMPXViewUtilityImp::ActivatePreviousViewL activating jump = %d", jump );
                }
#ifdef _DEBUG
            else
                {
                // current UID doesn't exist in default path, just follow the current path
                MPX_DEBUG1( "CMPXViewUtilityImp::ActivatePreviousViewL activating current UID doesn't exist in default path" );
                }
#endif // _DEBUG
            }
#ifdef _DEBUG
        else
            {
            // default path doesn't exist, just use current history path
            MPX_DEBUG1( "CMPXViewUtilityImp::ActivatePreviousViewL activating without default history" );
            }
#endif // _DEBUG
        RArray<TUid> array;
        CleanupClosePushL( array );
        TInt lastIndex( iCurrentHistoryPath.Count() - 1 );
        TBool updatePreviousView( EFalse );
        TBool activatingPreviousView( EFalse );
        UpdatePreviousViewInfoL( lastIndex );
        if ( jump )
            {
            if ( previousViewType != KNullUid )
                {
                MPX_DEBUG2( "CMPXViewUtilityImp::ActivatePreviousViewL activating 0x%x jumped", previousViewType.iUid );
                updatePreviousView = ETrue;
                array.AppendL( previousViewType );
                }
            }
        else
            {
            TInt count = iCurrentHistoryPath[ lastIndex - 1 ]->iUids.Count();
            MPX_DEBUG2( "CMPXViewUtilityImp::ActivatePreviousViewL activating not jumped iUid.Count %d", count );
            for ( TInt i = 0; i < count; i++ )
                {
                TUid u = iCurrentHistoryPath[ lastIndex - 1 ]->iUids[ i ];
                MPX_DEBUG2( "CMPXViewUtilityImp::ActivatePreviousViewL activating not jumped, appending 0x%x", u.iUid );
                array.AppendL( u );
                }
            activatingPreviousView = ETrue;
            }
        // last index is current view, previous view is at last index - 1
        delete iCurrentHistoryPath[ lastIndex ];
        iCurrentHistoryPath.Remove( lastIndex );
        iCurrentHistoryPath.Compress();
        ActivateViewL( array, activatingPreviousView, updatePreviousView );
        CleanupStack::PopAndDestroy( &array );
        }
    else
        {
        // no history
        User::Leave( KErrNotFound );
        }
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Pre-load view plug-ins that have KMPXVpFeaturePreLoaded flag set.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::PreLoadViewsL()
    {
    MPX_FUNC( "CMPXViewUtilityImp::PreLoadViewsL" );
	PreLoadViewsL( KNullUid );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Pre-load view plug-ins that have KMPXVpFeaturePreLoaded flag set and also by resolving UID
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::PreLoadViewsL( const TUid& aUid )
    {
    MPX_DEBUG2( "CMPXViewUtilityImp::PreLoadViewsL aUid: 0x%x", aUid );
    RArray<TUid> array;
    CleanupClosePushL( array );

    // Get list of plug-ins that need to be pre-loaded
   	iPluginManager->GetPreLoadPluginsL( array, aUid );

    for ( TInt i = array.Count(); --i >= 0; )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::PreLoadViewsL loading UID: 0x%x", array[i] );
        CMPXViewPlugin* plugin = iPluginManager->PluginL( array[i] );
        plugin->CreateViewL();
        }

    CleanupStack::PopAndDestroy( &array );
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Pre-load view plug-in by resolving passed uid.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::PreLoadViewL( const TUid& aUid )
    {
    TUid pluginType( KNullUid );
    TUid pluginImpUid( KNullUid );
    CMPXViewPlugin* plugin(
        iPluginManager->ResolvePluginL( aUid, pluginType, pluginImpUid ) );
    plugin->CreateViewL();
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Get a reference to view plugin manager.
// ---------------------------------------------------------------------------
//
MMPXViewPluginManager& CMPXViewUtilityImp::ViewPluginManager()
    {
    return *iPluginManager;
    }

// ---------------------------------------------------------------------------
// From MMPXViewUtility
// Constructs default view history
// ---------------------------------------------------------------------------
//
TUid CMPXViewUtilityImp::ConstructDefaultViewHistoryL()
    {
    iDefaultHistoryPath.ResetAndDestroy();
    delete iDefaultView;
    iDefaultView = NULL;

    CRepository* cenRepository = CRepository::NewLC( KCRUidMPXSettings );
    TBool isEmbedded = CEikonEnv::Static()->StartedAsServerApp();
    MPX_DEBUG2( "CMPXViewUtilityImp::ConstructDefaultViewHistoryL isEmbedded: %d", isEmbedded );

    // check if embeded mode, if embeded mode use host application uid
    // CAiwGenericParamList
    // if not embeded mode, use app uid

    // get default paths
    TUid settingsUid( KNullUid );
    if ( isEmbedded )
        {
        // TO-DO: get host UID
        }
    else
        {
        CEikAppUi* appUi = CEikonEnv::Static()->EikAppUi();
        if ( appUi )
            {
            CEikApplication* app = appUi->Application();
            if ( app )
                {
                settingsUid = app->AppDllUid();
                }
            }
        }
    TInt ret ( KErrGeneral );
    HBufC* defaultPath = HBufC::NewLC( KMPXMaxHistoryLength );
    MPX_DEBUG2( "CMPXViewUtilityImp::ConstructDefaultViewHistoryL trying to read default history from uid 0x%x", settingsUid.iUid );
    TPtr dPathPtr = defaultPath->Des();
    ret = cenRepository->Get( settingsUid.iUid, dPathPtr );
    if ( ret == KErrNone )
        {
        TPtrC defaultPathPtr( *defaultPath );
        TInt nextLoc = defaultPathPtr.Find( KMPXDelimiter );
        TBool defaultSet( EFalse );
        while ( nextLoc != KErrNotFound )
            {
            TPtrC currentPath = defaultPathPtr.Left( nextLoc );
            AddToDefaultViewHistoryL( currentPath );
            if ( !defaultSet && iDefaultHistoryPath.Count() == 1 )
                {
                // first entry is the default view
                iDefaultView = CMPXViewHistory::NewL( iDefaultHistoryPath[0] );
                defaultSet = ETrue;
                }
            defaultPathPtr.Set( defaultPathPtr.Mid( nextLoc + 1 ) );
            nextLoc = defaultPathPtr.Find( KMPXDelimiter );
            }
        if ( defaultPathPtr.Length() > 0 )
            {
            AddToDefaultViewHistoryL( defaultPathPtr );
            }
        }
    else
        {
        // get default view
        HBufC* defaultView = HBufC::NewLC( KMPXMaxHistoryLength );
        TPtr dViewPtr = defaultView->Des();
        ret = cenRepository->Get( KMPXVUCurrentDefaultView, dViewPtr );
        if ( ret == KErrNone )
            {
            TPtrC defaultViewPtr( *defaultView );
            if ( defaultViewPtr.Length() > 0 )
                {
                TInt nextPos( 0 );
                TInt err(
                    CMPXViewUtilityHelper::ExtractNum( defaultViewPtr, nextPos, ret ) );
                if ( err == KErrNone )
                    {
                    iDefaultView = CMPXViewHistory::NewL();
                    iDefaultView->iPluginType = TUid::Uid( ret );
                    if ( iDefaultHistoryPath.Count() > 0 )
                        {
                        TRAP( ret, AddToDefaultViewHistoryL( defaultViewPtr, 0 ) );
                        }
                    }

                if ( iDefaultHistoryPath.Count() == 0 && err != KErrNone )
                    {
                    ret = err;
                    }
                }
            }
        CleanupStack::PopAndDestroy( defaultView );
        }
    CleanupStack::PopAndDestroy( defaultPath );

    // clean up duplicate entries
    TInt count( iDefaultHistoryPath.Count() );
    TUid previousUid( KNullUid );
    for ( TInt i = count - 1; i >= 0; i-- )
        {
        if ( iDefaultHistoryPath[ i ]->iPluginType == previousUid )
            {
            MPX_DEBUG3( "CMPXViewUtilityImp::ConstructDefaultViewHistoryL removing duplicate entry 0x%x at %d", iDefaultHistoryPath[ i ]->iPluginType.iUid, i );
            // duplicate entry, remove
            delete iDefaultHistoryPath[ i ];
            iDefaultHistoryPath.Remove( i );
            }
        else
            {
            previousUid = iDefaultHistoryPath[ i ]->iPluginType;
            }
        }
    iDefaultHistoryPath.Compress();
    MPX_DEBUG2( "CMPXViewUtilityImp::ConstructDefaultViewHistoryL after clean up, default path contains %d elements", iDefaultHistoryPath.Count() );

    CleanupStack::PopAndDestroy( cenRepository );
    return DefaultViewUid();
    }

// ---------------------------------------------------------------------------
// Push default history to view history
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::PushDefaultHistoryL()
    {
    MPX_FUNC( "CMPXViewUtilityImp::PushDefaultHistoryL" );
    TInt count( iDefaultHistoryPath.Count() );
    iCurrentHistoryPath.ResetAndDestroy();
    delete iPreviousView;
    iPreviousView = NULL;
    if ( count > 0 )
        {
        for ( TInt i = 0; i < count; i++ )
            {
            CMPXViewHistory* viewHistory = CMPXViewHistory::NewL();
            CleanupStack::PushL( viewHistory );
            TUid pluginType( iDefaultHistoryPath[ i ]->iPluginType );
            viewHistory->iPluginType = pluginType;
            viewHistory->iUids.Append( pluginType );
            viewHistory->iPlugin = NULL;
            iCurrentHistoryPath.Append( viewHistory );
            CleanupStack::Pop( viewHistory );
            }
        }
#ifdef _DEBUG
    else
        {
        MPX_DEBUG1( "CMPXViewUtilityImp::PushDefaultHistoryL default history is not defined, FAILED" );
        }
#endif
    }

// ---------------------------------------------------------------------------
// Gets default view's UID
// ---------------------------------------------------------------------------
//
TUid CMPXViewUtilityImp::DefaultViewUid()
    {
#ifdef _DEBUG
    if ( iDefaultView )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::DefaultViewUid = 0x%x", iDefaultView->iPluginType.iUid );
        }
#endif // _DEBUG
    return iDefaultView ? iDefaultView->iPluginType : KNullUid;
    }

// ---------------------------------------------------------------------------
// Returns the current history depth
// ---------------------------------------------------------------------------
//
TInt CMPXViewUtilityImp::ViewHistoryDepth()
    {
#ifdef _DEBUG
    TInt count( iCurrentHistoryPath.Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        TUid currentPluginType = iCurrentHistoryPath[ i ]->iPluginType;
        CMPXViewPlugin* plugin = iCurrentHistoryPath[ i ]->iPlugin;
        MPX_DEBUG4( "CMPXViewUtilityImp::ViewHistoryDepth (%d) type = 0x%x, address = 0x%x", i, currentPluginType.iUid, plugin );
        }
#endif
    return iCurrentHistoryPath.Count();
    }

// ---------------------------------------------------------------------------
// Returns the last activated view type
// ---------------------------------------------------------------------------
//
TUid CMPXViewUtilityImp::PreviousViewType()
    {
#ifdef _DEBUG
    if ( iPreviousView )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::PreviousViewType = 0x%x", iPreviousView->iPluginType.iUid );
        }
#endif // _DEBUG
    return iPreviousView ? iPreviousView->iPluginType : KNullUid;
    }

// ---------------------------------------------------------------------------
// Returns the current activated view type
// ---------------------------------------------------------------------------
//
TUid CMPXViewUtilityImp::ActiveViewType()
    {
#ifdef _DEBUG
    if ( iCurrentView )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::ActiveViewType = 0x%x", iCurrentView->iPluginType.iUid );
        }
#endif // _DEBUG
    return iCurrentView ? iCurrentView->iPluginType : KNullUid;
    }

// ---------------------------------------------------------------------------
// Returns the current activated view type
// ---------------------------------------------------------------------------
//
TUid CMPXViewUtilityImp::ActiveViewImplementationUid()
    {
#ifdef _DEBUG
    MPX_DEBUG1( "-->CMPXViewUtilityImp::ActiveViewImplementationUid" );
    if( iCurrentView &&
        ( iCurrentView->iPluginImplementationUid != KNullUid ) )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::ActiveViewImplementationUid = 0x%x",
            iCurrentView->iPluginImplementationUid.iUid );
        }
    else
        {
        MPX_DEBUG1( "CMPXViewUtilityImp::ActiveViewImplementationUid - active view implementation uid not resolved yet" );
        }
    MPX_DEBUG1( "<--CMPXViewUtilityImp::ActiveViewImplementationUid" );
#endif // _DEBUG
    return iCurrentView ? iCurrentView->iPluginImplementationUid : KNullUid;
    }

// ---------------------------------------------------------------------------
// Add observer.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::AddObserverL(
    MMPXViewActivationObserver* aObserver )
    {
    if ( aObserver )
        {
        iObservers.AppendL( aObserver );
        }
    }

// ---------------------------------------------------------------------------
// Remove observer.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::RemoveObserver(
    MMPXViewActivationObserver* aObserver )
    {
    TInt index = iObservers.Find( aObserver );
    ASSERT( index >= 0 );
    if ( index >= 0 )
        {
        iObservers.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// Handle custom commands.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::HandleCustomCommand(
    const TUid& aUid,
    const TInt aCommand,
    const TDesC8& aData )
    {
    MPX_FUNC( "CMPXViewUtilityImp::HandleCustomCommand" );
    if ( aUid.iUid == KMPXViewUtilityCustomCommand )
        {
        switch ( aCommand )
            {
            case EMPXViewUtilCmdViewActivated:
                {
                // TO-DO
                break;
                }
            case EMPXViewUtilCmdViewDeActivated:
                {
                MPX_DEBUG1( "CMPXViewUtilityImp::HandleCustomCommand EMPXViewUtilCmdViewDeActivated" );
                // aData contains the pointer to the view
                if ( aData.Length() > 0 )
                    {
                    TPckgBuf<TInt> buf( KErrNotFound );
                    buf.Copy( aData );
                    TInt viewPtr = buf();
                    MPX_DEBUG2( "CMPXViewUtilityImp::HandleCustomCommand EMPXViewUtilCmdViewDeActivated address = 0x%x", viewPtr );
                    CMPXViewPlugin* piPtr = reinterpret_cast<CMPXViewPlugin*>( viewPtr );
                    TRAP_IGNORE( MarkViewAsClosedL( piPtr ) );
                    }
                break;
                }
            default:
                {
                break;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CMPXViewUtilityImp::HandlePluginHandlerEvent
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::HandlePluginHandlerEvent(
    TPluginHandlerEvents aEvent,
    const TUid& aPluginUid,
    TInt aLoaded,
    TInt aData)
    {
    MPX_FUNC("CMPXViewUtilityImp::HandlePluginHandlerEvent");

    switch (aEvent)
        {
        case EPluginAdd:
            {
            NotifyViewUpdate(aPluginUid, MMPXViewActivationObserver::EViewPluginAdd,
                aLoaded, aData);
            break;
            }
        case EPluginUpdateStart:
            {
            NotifyViewUpdate(aPluginUid, MMPXViewActivationObserver::EViewPluginUpdate,
                aLoaded, aData);
            break;
            }
        case EPluginRemove:
            {
            NotifyViewUpdate(aPluginUid, MMPXViewActivationObserver::EViewPluginRemove,
                aLoaded, aData);
            break;
            }
        case EPluginUpdateEnd:
            {
            // do nothing
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// Activate view by resolving passed uids.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::ActivateViewL(
    const RArray<TUid>& aUids,
    TBool aIsActivatingPreviousView,
    TBool aUpdatedPreviousView,
    const TDesC* aParam )
    {
    TUid pluginType( KNullUid );
    TUid pluginImpUid( KNullUid );
    CMPXViewPlugin* plugin(
        iPluginManager->ResolvePluginL( aUids, pluginType, pluginImpUid ) );
    CMPXViewPlugin* currentPlugin( NULL );

    // if the resolved view is already activated
    TInt count = iCurrentHistoryPath.Count();
    TUid currentPluginType( KNullUid );
    if ( count > 0 )
        {
        currentPluginType = iCurrentHistoryPath[ count - 1 ]->iPluginType;
        currentPlugin = iCurrentHistoryPath[ count - 1 ]->iPlugin;
        }

    if ( ( currentPluginType != pluginType ) || ( currentPlugin != plugin ) ||
        ( aIsActivatingPreviousView ) )
        {
        if ( ( pluginType != KNullUid ) && ( !aIsActivatingPreviousView ) )
            {
            if ( !aUpdatedPreviousView )
                {
                if ( count > 0 )
                    {
                    UpdatePreviousViewInfoL( KErrNotFound );
                    }
                }

            // check if the view has been activated before
            TInt index( FindCurrentHistoryEntryByTypeL( pluginType ) );
            if ( index != KErrNotFound )
                {
                // cleanup, count down
                for ( TInt i = count - 1; i > index; i-- )
                    {
                    MPX_DEBUG2( "CMPXViewUtilityImp::ActivateViewL removing entry %d", i );
                    delete iCurrentHistoryPath[ i ];
                    iCurrentHistoryPath.Remove( i );
                    }
                iCurrentHistoryPath.Compress();

                // update the uids used to activate this view
                CMPXViewHistory* vh( iCurrentHistoryPath[ index ] );
                vh->iPlugin = plugin;
                vh->iUids.Reset();
                TInt uidCount( aUids.Count() );
                for ( TInt i = 0; i < uidCount; i++ )
                    {
                    TUid u = aUids[ i ];
                    vh->iUids.Append( u );
                    }
                }
            else
                {
                MPX_DEBUG3( "CMPXViewUtilityImp::ActivateViewL adding type: 0x%x, address: 0x%x to current view history", pluginType.iUid, plugin );
                CMPXViewHistory* viewHistory = CMPXViewHistory::NewL();
                CleanupStack::PushL( viewHistory );
                TInt uidCount( aUids.Count() );
                for ( TInt i = 0; i < uidCount; i++ )
                    {
                    TUid u = aUids[ i ];
                    viewHistory->iUids.Append( u );
                    }
                viewHistory->iPlugin = plugin;
                viewHistory->iPluginType = pluginType;
                viewHistory->iPluginImplementationUid = pluginImpUid;
                iCurrentHistoryPath.Append( viewHistory );
                CleanupStack::Pop( viewHistory );
                }
            }
        MPX_DEBUG2( "CMPXViewUtilityImp::ActivateViewL activating 0x%x", pluginType.iUid );

        if ( plugin != iCurrentView->iPlugin )
            {
            delete iCurrentView;
            iCurrentView = NULL;
            iCurrentView = CMPXViewHistory::NewL( aUids, pluginType, pluginImpUid, plugin );
            // Notify observers for view activation, ActivateViewL maybe sync
            // and won't return until the view is closed
            NotifyObservers( ActiveViewType(), PreviousViewType() );
            TRAPD( err, plugin->ActivateViewL( aParam ) );
            if ( err != KErrNone )
                {
                MPX_DEBUG2( "CMPXViewUtilityImp::ActivateViewL error duing ActivateViewL, leaving with code %d", err );
                // error during view activation, clean up view history
                MarkViewAsClosedL( plugin );
                User::Leave( err );
                }
            }
        else
            {
            MPX_DEBUG2( "CMPXViewUtilityImp::ActivateViewL view 0x%x already activated, ignoring", pluginType.iUid );
            }
        }
    else
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::ActivateViewL view 0x%x already activated, ignoring", pluginType.iUid );
        }
    }

// ---------------------------------------------------------------------------
// Marks the view closed.  This does not actually close the view.
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::MarkViewAsClosedL( CMPXViewPlugin* aPlugin )
    {
    MPX_DEBUG2( "CMPXViewUtilityImp::MarkViewAsClosedL called 0x%x", aPlugin );
    TInt count = iCurrentHistoryPath.Count();
    TBool removed( EFalse );
    // Count down, counting up and removing items is bad
    for ( TInt i = count - 1; i >= 0 && !removed ; i-- )
        {
        if ( iCurrentHistoryPath[ i ]->iPlugin == aPlugin )
            {
            MPX_DEBUG2( "CMPXViewUtilityImp::MarkViewAsClosedL removing location %d", i );
            UpdatePreviousViewInfoL( i );

            delete iCurrentView;
            iCurrentView = NULL;
            if ( i > 0 )
                {
                iCurrentView = CMPXViewHistory::NewL(
                    iCurrentHistoryPath[ ( i != count - 1 ) ? count - 1 : i - 1 ] );
                }

            // match found, remove it from history
            delete iCurrentHistoryPath[ i ];
            iCurrentHistoryPath.Remove( i );
            removed = ETrue;

            // Notify observers for view activation, ActivateViewL maybe sync
            // and won't return until the view is closed
            NotifyObservers( ActiveViewType(), PreviousViewType() );
            }
        }
    if ( removed )
        {
        iCurrentHistoryPath.Compress();
        }
#ifdef _DEBUG
    else
        {
        MPX_DEBUG1( "CMPXViewUtilityImp::MarkViewAsClosedL Nothing removed" );
        }
#endif
    }

// -----------------------------------------------------------------------------
// Notify observers for view activation
// -----------------------------------------------------------------------------
//
void CMPXViewUtilityImp::NotifyObservers(
    const TUid& aCurrentViewType,
    const TUid& aPreviousViewType )
    {
    MPX_FUNC( "CMPXViewUtilityImp::NotifyObservers" );
    MPX_DEBUG3( "CMPXViewUtilityImp::NotifyObservers current: 0x%x, previous: 0x%x", aCurrentViewType.iUid, aPreviousViewType.iUid );
    TInt count( iObservers.Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        iObservers[i]->HandleViewActivation(
            aCurrentViewType,
            aPreviousViewType );
        }
    }

// -----------------------------------------------------------------------------
// Notify observers of a view update event
// -----------------------------------------------------------------------------
//
void CMPXViewUtilityImp::NotifyViewUpdate(
    const TUid& aViewUid,
    MMPXViewActivationObserver::TViewUpdateEvent aUpdateEvent,
    TBool aLoaded,
    TInt aVersion /* = 0 */)
    {
    MPX_FUNC( "CMPXViewUtilityImp::NotifyViewUpdate" );

    TInt count(iObservers.Count());
    for (TInt i = 0; i < count; ++i)
        {
        iObservers[i]->HandleViewUpdate(aViewUid, aUpdateEvent, aLoaded, aVersion);
        }
    }

// ---------------------------------------------------------------------------
// Insert/Append to default view history array
// ---------------------------------------------------------------------------
//
TInt CMPXViewUtilityImp::AddToDefaultViewHistoryL(
    const TDesC& aData, TInt aLoc )
    {
    TInt result( KErrGeneral );
    TInt nextPos( 0 );
    User::LeaveIfError( CMPXViewUtilityHelper::ExtractNum( aData, nextPos, result ) );
    CMPXViewHistory* viewHistory = CMPXViewHistory::NewL();
    CleanupStack::PushL( viewHistory );
    viewHistory->iPluginType.iUid = result;
    if ( aLoc == -1 )
        {
        MPX_DEBUG2( "CMPXViewUtilityImp::AddToDefaultViewHistoryL Appending 0x%x to default view history", result );
        User::LeaveIfError( iDefaultHistoryPath.Append( viewHistory ) );
        }
    else
        {
        MPX_DEBUG3( "CMPXViewUtilityImp::AddToDefaultViewHistoryL Inserting 0x%x to default view history at %d", result, aLoc );
        User::LeaveIfError( iDefaultHistoryPath.Insert( viewHistory, aLoc ) );
        }
    CleanupStack::Pop( viewHistory );
    return result;
    }

// ---------------------------------------------------------------------------
// Updates previous view info
// ---------------------------------------------------------------------------
//
void CMPXViewUtilityImp::UpdatePreviousViewInfoL( TInt aIndex )
    {
    MPX_FUNC( "CMPXViewUtilityImp::UpdatePreviousViewInfoL" );
    delete iPreviousView;
    iPreviousView = NULL;
    if ( aIndex != KErrNotFound )
        {
        TInt currentHistoryCount = iCurrentHistoryPath.Count();
        if ( ( currentHistoryCount > aIndex ) && ( aIndex >= 0 ) )
            {
            iPreviousView = CMPXViewHistory::NewL( iCurrentHistoryPath[ aIndex ] );
            iPreviousView->iPlugin = NULL;
            MPX_DEBUG2( "CMPXViewUtilityImp::UpdatePreviousViewInfoL previous view type 0x%x", iPreviousView->iPluginType.iUid );
            }
        }
    else
        {
        delete iPreviousView;
        iPreviousView = NULL;
        iPreviousView = CMPXViewHistory::NewL( iCurrentView );
        }
    }

// ---------------------------------------------------------------------------
// Find if the view exist in current history
// ---------------------------------------------------------------------------
//
TInt CMPXViewUtilityImp::FindCurrentHistoryEntryByTypeL( const TUid aType )
    {
    TInt count = iCurrentHistoryPath.Count();
    TInt index = KErrNotFound;
    for ( TInt i = 0; i < count && index == KErrNotFound; i++ )
        {
        if ( iCurrentHistoryPath[ i ]->iPluginType == aType )
            {
            index = i;
            }
        }
    return index;
    }

//  End of File
