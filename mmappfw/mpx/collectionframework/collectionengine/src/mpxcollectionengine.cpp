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
* Description:  Manages collection information for the clients
*
*/

#include <driveinfo.h>
#include <sysutil.h> 

#include <mpxprivatecrkeys.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionobserver.h>
#include <mpxclientlist.h>
#include <mpxcmn.h>
#include <mpxcollectiontype.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxpluginmonitor.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionplugin.hrh>
#include <mpxcollectionplugin.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxmessagecontainerdefs.h>
#include <mpxmessagepluginupdatedefs.h>
#include <mmf/common/mmfcontrollerpluginresolver.h>

#include "mpxcollectionpluginhandler.h"
#include "mpxcollectioncache.h"
#include "mpxcollectionengine.h"

// CONSTANTS
const TInt KMPXMaxCacheSizeRatio = 70;
// ============================ LOCAL FUNCTIONS ==============================

/**
* Container class for the plugin reference counting mechanism.
* Instances of this class are pushed onto the engine internal stack.
*/
class TReleaseInfo
    {
    public:
        TReleaseInfo(CMPXCollectionPluginHandler* aHandler,
            CMPXCollectionPlugin* aPlugin);

    public:
        // Not owned
        CMPXCollectionPluginHandler* iHandler;
        CMPXCollectionPlugin* iPlugin;
    };

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
TReleaseInfo::TReleaseInfo(
    CMPXCollectionPluginHandler* aHandler,
    CMPXCollectionPlugin* aPlugin) :
    iHandler(aHandler),
    iPlugin(aPlugin)
    {
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionEngine* CMPXCollectionEngine::NewL()
    {
    CMPXCollectionEngine* p=new(ELeave)CMPXCollectionEngine();
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionEngine::CMPXCollectionEngine()
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::ConstructL()
    {
    iMediaForHeapKeepAlive = CMPXMedia::NewL(); // To avoid continuous heap 
    iPluginHandler=CMPXCollectionPluginHandler::NewL(*this, *this);
    iCache = CMPXCollectionCache::NewL(KMPXMaxCacheSizeRatio);
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionEngine::~CMPXCollectionEngine()
    {
    delete iMediaForHeapKeepAlive;
    iCleanupStack.Close();

    iContexts.ResetAndDestroy();
    iContexts.Close();
    delete iPluginHandler;
    delete iCache;
    }

// ----------------------------------------------------------------------------
// Session Initialiser
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionClientContext& CMPXCollectionEngine::SessionInitL(
    const TUid& aModeId,
    TThreadId aClientId,
    CMPXMessageQueue* aMsgQueue)
    {
    CMPXCollectionClientContext* context(NULL);

    if (aModeId == KMcModeDefault)
        {
        // Connects the client to the collection path in its thread, or
        // creates one if it doesn't exist
        context = FindDefaultContext(aClientId);
        if (context)
            {
            MPX_DEBUG1("CMPXCollectionEngine add client into the default context");
            }
        else
            {
            // Creates a isolated collection path.
            context = CreateNewContextL(KMcModeIsolated);
            MPX_DEBUG1("CMPXCollectionEngine add client into the isolated context");
            }
        }
     else if (aModeId == KMcModeIsolated || aModeId == KMcModePlaylist)
        {
        // Creates a isolated collection path.
        context = CreateNewContextL(aModeId);
        MPX_DEBUG1("CMPXCollectionEngine add client into the isolated/playlist context");
        }
     else
        // Connects to the collection context of an application, or creates
        // it if it doesn't exist
        {
        for (TInt i=0;i<iContexts.Count();++i)
            {
            if (iContexts[i]->ModeId() == aModeId)
                {
                context = iContexts[i];
                break;
                }
            }

        if (!context)
            {
            // Create the application context
            context = CreateNewContextL(aModeId);
            }

        MPX_DEBUG1("CMPXCollectionEngine add client into the application context");
        }
    context->AddClientL(aClientId, aMsgQueue);
    MPX_DEBUG3("CMPXCollectionEngine::SessionInitL return context 0x%08x with mode 0x%08x",
               context, context->ModeId().iUid);
    return *context;
    }

// ----------------------------------------------------------------------------
// Notifies all contexts of an event
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionEngine::NotifyL( TMPXCollectionBroadCastMsg aMsg,
                                             TInt aData )
    {
    MPX_FUNC("CMPXCollectionEngine::NotifyL");

    TInt command = KErrNotSupported;
    TInt data = 0;
    TBool clearCache ( EFalse );
    TBool notify( ETrue );
    TInt count = iContexts.Count();
    CMPXCollectionClientContext* context( NULL );
    switch( aMsg )
        {
        case EMcMsgFormatStart:
        case EMcMsgDiskRemoved:
        case EMcMsgUSBMassStorageStart:
            {
            command = EMcCloseCollection;
            data = aData;

            // Clear the cache
            clearCache = ETrue;
            break;
            }
        case EMcMsgDiskInserted:        	
            {
            // inverting the notification order, this to get the message to 
            // the UI faster.
            for( TInt i=0; i<count; ++i )
                {
                context = iContexts[i];
                context->NotifyL( aMsg, aData );
                }
            notify = EFalse;
            command = EMcReOpenCollection;
            data = aData;
            // Clear the cache
        	  clearCache = ETrue;
            break;
            }
        case EMcMsgFormatEnd:
        case EMcMsgUSBMassStorageEnd:
            {
            command = EMcReOpenCollection;
            data = aData;
            
            // Clear the cache
        	  clearCache = ETrue;
            break;
            }
        case EMcMsgUSBMTPStart:
            {
            iRefreshing = ETrue;
            command = EMcCmdMtpStart;
            break;
            }
        case EMcMsgUSBMTPEnd:
            {
            iRefreshing = EFalse;
            command = EMcCmdMtpEnd;
            // Clear the cache
            clearCache = ETrue;
            break;
            }
        case EMcMsgRefreshStart:
            {
            iRefreshing = ETrue;
            command = EMcRefreshStarted;
            break;
            }
        case EMcMsgRefreshEnd:
            {
            iRefreshing = EFalse;
            command = EMcRefreshEnded;
            
            // Clear the cache
            clearCache = ETrue;
            break;
            }

        default:
            break;
        }
    if( command != KErrNotSupported )
        {
        Command( (TMPXCollectionCommand)command, data );
        }

    if( clearCache )
    	{
        RFs rfs;
        User::LeaveIfError( rfs.Connect() );        
        DriveInfo::TDriveArray driveArray;
        User::LeaveIfError ( DriveInfo::GetUserVisibleDrives( rfs, driveArray ) );
        TInt count( driveArray.Count () );
        TBool diskFull( EFalse );
        for ( TInt i = 0; i < count; ++i )
            {
            TRAP_IGNORE( diskFull = SysUtil::DiskSpaceBelowCriticalLevelL( &rfs, 0,
            		driveArray[i] ) );
            if ( diskFull )
                {
                MPX_DEBUG2("CMPXCollectionEngine::NotifyL: Disk full, driveArray[i] = %d ", driveArray[i] );
                break;
                }
            } 
        if( !diskFull )
        	{
        	iCache->Reset();
        	}
        rfs.Close();
    	}
    
    if ( notify )
    	{
		for( TInt i=0; i<count; ++i )
			{
			context = iContexts[i];
			context->NotifyL( aMsg, aData );
			}
    	}
    }
void CMPXCollectionEngine::Command( TMPXCollectionCommand aCmd, TInt aData )
    {
    TArray<CMPXCollectionPlugin*> plugins = iPluginHandler->LoadedPlugins();
    for( TInt i=0; i<plugins.Count(); ++i )
        {
        CMPXCollectionPlugin* plugin = plugins[i];
        TRAP_IGNORE(plugin->CommandL( aCmd, aData));
        }
    }

// ----------------------------------------------------------------------------
// Retrieves all of the supported types in the collection
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionEngine::GetSupportedTypesL(
                                    RPointerArray<CMPXCollectionType>& aArray )
    {
    CleanupResetAndDestroyPushL(aArray); 
    RArray<TUid> uids;
    CleanupClosePushL(uids);
    iPluginHandler->GetPluginUids(uids);

    for (TInt i=0; i<uids.Count();++i)
        {
        const TUid& uid = uids[i];
        CDesCArray* mimetypes =
                iPluginHandler->SupportedMimeTypesL(uid);
        CleanupStack::PushL(mimetypes);
        CDesCArray* extensions =
                iPluginHandler->SupportedExtensionsL(uid);
        CleanupStack::PushL(extensions);
        CMPXCollectionType* type = new(ELeave)CMPXCollectionType(
                                                            uid,
                                                            mimetypes,
                                                            extensions);
        CleanupStack::Pop(extensions);
        CleanupStack::Pop(mimetypes);
        CleanupStack::PushL(type);
        aArray.AppendL( type );
        CleanupStack::Pop(type);
        }
    CleanupStack::PopAndDestroy(&uids);
    CleanupStack::Pop(&aArray); 
    }

// ----------------------------------------------------------------------------
// Virtual collection ID to implementation ID lookup
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXCollectionEngine::LookupCollectionPluginID( const TUid aUid )
    {
    // Lookup virtual ID and map it to an implementation ID
    TUid realId = TUid::Uid(0);
    if( aUid == KLocalCollection )
        {
        realId = TUid::Uid(0x101FFC3A);
        }
    else if( aUid == KPodcastCollection )
        {
        realId = TUid::Uid(0x101FFC3C);
        }
    else if( aUid == KInMemoryCollection )
        {
        realId = TUid::Uid(0x101FFCD8);
        }
    else
        {
        realId=TUid::Null();
        }

    return realId;
    }

// ----------------------------------------------------------------------------
// Resolve a collection plugin UID
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXCollectionEngine::ResolvePluginUid(const TArray<TUid>& aAry)
    {
    // Resolve the plugin ID using the plugin handler
    //
    return iPluginHandler->FindPlugin( aAry );
    }

// ----------------------------------------------------------------------------
// Gets a list of the plugins available by name.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::ListPluginsL(
    CMPXMedia& aEntries,
    const TArray<TUid>& aSupportedUids)
    {
    MPX_FUNC("CMPXCollectionEngine::ListPluginsL");

#ifdef _DEBUG
    for (TInt ii=0; ii < aSupportedUids.Count(); ++ii)
        {
        MPX_DEBUG2("Supported uid 0x%08x", aSupportedUids[ii]);
        }
#endif
    RArray<TInt> supportedIds;
    CleanupClosePushL(supportedIds);
    supportedIds.AppendL(KMPXMediaIdGeneral);

    CMPXMediaArray* array=CMPXMediaArray::NewL();
    CleanupStack::PushL(array);

    RArray<TUid> pluginList;
    CleanupClosePushL(pluginList);
    iPluginHandler->GetPluginUids(pluginList);
    RArray<TInt> pluginTypes;
    CleanupClosePushL(pluginTypes);
    iPluginHandler->GetPluginTypes(pluginTypes);
    TInt count=pluginList.Count();
    MPX_ASSERT(pluginTypes.Count() >= count);
    TInt arrayCount(0);
    for(TInt i = 0;i < count ;i++)
        {
        // Show only visible plugins
        //
        if(pluginTypes[i] != EMPXCollectionPluginHidden &&
           iPluginHandler->SupportUids(pluginList[i], aSupportedUids))
            {
            CMPXMedia* entry=CMPXMedia::NewL(supportedIds.Array());
            CleanupStack::PushL(entry);
            entry->SetTextValueL(KMPXMediaGeneralTitle,
                                 iPluginHandler->PluginName(pluginList[i]));
            entry->SetTObjectValueL(KMPXMediaGeneralType, EMPXGroup);
            entry->SetTObjectValueL(KMPXMediaGeneralCategory,
                                    EMPXCollection);
            entry->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId,
                                                TMPXItemId(pluginList[i].iUid));
            array->AppendL(entry);
            CleanupStack::Pop(entry);
            ++arrayCount;
            }
        }
    aEntries.SetCObjectValueL(KMPXMediaArrayContents,array);
    aEntries.SetTObjectValueL(KMPXMediaArrayCount,arrayCount);
    CleanupStack::PopAndDestroy(&pluginTypes);
    CleanupStack::PopAndDestroy(&pluginList);
    CleanupStack::PopAndDestroy(array);
    CleanupStack::PopAndDestroy(&supportedIds);
    }

// ----------------------------------------------------------------------------
// Find a plugin from collection path
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionEngine::ResolvePluginL(
    const CMPXCollectionPath& aPath)
    {
    return ResolvePluginL(TUid::Uid(aPath.Id(CMPXCollectionPath::ECollectionUid)));
    }

// ----------------------------------------------------------------------------
// Find a plugin from collection uid
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionEngine::ResolvePluginL(
    const TUid& aUid)
    {
    iPluginHandler->SelectPluginL(aUid);
    return iPluginHandler->Plugin();
    }

// ----------------------------------------------------------------------------
// Find a plugin from uri
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionEngine::ResolvePluginL(
    const TDesC& aUri)
    {
    iPluginHandler->ClearSelectionCriteria();
    iPluginHandler->SelectPluginL(aUri, KNullDesC8);
    return iPluginHandler->Plugin();
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::UsePlugin
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::UsePlugin(
    const TUid& aPluginUid)
    {
    iPluginHandler->UsePlugin(aPluginUid);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::ReleasePlugin
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionEngine::ReleasePlugin(
    const TUid& aPluginUid )
    {
    MPX_FUNC("CMPXCollectionEngine::ReleasePlugin");
    TBool unloaded(iPluginHandler->ReleasePlugin(aPluginUid));
    if (unloaded)
        {
        HandlePluginUnloaded(aPluginUid);
        }
    return unloaded;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::CleanupPluginPushL
//
// Pushes the plugin release information onto the cleanup stack. Both the plugin
// handler and the plugin itself have to be pushed and therefore cannot use the
// cleanup stack alone. Even if it would create a dynamic TReleaseInfo instance
// and push that onto the cleanup stack, CleanupStack::Pop would not take care
// of deleting it and therefore a separate stack has to be used for the release
// info.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::CleanupPluginPushL(
    CMPXCollectionPlugin* aPlugin)
    {
    MPX_FUNC("CMPXCollectionEngine::CleanupPluginPushL");

    // add the release information to the internal stack
    iCleanupStack.AppendL(TReleaseInfo(iPluginHandler, aPlugin));

    // push a pointer to the engine onto the cleanup stack
    CleanupStack::PushL(TCleanupItem(ReleasePlugin, this));
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::PluginPop
//
// Has to be called instead of CleanupStack::Pop for cleaning up correctly
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::PluginPop()
    {
    MPX_FUNC("CMPXCollectionEngine::PluginPop");

    // remove from the cleanup stack
    CleanupStack::Pop(this);

    // remove from the internal stack
    iCleanupStack.Remove(iCleanupStack.Count() - 1);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::HandlePluginHandlerEvent
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::HandlePluginHandlerEvent(
    TPluginHandlerEvents aEvent,
    const TUid& aPluginUid,
    TBool aLoaded,
    TInt aData)
    {
    MPX_FUNC("CMPXCollectionEngine::HandlePluginHandlerEvent");

    switch (aEvent)
        {
        case MMPXPluginHandlerObserver::EPluginAdd:
            {
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginAdd, aPluginUid,
                aLoaded, aData));
            break;
            }
        case MMPXPluginHandlerObserver::EPluginUpdateStart:
            {
            // Handling the unloading of the previous plugin version and the loading
            // of the new plugin version is synchronous and therefore new requests
            // will not be processed by the server/engine in between EPluginUpdateStart
            // and EPluginUpdateEnd.
            //
            // If the plugin handler would unload/load plugins asynchronously then a
            // mechanism must be created where the engine does not accept requests for
            // the plugin that is being updated for the duration of the update.

            // Send update start message
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginUpdateStart, aPluginUid,
                aLoaded, aData));

            // Cancel all outstanding requests for the plugin
            CMPXCollectionPlugin* plugin = iPluginHandler->LoadedPlugin(aPluginUid);
            if (plugin)
                {
                plugin->CompleteAllTasks(KErrNotReady);
                }
            break;
            }
        case MMPXPluginHandlerObserver::EPluginUpdateEnd:
            {
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginUpdateEnd, aPluginUid,
                aLoaded, aData));
            break;
            }
        case MMPXPluginHandlerObserver::EPluginRemove:
            {
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginRemove, aPluginUid,
                aLoaded));

            // Cancel all outstanding requests for the plugin
            CMPXCollectionPlugin* plugin = iPluginHandler->LoadedPlugin(aPluginUid);
            if (plugin)
                {
                plugin->CompleteAllTasks(KErrNotReady);
                }
            break;
            }
        default:
            {
            // ignore the event
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPluginHandler::LoadedPlugin
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionEngine::LoadedPlugin(const TUid& aUid)
    {
    return iPluginHandler->LoadedPlugin(aUid);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::ReleasePlugin
//
// Executed when the cleanup stack item is destroyed as a result of
// CleanupStack::PopAndDestroy being called. This could either be called
// manually or as a result of a leave being generated and trapped. When this
// method is called the cleanup stack item is already removed from the cleanup
// stack and therefore it only has to take care of removing the local item.
//
// This is a static method.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::ReleasePlugin(
    TAny* aEngine)
    {
    MPX_FUNC("CMPXCollectionEngine::ReleasePlugin");

    // get the engine instance
    CMPXCollectionEngine* engine = static_cast<CMPXCollectionEngine*>(aEngine);

    // last pushed release info
    TReleaseInfo& relInfo = engine->iCleanupStack[engine->iCleanupStack.Count() - 1];

    if (relInfo.iPlugin)
        {
        // release the plugin
        TUid uid(relInfo.iPlugin->Uid());
        TBool unloaded(relInfo.iHandler->ReleasePlugin(uid));
        if (unloaded)
            {
            engine->HandlePluginUnloaded(uid);
            }
        }

    // remove the item from the local stack
    engine->iCleanupStack.Remove(engine->iCleanupStack.Count() - 1);
    }

// ----------------------------------------------------------------------------
// Find the cachable attribute of a plugin
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionEngine::PluginCacheable( const TUid& aUid )
    {
    return iPluginHandler->PluginCachable( aUid );
    }

// ----------------------------------------------------------------------------
// Find the non-cacaheable attributes of a plugin
// ----------------------------------------------------------------------------
//
const TArray<TUid> CMPXCollectionEngine::PluginNonCacheableAttributesL( const TUid& aUid )
    {
    return iPluginHandler->NonCacheableAttributesL( aUid );
    }


// ----------------------------------------------------------------------------
// Create a new client context object
// ----------------------------------------------------------------------------
//
CMPXCollectionClientContext* CMPXCollectionEngine::CreateNewContextL(
    const TUid& aModeId)
    {
    CMPXCollectionClientContext* context =
            CMPXCollectionClientContext::NewL(*this, *iCache, aModeId);
    CleanupStack::PushL(context);
    iContexts.AppendL(context);
    CleanupStack::Pop(context);
    return context;
    }

// ----------------------------------------------------------------------------
// Find a shareable client context object created in the same thread
// ----------------------------------------------------------------------------
//
CMPXCollectionClientContext* CMPXCollectionEngine::FindDefaultContext(
    TThreadId aClientTid)
    {
    CMPXCollectionClientContext* context(NULL);

    for (TInt i=0;i<iContexts.Count();++i)
        {
        if (iContexts[i]->HasShareableClient(aClientTid))
            {
            context = iContexts[i];
            break;
            }
        }
    return context;
    }

// ----------------------------------------------------------------------------
// Handle a change message
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::DoNotifyChangeL( const CMPXMessage& aMessage )
    {
    TInt msgGeneralId(0);
    if( aMessage.IsSupported(KMPXMessageGeneralId) )
        {
        msgGeneralId = aMessage.ValueTObjectL<TMPXMessageId>(KMPXMessageGeneralId);
        }

    // Only handle change messages to invalidate the collection cache
    //
    if( msgGeneralId == KMPXMessageIdItemChanged)
        {
        MPX_DEBUG1("CMPXCollectionEngine::DoNotifyChangeL -- KMPXMessageIdItemChanged");

        // Multiple messages
        //
        if( aMessage.IsSupported(KMPXMessageArrayContents) )
            {
            const CMPXMessageArray* messageArray =
                    aMessage.Value<CMPXMessageArray>(KMPXMessageArrayContents);
            User::LeaveIfNull(const_cast<CMPXMessageArray*>(messageArray));
            TInt count(messageArray->Count());
            for(TInt i=0; i<count; ++i )
                {
                TBool invalidated = DoNotifySingleChangeL(*(messageArray->AtL(i)));
                if( invalidated )
                    {
                    // Cache got cleared, no need to process other messages
                    break;
                    }
                }
            }
        // Single message
        //
        else
            {
            DoNotifySingleChangeL(aMessage);
            }
        }
    }

// ----------------------------------------------------------------------------
// Handle a single change message
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionEngine::DoNotifySingleChangeL( const CMPXMessage& aMessage )
    {
    TBool invalidated(EFalse);

    // Get the change message data
    //
    if( aMessage.IsSupported(KMPXMessageCollectionId) )
        {
        TUid collectionId( aMessage.ValueTObjectL<TUid>(KMPXMessageCollectionId) );

        if( PluginCacheable( collectionId ) )
            {
            TMPXChangeEventType changeType =
                aMessage.ValueTObjectL<TMPXChangeEventType>(KMPXMessageChangeEventType);

            // If item was inserted, we wipe out everything from that collection
            //
            if( changeType == EMPXItemInserted )
                {
                // Remove all cached nodes from a plugin
                //
                iCache->HandleChangeL( collectionId, TMPXItemId(collectionId.iUid) );
                invalidated = ETrue;
                }
            else if ( aMessage.IsSupported( KMPXMessageMediaDeprecatedId ) )
                {
                iCache->HandleChangeL( collectionId,
                      aMessage.ValueTObjectL<TMPXItemId>(KMPXMessageMediaDeprecatedId) );
                }
            else if ( aMessage.IsSupported( KMPXMessageMediaGeneralId ) )
                {
                iCache->HandleChangeL( collectionId,
                        aMessage.ValueTObjectL<TMPXItemId>(KMPXMessageMediaGeneralId) );
                }
            }
        }

    return invalidated;
    }

// ----------------------------------------------------------------------------
// Retrieves all of the supported types in the collection
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::RemoveContext(
    const CMPXCollectionClientContext& aContext)
    {
    MPX_DEBUG2("CMPXCollectionEngine::RemoveContext %08x", &aContext);
    TRAP_IGNORE(::DeleteL(&aContext, iContexts));
    }

// ----------------------------------------------------------------------------
// Notify collection changes
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::NotifyChange(
    const CMPXCollectionClientContext& aNotifier,
    CMPXMessage* aMessage,
    TInt aError)
    {
    MPX_FUNC("CMPXCollectionEngine::NotifyChange");
    for (TInt i=0; i<iContexts.Count(); ++i)
        {
        CMPXCollectionClientContext* context = iContexts[i];
        if (context != &aNotifier)
            {
            context->DoHandleMessage(aMessage, aError, EFalse);
            }
        }

    if( aMessage && !iRefreshing )
        {
        TRAP_IGNORE( DoNotifyChangeL( *aMessage ) );
        }
    }

// ----------------------------------------------------------------------------
// Remove tasks of a client which is the observer of tasks
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::RemoveTask(TAny* aCallback)
    {
    iPluginHandler->RemoveTask(aCallback);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::HandlePluginUnloaded
// Called when a plugin is unloaded. Could be used to cleanup other references
// to the plugin, like the generic cache.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::HandlePluginUnloaded(
    const TUid& aUid )
    {
    TRAP_IGNORE( DoHandlePluginUnloadedL( aUid ) );
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::DoHandlePluginUnloadedL
// Called when a plugin is unloaded. Could be used to cleanup other references
// to the plugin, like the generic cache.
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::DoHandlePluginUnloadedL(
    const TUid& aUid)
    {
    // Construct a plugin level path
    // This should remove all levels after this path
    //
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( aUid.iUid );
    iCache->RemoveL( *path );
    CleanupStack::PopAndDestroy( path );
    }

// ----------------------------------------------------------------------------
// CMPXCollectionEngine::SendPluginHandlerMessageL
// ----------------------------------------------------------------------------
//
void CMPXCollectionEngine::SendPluginHandlerMessageL(
    TInt aMessageId,
    const TUid& aPluginUid,
    TBool aLoaded,
    TInt aVersion /* = 0 */)
    {
    MPX_FUNC("CMPXCollectionEngine::SendPluginHandlerMessageL");
    MPX_DEBUG5("Message id 0x%08x, plugin id 0x%08x Loaded %d Version %d",
               aMessageId,aPluginUid.iUid,aLoaded,aVersion);
    CMPXMessage* msg = CMPXMedia::NewL();
    CleanupStack::PushL(msg);

    msg->SetTObjectValueL<TMPXMessageId>(KMPXMessageGeneralId, aMessageId);
    msg->SetTObjectValueL<TUid>(KMPXAttrPluginId, aPluginUid);
    msg->SetTObjectValueL<TBool>(KMPXAttrPluginLoaded, aLoaded);
    if (aVersion > 0)
        {
        msg->SetTObjectValueL<TInt>(KMPXAttrPluginVersion, aVersion);
        }

    HandleMessage(msg, KErrNone);

    CleanupStack::PopAndDestroy(msg);
    }

// ----------------------------------------------------------------------------
// Resets the content of the cache related to specified client context path
// ----------------------------------------------------------------------------
// 
void CMPXCollectionEngine::ResetCacheL(CMPXCollectionPath& aPath)
    {
    MPX_FUNC("CMPXCollectionEngine::ResetCache");
    CMPXCollectionPath* newPath = CMPXCollectionPath::NewL();
    CleanupStack::PushL(newPath);
    newPath->AppendL(aPath.Id(0).iId1);
    iCache->RemoveL(*newPath);
    CleanupStack::PopAndDestroy(newPath);
    }
// End of file
