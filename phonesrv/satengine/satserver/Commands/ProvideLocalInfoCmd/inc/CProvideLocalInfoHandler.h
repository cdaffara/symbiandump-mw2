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
* Description:  ProvideLocalInfo command handler
*
*/


#ifndef CPROVIDELOCALINFOHANDLER_H
#define CPROVIDELOCALINFOHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for ProvideLocalInfo command.
*
*  @lib ProvideLocalInfo.lib
*  @since Series 60 3.0
*/
class CProvideLocalInfoHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface.
        */
        static CProvideLocalInfoHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CProvideLocalInfoHandler();

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
        * From CSatCommandHandler Called when USAT API.
        * notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler Indicates the failure
        * of launching ui client.
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CProvideLocalInfoHandler();

    protected:    // Data

         // The data structure for the command data
        RSat::TLocalInfoV3 iLocalInfoData;

        // The data package for IPC
        RSat::TLocalInfoV3Pckg iLocalInfoPckg;

        // Response from client
        RSat::TLocalInfoRspV3 iLocalInfoRsp;

        // Response package.
        RSat::TLocalInfoRspV3Pckg iLocalInfoRspPckg;

    };

#endif      // CPROVIDELOCALINFOHANDLER_H

// End of File
