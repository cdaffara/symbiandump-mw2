/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsMessageCleanupTimer class.
*
*/


#ifndef CCBSMESSAGECLEANUPTIMER_H
#define CCBSMESSAGECLEANUPTIMER_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CCbsDbImpTopicList;
class CCbsDbImpSettings;
class CCbsMessage;
class CCbsRecCollector;

typedef CArrayPtrFlat<CCbsMessage> CMessageBuffer;


// CLASS DECLARATION

// DESCRIPTION
// Handles the cleaning up of read messages, according to time interval
// set by the user.

class CCbsMessageCleanupTimer : public CTimer
    {
    public:  // Constructors and destructor
        
        /**
        *   C++ default constructor.
        *
        *   @param  aTopicList  Reference to topic list database
        *   @param  aSettings   Reference to settings database
        */
        CCbsMessageCleanupTimer( CCbsRecCollector& aCbsRecCollector, CMessageBuffer& aArray );

        /**
        *   Two-phased constructor.
        *
        *   @param  aTopicList  Reference to topic list database
        *   @param  aSettings   Reference to settings database
        */
        static CCbsMessageCleanupTimer* NewL( CCbsRecCollector& aCbsRecCollector, CMessageBuffer& aArray );
        
        /**
        *   Destructor.
        */
        ~CCbsMessageCleanupTimer();

    public: // New functions
        
        /** 
        *   Requests the timer to be started.
        */
        void StartTimer();

        /** 
        *   Requests the timer to be stopped.
        */
        void StopTimer();

    public: // Functions from base classes

        /**
        *   Called when the timer expires. Renews the request.
        */
        void RunL();
        
    private:

        /**
        *   By default constructor is private.
        */
        void ConstructL();

    private:    // Data

		TTimeIntervalMinutes	iInterval;
		CCbsRecCollector&		iCbsRecCollector;
		CMessageBuffer&			iArray;
    };

#endif      // CCbsMessageCleanupTimer_H
            
// End of File
