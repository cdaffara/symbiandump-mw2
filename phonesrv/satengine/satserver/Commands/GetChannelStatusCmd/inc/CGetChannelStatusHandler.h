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
* Description:  GetChannelStatus command handler
*
*/



#ifndef CGETCHANNELSTATUSHANDLER_H
#define CGETCHANNELSTATUSHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "SatSTypes.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for GetChannelStatus command.
*
*  @lib GetChannelStatusCmd.lib
*  @since Series 60 3.0
*/
class CGetChannelStatusHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CGetChannelStatusHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CGetChannelStatusHandler();

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

    private:

        /**
        * Gets channel statuses and adds them into Terminalresponse
        * @param aChannelCount The number of non-closed channels
        */
        void GetChannelStatusesL( const TInt aChannelCount );

        /**
        * C++ default constructor.
        */
        CGetChannelStatusHandler();

    private:    // Data

        // GetChannelStatus command data
        RSat::TGetChannelStatusV2 iGetChannelData;

        // GetChannelStatus command package
        RSat::TGetChannelStatusV2Pckg iGetChannelDataPckg;

        // GetChannelStatus response data
        RSat::TGetChannelStatusRspV2 iGetChannelRsp;

        // GetChannelStatus response package
        RSat::TGetChannelStatusRspV2Pckg iGetChannelRspPckg;
    };

#endif      // CGETCHANNELSTATUSHANDLER_H

// End of File
