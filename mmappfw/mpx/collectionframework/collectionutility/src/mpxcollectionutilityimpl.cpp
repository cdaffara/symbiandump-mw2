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
* Description:  Collection Utility implementation
*
*/


#include <bamdesca.h>
#include <s32mem.h>
#include <e32math.h>
#include <mpxmessagemonitor.h>
#include <mpxmedia.h>
#include <mpxcollectionobserver.h>
#include <mpxcollectionmessage.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxcmn.h>
#include <mpxtaskqueue.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionplaylist.h>
#include <mpxcollectiontype.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxsubscription.h>
#include <mpxlog.h>

#include "mpxcollectionutilityimpl.h"

// ============================== MEMBER FUNCTIONS ============================
 
// ----------------------------------------------------------------------------
// Two phase constructor
// ----------------------------------------------------------------------------
// 
CMPXCollectionUtility* CMPXCollectionUtility::NewL(
    const TUid& aModeId,
    MMPXCollectionObserver* aObs)
    {
    CMPXCollectionUtility* p=new(ELeave)CMPXCollectionUtility(aObs);
    CleanupStack::PushL(p);
    p->ConstructL(aModeId);
    CleanupStack::Pop(p);
    return p;
    }
 
// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
// 
CMPXCollectionUtility::~CMPXCollectionUtility()
    { 
    Cancel();
#ifdef _ENABLE_GUARD_TIMER
    delete iGuardTimer;
#endif
    if (iTaskQueue)
        {
        delete iTaskQueue;
        }
 
    delete iCurrentEntries;
 
    if (iMsgMonitor)
        {
        delete iMsgMonitor;
        }

    iMcs.Close();

    delete iBuffer;
    delete iArray;
    delete iMedia;
    delete iMediaOnError;
    }

// ----------------------------------------------------------------------------
// C++ constructor
// Create a unique name out of thread ID and this pointer: no other instance of
// this object will have the same name; used to identify this object for
// receiving messages 
// ----------------------------------------------------------------------------
// 
CMPXCollectionUtility::CMPXCollectionUtility(MMPXCollectionObserver* aObs)
:   CActive(EPriorityStandard),
    iObserver(aObs),
    iCallbackOngoing(EFalse)
    { 
    CActiveScheduler::Add(this); 
    TThreadId threadId=RThread().Id(); // this thread id
    } 
 
// ----------------------------------------------------------------------------
// 2nd phase constructor
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::ConstructL(const TUid& aModeId)
    {
    MPX_FUNC_EX("CMPXCollectionUtility::ConstructL");
    iTaskQueue = CMPXTaskQueue::NewL();
    User::LeaveIfError(
        iMcs.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    iMcs.SendReceiveL(EMcsSetMode,TIpcArgs(aModeId.iUid)); 
    iMsgMonitor = CMPXMessageMonitor::NewL(iMcs,*this);
    iMediaOnError = CMPXMedia::NewL();
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer=CPeriodic::NewL(CActive::EPriorityStandard);
#endif
    }
 
// ----------------------------------------------------------------------------
// Return reference to collection
// ----------------------------------------------------------------------------
// 
MMPXCollection& CMPXCollectionUtility::Collection()
    {
    return *this;
    }

// ----------------------------------------------------------------------------
// Retrieves the ID of the collection resolved based on selection criteria
// ----------------------------------------------------------------------------
// 
TUid CMPXCollectionUtility::CollectionIDL(const TArray<TUid>& aUids)
    {
    // Buffer to transfer UIDs
    //
    CBufBase* buf=CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL( buf );

    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    ::ExternalizeL(aUids, writeStream);
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy(&writeStream);
 
    // IPC the data
    //
    TPtr8 des = buf->Ptr(0);
    TPckgBuf<TUid> pkg;
    iMcs.SendReceiveL( EMcsCollectionIdLookup, TIpcArgs(&des, &pkg) );
    CleanupStack::PopAndDestroy( buf );
 
    return pkg();
    }
 
// ----------------------------------------------------------------------------
// CMPXCollectionUtility::AddSubscriptionL
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::AddSubscriptionL(
    const CMPXSubscription& aSubscription)
    {
    const CMPXMediaArray* items = aSubscription.ItemsL();
 
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionAdd);
    cmd->SetCObjectValueL(KMPXCommandSubscriptionAddItems, items);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd); 
    }
 
// ----------------------------------------------------------------------------
// CMPXCollectionUtility::RemoveSubscriptionL
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::RemoveSubscriptionL(
    const CMPXSubscription& aSubscription)
    {
    const CMPXMediaArray* items = aSubscription.ItemsL();
 
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionRemove);
    cmd->SetCObjectValueL(KMPXCommandSubscriptionAddItems, items);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd); 
    }
 
// ----------------------------------------------------------------------------
// CMPXCollectionUtility::ClearSubscriptionsL
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::ClearSubscriptionsL()
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionRemoveAll);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd);
    } 

// ----------------------------------------------------------------------------
// Destroy this object
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::Close()
    {
    MPX_DEBUG2("CMPXCollectionUtility::Close %08x", this);
    iMsgMonitor->Cancel();
    delete this;
    } 

// ----------------------------------------------------------------------------
//  Open's collection with the mode. Data returned in callback
// ----------------------------------------------------------------------------
//
void CMPXCollectionUtility::OpenL(TMPXOpenMode aMode)
    {
    MPX_DEBUG2("CMPXCollectionUtility::OpenL with mode %d", aMode);
    AddRequestL(EMcsOpen, NULL, aMode);
    }
 
// ----------------------------------------------------------------------------
// Open a collection
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::OpenL(TInt aIndex,TMPXOpenMode aMode)
    {
    RArray<TMPXAttribute> dummy;
    CleanupClosePushL(dummy);
    OpenL(aIndex, dummy.Array(), aMode);
    CleanupStack::PopAndDestroy(&dummy); 
    }

// ----------------------------------------------------------------------------
// Open a collection by path
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::OpenL(
    const CMPXCollectionPath& aPath,
    TMPXOpenMode aMode)
    {
    RArray<TMPXAttribute> dummy;
    CleanupClosePushL(dummy);
    OpenL(aPath, dummy.Array(), aMode);
    CleanupStack::PopAndDestroy(&dummy); 
    }

// ----------------------------------------------------------------------------
//  Opens the collection at a specific index 
// ----------------------------------------------------------------------------
//
void CMPXCollectionUtility::OpenL(
    TInt aIndex,
    const TArray<TMPXAttribute>& aAttrs,
    TMPXOpenMode aMode)
    {
    // Externalize parameters
    //
    CBufBase* buf=CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL( buf );

    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    // Mode
    writeStream.WriteInt32L(aMode); 
    // externalize attributes array
    ::ExternalizeL(aAttrs, writeStream);
    // Close and compress buffer
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy(&writeStream);
 
    // Async version, Add request to the task queue 
    AddRequestL(EMcsOpenIndex, NULL, aIndex, buf);
    CleanupStack::Pop( buf );  // ownership transferred to the queue
    }

// ----------------------------------------------------------------------------
//  Opens the collection at a specific level 
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::OpenL(
    const CMPXCollectionPath& aPath,
    const TArray<TMPXAttribute>& aAttrs,
    TMPXOpenMode aMode)
    {
    MPX_FUNC("CMPXCollectionUtility::OpenL(const CMPXCollectionPath& aPath)");
    MPX_DEBUG_PATH(aPath);
    CBufBase* buffer=CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    //collection path 
    writeStream << aPath; 
    // externalize open mode
    writeStream.WriteInt32L(aMode);
    // externalize attributes array
    ::ExternalizeL(aAttrs, writeStream);
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy(&writeStream);
    AddRequestL(EMcsOpenPath, NULL, 0, buffer);
    CleanupStack::Pop(buffer); //buffer ownership transferred
    }

// ----------------------------------------------------------------------------
//  Opens the collection in its current state, or if aUids do not
//  apply to currently opened collection, then the state will
//  revert back to root level
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::OpenL(
    const TArray<TUid>& aUids,
    TMPXOpenMode aMode)
    {
    MPX_FUNC_EX("CMPXCollectionUtility::OpenL(aUids, aMode)");
    CBufBase* buf=CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL( buf );
    // Externalize parameters
    //
    RArray<TUid> uids;
    CleanupClosePushL(uids);
    TInt count(aUids.Count());
    for (TInt i=0; i<count; ++i)
        { // sort uids
        uids.InsertInOrderL(aUids[i], MPXUser::CompareUids);
        }
    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    // Mode
    writeStream.WriteInt32L(aMode); 
    // Uids
    ::ExternalizeL(uids.Array(), writeStream);
    // Close and compress buffer
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy(&writeStream);
 
    CleanupStack::PopAndDestroy(&uids);
    // Async version, Add request to the task queue 
    AddRequestL(EMcsOpenByUids, NULL, 0, buf);
    CleanupStack::Pop( buf );  // ownership transferred to the queue
    }

// ----------------------------------------------------------------------------
//  Opens the collection in its current state, or if aUid does not
//  apply to currently opened collection, then the state will
//  revert back to root level
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::OpenL(
    const TUid& aUid,
    TMPXOpenMode aMode)
    {
    RArray<TUid> uids;
    CleanupClosePushL(uids);
    uids.AppendL(aUid);
    OpenL(uids.Array(),aMode);
    CleanupStack::PopAndDestroy(&uids);
    }

// ----------------------------------------------------------------------------
//  Apply a filter to collection browsing; all subsequent calls
//  to OpenL() will have this filter applied
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::SetFilterL(CMPXFilter* aFilter)
    {
    if (aFilter)
        { // Send filter to server
        CBufBase* buf=NULL;
        ::CreateBufferL<CMPXFilter>(*aFilter,buf);
        CleanupStack::PushL(buf);
        TPtr8 ptr=buf->Ptr(0);
        iMcs.SendReceiveL(EMcsSetFilter,TIpcArgs(&ptr));
        CleanupStack::PopAndDestroy(buf);
        }
    else
        { // Else send zero length descriptor
        iMcs.SendReceiveL(EMcsSetFilter,TIpcArgs(&KNullDesC8));
        }
    }

// ----------------------------------------------------------------------------
//  Current filter that is applied to collection browsing
// ----------------------------------------------------------------------------
// 
CMPXFilter* CMPXCollectionUtility::FilterL()
    {
    TInt size=iMcs.SendReceiveL(EMcsFilter);
    // 
    // Transfer results from server
    //
    CMPXMedia* filter=NULL;
    if (size)
        {
        ::TransferBufferFromServerL(iMcs,EMcsGetSyncBuffer,size,iBuffer);
        ::NewFromBufferL<CMPXFilter>(*iBuffer,filter); 
        }
    return filter;
    }

// ----------------------------------------------------------------------------
// The UID identifying this collection
// ----------------------------------------------------------------------------
// 
TUid CMPXCollectionUtility::UidL() const
    {
    TPckgBuf<TInt> uidPkg; 
    iMcs.SendReceiveL(EMcsGetUid, TIpcArgs(&uidPkg));
    return TUid::Uid(uidPkg());
    }
 
// ----------------------------------------------------------------------------
// Return current collection path
// ----------------------------------------------------------------------------
// 
CMPXCollectionPath* CMPXCollectionUtility::PathL()
    {
    CMPXCollectionPath* path = NULL;
    TInt size = iMcs.SendReceiveL(EMcsGetPath);
    if (size >0)
        {
        ::TransferBufferFromServerL(iMcs, EMcsGetSyncBuffer, size, iBuffer);
        ::NewFromBufferL<CMPXCollectionPath>(*iBuffer, path);
        }
    return path;
    }

// ----------------------------------------------------------------------------
// Back to upper level
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::BackL()
    {
    AddRequestL(EMcsBack, NULL);
    }

// ----------------------------------------------------------------------------
// Is remote collection path
// ----------------------------------------------------------------------------
// 
TBool CMPXCollectionUtility::IsRemote(const CMPXCollectionPath& aPath)
    {
    (void)aPath; 
    return EFalse; 
    }

// ----------------------------------------------------------------------------
// Remove outstanding requests
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::CancelRequest()
    {
    Cancel();
    iTaskQueue->Reset();
    }

// ----------------------------------------------------------------------------
// Add item(s)
// ----------------------------------------------------------------------------
//
void CMPXCollectionUtility::AddL(const CMPXMedia& aNewProperties)
    {
    delete iMedia;
    iMedia=NULL;
    iMedia=CMPXMedia::NewL(aNewProperties);
 
    CBufBase* buf(NULL);
    ::CreateBufferL<CMPXMedia>( aNewProperties, buf );
    CleanupStack::PushL( buf );
    TPtr8 ptr = buf->Ptr(0);
    iMcs.SendReceiveL( EMcsAddItem, TIpcArgs(&ptr));
    CleanupStack::PopAndDestroy( buf );
    }

// ----------------------------------------------------------------------------
// Remove a single item
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::RemoveL( 
    const CMPXCollectionPath& aPath,
    MMPXCollectionRemoveObserver* aObs )
    {
    // Serialize the path and send it to the collection
    //
    if (aPath.Levels()<=0)
        {
        User::Leave(KErrArgument);
        }
    CBufBase* buf(NULL);
    ::CreateBufferL<CMPXCollectionPath>( aPath, buf );
    CleanupStack::PushL(buf);
    AddRequestL(EMcsRemovePath, aObs, 0, buf);  // buf ownership transferred
    CleanupStack::Pop(buf);
    }

// ----------------------------------------------------------------------------
// Remove a list of items
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::RemoveL(const CMPXMedia& aProperties)
    {
    delete iMedia;
    iMedia=NULL;
    iMedia=CMPXMedia::NewL(aProperties);
 
    CBufBase* buf(NULL);
    ::CreateBufferL<CMPXMedia>( aProperties, buf );
    CleanupStack::PushL( buf );
    TPtr8 ptr = buf->Ptr(0);
    iMcs.SendReceiveL( EMcsRemoveItem, TIpcArgs(&ptr));
    CleanupStack::PopAndDestroy( buf );
    }

// ----------------------------------------------------------------------------
// Set properties
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::SetSyncL(const CMPXMedia& aMedia)
    {
    delete iMedia;
    iMedia=NULL;
    iMedia=CMPXMedia::NewL(aMedia);
 
    CBufBase* buf(NULL);
    ::CreateBufferL<CMPXMedia>( aMedia, buf );
    CleanupStack::PushL( buf );
    TPtr8 ptr = buf->Ptr(0);
    iMcs.SendReceiveL( EMcsSetMedia, TIpcArgs(&ptr));
    CleanupStack::PopAndDestroy( buf );
    }

// ----------------------------------------------------------------------------
// Set properties Asynchronous
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::SetL(const CMPXMedia& /*aMedia*/)
    {
    ASSERT(0); // DEPRECATED, please use CommandL(CMPXCommand& aCmd)
    }

// ----------------------------------------------------------------------------
// Find media
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::FindAllL(const CMPXMedia& aMedia,
                                     const TArray<TMPXAttribute>& aAttrs,
                                     MMPXCollectionFindObserver& aObs)
    {
    // Externalize parameters
    //
    CBufBase* buf(NULL);
    ::CreateBufferL(aAttrs, buf);
    CleanupStack::PushL(buf);
    // Increase reference count on media ownership transferred
    CMPXMedia* ref = CMPXMedia::NewL( aMedia ); 
    CleanupStack::PushL(ref);
    AddRequestL(EMcsFindAll, &aObs, aMedia.Data(), buf, NULL, ref);
    // Async version, Add request to the task queue 
    CleanupStack::Pop(ref);
    CleanupStack::Pop( buf );  // ownership transferred to the queue
    }

// ----------------------------------------------------------------------------
// Find media sync
// ----------------------------------------------------------------------------
// 
CMPXMedia* CMPXCollectionUtility::FindAllL(const CMPXMedia& aMedia,
                                           const TArray<TMPXAttribute>& aAttrs)
    {
    CMPXMedia* media=CMPXMedia::NewL(aMedia); // keep media alive
    CleanupStack::PushL(media);
 
    CBufBase* buf(NULL);
    ::CreateBufferL(aAttrs, buf);
    CleanupStack::PushL(buf);
 
    // Synchronous version, transfer directly
    TPtr8 ptr = buf->Ptr(0);
    TInt size = iMcs.SendReceiveL( EMcsFindAll, 
                                   TIpcArgs(&ptr, 1, media->Data()) );
    CleanupStack::PopAndDestroy( buf );
 
    // Transfer results from server
    //
    CMPXMedia* results(NULL);
    ::TransferBufferFromServerL( iMcs, EMcsGetSyncBuffer, 
                                 size, iBuffer );
    ::NewFromBufferL<CMPXMedia>( *iBuffer, results );
    CleanupStack::PopAndDestroy(media);
    return results;
    }

// ----------------------------------------------------------------------------
// Issue media request
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::MediaL(
    const CMPXCollectionPath& aPath,
    const TArray<TMPXAttribute>& aAttrs,
    CMPXAttributeSpecs* aSpecs/*=NULL*/,
    CMPXFilter* aFilter/*=NULL*/)
    {
    MPX_FUNC_EX("CMPXCollectionUtility::MediaL()");
    MPX_DEBUG_PATH(aPath);
    if (aPath.Levels()<=0)
        {
        User::Leave(KErrArgument);
        }
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);

    CMPXCollectionPath* path = CMPXCollectionPath::NewL(aPath);
    CleanupStack::PushL(path);
    path->Set(aAttrs); // aAttrs should be removed from parameter
                       // Clients should set attribute before API is called
 
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandContentIdMedia);
    cmd->SetCObjectValueL<CMPXCollectionPath>(KMPXCommandGeneralTargetIds, path);
    CMPXAttributeSpecs* attrSpecs = aSpecs ? 
            CMPXAttributeSpecs::NewL(*aSpecs) : CMPXAttributeSpecs::NewL();
    CleanupStack::PushL(attrSpecs);
    cmd->SetCObjectValueL<CMPXAttributeSpecs>(KMPXCommandMediaAttributeSpecs, attrSpecs);
    CleanupStack::PopAndDestroy(attrSpecs);
    CMPXFilter* filter = aFilter ? CMPXFilter::NewL(*aFilter) : CMPXFilter::NewL();
    CleanupStack::PushL(filter);
    cmd->SetCObjectValueL<CMPXFilter>(KMPXCommandMediaFilter, filter);
    CleanupStack::PopAndDestroy(filter);
    //call CommandL in the future. All APIs will be mapped to CommandL
    AddRequestL(EMcsMediaByPath, NULL, 0, NULL, (TAny*)cmd, cmd);
    CleanupStack::PopAndDestroy(path);
    CleanupStack::Pop(cmd); // Ownership transferred to the task queue
    }

// ----------------------------------------------------------------------------
// Add a request to broadcast a message
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::NotifyL( TMPXCollectionBroadCastMsg aMsg, 
                                     TInt aData )
    {
    // Synchronous, for closing server file handles
    iMcs.SendReceiveL( EMcsNotifyEvent,
                       TIpcArgs((TInt)aMsg, aData));
    }

// ----------------------------------------------------------------------------
// Send a Command to the collection
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::CommandL(TMPXCollectionCommand aCmd, TInt aData )
    {
    // Synchronous, send a command to the server
    iMcs.SendReceiveL( EMcsCommand, TIpcArgs( (TInt)aCmd, aData ) );
    }
 
// ----------------------------------------------------------------------------
// Send a Command to the collection
// ----------------------------------------------------------------------------
//
void CMPXCollectionUtility::CommandL(CMPXCommand& aCmd)
    {
    TBool sync(EFalse); // by default command is asynchronous
    if (aCmd.IsSupported(KMPXCommandGeneralDoSync))
        { // check if command is sync
        sync=(aCmd.ValueTObjectL<TBool>(KMPXCommandGeneralDoSync));
        }
    if (sync)
        { // sync request
        iMcs.SendReceiveL(EMcsCommandExt, TIpcArgs(EFalse, aCmd.Data()));
        }
    else
        { // async request
        // Increase reference count on command ownership transferred
        CMPXCommand* ref = CMPXCommand::NewL(aCmd); 
        // async version, Add request to the task queue 
        CleanupStack::PushL(ref);
        AddRequestL(EMcsCommandExt, NULL, ETrue, NULL, (TAny*)ref, ref );
        CleanupStack::Pop(ref); // Ownership transferred to the task queue
        }
    }

// ----------------------------------------------------------------------------
// Retrieve the list of supported mime types
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::GetSupportedTypesL( 
    RPointerArray<CMPXCollectionType>& aArray )
    {
    TInt size = iMcs.SendReceiveL( EMcsGetSupportedTypes );
    ::ArrayFromServerL( iMcs, EMcsGetSyncBuffer, size, aArray );
    }

// ----------------------------------------------------------------------------
// Retrieve the bitmask of supported capabilities of the collection
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::GetSupportedCapabilitiesL(
    TCollectionCapability& aCapability)
    {
    TPckgBuf<TCollectionCapability> pkg;
    iMcs.SendReceiveL( EMcsGetCapabilities, TIpcArgs(&pkg) );
    aCapability = pkg();
    }

// ----------------------------------------------------------------------------
// Retrieves the actual collection implementation UID
// ----------------------------------------------------------------------------
// 
TUid CMPXCollectionUtility::CollectionIDL(TUid& aCollection)
    {
    TPckgBuf<TUid> pkg( aCollection );
    iMcs.SendReceiveL( EMcsCollectionID, TIpcArgs(&pkg) );
 
    return pkg();
    }

// ----------------------------------------------------------------------------
// Message received 
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::MessageReceived(TInt aMsgData, TInt aError)
    {
    TRAP_IGNORE(HandleMessageL(aMsgData, aError));
    }

// ----------------------------------------------------------------------------
// Handles an active object's request completion event
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::RunL()
    {
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer->Cancel();
#endif
    TInt err=iStatus.Int();
    TInt task = iTaskQueue->Task();
    MPX_DEBUG4("-->CMPXCollectionUtility::RunL 0x%08x task %d err%d", 
               this, task, err);
    if (KErrNotFound == task)
        {
        MPX_DEBUG1("CMPXCollectionUtility::RunL no task");
        return;
        }

    // Try again in case server is down for IAD
    if (err == KErrDied || err == KErrServerTerminated)
        {
        MPX_DEBUG3("-->CMPXCollectionUtility::RunL Reconnecting session for IAD 0x%08x task %d",
                this, task);
        
        // attempt to bring the server back up
        err = iMcs.Reconnect();
        
        if (err != KErrNone)
            {
            MPX_DEBUG1("-->CMPXCollectionUtility::RunL Reconnect Failed");
            }
        }
    
    // If there was an error, callback to observer with error code
    User::LeaveIfError(err);
    
    switch(task)
        {
        case EMcsOpen:
        case EMcsBack:
        case EMcsOpenIndex:
        case EMcsOpenPath:
        case EMcsOpenByUids:
            {
            MPX_DEBUG1("CMPXCollectionUtility::RunL handle open");
            if(err == KMPXCollectionPath && iObserver)
                {
                CMPXCollectionPath* path = PathL();
                CleanupStack::PushL(path);
                if (path->OpenNextMode() != EMPXOpenNoPlaylist)
                    {
                    CMPXCollectionPlaylist* p=CMPXCollectionPlaylist::NewL(*path);
                    CleanupStack::PushL(p);
 
                    if( path->OpenNextMode() == EMPXOpenPlaylistOnly )
                        {
                        p->SetToIndex( KErrNotFound );
                        }
 
                    iCallbackOngoing = ETrue;
                    iObserver->HandleOpenL(*p, KErrNone);
                    iCallbackOngoing = EFalse;
                    CleanupStack::PopAndDestroy(p);
                    }
                CleanupStack::PopAndDestroy(path);
                }
            else if (KMPXCollectionEntries == err && iObserver)
                {
                MPX_DEBUG2("CMPXCollectionUtility::RunL with KMPXCollectionEntries %d", 
                            iSizePckg());
                if (iSizePckg() >0)
                    {
                    ::TransferBufferFromServerL(iMcs,EMcsGetAsyncBuffer, 
                                                iSizePckg(), iBuffer);
                    ::NewFromBufferL<CMPXMedia>(*iBuffer, iCurrentEntries);
                    }
                MPX_DEBUG1("CMPXCollectionUtility::RunL call back client with entries");
                iCallbackOngoing = ETrue;
                iObserver->HandleOpenL(*iCurrentEntries, 
                                       iSelectionPckg(), 
                                       (TBool)iCompletePckg(), 
                                       KErrNone);
                iCallbackOngoing = EFalse;
                } // else message will be received which triggers to read buffer
            break;
            }
         case EMcsMediaByPath:
            {
            CMPXMedia* props = NULL;
            if (iObserver)
                {
                if(iSizePckg() && KMPXCollectionMedia == err)
                    {
                    ::TransferBufferFromServerL(iMcs,EMcsGetAsyncBuffer, 
                                                iSizePckg(), iBuffer);
                    ::NewFromBufferL<CMPXMedia>(*iBuffer, props);
                    err = KErrNone;
                    CleanupStack::PushL(props);
                    }
                else
                    {
                    props = iMediaOnError;
                    }
                iCallbackOngoing = ETrue;
                iObserver->HandleCollectionMediaL(*props, err);
                iCallbackOngoing = EFalse;
                if (props && props != iMediaOnError)
                    {
                    CleanupStack::PopAndDestroy(props);
                    }
                }
            break;
            }
        case EMcsRemovePath:
            {
            MMPXCollectionRemoveObserver* obs = 
                     (MMPXCollectionRemoveObserver*) iTaskQueue->Callback();
 
            // Optional observer callback
            CDesCArray* ary(NULL);
            if( obs && err>0 )
                {
                ::TransferBufferFromServerL(iMcs,EMcsGetAsyncBuffer, 
                                            err, iBuffer);
                MPXUser::CreateFromBufferL(*iBuffer, ary);
                CleanupStack::PushL( ary );
                iCallbackOngoing = ETrue;
                obs->HandleRemoveL( *ary, KErrNone );
                iCallbackOngoing = EFalse;
                CleanupStack::PopAndDestroy( ary );
                } // else error message will be sent later
            break;
            }
        case EMcsFindAll:
            {
            MMPXCollectionFindObserver* obs = 
                        (MMPXCollectionFindObserver*) iTaskQueue->Callback();
            CMPXMedia* results(NULL);
 
            // Any results?
            if( err > 0 )
                {
                if(obs)
                    {
                    ::TransferBufferFromServerL( iMcs, EMcsGetAsyncBuffer, 
                                                 err, iBuffer );
                    ::NewFromBufferL<CMPXMedia>( *iBuffer, results );
                    CleanupStack::PushL( results );
                    iCallbackOngoing = ETrue;
                    obs->HandleFindAllL( *results, ETrue, KErrNone );
                    iCallbackOngoing = EFalse;
                    CleanupStack::PopAndDestroy( results );
                    }
                }
            break;
            }
        case EMcsSetMediaAsync:
            { // DEPRECATED, please use CommandL(CMPXCommand& aCmd)
            ASSERT(0);
            break;
            }
        case EMcsCommandExt:
            {
            CMPXCommand* result = NULL;
            if (iObserver && KErrNone == err)
                {
                if(iSizePckg())
                    {
                    ::TransferBufferFromServerL(iMcs,EMcsGetAsyncBuffer, 
                                                iSizePckg(), iBuffer);
                    ::NewFromBufferL<CMPXCommand>(*iBuffer, result);
                    }
                CleanupStack::PushL(result);
                iCallbackOngoing = ETrue;
                iObserver->HandleCommandComplete(result, KErrNone);
                iCallbackOngoing = EFalse;
                CleanupStack::PopAndDestroy(result);
                }
            break;
            }
        default:
            ASSERT(0);
        }
    iTaskQueue->RemoveTask();
    ExecuteNextRequest();
    MPX_DEBUG4("<--CMPXCollectionUtility::RunL 0x%08x task %d err%d", 
               this, task, err);
    }

// ----------------------------------------------------------------------------
// Sends error message to all observers - maybe it should only be to the 
// client that calls the async method?
// ----------------------------------------------------------------------------
// 
TInt CMPXCollectionUtility::RunError(TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionUtility::RunError");
    TRAP_IGNORE(HandleRunErrorL(aError));
    iCallbackOngoing = EFalse;
    iTaskQueue->RemoveTask();
    ExecuteNextRequest();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Handle cancelling request
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::DoCancel()
    {
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer->Cancel();
#endif
    TRAP_IGNORE(iMcs.SendReceiveL(EMcsCancelRequest));
    }

// ----------------------------------------------------------------------------
// Adds a sync request to the queue: if there is no outstanding request,
// it will be executed immediately
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::AddRequestL(
    TMPXCollectionServerOp aFunction,
    TAny* aObs,
    TInt aParamData/*=0*/,
    CBufBase* aBuf/*=NULL*/,
    TAny* aPtr/*=NULL*/,
    CBase* aAlivePtr1/*=NULL*/,
    CBase* aAlivePtr2/*=NULL*/)
    {
    iTaskQueue->AddTaskL(aFunction, aObs, aParamData, aBuf, 
                         aPtr, aAlivePtr1, aAlivePtr2);
    MPX_DEBUG3("CMPXCollectionUtility::AddRequestL this 0x%08x task=%d", 
            this, aFunction);
    if (!IsActive() && !iCallbackOngoing)
        {
        ExecuteNextRequest();
        }
    }
 
// ----------------------------------------------------------------------------
// Executes the next request in the queue. 
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::ExecuteNextRequest()
    {
    TInt task = iTaskQueue->Task();
    if (KErrNotFound != task && !IsActive() &&  !iCallbackOngoing)
        {
        switch(task)
            {
            case EMcsOpenIndex:
                {
                iSelectionPckg = (TInt)iTaskQueue->Param();
                } // deliberate fall through
            case EMcsOpenByUids:
            case EMcsOpenPath:
                {
                iMcs.SendReceive(task, 
                                 TIpcArgs(&iSizePckg,   // [out] size of buffer
                                          &iSelectionPckg, // [in, out] selection index 
                                          &iCompletePckg,  // [out] complete
                                          &iTaskQueue->BufData()), // index, mode and attr
                                 iStatus);
                break;
                }
 
            case EMcsOpen:
                {
                iMcs.SendReceive(EMcsOpen, 
                                 TIpcArgs(&iSizePckg, 
                                          &iSelectionPckg, 
                                          &iCompletePckg,
                                          iTaskQueue->Param()),  // mode
                                 iStatus);
                break;
                }
            case EMcsMediaByPath:
                {
                iMcs.SendReceive(EMcsMediaByPath, 
                                 TIpcArgs(&iSizePckg,
                                         ((CMPXCommand*)(iTaskQueue->PtrData()))->Data()), // command handle
                                 iStatus);
                break;
                } 
            case EMcsBack: 
                {
                iMcs.SendReceive(EMcsBack, 
                                 TIpcArgs(&iSizePckg, 
                                          &iSelectionPckg, 
                                          &iCompletePckg),
                                 iStatus);
                break;
                }
            case EMcsRemovePath: // deliberate fall through
                {
                iMcs.SendReceive( task, 
                                  TIpcArgs(&iTaskQueue->BufData()), 
                                  iStatus);
                break; 
                }
            case EMcsFindAll:
                {
                iMcs.SendReceive( task, 
                                  TIpcArgs(&iTaskQueue->BufData(), // Attrs
                                           0,                    // Not sync
                                           iTaskQueue->Param()), // Media data handle
                                  iStatus );
                break; 
                }
            case EMcsCommandExt:
                {
                iMcs.SendReceive(EMcsCommandExt, 
                                 TIpcArgs(iTaskQueue->Param(), //[in] Async flag
                                          ((CMPXCommand*)(iTaskQueue->PtrData()))->Data(), 
                                                               //[in] command handle 
                                          &iSizePckg), //[out] size of buffer 
                                 iStatus);
                break;
                }
            default:
                ASSERT(0);
            }
    #ifdef _ENABLE_GUARD_TIMER
         iGuardTimer->Start(KMPXGuardTimer,KMPXGuardTimer,
                            TCallBack(GuardTimerCallback,this)); 
    #endif
        SetActive(); 
        MPX_DEBUG3("CMPXCollectionUtility::ExecuteNextRequest 0x%08x task %d ", 
                this, task);
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionUtility::HandleMessageL 
// ----------------------------------------------------------------------------
// 
void CMPXCollectionUtility::HandleMessageL(TInt aMsgData, TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionUtility::HandleMessageL");
    if (iObserver)
        {
        if (aMsgData>0)
            {
            CMPXMessage* msg= CMPXMessage::NewL(aMsgData);
            CleanupStack::PushL(msg);
            MPX_ASSERT(msg->IsSupported(KMPXMessageGeneralId));
            iObserver->HandleCollectionMessage(msg, aError);
            CleanupStack::PopAndDestroy(msg);
            }
        else
            { // NULL message
            iObserver->HandleCollectionMessage(NULL, aError);
            }
        }
    else
        {
        MPX_DEBUG1("CMPXCollectionUtility::HandleMessageL no observer");
        }
    iMsgMonitor->GetNextMessage();
    }


// ----------------------------------------------------------------------------
//  Handle error in RunL
// ----------------------------------------------------------------------------
//
void CMPXCollectionUtility::HandleRunErrorL(TInt aError)
    {
    TInt task = iTaskQueue->Task();
    if (!iCallbackOngoing)
        { // Error happens during preparing callback
        iCallbackOngoing = ETrue; // will be reset to EFalse in the RunError
        switch(task)
            {
            case EMcsOpen:
            case EMcsBack:
            case EMcsOpenIndex:
            case EMcsOpenPath:
            case EMcsOpenByUids:
                {
                if (iObserver)
                    {
                    iObserver->HandleOpenL(*iMediaOnError, KErrNotFound, 
                                           ETrue, aError);
                    }
                break;
                }
             case EMcsMediaByPath:
                {
                if (iObserver)
                    {
                    iObserver->HandleCollectionMediaL(*iMediaOnError, aError);
                    }
                break;
                }
            case EMcsRemovePath:
                {
                MMPXCollectionRemoveObserver* obs = 
                         (MMPXCollectionRemoveObserver*) iTaskQueue->Callback();
                if( obs)
                    {
                    CDesCArray* ary(NULL);
                    obs->HandleRemoveL(*ary, aError); // change HandleRemoveL return pointer
                    } // else error message will be sent later
                break;
                }
            case EMcsFindAll:
                {
                MMPXCollectionFindObserver* obs = 
                            (MMPXCollectionFindObserver*) iTaskQueue->Callback();
                if (obs)
                    {
                    obs->HandleFindAllL( *iMediaOnError, ETrue, aError);
                    }
                break;
                }
            case EMcsCommandExt:
                {
                if (iObserver)
                    {
                    iObserver->HandleCommandComplete(iMediaOnError, aError);
                    }
                break;
                }
            default:
                ASSERT(0);
            }
        } // do nothing if error happens during client processing callback.
          // all callback should be changed into non-leave so that this will
          // be never happen.
    }

#ifdef _ENABLE_GUARD_TIMER
// ----------------------------------------------------------------------------
// Guard timer time out
// ----------------------------------------------------------------------------
// 
TInt CMPXCollectionUtility::GuardTimerCallback(TAny* aPtr)
    {
    CMPXCollectionUtility* cu = static_cast<CMPXCollectionUtility*>(aPtr);
    MPX_DEBUG3("CMPXCollectionUtility::GaurdTimerCallback this 0x%08x task=%d", 
               cu, cu->iTaskQueue->Task());
    cu->iGuardTimer->Cancel();
    ASSERT(0); // the function should never be called
    return KErrNone;
    }
#endif
// End of file
