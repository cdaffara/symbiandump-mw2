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
* Description:  Monitor of SIM card removal
*
*/


#ifndef CSATSIMREMOVALMONITOR_H
#define CSATSIMREMOVALMONITOR_H

//  INCLUDES
#include "MSatSystemStateObserver.h"
#include "CSatEventMonitorHandler.h"

// FORWARD DECLARATION
class MSatSystemStateChangeNotifier;

// CLASS DECLARATION

/**
*  Monitors SIM removal state and informs SAT Engine if SIM gets removed.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatSimRemovalMonitor : public CSatEventMonitorHandler,
                              public MSatSystemStateObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatSimRemovalMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatSimRemovalMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

        /**
        * From MSatSystemStateObserver Notifies that system state changed.
        */
        void StateChanged();

    protected: // New functions

        /**
        * From CSatEventMonitorHandler Starts event monitoring
        * @return Error code indicating the status of starting the monitor
        */
        TInt StartMonitor();

    private:

        /**
        * C++ default constructor.
        * @param aUtils Interface for Event monitor utils
        */
        CSatSimRemovalMonitor( MSatEventMonitorUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // P&S change notifier
        MSatSystemStateChangeNotifier* iPSNotifier;

    };

#endif      // CSATSIMREMOVALMONITOR_H

// End of File
