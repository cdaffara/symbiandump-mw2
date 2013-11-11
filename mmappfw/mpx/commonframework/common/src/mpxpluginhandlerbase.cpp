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
*  CMPXPluginHandlerBase instantiates and owns a CMPXPluginMonitor to monitor
*  plugins for the designated plugin interface. When a plugin for that plugin
*  interface is added/removed, PluginsChangedL is called. This base plugin
*  handler provides a default implementation for PluginsChangedL which requests
*  child class to perform plugin resolution.
*
*  CMPXPluginHandlerBase also provides plugin selection plugin APIs. These
*  methods set up the selection criteria and then request child class to
*  resolve these selection criteria to a plugin via ResolvePluginL.
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
#include <mpxcmn.h>
#include <mpxplugininfo.h>
#include "mpxpluginhandlerobserver.h"
#include "mpxpluginhandlerbase.h"

// CONSTANTS

// ============================ LOCAL FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// Local function to cleanup an array
// ---------------------------------------------------------------------------
//
static void CleanupPluginInfoArray(
    TAny* item)
    {
    ((RPointerArray<CMPXPluginInfo>*)item)->ResetAndDestroy();
    ((RPointerArray<CMPXPluginInfo>*)item)->Close();
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPluginHandlerBase::CMPXPluginHandlerBase(
    TUid aInterfaceUid,
    TPluginSelection aSelection,
    TInt aSelectionType,
    MMPXPluginHandlerObserver& aObserver,
    CMPXPluginMonitor* aPluginMonitor/*=NULL*/)
:   iObserver(aObserver),
    iInterfaceUid(aInterfaceUid),
    iPluginMonitor(aPluginMonitor),
    iSelection(aSelection),
    iSelectedType(aSelectionType)
    {
    iOwnedPluginMonitor = iPluginMonitor ? EFalse : ETrue;
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::BaseConstructL()
    {
    CreatePluginListL();
    if (iOwnedPluginMonitor)
        {
        iPluginMonitor = CMPXPluginMonitor::NewL(iInterfaceUid);
        }
    iPluginMonitor->AddObserverL(*this);
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPluginHandlerBase::~CMPXPluginHandlerBase()
    {
    if ( iPluginMonitor )
        {
        TRAP_IGNORE( iPluginMonitor->RemoveObserverL( *this ) );
        }
    if (iOwnedPluginMonitor)
        {
        delete iPluginMonitor;
        }
    delete iDataType;
    delete iScheme;
    delete iExt;

    iPluginInfoArray.ResetAndDestroy();
    }
// ----------------------------------------------------------------------------
// Select this specific plugin
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPluginL(const TUid& aPluginUid)
    {
    MPX_DEBUG2("CMPXPluginHandlerBase::SelectPluginL(Uid 0x%08x)", aPluginUid.iUid);

    ClearSelectionCriteria();

    iSelection=ESelectionUid;
    iSelectedUid=aPluginUid;

    ResolvePluginL();
    MPX_DEBUG1("<--CMPXPluginHandlerBase::SelectPluginL() exits");
    }

// ----------------------------------------------------------------------------
// select the plugin with the specified name
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPluginL(const TDesC& aPluginName)
    {
    MPX_DEBUG2("CMPXPluginHandlerBase::SelectPluginL(PluginName %S)", &aPluginName);

    SelectPluginL( PluginUid( aPluginName ) );
    MPX_DEBUG1("<--CMPXPluginHandlerBase::SelectPluginL() exits");
    }

// ----------------------------------------------------------------------------
// Set up selection criteria, e.g. data type, extension, and scheme and then
// resolve plugin
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPluginL(
    const TDesC& aUri,
    const TDesC8& aDataType)
    {
    MPX_FUNC("CMPXPluginHandlerBase::PluginL");

    delete iScheme;
    iScheme=NULL;
    delete iExt;
    iExt=NULL;
    delete iDataType;
    iDataType=NULL;

    TPtrC fullpath;
    TUriParser up;
    TParse fp;
    //
    // Is it a valid URI, or even full path (e.g. c:\\data\test.mp3")
    //
    if (up.Parse(aUri)==KErrNone)
        {
        const TDesC& uriScheme=up.Extract(EUriScheme);
        //
        // Could be just drive letter if path
        // which isn't really a scheme
        //
        if (uriScheme.Length()>1)
             {
             iScheme=MPXUser::Alloc8L(uriScheme);
             }
         else if (fp.Set(aUri,NULL,NULL)==KErrNone) // Must be a full path
             {
             fullpath.Set(fp.FullName());
             }
         else
             {
             User::Leave(KErrArgument);
             }
        //
        // Get the extension
        //
        iExt=MPXUser::Alloc8L(fp.Ext());
        }
    if (aDataType.Length())
        {
        iDataType=aDataType.AllocL();
        }
    else if (fullpath.Length())
        {
        RApaLsSession aps;
        TInt error  = aps.Connect(); // always fail in console test
        if (KErrNone == error)
            {
            CleanupClosePushL(aps);
            TDataType dataType;
            TUid ignore;
            if(aps.AppForDocument(fullpath,ignore,dataType)==KErrNone)
                {
                iDataType=dataType.Des8().AllocL();
                }
            CleanupStack::PopAndDestroy(&aps);
            } // else APPARC is not working in Console environment,
              // Always find plugin by scheme & etension
        }
    else
        {
        if ( !iScheme ) // last valid case
            {
            User::Leave(KErrArgument);
            }
        }

    ResolvePluginL();
    }

// ----------------------------------------------------------------------------
// Select a plugin with the specific type
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPluginL(TInt aPluginType)
    {
    MPX_DEBUG2("CMPXPluginHandlerBase::SelectPluginL(PluginType %x)", aPluginType);

    ClearSelectionCriteria();
    iSelectedType=aPluginType;
    ResolvePluginL();
    MPX_DEBUG1("<--CMPXPluginHandlerBase::SelectPluginL() exits");
    }

// ----------------------------------------------------------------------------
// select plug-in appropriate for a file
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPluginL(const RFile& aFile)
    {
    MPX_FUNC("CMPXPluginHandlerBase::SelectPluginL(RFile)");

    TFileName fn;
    User::LeaveIfError(aFile.FullName(fn));
    SelectPluginL(fn, KNullDesC8);
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::UsePlugin
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::UsePlugin(
    const TUid& aPluginUid)
    {
    MPX_FUNC_EX("CMPXPluginHandlerBase::UsePlugin");

    TInt index(IndexOf(aPluginUid));
    if (index != KErrNotFound)
        {
        iPluginInfoArray[index]->ReferenceCount()++;
        }
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::ReleasePlugin
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXPluginHandlerBase::ReleasePlugin(
    const TUid& aPluginUid)
    {
    MPX_FUNC_EX("CMPXPluginHandlerBase::ReleasePlugin");

    TBool unload(EFalse);
    TInt index(IndexOf(aPluginUid));
    if (index != KErrNotFound)
        {
        TInt& refCount = iPluginInfoArray[index]->ReferenceCount();
        refCount--;

        if (!refCount)
            {
            UnloadPlugin(aPluginUid);
            unload = ETrue;
            }
        }

    return unload;
    }

// ----------------------------------------------------------------------------
// Get current selection criteria
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::GetSelectionCriteria(
    TPluginSelection& aPluginSelection,
    TInt& aPluginType,
    TUid& aPluginUid,
    TPtrC8& aDataType,
    TPtrC8& aScheme,
    TPtrC8& aExtension)
    {
    aPluginSelection=iSelection;
    aPluginType=iSelectedType;
    aPluginUid=iSelectedUid;

    aDataType.Set(*iDataType);
    aScheme.Set(*iScheme);
    aExtension.Set(*iExt);
    }

// ----------------------------------------------------------------------------
// Clear current selection criteria
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::ClearSelectionCriteria()
    {
    iSelection=ESelectionType;
    iSelectedType=0; // unknown type. this info is unknown
    iSelectedUid=KNullUid;

    delete iScheme;
    iScheme=NULL;
    delete iExt;
    iExt=NULL;
    delete iDataType;
    iDataType=NULL;
    }

// ----------------------------------------------------------------------------
// Resolve a plugin, based on properties (iDataType, iExt and iScheme)
// and selection criteria. If selection is by type, then there is always a
// plug-in resolved (if there are any of that type).
// TO-DO: Room for optimisation
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::DoResolvePluginL(
    TUid& aPluginUid,
    TInt& aIndex,
    TPtrC& aDisplayName,
    TInt& aPluginType)
    {
    MPX_DEBUG2("===>CMPXPluginHandlerBase::DoResolvePluginL 0x%08x", this);
    TInt index=KErrNotFound;
    if (iSelection==ESelectionUid)
        {
        index=IndexOf(iSelectedUid);
        }
    else // if (iSelection==ESelectionType)
        {
        TInt score=0;
        TInt maxScore=0;

        TInt count(iPluginInfoArray.Count());
        for(TInt i = 0; i < count; ++i)
            {
            // Must scan from beginning of plugin info array, because plugins
            // sorted by priority descendingly
            score=0;

            //
            // First, try scheme then data type, then extension
            // and total up the points
            //
            HBufC* hbuf = NULL;
            TInt pos(0);
            const CDesCArray& schemas = iPluginInfoArray[i]->SupportedSchemas();
            if (schemas.MdcaCount() && iScheme)
                {
                if (iScheme->Length())
                    {
                    hbuf = MPXUser::AllocL(*iScheme);
                    if (!schemas.FindIsq(*hbuf, pos))
                        { // schema supported by the plugin
                        score+=4;
                        }
                    delete hbuf;
                    hbuf=NULL;
                    }
                } // else schema not supported

            if (iDataType)
                {
                hbuf = MPXUser::AllocL(*iDataType);
                if (!iPluginInfoArray[i]->SupportedMimeTypes().FindIsq(*hbuf, pos))
                    { // mime type supported by the plugin
                    score+=3;
                    }
                delete hbuf;
                hbuf=NULL;
                }

            if (iExt)
                {
                hbuf = MPXUser::AllocL(*iExt);
                if (!iPluginInfoArray[i]->SupportedExtensions().FindIsq(*hbuf, pos))
                    { // extension supported by the plugin
                    score+=2;
                    }
                delete hbuf;
                hbuf=NULL;
                }

            if (iSelectedType == iPluginInfoArray[i]->PluginType().iUid)
                {
                score+=1; // Just for being the right "type"
                }

            if (score > maxScore)
                {
                maxScore=score;
                index=i;
                }
            }
        }

    if (index==KErrNotFound)
        {
        User::Leave(KErrNotSupported);
        }

    //
    // set return values
    //
    aPluginUid = iPluginInfoArray[index]->ImplementationUid();
    aIndex = index;
    aDisplayName.Set(iPluginInfoArray[index]->DisplayName());
    aPluginType = iPluginInfoArray[index]->PluginType().iUid;

#ifdef _DEBUG
    PrintPluginInfo();
#endif

    MPX_DEBUG5("<===CMPXPluginHandlerBase::DoResolvePluginL 0x%08x (aPluginUid 0x%08x, aIndex %d, aDisplayName %S)",
               this, aPluginUid.iUid, aIndex, &aDisplayName);
    }

// ----------------------------------------------------------------------------
// Handles request completion event
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::PluginsChangedL()
    {
    MPX_DEBUG2("===> CMPXPluginHandlerBase::PluginsChangedL 0x%08x", this);
    CreatePluginListL();
    ResolvePluginL();
    MPX_DEBUG2("<=== CMPXPluginHandlerBase::PluginsChangedL 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Handles a plugin unload request
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::HandlePluginUnload(
    const TUid& aPluginUid)
    {
    UnloadPlugin(aPluginUid);
    }

// ----------------------------------------------------------------------------
// Resolve a plugin
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::ResolvePluginL()
    {
    // Do nothing in base class
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::IsPluginLoaded
// This method had to be made virtual (and not pure virtual) because
// the playlist engine instantiates the base class.
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXPluginHandlerBase::IsPluginLoaded(
    const TUid& /* aPluginUid */)
    {
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::LoadPluginL
// This method had to be made virtual (and not pure virtual) because
// the playlist engine instantiates the base class.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::LoadPluginL(
    const TUid& /* aPluginUid */)
    {
    // do nothing
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::UnloadPlugin
// This method had to be made virtual (and not pure virtual) because
// the playlist engine instantiates the base class.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::UnloadPlugin(
    const TUid& /* aPluginUid */)
    {
    // do nothing
    }

// ----------------------------------------------------------------------------
// Return a new instance of the plugin info class
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPluginInfo* CMPXPluginHandlerBase::ConstructPluginInfoLC(
                                     const CImplementationInformation& aData  )
    {
    return CMPXPluginInfo::NewLC( aData );
    }


// ----------------------------------------------------------------------------
// Return plugin name which supports the specified type
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMPXPluginHandlerBase::PluginName(
    TInt aPluginType) const
    {
    const TDesC* name=&KNullDesC;
    for ( TInt i=iPluginInfoArray.Count(); --i >= 0 ; )
        {
        if ( aPluginType == iPluginInfoArray[i]->PluginType().iUid )
            {
            name = &(iPluginInfoArray[i]->DisplayName());
            break;
            }
        }
    return *name;
    }

// ----------------------------------------------------------------------------
// Return plugin name which supports the specified uid
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMPXPluginHandlerBase::PluginName(const TUid& aUid) const
    {
    const TDesC* name;
    TInt index(IndexOf(aUid));
    MPX_ASSERT(KErrNotFound!=index && index<iPluginInfoArray.Count());
    name = &iPluginInfoArray[index]->DisplayName();
    return *name;
    }

// ----------------------------------------------------------------------------
// Return a list of plugin names
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::PluginNamesL()
    {
    CDesCArray* desArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    CleanupStack::PushL(desArray);
    for(TInt i=iPluginInfoArray.Count(); --i>=0 ;)
        {
        HBufC* item = iPluginInfoArray[i]->DisplayName().AllocLC();

        //ignore leave when insert the same item
        TRAP_IGNORE(desArray->InsertIsqL(*item));
        CleanupStack::PopAndDestroy(item);
        }
    CleanupStack::Pop(desArray);
    return desArray;
    }

// ----------------------------------------------------------------------------
// returns a list of plugin types
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::GetPluginTypes(RArray<TInt>& aTypes) const
    {
    aTypes.Reset();
    TInt count(iPluginInfoArray.Count());
    for(TInt i=0; i<count; ++i)
        {
        aTypes.Append(iPluginInfoArray[i]->PluginType().iUid);
        }
    }

// ----------------------------------------------------------------------------
// Return the list of UIDs of all plugins
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::GetPluginUids(RArray<TUid>& aUids) const
    {
    aUids.Reset();
    TInt count(iPluginInfoArray.Count());
    for(TInt i=0; i<count; ++i)
        {
        aUids.Append(iPluginInfoArray[i]->ImplementationUid());
        }
    }

// ----------------------------------------------------------------------------
// Get plugin list with the specific type
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::GetPluginUidsL(
    RArray<TUid>& aPlugins,
    TInt aPluginType) const
    {
    aPlugins.Reset();
    for(TInt i=iPluginInfoArray.Count(); --i>=0 ;)
        {
        if (aPluginType == iPluginInfoArray[i]->PluginType().iUid)
            {
            aPlugins.Append(iPluginInfoArray[i]->ImplementationUid());
            }
        }
    }

// ----------------------------------------------------------------------------
// Get plugin UID with the specified display name
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXPluginHandlerBase::PluginUid(const TDesC& aPluginName) const
    {
    TUid uid( KNullUid );

    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        if ( aPluginName.Compare(iPluginInfoArray[i]->DisplayName()) == 0 )
            {
            uid = iPluginInfoArray[i]->ImplementationUid();
            break;
            }
        }

    return uid;
    }

// ----------------------------------------------------------------------------
// returns the index of the specified plugin
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPluginHandlerBase::IndexOf(const TUid& aPluginUid) const
    {
    TInt ret(KErrNotFound);
    TInt count(iPluginInfoArray.Count());
    for(TInt i=0; i<count; ++i)
        {
        if (iPluginInfoArray[i]->ImplementationUid()==aPluginUid)
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Return a list of mime types supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::SupportedMimeTypesL()
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        if( iPluginInfoArray[i] )
        	{
			MPXUser::MergeArray(iPluginInfoArray[i]->SupportedMimeTypes(),
				                *descArray);
        	}
        }
    return descArray;
    }

// ----------------------------------------------------------------------------
// Return a list of extensions supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::SupportedExtensionsL()
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        MPXUser::MergeArray(iPluginInfoArray[i]->SupportedExtensions(),
                            *descArray);
        }
    return descArray;
    }

// ----------------------------------------------------------------------------
// Get plugin type for a given implementation UID
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXPluginHandlerBase::PluginType( const TUid& aUid ) const
    {
    TUid ret( KNullUid );
    TInt count( iPluginInfoArray.Count() );
    for( TInt i = 0; i < count; ++i )
        {
        if ( iPluginInfoArray[i]->ImplementationUid() == aUid )
            {
            ret = iPluginInfoArray[i]->PluginType();
            break;
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Get supported app uid for a given implementation UID
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXPluginHandlerBase::SupportedAppUid( const TUid& aUid ) const
    {
    TUid ret( KNullUid );
    TInt count( iPluginInfoArray.Count() );
    for( TInt i = 0; i < count; ++i )
        {
        if ( iPluginInfoArray[i]->ImplementationUid() == aUid )
            {
            ret = iPluginInfoArray[i]->SupportedAppUid();
            MPX_DEBUG2("SupportedAppUid = %d", ret);
            break;
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Get plugin type for a given implementation UID
// ----------------------------------------------------------------------------
//
EXPORT_C TUint CMPXPluginHandlerBase::PluginFlagsL( const TUid& aUid ) const
    {
    TUint ret( 0 );
    TInt count( iPluginInfoArray.Count() );
    TBool found( EFalse );
    for( TInt i = 0; !found && i < count; ++i )
        {
        if ( iPluginInfoArray[i]->ImplementationUid() == aUid )
            {
            ret = iPluginInfoArray[i]->Flags();
            found = ETrue;
            }
        }
    if ( !found )
        {
        User::Leave( KErrNotFound );
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Return a list of schemas supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::SupportedSchemasL()
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        MPXUser::MergeArray(iPluginInfoArray[i]->SupportedSchemas(),
                            *descArray);
        }
    return descArray;
    }

// ----------------------------------------------------------------------------
// Return a list of mime types supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::SupportedMimeTypesL(const TUid& aUid)
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    CleanupStack::PushL(descArray);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        if (aUid == iPluginInfoArray[i]->ImplementationUid())
            {
            MPXUser::CopyArrayL(iPluginInfoArray[i]->SupportedMimeTypes(),
                            *descArray);
            break;
            }
        }
    CleanupStack::Pop(descArray);
    return descArray;
    }

// ----------------------------------------------------------------------------
// Return a list of extensions supported by the plugins
// ----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CMPXPluginHandlerBase::SupportedExtensionsL(const TUid& aUid)
    {
    CDesCArray* descArray = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    CleanupStack::PushL(descArray);
    for(TInt i=iPluginInfoArray.Count();--i>=0;)
        {
        if (aUid == iPluginInfoArray[i]->ImplementationUid())
            {
            MPXUser::CopyArrayL(iPluginInfoArray[i]->SupportedExtensions(),
                            *descArray);
            break;
            }
        }
    CleanupStack::Pop(descArray);
    return descArray;
    }

// ----------------------------------------------------------------------------
// Recreates the list of plugins. Handles the scenarios of plugin update,
// addition and removal by comparing the previous list with the current one.
// In case any change is detected to the plugin list the observer interface is
// used to notify the owner.
//
// NOTE: ECOM takes care of versioning for us. When two plugins that implement
// the same interface and have the same implementation UID are detected, it loads
// the one with the higher version. There is no way to have two implementations
// of the same interface that share the implementation UID returned by
// REComSession::ListImplementationsL.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::CreatePluginListL()
    {
    MPX_DEBUG_THREAD("CMPXPluginHandlerBase::CreatePluginListL");
    // Get the current list of plugins that implement the interface
    // The ECOM versioning mechanism is appied here.
    // Auto destroy array, no need to call ResetAndDestroy and Close
    RMPXPointerArray<CImplementationInformation> implInfoArray;
    REComSession::ListImplementationsL(iInterfaceUid, implInfoArray);

    // Temporary array to store the new plugin infos.
    RPointerArray<CMPXPluginInfo> tempInfoArray;
    CleanupStack::PushL(TCleanupItem(CleanupPluginInfoArray, &tempInfoArray));

    // Reused variables
    CMPXPluginInfo* prevInfo(NULL);
    TBool loaded(EFalse);

    // Iterate the new plugins
    // It is expected that when the loop ends the temporary array will contain
    // all current plugins and iPluginInfoArray will contain the plugins that
    // have been removed
    TInt count(implInfoArray.Count());
    MPX_DEBUG3("CMPXPluginHandlerBase::CreatePluginListL numbers of the plugin 0x%08x %d",
                this, count);
    TInt index(0);
    CMPXPluginInfo* info(NULL);
    for (; index < count; ++index)
        {
        // Convert to an internal plugin info instance
        info = ConstructPluginInfoLC(*implInfoArray[index]);
        const TUid& uid = info->ImplementationUid();

        // Check if the plugin existed in the previous list
        TInt prevIndex = IndexOf(uid);
        if (prevIndex != KErrNotFound)
            {
            // The plugin existed before the update
            prevInfo = iPluginInfoArray[prevIndex];
            MPX_DEBUG5("Process the existing plugin 0x%08x uid 0x%08x, old version %d new version %d",
                       this, uid.iUid, prevInfo->Version(), info->Version());
            // Copy the reference count
            info->ReferenceCount() = prevInfo->ReferenceCount();

            // Compare the versions
            if (prevInfo->Version() < info->Version())
                {
                // The plugin is being upgraded
                loaded = IsPluginLoaded(uid);

                // Generate an update start notification
                iObserver.HandlePluginHandlerEvent(
                    MMPXPluginHandlerObserver::EPluginUpdateStart, uid, loaded,
                    info->Version());

                // Check if this plugin was loaded. The plugin is still loaded if this is the first
                // handler that got notified or it is already unloaded for any otehr handlers.
                if (info->ReferenceCount())
                    {
                    if (loaded)
                        {
                        // Unload the old plugin version
                        MPX_DEBUG2("CMPXPluginHandlerBase::CreatePluginListL unload old plugin 0x%08x", this);

                        // The plugin is loaded - ask the monitor to tell all
                        // handler instances (including this one) to unload it.
                        // After the call below all plugin instances owned by
                        // all monitors are unloaded.
                        //
                        // Only make this call if the plugin is still loaded,
                        // i.e. from the first handler that detects the update.
                        iPluginMonitor->NotifyUnload(uid);
                        
                        // Necessary to ensure version upgrade of ECom plugins
                        // is successful. This call will unload any garbage
                        // plugins. 
                        REComSession::FinalClose();                       
                        }

                    MPX_DEBUG2("CMPXPluginHandlerBase::CreatePluginListL load new plugin 0x%08x", this);
                    // Load the new plugin version
                    LoadPluginL(uid);
                    MPX_DEBUG2("CMPXPluginHandlerBase::CreatePluginListL load new plugin finished 0x%08x", this);
                    }

                // Generate an update end notification
                iObserver.HandlePluginHandlerEvent(
                    MMPXPluginHandlerObserver::EPluginUpdateEnd, uid, loaded,
                    info->Version());
                }

            // Delete the entry from the info array
            iPluginInfoArray.Remove(prevIndex);
            delete prevInfo;
            prevInfo = NULL;
            }
        else
            {
            MPX_DEBUG2("CMPXPluginHandlerBase::CreatePluginListL found new plugin 0x%08x", this);
            // A new plugin is being added
            iObserver.HandlePluginHandlerEvent(
                MMPXPluginHandlerObserver::EPluginAdd, uid, EFalse, info->Version());
            }

        // Insert into the temporary array - order by priority, high priority first
        tempInfoArray.InsertInOrderAllowRepeatsL(info,
                                    CMPXPluginInfo::ComparePluginInfoByPriority);

        CleanupStack::Pop(info);
        }

    // What is left in iPluginInfoArray are removed plugins
    while (iPluginInfoArray.Count())
        {
        prevInfo = iPluginInfoArray[0];
        const TUid& uid = prevInfo->ImplementationUid();

        loaded = IsPluginLoaded(uid);
        MPX_DEBUG3("CMPXPluginHandlerBase::CreatePluginListL plugin removed 0x%08x uid  0x%08x",
                   this, uid.iUid);
        // Generate an remove notification
        iObserver.HandlePluginHandlerEvent(
            MMPXPluginHandlerObserver::EPluginRemove, uid, loaded,
            prevInfo->Version());

        // Check if the plugin is currently loaded
        if (loaded)
            {
            // Unload the plugin
            UnloadPlugin(uid);
            }

        iPluginInfoArray.Remove(0);
        delete prevInfo;
        prevInfo = NULL;
        }

    // iPluginInfoArray should be empty and the temp array should contain all new plugins
    // Copy the pointers to iPluginInfoArray - the ownership is transferred
    for (index = 0; index < count; ++index)
        {
        info = tempInfoArray[0];
        iPluginInfoArray.AppendL(info);
        tempInfoArray.Remove(0);
        // check if this plugin should be pre-loaded and not loaded yet
        if (info->Flags() & EMPXPluginFlagPreLoad && info->ReferenceCount() == 0)
            {
            // pre-load the plugin - have to go through SelectPluginL
            // to apply the reference counting consistently
            TRAP_IGNORE(SelectPluginL(info->ImplementationUid()));
            }
        }

    // Reset the temp array - this does not delete the plugin info instances pointed to
    // from the array - these were copied to iPluginInfoArray
    CleanupStack::Pop(); // TCleanupItem(CleanupPluginInfoArray, &tempInfoArray)
    tempInfoArray.Close();
    }

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::SupportUids
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXPluginHandlerBase::SupportUids(
    const TUid& aPluginId,
    const TArray<TUid>& aUids) const
    {
    TInt index(IndexOf(aPluginId));
    MPX_ASSERT(index!=KErrNotFound && index < iPluginInfoArray.Count());
    return iPluginInfoArray[index]->SupportUids(aUids);
    }

// ----------------------------------------------------------------------------
// Search a plugin by matching implementation id, supported id and plugin type
// If two plugins get the same score, high priority plugin's implementation
// uid will be returned
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXPluginHandlerBase::FindPlugin(const TArray<TUid>& aUids) const
    {
	MPX_DEBUG1("#### MPX Plugins ####");
    TInt count(iPluginInfoArray.Count());
    for (TInt index = 0; index < count; ++index)
        {
        CMPXPluginInfo* info = iPluginInfoArray[index];
        const TDesC& name = info->DisplayName();
        MPX_DEBUG5("%S [0x%08X] Priority: %d Ref Count: %d", &name, info->ImplementationUid(),
            info->Priority(), info->ReferenceCount());
        MPX_DEBUG2("Version %d", info->Version());
        }

    TInt lastScore( 0 );
    CMPXPluginInfo* mostPossiblePluginInfo( NULL );
    TUid uid = TUid::Null();

    // Find the most suitable plugin
    for ( TInt i = iPluginInfoArray.Count(); --i >= 0; )
        {
        TInt score( 0 );
        CMPXPluginInfo* info = iPluginInfoArray[i];
        const TArray<TUid> supportedUids = info->SupportedUids();

        for ( TInt j = aUids.Count(); --j >= 0; )
            {
            for (TInt k=supportedUids.Count(); --k >=0;)
                {
                if ( aUids[j] ==  supportedUids[k] )
                   {
                   // Five scores if in the supported uids list.
                   score += 5;

                   // We now allow a UID to be repeated multiple times
                   // for emphasized scoring.
                   //break;
                   }
                }
            if (info->ImplementationUid() == aUids[j])
                {
                score += 100;
                }
            if ( info->PluginType() == aUids[j])
                { // One more if match plugin type
                score++;
                }
            }

        if ( score )
            {
            if ( lastScore < score )
                {
                lastScore = score;
                mostPossiblePluginInfo = info;
                }
            else if ( lastScore == score )
                {
                // Use priority to determine if scores are the same
                // If same priorities are detected, the first found plugin
                // will be used.
                if (mostPossiblePluginInfo->Priority() < info->Priority())
                    {
                    mostPossiblePluginInfo = info;
                    }
                } // else do nothing
            }
        }
    if (mostPossiblePluginInfo)
        {
        uid =  mostPossiblePluginInfo->ImplementationUid();
        }
    return uid;
    }

#ifdef _DEBUG

// ----------------------------------------------------------------------------
// CMPXPluginHandlerBase::PrintPluginInfo
// ----------------------------------------------------------------------------
//
void CMPXPluginHandlerBase::PrintPluginInfo()
    {
    MPX_DEBUG1("#### MPX Plugins ####");

    TInt count(iPluginInfoArray.Count());
    for (TInt index = 0; index < count; ++index)
        {
        CMPXPluginInfo* info = iPluginInfoArray[index];
        const TDesC& name = info->DisplayName();
        MPX_DEBUG5("%S [0x%08X] Priority: %d Ref Count: %d", &name, info->ImplementationUid(),
            info->Priority(), info->ReferenceCount());
        MPX_DEBUG2("Version %d", info->Version());
        }
    }

#endif

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
// ----------------------------------------------------------------------------
// select plug-in appropriate for a file
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginHandlerBase::SelectPlugin64L(const RFile64& aFile)
    {
    MPX_FUNC("CMPXPluginHandlerBase::SelectPlugin64L(RFile64)");

    TFileName fn;
    User::LeaveIfError(aFile.FullName(fn));
    SelectPluginL(fn, KNullDesC8);
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

// End of file
