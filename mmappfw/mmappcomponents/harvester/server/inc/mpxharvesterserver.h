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
* Description:  Harvester Server 
*
*/

 
#ifndef CMPXHARVESTERSERVER_H
#define CMPXHARVESTERSERVER_H

#include <e32base.h>

class CMPXHarvesterEngine;

/**
* Harvester Server
*/
NONSHARABLE_CLASS(CMPXHarvesterServer) : public CPolicyServer
    {
public:

    enum { EMPXHarvesterServerPriority = CActive::EPriorityStandard };
     
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @return The constructed object
    */
    static CMPXHarvesterServer* NewL();

    /**
    *  Destructor
    */
    ~CMPXHarvesterServer();
    
    /**
    * Increments number of sessions this server holds
    */
    void AddClient();
    
    /**
    *  Decrements number of sessions this server holds, and if now zero, shuts
    *  down the server.
    */
    void RemoveClient();
    
private:

    /**
    *  C++ constructor
    *
    *  @param aPriority  priority of server
    *  @param aPolicy security policy
    */
    CMPXHarvesterServer(TInt aPriority, const TPolicy &aPolicy);

    /**
    *  2nd phase constructor
    */
    void ConstructL();
    
    /**
    *  Create a new session
    *
    *  @param aVersion version of the server
    *  @param aMessage message object
    */
    CSession2* NewSessionL(const TVersion& aVersion,
                           const RMessage2& aMessage) const;
private:
 
    CMPXHarvesterEngine* iEngine;  // Engine for the server
    TInt iClients;
    };
 
#endif // CMPXHARVESTERSERVER_H

