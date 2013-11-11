/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the active scheduler of the SAT Server.
*
*/


#ifndef CSATSSCHEDULER_H
#define CSATSSCHEDULER_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSatSServer;

//  CLASS DECLARATION

/**
*  SAT Server includes a scheduler of it's own to simplify
*  client request handling code. All active object leaves are
*  handled in function Error(). The behaviour of Error()
*  depends on whether there is an outstanding client request
*  or not: it will tell the client the reason for inability
*  to handle the client request. It will also panic the
*  client if the descriptor passed to the server was malformed.
*  Finally, a panic is raised if the error does not originate
*  from the server itself.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/

class CSatSScheduler : public CActiveScheduler
    {
    public: // Constructors

        /**
        *   Default constructor
        */
        IMPORT_C CSatSScheduler();

        /**
        *   Gives a pointer of the current CSatSServer instance
        *   to the scheduler.
        *   @param aServer The current server pointer
        */
        IMPORT_C void SetServer( CSatSServer* aServer );

    public:     // from CActiveScheduler

        /**
        *   Overridden to handle leaves in request functions and
        *   unexpected situations.
        *   @param aError Error code, see EPOC system documentation.
        */
        void Error( TInt aError ) const;

    private:    // prohibited operators and functions

        // Copy constructor
        CSatSScheduler( const CSatSScheduler& );

        // Assignment operator
        CSatSScheduler& operator=( const CSatSScheduler& );

    private:    // data

        // Ref: server instance
        CSatSServer* iServer;
    };

#endif          // CSATSSCHEDULER_H

// End of File
