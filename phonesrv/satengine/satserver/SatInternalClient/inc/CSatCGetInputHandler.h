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
* Description:  This is the handler for the SIM Application Toolkit Get Input
*                proactive command.
*
*/



#ifndef CSATCGETINPUTHANDLER_H
#define CSATCGETINPUTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// FORWARD DECLARATIONS

class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the Get Input command active object handler.
 *  The class receives command data from ETel, processes it and
 *  passes it on to the user. Following that, the response is encoded and
 *  sent back to ETel.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCGetInputHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCGetInputHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCGetInputHandler();

    public: // New functions

        /**
         * Starts listening for the Sat command.
         */
        void Start();

    private:

        /**
         * Converts USAT character set to SAT character set type.
         * @param aCharacterSet USAT character set
         * @return SAT character set.
         */
        TSatCharacterSet ConvertCharacterSet(
            const RSat::TGetInputRspFormat& aCharacterSet ) const;

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
        CSatCGetInputHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // New functions

        /**
         * Examine the client response.
         * @param aResponse Client response data
         * @param aInputText User input
         * @param aRequestedIconDisplayed Informs if icon is not used
         */
        void ExamineClientResponse(
            TSatUiResponse aResponse,
            const TDes& aInputText,
            TBool aRequestedIconDisplayed );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */
        RSat::TGetInputV1 iGetInputData;

        /**
         * The data package for IPC
         */
        RSat::TGetInputV1Pckg iGetInputPckg;

        /**
         * The data structure for the response data
         */
        RSat::TGetInputRspV1 iGetInputRsp;

        /**
         * The response package for IPC
         */
        RSat::TGetInputRspV1Pckg iGetInputRspPckg;

    };

#endif // CSATCGETINPUTHANDLER_H

// End of File
