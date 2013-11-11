/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  DisplayText command handler
*
*/



#ifndef CDISPLAYTEXTHANDLER_H
#define CDISPLAYTEXTHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "SatSTypes.h"

// FORWARD DECLARATION
class MSatUtils;
class CClearScreenHandler;

// CLASS DECLARATION

/**
*  Command handler for DisplayText command.
*
*  @lib DisplayTextCmd.lib
*  @since Series 60 3.0
*/
class CDisplayTextHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CDisplayTextHandler* NewL( MSatUtils* aUtils );


        /**
        * Destructor.
        */
        virtual ~CDisplayTextHandler();

    public: // Functions from base classes

        /**
        * From MSatCommand.
        */
        void ClientResponse();

    protected:  // Functions from base classes

        /**
        * From CActive Cancels the usat request.
        */
        void DoCancel();

        /**
        * From CSatCommandHandler Requests the command notification.
        * @param aStatus Request status
        */
        void IssueUSATRequest( TRequestStatus& aStatus );

        /**
        * From CSatCommandHandler Precheck before executing the command.
        */
        TBool CommandAllowed();

        /**
        * From CSatCommandHandler Need for ui session.
        */
        TBool NeedUiSession();

        /**
        * From CSatCommandHandler Called when USAT API notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler. Indicates the failure of launching ui client
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CDisplayTextHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Return given duration in seconds.
        * @param aDuration Duration data
        * @return Return given duration in seconds.
        */
        TTimeIntervalSeconds DurationInSeconds(
            const RSat::TDuration& aDuration ) const;

    private:    // Data

        // DisplayText command data.
        RSat::TDisplayTextV2 iDisplayTextData;

        // DisplayText command package.
        RSat::TDisplayTextV2Pckg iDisplayTextPckg;

        // Response from client.
        RSat::TDisplayTextRspV1 iDisplayTextRsp;

        // Response package.
        RSat::TDisplayTextRspV1Pckg iDisplayTextRspPckg;

        // DisplayText IPC data.
        TSatDisplayTextV1 iDisplayTextSendData;

        // DisplayText IPC package.
        TSatDisplayTextV1Pckg iDisplayTextSendPckg;

        // ClearScreen handler.
        CClearScreenHandler* iClearScreenHandler;

        // Own information of Immediate response
        TBool iImmediateResponse;

        // Next immediate pending
        TBool iImmediatePending;

    };

#endif      // CDISPLAYTEXTHANDLER_H

// End of File
