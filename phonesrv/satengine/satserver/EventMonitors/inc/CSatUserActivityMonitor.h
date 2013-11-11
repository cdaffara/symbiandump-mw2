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
* Description:  Monitor of user activity
*
*/


#ifndef CSATUSERACTIVITYMONITOR_H
#define CSATUSERACTIVITYMONITOR_H

#include <activitymanager.h>
#include "CSatEventMonitorHandler.h"

/**
*  Monitors user activity and informs SAT Engine if there
*  is no user activity in certain time.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatUserActivityMonitor : public CSatEventMonitorHandler
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatUserActivityMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatUserActivityMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

    protected: // New functions

        /**
        * From CSatEventMonitorHandler Starts event monitoring
        * @return Error code indicating the status of starting the monitor
        */
        TInt StartMonitor();

    private:

        /**
        * Cancels monitoring of user activity.
        */
        void Cancel();

        /**
        * C++ default constructor.
        * @param aUtils Interface for Event monitor utils
        */
        CSatUserActivityMonitor( MSatEventMonitorUtils& aUtils );

        // ConstructL
        void ConstructL();

        /**
        * Starts activity manager for user actions.
        */
        void StartActivityManagerL();

        /**
        * Called when user has been
        * passive for a pre-defined period
        * @param aPtr Pointer to creator, not currently used
        * @return Completing result value
        */
        static TInt DispatchUserInactive( TAny* /* aPtr */ );

        /**
        * Called when user does something
        * @param aPtr Pointer to creator
        * @return Completing result value
        */
        static TInt DispatchUserActive( TAny* aPtr );

    private: // Data

        // Activity manager to follow user activity.
        CUserActivityManager* iActivityManager;
        // Indicates is this monitor active
        TBool iMonitoring;
    };

#endif      // CSATUSERACTIVITYMONITOR_H

// End of File
