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
*                Select Item proactive command.
*
*/



#ifndef CSATCSELECTITEMHANDLER_H
#define CSATCSELECTITEMHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// FORWARD DECLARATIONS
class RSatUiSession;

// CLASS DECLARATION

/**
 *  This is the Select Item command active object handler.
 *  The class processes data from ETel SAT API and passes it on to
 *  the client. The selection made by the end-user is passed back to ETel.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCSelectItemHandler : public CActive
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCSelectItemHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCSelectItemHandler();

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
        CSatCSelectItemHandler(
            TInt aPriority,
            RSatUiSession* aSat );

    private: // New functions

        /**
         * Examine the client response.
         * @param aResponse Client response data
         * @param aSelection User selection
         * @param aRequestedIconDisplayed Informs if icon is not used
         * @param aDefaultItemIdFound informs if default item is found
         * from item list
         */
        void ExamineClientResponse(
            TSatUiResponse aResponse,
            TUint8 aSelection,
            TBool aRequestedIconDisplayed,
            TBool aDefaultItemIdFound );

    private: // Data

        /**
         * Reference to the Sat API
         */        
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        RSat::TSelectItemV2 iSelectItemData;

        /**
         * The data package for IPC
         */        
        RSat::TSelectItemV2Pckg iSelectItemPckg;

        /**
         * The data structure for the response data
         */        
        RSat::TSelectItemRspV1 iSelectItemRsp;
 
        /**
         * The response package for IPC
         */        
        RSat::TSelectItemRspV1Pckg iSelectItemRspPckg;
    };

#endif // CSATCSELECTITEMHANDLER_H

// End of File
