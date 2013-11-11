/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for sub-sessions
*
*/



#ifndef CSATSSUBSESSION_H
#define CSATSSUBSESSION_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARAITION
class CSatSSession;
class CSatSServer;
// CLASS DECLARATION

/**
*  Base class for sub-sessions.
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class CSatSSubSession : public CObject
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CSatSSubSession();

    public: // New functions

        /**
        * Handles the subsession functions. The opening and closing of
        * sub-session is handled by CSatSSession, so this method is not
        * called in opening or in closing of sub-session.
        * @param aMessage IPC message from client.
        */
        virtual void HandleCommandL( const RMessage2& aMessage ) = 0;

    protected:

        /**
        * C++ default constructor.
        */
        CSatSSubSession( CSatSSession& aSession );

    protected: // Data
        // Session class
        CSatSSession& iSession;

    };

#endif      // CSATSSUBSESSION_H

// End of File
