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
* Description:  Observer for the sms event
*
*/



#ifndef MSATSMSOBSERVER_H
#define MSATSMSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Observer for send sms event.
*
*  @lib SendSmCmd.lib
*  @since Series 60 3.0
*/
class MSatSmsObserver
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatSmsObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatSmsObserver() {};

    public: // New functions

        /**
        * Notification of sms sent event.
        * @param aErrorCode Error code.
        */
        virtual void SmsSent( TInt aErrorCode ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSmsObserver( const MSatSmsObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSmsObserver& operator=( const MSatSmsObserver& );

    };

#endif      // MSATSMSOBSERVER_H

// End of File
