/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSatSUiClientHandler is responsible for handling activity
*                with SAT Ui. For example launch UI, observe ui thread deaths
*
*/



#ifndef CSATSUICLIENTHANDLER_H
#define CSATSUICLIENTHANDLER_H

//  INCLUDES
#include    <e32base.h>

#include    "MThreadDeathObserver.h"
#include    "MSatSUiClientHandler.h"
#include    "MSatEventObserver.h"
#include    "MThreadDeathNotifier.h"

// FORWARD DECLARATIONS
class CSatThreadDeathMonitor;
class TSatEventMediator;
class MSatSSessions;
class MSatShellController;
class MSatUtils;

// CLASS DECLARATION

/**
*  Class is responsible for some activity conserning to client.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatSUiClientHandler : public CBase,
                             public MThreadDeathNotifier,
                             public MThreadDeathObserver,
                             public MSatSUiClientHandler,
                             public MSatEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSessions Accessor for sessions
        * @param aEventMediator Event handler class
        */
        static CSatSUiClientHandler* NewL(
            MSatSSessions& aSessions,
            TSatEventMediator& aEventMediator );

        /**
        * Destructor.
        */
        virtual ~CSatSUiClientHandler();

    public: // New functions

        /**
        * Sets Utils interface
        * @param aUtils Utils interface
        */
        void SetUtils( MSatUtils* aUtils );

    public: // Functions from base classes

        /**
        * From MThreadDeathNotifier Sets thread that is observered.
        * @param aThreadId Thread Id which death should be observered
        */
        void SetObserveredThread( TThreadId& aThreadId );

        /**
        * From MThreadDeathNotifier Notifies Thread death monitor.
        */
        void NotifyThreadDeath();

        /**
        * From MThreadDeathNotifier Start monitoring thread's death
        */
        void StartObservingThread();

        /**
        * From MThreadDeathObserver.
        * Notification of observed thread death.
        */
        void ThreadDiedL();

        /**
        * From MThreadDeathObserver.
        * Notification of observed thread paniced.
        */
        void ThreadPanicedL();

        /**
        * From MSatEventObserver. Event notification.
        * @param aEvent An event that has been notified by TSatEventMediator
        */
        void Event( TInt aEvent );

        /**
        * Gives pointer to ui session interface. If pointer is NULL, then
        * ui session is not available.
        * @return Pointer to ui session interface.
        */
        MSatUiSession* UiSession();

        /**
        * Launches the SAT UI Client.
        */
        void LaunchSatUiL();

        /**
        * Tells if user has started the ui client.
        * @return ETrue if user has launched the ui client.
        */
        TBool UiLaunchedByUser();

        /**
        * Gives pointer to MSatShellController
        * @return Pointer to sat shell controller.
        */
        MSatShellController& ShellController();

        /**
        * Tells is UI closing
        * @return Boolean value indicating is UI closing
        */
        TBool IsUiClosing() const;

    private:

        /**
        * C++ default constructor.
        * @param aSessions Accessor for sessions
        * @param aEventMediator Event handler class
        */
        CSatSUiClientHandler(
            MSatSSessions& aSessions,
            TSatEventMediator& aEventMediator );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Accessor for sessions
        MSatSSessions& iSessions;

        // Thread death monitor
        CSatThreadDeathMonitor* iThreadDeathMonitor;

        // Handle to SatShellController library
        RLibrary iSatShellCntrlLibrary;

        // Interface for SatShellController.
        MSatShellController* iSatShellCntrl;

        // Event mediator
        TSatEventMediator& iEventMediator;

        // Utils interface
        MSatUtils* iUtils;

        // Is user launced the ui.
        TBool iUiLaunchedByUser;

        // Is Sat ui closing
        TBool iSatUiClosing;

        // Do we have to launch SAT Ui after it is closed
        TBool iLaunchSatUiAfterClose;

        // Thread that is closing
        TThreadId iThread;

        // Indicates if SAT UI is paniced
        TBool iSatUiPanicked;
    };

#endif      // CSATSUICLIENTHANDLER_H

// End of File
