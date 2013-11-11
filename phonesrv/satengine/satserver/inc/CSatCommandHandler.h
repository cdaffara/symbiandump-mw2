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
* Description:  Base class for SAT commands
*
*/

#ifndef CSATCOMMANDHANDLER_H
#define CSATCOMMANDHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include <SecondaryDisplay/secondarydisplaysatapi.h>
#include "MSatUtils.h"
#include "MSatEventObserver.h"
#include "MSatCommand.h"

// CLASS DECLARATION

/**
*  Base class for SAT commands.
*
*  @lib SatServer
*  @since Series 60 3.0
*/
class CSatCommandHandler : public CActive, public MSatEventObserver,
                           public MSatCommand
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aImplUid Uid for derived class's plugin
        * @param aUtils Utils interface
        */
        static CSatCommandHandler* NewL(
            const TUid& aImplUid,
            MSatUtils* aUtils );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CSatCommandHandler();

    public: // New fucntions

        /**
        * Start the waiting of SAT command from USAT API.
        */
        IMPORT_C void Start();

        /**
        * Gives a panic.
        * @param aCategory A category for this panic
        * @param aReason Reason for this panic
        */
        IMPORT_C void Panic( const TDesC& aCategory, TInt aReason ) const;

        /**
        * Indicates is this command handler currently executing a command.
        * @return ETrue if executing.
        */
        IMPORT_C TBool IsCommandExecuting() const ;

    public: // Functions from base classes

        /**
        * From MSatEventObserver Notification of event.
        * @param aEvent An event that has been notified
        */
        IMPORT_C void Event( TInt aEvent );

        /**
        * From MSatCommand Response from client.
        * Default implementation will cause panic.
        */
        IMPORT_C virtual void ClientResponse();

    protected: // New functions

        /**
        * Start of the command handling. Asks from derived class whether the
        * command is allowed to execute by calling AllowCommand.
        * If command is allowed to execute then the need of ui session
        * is queried by calling NeedUiSession. If ui session is needed and
        * its not available, ui is launcehd and this class registers itself
        * to listen the event of ui launch. After the ui is launched,
        * HandleCommand is called. If the derived class does not need the ui
        * session or ui session is available, HandleCommand is called
        * immediatly.
        */
        IMPORT_C virtual void DoHandleCommand();

        /**
        * Derived class needs to make the request from USatApi.
        * @param aStatus Status of request.
        */
        virtual void IssueUSATRequest( TRequestStatus& aStatus ) = 0;

        /**
        * Command has to check the state of the system and make the
        * desision of allowing the command to perform. If desision
        * is not to allow the command, then the terminal response has to
        * sent from this method.
        * @return TBool indicating is this command allowed or not
        */
        virtual TBool CommandAllowed() = 0;

        /**
        * Queries need of ui session in order to
        * execute the command. If ui session is needed, the ui client
        * launched and this class will register it self for the notification
        * of ui request.
        * @return ETrue if ui session is needed.
        */
        virtual TBool NeedUiSession() = 0;

        /**
        * Called when USAT API notifies that command
        * data is available.
        */
        virtual void HandleCommand() = 0;

        /**
        * Called if launch of a Sat Ui application failed.
        * Every derived class, which needs a ui session in order to
        * execute command, needs to send a terminal
        * response from this method.
        */
        virtual void UiLaunchFailed() = 0;

    protected:

        /**
        * Sends terminal response and renews the usat request of the command.
        * @param aPCmd Identifies the command, which is sending
        * terminal response.
        * @param aRsp Terminal response data package.
        */
        IMPORT_C void TerminalRsp( RSat::TPCmd aPCmd, const TDesC8& aRsp );

        /**
        * Calls SatSystemState's IsPhoneInIdleState if not using Dummy TSY,
        * otherwise returns ETrue
        * @return ETrue if using Dymmy TSY, otherwise asks status
        * from SatSystemState and returns it.
        */
        IMPORT_C TBool IsPhoneInIdleState();

    protected:  // From CActive

        /**
        * From CActive base class
        */
        IMPORT_C void RunL();

    protected:

        /**
        * C++ default constructor.
        */
        IMPORT_C CSatCommandHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aPtr Pointer to Utils interface
        */
        IMPORT_C void BaseConstructL( MSatUtils* aPtr );

    protected:  // Data

        // Utils interface
        MSatUtils* iUtils;

        // Indicates that command is pending for UI to launch
        TBool iWaitingUiLaunch;

    private:    // Data

        // Requiered attribute for the ECOM framework.
        TUid iDtorIDKey;

        // Indicates is this command handler executing
        TBool iIsExecuting;

        // Timer to get UI Launch notification released before
        // starting to handle pending proactive command.
        RTimer iDelayTimer;

    };

#endif      // CSATCOMMANDHANDLER_H

// End of File
