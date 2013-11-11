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
* Description:  Monitor of data receiving event.
*
*/


#ifndef CSATBIPDATAAVAILABLEMONITOR_H
#define CSATBIPDATAAVAILABLEMONITOR_H

// INCLUDES
#include "CSatEventMonitorHandler.h"
#include "MSatBIPDataAvailableObserver.h"

// FORWARD DECLARATION
class MSatBIPEventNotifier;

// CLASS DECLARATION

/**
*  Monitors DataAvailable event from BIP Data channels
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatBIPDataAvailableMonitor : public CSatEventMonitorHandler,
                                    public MSatBIPDataAvailableObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatBIPDataAvailableMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatBIPDataAvailableMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

        /**
        * From MSatBIPDataAvailableObserver Notification of an event.
        * @param aChannelId ID of the channel where the data is available.
        * @param aLength Number of bytes available.
        */
        void DataAvailable( const TInt aChannelId, const TInt aLength );

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
        CSatBIPDataAvailableMonitor( MSatEventMonitorUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // Indicates is this monitor active or not
        TBool iIsActive;

    };

#endif      // CSATBIPDATAAVAILABLEMONITOR_H

// End of File
