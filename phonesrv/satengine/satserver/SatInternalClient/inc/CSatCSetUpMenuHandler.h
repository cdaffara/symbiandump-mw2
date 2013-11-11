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
* Description:  This is the handler for the SIM Application Toolkit
*                Set Up Menu proactive command.
*
*/



#ifndef CSATCSETUPMENUHANDLER_H
#define CSATCSETUPMENUHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the Set Up Menu command active object handler.
 *  The class processes data from ETel SAT API and passes it on to
 *  the client. The selection made by the end-user is passed back to ETel.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCSetUpMenuHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCSetUpMenuHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCSetUpMenuHandler();

    public: // New functions

        /**
         * Starts listening for the Sat command.
         */
        void Start();

        /**
         * Returns a pointer to the object that implements old the command data
         * object.
         * @return The set up menu command handler object pointer.
         */
        RSat::TSetUpMenuV2 OldSetUpMenuData() const;

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
        CSatCSetUpMenuHandler( TInt aPriority, RSatUiSession* aSat );

    private: // Data

        /**
         * Reference to the Sat API
         */        
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */
        RSat::TSetUpMenuV2 iSetUpMenuData;

        /**
         * The data structure for old the command data
         * Used for envelope menu selection
         */        
        RSat::TSetUpMenuV2 iOldSetUpMenuData;

        /**
         * The data package for IPC
         */         
        RSat::TSetUpMenuV2Pckg iSetUpMenuPckg;

        /**
         * The data structure for the response data
         */         
        RSat::TSetUpMenuRspV1 iSetUpMenuRsp;

        /**
         * The response package for IPC
         */         
        RSat::TSetUpMenuRspV1Pckg iSetUpMenuRspPckg;

    };

#endif // CSATCSETUPMENUHANDLER_H

// End of File
