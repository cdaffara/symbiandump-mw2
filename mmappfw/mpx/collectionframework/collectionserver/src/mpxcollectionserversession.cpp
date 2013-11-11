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
* Description:  Collection server session
*
*/

#include <e32std.h>
#include <e32svr.h>
#include <bamdesca.h>
#include <mpxlog.h>
#include <mpxclientlist.h>
#include <mpxcmn.h>
#include <mpxcollectionpath.h>
#include <mpxcollectiontype.h>
#include <mpxmessagequeue.h>
#include <mpxmediaarray.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include "mpxcollectionserver.h"
#include "mpxcollectionengine.h"
#include "mpxcollectionclientcontext.h"
#include "mpxcollectionserversession.h"

// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Panic client
// ----------------------------------------------------------------------------
//
LOCAL_C void PanicClient(const RMessage2 &aMessage,TInt aPanic)
    {
    _LIT(KTxtServer,"Collection server Session");
    aMessage.Panic(KTxtServer,aPanic);
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXCollectionSession* CMPXCollectionSession::NewL(
    CMPXCollectionEngine& aEngine)
    {
    CMPXCollectionSession* s = new(ELeave)CMPXCollectionSession(aEngine);
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXCollectionSession::CMPXCollectionSession(CMPXCollectionEngine& aEngine)
    : iEngine(aEngine)
    {
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::ConstructL()
    {
    MPX_FUNC_EX("CMPXCollectionSession::ConstructL");
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXCollectionSession::~CMPXCollectionSession()
    {
    MPX_DEBUG2("CMPXCollectionSession::~CMPXCollectionSession this %08x", this);
    CancelRequests();
    if (iContext)
        {
        iContext->RemoveClient(*iMessageQueue);
        }
    static_cast<CMPXCollectionServer*>(
            const_cast<CServer2*>(Server()))->RemoveClient();
    delete iAsyncBuffer;
    delete iSyncBuffer;
    delete iSyncMedia;
    delete iAsyncMedia;
    delete iMessageQueue;
    delete iRootMediaArray;
    delete iMediaCommand;
    delete iMediaPath;
    }

// ----------------------------------------------------------------------------
// Service request
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::ServiceL(const RMessage2& aMessage)
    {
    // by default - change for async in helper methods if required
    iCompleteRequest=ETrue;
    TInt r=KErrNone;
    MPX_TRAPD(err,r = DispatchMessageL(aMessage));
    TBool isErr=(err!=KErrNone);
    // If it's not async, complete now (or an async helper method leaves)
    if(iCompleteRequest)
        {
        aMessage.Complete(isErr ? err : r);
        }
    else  if (isErr) // Async and error,  remove message from message queue
        {
        CompleteAsync(err);
        }
    }

// ----------------------------------------------------------------------------
// Message dispatcher
// ----------------------------------------------------------------------------
//
TInt CMPXCollectionSession::DispatchMessageL(const RMessage2& aMessage)
    {
    TInt r = KErrNone;
    TInt op=aMessage.Function();
    MPX_DEBUG3("-->CMPXCollectionSession::DispatchMessageL %d, this 0x%08x",
               op, this);

    if (op == EMcsSetMode)
        {
        SetModeL(aMessage); // This is the only operation when the iContext
                            // hasn't been defined.
        }
    else if (!iContext)
        {
        MPX_DEBUG1("CMPXCollectionSession::DispatchMessageL no context");
        User::Leave(KErrNotReady);
        }
    else
        {
        CBufBase* buffer(NULL);
        CMPXMedia* media( NULL );
        switch(op)
            {
            case EMcsOpenPath:
                {
                SetAsyncL(aMessage);
                MPXUser::CreateBufferL(aMessage, 3, buffer);
                CleanupStack::PushL(buffer);
                RBufReadStream readStream( *buffer );
                CleanupClosePushL(readStream);
                CMPXCollectionPath* path = CMPXCollectionPath::NewL(readStream);
                CleanupStack::PushL(path);
                // Internalize open mode
                TInt mode = readStream.ReadInt32L();
                if (path->Levels() >0 )
                    {
                    // Internalize attributes
                    RArray<TMPXAttribute> attrs;
                    CleanupClosePushL(attrs);
                    ::InternalizeL(attrs, readStream);
                    path->SetL(attrs.Array());
                    CleanupStack::PopAndDestroy(&attrs);
                    }
                // ownership of path transferred
                iContext->OpenL(path, mode, this);
                CleanupStack::Pop(path);
                CleanupStack::PopAndDestroy(&readStream);
                CleanupStack::PopAndDestroy(buffer);
                break;
                }
            case EMcsOpenIndex:
                {
                SetAsyncL(aMessage);
                TPckgBuf<TInt> indexPkg;
                aMessage.ReadL(1,indexPkg);
                MPXUser::CreateBufferL(aMessage, 3, buffer);
                CleanupStack::PushL(buffer);
                RBufReadStream readStream( *buffer );
                CleanupClosePushL(readStream);
                // Internalize mode
                TInt mode = readStream.ReadInt32L();
                // Internalize attributes
                RArray<TMPXAttribute> attrs;
                CleanupClosePushL(attrs);
                ::InternalizeL(attrs, readStream);
                iContext->OpenL(indexPkg(), mode, attrs.Array(), this);
                CleanupStack::PopAndDestroy(&attrs);
                CleanupStack::PopAndDestroy(&readStream);
                CleanupStack::PopAndDestroy(buffer);
                break;
                }
            case EMcsOpenByUids:
                {
                SetAsyncL(aMessage);
                MPXUser::CreateBufferL(aMessage, 3, buffer);
                CleanupStack::PushL(buffer);
                RBufReadStream readStream(*buffer);
                CleanupClosePushL(readStream);
                TInt mode = readStream.ReadInt32L();
                RArray<TUid> uids;
                CleanupClosePushL(uids);
                ::InternalizeL(uids, readStream);
                iContext->OpenL(uids.Array(), mode, this);
                CleanupStack::PopAndDestroy(&uids);
                CleanupStack::PopAndDestroy(&readStream);
                CleanupStack::PopAndDestroy(buffer);
                break;
                }
            case EMcsOpen:
                {
                SetAsyncL(aMessage);
                iContext->OpenL(aMessage.Int3(), this);
                break;
                }
            case EMcsMediaByPath:
                {
                SetAsyncL(aMessage);
                // Reset previous media command and result
                delete iRootMediaArray;
                iRootMediaArray = NULL;
                delete iMediaCommand;
                iMediaCommand = NULL;
                iMediaCommand = CMPXCommand::NewL(aMessage.Int1());
                // Extract client's capabilites from the RMessage
                TCapabilitySet theCaps =  TSecurityInfo(aMessage).iCaps ;
                iMediaCommand->SetTObjectValueL<TCapabilitySet>(
                                        KMPXCommandMediaCapbilitySet, theCaps);
                MPX_ASSERT(iMediaCommand->IsSupported(KMPXCommandGeneralTargetIds));
                delete iMediaPath;
                iMediaPath = NULL;
                iMediaPath = iMediaCommand->ValueCObjectL<CMPXCollectionPath>(
                                                   KMPXCommandGeneralTargetIds);
                MPX_ASSERT(iMediaPath->Levels()>0);
                if (iMediaPath->Selection().Count()>0 && iMediaPath->Levels() == 1)
                    { // multiple selection at root level
                    iRootMediaArray=CMPXMediaArray::NewL(); // array of media result
                    iMediaCommand->SetTObjectValueL<TInt>(KMPXCommandMediaIndex, 0);
                    iMediaPath->Set(iMediaPath->Selection()[0]);
                    // update the collection path in the command
                    iMediaCommand->SetCObjectValueL<CMPXCollectionPath>(KMPXCommandGeneralTargetIds,
                                                                        iMediaPath);
                    }
                iContext->MediaL(*iMediaCommand, this);
                break;
                }
            case EMcsBack:
                {
                SetAsyncL(aMessage);
                iContext->BackL(this);
                break;
                }
            case EMcsRemovePath:
                {
                SetAsyncL(aMessage);
                CMPXCollectionPath* path( NULL );
                ::NewFromMessageL(aMessage, 0, path);
                CleanupStack::PushL( path );
                MPX_ASSERT(path);
                MPX_ASSERT(path->Levels()>0);
                iContext->RemoveL(path, this);
                CleanupStack::Pop( path ); // ownership transferred
                break;
                }
            case EMcsGetUid:
                {
                MPX_DEBUG1("CMPXCollectionSession::DispatchMessageL EMcsGetUid");
                TPckgC<TInt> uidPkg(iContext->PluginId().iUid);
                aMessage.Write(0,uidPkg);
                break;
                }
            case EMcsGetPath:
                {
                MPX_DEBUG1("CMPXCollectionSession::DispatchMessageL EMcsGetPath");
                ::CreateBufferL<CMPXCollectionPath>(iContext->Path(), iSyncBuffer);
                r = iSyncBuffer->Size();
                break;
                }
            case EMcsGetSyncBuffer:
                {
                MPX_DEBUG1("CMPXCollectionSession::DispatchMessageL EMcsGetSyncBuffer");
                aMessage.WriteL(0,iSyncBuffer->Ptr(0));
                delete iSyncBuffer;
                iSyncBuffer = NULL;
                break;
                }
            case EMcsGetAsyncBuffer:
                {
                MPX_DEBUG1("CMPXCollectionSession::DispatchMessageL EMcsGetAsyncBuffer");
                aMessage.WriteL(0,iAsyncBuffer->Ptr(0));
                delete iAsyncBuffer;
                iAsyncBuffer = NULL;
                break;
                }
            case EMcsCommandExt:
                {
                if (aMessage.Int0())
                    { // async command
                    SetAsyncL(aMessage);
                    }
                CMPXCommand* cmd = CMPXCommand::NewL(aMessage.Int1());
                CleanupStack::PushL(cmd);
                iContext->CommandL(*cmd, this, *iMessageQueue);
                CleanupStack::PopAndDestroy(cmd);
                break;
                }
            case EMcsAddItem:
                {
                ::NewFromMessageL<CMPXMedia>(aMessage,0,media);
                CleanupStack::PushL(media);
                iContext->AddL(*media);
                CleanupStack::PopAndDestroy(media);
                break;
                }
            case EMcsRemoveItem:
                {
                ::NewFromMessageL<CMPXMedia>(aMessage,0,media);
                CleanupStack::PushL(media);
                iContext->RemoveL(*media);
                CleanupStack::PopAndDestroy(media);
                break;
                }
            case EMcsSetMedia:
                {
                ::NewFromMessageL<CMPXMedia>(aMessage,0,media);
                CleanupStack::PushL(media);
                iContext->SetL(*media);
                CleanupStack::PopAndDestroy(media);
                break;
                }
            case EMcsSetFilter:
                {
                CMPXFilter* filter=NULL;
                if (aMessage.GetDesLengthL(0))
                    {
                    ::NewFromMessageL<CMPXFilter>(aMessage,0,filter);
                    }
                if (filter)
                    {
                    CleanupStack::PushL(filter);
                    iContext->SetFilterL(filter);
                    CleanupStack::PopAndDestroy(filter);
                    }
                else
                    {
                    iContext->SetFilterL(NULL);
                    }
                break;
                }
            case EMcsCommand:
                {
                iContext->CommandL((TMPXCollectionCommand) aMessage.Int0(),
                                     aMessage.Int1());
                break;
                }
            case EMcsFindAll:
                {
                CMPXMedia* media = CMPXMedia::NewL(aMessage.Int2());
                CleanupStack::PushL(media);
                MPXUser::CreateBufferL( aMessage, 0, buffer );
                CleanupStack::PushL(buffer);
                if (aMessage.Int1())
                    { //Sync find
                    delete iSyncMedia;
                    iSyncMedia = NULL;
                    iSyncMedia = iContext->FindAllSyncL(*media, *buffer);
                    if (iSyncMedia)
                        {
                        ::CreateBufferL<CMPXMedia>(*iSyncMedia,iSyncBuffer);
                        r = iSyncBuffer->Size();
                        }
                    else
                        {
                        r = KErrNotFound;
                        }
                    CleanupStack::PopAndDestroy(buffer);
                    }
                else
                    { // Async
                    SetAsyncL(aMessage);
                    iContext->FindAllL(*media, buffer, this);
                    CleanupStack::Pop(buffer); // ownership transferred
                    }
                CleanupStack::PopAndDestroy(media);
                break;
                }
            case EMcsNotifyEvent:
                {
                iEngine.NotifyL( (TMPXCollectionBroadCastMsg) aMessage.Int0(), aMessage.Int1() );
                break;
                }
            case EMcsGetSupportedTypes:
                {
                RPointerArray<CMPXCollectionType> array;
                iEngine.GetSupportedTypesL( array );
                ::CreateBufferL(array.Array() , iSyncBuffer);
                r = iSyncBuffer->Size();
                array.ResetAndDestroy();
                array.Close();
                break;
                }
            case EMcsGetCapabilities:
                {
                // Get the capabilities from the client context
                TCollectionCapability cap = iContext->GetCapabilities();
                TPckgC<TCollectionCapability> p(cap);
                aMessage.Write(0,p);
                break;
                }
            case EMcsCancelRequest:
                {
                CancelRequests();
                break;
                }
            case EMcsCollectionIdLookup:
                {
                // Recreate buffer
                CBufBase* buffer(NULL);
                MPXUser::CreateBufferL( aMessage, 0, buffer );
                CleanupStack::PushL( buffer );

                // Array from buffer
                RArray<TUid> uids;
                CleanupClosePushL( uids );

                RBufReadStream rs( *buffer );
                CleanupClosePushL( rs );
                ::InternalizeL<TUid>( uids, rs );
                CleanupStack::PopAndDestroy( &rs );

                // Resolve and return
                TUid pluginId = iEngine.ResolvePluginUid( uids.Array() );
                TPckgC<TUid> p( pluginId );
                aMessage.WriteL(1,p);
                CleanupStack::PopAndDestroy( 2, buffer ); // buffer, uids
                break;
                }
            case EMcsCollectionID:
                {
                // This is a placeholder function,
                //
                TPckgBuf<TUid> id;
                aMessage.ReadL(0,id);

                // Lookup id and write back to client
                TUid realId = iEngine.LookupCollectionPluginID( id() );
                TPckgC<TUid> p(realId);
                aMessage.WriteL(0,p);
                break;
                }
            case EMcsFilter:
                {
                const CMPXFilter* filter=iContext->Filter();
                if (filter)
                    {
                    ::CreateBufferL<CMPXFilter>(*filter,iSyncBuffer);
                    r=iSyncBuffer->Size();
                    }
                break;
                }
            case EMcsGetNextMessage:
                {
                MPX_ASSERT(iMessageQueue);
                iMessageQueue->SendNext(aMessage);
                iCompleteRequest=EFalse;
                break;
                }
            case EMcsCancelGetMessage:
                {
                MPX_ASSERT(iMessageQueue);
                iMessageQueue->Reset();
                break;
                }
            default:
                {
                PanicClient(aMessage, KErrNotSupported);
                break;
                }
            }
        }
    MPX_DEBUG3("<--CMPXCollectionSession::DispatchMessageL %d, this 0x%08x",
               op, this);
    return r;
    }

// ----------------------------------------------------------------------------
// Get current selection
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::GetSelectionL(const RMessage2& aMessage)
    {
    (void)aMessage; 
    }

// ----------------------------------------------------------------------------
// Queue the message and complete async
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::SetAsyncL(const RMessage2& aMessage)
    {
    MPX_ASSERT(iMessage.IsNull());
    iMessage = aMessage;
    iCompleteRequest=EFalse;
    }

// ----------------------------------------------------------------------------
// Complete queued message
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::CompleteAsync(
    TInt aErr,
    TInt aSlot1, const TDesC8* aVal1,
    TInt aSlot2, const TDesC8* aVal2,
    TInt aSlot3, const TDesC8* aVal3)
    {
    MPX_ASSERT(!iMessage.IsNull());
    TInt err(KErrNone);
    if (aErr>=0)
        {
        err = DoWriteData(aSlot1, aVal1, aSlot2, aVal2, aSlot3, aVal3);
        if (err)
            {// Set to new error
            aErr=err;
            }
        }
    MPX_DEBUG4("CMPXCollectionSession::CompleteAsync 0x%08x task %d err %d",
               this, iMessage.Function(), aErr);
    iMessage.Complete(aErr);
    }

// ----------------------------------------------------------------------------
// Write data back to client
// ----------------------------------------------------------------------------
//
TInt CMPXCollectionSession::DoWriteData(
    TInt aSlot1, const TDesC8* aVal1,
    TInt aSlot2, const TDesC8* aVal2,
    TInt aSlot3, const TDesC8* aVal3)
    {
    TInt ret(KErrNone);
    if (aVal1)
        {
        ret=iMessage.Write(aSlot1,*aVal1);
        }

    if (aVal2 && KErrNone==ret)
        {
        ret=iMessage.Write(aSlot2,*aVal2);
        }

    if (aVal3 && KErrNone==ret)
        {
        ret=iMessage.Write(aSlot3,*aVal3);
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Cancel all outstanding requests on this session
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::CancelRequests()
    {
    if (iContext)
        {
        iContext->CancelRequest(this);
        }
    if (!iMessage.IsNull())
        {
        iMessage.Complete(KErrCancel);
        }
    }

// ----------------------------------------------------------------------------
// The thread ID of the client thread
// ----------------------------------------------------------------------------
//
TThreadId CMPXCollectionSession::ClientIdL(const RMessage2& aMessage)
    {
    RThread t;
    aMessage.ClientL(t);
    TThreadId tid=t.Id();
    t.Close();
    return tid;
    }

// ----------------------------------------------------------------------------
// Set collection mode
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::SetModeL(const RMessage2& aMessage)
    {
    if (!iMessageQueue)
        {
        iMessageQueue = CMPXMessageQueue::NewL();
        }
    iContext = &iEngine.SessionInitL(TUid::Uid(aMessage.Int0()),
                                     ClientIdL(aMessage),
                                     iMessageQueue);
    }

// ----------------------------------------------------------------------------
// Handles the collection entries being opened.
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::HandleOpen(
    CMPXMedia* aMedia,
    TInt aIndex,
    TBool aComplete,
    TInt aError)
    {
    MPX_DEBUG2("CMPXCollectionSession::HandleOpen with media err %d", aError);
    TInt op(iMessage.Function());
    MPX_ASSERT((!iMessage.IsNull() &&
        (EMcsOpen == op || EMcsOpenIndex == op ||
         EMcsOpenPath == op || EMcsBack == op || EMcsOpenByUids == op)));
    TInt size(0);
    if (aError > 0 && aError != KMPXPathUpdated &&
        aError != KMPXCollectionPath && aMedia)
        {
        MPX_DEBUG2("CMPXCollectionSession::HandleOpen media count %d",
                   aMedia->Count());
        if (aMedia)
            {
            delete iAsyncMedia;
            iAsyncMedia=NULL;
            TRAPD(err,iAsyncMedia=CMPXMedia::NewL(*aMedia));
            if(err == KErrNone)
                {
                TRAP(err, ::CreateBufferL(*iAsyncMedia,iAsyncBuffer));
                }
            if (err)
                {
                aError = err;
                }
            else
                {
                size = iAsyncBuffer->Size();
                }
            MPX_DEBUG2("CMPXCollectionSession::HandleOpen buffer size %d", size);
            }
        }
    TPckgC<TInt> index(aIndex);
    TPckgC<TInt> complete(static_cast<TInt>(aComplete));
    TPckgC<TInt> sizePkg(size);
    MPX_DEBUG3("CMPXCollectionSession::HandleOpen completed with media: err %d, size %d",
               aError, sizePkg());
    CompleteAsync(aError,
                   0, &sizePkg,
                   1, &index,
                   2, &complete);
    }

// ----------------------------------------------------------------------------
// Handle completion of an async op
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::HandleOpComplete( TInt aError )
    {
    CompleteAsync(aError);
    }

// ----------------------------------------------------------------------------
// Handle media request callback
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::HandleMedia(
    CMPXMedia* aMedia,
    TInt aError)
    {
    TInt op(iMessage.Function());
    TInt size(0);
    TInt err(KErrNone);
    TBool done(EFalse);
    MPX_ASSERT(!iMessage.IsNull() &&
        (EMcsMediaByPath == op || EMcsMedia == op));
    if (iRootMediaArray)
        { // multiple selection at root level
        // Advance to next item
        TInt* pIndex = iMediaCommand->Value<TInt>(KMPXCommandMediaIndex);
        if (!pIndex)
            {
            aError = KErrNoMemory;
            }
        else
            {
            *pIndex = *pIndex + 1;
            }
        if (KErrNone == aError && aMedia)
            {
            MPX_TRAP(err, iRootMediaArray->AppendL(*aMedia));
            }
        else
            {// append dummy media for this item
            TRAP(err, iRootMediaArray->AppendL(CMPXMedia::NewL()));
            }

        if (err)
            {
            aError = err; // set to latest error code
            }
        if (KErrNone==aError)
            { // done for all root items or error happens on an item
            if (*pIndex == iMediaPath->Selection().Count())
                {
                done = ETrue;
                delete iAsyncMedia;
                iAsyncMedia=NULL;
                TRAP(err, iAsyncMedia=CMPXMedia::NewL());
                if(KErrNone==err)
                    {
                    TRAP(err,
                        iAsyncMedia->SetCObjectValueL<CMPXMediaArray>(
                            KMPXMediaArrayContents, iRootMediaArray);
                        iAsyncMedia->SetTObjectValueL<TInt>(KMPXMediaArrayCount, *pIndex));
                    }

                if (KErrNone==err)
                    {
                    aError = KMPXCollectionMedia;
                    }
                }
            else
                { // Send next request

                TRAP(err,
                    iMediaCommand->SetTObjectValueL<TInt>(KMPXCommandMediaIndex, *pIndex);
                    iMediaPath->Set(iMediaPath->Selection()[*pIndex]);
                    // update path in command
                    iMediaCommand->SetCObjectValueL<CMPXCollectionPath>(KMPXCommandGeneralTargetIds,
                                                              iMediaPath);
                    iContext->MediaL(*iMediaCommand, this));
                }
            if (err)
                {
                aError = err;
                }
            }

        if (aError<0)
            { // done with error
            done = ETrue;
            delete iAsyncMedia;
            iAsyncMedia = NULL;
            }
        }
    else
        {
        done = ETrue;
        if (KErrNone == aError && aMedia)
            {
            aError = KMPXCollectionMedia;
            delete iAsyncMedia;
            iAsyncMedia=NULL;
            TRAP_IGNORE(iAsyncMedia=CMPXMedia::NewL(*aMedia));
            }
        }

    if (done)
        {
        // Free resource
        delete iMediaPath;
        iMediaPath = NULL;
        if (iAsyncMedia && aError>=0)
            {
            TRAP(err, ::CreateBufferL<CMPXMedia>(*iAsyncMedia,iAsyncBuffer));
            if (iAsyncBuffer)
                {
                size = iAsyncBuffer->Size();
                }
            }
        if (err)
            {
            aError = err;
            }
        TPckgC<TInt> sizePkg(size);
        CompleteAsync(aError,0, &sizePkg);
        }
    }

// ----------------------------------------------------------------------------
// Callback of async CommandL
// ----------------------------------------------------------------------------
//
 void CMPXCollectionSession::HandleCommandComplete(
    CMPXCommand* aCommandResult,
    TInt aError)
     {
     MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EMcsCommandExt);
     TInt size(0);
     if( aError == KErrNone && aCommandResult)
         {
         delete iAsyncMedia;
         iAsyncMedia=NULL;
         TRAP(aError, iAsyncMedia=CMPXMedia::NewL(*aCommandResult));
         if (KErrNone==aError)
            {
            TRAP(aError, ::CreateBufferL<CMPXMedia>(*iAsyncMedia,iAsyncBuffer));
            }

         if (KErrNone==aError)
             {
             size = iAsyncBuffer->Size();
             }
         }
     TPckgC<TInt> sizePkg(size);
     CompleteAsync(aError, 2, &sizePkg);
     }

// ----------------------------------------------------------------------------
// Handle removing media by collection path
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::HandleRemove(
    const CDesCArray& aUriArray,
    TInt aError)
    {
    MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EMcsRemovePath);
    if( aError == KErrNone )
        {
        TRAPD(err, MPXUser::CreateBufferL(&aUriArray, iAsyncBuffer));
        aError = err == KErrNone ? iAsyncBuffer->Size() : err;
        }
    CompleteAsync(aError);
    }

// ----------------------------------------------------------------------------
// Handles find results
// ----------------------------------------------------------------------------
//
void CMPXCollectionSession::HandleFindAll(CMPXMedia* aMedia, TInt aError)
    {
    MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EMcsFindAll);
    if( aError == KErrNone && aMedia)
        {
        delete iAsyncMedia;
        iAsyncMedia=NULL;
        TRAPD(err, iAsyncMedia=CMPXMedia::NewL(*aMedia));
        if(KErrNone == err)
            {
            TRAP(err, ::CreateBufferL<CMPXMedia>(*iAsyncMedia,iAsyncBuffer));
            }

        aError = err == KErrNone ? iAsyncBuffer->Size() : err;
        }
    CompleteAsync(aError);
    }
// End of file
