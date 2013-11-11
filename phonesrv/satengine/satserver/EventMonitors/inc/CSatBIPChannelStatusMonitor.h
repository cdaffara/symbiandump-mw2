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
* Description:  Monitor of channel status events.
*
*/


#ifndef CSATBIPCHANNELSTATUSMONITOR_H
#define CSATBIPCHANNELSTATUSMONITOR_H

// INCLUDES
#include "CSatEventMonitorHandler.h"
#include "MSatBIPChannelStatusObserver.h"

// FORWARD DECLARATION
class MSatBIPEventNotifier;

// CLASS DECLARATION

/**
*  Monitors ChannelStatus event from BIP Data channels
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatBIPChannelStatusMonitor : public CSatEventMonitorHandler,
                                    public MSatBIPChannelStatusObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatBIPChannelStatusMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatBIPChannelStatusMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

        /**
        * From MSatBIPChannelStatusObserver Notification of an event.
        * @param aChannelId ID of the channel that started this event.
        * @param aStatus Status that caused this event.
        */
        void ChannelStatus( const TInt aChannelId, const TInt aStatus );

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
        CSatBIPChannelStatusMonitor( MSatEventMonitorUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // Indicates is this monitor active or not
        TBool iIsActive;

    };

#endif      // CSATBIPCHANNELSTATUSMONITOR_H

// End of File
