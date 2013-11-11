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
* Description:  MO Short Message Control command handler
*
*/



#ifndef CMOSMCONTROLHANDLER_H
#define CMOSMCONTROLHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "SatSTypes.h"


// FORWARD DECLARATIONS
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for MO Short message control command
*
*  @lib MoSmControlCmd.lib
*  @since Series 60 3.0
*/
class CMoSmControlHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface
        */
        static CMoSmControlHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CMoSmControlHandler();

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
        * @param aStatus Request status to active object notification
        */
        void IssueUSATRequest( TRequestStatus& aStatus );

        /**
        * From CSatCommandHandler Precheck before executing the command.
        * @return Boolean indicating is this command allowed to execute.
        */
        TBool CommandAllowed();

        /**
        * From CSatCommandHandler Need for ui session.
        * @return Boolean indicating does this command need UI session.
        */
        TBool NeedUiSession();

        /**
        * From CSatCommandHandler Called when USAT API notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler. Indicates the failure of launching ui client.
        */
        void UiLaunchFailed();

    private:  // Default functions

        /**
        * C++ default constructor
        */
        CMoSmControlHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // MoSmControl command data.
        RSat::TMoSmControlV1        iMoSmControlData;
        // MoSmControl command package.
        RSat::TMoSmControlV1Pckg    iMoSmControlPckg;
        // MoSmControl command data for SatClient.
        TSatNotificationV1           iMoSmControlSendData;
        // MoSmControl command package for SatClient.
        TSatNotificationV1Pckg       iMoSmControlSendPckg;
        // MoSmControl command response data from SatClient
        TSatNotificationRspV1         iMoSmControlUiRespData;
        // MoSmControl command response package from SatClient
        TSatNotificationRspV1Pckg     iMoSmControlUiRespPckg;
        // Indicates is UI needed or not
        TBool iNeedUiSession;

    };

#endif      // CMOSMCONTROLHANDLER_H

// End of File
