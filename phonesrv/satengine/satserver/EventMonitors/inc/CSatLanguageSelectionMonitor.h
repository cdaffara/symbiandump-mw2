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
* Description:  Monitor of language selection
*
*/


#ifndef CSATLANGUAGESELECTIONMONITOR_H
#define CSATLANGUAGESELECTIONMONITOR_H

// INCLUDES
#include "MSatSystemStateObserver.h"
#include "CSatEventMonitorHandler.h"

// FORWARD DECLARATIONS
class MSatSystemStateChangeNotifier;

// CLASS DECLARATION

/**
*  Monitors language selection events.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class CSatLanguageSelectionMonitor : public CSatEventMonitorHandler,
                                     public MSatSystemStateObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Interface for Event monitor utils
        */
        static CSatLanguageSelectionMonitor* NewL(
            MSatEventMonitorUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatLanguageSelectionMonitor();

    public: // From base class

        /**
        * From CSatEventMonitorHandler Cancels event monitoring
        */
        void DoCancelMonitor();

        /**
        * From MSatSystemStateObserver Notifies that system state changed.
        * @param aValue ID of the selected language.
        */
        void StateChanged( const TInt aValue );

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
        CSatLanguageSelectionMonitor( MSatEventMonitorUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // CentRep change notifier
        MSatSystemStateChangeNotifier* iPSNotifier;

    };

#endif      // CSATLANGUAGESELECTIONMONITOR_H

// End of File
