/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SelectItem command handler
*
*/



#ifndef CSELECTITEMHANDLER_H
#define CSELECTITEMHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for SelectItem command.
*
*  @lib SelectItemCmd.lib
*  @since Series 60 3.0
*/
class CSelectItemHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface.
        */
        static CSelectItemHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CSelectItemHandler();

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
        CSelectItemHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    protected:    // Data

        // SetUpCall command data.
        RSat::TSelectItemV2 iSelectItemData;

        // SetUpCall command package.
        RSat::TSelectItemV2Pckg iSelectItemPckg;

        // Response from client
        RSat::TSelectItemRspV1 iSelectItemRsp;

        // Response package.
        RSat::TSelectItemRspV1Pckg iSelectItemRspPckg;
        
        // Flag to signal that command has icon data
        // To be removed when icons are allowed in this command
        TBool iIconCommand;

    };

#endif      // CSELECTITEMHANDLER_H

// End of File
