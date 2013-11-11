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
* Description:  Playback server session
*
*/

#include <e32std.h>
#include <e32svr.h>
#include <CustomCommandUtility.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxmedia.h>
#include <mpxclientlist.h>
#include <mpxcmn.h>
#include <mpxcollectionplaylist.h>
#include <mpxlog.h>
#include <mpxmessagequeue.h>
#include "mpxplaybackserver.h"
#include "mpxplaybackengine.h"
#include "mpxplaybackserversession.h"

// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Panic client
// ----------------------------------------------------------------------------
//
LOCAL_C void PanicClient(const RMessage2 &aMessage,TInt aPanic)
    {
    _LIT(KTxtServer,"Playback server Session");
    aMessage.Panic(KTxtServer,aPanic);
    }


// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackSession* CMPXPlaybackSession::NewL()
    {
    CMPXPlaybackSession* s = new(ELeave)CMPXPlaybackSession();
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXPlaybackSession::CMPXPlaybackSession()
    {
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::ConstructL()
    {
    MPX_DEBUG2("CMPXPlaybackSession::ConstructL(), this 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackSession::~CMPXPlaybackSession()
    {
    CancelRequests();
    CMPXPlaybackServer* server = static_cast<CMPXPlaybackServer*>(
            const_cast<CServer2*>(Server()));
    if ( iMessageQueue )
        {
        server->RemoveClient( *iMessageQueue );
        }
    delete iSyncBuffer;
    delete iAsyncBuffer;
    delete iMedia;
    delete iMessageQueue;
    }

// ----------------------------------------------------------------------------
// Service request
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::ServiceL(const RMessage2& aMessage)
    {
    // by default - change for async in helper methods if required
    iCompleteRequest=ETrue;
    TInt r=KErrNone;
    TRAPD( err, DispatchMessageL( aMessage, r ) );
    TBool isErr=(err!=KErrNone);
    // If it's not async, complete now (or an async helper method leaves)
    if(iCompleteRequest)
        {
        if (!aMessage.IsNull())
            {
            aMessage.Complete(isErr ? err : r);
            } // otherwise message already completed DispatchMessageL
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
void CMPXPlaybackSession::DispatchMessageL( const RMessage2& aMessage, TInt& aMsgHandleResult )
    {
    aMsgHandleResult = KErrNone;
    //
    // All methods apart from the player creation methods, require a player
    //
    TInt op=aMessage.Function();
    if (op != EPbsSetMode)
        {
        CheckPlayerL();
        }
    MPX_DEBUG3("-->CMPXPlaybackSession::DispatchMessageL %d, this 0x%08x",
            op, this);
    switch(op)
        {
        case EPbsSetMode:
            {
            SetModeL(aMessage);
            break;
            }
        case EPbsGetClients:
            {
            RArray<TProcessId> procArray;
            ::CopyArrayL<TProcessId>(
                iPlayer->ClientList()->ClientProcessList(),
                procArray );
            TProcessId lastPid = static_cast<CMPXPlaybackServer*>(
                         const_cast<CServer2*>(Server()))->LastActiveProcessId();

            TInt index( procArray.Find( lastPid ));
            if ( KErrNotFound != index && index )
                {
                procArray.Remove( index );
                procArray.Insert( lastPid, 0 );
                }
            ::CreateBufferL<TProcessId>(
                            procArray.Array(),
                            iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            procArray.Close();
            break;
            }
        case EPbsGetSyncBuffer:
            {
            aMessage.WriteL(0,iSyncBuffer->Ptr(0));
            delete iSyncBuffer;
            iSyncBuffer = NULL;
            break;
            }
        case EPbsGetAsyncBuffer:
            {
            //In case of other application leaving, causing us to leave
            //we could have a task that does not get removed from the top of the queue
            //but the data for that task has been deleted.
            //When the task runs again, there will be no data causing a panic - leave if this occurs
            User::LeaveIfNull(iAsyncBuffer);                         
            aMessage.WriteL(0,iAsyncBuffer->Ptr(0));
            delete iAsyncBuffer;
            iAsyncBuffer = NULL;       
            break;                            
            }
        case EPbsInitFromCollection:
            {
            MPXUser::CreateBufferL(aMessage, 0, iSyncBuffer);
            CMPXCollectionPlaylist* p = NULL;
            ::NewFromBufferL(*iSyncBuffer, p);
            delete iSyncBuffer;
            iSyncBuffer = NULL;
            CleanupStack::PushL(p);
            iPlayer->InitL(*p,aMessage.Int1());
            CleanupStack::PopAndDestroy(p);
            }
            break;
        case EPbsInitFromUri:
            {
            CBufBase* buf0(NULL);
            MPXUser::CreateBufferL(aMessage,0,buf0);
            CleanupStack::PushL(buf0);

            // Type parameter is optional
            if ( aMessage.GetDesLength( 1 ) > 0 )
                {
                CBufBase* buf1(NULL);
                MPXUser::CreateBufferL(aMessage,1,buf1);
                CleanupStack::PushL(buf1);
                iPlayer->InitL(MPXUser::Ptr(buf0->Ptr(0)), buf1->Ptr(0));
                CleanupStack::PopAndDestroy(buf1);
                }
            else
                {
                MPX_DEBUG2("CMPXPlaybackSession::DispatchMessageL %d: Type is empty", op);
                iPlayer->InitL( MPXUser::Ptr( buf0->Ptr(0) ), KNullDesC8 );
                }
            CleanupStack::PopAndDestroy(buf0);
            }
            break;
        case EPbsInitFromFile:
            {
            InitFromFileL(aMessage);
            break;
            }
        case EPbsInitStreamingFromUri:
            {            
            CBufBase* buf0(NULL);
            MPXUser::CreateBufferL(aMessage,0,buf0);
            CleanupStack::PushL(buf0);

            // Type parameter is optional
            if ( aMessage.GetDesLength( 1 ) > 0 )
                {
                CBufBase* buf1(NULL);
                MPXUser::CreateBufferL(aMessage,1,buf1);
                CleanupStack::PushL(buf1);
                iPlayer->InitStreamingL(MPXUser::Ptr(buf0->Ptr(0)), buf1->Ptr(0), aMessage.Int2());
                CleanupStack::PopAndDestroy(buf1);
                }
            else
                {
                MPX_DEBUG2("CMPXPlaybackSession::DispatchMessageL %d: Type is empty", op);
                iPlayer->InitStreamingL( MPXUser::Ptr( buf0->Ptr(0) ), KNullDesC8, aMessage.Int2() );
                }
            CleanupStack::PopAndDestroy(buf0);
            break;                          
            }
         case EPbsInitStreamingFromFile:
            {
            RFile file;
            User::LeaveIfError(file.AdoptFromClient(aMessage,0,1));
            iPlayer->InitStreamingL(file, aMessage.Int2());
            file.Close();
            break;
            }            
        case EPbsCancelRequest:
            {
            CancelRequests();
            break;
            }
        case EPbsGetState:
            {
            aMsgHandleResult = iPlayer->State();
            break;
            }
        case EPbsSetProperty:
            {
            iPlayer->SetL(static_cast<TMPXPlaybackProperty>(aMessage.Int0()),
                          aMessage.Int1());
            break;
            }
        case EPbsGetProperty:
            {
            SetAsync(aMessage);
            iPlayer->PropertyL(
                    static_cast<TMPXPlaybackProperty>(aMessage.Int0()),*this);
            break;
            }
        case EPbsGetPlayerTypes:
            {
            RArray<TMPXPlaybackPlayerType> pluginTypes;
            CleanupClosePushL(pluginTypes);
            RArray<TInt> types;
            CleanupClosePushL(types);
            iPlayer->PluginHandler()->GetPluginTypes(types);
            for (TInt i=0; i< types.Count(); ++i)
                {
                pluginTypes.AppendL(
                        static_cast<TMPXPlaybackPlayerType>(types[i]));
                }
            CleanupStack::PopAndDestroy(&types);
            ::CreateBufferL<TMPXPlaybackPlayerType>(
                                    pluginTypes.Array(),
                                    iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            CleanupStack::PopAndDestroy(&pluginTypes);
            break;
            }
        case EPbsGetPlayerTypeDisplayName:
            {
            const TDesC& playerName = iPlayer->PluginHandler()->PlayerName(
                        static_cast<TMPXPlaybackPlayerType>(aMessage.Int0()));
            aMsgHandleResult = 0;
            if (playerName.Length()>0)
                {
                MPXUser::CreateBufferL(playerName, iSyncBuffer);
                aMsgHandleResult = iSyncBuffer->Size();
                }
            break;
            }
        case EPbsGetAllPlayersUids:
            {
            RArray<TUid> uids;
            CleanupClosePushL(uids);
            iPlayer->PluginHandler()->GetPluginUids(uids);

            ::CreateBufferL<TUid>(
                    uids.Array(),
                    iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            CleanupStack::PopAndDestroy(&uids);
            break;
            }
        case EPbsGetPlayersUidsForType:
            {
            aMsgHandleResult = CreatePlayerUidsBufferL(aMessage);
            break;
            }
        case EPbsGetSubPlayerNamesByUid:
            {
            SetAsync(aMessage);
            iPlayer->SubPlayerNamesL(TUid::Uid(aMessage.Int0()),*this);
            break;
            }
        case EPbsSelectPlayerByType:
            {
            iPlayer->PluginHandler()->SelectPlayersL(
                         static_cast<TMPXPlaybackPlayerType>(aMessage.Int0()));
            break;
            }
        case EPbsSelectPlayerByUid:
            {
            iPlayer->PluginHandler()->SelectPlayerL(TUid::Uid(aMessage.Int0()));
            break;
            }
        case EPbsSelectSubPlayer:
            {
            iPlayer->PluginHandler()->SelectSubPlayerL(
                                  TUid::Uid(aMessage.Int0()),aMessage.Int1());
            break;
            }
        case EPbsClearPlayerSelection:
            {
            iPlayer->PluginHandler()->ClearSelectPlayersL();
            break;
            }
        case EPbsGetSelection:
            {
            GetSelectionL(aMessage);
            break;
            }
        case EPbsPlayerFound:
            {
            aMsgHandleResult = iPlayer->PluginHandler()->PlayerFound();
            break;
            }
        case EPbsGetPlayerType:
            {
            aMsgHandleResult = iPlayer->PluginHandler()->PluginType();
            break;
            }
        case EPbsGetTypeName:
            {
            MPXUser::CreateBufferL(iPlayer->PluginHandler()->PlayerName(),
                                   iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            break;
            }
        case EPbsGetSubPlayerIndex:
            {
            aMsgHandleResult = iPlayer->PluginHandler()->SubPlayer();
            break;
            }
        case EPbsGetPlayerUid:
            {
            TPckgC<TInt> uidPkg((iPlayer->PluginHandler()->PluginUid()).iUid);
            aMessage.Write(0,uidPkg);
            break;
            }
        case EPbsGetCollectionPlaylist:
            {
            aMsgHandleResult = 0;
            if (iPlayer->Playlist())
                {
                ::CreateBufferL<CMPXCollectionPlaylist>(*(iPlayer->Playlist()),
                                                        iSyncBuffer);
                aMsgHandleResult = iSyncBuffer->Size();
                }
            break;
            }
        case EPbsGetFile:
            {
            const RFile& file = iPlayer->File();
            if (file.SubSessionHandle())
                {
                aMsgHandleResult = file.TransferToClient(aMessage,0); //message completed
                }
            else
                {
                TPckgC<TInt> handle(KErrNotFound);
                aMessage.Write(0, handle);
                }
            break;
            }
        case EPbsGetUri:
            {
            aMsgHandleResult=0;
            if (iPlayer->Uri().Length()>0)
                {
                MPXUser::CreateBufferL(iPlayer->Uri(),iSyncBuffer);
                aMsgHandleResult = iSyncBuffer->Size();
                }
            break;
            }
        case EPbsGetMedia:
            {
            SetAsync( aMessage );
            CMPXCommand* cmd( CMPXCommand::NewL( aMessage.Int1() ) );
            CleanupStack::PushL( cmd );
            iPlayer->MediaL( *this, *cmd );
            CleanupStack::PopAndDestroy( cmd );
            break;
            }
        case EPbsGetSupportedMimeTypes:
            {
            CDesCArray* mimeTypes =
               iPlayer->PluginHandler()->SupportedMimeTypesL();
            CleanupStack::PushL(mimeTypes);
            MPXUser::CreateBufferL((const MDesCArray*)mimeTypes, iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            CleanupStack::PopAndDestroy(mimeTypes);
            }
            break;
        case EPbsGetSupportedExtensions:
            {
            CDesCArray* exts =
              iPlayer->PluginHandler()->SupportedExtensionsL();
            CleanupStack::PushL(exts);
            MPXUser::CreateBufferL((const MDesCArray*)exts, iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            CleanupStack::PopAndDestroy(exts);
            }
            break;
        case EPbsGetSupportedSchemas:
            {
            CDesCArray* schemas =
                 iPlayer->PluginHandler()->SupportedSchemasL();
            CleanupStack::PushL(schemas);
            MPXUser::CreateBufferL((const MDesCArray*)schemas, iSyncBuffer);
            aMsgHandleResult = iSyncBuffer->Size();
            CleanupStack::PopAndDestroy(schemas);
            }
            break;
        case EPbsGetNextMessage:
            {
            ASSERT(iMessageQueue);
            iMessageQueue->SendNext(aMessage);
            iCompleteRequest=EFalse;
            break;
            }
        case EPbsCancelGetMessage:
            {
            ASSERT(iMessageQueue);
            iMessageQueue->Reset();
            break;
            }
        case EPbsCommand:
            {
            CMPXCommand* cmd( NULL );
            ::NewFromMessageL<CMPXMedia>(aMessage, 1, cmd);
            CleanupStack::PushL(cmd);
            iPlayer->CommandL(*cmd, *iMessageQueue);
            CleanupStack::PopAndDestroy(cmd);
            break;
            }
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        case EPbsInitFromFile64:
            {
            RFile64 file;
            User::LeaveIfError(file.AdoptFromClient(aMessage,0,1));
            iPlayer->Init64L(file);
            file.Close();
            break;
            }
        case EPbsInitStreamingFromFile64:
            {
            RFile64 file;
            User::LeaveIfError(file.AdoptFromClient(aMessage,0,1));
            iPlayer->InitStreaming64L(file, aMessage.Int2());           
            file.Close();
            break;
            }
        case EPbsGetFile64:
            {
            const RFile64& file = iPlayer->File64();
            if (file.SubSessionHandle())
                {
                aMsgHandleResult = file.TransferToClient(aMessage,0); //message completed
                }
            else
                {
                TPckgC<TInt> handle(KErrNotFound);
                aMessage.Write(0, handle);
                }
            break;
            }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        case EPbsSetPrimaryClient:
            {
            aMsgHandleResult = iPlayer->ClientList()->SetPrimaryClient(*iMessageQueue);            
            break;
            }
        default:
            {
            PanicClient(aMessage,KErrNotSupported);
            break;
            }
        }
    MPX_DEBUG1("<---CMPXPlaybackSession::DispatchMessageL");
    }

// ----------------------------------------------------------------------------
// Get current selection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::GetSelectionL(const RMessage2& aMessage)
    {
    TMPXPlaybackPlayerType type;
    TUid uid;
    TInt index;
    const TInt KMsgSlotZero = 0;
    const TInt KMsgSlotOne = 1;
    const TInt KMsgSlotTwo = 2;
    const TInt KMsgSlotThree = 3;
    TPtrC subPlayerName(KNullDesC);
    iPlayer->PluginHandler()->GetSelection(type,uid,index, subPlayerName);
    TInt size(0);
    if (subPlayerName != KNullDesC)
        {
        MPXUser::CreateBufferL(subPlayerName, iSyncBuffer);
        size = iSyncBuffer->Size();
        }
    ::WriteL<TMPXPlaybackPlayerType>( aMessage, KMsgSlotZero, type );
    ::WriteL<TUid>( aMessage, KMsgSlotOne, uid );
    ::WriteL<TInt>( aMessage, KMsgSlotTwo, index );
    ::WriteL<TInt>( aMessage, KMsgSlotThree, size );
    }

// ----------------------------------------------------------------------------
// Queue the message and complete async
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::SetAsync(const RMessage2& aMessage)
    {
    MPX_FUNC_EX("CMPXPlaybackSession::SetAsync");
    MPX_ASSERT(iMessage.IsNull() && !aMessage.IsNull());
    iMessage = aMessage;
    iCompleteRequest=EFalse;
    }

// ----------------------------------------------------------------------------
// Complete queued message
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::CompleteAsync(
    TInt aErr,
    TInt aSlot1, const TDesC8* aVal1,
    TInt aSlot2, const TDesC8* aVal2)
    {
    MPX_ASSERT(!iMessage.IsNull());
    TInt err(KErrNone);
    if (KErrNone==aErr)
        {
        err = DoWriteData(aSlot1, aVal1, aSlot2, aVal2);
        if (err)
            { // Set to new error
            aErr = err;
            }
        }
    MPX_DEBUG4("CMPXPlaybackSession::CompleteAsync 0x%08x task %d err %d",
            this, iMessage.Function(), aErr);
    iMessage.Complete(aErr);
    }

// ----------------------------------------------------------------------------
// Write data back to client
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackSession::DoWriteData(
    TInt aSlot1, const TDesC8* aVal1,
    TInt aSlot2, const TDesC8* aVal2)
    {
    TInt ret(KErrNone);
    if (aVal1)
        {
        ret = iMessage.Write(aSlot1,*aVal1);
        }

    if (aVal2 && KErrNone==ret)
        {
        ret = iMessage.Write(aSlot2,*aVal2);
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Init player from file
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::InitFromFileL(const RMessage2& aMessage)
    {
    RFile file;
    User::LeaveIfError(file.AdoptFromClient(aMessage,0,1));
    iPlayer->InitL(file);
    file.Close();
    }

// ----------------------------------------------------------------------------
// Cancel all outstanding requests on this session
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::CancelRequests()
    {
    MPX_FUNC_EX("CMPXPlaybackSession::CancelRequests");
    if (!iMessage.IsNull())
        {
        iMessage.Complete(KErrCancel);
        iPlayer->CancelRequests();
        }
    }

// ----------------------------------------------------------------------------
// The thread ID of the client thread
// ----------------------------------------------------------------------------
//
TThreadId CMPXPlaybackSession::ClientIdL(const RMessage2& aMessage)
    {
    RThread t;
    aMessage.ClientL(t);
    TThreadId tid=t.Id();
    t.Close();
    return tid;
    }

// ----------------------------------------------------------------------------
// Set playback mode
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::SetModeL(const RMessage2& aMessage)
    {
    MPX_DEBUG3("-->CMPXPlaybackSession::SetModeL 0x%08x, mode 0x%08x", this, aMessage.Int0());
    if (iPlayer)
        {
        MPX_DEBUG1("CMPXPlaybackSession::SetModeL detach from old player");
        // swap engine
        static_cast<CMPXPlaybackServer*>(
                const_cast<CServer2*>(Server()))->AddClient();
        static_cast<CMPXPlaybackServer*>(
                const_cast<CServer2*>(Server()))->RemoveClient(*iMessageQueue);
        iPlayer = NULL;
        ASSERT(iMessageQueue);
        iMessageQueue->Reset();
        }
    if (!iMessageQueue)
        {
        iMessageQueue = CMPXMessageQueue::NewL();
        }
    iPlayer=static_cast<CMPXPlaybackServer*>(
            const_cast<CServer2*>(Server()))->CreatePlayerL(
                                                TUid::Uid(aMessage.Int0()),
                                                aMessage.Int1(), 
                                                ClientIdL(aMessage),
                                                iMessageQueue);
    MPX_DEBUG3("<--CMPXPlaybackSession::SetModeL 0x%08x, engine 0x%08x", this, iPlayer);
    }

// ----------------------------------------------------------------------------
// An array must be supplied to receive UIDs, so a temporary one is created
// here before being copied into the buffer
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackSession::CreatePlayerUidsBufferL(const RMessage2& aMessage)
    {
    RArray<TUid> players;
    CleanupClosePushL(players);
    iPlayer->PluginHandler()->GetPlayerListL(players,
                        static_cast<TMPXPlaybackPlayerType>(aMessage.Int0()));
    ::CreateBufferL<TUid>(players.Array(), iSyncBuffer);
    TInt r = iSyncBuffer->Size();
    CleanupStack::PopAndDestroy(&players);
    return r;
    }

// ----------------------------------------------------------------------------
// Handle playback property
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::HandleProperty(
    TMPXPlaybackProperty /*aProperty*/,
    TInt aValue,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackSession::HandleProperty");
    MPX_DEBUG3("CMPXPlaybackSession::HandleProperty(): aValue = %d, aError = %d", aValue, aError);
    MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EPbsGetProperty);
    TPckgC<TInt> val(aValue);
    CompleteAsync(aError,1,&val);
    }

// ----------------------------------------------------------------------------
// Method is called continously until aComplete=ETrue, signifying that it is
// done and there will be no more callbacks.Only new items are passed each time
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::HandleSubPlayerNames(
    TUid /*aPlayer*/,
    const MDesCArray* aSubPlayers,
    TBool aComplete,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackSession::HandleSubPlayerNames");
    MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EPbsGetSubPlayerNamesByUid);
    TRAPD(err,MPXUser::CreateBufferL(aSubPlayers, iAsyncBuffer));
    TInt size(0);
    if(aSubPlayers && !err)
        {
        size = iAsyncBuffer->Size();
        }
    else if (err)
        { // err to create buffer
        aError = err;
        }
    else
        { // aSubPlayers is NULL
        aError = KErrNotFound;
        }
    TPckgC<TBool> complete(aComplete);
    TPckgC<TInt> sizePkg(size);
    CompleteAsync(aError,1,&sizePkg,2,&complete);
    }

// ----------------------------------------------------------------------------
// Handle media properties
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::HandleMedia(CMPXMedia* aMedia, TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackSession::HandleMedia");
    MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EPbsGetMedia);
    TInt size(0);
    if (KErrNone == aError && aMedia)
        {
        delete iMedia;
        iMedia=NULL;
        TInt err(KErrNone);
        TRAP(err, iMedia=CMPXMedia::NewL(*aMedia));
        if (!err)
            {
            TRAP(err, ::CreateBufferL<CMPXMedia>(*iMedia,iAsyncBuffer));
            }
        if (!err)
            {
            size = iAsyncBuffer->Size();
            }
        } // else complete with error
    TPckgC<TInt> sizePkg(size);
    CompleteAsync(aError,0,&sizePkg);
    }

// ----------------------------------------------------------------------------
// Callback of async CommandL
// ----------------------------------------------------------------------------
//
 void CMPXPlaybackSession::HandlePlaybackCommandComplete(
    CMPXCommand* aCommandResult,
    TInt aError)
     {
     MPX_ASSERT(!iMessage.IsNull() && iMessage.Function() == EPbsCommandExt);
     TInt size(0);
     if( aError == KErrNone && aCommandResult)
         {
         delete iMedia;
         iMedia=NULL;
         TRAP( aError, iMedia = CMPXMedia::NewL( *aCommandResult ) );
         if ( KErrNone == aError )
             TRAP( aError, ::CreateBufferL<CMPXMedia>( *iMedia, iAsyncBuffer ) );
         if ( KErrNone == aError )
             {
             size = iAsyncBuffer->Size();
             }
         }
     TPckgC<TInt> sizePkg(size);
     CompleteAsync(aError, 2, &sizePkg);
     }

// ----------------------------------------------------------------------------
// Check if player is valid
// ----------------------------------------------------------------------------
//
void CMPXPlaybackSession::CheckPlayerL()
    {
    if (!iPlayer)
        {
        User::Leave(KErrNotReady);
        }
    }

// End of file
