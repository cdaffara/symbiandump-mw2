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
* Description:  Monitor of idle screen availability.
*
*/



#ifndef CSATIDLESCREENMONITOR_H
#define CSATIDLESCREENMONITOR_H

//  INCLUDES
#include "MSatSystemStateObserver.h"
#include "CSatEventMonitorHandler.h"

// CLASS DECLARATION
class MSatSystemStateChangeNotifier;

/**
*  Monitors idle screen state and informs SAT Engine if idle screen is available
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatIdleScreenMonitor : public CSatEventMonitorHandler,
                              public MSatSystemStateObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatIdleScreenMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatIdleScreenMonitor();

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
        CSatIdleScreenMonitor( MSatEventMonitorUtils& aUtils );

        // ConstructL
        void ConstructL();

    private: // Data

        // P&S change notifier
        MSatSystemStateChangeNotifier* iPSNotifier;

    };

#endif      // CSATIDLESCREENMONITOR_H

// End of File
