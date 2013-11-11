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



#ifndef __CSuplServerDelayedShutDown_H__
#define __CSuplServerDelayedShutDown_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSuplServer;
// CLASS DECLARATION

/**
 *  Class to handle delayed server shutdown
 */
class CSuplServerDelayedShutdown : public CTimer
    {
    private:    // Constructor
        /**
        * C++ Constructor
        */
        CSuplServerDelayedShutdown(CSuplServer& aServer);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~CSuplServerDelayedShutdown();

        /**
        * Two-phase constructor
        */
        static CSuplServerDelayedShutdown* NewL(
            /* IN  */   CSuplServer& aServer
            );

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
        CSuplServerDelayedShutdown( const CSuplServerDelayedShutdown& );
        // Prohibit assigment operator
        CSuplServerDelayedShutdown& operator= ( const CSuplServerDelayedShutdown& );

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Called when timer completes
        */
        void RunL();

    private:    // Data
       CSuplServer&     iSuplServer; 
    };

#endif // __CSuplServerDelayedShutDown_H__

// End of File
