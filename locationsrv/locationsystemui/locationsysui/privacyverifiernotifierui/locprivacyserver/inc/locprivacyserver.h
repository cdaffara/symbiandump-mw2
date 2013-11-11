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
* Description:  server class definition.
*
*/

#ifndef CLOCPRIVACYSERVER_H
#define CLOCPRIVACYSERVER_H

//  INCLUDES
#include <e32base.h>
#include <locprivacyinternal.h>

// FORWARD DECLARATIONS

class CLocPrivacyServerSession;

// CLASS DECLARATION

/**
 *  The main server side class.
 */
class CLocPrivacyServer : public CServer2
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CLocPrivacyServer* NewL();

    /**
     * Destructor.
     */
    ~CLocPrivacyServer();

private:

    /**
     * C++ constructor.
     *
     * @param aPriority Priority at which to run the server active object.
     */
    CLocPrivacyServer(
    /* IN  */TInt aPriority);

    /**
     * EPOC default constructor.
     */
    void ConstructL();

public:
    // New functions

    // Server priority. 950 is standard value.
    enum
        {
        EPriority = 950
        };

    /**
     * Increment number of sessions.
     */
    void IncrementSessions();

    /**
     * Decrement number of sessions.
     *
     * @param aSession Pointer to session that is calling
     *                 DecrementSessions().It will be used in future.
     */
    void DecrementSessions(
    /* IN  */CLocPrivacyServerSession* /*aSession */);

public:
    // Functions from base classes

    /**
     * From CServer.
     *
     * @param aVersion the server version.
     * @param aMessage The connect message from the client.
     * @return a new session
     */
    CSession2* NewSessionL(
    /* IN  */const TVersion& aVersion,
    /* IN  */const RMessage2& aMessage) const;

    /**
     * From CActive.
     *
     * @param aError the error code of the server leave.
     * @return always KErrNone.
     */
    TInt RunError(
    /* IN  */TInt aError);

private:

    // By default, prohibit copy constructor
    CLocPrivacyServer(const CLocPrivacyServer&);
    // Prohibit assigment operator
    CLocPrivacyServer& operator=(const CLocPrivacyServer&);

private:
    // Data
    TInt iNumSessions;
    };

#endif      // CLOCPRIVACYSERVER_H
// End of File

