/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Event monitor interface
*
*/


#ifndef MSATEVENTMONITOR_H
#define MSATEVENTMONITOR_H

//  INCLUDES
#include <e32base.h>
// CLASS DECLARATION

/**
*  Interface for event monitors.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class MSatEventMonitor
    {
    public: // Enums

        // Monitored events
        enum TEvent
            {
            EUserActivity,
            EIdleScreenAvailable,
            ELanguageSelection,
            ESimRemoved,
            EBrowserTerminated,
            EDataAvailable,
            EChannelStatus
            };

    public: // New functions

        /**
        * Cancels event monitoring
        */
        virtual void CancelMonitor() = 0;

        /**
        * Updates monitors state. This function call starts or cancels monitor
        * @param aEvents A list of events to be monitored. If this monitors event
        * is not on the list, this monitor is cancelled.
        * @return TInt An error code indicating update status
        */
        virtual TInt UpdateMonitor( TUint aEvents ) = 0;

        /**
        * Checks is this event monitor monitoring particular event
        * @param aEvent Event to check against
        * @return ETrue if aEvent is the same event this is monitoring
        */
        virtual TBool IsMonitoringEvent( const TInt aEvent ) = 0;

        /**
        * Destructor.
        */
        virtual ~MSatEventMonitor() {};

    protected:

        /**
        * C++ default constructor.
        */
        MSatEventMonitor() {};

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatEventMonitor( const MSatEventMonitor& );
 
        // Prohibit assigment operator if not deriving from CBase.
        MSatEventMonitor& operator=( const MSatEventMonitor& );

    };

#endif      // MSATEVENTMONITOR_H

// End of file
