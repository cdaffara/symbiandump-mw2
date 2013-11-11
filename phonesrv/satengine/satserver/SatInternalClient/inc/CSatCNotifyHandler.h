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
* Description:  This is the handler for various notifications related to
*                those SIM Application Toolkit proactive commands that
*                uses notifications to inform user about their actions.
*
*/



#ifndef CSATCNOTIFYHANDLER_H
#define CSATCNOTIFYHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "SatSTypes.h"

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the active object handler for various end-user notifications.
 *
 *  @lib SatClient.lib
 *  @since Series 60 3.1
 */
class CSatCNotifyHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCNotifyHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCNotifyHandler();

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
        CSatCNotifyHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private:    // Data

        /**
         * Reference to the Sat API
         */        
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        TSatNotificationV1 iNotifyData;

        /**
         * The data package for IPC
         */        
        TSatNotificationV1Pckg iNotifyPckg;

        /**
         * The response data structure
         */        
        TSatNotificationRspV1 iNotifyRsp;

        /**
         * The response package for IPC
         */        
        TSatNotificationRspV1Pckg iNotifyRspPckg;

    };

#endif // CSATCNOTIFYHANDLER_H

// End of File
