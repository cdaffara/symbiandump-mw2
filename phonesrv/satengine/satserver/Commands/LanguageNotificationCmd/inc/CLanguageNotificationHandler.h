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
* Description:  Launguage Notification command handler
*
*/


#ifndef CLANGUAGENOTIFICATIONHANDLER_H
#define CLANGUAGENOTIFICATIONHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "SatSTypes.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for Language Notification command.
*
*  @lib CLanguageNotificationHandler.lib
*  @since Series 60 3.0
*/
class CLanguageNotificationHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils interface
        */
        static CLanguageNotificationHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CLanguageNotificationHandler();

    public: // Functions from base classes

        /**
        * From MSatCommand.
        */
        void ClientResponse(){};

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
        * From CSatCommandHandler. Indicates the failure of launching ui client
        */
        void UiLaunchFailed(){};

    private:

        /**
        * C++ default constructor.
        */
        CLanguageNotificationHandler();

    private:    // Data

        // The data structure for the command data
        RSat::TLanguageNotificationV2 iLanguageNotify;

        // The data package for IPC
        RSat::TLanguageNotificationV2Pckg iLanguageNotifyPckg;

        // The response.
        RSat::TLanguageNotificationRspV2 iLanguageNotifyRsp;

        // The response package.
        RSat::TLanguageNotificationRspV2Pckg iLanguageNotifyRspPckg;

    };

#endif      // CLANGUAGENOTIFICATIONHANDLER_H

// End of File
