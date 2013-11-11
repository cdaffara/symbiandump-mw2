/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Playback Utility implementation
*
*/


#include <bamdesca.h>
#include <s32mem.h>
#include <e32math.h>
#include <mpxcmn.h>
#include <mpxplaybackobserver.h>
#include <mpxcollectionplaylist.h>
#include <mpxmedia.h>
#include <mpxmessagemonitor.h>
#include <mpxtaskqueue.h>
#include <mpxuser.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectionpath.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxsubscription.h>
#include <mpxlog.h>

#include "mpxplaybackserverdefs.h"
#include "mpxplaybackutilityimpl.h"

// ============================== MEMBER FUNCTIONS ============================

// ---------------------------------------------------------------------------
// Retrieves playback utility from TLS. Creates if it's not there.
// ---------------------------------------------------------------------------
//
MMPXPlaybackUtility* CMPXPlaybackUtility::UtilityL(
    const TUid& aModeId,
    const TMPXCategory aCategory)
    {
    MPX_FUNC("CMPXPlaybackUtility::UtilityL");
    MMPXPlaybackUtility* utility( NULL );
    // Currently only make the default mode a singleton since it's used the most
    // If needed we could make other modes a singleton as well.  This is why
    // a struct was used to store in the TLS so that more utilities can be
    // added easily.
    if ( KPbModeDefault == aModeId )
        {
        TMPXPlaybackUtilityStruct* s( reinterpret_cast<TMPXPlaybackUtilityStruct*>( Dll::Tls() ));
        if ( !s )
            {
            MPX_DEBUG1("CMPXPlaybackUtility::UtilityL(): Creating default mode");
            utility = CMPXPlaybackUtility::NewL( aModeId, NULL, aCategory );
            CleanupClosePushL( *utility );
            MPX_DEBUG2("CMPXPlaybackUtility::UtilityL(): utility = 0x%08x", utility);
            s = new (ELeave) TMPXPlaybackUtilityStruct;
            s->iDefaultPlaybackUtility = utility;
            Dll::SetTls( s );
            CleanupStack::Pop( utility );
            }
        else
            {
            utility = s->iDefaultPlaybackUtility;
            MPX_DEBUG2("CMPXPlaybackUtility::UtilityL(): Retrieving default mode 0x%08x", utility);
            static_cast<CMPXPlaybackUtility*>( utility )->iRefCount++;
            }
        }
    else
        {
        MPX_DEBUG1("CMPXPlaybackUtility::UtilityL(): Not default mode");
        utility = CMPXPlaybackUtility::NewL( aModeId, NULL, aCategory );
        }
    return utility;
    }

// ----------------------------------------------------------------------------
// Two phases constructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackUtility* CMPXPlaybackUtility::NewL(const TUid& aModeId,
                                               MMPXPlaybackObserver* aObs,
                                               const TMPXCategory aCategory)
    {
    CMPXPlaybackUtility* p=new(ELeave)CMPXPlaybackUtility();
    CleanupStack::PushL(p);
    p->ConstructL(aModeId,aObs,aCategory);
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackUtility::~CMPXPlaybackUtility()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::~CMPXPlaybackUtility");
    Cancel();
#ifdef _ENABLE_GUARD_TIMER
    delete iGuardTimer;
#endif
    if (iTaskQueue)
        {
        delete iTaskQueue;
        }
    if (iMsgMonitor)
        {
        delete iMsgMonitor;
        }
    iObservers.Close();
    iPbs.Close();
    iFile.Close();
    if (iBuffer)
        {
        delete iBuffer;
        }
    delete iMediaOnError;
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    iFile64.Close();
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    }

// ----------------------------------------------------------------------------
// C++ constructor
// Create a unique name out of thread ID and this pointer: no other instance of
// this object will have the same name; used to identify this object for
// recieving messages
// ----------------------------------------------------------------------------
//
CMPXPlaybackUtility::CMPXPlaybackUtility()
    : CActive(EPriorityStandard),
    iCallbackOngoing(EFalse)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// 2nd construtor
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::ConstructL(const TUid& aModeId,
                                     MMPXPlaybackObserver* aObs, 
                                     const TMPXCategory aCategory)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::ConstructL()");
    MPX_DEBUG4("CMPXPlaybackUtility::ConstructL this 0x%08x mode 0x%08x observer 0x%08x",
               this, aModeId.iUid, aObs);
    if (aObs)
        {
        AddObserverL(*aObs);
        }
    iTaskQueue = CMPXTaskQueue::NewL();
    User::LeaveIfError(iPbs.Connect(KMPXPlaybackServerName,
                                    KMPXPlaybackServerImg,
                                    TVersion(KMPXPlaybackServerMajorVersionNumber,
                                             KMPXPlaybackServerMinorVersionNumber,
                                             KMPXPlaybackServerBuildVersionNumber)));
    iPbs.SendReceiveL(EPbsSetMode,TIpcArgs(aModeId.iUid, aCategory));
    iMsgMonitor = CMPXMessageMonitor::NewL(iPbs, *this);
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer=CPeriodic::NewL(CActive::EPriorityStandard);
#endif
    iMediaOnError = CMPXMedia::NewL();
    iRefCount++;
    }

// ----------------------------------------------------------------------------
// Add a observer
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::AddObserverL(MMPXPlaybackObserver& aObs)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::AddObserverL()");
    iObservers.AppendL(&aObs);
    }

// ----------------------------------------------------------------------------
// Remove a observer
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::RemoveObserverL(MMPXPlaybackObserver& aObs)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::RemoveObserverL()");
    TInt i=iObservers.FindL(&aObs);
    iObservers.Remove(i);
    }

// ----------------------------------------------------------------------------
// Returns PIds of clients that are using the engine in the mode
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::GetClientsL(RArray<TProcessId>& aClients)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::GetClientsL()");
    TInt size = iPbs.SendReceiveL(EPbsGetClients);
    ::ArrayFromServerL<TProcessId>(iPbs, EPbsGetSyncBuffer,size,aClients);
    }

// ----------------------------------------------------------------------------
// Initializes a track given by aIndex in path aCollectionPath
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitL(
    const CMPXCollectionPlaylist& aPlaylist,
    TBool aPlay /*=ETrue*/ )
    {
    MPX_FUNC("CMPXPlaybackUtility::InitL()");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitL()");
    CBufBase* buffer(NULL);
    ::CreateBufferL<CMPXCollectionPlaylist>(aPlaylist, buffer);
    CleanupStack::PushL( buffer );
    TPtr8 p=buffer->Ptr(0);
    iPbs.SendReceiveL(EPbsInitFromCollection,TIpcArgs(&p,aPlay));
    CleanupStack::PopAndDestroy( buffer );
    }

// ----------------------------------------------------------------------------
// Inititialises with a single song, may not be part of any collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitL(const TDesC& aUri,const TDesC8* aType)
    {
    MPX_FUNC("CMPXPlaybackUtility::InitL(const TDesC& aUri,const TDesC8* aType)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitL()");
    const TDesC8& type=aType?*aType:KNullDesC8;
    TPtrC8 uri = MPXUser::Ptr(aUri);
    iPbs.SendReceiveL(EPbsInitFromUri,TIpcArgs(&uri,&type));
    }

// ----------------------------------------------------------------------------
// Inititialises with a single song, may not be part of any collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitL(RFile& aShareableFile)
    {
    MPX_FUNC("CMPXPlaybackUtility::InitL(RFile& aShareableFile)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitL()");
    if (!aShareableFile.SubSessionHandle())
        {
        User::Leave(KErrArgument);
        }
    TIpcArgs args;
    aShareableFile.TransferToServer(args,0,1);
    iPbs.SendReceiveL(EPbsInitFromFile,args);
    }

// ----------------------------------------------------------------------------
// Inititialises with a URI
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitStreamingL(const TDesC& aUri, const TDesC8* aType, const TInt aAccessPoint)
{
    MPX_FUNC("CMPXPlaybackUtility::InitStreamingL(const TDesC& aUri, const TInt aAccessPoint)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitStreamingL()");

    const TDesC8& type=aType?*aType:KNullDesC8;
    TPtrC8 uri = MPXUser::Ptr(aUri);
    iPbs.SendReceiveL(EPbsInitStreamingFromUri,TIpcArgs(&uri, &type, aAccessPoint));

}


// ----------------------------------------------------------------------------
// Inititialises with a file, may not be part of any collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitStreamingL(RFile& aShareableFile, const TInt aAccessPoint)
{
    MPX_FUNC("CMPXPlaybackUtility::InitStreamingL(RFile& aShareableFile, const TInt aAccessPoint)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitStreamingL()");
    if (!aShareableFile.SubSessionHandle())
        {
        User::Leave(KErrArgument);
        }
    TIpcArgs args;
    aShareableFile.TransferToServer(args,0,1); 
    args.Set(2, aAccessPoint);  //use index "2" for setting the AccessPoint   

    iPbs.SendReceiveL(EPbsInitStreamingFromFile, args);
}

// ----------------------------------------------------------------------------
// Frees up client side resources only; a player is freed when there are no
// clients using it, and all resources are freed when the last client closed
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::Close()
    {
    MPX_DEBUG2("CMPXPlaybackUtility::Close 0x%08x", this);
    ASSERT( iRefCount > 0 );
    if ( --iRefCount == 0 )
        {
        // last client released
        iMsgMonitor->Cancel();
        TMPXPlaybackUtilityStruct* s( reinterpret_cast<TMPXPlaybackUtilityStruct*>( Dll::Tls() ));
        if ( s )
            {
            if ( s->iDefaultPlaybackUtility == this )
                {
                delete s;
                Dll::SetTls( NULL );
                }
            }
        delete this;
        }
    }

// ----------------------------------------------------------------------------
// Stops any async operations that are currently under way
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::CancelRequest()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::CancelRequest()");
    Cancel();
    iTaskQueue->Reset();
    }

// ----------------------------------------------------------------------------
// Issue player commands, with optional data.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::CommandL(TMPXPlaybackCommand aCmd,TInt aData)
    {
    MPX_DEBUG2("-->CMPXPlaybackUtility::CommandL(%d)", aCmd);
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::CommandL()");
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    // All of current commands are sync
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, aCmd);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, aData);
    CommandL(*cmd, NULL);
    CleanupStack::PopAndDestroy(cmd);
    MPX_DEBUG2("<--CMPXPlaybackUtility::CommandL(%d)", aCmd);
    }

// ----------------------------------------------------------------------------
// Issue player commands
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::CommandL(
    CMPXCommand& aCmd,
    MMPXPlaybackCallback* aCallback/*=NULL*/)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::CommandL(CMPXCommand& aCmd)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::CommandL(CMPXCommand& aCmd)");
    ASSERT(aCmd.IsSupported(KMPXCommandGeneralId));
    TInt id = aCmd.ValueTObjectL<TInt>(KMPXCommandGeneralId);
    MPX_DEBUG2("CMPXPlaybackUtility::CommandL(): id = 0x%08x", id);
    if (KMPXCommandIdPlaybackGeneral==id)
        {
        ASSERT(aCmd.IsSupported(KMPXCommandPlaybackGeneralType));
        TMPXPlaybackCommand cmdType = static_cast<TMPXPlaybackCommand>(
                      aCmd.ValueTObjectL<TInt>(KMPXCommandPlaybackGeneralType));
        MPX_DEBUG2("CMPXPlaybackUtility::CommandL(): cmdType = %d", cmdType);
        if ( EPbCmdPlay == cmdType ||
             EPbCmdPlayPause == cmdType ||
             EPbCmdStop == cmdType)
            {
            if( !aCmd.IsSupported( KMPXCommandPlaybackGeneralClientPid ) )
                {
                aCmd.SetTObjectValueL<TProcessId> (
                       KMPXCommandPlaybackGeneralClientPid, RProcess().Id()); // current process id
                }
            }
        }

    TBool asyncVar(ETrue); // by default command is asynchronous
    if (aCmd.IsSupported(KMPXCommandGeneralDoSync))
        { // check if command is sync
        asyncVar=!(aCmd.ValueTObjectL<TBool>(KMPXCommandGeneralDoSync));
        }
    CBufBase* buf(NULL);
    ::CreateBufferL<CMPXCommand>( aCmd, buf );
    CleanupStack::PushL( buf );
    TPtr8 ptr = buf->Ptr(0);
    if (asyncVar)
        { // async request
        MPX_DEBUG1("CMPXPlaybackUtility::CommandL asynchronous command");		
        ASSERT(aCallback); // callback must be provided for asynchronous command
        // Increase reference count on command ownership transferred
        CMPXCommand* ref = CMPXCommand::NewL(aCmd);
        // Async version, Add request to the task queue
        CleanupStack::PushL(ref);
        AddRequestL(EPbsCommand, aCallback, asyncVar, buf, NULL, ref );
        CleanupStack::Pop(ref);
        CleanupStack::Pop( buf );  // ownership transferred to the queue
        }
    else
        { // send request if sync
        iPbs.SendReceiveL(EPbsCommand, TIpcArgs(asyncVar, &ptr));
        CleanupStack::PopAndDestroy( buf );
        }
    }

// ----------------------------------------------------------------------------
// Current state of player
// ----------------------------------------------------------------------------
//
TMPXPlaybackState CMPXPlaybackUtility::StateL() const
    {
    MPX_DEBUG2("CMPXPlaybackUtility::StateL 0x%08x", this);
    return static_cast<TMPXPlaybackState>(iPbs.SendReceiveL(EPbsGetState));
    }

// ----------------------------------------------------------------------------
// Determine whether there is a song by the state of the engine: if there is,
// its OK to return MMPXMedia, else NULL is returned
// ----------------------------------------------------------------------------
//
MMPXSource* CMPXPlaybackUtility::Source()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::Source()");
    TMPXPlaybackState s=EPbStateNotInitialised;
    TRAP_IGNORE(s=StateL());
    return (s==EPbStateNotInitialised ||
            s==EPbStateShuttingDown) ? NULL:this;
    }

// ----------------------------------------------------------------------------
// Get player manager
// ----------------------------------------------------------------------------
//
MMPXPlayerManager& CMPXPlaybackUtility::PlayerManager()
    {
    return *this;
    }

// ----------------------------------------------------------------------------
// Set playback property, EPropertyChanged event when complete
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SetL(TMPXPlaybackProperty aProperty,TInt aValue)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SetL()");
    iPbs.SendReceiveL(EPbsSetProperty,TIpcArgs(aProperty,aValue));
    }

// ----------------------------------------------------------------------------
// Send property request
// Result will be called back in HandleProperty
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::ValueL(MMPXPlaybackCallback& aCallback,
                                 TMPXPlaybackProperty aProperty)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::ValueL()");
    PropertyL(aCallback, aProperty);
    }

// ----------------------------------------------------------------------------
// Send property request
// Result will be called back in HandleProperty
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::PropertyL(
    MMPXPlaybackCallback& aCallback,
    TMPXPlaybackProperty aProperty)
    {
    MPX_DEBUG2("-->CMPXPlaybackUtility::PropertyL(%d)", aProperty);
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::PropertyL()");
    AddRequestL(EPbsGetProperty, &aCallback, aProperty);
    MPX_DEBUG2("<--CMPXPlaybackUtility::PropertyL(%d)", aProperty);
    }

// ----------------------------------------------------------------------------
// Return a list of mime types supported by playback framework
// ----------------------------------------------------------------------------
//
CDesCArray* CMPXPlaybackUtility::SupportedMimeTypes()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SupportedMimeTypes()");
    TInt size = 0;
    TRAPD( err, size = iPbs.SendReceiveL( EPbsGetSupportedMimeTypes ) );
    CDesCArray* desArray = NULL;
    if ( size && ( err == KErrNone ) )
        {
        TRAP_IGNORE(
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, size, iBuffer);
        MPXUser::CreateFromBufferL(*iBuffer, desArray));
        }
    return desArray;
    }

// ----------------------------------------------------------------------------
// Return a list of mime types supported by playback framework
// ----------------------------------------------------------------------------
//
CDesCArray* CMPXPlaybackUtility::SupportedExtensions()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SupportedExtensions()");
    TInt size = 0;
    TRAPD( err, size = iPbs.SendReceiveL( EPbsGetSupportedExtensions ) );
    CDesCArray* desArray = NULL;
    if ( size && ( err == KErrNone ) )
        {
        TRAP_IGNORE(
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, size, iBuffer);
        MPXUser::CreateFromBufferL(*iBuffer, desArray));
        }
    return desArray;
    }

// ----------------------------------------------------------------------------
// Return a list of mime types supported by playback framework
// ----------------------------------------------------------------------------
//
CDesCArray* CMPXPlaybackUtility::SupportedSchemas()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SupportedSchemas()");
    TInt size = 0;
    TRAPD( err, size = iPbs.SendReceiveL( EPbsGetSupportedSchemas ) );
    CDesCArray* desArray = NULL;
    if ( size && ( err == KErrNone ) )
        {
        TRAP_IGNORE(
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, size, iBuffer);
        MPXUser::CreateFromBufferL(*iBuffer, desArray));
        }
    return desArray;
    }

// ----------------------------------------------------------------------------
// Sets the priority of the playback utility
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SetPriority( TInt aPriority )
    {
    MPX_DEBUG2("-->CMPXPlaybackUtility::SetPriority(%d)", aPriority);
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::SetPriority()");
    CActive::SetPriority( aPriority );
    MPX_DEBUG2("-->CMPXPlaybackUtility::SetPriority(%d)", aPriority);
    }


// ----------------------------------------------------------------------------
// CMPXPlaybackUtility::AddSubscriptionL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::AddSubscriptionL(
    const CMPXSubscription& aSubscription)
    {
    const CMPXMediaArray* items = aSubscription.ItemsL();
    User::LeaveIfNull(const_cast<CMPXMediaArray*>(items));
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionAdd);
    cmd->SetCObjectValueL(KMPXCommandSubscriptionAddItems, items);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd);
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackUtility::RemoveSubscriptionL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::RemoveSubscriptionL(
    const CMPXSubscription& aSubscription)
    {
    const CMPXMediaArray* items = aSubscription.ItemsL();
    User::LeaveIfNull(const_cast<CMPXMediaArray*>(items));
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionRemove);
    cmd->SetCObjectValueL(KMPXCommandSubscriptionAddItems, items);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd);
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackUtility::ClearSubscriptionsL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::ClearSubscriptionsL()
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL(cmd);
    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandSubscriptionRemoveAll);
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    CommandL(*cmd);
    CleanupStack::PopAndDestroy(cmd);
    }
// ----------------------------------------------------------------------------
// Get a list of player types
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::GetPlayerTypesL(
    RArray<TMPXPlaybackPlayerType>& aTypes)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::GetPlayerTypesL()");
    TInt size = iPbs.SendReceiveL(EPbsGetPlayerTypes);
    ::ArrayFromServerL<TMPXPlaybackPlayerType>(iPbs, EPbsGetSyncBuffer,size,aTypes);
    }

// ----------------------------------------------------------------------------
// Returns display name for custom types
// ----------------------------------------------------------------------------
//
HBufC* CMPXPlaybackUtility::PlayerTypeDisplayNameL(TMPXPlaybackPlayerType aType)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::PlayerTypeDisplayNameL()");
    return DesL(iPbs.SendReceiveL(EPbsGetPlayerTypeDisplayName,TIpcArgs(aType)));
    }

// ----------------------------------------------------------------------------
// Get the list of UIDs of all players
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::GetPlayerListL(RArray<TUid>& aPlayers)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::GetPlayerListL(Rarray)");
    TInt size = iPbs.SendReceiveL(EPbsGetAllPlayersUids);
    ::ArrayFromServerL<TUid>(iPbs, EPbsGetSyncBuffer,size,aPlayers);
    }

// ----------------------------------------------------------------------------
// Get the list of UIDs of players with the specific type
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::GetPlayerListL(RArray<TUid>& aPlayers,
                                         TMPXPlaybackPlayerType aType)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::GetPlayerListL(TMPXPlaybackPlayerType)");
    TInt size = iPbs.SendReceiveL(EPbsGetPlayersUidsForType, TIpcArgs(aType));
    ::ArrayFromServerL<TUid>(iPbs, EPbsGetSyncBuffer,size,aPlayers);
    }

// ----------------------------------------------------------------------------
// Asynchronous method: when server completes message, RunL() will be called
// and then the observer is notified of the results. Data required for the
// results that is not supplied by the server is stored; data supplied by the
// server is written back into packaged descriptors in this address space
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SubPlayerNamesL(MMPXPlaybackCallback& aCallback,
                                          TUid aPlayer)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SubPlayerNamesL()");
    AddRequestL(EPbsGetSubPlayerNamesByUid, &aCallback, aPlayer.iUid);
    }

// ----------------------------------------------------------------------------
// Selects all players with the type
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SelectPlayersL(TMPXPlaybackPlayerType aType)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SelectPlayersL(TMPXPlaybackPlayerType)");
    iPbs.SendReceiveL(EPbsSelectPlayerByType,TIpcArgs(aType));
    }

// ----------------------------------------------------------------------------
// Selects a specific player
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SelectPlayerL(TUid aPlayer)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SelectPlayersL(TUid)");
    iPbs.SendReceiveL(EPbsSelectPlayerByUid,TIpcArgs(aPlayer.iUid));
    }

// ----------------------------------------------------------------------------
// Selects a specific player and sub player
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SelectSubPlayerL(TUid aPlayer,TInt aSubPlayerIndex)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SelectSubPlayerL()");
    iPbs.SendReceiveL(EPbsSelectSubPlayer,
                      TIpcArgs(aPlayer.iUid,aSubPlayerIndex));
    }

// ----------------------------------------------------------------------------
// Clears all selection criteria.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::ClearSelectPlayersL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::ClearSelectPlayersL()");
    iPbs.SendReceiveL(EPbsClearPlayerSelection);
    }

// ----------------------------------------------------------------------------
// Retreives the current selection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::GetSelectionL(TMPXPlaybackPlayerType& aType,
                                        TUid& aPlayer,
                                        TInt& aSubPlayerIndex,
                                        HBufC*& aSubPlayerName)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::GetSelectionL()");
    if (aSubPlayerName)
        {
        delete aSubPlayerName;
        aSubPlayerName = NULL;
        }
    TPckg<TMPXPlaybackPlayerType> type(aType);
    TPckg<TUid> uid(aPlayer);
    TPckg<TInt> index(aSubPlayerIndex);
    TPckgBuf<TInt> size;
    iPbs.SendReceiveL(EPbsGetSelection,TIpcArgs(&type,&uid,&index, &size));
    if (size())
        {
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, size(), iBuffer);
        aSubPlayerName = MPXUser::Ptr(iBuffer->Ptr(0)).AllocL();
        } // else aSubPlayer return NULL;
    }

// ----------------------------------------------------------------------------
// The current player
// ----------------------------------------------------------------------------
//
MMPXPlayer* CMPXPlaybackUtility::CurrentPlayer()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::CurrentPlayer()");
    TInt r=0;
    TRAP_IGNORE(r=iPbs.SendReceiveL(EPbsPlayerFound));
    return r ? this : NULL;
    }

// ----------------------------------------------------------------------------
// The 'type' of the player
// ----------------------------------------------------------------------------
//
TMPXPlaybackPlayerType CMPXPlaybackUtility::TypeL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::TypeL()");
    return static_cast<TMPXPlaybackPlayerType>(
                                iPbs.SendReceiveL(EPbsGetPlayerType));
    }

// ----------------------------------------------------------------------------
// The name of player type
// ----------------------------------------------------------------------------
//
HBufC* CMPXPlaybackUtility::TypeNameL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::TypeNameL()");
    return DesL(iPbs.SendReceiveL(EPbsGetTypeName));
    }

// ----------------------------------------------------------------------------
// Get sub players
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::SubPlayerNamesL(MMPXPlaybackCallback& aCallback)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::SubPlayerNamesL()");
    SubPlayerNamesL(aCallback,UidL());
    }

// ----------------------------------------------------------------------------
// Selected sub player index
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackUtility::SubPlayerL() const
    {
    MPX_DEBUG2("CMPXPlaybackUtility::SubPlayerL 0x%08x", this);
    return iPbs.SendReceiveL(EPbsGetSubPlayerIndex);
    }

// ----------------------------------------------------------------------------
// The UID identifying this player
// ----------------------------------------------------------------------------
//
TUid CMPXPlaybackUtility::UidL() const
    {
    MPX_DEBUG2("-->CMPXPlaybackUtility::UidL 0x%08x", this);
    TPckgBuf<TInt> uidPkg;
    iPbs.SendReceiveL(EPbsGetPlayerUid, TIpcArgs(&uidPkg));
    MPX_DEBUG2("<--CMPXPlaybackUtility::UidL 0x%08x", this);
    return TUid::Uid(uidPkg());
    }

// ----------------------------------------------------------------------------
// Path to the collection
// ----------------------------------------------------------------------------
//
CMPXCollectionPlaylist* CMPXPlaybackUtility::PlaylistL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::PlaylistL()");
    CMPXCollectionPlaylist *p = NULL;
    TInt size = iPbs.SendReceiveL(EPbsGetCollectionPlaylist);
    if (size > 0)
        {
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, size, iBuffer);
        ::NewFromBufferL<CMPXCollectionPlaylist>(*iBuffer, p);
        }
    return p;
    }

// ----------------------------------------------------------------------------
// Return file handle
// ----------------------------------------------------------------------------
//
RFile* CMPXPlaybackUtility::FileL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::FileL()");
    TPckgBuf<TInt> fileHandle;
    TInt fsHandle = iPbs.SendReceiveL(EPbsGetFile,TIpcArgs(&fileHandle));
    RFile* file(NULL);
    if (KErrNotFound != fileHandle())
        {
        iFile.Close();
        User::LeaveIfError(iFile.AdoptFromServer(fsHandle,fileHandle()));
        file = &iFile;
        } // else return NULL
    return file;
    }

// ----------------------------------------------------------------------------
// URI of current song
// ----------------------------------------------------------------------------
//
HBufC* CMPXPlaybackUtility::UriL()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::UriL()");
    return DesL(iPbs.SendReceiveL(EPbsGetUri));
    }

// ----------------------------------------------------------------------------
// Request for media properties.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::MediaL(
    const TArray<TMPXAttribute>& aAttrs,
    MMPXPlaybackCallback& aCallback)
    {
    MPX_FUNC_EX( "-->CMPXPlaybackUtility::MediaL 2 parameters" );
    MediaL( aAttrs, aCallback, NULL );
    }

// ----------------------------------------------------------------------------
// Request for media properties.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::MediaL(
    const TArray<TMPXAttribute>& aAttrs,
    MMPXPlaybackCallback& aCallback,
    CMPXAttributeSpecs* aSpecs)
    {
    MPX_FUNC_EX( "-->CMPXPlaybackUtility::MediaL 3 parameters" );
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::MediaL()");

    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandContentIdMedia);

    CBufBase* buf = CBufFlat::NewL( KMPXBufGranularity );
    CleanupStack::PushL( buf );
    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    // externalize attributes array
    ::ExternalizeL(aAttrs, writeStream);
    // Close and compress buffer
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy( &writeStream );
    TPtrC ptr = MPXUser::Ptr( buf->Ptr( 0 ) );
    cmd->SetTextValueL( KMPXCommandMediaAttribute, ptr );
    CleanupStack::PopAndDestroy( buf );

    CMPXAttributeSpecs* attrSpecs = aSpecs ?
            CMPXAttributeSpecs::NewL(*aSpecs) : CMPXAttributeSpecs::NewL();
    CleanupStack::PushL(attrSpecs);
    cmd->SetCObjectValueL<CMPXAttributeSpecs>(
        KMPXCommandMediaAttributeSpecs, attrSpecs );
    CleanupStack::PopAndDestroy(attrSpecs);

    AddRequestL(EPbsGetMedia, &aCallback, 0, NULL, (TAny*)cmd, cmd );
    CleanupStack::Pop( cmd ); // Ownership transferred to the task queue
    }

// ----------------------------------------------------------------------------
// Server has completed the message, and if there's no error, the results are
// available. The server completes with the function id (or error) so the correct
// callback is made
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::RunL()
    {
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer->Cancel();
#endif
    TInt err=iStatus.Int();
    TInt task = iTaskQueue->Task();
    MPX_DEBUG4("CMPXPlaybackUtility::RunL 0x%08x task %d err%d",
            this, task, err);
    if (KErrNotFound == task)
        {
        return;
        }
    
    // Try again in case server is down for IAD
    if (err == KErrDied || err == KErrServerTerminated)
        {
        MPX_DEBUG3("-->CMPXPlaybackUtility::RunL Reconnecting session for IAD 0x%08x task %d",
                this, task);
        
        // attempt to bring the server back up
        err = iPbs.Reconnect();
        
        if (err != KErrNone)
            {
            MPX_DEBUG1("-->CMPXPlaybackUtility::RunL Reconnect Failed");
            }
        }
        
    User::LeaveIfError(err);
    MMPXPlaybackCallback* cb =
            static_cast<MMPXPlaybackCallback*>(iTaskQueue->Callback());
    switch(task)
        {
        case EPbsGetProperty:
            {
            iCallbackOngoing = ETrue;
            cb->HandlePropertyL(
                    static_cast<TMPXPlaybackProperty>(iTaskQueue->Param()),
                    iPropertyValuePckg(), err);
            iCallbackOngoing = EFalse;
            break;
            }
        case EPbsGetSubPlayerNamesByUid:
            {
            if (KErrNone == err)
                {
                CDesCArray* desArray = NULL;
                if (iResultSizePckg())
                    {
                    ::TransferBufferFromServerL(iPbs, EPbsGetAsyncBuffer,
                                                iResultSizePckg(), iBuffer);
                    MPXUser::CreateFromBufferL(*iBuffer, desArray);
                    CleanupStack::PushL(desArray);
                    }
                iCallbackOngoing = ETrue;
                cb->HandleSubPlayerNamesL(TUid::Uid(iTaskQueue->Param()),
                                          desArray, iCompletePckg(),err);
                if (!iCompletePckg())
                    { // Add next request in the task queue, but not mess up runerror
                    TRAPD(err, SubPlayerNamesL(*cb,TUid::Uid(iTaskQueue->Param())));
                    if (KErrNone!=err)
                        { // complete searching sub players when error on request
                        cb->HandleSubPlayerNamesL(TUid::Uid(iTaskQueue->Param()),
                                                  desArray, ETrue, err);
                        }
                    }
                iCallbackOngoing = EFalse;
                if (desArray)
                    {
                    CleanupStack::PopAndDestroy(desArray);
                    }
                }
            else
                {
                iCallbackOngoing = ETrue;
                cb->HandleSubPlayerNamesL(TUid::Uid(iTaskQueue->Param()),
                                         NULL, ETrue, err);
                iCallbackOngoing = EFalse;
                }
            break;
            }
        case EPbsGetMedia:
            {
            if (iResultSizePckg() && KErrNone == err)
                {
                CMPXMedia* media(NULL);
                ::TransferBufferFromServerL(iPbs, EPbsGetAsyncBuffer,
                                            iResultSizePckg(), iBuffer);
                ::NewFromBufferL(*iBuffer, media);
                CleanupStack::PushL(media);
                iCallbackOngoing = ETrue;
                cb->HandleMediaL(*media, err);
                iCallbackOngoing = EFalse;
                CleanupStack::PopAndDestroy(media);
                }
            else
                {
                iCallbackOngoing = ETrue;
                cb->HandleMediaL(*iMediaOnError, err); // return dummy media
                iCallbackOngoing = EFalse;
                }
            break;
            }
        case EPbsCommand:
            {
            CMPXCommand* result = NULL;
            if (KErrNone == err)
                {
                if(iResultSizePckg())
                    {
                    ::TransferBufferFromServerL(iPbs,EPbsGetAsyncBuffer,
                                                iResultSizePckg(), iBuffer);
                    ::NewFromBufferL<CMPXCommand>(*iBuffer, result);
                    }
                CleanupStack::PushL(result);
                iCallbackOngoing = ETrue;
                cb->HandlePlaybackCommandComplete(result, KErrNone);
                iCallbackOngoing = EFalse;
                CleanupStack::PopAndDestroy(result);
                }
            else
                {
                iCallbackOngoing = ETrue;
                cb->HandlePlaybackCommandComplete(NULL, err);
                iCallbackOngoing = EFalse;
                }
            break;
            }
        default:
            ASSERT(0);
        }
    MPX_DEBUG2("CMPXPlaybackUtility::RunL remove a task from queue 0x%08x",
               iTaskQueue);
    iTaskQueue->RemoveTask();
    ExecuteNextRequest();
    }

// ----------------------------------------------------------------------------
// Canceling by the server
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::DoCancel()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::DoCancel()");
#ifdef _ENABLE_GUARD_TIMER
    iGuardTimer->Cancel();
#endif
    TRAP_IGNORE(iPbs.SendReceiveL(EPbsCancelRequest));
    }

// ----------------------------------------------------------------------------
// Sends error message to all observers - maybe it should only be to the
// client that calls the async method
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackUtility::RunError(TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::RunError");
    TRAP_IGNORE(HandleRunErrorL(aError));
    iCallbackOngoing = EFalse;
    MPX_DEBUG2("CMPXPlaybackUtility::RunError remove a task from queue 0x%08x",
               iTaskQueue);
    iTaskQueue->RemoveTask();
    ExecuteNextRequest();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Distributes messages to all observers
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::MessageReceived(TInt aMsgData, TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::MessageReceived()");
    TRAP_IGNORE(HandleMessageL(aMsgData,aError));
    }

// ----------------------------------------------------------------------------
// Distributes messages to all observers
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::HandleMessageL(TInt aMsgData, TInt aError)
    {
    ASSERT(aMsgData>0 || aError); // aMessage should not be NULL and aError is KErrNone
    MPX_DEBUG4("CMPXPlaybackUtility::HandleMessageL 0x%08x, MsgHandle 0x%08x, Err %d",
               this, aMsgData, aError);
    if (aMsgData > 0)
        {
        CMPXMessage* msg = CMPXMessage::NewL(aMsgData);
        CleanupStack::PushL(msg);
        ASSERT(msg->IsSupported(KMPXMessageGeneralId));
        TInt id = static_cast<TInt>(
                    msg->ValueTObjectL<TMPXMessageId>(KMPXMessageGeneralId));
        if (id==KMPXMessageGeneral)
            { // DEPRECATED, covert the general message into TMPXMessage
            ASSERT(msg->IsSupported(KMPXMessageGeneralEvent));
            TMPXPlaybackMessage::TEvent event =
                static_cast<TMPXPlaybackMessage::TEvent>(
                             msg->ValueTObjectL<TInt>(KMPXMessageGeneralEvent));
            if (event == TMPXPlaybackMessage::EActivePlayerChanged)
                {
                ASSERT(msg->IsSupported(KMPXMessageGeneralType));
                TInt type(msg->ValueTObjectL<TInt>(KMPXMessageGeneralType));
                ASSERT(msg->IsSupported(KMPXMessageGeneralData));
                TInt data(msg->ValueTObjectL<TInt>(KMPXMessageGeneralData));
                if ( type)
                    { // receive this message because this client attached to
                      // ActivePlayer
                    MPX_DEBUG2("CMPXPlaybackUtility::MessageReceived 0x%08x, rebinds to active player",
                               this);
                    // Cancel request to old player
                    CancelRequest();
                    TRAP_IGNORE(iPbs.SendReceiveL(EPbsSetMode,
                                           TIpcArgs(KPbModeActivePlayer.iUid, EMPXCategoryMusic)));
                    }
                }
            }
        for (TInt i = iObservers.Count(); --i >= 0;)
            {
            iObservers[i]->HandlePlaybackMessage(msg, aError);
            }
        CleanupStack::PopAndDestroy(msg);
        }
    else
        {
        for (TInt i = iObservers.Count(); --i >= 0;)
            {
            iObservers[i]->HandlePlaybackMessage(NULL, aError);
            }
        }
    iMsgMonitor->GetNextMessage();
    }

// ----------------------------------------------------------------------------
//  Handle error in RunL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::HandleRunErrorL(TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::HandleRunErrorL()");
    TInt task = iTaskQueue->Task();
    MMPXPlaybackCallback* cb =
            static_cast<MMPXPlaybackCallback*>(iTaskQueue->Callback());
    if (!iCallbackOngoing)
        { // Error happens during preparing callback
        iCallbackOngoing = ETrue; // will be reset to EFalse in the RunError
        switch(task)
            {
            case EPbsGetProperty:
                {
                cb->HandlePropertyL(
                        static_cast<TMPXPlaybackProperty>(iTaskQueue->Param()),
                        0, // dummy value on error
                        aError);
                break;
                }
            case EPbsGetSubPlayerNamesByUid:
                {
                cb->HandleSubPlayerNamesL(TUid::Uid(iTaskQueue->Param()),
                                          NULL, ETrue, aError);
                break;
                }
            case EPbsGetMedia:
                {
                cb->HandleMediaL(*iMediaOnError, aError);
                break;
                }
            case EPbsCommand:
                {
                cb->HandlePlaybackCommandComplete(iMediaOnError, aError);
                break;
                }
            default:
                {
                ASSERT(0);
                break;
                }
            }
        } // do nothing if error happens during client processing callback.
          // all callback should be changed into non-leave so that this will
          // be never happen.
    }


// ----------------------------------------------------------------------------
// Read a buffer from server
// ----------------------------------------------------------------------------
//
HBufC* CMPXPlaybackUtility::DesL(TInt aSize)
    {
    HBufC* des=NULL;
    if (aSize)
        {
        ::TransferBufferFromServerL(iPbs, EPbsGetSyncBuffer, aSize, iBuffer);
        TPtrC ptr = MPXUser::Ptr(iBuffer->Ptr(0));
        des = ptr.AllocL();
        }
    return des;
    }

// ----------------------------------------------------------------------------
// Adds a sync request to the queue: if there is no outstanding request,
// it will be executed immediately
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::AddRequestL(TMPXPlaybackServerOp aFunction,
                                      MMPXPlaybackCallback* aCallback/*=NULL*/,
                                      TInt aParamData/*=0*/,
                                      CBufBase* aBuf/*=NULL*/,
                                      TAny* aPtr,
                                      CBase* aAlivePtr/*=NULL*/)
    {
    iTaskQueue->AddTaskL(aFunction, aCallback, aParamData, aBuf, aPtr, aAlivePtr, NULL);
    MPX_DEBUG3("CMPXPlaybackUtility::AddRequestL this 0x%08x task=%d",
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
void CMPXPlaybackUtility::ExecuteNextRequest()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::ExecuteNextRequest()");
    TInt task = iTaskQueue->Task();
    if (KErrNotFound != task && !IsActive())
        {
        switch(task)
            {
            case EPbsGetProperty:
                iPbs.SendReceive(EPbsGetProperty,
                             TIpcArgs(iTaskQueue->Param(), &iPropertyValuePckg),
                             iStatus);
                break;
            case EPbsGetSubPlayerNamesByUid:
                iPbs.SendReceive(EPbsGetSubPlayerNamesByUid,
                                 TIpcArgs(iTaskQueue->Param(),
                                          &iResultSizePckg,&iCompletePckg),
                                 iStatus);
                break;
            case EPbsGetMedia:
                iPbs.SendReceive(task,
                         TIpcArgs(&iResultSizePckg,
                         ((CMPXCommand*)(iTaskQueue->PtrData()))->Data()),
                         iStatus);
                break;
            case EPbsCommand:
                {
                iPbs.SendReceive(EPbsCommand,
                                 TIpcArgs(iTaskQueue->Param(), //[in] Async flag
                                          &iTaskQueue->BufData(),//[in] command
                                          &iResultSizePckg), //[out] size of buffer
                                 iStatus);
                break;
                }
            default:
                ASSERT(0);
            }
    #ifdef _ENABLE_GUARD_TIMER
         if ( iGuardTimer->IsActive() );
             iGuardTimer->Cancel();
         iGuardTimer->Start(KMPXGuardTimer,KMPXGuardTimer,
                            TCallBack(GuardTimerCallback,this));
    #endif
         SetActive();
         MPX_DEBUG3("CMPXPlaybackUtility::ExecuteNextRequest 0x%08x task %d ",
                 this, task);
        }
    }
#ifdef _ENABLE_GUARD_TIMER
// ----------------------------------------------------------------------------
// Guard timer time out
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackUtility::GuardTimerCallback(TAny* aPtr)
    {
    CMPXPlaybackUtility* pb = static_cast<CMPXPlaybackUtility*>(aPtr);
    MPX_DEBUG3("CMPXPlaybackUtility::GaurdTimerCallback this 0x%08x task=%d",
               pb, pb->iTaskQueue->Task());
    pb->iGuardTimer->Cancel();
    ASSERT(0); // the function should never be called
    return KErrNone;
    }
#endif

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
// ----------------------------------------------------------------------------
// Inititialises with a single song, may not be part of any collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::Init64L(RFile64& aShareableFile)
    {
    MPX_FUNC("CMPXPlaybackUtility::Init64L(RFile64& aShareableFile)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::Init64L()");
    if (!aShareableFile.SubSessionHandle())
        {
        User::Leave(KErrArgument);
        }
    TIpcArgs args;
    aShareableFile.TransferToServer(args,0,1);
    iPbs.SendReceiveL(EPbsInitFromFile64,args);
    }
 
// ----------------------------------------------------------------------------
// Inititialises with a file, may not be part of any collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackUtility::InitStreaming64L(RFile64& aShareableFile, const TInt aAccessPoint)
    {
    MPX_FUNC("CMPXPlaybackUtility::InitStreaming64L(RFile64& aShareableFile, const TInt aAccessPoint)");
    MPX_DEBUG_THREAD("CMPXPlaybackUtility::InitStreaming64L()");
    if (!aShareableFile.SubSessionHandle())
        {
        User::Leave(KErrArgument);
        }
    TIpcArgs args;
    aShareableFile.TransferToServer(args,0,1); 
    args.Set(2, aAccessPoint);  //use index "2" for setting the AccessPoint   

    iPbs.SendReceiveL(EPbsInitStreamingFromFile64, args);
    }

// ----------------------------------------------------------------------------
// Return file handle
// ----------------------------------------------------------------------------
//
RFile64* CMPXPlaybackUtility::File64L()
    {
    MPX_FUNC_EX("CMPXPlaybackUtility::File64L()");
    TPckgBuf<TInt> fileHandle;
    TInt fsHandle = iPbs.SendReceiveL(EPbsGetFile64,TIpcArgs(&fileHandle));
    RFile64* file(NULL);
    if (KErrNotFound != fileHandle())
        {
        iFile64.Close();
        User::LeaveIfError(iFile64.AdoptFromServer(fsHandle,fileHandle()));
        file = &iFile64;
        } // else return NULL
    return file;
    }

#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

// ----------------------------------------------------------------------------
// Set primary client
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackUtility::SetPrimaryClientL()
    {
    MPX_FUNC("CMPXPlaybackUtility::SetPrimaryClientL");
    return iPbs.SendReceiveL(EPbsSetPrimaryClient);
    }
// End of file
