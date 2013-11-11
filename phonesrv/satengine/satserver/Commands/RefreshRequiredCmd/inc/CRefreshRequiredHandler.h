/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RefreshRequired command handler
*
*/



#ifndef CREFRESHREQUIREDHANDLER_H
#define CREFRESHREQUIREDHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "SatSTypes.h"
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for RefreshRequired command.
*
*  @lib RefreshRequiredCmd.lib
*  @since Series 60 3.0
*/
class CRefreshRequiredHandler : public CSatCommandHandler
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CRefreshRequiredHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CRefreshRequiredHandler();

    public:     // Functions from base classes

        /**
        * From MSatEventObserver. Overrides the default implementation.
        * @param aEvent An event that is occured.
        */
        void Event( TInt aEvent );

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
        CRefreshRequiredHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // New functions

        /**
        * Checks whether EFimsi file change conditions prevent refresh.
        * @return TBool indicating EFimsi file refresh allowed.
        */
        TBool EFIMSIRefreshAllowed() const;

        /**
        * Checks whether phone state prevents refresh.
        * @return TBool indicating refresh allowed.
        */
        TBool BusyNotAllowed();

        /**
        * Asks permission to make refresh from clients.
        * @return TBool indicating refresh can be started immediately.
        */
        TBool QueryRefreshClients();

        /**
        * Refresh observers call this to indicate refresh allow.
        * Starts the refresh when all clients has agreed.
        * @param aAllow Indicates allowness of this client.
        */
        void AllowRefresh( TBool aAllow );

        /**
        * Start refresh.
        */
        void DoRefresh();

        /**
        * Notifies the refresh clients about refresh cancel.
        */
        void NotifyRefreshClientsCancel();

        /**
        * Sends the RefreshRequired specific response to the SIM and renews
        * the sat request. Note, this replaces TerminalRsp() here.
        * @param aRsp RefreshRequired response data package.
        */
        void RefreshAllowed( const TDesC8& aRsp );
        
    private:    // Data

        // Refresh command data.
        RSat::TRefreshV2 iRefreshRequiredData;

        // Refresh command package.
        RSat::TRefreshV2Pckg iRefreshRequiredPckg;

        // Response from client.
        RSat::TRefreshRspV1 iRefreshRequiredRsp;

        // Response package.
        RSat::TRefreshRspV1Pckg iRefreshRequiredRspPckg;

        // Query command data.
        TSatQueryV1 iQueryData;

        // Query package.
        TSatQueryV1Pckg iQueryPckg;

        // Query response.
        TSatQueryRspV1 iQueryRsp;

        // Query response package.
        TSatQueryRspV1Pckg iQueryRspPckg;

        // Count of refresh clients queried refresh allow but not yet answered.
        TInt iRefreshQueryClientCount;

        // Response of refresh query.
        TBool iAllowRefresh;
        
    };

#endif      // CREFRESHREQUIREDHANDLER_H

// End of File
