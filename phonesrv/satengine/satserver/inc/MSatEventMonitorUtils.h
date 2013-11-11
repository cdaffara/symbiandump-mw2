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
* Description:  Interface for event monitor utils
*
*/



#ifndef MSATEVENTMONITORUTILS_H
#define MSATEVENTMONITORUTILS_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// FORWARD DECLARATION
class MSatBIPEventNotifier;

// CLASS DECLARATION

/**
*  Interface for event monitors.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class MSatEventMonitorUtils
    {
    public: // New functions

        /**
        * Sends notify to SIM about some event using EventDownload command.
        * @param aSingleEvent Single event reported to SIM.
        * @param aFirstArg First argument interpreted according to type of event
        * @param aSecArg Second argument interpreted according to type of event
        */
        virtual void EventDownload(
            RSat::TEventList aSingleEvent,
            TInt aFirstArg,
            TInt aSecArg ) = 0;

        /**
        * Gives a reference to BIP Event notifier interface.
        * @return A reference to BIP Event notifier interface.
        */
        virtual MSatBIPEventNotifier& BipNotifier() = 0;

        /**
        * Tells to observer that SIM is removed. Observer should cancel all
        * monitors.
        */
        virtual void SimRemoved() = 0;

        /**
        * From MSatEventMonitorUtils
        * Writes time between language selection and reboot.
        * @param aTime Time before reboot.
        * @return KErrNone if Language Selection Time is set.
        */
        virtual TInt WriteLanguageSelectionTime( TInt aTime ) = 0;

    protected:  // New functions

        /**
        * Destructor.
        */
        virtual ~MSatEventMonitorUtils() {};

        /**
        * C++ default constructor.
        */
        MSatEventMonitorUtils() {};

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatEventMonitorUtils( const MSatEventMonitorUtils& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatEventMonitorUtils& operator=( const MSatEventMonitorUtils& );

    };

#endif      // MSATEVENTMONITORUTILS_H

// End of file
