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
*                Display Text proactive command.
*
*/



#ifndef CSATCDISPLAYTEXTHANDLER_H
#define CSATCDISPLAYTEXTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

#include "SatSTypes.h"

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  The active object handler for the Display Text command.
 *  Processes the received data and passes it on to the session client.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCDisplayTextHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCDisplayTextHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCDisplayTextHandler();

    public: // New functions

        /**
        * Starts listening for the Sat command.
        */
        void Start();

    protected:  // Functions from base classes

        /**
         * From CActive
         * Handles the request completion.
         */
        void RunL();

        /**
         * From CActive
         * Implements the cancel protocol.
         */
        void DoCancel();

    private: // Constructors

        /**
         * C++ default constructor.
         * @param aPriority The priority of this active object.
         * @param aSat A pointer to a session (does not take ownership).
         */
        CSatCDisplayTextHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // New functions

        /**
         * Examine the client response.
         * @param sResponse response data
         * @param aRequestedIconDisplayed Informs if icon is not used
         */
        void ExamineClientResponse(
            TSatUiResponse sResponse,
            TBool aRequestedIconDisplayed );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        TSatDisplayTextV1 iDisplayTextData;

        /**
         * The data package for IPC
         */          
        TSatDisplayTextV1Pckg iDisplayTextPckg;

        /**
         * The response data structure
         */          
        RSat::TDisplayTextRspV1 iDisplayTextRsp;

        /**
         * The response package for IPC
         */          
        RSat::TDisplayTextRspV1Pckg iDisplayTextRspPckg;

    };

#endif // CSATCDISPLAYTEXTHANDLER_H

// End of File
