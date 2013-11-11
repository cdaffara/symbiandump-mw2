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
* Description:  SetUpEventList command handler
*
*/



#ifndef CSETUPEVENTLISTHANDLER_H
#define CSETUPEVENTLISTHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for SetUpEventList command.
*
*  @lib SetUpEventListCmd.lib
*  @since Series 60 3.0
*/
class CSetUpEventListHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CSetUpEventListHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CSetUpEventListHandler();

    public: // Functions from base classes

        /**
        * From MSatEventObserver. Notification of Refresh actions.
        * @param aEvent Identifies the event.
        */
        void Event( TInt aEvent );

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
        * @return TBool indicating command is currently allowed.
        */
        TBool CommandAllowed();

        /**
        * From CSatCommandHandler Need for ui session.
        * @return TBool indicating need of ui session.
        */
        TBool NeedUiSession();

        /**
        * From CSatCommandHandler Called when USAT API notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler Never called since this command doesn't need UI
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CSetUpEventListHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // SetUpEventList command data.
        RSat::TSetUpEventListV1 iSetUpEventListData;

        // SetUpEventList command package.
        RSat::TSetUpEventListV1Pckg iSetUpEventListPckg;

        // Response from client
        RSat::TSetUpEventListRspV1 iSetUpEventListRsp;

        // Response package.
        RSat::TSetUpEventListRspV1Pckg iSetUpEventListRspPckg;

    };

#endif      // CSETUPEVENTLISTHANDLER_H

// End of File
