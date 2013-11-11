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
* Description:  Interface for ChannelStatus event observer.
*
*/



#ifndef MSATBIPCHANNELSTATUSOBSERVER_H
#define MSATBIPCHANNELSTATUSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for ChannelStatus event observer. When ChannelStatus event occurs, the
*  implementors of this class will be notified. However, this class must be send to
*  MSatBIPEventNotifier to get the notification.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatBIPChannelStatusObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatBIPChannelStatusObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatBIPChannelStatusObserver() {};

    public: // New functions

        /**
        * Notification of an event.
        * @param aChannelId ID of the channel that started this event.
        * @param aStatus Status that caused this event.
        */
        virtual void ChannelStatus(
            const TInt aChannelId,
            const TInt aStatus ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatBIPChannelStatusObserver(
            const MSatBIPChannelStatusObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatBIPChannelStatusObserver&
            operator=( const MSatBIPChannelStatusObserver& );

    };

#endif      // MSATBIPCHANNELSTATUSOBSERVER_H

// End of File
