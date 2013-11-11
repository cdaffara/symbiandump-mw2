/*
* ==============================================================================
*  Name        : epos_csuplsessionretryq.h
*  Part of     : SUPL Framework / SUPL Server
*  Interface   : --
*  Description : Class to handle retries of failed requests. It waits for all the current sessions to end and then
*  				 notifies the failed sessions that they can retry.
*  Version     :
*
*  Copyright (c) 2010 Nokia Corporation.
*  This material, including documentation and any related
*  computer programs, is protected by copyright controlled by
*  Nokia Corporation. All rights are reserved. Copying,
*  including reproducing, storing, adapting or translating, any
*  or all of this material requires the prior written consent of
*  Nokia Corporation. This material also contains confidential
*  information which may not be disclosed to others without the
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/

#ifndef EPOS_CSUPLSESSIONRETRYQ_H
#define EPOS_CSUPLSESSIONRETRYQ_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION
class CSuplSessionRequest;

/**
 * Class to handle retries of failed requests. It waits for all the current sessions to end and then
 *		 notifies the failed sessions that they can retry.
 */
class CSuplSessionRetryQ : public CTimer
    {
    public:
        static CSuplSessionRetryQ* NewL();
		~CSuplSessionRetryQ();

        void AddToQueueL(CSuplSessionRequest& aSessionRequest);
        void RemoveFromQueueL(CSuplSessionRequest& aSessionRequest);
		void SessionStarted();
		void SessionEnded();

    private:
		CSuplSessionRetryQ();
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplSessionRetryQ( const CSuplSessionRetryQ& );
        // Prohibit assigment operator
        CSuplSessionRetryQ& operator= ( const CSuplSessionRetryQ& );

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Called when timer completes
        */
        void RunL();

    private:    // Data
		RPointerArray<CSuplSessionRequest> iSessionRequests;
		TInt iSessionCount;
		TBool iSomeSessionEnded;
		TTime iTimeOfLastEndedSession;
    };

#endif // EPOS_CSUPLSESSIONRETRYQ_H

// End of File
