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
* Description:  Monitors browser's thread death.
*
*/


#ifndef CSATBROWSERTHREADMONITOR_H
#define CSATBROWSERTHREADMONITOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MSatBrowserThreadObserver;

// CLASS DECLARATION

/**
*  Monitors the terminating of the browser and informs the observer when
*  the browser is terminated.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/

class CSatBrowserThreadMonitor : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aThread The thread of the browser.
        */
        static CSatBrowserThreadMonitor* NewL(
            const RThread& aThread );

        /**
        * Destructor.
        */
        virtual ~CSatBrowserThreadMonitor();

    public: // New functions

        /**
        * Starts Browser termination monitor.
        * @param aObserver Observer for browser termination.
        */
        void StartMonitor(
            MSatBrowserThreadObserver* aObserver );

        /**
        * Cancels event monitoring
        */
        void DoCancelMonitor();

    protected: // Functions from base classes

        /**
        * From CActive, handles the request completion.
        */
        void RunL();

        /**
        * From CActive, implements the cancel protocol.
        */
        void DoCancel();

    private: // Constructors

        /**
        * C++ default constructor.
        * @param aMonitor Active object for the start of the browser.
        * @param aThread The thread of the browser.
        */
        CSatBrowserThreadMonitor(
            const RThread& aThread );

        /**
        * Fetch application exit cause.
        * @return Exit cause.
        */
        RSat::TBrowserTerminationCause ExitCause() const;

    private:

        /**
        * Starts browser termination monitor.
        */
        void Start();

    private: // Data

        // Active object for starting browser.
        MSatBrowserThreadObserver* iObserver;

        //  The thread of the browser.
        RThread iThread;
    };

#endif      // CSATBROWSERTHREADMONITOR_H

// End of File