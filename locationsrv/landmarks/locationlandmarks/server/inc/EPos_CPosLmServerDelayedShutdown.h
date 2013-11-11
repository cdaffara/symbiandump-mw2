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
* Description: 
 *     Class to handle server delayed shutdown
*
*/


#ifndef CPOSLMSERVERDELAYEDSHUTDOWN_H
#define CPOSLMSERVERDELAYEDSHUTDOWN_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
 *  Class to handle delayed server shutdown
 */
class CPosLmServerDelayedShutdown : public CActive
    {
    public:    // Constructors and destructor

        /**
        * Two-phase constructor
        */
        static CPosLmServerDelayedShutdown* NewL();

        /**
        * C++ Destructor
        */
        ~CPosLmServerDelayedShutdown();

    public:     // New functions

        /**
        * Start shutdown countdown.
        * @param aDelay Delay time for shutdown.
        */
        void Start(
        /* IN */        TTimeIntervalMicroSeconds32 aDelay
        );

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Active Object event handler.
        */
        void RunL();

        /**
        * From CActive.
        * Active Object cancel handler.
        */
        void DoCancel();

    private:

        /**
        * C++ Constructor
        */
        CPosLmServerDelayedShutdown();

        /**
        *  By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPosLmServerDelayedShutdown(
            const CPosLmServerDelayedShutdown& );
        // Prohibit assigment operator
        CPosLmServerDelayedShutdown& operator= (
            const CPosLmServerDelayedShutdown& );

    private:    // Data

        RTimer iTimer;

    };

#endif // CPOSLMSERVERDELAYEDSHUTDOWN_H

// End of File
