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
* Description:  Handles the SetUpMenu proactive command.
*
*/



#ifndef CSETUPMENUHANDLER_H
#define CSETUPMENUHANDLER_H

//  INCLUDES
#include    <etelsat.h>
#include    "CSatCommandHandler.h"

// CLASS DECLARATION

/**
*  Handles the SetUpMenu proactive command.
*
*  @lib CSetUpMenuCmd.dll
*  @since Series 60 3.0
*/
class CSetUpMenuHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface.
        */
        static CSetUpMenuHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CSetUpMenuHandler();

    public: // Functions from base classes

        /**
        * From MSatCommand.
        */
        void ClientResponse();

        /**
        * From CSatCommandHandler Overrides the default implementation.
        * @param aEvent An event that is occured.
        */
        void Event( TInt aEvent );

    protected: // Functions from base classes

        /**
        * From CActive Cancels the SAT request.
        */
        void DoCancel();

    private: // New functions

        /**
        * Adds icon to shell.
        * @param aAlphaId Alpha id.
        * @param aIconId Icon id.
        */
        void AddIconToShellL(
            const TDesC& aAlphaId,
            const RSat::TIconId& aIconId );

        /**
        * Removes icon from shell.
        */
        void RemoveIconFromShellL();

        /**
        * Sends setup menu data to client.
        */
        void SendSetUpMenuToClient();

        /**
        * Update setup menu data to client.
        */
        void UpdateSetUpMenuToClientL();
        
        /**
        * Sends empty setup menu data to client to indicate SIM services is NA.
        */        
        void SendEmptySetupMenuToClient();

     private:  // Functions from base classes

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
        * From CSatCommandHandler Handles the notification of SetUpMenu command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler. Ui launch failed.
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CSetUpMenuHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Set up menu command data.
        RSat::TSetUpMenuV2 iSetUpMenuData;

        // Package for setupmenu command.
        RSat::TSetUpMenuV2Pckg iSetUpMenuPckg;

        // Set up menu arrived data. Needs to be different than
        // the one used in request( iSetUpMenuData ), because
        // SetUpMenu makes the new request before ui has handled
        // the command.
        RSat::TSetUpMenuV2 iArrivedSetUpMenuData;

        // Package of arrived set up menu data.
        RSat::TSetUpMenuV2Pckg iArrivedSetUpMenuPckg;

        // Indication that command has arrived from SIM.
        TBool iSetUpMenuArrived;

        // Name should be changed when SetUp menu is launched.
        TBool iSatAppNameUpdated;
        
        // Information about SAT service availability
        TBool iSatServiceAvailable;
        
        // Flag to signal that command has icon data
        // To be removed when icons are allowed in this command
        TBool iIconCommand;

    };

#endif      // CSETUPMENUHANDLER_H

// End of File
