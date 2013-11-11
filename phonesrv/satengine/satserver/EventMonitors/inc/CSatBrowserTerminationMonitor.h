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
* Description:  Monitors browser termination.
*
*/


#ifndef CSATBROWSERTERMINATIONMONITOR_H
#define CSATBROWSERTERMINATIONMONITOR_H

// INCLUDES
#include <w32std.h>
#include "msatbrowserwsobserver.h"
#include "CSatEventMonitorHandler.h"

// FORWARD DECLARATION
class CSatBrowserWSMonitor;

// CLASS DECLARATION

/**
*  Monitors browser termination.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatBrowserTerminationMonitor : public CSatEventMonitorHandler,
                                      public MSatBrowserWSObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Event monitor utils interface
        */
        static CSatBrowserTerminationMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatBrowserTerminationMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

        /**
        * This function is called when browser's thread has actually died.
        * @param aCause The cause for the browser termination.
        */
        void NotifyBrowserTerminated(
            const RSat::TBrowserTerminationCause aCause );

    protected: // From CTimer

        /**
        * From CActive.
        */
        void RunL();

        /**
        * From CActive, implements the cancel protocol.
        */
        void DoCancel();

    protected: // From CSatEventMonitorHandler

        /**
        * From CSatEventMonitorHandler Starts event monitoring
        * @return Error code indicating the status of starting the monitor
        */
        TInt StartMonitor();

    private:

        /**
        * C++ default constructor.
        * @param aUtils Event monitor utils interface
        */
        CSatBrowserTerminationMonitor( MSatEventMonitorUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // Window session termination monitor.
        CSatBrowserWSMonitor* iWSMonitor;
    };

#endif      // CSATBROWSERTERMINATIONMONITOR_H

// End of File
