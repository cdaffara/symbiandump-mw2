/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A timer based class used to notify objects when an automatic
*                response is expected.
*
*/


#ifndef CPHSRVUSSDREPLYTIMER_H
#define CPHSRVUSSDREPLYTIMER_H


// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class MPhSrvUssdReplyTimerObserver;


// CLASS DECLARATION

/**
*  A timer based class used to notify objects when an 
*  automatic response is expected
*
*  @since 1.0
*/
class CPhSrvUssdReplyTimer : public CTimer
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aObserver Observer for reply timer expiration event.
        */
        static CPhSrvUssdReplyTimer* 
            NewL( MPhSrvUssdReplyTimerObserver& aObserver );

        /**
        * Destructor.
        */
        ~CPhSrvUssdReplyTimer();
        

    public: // New functions

        /**
        * Query is the timer active or not.
        * 
        * @return True if timer is running/paused.
        */
        TBool IsTimerActive() const;

        /**
        * Start the timer.
        */
        void Start();

        /**
        * Pause the timer.
        */
        void Pause();

        /**
        * Continue paused timer.
        */
        void Continue();

        /**
        * Stop the timer.
        */
        void Stop();


    private: // Functions from base classes

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );


    private:

        /**
        * C++ constructor.
        */
        CPhSrvUssdReplyTimer( MPhSrvUssdReplyTimerObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:     // Data

        // The actual timer.
        RTimer iTimer;

        // The counter for remaining time.
        TTimeIntervalMicroSeconds32 iTimeLeft;

        // When the timer was started last time.
        TTime iLastStartTime;

        // Observer for timer events.
        MPhSrvUssdReplyTimerObserver& iObserver;
    };

#endif // CPHSRVUSSDREPLYTIMER_H


// End of File
