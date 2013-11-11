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
* Description:   Class to handle socket connection timeout 
*
*/



#ifndef __CSuplSockettimer_H
#define __CSuplSockettimer_H

// INCLUDES
#include <e32base.h>

class CSuplConnTrace;
class CSuplConnection;

// CLASS DECLARATION

/**
 *  Class to handle socket connection timeout 
 */
NONSHARABLE_CLASS(CSuplSocketTimer) : public CTimer
    {
    private:    // Constructor
        /**
        * C++ Constructor
        */
        CSuplSocketTimer(CSuplConnection* aConnection);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~CSuplSocketTimer();

        /**
        * Two-phase constructor
        */
        static CSuplSocketTimer* NewL(CSuplConnection* aConnection);

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
        CSuplSocketTimer( const CSuplSocketTimer& );
        // Prohibit assigment operator
        CSuplSocketTimer& operator= ( const CSuplSocketTimer& );

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Called when timer completes
        */
        void RunL();

    private:    // Data
    	 // Trace Log
        CSuplConnTrace* iTrace;

        // Reference to connection object
        CSuplConnection* iConnection;

    };

#endif // __CSuplSockettimer_H

// End of File
