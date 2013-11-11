/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the client command processor class that is
*                responsible for the active objects needed to maintain that
*                connection.
*
*/



#ifndef CSATCCOMMANDPROCESSOR_H
#define CSATCCOMMANDPROCESSOR_H

//  INCLUDES
#include <e32base.h>

#include "CSatCDisplayTextHandler.h"
#include "CSatCGetInkeyHandler.h"
#include "CSatCGetInputHandler.h"
#include "CSatCSetUpMenuHandler.h"
#include "CSatCSelectItemHandler.h"
#include "CSatCPlayToneHandler.h"
#include "CSatCActiveCommandHandler.h"
#include "CSatCEventHandler.h"
#include "CSatCQueryHandler.h"
#include "CSatCNotifyHandler.h"

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This class is the container for all command handlers.
 *  The class, owned by the session objects, initializes and starts all
 *  of the active objects which are part of the Sat Client API.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCCommandProcessor : public CBase
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSession A pointer to a session (does not take ownership).
         */
        static CSatCCommandProcessor* NewL( RSatUiSession* aSession );

        /**
         * Destructor.
         */
        virtual ~CSatCCommandProcessor();

    public:  // New functions

        /**
         * Starts the active objects.
         */
        void Start();

        /**
         * Returns a pointer to the object that implements set up menu command
         * handler object.
         * @return The set up menu command handler object pointer.
         */
        CSatCSetUpMenuHandler* SetUpMenuHandler();

        /**
         * Returns a pointer to a active command handler object.
         * @return The active command handler object pointer.
         */
        CSatCActiveCommandHandler* ActiveCommandHandler();

    private: // Constructors

        /**
         * C++ default constructor.
         * @param aSession A pointer to a session (does not take ownership).
         */
        CSatCCommandProcessor( RSatUiSession* aSession );

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL();

    private: // Data

        /**
         * The Sat Client Session.
         */        
        RSatUiSession* iSession;

        /**
         * The Display Text command handler.
         */          
        CSatCDisplayTextHandler* iDisplayTextHandler;

        /**
         * The Get Inkey command handler.
         */          
        CSatCGetInkeyHandler* iGetInkeyHandler;

        /**
         * The Get Input command handler.
         */          
        CSatCGetInputHandler* iGetInputHandler;
 
        /**
         * The Set Up Menu command handler.
         */          
        CSatCSetUpMenuHandler* iSetUpMenuHandler;

        /**
         * The Select Item command handler.
         */          
        CSatCSelectItemHandler* iSelectItemHandler;

        /**
         * The Play Tone command handler.
         */          
        CSatCPlayToneHandler* iPlayToneHandler;

        /**
         * Handler for various server-side events.
         */          
        CSatCEventHandler* iEventHandler;

        /**
         * Handler for various end-user queries.
         */          
        CSatCQueryHandler* iQueryHandler;

        /**
         * Notification handler
         */          
        CSatCNotifyHandler* iNotifyHandler;

        /**
         * The handler for all user-initiated commands.
         */          
        CSatCActiveCommandHandler* iActiveCommandHandler;

    };

#endif // CSATCCOMMANDPROCESSOR_H

// End of File
