/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Playback server
*
*/


#include <e32std.h>
#include <e32svr.h>
#ifdef RD_TSP_CLIENT_MAPPER
#include <tspclientmapper.h>
#endif
#include <mpxclientlist.h>
#include <mpxpluginmonitor.h>
#include <mpxlog.h>
#include "mpxplaybackserverdefs.h"
#include "mpxplaybackengine.h"
#include "mpxplaybackserversession.h"
#include "mpxplaybackserver.h"

// CONSTANTS

// Server Security Policy
const TUint KMPXPlaybackServerRangeCount = 2;
const TInt KMPXPlaybackServerRanges[KMPXPlaybackServerRangeCount] =
    {
    0,                  //range is [0-EPbsServerOpEnd)
    EPbsServerOpEnd,    //range is [EPbsServerOpEnd-KMaxTInt]
    };
const TUint8 KMPXPlaybackSeverElementsIndex[KMPXPlaybackServerRangeCount] =
    {
    0,                            //applies to range [0-EPbsServerOpEnd)
    CPolicyServer::ENotSupported, //applies to range [EPbsServerOpEnd-KMaxTInt]
    };

const CPolicyServer::TPolicyElement KMPXPlaybackServerPolicyElements[] =
    {
    {_INIT_SECURITY_POLICY_C1(ECapability_None),
                              CPolicyServer::EFailClient},
    };

const CPolicyServer::TPolicy KMPXPlaybackServerPolicy =
    {
    CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
    KMPXPlaybackServerRangeCount,
    KMPXPlaybackServerRanges,
    KMPXPlaybackSeverElementsIndex,
    KMPXPlaybackServerPolicyElements,
    };

// ----------------------------------------------------------------------------
// Start playback server
//
LOCAL_C void StartServerL()
    {
    User::LeaveIfError(User::RenameThread(KMPXPlaybackServerName));
    CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    CMPXPlaybackServer* server = CMPXPlaybackServer::NewL();
    CleanupStack::PushL(server);
    RProcess::Rendezvous(KErrNone);
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy(server);
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackServer* CMPXPlaybackServer::NewL()
    {
    CMPXPlaybackServer *pS = new(ELeave) CMPXPlaybackServer(
                                                CActive::EPriorityStandard,
                                                KMPXPlaybackServerPolicy);
    CleanupStack::PushL(pS);
    pS->ConstructL();
    CleanupStack::Pop(pS);
    return pS;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXPlaybackServer::CMPXPlaybackServer(TInt aPriority, const TPolicy &aPolicy)
    : CPolicyServer(aPriority, aPolicy)
    {}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackServer::~CMPXPlaybackServer()
    {
    iPlayers.ResetAndDestroy();
    delete iPluginMonitor;
#ifdef RD_TSP_CLIENT_MAPPER
    delete iClientMapper;
#endif
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::ConstructL()
    {
    StartL(KMPXPlaybackServerName);
    iPluginMonitor = CMPXPluginMonitor::NewL(KMPXPlaybackInterfaceUid);
#ifdef RD_TSP_CLIENT_MAPPER
    iClientMapper = CTspClientMapper::NewL();
#endif
    }

// ----------------------------------------------------------------------------
// Searches process id for target selector plugin.
// When the player has been opened in a mode, which is bound to the same engine
// than certain application's engine, then need to fetch its process id.
// (As a reference for parameter aMode see MMPXPlaybackUtility modes.)
// Otherwise target selector plugin is notified wrongly about client statuses 
// and key events are not propagated to correct application.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::FindProcessIdForTsp(
    const CMPXPlaybackEngine* aEngine,
    TProcessId& aProcessId )
    {
    TBool processFound( EFalse );
    TFindProcess processFinder;
    TFullName processName;

    while ( processFinder.Next( processName ) == KErrNone && !processFound )
        {
        RProcess process;
        TInt err = process.Open( processFinder );
        if( err == KErrNone )
            {
            if( process.SecureId().iId == aEngine->ModeId().iUid && 
                process.ExitType() == EExitPending )
                {
                MPX_DEBUG4("CMPXPlaybackServer::FindProcessIdForTsp(): pid changed from %d to %d (mode 0x%x)",
                           TUint(aProcessId), TUint(process.Id()), aEngine->ModeId().iUid);
                aProcessId = process.Id();
                processFound = ETrue;
                }
            process.Close();
            }
        }
    }

// ----------------------------------------------------------------------------
// Increments number of sessions this server holds
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::AddClient()
    {
    iClients++;
    }


// ----------------------------------------------------------------------------
// Find the player that has client aName, and remove it from that player; if
// that player has no clients, then delete that player. If the server has no
//  more client sessions, then stop the server.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::RemoveClient(const CMPXMessageQueue& aMsgQueue)
    {
    MPX_DEBUG2("CMPXPlaybackServer::RemoveClient from %d",
                iPlayers.Count());
    for (TInt i=iPlayers.Count();--i>=0;)
        {
        CMPXPlaybackEngine* p=iPlayers[i];
        CMPXClientList* cl=p->ClientList();
        TInt index(cl->Find(aMsgQueue));
        if (KErrNotFound != index)
            {
            MPX_DEBUG1("CMPXPlaybackServer::RemoveClient found client");
            cl->RemoveClient(index);
            if (cl->ClientCount()==0)
                {
                MPX_DEBUG1("CMPXPlaybackServer::RemoveClient delete a player");
                CMPXPlaybackEngine* enginePtr = p; 
                delete p;
				//Due to callbacks in PlaybackServer we have to remove engine from the iPlayers array after deleting.
                //enginePtr is a invalid pointer as p is already deleted
    		    //Find the index of deleted engine using its address
				TInt engineIndex = iPlayers.Find(enginePtr);
                if ( engineIndex != KErrNotFound )
                    {
                    iPlayers.Remove(engineIndex);
                    }
                }
            break;
            }
        }
    iClients--;
    ASSERT(iClients>=0);
    if (iClients==0)
        {
        CActiveScheduler::Stop();
        }
    }

// ----------------------------------------------------------------------------
// Return last active process id
// ----------------------------------------------------------------------------
//
TProcessId CMPXPlaybackServer::LastActiveProcessId()
    {
    TProcessId id(KNullProcessId);
    if (iPlayers.Count())
        {
        id = iPlayers[0]->LastActiveProcessId();
        }
    return id;
    }

// ----------------------------------------------------------------------------
// Creates a brand new player to the player list with this client name and TID,
// and whether it's the music player.
// ----------------------------------------------------------------------------
//
CMPXPlaybackEngine* CMPXPlaybackServer::AddNewPlayerL(
    TThreadId aClientId,
    const TUid& aModeId,
    CMPXMessageQueue* aMsgQueue,
    const TInt aCategory) 
    {
    CMPXPlaybackEngine* p =
                CMPXPlaybackEngine::NewL(*iPluginMonitor,*this,this,aModeId, aCategory); 
    MPX_DEBUG2("CMPXPlaybackServer::AddNewPlayerL 0x%08x", p);
    CleanupStack::PushL(p);
    p->ClientList()->AddClientL(aClientId, aModeId.iUid, aMsgQueue, aCategory); 
    iPlayers.AppendL(p);
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Create player according to the mode:
// ----------------------------------------------------------------------------
//
CMPXPlaybackEngine* CMPXPlaybackServer::CreatePlayerL(
    const TUid& aModeId,
    const TInt aCategory, 
    TThreadId aClientId,
    CMPXMessageQueue* aMsgQueue)
    {
    CMPXPlaybackEngine* p=NULL;
    if (aModeId == KPbModeDefault)
        {
        // Add the new client to the player created by another client running
        // in the same thread. If there isn't one, then create the first player
        // for this thread
        //
        p=AddToDefaultPlayerL(aClientId,aMsgQueue,aCategory);
        if(!p)
            {

            p=AddNewPlayerL(aClientId, aModeId, aMsgQueue, aCategory);
            }
        }
        //
    else if (aModeId == KPbModeNewPlayer)
        {
        //
        // Create a brand new player for this client
        //
        p=AddNewPlayerL(aClientId, aModeId, aMsgQueue, aCategory);
        }
    else if (aModeId == KPbModeActivePlayer)
        { 
        TInt index = IndexOfRecentActivePlayer(aCategory);
        if (KErrNotFound != index)
            {
            p = iPlayers[index]; // active & matching category
            }
        if (!p && iPlayers.Count()>0)
            { //no recent activeplayer, attach to the first player with matching category
            TInt count = iPlayers.Count();
            for(TInt i = 0; i < count; i++)
                {
                if(iPlayers[i]->Category() == aCategory)
                    {
                    p = iPlayers[i]; // not active, first matching category 
                    break;
                    }
                }
            }
        if (p)
            {
            p->ClientList()->AddClientL(aClientId,aModeId.iUid,aMsgQueue, aCategory);
            }
        else
            { // no player available (no active player with matching category)
            MPX_DEBUG1("CMPXPlaybackServer::CreatePlayerL create a new player");	
            p=AddNewPlayerL(aClientId, aModeId, aMsgQueue, aCategory);
            }
        }
    else
        {
        //
        // Add the new client to the player created by one application. If
        // there isn't one, then create the first player for the application
        //
        for (TInt i=iPlayers.Count();--i>=0;)
            {
            if (iPlayers[i]->ModeId() == aModeId)
                {
                p = iPlayers[i];
                p->ClientList()->AddClientL(aClientId,
                                            aModeId.iUid, aMsgQueue, aCategory);
                break;
                }
            }

        if (!p)
            {		
            p=AddNewPlayerL(aClientId, aModeId, aMsgQueue, aCategory);		
            }
        }
    return p;
    }

// ----------------------------------------------------------------------------
// Add the new client (defined by it's TID) to the player
// associated with the Tid aExistingClientId
// ----------------------------------------------------------------------------
//
CMPXPlaybackEngine* CMPXPlaybackServer::AddToDefaultPlayerL(
    TThreadId aClientId,
    CMPXMessageQueue* aMsgQueue,
    const TInt aCategory)
    {
    CMPXPlaybackEngine* p=NULL;
    for (TInt i=0; i < iPlayers.Count(); ++i)
        {
        p=iPlayers[i];
        CMPXClientList* cl=p->ClientList();
        TInt index = cl->Find(aClientId);
        if (KErrNotFound != index)
            {
            // if the player is an active player, change mode from default
            // to activeplayer. Default mode always uses the mode of first client
            TInt mode = (cl->ClientMode(index) == KPbModeActivePlayer.iUid) ?
                         KPbModeActivePlayer.iUid : KPbModeDefault.iUid;
            MPX_DEBUG3("CMPXPlaybackServer::AddToDefaultPlayerL mode 0x%08x, engine 0x%08x",
                       mode, p);				   
            cl->AddClientL(aClientId,mode,aMsgQueue, aCategory);
            break;
            }
        }
    return p;
    }

// ----------------------------------------------------------------------------
// Return the index of an active player. If there is no active player, return
// the index to the player in paused state which may be interrupted.
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackServer::IndexOfRecentActivePlayer(TInt aCategory)
    {
    TInt ret(KErrNotFound);
    for (TInt i=0; i<iPlayers.Count(); ++i)
        {
        CMPXPlaybackEngine* p = iPlayers[i];
        if (p->IsPlayerActive() && aCategory == p->Category())
            { // Found active player
            ret = i;
            break;
            }
        else if (EPbStatePaused == p->State() && KErrNotFound == ret && aCategory == p->Category())
            { // first paused player
            ret = i;
            }
        }
    return ret;
    }

TInt CMPXPlaybackServer::IndexOfRecentActivePlayer()
    {
    TInt ret(KErrNotFound);
    for (TInt i=0; i<iPlayers.Count(); ++i)
        {
        CMPXPlaybackEngine* p = iPlayers[i];
        if (p->IsPlayerActive())
            { // Found active player
            ret = i;
            break;
            }
        else if (EPbStatePaused == p->State() && KErrNotFound == ret)
            { // first paused player
            ret = i;
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Handle active player changed evnet.
// Notify all clients of EPbModeActivePlayer mode in inactive engines
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::HandleActiveEngineL(
    const CMPXPlaybackEngine* aEngine,
    TBool aActive)
    {
    MPX_DEBUG3("CMPXPlaybackServer::HandleActiveEngineL Engine: 0x%08x, Active %d",
               aEngine, aActive);
    TInt i(0);
    TInt indexOfActive = iPlayers.Find(aEngine);
    ASSERT(KErrNotFound!=indexOfActive);
    if (!aActive)
        {
#ifdef RD_TSP_CLIENT_MAPPER
        TProcessId lastPid( aEngine->LastActiveProcessId() ); 
        FindProcessIdForTsp( aEngine, lastPid );
        iClientMapper->SetTspTargetClientToOtherType(
                CTspClientMapper::EStoppedClients,
                lastPid);
        MPX_DEBUG2("CMPXPlaybackServer::HandleActiveEngineL(): Added as stopped client %d",
                   TUint( lastPid));
#endif
        if (EPbStatePaused != aEngine->State())
            {
            indexOfActive = IndexOfRecentActivePlayer();
            }
        else
            { // else no need to rebind. if the player is interruptted by other player
              // there must be another HandleActiveEngineL callback with aActive = ETrue
            indexOfActive = KErrNotFound;
            }
        }
    else
        {// else aEngine is active player
#ifdef RD_TSP_CLIENT_MAPPER
        TProcessId lastPid( aEngine->LastActiveProcessId() ); 
        FindProcessIdForTsp( aEngine, lastPid );
        TInt err = iClientMapper->SetTspTargetClientToOtherType( 
            CTspClientMapper::EPlayingClients, lastPid );
        if ( err != KErrNone && (TUint)lastPid != KNullProcessId )
            {
            // Setting target type failed probably because client PID could not be found.
            // As a fallback set client as new playing client.
            MPX_DEBUG2("CMPXPlaybackServer::HandleActiveEngineL(): Adding to playing client %d", 
                (TUint)lastPid );
            iClientMapper->SetTspTargetClient( CTspClientMapper::EPlayingClients, lastPid );
            }
        MPX_DEBUG2("CMPXPlaybackServer::HandleActiveEngineL(): Added as playing client %d", 
            (TUint)lastPid );
#endif
        }

    if (KErrNotFound != indexOfActive)
        {
        // Move the active player engine to the top of player list
        CMPXPlaybackEngine* player(NULL);
        if (indexOfActive)
            { // not 0
            player = iPlayers[indexOfActive];
            iPlayers.Remove(indexOfActive);
            iPlayers.Insert(player, 0);
            }

        // Active engine changed, notify all clients who are instrested.
        // e.g. KPbModeActivePlayer
        for (i=1; i < iPlayers.Count(); ++i)
            {
            player = iPlayers[i];
            if (player == aEngine && aActive)
                {
                continue;
                }
            MPX_DEBUG2("CMPXPlaybackServer::HandleActiveEngineL Player: 0x%08x", player);
            CMPXClientList* cl = player->ClientList();
            for (TInt j = cl->ClientCount(); --j>=0;)
                {
                if ((KPbModeActivePlayer.iUid == cl->ClientMode(j)) && 
                        (aEngine->Category() == cl->ClientCategory(j))) 
                    {
                    MPX_DEBUG2(
                      "CMPXPlaybackServer notify %d client player changed", j);
                    player->ClientList()->SendMsgL(
                                j,
                                TMPXPlaybackMessage(
                                    TMPXPlaybackMessage::EActivePlayerChanged,
                                    1, aActive));
                    }
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// Observe client change event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackServer::HandleClientChange(
    const TProcessId& aPid,
    MMPXClientlistObserver::TChangeType aChangeType)
    {
#ifdef RD_TSP_CLIENT_MAPPER
    if (MMPXClientlistObserver::EAdd == aChangeType)
        {
        iClientMapper->SetTspTargetClient(
                CTspClientMapper::ERegisteredClients,
                aPid);
        MPX_DEBUG2("CMPXPlaybackServer::HandleClientChange(): Added as registered client %d",
                   TUint( aPid ));
        }
    else
        {
        iClientMapper->RemoveTspTargetClient(
                CTspClientMapper::EPlayingClients,
                aPid);
        MPX_DEBUG2("CMPXPlaybackServer::HandleClientChange(): Removed from EPlayingClients client %d",
                   TUint( aPid ));
        }
#else
    (void)aPid;
    (void)aChangeType;
#endif
    }

// ----------------------------------------------------------------------------
// Create a new session
// ----------------------------------------------------------------------------
//
CSession2* CMPXPlaybackServer::NewSessionL(const TVersion& aVersion,
                                           const RMessage2& /*aMessage*/) const
    {
    TVersion v(KMPXPlaybackServerMajorVersionNumber,
               KMPXPlaybackServerMinorVersionNumber,
               KMPXPlaybackServerBuildVersionNumber);
    if (!User::QueryVersionSupported(v,aVersion))
        User::Leave(KErrNotSupported);
    CMPXPlaybackSession* session = CMPXPlaybackSession::NewL();
    const_cast<CMPXPlaybackServer*>(this)->AddClient();
    return session;
    }

// ----------------------------------------------------------------------------
// Server exe entry
// ----------------------------------------------------------------------------
//
TInt E32Main()
    {
    __UHEAP_MARK;
    //
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt r=KErrNoMemory;
    if (cleanup)
        {
        TRAP(r,StartServerL());
        }

    delete cleanup;
    __UHEAP_MARKEND;
    return r;
    }

// End of file
