/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of server core class.
*
*/

#ifndef POSINDICATORSERVERCORE_H
#define POSINDICATORSERVERCORE_H

#include <e32base.h>
#include "posindicatorlocationrequestor.h"

class CPosIndicatorServerCore : public CServer2
    {
public:     
    /**
     * Destructor
     *
     */
    ~CPosIndicatorServerCore();
    
    /**
     * Instantiates a new object of 
     * CPosIndicatorServerCore
     *
     */
    static CPosIndicatorServerCore* NewLC();

    /**      
     * Starts the position indicator server to listen
     * to incoming requests
     *
     * @param[in] aName the name of the server which the 
     * client will use to connect
     */     
    void StartL(const TDesC &aName);
    
    /**
     * Returns the handle of location requestor
     */
    CPosIndicatorLocationRequestor* GetLocationRequestorHandle() const;
    
    /**
     * Handle session closure.
     */
    void HandleSessionClosure() const;

private:
    /**
     * C++ constructor.
     *         
     * @param aServerType Sharable or non-sharable sessions.
     *
     */
    CPosIndicatorServerCore( TServerType aServerType = ESharableSessions );
     
    /**
     * 2nd phase constructor for instantiating member variables
     *
     */
    void ConstructL();
            
    /**
     * From CServer2. Creates a server-side session object.
     *
     * @param aVersion The version information supplied by the client. 
     * @param aMessage Represents the details of the client request that 
     * is requesting the creation of the session.
     *
     * @return A pointer to the newly created server-side session object
     */
    CSession2* NewSessionL(const TVersion& aVersion, 
                           const RMessage2& aMessage) const;
    
    
private: // Private Data
    /**
     * Pointer to location requestor object.
     * Own
     */
    CPosIndicatorLocationRequestor* iLocationRequestor;

    /**
     * Session counter
     */
    mutable TInt iSessionCount;	
    };


#endif /* POSINDICATORSERVERCORE_H */
