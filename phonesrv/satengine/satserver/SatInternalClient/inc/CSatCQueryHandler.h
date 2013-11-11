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
* Description:  This is the handler for various query requests related to
*                those SIM Application Toolkit proactive commands that
*                require user permissions to complete their actions.
*
*/



#ifndef CSATCQUERYHANDLER_H
#define CSATCQUERYHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "SatSTypes.h"

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the active object handler for various end-user queries.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCQueryHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCQueryHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCQueryHandler();

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
        CSatCQueryHandler(
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
        TSatQueryV1 iQueryData;

        /**
         * The data package for IPC
         */        
        TSatQueryV1Pckg iQueryPckg;

        /**
         * The response data structure
         */        
        TSatQueryRspV1 iQueryRsp;

        /**
         * The response package for IPC
         */        
        TSatQueryRspV1Pckg iQueryRspPckg;

    };

#endif // CSATCQUERYHANDLER_H

// End of File
