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
* Description:  Maintains sessions to paths
*
*/

#include <badesca.h>
#include <mpxcollectionpath.h>
#include <mpxclientlist.h>
#include <mpxcollectionplugin.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxcmn.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxmessagecontainerdefs.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectionopenlresultdef.h>
#include <mpxcommand.h>
#include <mpxlog.h>
#include <mpxsubscription.h>
#include "mpxcollectionengineobserver.h"
#include "mpxcollectionpluginhandler.h"
#include "mpxcollectionengine.h"
#include "mpxcollectioncache.h"
#include "mpxcollectionclientcontext.h"

// ----------------------------------------------------------------------------
// Helper. Sets next open mode on path
// ----------------------------------------------------------------------------
//
LOCAL_C void SetPathOpenMode(CMPXCollectionPath& aPath, TInt aMode)
    {
    TMPXOpenMode mode=static_cast<TMPXOpenMode>(aMode);
    if (aPath.Levels())
        {
        aPath.Set(mode == EMPXOpenDefault ? aPath.OpenPreviousMode() : mode);
        }
    }

// ----------------------------------------------------------------------------
// Helper. Returns whether next open mode is the same as previous open
// request
// ----------------------------------------------------------------------------
//
LOCAL_C TBool OpenModeValid(const CMPXCollectionPath& aPath)
    {
    TMPXOpenMode next=aPath.OpenNextMode();
    return (aPath.OpenPreviousMode() == next) || (next == EMPXOpenDefault);
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionClientContext* CMPXCollectionClientContext::NewL(
    CMPXCollectionEngine& aEngine,
    CMPXCollectionCache& aCache,
    const TUid& aModeId)
    {
    CMPXCollectionClientContext* p =
                    new(ELeave) CMPXCollectionClientContext(aEngine, aCache, aModeId);
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// C++ constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionClientContext::CMPXCollectionClientContext(
    CMPXCollectionEngine& aEngine,
    CMPXCollectionCache& aCache,
    const TUid& aModeId)
  : iModeId(aModeId),iFocusItemId(KMPXInvalidItemId),
    iEngine(aEngine),iCache(aCache)
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ConstructL()
    {
    iClientList = CMPXClientList::NewL();
    iBrowsePath = CMPXCollectionPath::NewL();
    iMedia = CMPXMedia::NewL(); // empty media
    // initialize to null uid
    for (TInt index = 0; index < EContextCount; ++index)
        {
        iPluginUids.AppendL(KNullUid);
        }
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXCollectionClientContext::~CMPXCollectionClientContext()
    {
    MPX_DEBUG2("CMPXCollectionClientContext::~CMPXCollectionClientContext %08x",
               this);

    // Release all plugins this context has references on, this could result in the
    // plugin(s) being unloaded.
    for (TInt index = 0; index < EContextCount; ++index)
        {
        if (iPluginUids[index] != KNullUid)
            {
            iEngine.ReleasePlugin(iPluginUids[index]);
            }
        }

    delete iClientList;
    delete iBrowsePath;
    delete iMediaPath;
    delete iMedia;
    delete iFilter;
    iPluginUids.Close();
    iUids.Close();
    }

// ----------------------------------------------------------------------------
// Open collection by path
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::OpenL(
    CMPXCollectionPath* aPath,
    TInt aMode,
    MMPXCollectionEngineObserver* aCallback)
    {
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 1 <--");
    MPX_ASSERT(aPath);
    if (aPath->Levels() > 0)
        {
        // Increment the plugin reference count
        CMPXCollectionPlugin*plugin = iEngine.ResolvePluginL(*aPath);
        if (!plugin)
            {
            MPX_DEBUG1("CMPXCollectionClientContext::OpenL 1 bad path plugin");
            User::Leave(KErrArgument);
            }

        iEngine.CleanupPluginPushL(plugin);

        // Ownership of aPath transferred
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL 1 Add task");
        plugin->AddTaskL(EMcsOpenPath, aCallback, this,
                        aMode, NULL, plugin, aPath);

        // Pop the plugin
        iEngine.PluginPop();
        }
    else
        { // Go back to root level
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL restarting path");
        iIndex=0;
        iFocusItemId = KMPXInvalidItemId;
        InitL(aCallback);
        }
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 1 -->");
    }

// ----------------------------------------------------------------------------
// Open collection by index
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::OpenL(
    TInt aIndex,
    TInt aMode,
    const TArray<TMPXAttribute>& aAttrs,
    MMPXCollectionEngineObserver* aCallback)
    {
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 2 <--");
    if (iBrowsePath->Levels()<=0)
        {
        User::Leave(KErrNotReady);
        }
    if (aIndex<0 || aIndex>=iBrowsePath->Count())
        {
        User::Leave(KErrArgument);
        }
    iBrowsePath->Set(aIndex);
    // plugin not resolved yet
    if (iPluginUids[EContextBrowse] == KNullUid)
        {
        // Increment the new plugin reference count, decrement the old.
        // No need to push/release the plugin it is stored in a member
        // variable and will be released when that is overwritten.
        ResolvePluginL(*iBrowsePath, iPluginUids[EContextBrowse]);
        }

    if (iPluginUids[EContextBrowse] == KNullUid)
        {
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL 2 bad plugin");
        User::Leave(KErrNotReady);
        }

    // add request to the queue
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 2 Add task");
    iBrowsePath->SetL(aAttrs);
    LoadedPlugin(EContextBrowse)->AddTaskL(EMcsOpenIndex, aCallback, this,
                                           aIndex, NULL, (TAny*)aMode);
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 2 -->");
    }

// ----------------------------------------------------------------------------
// Open collection by uids
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::OpenL(
    const TArray<TUid>& aUids,
    TInt aMode,
    MMPXCollectionEngineObserver* aCallback)
    {
    // Update path for open request
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 3 <---");
    SetPathOpenMode(*iBrowsePath,aMode);
    if (!MPXUser::CompareOrderedUidArrays(iUids.Array(), aUids))
        { // the same context
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL 3 return Open");
        aCallback->HandleOpen(iMedia, iIndex, ETrue, iMediaType);
        }
    else
        { // go back to root level
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL 3 Going to Root");
        iUids.Reset();
        ::CopyArrayL(aUids, iUids);
        iIndex=0;
        iFocusItemId = KMPXInvalidItemId;
        InitL(aCallback);
        }
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 3 --->");
    }

// ----------------------------------------------------------------------------
// Request current content of browse context
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::OpenL(
    TInt aMode,
    MMPXCollectionEngineObserver* aCallback)
    {
    // Update path for open request
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 <---");
    SetPathOpenMode(*iBrowsePath,aMode);
    if (iPluginUids[EContextBrowse] == KNullUid)
        {// No plug-in has been resolved
        if (iBrowsePath->Levels() == 0)
            //
            // Open called at root level: just need to list the plug-ins,
            // no need to resolve any plugin. Open request doesn't apply
            // at this level
            {
            MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 InitL()");
            iIndex=0;
            iFocusItemId = KMPXInvalidItemId;
            InitL(aCallback);
            }
        else if (OpenModeValid(*iBrowsePath) && !iPathUpdated)
            //
            // The existing media is still valid (no change
            // in the request for entries
            //
            {
            MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 return media");
            aCallback->HandleOpen(iMedia,
                                  iIndex, ETrue,
                                  iMediaType);
            }
           else // Open mode has changed and open NOT called at root level
               {
               MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 add task different open mode");
               ResolvePluginL(*iBrowsePath, iPluginUids[EContextBrowse]); // Find a plug-in
               if (iPluginUids[EContextBrowse] != KNullUid)
                {
                LoadedPlugin(EContextBrowse)->AddTaskL(EMcsOpen, aCallback, this);
                }
               }
        }
    else
        {
        MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 add task");
        LoadedPlugin(EContextBrowse)->AddTaskL(EMcsOpen, aCallback,this);
        }
    MPX_DEBUG1("CMPXCollectionClientContext::OpenL 4 --->");
    }

// ----------------------------------------------------------------------------
// Media request by collection path
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::BackL(
    MMPXCollectionEngineObserver* aCallback)
    {
    MPX_DEBUG1("CMPXCollectionClientContext::BackL()");
    if (iPluginUids[EContextBrowse]==KNullUid)
        {
        User::Leave(KErrNotReady);
        }

    MPX_DEBUG1("CMPXCollectionClientContext::BackL() -- Add task");
    LoadedPlugin(EContextBrowse)->AddTaskL(EMcsBack, aCallback, this);
    }

// ----------------------------------------------------------------------------
// MediaL command
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::MediaL(
    const CMPXCommand& aCmd,
    MMPXCollectionEngineObserver* aCallback)
    {
    CMPXCollectionPath* path =
        aCmd.ValueCObjectL<CMPXCollectionPath>(KMPXCommandGeneralTargetIds);
    CleanupStack::PushL(path);

    // Increment the plugin reference count
    CMPXCollectionPlugin*plugin = iEngine.ResolvePluginL(*path);
    if (!plugin)
        {
        User::Leave(KErrArgument);
        }

    iEngine.CleanupPluginPushL(plugin);

    CMPXCommand* cmd = CMPXCommand::NewL(aCmd); // make a copy
    CleanupStack::PushL(cmd);

    plugin->AddTaskL(EMcsMediaByPath, aCallback, this,
                     0, NULL, plugin, cmd, path);

    CleanupStack::Pop(cmd); // Ownership transferred to the task queue
    iEngine.PluginPop();    // Pop the plugin
    CleanupStack::Pop(path);// Ownership transferred to the task queue
    }

// ----------------------------------------------------------------------------
// Add a media
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::AddL(const CMPXMedia& aMedia)
    {
    DoUpdateMediaL( EMcsAddItem, aMedia );
    }

// ----------------------------------------------------------------------------
// Remove a media
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::RemoveL(const CMPXMedia& aMedia)
    {
    DoUpdateMediaL( EMcsRemoveItem, aMedia );
    }

// ----------------------------------------------------------------------------
// Update a media
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::SetL(const CMPXMedia& aMedia)
    {
    DoUpdateMediaL( EMcsSetMedia, aMedia );
    }

// ----------------------------------------------------------------------------
// Remove media by path
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::RemoveL(
    CMPXCollectionPath* aPath,
    MMPXCollectionEngineObserver* aCallback)
    {
    TInt id(aPath->Id(CMPXCollectionPath::ECollectionUid));

    // Increment the plugin reference count
    CMPXCollectionPlugin* plugin = iEngine.ResolvePluginL(TUid::Uid(id));
    if (!plugin)
        {
        User::Leave(KErrArgument);
        }

    iEngine.CleanupPluginPushL(plugin);

    // Ownership of aPath transferred
    plugin->AddTaskL(EMcsRemovePath, aCallback, this, 0, NULL, plugin, aPath);

    iEngine.PluginPop();
    }

// ----------------------------------------------------------------------------
// Return current plugin id
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CMPXCollectionClientContext::PluginId() const
    {
    return iPluginUids[EContextBrowse];
    }

// ----------------------------------------------------------------------------
// Handle async request from client
// ----------------------------------------------------------------------------
//
EXPORT_C const CMPXCollectionPath& CMPXCollectionClientContext::Path() const
    {
    return *iBrowsePath;
    }

// ----------------------------------------------------------------------------
// Cancel request
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::CancelRequest(
    MMPXCollectionEngineObserver* aCallback)
    {
    MPX_DEBUG3("CMPXCollectionClientContext::CancelRequest, this %08x, aCallback %08x",
               this, aCallback);
    iEngine.RemoveTask(aCallback);
    }

// ----------------------------------------------------------------------------
// Add a client
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::AddClientL(
    TThreadId aId,
    CMPXMessageQueue* aMsgQueue)
    {
    iClientList->AddClientL(aId, aMsgQueue);
    }

// ----------------------------------------------------------------------------
// Remove a client
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::RemoveClient(
    const CMPXMessageQueue& aMsgQueue)
    {
    MPX_DEBUG2("CMPXCollectionClientContext::RemoveClient with the message queue 0x%08x",
               &aMsgQueue);
    TInt index(iClientList->Find(aMsgQueue));
    if (KErrNotFound != index)
        {
        iClientList->RemoveClient(index);
        }
    if (!iClientList->ClientCount())
        {
        iEngine.RemoveContext(*this);
        }
    }

// ----------------------------------------------------------------------------
// Gets the supported capabilities of the current browse plugin
// ----------------------------------------------------------------------------
//
EXPORT_C TCollectionCapability CMPXCollectionClientContext::GetCapabilities()
    {
    TCollectionCapability cap(0);
    if (iPluginUids[EContextBrowse]!=KNullUid)
        {
        LoadedPlugin(EContextBrowse)->GetCapabilities();
        }
    return cap;
    }

// ----------------------------------------------------------------------------
// Find all aSync
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::FindAllL(
    const CMPXMedia& aMedia,
    CBufBase* aBuf,
    MMPXCollectionEngineObserver* aCallback)
    {
    CMPXCollectionPlugin* plugin(NULL); // Not owned

    // Increment the plugin reference count
    ResolvePluginL(aMedia, plugin);
    if (!plugin)
        {
        User::Leave(KErrNotFound);  // found nothing
        }

    iEngine.CleanupPluginPushL(plugin);

    // Add the task
    CMPXMedia* media = CMPXMedia::NewL(aMedia);
    CleanupStack::PushL(media);
    plugin->AddTaskL(EMcsFindAll, aCallback, this, 0, aBuf, plugin, media);
    CleanupStack::Pop(media); // Ownership transferred

    iEngine.PluginPop();
    }

// ----------------------------------------------------------------------------
// Find all sync
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXCollectionClientContext::FindAllSyncL(
    const CMPXMedia& aMedia,
    const CBufBase& aBuf)
    {
    CMPXCollectionPlugin* plugin(NULL); // Not owned

    // Increment the plugin reference count
    ResolvePluginL(aMedia, plugin);
    iEngine.CleanupPluginPushL(plugin);

    CMPXMedia* result(NULL);
    FindAllL(aMedia, aBuf, *plugin, &result, ETrue);

    // Synchronous call, safe to release the plugin
    CleanupStack::PopAndDestroy();    // plugin
    return result;
    }

// ----------------------------------------------------------------------------
// Set filter
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::SetFilterL(const CMPXFilter* aFilter)
    {
    delete iFilter;
    iFilter = NULL;
    iFilter = aFilter ? CMPXFilter::NewL(*aFilter) : NULL;
    }

// ----------------------------------------------------------------------------
// Returns filter
// ----------------------------------------------------------------------------
//
EXPORT_C const CMPXFilter* CMPXCollectionClientContext::Filter() const
    {
    return iFilter;
    }

// ----------------------------------------------------------------------------
// Handle a command
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::CommandL(
    TMPXCollectionCommand aCmd,
    TInt aData)
    {
    switch( aCmd )
        {
        case EMcCmdRemoveAll:
        case EMcCmdReCreateDB: // fall through
        case EMcCmdDbCorrupted: // fall through
            {
            TUid uid = TUid::Uid(aData);

            // Increment the plugin reference count
            CMPXCollectionPlugin* plugin = iEngine.ResolvePluginL( uid );
            iEngine.CleanupPluginPushL(plugin);

            if( plugin )
                {
                // The trap is needed to keep the plugin reference count in sync
                plugin->CommandL(aCmd);
                }

            CleanupStack::PopAndDestroy();    // plugin
            break;
            }
        case EMcCmdSelect:
            if (iBrowsePath->Levels()<=0)
                {
                User::Leave(KErrNotReady);
                }
            if (aData<0 || aData>=iBrowsePath->Count())
                {
                User::Leave(KErrArgument);
                }
            iBrowsePath->Set(aData);
            iClientList->SendMsgL(
                  TMPXCollectionMessage(TMPXCollectionMessage::EFocusChanged,
                                        aData, aData));
            iIndex = aData; // iIndex will always be up to date
            iFocusItemId = iBrowsePath->Id();
            break;
        case EMcCmdCollectionInit:
        case EMcCmdCollectionResyn:
            {
            TUid uid = TUid::Uid(aData);
            // Increment the plugin reference count
            CMPXCollectionPlugin* plugin = iEngine.ResolvePluginL(uid);
            if( !plugin )
                {
                User::Leave(KErrArgument);
                }

            iEngine.CleanupPluginPushL(plugin);

            plugin->AddTaskL(EMcsCommand, NULL, this, aCmd, NULL, plugin);

            iEngine.PluginPop();
            break;
            }
        case EMcCloseCollection:
            {
            // Forward to all plug-ins; argument is drive number
            iEngine.Command ( aCmd, aData );
            break;
            }

        default:
            {
            User::Leave(KErrNotSupported);
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// Handle a command
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionClientContext::CommandL(
    const CMPXCommand& aCmd,
    MMPXCollectionEngineObserver* aCallback,
    const CMPXMessageQueue& aMsgQueue)
    {
    // Use the plugin id defined by the command object,
    // or the browse context if collection id not defined
    CMPXCollectionPlugin* plugin = NULL;
    if (aCmd.IsSupported(KMPXCommandGeneralCollectionId))
        {
        TInt data = aCmd.ValueTObjectL<TInt>(KMPXCommandGeneralCollectionId);

        // Increment the plugin reference count
        plugin = iEngine.ResolvePluginL(TUid::Uid(data));
        }
    if (!plugin && iPluginUids[EContextBrowse]!=KNullUid)
        {
        plugin = LoadedPlugin(EContextBrowse);

        // Increment the plugin reference count manually
        iEngine.UsePlugin(plugin->Uid());
        }
   iEngine.CleanupPluginPushL(plugin);

   TBool async(ETrue); // by default command is asynchronous
   if (aCmd.IsSupported(KMPXCommandGeneralDoSync))
       { // check if command is sync
       async=!(aCmd.ValueTObjectL<TBool>(KMPXCommandGeneralDoSync));
       }

   if (async)
        { // Async, cmd ownership transferred
        if (!plugin)
            {
            User::Leave(KErrNotReady);
            }

        CMPXCommand* cmd = CMPXCommand::NewL(aCmd);
        CleanupStack::PushL(cmd);
        plugin->AddTaskL(EMcsCommandExt, aCallback, this, 0,
                        NULL, plugin, cmd);
        CleanupStack::Pop(cmd);

        // Async, the plugin pointer passed through the task queue
        iEngine.PluginPop();
        }
    else
        {
        DoHandleSyncCommandL(aCmd, aMsgQueue, plugin);
        CleanupStack::PopAndDestroy();     // plugin
        }
    }

// ----------------------------------------------------------------------------
// Find a shareable client
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionClientContext::HasShareableClient(
    TThreadId aId)
    {
    TBool ret(EFalse);
    if (iModeId != KMcModePlaylist)
        {
        if (iClientList->IsClient(aId) )    // Just look for same thread
            {
            ret = ETrue;
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Notifies all clients of events
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::NotifyL( TMPXCollectionBroadCastMsg aMsg,
                                           TInt aData )
    {
    iClientList->SendMsgL(
          TMPXCollectionMessage(TMPXCollectionMessage::EBroadcastEvent,
                                aMsg,aData));
    }

const TUid& CMPXCollectionClientContext::ModeId() const
    {
    return iModeId;
    }

// ----------------------------------------------------------------------------
// Initialises from file. iFile is not owned here
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::OpenL()
    {
    if (iPathUpdated)
        {
        ReOpenL();
        }
    else
        {
        CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
        MPX_ASSERT(plugin);
        plugin->CompleteTask();
        if (plugin->Callback())
            {
            plugin->Callback()->HandleOpen(iMedia,
                                                   iIndex, ETrue,iMediaType);
            plugin->SetCallback( NULL ); // Reset current observer
            plugin->SetObserver(iEngine);
            }
        }
    }

// ----------------------------------------------------------------------------
// Cancels all outstanding calls (tasks): plug-in should only have one
// outstanding so that is canceled; the tasks are deleted and removed from the
//  queue
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::BackL()
    {
    TInt d=iBrowsePath->Levels();
    if(d>1)
        {
        iPathUpdated = EFalse;
        // remove last item - the actual item selected
        iBrowsePath->Back();
        // preserve focus, since we're rebuilding this level
        iIndex=iBrowsePath->Index();
        iFocusItemId = iBrowsePath->Id();
        ReOpenL();
        }
    }

// ----------------------------------------------------------------------------
// Re-open current level
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ReOpenL()
    {
    // remove last item - go to the container which contained the item
    if (iBrowsePath->Levels())
        {
        iBrowsePath->Back();
        }
    if (iBrowsePath->Levels() == 0)
        {
        //Plugin must be resolved ealier
        CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
        plugin->CompleteTask();
        InitL(plugin->Callback());
        }
    else
        {
        DoPluginOpenL();
        }
    }

// ----------------------------------------------------------------------------
// Retrieve media info based on a find criteria
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::FindAllL(
    const CMPXMedia& aMedia,
    const CBufBase& aBuf,
    CMPXCollectionPlugin& aPlugin,
    CMPXMedia** aResult /*= NULL*/,
    TBool aSync /*= EFalse*/)
    {
    // Find critera and return attributes
    //
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL(attrs);
    ::CreateFromBufferL(aBuf, attrs);

    // Do the query
    if (!aSync)
        {
        aPlugin.SetObserver(*this);
        aPlugin.FindAllL(aMedia,attrs.Array());
        }
    else
        {
        *aResult = aPlugin.FindAllSyncL(aMedia,attrs.Array());
        }
    CleanupStack::PopAndDestroy( &attrs );
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::HandleMessage
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleMessage(
    CMPXMessage* aMsg,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionClientContext::HandleMessage");
    DoHandleMessage(aMsg, aError, ETrue);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::HandleMessage
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleMessage(
    CMPXMessage* aMsg,
    TInt aError,
    TBool aNotifyOthers)
    {
    MPX_DEBUG3("-->CMPXCollectionClientContext::HandleMessage this %08x, %d", this, aNotifyOthers);
    if (!aMsg)
        {
        iClientList->SendMsg(aMsg, aError);
        if (aNotifyOthers)
            {
            iEngine.NotifyChange(*this, aMsg, aError);
            }
        }
    else
        {
        if (aMsg->IsSupported(KMPXMessageGeneralId))
            {
            if (aMsg->IsSupported(KMPXMessageArrayContents))
                {
                TBool pathUpdated(EFalse);
                const CMPXMessageArray* messageArray =
                    aMsg->Value<CMPXMessageArray>(KMPXMessageArrayContents);
                if (messageArray)
                    {
                    TInt count = messageArray->Count();

                    MPX_DEBUG2("%d messages:", count);

                    for (TInt i=0; i<count; i++)
                        {
                        const CMPXMessage* message =(*messageArray)[i];
                        if (message)
                            {
                            TRAP_IGNORE(DoHandleMessageL(*message, KErrNone));
                            pathUpdated |=iPathUpdated;
                            }
                        }
                    iPathUpdated = pathUpdated;
                    }
                // Notify other context, collection is changed.
                // Send the batched version
                //
                if (aNotifyOthers)
                    {
                    iEngine.NotifyChange(*this,
                                         const_cast<CMPXMessage*>(aMsg),
                                         aError);
                    }
                }
            else
                {
                TRAP_IGNORE(DoHandleMessageL(*aMsg, aError));

                // Notify other context, collection is changed.
                if (aNotifyOthers)
                    {
                    iEngine.NotifyChange(*this, aMsg, aError);
                    }
                }

            // Broadcast change messages to the clients to handle
            //
            if( iModeId == KMcModePlaylist )
                {
                // Playlist should not be handling msgs during refresh
                if( !iEngine.IsRefreshing() )
                    {
                    iClientList->SendMsg( aMsg, aError );
                    }
                }
            else
                {
                // Other modes receive msg all the time
                iClientList->SendMsg( aMsg, aError );
                }

            // Batched collection change messages and only send this once
            // Path changed isn't sent to the UI during refresh
            //
            if (iPathUpdated)
                {
                MPX_DEBUG1("CMPXCollectionClientContext::DoHandleMessageL detected path changed");
                MPX_DEBUG_PATH(*iBrowsePath);
                // notify clients to refresh
                TRAP_IGNORE(iClientList->SendMsgL(
                    TMPXCollectionMessage(TMPXCollectionMessage::EPathChanged,
                                          EMcPathChangedByCollectionChange)));
                }
            }
        }

    MPX_DEBUG3("<--CMPXCollectionClientContext::HandleMessage this %08x, %d", this, aNotifyOthers);
    }

// ----------------------------------------------------------------------------
// Handle collection item change message
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleMessageL(
    const CMPXMessage& aMsg,
    TInt /*aError*/)
    {
    MPX_DEBUG2("-->CMPXCollectionClientContext::DoHandleMessageL this %08x", this);
    TInt browseLevels = iBrowsePath->Levels(); // browse depth

    if (aMsg.ValueTObjectL<TMPXMessageId>(KMPXMessageGeneralId)==
        KMPXMessageIdItemChanged)
        {
        if (!aMsg.IsSupported(KMPXMessageCollectionId) ||
            !aMsg.IsSupported(KMPXMessageChangeEventType) ||
            !aMsg.IsSupported(KMPXMessageMediaGeneralCategory) ||
            !aMsg.IsSupported(KMPXMessageMediaGeneralId))
            {
            User::Leave(KErrArgument);
            }

        TUid collectionId = aMsg.ValueTObjectL<TUid>(KMPXMessageCollectionId);

        TMPXChangeEventType changeType =
            aMsg.ValueTObjectL<TMPXChangeEventType>(KMPXMessageChangeEventType);

        TMPXGeneralCategory category =
            aMsg.ValueTObjectL<TMPXGeneralCategory>(KMPXMessageMediaGeneralCategory);

        TMPXItemId itemId = aMsg.ValueTObjectL<TMPXItemId>(KMPXMessageMediaGeneralId);

        TMPXItemId deprecatedId(0);
        if (aMsg.IsSupported(KMPXMessageMediaDeprecatedId))
            {
            deprecatedId = aMsg.ValueTObjectL<TMPXItemId>(KMPXMessageMediaDeprecatedId);
            }

        MPX_DEBUG5("CMPXCollectionClientContext::DoHandleMessageL colId %08x, id %d, deprecatedId %d, type %d",
                  collectionId.iUid, itemId.iId1, deprecatedId.iId1, changeType);


        if(!iEngine.IsRefreshing())
            {
            // Invalidate path
            TInt pct = KErrNotFound; // path change type
            if (changeType == EMPXItemInserted && category != EMPXPlaylist)
                {
                pct = CMPXCollectionPath ::EAdded;
                }
            else if (changeType == EMPXItemModified && category != EMPXPlaylist)
                {
                pct = CMPXCollectionPath ::EModified;
                }
            else if (changeType == EMPXItemDeleted)
                {
                pct = CMPXCollectionPath ::EDeleted;
                }
            else if ((changeType == EMPXItemInserted || changeType == EMPXItemModified) &&
                     category == EMPXPlaylist)
                {
                pct = CMPXCollectionPath ::EGroupModified;
                }
            if( changeType == EMPXItemModified && category == EMPXCollection )
                {
                // Overwrite previous type
                pct = CMPXCollectionPath ::EGroupModified;
                }

            if (KErrNotFound != pct)
                { // update browse path
                CMPXCollectionPath::TMPXCollectionPathChange ct =
                    static_cast<CMPXCollectionPath::TMPXCollectionPathChange>(pct);
                TInt pUpdated(CMPXCollectionPath::EPathUnchanged);

                // If browse context and browse context is being modified
                //
                if (iModeId != KMcModePlaylist && browseLevels &&
                    iBrowsePath->Id(0) == collectionId.iUid )
                    { // check the browse path
                    TInt tmpIndex(0);

                    pUpdated = iBrowsePath->HandleChange(
                                       collectionId, itemId, deprecatedId, ct,
                                       tmpIndex);

                    // Take the updated selection index only if it was affected
                    // We need to store the TMPXItemId for iIndex because
                    // a previous collection change would have removed the
                    // top level of the collection path
                    //
                    iPathUpdated = iPathUpdated ||
                                   (pUpdated != CMPXCollectionPath::EPathUnchanged);
                    if( pUpdated )
                        {
                        // Only update the selection index if the itemid that
                        // was modified is equal to the one selected
                        //
                        // OR take in the index if clipped the collection path
                        // (pUpdated == KPathClipped )
                        //
                        if( pUpdated == CMPXCollectionPath::EPathClipped )
                            {
                            iIndex = tmpIndex;
                            iFocusItemId = KMPXInvalidItemId;
                            }
                        else if( iFocusItemId.ApproxEqual(itemId ) &&
                                 ( iFocusItemId != KMPXInvalidItemId )  )
                            {
                            // Also make sure the index that came back from
                            // checking the collection path isn't -1
                            //
                            // The only case where the selected item will shift
                            // is when we are deleting that item. updaing the item or adding
                            // an item to the current path should not move the focus to another
                            // item
                            //
                            if( tmpIndex != -1 && changeType == EMPXItemDeleted)
                                {
                                if( tmpIndex == iBrowsePath->Count()-1 )
                                    {
                                    // Last item, focus on previous
                                    iIndex = tmpIndex-1;
                                    iFocusItemId = iBrowsePath->IdOfIndex( tmpIndex-1 );
                                    }
                                else
                                    {
                                    // Not last, focus on next
                                    iFocusItemId = iBrowsePath->IdOfIndex( tmpIndex+1 );
                                    }
                                }
                            }
                        else if( iFocusItemId.ApproxEqual( deprecatedId ) &&
                                 changeType == EMPXItemModified )
                            {
                            // If we are modifying the item id of an item currently in focus
                            // We move the focus to the new item id
                            //
                            iFocusItemId = itemId;
                            }
                        }
                    }
                }
            }
        }
    else
        {
        // do nothing
        }
    MPX_DEBUG2("<--CMPXCollectionClientContext::DoHandleMessageL this %08x", this);
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleOpen(
    CMPXMedia* aMedia,
    TInt aErr)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleOpen with media returned");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
    MPX_ASSERT(plugin);

    // Callback and Task Completion
    //
    MMPXCollectionEngineObserver* callback(NULL);
    TBool openTask(EFalse);
    TInt task = plugin->Task();
    if( task == EMcsOpen || task == EMcsOpenPath ||
        task == EMcsBack || task == EMcsOpenIndex )
        {
        callback = plugin->Callback();
        openTask = ETrue;
        }

    // Cachable flag
    //
    TBool cache(EFalse);
    if( iBrowsePath &&
        iBrowsePath->Levels() >= 1 )
        {
        TMPXItemId id(iBrowsePath->Id(CMPXCollectionPath::ECollectionUid) );
        cache = iEngine.PluginCacheable( TUid::Uid( id ) );
        }

    TRAPD(err, DoHandleOpenL(aMedia, NULL, callback, aErr, cache, openTask));
    if (err && openTask)
        {
        HandleError(*plugin, err);
        }
    else if( openTask )
        {
        plugin->CompleteTask();
        }
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleOpen(
    CMPXMedia* aMedia,
    const CMPXCollectionPath* aPath,
    TInt aErr)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleOpen with media returned");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
    MPX_ASSERT(plugin);

    // Callback and Task Completion
    //
    MMPXCollectionEngineObserver* p(NULL);
    TBool openTask(EFalse);
    TInt task = plugin->Task();
    if( task == EMcsOpen || task == EMcsOpenPath ||
        task == EMcsBack || task == EMcsOpenIndex )
        {
        p = plugin->Callback();
        openTask = ETrue;
        }

    // Cachable flag
    //
    TBool cache(EFalse);
    if( iBrowsePath &&
        iBrowsePath->Levels() >= 1 )
        {
        TMPXItemId id(iBrowsePath->Id(CMPXCollectionPath::ECollectionUid) );
        cache = iEngine.PluginCacheable( TUid::Uid( id ) );
        }

    // Handle OpenL for Media
    //
    TRAPD(err, DoHandleOpenL(aMedia, aPath, p, aErr, cache, openTask));
    if (err && openTask)
        {
        HandleError(*plugin, err);
        }
    else if( openTask )
        {
        plugin->CompleteTask();
        }
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleOpenL(
    CMPXMedia* aMedia,
    const CMPXCollectionPath* aPath,
    MMPXCollectionEngineObserver* aCallback,
    TInt aErr,
    TBool aAddToCache,
    TBool aOpenTask )
    {
    MPX_FUNC("CMPXCollectionClientContext::DoHandleOpen with media returned");

    // Make sure if we are updating the results the paths are aligned!
    //
    TBool openUpdate(EFalse);
    if( !aOpenTask )
        {
        openUpdate = ETrue;
        iCacheMedia = ETrue;
        if( aPath &&
            aPath->Levels() != iBrowsePath->Levels() )
            {
            User::Leave( KErrArgument );
            }
        }

    // Add to cache
    //
    if ( iBrowsePath->Levels() && aAddToCache && aMedia && iCacheMedia )
        {
        // manage the priority
        CMPXCollectionCache::TCachePriority priority(CMPXCollectionCache::EPriorityNormal);
        if ((1 == iBrowsePath->Levels()) ||
            ((2 == iBrowsePath->Levels()) && ((iBrowsePath->Id(1)).iId1 == 0)))
            {
            priority = CMPXCollectionCache::EPriorityHigh;
            }

        if( !aOpenTask )
            {
            // Update the current browse path
            //
            CMPXCollectionPath* container = iBrowsePath->ContainerPathL();
            CleanupStack::PushL( container );

            // Add the data to the generic cache and get the latest copy
            //
            aMedia = AddToCache( *container,
                         aMedia->Attributes(),
                         *aMedia,
                         ETrue,
                         priority);
            CleanupStack::PopAndDestroy( container );
            }
        else
            {
            AddToCache( *iBrowsePath,
                         aMedia->Attributes(),
                         *aMedia,
                         ETrue,
                         priority);
            }
        }

    // Current client context browse path media
    //
    iMediaType = KMPXCollectionEntries;
    if (iMedia != aMedia)
        { // new media
        delete iMedia;
        iMedia = NULL;
        if (aMedia)
            {
            iMedia = CMPXMedia::NewL(*aMedia);
            }
        }

    TInt n(0);
    if (aMedia && KErrNone == aErr)
        {  // Update the path
        RArray<TInt> selectionIndicies;
        CleanupClosePushL( selectionIndicies );

        if (aPath)
            { // Update path from aPath
            if ( aMedia->IsSupported (KMPXCollectionOpenLAllResultRange))
                {
                iBrowsePath->Back ();
                RArray<TMPXItemId> ids;
                CleanupClosePushL (ids);
                RArray<TMPXOpenDataBlock> datablocks;
                CleanupClosePushL (datablocks);
                // De-serialize from global data, would be good to have global arrays
                //
                const TDesC
                        & buf = aMedia->ValueText (KMPXCollectionOpenLAllResultRange);
                CBufBase* buffer(NULL);
                MPXUser::CreateBufferL ( buf, buffer);
                CleanupStack::PushL ( buffer);
                ::CreateFromBufferL ( *buffer, datablocks);
                CleanupStack::PopAndDestroy ( buffer);

                if ( aMedia->IsSupported (KMPXMediaArrayContents))
                    {
                    const CMPXMediaArray
                            * mediaArray = aMedia->Value<CMPXMediaArray> (KMPXMediaArrayContents);
                    User::LeaveIfNull (const_cast<CMPXMediaArray*>(mediaArray));
                    TInt dataCount = mediaArray->Count ();
                    TInt rangeCount = datablocks.Count ();
                    for (TInt index = 0; index < dataCount; ++index)
                        {
                        CMPXMedia* media = mediaArray->AtL(index);

                        //Check range
                        TBool validItem(EFalse);
                        MPX_ASSERT(rangeCount > 0);
                        for (TInt rangeIndex = 0; rangeIndex < rangeCount; ++rangeIndex)
                            {
                            TInt offset = datablocks[rangeIndex].iOffset;
                            TInt size = datablocks[rangeIndex].iSize;
                            if ( (index >= offset) && (index < offset + size))
                                {
                                validItem = ETrue;
                                break;
                                }
                            }
                        
                        if (validItem)
                            {
                            const TMPXItemId
                                    id = media->ValueTObjectL<TMPXItemId> (KMPXMediaGeneralId);
                            ids.AppendL (id);
                            }
                        else
                            {
                            ids.AppendL (KMPXInvalidItemId);
                            }
                        }
                    iBrowsePath->AppendL (ids.Array ());
                    }
                CleanupStack::PopAndDestroy (&datablocks);
                CleanupStack::PopAndDestroy (&ids);
                }
            else
                {
                delete iBrowsePath;
                iBrowsePath = NULL;
                iBrowsePath = CMPXCollectionPath::NewL ( *aPath);
                }
            }
        else
            { // Update path from media
            RArray<TMPXItemId> ids;
            CleanupClosePushL(ids);
            if( aMedia->IsSupported(KMPXMediaArrayContents) )
                {
                const CMPXMediaArray* mediaArray=aMedia->Value<CMPXMediaArray>(
                                                            KMPXMediaArrayContents);
                User::LeaveIfNull(const_cast<CMPXMediaArray*>(mediaArray));
                n=mediaArray->Count();
                for (TInt i=0;i<n;++i)
                    {
                    CMPXMedia* media=mediaArray->AtL(i);
                    const TMPXItemId id=media->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
                    ids.AppendL(id);

                    // Try to look for the selection that we want
                    if( iFocusItemId == id )
                        {
                        selectionIndicies.AppendL( i );
                        }
                    }
                }

            iBrowsePath->AppendL(ids.Array());
            CleanupStack::PopAndDestroy(&ids);
            }

        // List of items
        const TArray<TMPXItemId>& items = iBrowsePath->Items();

        // OpenL Media has been updated, update the itemid
        // based on the current index
        if( openUpdate )
            {
            // Check if we have item id first
            //
            if( iFocusItemId != KMPXInvalidItemId )
                {
                TInt index = iBrowsePath->IndexOfId( iFocusItemId );
                if( index != KErrNotFound )
                    {
                    iIndex = index;
                    }
                }

            // Then update based on index
            if( iIndex >=0 && iIndex < iBrowsePath->Count() )
                {
                iBrowsePath->Set(iIndex);
                iFocusItemId = iBrowsePath->Id();
                }
            }
        else if( aMedia->IsSupported( KMPXCollectionOpenLResultRange ) )
            {
            TMPXOpenDataBlock data =
                aMedia->ValueTObjectL<TMPXOpenDataBlock>(KMPXCollectionOpenLResultRange);
            if( iBrowsePath->Count() &&
                iIndex >= 0 &&
                iIndex < iBrowsePath->Count() )
                {
                // If all blank items then we don't change index yet
                // Wait until we have re-opened the partial data
                if( data.iOffset != KErrNotFound )
                    {
                    // Not supported, stay with current focus
                    iBrowsePath->Set(iIndex);
                    iFocusItemId = iBrowsePath->Id();
                    }
                }
            }
        // If this selection appears more than once, then we have to find
        // the closest matching index to what we had before
        //
        //
        else if( selectionIndicies.Count() > 1 &&
                 iFocusItemId != KMPXInvalidItemId )
            {
            TInt best(selectionIndicies[0]);
            TInt idAppearance( selectionIndicies.Count() );
            for( TInt i=0; i<idAppearance; ++i )
                {
                if( Abs(selectionIndicies[i]-iIndex) <= Abs(best-iIndex) )
                    {
                    best = selectionIndicies[i];
                    }
                }
            iIndex = best;
            iBrowsePath->Set(iIndex);
            }
        // Always set by item id if we have it
        // This is needed if an item was added to the current browse level
        // We still want to select the item we were previously at
        //
        else if( iFocusItemId != KMPXInvalidItemId )
            {
            TInt index = iBrowsePath->IndexOfId(iFocusItemId);
            if (KErrNotFound != index)
                {
                iBrowsePath->Set(index);
                }
            else if( n > 0 ) // Focus item has been deleted, select next
                {
                // Bounds check for largest and smallest
                if( iIndex >= n )
                    {
                    iIndex = n-1;
                    }
                if( iIndex < 0 )
                    {
                    iIndex = 0;
                    }
                iBrowsePath->Set(iIndex);
                iFocusItemId = iBrowsePath->Id();
                }
            iIndex = iBrowsePath->Index();
            }
        else
            {
            // Bounds checking for iIndex
            if( iIndex >= n && iIndex > 0 )
                {
                iIndex = n-1;

                // Just in case if ids has 0 items
                if( iIndex > 0 && iIndex < items.Count() )
                    {
                    iFocusItemId = items[iIndex];
                    }
                }

            if (iBrowsePath->Levels()>0 && iIndex>=0 &&
                iIndex<iBrowsePath->Count())
                {
                iBrowsePath->Set(iIndex);
                }
            iFocusItemId = iBrowsePath->Id();
            }
        CleanupStack::PopAndDestroy(&selectionIndicies);
        }

    TInt err;
    if( !iPathUpdated )
        {
        // Normal open mode
        err = KErrNone == aErr ? KMPXPathUpdated : aErr;
        }
    else
        {
        // Updates open mode, should be returning entries
        err = KErrNone == aErr ? KMPXCollectionEntries : aErr;
        }

    // Should not be broadcasting container opened messages
    // When we are only rebuilding the leaf level
    //
    if( !iPathUpdated && aOpenTask )
        {
        if( err >= KErrNone)
            {
            iClientList->SendMsgL(
                TMPXCollectionMessage(TMPXCollectionMessage::EPathChanged,
                                      EMcPathChangedByOpen,
                                      EMcContainerOpened));
            }
        }

    iPathUpdated = EFalse;

    // Notes: there should no leave function called after aCallback->HandleOpen
    if (aCallback)
        {  // Complete open with error or PathUpdated
        aCallback->HandleOpen(iMedia, iIndex, ETrue, err);
        if (iPluginUids[EContextBrowse]!=KNullUid)
            {
            CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
            MPX_ASSERT(aCallback == plugin->Callback());
            plugin->SetCallback(NULL);
            plugin->SetObserver(iEngine);
            }
        }
    MPX_DEBUG_PATH(*iBrowsePath);
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleOpen(
   CMPXCollectionPath* aPath,
   TInt aErr)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleOpen with path returned");
    TRAPD(err, DoHandleOpenL(aPath, aErr));
    if (err)
        {
        HandleError(*(LoadedPlugin(EContextBrowse)), err);
        }
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleOpenL(
    CMPXCollectionPath* aPath,
    TInt aErr)
    {
    MPX_FUNC("CMPXCollectionClientContext::DoHandleOpen with path returned");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
    MPX_ASSERT(plugin);

    plugin->CompleteTask();

    if (iBrowsePath != aPath && KErrNone == aErr)
        {
        delete iBrowsePath;
        iBrowsePath = NULL;
        MPX_ASSERT(aPath);
        iBrowsePath = CMPXCollectionPath::NewL(*aPath);
        }
    iMediaType = KMPXCollectionPath;
    iIndex = iBrowsePath->Index();
    iFocusItemId = iBrowsePath->Id();

    TInt err = KErrNone == aErr ? KMPXPathUpdated : aErr;

    // Complete msg
    // Complete open with error or PathUpdated
    plugin->Callback()->HandleOpen(iMedia, iIndex, ETrue, err);
    plugin->SetCallback(NULL);
    plugin->SetObserver(iEngine);
    if (aPath->OpenNextMode() != EMPXOpenNoPlaylist)
         //
         // The request was NOT to play, but the plug-in returned
         // the path implying that we should play; so we don't
         // send it
         //
        {
        iClientList->SendMsgL(
               TMPXCollectionMessage(TMPXCollectionMessage::EPathChanged,
                                     EMcPathChangedByOpen,
                                     EMcItemOpened));
        }

    MPX_DEBUG_PATH(*iBrowsePath);
    }

// ----------------------------------------------------------------------------
// Callback of retrieving extended media property
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleMedia(
    CMPXMedia* aMedia,
    TInt aError)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleMedia");

    CMPXMedia* ret( aMedia );

    TBool pluginCacheable(EFalse);
    if( iMediaPath )
        {
        TMPXItemId id(iMediaPath->Id(CMPXCollectionPath::ECollectionUid) );
        pluginCacheable = iEngine.PluginCacheable( TUid::Uid( id ) );

        // if media returned was resulted from multiple selections we don't cache the
        // results as it's very hard to reuse it anyway
        if(iMediaPath->Selection().Count() > 0)
            {
            pluginCacheable = EFalse;
            }
        }

    // Cache the media
    if ( !aError && aMedia && iCacheMedia && iMediaPath && pluginCacheable )
        {
        TMPXItemId id( iMediaPath->Id(
                            CMPXCollectionPath::ECollectionUid ));
        ret = AddToCache( *iMediaPath, aMedia->Attributes(), *aMedia );
        }

    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextMedia);

    plugin->CompleteTask();
    MPX_ASSERT(plugin->Callback());
    plugin->Callback()->HandleMedia(ret, aError);
    plugin->SetCallback(NULL); // Reset current observer
    plugin->SetObserver(iEngine);
    }

// ----------------------------------------------------------------------------
// Callback of async CommandL
// ----------------------------------------------------------------------------
//
 void CMPXCollectionClientContext::HandleCommandComplete(
    CMPXCommand* aCommandResult,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionClientContext::HandleCommandComplete");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextMedia);

    plugin->CompleteTask();
    MPX_ASSERT(plugin->Callback());
    plugin->Callback()->HandleCommandComplete(aCommandResult, aError);
    plugin->SetCallback(NULL); // Reset current observer
    plugin->SetObserver(iEngine);
    }

// ----------------------------------------------------------------------------
// Handle find all
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleFindAll(
    CMPXMedia* aMedia,
    TInt aError)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleFindAll");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextMedia);
    plugin->CompleteTask();
    MPX_ASSERT(plugin->Callback());
    plugin->Callback()->HandleFindAll(aMedia, aError);
    plugin->SetCallback(NULL); // Reset current observer
    plugin->SetObserver(iEngine);
    }

// ----------------------------------------------------------------------------
// Handle delete all
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleRemove(
    const CDesCArray& aUriArray,
    TInt aError)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleRemove");
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextRemove);

    plugin->CompleteTask();
    MPX_ASSERT(plugin->Callback());
    plugin->Callback()->HandleRemove(aUriArray, aError);
    plugin->SetCallback(NULL); // Reset current observer
    plugin->SetObserver(iEngine);
    }

// ----------------------------------------------------------------------------
// Execute a async task
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ExecuteTask(
    TInt aTask,
    TInt aParamData,
    TAny* aPtrData,
    const CBufBase& aBuf,
    TAny* aCallback,
    CBase* aCObject1,
    CBase* aCObject2)
    {
    MPX_DEBUG2("-->CMPXCollectionClientContext::ExecuteTask %d", aTask);
    CMPXCollectionPlugin* plugin(NULL);
    TRAPD(err, ExecuteTaskL(aTask, aParamData, aPtrData, aCallback, aBuf,
                            aCObject1,aCObject2,plugin));
    if (KErrNone !=err)
        {
        HandleError(*plugin, err, EFalse);
        }
    MPX_DEBUG1("<--CMPXCollectionClientContext::ExecuteTask");
    }

// ----------------------------------------------------------------------------
// Indicates that a task was terminated with an error
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleTaskError(
    TInt aTask,
    TAny* aPtrData,
    TAny* aCallback,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionClientContext::HandleTaskError");
    CMPXCollectionPlugin* plugin(NULL);
    switch (aTask)
        {
        case EMcsOpen:
        case EMcsBack:
        case EMcsOpenIndex:
            plugin = LoadedPlugin(EContextBrowse);
            plugin->SetObserver(*this);
            plugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            HandleError(*plugin, aError, EFalse);
            break;
        case EMcsOpenPath:
        case EMcsMediaByPath:
        case EMcsCommandExt:
        case EMcsRemovePath:
        case EMcsFindAll:
        case EMcsCommand:
            plugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            plugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            HandleError(*plugin, aError, ETrue);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// Initialize before first open
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::InitL(MMPXCollectionEngineObserver* aCallback)
    {
    MPX_DEBUG1("CMPXCollectionClientContext::InitL() <---");
    delete iMedia;
    iMedia = NULL;
    delete iBrowsePath;
    iBrowsePath = NULL;
    RArray<TInt> supportedIds;
    CleanupClosePushL(supportedIds);
    supportedIds.AppendL(KMPXMediaIdContainer);
    supportedIds.AppendL(KMPXMediaIdGeneral);
    iMedia=CMPXMedia::NewL(supportedIds.Array());
    iBrowsePath = CMPXCollectionPath::NewL();
    CleanupStack::PopAndDestroy(&supportedIds);

    iEngine.ListPluginsL(*iMedia, iUids.Array());
    if (iPluginUids[EContextBrowse]!=KNullUid )
        {
        MPX_DEBUG1("CMPXCollectionClientContext::InitL() Collection changed");
        iClientList->SendMsgL(
            TMPXCollectionMessage(TMPXCollectionMessage::ECollectionChanged,0,0));
        }

    // sets the browse plugin to NULL
    SetPlugin(EContextBrowse, NULL);

    // Update collection path, and buffer, send path update msg,
    // complete request
    TRAPD(err, DoHandleOpenL(iMedia, NULL, aCallback, KErrNone, ETrue, ETrue));
    if (err)
        {
        MPX_DEBUG2("CMPXCollectionClientContext::InitL() Complete Open %i", err);
        aCallback->HandleOpen(iMedia, // Not used
                              iIndex,
                              ETrue, err);
        }
    MPX_DEBUG1("CMPXCollectionClientContext::InitL() --->");
    }

// ----------------------------------------------------------------------------
// Execute a async task
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ExecuteTaskL(
    TInt aTask,
    TInt aParamData,
    TAny* aPtrData,
    TAny* aCallback,
    const CBufBase& aBuf,
    CBase* aCObject1,
    CBase* aCObject2,
    CMPXCollectionPlugin*& aPlugin)
    {
    MPX_DEBUG4("CMPXCollectionClientContext::ExecuteTaskL 0x%08x, task %d, aParam %d",
               this, aTask, aParamData);
    switch (aTask)
        {
        case EMcsOpen:
            {
            aPlugin = LoadedPlugin(EContextBrowse);
            aPlugin->SetObserver(*this);
            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            OpenL();
            break;
            }
        case EMcsOpenPath:
            {
            // Set plugin first for handling error
            TUid oldUid = iPluginUids[EContextBrowse];
            aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            // Set plugin and task pointer for browse context
            SetPlugin(EContextBrowse, aPlugin);
            iPathUpdated = EFalse;
            aPlugin->SetObserver(*this);
            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            CMPXCollectionPath* path = static_cast<CMPXCollectionPath*>(aCObject1);
            delete iBrowsePath;
            iBrowsePath = NULL;
            iBrowsePath = CMPXCollectionPath::NewL(*path);
            SetPathOpenMode(*iBrowsePath, static_cast<TMPXOpenMode>(aParamData));

            // If the browsing plugin is changing, need to broadcast the
            // collection changed message
            if (iPluginUids[EContextBrowse] != oldUid)
                {
                // no leave before plugin API OpenL
                TRAP_IGNORE(iClientList->SendMsgL(
                    TMPXCollectionMessage(
                         TMPXCollectionMessage::ECollectionChanged,0,iPluginUids[EContextBrowse].iUid)));
                }
            iIndex=0;
            iFocusItemId = KMPXInvalidItemId;
            DoPluginOpenL();
            break;
            }
        case EMcsOpenIndex:
            {
            // Internalize mode
            TInt mode = (TInt)aPtrData;
            aPlugin = LoadedPlugin(EContextBrowse);
            aPlugin->SetObserver(*this);
            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            iBrowsePath->Set(aParamData);
            SetPathOpenMode(*iBrowsePath,static_cast<TMPXOpenMode>(mode));
            iIndex=0;
            iFocusItemId = KMPXInvalidItemId;
            DoPluginOpenL();
            break;
            }
        case EMcsBack:
            {
            aPlugin = LoadedPlugin(EContextBrowse);
            aPlugin->SetObserver(*this);
            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            BackL();
            break;
            }
        case EMcsMediaByPath:
            {
            // Setup plugin first for handle error
            aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            SetPlugin(EContextMedia, aPlugin);

            aPlugin->SetObserver(*this);
            aPlugin->SetCallback(
                        static_cast<MMPXCollectionEngineObserver*>(aCallback));
            // Media path
            CMPXCommand* cmd = reinterpret_cast<CMPXCommand*>(aCObject1);
            MPX_ASSERT(cmd->IsSupported(KMPXCommandGeneralTargetIds));
            MPX_ASSERT(cmd->IsSupported(KMPXCommandMediaAttributeSpecs));
            CMPXCollectionPath* path = reinterpret_cast<CMPXCollectionPath*>(aCObject2);
            CMPXAttributeSpecs* specs =
                cmd->Value<CMPXAttributeSpecs>(KMPXCommandMediaAttributeSpecs);
            User::LeaveIfNull(specs);
            CMPXAttributeSpecs* filter =
                cmd->Value<CMPXAttributeSpecs>(KMPXCommandMediaFilter);
            User::LeaveIfNull(filter);
            TCapabilitySet caps = cmd->ValueTObjectL<TCapabilitySet>(KMPXCommandMediaCapbilitySet);
            // Ask plugin for media
            DoPluginMediaL( *path, caps, *specs, *filter );
            break;
            }
        case EMcsRemovePath:
            {
            aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            // set the remove plugin
            SetPlugin(EContextRemove, aPlugin);

            aPlugin->SetObserver(*this);
            CMPXCollectionPath* path =
                reinterpret_cast<CMPXCollectionPath*>(aCObject1);
            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            aPlugin->RemoveL( *path );
            break;
            }
        case EMcsFindAll:
            {
            aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            // set the media plugin
            SetPlugin(EContextMedia, aPlugin);

            aPlugin->SetCallback(
                    reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback));
            CMPXMedia* media = static_cast<CMPXMedia*>(aCObject1);
            FindAllL(*media, aBuf, *aPlugin);
            break;
            }
        case EMcsCommand:
            {
            TMPXCollectionCommand cmd =
                static_cast<TMPXCollectionCommand>( aParamData );
            switch( cmd )
                {
                case EMcCmdCollectionInit:
                case EMcCmdCollectionResyn:
                    // Decrement the old plugin reference count
                    aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
                    SetPlugin(EContextMedia, aPlugin);

                    if( aPlugin )
                        {
                        aPlugin->SetObserver(*this);
                        aPlugin->CommandL( cmd );
                        aPlugin->SetObserver(iEngine);
                        aPlugin->CompleteTask();
                        }
                    break;
                default:
                    MPX_ASSERT(0);
                    break;
                }
            break;
            }
        case EMcsCommandExt:
            {
            CMPXCommand* cmd = reinterpret_cast<CMPXCommand*>(aCObject1);
            aPlugin = reinterpret_cast<CMPXCollectionPlugin*>(aPtrData);
            MMPXCollectionEngineObserver* callback =
                             reinterpret_cast<MMPXCollectionEngineObserver*>(aCallback);
            if( cmd->IsSupported( KMPXCommandGeneralId) )
                {
                TMPXCommandId commandId = cmd->ValueTObjectL<TMPXCommandId>(KMPXCommandGeneralId);
                // Decrement the old plugin reference count
                SetPlugin(EContextMedia, aPlugin);
                aPlugin->SetCallback( callback );

                if( commandId == KMPXCommandIdCollectionSelect )
                    {
                    // Decrement the old plugin reference count
                    //
                    DoHandleSelectCommandL( *cmd );
                    aPlugin->SetCallback( NULL );
                    aPlugin->CompleteTask();
                    callback->HandleCommandComplete( NULL, KErrNone );
                    }
                else
                    {
                    aPlugin->SetObserver(*this);
                    aPlugin->CommandL(*cmd);
                    }
                }
            else
                {
                aPlugin->CompleteTask();
                callback->HandleCommandComplete( NULL, KErrArgument );  // return error message
                }
            break;
            }
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// Error happens upon request
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::HandleError(
    CMPXCollectionPlugin& aPlugin,
    TInt aError,
    TBool aUnusePlugin/*=EFalse*/)
    {
    TInt task(aPlugin.Task());
    MPX_DEBUG3("CMPXCollectionClientContext::HandleError %d, task %d",
              aError, task);
    switch (task)
        {
        case EMcsOpen:
        case EMcsOpenPath:
        case EMcsBack:
        case EMcsOpenIndex:
            aPlugin.Callback()->HandleOpen(iMedia, // Not used
                                           iIndex,
                                           ETrue, aError);
            aPlugin.CompleteTask();
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            break;
        case EMcsMediaByPath:
            aPlugin.Callback()->HandleMedia(iMedia, aError);
            aPlugin.CompleteTask();
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            break;
        case EMcsCommandExt:
            aPlugin.Callback()->HandleCommandComplete(iMedia, aError);
            aPlugin.CompleteTask();
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            break;
        case EMcsRemovePath:
            {
            CDesCArray* dummy(NULL); // object will not be dereferenced in callback
            aPlugin.Callback()->HandleRemove(*dummy, aError);
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            aPlugin.CompleteTask();
            }
            break;
        case EMcsFindAll:
            aPlugin.Callback()->HandleFindAll(iMedia, aError);
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            aPlugin.CompleteTask();
            break;
        case EMcsCommand:
            {
            // At least complete the task to not jam up the task queue
            aPlugin.SetCallback(NULL); // Reset current observer
            aPlugin.SetObserver(iEngine);
            aPlugin.CompleteTask();
            break;
            }
        default:
            break;
        }
    if (aUnusePlugin)
        {
        iEngine.ReleasePlugin(aPlugin.Uid());
        }
    MPX_DEBUG1("<--CMPXCollectionClientContext::HandleError");
    }

// ----------------------------------------------------------------------------
// Error happens upon request
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ResolvePluginL(
    const CMPXCollectionPath& aPath,
    TUid& aUid)
    {
    CMPXCollectionPlugin* p= iEngine.LoadedPlugin(aUid); // Save old plugin
    // Resolve new plugin
    CMPXCollectionPlugin* plugin=iEngine.ResolvePluginL(aPath);
    // Update with new Uid
    aUid = plugin->Uid();
    if (p != plugin && &aPath == iBrowsePath)
        {
        // browsing plugin changed
        iClientList->SendMsgL(TMPXCollectionMessage(
                 TMPXCollectionMessage::ECollectionChanged,0,aUid.iUid));
        }

    if (p)
        {
        // Make sure we decrement the reference count for the old plugin
        iEngine.ReleasePlugin(p->Uid());
        }
    }

// ----------------------------------------------------------------------------
// resolve plugin for FindAllL
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::ResolvePluginL(
    const CMPXMedia& aMedia,
    CMPXCollectionPlugin*& aPlugin)
    {
    // We have to be finding from the same context as browse
    // if we are from MPX UI
    //
    if (iPluginUids[EContextBrowse]!=KNullUid)
        {
        aPlugin = LoadedPlugin(EContextBrowse);

        // Increment the reference count manually
        iEngine.UsePlugin(aPlugin->Uid());
        }
    else // browse context does not exist
        {
        if (aMedia.IsSupported(KMPXMediaGeneralCollectionId))
            {
            TUid col = aMedia.ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);

            // Increment the plugin reference count
            aPlugin = iEngine.ResolvePluginL(col);
            }
        else if (aMedia.IsSupported(KMPXMediaGeneralUri))
            {
            const TDesC& uri = aMedia.ValueText(KMPXMediaGeneralUri);

            // Increment the plugin reference count
            aPlugin = iEngine.ResolvePluginL(uri);
            }
        else
            {
            // Unable to find a collection plugin
            User::Leave(KErrNotSupported);
            }
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::SetPlugin
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::SetPlugin(
    TContextType aType,
    CMPXCollectionPlugin* aPlugin)
    {
    if (iPluginUids[aType]!=KNullUid)
        {
        iEngine.ReleasePlugin(iPluginUids[aType]);
        }
    if (aPlugin)
        {
        iPluginUids[aType] = aPlugin->Uid();
        }
    else
        {
        iPluginUids[aType] = KNullUid;
        }
    }

// ----------------------------------------------------------------------------
// Add a media object to the collection
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoUpdateMediaL( TInt aOp, const CMPXMedia& aMedia )
    {
    // Media object can be a single "item" or a list of "items"
    //
    TMPXGeneralType type = EMPXNoType;
    if (aMedia.IsSupported(KMPXMediaGeneralType))
        {
        type = aMedia.ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType);
        }
    else
        {
        User::Leave(KErrArgument);
        }

    if ( type == EMPXGroup )
        {
        // Group has to have a "container"
        MPX_ASSERT(aMedia.IsSupported(KMPXMediaArrayContents));
        const CMPXMediaArray* array =
                        aMedia.Value<CMPXMediaArray>(KMPXMediaArrayContents);
        User::LeaveIfNull(const_cast<CMPXMediaArray*>(array));
        TInt count = array->Count();
        for( TInt i=0; i<count; ++i )
            {
            DoUpdateMediaL( aOp, *(array->AtL(i)) );
            }
        }
    else if ( type == EMPXItem )
        {
        DoHandleItemL( aOp, aMedia );
        }
    else
        {
        // How do we handle "group" artists? 
        User::Leave( KErrNotSupported );
        }
    }

// ----------------------------------------------------------------------------
// Add the media object to the correct plugin
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleItemL( TInt aOp, const CMPXMedia& aMedia)
    {
    MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL <---");

    // Find the correct plugin.
    // The code below increments the plugin reference count
    CMPXCollectionPlugin* plugin(NULL);
    if (aMedia.IsSupported(KMPXMediaGeneralCollectionId))
        {
        const TUid& uid = aMedia.ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        MPX_DEBUG2("CMPXCollectionClientContext::DoHandleItemL Collection %i", uid.iUid);

        // Increment the plugin reference count
        plugin = iEngine.ResolvePluginL( uid );
        }
    else if (aMedia.IsSupported(KMPXMediaGeneralUri))
        {
        const TDesC& uri = aMedia.ValueText(KMPXMediaGeneralUri);
        MPX_DEBUG2("CMPXCollectionClientContext::DoHandleItemL Collection %S", &uri);

        // Increment the plugin reference count
        plugin = iEngine.ResolvePluginL( uri );
        }
    else
        {
        MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL Cannot resolve a plugin");
        User::Leave(KErrNotSupported);
        }

    if( !plugin )
        {
        MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL Cannot resolve a plugin");
        User::Leave(KErrNotSupported);
        }

    iEngine.CleanupPluginPushL(plugin);

    switch( aOp )
        {
        case EMcsAddItem:
            {
            MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL Add");

            plugin->SetObserver(*this);
            plugin->AddL( aMedia );
            plugin->SetObserver(iEngine);
            break;
            }
        case EMcsRemoveItem:
            {
            MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL Remove");
            plugin->SetObserver(*this);
            plugin->RemoveL( aMedia );
            plugin->SetObserver(iEngine);
            break;
            }
        case EMcsSetMedia:
            {
            MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL Set");
            plugin->SetObserver(*this);
            plugin->SetL( aMedia );
            plugin->SetObserver(iEngine);
            break;
            }
        default:
            {
            User::Leave(KErrNotSupported);
            break;
            }
        }  // switch

    CleanupStack::PopAndDestroy();    // plugin

    MPX_DEBUG1("CMPXCollectionClientContext::DoHandleItemL --->");
    }

// ----------------------------------------------------------------------------
// Handle a synchorouns command to the collection
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleSyncCommandL(
    const CMPXCommand& aCmd,
    const CMPXMessageQueue& aMsgQueue,
    CMPXCollectionPlugin* aPlugin/*=NULL*/)
    {
    // Only process the command if the command id has been defined,
    // otherwise leave with KErrArgument
    if (aCmd.IsSupported(KMPXCommandGeneralId))
        {
        TMPXCommandId commandId = aCmd.ValueTObjectL<TMPXCommandId>(KMPXCommandGeneralId);
        switch( commandId )
            {
            case KMPXCommandIdCollectionSelect:
                {
                DoHandleSelectCommandL( aCmd );
                break;
                }

            case KMPXCommandSubscriptionAdd:
                {
                TInt index( iClientList->Find( aMsgQueue ));
                CMPXMediaArray* items(
                    aCmd.Value<CMPXMediaArray>( KMPXCommandSubscriptionAddItems ));
                User::LeaveIfNull(items);
                CMPXSubscription* subscription( CMPXSubscription::NewL( *items ));
                 CleanupStack::PushL(subscription);
                 iClientList->AddSubscriptionL( index, subscription );   // ownership transferred
                 CleanupStack::Pop(subscription);
                break;
                }
            case KMPXCommandSubscriptionRemove:
                {
                TInt index( iClientList->Find( aMsgQueue ));
                CMPXMediaArray* items(
                    aCmd.Value<CMPXMediaArray>( KMPXCommandSubscriptionAddItems ));
                User::LeaveIfNull(items);
                CMPXSubscription* subscription( CMPXSubscription::NewL( *items ));
                 CleanupStack::PushL(subscription);
                 iClientList->RemoveSubscriptionL( index, *subscription );
                 CleanupStack::PopAndDestroy(subscription);
                break;
                }
            case KMPXCommandSubscriptionRemoveAll:
                {
                TInt index( iClientList->Find( aMsgQueue ));
                 iClientList->RemoveAllSubscriptionsL( index );
                break;
                }
                
           case KMPXCommandIdCollectionPrepareDelete:
                {
                if (aCmd.IsSupported (KMPXCommandCollectionPrepareRemovePath))
                    {
                    CMPXCollectionPath
                            * path = aCmd.ValueCObjectL<CMPXCollectionPath> (KMPXCommandCollectionPrepareRemovePath);
                    CleanupStack::PushL(path);
                    iEngine.ResetCacheL(*path);
                    CleanupStack::PopAndDestroy(path);
                    }
                break;
                }

            default:
                {
                MPX_ASSERT(aPlugin);
                aPlugin->SetObserver(*this);
                TRAPD( err, aPlugin->CommandL( const_cast<CMPXCommand&>(aCmd) ) );
                aPlugin->SetObserver(iEngine);
                User::LeaveIfError( err );
                break;
                }
            }
        }
    else
        {
        User::Leave(KErrArgument);
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::DoHandleSelectCommandL
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoHandleSelectCommandL(const CMPXCommand& aCmd)
    {
    TInt index(iIndex);

    // Selecting a new index
    if( aCmd.IsSupported(KMPXCommandCollectionSelectIndex) )
        {
        index = aCmd.ValueTObjectL<TInt>(KMPXCommandCollectionSelectIndex);
        if (iBrowsePath->Levels()<=0)
            {
            User::Leave(KErrNotReady);
            }
        if (index<0 || index>=iBrowsePath->Count())
            {
            User::Leave(KErrArgument);
            }
        iBrowsePath->Set(index);
        }
    // Re-select the current index
    else
        {
        TInt count = iBrowsePath->Count();

        if( iFocusItemId != KMPXInvalidItemId )
            {
            TInt index = iBrowsePath->IndexOfId(iFocusItemId);
            if (KErrNotFound != index)
                {
                iBrowsePath->Set(index);
                }
            else if( count > 0 ) // Focus item has been deleted, select next
                {
                // Bounds check for largest and smallest
                if( iIndex >= count )
                    {
                    iIndex = count-1;
                    }
                if( iIndex < 0 )
                    {
                    iIndex = 0;
                    }
                iBrowsePath->Set(iIndex);
                iFocusItemId = iBrowsePath->Id();
                }
            iIndex = iBrowsePath->Index();
            }
        else
            {
            // Bounds checking for iIndex
            if( iIndex >= count && iIndex > 0 )
                {
                iIndex = count-1;

                // Just in case if ids has 0 items
                if( iIndex > 0 && iIndex < count )
                    {
                    iFocusItemId = iBrowsePath->IdOfIndex(iIndex);
                    }
                }

            if (iBrowsePath->Levels()>0 && iIndex>=0 &&
                iIndex<iBrowsePath->Count())
                {
                iBrowsePath->Set(iIndex);
                }
            iFocusItemId = iBrowsePath->Id();
            }
        }
    iClientList->SendMsgL(
          TMPXCollectionMessage(TMPXCollectionMessage::EFocusChanged,
                                index, index));
    iIndex = index; // iIndex will always be up to date
    iFocusItemId = iBrowsePath->Id();
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::DoPluginOpenL
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoPluginOpenL()
    {
    MPX_FUNC("CMPXCollectionClientContext::DoPluginOpenL()");
    TBool callOpen(ETrue);
    MPX_ASSERT(iPluginUids[EContextBrowse]!=KNullUid);
    TBool pluginCacheable(EFalse);
    if( iBrowsePath )
        {
        TMPXItemId id(iBrowsePath->Id(CMPXCollectionPath::ECollectionUid) );
        pluginCacheable = iEngine.PluginCacheable( TUid::Uid( id ) );
        }

    // Check for open playlist only mode.  If in that mode, do not return the media
    // from the cache but call the plugin to open, as that will callback a
    // different HandleOpenL() with the collection path instead.
    TMPXOpenMode mode = EMPXOpenDefault;
    if(iBrowsePath)
        {
        mode = iBrowsePath->OpenNextMode();
        }
    CMPXCollectionPlugin* plugin = LoadedPlugin(EContextBrowse);
    if ( !iFilter && mode != EMPXOpenPlaylistOnly )
        {
        // try to get the results from the cache only if the plugin is cacheable
        if ( pluginCacheable )
            {
            CMPXMedia* results( iCache.GetL( *iBrowsePath,
                                             iBrowsePath->OpenAttributes(),
                                             ETrue ));
            if (results)
                {
                MPX_DEBUG1("CMPXCollectionClientContext::DoPluginOpenL(): Results found in cache");
                MMPXCollectionEngineObserver* callback = plugin->Callback();
                plugin->CompleteTask();
                TRAPD(err, DoHandleOpenL(results, NULL, callback, KErrNone, EFalse, ETrue));
                if (err)
                    {
                    HandleError(*plugin, err);
                    }

                callOpen = EFalse;
                }
            }
        }

    if (callOpen)
        {
        if ( pluginCacheable )
            {
            iCacheMedia = AttributesCacheableL( iBrowsePath->OpenAttributes(), *iBrowsePath );
            }
        if( iBrowsePath )
            {
            plugin->OpenL(*iBrowsePath, iBrowsePath->OpenAttributes(), iFilter);
            }
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::DoPluginMediaL
// ----------------------------------------------------------------------------
//
void CMPXCollectionClientContext::DoPluginMediaL(
    CMPXCollectionPath& aPath,
    const TCapabilitySet& aCaps,
    CMPXAttributeSpecs& aSpecs,
    CMPXFilter& aFilter)
    {
    MPX_FUNC("CMPXCollectionClientContext::DoPluginMediaL()");
    TBool callMedia(ETrue);
    delete iMediaPath;
    iMediaPath = NULL;

    TMPXItemId id(aPath.Id(CMPXCollectionPath::ECollectionUid) );
    TBool pluginCacheable( iEngine.PluginCacheable( TUid::Uid( id ) ));

    // try to get the results from the cache only if
    // the plugin is cacheable
    CMPXMedia* results( NULL );
    if ( pluginCacheable )
        {
        results = iCache.GetL( aPath, aPath.OpenAttributes() );

        if (results)
            {
            // Check if the attribute specs match
            if ( aSpecs.Count() > 0 )
                {
                /*
                // need to check if requested attribute specs
                // match what we have cached
                if ( results->IsSupported( KMPXCommandMediaAttributeSpecs ))
                    {
                    CMPXAttributeSpecs* specs( results->Value<CMPXAttributeSpecs>(
                                                KMPXCommandMediaAttributeSpecs ));
                    User::LeaveIfNull(specs);
                    if ( *specs == *aSpecs )
                        {
                        callMedia = EFalse;
                        }
                    }
                */
                }
            else
                {
                // Else client did not specify attribute specs, so we can return
                // the match
                callMedia = EFalse;
                }
            }
        }

    if ( callMedia )
        {
        iMediaPath = CMPXCollectionPath::NewL( aPath );

        if ( pluginCacheable )
            {
            iCacheMedia = AttributesCacheableL( aPath.OpenAttributes(), aPath );
            }
        LoadedPlugin(EContextMedia)->MediaL( aPath,
                                             aPath.OpenAttributes(),
                                             aCaps,
                                             &aSpecs,
                                             &aFilter);
        }
    else
        {
        HandleMedia( results, KErrNone );
        }
    }


// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::AttributesCacheableL
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionClientContext::AttributesCacheableL(
    const TArray<TMPXAttribute>& aAttrs,
    const CMPXCollectionPath& aPath )
    {
    MPX_DEBUG1("-->CMPXCollectionClientContext::AttributesCacheableL");

    TMPXItemId id( aPath.Id( CMPXCollectionPath::ECollectionUid ));
    const TArray<TUid>& nonCacheAttrs( iEngine.PluginNonCacheableAttributesL( TUid::Uid( id )));

    TBool found( EFalse );
    TInt nonCacheCount( nonCacheAttrs.Count() );
    for ( TInt i = 0; i < nonCacheCount && !found; i++ )
        {
        TInt attrCount( aAttrs.Count() );
        for ( TInt j = 0; j < attrCount && !found; j++ )
            {
            TMPXAttribute att( aAttrs[j] );
            if ( nonCacheAttrs[i] == TUid::Uid( att.ContentId() ))
                {
                found = ETrue;
                }
            }
        }
    MPX_DEBUG1("<--CMPXCollectionClientContext::AttributesCacheableL");
    return !found;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::AddToCache
// ----------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionClientContext::AddToCache(
    const CMPXCollectionPath& aPath,
    const TArray<TMPXAttribute>& aAttrs,
    CMPXMedia& aResults,
    TBool aMediaFromOpenL,
    CMPXCollectionCache::TCachePriority aPriority /* = EPriorityNormal */)
    {
    CMPXMedia* ret( &aResults );
    TRAP_IGNORE(ret = iCache.AddL(aPath, aAttrs, aResults, aMediaFromOpenL, aPriority));
    return ret;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionClientContext::LoadedPlugin
// ----------------------------------------------------------------------------
//
CMPXCollectionPlugin* CMPXCollectionClientContext::LoadedPlugin(TContextType aType)
    {
    MPX_ASSERT(iPluginUids[aType]!=KNullUid);
    CMPXCollectionPlugin* plugin(iEngine.LoadedPlugin(iPluginUids[aType]));
    MPX_ASSERT(plugin);
    return plugin;
    }

// End of file
