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
*                Play Tone proactive command.
*
*/


#ifndef CSATCPLAYTONEHANDLER_H
#define CSATCPLAYTONEHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// CONSTANTS

// Number of microseconds in a minute.
const TInt64 KSatCMicroSecondsInMinute = 60*1000000;

// Number of microseconds in a seconds.
const TInt64 KSatCMicroSecondsInSecond = 1000000;

// Number of microseconds in one tenth of a seconds.
const TInt64 KSatCMicroSecondsInOneTenthSecond = 100000;

// FORWARD DECLARATIONS

class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the Play Tone command active object handler.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCPlayToneHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCPlayToneHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCPlayToneHandler();

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

        CSatCPlayToneHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // New functions

        /**
         * Examine the client response.
         * @param aResponse Client response data
         * @param aRequestedIconDisplayed Informs if icon is not used
         */
        void ExamineClientResponse(
            TSatUiResponse aResponse,
            TBool aRequestedIconDisplayed );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        RSat::TPlayToneV2 iPlayToneData;

        /**
         * The data package for IPC
         */        
        RSat::TPlayToneV2Pckg iPlayTonePckg;

        /**
         * The data structure for the response data
         */        
        RSat::TPlayToneRspV1 iPlayToneRsp;

        /**
         * The response package for IPC
         */        
        RSat::TPlayToneRspV1Pckg iPlayToneRspPckg;

    };

#endif // CSATCPLAYTONEHANDLER_H

// End of File
