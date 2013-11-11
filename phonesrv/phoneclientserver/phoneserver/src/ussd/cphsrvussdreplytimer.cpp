/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  USSD Reply Timer.
*
*/



// INCLUDE FILES
#include "cphsrvussdreplytimer.h" 
#include "mphsrvussdreplytimerobserver.h" 


// CONSTANTS
const TUint KPhSrvUssdReplyTimerInterval = 300000000; // 5 min


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::CPhSrvUssdReplyTimer
//
// Constructor
//
// -----------------------------------------------------------------------------
//
CPhSrvUssdReplyTimer::CPhSrvUssdReplyTimer( 
    MPhSrvUssdReplyTimerObserver& aObserver )
:   CTimer( CActive::EPriorityStandard ), 
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvUssdReplyTimer* CPhSrvUssdReplyTimer::NewL( 
    MPhSrvUssdReplyTimerObserver& aObserver )
    {
    CPhSrvUssdReplyTimer* self = 
        new( ELeave ) CPhSrvUssdReplyTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::~CPhSrvUssdReplyTimer
//
// Destructor
//
// -----------------------------------------------------------------------------
//
CPhSrvUssdReplyTimer::~CPhSrvUssdReplyTimer()
    {
    Cancel();
    iTimer.Close();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::ConstructL
//
// Symbian OS second phase constructor
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::ConstructL()
    {
    CTimer::ConstructL();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::IsTimerActive
// -----------------------------------------------------------------------------
//
TBool CPhSrvUssdReplyTimer::IsTimerActive() const
    {
    return iTimeLeft.Int() != 0;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::Start
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::Start()
    {
    Cancel();
    iTimeLeft = KPhSrvUssdReplyTimerInterval;
    iLastStartTime.UniversalTime();
    After( iTimeLeft );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::Pause
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::Pause()
    {
    if ( !IsActive() )
        {
        return;
        }
    Cancel();
    iTimeLeft = KPhSrvUssdReplyTimerInterval;           
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::Continue
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::Continue()
    {
    if ( IsActive() )
        {
        return;
        }
    
    if ( iTimeLeft.Int() <= 0 )
        {
        iTimeLeft = 0;
        TRAP_IGNORE(  iObserver.UssdReplyTimerObserverHandleExpiredL( KErrNone ) );
        return;
        }
    
    iLastStartTime.UniversalTime();
    After( iTimeLeft );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::Stop
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::Stop()
    {
    Cancel();
    iTimeLeft = 0;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::RunL
//
// Called when the timer expires. Notify observer
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReplyTimer::RunL()
    {
    // Called only when status is KErrNone, since when the system
    // time is changed, timers may be completed with KErrAbort.
    iTimeLeft = 0;
    if ( iStatus.Int() == KErrNone )
        {
        iObserver.UssdReplyTimerObserverHandleExpiredL( KErrNone );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReplyTimer::RunError
//
// Called when RunL leaves. 
// The RunL can leave in observer side, then it is ok to just return KErrNone.
//
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdReplyTimer::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


//  End of File
