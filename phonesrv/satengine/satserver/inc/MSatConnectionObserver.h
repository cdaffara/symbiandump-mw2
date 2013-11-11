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
* Description:  Interface for Connection observers.
*
*/



#ifndef MSATCONNECTIONOBSERVER_H
#define MSATCONNECTIONOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for Connection observers. These observers are notified, when
*  connection is fully up, or an error occurs during connection stage.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatConnectionObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatConnectionObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatConnectionObserver() {};

    public: // New functions

        /**
        * Notification of connection stage
        * @param aError Indicates the status of connection.
        */
        virtual void ConnectionNotification( const TInt aError ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatConnectionObserver( const MSatConnectionObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatConnectionObserver& operator=( const MSatConnectionObserver& );

    };

#endif      // MSATSENDDATAOBSERVER_H

// End of File
