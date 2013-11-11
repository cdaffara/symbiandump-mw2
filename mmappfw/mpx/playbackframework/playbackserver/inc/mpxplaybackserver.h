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


#ifndef CMPXPLAYBACKSERVER_H
#define CMPXPLAYBACKSERVER_H

#include <e32base.h>
#include "mpxclientlistobserver.h"
#include "mpxplaybackserverdefs.h"
#include "mpxplaybackactiveengineobserver.h"

class CMPXPlaybackEngine;
class CMPXMessageQueue;
class CTspClientMapper;
class CMPXPluginMonitor;

/**
* Music playback server
*/
NONSHARABLE_CLASS(CMPXPlaybackServer) : public CPolicyServer,
                                        public MMPXPlaybackActiveEngineObserver,
                                        public MMPXClientlistObserver
    {
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @return The constructed object
    */
    static CMPXPlaybackServer* NewL();

    /**
    *  Destructor
    */
    ~CMPXPlaybackServer();

    /**
    *  Create a player for a specific client, or returns an existing player
    *  depending on the mode and whether it already exists.
    *
    *  @param aModeId playback mode id
    *  @param aClientId thread id of the client
    *  @aMsgQueue message queue for the client
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    */
    CMPXPlaybackEngine* CreatePlayerL(const TUid& aModeId,
                                      const TInt aCategory, 
                                      TThreadId aClientId,
                                      CMPXMessageQueue* aMsgQueue);

    /**
    * Increments number of sessions this server holds
    */
    void AddClient();

    /**
    *  Decrements number of sessions this server holds, and if now zero, shuts
    *  down server. ALSO removes client from player, and if the player has no
    *  more clients, it gets deleted.
    *
    *  @param aMsgQueue message queue for the client
    */
    void RemoveClient(const CMPXMessageQueue& aMsgQueue);

    /**
     *  Retursn last active process id
     *
     *  @return process id of last active process
     */
    TProcessId LastActiveProcessId();

private:

    /**
    *  Add a new player
    *
    *  @param aClientId thread id of the client
    *  @param aModeId playback mode id
    *  @aMsgQueue message queue for the client
	*  @param aCategory category of engine
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    *  @return pointer to engine
    */
    CMPXPlaybackEngine* AddNewPlayerL(TThreadId aClientId,
                                      const TUid& aModeId,
                                      CMPXMessageQueue* aMsgQueue,
                                      const TInt aCategory); 

    /**
    *  Attach to a default player within the same thread
    *
    *  @param aClientId thread id of the client which attachs to existing player
    *  @aMsgQueue message queue for the client
	*  @param aCategory category of engine
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    *  @return pointer to engine or NULL if not found
    */
    CMPXPlaybackEngine* AddToDefaultPlayerL(TThreadId aClientId,
                                            CMPXMessageQueue* aMsgQueue,
                                            const TInt aCategory); 

    /**
     *  Returns index of recent active player
     *
     *  @leave The function leaves with one of the system wide error codes,
     *         if the operation fails.
     *  @return index to recent active player, KErrNotFound if not found
     */
     TInt IndexOfRecentActivePlayer();

	 /**
     *  Returns index of recent active player
     *	@param aCategory category of engine
     *  @leave The function leaves with one of the system wide error codes,
     *         if the operation fails.
     *  @return index to recent active player, KErrNotFound if not found
     */
     TInt IndexOfRecentActivePlayer(TInt aCategory);

private:

    /**
    *  C++ constructor
    *
    *  @param aPriority  priority of server
    *  @param aPolicy security policy
    */
    CMPXPlaybackServer(TInt aPriority, const TPolicy &aPolicy);

    /**
    *  2nd phase constructor
    */
    void ConstructL();

    /**
    *  Process id finder.
    *
    *  @param aEngine pointer to active engine
    *  @param aProcessId process id of owning process
    */
    void FindProcessIdForTsp(const CMPXPlaybackEngine* aEngine,
                             TProcessId& aProcessId );

private: // from base class
    /**
    *   From MMPXPlaybackActiveEngineObserver
    *   Handle active engine
    *
    *   @param aEngine pointer to active engine
    *   @param aActive ETrue activated, EFalse deactivated
    */
    void HandleActiveEngineL(const CMPXPlaybackEngine* aEngine, TBool aActive);

    /**
    *  From CPolicyServer/CServer2
    *  Create a new session
    *
    *  @param aVersion version of the server
    *  @param aMessage message object
    */
    CSession2* NewSessionL(const TVersion& aVersion,
                           const RMessage2& aMessage) const;

    /**
     *  From MMPXClientlistObserver
     *  Observer client change
     */
    void HandleClientChange(const TProcessId& aPid,
                            MMPXClientlistObserver::TChangeType aChangeType);

private:
    // The players are sorted by the time activated. In other word, the last
    // active player is always the first item.
    RPointerArray<CMPXPlaybackEngine> iPlayers;
    TInt iClients;
    CMPXPluginMonitor* iPluginMonitor;
#ifdef RD_TSP_CLIENT_MAPPER
    CTspClientMapper* iClientMapper;
#endif
    };

#endif // CMPXPLAYBACKSERVER_H

