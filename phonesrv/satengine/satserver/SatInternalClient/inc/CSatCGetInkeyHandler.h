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
* Description:  This is the handler for the SIM Application Toolkit Get Inkey
*                proactive command.
*
*/



#ifndef CSATCGETINKEYHANDLER_H
#define CSATCGETINKEYHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "etelsat.h"

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the Get Inkey command active object handler.
 *  The class receives command data from ETel, processes it and
 *  passes it on to the user. Following that, the response is encoded and
 *  sent back to ETel.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCGetInkeyHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCGetInkeyHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCGetInkeyHandler();

    public: // New functions

        /**
         * Starts listening for the Sat command.
         */
        void Start();

    protected: // Functions from base classes

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
        CSatCGetInkeyHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // New functions

        /**
         * Examine the client response.
         * @param aResponse Client response data
         * @param aCharacter User input
         * @param aRequestedIconDisplayed Informs if icon is not used
         */
        void ExamineClientResponse(
            TSatUiResponse aResponse,
            const TChar& aCharacter,
            TBool aRequestedIconDisplayed );

        /**
         * Return given duration in seconds.
         * @return Return given duration in seconds.
         */
        TUint DurationInTenthOfSeconds() const;

        /**
         * Convert seconds to duration.
         * @param aDurationInSeconds Duration in seconds
         */
        void TenthOfSecondsToDuration( TUint aDurationInSeconds );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        RSat::TGetInkeyV2 iGetInkeyData;

        /**
         * The data package for IPC
         */        
        RSat::TGetInkeyV2Pckg iGetInkeyPckg;

        /**
         * The response data structure
         */        
        RSat::TGetInkeyRspV2 iGetInkeyRsp;

        /**
         * The response package for IPC
         */        
        RSat::TGetInkeyRspV2Pckg iGetInkeyRspPckg;

    };

#endif // CSATCGETINKEYHANDLER_H

// End of File
