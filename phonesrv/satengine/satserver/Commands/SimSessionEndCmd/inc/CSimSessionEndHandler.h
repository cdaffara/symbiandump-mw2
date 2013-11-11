/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SimSessionEnd command handler
*
*/



#ifndef CSIMSESSIONENDHANDLER_H
#define CSIMSESSIONENDHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "MSatEventObserver.h"

// CLASS DECLARATION

/**
*  Command handler for SimSessionEnd command.
*
*  @lib SimSessionEndCmd.lib
*  @since Series 60 3.0
*/
class CSimSessionEndHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CSimSessionEndHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CSimSessionEndHandler();

    private: // New function

        /**
        * Timer call back for sim session end generation
        */
        static TInt TimerCallback( TAny* aSimSessionEndHandler );

    public: // Functions from base classes

        /**
        * From CSatCommandHandler Overrides the default implementation.
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
        * From CSatCommandHandler. Ui launch failed.
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CSimSessionEndHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // Indication of user terminated the SIM session.
        TBool iSimSessionTerminatedByUser;

        // Timer for launcing simsession end.
        CPeriodic* iTimer;

        // Sustained display text on screen
        TBool iSustainedTextActive;

        // Sim session end pending because sustained text
        TBool iSimSessionEndPending;

    };

#endif      // CSIMSESSIONENDHANDLER_H

// End of File
