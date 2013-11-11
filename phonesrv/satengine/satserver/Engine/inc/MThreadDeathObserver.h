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
* Description:  Observer for CSatSThreadDeathMonitor
*
*/



#ifndef MTHREADDEATHOBSERVER_H
#define MTHREADDEATHOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Observer for CSatSThreadDeathObserver
*
*  @lib SatServer
*  @since Series 60 2.6
*/
class MThreadDeathObserver
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MThreadDeathObserver() {};

        /**
        * Destructor.
        */
        virtual ~MThreadDeathObserver() {};

    public: // New functions

        /**
        * Notification of observed thread death.
        */
        virtual void ThreadDiedL() = 0;

        /**
        * Notification of observed thread panic.
        */
        virtual void ThreadPanicedL() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MThreadDeathObserver( const MThreadDeathObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MThreadDeathObserver& operator=( const MThreadDeathObserver& );

    };

#endif      // MTHREADDEATHOBSERVER_H

// End of File
