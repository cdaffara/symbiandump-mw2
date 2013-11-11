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
* Description:  Monitors threads death and notifies an event, when thread dies
*
*/



#ifndef CSATTHREADDEATHMONITOR_H
#define CSATTHREADDEATHMONITOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MThreadDeathObserver;

// CLASS DECLARATION
/**
*  Monitors thread deaths and notifies an event, when one dies
*
*  @lib SatEngine.lib
*  @since Series 60 2.6
*/
class CSatThreadDeathMonitor : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils API for event notify
        */
        static CSatThreadDeathMonitor* NewL();

        /**
        * Destructor.
        */
        virtual ~CSatThreadDeathMonitor();

    public: // New functions

        /**
        * Notfifies the observer, when thread dies. Cancels outstanding
        * thread death notification, if one exists.
        * @param aThreadId Id of thread.
        * @param aObserver Observer of the thread death
        */
        void NotifyThreadDeathL(
            const TThreadId& aThreadId,
            MThreadDeathObserver& aObserver );

    protected: // New functions

        /**
        * Issues the request of thread death.
        */
        void StartObservingThreadDeath();

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        void RunL();

    private:

        /**
        * C++ default constructor.
        */
        CSatThreadDeathMonitor();

    private:    // Data

        // Thread that is monitored
        RThread iThread;

        // Observer for thread death.
        MThreadDeathObserver* iObserver;

    };

#endif      // CSATTHREADDEATHMONITOR_H

// End of File
