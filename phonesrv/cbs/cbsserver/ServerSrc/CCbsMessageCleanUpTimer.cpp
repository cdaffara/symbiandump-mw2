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
* Description:  This module contains the implementation of CCbsMessageCleanupTimer class 
*                member functions.
*
*/



// INCLUDE FILES
#include "CCbsMessageCleanUpTimer.h"
#include "CCbsRecCollector.h"
#include "CbsLogger.h"

// CONSTANTS

// Message cleanup timer in minutes. This is only for livecast messages.
const TInt KMessageCleanUpTime = 10;

// ==================== LOCAL FUNCTIONS =====================================

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::CCbsMessageCleanupTimer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMessageCleanupTimer::CCbsMessageCleanupTimer( CCbsRecCollector& aCbsRecCollector, CMessageBuffer& aArray )
		: CTimer( EPriorityStandard ),
		iCbsRecCollector ( aCbsRecCollector ),
		iArray ( aArray )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsMessageCleanupTimer::ConstructL()
    {
    // Construct the timer
    CTimer::ConstructL();       

	iInterval = KMessageCleanUpTime;
    }

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsMessageCleanupTimer* CCbsMessageCleanupTimer::NewL( CCbsRecCollector& aCbsRecCollector, CMessageBuffer& aArray )
    {
    CCbsMessageCleanupTimer* self = 
        new ( ELeave ) CCbsMessageCleanupTimer( aCbsRecCollector, aArray );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// Destructor
CCbsMessageCleanupTimer::~CCbsMessageCleanupTimer()
    {   
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessageCleanupTimer::~CCbsMessageCleanupTimer()"); 
    Cancel();
    CBSLOGSTRING("CBSSERVER: <<< CCbsMessageCleanupTimer::~CCbsMessageCleanupTimer()");
    }

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::StartTimer
// Starts the clean up timer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessageCleanupTimer::StartTimer()
    {    
    // Cancel a pending request
    if ( IsActive() )
        {
        Cancel();
        }

    // Get the current time
    TTime now;
    now.HomeTime();

    if ( iInterval.Int() > 0 )
        {
        TTime nextCleanup( 0 );
        nextCleanup = now + iInterval;
        
        // Start the timer
        At( nextCleanup );
        }
    }

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::StopTimer
// Stops (cancels) the clean up timer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessageCleanupTimer::StopTimer()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCbsMessageCleanupTimer::RunL
// Called after the timer expires. Makes the request to delete the read
// messages and to set the last clean up time.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessageCleanupTimer::RunL()	
    {
    if ( iStatus == KErrNone )
        {
		iCbsRecCollector.DeleteChainL ( iArray );
		StopTimer();
        }
	}

// ================= OTHER EXPORTED FUNCTIONS ===============================



//  End of File 
