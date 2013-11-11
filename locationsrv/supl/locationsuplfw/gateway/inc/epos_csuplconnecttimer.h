/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class to handle server delayed shutdown
*
*/



#ifndef __CSuplConnecttimer_H
#define __CSuplConnecttimer_H

// INCLUDES
#include <e32base.h>

#include "epos_mconnecttimerobserver.h"

// CLASS DECLARATION

/**
 *  Class to handle delayed server shutdown
 */
class CSuplConnectTimer : public CTimer
    {
    private:    // Constructor
        /**
        * C++ Constructor
        */
        CSuplConnectTimer(MConnectTimerObserver *aObserver);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~CSuplConnectTimer();

        /**
        * Two-phase constructor
        */
        static CSuplConnectTimer* NewL(MConnectTimerObserver *aObserver);

    public:     // New functions

		/**
        * Start shutdown countdown.
        */
        void Start(TTimeIntervalMicroSeconds32 aDelay);

    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplConnectTimer( const CSuplConnectTimer& );
        // Prohibit assigment operator
        CSuplConnectTimer& operator= ( const CSuplConnectTimer& );

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Called when timer completes
        */
        void RunL();

    private:    // Data
        MConnectTimerObserver	*iObserver;
    };

#endif // __CSuplConnecttimer_H

// End of File
