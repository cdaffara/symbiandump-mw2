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
* Description:  Base class for event monitors
*
*/



#ifndef CSATEVENTMONITORHANDLER_H
#define CSATEVENTMONITORHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "MSatEventMonitor.h"

// FORWARD DECLARATIONS
class MSatEventMonitorUtils;


// CLASS DECLARATION

/**
*  Base class for Event monitors. Responsible for
*  starting and cancelling event monitoring and to inform
*  Event observers of an event occurrence.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatEventMonitorHandler : public CBase,
                                public MSatEventMonitor
    {
    public:  // Constructor and destructor

        /**
        * C++ default constructor.
        * @param aUtils Interface for Event monitor utils.
        */
        CSatEventMonitorHandler( MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatEventMonitorHandler();

    public: // From base class

        /**
        * From MSatEventMonitor Cancels event monitoring
        */
        IMPORT_C void CancelMonitor();

        /**
        * From MSatEventMonitor Updates monitors state.
        * This function call starts or cancels monitor
        * @param aEvents A list of events to be monitored. If this monitors event
        * is not on the list, this monitor is cancelled.
        * @return TInt An error code indicating update status
        */
        IMPORT_C TInt UpdateMonitor( TUint aEvents );

        /**
        * From MSatEventMonitor Checks is this event monitor monitoring
        * particular event.
        * @param aEvent Event to check against
        * @return ETrue if aEvent is the same event this is monitoring
        */
        IMPORT_C TBool IsMonitoringEvent( const TInt aEvent );

    protected: // New functions

        /**
        * Starts event monitoring
        * @return An error code indicating monitor start status
        */
        virtual TInt StartMonitor() = 0;

        /**
        * Cancels event monitoring
        */
        virtual void DoCancelMonitor() = 0;

        /**
        * Base class constructor
        * @param aEvent Monitor ID from the derived event monitor.
        */
        void BaseConstructL( MSatEventMonitor::TEvent aEvent );

        /**
        * Passes event information to SatEngine when monitored event occurs.
        * @param aIntArg An extra argument. Not all monitors use this.
        * @param aSecArg An extra argument. Used if needed.
        */
        void EventOccured(
            TInt aIntArg = -1,
            TInt aSecArg = -1
            );

    protected: // Data

        // Event container. Link to SatEngine
        MSatEventMonitorUtils& iEventUtils;

    private:

        // Derived monitor's event
        RSat::TEventList iEvent;

        // Indicates is derived monitor SimRemovalMonitor
        TBool iIsSimRemovalMonitor;

        // Indicaties is SimRemovalMonitor active
        TBool iSimRemovalMonitorActive;

    };

#endif      // CSATEVENTMONITORHANDLER_H

// End of File
