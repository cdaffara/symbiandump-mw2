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
* Description:  Refresh command handler
*
*/



#ifndef CREFRESHHANDLER_H
#define CREFRESHHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "SatSTypes.h"
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for Refresh command.
*
*  @lib RefreshCmd.lib
*  @since Series 60 3.0
*/
class CRefreshHandler : public CSatCommandHandler
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CRefreshHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CRefreshHandler();

    public:     // Functions from base classes

        /**
        * From MSatEventObserver. Overrides the default implementation.
        * @param aEvent An event that is occured.
        */
        void Event( TInt aEvent );

    protected:  // Functions from base classes

        /**
        * From CActive Cancels the usat request.
        */
        void DoCancel();

        /**
        * From CCommandHandler Requests the command notification.
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
        * From CSatCommandHandler. Indicates the failure of launching ui client.
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CRefreshHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // New functions

        /**
        * Notifies the refresh clients about refresh cancel.
        */
        void NotifyRefreshClientsCancel();

        /**
        * Completes refresh when all clients has completed.
        */
        void CompleteRefresh();

    private:    // Data

        // Refresh command data.
        RSat::TRefreshV2 iRefreshData;

        // Refresh command package.
        RSat::TRefreshV2Pckg iRefreshPckg;

        // Response from client
        RSat::TRefreshRspV1 iRefreshRsp;

        // Response package.
        RSat::TRefreshRspV1Pckg iRefreshRspPckg;

        // Count of refresh clients notified refresh but not yet answered.
        TInt iRefreshNotifyClientCount;

        // True whether any client has indicated additional EF read.
        TBool iAdditionalEFRead;

        // Indicates SimReset type refresh is executing.
        TBool iSimResetExecuting;
    };

#endif      // CREFRESHHANDLER_H

// End of File
