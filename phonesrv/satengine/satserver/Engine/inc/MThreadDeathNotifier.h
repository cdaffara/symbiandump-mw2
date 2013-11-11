/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ThreadDeath notifier for CSatSession
*
*/



#ifndef MTHREADDEATHNOTIFIER_H
#define MTHREADDEATHNOTIFIER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Notifies CSatThreadDeathMonitor to start monitor new thread death
*
*  @lib SatServer
*  @since Series 60 3.0
*/
class MThreadDeathNotifier
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MThreadDeathNotifier() {};

        /**
        * Destructor.
        */
        virtual ~MThreadDeathNotifier() {};

    public: // New functions

        /**
        * Notifies Thread death monitor that thread is closing
        */
        virtual void NotifyThreadDeath() = 0;

        /**
        * Start monitoring thread's death
        */
        virtual void StartObservingThread() = 0;

        /**
        * Sets thread that is observered.
        * @param aThreadId Thread which death should be observered
        */
        virtual void SetObserveredThread( TThreadId& aThread ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MThreadDeathNotifier( const MThreadDeathNotifier& );

        // Prohibit assigment operator if not deriving from CBase.
        MThreadDeathNotifier& operator=( const MThreadDeathNotifier& );

    };

#endif      // MTHREADDEATHNOTIFIER_H

// End of File
