/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   General purpose class for timeout notification.
*
*/



#include "epos_comasupltimeouttimer.h"
#include "epos_momasupltimeoutnotifier.h"

#if defined(_DEBUG)
	_LIT( KPanicMsg, "COMASuplTimeoutTimer");
#endif	
    
// COMASuplTimeoutTimer::NewL
// Creates instance of COMASuplTimeoutTimer

COMASuplTimeoutTimer* COMASuplTimeoutTimer::NewL(MOMASuplTimeOutNotifier& aTimeOutNotify)
    {
    COMASuplTimeoutTimer* self = COMASuplTimeoutTimer::NewLC(aTimeOutNotify);
    CleanupStack::Pop(self);
    return self;
    }

// COMASuplTimeoutTimer::NewLC
// Creates instance of COMASuplTimeoutTimer & pushes into cleanup stack    
COMASuplTimeoutTimer* COMASuplTimeoutTimer::NewLC(MOMASuplTimeOutNotifier& aTimeOutNotify)
    {
    COMASuplTimeoutTimer* self = new (ELeave) COMASuplTimeoutTimer(aTimeOutNotify);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// COMASuplTimeoutTimer::COMASuplTimeoutTimer
// Constructor
COMASuplTimeoutTimer::COMASuplTimeoutTimer(MOMASuplTimeOutNotifier& aTimeOutNotify)
: CTimer(CActive::EPriorityStandard), iNotify(aTimeOutNotify),iTimerExpired(EFalse)
    {
    }

// COMASuplTimeoutTimer::~COMASuplTimeoutTimer()
// Destuctor
COMASuplTimeoutTimer::~COMASuplTimeoutTimer()
    {
    }

// COMASuplTimeoutTimer::ConstructL()
// 2nd phase constuction
void COMASuplTimeoutTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

// COMASuplTimeoutTimer::StartTimer()
// Starts timer
void COMASuplTimeoutTimer::StartTimer( const TInt aTimeOut)
    {
	    if (!IsActive())
	    	{
	    		iTimerExpired = EFalse;
	    		After(aTimeOut);
	    	}
    }

// COMASuplTimeoutTimer::StopTimer()
// Stops timer
void COMASuplTimeoutTimer::StopTimer()
    {
    		iTimerExpired = EFalse;
			Cancel();
    }

// COMASuplTimeoutTimer::RunL()
// Gets called when time out occurs
void COMASuplTimeoutTimer::RunL()
    {
    // Timer request has completed, so notify the timer's owner
    if (iStatus == KErrNone)
        {
        	iTimerExpired = ETrue;
        	iNotify.TimerExpiredL();
        }
    else
        {
        #if defined(_DEBUG)
        	User::Panic(KPanicMsg, ESuplTimeoutTimerFailed);
        #endif	
        }
    }

TBool COMASuplTimeoutTimer::IsTimerExpired()
	{
		return iTimerExpired;
	}
