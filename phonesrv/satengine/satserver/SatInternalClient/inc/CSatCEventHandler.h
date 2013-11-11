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
* Description:  This is the handler for various events related to those SIM
*                Application Toolkit proactive commands that simply indicate
*                that an action occured at the server.
*
*/



#ifndef CSATCEVENTHANDLER_H
#define CSATCEVENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "SatSTypes.h"

// FORWARD DECLARATIONS

class RSatUiSession;

// CLASS DECLARATION

/**
 *  This class notifies the client about various server-side events.
 *  The events in question are start/end of call, start/end of SIM refresh,
 *  SS send and SIM session end.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCEventHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCEventHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCEventHandler();

    public: // New functions

        /**
         * Starts listening for the Sat command.
         */
        void Start();

    protected:  // Functions from base classes

        /**
         * From CActive
         *
         * Handles the request completion.
         */
        void RunL();

        /**
         * From CActive
         * 
         * Implements the cancel protocol.
         */
        void DoCancel();

    private: // Constructors

        /**
         * C++ default constructor.
         * @param aPriority The priority of this active object.
         * @param aSat A pointer to a session (does not take ownership).
         */
        CSatCEventHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        TSatEventV1 iEventData;

        /**
         * The data package for IPC
         */                
        TSatEventV1Pckg iEventPckg;
    };

#endif // CSATCEVENTHANDLER_H

// End of File
