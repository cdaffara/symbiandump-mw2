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
* Description:  Notifier of the BIP Events.
*
*/



#ifndef MSATBIPEVENTNOTIFIER_H
#define MSATBIPEVENTNOTIFIER_H

//  INCLUDES
#include <e32std.h>

class MSatBIPChannelStatusObserver;
class MSatBIPDataAvailableObserver;

// CLASS DECLARATION

/**
*  BIP Event notifier interface. Responsible to notify BIP Event observers
*  about BIP Events.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatBIPEventNotifier
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatBIPEventNotifier() {};

        /**
        * Destructor.
        */
        virtual ~MSatBIPEventNotifier() {};

    public: // New functions

        /**
        * Notification request for DataAvailable event.
        * @param aObserver An observer of the event.
        */
        virtual void NotifyDataAvailable(
            MSatBIPDataAvailableObserver* aObserver ) = 0;

        /**
        * Notification request for ChannelStatus event.
        * @param aObserver An observer of the event.
        */
        virtual void NotifyChannelStatus(
            MSatBIPChannelStatusObserver* aObserver ) = 0;

        /**
        * Cancels DataAvailable event notification request.
        */
        virtual void CancelDataAvailable() = 0;

        /**
        * Cancels ChannelStatus event notification request.
        */
        virtual void CancelChannelStatus() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatBIPEventNotifier( const MSatBIPEventNotifier& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatBIPEventNotifier& operator=( const MSatBIPEventNotifier& );

    };

#endif      // MSATBIPEVENTNOTIFIER_H

// End of File
