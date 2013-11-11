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
* Description:  Collection server
*
*/

 
#ifndef CMPXCOLLECTIONSERVER_H
#define CMPXCOLLECTIONSERVER_H

#include <e32base.h>
#include "mpxcollectionserverdefs.h"

class MMPXCollectionCallback;
class CMPXCollectionEngine;

/**
* Music collection server
*/
NONSHARABLE_CLASS(CMPXCollectionServer) : public CPolicyServer
    {
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @return The constructed object
    */
    static CMPXCollectionServer* NewL();

    /**
    *  Destructor
    */
    ~CMPXCollectionServer();
    
    /**
    *  Decrements number of sessions this server holds, and if now zero, shuts
    *  down the server.
    *
    */
    void RemoveClient();

private: // From base class
    /** From CServer2 from which CPolicyServer derives
     *  Create a new session
     *
     *  @param aVersion version of the server
     *  @param aMessage message object
     */
     CSession2* NewSessionL(const TVersion& aVersion,
                            const RMessage2& aMessage) const;

private:

    /**
    *  C++ constructor
    *
    *  @param aPriority  priority of server
    *  @param aPolicy security policy
    */
    CMPXCollectionServer(TInt aPriority, const TPolicy &aPolicy);

    /**
    *  2nd phase constructor
    */
    void ConstructL();
private:
 
    CMPXCollectionEngine* iEngine;  // Single collection engine
    TInt iClients;
    };
    
#endif // CMPXCOLLECTIONSERVER_H

