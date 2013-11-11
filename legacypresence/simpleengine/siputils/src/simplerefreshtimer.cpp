/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Refresh timer
*
*/






// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include "simplerefreshtimer.h"
#include "simplesipconncallback.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// CONSTANTS

const TInt KUseAfterLimit = 240;

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Refresh timer
// ---------------------------------------------------------------------------
//

// ======== MEMBER FUNCTIONS ========

CSimpleRefreshTimer::CSimpleRefreshTimer(
    MSimpleSipConnCallback& aEngine, CSimpleRequest& aReq  )
    : CActive( CActive::EPriorityStandard),
      iEngine(aEngine), iRequest( aReq )

    {
    // Add this to the scheduler
    (void) iTimer.CreateLocal();
    CActiveScheduler::Add(this);
    }

CSimpleRefreshTimer::~CSimpleRefreshTimer()
    {
    Cancel();
    iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CSimpleRefreshTimer::Start
// -----------------------------------------------------------------------------
void CSimpleRefreshTimer::Start( TInt aWaitSeconds )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("RefreshTimer: Start %d sec" ), aWaitSeconds );
#endif

    // Cancel is needed because of the timer may be reset.
    Cancel();

    if ( aWaitSeconds <= 0)
        {
        return;
        }

    iSeconds = aWaitSeconds;
    
    // The At function caused a CUserbase-Panic 46 in very small
    // time values. 1-4 seconds. Now if the KeepAlive time
    // is smaller than UseAfterLimit, then we use the After function
    // If it is larger then use the At function
    // The reason not to use the After function for every situation is
    // that the TInt overflows after 35 minutes. 1000000*60*36 > 2^31
    if( iSeconds <= KUseAfterLimit )
        {
        iTimer.After( iStatus, iSeconds * 1000000 );
        }
    else
        {
        TTime myKeepAlive;
        myKeepAlive.HomeTime();
        myKeepAlive += TTimeIntervalSeconds( iSeconds );
        iTimer.At( iStatus, myKeepAlive );
        }
    iStatus = KRequestPending;
    SetActive();

    }

// -----------------------------------------------------------------------------
// CSimpleRefreshTimer::DoCancel
// -----------------------------------------------------------------------------
void CSimpleRefreshTimer::DoCancel( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("RefreshTimer: DoCancel" ));
#endif
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CSimpleRefreshTimer::RunL
// -----------------------------------------------------------------------------
void CSimpleRefreshTimer::RunL(  )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("RefreshTimer: RunL" ));
#endif
    // Inside StartToRefreshL this entity may be deleted,
    // thus after the method call do not do anything in RunL.
    iEngine. StartToRefreshL( iRequest );
    }

// -----------------------------------------------------------------------------
// CSimpleRefreshTimer::RunError
// -----------------------------------------------------------------------------
TInt CSimpleRefreshTimer::RunError(TInt /*aError*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("RefreshTimer: RunError" ));
#endif
    return KErrNone;
    }
    
    
// ---------------------------------------------------------------------------
// Expiry timer
// ---------------------------------------------------------------------------
//

// ======== MEMBER FUNCTIONS ========

CSimpleExpiryTimer::CSimpleExpiryTimer(
    MSimpleSipConnCallback& aEngine, CSimpleRequest& aReq  )
    : CActive( CActive::EPriorityStandard),
      iEngine(aEngine), iRequest( aReq )

    {
    // Add this to the scheduler
    (void) iTimer.CreateLocal();
    CActiveScheduler::Add(this);
    }

CSimpleExpiryTimer::~CSimpleExpiryTimer()
    {
    Cancel();
    iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CSimpleExpiryTimer::Start
// -----------------------------------------------------------------------------
void CSimpleExpiryTimer::Start( TInt aWaitSeconds )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("ExpiryTimer: Start %d sec" ), aWaitSeconds );
#endif

    // Cancel is needed because of the timer may be reset.
    Cancel();

    if ( aWaitSeconds <= 0 )
        {
        return;
        }

    iSeconds = aWaitSeconds;

    // The At function caused a CUserbase-Panic 46 in very small
    // time values. 1-4 seconds. Now if the KeepAlive time
    // is smaller than UseAfterLimit, then we use the After function
    // If it is larger then use the At function
    // The reason not to use the After function for every situation is
    // that the TInt overflows after 35 minutes. 1000000*60*36 > 2^31
    if( iSeconds <= KUseAfterLimit )
        {
        iTimer.After( iStatus, iSeconds * 1000000 );
        }
    else
        {
        TTime myKeepAlive;
        myKeepAlive.HomeTime();
        myKeepAlive += TTimeIntervalSeconds( iSeconds );
        iTimer.At( iStatus, myKeepAlive );
        }
    iStatus = KRequestPending;
    SetActive();

    }

// -----------------------------------------------------------------------------
// CSimpleExpiryTimer::DoCancel
// -----------------------------------------------------------------------------
void CSimpleExpiryTimer::DoCancel( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("ExpiryTimer: DoCancel" ));
#endif
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CSimpleExpiryTimer::RunL
// -----------------------------------------------------------------------------
void CSimpleExpiryTimer::RunL(  )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("ExpiryTimer: RunL" ));
#endif
    iEngine.StartToCheckExpiryL( iRequest );
    // Inside StartToCheckExpiryL this entity may be deleted,
    // thus after the method call do not do anything in RunL.    
    }

// -----------------------------------------------------------------------------
// CSimpleExpiryTimer::RunError
// -----------------------------------------------------------------------------
TInt CSimpleExpiryTimer::RunError(TInt /*aError*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("ExpiryTimer: RunError" ));
#endif
    return KErrNone;
    }
    

