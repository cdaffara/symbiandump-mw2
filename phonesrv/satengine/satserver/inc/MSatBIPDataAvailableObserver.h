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
* Description:  Interface for DataAvailable event observer.
*
*/



#ifndef MSATBIPDATAAVAILABLEOBSERVER_H
#define MSATBIPDATAAVAILABLEOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for DataAvailable event observer. When DataAvailable event occurs, the
*  implementors of this class will be notified. However, this class must be send to
*  MSatBIPEventNotifier to get the notification.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatBIPDataAvailableObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatBIPDataAvailableObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatBIPDataAvailableObserver() {};

    public: // New functions

        /**
        * Notification of an event.
        * @param aChannelId ID of the channel where the data is available.
        * @param aLength Number of bytes available
        */
        virtual void DataAvailable(
            const TInt aChannelId,
            const TInt aLength ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatBIPDataAvailableObserver(
            const MSatBIPDataAvailableObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatBIPDataAvailableObserver& operator=(
            const MSatBIPDataAvailableObserver& );

    };

#endif      // MSATBIPDATAAVAILABLEOBSERVER_H

// End of File
